#!/bin/sh
# tools/genh.sh src/lexer.c  ->  src/lexer.h
set -e
src=$1
hdr=${src%.c}.h
guard=$(basename "$hdr" | tr 'a-z.' 'A-Z_')

{
  printf '#ifndef %s\n# define %s\n\n' "$guard" "$guard"
  printf '# include "types.h"\n\n'
  cproto -E 0 "$src" 2>/dev/null | grep -v '^/\*'
  printf '\n#endif\n'
} > "$hdr.tmp" && mv "$hdr.tmp" "$hdr"