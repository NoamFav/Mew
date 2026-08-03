#ifndef IO_H
#define IO_H

#include <unistd.h>

int write_all(int fd, const char *buf, ssize_t len);
#endif
