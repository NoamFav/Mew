#ifndef NUM_H
#define NUM_H

#include <stddef.h>

size_t count_digits(size_t n);
size_t num_to_buf(size_t n, char *num);
int nf_str_to_long(const char *s, long *out);
#endif
