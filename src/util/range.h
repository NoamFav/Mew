#ifndef RANGE_H
#define RANGE_H

#include "options.h"
#include <stddef.h>

int range_component(const char *s, size_t len, long *out);
void add_range(t_opts *opts, const char *spec, const char *prog);
void normalize_ranges(t_opts *opts);

#endif
