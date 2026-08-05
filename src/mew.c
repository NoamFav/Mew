#include "mew.h"
#include "files.h"
#include "out/display_file.h"
#include "parser.h"
#include "util/error.h"

static int display_loop(int file_count, char **file, t_opts opts) {
    int fd;
    int i;
    int error;

    error = 0;
    if (opts.firstoperand == file_count)
        return (display_file(0, opts, "-"));
    i = opts.firstoperand;
    while (i < file_count) {
        fd = open_operand(file[i]);
        if (fd == -1)
            error = file_error(file[i]);
        else {
            if (display_file(fd, opts, file[i]))
                error = 1;
            close_operand(fd);
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
