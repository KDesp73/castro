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
