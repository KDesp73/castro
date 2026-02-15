#ifndef TEST_PERFT_MACRO_H
#define TEST_PERFT_MACRO_H

#include "IncludeOnly/test.h"
#define PERFT_TEST(num, fen, depth, expected) \
    Board board; \
    castro_BoardInitFen(&board, fen); \
    u64 count = castro_Perft(&board, depth, true); \
    castro_BoardFree(&board); \
\
    if(count != expected){ \
        FAIL("Perft %d. For depth %d. Expected %llu. Found %llu", num, depth, expected, count); \
        return 0; \
    } \
 \
    SUCC("Perft %d. For depth %d", num, depth); \
    return 1

#endif // TEST_PERFT_MACRO_H
