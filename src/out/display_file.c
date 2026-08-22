#define _POSIX_C_SOURCE 200809L

#include "out/display_file.h"
#include "out/outbuf.h"
#include "out/render.h"
#include "util/error.h"
#include <unistd.h>

/* Advance the range cursor (ranges are sorted and merged) and report whether
 * the current physical line must be emitted. */
static int range_wants(t_linestate *ls, const t_opts *opts) {
    while (ls->range_idx < (size_t)opts->nranges &&
           opts->ranges[ls->range_idx].hi != -1 &&
           ls->phys_line > (size_t)opts->ranges[ls->range_idx].hi) {
        ls->range_idx++;
    }
    if (ls->range_idx >= (size_t)opts->nranges) {
        return (0);
    }
    if (ls->phys_line < (size_t)opts->ranges[ls->range_idx].lo) {
        return (0);
    }
    if (opts->ranges[ls->range_idx].hi != -1 &&
        ls->phys_line > (size_t)opts->ranges[ls->range_idx].hi) {
        return (0);
    }
    return (1);
}

/* Once past the highest wanted line (all ranges closed) stop reading: the
 * operand is not drained further, matching head's short-circuit behavior. */
static int range_done(const t_opts *opts, size_t phys_line) {
    long last;

    if (opts->nranges == 0) {
        return (0);
    }
    last = opts->ranges[opts->nranges - 1].hi;
    return (last != -1 && phys_line > (size_t)last);
}

int display_file(int fd, t_opts opts, const char *name) {
    unsigned char buf[READ_SIZE];
    int interactive = isatty(fd);
    ssize_t n = 0;
    size_t i, j;

    t_linestate linestate = {1, 1, 1, 1, 0, 0, 1};
    t_outbuf outbuf = {{0}, 0};

    while (1) {
        n = read(fd, buf, READ_SIZE);
        if (n <= 0) {
            if (ob_flush(&outbuf) == -1 || n == -1) {
                return (file_error(name));
            }
            return (0);
        }
        i = 0;
        while (i < (size_t)n) {
            if (linestate.line_start) {
                if (range_done(&opts, linestate.phys_line)) {
                    if (ob_flush(&outbuf) == -1) {
                        return (file_error(name));
                    }
                    return (0);
                }
                linestate.in_range =
                    opts.nranges == 0 || range_wants(&linestate, &opts);
                linestate.line_start = 0;
            }
            if (!linestate.in_range) {
                j = i;
                while (j < (size_t)n && buf[j] != '\n') {
                    j++;
                }
                if (j == (size_t)n) {
                    break;
                }
                linestate.phys_line++;
                linestate.line_start = 1;
                i = j + 1;
                continue;
            }
            if (buf[i] != '\n') {
                linestate.nl_run = 0;
            } else if (opts.squeeze_blank && linestate.nl_run >= 2) {
                linestate.phys_line++;
                linestate.line_start = 1;
                i++;
                continue;
            }
            if (linestate.at_bol && (opts.number_lines || opts.number_nonblank)) {
                if (emit_prefix(&linestate, opts, &outbuf, buf[i] == '\n', opts) == -1) {
                    return (file_error(name));
                }
                linestate.at_bol = 0;
            }

            for (j = i; j < (size_t)n && !is_special(buf[j], opts); j++)
                ;

            if (ob_append((char *)buf + i, j - i, &outbuf) == -1) {
                return (file_error(name));
            }
            if (j == (size_t)n) {
                break;
            }
            if (emit_special(buf[j], opts, &outbuf) == -1) {
                return (file_error(name));
            }
            if (buf[j] == '\n') {
                linestate.nl_run++;
                linestate.at_bol = 1;
                linestate.phys_line++;
                linestate.line_start = 1;
            }
            i = j + 1;
        }
        if (interactive && ob_flush(&outbuf) == -1) {
            return (file_error(name));
        }
    }
}
