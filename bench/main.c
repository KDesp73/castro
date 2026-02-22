/*
 * benchmark.c — Castro move generation library benchmarker
 *
 * Compile:
 * gcc -O2 -o benchmark benchmark.c -lcastro
 */

#include "castro.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

#define C_RESET  "\033[0m"
#define C_BOLD   "\033[1m"
#define C_TITLE  "\033[38;5;111m"  /* Arctic Blue */
#define C_SUB    "\033[38;5;244m"  /* Muted Grey */
#define C_NUM    "\033[38;5;80m"   /* Glacier Teal */
#define C_TIME   "\033[38;5;251m"  /* Snow White */
#define C_RATE   "\033[38;5;215m"  /* Aurora Orange */
#define C_PASS   "\033[38;5;114m"  /* Moss Green */
#define C_FAIL   "\033[38;5;203m"  /* Red */

static bool g_color = true;
static FILE* g_log = NULL;
static int g_max_depth = 99;

static const char* cc(const char* code) { return g_color ? code : ""; }

typedef struct { double seconds; uint64_t nodes; double mnps; } BenchResult;

static double now_s(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

static BenchResult make_result(uint64_t nodes, double elapsed) {
    return (BenchResult){
        .nodes   = nodes,
        .seconds = elapsed,
        .mnps    = elapsed > 0.0 ? (nodes / elapsed) / 1e6 : 0.0
    };
}

static void print_banner(const char* log_path) {
    printf("\n %s%sCASTRO%s %sMove Generation Benchmark%s\n", cc(C_BOLD), cc(C_TITLE), cc(C_RESET), cc(C_SUB), cc(C_RESET));
    printf(" %sLogging to %s%s\n", cc(C_SUB), log_path, cc(C_RESET));
}

static void section_open(const char* title) {
    printf("\n %s§ %s%s%s\n", cc(C_TITLE), cc(C_BOLD), title, cc(C_RESET));
    printf(" %s──────────────────────────────────────────────────────────────────%s\n", cc(C_SUB), cc(C_RESET));
    if (g_log) fprintf(g_log, "\n--- %s ---\n", title);
}

static void col_header(const char* label, const char* unit) {
    printf(" %-26s %12s %10s %14s\n", label, "Count", "Time", unit);
}

static void row_perft(const char* label, BenchResult r, bool ok, uint64_t expected) {
    const char* icon = ok ? "•" : "!";
    const char* col  = ok ? cc(C_PASS) : cc(C_FAIL);

    printf(" %s%s%s %-24s %s%12" PRIu64 "%s %s%9.3fs%s %s%10.2f MN/s%s\n",
        col, icon, cc(C_RESET), label,
        cc(C_NUM), r.nodes, cc(C_RESET),
        cc(C_TIME), r.seconds, cc(C_RESET),
        cc(C_RATE), r.mnps, cc(C_RESET));

    if (!ok) {
        printf("   %s└─ FAILED: expected %" PRIu64 "%s\n", cc(C_FAIL), expected, cc(C_RESET));
    }

    if (g_log) fprintf(g_log, "[%c] %-24s %12" PRIu64 " %9.3fs %10.2f MN/s\n", 
                       ok ? 'P' : 'F', label, r.nodes, r.seconds, r.mnps);
}

static void row_plain(const char* label, BenchResult r, const char* unit) {
    printf(" %s›%s %-24s %s%12" PRIu64 "%s %s%9.3fs%s %s%10.2f %s%s\n",
        cc(C_SUB), cc(C_RESET), label,
        cc(C_NUM), r.nodes, cc(C_RESET),
        cc(C_TIME), r.seconds, cc(C_RESET),
        cc(C_RATE), r.mnps, unit, cc(C_RESET));

    if (g_log) fprintf(g_log, " ›  %-24s %12" PRIu64 " %9.3fs %10.2f %s\n", 
                       label, r.nodes, r.seconds, r.mnps, unit);
}

static void print_scoreboard(int passed, int total, double wall, uint64_t nodes) {
    bool all_ok = (passed == total);
    const char* res_col = all_ok ? C_PASS : C_FAIL;

    printf("\n %s%s%s\n", cc(res_col), "──────────────────────────────────────────────────────────────────", cc(C_RESET));
    printf("  %s%s%s  %d / %d Passed  %s|%s  %.3fs  %s|%s  %" PRIu64 " Nodes\n",
        cc(C_BOLD), cc(res_col), all_ok ? "PASSED" : "FAILED", passed, total, 
        cc(C_SUB), cc(C_RESET), wall, 
        cc(C_SUB), cc(C_RESET), nodes);
    printf(" %s%s%s\n\n", cc(res_col), "──────────────────────────────────────────────────────────────────", cc(C_RESET));
}

typedef struct { const char* name; const char* fen; int depth; uint64_t expected; } PerftCase;

static const PerftCase PERFT_SUITE[] = {
    { "Start d4", STARTING_FEN, 4, 197281 },
    { "Start d5", STARTING_FEN, 5, 4865609 },
    { "Kiwipete d3", "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 3, 97862 },
    { "Kiwipete d4", "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 4, 4085603 },
    { "Endgame d5", "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 5, 674624 },
    { "Pos5 d4", "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 4, 2103487 }
};

typedef struct { const char* name; const char* fen; } Pos;
static const Pos POSITIONS[] = {
    { "Starting Pos", STARTING_FEN },
    { "Kiwipete", "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1" },
    { "Middlegame", "r1bqkb1r/pp3ppp/2nppn2/8/2B1P3/2N2N2/PPPP1PPP/R1BQK2R w KQkq - 0 6" }
};

static BenchResult run_perft(const char* fen, int depth, uint64_t* out) {
    Board board;
    castro_BoardInitFen(&board, fen);
    double t0 = now_s();
    uint64_t n = castro_Perft(&board, depth, true);
    double elapsed = now_s() - t0;
    castro_BoardFree(&board);
    if (out) *out = n;
    return make_result(n, elapsed);
}

static BenchResult run_perft_fast(const char* fen, int depth, uint64_t* out) {
    Board board;
    castro_BoardInitFen(&board, fen);
    double t0 = now_s();
    uint64_t n = castro_PerftPseudoLegal(&board, depth);
    double elapsed = now_s() - t0;
    castro_BoardFree(&board);
    if (out) *out = n;
    return make_result(n, elapsed);
}

static BenchResult bench_movegen(const char* fen, MoveType type) {
    Board board;
    castro_BoardInitFen(&board, fen);
    double t0 = now_s();
    uint64_t total = 0;
    for (int i = 0; i < 200000; i++) {
        Moves m = castro_GenerateMoves(&board, type);
        total += m.count;
    }
    double elapsed = now_s() - t0;
    castro_BoardFree(&board);
    return make_result(total, elapsed);
}

static BenchResult bench_make_unmake(const char* fen) {
    Board board;
    castro_BoardInitFen(&board, fen);
    Moves moves = castro_GenerateLegalMoves(&board);
    double t0 = now_s();
    uint64_t total = 0;
    for (int i = 0; i < 500000; i++) {
        for (size_t m = 0; m < moves.count; m++) {
            if (castro_MakeMove(&board, moves.list[m])) {
                total++;
                castro_UnmakeMove(&board);
            }
        }
    }
    double elapsed = now_s() - t0;
    castro_BoardFree(&board);
    return make_result(total, elapsed);
}

int main(int argc, char* argv[]) {
    const char* log_path = "benchmark.log";
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--no-color")) g_color = false;
        if (!strcmp(argv[i], "--quick")) g_max_depth = 4;
    }

    g_log = fopen(log_path, "w");
    castro_InitZobrist();
    castro_InitMasks();
    castro_InitMagic();

    print_banner(log_path);

    /* --- §1 PERFT --- */
    section_open("PERFT VERIFICATION");
    col_header("Target Position", "Nodes");
    int passed = 0, total = 0;
    double p_wall = 0; uint64_t p_nodes = 0;

    for (int i = 0; i < (int)(sizeof(PERFT_SUITE)/sizeof(PERFT_SUITE[0])); i++) {
        const PerftCase* tc = &PERFT_SUITE[i];
        if (tc->depth > g_max_depth) continue;

        uint64_t got;
        BenchResult r = run_perft(tc->fen, tc->depth, &got);
        bool ok = (got == tc->expected);

        total++; if (ok) passed++;
        p_wall += r.seconds; p_nodes += r.nodes;
        row_perft(tc->name, r, ok, tc->expected);
    }

    /* --- §2 MOVEGEN --- */
    section_open("MOVEGEN THROUGHPUT");
    col_header("Target Position", "Moves");
    for (int i = 0; i < (int)(sizeof(POSITIONS)/sizeof(POSITIONS[0])); i++) {
        row_plain(POSITIONS[i].name, bench_movegen(POSITIONS[i].fen, MOVE_LEGAL), "MN/s");
    }

    /* --- §3 MAKE/UNMAKE --- */
    section_open("LIFECYCLE THROUGHPUT");
    col_header("Target Position", "Cycles");
    for (int i = 0; i < (int)(sizeof(POSITIONS)/sizeof(POSITIONS[0])); i++) {
        row_plain(POSITIONS[i].name, bench_make_unmake(POSITIONS[i].fen), "MCy/s");
    }

    print_scoreboard(passed, total, p_wall, p_nodes);

    if (g_log) fclose(g_log);
    return (passed == total) ? 0 : 1;
}
