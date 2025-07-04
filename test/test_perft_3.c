#include "tests.h"
#include "test-perft.h"

int test_perft_3(int depth, u64 expected)
{
    PERFT_TEST("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ", depth, expected);
}
