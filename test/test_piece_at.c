#define TAG "piece_at"
#include "IncludeOnly/test.h"
#define CASTRO_STRIP_PREFIX
#include "castro.h"

int test_piece_at(const char* fen, Square square, char expected)
{
    Board board;
    BoardInitFen(&board, fen);

    char piece = PieceAt(&board, square).type;
    if(piece != expected){
        FAIL("Fen %s. Expected: %c. Found: %c", fen, expected, piece);
        BoardFree(&board);
        return 0;
    }
    BoardFree(&board);
    SUCC("Passed");
    return 1;
}
