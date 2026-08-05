#include "util/error.h"
#include "util/io.h"
#include "util/str.h"

static void put_fd(int fd, const char *s) {
    write_all(fd, s, getlen(s));
}

static void put_err(const char *s) {
    put_fd(STDERR_FILENO, s);
}

int file_error(const char *name) {
    put_err(PROGNAME ": ");
    put_err(name);
    put_err(": ");
    put_err(strerror(errno));
    put_err("\n");
    return (1);
}
