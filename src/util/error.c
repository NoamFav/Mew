#include "util/error.h"
#include "mew.h"
#include "util/io.h"
#include "util/str.h"

static void put_err(const char *s) {
    write_all(STDERR_FILENO, s, getlen(s));
}

int file_error(const char *name) {
    put_err(PROGNAME ": ");
    put_err(name);
    put_err(": ");
    put_err(strerror(errno));
    put_err("\n");
    return (1);
}

void usage_exit(const char *prog, int c) {
    char ch;

    ch = (char)c;
    put_err(prog);
    put_err(": invalid option -- '");
    write_all(STDERR_FILENO, &ch, 1);
    put_err("'\n");
    put_err("usage: ");
    put_err(prog);
    put_err(" [-benstuvAET] [file ...]\n");
    exit(1);
}
