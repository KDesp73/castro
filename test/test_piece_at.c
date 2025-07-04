#include "castro.h"
#include "tests.h"
#include "IncludeOnly/test.h"

int test_piece_at(const char* fen, Square square, char expected)
{
    Board board;
    BoardInitFen(&board, fen);

    char piece = PieceAt(&board, square).type;
    if(piece != expected){
        FAILF(fen, "Expected: %c. Found: %c", expected, piece);
        BoardFree(&board);
        return 0;
    }
    BoardFree(&board);
    SUCC("Passed");
    return 1;
}
