#include "../includes/minette.h"

static int write_all(int fd, const char *buf, ssize_t len) {
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

int display_file(int fd, const char *name) {
    int line;
    char buf[1024];
    int chunk = sizeof(buf);

    while (1) {
        line = read(fd, buf, chunk);
        if (line == -1) {
            return (file_error(name));
        }
        if (line == 0) {
            return (0);
        }
        write_all(1, buf, line);
    }
    return (0);
}
