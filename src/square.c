#include "castro.h"
#include <assert.h>
#include <string.h>

Square SquareFromCoords(size_t y, size_t x)
{
    return y * 8 + x;
}
Square SquareFromName(const char* name)
{
    return NameToSquare(name);
}

void SquareToName(char buffer[3], Square square)
{
    if(!(square >= 0 && square <= 63)){
        strcat(buffer, "-");
        return;
    }

    char file = 'a' + (square % 8);
    char rank = '1' + (square / 8);

    buffer[0] = file;
    buffer[1] = rank;
    buffer[2] = '\0';  // Null-terminate the string
}

_Bool squareIsValid(const char* name)
{
    if(name == NULL) return 0;
    if(name[0] < 'a' || name[0] > 'h') return 0;
    if(name[1] < '1' || name[1] > '8') return 0;
    
    return 1;
}

Square NameToSquare(const char buffer[3])
{
    if(!squareIsValid(buffer)) return 64;

    char file = buffer[0];
    char rank = buffer[1];

    int fileIndex = file - 'a';

    int rankIndex = rank - '1';

    Square square = (rankIndex*8) + fileIndex;

    return square;
}

// void SquareToSquareT(square_t* square, Square src)
// {
//     int file = (src% 8);
//     int rank = (7 - (src/ 8));
//     square_from_coords(square, rank, file);
// }

int Rank(Square square)
{
    return square / BOARD_SIZE;
}

int File(Square square)
{
    return square % BOARD_SIZE;
}

bool IsSquareValid(Square square)
{
    return square >= 0 && square <= 63;
}
