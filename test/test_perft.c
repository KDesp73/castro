#include "tests.h"

int test_perft(char* fen, int depth, u64 expected)
{
    Board board;
    castro_BoardInitFen(&board, fen);
    u64 count = castro_Perft(&board, depth, 1);
    castro_BoardFree(&board);
    if (count != expected) {
        fprintf(stderr,
                "\e[0;31m"
                "FAILED: "
                "Perft %s. For depth %d. Expected %llu. Found %llu"
                "\e[0;39m"
                "\n",
                fen, depth, expected, count);
        return 0;
    }
    printf("\e[0;32m"
            "SUCCESS: "
            "Perft %s. For depth %d"
            "\e[0;39m"
            "\n",
            fen, depth);
    return 1;
}
