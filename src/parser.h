#ifndef PARSER_H
#define PARSER_H

typedef struct s_opts {
    int number_lines;
    int number_nonblank;
    int show_ends;
    int show_tabs;
    int show_nonprint;
    int squeeze_blank;
    int firstoperand;
} t_opts;

t_opts opts_parser(int argc, char **argv);

#endif
