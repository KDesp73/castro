#include "castro.h"

static inline Bitboard get_ray_between(Square from, Square to)
{
    Bitboard ray = 0ULL;

    int r1 = from >> 3;
    int f1 = from & 7;
    int r2 = to >> 3;
    int f2 = to & 7;

    int dr = (r2 > r1) - (r2 < r1);
    int df = (f2 > f1) - (f2 < f1);

    int r = r1 + dr;
    int f = f1 + df;

    while (r != r2 || f != f2) {
        ray |= BB((r << 3) | f);
        r += dr;
        f += df;
    }

    return ray;
}

LegalityContext castro_CalculateLegality(const Board* board)
{
    LegalityContext ctx;

    ctx.check_mask = ~0ULL;
    ctx.check_count = 0;

    for (int i = 0; i < 64; i++)
        ctx.pin_masks[i] = ~0ULL;

    PieceColor us   = board->turn;
    PieceColor them = !us;

    Square king_sq = lsb(board->bitboards[us * 6 + INDEX_KING]);

    Bitboard empty = board->empty;
    Bitboard enemy = castro_GetEnemyColor(board, us);
    Bitboard friendly = castro_GetEnemyColor(board, !us);
    Bitboard occupied = ~empty;

    Bitboard rook_sliders =
        board->bitboards[them * 6 + INDEX_ROOK] |
        board->bitboards[them * 6 + INDEX_QUEEN];

    Bitboard bishop_sliders =
        board->bitboards[them * 6 + INDEX_BISHOP] |
        board->bitboards[them * 6 + INDEX_QUEEN];

    /* Find sliders on the same rank/file/diagonal as the king (even through
     * friendly pieces), so we can detect pins and direct checks correctly. */
    Bitboard king_rank = (0xFFULL << (8 * (king_sq >> 3)));
    Bitboard king_file = (0x0101010101010101ULL << (king_sq & 7));
    Bitboard king_diag = BB(king_sq) | castro_DiagonalMask(king_sq);
    Bitboard king_anti = BB(king_sq) | castro_AntiDiagonalMask(king_sq);

    Bitboard snipers =
        ((king_rank | king_file) & rook_sliders) |
        ((king_diag | king_anti) & bishop_sliders);

    while (snipers) {
        Square sniper_sq = poplsb(&snipers);

        Bitboard ray = get_ray_between(king_sq, sniper_sq);
        Bitboard blockers = ray & occupied;

        Bitboard friendly_blockers = blockers & friendly;
        Bitboard enemy_blockers    = blockers & enemy;

        if (!friendly_blockers && !enemy_blockers) {
            // Direct check
            ctx.check_count++;
            ctx.check_mask = BB(sniper_sq) | ray;
        }
        else if (popcount(friendly_blockers) == 1 && !enemy_blockers) {
            // Single friendly piece between king and sniper → pinned
            Square pinned_sq = lsb(friendly_blockers);
            Bitboard pin_ray = ray | BB(sniper_sq);
            ctx.pin_masks[pinned_sq] &= pin_ray;  /* intersect if pinned by multiple */
        }
    }

    // Knight checks
    Bitboard knight_attackers =
        castro_KnightMoveMask(king_sq) &
        board->bitboards[them * 6 + INDEX_KNIGHT];

    while (knight_attackers) {
        Square s = poplsb(&knight_attackers);
        ctx.check_count++;
        ctx.check_mask = BB(s);
    }

    // Pawn checks
    Bitboard pawn_attackers =
        castro_PawnAttackMask(king_sq, us) &
        board->bitboards[them * 6 + INDEX_PAWN];

    while (pawn_attackers) {
        Square s = poplsb(&pawn_attackers);
        ctx.check_count++;
        ctx.check_mask = BB(s);
    }

    return ctx;
}
