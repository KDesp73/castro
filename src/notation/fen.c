#include "castro.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Maps a piece character to its corresponding bitboard index.
 * Uppercase characters are white pieces, lowercase are black.
 */
int charToPieceIndex(char c) 
{
    switch (c) {
        case 'P': return INDEX_WHITE_PAWN;
        case 'N': return INDEX_WHITE_KNIGHT;
        case 'B': return INDEX_WHITE_BISHOP;
        case 'R': return INDEX_WHITE_ROOK;
        case 'Q': return INDEX_WHITE_QUEEN;
        case 'K': return INDEX_WHITE_KING;
        case 'p': return INDEX_BLACK_PAWN;
        case 'n': return INDEX_BLACK_KNIGHT;
        case 'b': return INDEX_BLACK_BISHOP;
        case 'r': return INDEX_BLACK_ROOK;
        case 'q': return INDEX_BLACK_QUEEN;
        case 'k': return INDEX_BLACK_KING;
        default: return -1;
    }
}


void FenImport(Board *board, const char *fen)
{
    memset(board, 0, sizeof(Board));

    const char *ptr = fen;
    int rank = 7;
    int file = 0;

    // Parse board state
    while (*ptr && *ptr != ' ') {
        if (*ptr == '/') {
            rank--;
            file = 0;
        } else if (isdigit(*ptr)) {
            for(size_t i = 0; i < *ptr - '0'; i++){
                board->grid[rank][file] = EMPTY_SQUARE;
                file++;
            }
        } else {
            int piece_index = charToPieceIndex(*ptr);
            if (piece_index == -1) {
                return; // Invalid piece character
            }

            int square = rank * 8 + file; // Calculate square index (0 = a1, ..., 63 = h8)
            board->grid[COORDS(square)] = *ptr;
            board->bitboards[piece_index] |= (1ULL << square);
            file++;
        }
        ptr++;
    }

    if (*ptr != ' ') return;
    ptr++;

    // Parse active color
    board->turn = (*ptr == 'w') ? 1 : 0;
    ptr += 2;

    // Parse castling rights
    board->castling_rights = 0;
    while (*ptr && *ptr != ' ') {
        switch (*ptr) {
            case 'K': board->castling_rights |= 0x1; break; // White kingside
            case 'Q': board->castling_rights |= 0x2; break; // White queenside
            case 'k': board->castling_rights |= 0x4; break; // Black kingside
            case 'q': board->castling_rights |= 0x8; break; // Black queenside
        }
        ptr++;
    }

    if (*ptr != ' ') return;
    ptr++;

    // Parse en passant square
    if (*ptr == '-') {
        board->enpassant_square = 64; // No en passant
        ptr++;
    } else {
        char file_char = ptr[0];
        char rank_char = ptr[1];
        if (file_char < 'a' || file_char > 'h' || rank_char < '1' || rank_char > '8') {
            return; // Invalid FEN
        }
        board->enpassant_square = (rank_char - '1') * 8 + (file_char - 'a');
        ptr += 2;
    }

    if (*ptr != ' ') return;
    ptr++;

    // Parse halfmove clock
    board->halfmove = atoi(ptr);
    while (*ptr && *ptr != ' ') ptr++;

    if (*ptr != ' ') return;
    ptr++;

    // Parse fullmove number
    board->fullmove = atoi(ptr);
}

void FenExport(const Board* board, char buffer[])
{
    char* ptr = buffer;

    // Loop through each rank
    for (int rank = 7; rank >= 0; rank--) {
        int empty_squares = 0;

        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            _Bool piece_found = 0;

            for (int piece = 0; piece < PIECE_TYPES; piece++) {
                if (board->bitboards[piece] & (1ULL << square)) {
                    if (empty_squares > 0) {
                        *ptr++ = '0' + empty_squares;
                        empty_squares = 0;
                    }
                    *ptr++ = PIECES[piece];
                    piece_found = 1;
                    break;
                }
            }

            if (!piece_found) {
                empty_squares++;
            }
        }

        if (empty_squares > 0) {
            *ptr++ = '0' + empty_squares;
        }

        if (rank > 0) {
            *ptr++ = '/';
        }
    }

    *ptr++ = ' ';

    // Side to move
    *ptr++ = (board->turn) ? 'w' : 'b';
    *ptr++ = ' ';

    // Castling rights
    _Bool castling_written = 0;
    if (board->castling_rights & 0x1) { *ptr++ = 'K'; castling_written = 1; }
    if (board->castling_rights & 0x2) { *ptr++ = 'Q'; castling_written = 1; }
    if (board->castling_rights & 0x4) { *ptr++ = 'k'; castling_written = 1; }
    if (board->castling_rights & 0x8) { *ptr++ = 'q'; castling_written = 1; }
    if (!castling_written) {
        *ptr++ = '-';
    }
    *ptr++ = ' ';

    // En passant square
    if (board->enpassant_square < BOARD_SIZE) {
        int file = board->enpassant_square % 8;
        int rank = board->enpassant_square / 8;
        *ptr++ = 'a' + file;
        *ptr++ = '1' + rank;
    } else {
        *ptr++ = '-';
    }
    *ptr++ = ' ';

    // Halfmove clock
    ptr += sprintf(ptr, "%zu ", board->halfmove);

    // Fullmove number
    ptr += sprintf(ptr, "%zu", board->fullmove);

    // Null-terminate the buffer
    *ptr = '\0';
}
