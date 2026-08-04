#include "mew.h"
#include "out/display_file.h"
#include "parser.h"
#include "util/error.h"
#include <fcntl.h>
#include <unistd.h>

static int display_loop(int argc, char **argv, t_opts opts) {
    int fd;
    int i;
    int error;

    error = 0;
    if (opts.firstoperand == argc)
        return (display_file(0, opts, "-"));
    i = opts.firstoperand;
    while (i < argc) {
        if (argv[i][0] == '-' && argv[i][1] == '\0')
            fd = 0;
        else
            fd = open(argv[i], O_RDONLY);
        if (fd == -1)
            error = file_error(argv[i]);
        else {
            if (display_file(fd, opts, argv[i]))
                error = 1;
            if (fd > 0)
                close(fd);
        }
        i++;
    }
    return (error);
}

int mew(int argc, char **argv) {
    t_opts opts;

    opts = opts_parser(argc, argv);

    return (display_loop(argc, argv, opts));
}
