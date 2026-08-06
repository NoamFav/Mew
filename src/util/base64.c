#include "base64.h"

static const char b64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

size_t base64_encode(const unsigned char *src, size_t len, char *out) {
    size_t i = 0, j = 0;
    for (; i + 2 < len; i += 3) {
        out[j++] = b64_table[(src[i] >> 2) & 0x3F];
        out[j++] = b64_table[((src[i] & 0x03) << 4) | ((src[i + 1] >> 4) & 0x0F)];
        out[j++] = b64_table[((src[i + 1] & 0x0F) << 2) | ((src[i + 2] >> 6) & 0x03)];
        out[j++] = b64_table[src[i + 2] & 0x3F];
    }
    if (i < len) {
        out[j++] = b64_table[(src[i] >> 2) & 0x3F];
        if (i + 1 < len) {
            out[j++] = b64_table[((src[i] & 0x03) << 4) | ((src[i + 1] >> 4) & 0x0F)];
            out[j++] = b64_table[(src[i + 1] & 0x0F) << 2];
            out[j++] = '=';
        } else {
            out[j++] = b64_table[(src[i] & 0x03) << 4];
            out[j++] = '=';
            out[j++] = '=';
        }
    }
    out[j] = '\0';
    return j;
}
