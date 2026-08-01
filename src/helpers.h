#ifndef HELPERS_H
#define HELPERS_H

#define PROGNAME "mew"
#define BUF_SIZE 4096

#include <errno.h>
#include <string.h>
#include <unistd.h>
int getlen(const char *str);
int file_error(const char *name);
int write_all(int fd, const char *buf, ssize_t len);
#endif
