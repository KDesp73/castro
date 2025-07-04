#include "castro.h"
#include "tests.h"
#include "IncludeOnly/test.h"

int test_is_in_check(const char* fen, bool expected)
{
    Board board;
    BoardInitFen(&board, fen);

    Bitboard attacks = GeneratePseudoLegalAttacks(&board, !board.turn);
    bool found = IsInCheck(&board);
    if(expected != found){
        BoardPrintBitboard(&board, attacks);
        FAILF(fen, "Expected %d. Found %d", expected, found);
        goto fail;
    }

    BoardFree(&board);
    SUCC("Passed");
    return true;

fail:
    BoardFree(&board);
    return false;
}
