#include "files.h"
#include <fcntl.h>
#include <unistd.h>

int open_operand(const char *name) {
    if (name[0] == '-' && name[1] == '\0')
        return (0);
    return (open(name, O_RDONLY));
}

void close_operand(int fd) {
    if (fd > 0)
        close(fd);
}
