#define TAG "board_hash"
#include "IncludeOnly/test.h"
#define CASTRO_STRIP_PREFIX
#include "castro.h"
#include <stdint.h>
#include <stdlib.h>

void debug_hash_mismatch(Board* board, uint64_t actual) {
    uint64_t expected = board->hash;
    uint64_t diff = expected ^ actual;

    printf("\n--- ZOBRIST HASH MISMATCH ---\n");
    printf("Diff Bitmask: %llu\n", (unsigned long long)diff);

    // 1. Check Turn (Index 780)
    if (diff == Random64[780]) {
        printf("FAILED: Side-to-move bit mismatch (Index 780).\n");
    }

    // 2. Check Castling (Indices 768-771)
    for (int i = 0; i < 4; i++) {
        if (diff == Random64[768 + i]) {
            printf("FAILED: Castling rights bit mismatch (Index %d).\n", 768 + i);
        }
    }

    // 3. Check En Passant (Indices 772-779)
    for (int i = 0; i < 8; i++) {
        if (diff == Random64[772 + i]) {
            printf("FAILED: En Passant bit mismatch on file %c (Index %d).\n", 'a' + i, 772 + i);
        }
    }

    // 4. Check Pieces (Indices 0-767)
    for (int p = 0; p < 12; p++) {
        for (int sq = 0; sq < 64; sq++) {
            if (diff == Random64[p * 64 + sq]) {
                char* pieceNames[] = {"P","N","B","R","Q","K","p","n","b","r","q","k"};
                char sqName[3];
                castro_SquareToName(sqName, sq);
                printf("FAILED: Piece mismatch! [%s] on square %s (Index %d).\n", 
                        pieceNames[p], sqName, p * 64 + sq);
            }
        }
    }
    printf("-----------------------------\n\n");
}

void debug_verify_indices() {
    printf("VERIFYING ZOBRIST INDICES:\n");
    char pieces[] = "PNBRQKpnbrqk";
    for(int i = 0; i < 12; i++) {
        printf("Piece %c maps to index: %d\n", pieces[i], castro_ZobristPieceToIndex(pieces[i]));
    }
}

int test_board_hash(char* initial_position, size_t moves)
{
    Board board = {0};
    castro_BoardInitFen(&board, initial_position);

    for(size_t i = 0; i < moves; ++i) {
        Moves moves = castro_GenerateMoves(&board, MOVE_LEGAL);
        Move move = moves.list[rand() % moves.count];

        if(!castro_MakeMove(&board, move)) {
            FAIL("Critical Error. Invalid move!");
            castro_BoardFree(&board);
            return false;
        }

        uint64_t actual = castro_CalculateZobristHash(&board);
        if (board.hash != actual) {
            FAIL("Board hash '%lu' != actual hash '%lu'", board.hash, actual);
            debug_hash_mismatch(&board, actual);
            castro_BoardFree(&board);
            return false;
        }
    }
    castro_BoardFree(&board);
    return true;
}
