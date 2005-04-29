/*
 * file input/output function prototypes
 *
 * Andy Duplain, BT Customer Systems, Brighton, UK.  duplain@btcs.bt.co.uk
 */

#ifndef __IO_H
#define __IO_H

#include "cproto.h"

Ferror check_stream P__((FILE *fp));
Byte read_byte P__((FILE *ifp));
Halfword read_halfword P__((FILE *ifp));
Word read_word P__((FILE *ifp));
int free_chunk_memory P__((char *ptr));
struct chunkhdr *read_chunkhdr P__((FILE *ifp));
struct chunkent *read_chunkents P__((FILE *ifp, struct chunkhdr *hdr));
char *read_stringtab P__((FILE *ifp, struct chunkent *strent));
struct symbol *read_symboltab P__((FILE *ifp, struct chunkent *strent, int numsyms));
char *read_ident P__((FILE *ifp, struct chunkent *ident));
struct aofhdr *read_aofhdr P__((FILE *ifp, struct chunkent *hdrent));
struct reloc *read_reloc P__((FILE *ifp));

#endif /* __IO_H */
