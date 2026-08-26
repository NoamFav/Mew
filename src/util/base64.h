#ifndef MEW_BASE64_H
#define MEW_BASE64_H

#include <stddef.h>

size_t base64_encode(const unsigned char *src, size_t len, char *out, size_t out_size);

#endif
