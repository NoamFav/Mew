#include "out/render.h"
#include "out/display_file.h"
#include "out/outbuf.h"
#include "util/num.h"

int is_special(unsigned char c, t_opts opts) {
    return (c == '\n' || (opts.show_tabs && c == '\t') ||
            (opts.show_nonprint && c != '\t' &&
             ((c >= 0x00 && c <= 0x1F) || (c == 0x7F) || (c >= 0x80 && c <= 0x9F) ||
              (c >= 0xA0 && c <= 0xFE) || (c == 0xFF))));
}

int emit_special(unsigned char c, t_opts opts, t_outbuf *ob) {
    if (opts.show_nonprint) {
        if (c >= 0x00 && c <= 0x1F && c != '\t' && c != '\n') {
            char value = c ^ 0x40;

            if (ob_append("^", 1, ob) == -1) {
                return (-1);
            }
            return (ob_append(&value, 1, ob));
        }
        if (c == 0x7F) {
            return (ob_append("^?", 2, ob));
        }
        if (c >= 0x80 && c <= 0x9F) {
            char value = ((c & 0x7F) ^ 0x40);

            if (ob_append("M-^", 3, ob)) {
                return (-1);
            }
            return (ob_append(&value, 1, ob));
        }
        if (c >= 0xA0 && c <= 0xFE) {
            char value = c & 0x7F;

            if (ob_append("M-", 2, ob) == -1) {
                return (-1);
            }
            return (ob_append(&value, 1, ob));
        }
        if (c == 0xFF) {
            return (ob_append("M-^?", 4, ob));
        }
    }
    if (opts.show_tabs && c == '\t') {
        return (ob_append("^I", 2, ob));
    }
    if (opts.show_ends && ob_append("$", 1, ob) == -1) {
        return (-1);
    }
    return (ob_append("\n", 1, ob));
}

int emit_prefix(t_linestate *ls, t_opts o, t_outbuf *ob, int blank) {
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
    num[len++] = '\t';
    return (ob_append(num, len, ob));
}
