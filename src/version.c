#include "castro.h"

void version(int* major, int* minor, int* patch)
{
    if (major) *major = CASTRO_VERSION_MAJOR;
    if (minor) *minor = CASTRO_VERSION_MINOR;
    if (patch) *patch = CASTRO_VERSION_PATCH;
}

