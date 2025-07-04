#include "castro.h"
#include <string.h>
#include "IncludeOnly/test.h"

int test_square_to_name(Square square, const char* expected)
{
    char name[3];
    SquareToName(name, square);

    if(strcmp(name, expected)){
        FAIL("For square %d. Expected %s. Found %s", square, expected, name);
        return 0;
    }

    SUCC("For square %d", square);
    return 1;
}
