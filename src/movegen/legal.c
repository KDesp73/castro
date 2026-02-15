#include "castro.h"
#include <stdio.h>

#ifndef RELEASE
#include "IncludeOnly/bench.h"
#endif // RELEASE

Moves castro_GenerateLegalPawnMoves(const Board *board, Bitboard pieces, PieceColor color)
{
    Moves moves = {0};
    Board temp = *board;

    while (pieces) {
        Square current = poplsb(&pieces);
        Bitboard pseudoLegal = castro_GeneratePawnMoves(board, current, color);

        while (pseudoLegal) {
            Square target = poplsb(&pseudoLegal);

            // Handle normal moves
            Move move = castro_MoveEncode(current, target, PROMOTION_NONE, FLAG_NORMAL);
            Move* mp = &move;
            if(!castro_IsPromotion((Board*) board, mp)){
                if(castro_MakeMove(&temp, move)){
                    if (!castro_IsInCheckColor(&temp, !temp.turn)){
                        castro_MovesAppend(&moves, move);
                    }
                    castro_UnmakeMove(&temp);
                } else continue;
            } else {
                // Handle promotions
                if(castro_PawnPromotionMask(current, color) || castro_PawnPromotionAttackMask(current, color)) {
                    Promotion promotions[] = {PROMOTION_QUEEN, PROMOTION_ROOK, PROMOTION_BISHOP, PROMOTION_KNIGHT};
                    for(size_t i = 0; i < 4; i++){
                        Move promotion = castro_MoveEncode(current, target, promotions[i], FLAG_PROMOTION);
                        if(castro_MakeMove(&temp, promotion)){
                            if (!castro_IsInCheckColor(&temp, !temp.turn)) {
                                castro_MovesAppend(&moves, promotion);
                            }
                            castro_UnmakeMove(&temp);
                        }
                    }
                }
            }
        }
    }

    return moves;
}

Moves castro_GenerateLegalKnightMoves(const Board* board, Bitboard pieces, PieceColor color)
{
    Moves moves = {0};
    Board temp = *board;

    while (pieces) {
        Square current = poplsb(&pieces);
        Bitboard pseudoLegal = castro_GenerateKnightMoves(board, current, color);

        while (pseudoLegal) {
            Square target = poplsb(&pseudoLegal);

            Move move = castro_MoveEncode(current, target, PROMOTION_NONE, FLAG_NORMAL);
            if(castro_MakeMove(&temp, move)){
                if (!castro_IsInCheckColor(&temp, !temp.turn)){
                    castro_MovesAppend(&moves, move);
                } 
                castro_UnmakeMove(&temp);
            }
        }
    }

    return moves;
}

Moves castro_GenerateLegalBishopMoves(const Board* board, Bitboard pieces, PieceColor color)
{
    Moves moves = {0};
    Board temp = *board;

    while (pieces) {
        Square current = poplsb(&pieces);
        Bitboard pseudoLegal = castro_GenerateBishopMoves(board, current, color);

        while (pseudoLegal) {
            Square target = poplsb(&pseudoLegal);

            Move move = castro_MoveEncode(current, target, PROMOTION_NONE, FLAG_NORMAL);
            if(castro_MakeMove(&temp, move)){
                if (!castro_IsInCheckColor(&temp, !temp.turn)){
                    castro_MovesAppend(&moves, move);
                }
                castro_UnmakeMove(&temp);
            }
        }
    }

    return moves;
}

Moves castro_GenerateLegalRookMoves(const Board* board, Bitboard pieces, PieceColor color)
{
    Moves moves = {0};
    Board temp = *board;

    while (pieces) {
        Square current = poplsb(&pieces);
        Bitboard pseudoLegal = castro_GenerateRookMoves(board, current, color);

        while (pseudoLegal) {
            Square target = poplsb(&pseudoLegal);

            Move move = castro_MoveEncode(current, target, PROMOTION_NONE, FLAG_NORMAL);
            if(castro_MakeMove(&temp, move)){
                if (!castro_IsInCheckColor(&temp, !temp.turn)){
                    castro_MovesAppend(&moves, move);
                }
                castro_UnmakeMove(&temp);
            }
        }
    }

    return moves;
}

Moves castro_GenerateLegalQueenMoves(const Board* board, Bitboard pieces, PieceColor color)
{
    return castro_MovesCombine(
        castro_GenerateLegalBishopMoves(board, pieces, color),
        castro_GenerateLegalRookMoves(board, pieces, color)
    );
}

Moves castro_GenerateLegalKingMoves(const Board* board, Bitboard pieces, PieceColor color)
{
    Moves moves = {0};
    Board temp = *board;

    Square king = lsb(pieces);
    Bitboard pseudoLegal = castro_GenerateKingMoves(&temp, king, color);

    Bitboard opponentAttacks = castro_GeneratePseudoLegalAttacks(&temp, !color);
    pseudoLegal &= ~opponentAttacks;

    // Get the opposing king's position and generate its control zone
    Bitboard opposingKing = temp.bitboards[(!color)*6 + INDEX_BLACK_KING];
    if (opposingKing) {
        Square opposingKingSquare = lsb(opposingKing);
        Bitboard opposingKingControl = castro_KingMoveMask(opposingKingSquare);

        // Exclude squares controlled by the opposing king
        pseudoLegal &= ~opposingKingControl;
    }

    // Process pseudo-legal moves
    while (pseudoLegal) {
        Square target = poplsb(&pseudoLegal);

        Move move = castro_MoveEncode(king, target, PROMOTION_NONE, FLAG_NORMAL);
        if (castro_MakeMove(&temp, move)) {
            bool isInCheck = castro_IsInCheckColor(&temp, !temp.turn);
            if (!isInCheck){
                castro_MovesAppend(&moves, move);
            }
            castro_UnmakeMove(&temp);
        }
    }

    return moves;
}

Bitboard castro_GenerateLegalMovesBitboard(const Board* board)
{
    return castro_MovesToBitboard(castro_GenerateLegalMoves(board));
}

bool castro_IsLegal(const Board* board, Move move)
{
    Moves moves = castro_GenerateLegalMoves(board);
    for(size_t i = 0; i < moves.count; i++){
        if(castro_MoveCmp(moves.list[i], move)) return true;
    }
    return false;
}

Moves castro_GenerateLegalMoves(const Board* board)
{
#ifndef RELEASE
    BENCH_START();
#endif // RELEASE
    Moves moves = {0};
    PieceColor color = board->turn;

    moves = castro_MovesCombine(
            castro_GenerateLegalPawnMoves(board, board->bitboards[color*6 + INDEX_BLACK_PAWN], color),
            castro_MovesCombine(
            castro_GenerateLegalKnightMoves(board, board->bitboards[color*6 + INDEX_BLACK_KNIGHT], color),
            castro_MovesCombine(
            castro_GenerateLegalBishopMoves(board, board->bitboards[color*6 + INDEX_BLACK_BISHOP], color),
            castro_MovesCombine(
            castro_GenerateLegalRookMoves(board, board->bitboards[color*6 + INDEX_BLACK_ROOK], color),
            castro_MovesCombine(
            castro_GenerateLegalQueenMoves(board, board->bitboards[color*6 + INDEX_BLACK_QUEEN], color),
            castro_GenerateLegalKingMoves(board, board->bitboards[color*6 + INDEX_BLACK_KING], color)
            )))));

#ifndef RELEASE
    BENCH_END();
    BENCH_LOG("GenerateLegalMoves");
#endif // RELEASE
    return moves;
}

Moves castro_GenerateLegalMovesSquare(const Board* board, Square square)
{
    Moves moves = castro_GenerateLegalMoves(board);
    Moves result = {0};
    for(size_t i = 0; i < moves.count; i++){
        if(castro_GetFrom(moves.list[i]) == square){
            castro_MovesAppend(&result, moves.list[i]);
        }
    }

    return result;
}

Moves castro_GenerateMoves(const Board* board, MoveType type)
{
    switch (type) {
    case MOVE_LEGAL:
        return castro_GenerateLegalMoves(board);
    case MOVE_PSEUDO:
        return castro_GeneratePseudoLegalMoves(board);
    }

    return NO_MOVES;
}
