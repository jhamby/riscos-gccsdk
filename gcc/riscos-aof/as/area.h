/*
 * area.h
 * Copyright © 1992 Niklas Röjemo
 */

#ifndef area_header_included
#define area_header_included

#include "symbol.h"
#include "reloc.h"
#include "lit.h"

#define AREA_INIT		0x00000002
#define AREA_CODE		0x00000200
#define AREA_DATA		0x00000000
#define AREA_COMMONDEF		0x00000400	/* common block definition */
#define AREA_COMMONREF		0x00000800	/* common block reference */
#define AREA_UDATA		0x00001000	/* uninitialised (0-initialised) */
#define AREA_READONLY		0x00002000
#define AREA_PIC		0x00004000
#define AREA_DEBUG		0x00008000
#define AREA_32BITAPCS		0x00010000
#define AREA_REENTRANT		0x00020000
#define AREA_EXTFPSET		0x00040000
#define AREA_NOSTACKCHECK	0x00080000
#define AREA_BASED		0x00100000
#define AREA_STUBDATA		0x00200000
#define AREA_LINKONCE		0x10000000 /* GNU linkonce (extension) */

#define AREA_IMAGE(x) (!((x)->type&AREA_UDATA))
#define AREA_NOSPACE(x,v) ((x)->imagesize < v)
typedef struct AREA
{
  Symbol *next;
  unsigned int type;
  int imagesize;
  unsigned char *image;
  int norelocs;
  Reloc *relocs;
  LitInfo *lits;		/* ldr reg,=value */
}
Area;

extern Symbol *areaCurrent;
extern Symbol *areaEntry;
extern int areaEntryOffset;
extern Symbol *areaHead;

extern void areaError (void);		/* report "No area defined" */
extern void areaInit (void);
extern void areaFinish (void);
extern void areaGrow (Area * area, int mingrow);
extern void areaPrint (Symbol * area);

#endif
