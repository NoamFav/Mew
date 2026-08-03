#include "util/io.h"

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
