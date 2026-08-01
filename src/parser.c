#include "parser.h"
#include "helpers.h"
#include <unistd.h>

t_opts opts_parser(int argc, char **argv) {
    t_opts opts = {0};
    int opt;

    opterr = 0;
    while ((opt = getopt(argc, argv, "nbeEtTAvsu")) != -1) {
        if (opt == 'n')
            opts.number_lines = 1;
        else if (opt == 'b')
            opts.number_nonblank = 1;
        else if (opt == 'E')
            opts.show_ends = 1;
        else if (opt == 'T')
            opts.show_tabs = 1;
        else if (opt == 'v')
            opts.show_nonprint = 1;
        else if (opt == 's')
            opts.squeeze_blank = 1;
        else if (opt == 'e')
            opts.show_ends = (opts.show_nonprint = 1);
        else if (opt == 't')
            opts.show_tabs = (opts.show_nonprint = 1);
        else if (opt == 'A')
            opts.show_ends = (opts.show_tabs = (opts.show_nonprint = 1));
        else if (opt == 'u')
            (void)opt;
        else
            usage_exit(argv[0], optopt);
    }
    if (opts.number_nonblank)
        opts.number_lines = 0;
    opts.firstoperand = optind;
    return (opts);
}
