#include "castro.h"
#include "IncludeOnly/logging.h"
#include <stdio.h>
#include <stdlib.h>

void castro_InitHashTableHash(HashTable* table, uint64_t starting_hash)
{
    assert(table);

    table->capacity = HASH_TABLE_CAPACITY;
    table->entries  = calloc(table->capacity, sizeof(HashEntry));
    if (!table->entries) {
        ERRO("Memory allocation failed for hash_table entries.\n");
        exit(EXIT_FAILURE);
    }

    table->last_added = 0;

    if (starting_hash != 0)
        castro_UpdateHashTable(table, starting_hash);
}

void castro_InitHashTable(HashTable* table, const char* starting_fen)
{
    assert(table);

    table->capacity = HASH_TABLE_CAPACITY;
    table->entries  = calloc(table->capacity, sizeof(HashEntry));
    if (!table->entries) {
        ERRO("Memory allocation failed for hash_table entries.\n");
        exit(EXIT_FAILURE);
    }

    table->last_added = 0;

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
    if (hash == 0) return 0; /* 0 used as empty sentinel */

    const size_t cap = table->capacity;
    const size_t mask = cap - 1;
    size_t i = (size_t)(hash & mask);

    for (size_t n = 0; n < cap; n++) {
        if (table->entries[i].hash == hash) {
            table->entries[i].count++;
            table->last_added = hash;
            return table->entries[i].count >= 3;
        }
        if (table->entries[i].hash == 0) {
            table->entries[i].hash  = hash;
            table->entries[i].count = 1;
            table->last_added = hash;
            return 0;
        }
        i = (i + 1) & mask;
    }

    return 0; /* table full */
}

void castro_HashTableDecrement(HashTable* table, uint64_t hash)
{
    if (hash == 0) return;

    const size_t cap = table->capacity;
    const size_t mask = cap - 1;
    size_t i = (size_t)(hash & mask);

    for (size_t n = 0; n < cap; n++) {
        if (table->entries[i].hash == hash) {
            if (table->entries[i].count > 0)
                table->entries[i].count--;
            if (table->entries[i].count == 0) {
                table->entries[i].hash = 0;
                table->entries[i].count = 0;
            }
            return;
        }
        if (table->entries[i].hash == 0)
            return; /* not found */
        i = (i + 1) & mask;
    }
}
