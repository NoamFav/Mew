#define _POSIX_C_SOURCE 200809L
#include "parser.h"
#include "util/error.h"
#include <getopt.h>
#include <unistd.h>

t_opts opts_parser(int argc, char **argv) {
    t_opts opts = {0};
    int opt;
    opterr = 0;

    static const struct option longopts[] = {
        {"number", no_argument, NULL, 'n'},
        {"number-nonblank", no_argument, NULL, 'b'},
        {"show-ends", no_argument, NULL, 'E'},
        {"show-tabs", no_argument, NULL, 'T'},
        {"show-nonprinting", no_argument, NULL, 'v'},
        {"squeeze-blank", no_argument, NULL, 's'},
        {"show-all", no_argument, NULL, 'A'},
        {"show-colors", optional_argument, NULL, OPT_COLOR},
        {"show-range", required_argument, NULL, OPT_RANGE},
        {"help", no_argument, NULL, 'h'},
        {"version", no_argument, NULL, 'V'},
        {0, 0, 0, 0},
    };

    while ((opt = getopt_long(argc, argv, "+nbeEtTAvshVu", longopts, NULL)) != -1) {
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
        case 'h':
            print_help(argv[0]);
            break;
        case 'V':
            print_version(argv[0]);
            break;
        case 'u':
            break;
        default:
            if (optopt == 0) {
                usage_exit_long(argv[0], argv[optind - 1]);
            } else {
                usage_exit(argv[0], optopt);
            }
        }
    }

    if (opts.number_nonblank)
        opts.number_lines = 0;
    opts.firstoperand = optind;
    return (opts);
}
