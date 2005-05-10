/*
 * defines types used in decaof
 *
 * Andy Duplain, BT Customer Systems, Brighton, UK.  duplain@btcs.bt.co.uk
 * Copyright 2005 GCCSDK Developers
 */

#ifndef DECAOF_HEADER_INCLUDED
#define DECAOF_HEADER_INCLUDED

/*
 * "Word" must be a 4-byte type.
 * "Halfword" must be a 2-byte type.
 * "Byte" must be a 1-byte type.
 */
#ifdef HAVE_STDINT_H
#include <stdint.h>
typedef uint32_t Word;
typedef uint16_t Halfword;
typedef uint8_t Byte;
#else
typedef unsigned int	Word;
typedef unsigned short	Halfword;
typedef unsigned char	Byte;
#endif

/*
 * define the path seperator character, and file open mode string.
 */
#ifdef CROSS_COMPILE
#define PATHSEP	'/'
#define PATHSEPSTR	"/"
#define W_OPENMODE	"w"
#define R_OPENMODE	"r"
#else
#define PATHSEP	'.'
#define PATHSEPSTR	"."
#define W_OPENMODE	"w"
#define R_OPENMODE	"r"
#endif


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
typedef enum {FNOERR, FEND, FRWERR} Ferror;

/*
 * universal date structure
 */
typedef struct {
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

struct chunkhdr {
	Word chunkfileid;
	Word maxchunks;
	Word numchunks;
};

struct chunkent {
	char chunkid[8];
	Word offset;
	Word size;
};

struct aofhdr {			/* fixed part of AOF header only */
	Word filetype;
	Word version;
	Word numareas;
	Word numsyms;
	Word entryarea;
	Word entryoffset;
};

#ifdef WORDS_BIGENDIAN
struct areahdr_flags {
	Halfword zeroes;
	Byte AT;
	Byte AL;
};
#else
struct areahdr_flags {
	Byte AL;
	Byte AT;
	Halfword zeroes;
};
#endif

struct areahdr {
	Word name;
	Word flags;
	Word size;
	Word numrelocs;
	Word reserved;
};

/*
 * area attributes (areahdr->flags.AT)
 */
#define AREA_CODE		0x000002
#define AREA_COMMONDEF		0x000004 /* Common block definition       */
#define AREA_COMMONREF		0x000008 /* Common block reference        */
#define AREA_UDATA		0x000010 /* Uninitialised (0-initialised) */
#define AREA_READONLY		0x000020
#define AREA_PIC		0x000040
#define AREA_DEBUG		0x000080
#define AREA_32BITAPCS		0x000100
#define AREA_REENTRANT		0x000200
#define AREA_EXTFPSET		0x000400
#define AREA_NOSTACKCHECK	0x000800
#define AREA_BASED		0x001000
#define AREA_STUBDATA		0x002000
#define AREA_SOFTFLOAT		0x080000 /* Avoids FP instructions        */
#define AREA_LINKONCE		0x100000 /* GNU linkonce (extension)      */

struct reloc {
	Word offset;
	Word flags;		/* varies depending on type */
};

struct symbol {
	Word name;
	Word flags;
	Word value;
	Word areaname;
};


#endif /* DECAOF_HEADER_INCLUDED */
