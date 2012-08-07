/*
 * defines types used in decaof
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

#ifndef DECAOF_HEADER_INCLUDED
#define DECAOF_HEADER_INCLUDED

#include <stdint.h>

/*
 * "Word" must be a 4-byte type.
 * "Halfword" must be a 2-byte type.
 * "Byte" must be a 1-byte type.
 */
typedef uint32_t Word;
typedef uint16_t Halfword;
typedef uint8_t Byte;

/*
 * define maximum filename and pathname length
 */
#ifdef CROSS_COMPILE
#define FILENAMELEN	256
#define PATHNAMELEN	1024
#else
#define FILENAMELEN	256
#define PATHNAMELEN	256
#endif

/*
 * stream error enumeration
 */
typedef enum
{
  FNOERR,
  FEND,
  FRWERR
} Ferror;

/*
 * universal date structure
 */
typedef struct
{
  int second;
  int minute;
  int hour;
  int day;
  int month;
  int year;
} Date;

/*
 * Chunk file / AOF structures
 */
struct chunkhdr
{
  Word chunkfileid;
  Word maxchunks;
  Word numchunks;
};

struct chunkent
{
  char chunkid[8];
  Word offset;
  Word size;
};

struct aofhdr
{				/* fixed part of AOF header only */
  Word filetype;
  Word version;
  Word numareas;
  Word numsyms;
  Word entryarea;
  Word entryoffset;
};

#ifdef WORDS_BIGENDIAN
struct areahdr_flags
{
  Halfword zeroes;
  Byte AT;
  Byte AL;
};
#else
struct areahdr_flags
{
  Byte AL;
  Byte AT;
  Halfword zeroes;
};
#endif

struct areahdr
{
  Word name;
  Word flags;
  Word size;
  Word numrelocs;
  Word baseaddr;
};

/*
 * area attributes
 */
#define AREA_ABS		0x00000100
#define AREA_CODE		0x00000200
#define AREA_COMMONDEF		0x00000400	/* Common block definition */
#define AREA_COMMONREF		0x00000800	/* Common block reference */
#define AREA_UDATA		0x00001000	/* Uninitialised (0-initialised) */
#define AREA_READONLY		0x00002000
#define AREA_PIC		0x00004000
#define AREA_DEBUG		0x00008000
#define AREA_32BITAPCS		0x00010000	/* Code area only */
#define AREA_REENTRANT		0x00020000	/* Code area only */
#define AREA_EXTFPSET		0x00040000	/* Code area only */
#define AREA_NOSTACKCHECK	0x00080000	/* Code area only */
#define AREA_THUMB		0x00100000	/* Code area only */
#define AREA_HALFWORD		0x00200000	/* Code area only */
#define AREA_INTERWORK		0x00400000	/* Code area only */
#define AREA_BASED		0x00100000	/* Data area only */
#define AREA_STUBDATA		0x00200000	/* Data area only */
#define AREA_RESERVED22		0x00400000
#define AREA_RESERVED23		0x00800000
#define AREA_MASKBASEREGS	0x0F000000	/* Base regs, data area only */
#define AREA_LINKONCE		0x10000000	/* GNU linkonce (GCCSDK extension) Normally a reserved bit. */
#define AREA_RESERVED29		0x20000000
#define AREA_RESERVED30		0x40000000
#define AREA_SOFTFLOAT		0x80000000	/* Avoids FP instructions (GCCSDK extension) Normally reserved bit. */

#define AREA_UNKNOWNBITS	0x60E00000

struct reloc
{
  Word offset;
  Word flags;			/* varies depending on type */
};

struct symbol
{
  Word name;
  Word flags;
  Word value;
  Word areaname;
};


#endif /* DECAOF_HEADER_INCLUDED */
