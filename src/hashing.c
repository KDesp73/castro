#include "castro.h"
#include "IncludeOnly/logging.h"
#include <stdio.h>
#include <stdlib.h>

void InitHashTableHash(HashTable* table, uint64_t starting_hash)
{
    assert(table);

    table->entries = calloc(MAX_MOVES, sizeof(HashEntry));
    if (!table->entries) {
        ERRO("Memory allocation failed for hash_table entries.\n");
        exit(EXIT_FAILURE);
    }

    table->count = 0;

    // Adding starting position
    UpdateHashTable(table, starting_hash);
}

void InitHashTable(HashTable* table, const char* starting_fen)
{
    assert(table);

    table->entries = calloc(MAX_MOVES, sizeof(HashEntry));
    if (!table->entries) {
        ERRO("Memory allocation failed for hash_table entries.\n");
        exit(EXIT_FAILURE);
    }

    table->count = 0;

    // Adding starting position
    Board board;
    FenImport(&board, (starting_fen) ? starting_fen : STARTING_FEN);
    UpdateHashTable(table, CalculateZobristHash(&board));
    BoardFree(&board);
}

void FreeHashTable(HashTable* table)
{
    free(table->entries);
}

_Bool UpdateHashTable(HashTable* table, uint64_t hash)
{
    // TODO: Faster search. HashMap maybe
    for (size_t i = 0; i < table->count; i++) {
        if (table->entries[i].hash == hash) {
            table->entries[i].count++;
            table->last_added = hash;
            return table->entries[i].count >= 3;
        }
    }

    if (table->count < MAX_MOVES) {
        table->entries[table->count].hash = hash;
        table->entries[table->count].count = 1;
        table->count++;
        table->last_added = hash;
    }

    return 0;
}
