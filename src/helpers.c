#include "../includes/minette.h"

int getlen(const char *str) {
    int(i) = 0;
    while (str[i]) {
        i++;
    }
    return (i);
}

int file_error(const char *name) {
    (void)write(2, PROGNAME ": ", getlen(PROGNAME ": "));
    (void)write(2, name, getlen(name));
    (void)write(2, ": ", 2);
    (void)write(2, strerror(errno), getlen(strerror(errno)));
    (void)write(2, "\n", 1);
    return (1);
}
