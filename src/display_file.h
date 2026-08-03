#ifndef DISPLAY_FILE_H
#define DISPLAY_FILE_H

#include "helpers.h"
#include "parser.h"

typedef struct s_outbuf {
    char data[BUF_SIZE];
    size_t len;
} t_outbuf;

typedef struct s_linestate {
    size_t line_num;
    int at_bol;
} t_linestate;

int display_file(int fd, t_opts opts, const char *name);

#endif
