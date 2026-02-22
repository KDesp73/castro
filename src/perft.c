#include "castro.h"
#include <stdlib.h>

typedef struct {
    int depth;
    size_t move_index;
    Move move_made;  /* move that led to this frame (NULL_MOVE at root) */
    Moves moves;
} PerftStackFrame;

u64 castro_Perft(Board* board, int depth, bool root) {
    (void)root;
    if (depth == 0) return 1;

    Board work = *board;
    size_t stack_capacity = 1024;
    PerftStackFrame* stack = malloc(sizeof(PerftStackFrame) * stack_capacity);
    if (!stack) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    stack[0].depth = depth;
    stack[0].move_index = 0;
    stack[0].move_made = NULL_MOVE;
    stack[0].moves = castro_GenerateMoves(&work, MOVE_LEGAL);
    size_t stack_ptr = 1;
    u64 total = 0;

    while (stack_ptr > 0) {
        if (stack_ptr >= stack_capacity) {
            size_t new_capacity = stack_capacity * 2;
            if (new_capacity > 1000000) {
                fprintf(stderr, "Perft stack too large\n");
                free(stack);
                exit(1);
            }
            PerftStackFrame* new_stack = realloc(stack, sizeof(PerftStackFrame) * new_capacity);
            if (!new_stack) {
                free(stack);
                fprintf(stderr, "Memory reallocation failed\n");
                exit(1);
            }
            stack = new_stack;
            stack_capacity = new_capacity;
        }

        PerftStackFrame* current = &stack[stack_ptr - 1];

        if (current->depth == 1) {
            total += current->moves.count;
            if (current->move_made != NULL_MOVE)
                castro_UnmakeMove(&work);
            stack_ptr--;
            continue;
        }

        if (current->move_index < current->moves.count) {
            Move move = current->moves.list[current->move_index];
            current->move_index++;
            if (!castro_MakeMove(&work, move))
                continue;
            stack[stack_ptr].depth = current->depth - 1;
            stack[stack_ptr].move_index = 0;
            stack[stack_ptr].move_made = move;
            stack[stack_ptr].moves = castro_GenerateMoves(&work, MOVE_LEGAL);
            stack_ptr++;
        } else {
            if (current->move_made != NULL_MOVE)
                castro_UnmakeMove(&work);
            stack_ptr--;
        }
    }

    free(stack);
    return total;
}
