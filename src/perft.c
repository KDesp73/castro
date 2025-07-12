#include "castro.h"
#include <pthread.h>
#include <stdlib.h>

typedef struct {
    Board board;
    int depth;
    int move_index;
    Moves moves;
} PerftStackFrame;

u64 Perft(Board* board, int depth, bool root) {
    if (depth == 0) return 1;

    // Start with reasonable initial size
    size_t stack_capacity = 1024;
    PerftStackFrame* stack = malloc(sizeof(PerftStackFrame) * stack_capacity);
    if (!stack) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    // Initialize stack
    stack[0].board = *board;
    stack[0].depth = depth;
    stack[0].move_index = 0;
    stack[0].moves = GenerateMoves(&stack[0].board, MOVE_LEGAL);
    int stack_ptr = 1;
    
    u64 total = 0;

    while (stack_ptr > 0) {
        // Check if we need to grow the stack
        if (stack_ptr >= stack_capacity) {
            size_t new_capacity = stack_capacity * 2;
            if (new_capacity > 1000000) { // Cap at 1M positions
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

        PerftStackFrame* current = &stack[stack_ptr-1];
        
        if (current->depth == 1) {
            // Leaf node - count moves
            total += current->moves.count;
            stack_ptr--;
            continue;
        }

        if (current->move_index < current->moves.count) {
            // Process next move at this depth
            Move move = current->moves.list[current->move_index];
            current->move_index++;
            
            // Create new stack frame for next depth
            stack[stack_ptr].board = current->board;
            if (!MakeMove(&stack[stack_ptr].board, move)) {
                continue; // Skip illegal moves
            }
            stack[stack_ptr].depth = current->depth - 1;
            stack[stack_ptr].move_index = 0;
            stack[stack_ptr].moves = GenerateMoves(&stack[stack_ptr].board, MOVE_LEGAL);
            stack_ptr++;
        } else {
            // Done with this depth level
            stack_ptr--;
        }
    }

    free(stack);
    return total;
}
