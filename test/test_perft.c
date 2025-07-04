#include "castro.h"
#include "tests.h"
#include "test-perft.h"

int test_perft(int depth, u64 expected)
{
    PERFT_TEST(NULL, depth, expected);
}
