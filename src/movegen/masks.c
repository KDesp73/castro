#include "castro.h"

Bitboard GeneralOccupancy(Bitboard whitePieces, Bitboard blackPieces)
{
    return whitePieces | blackPieces;
}

Bitboard ComputeDiagonalMask(Square square)
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

Bitboard ComputeAntiDiagonalMask(Square square)
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

Bitboard ComputeHorizontalMask(Square square)
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

Bitboard ComputeVerticalMask(Square square)
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

Bitboard DiagonalMask(Square square)
{
    return DIAGONAL_MASKS[square];
}
Bitboard AntiDiagonalMask(Square square)
{
    return ANTI_DIAGONAL_MASKS[square];
}
Bitboard HorizontalMask(Square square)
{
    return HORIZONTAL_MASKS[square];
}
Bitboard VerticalMask(Square square)
{
    return VERTICAL_MASKS[square];
}

void InitMasks()
{
    for (int square = 0; square < 64; ++square) {
        DIAGONAL_MASKS[square] = ComputeDiagonalMask(square);
        ANTI_DIAGONAL_MASKS[square] = ComputeAntiDiagonalMask(square);
        HORIZONTAL_MASKS[square] = ComputeHorizontalMask(square);
        VERTICAL_MASKS[square] = ComputeVerticalMask(square);
    }

    for (int square = 0; square < 64; ++square) {
        PAWN_PUSH_MASKS[COLOR_WHITE][square] = ComputePawnPushMask(square, COLOR_WHITE);
        PAWN_PUSH_MASKS[COLOR_BLACK][square] = ComputePawnPushMask(square, COLOR_BLACK);
        PAWN_DOUBLE_PUSH_MASKS[COLOR_WHITE][square] = ComputePawnDoublePushMask(square, COLOR_WHITE);
        PAWN_DOUBLE_PUSH_MASKS[COLOR_BLACK][square] = ComputePawnDoublePushMask(square, COLOR_BLACK);
        PAWN_ATTACK_MASKS[COLOR_WHITE][square] = ComputePawnAttackMask(square, COLOR_WHITE);
        PAWN_ATTACK_MASKS[COLOR_BLACK][square] = ComputePawnAttackMask(square, COLOR_BLACK);
        PAWN_PROMOTION_MASKS[COLOR_WHITE][square] = ComputePawnPromotionMask(square, COLOR_WHITE);
        PAWN_PROMOTION_MASKS[COLOR_BLACK][square] = ComputePawnPromotionMask(square, COLOR_BLACK);
        PAWN_PROMOTION_ATTACK_MASKS[COLOR_WHITE][square] = ComputePawnPromotionAttackMask(square, COLOR_WHITE);
        PAWN_PROMOTION_ATTACK_MASKS[COLOR_BLACK][square] = ComputePawnPromotionAttackMask(square, COLOR_BLACK);

        KNIGHT_MOVE_MASKS[square] = ComputeKnightMoveMask(square);
        BISHOP_MOVE_MASKS[square] = ComputeBishopMoveMask(square);
        ROOK_MOVE_MASKS[square] = ComputeRookMoveMask(square);
        QUEEN_MOVE_MASKS[square] = ComputeQueenMoveMask(square);
        KING_MOVE_MASKS[square] = ComputeKingMoveMask(square);
    }

}

Bitboard BlockerMasks(Bitboard slidingPiece, Bitboard occupancy)
{
    int square = lsb(slidingPiece); // Find the least significant bit

    Bitboard diagonalMask = DiagonalMask(square);
    Bitboard antiDiagonalMask = AntiDiagonalMask(square);

    Bitboard diagonalBlockers = occupancy & diagonalMask;
    Bitboard antiDiagonalBlockers = occupancy & antiDiagonalMask;

    return diagonalBlockers | antiDiagonalBlockers;
}


Bitboard ComputePawnPushMask(Square square, PieceColor color)
{
    Bitboard bb = 1ULL << square, oneSquarePush;
    if(color == COLOR_WHITE){
        oneSquarePush = (bb << 8);
    } else {
        oneSquarePush = (bb >> 8);
    }

    return oneSquarePush;
}

Bitboard ComputePawnDoublePushMask(Square square, PieceColor color)
{
    Bitboard bb = 1ULL << square, twoSquarePush;
    if(color == COLOR_WHITE){
        twoSquarePush = (bb & RANK_2) << 16;
    } else {
        twoSquarePush = (bb & RANK_7) >> 16;
    }

    return twoSquarePush;
}

Bitboard ComputePawnAttackMask(Square square, PieceColor color)
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

Bitboard ComputePawnPromotionMask(Square square, PieceColor color)
{
    Bitboard bb = 1ULL << square;
    return (color) ? (bb << 8) & RANK_8 : (bb >> 8) & RANK_1;
}

Bitboard ComputePawnPromotionAttackMask(Square square, PieceColor color)
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

Bitboard ComputeKnightMoveMask(Square square)
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

Bitboard ComputeBishopMoveMask(Square square)
{
    return DiagonalMask(square) | AntiDiagonalMask(square);
}

Bitboard ComputeRookMoveMask(Square square)
{
    return HorizontalMask(square) | VerticalMask(square);

}

Bitboard ComputeQueenMoveMask(Square square)
{
    return ComputeRookMoveMask(square) | ComputeBishopMoveMask(square);
}

Bitboard ComputeKingMoveMask(Square square)
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

Bitboard PawnPushMask(Square square, PieceColor color)
{
    if (square == 64) return 0ULL;
    return PAWN_PUSH_MASKS[color][square];
}
Bitboard PawnDoublePushMask(Square square, PieceColor color)
{
    if (square == 64) return 0ULL;
    return PAWN_DOUBLE_PUSH_MASKS[color][square];
}
Bitboard PawnAttackMask(Square square, PieceColor color)
{
    if (square == 64) return 0ULL;
    return PAWN_ATTACK_MASKS[color][square];
}
Bitboard PawnPromotionMask(Square square, PieceColor color)
{
    if (square == 64) return 0ULL;
    return PAWN_PROMOTION_MASKS[color][square];
}
Bitboard PawnPromotionAttackMask(Square square, PieceColor color)
{
    if (square == 64) return 0ULL;
    return PAWN_PROMOTION_ATTACK_MASKS[color][square];
}

Bitboard KnightMoveMask(Square square)
{
    if (square == 64) return 0ULL;
    return KNIGHT_MOVE_MASKS[square];
}
Bitboard BishopMoveMask(Square square)
{
    if (square == 64) return 0ULL;
    return BISHOP_MOVE_MASKS[square];
}
Bitboard RookMoveMask(Square square)
{
    if (square == 64) return 0ULL;
    return ROOK_MOVE_MASKS[square];
}
Bitboard QueenMoveMask(Square square)
{
    if (square == 64) return 0ULL;
    return QUEEN_MOVE_MASKS[square];
}
Bitboard KingMoveMask(Square square)
{
    if (square == 64) return 0ULL;
    return KING_MOVE_MASKS[square];
}
