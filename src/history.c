#include "castro.h"
#include <stdio.h>

bool castro_AddUndo(Board* board, uint32_t move)
{
    if (board->history.count >= MAX_MOVES) {
        return false;
    }

    board->history.moves[(board->history.count)++] = castro_MakeUndo(board, move);

    return true;
}

void castro_HistoryRemove(History* history)
{
    if (history->count == 0) return;
    history->count--;

    for (size_t i = 0; i < history->positions.count; i++) {
        if (history->positions.last_added == history->positions.entries[i].hash) {
            if (history->positions.entries[i].count > 0) {
                history->positions.entries[i].count--;
            }
            break;
        }
    }
}

void castro_UndoPrint(Undo undo)
{
    castro_MovePrint(undo.move);
    printf("Castling: %d\n", undo.castling);
    printf("Halfmove: %zu\n", undo.fiftyMove);
    printf("Captured: %c\n", undo.captured);
    if(undo.enpassant == 64)
        printf("Enpassant: -\n");
    else
        SQUARE_PRINT(undo.enpassant);
}

Undo castro_HistoryGetLast(History history)
{
    if(history.count <= 0) {
        fprintf(stderr, "No more moves\n");
        return NULL_UNDO;
    }
    return history.moves[history.count-1];
}

Undo castro_LoadLastUndo(Board* board)
{
    Undo undo = castro_HistoryGetLast(board->history);
    if(undo.move == NULL_MOVE) return undo;

    board->halfmove = undo.fiftyMove;
    board->castling_rights = undo.castling;
    board->enpassant_square = undo.enpassant;

    return undo;
}
