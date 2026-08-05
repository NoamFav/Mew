#ifndef OPTIONS_H
#define OPTIONS_H

enum {
    OPT_COLOR = 256,
    OPT_RANGE = 256,
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
} t_opts;

#endif
