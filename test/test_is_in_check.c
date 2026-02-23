#define TAG "is_in_check"
#include "IncludeOnly/test.h"
#define CASTRO_STRIP_PREFIX
#include "castro.h"

int test_is_in_check(const char* fen, bool expected)
{
    Board board;
    BoardInitFen(&board, fen);

    Bitboard attacks = GeneratePseudoLegalAttacks(&board, !board.turn);
    bool found = IsInCheck(&board);
    if(expected != found){
        BoardPrintBitboard(&board, attacks);
        FAIL("Fen %s. Expected %d. Found %d", fen, expected, found);
        goto fail;
    }

    BoardFree(&board);
    SUCC("Passed");
    return true;

fail:
    BoardFree(&board);
    return false;
}
