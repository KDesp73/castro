#include "castro.h"
#include <stdint.h>
#include <stdlib.h>


Bitboard castro_PawnPushes(Square pawn, Bitboard emptySquares, uint8_t color)
{
    Bitboard oneSquarePushes = castro_PawnPushMask(pawn, color) & emptySquares;

    Direction dir = color ? NORTH : SOUTH;

    Bitboard twoSquarePushes = 0ULL;
    if (oneSquarePushes != 0) {
        twoSquarePushes = castro_PawnDoublePushMask(pawn, color) & emptySquares & shift(oneSquarePushes, dir);
    }

    return oneSquarePushes | twoSquarePushes;
}

Bitboard castro_PawnPromotions(Square pawn, Bitboard emptySquares, uint8_t color)
{
    return castro_PawnPromotionMask(pawn, color) & emptySquares;
}

Bitboard castro_PawnPromotionCaptures(Square pawn, Bitboard opponentPieces, uint8_t color)
{
    return castro_PawnPromotionAttackMask(pawn, color) & opponentPieces;
}

Bitboard castro_PawnAttacks(Square pawn, Bitboard enemySquares, uint8_t color)
{
    return castro_PawnAttackMask(pawn, color) & enemySquares;
}

Bitboard castro_KnightAttacks(Square knight, Bitboard emptySquares, Bitboard enemySquares)
{
    Bitboard emptyOrEnemySquares = emptySquares | enemySquares;
    return castro_KnightMoveMask(knight) & emptyOrEnemySquares;
}

Bitboard castro_KingAttacks(Square king, Bitboard emptySquares, Bitboard enemySquares)
{
    Bitboard emptyOrEnemySquares = emptySquares | enemySquares;
    return castro_KingMoveMask(king) & emptyOrEnemySquares;
}

static Bitboard calculateBishopAttacks(int square, Bitboard occupancy) {
    Bitboard attacks = 0;
    int rank = square / 8;
    int file = square % 8;

    for (int d = 0; d < 4; ++d) {
        int direction = BISHOP_OFFSETS[d];
        int currentSquare = square;
        
        while (1) {
            // Move to the next square along the ray
            currentSquare += direction;
            
            // Calculate the new rank and file
            int currentRank = currentSquare / 8;
            int currentFile = currentSquare % 8;
            
            // Break if we move off the board
            if (currentRank < 0 || currentRank > 7 || currentFile < 0 || currentFile > 7) break;

            // Check if we've crossed a board edge
            if (abs(currentFile - file) != abs(currentRank - rank)) break;

            // Add the square to the attack bitboard
            attacks |= (1ULL << currentSquare);

            // Stop if a blocker is encountered
            if (occupancy & (1ULL << currentSquare)) break;
        }
    }

    return attacks;
}

Bitboard castro_BishopAttacks(Square bishop, Bitboard emptySquares, Bitboard enemySquares)
{
    Bitboard attacks = calculateBishopAttacks(bishop, ~emptySquares);
    Bitboard friendly = ~(emptySquares | enemySquares);

    return attacks & ~friendly;
}

static Bitboard calculateRookAttacks(int square, Bitboard occupancy) {
    Bitboard attacks = 0;
    int rank = square / 8;

    for (int d = 0; d < 4; ++d) {
        int direction = ROOK_OFFSETS[d];
        int currentSquare = square;

        while (1) {
            currentSquare += direction;

            int currentRank = currentSquare / 8;
            int currentFile = currentSquare % 8;

            if (currentRank < 0 || currentRank > 7 || currentFile < 0 || currentFile > 7) break;

            if ((direction == -1 || direction == 1) && currentRank - rank != 0) break;

            attacks |= (1ULL << currentSquare);

            if (occupancy & (1ULL << currentSquare)) break;
        }
    }

    return attacks;
}

Bitboard castro_RookAttacks(Square rook, Bitboard emptySquares, Bitboard enemySquares)
{
    Bitboard attacks = calculateRookAttacks(rook, ~emptySquares);
    Bitboard friendly = ~(emptySquares | enemySquares);

    return attacks & ~friendly;
}

Bitboard castro_QueenAttacks(Square queens, Bitboard emptySquares, Bitboard enemySquares)
{
    return castro_BishopAttacks(queens, emptySquares, enemySquares) | castro_RookAttacks(queens, emptySquares, enemySquares);
}

