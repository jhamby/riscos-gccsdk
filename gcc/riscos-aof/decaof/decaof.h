/*
 * defines types used in decaof
 *
 * Andy Duplain, BT Customer Systems, Brighton, UK.  duplain@btcs.bt.co.uk
 */

#ifndef __DECAOF_H
#define __DECAOF_H

/*
 * globalise system-type defines...
 */
#if defined(BSD42) || defined(BSD43) || defined(BSD44)
#define UNIX
#define BSD
#endif
#if defined(SYSV2) || defined(SYSV3) || defined (SYSV4)
#define UNIX
#define SYSV
#endif
#if defined(RISCOS2) || defined(RISCOS3)
#define RISCOS
#define LITTLE_ENDIAN
#endif
#if defined(MSDOS2) || defined(MSDOS3) || defined(MSDOS4) || defined(MSDOS5)
#define MSDOS
#define LITTLE_ENDIAN
#endif

#ifdef UNIX
#include <endian.h>
#endif

/*
 * "Word" must be a 4-byte type.
 * "Halfword" must be a 2-byte type.
 * "Byte" must be a 1-byte type.
 */
#ifdef MSDOS
typedef unsigned long	Word;
typedef	unsigned int	Halfword;
typedef unsigned char	Byte;
#else /* not MSDOS */
typedef unsigned int	Word;
typedef unsigned short	Halfword;
typedef unsigned char	Byte;
#endif /* MSDOS */

/*
 * define the path seperator character, and file open mode string.
 */
#if defined(UNIX)
#define PATHSEP	'/'
#define PATHSEPSTR	"/"
#define W_OPENMODE	"w"
#define R_OPENMODE	"r"
#endif
#if defined(RISCOS)
#define PATHSEP	'.'
#define PATHSEPSTR	"."
#define W_OPENMODE	"w"
#define R_OPENMODE	"r"
#endif
#if defined(MSDOS)
#define PATHSEP	'\\'
#define PATHSEPSTR	"\\"
#define W_OPENMODE	"wb"
#define R_OPENMODE	"rb"
#endif

/*
 * define maximum filename and pathname length
 */
#if defined(BSD) || defined(SYSV4)
#define FILENAMELEN	256
#define PATHNAMELEN	1024
#endif
#if defined(SYSV) && !defined(SYSV4)
#define FILENAMELEN	14
#define PATHNAMELEN	256
#endif
#if defined (RISCOS)
#define FILENAMELEN	10
#define PATHNAMELEN	256
#endif
#if defined (MSDOS)
#define FILENAMELEN	12		/* including dot */
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

#if defined(LITTLE_ENDIAN)
struct areahdr_flags {
	Byte AL;
	Byte AT;
	Halfword zeroes;
};
#else /* big endian assumed */
struct areahdr_flags {
	Halfword zeroes;
	Byte AT;
	Byte AL;
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


#endif /* __DECAOF_H */
