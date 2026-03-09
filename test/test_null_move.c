#include <stdint.h>
#define TAG "null_move"
#include "IncludeOnly/test.h"
#define CASTRO_STRIP_PREFIX
#include "castro.h"

int test_null_move(const char* fen)
{
    Board board = {0};
    castro_BoardInitFen(&board, fen);

    uint64_t hash_before = board.hash;
    int side_before = board.turn;
    int ep_before = board.enpassant_square;

    castro_MakeNullMove(&board);
    castro_UnmakeNullMove(&board);

    if (hash_before != board.hash || side_before != board.turn || ep_before != board.enpassant_square) {
        FAIL("State mismatch after null move cycle at FEN: %s", fen);
        castro_BoardFree(&board);
        return false;
    }

    castro_BoardFree(&board);
    SUCC("Valid null move restoration for FEN: %s", fen);
    return true;
}
