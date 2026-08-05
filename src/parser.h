#ifndef PARSER_H
#define PARSER_H

#include "options.h"

enum {
    COLOR_AUTO,
    COLOR_NEVER,
    COLOR_ALWAYS,
};

t_opts opts_parser(int argc, char **argv);

#endif
