/*
** Drlink AOF linker
**
** Copyright (c) 2005  GCCSDK Developers
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
**
** This source file contains the structures and definitions used
** for handling libraries.
*/

#ifndef libraries_h
#define libraries_h

#include <stdio.h>

#include "chunkhdr.h"
#include "filehdr.h"

#define ALFV1 1		/* Library is ALF format version 1 */
#define ALFV2 2		/* Library is ALF format version 2 (FIXME: what *is* version 2 ALF ?) */

/* This struct is followed by external symbol name. */
typedef struct
{
  unsigned int chunkindex;	/* Index of chunk containing entry */
  unsigned int entrysize;	/* Size of OFL_SYMT entry */
  unsigned int datasize;	/* Size of data portion of OFL_SYMT entry */
} libsymtentry;

/* This struct is followed by the library member name (potentionally also
 * other relevant library module information and a 2 word, word-aligned,
 * time stamp).
 */
typedef struct
{
  unsigned int dataindex;	/* Index of LIB_DATA chunk */
  unsigned int dirylen;		/* Size of LIB_DIRY entry */
  unsigned int dirydatalen;	/* Size of data portion of LIB_DIRY entry */
} libdiryentry;

/* LIB_XXXX overview:  */
typedef struct
{
  chunkhdr *base;		/* Address of library in memory (fullyloaded == TRUE) or address of all chunk indexes (fullyloaded == FALSE).  Can be NULL when fullyloaded == FALSE and open_library() hasn't been called yet. */
  /* OFL_SYMT */
  libsymtentry *symtptr;	/* Address of OFL_SYMT chunk when in memory */
  unsigned int symtsize;	/* Size of OFL_SYMT chunk */

  /* LIB_DIRY */
  libdiryentry *diryptr;	/* Address of LIB_DIRY chunk when in memory */
  unsigned int dirysize;	/* Size of LIB_DIRY chunk */

  /* LIB_VSRN */
  unsigned int *vsrnptr;	/* Address of LIB_VSRN chunk when in memory */
  unsigned int vsrnsize;	/* Size of LIB_VSRN chunk */
} lib_overview;

/* Library symbol table entry */
typedef struct libentry
{
  struct libentry *left;		/* Left in binary tree */
  struct libentry *right;		/* Right in binary tree */
  int libhash;				/* Hash value of library entry name */
  const char *libname;			/* Pointer to library *entry* name */
  const char *libmember;		/* Pointer to name of entry's library member */
  unsigned int liboffset;		/* Offset in library of entry's LIB_DATA chunk */
  unsigned int libsize;			/* Size of LIB_DATA chunk */
  const char *alfname;			/* Name of library holding LIB_DATA chunk, not to be confused with libname :-( */
} libentry;

typedef libentry *libtable[MAXENTRIES];	/* Library symbol hash table */

/* Describes a library */
typedef struct libheader
{
  const char *libname;			/* Name of library */
  FILE *fhandle;			/* File handle used to load individual members in case the complete library can't be loaded in one go */
  unsigned int libextent;		/* Size of library */

  lib_overview lib;

  bool fullyloaded;			/* TRUE if library is fully loaded at lib.base; FALSE if only the chunk indexes are available at lib.base */
  bool libgotsyms;			/* TRUE if library symbol table has been read */
  libtable librarysyms;			/* Library's symbol table */

  struct libheader *libflink;		/* Pointer to next library in list */
} libheader;

extern libheader * liblist,	/* Pointer to list of libraries */
 *liblast;			/* Pointer to end of list of libraries */

extern libheader *current_lib;		/* Pointer to libheader entry for library being searched */

extern void init_library (void);
extern bool addto_liblist (const char *libname, const lib_overview *liboverviewp, unsigned int filesize);
extern bool open_library (libheader *);
extern void close_library (libheader *);
extern bool load_member (libentry *, filelist *, symbol *);
extern bool load_wholelib (const char *libname, chunkhdr *ch, unsigned int filesize);
extern bool isloaded (libentry *);
extern void free_libmem (void);
extern bool discard_libraries (void);

extern bool lib_check_and_adjust (chunkhdr * ch, const char *filename, unsigned int filesize, lib_overview *liboverviewp);

#endif
