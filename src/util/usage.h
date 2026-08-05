#ifndef USAGE_H
#define USAGE_H

#include <stdlib.h>

#define VERSION "v1.1.0"
void print_help(const char *prog);
void print_version(const char *prog);
void usage_exit(const char *prog, int c);
void usage_exit_long(const char *prog, const char *s);

#endif
