#ifndef OUTBUF_H
#define OUTBUF_H

#include <stddef.h>

#define BUF_SIZE 4096

typedef struct s_outbuf {
    char data[BUF_SIZE];
    size_t len;
} t_outbuf;

int ob_flush(t_outbuf *outbuf);
int ob_append(const char *s, size_t len, t_outbuf *outbuf);

#endif
