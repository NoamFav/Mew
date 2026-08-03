#ifndef OUTBUF_H
#define OUTBUF_H

#include <stddef.h>

#define BUF_SIZE 4096

typedef struct s_outbuf {
    char data[BUF_SIZE];
    size_t len;
} t_outbuf;

void flush_out(t_outbuf *outbuf);
void append(const char *s, size_t len, t_outbuf *outbuf);

#endif
