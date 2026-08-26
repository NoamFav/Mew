#include "decor.h"
#include "options.h"
#include "util/colors.h"
#include "util/str.h"
#include <unistd.h>

#define RULE_CHAR "\xe2\x94\x80" /* U+2500 BOX DRAWINGS LIGHT HORIZONTAL */
#define RULE_WIDTH 40

int emit_header(const char *filename, t_opts opts, t_outbuf *ob) {
    int color = iscolor(opts, STDOUT_FILENO);

    if (color && ob_append(COL_HEAD, getlen(COL_HEAD), ob) == -1) {
        return (-1);
    }
    if (ob_append(filename, getlen(filename), ob) == -1) {
        return (-1);
    }
    if (color && ob_append(COL_RESET, getlen(COL_RESET), ob) == -1) {
        return (-1);
    }
    return (ob_append("\n", 1, ob));
}

int emit_rule(t_opts opts, t_outbuf *ob) {
    int color = iscolor(opts, STDOUT_FILENO);
    /* Plain '-' fallback when color is off, so raw pipes stay ASCII-safe. */
    const char *ch = color ? RULE_CHAR : "-";
    size_t chlen = color ? getlen(RULE_CHAR) : 1;
    size_t i = 0;

    if (color && ob_append(COL_HEAD, getlen(COL_HEAD), ob) == -1) {
        return (-1);
    }
    while (i < RULE_WIDTH) {
        if (ob_append(ch, chlen, ob) == -1) {
            return (-1);
        }
        i++;
    }
    if (color && ob_append(COL_RESET, getlen(COL_RESET), ob) == -1) {
        return (-1);
    }
    return (ob_append("\n", 1, ob));
}
