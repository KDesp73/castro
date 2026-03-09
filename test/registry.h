
#ifndef TEST_REGISTRY_H
#define TEST_REGISTRY_H

#include <string.h>
#include "IncludeOnly/test.h"

#include "test_is_in_check.h"
#include "test_knight_moves.h"
#include "test_pawn_moves.h"
#include "test_perft.h"
#include "test_name_to_square.h"
#include "test_bishop_moves.h"
#include "test_undo.h"
#include "test_knight_pseudo.h"
#include "test_king_pseudo.h"
#include "test_king_moves.h"
#include "test_queen_pseudo.h"
#include "test_null_move.h"
#include "test_pawn_pseudo.h"
#include "test_rook_moves.h"
#include "test_board_hash.h"
#include "test_bishop_pseudo.h"
#include "test_square_to_name.h"
#include "test_rook_pseudo.h"
#include "test_queen_moves.h"
#include "test_piece_at.h"

// Auto-generated dispatcher
static int dispatch_test_by_name(const char* name) {
    if (strcmp(name, "test_is_in_check") == 0) return test(TEST_test_is_in_check, END);
    if (strcmp(name, "test_knight_moves") == 0) return test(TEST_test_knight_moves, END);
    if (strcmp(name, "test_pawn_moves") == 0) return test(TEST_test_pawn_moves, END);
    if (strcmp(name, "test_perft") == 0) return test(TEST_test_perft, END);
    if (strcmp(name, "test_name_to_square") == 0) return test(TEST_test_name_to_square, END);
    if (strcmp(name, "test_bishop_moves") == 0) return test(TEST_test_bishop_moves, END);
    if (strcmp(name, "test_undo") == 0) return test(TEST_test_undo, END);
    if (strcmp(name, "test_knight_pseudo") == 0) return test(TEST_test_knight_pseudo, END);
    if (strcmp(name, "test_king_pseudo") == 0) return test(TEST_test_king_pseudo, END);
    if (strcmp(name, "test_king_moves") == 0) return test(TEST_test_king_moves, END);
    if (strcmp(name, "test_queen_pseudo") == 0) return test(TEST_test_queen_pseudo, END);
    if (strcmp(name, "test_null_move") == 0) return test(TEST_test_null_move, END);
    if (strcmp(name, "test_pawn_pseudo") == 0) return test(TEST_test_pawn_pseudo, END);
    if (strcmp(name, "test_rook_moves") == 0) return test(TEST_test_rook_moves, END);
    if (strcmp(name, "test_board_hash") == 0) return test(TEST_test_board_hash, END);
    if (strcmp(name, "test_bishop_pseudo") == 0) return test(TEST_test_bishop_pseudo, END);
    if (strcmp(name, "test_square_to_name") == 0) return test(TEST_test_square_to_name, END);
    if (strcmp(name, "test_rook_pseudo") == 0) return test(TEST_test_rook_pseudo, END);
    if (strcmp(name, "test_queen_moves") == 0) return test(TEST_test_queen_moves, END);
    if (strcmp(name, "test_piece_at") == 0) return test(TEST_test_piece_at, END);
    return 1;
}

#endif