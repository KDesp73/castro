#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

#define ANSI_IMPLEMENTATION
#include "ansi.h"

#ifndef ANSI_VERSION
    #error "menu.h depends on ansi.h"
#endif

#define MENU_VERSION_MAJOR 0
#define MENU_VERSION_MINOR 0
#define MENU_VERSION_PATCH 1
#define MENU_VERSION "0.0.1"

#ifndef MENUAPI
    #define MENUAPI
#endif // MENUAPI

#ifdef _WIN32
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>

    MENUAPI int _getch();
#endif

// TODO: Add more keys
typedef enum {
    // arrows (defined later)
    MENU_KEY_ARROW_UP,
    MENU_KEY_ARROW_DOWN,
    MENU_KEY_ARROW_LEFT,
    MENU_KEY_ARROW_RIGHT,

    MENU_KEY_UNKNOWN = -1,
    MENU_KEY_ENTER = 10,
    MENU_KEY_BACKSPACE = 127,
    MENU_KEY_TAB = 9,
    MENU_KEY_ESC = 27,
    MENU_KEY_SPACE = 32,

    // numbers
    MENU_KEY_0 = '0',
    MENU_KEY_1 = '1',
    MENU_KEY_2 = '2',
    MENU_KEY_3 = '3',
    MENU_KEY_4 = '4',
    MENU_KEY_5 = '5',
    MENU_KEY_6 = '6',
    MENU_KEY_7 = '7',
    MENU_KEY_8 = '8',
    MENU_KEY_9 = '9',

    // uppercase
    MENU_KEY_A = 'A',
    MENU_KEY_B = 'B',
    MENU_KEY_C = 'C',
    MENU_KEY_D = 'D',
    MENU_KEY_E = 'E',
    MENU_KEY_F = 'F',
    MENU_KEY_G = 'G',
    MENU_KEY_H = 'H',
    MENU_KEY_I = 'I',
    MENU_KEY_J = 'J',
    MENU_KEY_K = 'K',
    MENU_KEY_L = 'L',
    MENU_KEY_M = 'M',
    MENU_KEY_N = 'N',
    MENU_KEY_O = 'O',
    MENU_KEY_P = 'P',
    MENU_KEY_Q = 'Q',
    MENU_KEY_R = 'R',
    MENU_KEY_S = 'S',
    MENU_KEY_T = 'T',
    MENU_KEY_U = 'U',
    MENU_KEY_V = 'V',
    MENU_KEY_W = 'W',
    MENU_KEY_X = 'X',
    MENU_KEY_Y = 'Y',
    MENU_KEY_Z = 'Z',

    // lowercase
    MENU_KEY_a = 'a',
    MENU_KEY_b = 'b',
    MENU_KEY_c = 'c',
    MENU_KEY_d = 'd',
    MENU_KEY_e = 'e',
    MENU_KEY_f = 'f',
    MENU_KEY_g = 'g',
    MENU_KEY_h = 'h',
    MENU_KEY_i = 'i',
    MENU_KEY_j = 'j',
    MENU_KEY_k = 'k',
    MENU_KEY_l = 'l',
    MENU_KEY_m = 'm',
    MENU_KEY_n = 'n',
    MENU_KEY_o = 'o',
    MENU_KEY_p = 'p',
    MENU_KEY_q = 'q',
    MENU_KEY_r = 'r',
    MENU_KEY_s = 's',
    MENU_KEY_t = 't',
    MENU_KEY_u = 'u',
    MENU_KEY_v = 'v',
    MENU_KEY_w = 'w',
    MENU_KEY_x = 'x',
    MENU_KEY_y = 'y',
    MENU_KEY_z = 'z',

    // special characters
    MENU_KEY_EXCLAMATION = '!',
    MENU_KEY_AT = '@',
    MENU_KEY_HASH = '#',
    MENU_KEY_DOLLAR = '$',
    MENU_KEY_PERCENT = '%',
    MENU_KEY_CARET = '^',
    MENU_KEY_AMPERSAND = '&',
    MENU_KEY_ASTERISK = '*',
    MENU_KEY_LEFT_PAREN = '(',
    MENU_KEY_RIGHT_PAREN = ')',
    MENU_KEY_DASH = '-',
    MENU_KEY_UNDERSCORE = '_',
    MENU_KEY_EQUAL = '=',
    MENU_KEY_PLUS = '+',
    MENU_KEY_LEFT_BRACKET = '[',
    MENU_KEY_RIGHT_BRACKET = ']',
    MENU_KEY_LEFT_BRACE = '{',
    MENU_KEY_RIGHT_BRACE = '}',
    MENU_KEY_SEMICOLON = ';',
    MENU_KEY_COLON = ':',
    MENU_KEY_SINGLE_QUOTE = '\'',
    MENU_KEY_DOUBLE_QUOTE = '"',
    MENU_KEY_COMMA = ',',
    MENU_KEY_PERIOD = '.',
    MENU_KEY_LESS = '<',
    MENU_KEY_GREATER = '>',
    MENU_KEY_SLASH = '/',
    MENU_KEY_QUESTION = '?',
    MENU_KEY_BACKSLASH = '\\',
    MENU_KEY_PIPE = '|',
    MENU_KEY_BACKTICK = '`',
    MENU_KEY_TILDE = '~',
} ClibKey;

typedef void (*MenuPrintOptionFunc)(const char* option, int is_selected, int color);

MENUAPI void menu_default_print_option(const char* option, int is_selected, int color);
MENUAPI void menu_arrow_print_option(const char* option, int is_selected, int color);
MENUAPI void menu_brackets_print_option(const char* option, int is_selected, int color);

MENUAPI void menu_enable_input_buffering();
MENUAPI void menu_disable_input_buffering();
MENUAPI int menu_getch();

MENUAPI int menu(const char* title, int color, MenuPrintOptionFunc print_option, char* first_option, ...);

#ifdef MENU_IMPLEMENTATION
#ifndef _WIN32
    MENUAPI int _getch() {
        struct termios oldt, newt;
        int ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }
#endif

MENUAPI void menu_enable_input_buffering(){
    #ifdef _WIN32
        // Enable console input buffering
        HANDLE hConsoleInput = GetStdHandle(STD_INPUT_HANDLE);

        DWORD consoleMode;
        GetConsoleMode(hConsoleInput, &consoleMode);

        consoleMode |= ENABLE_LINE_INPUT;

        SetConsoleMode(hConsoleInput, consoleMode);
    #else
        struct termios term;
        tcgetattr(STDIN_FILENO, &term);
        term.c_lflag |= (ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &term);

    #endif
    ANSI_SHOW_CURSOR();
}

MENUAPI void menu_disable_input_buffering(){
    #ifdef _WIN32
        // Disable console input buffering
        DWORD mode;
        HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
        GetConsoleMode(hInput, &mode);
        SetConsoleMode(hInput, mode & ~ENABLE_ECHO_INPUT & ~ENABLE_LINE_INPUT);
    #else
        struct termios term;
        tcgetattr(STDIN_FILENO, &term);
        term.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &term);

    #endif
    ANSI_HIDE_CURSOR();
}

MENUAPI int menu_getch() {
    int ch;
    #ifdef _WIN32
        ch = _getch();
        if (ch == 0 || ch == 224) {
            // Handle extended keys (arrows, function keys)
            ch = _getch();
            switch (ch) {
                case 72: return MENU_KEY_ARROW_UP;
                case 80: return MENU_KEY_ARROW_DOWN;
                case 75: return MENU_KEY_ARROW_LEFT;
                case 77: return MENU_KEY_ARROW_RIGHT;
                default: return MENU_KEY_UNKNOWN;
            }
        }
    #else
        ch = _getch();
        if (ch == 27) {
            if (getchar() == '[') {
                switch (getchar()) {
                    case 'A': return MENU_KEY_ARROW_UP;
                    case 'B': return MENU_KEY_ARROW_DOWN;
                    case 'C': return MENU_KEY_ARROW_RIGHT;
                    case 'D': return MENU_KEY_ARROW_LEFT;
                    default: return MENU_KEY_UNKNOWN;
                }
            }
            return MENU_KEY_ESC;
        }
    #endif

    switch (ch) {
        case 10: return MENU_KEY_ENTER;
        case 127: return MENU_KEY_BACKSPACE;
        case 9: return MENU_KEY_TAB;
        case 27: return MENU_KEY_ESC;
        case 32: return MENU_KEY_SPACE;

        // digits
        case '0': return MENU_KEY_0;
        case '1': return MENU_KEY_1;
        case '2': return MENU_KEY_2;
        case '3': return MENU_KEY_3;
        case '4': return MENU_KEY_4;
        case '5': return MENU_KEY_5;
        case '6': return MENU_KEY_6;
        case '7': return MENU_KEY_7;
        case '8': return MENU_KEY_8;
        case '9': return MENU_KEY_9;

        // uppercase
        case 'A': return MENU_KEY_A;
        case 'B': return MENU_KEY_B;
        case 'C': return MENU_KEY_C;
        case 'D': return MENU_KEY_D;
        case 'E': return MENU_KEY_E;
        case 'F': return MENU_KEY_F;
        case 'G': return MENU_KEY_G;
        case 'H': return MENU_KEY_H;
        case 'I': return MENU_KEY_I;
        case 'J': return MENU_KEY_J;
        case 'K': return MENU_KEY_K;
        case 'L': return MENU_KEY_L;
        case 'M': return MENU_KEY_M;
        case 'N': return MENU_KEY_N;
        case 'O': return MENU_KEY_O;
        case 'P': return MENU_KEY_P;
        case 'Q': return MENU_KEY_Q;
        case 'R': return MENU_KEY_R;
        case 'S': return MENU_KEY_S;
        case 'T': return MENU_KEY_T;
        case 'U': return MENU_KEY_U;
        case 'V': return MENU_KEY_V;
        case 'W': return MENU_KEY_W;
        case 'X': return MENU_KEY_X;
        case 'Y': return MENU_KEY_Y;
        case 'Z': return MENU_KEY_Z;

        // lowercase
        case 'a': return MENU_KEY_a;
        case 'b': return MENU_KEY_b;
        case 'c': return MENU_KEY_c;
        case 'd': return MENU_KEY_d;
        case 'e': return MENU_KEY_e;
        case 'f': return MENU_KEY_f;
        case 'g': return MENU_KEY_g;
        case 'h': return MENU_KEY_h;
        case 'i': return MENU_KEY_i;
        case 'j': return MENU_KEY_j;
        case 'k': return MENU_KEY_k;
        case 'l': return MENU_KEY_l;
        case 'm': return MENU_KEY_m;
        case 'n': return MENU_KEY_n;
        case 'o': return MENU_KEY_o;
        case 'p': return MENU_KEY_p;
        case 'q': return MENU_KEY_q;
        case 'r': return MENU_KEY_r;
        case 's': return MENU_KEY_s;
        case 't': return MENU_KEY_t;
        case 'u': return MENU_KEY_u;
        case 'v': return MENU_KEY_v;
        case 'w': return MENU_KEY_w;
        case 'x': return MENU_KEY_x;
        case 'y': return MENU_KEY_y;
        case 'z': return MENU_KEY_z;

        // special characters
        case '!': return MENU_KEY_EXCLAMATION;
        case '@': return MENU_KEY_AT;
        case '#': return MENU_KEY_HASH;
        case '$': return MENU_KEY_DOLLAR;
        case '%': return MENU_KEY_PERCENT;
        case '^': return MENU_KEY_CARET;
        case '&': return MENU_KEY_AMPERSAND;
        case '*': return MENU_KEY_ASTERISK;
        case '(': return MENU_KEY_LEFT_PAREN;
        case ')': return MENU_KEY_RIGHT_PAREN;
        case '-': return MENU_KEY_DASH;
        case '_': return MENU_KEY_UNDERSCORE;
        case '=': return MENU_KEY_EQUAL;
        case '+': return MENU_KEY_PLUS;
        case '[': return MENU_KEY_LEFT_BRACKET;
        case ']': return MENU_KEY_RIGHT_BRACKET;
        case '{': return MENU_KEY_LEFT_BRACE;
        case '}': return MENU_KEY_RIGHT_BRACE;
        case ';': return MENU_KEY_SEMICOLON;
        case ':': return MENU_KEY_COLON;
        case '\'': return MENU_KEY_SINGLE_QUOTE;
        case '"': return MENU_KEY_DOUBLE_QUOTE;
        case ',': return MENU_KEY_COMMA;
        case '.': return MENU_KEY_PERIOD;
        case '<': return MENU_KEY_LESS;
        case '>': return MENU_KEY_GREATER;
        case '/': return MENU_KEY_SLASH;
        case '?': return MENU_KEY_QUESTION;
        case '\\': return MENU_KEY_BACKSLASH;
        case '|': return MENU_KEY_PIPE;
        case '`': return MENU_KEY_BACKTICK;
        case '~': return MENU_KEY_TILDE;

        default: return ch;
    }
}


MENUAPI void menu_default_print_option(const char* option, int is_selected, int color){
    char c[32];
    ANSI_COLOR_BG(c, color);
    is_selected ? printf("%s%s%s", c, option, ANSI_RESET) : printf("%s", option);
}

MENUAPI void menu_arrow_print_option(const char* option, int is_selected, int color){
    char c[32];
    ANSI_COLOR_FG(c, color);
    is_selected ? printf("%s>%s %s", c, ANSI_RESET, option) : printf("  %s", option);
}

MENUAPI void menu_brackets_print_option(const char* option, int is_selected, int color)
{
    char c[32];
    ANSI_COLOR_FG(c, color);
    is_selected ? printf("%s[%s%s%s]%s", c, ANSI_RESET, option, c, ANSI_RESET) : printf(" %s ", option);
}

MENUAPI int menu(const char* title, int color, MenuPrintOptionFunc print_option, char* first_option, ...)
{
    if (first_option == NULL) return -1;

    menu_disable_input_buffering();

    int selected = 0;
    size_t option_count = 0;

    va_list args;
    va_start(args, first_option);
    for (char* opt = first_option; opt != NULL; opt = va_arg(args, char*)) {
        option_count++;
    }
    va_end(args);

    char** options = (char**) malloc(option_count * sizeof(char*));
    if (options == NULL) {
        menu_enable_input_buffering();
        return -1; // Allocation failed
    }

    va_start(args, first_option);
    options[0] = first_option; // Add first option
    for (size_t i = 1; i < option_count; ++i) {
        options[i] = va_arg(args, char*);
    }
    va_end(args);

    while (1) {
        if (title != NULL) {
            char color_code[32];
            ANSI_COLOR_FG(color_code, color);
            printf("%s%s%s\n", color_code, title, ANSI_RESET);
        }

        for (size_t i = 0; i < option_count; ++i) {
            print_option(options[i], selected == i, color);
            printf("\n");
        }

        int pressed = menu_getch();
        switch (pressed) {
            case MENU_KEY_ARROW_UP:
                selected = (selected + option_count - 1) % option_count; // Wrap around
                break;
            case MENU_KEY_ARROW_DOWN:
                selected = (selected + 1) % option_count; // Wrap around
                break;
            case MENU_KEY_ENTER:
                free(options);
                menu_enable_input_buffering();
                return selected;
            default:
                break;
        }

        ANSI_MOVE_CURSOR_UP(option_count + (title != NULL));
        ANSI_CLEAR_BELOW_CURSOR();
    }
}

#endif // MENU_IMPLEMENTATION

#endif // MENU_H

