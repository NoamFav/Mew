#include "util/num.h"

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
