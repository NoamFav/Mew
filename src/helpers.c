#include "../includes/minette.h"

int getlen(const char *str) {
    int(i) = 0;
    while (str[i]) {
        i++;
    }
    return (i);
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
    write_all(2, PROGNAME ": ", getlen(PROGNAME ": "));
    write_all(2, name, getlen(name));
    write_all(2, ": ", 2);
    write_all(2, strerror(errno), getlen(strerror(errno)));
    write_all(2, "\n", 1);
    return (1);
}
