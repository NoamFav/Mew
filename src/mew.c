#include "mew.h"
#include "files.h"
#include "out/display_file.h"
#include "parser.h"
#include "util/error.h"

static int is_dash(const char *s) {
    return (s[0] == '-' && s[1] == '\0');
}

static int display_loop(int file_count, char **file, t_opts opts) {
    int fd;
    int i;
    int error;
    const char *label;

    error = 0;
    if (opts.firstoperand == file_count)
        return (display_file(0, opts, "STDIN", 1));
    i = opts.firstoperand;
    while (i < file_count) {
        fd = open_operand(file[i]);
        if (fd == -1)
            error = file_error(file[i]);
        else {
            label = is_dash(file[i]) ? "STDIN" : file[i];
            if (display_file(fd, opts, label, i == opts.firstoperand))
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
