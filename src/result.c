#include "castro.h"
#include <ctype.h>

Result IsResult(Board* board)
{
    if(IsCheckmate(board))
        return board->turn == COLOR_WHITE ? RESULT_BLACK_WON : RESULT_WHITE_WON;
    if(IsStalemate(board)) 
        return RESULT_STALEMATE;
    if(IsInsufficientMaterial(board)) 
        return RESULT_DRAW_DUE_TO_INSUFFICIENT_MATERIAL;
    if(IsThreefoldRepetition(board)) 
        return RESULT_DRAW_BY_REPETITION;

    return RESULT_NONE;
}

bool IsCheckmate(const Board* board)
{
    Moves moves = GenerateMoves(board, MOVE_LEGAL);
    if(moves.count != 0) return false;

    return IsInCheck(board);
}

bool IsStalemate(const Board* board)
{
    Moves moves = GenerateMoves(board, MOVE_LEGAL);
    if(moves.count != 0) return false;

    return !IsInCheck(board);
}

bool IsInsufficientMaterial(const Board* board)
{
    int white_bishops = 0, black_bishops = 0;
    int white_knights = 0, black_knights = 0;
    int white_pieces = 0, black_pieces = 0;

    for (int rank = 0; rank < BOARD_SIZE; rank++) {
        for (int file = 0; file < BOARD_SIZE; file++) {
            Square square = rank*8 + file; 
            Piece piece = PieceAt(board, square);
            int color = piece.color;

            if (piece.type == EMPTY_SQUARE) continue;

            switch (tolower(piece.type)) {
                case 'p':
                case 'q':
                case 'r':
                    // Pawns, queens, and rooks can checkmate, so return early
                    return 0;

                case 'b':
                    if (color == COLOR_WHITE) {
                        white_bishops++;
                    } else {
                        black_bishops++;
                    }
                    break;

                case 'n':
                    if (color == COLOR_WHITE) {
                        white_knights++;
                    } else {
                        black_knights++;
                    }
                    break;

                default:
                    // Count kings or invalid pieces
                    if (color == COLOR_WHITE) {
                        white_pieces++;
                    } else {
                        black_pieces++;
                    }
            }
        }
    }

    // Count total pieces for each player
    int total_white = white_bishops + white_knights + white_pieces;
    int total_black = black_bishops + black_knights + black_pieces;

    // Insufficient material cases:
    // 1. Two lone kings
    if (total_white == 1 && total_black == 1) {
        return 1;
    }

    // 2. A lone king vs king and bishop
    if ((total_white == 1 && total_black == 2 && black_bishops == 1) ||
        (total_black == 1 && total_white == 2 && white_bishops == 1)) {
        return 1;
    }

    // 3. A lone king vs king and knight
    if ((total_white == 1 && total_black == 2 && black_knights == 1) ||
        (total_black == 1 && total_white == 2 && white_knights == 1)) {
        return 1;
    }

    // 4. King and knight vs king and bishop
    if (total_white == 2 && total_black == 2 &&
        white_knights == 1 && black_bishops == 1) {
        return 1;
    }

    if (total_black == 2 && total_white == 2 &&
        black_knights == 1 && white_bishops == 1) {
        return 1;
    }

    // 5. King and knight vs king and knight
    if (total_white == 2 && total_black == 2 &&
        white_knights == 1 && black_knights == 1) {
        return 1;
    }

    if (total_white == 2 && total_black == 2 &&
        white_bishops == 1 && black_bishops == 1) {
        return 1;
    }

    // 7. A lone king vs king and 2 knights
    if ((total_white == 1 && total_black == 3 && black_knights == 2) ||
        (total_black == 1 && total_white == 3 && white_knights == 2)) {
        return 1;
    }

    // Not insufficient material
    return 0;

}

bool IsThreefoldRepetition(Board* board)
{
    uint64_t hash = CalculateZobristHash(board);
    return UpdateHashTable(&board->history.positions, hash);
}

