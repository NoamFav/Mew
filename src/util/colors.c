#include "util/colors.h"
#include "options.h"
#include "util/io.h"
#include "util/str.h"
#include <unistd.h>

void col_on(int fd, int color, const char *code) {
    if (color)
        write_all(fd, code, getlen(code));
}

void col_off(int fd, int color) {
    if (color)
        write_all(fd, COL_RESET, getlen(COL_RESET));
}

void put_c(int fd, int color, const char *code, const char *s) {
    col_on(fd, color, code);
    write_all(fd, s, getlen(s));
    col_off(fd, color);
}

int iscolor(t_opts opts, int fd) {
    if (opts.show_color == COLOR_ALWAYS)
        return (1);
    if (opts.show_color == COLOR_NEVER)
        return (0);
    return (isatty(fd));
}
