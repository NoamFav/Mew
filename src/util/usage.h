#ifndef USAGE_H
#define USAGE_H

#include "options.h"
#define VERSION "v2.0.0"
void print_help(const char *prog, t_opts opts);
void print_version(const char *prog, t_opts opts);
void usage_exit(const char *prog, int c, t_opts opts);
void usage_exit_long(const char *prog, const char *s, t_opts opts);

#endif
