#ifndef HELPERS_H
#define HELPERS_H

#define PROGNAME "mew"
#define BUF_SIZE 4096
#define NUM_WIDTH 6

#include <errno.h>
#include <string.h>
#include <unistd.h>
int getlen(const char *str);
int file_error(const char *name);
int write_all(int fd, const char *buf, ssize_t len);
void usage_exit(const char *prog, int c);
void *nf_memcpy(void *dst, const void *src, size_t n);
size_t num_to_buf(size_t n, char *num);
size_t count_digits(size_t n);
#endif
