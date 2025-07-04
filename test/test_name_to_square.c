#include "castro.h"
#include "IncludeOnly/test.h"
#include "tests.h"

int test_name_to_square(const char name[3], int expected)
{
    Square found = NameToSquare(name);
    if(found != expected){
        FAIL("For %s. Expected %d. Found %d", name, expected, found);
        return 0;
    }

    SUCC("Passed for %s", name);
    return 1;
}
