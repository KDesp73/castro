#define TAG "perft"
#include "IncludeOnly/test.h"
#include "registry.h"
#define CASTRO_STRIP_PREFIX
#include "castro.h"
#include <stdarg.h>

int test_perft(char* fen, int depth, unsigned long long expected)
{
    Board board;
    castro_BoardInitFen(&board, fen);
    u64 count = castro_Perft(&board, depth, 1);
    castro_BoardFree(&board);
    if (count != expected) {
        FAIL("Perft %s. For depth %d. Expected %llu. Found %llu",
                fen, depth, expected, count);
        return 0;
    }
    SUCC("Perft %s. For depth %d", fen, depth);
    return 1;
}
