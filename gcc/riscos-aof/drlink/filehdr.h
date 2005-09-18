/*
** Drlink AOF linker
**
** Copyright (c) 1993, 1994, 1995, 1996, 1997, 1998  David Daniels
** Copyright (c) 2001, 2002, 2003, 2004, 2005  GCCSDK Developers
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
** This file contains structures and definitions relating to AOF
** files plus anything else to do with file handling
*/

#ifndef filehdr_h
#define filehdr_h

#include <stdio.h>

#include "chunkhdr.h"
#include "areahdr.h"
#include "symbolhdr.h"

#define MINBUFFER 0x2000	/* Minumum image file buffer size - 8K bytes */
#define STDBUFFER 0x10000	/* Default image file buffer size - 64K bytes */

/* Linker output file type */
typedef enum
{
  NOTYPE,
  AIF,
  AOF,
  ALF,
  BIN,
  RMOD,
  RELOC
} image;

/* Type of file currently being read */
typedef enum
{
  NOWT,				/* Invalid file for linking purposes */
  AOFILE,			/* AOF */
  LIBRARY,			/* ALF, new style, i.e. with LIB_VSRN chunk */
  OLDLIB			/* ALF, odl style, i.e. without LIB_VSRN chunk */
} fileinfo;

typedef struct linkfiles
{				/* AOF File to be included in link */
  const char *linkname;		/* Name of a file to link */
  struct linkfiles *linknext;	/* Pointer to next file in link list */
} linkfiles;

typedef struct debugfiles
{				/* Name of file whose debugging info is to be kept */
  const char *dbgname;		/* Name of file where debug is wanted */
  int dbghash;			/* Hashed version of name */
  bool dbgread;			/* TRUE if file was loaded */
  struct debugfiles *dbgnext;	/* Pointer to next file */
} debugfiles;

typedef struct
{
  /* OBJ_HEAD */
  objheadhdr *headptr;		/* Pointer to OBJ_HEAD loaded chunk */
  unsigned int headsize;	/* Size of OBJ_HEAD chunk */
  /* OBJ_AREA */
  unsigned int *areaptr;	/* Pointer to OBJ_AREA loaded chunk */
  unsigned int areasize;	/* Size of OBJ_AREA chunk */
  /* OBJ_SYMT */
  symtentry *symtptr;		/* Pointer to OBJ_SYMT loaded chunk */
  unsigned int symtsize;	/* Size of OBJ_SYMT chunk */
  /* OBJ_STRT */
  char *strtptr;		/* Pointer to OBJ_STRT loaded chunk */
  unsigned int strtsize;	/* Size of OBJ_STRT chunk */
} obj_overview;

/*
** 'filelist' is the structure that contains details of an AOF file loaded
** into memory.
*/
typedef struct filelist
{
  const char *chfilename;	/* Name of this file. Useful for error messages */
  unsigned int chfilehash;	/* Hashed version of file's name */
  unsigned int chfilesize;	/* Size of file being processed */
  bool keepdebug;		/* TRUE if debug areas in file are to be kept */
  bool aofv3;			/* TRUE if object code is AOF version 3 */

  obj_overview obj;		/* obj chunk overview */

  unsigned int areacount;	/* Number of areas in OBJ_AREA chunk */
  unsigned int symtcount;	/* Number of symbols in OBJ_SYMT chunk */

  symtable localsyms;		/* File's local symbol table */
  union
  {
    symbol *wantedsyms;		/* List of external symbols */
    indextable *symtlookup;	/* Old->new SYMT index lookup table */
  } symtries;
  symbol *strongrefs;		/* List of refs to strong symbols to be resolved */
  struct filelist *nextfile;	/* Pointer to next file list entry */
} filelist;

extern image imagetype;		/* What sort of file linker will produce */

extern filelist * aofilelist,	/* Pointer to list of loaded files */
 *aofilelast;			/* Pointer to last loaded file entry */

extern FILE * imagefile,	/* Image file being produced */
 *symbolfile,			/* Symbol file being written */
 *mapfile;			/* Area map file */

extern char *symbolname,	/* Pointer to name of symbol file */
 *mapfilename;			/* Pointer to name of map file */

extern const char *imagename;	/* Pointer to name of linker output file */

extern const char *objectname;	/* AOF/ALF file currently being read */

extern unsigned int buffersize,	/* Size of image file buffer */
  headersize,			/* Size of AIF (or BIN) header */
  debugsize,			/* Size of debugging tables in image */
  imagesize;			/* Size of image file to be written to disk */

extern linkfiles *linklist,	/* Pointer to list of files to link */
 *linklast;			/* Pointer to end of file list */

extern debugfiles *debugflist;	/* List of files where debug info is to be kept */

extern char *cvptr,		/* Pointer to whichever buffer is being used */
 *cvptrend,			/* Pointer to end of a buffer */
 *lastcvptr;			/* Saved pointer */

#endif
