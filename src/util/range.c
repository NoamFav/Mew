#include "util/range.h"
#include "options.h"
#include "util/num.h"
#include "util/print/usage.h"
#include "util/str.h"
#include <stddef.h>

int range_component(const char *s, size_t len, long *out) {
    char buf[24];
    size_t i;

    if (len == 0 || len >= sizeof(buf)) {
        return (-1);
    }
    i = 0;
    while (i < len) {
        buf[i] = s[i];
        i++;
    }
    buf[i] = '\0';
    if (nf_str_to_long(buf, out) == -1) {
        return (-1);
    }
    if (*out < 1) {
        return (-1);
    }
    return (0);
}

/* Validate and append one range spec: N:M, N:, :M or N (1-based, inclusive). */
void add_range(t_opts *opts, const char *spec, const char *prog) {
    const char *colon = NULL;
    const char *c = spec;
    long lo = 1;
    long hi = -1;
    int colons = 0;

    while (*c) {
        if (*c == ':') {
            colons++;
            colon = c;
        }
        c++;
    }
    if (colons > 1) {
        range_error_exit(prog, spec, *opts);
    }
    if (colons == 0) {
        if (range_component(spec, getlen(spec), &lo) == -1) {
            range_error_exit(prog, spec, *opts);
        }
        hi = lo;
    } else {
        if (colon > spec) {
            if (range_component(spec, (size_t)(colon - spec), &lo) == -1) {
                range_error_exit(prog, spec, *opts);
            }
        }
        if (colon[1] != '\0') {
            if (range_component(colon + 1, getlen(colon + 1), &hi) == -1) {
                range_error_exit(prog, spec, *opts);
            }
        }
    }
    if (hi != -1 && lo > hi) {
        range_error_exit(prog, spec, *opts);
    }
    if (opts->nranges >= MAX_RANGES) {
        range_error_exit(prog, spec, *opts);
    }
    opts->ranges[opts->nranges].lo = lo;
    opts->ranges[opts->nranges].hi = hi;
    opts->nranges++;
}

/* Sort ranges by start and merge overlaps so each line prints at most once. */
void normalize_ranges(t_opts *opts) {
    t_range sorted[MAX_RANGES];
    t_range tmp;
    int i, j, n;

    i = 1;
    while (i < opts->nranges) {
        tmp = opts->ranges[i];
        j = i;
        while (j > 0 && opts->ranges[j - 1].lo > tmp.lo) {
            opts->ranges[j] = opts->ranges[j - 1];
            j--;
        }
        opts->ranges[j] = tmp;
        i++;
    }
    n = 0;
    i = 0;
    while (i < opts->nranges) {
        if (n > 0 && (sorted[n - 1].hi == -1 || opts->ranges[i].lo <= sorted[n - 1].hi)) {
            if (sorted[n - 1].hi != -1 && opts->ranges[i].hi > sorted[n - 1].hi) {
                sorted[n - 1].hi = opts->ranges[i].hi;
            } else if (sorted[n - 1].hi != -1 && opts->ranges[i].hi == -1) {
                sorted[n - 1].hi = -1;
            }
        } else {
            sorted[n] = opts->ranges[i];
            n++;
        }
        i++;
    }
    i = 0;
    while (i < n) {
        opts->ranges[i] = sorted[i];
        i++;
    }
    opts->nranges = n;
}
