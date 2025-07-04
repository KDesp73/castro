#include "castro.h"

Bitboard DoMove(Bitboard* current, Move move)
{
    Square source, destination;
    uint8_t promotion, flag;
    MoveDecode(move, &source, &destination, &promotion, &flag);

    // Update the bitboard
    // Clear the source bit
    *current &= ~(1ULL << source);

    // Set the destination bit
    *current |= (1ULL << destination);

    if(flag == FLAG_PROMOTION){
        *current &= ~(1ULL << destination);
        return 1ULL << destination; 
    }

    return 0;
}

Bitboard UndoMove(Bitboard* current, Move move)
{
    MOVE_DECODE(move);

    if (dst < 0 || dst > 63 || src < 0 || src > 63) {
        return 0;
    }

    *current &= ~(1ULL << dst);

    *current |= (1ULL << src);

    if (flag == FLAG_PROMOTION) {
        *current &= ~(1ULL << src);
        return 1ULL << dst;
    }
    if (flag == FLAG_CASTLING) {
        // Ensure dst + 1 and dst - 1 are within bounds
        if (File(dst) > File(src)) {
            if (dst + 1 < 64 && dst - 1 >= 0)
                return 1ULL << (dst+1) | 1ULL << (dst-1);
        } else {
            if (dst + 1 < 64 && dst - 2 >= 0)
                return 1ULL << (dst+1) | 1ULL << (dst-2);
        }
    }
    if (flag == FLAG_ENPASSANT) {
        int color = (Rank(dst) > Rank(src));
        int target = dst + ((color) ? -8 : 8);
        if (target >= 0 && target < 64)
            return 1ULL << target;
    }

    return 0;
}


bool IsKingInCheck(Bitboard kingPosition, Bitboard enemyAttacks)
{
    return (kingPosition & enemyAttacks) != 0;
}

bool IsSquareAttacked(const Board* board, Square square, PieceColor attackerColor)
{
    Bitboard attacks = GeneratePseudoLegalAttacks(board, attackerColor);
    return attacks & (1ULL << square);
}
