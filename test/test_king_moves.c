#include "tests.h"
#include <stdarg.h>

int test_king_moves(const char* fen, const char* square, const char* first, ...)
{
    Board board;
    BoardInitFen(&board, fen);

    Square from = SquareFromName(square);

    Bitboard moves = 0ULL;
    if(first != NULL){
        va_list args;
        va_start(args, first);
        on(&moves, SquareFromName(first));
        const char* next = va_arg(args, const char*);
        while(next != NULL){
            on(&moves, SquareFromName(next));
            next = va_arg(args, const char*);
        }
        va_end(args);
    }

    PieceColor color = PieceAt(&board, from).color;
    Moves legal = {0};
    LegalityContext ctx = castro_CalculateLegality(&board);
    castro_GenerateLegalKingMoves(&board, BB(from), color, &ctx, &legal, false);
    Bitboard found = MovesToBitboard(legal);
    if(found != moves){
        FAILF(fen, "For square %s", square);
        printf("Expected: \n");
        BoardPrintBitboard(&board, moves);
        printf("Found: \n");
        BoardPrintBitboard(&board, found);
        goto fail;
    }

    BoardFree(&board);
    SUCC("Passed");
    return true;

fail:
    BoardFree(&board);
    return false;
}

