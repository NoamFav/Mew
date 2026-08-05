#ifndef COLORS_H
#define COLORS_H

#include "options.h"
#define COL_RESET "\033[0m"
#define COL_HEAD "\033[1;33m"
#define COL_FLAG "\033[1;36m"
#define COL_ERR "\033[1;31m"
#define COL_VERSION "\033[1;35m"

void col_on(int fd, int color, const char *code);
void col_off(int fd, int color);
void put_c(int fd, int color, const char *code, const char *s);
int iscolor(t_opts opts, int fd);
#endif
