#ifndef OPTIONS_H
#define OPTIONS_H

#define MAX_RANGES 8

enum {
    OPT_COLOR = 256,
    OPT_RANGE = 257,
};

enum {
    COLOR_AUTO,
    COLOR_NEVER,
    COLOR_ALWAYS,
};

typedef struct s_range {
    long lo;
    long hi;
} t_range;

typedef struct s_opts {
    int number_lines;
    int number_nonblank;
    int show_ends;
    int show_tabs;
    int show_nonprint;
    int squeeze_blank;
    int firstoperand;
    int show_color;
    t_range ranges[MAX_RANGES];
    int nranges;
} t_opts;

#endif
