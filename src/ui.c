#include "castro.h"
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void BoardPrintGrid(const Board* board)
{
    const char* padding = "   ";

    printf("%s┌───┬───┬───┬───┬───┬───┬───┬───┐\n", padding);

    for (int rank = 7; rank >= 0; rank--) {
        printf(" %d ", rank + 1);

        for (int file = 0; file <= 7; ++file) {
            printf("│");

            printf(" %c ", board->grid[rank][file]);
        }
        printf("│\n");

        if (rank != 0) {
            printf("%s├───┼───┼───┼───┼───┼───┼───┼───┤\n", padding);
        }
    }
    printf("%s└───┴───┴───┴───┴───┴───┴───┴───┘\n", padding);

    printf("%s", padding);
    for (int file = 0; file < 8; ++file) {
        char label = 'a' + file;
        printf("  %c ", label);
    }
    printf("\n");

    printf("\n");

}

void BoardPrintSquares(const Board* board, Square* squares, size_t count)
{
    const char* yellow_bg = "\033[48;5;214m"; // Background yellow color
    const char* reset = "\033[0m";            // Reset color formatting

    const char* padding = "   ";

    printf("%s┌───┬───┬───┬───┬───┬───┬───┬───┐\n", padding);

    for (int rank = 7; rank >= 0; rank--) {
        printf(" %d ", rank + 1);

        for (int file = 0; file <= 7; ++file) {
            int highlighted = 0;

            if (squares != NULL) {
                for (size_t i = 0; i < count; ++i) {
                    int square_rank = squares[i] / 8;
                    int square_file = squares[i] % 8;


                    if (rank == square_rank && file == square_file) {
                        highlighted = 1;
                        break;
                    }
                }
            }

            printf("│");

            char piece = ' '; // Default empty square
            for (int i = 0; i < PIECE_TYPES; ++i) {
                if (board->bitboards[i] & (1ULL << (rank * 8 + file))) {
                    piece = PIECES[i];
                    break;
                }
            }

            if (highlighted) {
                printf("%s %c %s", yellow_bg, piece, reset);
            } else {
                printf(" %c ", piece);
            }
        }
        printf("│\n");

        if (rank != 0) {
            printf("%s├───┼───┼───┼───┼───┼───┼───┼───┤\n", padding);
        }
    }
    printf("%s└───┴───┴───┴───┴───┴───┴───┴───┘\n", padding);

    printf("%s", padding);
    for (int file = 0; file < 8; ++file) {
        char label = 'a' + file;
        printf("  %c ", label);
    }
    printf("\n");

    printf("\n");
}

void BoardPrintBitboard(const Board* board, Bitboard highlight)
{
    Square* squares = NULL;
    int count = 0;

    squares = (Square*)malloc(64 * sizeof(Square)); 
    if (!squares) {
        perror("malloc failed");
        return;
    }

    for(size_t i = 0; i < 64; i++){
        if(highlight & (1ULL << i)) squares[count++] = (Square) i;
    }

    BoardPrintSquares(board, squares, count);
    free(squares);
}

#define TERMINATOR 64
#define INITIAL_ALLOCATION 100
void BoardPrint(const Board* board, Square first, ...)
{
    Square* squares = NULL;
    int count = 0;

    if (first != TERMINATOR) {
        va_list args;
        va_start(args, first);

        squares = (Square*)malloc(INITIAL_ALLOCATION * sizeof(Square)); 
        if (!squares) {
            perror("malloc failed");
            va_end(args);
            return;
        }

        squares[count++] = first;

        Square next_square;
        while ((next_square = va_arg(args, int)) != TERMINATOR) {
            if (count >= INITIAL_ALLOCATION) {
                Square* temp = realloc(squares, (count + INITIAL_ALLOCATION) * sizeof(Square));
                if (!temp) {
                    perror("realloc failed");
                    free(squares);
                    va_end(args);
                    return;
                }
                squares = temp;
            }
            squares[count++] = next_square;
        }

        va_end(args);

        // Resize to the exact number of squares used
        Square* temp = realloc(squares, count * sizeof(Square));
        if (!temp) {
            perror("realloc failed");
            free(squares);
            return;
        }
        squares = temp;
    }

    BoardPrintSquares(board, squares, count);

    if (squares) {
        free(squares);
    }
}

void BoardPrintBitboards(Board board)
{
    for(size_t i = 0; i < PIECE_TYPES; i++){
        printf("%2zu) %c ", i, PIECES[i]);
        Uint64Print(board.bitboards[i]);
    }
}

