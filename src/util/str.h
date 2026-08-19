#ifndef STR_H
#define STR_H

#include <stddef.h>

void *nf_memcpy(void *dst, const void *src, size_t n);
size_t getlen(const char *str);
int nf_strcmp(const char *s1, const char *s2);
int nf_memcmp(const void *a, const void *b, size_t n);
int parse_style(const char *s, unsigned *out, const char **bad, size_t *blen);

#endif
