#include "castro.h"
#include <ctype.h>

Result castro_IsResult(Board* board)
{
    if(castro_IsCheckmate(board))
        return board->turn == COLOR_WHITE ? RESULT_BLACK_WON : RESULT_WHITE_WON;
    if(castro_IsStalemate(board)) 
        return RESULT_STALEMATE;
    if(castro_IsInsufficientMaterial(board)) 
        return RESULT_DRAW_DUE_TO_INSUFFICIENT_MATERIAL;
    if(castro_IsThreefoldRepetition(board)) 
        return RESULT_DRAW_BY_REPETITION;

    return RESULT_NONE;
}

bool castro_IsCheckmate(const Board* board)
{
    Moves moves = castro_GenerateMoves(board, MOVE_LEGAL);
    if(moves.count != 0) return false;

    return castro_IsInCheck(board);
}

bool castro_IsStalemate(const Board* board)
{
    Moves moves = castro_GenerateMoves(board, MOVE_LEGAL);
    if(moves.count != 0) return false;

    return !castro_IsInCheck(board);
}

bool castro_IsInsufficientMaterial(const Board* board)
{
    int white_bishops = 0, black_bishops = 0;
    int white_knights = 0, black_knights = 0;
    int white_pieces = 0, black_pieces = 0;
    Square white_bishop_square = 64, black_bishop_square = 64;

    for (int rank = 0; rank < BOARD_SIZE; rank++) {
        for (int file = 0; file < BOARD_SIZE; file++) {
            Square square = rank*8 + file; 
            Piece piece = castro_PieceAt(board, square);
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
                        white_bishop_square = square;
                    } else {
                        black_bishops++;
                        black_bishop_square = square;
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

    // 6. King and bishop vs king and bishop — draw only if bishops are on same colour (cannot force mate)
    if (total_white == 2 && total_black == 2 &&
        white_bishops == 1 && black_bishops == 1) {
        int w_sq = (castro_File(white_bishop_square) + castro_Rank(white_bishop_square)) % 2;
        int b_sq = (castro_File(black_bishop_square) + castro_Rank(black_bishop_square)) % 2;
        return w_sq == b_sq;
    }

    // 7. A lone king vs king and 2 knights
    if ((total_white == 1 && total_black == 3 && black_knights == 2) ||
        (total_black == 1 && total_white == 3 && white_knights == 2)) {
        return 1;
    }

    // Not insufficient material
    return 0;

}

bool castro_IsThreefoldRepetition(Board* board)
{
    uint64_t hash = board->hash;
    /* Only add in MakeMove; we just check. Same position twice in current path = draw. */
    return castro_HashTableGetCount(&board->history.positions, hash) >= 2;
}

bool castro_IsDraw(Board* board)
{
    if (board->halfmove >= 100)
        return true;

    if (castro_IsThreefoldRepetition(board))
        return true;

    if (castro_IsInsufficientMaterial(board))
        return true;

    return false;
}

