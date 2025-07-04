#include "castro.h"
#include "test-perft.h"
#include "tests.h"

int test_perft_2(int depth, u64 expected)
{
    PERFT_TEST("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", depth, expected);
}

