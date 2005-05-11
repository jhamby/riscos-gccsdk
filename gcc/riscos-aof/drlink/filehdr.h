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


#define MINBUFFER 0x2000		/* Minumum image file buffer size - 8K bytes */
#define STDBUFFER 0x10000		/* Default image file buffer size - 64K bytes */

typedef enum {NOTYPE, AIF, AOF, ALF, BIN, RMOD, RELOC} image;	/* Linker output file type */

typedef enum {NOWT, AOFILE, LIBRARY, OLDLIB} fileinfo;	/* Type of file currently being read */

typedef struct linkfiles {		/* AOF File to be included in link */
  const char *linkname;			/* Name of a file to link */
  struct linkfiles *linknext;		/* Pointer to next file in link list */
} linkfiles;

typedef struct debugfiles {		/* Name of file whose debugging info is to be kept */
  const char *dbgname;			/* Name of file where debug is wanted */
  int dbghash;				/* Hashed version of name */
  bool dbgread;				/* TRUE if file was loaded */
  struct debugfiles *dbgnext;		/* Pointer to next file */
} debugfiles;

/*
** 'filelist' is the structure that contains details of an AOF file loaded
** into memory.
*/
typedef struct filelist {
  char *chfilename;			/* Name of this file. Useful for error messages */
  unsigned int chfilehash;		/* Hashed version of file's name */
  unsigned int chfilesize;		/* Size of file being processed */
  bool keepdebug;			/* TRUE if debug areas in file are to be kept */
  bool cached;				/* TRUE if the AOF file is in memory */
  bool edited;				/* TRUE if the file has been edited */
  bool aofv3;				/* TRUE if object code is AOF version 3 */
  struct objheadhdr *objheadptr;	/* Pointer to OBJ_HEAD loaded chunk */
  unsigned int objheadsize;		/* Size of OBJ_HEAD chunk */
  unsigned int *objareaptr;		/* Pointer to OBJ_AREA loaded chunk */
  unsigned int objareasize;		/* Size of OBJ_AREA chunk */
  struct symtentry *objsymtptr;		/* Pointer to OBJ_SYMT loaded chunk */
  unsigned int objsymtsize;		/* Size of OBJ_SYMT chunk */
  char *objstrtptr;			/* Pointer to OBJ_STRT loaded chunk */
  unsigned int objstrtsize;		/* Size of OBJ_STRT chunk */
  unsigned int areacount;		/* Number of areas in OBJ_AREA chunk */
  unsigned int symtcount;		/* Number of symbols in OBJ_SYMT chunk */
  symtable localsyms;			/* File's local symbol table */
  union {
    symbol *wantedsyms;			/* List of external symbols */
    indextable *symtlookup;		/* Old->new SYMT index lookup table */
  } symtries;
  symbol *strongrefs;			/* List of refs to strong symbols to be resolved */
  struct filelist *nextfile;		/* Pointer to next file list entry */
} filelist;

extern image imagetype;			/* What sort of file linker will produce */

extern filelist
  *aofilelist,				/* Pointer to list of loaded files */
  *aofilelast;				/* Pointer to last loaded file entry */

extern libheader
  *liblist,				/* Pointer to list of libraries */
  *liblast;				/* Pointer to end of list of libraries */

extern chunkheader header;		/* Chunk header from last file read */

extern FILE
  *objectfile,				/* AOF or library being read */
  *imagefile,				/* Image file being produced */
  *symbolfile,				/* Symbol file being written */
  *mapfile;				/* Area map file */

extern char
   *symbolname,				/* Pointer to name of symbol file */
   *mapfilename;			/* Pointer to name of map file */

extern const char
   *imagename;				/* Pointer to name of linker output file */

extern bool
  object_open,				/* TRUE if an object file is being read */
  image_open,				/* TRUE if the image file is being written */
  symbol_open,				/* TRUE if the symbol file is being written */
  map_open;				/* TRUE if the area map file is being written */

extern char objectname[FNAMELEN];	/* File currently being read */

extern unsigned int
  filecount,				/* Count of AOF files loaded */
  buffersize,				/* Size of image file buffer */
  headersize,				/* Size of AIF (or BIN) header */
  debugsize,				/* Size of debugging tables in image */
  imagesize;				/* Size of image file to be written to disk */

extern unsigned int *filebase;		/* Pointer to a file when loaded */

extern linkfiles
  *linklist,				/* Pointer to list of files to link */
  *linklast;				/* Pointer to end of file list */

extern debugfiles *debugflist;		/* List of files where debug info is to be kept */

extern char
  *cvptr,				/* Pointer to whichever buffer is being used */
  *cvptrend,				/* Pointer to end of a buffer */
  *lastcvptr;				/* Saved pointer */

#endif
