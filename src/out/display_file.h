#ifndef DISPLAY_FILE_H
#define DISPLAY_FILE_H

#include "parser.h"
#include <unistd.h>

#define NUM_WIDTH 6
#define SPACES "      "
#define READ_SIZE 4096

typedef struct s_linestate {
    size_t line_num;
    int at_bol;
    int nl_run;
} t_linestate;

int display_file(int fd, t_opts opts, const char *name);

#endif
