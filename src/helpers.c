#include "../includes/minette.h"

int getlen(const char *str) {
    int(i) = 0;
    while (str[i]) {
        i++;
    }
    return (i);
}

int file_error(const char *name) {
    write(2, PROGNAME ": ", getlen(PROGNAME ": "));
    write(2, name, getlen(name));
    write(2, ": ", 2);
    write(2, strerror(errno), getlen(strerror(errno)));
    write(2, "\n", 1);
    return (1);
}
