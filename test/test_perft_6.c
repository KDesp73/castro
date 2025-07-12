#include "tests.h"
#include "castro.h"
#include "test-perft.h"

int test_perft_6(int depth, u64 expected)
{
    PERFT_TEST(6, "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", depth, expected);
}


