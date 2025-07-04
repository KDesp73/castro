#include "castro.h"
#include <assert.h>

Square lsb(Bitboard b)
{
    if(!b) return 64;

#if defined(__GNUC__)
    return (Square)(__builtin_ctzll(b));
#elif defined(_MSC_VER)
    #ifdef _WIN64

    unsigned long idx;
    _BitScanForward64(&idx, b);
    return (Square) idx;

    #else
    unsigned long idx;

    if (b & 0xffffffff)
    {
        _BitScanForward(&idx, int32_t(b));
        return (Square)idx;
    }
    else
    {
        _BitScanForward(&idx, int32_t(b >> 32));
        return (Square)(idx + 32);
    }
    #endif
#else  // Compiler is neither GCC nor MSVC compatible
    #error "Compiler not supported."
#endif
}

Square msb(Bitboard b)
{
    assert(b);

#if defined(__GNUC__)  // GCC, Clang, ICX
    return (Square)(63 ^ __builtin_clzll(b));
#elif defined(_MSC_VER)
    #ifdef _WIN64  // MSVC, WIN64

    unsigned long idx;
    _BitScanReverse64(&idx, b);
    return (Square)(idx);

    #else  // MSVC, WIN32

    unsigned long idx;

    if (b >> 32)
    {
        _BitScanReverse(&idx, int32_t(b >> 32));
        return (Square)(idx + 32);
    }
    else
    {
        _BitScanReverse(&idx, int32_t(b));
        return (Square)(idx);
    }
    #endif
#else  // Compiler is neither GCC nor MSVC compatible
    #error "Compiler not supported."
#endif
}

Bitboard shift(Bitboard b, Direction D)
{
    return D == NORTH         ? b << 8
         : D == SOUTH         ? b >> 8
         : D == NORTH + NORTH ? b << 16
         : D == SOUTH + SOUTH ? b >> 16
         : D == EAST          ? (b & ~FILE_H) << 1
         : D == WEST          ? (b & ~FILE_A) >> 1
         : D == NORTH_EAST    ? (b & ~FILE_H) << 9
         : D == NORTH_WEST    ? (b & ~FILE_A) << 7
         : D == SOUTH_EAST    ? (b & ~FILE_H) >> 7
         : D == SOUTH_WEST    ? (b & ~FILE_A) >> 9
                              : 0;
}

Square poplsb(Bitboard* b)
{
    Square s = lsb(*b);
    off(b, s);
    return s;
}

void on(Bitboard* bitboard, Square square)
{
    *bitboard |= (1ULL << square);
}

void off(Bitboard* bitboard, Square square)
{
    *bitboard &= ~(1ULL << square);
}

int popcount(Bitboard bb)
{
    int count = 0;

    // Loop through all set bits using bitwise operations
    while (bb) {
        bb &= (bb - 1); // Clear the least significant set bit
        count++;
    }

    return count;
}
