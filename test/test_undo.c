#include "IncludeOnly/test.h"
#include <stdint.h>
#include <stdio.h>
#include "tests.h"
#include "castro.h"

#define DEBUG
#include "IncludeOnly/logging.h"

int test_undo(const char* fen, Move move)
{
    Board board;
    BoardInitFen(&board, fen);
    Board originalBoard = board;
    uint64_t hash = CalculateZobristHash(&originalBoard);
    
    bool succ = MakeMove(&board, move);
    if (!succ) {
        FAIL("MakeMove failed for move: ");
        MovePrint(move);
        goto fail;
    }

    UnmakeMove(&board);

    for(size_t i = 0; i < PIECE_TYPES; i++){
        if(originalBoard.bitboards[i] != board.bitboards[i]) {
            FAILF(fen, "Mismatch in bitboard index %zu", i);
            goto fail;
        }
    }

    if(board.enpassant_square != originalBoard.enpassant_square){
        FAILF(fen, "Mismatch in enpassant square. Expected %d. Found %d", originalBoard.enpassant_square, board.enpassant_square);
        goto fail;
    }

    if(board.castling_rights != originalBoard.castling_rights){
        FAILF(fen, "Mismatch in castling rights");
        goto fail;
    }

    if(board.turn != originalBoard.turn) {
        FAILF(fen, "Mismatch in turn");
        goto fail;
    }

    if(board.fullmove != originalBoard.fullmove) {
        FAILF(fen, "Mismatch in fullmove");
        goto fail;
    }

    if(board.halfmove != originalBoard.halfmove) {
        FAILF(fen, "Mismatch in halfmove");
        goto fail;
    }

    if(CalculateZobristHash(&board) != hash){
        FAILF(fen, "Hashes don't match");
        goto fail;
    }

    BoardFree(&board);
    SUCC("Passed");
    return 1;

fail:
    BoardFree(&board);
    return false;
}
