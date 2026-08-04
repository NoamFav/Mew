#ifndef RENDER_H
#define RENDER_H
#include "out/display_file.h"
#include "out/outbuf.h"
#include "parser.h"

int is_special(unsigned char c, t_opts opts);
int emit_special(unsigned char c, t_opts opts, t_outbuf *ob);
int emit_prefix(t_linestate *ls, t_opts o, t_outbuf *ob, int blank);

#endif
