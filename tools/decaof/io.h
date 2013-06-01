/*
 * file input/output function prototypes
 *
 * Copyright (c) 1992 Andy Duplain, andy.duplain@dsl.pipex.com
 * Copyright (c) 2005-2013 GCCSDK Developers
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */

#ifndef IO_HEADER_INCLUDED
#define IO_HEADER_INCLUDED

#include <stdio.h>

typedef struct
{
  uint32_t size;
  char str[];
} aof_obj_strt;

Ferror check_stream (FILE *fp);
Byte read_byte (FILE *ifp);
Halfword read_halfword (FILE *ifp);
Word read_word (FILE *ifp);
void free_chunk_memory (const void *ptr);

const struct chunkhdr *read_chunkhdr (FILE *ifp);
const struct chunkent *read_chunkents (FILE *ifp, const struct chunkhdr *hdr);
const aof_obj_strt *read_stringtab (FILE *ifp, const struct chunkent *strent);
const struct symbol *read_symboltab (FILE *ifp, const struct chunkent *strent,
				     uint32_t numSyms);
const char *read_ident (FILE *ifp, const struct chunkent *ident);
const struct aofhdr *read_aofhdr (FILE *ifp, const struct chunkent *hdrent);
const struct reloc *read_reloc (FILE *ifp);

#endif
