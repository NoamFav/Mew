#include "util/str.h"

size_t getlen(const char *str) {
    size_t i = 0;

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

int nf_strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    if (!*s1 && !*s2)
        return (0);
    else
        return (*(unsigned char *)s1 - *(unsigned char *)s2);
}
