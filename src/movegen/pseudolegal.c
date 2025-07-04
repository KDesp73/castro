#include "castro.h"
#include <stdio.h>
#include <stdlib.h>

Bitboard GeneratePseudoLegalPawnMoves(Bitboard pawns, Bitboard empty, PieceColor color)
{
    Bitboard result = 0ULL;
    while(pawns) {
        Square current = poplsb(&pawns);
        result |= PawnPushes(current, empty, color);
    }
    return result;
}

Bitboard GeneratePseudoLegalPawnAttacks(Bitboard pawns, Bitboard enemy, PieceColor color, bool strict)
{
    Bitboard result = 0ULL;
    while(pawns){
        Square current = poplsb(&pawns);
        if(strict)
            result |= PawnAttacks(current, enemy, color);
        else
            result |= PawnAttackMask(current, color);
    }
    return result;
}

Bitboard GeneratePseudoLegalKnightAttacks(Bitboard knights, Bitboard empty, Bitboard enemy)
{
    Bitboard result = 0ULL;
    while(knights){
        Square current = poplsb(&knights);
        result |= KnightAttacks(current, empty, enemy);
    }
    return result;
}

Bitboard GeneratePseudoLegalBishopAttacks(Bitboard bishops, Bitboard empty, Bitboard enemy)
{
    Bitboard result = 0ULL;
    while(bishops){
        Square current = poplsb(&bishops);
        result |= BishopAttacks(current, empty, enemy);
    }
    return result;
}

Bitboard GeneratePseudoLegalRookAttacks(Bitboard rooks, Bitboard empty, Bitboard enemy)
{
    Bitboard result = 0ULL;
    while(rooks){
        Square current = poplsb(&rooks);
        result |= RookAttacks(current, empty, enemy);
    }
    return result;
}

Bitboard GeneratePseudoLegalQueenAttacks(Bitboard queens, Bitboard empty, Bitboard enemy)
{
    Bitboard result = 0ULL;
    while(queens){
        Square current = poplsb(&queens);
        result |= QueenAttacks(current, empty, enemy);
    }
    return result;
}

Bitboard GeneratePseudoLegalKingAttacks(Bitboard kings, Bitboard empty, Bitboard enemy)
{
    Bitboard result = 0ULL;
    while(kings){
        Square current = poplsb(&kings);
        result |= KingAttacks(current, empty, enemy);
    }
    return result;
}


Bitboard GeneratePseudoLegalAttacks(const Board* board, PieceColor attackerColor)
{
    size_t start = (attackerColor) ? 6 : 0;
    Bitboard enemy = GetEnemyColor(board, attackerColor);
    Bitboard empty = GetEmpty(board);
    Square enpassantSquare = board->enpassant_square;

    Bitboard attacks = 
        GeneratePseudoLegalPawnAttacks(board->bitboards[start + INDEX_BLACK_PAWN], enemy, attackerColor, false)
        | GeneratePseudoLegalKnightAttacks(board->bitboards[start + INDEX_BLACK_KNIGHT], empty, enemy)
        | GeneratePseudoLegalBishopAttacks(board->bitboards[start + INDEX_BLACK_BISHOP], empty, enemy)
        | GeneratePseudoLegalRookAttacks(board->bitboards[start + INDEX_BLACK_ROOK], empty, enemy)
        | GeneratePseudoLegalQueenAttacks(board->bitboards[start + INDEX_BLACK_QUEEN], empty, enemy)
        | GeneratePseudoLegalKingAttacks(board->bitboards[start + INDEX_BLACK_KING], empty, enemy)
        ;

    if(enpassantSquare != 64){
        if(attackerColor == COLOR_WHITE){
            if((1ULL << enpassantSquare) & RANK_6){
                attacks |= (
                        PawnAttacks(board->bitboards[INDEX_WHITE_PAWN], ~0ULL, attackerColor) 
                        & (1ULL << enpassantSquare)
                        );
            }
        } else {
            if((1ULL << enpassantSquare) & RANK_3){
                attacks |= (
                        PawnAttacks(board->bitboards[INDEX_BLACK_PAWN], ~0ULL, attackerColor) 
                        & (1ULL << enpassantSquare)
                        );
            }
        }
    }

    return attacks;
}


Bitboard GeneratePawnMoves(const Board* board, Square piece, PieceColor color)
{
    Bitboard pseudoQuiet = 0ULL;
    Square enpassantSquare = board->enpassant_square;
    Bitboard enemySquares = GetEnemy(board);
    Bitboard emptySquares = GetEmpty(board);
    Bitboard enpassantBB = BB(enpassantSquare);

    pseudoQuiet = PawnPushes(piece, emptySquares, color);
    Bitboard pseudoAttacks = PawnAttacks(piece, enemySquares, color);

    if(
        (color == COLOR_WHITE && enpassantBB & RANK_6) ||
        (color == COLOR_BLACK && enpassantBB & RANK_3)
    ){
        enpassantBB = (PawnAttackMask(piece, color) & enpassantBB);
        pseudoAttacks |= enpassantBB;
    }

    return pseudoQuiet | pseudoAttacks;
}

Bitboard GenerateKnightMoves(const Board* board, Square piece, PieceColor color)
{
    Bitboard emptySquares = GetEmpty(board);
    Bitboard enemySquares = GetEnemyColor(board, color);
    Bitboard attacks = KnightAttacks(piece, emptySquares, enemySquares);

    return attacks;
}

Bitboard GenerateBishopMoves(const Board* board, Square piece, PieceColor color)
{
    Bitboard emptySquares = GetEmpty(board);
    Bitboard enemySquares = GetEnemyColor(board, color);
    Bitboard attacks = BishopAttacks(piece, emptySquares, enemySquares);

    return attacks;
}

Bitboard GenerateRookMoves(const Board* board, Square piece, PieceColor color)
{
    Bitboard emptySquares = GetEmpty(board);
    Bitboard enemySquares = GetEnemyColor(board, color);
    Bitboard attacks = RookAttacks(piece, emptySquares, enemySquares);

    return attacks;
}

Bitboard GenerateQueenMoves(const Board* board, Square piece, PieceColor color)
{
    Bitboard emptySquares = GetEmpty(board);
    Bitboard enemySquares = GetEnemyColor(board, color);
    Bitboard attacks = QueenAttacks(piece, emptySquares, enemySquares);

    return attacks;
}

Bitboard GenerateKingMoves(const Board* board, Square square, PieceColor color)
{
    Bitboard emptySquares  = GetEmpty(board);
    Bitboard enemySquares  = GetEnemyColor(board, color);
    Bitboard pseudoLegal   = KingAttacks(square, emptySquares, enemySquares);
    Bitboard attacksWhite  = GeneratePseudoLegalAttacks(board, COLOR_WHITE);
    Bitboard attacksBlack  = GeneratePseudoLegalAttacks(board, COLOR_BLACK);

    Bitboard castlingMoves = 0ULL;
    if (color == COLOR_WHITE && square == E1) {
        // White Kingside Castling: Squares e1 -> f1 -> g1 (4 -> 5 -> 6)
        if(HasCastlingRights(board, CASTLE_WHITE_KINGSIDE)
            && BB_ALL_MATCH(WHITE_KINGSIDE_CASTLE_EMPTY, emptySquares)
            && BB_NO_MATCH(WHITE_KINGSIDE_ATTACKS, attacksBlack)
        ) on(&castlingMoves, G1);

        // White Queenside Castling: Squares e1 -> d1 -> c1 (4 -> 3 -> 2)
        if(HasCastlingRights(board, CASTLE_WHITE_QUEENSIDE)
            && BB_ALL_MATCH(WHITE_QUEENSIDE_CASTLE_EMPTY, emptySquares)
            && BB_NO_MATCH(WHITE_QUEENSIDE_ATTACKS, attacksBlack)
        ) on(&castlingMoves, C1);
    } 
    else if (color == COLOR_BLACK && square == E8) {
        // Black Kingside Castling: Squares e8 -> f8 -> g8 (60 -> 61 -> 62)
        if(HasCastlingRights(board, CASTLE_BLACK_KINGSIDE)
            && BB_ALL_MATCH(BLACK_KINGSIDE_CASTLE_EMPTY, emptySquares)
            && BB_NO_MATCH(BLACK_KINGSIDE_ATTACKS, attacksWhite)
        ) on(&castlingMoves, G8);

        // Black Queenside Castling: Squares e8 -> d8 -> c8 (60 -> 59 -> 58)
        if(HasCastlingRights(board, CASTLE_BLACK_QUEENSIDE)
            && BB_ALL_MATCH(BLACK_QUEENSIDE_CASTLE_EMPTY, emptySquares)
            && BB_NO_MATCH(BLACK_QUEENSIDE_ATTACKS, attacksWhite)
        ) on(&castlingMoves, C8);
    }

    return pseudoLegal | castlingMoves;
}

Bitboard GeneratePseudoLegalMovesBitboard(const Board* board)
{
    return MovesToBitboard(GeneratePseudoLegalMoves(board));
}

Moves GeneratePseudoLegalMoves(const Board* board)
{
    Moves moves = {0};
    PieceColor color = board->turn;

    Bitboard pawnsBB = board->bitboards[color*6 + INDEX_BLACK_PAWN];
    while(pawnsBB){
        Square current = poplsb(&pawnsBB);
        MovesAppendList(&moves, BitboardToMoves(GeneratePawnMoves(board, current, color), current));
    }

    Bitboard knightsBB = board->bitboards[color*6 + INDEX_BLACK_KNIGHT];
    while(knightsBB){
        Square current = poplsb(&knightsBB);
        MovesAppendList(&moves, BitboardToMoves(GenerateKnightMoves(board, current, color), current));
    }

    Bitboard bishopsBB = board->bitboards[color*6 + INDEX_BLACK_BISHOP];
    while(bishopsBB){
        Square current = poplsb(&bishopsBB);
        MovesAppendList(&moves, BitboardToMoves(GenerateBishopMoves(board, current, color), current));
    }

    Bitboard rooksBB = board->bitboards[color*6 + INDEX_BLACK_ROOK];
    while(rooksBB){
        Square current = poplsb(&rooksBB);
        MovesAppendList(&moves, BitboardToMoves(GenerateRookMoves(board, current, color), current));
    }

    Bitboard queensBB = board->bitboards[color*6 + INDEX_BLACK_QUEEN];
    while(queensBB){
        Square current = poplsb(&queensBB);
        MovesAppendList(&moves, BitboardToMoves(GenerateQueenMoves(board, current, color), current));
    }

    Bitboard kingsBB = board->bitboards[color*6 + INDEX_BLACK_KING];
    while(kingsBB){
        Square current = poplsb(&kingsBB);
        MovesAppendList(&moves, BitboardToMoves(GenerateKingMoves(board, current, color), current));
    }

    return moves;
}
