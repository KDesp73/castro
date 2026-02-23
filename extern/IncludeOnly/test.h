/**
 * test.h
 * A sleek, data-driven testing framework - Execution Core
 */

#ifndef TEST_H
#define TEST_H

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define TEST_VERSION "0.1.4"

#ifndef TESTAPI
    #define TESTAPI extern
#endif 

#ifndef TAG
    #define TAG "root"
#endif

// Core API
TESTAPI int  test(int first, ...);
TESTAPI void test_batches(char*** batches, size_t batch_count, size_t* batch_sizes);
TESTAPI void _test_set_dispatcher_internal(int (*dispatcher)(const char*), const char* name);
#define test_set_dispatcher(fn) _test_set_dispatcher_internal(fn, #fn)
TESTAPI int  test_run(int argc, char** argv);

// UI Styles (Sleek Palette)
#define C_RESET  "\033[0m"
#define C_BOLD   "\033[1m"
#define C_TITLE  "\033[38;5;111m" /* Arctic Blue */
#define C_SUB    "\033[38;5;244m" /* Muted Grey */
#define C_NUM    "\033[38;5;80m"  /* Glacier Teal */
#define C_TIME   "\033[38;5;251m" /* Snow White */
#define C_PASS   "\033[38;5;114m" /* Moss Green */
#define C_FAIL   "\033[38;5;203m" /* Red */

#define SUCC(fmt, ...) printf(" %s%s✔%s %s%-15s%s │ " fmt "\n", C_PASS, C_BOLD, C_RESET, C_SUB, TAG, C_RESET, ##__VA_ARGS__)
#define FAIL(fmt, ...) fprintf(stderr, " %s%s✘%s %s%-15s%s │ " fmt "\n", C_FAIL, C_BOLD, C_RESET, C_SUB, TAG, C_RESET, ##__VA_ARGS__)

// DSL Macros
#define START_TESTS return !test(
#define END -1
#define END_TESTS ,END );

#ifdef TEST_IMPLEMENTATION

static int (*G_DISPATCHER)(const char*) = NULL;
static char*** G_BATCHES = NULL;
static size_t  G_BATCH_COUNT = 0;
static size_t* G_BATCH_SIZES = NULL;
static int     G_PASSED_TOTAL = 0;
static int     G_FAILED_TOTAL = 0;

static double _test_now_s(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

TESTAPI void _test_set_dispatcher_internal(int (*dispatcher)(const char*), const char* name) {
    G_DISPATCHER = dispatcher;
    printf("\n %s%s%s %sSet as active dispatcher%s\n", C_TITLE, C_BOLD, name, C_SUB, C_RESET);
}

TESTAPI void test_batches(char*** batches, size_t count, size_t* sizes) {
    G_BATCHES = batches;
    G_BATCH_COUNT = count;
    G_BATCH_SIZES = sizes;
}

static void print_section(const char* title) {
    printf("\n %s§ %s%s%s\n", C_TITLE, C_BOLD, title, C_RESET);
    printf(" %s──────────────────────────────────────────────────────────────────%s\n", C_SUB, C_RESET);
}

static int run_batch_by_index(size_t index) {
    if (!G_DISPATCHER || !G_BATCHES || index >= G_BATCH_COUNT) return 0;
    
    char** batch = G_BATCHES[index];
    int batch_fail = 0;

    for (size_t i = 0; i < G_BATCH_SIZES[index]; i++) { 
        if (!G_DISPATCHER(batch[i])) {
            batch_fail = 1;
            G_FAILED_TOTAL++;
        } else {
            G_PASSED_TOTAL++;
        }
    }
    return !batch_fail;
}

TESTAPI int test_run(int argc, char** argv) {
    if (!G_DISPATCHER) {
        FAIL("No dispatcher set. Aborting.");
        return 1;
    }

    int target_batch = -1;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printf("\n %sUSAGE:%s %s [options]\n", C_BOLD, C_RESET, argv[0]);
            printf("  -n <N>    Run specific batch number (1-%zu)\n", G_BATCH_COUNT);
            printf("  -h        Show this help menu\n\n");
            return 0;
        }
        if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            target_batch = atoi(argv[++i]) - 1;
        }
    }

    if (target_batch >= (int)G_BATCH_COUNT || (target_batch < -1)) {
        FAIL("Invalid batch index. Range is 1-%zu.", G_BATCH_COUNT);
        return 1;
    }

    double t0 = _test_now_s();
    printf(" %s%sTEST RUN%s %sData-Driven Suite%s\n", C_BOLD, C_TITLE, C_RESET, C_SUB, C_RESET);

    if (target_batch >= 0) {
        char title[64];
        snprintf(title, sizeof(title), "BATCH #%d", target_batch + 1);
        print_section(title);
        run_batch_by_index((size_t)target_batch);
    } else {
        for (size_t i = 0; i < G_BATCH_COUNT; i++) {
            char title[64];
            snprintf(title, sizeof(title), "BATCH #%zu", i + 1);
            print_section(title);
            run_batch_by_index(i);
        }
    }
    double elapsed = _test_now_s() - t0;

    // Scoreboard
    int total = G_PASSED_TOTAL + G_FAILED_TOTAL;
    bool all_ok = (G_FAILED_TOTAL == 0 && total > 0);
    const char* res_col = all_ok ? C_PASS : C_FAIL;

    printf("\n %s%s\n", res_col, "──────────────────────────────────────────────────────────────────");
    printf("  %s%s%s  %d / %d Passed  %s|%s  %s%.3fs%s  %s|%s  Suite: %s%s%s\n",
        C_BOLD, res_col, all_ok ? "PASSED" : "FAILED", 
        G_PASSED_TOTAL, total, 
        C_SUB, C_RESET, C_TIME, elapsed, C_RESET,
        C_SUB, C_RESET, C_TITLE, TAG, C_RESET);
    printf(" %s%s\n\n", res_col, "──────────────────────────────────────────────────────────────────");

    return !all_ok;
}

TESTAPI int test(int first, ...) {
    va_list args;
    va_start(args, first);
    int current = first;
    int failed = 0;
    while (current != END) {
        if (current == 0) failed++;
        current = va_arg(args, int);
    }
    va_end(args);
    return (failed == 0);
}

#endif // TEST_IMPLEMENTATION
#endif // TEST_H
