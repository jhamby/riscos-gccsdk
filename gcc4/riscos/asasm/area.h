/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2001-2010 GCCSDK Developers
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * area.h
 */

#ifndef area_header_included
#define area_header_included

#include <stdbool.h>
#include "symbol.h"
#include "reloc.h"
#include "lit.h"

/* Lowest 8 bits encode the alignment of the start of the area as a power
   of 2 and has a value between 2 and 32.  */
#define AREA_INIT		0x00000002
#define AREA_ABS		0x00000100
#define AREA_CODE		0x00000200
#define AREA_COMMONDEF		0x00000400 /* Common block definition */
#define AREA_COMMONREF		0x00000800 /* Common block reference */
#define AREA_UDATA		0x00001000 /* Uninitialised (0-initialised) */
#define AREA_READONLY		0x00002000
#define AREA_PIC		0x00004000
#define AREA_DEBUG		0x00008000
#define AREA_32BITAPCS		0x00010000 /* Code area only */
#define AREA_REENTRANT		0x00020000 /* Code area only */
#define AREA_EXTFPSET		0x00040000 /* Code area only */
#define AREA_NOSTACKCHECK	0x00080000 /* Code area only */
#define AREA_THUMB		0x00100000 /* Code area only */
#define AREA_HALFWORD		0x00200000 /* Code area only */
#define AREA_INTERWORK		0x00400000 /* Code area only */
#define AREA_BASED		0x00100000 /* Data area only */ /* FIXME: what support is needed for this ? */
#define AREA_STUBDATA		0x00200000 /* Data area only */
#define AREA_RESERVED22		0x00400000
#define AREA_RESERVED23		0x00800000
#define AREA_MASKBASEREGS	0x0F000000 /* Base regs, data area only */
#define AREA_LINKONCE		0x10000000 /* GNU linkonce (GCCSDK extension) Normally a reserved bit. */
#define AREA_RESERVED29		0x20000000
#define AREA_RESERVED30		0x40000000
#define AREA_SOFTFLOAT		0x80000000 /* Avoids FP instructions (GCCSDK extension) Normally reserved bit. */

#define AREA_IMAGE(x) (!((x)->type & AREA_UDATA))
#define AREA_NOSPACE(x,v) ((x)->imagesize < v)
typedef struct AREA
{
  Symbol *next;			/** The following area symbol.  */
  unsigned int type;		/* See AREA_ #defines */
  int imagesize;
  unsigned char *image;
  int norelocs;
  Reloc *relocs;
  LitInfo *lits;		/* ldr reg,=value */
} Area;

extern Symbol *areaCurrentSymbol; /** Symbol of the area which is currently being processed.  */
extern Symbol *areaEntrySymbol; /** Symbol of area which has been marked as ENTRY point.  */
extern int areaEntryOffset;
extern Symbol *areaHeadSymbol; /** Start of the linked list of all area symbols seen so far.  */

void areaError (void);		/* report "No area defined" */
void areaInit (void);
void areaFinish (void);
void areaGrow (Area *area, int mingrow);

bool Area_IsImplicit (const Symbol *sym);

bool c_align (void);
bool c_area (void);
bool c_reserve (void);
bool c_entry (void);

#endif
