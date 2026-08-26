#include "util/num.h"
#include <limits.h>

size_t count_digits(size_t n) {
    return (n < 10 ? 1 : 1 + count_digits(n / 10));
}

size_t num_to_buf(size_t n, char *num) {
    size_t len = 0;

    if (n < 10) {
        num[0] = n + '0';
        return (1);
    }
    len = num_to_buf(n / 10, num);
    num[len] = n % 10 + '0';
    return (len + 1);
}

int nf_str_to_long(const char *s, long *out) {
    long val = 0;

    if (!s || !*s) {
        return (-1);
    }
    while (*s) {
        if (*s < '0' || *s > '9') {
            return (-1);
        }
        if (val > (LONG_MAX - (*s - '0')) / 10) {
            return (-1);
        }
        val = val * 10 + (*s - '0');
        s++;
    }
    *out = val;
    return (0);
}
