#include "util/usage.h"
#include "util/colors.h"
#include "util/io.h"
#include "util/str.h"
#include <stdlib.h>
#include <unistd.h>

static void put_errch(char c) {
    write_all(STDERR_FILENO, &c, 1);
}

static void put_fd(int fd, const char *s) {
    write_all(fd, s, getlen(s));
}

static void put_err(const char *s) {
    put_fd(STDERR_FILENO, s);
}

static void put_str(const char *s) {
    put_fd(STDOUT_FILENO, s);
}

static void put_opt(int color, const char *flag, const char *desc) {
    put_c(STDOUT_FILENO, color, COL_FLAG, flag);
    put_str(desc);
}

static void print_usage_trailer(const char *prog, int color) {
    put_c(STDERR_FILENO, color, COL_HEAD, "Usage: ");
    put_err(prog);
    put_err(" [OPTIONS] [FILE]...\n");
    put_err("see: ");
    put_err(prog);
    put_err(" -h / --help for more information\n");
}

void usage_exit(const char *prog, int c, t_opts opts) {
    int color = iscolor(opts, STDERR_FILENO);

    col_on(STDERR_FILENO, color, COL_ERR);
    put_err(prog);
    put_err(": invalid option -- '");
    put_errch((char)c);
    put_err("'");
    col_off(STDERR_FILENO, color);
    put_err("\n");
    print_usage_trailer(prog, color);
    exit(1);
}

void usage_exit_long(const char *prog, const char *s, t_opts opts) {
    int color = iscolor(opts, STDERR_FILENO);

    col_on(STDERR_FILENO, color, COL_ERR);
    put_err(prog);
    put_err(": unknown options '");
    put_err(s);
    put_err("'");
    col_off(STDERR_FILENO, color);
    put_err("\n");
    print_usage_trailer(prog, color);
    exit(1);
}

void range_error_exit(const char *prog, const char *s, t_opts opts) {
    int color = iscolor(opts, STDERR_FILENO);

    col_on(STDERR_FILENO, color, COL_ERR);
    put_err(prog);
    put_err(": invalid line range '");
    put_err(s);
    put_err("'");
    col_off(STDERR_FILENO, color);
    put_err("\n");
    print_usage_trailer(prog, color);
    exit(2);
}

void print_help(const char *prog, t_opts opts) {
    int color = iscolor(opts, STDOUT_FILENO);

    put_str("A modern alternative to cat\n\n");
    put_c(STDOUT_FILENO, color, COL_HEAD, "Usage:\n");
    put_str(prog);
    put_str(" [OPTIONS] [FILE]...\n\n");
    put_c(STDOUT_FILENO, color, COL_HEAD, "Arguments:\n");
    put_str("  [FILE]...  File(s) to print. Use - or <blank> for stdin\n\n");
    put_c(STDOUT_FILENO, color, COL_HEAD, "Options:\n");
    put_opt(color, "  -A, --show-all", "           equivalent to -vET\n");
    put_opt(color, "  -b, --number-nonblank", "    number nonempty output lines, overrides -n\n");
    put_opt(color, "  -e", "                       equivalent to -vE\n");
    put_opt(color, "  -E, --show-ends", "          display $ at end of each line\n");
    put_opt(color, "  -n, --number", "             number all output lines\n");
    put_opt(color, "  -s, --squeeze-blank", "      suppress repeated empty output lines\n");
    put_opt(color, "  -t", "                       equivalent to -vT\n");
    put_opt(color, "  -T, --show-tabs", "          display TAB characters as ^I\n");
    put_opt(color, "  -u", "                       (ignored)\n");
    put_opt(color, "  -v, --show-nonprinting",
            "   use ^ and M- notation, except for LFD and TAB\n");
    put_opt(color, "  -h, --help", "               display this help and exit\n");
    put_opt(color, "  -V, --version", "            output version information and exit\n");
    exit(0);
}

void print_version(const char *prog, t_opts opts) {
    int color = iscolor(opts, STDOUT_FILENO);

    put_str(prog);
    put_str(": ");
    put_c(STDOUT_FILENO, color, COL_VERSION, VERSION);
    put_str("\n");
    exit(0);
}
