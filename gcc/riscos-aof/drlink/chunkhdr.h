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
** Chunk file definitions
*/

#ifndef chunkhdr_h
#define chunkhdr_h

#include "symbolhdr.h"

#define CHUNKFILE 0xC3CBC6C5	/* Says file uses Acorn chunk file format */
#define BIGENDIAN 0xC5C6CBC3	/* Big endian chunk file */
#define OBJ_XXXX 0x5F4A424F	/* Hex versions of the names of chunk types */
#define LIB_XXXX 0x5F42494C
#define OFL_XXXX 0x5F4C464F

#define OBJ_HEAD 0x44414548
#define OBJ_IDFN 0x4E464449
#define OBJ_AREA 0x41455241
#define OBJ_SYMT 0x544D5953
#define OBJ_STRT 0x54525453
#define LIB_DIRY 0x59524944
#define LIB_VSRN 0x4E535256
#define LIB_DATA 0x41544144
#define LIB_TIME 0x454D4954
#define OFL_TIME 0x454D4954
#define OFL_SYMT 0x544D5953

#define OBJFILETYPE 0xC5E2D080	/* Marks file as an AOF file */
#define AOFVERSION 311		/* Highest version of AOF supported */
#define AOFVER2 200		/* AOF version 2 */
#define AOFVER3 300		/* AOF version 3 */
#define AOFVER31 311		/* AOF version 3.11 */

typedef struct
{
  unsigned int chunkfileid;	/* Identifies file as a chunk file */
  unsigned int maxchunks;	/* Maximum no. of chunks in file */
  unsigned int numchunks;	/* Actual no. of chunks in file */
} chunkheader;

typedef struct
{
  unsigned int chunkclass;	/* What chunk contains */
  unsigned int chunktype;
  unsigned int chunkoffset;	/* Offset in file of chunk.  Must be divisible by 4.  If 0, this chunk is unused. */
  unsigned int chunksize;	/* Size of chunk. Not necessarily divisible by 4. */
} chunkindex;

typedef struct
{				/* Used when checking the start of a chunk file */
  chunkheader header;		/* Chunk file header */
  chunkindex firstentry;	/* First entry in chunk index */
} chunkhdr;

extern chunkindex *chunkhdrbase;	/* Address of file's chunk header when in memory */
extern unsigned int chunkcount;	/* Count of chunks in file */

extern symtentry *symtbase;	/* Address of OFL_SYMT chunk when in memory */

extern unsigned int *dirybase,	/* Address of LIB_DIRY chunk when in memory */
 *vsrnbase;			/* Address of LIB_VSRN chunk when in memory */

extern char *strtbase;		/* Address of OBJ_STRT chunk when in memory */

extern unsigned int symtsize,	/* Size of OFL_SYMT chunk */
  strtsize,			/* Size of OBJ_STRT chunk */
  dirysize,			/* Size of LIB_DIRY chunk */
  vsrnsize;			/* Size of LIB_VSRN chunk */

#endif
