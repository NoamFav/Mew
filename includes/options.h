#ifndef OPTIONS_H
#define OPTIONS_H

#include <stddef.h>

enum {
    OPT_COLOR = 256,
    OPT_RANGE = 257,
};

enum {
    COLOR_AUTO,
    COLOR_NEVER,
    COLOR_ALWAYS,
};

enum {
    OPT_STYLE = 258,
    OPT_PLAIN = 259,
};

enum {
    STYLE_HEADER = 1u << 0,
    STYLE_RULE = 1u << 1,
    STYLE_SYNTAX = 1u << 2,
};

#define STYLE_PLAIN 0u
#define STYLE_DEFAULT (STYLE_HEADER | STYLE_RULE)
#define STYLE_FULL (STYLE_HEADER | STYLE_RULE | STYLE_SYNTAX)

static const struct {
    const char *name;
    size_t len;
    unsigned bits;
    unsigned set;
} styles[] = {
    {"header", 6, STYLE_HEADER, 0},   {"rule", 4, STYLE_RULE, 0},
    {"syntax", 6, STYLE_SYNTAX, 0},   {"full", 4, STYLE_FULL, 1},
    {"default", 7, STYLE_DEFAULT, 1}, {"plain", 5, STYLE_PLAIN, 1},
};

typedef struct s_opts {
    int number_lines;
    int number_nonblank;
    int show_ends;
    int show_tabs;
    int show_nonprint;
    int squeeze_blank;
    int firstoperand;
    int show_color;
    unsigned style;
} t_opts;

#endif
