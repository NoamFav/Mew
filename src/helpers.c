#include "helpers.h"
#include <stdlib.h>

int getlen(const char *str) {
    int(i) = 0;
    while (str[i]) {
        i++;
    }
    return (i);
}

static void put_err(const char *s) {
    write_all(STDERR_FILENO, s, getlen(s));
}

int write_all(int fd, const char *buf, ssize_t len) {
    ssize_t n;

    while (len > 0) {
        n = write(fd, buf, (size_t)len);
        if (n <= 0) {
            return (-1);
        }
        buf += n;
        len -= n;
    }
    return (0);
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
