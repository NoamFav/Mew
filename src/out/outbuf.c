#include "outbuf.h"
#include "../util/io.h"
#include "../util/str.h"

void flush_out(t_outbuf *outbuf) {
    if (outbuf->len > 0) {
        write_all(STDOUT_FILENO, outbuf->data, outbuf->len);
        outbuf->len = 0;
    }
}

void append(const char *s, size_t len, t_outbuf *outbuf) {
    if (len >= BUF_SIZE) {
        flush_out(outbuf);
        write_all(STDOUT_FILENO, s, len);
        return;
    }
    if (outbuf->len + len > BUF_SIZE)
        flush_out(outbuf);
    nf_memcpy(outbuf->data + outbuf->len, s, len);
    outbuf->len += len;
}
