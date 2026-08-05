#include "util/error.h"
#include "mew.h"
#include "util/io.h"
#include "util/str.h"

static void put_err(const char *s) {
    write_all(STDERR_FILENO, s, getlen(s));
}

static void put_str(const char *s) {
    write_all(STDOUT_FILENO, s, getlen(s));
}

int file_error(const char *name) {
    put_err(PROGNAME ": ");
    put_err(name);
    put_err(": ");
    put_err(strerror(errno));
    put_err("\n");
    return (1);
}

void usage_exit(const char *prog, int c) {
    char ch;

    ch = (char)c;
    put_err(prog);
    put_err(": invalid option -- '");
    write_all(STDERR_FILENO, &ch, 1);
    put_err("'\n");
    put_err("usage: ");
    put_err(prog);
    put_err(" [-benstuvAET] [file ...]\n");
    exit(1);
}

void usage_exit_long(const char *prog, const char *s) {

    put_err(prog);
    put_err(": unknown options '");
    write_all(STDERR_FILENO, s, getlen(s));
    put_err("'\n");
    put_err("see: ");
    put_err(prog);
    put_err(" -h / --help for more information\n");
    exit(1);
}

void print_help(const char *prog) {
    put_str("A modern alternative to cat\n\n");
    put_str("Usage:\n");
    put_str(prog);
    put_str(" [OPTIONS] [FILE]...\n\n");
    put_str("Arguments:\n");
    put_str("  [FILE]...  File(s) to print. Use - or <blank> for stdin\n\n");
    put_str("Options:\n");
    put_str("  -A, --show-all           equivalent to -vET\n");
    put_str("  -b, --number-nonblank    number nonempty output lines, overrides -n\n");
    put_str("  -e                       equivalent to -vE\n");
    put_str("  -E, --show-ends          display $ at end of each line\n");
    put_str("  -n, --number             number all output lines\n");
    put_str("  -s, --squeeze-blank      suppress repeated empty output lines\n");
    put_str("  -t                       equivalent to -vT\n");
    put_str("  -T, --show-tabs          display TAB characters as ^I\n");
    put_str("  -u                       (ignored)\n");
    put_str("  -v, --show-nonprinting   use ^ and M- notation, except for LFD and TAB\n");
    put_str("  -h, --help               display this help and exit\n");
    put_str("  -V, --version            output version information and exit\n");
    exit(0);
}

void print_version(const char *prog) {
    put_str(prog);
    put_str(": ");
    put_str(VERSION);
    put_str("\n");
    exit(0);
}
