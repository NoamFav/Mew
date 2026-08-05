#include "out/outbuf.h"
#include "util/io.h"
#include "util/str.h"
#include <unistd.h>

int ob_flush(t_outbuf *outbuf) {
    int ret = 0;

    if (outbuf->len > 0) {
        ret = write_all(STDOUT_FILENO, outbuf->data, outbuf->len);
        outbuf->len = 0;
    }
    return (ret);
}

int ob_append(const char *s, size_t len, t_outbuf *outbuf) {
    if (len >= BUF_SIZE) {
        if (ob_flush(outbuf) == -1) {
            return (-1);
        }
        return (write_all(STDOUT_FILENO, s, len));
    }
    if (outbuf->len + len > BUF_SIZE && ob_flush(outbuf) == -1) {
        return (-1);
    }
    nf_memcpy(outbuf->data + outbuf->len, s, len);
    outbuf->len += len;
    return (0);
}
