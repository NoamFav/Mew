#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../src/util/base64.h"

void test_rfc4648_vectors() {
    char out[100];
    base64_encode((const unsigned char *)"", 0, out);
    assert(strcmp(out, "") == 0);

    base64_encode((const unsigned char *)"f", 1, out);
    assert(strcmp(out, "Zg==") == 0);

    base64_encode((const unsigned char *)"fo", 2, out);
    assert(strcmp(out, "Zm8=") == 0);

    base64_encode((const unsigned char *)"foo", 3, out);
    assert(strcmp(out, "Zm9v") == 0);

    base64_encode((const unsigned char *)"foob", 4, out);
    assert(strcmp(out, "Zm9vYg==") == 0);

    base64_encode((const unsigned char *)"fooba", 5, out);
    assert(strcmp(out, "Zm9vYmE=") == 0);

    base64_encode((const unsigned char *)"foobar", 6, out);
    assert(strcmp(out, "Zm9vYmFy") == 0);

    printf("✅ Base64 RFC 4648 test vectors passed\n");
}

int main() {
    test_rfc4648_vectors();
    return 0;
}
