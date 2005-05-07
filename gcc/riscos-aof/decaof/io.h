/*
 * file input/output function prototypes
 *
 * Andy Duplain, BT Customer Systems, Brighton, UK.  duplain@btcs.bt.co.uk
 */

#ifndef __IO_H
#define __IO_H

Ferror check_stream (FILE *fp);
Byte read_byte (FILE *ifp);
Halfword read_halfword (FILE *ifp);
Word read_word (FILE *ifp);
int free_chunk_memory (char *ptr);
struct chunkhdr *read_chunkhdr (FILE *ifp);
struct chunkent *read_chunkents (FILE *ifp, struct chunkhdr *hdr);
char *read_stringtab (FILE *ifp, struct chunkent *strent);
struct symbol *read_symboltab (FILE *ifp, struct chunkent *strent, int numsyms);
char *read_ident (FILE *ifp, struct chunkent *ident);
struct aofhdr *read_aofhdr (FILE *ifp, struct chunkent *hdrent);
struct reloc *read_reloc (FILE *ifp);

#endif /* __IO_H */
