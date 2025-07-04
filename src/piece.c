#include "castro.h"

int GetPieceColor(char piece)
{
    if(piece == ' ') return COLOR_NONE;

    return (piece < 'a') == COLOR_WHITE;
}

Piece PieceAt(const Board* board, Square square)
{
    Piece result = {0};
    result.type = board->grid[COORDS(square)];
    result.color = GetPieceColor(result.type);
    return result;
}

bool PieceCmp(Piece p1, Piece p2)
{
    return p1.type == p2.type
        && p1.color == p2.color;
}

