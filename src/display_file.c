#include "../includes/minette.h"

int display_file(int fd, const char *name) {
    int line;
    char buf[BUF_SIZE];
    int chunk = sizeof(buf);

    while (1) {
        line = read(fd, buf, chunk);
        if (line == -1) {
            return (file_error(name));
        }
        if (line == 0) {
            return (0);
        }
        write_all(1, buf, line);
    }
    return (0);
}
