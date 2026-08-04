#define _POSIX_C_SOURCE 200809L
#include "parser.h"
#include "util/error.h"
#include <unistd.h>

t_opts opts_parser(int argc, char **argv) {
    t_opts opts = {0};
    int opt;

    opterr = 0;
    while ((opt = getopt(argc, argv, "+nbeEtTAvsu")) != -1) {
        switch (opt) {
        case 'n':
            opts.number_lines = 1;
            break;
        case 'b':
            opts.number_nonblank = 1;
            break;
        case 'E':
            opts.show_ends = 1;
            break;
        case 'T':
            opts.show_tabs = 1;
            break;
        case 'v':
            opts.show_nonprint = 1;
            break;
        case 's':
            opts.squeeze_blank = 1;
            break;
        case 'e':
            opts.show_ends = opts.show_nonprint = 1;
            break;
        case 't':
            opts.show_tabs = opts.show_nonprint = 1;
            break;
        case 'A':
            opts.show_ends = opts.show_tabs = opts.show_nonprint = 1;
            break;
        case 'u':
            break;
        default:
            usage_exit(argv[0], optopt);
        }
    }
    if (opts.number_nonblank)
        opts.number_lines = 0;
    opts.firstoperand = optind;
    return (opts);
}
