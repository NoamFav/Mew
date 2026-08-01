#ifndef PARSER_H
#define PARSER_H

typedef struct s_opts {
    int number_of_line;
    int number_nonblanks;
    int show_ends;
    int show_tabs;
    int show_nonprint;
    int squeeze_blank;
} t_opts;

t_opts opts_parser(int argc, char **argv);

#endif
