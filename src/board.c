#include "castro.h"

#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


bool IsInCheck(const Board *board)
{
    return IsInCheckColor(board, board->turn);
}

bool IsInCheckColor(const Board* board, PieceColor color)
{
    Bitboard enemyAttacks = GeneratePseudoLegalAttacks(board, !color);

    return IsKingInCheck(board->bitboards[color*6 + INDEX_BLACK_KING], enemyAttacks);
}

Board BoardCopy(const Board* board)
{
    Board b = {0};
    b.turn = board->turn;
    b.history = board->history;
    b.fullmove = board->fullmove;
    b.halfmove = board->halfmove;
    b.castling_rights = board->castling_rights;
    b.enpassant_square = board->enpassant_square;
    memcpy(b.bitboards, board->bitboards, sizeof(b.bitboards));

    return b;
}

void BoardFree(Board* board)
{
    FreeHashTable(&board->history.positions);
}

Board* BoardInitFenHeap(const char* fen)
{
    Board* board = malloc(sizeof(Board));
    
    BoardInitFen(board, fen);

    return board;
}

/**
 * Parses a FEN string and initializes the Board structure.
 */
void BoardInitFen(Board *board, const char *fen)
{
    if(fen == NULL) {
        BoardInitFen(board, STARTING_FEN);
        return;
    }

    FenImport(board, fen);

    InitHashTable(&board->history.positions, fen);
}


bool IsSquareOccupiedBy(const Board* board, Square square, PieceColor color)
{
    uint64_t occupied = 0;
    for (int i = color * 6; i < (color + 1) * 6; ++i) {
        occupied |= board->bitboards[i];
    }
    return (occupied & (1ULL << square)) != 0;
}

bool IsSquareEmpty(const Board* board, Square square)
{
    for (int i = 0; i < 12; ++i) {
        if (board->bitboards[i] & (1ULL << square)) {
            return 0;
        }
    }
    return 1;
}

Square UpdateEnpassantSquare(Board* board, Move move)
{
    Piece piece = PieceAt(board, GetFrom(move));

    if(!IS_PAWN(piece)) {
        goto no_enpassant;
    }

    int from_file = Rank(GetFrom(move));
    int to_file = Rank(GetTo(move));
    int from_rank = File(GetFrom(move));
    int to_rank = File(GetTo(move));

    int file_diff = abs((int)from_file - (int)to_file);
    int rank_diff = (int)to_rank - (int)from_rank;

    if(file_diff != 0) goto no_enpassant;

    if(rank_diff != ((piece.color) ? 2 : -2)) {
        goto no_enpassant;
    }

    return ((piece.color) ? 3 : 6) * 8 + to_file;

no_enpassant:
    return 64;
}

uint8_t UpdateCastlingRights(Board* board, Move move)
{
    Square from = GetFrom(move);
    Square to = GetTo(move);
    Piece piece = PieceAt(board, from);
    Piece to_piece = PieceAt(board, to);
    int color = piece.color;
    uint8_t castling_rights = board->castling_rights;


    // Handle king moves: disable all castling rights for that color
    if (IS_KING(piece)) {
        switch (color) {
            case COLOR_WHITE:
                castling_rights &= ~(CASTLE_WHITE_KINGSIDE | CASTLE_WHITE_QUEENSIDE);
                break;
            case COLOR_BLACK:
                castling_rights &= ~(CASTLE_BLACK_KINGSIDE | CASTLE_BLACK_QUEENSIDE);
                break;
            case COLOR_NONE:
                break;
        }
    }

    // Handle rook moves: disable relevant castling rights
    if (IS_ROOK(piece)) {
        switch (color) {
        case COLOR_WHITE:
            switch (from) {
                case 0: castling_rights &= ~CASTLE_WHITE_QUEENSIDE; break;
                case 7: castling_rights &= ~CASTLE_WHITE_KINGSIDE; break;
                default: break;
            }
            break;
        case COLOR_BLACK:
            switch (from) {
                case 56: castling_rights &= ~CASTLE_BLACK_QUEENSIDE; break;
                case 63: castling_rights &= ~CASTLE_BLACK_KINGSIDE; break;
                default: break;
            }
            break;
        case COLOR_NONE:
            break;
        }
    }

    // Handle rook capture: disable castling rights for the opponent
    if (IS_ROOK(to_piece) && to_piece.color != color) {
        switch (to_piece.color) {
            case COLOR_WHITE:
                switch (to) {
                    case 0: castling_rights &= ~CASTLE_WHITE_QUEENSIDE; break;
                    case 7: castling_rights &= ~CASTLE_WHITE_KINGSIDE; break;
                    default: break;
                }
                break;
            case COLOR_BLACK:
                switch (to) {
                    case 56: castling_rights &= ~CASTLE_BLACK_QUEENSIDE; break;
                    case 63: castling_rights &= ~CASTLE_BLACK_KINGSIDE; break;
                    default: break;
                }
                break;
            case COLOR_NONE:
                break;
        }
    }

    return castling_rights;
}


void UpdateHalfmove(Board* board, Move move, size_t piece_count_before, size_t piece_count_after, char piece)
{
    int color = GetPieceColor(piece);
    int direction = (color == COLOR_WHITE) ? 1 : -1;
    bool is_pawn = tolower(piece) == 'p';
    bool is_capture = (piece_count_after < piece_count_before);
    int from_rank = Rank(GetFrom(move));
    int to_rank = Rank(GetTo(move));

    // Check if it's a pawn move
    bool is_pawn_advancement = is_pawn && (from_rank == (color == COLOR_WHITE ? 7 : 2)) &&
                                (to_rank == from_rank + direction);

    // If it's a pawn move, pawn advancement, or a capture, reset halfmove
    if (is_pawn_advancement || is_capture || is_pawn) {
        board->halfmove = 0;
    } else {
        board->halfmove++;
    }
}

size_t NumberOfPieces(const Board* board, PieceColor color)
{
    size_t count = 0;

    for (size_t i = 0; i < PIECE_TYPES; i++) {
        if ((color == COLOR_BLACK && i < 6) || (color == COLOR_WHITE && i >= 6)) {
            count += __builtin_popcountll(board->bitboards[i]);
        }
    }

    return count;
}

Bitboard GetWhite(const Board* board)
{
    return board->bitboards[INDEX_WHITE_PAWN]
         | board->bitboards[INDEX_WHITE_KNIGHT]
         | board->bitboards[INDEX_WHITE_BISHOP]
         | board->bitboards[INDEX_WHITE_ROOK]
         | board->bitboards[INDEX_WHITE_QUEEN]
         | board->bitboards[INDEX_WHITE_KING];
}
Bitboard GetBlack(const Board* board)
{
    return board->bitboards[INDEX_BLACK_PAWN]
         | board->bitboards[INDEX_BLACK_KNIGHT]
         | board->bitboards[INDEX_BLACK_BISHOP]
         | board->bitboards[INDEX_BLACK_ROOK]
         | board->bitboards[INDEX_BLACK_QUEEN]
         | board->bitboards[INDEX_BLACK_KING];
}

Bitboard GetEnemyColor(const Board *board, PieceColor us)
{
    return (us) 
        ? GetBlack(board)
        : GetWhite(board);
}

Bitboard GetEnemy(const Board *board)
{
    return GetEnemyColor(board, board->turn);
}

Bitboard GetEmpty(const Board* board)
{
    return ~(GetWhite(board) | GetBlack(board));
}


int HasCastlingRights(const Board* board, uint8_t castling_rights)
{
    return board->castling_rights & castling_rights;
}

void RevokeCastlingRights(Board* board, uint8_t castling_rights)
{
    board->castling_rights &= ~castling_rights;
}

int CountPieces(const Board* board, PieceColor color, PieceType type)
{
    Bitboard bb = board->bitboards[color * 6 + type];
    return popcount(bb);
}

