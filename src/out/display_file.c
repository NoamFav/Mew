#define _POSIX_C_SOURCE 200809L

#include "out/display_file.h"
#include "out/decor.h"
#include "out/outbuf.h"
#include "out/render.h"
#include "util/error.h"
#include <unistd.h>

int display_file(int fd, t_opts opts, const char *name, int is_first) {
    unsigned char buf[READ_SIZE];
    int interactive = isatty(fd);
    ssize_t n = 0;
    size_t i, j;

    t_linestate linestate = {1, 1, 1};
    t_outbuf outbuf = {{0}, 0};

    if (!is_first && (opts.style & STYLE_RULE) && emit_rule(opts, &outbuf) == -1) {
        return (file_error(name));
    }
    if ((opts.style & STYLE_HEADER) && emit_header(name, opts, &outbuf) == -1) {
        return (file_error(name));
    }
    while (1) {
        n = read(fd, buf, READ_SIZE);
        if (n <= 0) {
            if (ob_flush(&outbuf) == -1 || n == -1) {
                return (file_error(name));
            }
            return (0);
        }
        for (i = 0; i < (size_t)n;) {
            if (buf[i] != '\n') {
                linestate.nl_run = 0;
            } else if (opts.squeeze_blank && linestate.nl_run >= 2) {
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
            }
            i = j + 1;
        }
        if (interactive && ob_flush(&outbuf) == -1) {
            return (file_error(name));
        }
    }
}
