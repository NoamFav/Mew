#include "util/colors.h"
#include "util/io.h"
#include "util/str.h"

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
