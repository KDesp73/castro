/*
 * Magic bitboards for O(1) rook/bishop attack lookup.
 * Tables filled at init using castro_*AttacksFromOccupancy.
 */
#include "castro.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define BISHOP_ENTRIES 512
#define ROOK_ENTRIES   4096
#define BISHOP_SHIFT   (64 - 9)
#define ROOK_SHIFT     (64 - 12)

static Bitboard s_bishop_mask[64];
static Bitboard s_rook_mask[64];
static Bitboard s_bishop_magic[64];
static Bitboard s_rook_magic[64];
static Bitboard s_bishop_table[64][BISHOP_ENTRIES];
static Bitboard s_rook_table[64][ROOK_ENTRIES];
static int s_bishop_bits[64];
static int s_rook_bits[64];

static Bitboard random_u64(void) {
    Bitboard r = 0;
    for (int i = 0; i < 4; i++)
        r = (r << 16) | (Bitboard)(rand() & 0xFFFF);
    return r;
}

/* Map subset index to occupancy bitboard (subset of mask). */
static Bitboard subset_to_occupancy(int index, Bitboard mask, int n, Square* squares) {
    Bitboard occ = 0;
    for (int i = 0; i < n; i++)
        if (index & (1 << i))
            occ |= BB(squares[i]);
    return occ;
}

static int find_bishop_magic(Square sq) {
    Bitboard mask = s_bishop_mask[sq];
    int n = s_bishop_bits[sq];
    Square squares[16];
    int idx = 0;
    for (int i = 0; i < 64; i++)
        if (mask & BB(i))
            squares[idx++] = (Square)i;

    for (int try_count = 0; try_count < 100000; try_count++) {
        Bitboard magic = random_u64();
        Bitboard used[BISHOP_ENTRIES];
        memset(used, 0xFF, sizeof(used));

        int ok = 1;
        for (int i = 0; i < (1 << n) && ok; i++) {
            Bitboard occ = subset_to_occupancy(i, mask, n, squares);
            Bitboard attacks = castro_BishopAttacksFromOccupancy(sq, occ);
            uint32_t j = (uint32_t)((occ * magic) >> BISHOP_SHIFT);
            if (j >= BISHOP_ENTRIES) { ok = 0; break; }
            if (used[j] != 0xFF && used[j] != attacks) { ok = 0; break; }
            used[j] = attacks;
        }
        if (!ok) continue;

        s_bishop_magic[sq] = magic;
        for (int i = 0; i < BISHOP_ENTRIES; i++)
            s_bishop_table[sq][i] = (used[i] == 0xFF) ? 0 : used[i];
        /* Verify: every subset must lookup to the same attacks as FromOccupancy */
        for (int i = 0; i < (1 << n); i++) {
            Bitboard occ = subset_to_occupancy(i, mask, n, squares);
            Bitboard expected = castro_BishopAttacksFromOccupancy(sq, occ);
            uint32_t j = (uint32_t)((occ * magic) >> BISHOP_SHIFT);
            if (s_bishop_table[sq][j] != expected) { ok = 0; break; }
        }
        if (ok) return 1;
    }
    return 0;
}

static int find_rook_magic(Square sq) {
    Bitboard mask = s_rook_mask[sq];
    int n = s_rook_bits[sq];
    Square squares[16];
    int idx = 0;
    for (int i = 0; i < 64; i++)
        if (mask & BB(i))
            squares[idx++] = (Square)i;

    for (int try_count = 0; try_count < 200000; try_count++) {
        Bitboard magic = random_u64();
        Bitboard used[ROOK_ENTRIES];
        memset(used, 0xFF, sizeof(used));

        int ok = 1;
        for (int i = 0; i < (1 << n) && ok; i++) {
            Bitboard occ = subset_to_occupancy(i, mask, n, squares);
            Bitboard attacks = castro_RookAttacksFromOccupancy(sq, occ);
            uint32_t j = (uint32_t)((occ * magic) >> ROOK_SHIFT);
            if (j >= ROOK_ENTRIES) { ok = 0; break; }
            if (used[j] != 0xFF && used[j] != attacks) { ok = 0; break; }
            used[j] = attacks;
        }
        if (!ok) continue;

        s_rook_magic[sq] = magic;
        for (int i = 0; i < ROOK_ENTRIES; i++)
            s_rook_table[sq][i] = (used[i] == 0xFF) ? 0 : used[i];
        for (int i = 0; i < (1 << n); i++) {
            Bitboard occ = subset_to_occupancy(i, mask, n, squares);
            Bitboard expected = castro_RookAttacksFromOccupancy(sq, occ);
            uint32_t j = (uint32_t)((occ * magic) >> ROOK_SHIFT);
            if (s_rook_table[sq][j] != expected) { ok = 0; break; }
        }
        if (ok) return 1;
    }
    return 0;
}

void castro_InitMagic(void) {
    for (int sq = 0; sq < 64; sq++) {
        int r = sq / 8, f = sq % 8;
        s_bishop_mask[sq] = (castro_DiagonalMask(sq) | castro_AntiDiagonalMask(sq)) & ~BB(sq);
        /* Rook: rank and file, excluding piece and outer edges (a/h files, 1st/8th ranks). */
        Bitboard rank_bb = (0xFFULL << (8 * r)) & ~FILE_A & ~FILE_H;
        Bitboard file_bb = (0x0101010101010101ULL << f) & ~RANK_1 & ~RANK_8;
        s_rook_mask[sq]   = (rank_bb | file_bb) & ~BB(sq);
        s_bishop_bits[sq] = popcount(s_bishop_mask[sq]);
        s_rook_bits[sq]   = popcount(s_rook_mask[sq]);
    }

    for (int sq = 0; sq < 64; sq++) {
        if (!find_bishop_magic(sq))
            s_bishop_magic[sq] = 0;
        if (!find_rook_magic(sq))
            s_rook_magic[sq] = 0;
    }
}

Bitboard castro_BishopAttacksMagic(Square square, Bitboard occupancy) {
    if (!s_bishop_magic[square])
        return castro_BishopAttacksFromOccupancy(square, occupancy);
    uint32_t idx = (uint32_t)((occupancy & s_bishop_mask[square]) * s_bishop_magic[square] >> BISHOP_SHIFT);
    return s_bishop_table[square][idx];
}

Bitboard castro_RookAttacksMagic(Square square, Bitboard occupancy) {
    if (!s_rook_magic[square])
        return castro_RookAttacksFromOccupancy(square, occupancy);
    uint32_t idx = (uint32_t)((occupancy & s_rook_mask[square]) * s_rook_magic[square] >> ROOK_SHIFT);
    return s_rook_table[square][idx];
}
