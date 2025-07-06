/**
 * test.h v0.0.4
 *
 * A tiny data-driven testing framework
 *
 * Written by KDesp73
 */

#ifndef TEST_H
#define TEST_H

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define TEST_VERSION "0.0.4"

#ifndef TESTAPI
    #define TESTAPI static
#endif // TESTAPI

TESTAPI int test(int first, ...);
TESTAPI void load_test(const char* name);

#undef ANSI_RED
#define ANSI_RED "\e[0;31m"
#undef ANSI_RESET
#define ANSI_RESET "\e[0;39m"
#undef ANSI_GREEN
#define ANSI_GREEN "\e[0;32m"

#define FAIL(fmt, ...) \
    fprintf(stderr, ANSI_RED "FAILED: " fmt ANSI_RESET "\n", ##__VA_ARGS__)
#define SUCC(fmt, ...) \
    printf(ANSI_GREEN "SUCCESS: " fmt ANSI_RESET "\n", ##__VA_ARGS__)

#define START_TESTS \
        return !test(

#define END -1
#define END_TESTS \
        ,END \
    );

#define LOAD_TEST(test) \
    load_test(test)

#define RUN_TEST(test) \
    TEST_##test


#define TEST_ARGS(argc, argv)                                                                 \
    do {                                                                                      \
        cli_args_t args = cli_args_make(                                                      \
                cli_arg_new('h', "help", "Prints this message", no_argument),                 \
                cli_arg_new('l', "load", "Load the tests", no_argument),                      \
                cli_arg_new('1', "batch-1", "Run only batch #1", no_argument),                \
                cli_arg_new('2', "batch-2", "Run only batch #2", no_argument),                \
                cli_arg_new('3', "batch-3", "Run only batch #3", no_argument),                \
                cli_arg_new('4', "batch-4", "Run only batch #4", no_argument),                \
                NULL                                                                          \
            );                                                                                \
        int opt, rc = 0;                                                                      \
        LOOP_ARGS(opt, args){                                                                 \
            switch (opt) {                                                                    \
                case 'h':                                                                     \
                    cli_help(args, "check [OPTION]", "Written by KDesp73 - Part of test.h");  \
                    break;                                                                    \
                case 'l':                                                                     \
                    load();                                                                   \
                    break;                                                                    \
                case '1':                                                                     \
                    rc = _1();                                                                \
                    break;                                                                    \
                case '2':                                                                     \
                    rc = _2();                                                                \
                    break;                                                                    \
                case '3':                                                                     \
                    rc = _3();                                                                \
                    break;                                                                    \
                case '4':                                                                     \
                    rc = _4();                                                                \
                    break;                                                                    \
                default:                                                                      \
                    rc = 1;                                                                   \
                    break;                                                                    \
            }                                                                                 \
        }                                                                                     \
        cli_args_free(&args);                                                                 \
        if(argc > 1){ exit(rc); }                                                             \
    } while(0)

extern char* TEST_DIRECTORY;
extern void load(void);
extern int _1(void);
extern int _2(void);
extern int _3(void);
extern int _4(void);

#ifdef TEST_IMPLEMENTATION
#define STRAPPEND(buffer, fmt, ...) \
    snprintf(buffer + strlen(buffer), sizeof(buffer), fmt "\n", ##__VA_ARGS__)

TESTAPI void load_test(const char* name)
{
    char input_path[128];
    snprintf(input_path, sizeof(input_path), "%s/%s.ctd", TEST_DIRECTORY, name);
    FILE* input_file = fopen(input_path, "r");
    if (input_file == NULL) {
        FILE* write = fopen(input_path, "w");
        if(write == NULL) {
            perror("Failed to open ctd file. The test directory may be missing");
            return;
        }
        fclose(write);
        load_test(name);
        return;
    }

    char output_path[128];
    snprintf(output_path, sizeof(output_path), "%s/%s.h", TEST_DIRECTORY, name);

    FILE* output_file = fopen(output_path, "w");
    if (output_file == NULL) {
        perror("Failed to open output file");
        fclose(input_file);
        return;
    }

    char line[256];
    int line_count = 0;

    // First pass: Count non-empty, non-comment lines
    while (fgets(line, sizeof(line), input_file)) {
        // Remove trailing newline, if present
        line[strcspn(line, "\n")] = '\0';

        // Skip empty lines and lines starting with '#'
        if (line[0] == '\0' || line[0] == '#') {
            continue;
        }

        line_count++;
    }
    rewind(input_file);

    // Write the header file
    fprintf(output_file, "#ifndef %s_h\n", name);
    fprintf(output_file, "#define %s_h\n", name);
    fprintf(output_file, "\n#define TEST_%s", name);
    if (line_count > 0) {
        fprintf(output_file, "\\\n");
    } else {
        fprintf(output_file, " 1\n");
    }

    int current_line = 0;
    while (fgets(line, sizeof(line), input_file)) {
        // Remove trailing newline
        line[strcspn(line, "\n")] = '\0';

        // Skip empty lines and lines starting with '#'
        if (line[0] == '\0' || line[0] == '#') {
            continue;
        }

        if (current_line < line_count - 1) {
            fprintf(output_file, "\t%s(%s), \\\n", name, line);
        } else {
            fprintf(output_file, "\t%s(%s)\n", name, line);
        }

        current_line++;
    }

    fprintf(output_file, "\n#endif\n");

    // Close files
    fclose(input_file);
    fclose(output_file);
}


TESTAPI int test(int first, ...)
{
    size_t passed = 0, failed = 0;
    int result = 1;

    va_list args;
    va_start(args, first);

    if (first == 0) {
        failed++;
        result = 0;
    } else passed++;

    int current;
    while ((current = va_arg(args, int)) != END) {
        if (current == 0) {
            result = 0;
            failed++;
        } else passed++;
    }

    va_end(args);

    printf("\n%sPASSED: %zu/%zu%s\n", ANSI_GREEN, passed, passed + failed, ANSI_RESET);
    printf("%sFAILED: %zu/%zu%s\n", ANSI_RED, failed, passed + failed, ANSI_RESET);
    return result;
}
#endif // TEST_IMPLEMENTATION

#endif // TEST_H
