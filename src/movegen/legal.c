#include "castro.h"
#include <stdio.h>

#ifndef RELEASE
#include "IncludeOnly/bench.h"
#endif // RELEASE

Moves GenerateLegalPawnMoves(const Board *board, Bitboard pieces, PieceColor color)
{
    Moves moves = {0};
    Board temp = *board;

    while (pieces) {
        Square current = poplsb(&pieces);
        Bitboard pseudoLegal = GeneratePawnMoves(board, current, color);

        while (pseudoLegal) {
            Square target = poplsb(&pseudoLegal);

            // Handle normal moves
            Move move = MoveEncode(current, target, PROMOTION_NONE, FLAG_NORMAL);
            Move* mp = &move;
            if(!IsPromotion((Board*) board, mp)){
                if(MakeMove(&temp, move)){
                    if (!IsInCheckColor(&temp, !temp.turn)){
                        MovesAppend(&moves, move);
                    }
                    UnmakeMove(&temp);
                } else continue;
            } else {
                // Handle promotions
                if(PawnPromotionMask(current, color) || PawnPromotionAttackMask(current, color)) {
                    Promotion promotions[] = {PROMOTION_QUEEN, PROMOTION_ROOK, PROMOTION_BISHOP, PROMOTION_KNIGHT};
                    for(size_t i = 0; i < 4; i++){
                        Move promotion = MoveEncode(current, target, promotions[i], FLAG_PROMOTION);
                        if(MakeMove(&temp, promotion)){
                            if (!IsInCheckColor(&temp, !temp.turn)) {
                                MovesAppend(&moves, promotion);
                            }
                            UnmakeMove(&temp);
                        }
                    }
                }
            }
        }
    }

    return moves;
}

Moves GenerateLegalKnightMoves(const Board* board, Bitboard pieces, PieceColor color)
{
    Moves moves = {0};
    Board temp = *board;

    while (pieces) {
        Square current = poplsb(&pieces);
        Bitboard pseudoLegal = GenerateKnightMoves(board, current, color);

        while (pseudoLegal) {
            Square target = poplsb(&pseudoLegal);

            Move move = MoveEncode(current, target, PROMOTION_NONE, FLAG_NORMAL);
            if(MakeMove(&temp, move)){
                if (!IsInCheckColor(&temp, !temp.turn)){
                    MovesAppend(&moves, move);
                } 
                UnmakeMove(&temp);
            }
        }
    }

    return moves;
}

Moves GenerateLegalBishopMoves(const Board* board, Bitboard pieces, PieceColor color)
{
    Moves moves = {0};
    Board temp = *board;

    while (pieces) {
        Square current = poplsb(&pieces);
        Bitboard pseudoLegal = GenerateBishopMoves(board, current, color);

        while (pseudoLegal) {
            Square target = poplsb(&pseudoLegal);

            Move move = MoveEncode(current, target, PROMOTION_NONE, FLAG_NORMAL);
            if(MakeMove(&temp, move)){
                if (!IsInCheckColor(&temp, !temp.turn)){
                    MovesAppend(&moves, move);
                }
                UnmakeMove(&temp);
            }
        }
    }

    return moves;
}

Moves GenerateLegalRookMoves(const Board* board, Bitboard pieces, PieceColor color)
{
    Moves moves = {0};
    Board temp = *board;

    while (pieces) {
        Square current = poplsb(&pieces);
        Bitboard pseudoLegal = GenerateRookMoves(board, current, color);

        while (pseudoLegal) {
            Square target = poplsb(&pseudoLegal);

            Move move = MoveEncode(current, target, PROMOTION_NONE, FLAG_NORMAL);
            if(MakeMove(&temp, move)){
                if (!IsInCheckColor(&temp, !temp.turn)){
                    MovesAppend(&moves, move);
                }
                UnmakeMove(&temp);
            }
        }
    }

    return moves;
}

Moves GenerateLegalQueenMoves(const Board* board, Bitboard pieces, PieceColor color)
{
    return MovesCombine(
        GenerateLegalBishopMoves(board, pieces, color),
        GenerateLegalRookMoves(board, pieces, color)
    );
}

Moves GenerateLegalKingMoves(const Board* board, Bitboard pieces, PieceColor color)
{
    Moves moves = {0};
    Board temp = *board;

    Square king = lsb(pieces);
    Bitboard pseudoLegal = GenerateKingMoves(&temp, king, color);

    Bitboard opponentAttacks = GeneratePseudoLegalAttacks(&temp, !color);
    pseudoLegal &= ~opponentAttacks;

    // Get the opposing king's position and generate its control zone
    Bitboard opposingKing = temp.bitboards[(!color)*6 + INDEX_BLACK_KING];
    if (opposingKing) {
        Square opposingKingSquare = lsb(opposingKing);
        Bitboard opposingKingControl = KingMoveMask(opposingKingSquare);

        // Exclude squares controlled by the opposing king
        pseudoLegal &= ~opposingKingControl;
    }

    // Process pseudo-legal moves
    while (pseudoLegal) {
        Square target = poplsb(&pseudoLegal);

        Move move = MoveEncode(king, target, PROMOTION_NONE, FLAG_NORMAL);
        if (MakeMove(&temp, move)) {
            bool isInCheck = IsInCheckColor(&temp, !temp.turn);
            if (!isInCheck){
                MovesAppend(&moves, move);
            }
            UnmakeMove(&temp);
        }
    }

    return moves;
}

Bitboard GenerateLegalMovesBitboard(const Board* board)
{
    return MovesToBitboard(GenerateLegalMoves(board));
}

bool IsLegal(const Board* board, Move move)
{
    Moves moves = GenerateLegalMoves(board);
    for(size_t i = 0; i < moves.count; i++){
        if(MoveCmp(moves.list[i], move)) return true;
    }
    return false;
}

Moves GenerateLegalMoves(const Board* board)
{
#ifndef RELEASE
    BENCH_START();
#endif // RELEASE
    Moves moves = {0};
    PieceColor color = board->turn;

    moves = MovesCombine(
            GenerateLegalPawnMoves(board, board->bitboards[color*6 + INDEX_BLACK_PAWN], color),
            MovesCombine(
            GenerateLegalKnightMoves(board, board->bitboards[color*6 + INDEX_BLACK_KNIGHT], color),
            MovesCombine(
            GenerateLegalBishopMoves(board, board->bitboards[color*6 + INDEX_BLACK_BISHOP], color),
            MovesCombine(
            GenerateLegalRookMoves(board, board->bitboards[color*6 + INDEX_BLACK_ROOK], color),
            MovesCombine(
            GenerateLegalQueenMoves(board, board->bitboards[color*6 + INDEX_BLACK_QUEEN], color),
            GenerateLegalKingMoves(board, board->bitboards[color*6 + INDEX_BLACK_KING], color)
            )))));

#ifndef RELEASE
    BENCH_END();
    BENCH_LOG("GenerateLegalMoves");
#endif // RELEASE
    return moves;
}

Moves GenerateLegalMovesSquare(const Board* board, Square square)
{
    Moves moves = GenerateLegalMoves(board);
    Moves result = {0};
    for(size_t i = 0; i < moves.count; i++){
        if(GetFrom(moves.list[i]) == square){
            MovesAppend(&result, moves.list[i]);
        }
    }

    return result;
}

Moves GenerateMoves(const Board* board, MoveType type)
{
    switch (type) {
    case MOVE_LEGAL:
        return GenerateLegalMoves(board);
    case MOVE_PSEUDO:
        return GeneratePseudoLegalMoves(board);
    }

    return NO_MOVES;
}
