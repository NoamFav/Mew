#ifndef ERROR_H
#define ERROR_H

#include <errno.h>
#include <stdlib.h>
#include <string.h>

int file_error(const char *name);
void usage_exit(const char *prog, int c);

#endif
