#ifndef COLORS_H
#define COLORS_H

#include "options.h"
#define COL_RESET "\033[0m"
#define COL_HEAD "\033[1;33m"
#define COL_FLAG "\033[1;36m"
#define COL_ERR "\033[1;31m"
#define COL_VERSION "\033[1;35m"

#define COL_BLACK "\033[30m"
#define COL_RED "\033[31m"
#define COL_GREEN "\033[32m"
#define COL_YELLOW "\033[33m"
#define COL_BLUE "\033[34m"
#define COL_MAGENTA "\033[35m"
#define COL_CYAN "\033[36m"
#define COL_WHITE "\033[37m"

#define COL_BRIGHT_BLACK "\033[90m"
#define COL_BRIGHT_RED "\033[91m"
#define COL_BRIGHT_GREEN "\033[92m"
#define COL_BRIGHT_YELLOW "\033[93m"
#define COL_BRIGHT_BLUE "\033[94m"
#define COL_BRIGHT_MAGENTA "\033[95m"
#define COL_BRIGHT_CYAN "\033[96m"
#define COL_BRIGHT_WHITE "\033[97m"

void col_on(int fd, int color, const char *code);
void col_off(int fd, int color);
void put_c(int fd, int color, const char *code, const char *s);
int iscolor(t_opts opts, int fd);
#endif
