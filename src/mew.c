#include "mew.h"
#include "display_file.h"
#include "helpers.h"
#include <fcntl.h>
#include <unistd.h>

int display_loop(int argc, char **argv, int i, int error) {
    int fd;

    if (argc == 1) {
        return (display_file(0, "-"));
    }
    while (++i < argc) {

        if (argv[i][0] == '-' && argv[i][1] == '\0') {
            fd = 0;
        } else {
            fd = open(argv[i], O_RDONLY);
        }

        if (fd != -1) {
            if (display_file(fd, argv[i]) == 1) {
                error = 1;
            }
            if (fd > 0) {
                close(fd);
            }
        } else {
            error = file_error(argv[i]);
        }
    }
    return (error);
}

int mew(int argc, char **argv) {
    return (display_loop(argc, argv, 0, 0));
}
