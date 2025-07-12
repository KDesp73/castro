#include "tests.h"
#include "castro.h"
#include "test-perft.h"

int test_perft_4(int depth, u64 expected)
{
    PERFT_TEST(4, "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", depth, expected);
}
