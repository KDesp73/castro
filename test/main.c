#include "castro.h"
#define TEST_IMPLEMENTATION
#include "IncludeOnly/test.h"
#include "registry.h"

const char* TEST_DIRECTORY = "test";

int main(int argc, char** argv)
{
    castro_InitMasks();
    castro_InitZobrist();

    test_set_dispatcher(dispatch_test_by_name);

    char* b0[] = { "test_perft" };
    char* b1[] = { "test_pawn_moves", "test_knight_moves", "test_bishop_moves", "test_rook_moves", "test_queen_moves", "test_king_moves" };
    char* b2[] = { "test_pawn_pseudo", "test_knight_pseudo", "test_bishop_pseudo", "test_rook_pseudo", "test_queen_pseudo", "test_king_pseudo" };
    char* b3[] = { "test_piece_at", "test_name_to_square", "test_square_to_name", "test_undo", "test_board_hash", "test_is_in_check" };
    char* b4[] = { "test_null_move" };

    char** batches[] = { b0, b1, b2, b3, b4 };
    size_t sizes[] = {
        sizeof(b0) / sizeof(char*),
        sizeof(b1) / sizeof(char*),
        sizeof(b2) / sizeof(char*),
        sizeof(b3) / sizeof(char*),
        sizeof(b4) / sizeof(char*)
    };

    test_batches(batches, sizeof(batches) / sizeof(char**), sizes);

    return test_run(argc, argv);
}
