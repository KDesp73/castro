#ifndef ENGINE_TESTS_H
#define ENGINE_TESTS_H

#include "IncludeOnly/test.h"

#define CASTRO_STRIP_PREFIX
#include "castro.h"

#define FAILF(fen, fmt, ...) \
    fprintf(stderr, ANSI_RED "FAILED %s:%d: (%s) " fmt ANSI_RESET "\n", __FILE__, __LINE__, fen, ##__VA_ARGS__)

#include "test_undo.h"
int test_undo(const char* fen, Move move);

#include "test_board_hash.h"
int test_board_hash(char* initial_position, size_t moves);

// https://www.chessprogramming.org/Perft_Results
#include "test_perft.h"
int test_perft(char* fen, int depth, u64 expected);

#include "test_piece_at.h"
int test_piece_at(const char* fen, Square square, char expected);

#include "test_name_to_square.h"
int test_name_to_square(const char name[3], int expected);

#include "test_square_to_name.h"
int test_square_to_name(Square square, const char* expected);

#include "test_pawn_moves.h"
int test_pawn_moves(const char* fen, const char* square, const char* first, ...);

#include "test_knight_moves.h"
int test_knight_moves(const char* fen, const char* square, const char* first, ...);

#include "test_bishop_moves.h"
int test_bishop_moves(const char* fen, const char* square, const char* first, ...);

#include "test_rook_moves.h"
int test_rook_moves(const char* fen, const char* square, const char* first, ...);

#include "test_queen_moves.h"
int test_queen_moves(const char* fen, const char* square, const char* first, ...);

#include "test_king_moves.h"
int test_king_moves(const char* fen, const char* square, const char* first, ...);

#include "test_pawn_pseudo.h"
int test_pawn_pseudo(const char* fen, const char* square, const char* first, ...);

#include "test_knight_pseudo.h"
int test_knight_pseudo(const char* fen, const char* square, const char* first, ...);

#include "test_bishop_pseudo.h"
int test_bishop_pseudo(const char* fen, const char* square, const char* first, ...);

#include "test_rook_pseudo.h"
int test_rook_pseudo(const char* fen, const char* square, const char* first, ...);

#include "test_queen_pseudo.h"
int test_queen_pseudo(const char* fen, const char* square, const char* first, ...);

#include "test_king_pseudo.h"
int test_king_pseudo(const char* fen, const char* square, const char* first, ...);

#include "test_is_in_check.h"
int test_is_in_check(const char* fen, bool expected);

#endif // ENGINE_TESTS_H
