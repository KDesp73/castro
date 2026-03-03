/**
 * MIT License
 * 
 * Copyright (c) 2025 Konstantinos Despoinidis
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * ============================================================================
 *
⠀*⠀⠀⠀⠀⢠⣤⣤⡀⠀⠀⢀⣤⣤⣤⣤⡀⠀⠀⢀⣤⣤⡄⠀⠀⠀⠀⠀⠀
⠀*⠀⠀⠀⠀⢸⣿⣿⡇⠀⠀⢸⣿⣿⣿⣿⡇⠀⠀⢸⣿⣿⡇⠀⠀⠀⠀⠀⠀
⠀*⠀⠀⠀⠀⢸⣿⣿⣧⣤⣤⣼⣿⣿⣿⣿⣧⣤⣤⣼⣿⣿⡇⠀⠀⠀⠀⠀⠀
⠀*⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀
⠀*⠀⠀⠀⠀⠀⠀⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⠀⠀⠀⠀⠀⠀⠀⠀
⠀*⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀
⠀*⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀
⠀*⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀
⠀*⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀
⠀*⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀
⠀*⠀⠀⠀⠀⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⣤⠀⠀⠀⠀⠀⠀
⠀*⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀
⠀*⠀⠀⠀⠀⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉
 *
 * Castro is a chess move generation library written in C
 *
 * -Contents-
 * 1.  SQUARE
 * 2.  BITBOARD
 * 3.  HASHING
 * 4.  HISTORY
 * 5.  BOARD
 * 6.  ZOBRIST
 * 7.  MASKS
 * 8.  MOVE
 * 9.  PIECE
 * 10. NOTATION
 * 11. RESULT
 * 12. MOVEGEN
 * 13. PERFT
 * 14. POLYGLOT
 *
 * Written by Konstantinos Despoinidis <despoinidisk@gmail.com> (KDesp73)
 */

#ifndef CASTRO_H
#define CASTRO_H

// Includes
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// @module root

#define CASTRO_VERSION_MAJOR 0
#define CASTRO_VERSION_MINOR 3
#define CASTRO_VERSION_PATCH 0

#define CASTRO_VERSION_STRING "0.3.0"

#define CASTRO_VERSION_HEX ((CASTRO_VERSION_MAJOR * 10000) + (CASTRO_VERSION_MINOR * 100) + CASTRO_VERSION_PATCH)

// @function version
// @desc Fills in the provided pointers with the current version numbers.
// 
// @param major Pointer to an int to store the major version
// @param minor Pointer to an int to store the minor version
// @param patch Pointer to an int to store the patch version
static inline void castro_version(int* major, int* minor, int* patch)
{
    if (major) *major = CASTRO_VERSION_MAJOR;
    if (minor) *minor = CASTRO_VERSION_MINOR;
    if (patch) *patch = CASTRO_VERSION_PATCH;
}

/*------------------------------------.
// @module square
| ----------------------------------- |
| Bitboard representation of squares. |
| Square is an 8-bit integer ranging  |
| from 0-63.                          |
|                                     |
| We start counting from a8 and       |
| finish with h1                      |
`------------------------------------*/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// @type Square
// @desc Represents a square on the chessboard (0-63).
typedef uint8_t Square;

// @macro SQUARE_PRINT
// @desc Prints a square's name and index to stdout.
//
// @param square The square to print
//
// @example e4 = E4 28
#define SQUARE_PRINT(square)\
    do {\
        char __square_name__[3];\
        castro_SquareToName(__square_name__, square);\
        printf("%s = %s %d\n", #square, __square_name__, square);\
    } while(0)

// @macro COORDS
// @desc Macro to convert a square index to (row, col) coordinates.
//
// @param square Square index in [0, 63]
//
// @example board[COORDS(s)] = ...;` → expands to `board[(s)/8][(s)%8]
#define COORDS(square) (square) / 8][(square) % 8

// @function SquareToName
// @desc Converts a square index (0-63) to algebraic notation (e.g. "e4").
// 
// @param buffer [out] A 3-character buffer (e.g., `char name[3]`) to store the result.
// @param square [in] Square index from 0 to 63.
void castro_SquareToName(char buffer[3], Square square);

// @function NameToSquare
// @desc Converts algebraic notation (e.g. "e4") to a square index (0-63).
// 
// @param buffer A null-terminated string containing the square name.
//
// @returns Square index corresponding to the name, or SQUARE_NONE if invalid.
Square castro_NameToSquare(const char buffer[3]);

// @function Rank
// @desc Returns the rank (0–7) of a square index.
//
// @param square The square as an index
//
// @returns int The rank index
int castro_Rank(Square square);

// @function File
// @desc Returns the file (0–7) of a square index.
//
// @param square The square as an index
//
// @returns int The file index
int castro_File(Square square);

// @function IsSquareValid
// @desc Checks whether a square index is valid (0–63).
//
// @param square The square as an index
//
// @returns bool If the square is valid or not
bool castro_IsSquareValid(Square square);

// @function SquareFromCoords
// @desc Converts (rank, file) coordinates to a square index.
// 
// @param y Rank (0 = rank 1, 7 = rank 8)
// @param x File (0 = file A, 7 = file H)
//
// @returns Square index.
Square castro_SquareFromCoords(size_t y, size_t x);

// @function SquareFromName
// @desc Converts a square name (e.g. "d2") to an index.
// 
// @param name A 2-character string like "e4".
// @returns Square index or SQUARE_NONE.
Square castro_SquareFromName(const char* name);

// @macro SR
// @desc Flips a square vertically (used for mirror board logic).
//
// @param s The square to flip
// 
// @example SR(E2) returns the square index of E7.
#define SR(s) (7 - ((s) / 8)) * 8 + ((s) % 8)

/// Square indices (0–63), rank by rank from A1 to H8

// Rank 1
#define A1  0
#define B1  1
#define C1  2
#define D1  3
#define E1  4
#define F1  5
#define G1  6
#define H1  7

// Rank 2
#define A2  8
#define B2  9
#define C2 10
#define D2 11
#define E2 12
#define F2 13
#define G2 14
#define H2 15

// Rank 3
#define A3 16
#define B3 17
#define C3 18
#define D3 19
#define E3 20
#define F3 21
#define G3 22
#define H3 23

// Rank 4
#define A4 24
#define B4 25
#define C4 26
#define D4 27
#define E4 28
#define F4 29
#define G4 30
#define H4 31

// Rank 5
#define A5 32
#define B5 33
#define C5 34
#define D5 35
#define E5 36
#define F5 37
#define G5 38
#define H5 39

// Rank 6
#define A6 40
#define B6 41
#define C6 42
#define D6 43
#define E6 44
#define F6 45
#define G6 46
#define H6 47

// Rank 7
#define A7 48
#define B7 49
#define C7 50
#define D7 51
#define E7 52
#define F7 53
#define G7 54
#define H7 55

// Rank 8
#define A8 56
#define B8 57
#define C8 58
#define D8 59
#define E8 60
#define F8 61
#define G8 62
#define H8 63

// @const SQUARE_NONE
// @desc Special marker for an invalid or uninitialized square
#define SQUARE_NONE 64


/*------------------------------------.
// @module bitboard
|-------------------------------------|
| Low-level bitboard manipulation     |
`------------------------------------*/

// @type Bitboard
// @desc A 64-bit bitboard where each bit represents a square.
typedef uint64_t Bitboard;

// @enum Direction
// @desc Cardinal and diagonal directions for sliding piece movement or bitboard shifting.
typedef enum {
    NORTH       = 8,   ///< One rank up
    SOUTH       = -8,  ///< One rank down
    EAST        = 1,   ///< One file right
    WEST        = -1,  ///< One file left
    NORTH_EAST  = 9,   ///< Diagonal up-right
    NORTH_WEST  = 7,   ///< Diagonal up-left
    SOUTH_EAST  = -7,  ///< Diagonal down-right
    SOUTH_WEST  = -9   ///< Diagonal down-left
} Direction;

// @macro BB
// @desc Converts a square index to a bitboard with a single set bit.
//
// @param square The Square to covert into a bitboard
// 
// @returns Bitboard (0ULL if the square is SQUARE_NONE)
#define BB(square) (((square) == 64) ? 0ULL : 1ULL << (square))

// @function lsb
// @desc Returns the index of the least significant bit set (LSB).
// 
// @param b Input bitboard.
//
// @returns Square index (0–63) of the lowest bit set.
Square lsb(Bitboard b);

// @function msb
// @desc Returns the index of the most significant bit set (MSB).
// 
// @param b Input bitboard.
// @returns Square index (0–63) of the highest bit set.
Square msb(Bitboard b);

// @function shift
// @desc Shifts a bitboard in a specified direction.
// 
// @param b Input bitboard.
// @param D Direction to shift in.
//
// @returns Resulting shifted bitboard.
Bitboard shift(Bitboard b, Direction D);

// @function poplsb
// @desc Pops and returns the index of the least significant bit set. The bit is cleared from the input bitboard.
// 
// @param b Pointer to bitboard.
//
// @returns Square index that was popped.
Square poplsb(Bitboard* b);

// @function popcount
// @desc Counts the number of bits set in the bitboard.
// 
// @param bb Input bitboard.
//
// @returns Number of set bits.
int popcount(Bitboard bb);

// @function on
// @desc Sets the bit corresponding to the square in the bitboard.
//
// @param bitboard Pointer to the bitboard
// @param square The square we want to turn on
void on(Bitboard* bitboard, Square square);

// @function off
// @desc Clears the bit corresponding to the square in the bitboard.
// 
// @param bitboard Pointer to the bitboard
// @param square The square we want to turn off
void off(Bitboard* bitboard, Square square);


/*-----------------------------------------.
// @module pseudo
`-----------------------------------------*/

// @function PawnAttacks
// @desc Computes pseudo-legal pawn attacks.
// 
// @param pawn Square of pawn.
// @param enemySquares Bitboard of enemy pieces.
// @param color 0 = white, 1 = black.
//
// @returns The bitboard containing the pawn attacks
Bitboard castro_PawnAttacks(Square pawn, Bitboard enemySquares, uint8_t color);

// @function PawnPushes
// @desc Computes pseudo-legal pawn forward pushes.
// 
// @param pawn Square of pawn.
// @param emptySquares Bitboard of empty squares.
// @param color 0 = white, 1 = black.
//
// @returns The bitboard containing the pawn pushes
Bitboard castro_PawnPushes(Square pawn, Bitboard emptySquares, uint8_t color);

// @function PawnPromotions
// @desc Computes pawn promotions (non-capturing).
// 
// @param pawns Bitboard of pawns eligible to promote.
// @param emptySquares Bitboard of empty target squares.
// @param color 0 = white, 1 = black.
//
// @returns The bitboard containing the pawn promotions
Bitboard castro_PawnPromotions(Square pawns, Bitboard emptySquares, uint8_t color);

// @function PawnPromotionCaptures
// @desc Computes pawn promotion captures.
// 
// @param pawns Bitboard of pawns eligible to promote by capture.
// @param opponentPieces Bitboard of capturable opponent pieces.
// @param color 0 = white, 1 = black.
//
// @returns The bitboard containing the pawn promotion captures
Bitboard castro_PawnPromotionCaptures(Square pawns, Bitboard opponentPieces, uint8_t color);

// @function KnightAttacks
// @desc Computes knight attacks from a given square.
//
// @param knights Bitboard of knights
// @param emptySquares Bitboard with the empty squares on the board
// @param enemySquares Bitboard containing the enemy pieces positions
//
// @returns The bitboard containing the knight attacks
Bitboard castro_KnightAttacks(Square knights, Bitboard emptySquares, Bitboard enemySquares);

// @function KingAttacks
// @desc Computes king attacks from a given square.
//
// @param king Bitboard containing the king's position
// @param emptySquares Bitboard with the empty squares on the board
// @param enemySquares Bitboard containing the enemy pieces positions
//
// @returns The bitboard containing the king attacks
Bitboard castro_KingAttacks(Square king, Bitboard emptySquares, Bitboard enemySquares);

// @function BishopAttacksFromOccupancy
// @desc Computes bishop attacks based on an occupancy bitboard
// 
// @param square The square to calculate from
// @param occupancy The occupancy bitboard
//
// @returns The bitboard containing the diagonal attacks
Bitboard castro_BishopAttacksFromOccupancy(Square square, Bitboard occupancy);

// @function RookAttacksFromOccupancy
// @desc Computes rook attacks based on an occupancy bitboard
// 
// @param square The square to calculate from
// @param occupancy The occupancy bitboard
//
// @returns The bitboard containing the rook attacks
Bitboard castro_RookAttacksFromOccupancy(Square square, Bitboard occupancy);

// @function BishopAttacks
// @desc Computes bishop attacks using a sliding attack method.
//
// @param bishops The bishop's position
// @param emptySquares Bitboard with the empty squares on the board
// @param enemySquares Bitboard containing the enemy pieces positions
//
// @returns The bitboard containing the bishop attacks
Bitboard castro_BishopAttacks(Square bishop, Bitboard emptySquares, Bitboard enemySquares);

// @function RookAttacks
// @desc Computes rook attacks using a sliding attack method.
//
// @param rook The rook's position
// @param emptySquares Bitboard with the empty squares on the board
// @param enemySquares Bitboard containing the enemy pieces positions
//
// @returns The bitboard containing the rook attacks
Bitboard castro_RookAttacks(Square rook, Bitboard emptySquares, Bitboard enemySquares);

// @function QueenAttacks
// @desc Computes queen attacks as the union of rook and bishop attacks.
//
// @param queen The queen's position
// @param emptySquares Bitboard with the empty squares on the board
// @param enemySquares Bitboard containing the enemy pieces positions
//
// @returns The bitboard containing the queen attacks
Bitboard castro_QueenAttacks(Square queen, Bitboard emptySquares, Bitboard enemySquares);

// @function InitMagic
// @desc This function initializes the magic tables
void castro_InitMagic(void);

// @function BishopAttacksMagic
// @desc
Bitboard castro_BishopAttacksMagic(Square square, Bitboard occupancy);

// @function RookAttacksMagic
// @desc
Bitboard castro_RookAttacksMagic(Square square, Bitboard occupancy);


// @module kingsafety

// @function IsKingInCheck
// @desc Checks whether the king is in check.
// 
// @param kingPosition Bitboard with one bit set where the king is.
// @param enemyAttacks Bitboard of all enemy attacks.
//
// @returns bool Whether the king is in check or not
bool castro_IsKingInCheck(Bitboard kingPosition, Bitboard enemyAttacks);

Bitboard _PinnedPieces(Bitboard kingPosition, Bitboard slidingAttacks, Bitboard occupancy);


/*------------------------------------.
// @module hashing
|-------------------------------------|
| Position repetition tracking        |
`------------------------------------*/

// @type HashEntry
// @desc Represents a single hash entry (position and repetition count).
typedef struct {
    uint64_t hash;  ///< Zobrist hash of the position
    int count;      ///< Number of times this position has occurred
} HashEntry;

// @const HASH_TABLE_CAPACITY
// @desc Capacity of the repetition hash table (power of two for fast modulo).
#define HASH_TABLE_CAPACITY 4096

// @type HashTable
// @desc Tracks position repetition using Zobrist hashes.
// Uses open addressing (linear probing). Empty buckets have hash == 0.
typedef struct {
    uint64_t last_added;   ///< Last added hash (used when decrementing on unmake)
    HashEntry* entries;    ///< Buckets: index = hash & (capacity - 1), probe on collision
    size_t capacity;       ///< Number of buckets (power of two)
} HashTable;

// @function InitHashTable
// @desc Initializes a hash table from a FEN string.
// Parses the FEN, computes the initial Zobrist hash, and sets up the table.
// 
// @param table Pointer to an uninitialized HashTable
// @param starting_fen FEN string of the initial position
void castro_InitHashTable(HashTable* table, const char* starting_fen);

// @function InitHashTableHash
// @desc Initializes a hash table directly from a known Zobrist hash.
// 
// @param table Pointer to HashTable
// @param starting_hash Precomputed Zobrist hash of the position
void castro_InitHashTableHash(HashTable* table, uint64_t starting_hash);

// @function UpdateHashTable
// @desc Adds a new position hash or updates an existing entry.
// If the hash already exists, increments the count.
// 
// @param table Pointer to HashTable
// @param hash New Zobrist hash to insert
//
// @returns true if repetition >= 3 (e.g., threefold repetition), false otherwise
_Bool castro_UpdateHashTable(HashTable* table, uint64_t hash);

// @function HashTableDecrement
// @desc Decrements the repetition count for a position (used on unmake).
// Call with the hash that was last added before the move being undone.
//
// @param table Pointer to HashTable
// @param hash The hash of the position to decrement its repetition count
void castro_HashTableDecrement(HashTable* table, uint64_t hash);

// @function FreeHashTable
// @desc Frees all memory used by the hash table.
void castro_FreeHashTable(HashTable* table);


/*------------------------------------.
// @module history
|-------------------------------------|
| Move history and undo management    |
`------------------------------------*/

// @const MAX_MOVES
// @desc Maximum number of moves stored in history
#define MAX_MOVES (2 * 1024)

// @type Undo
// @desc Stores the necessary data to undo a move.
typedef struct {
    uint32_t move;       ///< Encoded move representation
    uint8_t castling;    ///< Castling rights before the move
    Square enpassant;    ///< En passant square before the move
    size_t fiftyMove;    ///< Fifty-move rule counter before the move
    char captured;       ///< Captured piece type (if any), 0 if none
} Undo;

// @const NULL_UNDO
// @desc Null undo object representing no previous move
#define NULL_UNDO (Undo){.move = NULL_MOVE}

// @function UndoPrint
// @desc Prints the contents of an Undo struct (for debugging).
//
// @param undo The undo struct to print
void castro_UndoPrint(Undo undo);

// @type History
// @desc Stores full game history for repetition detection and undo functionality.
typedef struct {
    HashTable positions;       ///< Hash table tracking seen positions
    Undo moves[MAX_MOVES];     ///< Stack of undo records
    size_t count;              ///< Number of moves in history
} History;

// @function HistoryRemove
// @desc Removes the last move from history (pop operation).
// Updates position table and count.
//
// @param history Pointer to a history struct
void castro_HistoryRemove(History* history);

// @function HistoryGetLast
// @desc Returns the most recent Undo record from history.
// 
// @param history Pointer to a history struct
//
// @returns Undo struct of the last move; undefined if history is empty.
Undo castro_HistoryGetLast(History history);


/*------------------------------------.
// @module board
|-------------------------------------|
| Game state and board representation |
`------------------------------------*/

/// Piece character definitions (FEN-compatible)
#define BLACK_ROOK   'r'
#define BLACK_KNIGHT 'n'
#define BLACK_BISHOP 'b'
#define BLACK_KING   'k'
#define BLACK_QUEEN  'q'
#define BLACK_PAWN   'p'

#define WHITE_ROOK   'R'
#define WHITE_KNIGHT 'N'
#define WHITE_BISHOP 'B'
#define WHITE_KING   'K'
#define WHITE_QUEEN  'Q'
#define WHITE_PAWN   'P'

// @enum PieceType
// @desc Piece type used for indexing and logic
typedef enum {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
} PieceType;

// @const EMPTY_SQUARE
// @desc Used to indicate no piece on a square
#define EMPTY_SQUARE ' '

// @const STARTING_FEN
// @desc Standard starting position in Forsyth-Edwards Notation (FEN)
#define STARTING_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

// @const BOARD_SIZE
// @desc The board dimensions
#define BOARD_SIZE   8

// @const PIECE_TYPES
// @desc The number of different pieces
#define PIECE_TYPES  12

/*---------------------------.
| Hybrid Representation      |
| `grid` is used for fast    |
| piece lookup               |
`---------------------------*/

// @type Board
// @desc Core board structure combining bitboards and grid for performance and simplicity.
typedef struct {
    Bitboard bitboards[PIECE_TYPES]; ///< One bitboard per piece type (white/black)
    char grid[8][8];                 ///< ASCII piece grid for quick access
    Bitboard white;                  ///< Cached: all white pieces
    Bitboard black;                  ///< Cached: all black pieces
    Bitboard empty;                  ///< Cached: empty squares (~(white|black))

    // Game state
    Square enpassant_square;         ///< En passant target square, if any
    bool turn;                       ///< true = white to move, false = black
    uint8_t castling_rights;         ///< Castling rights bitfield
    size_t halfmove;                 ///< Halfmove clock for 50-move rule
    size_t fullmove;                 ///< Fullmove number (starts at 1)

    History history;                 ///< Move history
    uint64_t hash;                   ///< Zobrist hash of current position
} Board;

// @function AddUndo
// @desc Records an undo step into the board's history.
// 
// @param board The board to update
// @param move The move to be undone later
//
// @returns true on success
bool castro_AddUndo(Board* board, uint32_t move);

// @function LoadLastUndo
// @desc Loads and removes the last undo record.
// 
// @param board The board to restore
//
// @returns Undo information for the last move
Undo castro_LoadLastUndo(Board* board);

// @const PIECES
// @desc All 12 supported pieces, as characters
#define PIECES "pnbrqkPNBRQK"

/// Bitboard index constants for each piece type
enum {
    INDEX_BLACK_PAWN,
    INDEX_BLACK_KNIGHT,
    INDEX_BLACK_BISHOP,
    INDEX_BLACK_ROOK,
    INDEX_BLACK_QUEEN,
    INDEX_BLACK_KING,
    INDEX_WHITE_PAWN,
    INDEX_WHITE_KNIGHT,
    INDEX_WHITE_BISHOP,
    INDEX_WHITE_ROOK,
    INDEX_WHITE_QUEEN,
    INDEX_WHITE_KING,
};

// Aliases for working with types generically (e.g. loops)
#define INDEX_PAWN   INDEX_BLACK_PAWN
#define INDEX_KNIGHT INDEX_BLACK_KNIGHT
#define INDEX_BISHOP INDEX_BLACK_BISHOP
#define INDEX_ROOK   INDEX_BLACK_ROOK
#define INDEX_QUEEN  INDEX_BLACK_QUEEN
#define INDEX_KING   INDEX_BLACK_KING

// @enum PieceColor
// @desc Color of pieces
typedef enum {
    COLOR_NONE = -1,
    COLOR_BLACK = 0,
    COLOR_WHITE
} PieceColor;

// @function PromotionToChar
// @desc Converts a promotion code to its corresponding character.
// 
// @param promotion Numeric code (0 = queen, 1 = rook, etc.)
//
// @returns Promotion piece character ('q', 'r', ...)
char castro_PromotionToChar(uint8_t promotion);

// @function CharToPromotion
// @desc Converts a promotion piece character to a numeric code.
// 
// @param promotion Piece character (e.g., 'q', 'n')
//
// @returns Numeric code
uint8_t castro_CharToPromotion(char promotion);

// @function BoardInitFen
// @desc Initializes a board from a FEN string.
// Must be freed with BoardFree().
//
// @param board Pointer to the board
// @param fen The FEN string
void castro_BoardInitFen(Board* board, const char* fen);

// @function BoardInitFenHeap
// @desc Heap-allocates and initializes a board from FEN.
// Must be freed with BoardFree().
// 
// @param fen The FEN string
//
// @returns Board* The pointer to the newly created board
Board* castro_BoardInitFenHeap(const char* fen);

// @function BoardFree
// @desc Frees heap-allocated board (from BoardInitFenHeap).
//
// @param board The pointer to the board
void castro_BoardFree(Board* board);

// @function BoardUpdateOccupancy
// @desc Recomputes and stores white, black, empty from bitboards.
// Call after any direct bitboard change.
//
// @param board The pointer to the board
void castro_BoardUpdateOccupancy(Board* board);

// @function GetWhite
// @desc Returns a bitboard of all white pieces.
//
// @param board Pointer to the board
//
// @returns Bitboard The bitboard containing the white occupancy
Bitboard castro_GetWhite(const Board* board);

// @function GetBlack
// @desc Returns a bitboard of all black pieces.
//
// @param board Pointer to the board
//
// @returns Bitboard The bitboard containing the black occupancy
Bitboard castro_GetBlack(const Board* board);

// @function GetEnemyColor
// @desc Returns a bitboard of all opponent pieces.
//
// @param board Pointer to the board
// @param us The color of the active player
//
// @returns Bitboard The bitboard containing the occupancy of the enemy
Bitboard castro_GetEnemyColor(const Board* board, PieceColor us);

// @function GetEnemy
// @desc Returns a bitboard of all enemy pieces (based on current turn).
//
// @param board Pointer to the board
//
// @returns Bitboard The bitboard containing the occupancy of the enemy
Bitboard castro_GetEnemy(const Board* board);

// @function GetEmpty
// @desc Returns a bitboard of all empty squares.
//
// @param board Pointer to the board
//
// @returns Bitboard The bitboard containing the empty squares
Bitboard castro_GetEmpty(const Board* board);

// @function CountPieces
// @desc Counts the number of a specific piece color/type on the board.
//
// @param board Pointer to the board
// @param color The color of the pieces to count
// @param type The type of piece to count
//
// @return int The final count
int castro_CountPieces(const Board* board, PieceColor color, PieceType type);

// @function HasCastlingRights
// @desc Checks if a board has certain castling rights.
//
// @param board Pointer to the board
// @param castling_rights The byte representing the castling rights
// 
// @returns int 0 means false and every positive integer means true
int castro_HasCastlingRights(const Board* board, uint8_t castling_rights);

// @function RevokeCastlingRights
// @desc Revokes specific castling rights from a board.
//
// @param board Pointer to the board
// @param castling_rights The byte contining the castle rights to revoke
void castro_RevokeCastlingRights(Board* board, uint8_t castling_rights);

// @function IsSquareAttacked
// @desc Checks if a square is attacked by a given color.
//
// @param board Pointer to the board
// @param square The square we want to check
// @param color The color of the side that might attack the square
//
// @returns bool Whether the square is attacked or not
bool castro_IsSquareAttacked(const Board* board, Square square, PieceColor color);

// @function IsSquareEmpty
// @desc Checks if a square is empty.
//
// @param board Pointer to the board
// @param square The square we want to check
//
// @returns bool Whether the square is empty or not
bool castro_IsSquareEmpty(const Board* board, Square square);

// @function castro_IsSquareOccupiedBy
// @desc Checks if a square is occupied by a given color.
//
// @param board Pointer to the board
// @param square The square we want to check
// @param color The color to check for
//
// @returns bool Whether the square is occupied or not
bool castro_IsSquareOccupiedBy(const Board* board, Square square, PieceColor color);

// @function castro_NumberOfPieces
// @desc Returns the number of pieces on the board for a given color.
//
// @param board Pointer to the board
// @param color The color to count
//
// @returns size_t The piece count
size_t castro_NumberOfPieces(const Board* board, PieceColor color);

// @function castro_IsInCheckColor
// @desc Checks if a specific color is in check.
//
// @param board Pointer to the board
// @param color The color to check
//
// @returns bool Whether the <color> king is in check
bool castro_IsInCheckColor(const Board* board, PieceColor color);

// @function castro_IsInCheck
// @desc Checks if the player currently to move is in check.
//
// @param board Pointer to the board
//
// @returns bool Whether the current player's king is in check
bool castro_IsInCheck(const Board* board);


// @function castro_BoardCopy
// @desc Returns a deep copy of the board.
//
// @param board Pointer to the source board
//
// @returns Board The copy as a stack variable
Board castro_BoardCopy(const Board* board);

//
// @module printing

// @function Uint32Print
// @desc Prints a 32-bit unsigned integer (e.g. in binary or hex).
// @param value The uint32_t number
void castro_Uint32Print(uint32_t value);

// @function Uint64Print
// @desc Prints a 64-bit unsigned integer (e.g. in binary or hex).
// @param value The uint64_t number
void castro_Uint64Print(uint64_t value);

// @function BitboardPrint
// @desc Prints a visual representation of a bitboard.
// Useful for debugging. Marks set bits on an 8x8 grid
// @param bitboard The bitboard to print
void castro_BitboardPrint(Bitboard bitboard);

// @function castro_BoardPrintSquares
// @desc Prints a list of squares (e.g. legal moves) on the board.
//
// @param board Pointer to the board
// @param squares Array of squares to print
// @param count Number of squares in the array
void castro_BoardPrintSquares(const Board* board, Square* squares, size_t count);

// @function castro_BoardPrintBitboard
// @desc Highlights a bitboard on the board (used for debugging).
//
// @param board Pointer to the board
// @param highlight The bitboard to overlay/highlight
void castro_BoardPrintBitboard(const Board* board, Bitboard highlight);

// @function castro_BoardPrint
// @desc Prints the board with a list of highlighted squares.
//
// @param board Pointer to the board
// @param first The first square to highlight (followed by variadic list)
void castro_BoardPrint(const Board* board, Square first, ...);

// @function castro_BoardPrintBitboards
// @desc Prints all bitboards in the board structure (for debugging).
//
// @param board The board structure
void castro_BoardPrintBitboards(Board board);

// @function castro_BoardPrintGrid
// @desc Prints the character grid of the board.
//
// @param board Pointer to the board
void castro_BoardPrintGrid(const Board* board);


/*------------------------------------.
// @module zobrist
|-------------------------------------|
| Zobrist hashing for board states    |
`------------------------------------*/

// @const CASTLING_OPTIONS
// @desc Total number of castling rights encoded (K, Q, k, q)
#define CASTLING_OPTIONS 4

// @var zobrist_table
// @desc Zobrist random numbers for each piece on each square.
// Dimensions:
// - PIECE_TYPES: 12 (black/white * 6 types)
// - BOARD_SIZE: 8x8 squares
// Indexed as [piece][rank][file]
extern uint64_t zobrist_table[PIECE_TYPES][BOARD_SIZE][BOARD_SIZE] __attribute__((unused));

// @var zobrist_castling
// @desc Zobrist keys for each of the 4 castling rights (K, Q, k, q)
extern uint64_t zobrist_castling[CASTLING_OPTIONS] __attribute__((unused));

// @var zobrist_en_passant
// @desc Zobrist keys for each en passant file (a-h)
extern uint64_t zobrist_en_passant[BOARD_SIZE] __attribute__((unused));

// @var zobrist_black_to_move
// @desc Zobrist key to represent "black to move"
extern uint64_t zobrist_black_to_move __attribute__((unused));

// @var InitZobrist
// @desc Initializes the Zobrist tables.
void castro_InitZobrist(void);

// @function CalculateZobristHash
// @desc Calculates the Zobrist hash of a board.
// This includes:
// - Pieces on the board
// - Side to move
// - Castling rights
// - En passant square
// 
// @param board Pointer to the Board structure
//
// @returns 64-bit Zobrist hash
uint64_t castro_CalculateZobristHash(const Board* board);

// @function CalculateZobristHashFen
// @desc Convenience function to calculate a Zobrist hash directly from a FEN string.
// 
// @param fen Forsyth-Edwards Notation string
//
// @returns 64-bit Zobrist hash
uint64_t castro_CalculateZobristHashFen(const char* fen);

// @function ZobristPieceToIndex
// @desc Translates piece as a character to expected zobrist index
//
// @param piece The piece as a character (example: white queen -> 'Q')
//
// @returns int The index
int castro_ZobristPieceToIndex(char piece);

// @module masks

// @type Bitboard
// @desc A 64-bit unsigned integer representing the squares of the chessboard. Each bit corresponds to a square (0-63), where a set bit (1) indicates the presence of a piece or a targeted square, and a cleared bit (0) indicates its absence. This allows for high-performance board manipulation using bitwise operators.
typedef uint64_t Bitboard;

#define FILE_A  0x0101010101010101ULL  // File A (a1, a2, ..., a8)
#define FILE_B  0x0202020202020202ULL  // File B (b1, b2, ..., b8)
#define FILE_C  0x0404040404040404ULL  // File C (c1, c2, ..., c8)
#define FILE_D  0x0808080808080808ULL  // File D (d1, d2, ..., d8)
#define FILE_E  0x1010101010101010ULL  // File E (e1, e2, ..., e8)
#define FILE_F  0x2020202020202020ULL  // File F (f1, f2, ..., f8)
#define FILE_G  0x4040404040404040ULL  // File G (g1, g2, ..., g8)
#define FILE_H  0x8080808080808080ULL  // File H (h1, h2, ..., h8)

#define RANK_1  0x00000000000000FFULL  // Rank 1 (a1-h1)
#define RANK_2  0x000000000000FF00ULL  // Rank 2 (a2-h2)
#define RANK_3  0x0000000000FF0000ULL  // Rank 3 (a3-h3)
#define RANK_4  0x00000000FF000000ULL  // Rank 4 (a4-h4)
#define RANK_5  0x000000FF00000000ULL  // Rank 5 (a5-h5)
#define RANK_6  0x0000FF0000000000ULL  // Rank 6 (a6-h6)
#define RANK_7  0x00FF000000000000ULL  // Rank 7 (a7-h7)
#define RANK_8  0xFF00000000000000ULL  // Rank 8 (a8-h8)

#define WHITE_KINGSIDE_CASTLE_EMPTY  0x60ULL  // {f1, g1}
#define WHITE_QUEENSIDE_CASTLE_EMPTY 0xeULL   // {d1, c1, b1}
#define BLACK_KINGSIDE_CASTLE_EMPTY  0x6000000000000000ULL  // {f8, g8}
#define BLACK_QUEENSIDE_CASTLE_EMPTY 0xe00000000000000ULL   // {d8, c8, b8}

#define WHITE_KINGSIDE_ATTACKS  0x70ULL // {e1, f1, g1}
#define WHITE_QUEENSIDE_ATTACKS 0x1cULL // {e1, d1, c1}
#define BLACK_KINGSIDE_ATTACKS  0x7000000000000000ULL // {e8, f8, g8}
#define BLACK_QUEENSIDE_ATTACKS 0x1c00000000000000ULL // {e8, d8, c8}

#define BB_ANY_MATCH(bb1, bb2) ((bb1) & (bb2))
#define BB_NO_MATCH(bb1, bb2) !(BB_ANY_MATCH(bb1, bb2))
#define BB_ALL_MATCH(bb1, bb2) (((bb1) & (bb2)) == (bb1))

static Bitboard DIAGONAL_MASKS[64]      __attribute__((unused));
static Bitboard ANTI_DIAGONAL_MASKS[64] __attribute__((unused));
static Bitboard HORIZONTAL_MASKS[64]    __attribute__((unused));
static Bitboard VERTICAL_MASKS[64]      __attribute__((unused));

static Bitboard PAWN_PUSH_MASKS[2][64]             __attribute__((unused));
static Bitboard PAWN_DOUBLE_PUSH_MASKS[2][64]      __attribute__((unused));
static Bitboard PAWN_ATTACK_MASKS[2][64]           __attribute__((unused));
static Bitboard PAWN_PROMOTION_MASKS[2][64]        __attribute__((unused));
static Bitboard PAWN_PROMOTION_ATTACK_MASKS[2][64] __attribute__((unused));

static Bitboard KNIGHT_MOVE_MASKS[64] __attribute__((unused));
static Bitboard BISHOP_MOVE_MASKS[64] __attribute__((unused));
static Bitboard ROOK_MOVE_MASKS[64]   __attribute__((unused));
static Bitboard QUEEN_MOVE_MASKS[64]  __attribute__((unused));
static Bitboard KING_MOVE_MASKS[64]   __attribute__((unused));

typedef enum {
    DIAGONAL,
    VERTICAL,
    HORIZONTAL
} RayType;

// @function InitMasks
// @desc Initializes all masks
void castro_InitMasks(void);

// @function GeneralOccupancy
// @desc Computes the general occupancy into a bitboard
//
// @param whitePieces The bitboard containing the positions of the white pieces
// @param blackPieces The bitboard containing the positions of the black pieces
//
// @returns Bitboard The combined bitboard
Bitboard castro_GeneralOccupancy(Bitboard whitePieces, Bitboard blackPieces);

// @function BlockerMasks
// @desc Calculates the blocker mask for a sliding piece
//
// @param slidingPiece The bitboard with the sliding piece's square bit turned on
// @param occupancy The general occupancy bitboard
//
// @returns Bitboard The blocker mask
Bitboard castro_BlockerMasks(Bitboard slidingPiece, Bitboard occupancy);

/*** Ray Masks ***/

// @function ComputeDiagonalMask
// @desc Generates a mask for the diagonal passing through the given square
// @param square The target square index
// @returns Bitboard The resulting diagonal mask
Bitboard castro_ComputeDiagonalMask(Square square);

// @function ComputeAntiDiagonalMask
// @desc Generates a mask for the anti-diagonal passing through the given square
// @param square The target square index
// @returns Bitboard The resulting anti-diagonal mask
Bitboard castro_ComputeAntiDiagonalMask(Square square);

// @function ComputeHorizontalMask
// @desc Generates a mask for the rank (row) of the given square
// @param square The target square index
// @returns Bitboard The resulting rank mask
Bitboard castro_ComputeHorizontalMask(Square square);

// @function ComputeVerticalMask
// @desc Generates a mask for the file (column) of the given square
// @param square The target square index
// @returns Bitboard The resulting file mask
Bitboard castro_ComputeVerticalMask(Square square);

// @function DiagonalMask
// @desc Retrieves the pre-computed diagonal mask for a square
// @param square The target square index
// @returns Bitboard The cached diagonal mask
Bitboard castro_DiagonalMask(Square square);

// @function AntiDiagonalMask
// @desc Retrieves the pre-computed anti-diagonal mask for a square
// @param square The target square index
// @returns Bitboard The cached anti-diagonal mask
Bitboard castro_AntiDiagonalMask(Square square);

// @function HorizontalMask
// @desc Retrieves the pre-computed horizontal mask for a square
// @param square The target square index
// @returns Bitboard The cached horizontal mask
Bitboard castro_HorizontalMask(Square square);

// @function VerticalMask
// @desc Retrieves the pre-computed vertical mask for a square
// @param square The target square index
// @returns Bitboard The cached vertical mask
Bitboard castro_VerticalMask(Square square);

/*** Piece Masks ***/

// @function ComputePawnPushMask
// @desc Calculates the single-square forward push for a pawn
// @param square The starting square of the pawn
// @param color The color of the pawn (determines direction)
// @returns Bitboard The resulting push mask
Bitboard castro_ComputePawnPushMask(Square square, PieceColor color);

// @function ComputePawnDoublePushMask
// @desc Calculates the two-square initial push for a pawn
// @param square The starting square of the pawn
// @param color The color of the pawn
// @returns Bitboard The resulting double-push mask
Bitboard castro_ComputePawnDoublePushMask(Square square, PieceColor color);

// @function ComputePawnPromotionMask
// @desc Generates a mask for squares where a pawn push results in promotion
// @param square The starting square of the pawn
// @param color The color of the pawn
// @returns Bitboard The resulting promotion mask
Bitboard castro_ComputePawnPromotionMask(Square square, PieceColor color);

// @function ComputePawnPromotionAttackMask
// @desc Generates a mask for diagonal attacks that result in a promotion
// @param square The starting square of the pawn
// @param color The color of the pawn
// @returns Bitboard The resulting promotion attack mask
Bitboard castro_ComputePawnPromotionAttackMask(Square square, PieceColor color);

// @function ComputePawnAttackMask
// @desc Calculates standard diagonal attack squares for a pawn
// @param square The starting square of the pawn
// @param color The color of the pawn
// @returns Bitboard The resulting attack mask
Bitboard castro_ComputePawnAttackMask(Square square, PieceColor color);

// @function ComputeKnightMoveMask
// @desc Generates all possible L-shaped jumps for a knight on a given square
// @param square The starting square of the knight
// @returns Bitboard The resulting knight move mask
Bitboard castro_ComputeKnightMoveMask(Square square);

// @function ComputeBishopMoveMask
// @desc Calculates all diagonal sliding moves for a bishop (ignoring blockers)
// @param square The starting square of the bishop
// @returns Bitboard The resulting bishop move mask
Bitboard castro_ComputeBishopMoveMask(Square square);

// @function ComputeRookMoveMask
// @desc Calculates all horizontal and vertical sliding moves for a rook (ignoring blockers)
// @param square The starting square of the rook
// @returns Bitboard The resulting rook move mask
Bitboard castro_ComputeRookMoveMask(Square square);

// @function ComputeQueenMoveMask
// @desc Calculates the union of rook and bishop moves for a queen
// @param square The starting square of the queen
// @returns Bitboard The resulting queen move mask
Bitboard castro_ComputeQueenMoveMask(Square square);

// @function ComputeKingMoveMask
// @desc Generates a mask for all adjacent squares reachable by a king
// @param square The starting square of the king
// @returns Bitboard The resulting king move mask
Bitboard castro_ComputeKingMoveMask(Square square);

// @function PawnPushMask
// @desc Retrieves pre-computed single push mask
// @param square starting square
// @param color piece color
// @returns Bitboard cached mask
Bitboard castro_PawnPushMask(Square square, PieceColor color);

// @function PawnDoublePushMask
// @desc Retrieves pre-computed double push mask
// @param square starting square
// @param color piece color
// @returns Bitboard cached mask
Bitboard castro_PawnDoublePushMask(Square square, PieceColor color);

// @function PawnPromotionMask
// @desc Retrieves pre-computed promotion mask
// @param square starting square
// @param color piece color
// @returns Bitboard cached mask
Bitboard castro_PawnPromotionMask(Square square, PieceColor color);

// @function PawnPromotionAttackMask
// @desc Retrieves pre-computed promotion attack mask
// @param square starting square
// @param color piece color
// @returns Bitboard cached mask
Bitboard castro_PawnPromotionAttackMask(Square square, PieceColor color);

// @function PawnAttackMask
// @desc Retrieves pre-computed pawn attack mask
// @param square starting square
// @param color piece color
// @returns Bitboard cached mask
Bitboard castro_PawnAttackMask(Square square, PieceColor color);

// @function KnightMoveMask
// @desc Retrieves pre-computed knight move mask
// @param square starting square
// @returns Bitboard cached mask
Bitboard castro_KnightMoveMask(Square square);

// @function BishopMoveMask
// @desc Retrieves pre-computed bishop move mask
// @param square starting square
// @returns Bitboard cached mask
Bitboard castro_BishopMoveMask(Square square);

// @function RookMoveMask
// @desc Retrieves pre-computed rook move mask
// @param square starting square
// @returns Bitboard cached mask
Bitboard castro_RookMoveMask(Square square);

// @function QueenMoveMask
// @desc Retrieves pre-computed queen move mask
// @param square starting square
// @returns Bitboard cached mask
Bitboard castro_QueenMoveMask(Square square);

// @function KingMoveMask
// @desc Retrieves pre-computed king move mask
// @param square starting square
// @returns Bitboard cached mask
Bitboard castro_KingMoveMask(Square square);

/*--------------------------------------.
// @module move
| ------------------------------------- |
| Moves are 32-bit integers             |
| where bits:                           |
| - 0-5 represent the source            |
| - 6-11 represent the destination      |
| - 12-15 represent the promotion piece |
| - 16-21 represent the flags           |
`--------------------------------------*/

// TODO: Make Move 16bits if an application is not found for the flags

// @enum Flag
// @desc Flags representing special move types.
typedef enum {
    FLAG_NORMAL = 0,              ///< Regular move
    FLAG_CASTLING,                ///< Castling move
    FLAG_ENPASSANT,               ///< En passant capture
    FLAG_PAWN_DOUBLE_MOVE,        ///< Initial two-square pawn push
    FLAG_PROMOTION,               ///< Promotion without capture
    FLAG_PROMOTION_WITH_CAPTURE   ///< Promotion with capture
} Flag;

// @enum Promotion
// @desc Types of piece promotions.
typedef enum {
    PROMOTION_NONE = 0,           ///< No promotion
    PROMOTION_KNIGHT,
    PROMOTION_BISHOP,
    PROMOTION_ROOK,
    PROMOTION_QUEEN
} Promotion;

// @enum Castling bytes
// @desc Bit flags representing castling rights.
enum {
    CASTLE_WHITE_KINGSIDE  = 0b0001,
    CASTLE_WHITE_QUEENSIDE = 0b0010,
    CASTLE_BLACK_KINGSIDE  = 0b0100,
    CASTLE_BLACK_QUEENSIDE = 0b1000
};

// @type NullMoveState
// @desc Used to store minimal board state when making a null move.
typedef struct {
    PieceColor turn;
    int halfmoveClock;
    int fullmoveNumber;
    Square epSquare;
} NullMoveState;

// @var nullState
// @desc Stores the state of the board before a null move is made
extern NullMoveState nullState;

// @type Move
// @desc Encoded move type (bitfield). Format:
// - bits 0–5:   from square
// - bits 6–11:  to square
// - bits 12–14: promotion type
// - bits 15–17: move flag
typedef uint32_t Move;

// @const NULL_MOVE
// @desc Special constant representing no move
#define NULL_MOVE ((Move) 0)

// @const MOVES_CAPACITY
// @desc Max number of moves in a move list
#define MOVES_CAPACITY 256

// @type Moves
// @desc Represents a dynamic list of moves.
typedef struct {
    Move list[MOVES_CAPACITY];
    size_t count;
} Moves;

// @const NO_MOVES
// @desc Empty move list constant
#define NO_MOVES ((Moves){.count = 0})

// @function MovesAppend
// @desc Appends a move to a move list.
// @param moves Pointer to the Moves list structure.
// @param move The specific move to add to the list.
void castro_MovesAppend(Moves* moves, Move move);

// @function MovesAppendList
// @desc Appends one move list to another.
// @param dest Pointer to the destination Moves list.
// @param src The source Moves list to copy from.
void castro_MovesAppendList(Moves* dest, Moves src);

// @function MovesCombine
// @desc Combines two move lists into a new one.
// @param m1 The first move list.
// @param m2 The second move list.
// @returns Moves A new move list containing elements from both m1 and m2.
Moves castro_MovesCombine(Moves m1, Moves m2);

// @function MakeUndo
// @desc Creates an Undo struct representing a move played on a board.
// @param board Pointer to the current board state.
// @param move The move intended to be played.
// @returns Undo A structure containing all information needed to revert the move.
Undo castro_MakeUndo(const Board* board, Move move);

/*-----------------------------------------------.
| Piece-specific movement offsets (mailbox 0x88) |
`-----------------------------------------------*/

#define KNIGHT_OFFSETS_COUNT 8
static const int KNIGHT_OFFSETS[] = {
    -17, -15, -10, -6, 6, 10, 15, 17
};

#define BISHOP_OFFSETS_COUNT 4
static const int BISHOP_OFFSETS[] = {
    -9, -7, 7, 9
};

#define ROOK_OFFSETS_COUNT 4
static const int ROOK_OFFSETS[] = {
    -8, -1, 1, 8
};

#define KING_OFFSETS_COUNT 8
static const int KING_OFFSETS[] = {
    -9, -8, -7, -1, 1, 7, 8, 9
};

// @macro MOVE_DECODE
// @desc Decodes a move into its components (used inside a scope).
#define MOVE_DECODE(move) \
    Square src, dst; \
    uint8_t promotion, flag; \
    castro_MoveDecode(move, &src, &dst, &promotion, &flag)

/*-----------------------------.
| Move encoding/decoding/util  |
`-----------------------------*/

// @function MoveIsValid
// @desc Checks whether a move is legal and does not leave the king in check.
// @param board Pointer to the constant board state.
// @param move The move to validate.
// @param color The color of the player making the move.
// @returns _Bool True if the move is legal.
_Bool castro_MoveIsValid(const Board* board, Move move, PieceColor color);

// @function MoveIsCapture
// @desc Returns true if the move captures a piece (or en passant).
// @param board Pointer to the constant board state.
// @param move The move to check.
// @returns _Bool True if the target square contains an opponent piece.
_Bool castro_MoveIsCapture(const Board* board, Move move);

// @function MoveGivesCheck
// @desc Returns true if the move gives check. Temporarily modifies the board (make/unmake).
// @param board Pointer to the board state (will be modified and restored).
// @param move The move to test.
// @returns _Bool True if the move results in the opponent's king being under attack.
_Bool castro_MoveGivesCheck(Board* board, Move move);

// @function PieceValueFromType
// @desc Piece value for MVV-LVA (pawn=1, knight/bishop=3, rook=5, queen=9, king=0).
// @param piece_type The character representation of the piece.
// @returns int The relative value used for move ordering.
int castro_PieceValueFromType(char piece_type);

// @function OrderLegalMoves
// @desc Reorders a legal move list for search: hash move first, then captures (MVV-LVA), killers, then checks, then quiet moves.
// @param board Pointer to the current board.
// @param moves Pointer to the list of moves to be sorted in place.
// @param hash_move The move retrieved from the transposition table (use NULL_MOVE if none).
// @param killer0 The first killer move for the current ply.
// @param killer1 The second killer move for the current ply.
// @param score_checks Boolean to determine if check detection should be part of the ordering.
void castro_OrderLegalMoves(Board* board, Moves* moves, Move hash_move, Move killer0, Move killer1, bool score_checks);

// @function MoveEncode
// @desc Encodes a move from components into a 32-bit integer.
// @param from The starting square index.
// @param to The destination square index.
// @param promotion The piece type to promote to (if applicable).
// @param flag Metadata flags (capture, double push, etc.).
// @returns Move The encoded 32-bit move.
Move castro_MoveEncode(Square from, Square to, uint8_t promotion, uint8_t flag);

// @function MoveEncodeNames
// @desc Encodes a move from algebraic names ("e2", "e4", etc.).
// @param from String representing the starting square.
// @param to String representing the destination square.
// @param promotion Promotion type index.
// @param flag Move flag index.
// @returns Move The encoded 32-bit move.
Move castro_MoveEncodeNames(const char* from, const char* to, uint8_t promotion, uint8_t flag);

// @function MoveDecode
// @desc Decodes a move into from-square, to-square, promotion, and flag.
// @param move The encoded 32-bit move.
// @param from Pointer to store the extracted starting square.
// @param to Pointer to store the extracted destination square.
// @param promotion Pointer to store the extracted promotion type.
// @param flag Pointer to store the extracted flag.
void castro_MoveDecode(Move move, Square* from, Square* to, uint8_t* promotion, uint8_t* flag);

// @function MoveSetFlag
// @desc Sets the move flag field in an existing move.
// @param move Pointer to the move to modify.
// @param flag The flag value to set.
void castro_MoveSetFlag(Move* move, Flag flag);

// @function MoveSetPromotion
// @desc Sets the promotion field in an existing move.
// @param move Pointer to the move to modify.
// @param promotion The promotion piece type.
void castro_MoveSetPromotion(Move* move, Promotion promotion);

/*-------------------------------.
| Bitboard Move Application API  |
`-------------------------------*/

// @function DoMove
// @desc Applies a move on a bitboard by updating the piece positions.
// @param current Pointer to the specific bitboard (e.g., WhitePawns) to be updated.
// @param move The encoded move containing source and destination squares.
// @returns Bitboard The updated bitboard state after the move is applied.
Bitboard castro_DoMove(Bitboard* current, Move move);

// @function UndoMove
// @desc Undoes a move on a bitboard, reverting the piece to its previous position.
// @param current Pointer to the bitboard to be reverted.
// @param move The encoded move that was previously applied.
// @returns Bitboard The restored bitboard state.
Bitboard castro_UndoMove(Bitboard* current, Move move);

/*-----------------------------.
| Full board move application  |
`-----------------------------*/

// @function MakeMove
// @desc Makes a move and updates board state accordingly. 
// Handles piece movement, captures, and state updates (castling rights, en passant, etc.).
// @param board Pointer to the board state to modify.
// @param move The encoded move to execute.
// @returns bool True if the move was successfully made (e.g., if it didn't leave the king in check).
bool castro_MakeMove(Board* board, Move move);

// @function UnmakeMove
// @desc Unmakes the last move and restores previous board state using stored history.
// @param board Pointer to the board state to restore.
void castro_UnmakeMove(Board* board);

// @function MakeNullMove
// @desc Performs a null move (swapping sides without moving a piece). 
// Used primarily in search algorithms like Null Move Pruning.
// @param board Pointer to the board state to modify.
void castro_MakeNullMove(Board* board);

// @function UnmakeNullMove
// @desc Reverts a null move and restores the original side to move and state.
// @param board Pointer to the board state to restore.
void castro_UnmakeNullMove(Board* board);

/*-------------------------------.
| Move-type specific helpers     |
`-------------------------------*/

// @function Castle
// @desc Executes a castling move, updating both the king and the rook positions on the bitboards.
// @param board Pointer to the board state to modify.
// @param move The encoded castling move.
// @returns bool True if the castling was successfully executed.
bool castro_Castle(Board* board, Move move);

// @function IsCastle
// @desc Checks if a move is a castling move based on piece type and move flags.
// @param board Pointer to the current board state.
// @param move Pointer to the move to check.
// @returns bool True if the move is a king-side or queen-side castle.
bool castro_IsCastle(const Board* board, Move* move);

// @function Enpassant
// @desc Executes an en passant capture, removing the opponent's pawn and moving the current pawn.
// @param board Pointer to the board state.
// @param move The encoded en passant move.
// @returns bool True if the capture was successful.
bool castro_Enpassant(Board* board, Move move);

// @function IsEnpassant
// @desc Checks if a move is an en passant capture by checking the target square and pawn flags.
// @param board Pointer to the board state.
// @param move Pointer to the move to check.
// @returns bool True if the move satisfies en passant requirements.
bool castro_IsEnpassant(const Board* board, Move* move);

// @function IsDoublePawnPush
// @desc Checks if a move is a two-square pawn advance from the starting rank.
// @param board Pointer to the board state.
// @param move The move to check.
// @returns bool True if the pawn moved two squares forward.
bool castro_IsDoublePawnPush(Board* board, Move move);

// @function IsPromotion
// @desc Checks if a move is a pawn promotion.
// @param board Pointer to the board state.
// @param move Pointer to the move to check.
// @returns bool True if the move results in a pawn reaching the last rank.
bool castro_IsPromotion(Board* board, Move* move);

// @function IsCapture
// @desc Checks if a move is a capture (regular or en passant).
// @param board Pointer to the board state.
// @param move The move to check.
// @returns bool True if the move results in an opponent's piece being removed.
bool castro_IsCapture(const Board* board, Move move);

// @function IsInCheckAfterMove
// @desc Simulates a move to check if it leaves the friendly king in check (illegal move detection).
// @param board Pointer to the board state.
// @param move The move to simulate.
// @returns bool True if the king is under attack after the move.
bool castro_IsInCheckAfterMove(Board *board, Move move);

// @function MoveMake
// @desc Makes a move with full legality rules, ensuring the move is valid and updates all board state.
// @param board Pointer to the board state.
// @param move The move to execute.
// @returns _Bool True if the move was legal and executed successfully.
_Bool castro_MoveMake(Board* board, Move move);

// @function MoveFreely
// @desc Applies a move directly to the bitboards, ignoring turn-order or check legality.
// @param board Pointer to the board state.
// @param move The move to apply.
// @param color The color of the piece being moved.
void castro_MoveFreely(Board* board, Move move, PieceColor color);

// @function MovePrint
// @desc Prints a move to stdout in algebraic format (e.g., "e2e4").
// @param move The move to print.
void castro_MovePrint(Move move);

// @function StringToMove
// @desc Converts an algebraic string (e.g., "e2e4") to an encoded 32-bit Move integer.
// @param str The source algebraic string.
// @returns Move The resulting encoded move.
Move castro_StringToMove(const char* str);

// @function MoveToString
// @desc Converts a move to a string in algebraic format (e.g., "e2e4").
// @param move The move to convert.
// @param buffer Pointer to the character buffer to store the result.
void castro_MoveToString(Move move, char* buffer);

/*--------------------.
| Move comparisons    |
`--------------------*/

// @function MoveCmp
// @desc Compares two moves for equality by checking only the source and destination squares, ignoring internal metadata like flags or search scores.
// @param m1 The first move to compare.
// @param m2 The second move to compare.
// @returns bool True if the squares match.
bool castro_MoveCmp(Move m1, Move m2);

// @function MoveCmpStrict
// @desc Compares two moves strictly, ensuring that the squares, promotion piece, and all metadata flags are identical.
// @param m1 The first move to compare.
// @param m2 The second move to compare.
// @returns bool True if every bit of the encoded moves is identical.
bool castro_MoveCmpStrict(Move m1, Move m2);

/*--------------------.
| Move field getters  |
`--------------------*/

// @function GetFrom
// @desc Extracts and returns the source (starting) square index from an encoded move.
// @param move The encoded 32-bit move.
// @returns Square The source square index (0-63).
Square castro_GetFrom(Move move);

// @function GetTo
// @desc Extracts and returns the destination (target) square index from an encoded move.
// @param move The encoded 32-bit move.
// @returns Square The destination square index (0-63).
Square castro_GetTo(Move move);

// @function GetPromotion
// @desc Retrieves the promotion piece type from the move bitfield.
// @param move The encoded 32-bit move.
// @returns uint8_t The promotion piece identifier (e.g., Knight, Bishop, Rook, Queen).
uint8_t castro_GetPromotion(Move move);

// @function GetFlag
// @desc Extracts the metadata flag associated with the move (e.g., double pawn push, en passant, or castling).
// @param move The encoded 32-bit move.
// @returns uint8_t The move flag value.
uint8_t castro_GetFlag(Move move);

/*----------------------------.
| Board state update helpers  |
`----------------------------*/

// @function UpdateHalfmove
// @desc Updates the 50-move counter (halfmove clock). The counter is reset if a pawn is moved or a capture occurs; otherwise, it is incremented.
// @param board Pointer to the board state to update.
// @param move The move being executed.
// @param piece_count_before Total piece count on the board before the move.
// @param piece_count_after Total piece count on the board after the move.
// @param piece The character type of the piece that was moved.
void castro_UpdateHalfmove(Board* board, Move move, size_t piece_count_before, size_t piece_count_after, char piece);

// @function UpdateCastlingRights
// @desc Updates the castling rights bitmask after a move. Rights are lost if the king or a rook moves, or if a rook is captured on its starting square.
// @param board Pointer to the current board.
// @param move The move that may affect castling rights.
// @returns uint8_t The updated castling rights bitmask.
uint8_t castro_UpdateCastlingRights(Board* board, Move move);

// @function UpdateEnpassantSquare
// @desc Updates the en passant target square. This is set if a pawn makes a double-square push, otherwise it is cleared.
// @param board Pointer to the current board.
// @param move The move being executed.
// @returns Square The square index of the new en passant target, or a null-square constant if none.
Square castro_UpdateEnpassantSquare(Board* board, Move move);

/*-------------------------.
| Bitboard-Move conversion |
`-------------------------*/

// @function BitboardToMoves
// @desc Iterates through all set bits in a destination bitboard and appends a corresponding move to a Moves list, using the provided source square.
// @param bitboard The bitboard representing valid target squares.
// @param from The starting square shared by all generated moves.
// @returns Moves A move list containing all individual moves derived from the bitboard.
Moves castro_BitboardToMoves(Bitboard bitboard, Square from);

// @function MovesToBitboard
// @desc Aggregates the destination squares of all moves in a list into a single bitboard representation.
// @param moves The list of moves to process.
// @returns Bitboard A bitboard where each bit is set if it is a destination square in the move list.
Bitboard castro_MovesToBitboard(Moves moves);

/*-------------------------.
| Board Debugging Helpers  |
`-------------------------*/

// @function BoardPrintMove
// @desc Prints a move on a board (highlighted view).
// @param board Pointer to the current board
// @param move The move to highlight on the board
void castro_BoardPrintMove(const Board* board, Move move);

// @macro MOVE_PRINT
// @desc Debug macro alias for MovePrint
#define MOVE_PRINT(move) MovePrint(move)


/*------------------------------------.
// @module piece
|-------------------------------------|
| Piece abstraction and utilities     |
`------------------------------------*/

// @type Piece
// @desc Represents a chess piece.
// Each piece is defined by a type (character) and a color.
// - Uppercase letters: white pieces (e.g., 'P', 'N')
// - Lowercase letters: black pieces (e.g., 'p', 'n')
typedef struct {
    char type;         ///< Character representing the piece ('P', 'n', etc.)
    PieceColor color;  ///< COLOR_WHITE or COLOR_BLACK
} Piece;

// @macro PIECE_PRINT
// @desc Prints the contents of a Piece struct (for debugging).
#define PIECE_PRINT(piece)\
    printf("%s = {.type=%c, .color=%d}\n", #piece, piece.type, piece.color)

/*------------------------------.
| Piece type identification     |
`------------------------------*/

// @macro IS_PAWN
// @desc Checks if a piece is a pawn.
#define IS_PAWN(piece) \
    (tolower(piece.type) == 'p')

// @macro IS_KNIGHT
// @desc Checks if a piece is a knight.
#define IS_KNIGHT(piece) \
    (tolower(piece.type) == 'n')

// @macro IS_BISHOP
// @desc Checks if a piece is a bishop.
#define IS_BISHOP(piece) \
    (tolower(piece.type) == 'b')

// @macro IS_ROOK
// @desc Checks if a piece is a rook.
#define IS_ROOK(piece) \
    (tolower(piece.type) == 'r')

// @macro IS_QUEEN
// @desc Checks if a piece is a queen.
#define IS_QUEEN(piece) \
    (tolower(piece.type) == 'q')

// @macro IS_KING
// @desc Checks if a piece is a king.
#define IS_KING(piece) \
    (tolower(piece.type) == 'k')

// @macro IS_COLOR
// @desc Checks if a piece has a given color.
#define IS_COLOR(piece, c) \
    (piece.color == c)

// @macro IS_WHITE
// @desc Checks if a piece is white.
#define IS_WHITE(piece) \
    IS_COLOR(piece, COLOR_WHITE)

// @macro IS_BLACK
// @desc Checks if a piece is black.
#define IS_BLACK(piece) \
    IS_COLOR(piece, COLOR_BLACK)

/*------------------------------.
| Piece utility functions       |
`------------------------------*/

// @function GetPieceColor
// @desc Gets the color of a piece given its character representation (e.g., 'P' for white, 'p' for black).
// @param piece The character representing a piece.
// @returns int COLOR_WHITE, COLOR_BLACK, or COLOR_NONE if the character is invalid.
int castro_GetPieceColor(char piece);

// @function PieceAt
// @desc Returns the Piece located at a specific square on the board by inspecting the active bitboards.
// @param board Pointer to the board state.
// @param square Square index (0–63).
// @returns Piece A struct representing the piece type and color at that square.
Piece castro_PieceAt(const Board* board, Square square);

// @function PieceCmp
// @desc Compares two Piece structs for type and color equality.
// @param p1 First piece to compare.
// @param p2 Second piece to compare.
// @returns bool True if both pieces share the same type and color.
bool castro_PieceCmp(Piece p1, Piece p2);

/*------------------------------------.
// @module notation
|-------------------------------------|
| Handles FEN/PGN I/O and SAN parsing |
`------------------------------------*/

/*-------------.
| FEN Support  |
`-------------*/

// @function FenImport
// @desc Imports a FEN string into a board.
// Sets up position, castling rights, side to move, en passant, etc.
// 
// @param board Pointer to the board to initialize
// @param fen FEN string
void castro_FenImport(Board* board, const char* fen);

// @function FenExport
// @desc Exports the current board state to a FEN string.
// 
// @param board Board to serialize
// @param fen Output buffer (must be large enough)
void castro_FenExport(const Board* board, char fen[]);

// @const MAX_HEADER_LENGTH
// @desc Maximum length for PGN header fields (Event, Site, etc.)
#define MAX_HEADER_LENGTH 256

/*-----------------.
| SAN and PGN I/O  |
`-----------------*/

// @type SanMove
// @desc Represents a move in Standard Algebraic Notation (e.g., "e4", "Nf3").
typedef struct {
    char move[16];
} SanMove;

// @type Game
// @desc PGN/notation-based game format.
// Stores metadata and the list of SAN moves.
typedef struct {
    char event[MAX_HEADER_LENGTH];   ///< Event name
    char site[MAX_HEADER_LENGTH];    ///< Site/location
    char date[MAX_HEADER_LENGTH];    ///< Date of game
    char white[MAX_HEADER_LENGTH];   ///< White player name
    char black[MAX_HEADER_LENGTH];   ///< Black player name
    char result[MAX_HEADER_LENGTH];  ///< Result string (e.g., "1-0")
    char fen[MAX_HEADER_LENGTH];     ///< Initial FEN position (optional)
    SanMove moves[MAX_MOVES];        ///< List of SAN moves
    size_t move_count;               ///< Number of moves made
} Game;

// @function move_name
// @desc Parses a SAN move and applies it to the board and game state.
// 
// @param board Current board
// @param game Game context
// @param move_str SAN move string (e.g., "e4", "O-O")
//
// @return true on success
bool castro_move_name(const Board* board, Game* game, const char* move_str);

// @macro MOVE
// @desc Shorthand for move_name(board, game, move)
#define MOVE(board, game, move) \
        move_name(board, game, move)

/*------------------------.
| Game metadata handling  |
`------------------------*/

// @function GameInit
// @desc Initializes a Game object with basic metadata and a starting FEN string.
// @param game Pointer to the Game structure to initialize.
// @param event The name of the tournament or event.
// @param site The location of the game.
// @param white The name of the player playing white.
// @param black The name of the player playing black.
// @param fen The starting position in Forsyth-Edwards Notation.
void castro_GameInit(Game* game, const char* event, const char* site, const char* white, const char* black, const char* fen);

// @function GameRun
// @desc Runs a game move-by-move, showing each updated board. Useful for debugging or terminal-based visualization.
// @param game The game object to execute.
void castro_GameRun(Game game);

// @function GamePrint
// @desc Prints the full PGN representation of a game, including metadata tags and the formatted move list.
// @param game The game object to print.
void castro_GamePrint(Game game);

// @function GameAddMove
// @desc Appends a move in Standard Algebraic Notation (SAN) to the game's move history.
// @param game Pointer to the Game structure.
// @param move The SanMove object to append.
void castro_GameAddMove(Game* game, SanMove move);

// @function GameSetEvent
// @desc Setters for PGN metadata: Sets the event field.
// @param game Pointer to the Game structure.
// @param event The event string.
void castro_GameSetEvent(Game* game, const char* event);

// @function GameSetSite
// @desc Setters for PGN metadata: Sets the site field.
// @param game Pointer to the Game structure.
// @param site The site string.
void castro_GameSetSite(Game* game, const char* site);

// @function GameSetDate
// @desc Setters for PGN metadata: Sets the date field.
// @param game Pointer to the Game structure.
// @param date The date string (typically YYYY.MM.DD).
void castro_GameSetDate(Game* game, const char* date);

// @function GameSetWhite
// @desc Setters for PGN metadata: Sets the white player's name.
// @param game Pointer to the Game structure.
// @param white The player's name.
void castro_GameSetWhite(Game* game, const char* white);

// @function GameSetBlack
// @desc Setters for PGN metadata: Sets the black player's name.
// @param game Pointer to the Game structure.
// @param black The player's name.
void castro_GameSetBlack(Game* game, const char* black);

// @function GameSetFen
// @desc Setters for PGN metadata: Sets the starting FEN position.
// @param game Pointer to the Game structure.
// @param fen The FEN string.
void castro_GameSetFen(Game* game, const char* fen);

// @function GameSetResult
// @desc Setters for PGN metadata: Sets the game result (e.g., "1-0", "0-1", "1/2-1/2").
// @param game Pointer to the Game structure.
// @param result The result string.
void castro_GameSetResult(Game* game, const char* result);

/*--------------------------.
| PGN Import/Export Support |
`--------------------------*/

// @function PgnImport
// @desc Parses a standard PGN string and populates the Game object with its metadata and move history.
// @param game Pointer to the Game structure to be populated.
// @param pgn The source string containing the PGN data.
void castro_PgnImport(Game* game, const char* pgn);

// @function PgnExport
// @desc Serializes the current Game state into a PGN formatted string.
// @param game Pointer to the Game structure to serialize.
// @param pgn Buffer to store the resulting PGN string.
void castro_PgnExport(Game* game, char* pgn);

// @function PgnExportFile
// @desc Serializes the Game to PGN format and writes it directly to a specified file path.
// @param game Pointer to the Game structure to save.
// @param path The filesystem path where the PGN file will be created or overwritten.
void castro_PgnExportFile(Game* game, const char* path);

/*-----------------------------.
| SAN <-> Move conversions     |
`-----------------------------*/

// @function Notate
// @desc Converts a Move to a SAN notation string (e.g., "Nf3", "O-O"). This requires the board state to determine move ambiguity (e.g., which knight moved) and whether the move results in check or checkmate.
// @param board Pointer to the current board state.
// @param move The internal encoded move to be converted.
// @param san Pointer to the SanMove structure where the resulting string will be stored.
void castro_Notate(Board* board, Move move, SanMove* san);

// @function SanToMove
// @desc Converts a SAN move string back into an internal 32-bit Move. It validates the notation against the current board to find the correct source square and piece type.
// @param board Pointer to the current board state.
// @param san The SanMove structure containing the notation string.
// @returns Move The internal bit-encoded representation of the move.
Move castro_SanToMove(Board* board, SanMove san);


/*------------------------------------.
// @module result
|-------------------------------------|
| Game termination and result logic   |
`------------------------------------*/

// @enum Result
// @desc Enumeration of possible game outcomes.
typedef enum {
    RESULT_NONE = 0,                               ///< Game is still ongoing
    RESULT_WHITE_WON,                              ///< White won by checkmate or resignation
    RESULT_BLACK_WON,                              ///< Black won by checkmate or resignation
    RESULT_STALEMATE,                              ///< Game ended in stalemate
    RESULT_DRAW_BY_REPETITION,                     ///< Game drawn by threefold repetition
    RESULT_DRAW_DUE_TO_INSUFFICIENT_MATERIAL,      ///< Draw due to insufficient mating material
    RESULT_DRAW_DUE_TO_50_MOVE_RULE,               ///< Draw due to 50-move rule (no pawn move or capture)
    RESULT_COUNT                                    ///< Internal count for range checking
} Result;

// @const result_score
// @desc String representations of results for PGN output.
// Matches the Result enum index:
// - "*" for ongoing
// - "1-0", "0-1" for wins
// - "1/2-1/2" for all draws
static const char result_score[][8] = {
    "*",
    "1-0",
    "0-1",
    "1/2-1/2",
    "1/2-1/2",
    "1/2-1/2",
    "1/2-1/2"
};

// @const result_message
// @desc Human-readable messages describing the result.
// Matches the Result enum index.
static const char result_message[][256] = {
    "No result yet",
    "White won",
    "Black won",
    "Stalemate",
    "Draw by repetition",
    "Draw due to insufficient material",
    "Draw due to 50 move rule"
};

/*-----------------------------.
| Result Detection Functions   |
`-----------------------------*/

// @function IsResult
// @desc Determines the current result of the game.
// 
// Checks for checkmate, stalemate, 3-fold repetition, 50-move rule, or insufficient material.
// 
// @param board Current board state
// @return Corresponding Result enum
Result castro_IsResult(Board* board);

// @function IsCheckmate
// @desc Determines if the current position is checkmate.
// 
// @param board Pointer to the board
// @return true if checkmate
bool castro_IsCheckmate(const Board* board);

// @function IsStalemate
// @desc Determines if the current position is stalemate.
// 
// @param board Pointer to the board
// @return true if stalemate
bool castro_IsStalemate(const Board* board);

// @function IsInsufficientMaterial
// @desc Checks if neither player has sufficient material to checkmate.
// 
// @param board Pointer to the board
// @return true if the game should be drawn due to insufficient material
bool castro_IsInsufficientMaterial(const Board* board);

// @function IsThreefoldRepetition
// @desc Determines if the current position has occurred three times (3-fold repetition).
// Uses the board's `History` and `HashTable` to detect repeated positions.
// 
// @param board Pointer to the board
// @return true if position repeated three times
bool castro_IsThreefoldRepetition(Board* board);

/*------------------------------------.
// @module movegen
|-------------------------------------|
| Move generation for legal and       |
| pseudo-legal moves in the engine    |
`------------------------------------*/

// @enum MoveType
// @desc Enumeration of move types to control legality enforcement.
typedef enum {
    MOVE_LEGAL,   ///< Fully legal moves that leave the king safe
    MOVE_PSEUDO,  ///< Pseudo-legal moves, ignoring king safety
    MOVE_CAPTURE,
    MOVE_ATTACK,
} MoveType;

/*-----------------------------.
| Pseudo-Legal Move Generation |
`-----------------------------*/

// @function GeneratePseudoLegalMoves
// @desc Generates all pseudo-legal moves for the current position. Includes moves that may leave the king in check.
// @param board Pointer to the constant board state.
// @returns Moves A list containing all possible moves before legality filtering.
Moves castro_GeneratePseudoLegalMoves(const Board* board);

// @function GeneratePseudoLegalMovesBitboard
// @desc Generates a bitboard representing all pseudo-legal destination squares for all pieces of the side to move.
// @param board Pointer to the constant board state.
// @returns Bitboard A 64-bit integer where each set bit is a reachable square.
Bitboard castro_GeneratePseudoLegalMovesBitboard(const Board* board);

// @function GeneratePseudoLegalPawnMoves
// @desc Pseudo-legal push moves for pawns, including single and double pushes.
// @param pawns Bitboard of the pawns to move.
// @param enemy Bitboard of all enemy pieces.
// @param color The color of the pawns.
// @returns Bitboard Destination squares for pawn pushes.
Bitboard castro_GeneratePseudoLegalPawnMoves(Bitboard pawns, Bitboard enemy, PieceColor color);

// @function GeneratePseudoLegalPawnAttacks
// @desc Pseudo-legal pawn attacks. If strict is true, diagonal movement is restricted to squares with capturable enemies.
// @param pawns Bitboard of the attacking pawns.
// @param enemy Bitboard of all enemy pieces.
// @param color The color of the pawns.
// @param strict Boolean to enforce capture-only diagonal movement.
// @returns Bitboard Squares under attack by pawns.
Bitboard castro_GeneratePseudoLegalPawnAttacks(Bitboard pawns, Bitboard enemy, PieceColor color, bool strict);

// @function GeneratePseudoLegalKnightAttacks
// @desc Pseudo-legal attacks for knights.
// @param knights Bitboard of knights.
// @param empty Bitboard of empty squares.
// @param enemy Bitboard of enemy pieces.
// @returns Bitboard Targeted squares.
Bitboard castro_GeneratePseudoLegalKnightAttacks(Bitboard knights, Bitboard empty, Bitboard enemy);

// @function GeneratePseudoLegalBishopAttacks
// @desc Pseudo-legal attacks for bishops.
// @param bishops Bitboard of bishops.
// @param empty Bitboard of empty squares.
// @param enemy Bitboard of enemy pieces.
// @returns Bitboard Targeted squares.
Bitboard castro_GeneratePseudoLegalBishopAttacks(Bitboard bishops, Bitboard empty, Bitboard enemy);

// @function GeneratePseudoLegalRookAttacks
// @desc Pseudo-legal attacks for rooks.
// @param rooks Bitboard of rooks.
// @param empty Bitboard of empty squares.
// @param enemy Bitboard of enemy pieces.
// @returns Bitboard Targeted squares.
Bitboard castro_GeneratePseudoLegalRookAttacks(Bitboard rooks, Bitboard empty, Bitboard enemy);

// @function GeneratePseudoLegalQueenAttacks
// @desc Pseudo-legal attacks for queens.
// @param queens Bitboard of queens.
// @param empty Bitboard of empty squares.
// @param enemy Bitboard of enemy pieces.
// @returns Bitboard Targeted squares.
Bitboard castro_GeneratePseudoLegalQueenAttacks(Bitboard queens, Bitboard empty, Bitboard enemy);

// @function GeneratePseudoLegalKingAttacks
// @desc Pseudo-legal attacks for kings.
// @param kings Bitboard of kings.
// @param empty Bitboard of empty squares.
// @param enemy Bitboard of enemy pieces.
// @returns Bitboard Targeted squares.
Bitboard castro_GeneratePseudoLegalKingAttacks(Bitboard kings, Bitboard empty, Bitboard enemy);

// @function GeneratePseudoLegalAttacks
// @desc Generates a combined bitboard of all squares attacked by a given color.
// @param board Pointer to the constant board state.
// @param color The color of the attacking side.
// @returns Bitboard All squares currently under attack by the specified color.
Bitboard castro_GeneratePseudoLegalAttacks(const Board* board, PieceColor color);

// @function GeneratePawnMoves
// @desc Generates potential destination squares for a single pawn.
// @param board Pointer to the board state.
// @param piece Square index of the pawn.
// @param color Color of the pawn.
// @returns Bitboard Target squares.
Bitboard castro_GeneratePawnMoves(const Board* board, Square piece, PieceColor color);

// @function GenerateKnightMoves
// @desc Generates potential destination squares for a single knight.
// @param board Pointer to the board state.
// @param piece Square index of the knight.
// @param color Color of the knight.
// @returns Bitboard Target squares.
Bitboard castro_GenerateKnightMoves(const Board* board, Square piece, PieceColor color);

// @function GenerateBishopMoves
// @desc Generates potential destination squares for a single bishop.
// @param board Pointer to the board state.
// @param piece Square index of the bishop.
// @param color Color of the bishop.
// @returns Bitboard Target squares.
Bitboard castro_GenerateBishopMoves(const Board* board, Square piece, PieceColor color);

// @function GenerateRookMoves
// @desc Generates potential destination squares for a single rook.
// @param board Pointer to the board state.
// @param piece Square index of the rook.
// @param color Color of the rook.
// @returns Bitboard Target squares.
Bitboard castro_GenerateRookMoves(const Board* board, Square piece, PieceColor color);

// @function GenerateQueenMoves
// @desc Generates potential destination squares for a single queen.
// @param board Pointer to the board state.
// @param piece Square index of the queen.
// @param color Color of the queen.
// @returns Bitboard Target squares.
Bitboard castro_GenerateQueenMoves(const Board* board, Square piece, PieceColor color);

// @function GenerateKingMoves
// @desc Generates potential destination squares for a single king.
// @param board Pointer to the board state.
// @param piece Square index of the king.
// @param color Color of the king.
// @returns Bitboard Target squares.
Bitboard castro_GenerateKingMoves(const Board* board, Square piece, PieceColor color);

/*------------------------.
| Legal Move Generation   |
`------------------------*/

// @struct LegalityContext
// @desc Contains pre-calculated data used to accelerate legality checks, such as check evasions and pins.
typedef struct {
Bitboard check_mask;
Bitboard pin_masks[64];
int check_count;
} LegalityContext;

// @function CalculateLegality
// @desc Pre-calculates the check and pin masks for the current board state.
// @param board Pointer to the board state.
// @returns LegalityContext The calculated context for fast move generation.
LegalityContext castro_CalculateLegality(const Board* board);

// @function IsLegal
// @desc Returns whether a move is fully legal, ensuring it doesn't leave the friendly king in check.
// @param board Pointer to the constant board state.
// @param move The move to validate.
// @returns bool True if the move is legal.
bool castro_IsLegal(const Board* board, Move move);

// @function GenerateLegalMoves
// @desc Generates all strictly legal moves for the current board position.
// @param board Pointer to the constant board state.
// @returns Moves A list containing all valid moves.
Moves castro_GenerateLegalMoves(const Board* board);

// @function GenerateLegalCaptures
// @desc Generates only legal captures (including en passant). Ideal for quiescence search algorithms.
// @param board Pointer to the constant board state.
// @returns Moves A list containing all legal capture moves.
Moves castro_GenerateLegalCaptures(const Board* board);

// @function GenerateLegalMovesSquare
// @desc Generates all legal moves that originate from a specific square.
// @param board Pointer to the board.
// @param square The source square index.
// @returns Moves List of legal moves for the piece on that square.
Moves castro_GenerateLegalMovesSquare(const Board* board, Square square);

// @function GenerateLegalMovesBitboard
// @desc Returns a bitboard of all legal destination squares for the side to move.
// @param board Pointer to the board.
// @returns Bitboard A 64-bit mask of all valid target squares.
Bitboard castro_GenerateLegalMovesBitboard(const Board* board);

// @function GenerateLegalPawnMoves
// @desc Generates legal pawn moves, applying check and pin constraints.
// @param board Pointer to the board.
// @param pieces Bitboard of pawns.
// @param color Color of the side to move.
// @param ctx Pointer to the pre-calculated legality context.
// @param moves Pointer to the list to append moves to.
// @param captures_only If true, only captures are generated.
void castro_GenerateLegalPawnMoves(const Board* board, Bitboard pieces, PieceColor color, const LegalityContext* ctx, Moves* moves, bool captures_only);

// @function GenerateLegalKnightMoves
// @desc Generates legal knight moves, applying check and pin constraints.
// @param board Pointer to the board.
// @param pieces Bitboard of knights.
// @param color Color of the side to move.
// @param ctx Pointer to the legality context.
// @param moves Pointer to the list to append moves to.
// @param captures_only If true, only captures are generated.
void castro_GenerateLegalKnightMoves(const Board* board, Bitboard pieces, PieceColor color, const LegalityContext* ctx, Moves* moves, bool captures_only);

// @function GenerateLegalBishopMoves
// @desc Generates legal bishop moves, applying check and pin constraints.
// @param board Pointer to the board.
// @param pieces Bitboard of bishops.
// @param color Color of the side to move.
// @param ctx Pointer to the legality context.
// @param moves Pointer to the list to append moves to.
// @param captures_only If true, only captures are generated.
void castro_GenerateLegalBishopMoves(const Board* board, Bitboard pieces, PieceColor color, const LegalityContext* ctx, Moves* moves, bool captures_only);

// @function GenerateLegalRookMoves
// @desc Generates legal rook moves, applying check and pin constraints.
// @param board Pointer to the board.
// @param pieces Bitboard of rooks.
// @param color Color of the side to move.
// @param ctx Pointer to the legality context.
// @param moves Pointer to the list to append moves to.
// @param captures_only If true, only captures are generated.
void castro_GenerateLegalRookMoves(const Board* board, Bitboard pieces, PieceColor color, const LegalityContext* ctx, Moves* moves, bool captures_only);

// @function GenerateLegalQueenMoves
// @desc Generates legal queen moves, applying check and pin constraints.
// @param board Pointer to the board.
// @param pieces Bitboard of queens.
// @param color Color of the side to move.
// @param ctx Pointer to the legality context.
// @param moves Pointer to the list to append moves to.
// @param captures_only If true, only captures are generated.
void castro_GenerateLegalQueenMoves(const Board* board, Bitboard pieces, PieceColor color, const LegalityContext* ctx, Moves* moves, bool captures_only);

// @function GenerateLegalKingMoves
// @desc Generates legal king moves, ensuring the king does not move into check.
// @param board Pointer to the board.
// @param pieces Bitboard of kings.
// @param color Color of the side to move.
// @param ctx Pointer to the legality context.
// @param moves Pointer to the list to append moves to.
// @param captures_only If true, only captures are generated.
void castro_GenerateLegalKingMoves(const Board* board, Bitboard pieces, PieceColor color, const LegalityContext* ctx, Moves* moves, bool captures_only);

/*---------------------------------------------.
| Convenience inline dispatcher for move types |
`---------------------------------------------*/

// @function GenerateMoves
// @desc Dispatches to legal or pseudo-legal move generation.
// 
// @param board The board to generate moves from
// @param type MoveType
// @return Moves struct containing the resulting moves
Moves castro_GenerateMoves(const Board* board, MoveType type);

// @module perft

// @type u64
typedef unsigned long long u64;

// @function Perft
// @desc See @link https://www.chessprogramming.org/Perft
u64 castro_Perft(Board* board, int depth, bool root);

// @function PerftPseudoLegal
// @desc Pseudo-legal perft: same node count as legal perft, faster (no pin/check pre-filter).
u64 castro_PerftPseudoLegal(Board* board, int depth);

// @module polyglot

// @type PolyglotEntry
// @desc Represents a single entry in a Polyglot-formatted opening book (.bin).
// This structure maps a specific board position to a recommended move with
// associated metadata for move selection.
typedef struct {
    uint64_t zobrist_hash;
    uint16_t move;
    uint16_t weight;
    uint32_t learn;
} PolyglotEntry;

// @macro U64
// @desc cross-platform u64 macro
#ifdef _MSC_VER
#  define U64(u) (u##ui64)
#else
#  define U64(u) (u##ULL)
#endif

// @const Random64
// @desc See @link http://hgm.nubati.net/book_format.html
static const uint64_t Random64[781] = {
   U64(0x9D39247E33776D41), U64(0x2AF7398005AAA5C7), U64(0x44DB015024623547), U64(0x9C15F73E62A76AE2),
   U64(0x75834465489C0C89), U64(0x3290AC3A203001BF), U64(0x0FBBAD1F61042279), U64(0xE83A908FF2FB60CA),
   U64(0x0D7E765D58755C10), U64(0x1A083822CEAFE02D), U64(0x9605D5F0E25EC3B0), U64(0xD021FF5CD13A2ED5),
   U64(0x40BDF15D4A672E32), U64(0x011355146FD56395), U64(0x5DB4832046F3D9E5), U64(0x239F8B2D7FF719CC),
   U64(0x05D1A1AE85B49AA1), U64(0x679F848F6E8FC971), U64(0x7449BBFF801FED0B), U64(0x7D11CDB1C3B7ADF0),
   U64(0x82C7709E781EB7CC), U64(0xF3218F1C9510786C), U64(0x331478F3AF51BBE6), U64(0x4BB38DE5E7219443),
   U64(0xAA649C6EBCFD50FC), U64(0x8DBD98A352AFD40B), U64(0x87D2074B81D79217), U64(0x19F3C751D3E92AE1),
   U64(0xB4AB30F062B19ABF), U64(0x7B0500AC42047AC4), U64(0xC9452CA81A09D85D), U64(0x24AA6C514DA27500),
   U64(0x4C9F34427501B447), U64(0x14A68FD73C910841), U64(0xA71B9B83461CBD93), U64(0x03488B95B0F1850F),
   U64(0x637B2B34FF93C040), U64(0x09D1BC9A3DD90A94), U64(0x3575668334A1DD3B), U64(0x735E2B97A4C45A23),
   U64(0x18727070F1BD400B), U64(0x1FCBACD259BF02E7), U64(0xD310A7C2CE9B6555), U64(0xBF983FE0FE5D8244),
   U64(0x9F74D14F7454A824), U64(0x51EBDC4AB9BA3035), U64(0x5C82C505DB9AB0FA), U64(0xFCF7FE8A3430B241),
   U64(0x3253A729B9BA3DDE), U64(0x8C74C368081B3075), U64(0xB9BC6C87167C33E7), U64(0x7EF48F2B83024E20),
   U64(0x11D505D4C351BD7F), U64(0x6568FCA92C76A243), U64(0x4DE0B0F40F32A7B8), U64(0x96D693460CC37E5D),
   U64(0x42E240CB63689F2F), U64(0x6D2BDCDAE2919661), U64(0x42880B0236E4D951), U64(0x5F0F4A5898171BB6),
   U64(0x39F890F579F92F88), U64(0x93C5B5F47356388B), U64(0x63DC359D8D231B78), U64(0xEC16CA8AEA98AD76),
   U64(0x5355F900C2A82DC7), U64(0x07FB9F855A997142), U64(0x5093417AA8A7ED5E), U64(0x7BCBC38DA25A7F3C),
   U64(0x19FC8A768CF4B6D4), U64(0x637A7780DECFC0D9), U64(0x8249A47AEE0E41F7), U64(0x79AD695501E7D1E8),
   U64(0x14ACBAF4777D5776), U64(0xF145B6BECCDEA195), U64(0xDABF2AC8201752FC), U64(0x24C3C94DF9C8D3F6),
   U64(0xBB6E2924F03912EA), U64(0x0CE26C0B95C980D9), U64(0xA49CD132BFBF7CC4), U64(0xE99D662AF4243939),
   U64(0x27E6AD7891165C3F), U64(0x8535F040B9744FF1), U64(0x54B3F4FA5F40D873), U64(0x72B12C32127FED2B),
   U64(0xEE954D3C7B411F47), U64(0x9A85AC909A24EAA1), U64(0x70AC4CD9F04F21F5), U64(0xF9B89D3E99A075C2),
   U64(0x87B3E2B2B5C907B1), U64(0xA366E5B8C54F48B8), U64(0xAE4A9346CC3F7CF2), U64(0x1920C04D47267BBD),
   U64(0x87BF02C6B49E2AE9), U64(0x092237AC237F3859), U64(0xFF07F64EF8ED14D0), U64(0x8DE8DCA9F03CC54E),
   U64(0x9C1633264DB49C89), U64(0xB3F22C3D0B0B38ED), U64(0x390E5FB44D01144B), U64(0x5BFEA5B4712768E9),
   U64(0x1E1032911FA78984), U64(0x9A74ACB964E78CB3), U64(0x4F80F7A035DAFB04), U64(0x6304D09A0B3738C4),
   U64(0x2171E64683023A08), U64(0x5B9B63EB9CEFF80C), U64(0x506AACF489889342), U64(0x1881AFC9A3A701D6),
   U64(0x6503080440750644), U64(0xDFD395339CDBF4A7), U64(0xEF927DBCF00C20F2), U64(0x7B32F7D1E03680EC),
   U64(0xB9FD7620E7316243), U64(0x05A7E8A57DB91B77), U64(0xB5889C6E15630A75), U64(0x4A750A09CE9573F7),
   U64(0xCF464CEC899A2F8A), U64(0xF538639CE705B824), U64(0x3C79A0FF5580EF7F), U64(0xEDE6C87F8477609D),
   U64(0x799E81F05BC93F31), U64(0x86536B8CF3428A8C), U64(0x97D7374C60087B73), U64(0xA246637CFF328532),
   U64(0x043FCAE60CC0EBA0), U64(0x920E449535DD359E), U64(0x70EB093B15B290CC), U64(0x73A1921916591CBD),
   U64(0x56436C9FE1A1AA8D), U64(0xEFAC4B70633B8F81), U64(0xBB215798D45DF7AF), U64(0x45F20042F24F1768),
   U64(0x930F80F4E8EB7462), U64(0xFF6712FFCFD75EA1), U64(0xAE623FD67468AA70), U64(0xDD2C5BC84BC8D8FC),
   U64(0x7EED120D54CF2DD9), U64(0x22FE545401165F1C), U64(0xC91800E98FB99929), U64(0x808BD68E6AC10365),
   U64(0xDEC468145B7605F6), U64(0x1BEDE3A3AEF53302), U64(0x43539603D6C55602), U64(0xAA969B5C691CCB7A),
   U64(0xA87832D392EFEE56), U64(0x65942C7B3C7E11AE), U64(0xDED2D633CAD004F6), U64(0x21F08570F420E565),
   U64(0xB415938D7DA94E3C), U64(0x91B859E59ECB6350), U64(0x10CFF333E0ED804A), U64(0x28AED140BE0BB7DD),
   U64(0xC5CC1D89724FA456), U64(0x5648F680F11A2741), U64(0x2D255069F0B7DAB3), U64(0x9BC5A38EF729ABD4),
   U64(0xEF2F054308F6A2BC), U64(0xAF2042F5CC5C2858), U64(0x480412BAB7F5BE2A), U64(0xAEF3AF4A563DFE43),
   U64(0x19AFE59AE451497F), U64(0x52593803DFF1E840), U64(0xF4F076E65F2CE6F0), U64(0x11379625747D5AF3),
   U64(0xBCE5D2248682C115), U64(0x9DA4243DE836994F), U64(0x066F70B33FE09017), U64(0x4DC4DE189B671A1C),
   U64(0x51039AB7712457C3), U64(0xC07A3F80C31FB4B4), U64(0xB46EE9C5E64A6E7C), U64(0xB3819A42ABE61C87),
   U64(0x21A007933A522A20), U64(0x2DF16F761598AA4F), U64(0x763C4A1371B368FD), U64(0xF793C46702E086A0),
   U64(0xD7288E012AEB8D31), U64(0xDE336A2A4BC1C44B), U64(0x0BF692B38D079F23), U64(0x2C604A7A177326B3),
   U64(0x4850E73E03EB6064), U64(0xCFC447F1E53C8E1B), U64(0xB05CA3F564268D99), U64(0x9AE182C8BC9474E8),
   U64(0xA4FC4BD4FC5558CA), U64(0xE755178D58FC4E76), U64(0x69B97DB1A4C03DFE), U64(0xF9B5B7C4ACC67C96),
   U64(0xFC6A82D64B8655FB), U64(0x9C684CB6C4D24417), U64(0x8EC97D2917456ED0), U64(0x6703DF9D2924E97E),
   U64(0xC547F57E42A7444E), U64(0x78E37644E7CAD29E), U64(0xFE9A44E9362F05FA), U64(0x08BD35CC38336615),
   U64(0x9315E5EB3A129ACE), U64(0x94061B871E04DF75), U64(0xDF1D9F9D784BA010), U64(0x3BBA57B68871B59D),
   U64(0xD2B7ADEEDED1F73F), U64(0xF7A255D83BC373F8), U64(0xD7F4F2448C0CEB81), U64(0xD95BE88CD210FFA7),
   U64(0x336F52F8FF4728E7), U64(0xA74049DAC312AC71), U64(0xA2F61BB6E437FDB5), U64(0x4F2A5CB07F6A35B3),
   U64(0x87D380BDA5BF7859), U64(0x16B9F7E06C453A21), U64(0x7BA2484C8A0FD54E), U64(0xF3A678CAD9A2E38C),
   U64(0x39B0BF7DDE437BA2), U64(0xFCAF55C1BF8A4424), U64(0x18FCF680573FA594), U64(0x4C0563B89F495AC3),
   U64(0x40E087931A00930D), U64(0x8CFFA9412EB642C1), U64(0x68CA39053261169F), U64(0x7A1EE967D27579E2),
   U64(0x9D1D60E5076F5B6F), U64(0x3810E399B6F65BA2), U64(0x32095B6D4AB5F9B1), U64(0x35CAB62109DD038A),
   U64(0xA90B24499FCFAFB1), U64(0x77A225A07CC2C6BD), U64(0x513E5E634C70E331), U64(0x4361C0CA3F692F12),
   U64(0xD941ACA44B20A45B), U64(0x528F7C8602C5807B), U64(0x52AB92BEB9613989), U64(0x9D1DFA2EFC557F73),
   U64(0x722FF175F572C348), U64(0x1D1260A51107FE97), U64(0x7A249A57EC0C9BA2), U64(0x04208FE9E8F7F2D6),
   U64(0x5A110C6058B920A0), U64(0x0CD9A497658A5698), U64(0x56FD23C8F9715A4C), U64(0x284C847B9D887AAE),
   U64(0x04FEABFBBDB619CB), U64(0x742E1E651C60BA83), U64(0x9A9632E65904AD3C), U64(0x881B82A13B51B9E2),
   U64(0x506E6744CD974924), U64(0xB0183DB56FFC6A79), U64(0x0ED9B915C66ED37E), U64(0x5E11E86D5873D484),
   U64(0xF678647E3519AC6E), U64(0x1B85D488D0F20CC5), U64(0xDAB9FE6525D89021), U64(0x0D151D86ADB73615),
   U64(0xA865A54EDCC0F019), U64(0x93C42566AEF98FFB), U64(0x99E7AFEABE000731), U64(0x48CBFF086DDF285A),
   U64(0x7F9B6AF1EBF78BAF), U64(0x58627E1A149BBA21), U64(0x2CD16E2ABD791E33), U64(0xD363EFF5F0977996),
   U64(0x0CE2A38C344A6EED), U64(0x1A804AADB9CFA741), U64(0x907F30421D78C5DE), U64(0x501F65EDB3034D07),
   U64(0x37624AE5A48FA6E9), U64(0x957BAF61700CFF4E), U64(0x3A6C27934E31188A), U64(0xD49503536ABCA345),
   U64(0x088E049589C432E0), U64(0xF943AEE7FEBF21B8), U64(0x6C3B8E3E336139D3), U64(0x364F6FFA464EE52E),
   U64(0xD60F6DCEDC314222), U64(0x56963B0DCA418FC0), U64(0x16F50EDF91E513AF), U64(0xEF1955914B609F93),
   U64(0x565601C0364E3228), U64(0xECB53939887E8175), U64(0xBAC7A9A18531294B), U64(0xB344C470397BBA52),
   U64(0x65D34954DAF3CEBD), U64(0xB4B81B3FA97511E2), U64(0xB422061193D6F6A7), U64(0x071582401C38434D),
   U64(0x7A13F18BBEDC4FF5), U64(0xBC4097B116C524D2), U64(0x59B97885E2F2EA28), U64(0x99170A5DC3115544),
   U64(0x6F423357E7C6A9F9), U64(0x325928EE6E6F8794), U64(0xD0E4366228B03343), U64(0x565C31F7DE89EA27),
   U64(0x30F5611484119414), U64(0xD873DB391292ED4F), U64(0x7BD94E1D8E17DEBC), U64(0xC7D9F16864A76E94),
   U64(0x947AE053EE56E63C), U64(0xC8C93882F9475F5F), U64(0x3A9BF55BA91F81CA), U64(0xD9A11FBB3D9808E4),
   U64(0x0FD22063EDC29FCA), U64(0xB3F256D8ACA0B0B9), U64(0xB03031A8B4516E84), U64(0x35DD37D5871448AF),
   U64(0xE9F6082B05542E4E), U64(0xEBFAFA33D7254B59), U64(0x9255ABB50D532280), U64(0xB9AB4CE57F2D34F3),
   U64(0x693501D628297551), U64(0xC62C58F97DD949BF), U64(0xCD454F8F19C5126A), U64(0xBBE83F4ECC2BDECB),
   U64(0xDC842B7E2819E230), U64(0xBA89142E007503B8), U64(0xA3BC941D0A5061CB), U64(0xE9F6760E32CD8021),
   U64(0x09C7E552BC76492F), U64(0x852F54934DA55CC9), U64(0x8107FCCF064FCF56), U64(0x098954D51FFF6580),
   U64(0x23B70EDB1955C4BF), U64(0xC330DE426430F69D), U64(0x4715ED43E8A45C0A), U64(0xA8D7E4DAB780A08D),
   U64(0x0572B974F03CE0BB), U64(0xB57D2E985E1419C7), U64(0xE8D9ECBE2CF3D73F), U64(0x2FE4B17170E59750),
   U64(0x11317BA87905E790), U64(0x7FBF21EC8A1F45EC), U64(0x1725CABFCB045B00), U64(0x964E915CD5E2B207),
   U64(0x3E2B8BCBF016D66D), U64(0xBE7444E39328A0AC), U64(0xF85B2B4FBCDE44B7), U64(0x49353FEA39BA63B1),
   U64(0x1DD01AAFCD53486A), U64(0x1FCA8A92FD719F85), U64(0xFC7C95D827357AFA), U64(0x18A6A990C8B35EBD),
   U64(0xCCCB7005C6B9C28D), U64(0x3BDBB92C43B17F26), U64(0xAA70B5B4F89695A2), U64(0xE94C39A54A98307F),
   U64(0xB7A0B174CFF6F36E), U64(0xD4DBA84729AF48AD), U64(0x2E18BC1AD9704A68), U64(0x2DE0966DAF2F8B1C),
   U64(0xB9C11D5B1E43A07E), U64(0x64972D68DEE33360), U64(0x94628D38D0C20584), U64(0xDBC0D2B6AB90A559),
   U64(0xD2733C4335C6A72F), U64(0x7E75D99D94A70F4D), U64(0x6CED1983376FA72B), U64(0x97FCAACBF030BC24),
   U64(0x7B77497B32503B12), U64(0x8547EDDFB81CCB94), U64(0x79999CDFF70902CB), U64(0xCFFE1939438E9B24),
   U64(0x829626E3892D95D7), U64(0x92FAE24291F2B3F1), U64(0x63E22C147B9C3403), U64(0xC678B6D860284A1C),
   U64(0x5873888850659AE7), U64(0x0981DCD296A8736D), U64(0x9F65789A6509A440), U64(0x9FF38FED72E9052F),
   U64(0xE479EE5B9930578C), U64(0xE7F28ECD2D49EECD), U64(0x56C074A581EA17FE), U64(0x5544F7D774B14AEF),
   U64(0x7B3F0195FC6F290F), U64(0x12153635B2C0CF57), U64(0x7F5126DBBA5E0CA7), U64(0x7A76956C3EAFB413),
   U64(0x3D5774A11D31AB39), U64(0x8A1B083821F40CB4), U64(0x7B4A38E32537DF62), U64(0x950113646D1D6E03),
   U64(0x4DA8979A0041E8A9), U64(0x3BC36E078F7515D7), U64(0x5D0A12F27AD310D1), U64(0x7F9D1A2E1EBE1327),
   U64(0xDA3A361B1C5157B1), U64(0xDCDD7D20903D0C25), U64(0x36833336D068F707), U64(0xCE68341F79893389),
   U64(0xAB9090168DD05F34), U64(0x43954B3252DC25E5), U64(0xB438C2B67F98E5E9), U64(0x10DCD78E3851A492),
   U64(0xDBC27AB5447822BF), U64(0x9B3CDB65F82CA382), U64(0xB67B7896167B4C84), U64(0xBFCED1B0048EAC50),
   U64(0xA9119B60369FFEBD), U64(0x1FFF7AC80904BF45), U64(0xAC12FB171817EEE7), U64(0xAF08DA9177DDA93D),
   U64(0x1B0CAB936E65C744), U64(0xB559EB1D04E5E932), U64(0xC37B45B3F8D6F2BA), U64(0xC3A9DC228CAAC9E9),
   U64(0xF3B8B6675A6507FF), U64(0x9FC477DE4ED681DA), U64(0x67378D8ECCEF96CB), U64(0x6DD856D94D259236),
   U64(0xA319CE15B0B4DB31), U64(0x073973751F12DD5E), U64(0x8A8E849EB32781A5), U64(0xE1925C71285279F5),
   U64(0x74C04BF1790C0EFE), U64(0x4DDA48153C94938A), U64(0x9D266D6A1CC0542C), U64(0x7440FB816508C4FE),
   U64(0x13328503DF48229F), U64(0xD6BF7BAEE43CAC40), U64(0x4838D65F6EF6748F), U64(0x1E152328F3318DEA),
   U64(0x8F8419A348F296BF), U64(0x72C8834A5957B511), U64(0xD7A023A73260B45C), U64(0x94EBC8ABCFB56DAE),
   U64(0x9FC10D0F989993E0), U64(0xDE68A2355B93CAE6), U64(0xA44CFE79AE538BBE), U64(0x9D1D84FCCE371425),
   U64(0x51D2B1AB2DDFB636), U64(0x2FD7E4B9E72CD38C), U64(0x65CA5B96B7552210), U64(0xDD69A0D8AB3B546D),
   U64(0x604D51B25FBF70E2), U64(0x73AA8A564FB7AC9E), U64(0x1A8C1E992B941148), U64(0xAAC40A2703D9BEA0),
   U64(0x764DBEAE7FA4F3A6), U64(0x1E99B96E70A9BE8B), U64(0x2C5E9DEB57EF4743), U64(0x3A938FEE32D29981),
   U64(0x26E6DB8FFDF5ADFE), U64(0x469356C504EC9F9D), U64(0xC8763C5B08D1908C), U64(0x3F6C6AF859D80055),
   U64(0x7F7CC39420A3A545), U64(0x9BFB227EBDF4C5CE), U64(0x89039D79D6FC5C5C), U64(0x8FE88B57305E2AB6),
   U64(0xA09E8C8C35AB96DE), U64(0xFA7E393983325753), U64(0xD6B6D0ECC617C699), U64(0xDFEA21EA9E7557E3),
   U64(0xB67C1FA481680AF8), U64(0xCA1E3785A9E724E5), U64(0x1CFC8BED0D681639), U64(0xD18D8549D140CAEA),
   U64(0x4ED0FE7E9DC91335), U64(0xE4DBF0634473F5D2), U64(0x1761F93A44D5AEFE), U64(0x53898E4C3910DA55),
   U64(0x734DE8181F6EC39A), U64(0x2680B122BAA28D97), U64(0x298AF231C85BAFAB), U64(0x7983EED3740847D5),
   U64(0x66C1A2A1A60CD889), U64(0x9E17E49642A3E4C1), U64(0xEDB454E7BADC0805), U64(0x50B704CAB602C329),
   U64(0x4CC317FB9CDDD023), U64(0x66B4835D9EAFEA22), U64(0x219B97E26FFC81BD), U64(0x261E4E4C0A333A9D),
   U64(0x1FE2CCA76517DB90), U64(0xD7504DFA8816EDBB), U64(0xB9571FA04DC089C8), U64(0x1DDC0325259B27DE),
   U64(0xCF3F4688801EB9AA), U64(0xF4F5D05C10CAB243), U64(0x38B6525C21A42B0E), U64(0x36F60E2BA4FA6800),
   U64(0xEB3593803173E0CE), U64(0x9C4CD6257C5A3603), U64(0xAF0C317D32ADAA8A), U64(0x258E5A80C7204C4B),
   U64(0x8B889D624D44885D), U64(0xF4D14597E660F855), U64(0xD4347F66EC8941C3), U64(0xE699ED85B0DFB40D),
   U64(0x2472F6207C2D0484), U64(0xC2A1E7B5B459AEB5), U64(0xAB4F6451CC1D45EC), U64(0x63767572AE3D6174),
   U64(0xA59E0BD101731A28), U64(0x116D0016CB948F09), U64(0x2CF9C8CA052F6E9F), U64(0x0B090A7560A968E3),
   U64(0xABEEDDB2DDE06FF1), U64(0x58EFC10B06A2068D), U64(0xC6E57A78FBD986E0), U64(0x2EAB8CA63CE802D7),
   U64(0x14A195640116F336), U64(0x7C0828DD624EC390), U64(0xD74BBE77E6116AC7), U64(0x804456AF10F5FB53),
   U64(0xEBE9EA2ADF4321C7), U64(0x03219A39EE587A30), U64(0x49787FEF17AF9924), U64(0xA1E9300CD8520548),
   U64(0x5B45E522E4B1B4EF), U64(0xB49C3B3995091A36), U64(0xD4490AD526F14431), U64(0x12A8F216AF9418C2),
   U64(0x001F837CC7350524), U64(0x1877B51E57A764D5), U64(0xA2853B80F17F58EE), U64(0x993E1DE72D36D310),
   U64(0xB3598080CE64A656), U64(0x252F59CF0D9F04BB), U64(0xD23C8E176D113600), U64(0x1BDA0492E7E4586E),
   U64(0x21E0BD5026C619BF), U64(0x3B097ADAF088F94E), U64(0x8D14DEDB30BE846E), U64(0xF95CFFA23AF5F6F4),
   U64(0x3871700761B3F743), U64(0xCA672B91E9E4FA16), U64(0x64C8E531BFF53B55), U64(0x241260ED4AD1E87D),
   U64(0x106C09B972D2E822), U64(0x7FBA195410E5CA30), U64(0x7884D9BC6CB569D8), U64(0x0647DFEDCD894A29),
   U64(0x63573FF03E224774), U64(0x4FC8E9560F91B123), U64(0x1DB956E450275779), U64(0xB8D91274B9E9D4FB),
   U64(0xA2EBEE47E2FBFCE1), U64(0xD9F1F30CCD97FB09), U64(0xEFED53D75FD64E6B), U64(0x2E6D02C36017F67F),
   U64(0xA9AA4D20DB084E9B), U64(0xB64BE8D8B25396C1), U64(0x70CB6AF7C2D5BCF0), U64(0x98F076A4F7A2322E),
   U64(0xBF84470805E69B5F), U64(0x94C3251F06F90CF3), U64(0x3E003E616A6591E9), U64(0xB925A6CD0421AFF3),
   U64(0x61BDD1307C66E300), U64(0xBF8D5108E27E0D48), U64(0x240AB57A8B888B20), U64(0xFC87614BAF287E07),
   U64(0xEF02CDD06FFDB432), U64(0xA1082C0466DF6C0A), U64(0x8215E577001332C8), U64(0xD39BB9C3A48DB6CF),
   U64(0x2738259634305C14), U64(0x61CF4F94C97DF93D), U64(0x1B6BACA2AE4E125B), U64(0x758F450C88572E0B),
   U64(0x959F587D507A8359), U64(0xB063E962E045F54D), U64(0x60E8ED72C0DFF5D1), U64(0x7B64978555326F9F),
   U64(0xFD080D236DA814BA), U64(0x8C90FD9B083F4558), U64(0x106F72FE81E2C590), U64(0x7976033A39F7D952),
   U64(0xA4EC0132764CA04B), U64(0x733EA705FAE4FA77), U64(0xB4D8F77BC3E56167), U64(0x9E21F4F903B33FD9),
   U64(0x9D765E419FB69F6D), U64(0xD30C088BA61EA5EF), U64(0x5D94337FBFAF7F5B), U64(0x1A4E4822EB4D7A59),
   U64(0x6FFE73E81B637FB3), U64(0xDDF957BC36D8B9CA), U64(0x64D0E29EEA8838B3), U64(0x08DD9BDFD96B9F63),
   U64(0x087E79E5A57D1D13), U64(0xE328E230E3E2B3FB), U64(0x1C2559E30F0946BE), U64(0x720BF5F26F4D2EAA),
   U64(0xB0774D261CC609DB), U64(0x443F64EC5A371195), U64(0x4112CF68649A260E), U64(0xD813F2FAB7F5C5CA),
   U64(0x660D3257380841EE), U64(0x59AC2C7873F910A3), U64(0xE846963877671A17), U64(0x93B633ABFA3469F8),
   U64(0xC0C0F5A60EF4CDCF), U64(0xCAF21ECD4377B28C), U64(0x57277707199B8175), U64(0x506C11B9D90E8B1D),
   U64(0xD83CC2687A19255F), U64(0x4A29C6465A314CD1), U64(0xED2DF21216235097), U64(0xB5635C95FF7296E2),
   U64(0x22AF003AB672E811), U64(0x52E762596BF68235), U64(0x9AEBA33AC6ECC6B0), U64(0x944F6DE09134DFB6),
   U64(0x6C47BEC883A7DE39), U64(0x6AD047C430A12104), U64(0xA5B1CFDBA0AB4067), U64(0x7C45D833AFF07862),
   U64(0x5092EF950A16DA0B), U64(0x9338E69C052B8E7B), U64(0x455A4B4CFE30E3F5), U64(0x6B02E63195AD0CF8),
   U64(0x6B17B224BAD6BF27), U64(0xD1E0CCD25BB9C169), U64(0xDE0C89A556B9AE70), U64(0x50065E535A213CF6),
   U64(0x9C1169FA2777B874), U64(0x78EDEFD694AF1EED), U64(0x6DC93D9526A50E68), U64(0xEE97F453F06791ED),
   U64(0x32AB0EDB696703D3), U64(0x3A6853C7E70757A7), U64(0x31865CED6120F37D), U64(0x67FEF95D92607890),
   U64(0x1F2B1D1F15F6DC9C), U64(0xB69E38A8965C6B65), U64(0xAA9119FF184CCCF4), U64(0xF43C732873F24C13),
   U64(0xFB4A3D794A9A80D2), U64(0x3550C2321FD6109C), U64(0x371F77E76BB8417E), U64(0x6BFA9AAE5EC05779),
   U64(0xCD04F3FF001A4778), U64(0xE3273522064480CA), U64(0x9F91508BFFCFC14A), U64(0x049A7F41061A9E60),
   U64(0xFCB6BE43A9F2FE9B), U64(0x08DE8A1C7797DA9B), U64(0x8F9887E6078735A1), U64(0xB5B4071DBFC73A66),
   U64(0x230E343DFBA08D33), U64(0x43ED7F5A0FAE657D), U64(0x3A88A0FBBCB05C63), U64(0x21874B8B4D2DBC4F),
   U64(0x1BDEA12E35F6A8C9), U64(0x53C065C6C8E63528), U64(0xE34A1D250E7A8D6B), U64(0xD6B04D3B7651DD7E),
   U64(0x5E90277E7CB39E2D), U64(0x2C046F22062DC67D), U64(0xB10BB459132D0A26), U64(0x3FA9DDFB67E2F199),
   U64(0x0E09B88E1914F7AF), U64(0x10E8B35AF3EEAB37), U64(0x9EEDECA8E272B933), U64(0xD4C718BC4AE8AE5F),
   U64(0x81536D601170FC20), U64(0x91B534F885818A06), U64(0xEC8177F83F900978), U64(0x190E714FADA5156E),
   U64(0xB592BF39B0364963), U64(0x89C350C893AE7DC1), U64(0xAC042E70F8B383F2), U64(0xB49B52E587A1EE60),
   U64(0xFB152FE3FF26DA89), U64(0x3E666E6F69AE2C15), U64(0x3B544EBE544C19F9), U64(0xE805A1E290CF2456),
   U64(0x24B33C9D7ED25117), U64(0xE74733427B72F0C1), U64(0x0A804D18B7097475), U64(0x57E3306D881EDB4F),
   U64(0x4AE7D6A36EB5DBCB), U64(0x2D8D5432157064C8), U64(0xD1E649DE1E7F268B), U64(0x8A328A1CEDFE552C),
   U64(0x07A3AEC79624C7DA), U64(0x84547DDC3E203C94), U64(0x990A98FD5071D263), U64(0x1A4FF12616EEFC89),
   U64(0xF6F7FD1431714200), U64(0x30C05B1BA332F41C), U64(0x8D2636B81555A786), U64(0x46C9FEB55D120902),
   U64(0xCCEC0A73B49C9921), U64(0x4E9D2827355FC492), U64(0x19EBB029435DCB0F), U64(0x4659D2B743848A2C),
   U64(0x963EF2C96B33BE31), U64(0x74F85198B05A2E7D), U64(0x5A0F544DD2B1FB18), U64(0x03727073C2E134B1),
   U64(0xC7F6AA2DE59AEA61), U64(0x352787BAA0D7C22F), U64(0x9853EAB63B5E0B35), U64(0xABBDCDD7ED5C0860),
   U64(0xCF05DAF5AC8D77B0), U64(0x49CAD48CEBF4A71E), U64(0x7A4C10EC2158C4A6), U64(0xD9E92AA246BF719E),
   U64(0x13AE978D09FE5557), U64(0x730499AF921549FF), U64(0x4E4B705B92903BA4), U64(0xFF577222C14F0A3A),
   U64(0x55B6344CF97AAFAE), U64(0xB862225B055B6960), U64(0xCAC09AFBDDD2CDB4), U64(0xDAF8E9829FE96B5F),
   U64(0xB5FDFC5D3132C498), U64(0x310CB380DB6F7503), U64(0xE87FBB46217A360E), U64(0x2102AE466EBB1148),
   U64(0xF8549E1A3AA5E00D), U64(0x07A69AFDCC42261A), U64(0xC4C118BFE78FEAAE), U64(0xF9F4892ED96BD438),
   U64(0x1AF3DBE25D8F45DA), U64(0xF5B4B0B0D2DEEEB4), U64(0x962ACEEFA82E1C84), U64(0x046E3ECAAF453CE9),
   U64(0xF05D129681949A4C), U64(0x964781CE734B3C84), U64(0x9C2ED44081CE5FBD), U64(0x522E23F3925E319E),
   U64(0x177E00F9FC32F791), U64(0x2BC60A63A6F3B3F2), U64(0x222BBFAE61725606), U64(0x486289DDCC3D6780),
   U64(0x7DC7785B8EFDFC80), U64(0x8AF38731C02BA980), U64(0x1FAB64EA29A2DDF7), U64(0xE4D9429322CD065A),
   U64(0x9DA058C67844F20C), U64(0x24C0E332B70019B0), U64(0x233003B5A6CFE6AD), U64(0xD586BD01C5C217F6),
   U64(0x5E5637885F29BC2B), U64(0x7EBA726D8C94094B), U64(0x0A56A5F0BFE39272), U64(0xD79476A84EE20D06),
   U64(0x9E4C1269BAA4BF37), U64(0x17EFEE45B0DEE640), U64(0x1D95B0A5FCF90BC6), U64(0x93CBE0B699C2585D),
   U64(0x65FA4F227A2B6D79), U64(0xD5F9E858292504D5), U64(0xC2B5A03F71471A6F), U64(0x59300222B4561E00),
   U64(0xCE2F8642CA0712DC), U64(0x7CA9723FBB2E8988), U64(0x2785338347F2BA08), U64(0xC61BB3A141E50E8C),
   U64(0x150F361DAB9DEC26), U64(0x9F6A419D382595F4), U64(0x64A53DC924FE7AC9), U64(0x142DE49FFF7A7C3D),
   U64(0x0C335248857FA9E7), U64(0x0A9C32D5EAE45305), U64(0xE6C42178C4BBB92E), U64(0x71F1CE2490D20B07),
   U64(0xF1BCC3D275AFE51A), U64(0xE728E8C83C334074), U64(0x96FBF83A12884624), U64(0x81A1549FD6573DA5),
   U64(0x5FA7867CAF35E149), U64(0x56986E2EF3ED091B), U64(0x917F1DD5F8886C61), U64(0xD20D8C88C8FFE65F),
   U64(0x31D71DCE64B2C310), U64(0xF165B587DF898190), U64(0xA57E6339DD2CF3A0), U64(0x1EF6E6DBB1961EC9),
   U64(0x70CC73D90BC26E24), U64(0xE21A6B35DF0C3AD7), U64(0x003A93D8B2806962), U64(0x1C99DED33CB890A1),
   U64(0xCF3145DE0ADD4289), U64(0xD0E4427A5514FB72), U64(0x77C621CC9FB3A483), U64(0x67A34DAC4356550B),
   U64(0xF8D626AAAF278509),
};

// "move" is a bit field with the following meaning (bit 0 is the least significant bit)
//
// bits                meaning
// ===================================
// 0,1,2               to file
// 3,4,5               to row
// 6,7,8               from file
// 9,10,11             from row
// 12,13,14            promotion piece
//
// "promotion piece" is encoded as follows
//
// none       0
// knight     1
// bishop     2
// rook       3
// queen      4
//
// If the move is "0" (a1a1) then it should simply be ignored. 
// It seems to me that in that case one might as well delete the entry from the book. 

// @function ConvertMove
// @desc Converts a move from the Polyglot 16-bit format (typically used in .bin opening books) to the Castro 32-bit internal move format.
// @param polyglotMove The 16-bit integer representing the move in Polyglot format.
// @returns Move The encoded 32-bit internal move representation.
Move castro_ConvertMove(uint16_t polyglotMove);

// @function LookupBookMove
// @desc Probes an external Polyglot opening book file to find a move matching the current position's Zobrist hash. If multiple moves exist, it typically selects one based on weight.
// @param position_hash The 64-bit Zobrist hash of the current board position.
// @param book_path The filesystem path to the Polyglot .bin opening book.
// @returns Move The selected book move, or a null move if no match is found.
Move castro_LookupBookMove(uint64_t position_hash, const char* book_path);


#ifdef __cplusplus
}
#endif

#ifdef CASTRO_STRIP_PREFIX

#define SquareToName castro_SquareToName
#define NameToSquare castro_NameToSquare
#define Rank castro_Rank
#define File castro_File
#define IsSquareValid castro_IsSquareValid
#define SquareFromCoords castro_SquareFromCoords
#define SquareFromName castro_SquareFromName
#define PawnAttacks castro_PawnAttacks
#define PawnPushes castro_PawnPushes
#define PawnPromotions castro_PawnPromotions
#define PawnPromotionCaptures castro_PawnPromotionCaptures
#define KnightAttacks castro_KnightAttacks
#define KingAttacks castro_KingAttacks
#define BishopAttacks castro_BishopAttacks
#define RookAttacks castro_RookAttacks
#define QueenAttacks castro_QueenAttacks
#define IsKingInCheck castro_IsKingInCheck
#define Uint32Print castro_Uint32Print
#define Uint64Print castro_Uint64Print
#define BitboardPrint castro_BitboardPrint
#define InitHashTable castro_InitHashTable
#define InitHashTableHash castro_InitHashTableHash
#define UpdateHashTable castro_UpdateHashTable
#define HashTableDecrement castro_HashTableDecrement
#define FreeHashTable castro_FreeHashTable
#define UndoPrint castro_UndoPrint
#define HistoryRemove castro_HistoryRemove
#define HistoryGetLast castro_HistoryGetLast
#define AddUndo castro_AddUndo
#define LoadLastUndo castro_LoadLastUndo
#define PromotionToChar castro_PromotionToChar
#define CharToPromotion castro_CharToPromotion
#define BoardInitFen castro_BoardInitFen
#define BoardBoardInitFenHeap Boardcastro_BoardInitFenHeap
#define BoardFree castro_BoardFree
#define GetWhite castro_GetWhite
#define GetBlack castro_GetBlack
#define GetEnemyColor castro_GetEnemyColor
#define GetEnemy castro_GetEnemy
#define GetEmpty castro_GetEmpty
#define CountPieces castro_CountPieces
#define HasCastlingRights castro_HasCastlingRights
#define RevokeCastlingRights castro_RevokeCastlingRights
#define IsSquareAttacked castro_IsSquareAttacked
#define IsSquareEmpty castro_IsSquareEmpty
#define IsSquareOccupiedBy castro_IsSquareOccupiedBy
#define NumberOfPieces castro_NumberOfPieces
#define IsInCheckColor castro_IsInCheckColor
#define IsInCheck castro_IsInCheck
#define BoardPrintSquares castro_BoardPrintSquares
#define BoardPrintBitboard castro_BoardPrintBitboard
#define BoardPrint castro_BoardPrint
#define BoardPrintBitboards castro_BoardPrintBitboards
#define BoardPrintGrid castro_BoardPrintGrid
#define BoardCopy castro_BoardCopy
#define InitZobrist castro_InitZobrist
#define CalculateZobristHash castro_CalculateZobristHash
#define CalculateZobristHashFen castro_CalculateZobristHashFen
#define InitMasks castro_InitMasks
#define GeneralOccupancy castro_GeneralOccupancy
#define BlockerMasks castro_BlockerMasks
#define ComputeDiagonalMask castro_ComputeDiagonalMask
#define ComputeAntiDiagonalMask castro_ComputeAntiDiagonalMask
#define ComputeHorizontalMask castro_ComputeHorizontalMask
#define ComputeVerticalMask castro_ComputeVerticalMask
#define DiagonalMask castro_DiagonalMask
#define AntiDiagonalMask castro_AntiDiagonalMask
#define HorizontalMask castro_HorizontalMask
#define VerticalMask castro_VerticalMask
#define ComputePawnPushMask castro_ComputePawnPushMask
#define ComputePawnDoublePushMask castro_ComputePawnDoublePushMask
#define ComputePawnPromotionMask castro_ComputePawnPromotionMask
#define ComputePawnPromotionAttackMask castro_ComputePawnPromotionAttackMask
#define ComputePawnAttackMask castro_ComputePawnAttackMask
#define ComputeKnightMoveMask castro_ComputeKnightMoveMask
#define ComputeBishopMoveMask castro_ComputeBishopMoveMask
#define ComputeRookMoveMask castro_ComputeRookMoveMask
#define ComputeQueenMoveMask castro_ComputeQueenMoveMask
#define ComputeKingMoveMask castro_ComputeKingMoveMask
#define PawnPushMask castro_PawnPushMask
#define PawnDoublePushMask castro_PawnDoublePushMask
#define PawnPromotionMask castro_PawnPromotionMask
#define PawnPromotionAttackMask castro_PawnPromotionAttackMask
#define PawnAttackMask castro_PawnAttackMask
#define KnightMoveMask castro_KnightMoveMask
#define BishopMoveMask castro_BishopMoveMask
#define RookMoveMask castro_RookMoveMask
#define QueenMoveMask castro_QueenMoveMask
#define KingMoveMask castro_KingMoveMask
#define MovesAppend castro_MovesAppend
#define MovesAppendList castro_MovesAppendList
#define MovesCombine castro_MovesCombine
#define MakeUndo castro_MakeUndo
#define MoveIsValid castro_MoveIsValid
#define MoveIsCapture castro_MoveIsCapture
#define MoveGivesCheck castro_MoveGivesCheck
#define PieceValueFromType castro_PieceValueFromType
#define OrderLegalMoves castro_OrderLegalMoves
#define MoveEncode castro_MoveEncode
#define MoveEncodeNames castro_MoveEncodeNames
#define MoveDecode castro_MoveDecode
#define MoveSetFlag castro_MoveSetFlag
#define MoveSetPromotion castro_MoveSetPromotion
#define DoMove castro_DoMove
#define UndoMove castro_UndoMove
#define MakeMove castro_MakeMove
#define UnmakeMove castro_UnmakeMove
#define MakeNullMove castro_MakeNullMove
#define UnmakeNullMove castro_UnmakeNullMove
#define Castle castro_Castle
#define IsCastle castro_IsCastle
#define Enpassant castro_Enpassant
#define IsEnpassant castro_IsEnpassant
#define IsDoublePawnPush castro_IsDoublePawnPush
#define IsPromotion castro_IsPromotion
#define IsCapture castro_IsCapture
#define IsInCheckAfterMove castro_IsInCheckAfterMove
#define MoveMake castro_MoveMake
#define MoveFreely castro_MoveFreely
#define MovePrint castro_MovePrint
#define StringToMove castro_StringToMove
#define MoveToString castro_MoveToString
#define MoveCmp castro_MoveCmp
#define MoveCmpStrict castro_MoveCmpStrict
#define GetFrom castro_GetFrom
#define GetTo castro_GetTo
#define GetPromotion castro_GetPromotion
#define GetFlag castro_GetFlag
#define UpdateHalfmove castro_UpdateHalfmove
#define UpdateCastlingRights castro_UpdateCastlingRights
#define UpdateEnpassantSquare castro_UpdateEnpassantSquare
#define BitboardToMoves castro_BitboardToMoves
#define MovesToBitboard castro_MovesToBitboard
#define BoardPrintMove castro_BoardPrintMove
#define GetPieceColor castro_GetPieceColor
#define PieceAt castro_PieceAt
#define PieceCmp castro_PieceCmp
#define FenImport castro_FenImport
#define FenExport castro_FenExport
#define move_name castro_move_name
#define GameInit castro_GameInit
#define GameRun castro_GameRun
#define GamePrint castro_GamePrint
#define GameAddMove castro_GameAddMove
#define GameSetEvent castro_GameSetEvent
#define GameSetSite castro_GameSetSite
#define GameSetDate castro_GameSetDate
#define GameSetWhite castro_GameSetWhite
#define GameSetBlack castro_GameSetBlack
#define GameSetFen castro_GameSetFen
#define GameSetResult castro_GameSetResult
#define PgnImport castro_PgnImport
#define PgnExport castro_PgnExport
#define PgnExportFile castro_PgnExportFile
#define Notate castro_Notate
#define SanToMove castro_SanToMove
#define IsResult castro_IsResult
#define IsCheckmate castro_IsCheckmate
#define IsStalemate castro_IsStalemate
#define IsInsufficientMaterial castro_IsInsufficientMaterial
#define IsThreefoldRepetition castro_IsThreefoldRepetition
#define GeneratePseudoLegalMoves castro_GeneratePseudoLegalMoves
#define GeneratePseudoLegalMovesBitboard castro_GeneratePseudoLegalMovesBitboard
#define GeneratePseudoLegalPawnMoves castro_GeneratePseudoLegalPawnMoves
#define GeneratePseudoLegalPawnAttacks castro_GeneratePseudoLegalPawnAttacks
#define GeneratePseudoLegalKnightAttacks castro_GeneratePseudoLegalKnightAttacks
#define GeneratePseudoLegalBishopAttacks castro_GeneratePseudoLegalBishopAttacks
#define GeneratePseudoLegalRookAttacks castro_GeneratePseudoLegalRookAttacks
#define GeneratePseudoLegalQueenAttacks castro_GeneratePseudoLegalQueenAttacks
#define GeneratePseudoLegalKingAttacks castro_GeneratePseudoLegalKingAttacks
#define GeneratePseudoLegalAttacks castro_GeneratePseudoLegalAttacks
#define GeneratePawnMoves castro_GeneratePawnMoves
#define GenerateKnightMoves castro_GenerateKnightMoves
#define GenerateBishopMoves castro_GenerateBishopMoves
#define GenerateRookMoves castro_GenerateRookMoves
#define GenerateQueenMoves castro_GenerateQueenMoves
#define GenerateKingMoves castro_GenerateKingMoves
#define IsLegal castro_IsLegal
#define GenerateLegalMoves castro_GenerateLegalMoves
#define GenerateLegalCaptures castro_GenerateLegalCaptures
#define GenerateLegalMovesSquare castro_GenerateLegalMovesSquare
#define GenerateLegalMovesBitboard castro_GenerateLegalMovesBitboard
#define GenerateLegalPawnMoves castro_GenerateLegalPawnMoves
#define GenerateLegalKnightMoves castro_GenerateLegalKnightMoves
#define GenerateLegalBishopMoves castro_GenerateLegalBishopMoves
#define GenerateLegalRookMoves castro_GenerateLegalRookMoves
#define GenerateLegalQueenMoves castro_GenerateLegalQueenMoves
#define GenerateLegalKingMoves castro_GenerateLegalKingMoves
#define GenerateMoves castro_GenerateMoves
#define Perft castro_Perft
#define ConvertMove castro_ConvertMove
#define LookupBookMove castro_LookupBookMove

#endif // CASTRO_STRIP_PREFIX

#endif // CASTRO_H
