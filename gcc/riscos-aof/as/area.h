/*
 * area.h
 * Copyright © 1992 Niklas Röjemo
 */

#ifndef _area_h
#define _area_h

#ifndef _symbol_h
#include "symbol.h"
#endif


#ifndef _reloc_h
#include "reloc.h"
#endif

#ifndef _lit_h
#include "lit.h"
#endif


#define AREA_INIT      0x0002
#define AREA_CODE      0x0200
#define AREA_DATA      0x0000
#define AREA_COMMONDEF 0x0400	/* common block definition */
#define AREA_COMMONREF 0x0800	/* common block reference */
#define AREA_UDATA     0x1000	/* uninitialised (0-initialised) */
#define AREA_READONLY  0x2000
#define AREA_PIC       0x4000
#define AREA_DEBUG     0x8000
#define AREA_32BITAPCS 0x10000
#define AREA_REENTRANT 0x20000
#define AREA_EXTFPSET  0x40000
#define AREA_NOSTACKCHECK 0x80000
#define AREA_BASED     0x100000
#define AREA_STUBDATA  0x200000

#define AREA_LINKONCE  0x10000000 /* GNU linkonce (extension) */

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
