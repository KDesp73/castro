#include "castro.h"
#include <assert.h>
#include <string.h>

Square castro_SquareFromCoords(size_t y, size_t x)
{
    return y * 8 + x;
}
Square castro_SquareFromName(const char* name)
{
    return castro_NameToSquare(name);
}

void castro_SquareToName(char buffer[3], Square square)
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

static _Bool squareIsValid(const char* name)
{
    if(name == NULL) return 0;
    if(name[0] < 'a' || name[0] > 'h') return 0;
    if(name[1] < '1' || name[1] > '8') return 0;
    
    return 1;
}

Square castro_NameToSquare(const char buffer[3])
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

int castro_Rank(Square square)
{
    return square / BOARD_SIZE;
}

int castro_File(Square square)
{
    return square % BOARD_SIZE;
}

bool castro_IsSquareValid(Square square)
{
    return square >= 0 && square <= 63;
}
