#include "IncludeOnly/logging.h"
#include "castro.h"

#ifndef RELEASE
#include "IncludeOnly/bench.h"
#endif

static inline bool move_is_legal_fast(
    Square from,
    Square to,
    const LegalityContext* ctx)
{
    // Double check → only king moves allowed
    if (ctx->check_count > 1)
        return false;

    // Single check → must block or capture checker
    if (ctx->check_count == 1) {
        if (!(BB(to) & ctx->check_mask))
            return false;
    }

    // If pinned → must stay on pin ray
    if (!(BB(to) & ctx->pin_masks[from]))
        return false;

    return true;
}

/* ===================== PAWNS ===================== */

void castro_GenerateLegalPawnMoves(
    const Board *board,
    Bitboard pieces,
    PieceColor color,
    const LegalityContext* ctx,
    Moves* moves)
{
    Board temp = *board;

    while (pieces) {
        Square from = poplsb(&pieces);
        Bitboard targets = castro_GeneratePawnMoves(board, from, color);

        while (targets) {
            Square to = poplsb(&targets);

            /* En passant: pin mask may reject (ep square off the ray) but the
             * capture can still be legal; always try and let MakeMove+IsInCheck decide. */
            if (!move_is_legal_fast(from, to, ctx) && to != board->enpassant_square)
                continue;

            int rank = to >> 3;
            bool isPromotion =
                (color == COLOR_WHITE && rank == 7) ||
                (color == COLOR_BLACK && rank == 0);

            Move move;

            if (!isPromotion) {
                move = castro_MoveEncode(from, to, PROMOTION_NONE, FLAG_NORMAL);

                // EP & rare discovered cases still require validation
                if (castro_MakeMove(&temp, move)) {
                    if (!castro_IsInCheckColor(&temp, color))
                        castro_MovesAppend(moves, move);
                    castro_UnmakeMove(&temp);
                }
            } else {
                static const Promotion promos[4] = {
                    PROMOTION_QUEEN,
                    PROMOTION_ROOK,
                    PROMOTION_BISHOP,
                    PROMOTION_KNIGHT
                };

                for (int i = 0; i < 4; i++) {
                    move = castro_MoveEncode(from, to, promos[i], FLAG_PROMOTION);

                    if (castro_MakeMove(&temp, move)) {
                        if (!castro_IsInCheckColor(&temp, color))
                            castro_MovesAppend(moves, move);
                        castro_UnmakeMove(&temp);
                    }
                }
            }
        }
    }
}

/* ===================== KNIGHTS ===================== */

void castro_GenerateLegalKnightMoves(
    const Board* board,
    Bitboard pieces,
    PieceColor color,
    const LegalityContext* ctx,
    Moves* moves)
{
    if (ctx->check_count > 1)
        return;

    while (pieces) {
        Square from = poplsb(&pieces);

        /* Knights cannot move if pinned */
        if (ctx->pin_masks[from] != ~0ULL)
            continue;

        Bitboard targets = castro_GenerateKnightMoves(board, from, color);

        if (ctx->check_count == 1)
            targets &= ctx->check_mask;

        /* Pseudo-legal knight moves that pass pin/check filter are always legal
         * (a knight cannot give discovered check). */
        while (targets) {
            Square to = poplsb(&targets);
            castro_MovesAppend(moves, castro_MoveEncode(from, to, PROMOTION_NONE, FLAG_NORMAL));
        }
    }
}

/* ===================== BISHOPS ===================== */

void castro_GenerateLegalBishopMoves(
    const Board* board,
    Bitboard pieces,
    PieceColor color,
    const LegalityContext* ctx,
    Moves* moves)
{
    if (ctx->check_count > 1)
        return;

    while (pieces) {
        Square from = poplsb(&pieces);

        Bitboard targets = castro_GenerateBishopMoves(board, from, color);
        targets &= ctx->pin_masks[from];
        if (ctx->check_count == 1)
            targets &= ctx->check_mask;

        /* Slider moving along its ray cannot give discovered check. */
        while (targets) {
            Square to = poplsb(&targets);
            castro_MovesAppend(moves, castro_MoveEncode(from, to, PROMOTION_NONE, FLAG_NORMAL));
        }
    }
}

/* ===================== ROOKS ===================== */

void castro_GenerateLegalRookMoves(
    const Board* board,
    Bitboard pieces,
    PieceColor color,
    const LegalityContext* ctx,
    Moves* moves)
{
    if (ctx->check_count > 1)
        return;

    while (pieces) {
        Square from = poplsb(&pieces);

        Bitboard targets = castro_GenerateRookMoves(board, from, color);
        targets &= ctx->pin_masks[from];
        if (ctx->check_count == 1)
            targets &= ctx->check_mask;

        while (targets) {
            Square to = poplsb(&targets);
            castro_MovesAppend(moves, castro_MoveEncode(from, to, PROMOTION_NONE, FLAG_NORMAL));
        }
    }
}

/* ===================== QUEENS ===================== */

void castro_GenerateLegalQueenMoves(
    const Board* board,
    Bitboard pieces,
    PieceColor color,
    const LegalityContext* ctx,
    Moves* moves)
{
    if (ctx->check_count > 1)
        return;

    while (pieces) {
        Square from = poplsb(&pieces);

        Bitboard targets = castro_GenerateQueenMoves(board, from, color);
        targets &= ctx->pin_masks[from];
        if (ctx->check_count == 1)
            targets &= ctx->check_mask;

        while (targets) {
            Square to = poplsb(&targets);
            castro_MovesAppend(moves, castro_MoveEncode(from, to, PROMOTION_NONE, FLAG_NORMAL));
        }
    }
}

/* ===================== KING ===================== */

void castro_GenerateLegalKingMoves(
    const Board* board,
    Bitboard pieces,
    PieceColor color,
    const LegalityContext* ctx,
    Moves* moves)
{
    (void) ctx;

    Board temp = *board;
    Square king = lsb(pieces);
    Bitboard targets = castro_GenerateKingMoves(board, king, color);
    Bitboard opponentAttacks = castro_GeneratePseudoLegalAttacks(board, !color);
    targets &= ~opponentAttacks;

    /* King can unblock a ray; to-square may become attacked after moving. Must verify. */
    while (targets) {
        Square to = poplsb(&targets);
        Move move = castro_MoveEncode(king, to, PROMOTION_NONE, FLAG_NORMAL);
        if (castro_MakeMove(&temp, move)) {
            if (!castro_IsInCheckColor(&temp, color))
                castro_MovesAppend(moves, move);
            castro_UnmakeMove(&temp);
        }
    }
}

/* ===================== MASTER GENERATOR ===================== */

Moves castro_GenerateLegalMoves(const Board* board)
{
#ifndef RELEASE
    BENCH_START();
#endif

    Moves moves = {0};
    PieceColor color = board->turn;

    LegalityContext ctx = castro_CalculateLegality(board);

    castro_GenerateLegalPawnMoves(
        board,
        board->bitboards[color * 6 + INDEX_PAWN],
        color,
        &ctx,
        &moves);

    castro_GenerateLegalKnightMoves(
        board,
        board->bitboards[color * 6 + INDEX_KNIGHT],
        color,
        &ctx,
        &moves);

    castro_GenerateLegalBishopMoves(
        board,
        board->bitboards[color * 6 + INDEX_BISHOP],
        color,
        &ctx,
        &moves);

    castro_GenerateLegalRookMoves(
        board,
        board->bitboards[color * 6 + INDEX_ROOK],
        color,
        &ctx,
        &moves);

    castro_GenerateLegalQueenMoves(
        board,
        board->bitboards[color * 6 + INDEX_QUEEN],
        color,
        &ctx,
        &moves);

    castro_GenerateLegalKingMoves(
        board,
        board->bitboards[color * 6 + INDEX_KING],
        color,
        &ctx,
        &moves);

#ifndef RELEASE
    BENCH_END();
    BENCH_LOG("GenerateLegalMoves");
#endif

    return moves;
}

/* ===================== ENTRY ===================== */

Moves castro_GenerateMoves(const Board* board, MoveType type)
{
    switch (type) {
    case MOVE_LEGAL:
        return castro_GenerateLegalMoves(board);
    case MOVE_PSEUDO:
        return castro_GeneratePseudoLegalMoves(board);
    default:
        WARN("Move type not implemented.");
        return NO_MOVES;
    }
}
