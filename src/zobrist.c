#include "castro.h"

#include <ctype.h>
#include <stdio.h>

uint64_t zobrist_table[PIECE_TYPES][BOARD_SIZE][BOARD_SIZE];
uint64_t zobrist_castling[CASTLING_OPTIONS];
uint64_t zobrist_en_passant[BOARD_SIZE];
uint64_t zobrist_black_to_move;

// https://vigna.di.unimi.it/ftp/papers/xorshift.pdf > page 20
static uint64_t rand64()
{
    static uint64_t seed = 270803ULL;

    seed ^= seed >> 12;
    seed ^= seed << 25;
    seed ^= seed >> 27;

    return seed * 2685821657736338717ULL;
}

void castro_InitZobrist()
{
    // Initialize zobrist_table
    for (int piece = 0; piece < PIECE_TYPES; piece++) {
        for (int rank = 0; rank < BOARD_SIZE; rank++) {
            for (int file = 0; file < BOARD_SIZE; file++) {
                zobrist_table[piece][rank][file] = rand64();
            }
        }
    }

    // Initialize castling rights
    for (int i = 0; i < CASTLING_OPTIONS; i++) {
        zobrist_castling[i] = rand64();
    }

    // Initialize en passant files
    for (int file = 0; file < BOARD_SIZE; file++) {
        zobrist_en_passant[file] = rand64();
    }

    zobrist_black_to_move = rand64();
}


int castro_ZobristPieceToIndex(char piece)
{
    switch (piece) {
        case 'p': return 0;  // Black Pawn
        case 'n': return 1;  // Black Knight
        case 'b': return 2;  // Black Bishop
        case 'r': return 3;  // Black Rook
        case 'q': return 4;  // Black Queen
        case 'k': return 5;  // Black King
        case 'P': return 6;  // White Pawn
        case 'N': return 7;  // White Knight
        case 'B': return 8;  // White Bishop
        case 'R': return 9;  // White Rook
        case 'Q': return 10; // White Queen
        case 'K': return 11; // White King
        default:  return -1; 
    }
}

uint64_t castro_CalculateZobristHash(const Board* board)
{
    uint64_t hash = 0;

    for (int pieceIndex = 0; pieceIndex < 12; pieceIndex++) {
        uint64_t bitboard = board->bitboards[pieceIndex];
        
        while (bitboard) {
            int square = poplsb(&bitboard);
            
            hash ^= Random64[pieceIndex * 64 + square];  
        }
    }

    if (board->castling_rights & CASTLE_WHITE_KINGSIDE) hash ^= Random64[768];  // White Kingside
    if (board->castling_rights & CASTLE_WHITE_QUEENSIDE) hash ^= Random64[769];  // White Queenside
    if (board->castling_rights & CASTLE_BLACK_KINGSIDE) hash ^= Random64[770];  // Black Kingside
    if (board->castling_rights & CASTLE_BLACK_QUEENSIDE) hash ^= Random64[771];  // Black Queenside

    /*
       If the opponent has performed a double pawn push and there is now a 
       pawn next to it belonging to the player to move then "enpassant" is 
       the entry from RandomEnPassant whose offset is the file of the 
       pushed pawn (counted from 0(=a) to 7(=h)). If this does not apply 
       then enpassant=0.

       Note that this is different from the FEN standard. In the FEN standard 
       the presence of an "en passant target square" after a double pawn push 
       is unconditional.

       Also note that it is irrelevant if the potential en passant capturing 
       move is legal or not (examples where it would not be legal are when 
       the capturing pawn is pinned or when the double pawn push was a 
       discovered check).  
    */
    Square ep = board->enpassant_square;
    if (ep != SQUARE_NONE) {
        Square pawn = ep + ((!board->turn) ? NORTH : SOUTH);
        if (pawn < 0 || pawn >= 64) return 0;

        Square possibleAttackers[] = { pawn + 1, pawn - 1 };
        bool includeEP = false;

        for (size_t i = 0; i < 2; i++) {
            if (possibleAttackers[i] < 0 || possibleAttackers[i] >= 64) continue;

            Piece piece = castro_PieceAt(board, possibleAttackers[i]);
            if (piece.type != ' ' && piece.color == board->turn && tolower(piece.type) == 'p') {
                includeEP = true;
                break;
            }
        }

        if (includeEP) {
            hash ^= Random64[772 + castro_File(ep)];
        }
    }

    if (board->turn == COLOR_BLACK) {
        hash ^= Random64[780];
    }

    return hash;
}

uint64_t castro_CalculateZobristHashFen(const char* fen)
{
    Board b;
    castro_BoardInitFen(&b, fen);
    uint64_t hash = castro_CalculateZobristHash(&b);
    castro_BoardFree(&b);
    return hash;
}
