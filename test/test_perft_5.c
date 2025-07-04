#include "castro.h"
#include "test-perft.h"
#include "tests.h"

int test_perft_5(int depth, u64 expected)
{
    PERFT_TEST("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ", depth, expected);
}


