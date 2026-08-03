#ifndef MEW_H
#define MEW_H

#define PROGNAME "mew"
#define BUF_SIZE 4096
#define NUM_WIDTH 6

#include "../src/parser.h"

int mew(int argc, char **argv);
int display_loop(int argc, char **argv, t_opts opts);

#endif
