#include "util/str.h"
#include "options.h"

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

int nf_memcmp(const void *a, const void *b, size_t n) {
    const unsigned char *p = a;
    const unsigned char *q = b;

    while (n--) {
        if (*p != *q)
            return (int)*p - (int)*q;
        p++;
        q++;
    }
    return 0;
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

int parse_style(const char *s, unsigned *out, const char **bad, size_t *blen) {
    unsigned mask = 0;

    while (*s) {
        while (*s == ',')
            s++;
        if (!*s)
            break;

        int neg = 0;
        if (*s == '-') {
            neg = 1;
            s++;
        }

        const char *tok = s;
        while (*s && *s != ',')
            s++;
        size_t len = (size_t)(s - tok);

        size_t i = 0;
        while (i < sizeof styles / sizeof *styles) {
            if (styles[i].len == len && nf_memcmp(styles[i].name, tok, len) == 0)
                break;
            i++;
        }
        if (i == sizeof styles / sizeof *styles) {
            *bad = tok;
            *blen = len;
            return 1;
        }
        if (neg)
            mask &= ~styles[i].bits;
        else if (styles[i].set)
            mask = styles[i].bits; /* preset: assign */
        else
            mask |= styles[i].bits;
    }
    *out = mask;
    return 0;
}
