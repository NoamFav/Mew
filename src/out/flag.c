#include "out/flag.h"
#include "out/display_file.h"
#include "out/outbuf.h"
#include "util/num.h"

int is_special(char c, t_opts opts) {
    if (c == '\n') {
        return (1);
    }
    if (opts.show_tabs && c == '\t') {
        return (1);
    }
    return (0);
}

int emit_special(char c, t_opts opts, t_outbuf *ob) {
    if (c == '\t') {
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
