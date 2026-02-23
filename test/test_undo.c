#define TAG "undo"
#include "IncludeOnly/test.h"
#define CASTRO_STRIP_PREFIX
#include "castro.h"
#include <stdint.h>
#include <stdio.h>

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
            FAIL("Fen %s. Mismatch in bitboard index %zu", fen, i);
            goto fail;
        }
    }

    if(board.enpassant_square != originalBoard.enpassant_square){
        FAIL("Fen %s. Mismatch in enpassant square. Expected %d. Found %d", fen, originalBoard.enpassant_square, board.enpassant_square);
        goto fail;
    }

    if(board.castling_rights != originalBoard.castling_rights){
        FAIL("Fen %s. Mismatch in castling rights", fen);
        goto fail;
    }

    if(board.turn != originalBoard.turn) {
        FAIL("Fen %s. Mismatch in turn", fen);
        goto fail;
    }

    if(board.fullmove != originalBoard.fullmove) {
        FAIL("Fen %s. Mismatch in fullmove", fen);
        goto fail;
    }

    if(board.halfmove != originalBoard.halfmove) {
        FAIL("Fen %s. Mismatch in halfmove", fen);
        goto fail;
    }

    if(CalculateZobristHash(&board) != hash){
        FAIL("Fen %s. Hashes don't match", fen);
        goto fail;
    }

    BoardFree(&board);
    SUCC("Passed");
    return 1;

fail:
    BoardFree(&board);
    return false;
}
