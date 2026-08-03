#include "str.h"

int getlen(const char *str) {
    int(i) = 0;
    while (str[i]) {
        i++;
    }
    return (i);
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
