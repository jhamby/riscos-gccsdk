/*
** Drlink AOF linker
**
** Copyright © David Daniels 1993, 1994, 1995, 1996, 1997, 1998.
** All rights reserved.
**
** This file contains the procedures that handle the creation of the
** debug tables, both the low-level and compiler generated types
*/

#include <string.h>
#include "Drlhdr.h"
#include "Symbolhdr.h"
#include "Areahdr.h"
#include "Procdefs.h"

/* Private declarations */

typedef struct {
  unsigned int lldrecinfo;	/* Record type/size */
  unsigned int lldflags;	/* Flag to say what record contains */
  unsigned int lldrostart;	/* Address of start of program code */
  unsigned int lldrwstart;	/* Address of start of read/write data */
  unsigned int lldrosize;	/* Size of read-only part */
  unsigned int lldrwsize;	/* Size of read/write part */
  unsigned int lldanon;		/* ?? */
  unsigned int lldbgsize;	/* Size of this debug part */
  unsigned int lldcount;	/* Count of symbols in low level tables */
} lldheader;

typedef struct {
  unsigned int lldoffset;	/* Entry type and offset of name in table */
  unsigned int lldvalue;	/* Symbol's value */
} lldinfo;

/* Entry types in low level debug tables */

#define LLD_SPECIAL 0x01000000		/* Linker symbols */
#define LLD_ROAREA 0x02000000		/* Read-only area name */
#define LLD_ROSYMBOL 0x03000000		/* Symbol in read-only part of image */
#define LLD_RWAREA 0x04000000		/* Read/write area name */
#define LLD_RWSYMBOL 0x05000000		/* Symbol in read/write area */
#define LLD_RWDATA 0x06000000		/* Symbol in read/write data area */
#define LLD_COMMON 0x07000000		/* Common block name */

#define LLD_FLAGS 0x02000000		/* Flag values at start of debug area */
int
  lldentco,				/* Count of entries in low level tables */
  lldnamesize;				/* Size of low level name table */

lldheader lldhdr;

static lldinfo *lldinfotable, *lldinfonext;	/* Entries to go in low level table */
static char *lldnametable, *lldnamenext;	/* Text of names to go in low level table */

/*
** 'nullfill' pads the character string at 'p' to a whole
** number of words long, returning the updated pointer
*/

#define ALIGNBITS (sizeof(int)-1)

static char *nullfill(char *p) {
  while ((COERCE(p, int) & ALIGNBITS)!=0) {
    *p = NULLCHAR;
    p++;
  }
  return p;
}

/*
** 'add_lldentry' adds one entry to the low level debugging tables
*/
static void add_lldentry(char *name, unsigned int class, int value) {
  strcpy(lldnamenext, name);
  lldinfonext->lldoffset = class+(lldnamenext-lldnametable);
  lldinfonext->lldvalue = value;
  lldinfonext++;
  lldnamenext+=strlen(name)+sizeof(char);
  lldentco+=1;
}

static void add_lldsymbol(symbol *sp) {
  symtentry *stp;
  stp = sp->symtptr;
  add_lldentry(stp->symtname, LLD_SPECIAL, stp->symtvalue);
}

/*
** 'gen_llsymt' goes through the OBJ_SYMT area of a entry
** in the file list and generates low-level debug entries for
** each symbol defined there. Both local and global symbols
** are included, but any that start 'x$' are excluded as they
** just clutter up the table. This is, admittedly, a bit of
** a kludge. Symbols which have no areas associated with them,
** which are absolute or are defined in areas where the count
** of references to the area is zero are also omitted.
*/
static void gen_llsymt(filelist *fp) {
  symtentry *sp;
  int n, count;
  unsigned int lldclass;
  unsigned char areattr;
  sp = fp->objsymtptr;
  count = fp->symtcount;
  for (n = 1; n<=count; n++) {
    if ((sp->symtattr & (SYM_DEFN | SYM_ABSVAL))==SYM_DEFN &&
     strncmp(sp->symtname, "x$", 2)!=0 && strcmp(sp->symtname, "__codeseg")!=0) {
      if (sp->symtarea.areaptr!=NIL && sp->symtarea.areaptr->arefcount!=0) {
        areattr = sp->symtarea.areaptr->aratattr;
        lldclass = ((areattr & ATT_RDONLY)!=0 ? LLD_ROSYMBOL : LLD_RWSYMBOL);
        add_lldentry(sp->symtname, lldclass, sp->symtvalue);
      }
    }
    sp++;
  }
}

/*
** 'fillin_lldhdr' completes the header record of the low-level
** debug table
*/
static void fillin_lldhdr(void) {
  unsigned int robase, rwbase;
  int tablesize;
  tablesize = lldnamenext-lldnametable;
  *(COERCE(lldnametable, int *)) = tablesize;
  robase = image_robase->symtptr->symtvalue;
  rwbase = image_rwbase->symtptr->symtvalue;
  debugsize+=sizeof(lldheader)+lldentco*sizeof(lldinfo)+tablesize;
  lldhdr.lldrecinfo = (sizeof(lldheader)<<16)+1;
  lldhdr.lldflags = LLD_FLAGS;
  lldhdr.lldrostart = robase;
  lldhdr.lldrwstart = rwbase;
  lldhdr.lldrosize = image_rolimit->symtptr->symtvalue-robase;
  lldhdr.lldrwsize = image_rwlimit->symtptr->symtvalue-rwbase-(image_zilimit->symtptr->symtvalue-image_zibase->symtptr->symtvalue);
  lldhdr.lldanon = 0;
  lldhdr.lldbgsize = debugsize;
  lldhdr.lldcount = lldentco;
}

/*
** 'write_lldtable' writes the completed debug table to the image
** file. Any errors that occur at this stage will be fatal, so
** it does not return anything to signify that it worked.
*/
void write_lldtable(void) {
  write_image(&lldhdr, sizeof(lldhdr));
  write_image(lldinfotable, lldentco*sizeof(lldinfo));
  write_image(lldnametable, lldnamenext-lldnametable);
  freemem(lldnametable, lldsize);
  freemem(lldinfotable, totalsymbols*sizeof(lldinfo));
}

/*
** 'gen_lowlevel' handles the creation of the linker low-level
** debug tables. It returns 'TRUE' if this worked okay otherwise
** it returns 'FALSE'
*/
static bool gen_lowlevel(void) {
  filelist *fp;
  arealist *ap;
  lldinfotable = lldinfonext = allocmem(totalsymbols*sizeof(lldinfo));
  lldsize = align(lldsize);
  lldnametable = lldnamenext = allocmem(lldsize);
  if (lldinfotable==NIL || lldnametable==NIL) {
    error("Error: Not enough memory to construct low level debugging tables");
    return FALSE;
  }
  lldnamenext+=sizeof(int);	/* First four bytes contains the table length */
  lldentco = 0;
  fp = aofilelist;
  do {
    if (fp->chfilesize!=0 && fp->keepdebug) gen_llsymt(fp);
    fp = fp->nextfile;
  } while (fp!=NIL);
  ap = zidatalist;	/* Now add common blocks to table */
  while (ap!=NIL) {
    if ((ap->aratattr & (ATT_COMMON | ATT_COMDEF))!=0) {
      add_lldentry(ap->arname, LLD_COMMON, ap->arplace);
    }
    ap = ap->arflink;
  }
  add_lldsymbol(image_robase);
  add_lldsymbol(image_rolimit);
  add_lldsymbol(image_rwbase);
  add_lldsymbol(image_rwlimit);
  add_lldsymbol(image_zibase);
  add_lldsymbol(image_zilimit);
  if (got_oldlibs) {	/* Include old symbols */
    add_lldsymbol(image_codebase);
    add_lldsymbol(image_codelimit);
    add_lldsymbol(image_database);
    add_lldsymbol(image_datalimit);
  }
  lldnamenext = nullfill(lldnamenext);
  fillin_lldhdr();
  return TRUE;
}

/*
** 'build_debugtables' is called to create the low-level debugging
** tables added to the image file
*/
bool build_debugtables(void) {
  bool ok;
  ok = gen_lowlevel();
  imagesize+=debugsize;
  if (imagetype==RELOC) relocaddr+=debugsize;
  return ok;
}
