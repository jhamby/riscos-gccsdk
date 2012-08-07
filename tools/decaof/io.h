/*
 * file input/output function prototypes
 *
 * Copyright (c) 1992 Andy Duplain, andy.duplain@dsl.pipex.com
 * Copyright (c) 2005-2012 GCCSDK Developers
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#ifndef IO_HEADER_INCLUDED
#define IO_HEADER_INCLUDED

#include <stdio.h>

typedef struct
{
  uint32_t size;
  char str[];
} aof_obj_strt;

extern Ferror check_stream (FILE *fp);
extern Byte read_byte (FILE *ifp);
extern Halfword read_halfword (FILE *ifp);
extern Word read_word (FILE *ifp);
extern void free_chunk_memory (const void *ptr);
extern struct chunkhdr *read_chunkhdr (FILE *ifp);
extern struct chunkent *read_chunkents (FILE *ifp, struct chunkhdr *hdr);
extern const aof_obj_strt *read_stringtab (FILE *ifp, struct chunkent *strent);
extern struct symbol *read_symboltab (FILE *ifp, struct chunkent *strent,
				      int numsyms);
extern char *read_ident (FILE *ifp, struct chunkent *ident);
extern struct aofhdr *read_aofhdr (FILE *ifp, struct chunkent *hdrent);
extern struct reloc *read_reloc (FILE *ifp);

#endif
