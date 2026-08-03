#include "out/outbuf.h"
#include "util/io.h"
#include "util/str.h"

void ob_flush(t_outbuf *outbuf) {
    if (outbuf->len > 0) {
        write_all(STDOUT_FILENO, outbuf->data, outbuf->len);
        outbuf->len = 0;
    }
}

void ob_append(const char *s, size_t len, t_outbuf *outbuf) {
    if (len >= BUF_SIZE) {
        ob_flush(outbuf);
        write_all(STDOUT_FILENO, s, len);
        return;
    }
    if (outbuf->len + len > BUF_SIZE)
        ob_flush(outbuf);
    nf_memcpy(outbuf->data + outbuf->len, s, len);
    outbuf->len += len;
}
