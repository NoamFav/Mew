#ifndef ERROR_H
#define ERROR_H

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define PROGNAME "mew"

int file_error(const char *name);
void usage_exit(const char *prog, int c);
void usage_exit_long(const char *prog, const char *s);
void print_help(const char *prog);
void print_version(const char *prog);

#endif
