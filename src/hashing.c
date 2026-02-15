#include "castro.h"
#include "IncludeOnly/logging.h"
#include <stdio.h>
#include <stdlib.h>

void castro_InitHashTableHash(HashTable* table, uint64_t starting_hash)
{
    assert(table);

    table->entries = calloc(MAX_MOVES, sizeof(HashEntry));
    if (!table->entries) {
        ERRO("Memory allocation failed for hash_table entries.\n");
        exit(EXIT_FAILURE);
    }

    table->count = 0;

    // Adding starting position
    castro_UpdateHashTable(table, starting_hash);
}

void castro_InitHashTable(HashTable* table, const char* starting_fen)
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
    castro_FenImport(&board, (starting_fen) ? starting_fen : STARTING_FEN);
    castro_UpdateHashTable(table, castro_CalculateZobristHash(&board));
    castro_BoardFree(&board);
}

void castro_FreeHashTable(HashTable* table)
{
    free(table->entries);
}

_Bool castro_UpdateHashTable(HashTable* table, uint64_t hash)
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
