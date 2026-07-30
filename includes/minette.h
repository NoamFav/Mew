#ifndef MINETTE_H
#define MINETTE_H

#define PROGNAME "mew"

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int display_loop(int argc, char **argv, int i, int error);
int minette(int argc, char **filename);
int display_file(int fd, const char *name);
int file_error(const char *name);
int getlen(const char *str);

#endif
