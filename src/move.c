#include "castro.h"

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>

bool castro_MoveCmpStrict(Move m1, Move m2)
{
    return castro_MoveCmp(m1, m2) && castro_GetFlag(m1) == castro_GetFlag(m2);
}


bool castro_MoveCmp(Move m1, Move m2)
{
    return (
        castro_GetFrom(m1) == castro_GetFrom(m2) &&
        castro_GetTo(m1) == castro_GetTo(m2) &&
        castro_GetPromotion(m1) == castro_GetPromotion(m2));
}

void castro_MoveSetFlag(Move* move, Flag flag)
{
    *move &= ~(0x7 << 16);
    *move |= ((flag & 0x7) << 16);
}
void castro_MoveSetPromotion(Move* move, Promotion promotion)
{
    *move &= ~(0xF << 12);
    *move |= ((promotion & 0xF) << 12); 
}

void castro_BoardPrintMove(const Board* board, Move move)
{
    MOVE_DECODE(move);
    castro_BoardPrint(board, src, dst, 64);
}

Undo castro_MakeUndo(const Board* board, Move move)
{
    char capture = castro_PieceAt(board, castro_GetTo(move)).type;
    return (Undo) {
        .move = move,
        .castling = board->castling_rights,
        .enpassant = board->enpassant_square,
        .fiftyMove=  board->halfmove,
        .captured = capture
    };
}

Moves castro_BitboardToMoves(Bitboard bitboard, Square from)
{
    Moves moves = {0};
    while (bitboard) {
        Square to = poplsb(&bitboard);
        moves.list[moves.count++] = castro_MoveEncode(from, to, PROMOTION_NONE, FLAG_NORMAL);
    }
    return moves;
}

Bitboard castro_MovesToBitboard(Moves moves)
{
    Bitboard bb = 0ULL;

    for(size_t i = 0; i < moves.count; i++){
        bb |= BB(castro_GetTo(moves.list[i]));
    }

    return bb;
}

Move castro_MoveEncodeNames(const char* from, const char* to, uint8_t promotion, uint8_t flag)
{
    return castro_MoveEncode(castro_NameToSquare(from), castro_NameToSquare(to), promotion, flag);
}

Move castro_MoveEncode(Square from, Square to, uint8_t promotion, uint8_t flag)
{
    return (from & 0x3F) | 
           ((to & 0x3F) << 6) | 
           ((promotion & 0xF) << 12) | 
           ((flag & 0x7) << 16);
}

void castro_MoveDecode(Move move, Square* from, Square* to, uint8_t* promotion, uint8_t* flag)
{
    *from = move & 0x3F;
    *to = (move >> 6) & 0x3F;
    *promotion = (move >> 12) & 0xF;
    *flag = (move >> 16) & 0x7;
}

Square castro_GetFrom(Move move)
{
    return move & 0x3F;
}
Square castro_GetTo(Move move)
{
    return (move >> 6) & 0x3F;
}
uint8_t castro_GetPromotion(Move move)
{
    return (move >> 12) & 0xF;
}
uint8_t castro_GetFlag(Move move)
{
    return (move >> 16) & 0x7;
}

void castro_MoveFreely(Board* board, Move move, PieceColor color) {
    // Decode the move
    Square from, to;
    uint8_t promotion, flags;
    castro_MoveDecode(move, &from, &to, &promotion, &flags);

    // Update the 2D character array for the board
    if (promotion == PROMOTION_NONE) {
        board->grid[COORDS(to)] = board->grid[COORDS(from)]; // Move the piece
    } else {
        char promotedPiece = castro_PromotionToChar(promotion); // Get promoted piece character
        board->grid[COORDS(to)] = (color == COLOR_WHITE) ? toupper(promotedPiece) : tolower(promotedPiece);
    }
    board->grid[COORDS(from)] = EMPTY_SQUARE; // Clear the source square

    // Bitboard updates
    uint64_t from_bb = 1ULL << from;
    uint64_t to_bb = 1ULL << to;

    // Identify and move the piece

    int piece;
    for (piece = 0; piece < 6; piece++) {
        uint64_t* bb = &board->bitboards[color * 6 + piece];
        if (*bb & from_bb) {
            *bb ^= from_bb; // Remove from source
            if (promotion != PROMOTION_NONE) {
                board->bitboards[color * 6 + promotion] |= to_bb; // Add promoted piece
            } else {
                *bb |= to_bb; // Move to destination
            }
            break; // Break after moving the piece
        }
    }

    // Handle captures (if the destination square was occupied by an opponent piece)
    int opponent = 1 - color;
    for (int piece = 0; piece < 6; piece++) {
        uint64_t* bb = &board->bitboards[opponent * 6 + piece];
        if (*bb & to_bb) {
            *bb ^= to_bb; // Remove the captured piece from the bitboard
            break; // Break after processing the capture
        }
    }
}

void castro_MoveToString(Move move, char* buffer)
{
    Square from, to;
    uint8_t promotion, flag;
    castro_MoveDecode(move, &from, &to, &promotion, &flag);

    char fromName[3], toName[3];
    castro_SquareToName(fromName, from);
    castro_SquareToName(toName, to);

    char p[2] = "";
    switch (promotion) {
        case PROMOTION_QUEEN:
            strcpy(p, "q"); break;
        case PROMOTION_ROOK:
            strcpy(p, "r"); break;
        case PROMOTION_BISHOP:
            strcpy(p, "b"); break;
        case PROMOTION_KNIGHT:
            strcpy(p, "n"); break;
        case PROMOTION_NONE:
            p[0] = '\0'; break; // Ensure it's an empty string
    }

    sprintf(buffer, "%s%s%s", fromName, toName, p); 
}

Move castro_StringToMove(const char* str) 
{
    if (!str || strlen(str) < 4 || strlen(str) > 5){
        fprintf(stderr, "invalid input\n");
        return NULL_MOVE;
    }

    Square from = castro_SquareFromName(str);
    Square to = castro_SquareFromName(str + 2);

    if (!castro_IsSquareValid(from)){
        fprintf(stderr, "invalid from square\n");
        return NULL_MOVE;
    }
    if(!castro_IsSquareValid(to)) {
        fprintf(stderr, "invalid to square\n");
        return NULL_MOVE;
    }

    Promotion promotion = PROMOTION_NONE;
    Flag flag = FLAG_NORMAL;

    if (strlen(str) == 5) {
        char promoChar = tolower(str[4]);
        switch (promoChar) {
            case 'q': promotion = PROMOTION_QUEEN; break;
            case 'r': promotion = PROMOTION_ROOK; break;
            case 'b': promotion = PROMOTION_BISHOP; break;
            case 'n': promotion = PROMOTION_KNIGHT; break;
            default: return NULL_MOVE;
        }
        flag = FLAG_PROMOTION;
    }

    return castro_MoveEncode(from, to, promotion, flag);
}

void castro_MovePrint(Move move)
{
    Square from, to;
    uint8_t promotion, flags;
    castro_MoveDecode(move, &from, &to, &promotion, &flags);

    char square_from[3], square_to[3];
    castro_SquareToName(square_from, from);
    castro_SquareToName(square_to, to);
    printf("Move: %s -> %s, Promotion: %d, Flags: %d\n",
            square_from, square_to,
            promotion, flags);
}

_Bool castro_MoveIsValid(const Board* board, Move move, PieceColor color)
{
    Square from, to;
    uint8_t promotion, flags;
    castro_MoveDecode(move, &from, &to, &promotion, &flags);
    uint64_t from_bb = 1ULL << from;

    for (int piece = 0; piece < 6; piece++) {
        if (board->bitboards[color * 6 + piece] & from_bb) {
            return 1;
        }
    }

    return 0;
}

char castro_PromotionToChar(uint8_t promotion)
{
    switch (promotion) {
    case PROMOTION_QUEEN: return 'q';
    case PROMOTION_ROOK: return 'r';
    case PROMOTION_BISHOP: return 'b';
    case PROMOTION_KNIGHT: return 'n';
    case PROMOTION_NONE:
    default:
          return '\0';
    }
}

uint8_t castro_CharToPromotion(char promotion)
{
    switch (promotion) {
    case 'q': return PROMOTION_QUEEN;
    case 'r': return PROMOTION_ROOK;
    case 'b': return PROMOTION_BISHOP;
    case 'n': return PROMOTION_KNIGHT;
    default:
          return PROMOTION_NONE;
    }
}

static size_t getBitboardIndexFromPiece(char piece)
{
    switch (piece) {
        case 'p': return 0;
        case 'n': return 1;
        case 'b': return 2;
        case 'r': return 3;
        case 'q': return 4;
        case 'k': return 5;
        case 'P': return 6;
        case 'N': return 7;
        case 'B': return 8;
        case 'R': return 9;
        case 'Q': return 10;
        case 'K': return 11;
        default:  return -1; // Invalid piece
    }
}

static size_t getBitboardIndexFromSquare(Board* board, Square square)
{
    Piece piece = castro_PieceAt(board, square);
    return getBitboardIndexFromPiece(piece.type);
}

bool castro_IsCastle(const Board* board, Move* move)
{
    Square from, to;
    uint8_t promotion, flag;
    castro_MoveDecode(*move, &from, &to, &promotion, &flag);

    Piece piece = castro_PieceAt(board, from);
    if (tolower(piece.type) != 'k' || piece.color != board->turn) {
        return false;
    }

    // Castling is a king move of exactly two squares horizontally
    int rowFrom = from / 8;
    int colFrom = from % 8;
    int rowTo = to / 8;
    int colTo = to % 8;

    if (rowFrom != rowTo) { // King must stay in the same rank
        return false;
    }

    int colDiff = colTo - colFrom;
    if (abs(colDiff) != 2) {
        return false;
    }

    castro_MoveSetFlag(move, FLAG_CASTLING);
    return true;
}

#define CASTLE_ROOK(dst_k, dst_q)\
    if (dst == dst_k) { \
        board->grid[COORDS(dst-1)] = dst_k < 8 ? 'R' : 'r'; \
        board->grid[COORDS(dst+1)] = EMPTY_SQUARE; \
        board->bitboards[color * 6 + INDEX_BLACK_ROOK] ^= (1ULL << (dst+1)) | (1ULL << (dst-1)); \
    } else if (dst == dst_q) { \
        board->grid[COORDS(dst+1)] = dst_q < 8 ? 'R' : 'r'; \
        board->grid[COORDS(dst-2)] = EMPTY_SQUARE; \
        board->bitboards[color * 6 + INDEX_BLACK_ROOK] ^= (1ULL << (dst-2)) | (1ULL << (dst+1)); \
    }


bool castro_Castle(Board* board, Move move)
{
    MOVE_DECODE(move);
    PieceColor color = board->turn;

    if(flag != FLAG_CASTLING) return false;
    Piece piece = castro_PieceAt(board, src);

    int colFrom = src % 8;
    int colTo = dst % 8;

    int colDiff = colTo - colFrom;

    if (colDiff == 2) { // Short castling (kingside)
        if (
            !(castro_HasCastlingRights(board, CASTLE_WHITE_KINGSIDE) && piece.color)
            &&
            !(castro_HasCastlingRights(board, CASTLE_BLACK_KINGSIDE) && !piece.color)
        ) return false;
        if(
            castro_IsSquareAttacked(board, (color) ? 6 : 62, !color)
            ||
            castro_IsSquareAttacked(board, (color) ? 5 : 61, !color)
        ) return false;
    } else if (colDiff == -2) { // Long castling (queenside)
        if (
            !(castro_HasCastlingRights(board, CASTLE_WHITE_QUEENSIDE) && piece.color)
            &&
            !(castro_HasCastlingRights(board, CASTLE_BLACK_QUEENSIDE) && !piece.color)
        ) return false;
        if(
            castro_IsSquareAttacked(board, (color) ? 3 : 59, !color)
            ||
            castro_IsSquareAttacked(board, (color) ? 2 : 58, !color)
        ) return false;
    }


    CASTLE_ROOK(6, 2);
    CASTLE_ROOK(62, 58)

    return true;
}

bool castro_IsEnpassant(const Board* board, Move* move)
{
    Square from, to;
    uint8_t promotion, flag;

    // Decode the move into its components
    castro_MoveDecode(*move, &from, &to, &promotion, &flag);

    // Ensure the piece moving is a pawn
    int color = board->turn;
    Bitboard pawnBB = board->bitboards[color * 6 + INDEX_BLACK_PAWN];
    if (!(pawnBB & (1ULL << from))) {
        return false; // Not a pawn move
    }

    // Ensure it's a diagonal move (en passant is always diagonal)
    int file_diff = abs((to % 8) - (from % 8));
    int rank_diff = abs((to / 8) - (from / 8));
    if (file_diff != 1 || rank_diff != 1) {
        return false; // Not a diagonal move
    }

    // Check if the destination square matches the en passant square
    if (to != board->enpassant_square) {
        return false; // Not en passant
    }

    castro_MoveSetFlag(move, FLAG_ENPASSANT);
    return true;
}

bool castro_Enpassant(Board* board, Move move)
{
    MOVE_DECODE(move);
    PieceColor color = board->turn;

    if (flag != FLAG_ENPASSANT) return false;

    if (dst != board->enpassant_square) return false;

    Square captured_square = color == COLOR_WHITE ? dst - 8 : dst + 8;

    // Remove the captured pawn
    board->bitboards[!color * 6 + INDEX_BLACK_PAWN] &= ~(1ULL << captured_square);
    board->grid[COORDS(captured_square)] = ' ';

    // Update the halfmove clock
    board->halfmove = 0;

    return true;
}

bool castro_IsPromotion(Board* board, Move* move)
{
    Square from = castro_GetFrom(*move);
    Square to = castro_GetTo(*move);

    if(!IS_PAWN(castro_PieceAt(board, from))) return false;

    PieceColor color = castro_PieceAt(board, from).color;

    if(color && (castro_Rank(from) != 6 || castro_Rank(to) != 7)) return false;
    if(!color && (castro_Rank(from) != 1 || castro_Rank(to) != 0)) return false;

    // if(File(from) != File(to))
    //     MoveSetFlag(move, FLAG_PROMOTION_WITH_CAPTURE);
    // else 
        castro_MoveSetFlag(move, FLAG_PROMOTION);
    return true;
}

bool castro_IsCapture(const Board* board, Move move)
{
    Square to = castro_GetTo(move);
    Piece target = castro_PieceAt(board, to);
    bool enemy_color = !board->turn;
    return IS_COLOR(target, enemy_color);
}

bool castro_IsInCheckAfterMove(Board* board, Move move)
{
    PieceColor color = board->turn ? COLOR_WHITE : COLOR_BLACK;

    if (!castro_MakeMove(board, move)) {
        return false;
    }

    bool in_check = castro_IsInCheckColor(board, color);

    castro_UnmakeMove(board);

    return in_check;
}


bool castro_IsDoublePawnPush(Board* board, Move move)
{
    Square from = castro_GetFrom(move);
    Piece piece = castro_PieceAt(board, from);
    if(!IS_PAWN(piece)) return false;

    Bitboard doublePushes = castro_PawnDoublePushMask(from, piece.color);

    return doublePushes & BB(castro_GetTo(move));
}

bool castro_MakeMove(Board* board, Move move)
{
    Piece piece = castro_PieceAt(board, castro_GetFrom(move));
    if (piece.type == EMPTY_SQUARE) return false;
    if (piece.color != board->turn) return false;

    uint8_t castling = castro_UpdateCastlingRights(board, move);
    Square enpassant = 64;

    if (castro_IsDoublePawnPush(board, move)) {
        enpassant = castro_GetFrom(move) + ((piece.color == COLOR_WHITE) ? 8 : -8);
    }

    bool succ = true;
    if (castro_IsCastle(board, &move)) {
        succ = castro_Castle(board, move);
    } else if (castro_IsEnpassant(board, &move)) {
        succ = castro_Enpassant(board, move);
    }

    if (!succ) return false;

    castro_IsPromotion(board, &move);

    castro_AddUndo(board, move);

    castro_MoveFreely(board, move, board->turn);

    board->castling_rights = castling;
    board->enpassant_square = enpassant;

    castro_UpdateHashTable(&board->history.positions, castro_CalculateZobristHash(board));

    if (board->turn == COLOR_BLACK) {
        board->fullmove++;
    }

    board->turn = !board->turn;
    board->hash = castro_CalculateZobristHash(board);

    return true;
}

void castro_UnmakeMove(Board* board)
{
    if (board->history.count <= 0) return;

    Undo undo = castro_LoadLastUndo(board);
    MOVE_DECODE(undo.move);

    Bitboard specialBB = castro_UndoMove(&board->bitboards[getBitboardIndexFromSquare(board, dst)], undo.move);
    board->grid[COORDS(src)] = board->grid[COORDS(dst)];
    board->grid[COORDS(dst)] = EMPTY_SQUARE;

    int color = !board->turn;

    if (flag == FLAG_PROMOTION) {
        // Remove promoted piece from bitboard
        switch (promotion) {
        case PROMOTION_QUEEN:
            board->bitboards[color*6 + INDEX_BLACK_QUEEN] &= ~specialBB;
            break;
        case PROMOTION_ROOK:
            board->bitboards[color*6 + INDEX_BLACK_ROOK] &= ~specialBB;
            break;
        case PROMOTION_BISHOP:
            board->bitboards[color*6 + INDEX_BLACK_BISHOP] &= ~specialBB;
            break;
        case PROMOTION_KNIGHT:
            board->bitboards[color*6 + INDEX_BLACK_KNIGHT] &= ~specialBB;
            break;
        }
        // Restore the pawn
        board->bitboards[color*6 + INDEX_BLACK_PAWN] |= (1ULL << src);
        board->grid[COORDS(src)] = (color) ? 'P' : 'p';
    } 
    else if (flag == FLAG_CASTLING) {
        if (castro_File(dst) > castro_File(src)) { // Kingside castling
            board->grid[COORDS(src + 3)] = (color) ? 'R' : 'r';
            board->grid[COORDS(dst - 1)] = EMPTY_SQUARE;
        } else { // Queenside castling
            board->grid[COORDS(src - 4)] = (color) ? 'R' : 'r';
            board->grid[COORDS(dst + 1)] = EMPTY_SQUARE;
        }
        board->bitboards[color*6 + INDEX_BLACK_ROOK] ^= specialBB;
    } 
    else if (flag == FLAG_ENPASSANT) {
        int capturedSquare = dst + ((color) ? -8 : 8);
        board->bitboards[!color*6 + INDEX_BLACK_PAWN] |= (1ULL << capturedSquare);
        board->grid[COORDS(capturedSquare)] = (color) ? 'p' : 'P';
    }

    if (undo.captured != EMPTY_SQUARE) {
        int capturedPieceIndex = getBitboardIndexFromPiece(undo.captured);
        board->bitboards[capturedPieceIndex] |= (1ULL << dst);
        board->grid[COORDS(dst)] = undo.captured;
    }

    if (board->turn == COLOR_WHITE) {
        board->fullmove--;
    }

    castro_HistoryRemove(&board->history); // Move before flipping turn

    board->turn = !board->turn;
    board->hash = castro_CalculateZobristHash(board);
}

NullMoveState nullState = {0};

void MakeNullMove(Board* board)
{
    nullState.turn = board->turn;
    nullState.halfmoveClock = board->halfmove;
    nullState.fullmoveNumber = board->fullmove;
    nullState.epSquare = board->enpassant_square;

    board->turn = (board->turn == COLOR_WHITE) ? COLOR_BLACK : COLOR_WHITE;

    board->halfmove++;
    if (board->turn == COLOR_WHITE) {
        board->fullmove++;
    }

    // Remove en passant rights
    board->enpassant_square = SQUARE_NONE;
    board->hash = castro_CalculateZobristHash(board);
}
void UnmakeNullMove(Board* board)
{
    board->turn = nullState.turn;
    board->halfmove= nullState.halfmoveClock;
    board->fullmove = nullState.fullmoveNumber;
    board->enpassant_square = nullState.epSquare;
    board->hash = castro_CalculateZobristHash(board);
}

// TODO: Don't recalculate the whole zobrist hash each time. Modify only what's changed

