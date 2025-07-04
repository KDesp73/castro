#include "castro.h"
#include <string.h>

void MovesAppend(Moves* moves, Move move)
{
    if(moves->count >= MOVES_CAPACITY) return;

    moves->list[moves->count++] = move;
}

void MovesAppendList(Moves* dest, Moves src)
{
    if (!dest) return;

    for (size_t i = 0; i < src.count && dest->count < MOVES_CAPACITY; i++) {
        dest->list[dest->count++] = src.list[i];
    }
}

Moves MovesCombine(Moves m1, Moves m2)
{
    Moves result;
    result.count = m1.count;

    // Copy the first list in one go
    memcpy(result.list, m1.list, m1.count * sizeof(Move));

    // Compute the remaining space available
    size_t remaining = MOVES_CAPACITY - result.count;
    size_t to_copy = (m2.count < remaining) ? m2.count : remaining;

    // Copy the second list in one go
    memcpy(result.list + result.count, m2.list, to_copy * sizeof(Move));
    result.count += to_copy;

    return result;
}
