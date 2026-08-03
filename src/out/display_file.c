#define _POSIX_C_SOURCE 200809L

#include "out/display_file.h"
#include "out/outbuf.h"
#include "util/error.h"
#include "util/num.h"
#include <unistd.h>

int display_file(int fd, t_opts opts, const char *name) {
    char buf[READ_SIZE], num[24];
    int interactive = isatty(fd);

    t_linestate linestate = {
        1,
        1,
    };

    t_outbuf outbuf = {
        {0},
        0,
    };

    ssize_t n = 0;
    size_t j, i, len, digits = 0;

    while (1) {
        n = read(fd, buf, READ_SIZE);
        if (n <= 0) {
            ob_flush(&outbuf);
            return (n == -1 ? file_error(name) : 0);
        }
        for (i = 0; i < (size_t)n;) {
            if (linestate.at_bol && (opts.number_lines || opts.number_nonblank)) {
                int blank = (buf[i] == '\n');
                if (!(opts.number_nonblank && blank)) {
                    digits = count_digits(linestate.line_num);
                    if (digits < NUM_WIDTH)
                        ob_append("      ", NUM_WIDTH - digits, &outbuf);
                    len = num_to_buf(linestate.line_num++, num);
                    num[len++] = '\t';
                    ob_append(num, len, &outbuf);
                }
                linestate.at_bol = 0;
            }

            for (j = i; j < (size_t)n && buf[j] != '\n'; j++)
                ;

            if (j < (size_t)n) {
                ob_append(buf + i, j - i + 1, &outbuf);
                linestate.at_bol = 1;
                i = j + 1;
            } else {
                ob_append(buf + i, n - i, &outbuf);
                i = (size_t)n;
            }
        }
        if (interactive) {
            ob_flush(&outbuf);
        }
    }
    return (0);
}
