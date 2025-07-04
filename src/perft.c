#include "castro.h"

u64 Perft(Board* board, int depth, bool root)
{
    uint64_t cnt = 0, nodes = 0;
    bool leaf = (depth == 2);

    Moves moves = GenerateMoves(board, MOVE_LEGAL);
    for (int i = 0; i < moves.count; i++) {
        Move move = moves.list[i];

        if (root && depth <= 1) {
            cnt = 1;
            nodes++;
        } else {
            if (!MakeMove(board, move)) continue;
            cnt = leaf ? GenerateMoves(board, MOVE_LEGAL).count : Perft(board, depth - 1, false);
            nodes += cnt;

            UnmakeMove(board);
        }

        if (root) {
            char moveStr[16];
            MoveToString(move, moveStr);
            printf("%s: %lu\n", moveStr, cnt);
        }
    }

    return nodes;
}

