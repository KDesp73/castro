#include "castro.h"

Move ConvertMove(uint16_t polyglotMove)
{
    int to_file = (polyglotMove >> 0) & 0b111;  // Bits 0-2
    int to_rank = (polyglotMove >> 3) & 0b111;  // Bits 3-5
    int from_file = (polyglotMove >> 6) & 0b111; // Bits 6-8
    int from_rank = (polyglotMove >> 9) & 0b111; // Bits 9-11
    int promotion = (polyglotMove >> 12) & 0b111; // Bits 12-14

    int from_square = (from_rank * 8) + from_file;
    int to_square = (to_rank * 8) + to_file;

    return (from_square) | (to_square << 6) | (promotion << 12) | FLAG_NORMAL;
}

Move LookupBookMove(uint64_t position_hash, const char* book_path)
{
    FILE* book = fopen(book_path, "rb");
    if (!book) return 0;

    PolyglotEntry entry;
    while (fread(&entry, sizeof(PolyglotEntry), 1, book)) {
        if (__builtin_bswap64(entry.zobrist_hash) == position_hash) {
            fclose(book);
            return ConvertMove(__builtin_bswap16(entry.move));
        }
    }

    fclose(book);
    return NULL_MOVE;
}
