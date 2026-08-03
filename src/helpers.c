#include "helpers.h"
#include <stdlib.h>

int getlen(const char *str) {
    int(i) = 0;
    while (str[i]) {
        i++;
    }
    return (i);
}

size_t count_digits(size_t n) {
    size_t d = 1;
    while (n >= 10) {
        n /= 10;
        d++;
    }
    return (d);
}

size_t num_to_buf(size_t n, char *num) {
    size_t len = 0;
    if (n < 10) {
        num[0] = n + '0';
        return 1;
    }
    len = num_to_buf(n / 10, num);
    num[len] = n % 10 + '0';

    return len + 1;
}

void *nf_memcpy(void *dst, const void *src, size_t n) {
    size_t i;
    unsigned char *d;
    const unsigned char *s;

    i = 0;
    d = dst;
    s = src;
    while (i < n) {
        d[i] = s[i];
        i++;
    }
    return (dst);
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
