#include "files.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int open_operand(const char *name) {
    struct stat st;
    int fd;

    if (name[0] == '-' && name[1] == '\0')
        return (0);
    fd = open(name, O_RDONLY);
    if (fd == -1)
        return (-1);
    if (fstat(fd, &st) == -1) {
        close(fd);
        return (-1);
    }
    if (S_ISDIR(st.st_mode)) {
        close(fd);
        errno = EISDIR;
        return (-1);
    }
    return (fd);
}

void close_operand(int fd) {
    if (fd > 0)
        close(fd);
}
