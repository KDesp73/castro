#include "castro.h"
#include <ctype.h>
#define LOGGING_IMPLEMENTATION
#include "IncludeOnly/logging.h"
#define ANSI_IMPLEMENTATION
#include "IncludeOnly/ansi.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define IS_EMPTY(x) (x[0] == '\0')

void PgnExport(Game* game, char* pgn)
{
    sprintf(pgn, "[Event \"%s\"]\n", IS_EMPTY(game->event) ? "??" : game->event);
    sprintf(pgn + strlen(pgn), "[Site \"%s\"]\n", IS_EMPTY(game->site) ? "??" : game->site);
    sprintf(pgn + strlen(pgn), "[Date \"%s\"]\n", IS_EMPTY(game->date) ? "????.??.??" : game->date);

    if(!IS_EMPTY(game->fen) && strcmp(game->fen, STARTING_FEN))
        sprintf(pgn + strlen(pgn), "[FEN \"%s\"]\n", game->fen);

    sprintf(pgn + strlen(pgn), "[White \"%s\"]\n", IS_EMPTY(game->white) ? "Player 1" : game->white);
    sprintf(pgn + strlen(pgn), "[Black \"%s\"]\n", IS_EMPTY(game->black) ? "Player 2" : game->black);

    sprintf(pgn + strlen(pgn), "[Result \"%s\"]\n", IS_EMPTY(game->result) ? "*" : game->result);

    sprintf(pgn + strlen(pgn), "\n");

    for (int i = 0; i < game->move_count; i++) {
        if (i % 2 == 0) {
            sprintf(pgn + strlen(pgn), "%d. %s ", (i / 2) + 1, game->moves[i].move);
        } else {
            sprintf(pgn + strlen(pgn), "%s ", game->moves[i].move);
        }
    }
    sprintf(pgn + strlen(pgn), "%s", IS_EMPTY(game->result) ? "*" : game->result);
    sprintf(pgn + strlen(pgn), "\n");
}

// FIX: has issues with moves
void PgnImport(Game* game, const char* pgn)
{
    int move_count = 0;

    memset(game, 0, sizeof(Game));

    const char* pgn_ptr = pgn;

    while (sscanf(pgn_ptr, "[Event \"%[^\"]\"]", game->event) ||
           sscanf(pgn_ptr, "[Site \"%[^\"]\"]", game->site) ||
           sscanf(pgn_ptr, "[Date \"%[^\"]\"]", game->date) ||
           sscanf(pgn_ptr, "[White \"%[^\"]\"]", game->white) ||
           sscanf(pgn_ptr, "[Black \"%[^\"]\"]", game->black) ||
           sscanf(pgn_ptr, "[Result \"%[^\"]\"]", game->result)) {

        pgn_ptr = strchr(pgn_ptr, '\n');
        if (pgn_ptr) pgn_ptr++;  // Skip to the next line
        else break;              // No more headers
    }

    const char* moves_start = pgn_ptr;
    while (*moves_start && *moves_start != '1') {
        moves_start++; // Skip non-move data
    }

    char* token;
    char moves_copy[4096];
    strncpy(moves_copy, moves_start, sizeof(moves_copy) - 1);
    moves_copy[sizeof(moves_copy) - 1] = '\0';

    token = strtok(moves_copy, " ");
    while (token != NULL) {
        // Skip move numbers (e.g., "1.", "2.")
        if (strchr(token, '.')) {
            token = strtok(NULL, " ");
            continue;
        }

        if (move_count < MAX_MOVES) {
            strncpy(game->moves[move_count].move, token, sizeof(game->moves[move_count].move) - 1);
            game->moves[move_count].move[sizeof(game->moves[move_count].move) - 1] = '\0';
            move_count++;
        }

        token = strtok(NULL, " ");
    }

    game->move_count = move_count;
}

void PgnExportFile(Game* game, const char* path)
{
    char pgn[2048];
    PgnExport(game, pgn);

    FILE* file = fopen(path, "w"); 
    fprintf(file, "%s\n", pgn);
    fclose(file);
}

void removeChars(char* str, const char* chars_to_remove)
{
    size_t i = 0, j = 0;
    size_t len = strlen(str);

    while (i < len) {
        // Check if the current character should be removed
        if (strchr(chars_to_remove, str[i]) == NULL) {
            str[j++] = str[i];
        }
        i++;
    }

    // Null-terminate the string
    str[j] = '\0';
}

Move SanToMove(Board *board, SanMove san)
{
    Moves moves = GenerateMoves(board, MOVE_LEGAL);
    for(size_t i = 0; i < moves.count; i++){
        SanMove s = {0};
        Notate(board, moves.list[i], &s);
        if(!strcmp(san.move, s.move))
            return moves.list[i];
    }
    return NULL_MOVE;
}

void Notate(Board* board, Move move, SanMove* san)
{
    MOVE_DECODE(move);

    Piece piece = PieceAt(board, src);
    char target_square[3];
    SquareToName(target_square, dst);

    // Castling moves
    if (IsCastle(board, &move)) {
        if (File(dst) > File(src)) {
            strcpy(san->move, "O-O"); // Kingside castling
        } else {
            strcpy(san->move, "O-O-O"); // Queenside castling
        }
        return;
    }

    // Default: normal move
    char piece_letter = IS_PAWN(piece) ? '\0' : toupper(piece.type); // Pawn moves omit the piece letter

    // Check if the move is a capture
    _Bool is_capture = board->grid[COORDS(dst)] != ' ' || IsEnpassant(board, &move);

    // Build SAN string
    if (piece_letter != '\0') {
        san->move[0] = piece_letter;
        san->move[1] = '\0'; // Ensure proper termination
    } else {
        san->move[0] = '\0';
    }

    // Check if there is a need dst specify file, rank, or both
    if (IS_PAWN(piece)) {
        // If the move is a pawn capture
        if (is_capture) {
            // Specify the file dst disambiguate
            san->move[0] = 'a' + File(dst);
            san->move[1] = 'x';
            san->move[2] = '\0'; // Ensure proper termination
        }
    } else {
        size_t ambiguities_count = 0;
        Square ambiguities[16];

        for (size_t rank = 0; rank < BOARD_SIZE; rank++) {
            for (size_t file = 0; file < BOARD_SIZE; file++) {
                Square current = rank*8 + file;

                // Skip empty squares
                if (board->grid[rank][file] == ' ') continue;

                // Skip our piece
                if (Rank(src) == rank && File(src) == file) continue;

                if (
                    PieceCmp(piece, PieceAt(board, current)) &&
                    MoveIsValid(board, MoveEncode(current, dst, PROMOTION_NONE, FLAG_NORMAL), board->turn)
                ) {
                    ambiguities[ambiguities_count++] = current;
                }
            }
        }

        size_t ambiguity_count = 0;
        Square single_ambiguity = 64;
        for (size_t i = 0; i < ambiguities_count; i++) {
            if (
                src == ambiguities[i] ||
                IS_KNIGHT(piece)
            ) {
                single_ambiguity = ambiguities[i];
                ambiguity_count++;
            }
        }

        if (IS_KNIGHT(piece) && !ambiguity_count && ambiguities_count)
            ambiguity_count = ambiguities_count;

        char disambiguation[3] = "";
        char name[3] = "";
        switch (ambiguity_count) {
        case 1:
            if (Rank(src) == Rank(single_ambiguity)) {
                sprintf(disambiguation, "%c", 'a' + File(src));  // File disambiguation
            } else if (File(src) == File(single_ambiguity)) {
                sprintf(disambiguation, "%d", Rank(src));  // Rank disambiguation
            } else if (IS_KNIGHT(piece)) {
                sprintf(disambiguation, "%c", 'a' + File(src));
            }
            break;
        case 0:
            break;
        default:
            SquareToName(name, src);
            sprintf(disambiguation, "%s", name);
            break;
        }
        strcat(san->move, disambiguation); // Proper termination ensured by strcat

        if (is_capture) {
            strcat(san->move, "x"); // Proper termination ensured by strcat
        }
    }

    strcat(san->move, target_square); // Proper termination ensured by strcat

    // Promotion
    if (promotion != PROMOTION_NONE) {
        if (promotion) {
            char promo[3] = {'=', toupper(PromotionToChar(promotion)), '\0'};
            strcat(san->move, promo); // Proper termination ensured by strcat
        }
    }

    Board temp = *board;
    MakeMove(&temp, move);
    if (IsCheckmate(&temp)) {
        strcat(san->move, "#"); // Proper termination ensured by strcat
    } else if (IsInCheck(&temp)) {
        strcat(san->move, "+"); // Proper termination ensured by strcat
    }
    UnmakeMove(&temp);
}

void get_current_date(char* buffer, size_t buffer_size)
{
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);

    // Format the date as YYYY.MM.DD
    strftime(buffer, buffer_size, "%Y.%m.%d", tm_info);
}

void GameInit(Game* game, 
    const char* event,
    const char* site,
    const char* white,
    const char* black,
    const char* fen
)
{
    game->move_count = 0;

    if(!event) game->event[0] = '\0';
    else GameSetEvent(game, event);

    if(!site) game->site[0] = '\0';
    else GameSetSite(game, site);

    get_current_date(game->date, MAX_HEADER_LENGTH);

    if(!white) game->white[0] = '\0';
    else GameSetWhite(game, white);

    if(!black) game->black[0] = '\0';
    else GameSetBlack(game, black);

    if(!fen) game->fen[0] = '\0';
    else GameSetFen(game, fen);
}

void GameAddMove(Game* game, SanMove move)
{
    if(game->move_count >= MAX_MOVES){
        ERRO("Maximum number of moves reached");
        return;
    }
    game->moves[game->move_count++] = move;
}

void GameSetEvent(Game* game, const char* event)
{
    if(!event) return;
    strcpy(game->event, event);
}

void GameSetSite(Game* game, const char* site)
{
    if(!site) return;
    strcpy(game->site, site);
}
void GameSetDate(Game* game, const char* date)
{
    if(!date) return;
    strcpy(game->date, date);
}
void GameSetWhite(Game* game, const char* white)
{
    if(!white) return;
    strcpy(game->white, white);
}
void GameSetBlack(Game* game, const char* black)
{
    if(!black) return;
    strcpy(game->black, black);
}
void GameSetFen(Game* game, const char* fen)
{
    if(!fen) return;
    strcpy(game->fen, fen);
}
void GameSetResult(Game* game, const char* result)
{
    if(!result) return;
    strcpy(game->result, result);
}

void GamePrint(Game game)
{
    printf("[Event \"%s\"]\n", IS_EMPTY(game.event) ? "??" : game.event);
    printf("[Site \"%s\"]\n", IS_EMPTY(game.site) ? "??" : game.site);
    printf("[Date \"%s\"]\n", IS_EMPTY(game.date) ? "????.??.??" : game.date);

    if(!IS_EMPTY(game.fen) && strcmp(game.fen, STARTING_FEN))
        printf("[FEN \"%s\"]\n", game.fen);

    printf("[White \"%s\"]\n", IS_EMPTY(game.white) ? "Player 1" : game.white);
    printf("[Black \"%s\"]\n", IS_EMPTY(game.black) ? "Player 2" : game.black);

    printf("[Result \"%s\"]\n", IS_EMPTY(game.result) ? "*" : game.result);

    printf("\n");

    for (int i = 0; i < game.move_count; i++) {
        if (i % 2 == 0) {
            printf("%d. %s ", (i / 2) + 1, game.moves[i].move);
        } else {
            printf("%s ", game.moves[i].move);
        }
    }
    printf("%s", IS_EMPTY(game.result) ? "*" : game.result);
    printf("\n");
}

void press_enter_to_continue()
{
    printf("Press Enter to continue...\n");
    getchar();  // Waits for the user to press Enter
}

void GameRun(Game game)
{
    Board board;
    BoardInitFen(&board, IS_EMPTY(game.fen) ? NULL : game.fen);
    ansi_clear_screen();
    BoardPrint(&board, 64);
    press_enter_to_continue();
    int result = RESULT_NONE;

    for(size_t i = 0; i < game.move_count; i++) {
        for(size_t j = 0; j < RESULT_COUNT; j++){
            if(!strcmp(game.moves[i].move, result_score[j])) {
                result = j;
                goto end;
            }
        }

        Move move = SanToMove(&board, game.moves[i]);

        if(!MakeMove(&board, move)){
            // ERRO("Invalid pgn. Move %s is not valid", game.moves[i].move);
            goto end;
        }

        ansi_clear_screen();
        BoardPrint(&board, 64);
        press_enter_to_continue();
    }

end:
    if(result > 0) {
        printf("%s %s\n", result_message[result], result_score[result]);
    } else {
        printf("Game did not end\n");
    }
    BoardFree(&board);
}
