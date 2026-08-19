#include "decor.h"
#include "options.h"
#include "util/colors.h"
#include "util/io.h"
#include "util/str.h"
#include <unistd.h>

#define RULE_CHAR "\xe2\x94\x80" /* U+2500 BOX DRAWINGS LIGHT HORIZONTAL */
#define RULE_WIDTH 40

static void show_rule(int color) {
    size_t i = 0;

    col_on(STDOUT_FILENO, color, COL_HEAD);
    while (i < RULE_WIDTH) {
        write_all(STDOUT_FILENO, RULE_CHAR, getlen(RULE_CHAR));
        i++;
    }
    col_off(STDOUT_FILENO, color);
    write_all(STDOUT_FILENO, "\n", 1);
}

void show_header(const char *filename, t_opts opts) {
    int color = iscolor(opts, STDOUT_FILENO);

    put_c(STDOUT_FILENO, color, COL_HEAD, filename);
    write_all(STDOUT_FILENO, "\n", 1);
    if (opts.style & STYLE_RULE) {
        show_rule(color);
    }
}
