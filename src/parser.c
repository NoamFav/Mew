#define _POSIX_C_SOURCE 200809L
#include "parser.h"
#include "util/num.h"
#include "util/str.h"
#include "util/usage.h"
#include <getopt.h>
#include <stddef.h>

static const struct option longopts[] = {
    {"number", no_argument, NULL, 'n'},
    {"number-nonblank", no_argument, NULL, 'b'},
    {"show-ends", no_argument, NULL, 'E'},
    {"show-tabs", no_argument, NULL, 'T'},
    {"show-nonprinting", no_argument, NULL, 'v'},
    {"squeeze-blank", no_argument, NULL, 's'},
    {"show-all", no_argument, NULL, 'A'},
    {"color", optional_argument, NULL, OPT_COLOR},
    {"line-range", required_argument, NULL, OPT_RANGE},
    {"help", no_argument, NULL, 'h'},
    {"version", no_argument, NULL, 'V'},
    {"style", required_argument, NULL, OPT_STYLE},
    {"plain", no_argument, NULL, 'p'},
    {0, 0, 0, 0},
};

/* Parse one range component (digits only). Returns 0 on success. */
static int range_component(const char *s, size_t len, long *out) {
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
static void add_range(t_opts *opts, const char *spec, const char *prog) {
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
static void normalize_ranges(t_opts *opts) {
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

t_opts opts_parser(int option_count, char **options) {
    t_opts opts = {0};
    int opt;
    opterr = 0;
    const char *errtok;
    size_t errlen;

    while ((opt = getopt_long(option_count, options, "+nbeEtTAvshVupr:", longopts, NULL)) != -1) {
        switch (opt) {
        case 'n':
            opts.number_lines = 1;
            break;
        case 'b':
            opts.number_nonblank = 1;
            break;
        case 'E':
            opts.show_ends = 1;
            break;
        case 'T':
            opts.show_tabs = 1;
            break;
        case 'v':
            opts.show_nonprint = 1;
            break;
        case 's':
            opts.squeeze_blank = 1;
            break;
        case 'e':
            opts.show_ends = opts.show_nonprint = 1;
            break;
        case 't':
            opts.show_tabs = opts.show_nonprint = 1;
            break;
        case 'A':
            opts.show_ends = opts.show_tabs = opts.show_nonprint = 1;
            break;
        case 'h':
            print_help(options[0], opts);
            break;
        case 'V':
            print_version(options[0], opts);
            break;
        case OPT_COLOR:
            if (optarg == NULL || nf_strcmp(optarg, "auto") == 0) {
                opts.show_color = COLOR_AUTO;
            } else if (nf_strcmp(optarg, "never") == 0) {
                opts.show_color = COLOR_NEVER;
            } else if (nf_strcmp(optarg, "always") == 0) {
                opts.show_color = COLOR_ALWAYS;
            }
            break;
        case OPT_STYLE:
            if (parse_style(optarg, &opts.style, &errtok, &errlen))
                usage_exit_style(options[0], errtok, opts);
            break;
        case 'p':
            opts.style = STYLE_PLAIN;
        case 'r':
        case OPT_RANGE:
            add_range(&opts, optarg, options[0]);
            break;
        case 'u':
            break;
        default:
            if (optopt == 0) {
                usage_exit_long(options[0], options[optind - 1], opts);
            } else {
                usage_exit(options[0], optopt, opts);
            }
        }
    }

    if (opts.nranges > 0) {
        normalize_ranges(&opts);
    }
    if (opts.number_nonblank)
        opts.number_lines = 0;
    opts.firstoperand = optind;
    return (opts);
}
