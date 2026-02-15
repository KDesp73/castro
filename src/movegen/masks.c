#include "castro.h"

Bitboard castro_GeneralOccupancy(Bitboard whitePieces, Bitboard blackPieces)
{
    return whitePieces | blackPieces;
}

Bitboard castro_ComputeDiagonalMask(Square square)
{
    Bitboard result = 0;
    int rank = square / 8, file = square % 8;

    // Traverse up-left and down-right along the diagonal
    for (int r = rank - 1, f = file - 1; r >= 0 && f >= 0; --r, --f)
        result |= (1ULL << (r * 8 + f));
    for (int r = rank + 1, f = file + 1; r < 8 && f < 8; ++r, ++f)
        result |= (1ULL << (r * 8 + f));

    return result;
}

Bitboard castro_ComputeAntiDiagonalMask(Square square)
{
    Bitboard result = 0;
    int rank = square / 8, file = square % 8;

    // Traverse up-right and down-left along the anti-diagonal
    for (int r = rank - 1, f = file + 1; r >= 0 && f < 8; --r, ++f)
        result |= (1ULL << (r * 8 + f));
    for (int r = rank + 1, f = file - 1; r < 8 && f >= 0; ++r, --f)
        result |= (1ULL << (r * 8 + f));

    return result;
}

Bitboard castro_ComputeHorizontalMask(Square square)
{
    Bitboard result = 0;
    int rank = square / 8, file = square % 8;

    for (int f = file - 1; f >= 0; --f) {
        result |= (1ULL << (rank * 8 + f));
    }
    for (int f = file + 1; f <= 7; ++f) {
        result |= (1ULL << (rank * 8 + f));
    }
    return result;
}

Bitboard castro_ComputeVerticalMask(Square square)
{
    Bitboard result = 0;
    int rank = square / 8, file = square % 8;

    for (int r = rank - 1; r >= 0; --r) {
        result |= (1ULL << (r * 8 + file));
    }
    for (int r = rank + 1; r <= 7; ++r) {
        result |= (1ULL << (r * 8 + file));
    }
    return result;
}

Bitboard castro_DiagonalMask(Square square)
{
    return DIAGONAL_MASKS[square];
}
Bitboard castro_AntiDiagonalMask(Square square)
{
    return ANTI_DIAGONAL_MASKS[square];
}
Bitboard castro_HorizontalMask(Square square)
{
    return HORIZONTAL_MASKS[square];
}
Bitboard castro_VerticalMask(Square square)
{
    return VERTICAL_MASKS[square];
}

void castro_InitMasks()
{
    for (int square = 0; square < 64; ++square) {
        DIAGONAL_MASKS[square] = castro_ComputeDiagonalMask(square);
        ANTI_DIAGONAL_MASKS[square] = castro_ComputeAntiDiagonalMask(square);
        HORIZONTAL_MASKS[square] = castro_ComputeHorizontalMask(square);
        VERTICAL_MASKS[square] = castro_ComputeVerticalMask(square);
    }

    for (int square = 0; square < 64; ++square) {
        PAWN_PUSH_MASKS[COLOR_WHITE][square] = castro_ComputePawnPushMask(square, COLOR_WHITE);
        PAWN_PUSH_MASKS[COLOR_BLACK][square] = castro_ComputePawnPushMask(square, COLOR_BLACK);
        PAWN_DOUBLE_PUSH_MASKS[COLOR_WHITE][square] = castro_ComputePawnDoublePushMask(square, COLOR_WHITE);
        PAWN_DOUBLE_PUSH_MASKS[COLOR_BLACK][square] = castro_ComputePawnDoublePushMask(square, COLOR_BLACK);
        PAWN_ATTACK_MASKS[COLOR_WHITE][square] = castro_ComputePawnAttackMask(square, COLOR_WHITE);
        PAWN_ATTACK_MASKS[COLOR_BLACK][square] = castro_ComputePawnAttackMask(square, COLOR_BLACK);
        PAWN_PROMOTION_MASKS[COLOR_WHITE][square] = castro_ComputePawnPromotionMask(square, COLOR_WHITE);
        PAWN_PROMOTION_MASKS[COLOR_BLACK][square] = castro_ComputePawnPromotionMask(square, COLOR_BLACK);
        PAWN_PROMOTION_ATTACK_MASKS[COLOR_WHITE][square] = castro_ComputePawnPromotionAttackMask(square, COLOR_WHITE);
        PAWN_PROMOTION_ATTACK_MASKS[COLOR_BLACK][square] = castro_ComputePawnPromotionAttackMask(square, COLOR_BLACK);

        KNIGHT_MOVE_MASKS[square] = castro_ComputeKnightMoveMask(square);
        BISHOP_MOVE_MASKS[square] = castro_ComputeBishopMoveMask(square);
        ROOK_MOVE_MASKS[square] = castro_ComputeRookMoveMask(square);
        QUEEN_MOVE_MASKS[square] = castro_ComputeQueenMoveMask(square);
        KING_MOVE_MASKS[square] = castro_ComputeKingMoveMask(square);
    }

}

Bitboard castro_BlockerMasks(Bitboard slidingPiece, Bitboard occupancy)
{
    int square = lsb(slidingPiece); // Find the least significant bit

    Bitboard diagonalMask = castro_DiagonalMask(square);
    Bitboard antiDiagonalMask = castro_AntiDiagonalMask(square);

    Bitboard diagonalBlockers = occupancy & diagonalMask;
    Bitboard antiDiagonalBlockers = occupancy & antiDiagonalMask;

    return diagonalBlockers | antiDiagonalBlockers;
}


Bitboard castro_ComputePawnPushMask(Square square, PieceColor color)
{
    Bitboard bb = 1ULL << square, oneSquarePush;
    if(color == COLOR_WHITE){
        oneSquarePush = (bb << 8);
    } else {
        oneSquarePush = (bb >> 8);
    }

    return oneSquarePush;
}

Bitboard castro_ComputePawnDoublePushMask(Square square, PieceColor color)
{
    Bitboard bb = 1ULL << square, twoSquarePush;
    if(color == COLOR_WHITE){
        twoSquarePush = (bb & RANK_2) << 16;
    } else {
        twoSquarePush = (bb & RANK_7) >> 16;
    }

    return twoSquarePush;
}

Bitboard castro_ComputePawnAttackMask(Square square, PieceColor color)
{
    Bitboard bb = 1ULL << square;
    Bitboard leftAttacks, rightAttacks;

    if(color == COLOR_WHITE){
        leftAttacks = (bb & ~FILE_A) << 7;
        rightAttacks = (bb & ~FILE_H) << 9;
    } else {
        leftAttacks = (bb & ~FILE_H) >> 7;
        rightAttacks = (bb & ~FILE_A) >> 9;
    }

    return leftAttacks | rightAttacks;
}

Bitboard castro_ComputePawnPromotionMask(Square square, PieceColor color)
{
    Bitboard bb = 1ULL << square;
    return (color) ? (bb << 8) & RANK_8 : (bb >> 8) & RANK_1;
}

Bitboard castro_ComputePawnPromotionAttackMask(Square square, PieceColor color)
{
    Bitboard bb = 1ULL << square;
    Bitboard leftCapture, rightCapture;

    if(color == COLOR_WHITE){
        leftCapture = (bb << 7) & ~FILE_H & RANK_8;
        rightCapture = (bb << 9) & ~FILE_A & RANK_8;
    } else {
        leftCapture = (bb >> 9) & ~FILE_H & RANK_1;
        rightCapture = (bb >> 7) & ~FILE_A & RANK_1;
    }

    return leftCapture | rightCapture;
}

Bitboard castro_ComputeKnightMoveMask(Square square)
{
    Bitboard bb = 1ULL << square;
    return (((bb << 6) & ~FILE_H & ~FILE_G)
         | ((bb << 10) & ~FILE_A & ~FILE_B)
         | ((bb >> 6) & ~FILE_A & ~FILE_B)
         | ((bb >> 10) & ~FILE_H & ~FILE_G)
         | ((bb << 15)  & ~FILE_H)
         | ((bb << 17) & ~FILE_A)
         | ((bb >> 15) & ~FILE_A)
         | ((bb >> 17) & ~FILE_H));
}

Bitboard castro_ComputeBishopMoveMask(Square square)
{
    return castro_DiagonalMask(square) | castro_AntiDiagonalMask(square);
}

Bitboard castro_ComputeRookMoveMask(Square square)
{
    return castro_HorizontalMask(square) | castro_VerticalMask(square);

}

Bitboard castro_ComputeQueenMoveMask(Square square)
{
    return castro_ComputeRookMoveMask(square) | castro_ComputeBishopMoveMask(square);
}

Bitboard castro_ComputeKingMoveMask(Square square)
{
    Bitboard bb = 1ULL << square;
    return (((bb << 1) & ~FILE_A)
         | ((bb << 7) & ~FILE_H)
         | ((bb << 8))
         | ((bb << 9) & ~FILE_A)
         | ((bb >> 1) & ~FILE_H)
         | ((bb >> 7) & ~FILE_A)
         | ((bb >> 8))
         | ((bb >> 9) & ~FILE_H));
}

Bitboard castro_PawnPushMask(Square square, PieceColor color)
{
    if (square == 64) return 0ULL;
    return PAWN_PUSH_MASKS[color][square];
}
Bitboard castro_PawnDoublePushMask(Square square, PieceColor color)
{
    if (square == 64) return 0ULL;
    return PAWN_DOUBLE_PUSH_MASKS[color][square];
}
Bitboard castro_PawnAttackMask(Square square, PieceColor color)
{
    if (square == 64) return 0ULL;
    return PAWN_ATTACK_MASKS[color][square];
}
Bitboard castro_PawnPromotionMask(Square square, PieceColor color)
{
    if (square == 64) return 0ULL;
    return PAWN_PROMOTION_MASKS[color][square];
}
Bitboard castro_PawnPromotionAttackMask(Square square, PieceColor color)
{
    if (square == 64) return 0ULL;
    return PAWN_PROMOTION_ATTACK_MASKS[color][square];
}

Bitboard castro_KnightMoveMask(Square square)
{
    if (square == 64) return 0ULL;
    return KNIGHT_MOVE_MASKS[square];
}
Bitboard castro_BishopMoveMask(Square square)
{
    if (square == 64) return 0ULL;
    return BISHOP_MOVE_MASKS[square];
}
Bitboard castro_RookMoveMask(Square square)
{
    if (square == 64) return 0ULL;
    return ROOK_MOVE_MASKS[square];
}
Bitboard castro_QueenMoveMask(Square square)
{
    if (square == 64) return 0ULL;
    return QUEEN_MOVE_MASKS[square];
}
Bitboard castro_KingMoveMask(Square square)
{
    if (square == 64) return 0ULL;
    return KING_MOVE_MASKS[square];
}
