#include "castro.h"
#include "tests.h"
#include "test-perft.h"

int test_perft(int depth, u64 expected)
{
    PERFT_TEST(1, NULL, depth, expected);
}
