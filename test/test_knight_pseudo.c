#define TAG "knight_pseudo"
#include "IncludeOnly/test.h"
#define CASTRO_STRIP_PREFIX
#include "castro.h"
#include <stdarg.h>

int test_knight_pseudo(const char* fen, const char* square, const char* first, ...)
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
    Bitboard found = GenerateKnightMoves(&board, from, color);
    if(found != moves){
        FAIL("Fen %s. For square %s", fen, square);
        goto fail;
    }

    BoardFree(&board);
    SUCC("Passed");
    return true;

fail:
    BoardFree(&board);
    return false;
}

