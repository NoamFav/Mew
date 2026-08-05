#include "out/render.h"
#include "out/display_file.h"
#include "out/outbuf.h"
#include "util/colors.h"
#include "util/num.h"
#include "util/str.h"
#include <unistd.h>

static int ob_append_color(const char *s, const char *color, int len, t_outbuf *ob, t_opts opts) {
    if (iscolor(opts, STDOUT_FILENO)) {
        if (ob_append(color, getlen(color), ob) == -1) {
            return (-1);
        }
    }
    if (ob_append(s, len, ob) == -1) {
        return (-1);
    }
    if (iscolor(opts, STDOUT_FILENO)) {
        return (ob_append(COL_RESET, getlen(COL_RESET), ob));
    }
    return (0);
}

int is_special(unsigned char c, t_opts opts) {
    return (c == '\n' || (opts.show_tabs && c == '\t') ||
            (opts.show_nonprint && c != '\t' &&
             ((c <= 0x1F) || (c == 0x7F) || (c >= 0x80 && c <= 0x9F) || (c >= 0xA0 && c <= 0xFE) ||
              (c == 0xFF))));
}

int emit_special(unsigned char c, t_opts opts, t_outbuf *ob) {
    if (opts.show_nonprint) {
        if (c <= 0x1F && c != '\t' && c != '\n') {
            char value = c ^ 0x40;

            if (ob_append_color("^", COL_CYAN, 1, ob, opts) == -1) {
                return (-1);
            }
            return (ob_append_color(&value, COL_CYAN, 1, ob, opts));
        }
        if (c == 0x7F) {
            return (ob_append_color("^?", COL_CYAN, 2, ob, opts));
        }
        if (c >= 0x80 && c <= 0x9F) {
            char value = ((c & 0x7F) ^ 0x40);

            if (ob_append_color("M-^", COL_CYAN, 3, ob, opts) == -1) {
                return (-1);
            }
            return (ob_append_color(&value, COL_CYAN, 1, ob, opts));
        }
        if (c >= 0xA0 && c <= 0xFE) {
            char value = c & 0x7F;

            if (ob_append_color("M-", COL_CYAN, 2, ob, opts) == -1) {
                return (-1);
            }
            return (ob_append_color(&value, COL_CYAN, 1, ob, opts));
        }
        if (c == 0xFF) {
            return (ob_append_color("M-^?", COL_CYAN, 4, ob, opts));
        }
    }
    if (opts.show_tabs && c == '\t') {
        return (ob_append_color("^I", COL_YELLOW, 2, ob, opts));
    }
    if (opts.show_ends && ob_append_color("$", COL_RED, 1, ob, opts) == -1) {
        return (-1);
    }
    return (ob_append("\n", 1, ob));
}

int emit_prefix(t_linestate *ls, t_opts o, t_outbuf *ob, int blank, t_opts opts) {
    char num[24];
    size_t len, digits;

    if (o.number_nonblank && blank) {
        return (0);
    }
    digits = count_digits(ls->line_num);
    if (digits < NUM_WIDTH && ob_append(SPACES, NUM_WIDTH - digits, ob) == -1) {
        return (-1);
    }
    len = num_to_buf(ls->line_num++, num);
    if (ob_append_color(num, COL_MAGENTA, len, ob, opts) == -1) {
        return (-1);
    }
    return (ob_append(" │ ", 5, ob));
}
