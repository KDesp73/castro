#include "castro.h"

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Hashing helpers that mirror castro_CalculateZobristHash exactly.
 *
 * Random64 layout (Polyglot):
 *   [0  .. 767] pieces: pieceIndex * 64 + square  (square = flat 0-63)
 *   [768]       castling: white kingside
 *   [769]       castling: white queenside
 *   [770]       castling: black kingside
 *   [771]       castling: black queenside
 *   [772..779]  en passant file 0-7  (only when a capturing pawn exists)
 *   [780]       black to move
 *
 * pieceIndex order (matches bitboards[]):
 *   0=black pawn, 1=black knight, 2=black bishop, 3=black rook,
 *   4=black queen, 5=black king,
 *   6=white pawn, 7=white knight, 8=white bishop, 9=white rook,
 *   10=white queen, 11=white king
 */

static inline void toggle_piece(uint64_t* hash, int pieceIndex, Square sq)
{
    *hash ^= Random64[pieceIndex * 64 + sq];
}

/*
 * Recompute the en-passant contribution, mirroring castro_CalculateZobristHash.
 * The ep is only included when a friendly pawn can actually capture en passant.
 */
static uint64_t ep_hash(const Board* board)
{
    Square ep = board->enpassant_square;
    if (ep == SQUARE_NONE) return 0ULL;

    Square pawn = ep + ((!board->turn) ? NORTH : SOUTH);
    if (pawn >= 64) return 0ULL;

    Square possibleAttackers[2] = { pawn + 1, pawn - 1 };
    for (int i = 0; i < 2; i++) {
        Square a = possibleAttackers[i];
        if (a >= 64) continue;
        Piece p = castro_PieceAt(board, a);
        if (p.type != EMPTY_SQUARE && p.color == board->turn && tolower(p.type) == 'p') {
            return Random64[772 + castro_File(ep)];
        }
    }
    return 0ULL;
}

static uint64_t castling_hash(uint8_t rights)
{
    uint64_t h = 0;
    if (rights & CASTLE_WHITE_KINGSIDE)  h ^= Random64[768];
    if (rights & CASTLE_WHITE_QUEENSIDE) h ^= Random64[769];
    if (rights & CASTLE_BLACK_KINGSIDE)  h ^= Random64[770];
    if (rights & CASTLE_BLACK_QUEENSIDE) h ^= Random64[771];
    return h;
}

/* ------------------------------------------------------------------ */

static int getBitboardIndexFromPiece(char piece)
{
    switch (piece) {
        case 'p': return 0;
        case 'n': return 1;
        case 'b': return 2;
        case 'r': return 3;
        case 'q': return 4;
        case 'k': return 5;
        case 'P': return 6;
        case 'N': return 7;
        case 'B': return 8;
        case 'R': return 9;
        case 'Q': return 10;
        case 'K': return 11;
        default:  return -1;
    }
}

static int getBitboardIndexFromSquare(Board* board, Square square)
{
    Piece piece = castro_PieceAt(board, square);
    return getBitboardIndexFromPiece(piece.type);
}

/* ------------------------------------------------------------------ */

void castro_MoveFreely(Board* board, Move move, PieceColor color)
{
    Square from, to;
    uint8_t promotion, flags;
    castro_MoveDecode(move, &from, &to, &promotion, &flags);

    int pieceIdx = getBitboardIndexFromSquare(board, from);

    uint64_t from_bb = 1ULL << from;
    uint64_t to_bb   = 1ULL << to;

    /* Remove moving piece from source */
    toggle_piece(&board->hash, pieceIdx, from);

    /* Handle captures BEFORE moving so we don't confuse our own piece */
    int opponent = 1 - color;
    for (int p = 0; p < 6; p++) {
        uint64_t* bb = &board->bitboards[opponent * 6 + p];
        if (*bb & to_bb) {
            toggle_piece(&board->hash, opponent * 6 + p, to);
            *bb ^= to_bb;
            break;
        }
    }

    /* Move piece bitboard */
    board->bitboards[pieceIdx] ^= from_bb;  /* clear from */

    if (promotion != PROMOTION_NONE) {
        int promIdx = color * 6 + promotion;
        board->bitboards[promIdx] |= to_bb;
        toggle_piece(&board->hash, promIdx, to);
        char promotedPiece = castro_PromotionToChar(promotion);
        board->grid[COORDS(to)] = (color == COLOR_WHITE) ? toupper(promotedPiece) : tolower(promotedPiece);
    } else {
        board->bitboards[pieceIdx] |= to_bb;
        toggle_piece(&board->hash, pieceIdx, to);
        board->grid[COORDS(to)] = board->grid[COORDS(from)];
    }

    board->grid[COORDS(from)] = EMPTY_SQUARE;

    castro_BoardUpdateOccupancy(board);
}

/* ------------------------------------------------------------------ */

bool castro_IsCastle(const Board* board, Move* move)
{
    Square from, to;
    uint8_t promotion, flag;
    castro_MoveDecode(*move, &from, &to, &promotion, &flag);

    Piece piece = castro_PieceAt(board, from);
    if (tolower(piece.type) != 'k' || piece.color != board->turn) {
        return false;
    }

    int rowFrom = from / 8;
    int colFrom = from % 8;
    int rowTo   = to   / 8;
    int colTo   = to   % 8;

    if (rowFrom != rowTo || abs(colTo - colFrom) != 2) {
        return false;
    }

    castro_MoveSetFlag(move, FLAG_CASTLING);
    return true;
}

#define CASTLE_ROOK(dst_k, dst_q) \
    if (dst == (dst_k)) { \
        toggle_piece(&board->hash, color * 6 + INDEX_BLACK_ROOK, (dst)+1); \
        board->grid[COORDS((dst)-1)] = (dst_k) < 8 ? 'R' : 'r'; \
        board->grid[COORDS((dst)+1)] = EMPTY_SQUARE; \
        board->bitboards[color * 6 + INDEX_BLACK_ROOK] ^= (1ULL << ((dst)+1)) | (1ULL << ((dst)-1)); \
        toggle_piece(&board->hash, color * 6 + INDEX_BLACK_ROOK, (dst)-1); \
    } else if (dst == (dst_q)) { \
        toggle_piece(&board->hash, color * 6 + INDEX_BLACK_ROOK, (dst)-2); \
        board->grid[COORDS((dst)+1)] = (dst_q) < 8 ? 'R' : 'r'; \
        board->grid[COORDS((dst)-2)] = EMPTY_SQUARE; \
        board->bitboards[color * 6 + INDEX_BLACK_ROOK] ^= (1ULL << ((dst)-2)) | (1ULL << ((dst)+1)); \
        toggle_piece(&board->hash, color * 6 + INDEX_BLACK_ROOK, (dst)+1); \
    }

bool castro_Castle(Board* board, Move move)
{
    MOVE_DECODE(move);
    PieceColor color = board->turn;
    if (flag != FLAG_CASTLING) return false;
    Piece piece = castro_PieceAt(board, src);

    int colDiff = (dst % 8) - (src % 8);

    if (colDiff == 2) {
        if (!(castro_HasCastlingRights(board, CASTLE_WHITE_KINGSIDE) && piece.color) &&
            !(castro_HasCastlingRights(board, CASTLE_BLACK_KINGSIDE) && !piece.color)) return false;
        if (castro_IsSquareAttacked(board, (color) ? 6  : 62, !color) ||
            castro_IsSquareAttacked(board, (color) ? 5  : 61, !color)) return false;
    } else if (colDiff == -2) {
        if (!(castro_HasCastlingRights(board, CASTLE_WHITE_QUEENSIDE) && piece.color) &&
            !(castro_HasCastlingRights(board, CASTLE_BLACK_QUEENSIDE) && !piece.color)) return false;
        if (castro_IsSquareAttacked(board, (color) ? 3  : 59, !color) ||
            castro_IsSquareAttacked(board, (color) ? 2  : 58, !color)) return false;
    }

    CASTLE_ROOK(6, 2);
    CASTLE_ROOK(62, 58);

    return true;
}

/* ------------------------------------------------------------------ */

bool castro_IsEnpassant(const Board* board, Move* move)
{
    Square from, to;
    uint8_t promotion, flag;
    castro_MoveDecode(*move, &from, &to, &promotion, &flag);

    int color = board->turn;
    Bitboard pawnBB = board->bitboards[color * 6 + INDEX_BLACK_PAWN];
    if (!(pawnBB & (1ULL << from))) return false;

    if (abs((to % 8) - (from % 8)) != 1 || abs((to / 8) - (from / 8)) != 1) return false;
    if (to != board->enpassant_square) return false;

    castro_MoveSetFlag(move, FLAG_ENPASSANT);
    return true;
}

bool castro_Enpassant(Board* board, Move move)
{
    MOVE_DECODE(move);
    PieceColor color = board->turn;
    if (flag != FLAG_ENPASSANT || dst != board->enpassant_square) return false;

    Square captured_square = color == COLOR_WHITE ? dst - 8 : dst + 8;
    int oppPawnIdx = (!color) * 6 + INDEX_BLACK_PAWN;

    toggle_piece(&board->hash, oppPawnIdx, captured_square);
    board->bitboards[oppPawnIdx] &= ~(1ULL << captured_square);
    board->grid[COORDS(captured_square)] = EMPTY_SQUARE;
    board->halfmove = 0;
    return true;
}

/* ------------------------------------------------------------------ */

bool castro_IsPromotion(Board* board, Move* move)
{
    Square from = castro_GetFrom(*move);
    Square to   = castro_GetTo(*move);
    if (!IS_PAWN(castro_PieceAt(board, from))) return false;

    PieceColor color = castro_PieceAt(board, from).color;
    if ( color && (castro_Rank(from) != 6 || castro_Rank(to) != 7)) return false;
    if (!color && (castro_Rank(from) != 1 || castro_Rank(to) != 0)) return false;

    castro_MoveSetFlag(move, FLAG_PROMOTION);
    return true;
}

bool castro_IsCapture(const Board* board, Move move)
{
    Square to = castro_GetTo(move);
    Piece target = castro_PieceAt(board, to);
    return IS_COLOR(target, !board->turn);
}

bool castro_IsInCheckAfterMove(Board* board, Move move)
{
    PieceColor color = board->turn ? COLOR_WHITE : COLOR_BLACK;
    if (!castro_MakeMove(board, move)) return false;
    bool in_check = castro_IsInCheckColor(board, color);
    castro_UnmakeMove(board);
    return in_check;
}

bool castro_IsDoublePawnPush(Board* board, Move move)
{
    Square from = castro_GetFrom(move);
    Piece piece = castro_PieceAt(board, from);
    if (!IS_PAWN(piece)) return false;
    return castro_PawnDoublePushMask(from, piece.color) & BB(castro_GetTo(move));
}

/* ------------------------------------------------------------------ */

bool castro_MakeMove(Board* board, Move move)
{
    Piece piece = castro_PieceAt(board, castro_GetFrom(move));
    if (piece.type == EMPTY_SQUARE || piece.color != board->turn) return false;

    board->hash ^= castling_hash(board->castling_rights);
    board->hash ^= ep_hash(board);

    uint8_t castling  = castro_UpdateCastlingRights(board, move);
    Square  enpassant = SQUARE_NONE;

    if (castro_IsDoublePawnPush(board, move)) {
        enpassant = castro_GetFrom(move) + ((piece.color == COLOR_WHITE) ? 8 : -8);
    }

    bool succ = true;
    if (castro_IsCastle(board, &move)) {
        succ = castro_Castle(board, move);
    } else if (castro_IsEnpassant(board, &move)) {
        succ = castro_Enpassant(board, move);
    }
    if (!succ) return false;

    castro_IsPromotion(board, &move);
    castro_AddUndo(board, move);

    castro_MoveFreely(board, move, board->turn);

    board->castling_rights  = castling;
    board->enpassant_square = enpassant;

    board->hash ^= castling_hash(board->castling_rights);

    board->turn = !board->turn;
    board->hash ^= ep_hash(board);
    board->turn = !board->turn;

    board->hash ^= Random64[780];

    castro_UpdateHashTable(&board->history.positions, board->hash);

    if (board->turn == COLOR_BLACK) board->fullmove++;
    board->turn = !board->turn;

    return true;
}

/* ------------------------------------------------------------------ */

void castro_UnmakeMove(Board* board)
{
    if (board->history.count <= 0) return;

    Undo undo = castro_LoadLastUndo(board);
    MOVE_DECODE(undo.move);

    /* Side that made the move */
    int color = !board->turn;

    /*
     * 1. Remove post-move meta from hash.
     *    board->turn is currently the opponent of the mover.
     */
    board->hash ^= Random64[780];                         /* side to move */
    board->hash ^= castling_hash(board->castling_rights); /* current castling */
    board->hash ^= ep_hash(board);                        /* current ep */

    /* ---- Undo piece movement ---- */

    if (flag == FLAG_PROMOTION) {
        int promIdx = getBitboardIndexFromSquare(board, dst);

        toggle_piece(&board->hash, promIdx, dst);
        board->bitboards[promIdx] &= ~(1ULL << dst);
        board->grid[COORDS(dst)] = EMPTY_SQUARE;

        int pawnIdx = color * 6 + INDEX_BLACK_PAWN;
        board->bitboards[pawnIdx] |= (1ULL << src);
        board->grid[COORDS(src)] = (color == COLOR_WHITE) ? 'P' : 'p';
        toggle_piece(&board->hash, pawnIdx, src);

    } else if (flag == FLAG_CASTLING) {
        int kingIdx = color * 6 + INDEX_BLACK_KING;
        int rookIdx = color * 6 + INDEX_BLACK_ROOK;

        toggle_piece(&board->hash, kingIdx, dst);
        board->bitboards[kingIdx] &= ~(1ULL << dst);
        board->grid[COORDS(dst)] = EMPTY_SQUARE;

        board->bitboards[kingIdx] |= (1ULL << src);
        board->grid[COORDS(src)] = (color == COLOR_WHITE) ? 'K' : 'k';
        toggle_piece(&board->hash, kingIdx, src);

        if (castro_File(dst) > castro_File(src)) {
            /* Kingside: rook went from src+3 to dst-1 */
            Square rookFrom = src + 3;
            Square rookTo   = dst - 1;
            toggle_piece(&board->hash, rookIdx, rookTo);
            board->bitboards[rookIdx] &= ~(1ULL << rookTo);
            board->bitboards[rookIdx] |=  (1ULL << rookFrom);
            board->grid[COORDS(rookTo)]   = EMPTY_SQUARE;
            board->grid[COORDS(rookFrom)] = (color == COLOR_WHITE) ? 'R' : 'r';
            toggle_piece(&board->hash, rookIdx, rookFrom);
        } else {
            /* Queenside: rook went from src-4 to dst+1 */
            Square rookFrom = src - 4;
            Square rookTo   = dst + 1;
            toggle_piece(&board->hash, rookIdx, rookTo);
            board->bitboards[rookIdx] &= ~(1ULL << rookTo);
            board->bitboards[rookIdx] |=  (1ULL << rookFrom);
            board->grid[COORDS(rookTo)]   = EMPTY_SQUARE;
            board->grid[COORDS(rookFrom)] = (color == COLOR_WHITE) ? 'R' : 'r';
            toggle_piece(&board->hash, rookIdx, rookFrom);
        }

    } else if (flag == FLAG_ENPASSANT) {
        int pawnIdx = color * 6 + INDEX_BLACK_PAWN;

        toggle_piece(&board->hash, pawnIdx, dst);
        board->bitboards[pawnIdx] &= ~(1ULL << dst);
        board->bitboards[pawnIdx] |=  (1ULL << src);
        board->grid[COORDS(dst)] = EMPTY_SQUARE;
        board->grid[COORDS(src)] = (color == COLOR_WHITE) ? 'P' : 'p';
        toggle_piece(&board->hash, pawnIdx, src);

        Square capturedSq = dst + ((color == COLOR_WHITE) ? -8 : 8);
        int    oppPawnIdx = (1 - color) * 6 + INDEX_BLACK_PAWN;
        board->bitboards[oppPawnIdx] |= (1ULL << capturedSq);
        board->grid[COORDS(capturedSq)] = (color == COLOR_WHITE) ? 'p' : 'P';
        toggle_piece(&board->hash, oppPawnIdx, capturedSq);

    } else {
        /* Normal move */
        int pieceIdx = getBitboardIndexFromSquare(board, dst);

        toggle_piece(&board->hash, pieceIdx, dst);
        board->bitboards[pieceIdx] &= ~(1ULL << dst);
        board->bitboards[pieceIdx] |=  (1ULL << src);
        board->grid[COORDS(src)] = board->grid[COORDS(dst)];
        board->grid[COORDS(dst)] = EMPTY_SQUARE;
        toggle_piece(&board->hash, pieceIdx, src);
    }

    /* Restore captured piece */
    if (flag != FLAG_ENPASSANT && undo.captured != EMPTY_SQUARE) {
        int capturedIdx = getBitboardIndexFromPiece(undo.captured);
        board->bitboards[capturedIdx] |= (1ULL << dst);
        board->grid[COORDS(dst)] = undo.captured;
        toggle_piece(&board->hash, capturedIdx, dst);
    }

    /* ---- Restore board meta ---- */
    if (board->turn == COLOR_WHITE) board->fullmove--;

    castro_HistoryRemove(&board->history);
    board->turn             = !board->turn;  /* back to the mover's turn */
    board->enpassant_square = undo.enpassant;
    board->castling_rights  = undo.castling;

    castro_BoardUpdateOccupancy(board);

    /*
     * 2. Add back pre-move meta.
     *    board->turn is now COLOR_BLACK or COLOR_WHITE (the side that moved).
     *    ep_hash uses board->turn to find attacking pawns — correct now.
     */
    board->hash ^= castling_hash(board->castling_rights);
    board->hash ^= ep_hash(board);
    if (board->turn == COLOR_BLACK) {
        board->hash ^= Random64[780];
    }
}

/* ------------------------------------------------------------------ */

NullMoveState nullState = {0};

void castro_MakeNullMove(Board* board)
{
    nullState.turn           = board->turn;
    nullState.halfmoveClock  = board->halfmove;
    nullState.fullmoveNumber = board->fullmove;
    nullState.epSquare       = board->enpassant_square;

    /* Remove current ep and side-to-move */
    board->hash ^= ep_hash(board);
    if (board->turn == COLOR_BLACK) board->hash ^= Random64[780];

    board->turn             = !board->turn;
    board->halfmove++;
    if (board->turn == COLOR_WHITE) board->fullmove++;
    board->enpassant_square = SQUARE_NONE;

    /* ep is now SQUARE_NONE so nothing to add for ep.
     * Add new side-to-move. */
    if (board->turn == COLOR_BLACK) board->hash ^= Random64[780];
}

void castro_UnmakeNullMove(Board* board)
{
    /* Remove current side-to-move */
    if (board->turn == COLOR_BLACK) board->hash ^= Random64[780];
    /* ep is SQUARE_NONE, nothing to remove */

    board->turn             = nullState.turn;
    board->halfmove         = nullState.halfmoveClock;
    board->fullmove         = nullState.fullmoveNumber;
    board->enpassant_square = nullState.epSquare;

    /* Restore original ep and side-to-move */
    board->hash ^= ep_hash(board);
    if (board->turn == COLOR_BLACK) board->hash ^= Random64[780];
}
