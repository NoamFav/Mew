#ifndef DECOR_H
#define DECOR_H

#include "options.h"
#include "out/outbuf.h"

int emit_header(const char *filename, t_opts opts, t_outbuf *ob);
int emit_rule(t_opts opts, t_outbuf *ob);

#endif
