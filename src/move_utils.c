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

_Bool castro_MoveIsCapture(const Board* board, Move move)
{
    uint8_t flag = castro_GetFlag(move);
    if (flag == FLAG_ENPASSANT || flag == FLAG_PROMOTION_WITH_CAPTURE)
        return 1;
    Square to = castro_GetTo(move);
    Piece p = castro_PieceAt(board, to);
    return p.type != EMPTY_SQUARE && p.color != board->turn;
}

_Bool castro_MoveGivesCheck(Board* board, Move move)
{
    if (!castro_MakeMove(board, move))
        return 0;
    _Bool gives = castro_IsInCheck(board);
    castro_UnmakeMove(board);
    return gives;
}

int castro_PieceValueFromType(char piece_type)
{
    switch (tolower((unsigned char)piece_type)) {
        case 'p': return 1;
        case 'n': case 'b': return 3;
        case 'r': return 5;
        case 'q': return 9;
        case 'k': return 0;
        default: return 0;
    }
}

/* Score for ordering: hash > good captures > killers > checks (optional) > quiet. */
static int score_move_for_ordering(Board* board, Move move, Move hash_move, Move killer0, Move killer1, bool score_checks)
{
    Square from = castro_GetFrom(move);
    Square to = castro_GetTo(move);

    if (hash_move != NULL_MOVE && castro_MoveCmp(move, hash_move))
        return 1000000;

    if (castro_MoveIsCapture(board, move)) {
        int mvv = castro_PieceValueFromType(castro_PieceAt(board, to).type);
        if (mvv == 0 && castro_GetFlag(move) == FLAG_ENPASSANT)
            mvv = 1;
        int lva = castro_PieceValueFromType(castro_PieceAt(board, from).type);
        return 100000 + (mvv * 10) - lva;
    }

    if (killer0 != NULL_MOVE && castro_MoveCmp(move, killer0))
        return 50000;
    if (killer1 != NULL_MOVE && castro_MoveCmp(move, killer1))
        return 40000;

    if (score_checks && castro_MoveGivesCheck(board, move))
        return 10000;

    return 0;
}

void castro_OrderLegalMoves(Board* board, Moves* moves, Move hash_move, Move killer0, Move killer1, bool score_checks)
{
    if (moves->count <= 1) return;

    int scores[MOVES_CAPACITY];
    for (size_t i = 0; i < moves->count; i++)
        scores[i] = score_move_for_ordering(board, moves->list[i], hash_move, killer0, killer1, score_checks);

    /* Selection sort (stable, simple; replace with qsort if list is large). */
    for (size_t i = 0; i < moves->count - 1; i++) {
        size_t best = i;
        for (size_t j = i + 1; j < moves->count; j++)
            if (scores[j] > scores[best])
                best = j;
        if (best != i) {
            Move tmp = moves->list[i];
            moves->list[i] = moves->list[best];
            moves->list[best] = tmp;
            int t = scores[i];
            scores[i] = scores[best];
            scores[best] = t;
        }
    }
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
