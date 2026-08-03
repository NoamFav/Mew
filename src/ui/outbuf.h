#ifndef OUTBUF_H
#define OUTBUF_H

#include "../helpers.h"
#include <stddef.h>

typedef struct s_outbuf {
    char data[BUF_SIZE];
    size_t len;
} t_outbuf;

void flush_out(t_outbuf *outbuf);
void append(const char *s, size_t len, t_outbuf *outbuf);

#endif
