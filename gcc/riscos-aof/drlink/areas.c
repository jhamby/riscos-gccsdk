/*
** Drlink AOF Linker - AOF Area handling
**
** Copyright © David Daniels 1993, 1994, 1995, 1996, 1997, 1998.
** All rights reserved.
**
** This file contains the functions involved with manipulating
** the AOF file OBJ_AREA chunks. It also contains all the
** relocation code
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "drlhdr.h"
#include "areahdr.h"
#include "filehdr.h"
#include "chunkhdr.h"
#include "procdefs.h"

#ifdef TARGET_RISCOS
#include <kernel.h>
#include <swis.h>
#endif

/* Variables referenced from other files */

arealist
  *rocodelist,			/* Pointer to R/O code list */
  *rwcodelist,			/* Pointer to R/W code list */
  *rodatalist,			/* Pointer to R/O data list */
  *rwdatalist,			/* Pointer to R/W data list */
  *zidatalist,			/* Pointer to zero-initialised (R/W) data list */
  *debuglist;			/* Pointer to debug list (R/O) */

unsigned int
  workspace,			/* Size of relocatable image workspace */
  progbase,			/* Address of start of program */
  codebase,			/* Start-of-code address */
  database,			/* Start-of-R/W data address */
  relocaddr;			/* Address of relocation code in image */

/** Image file entry point */

arealist *entryarea;		/* Pointer to area containing entry point */
unsigned int entryoffset;	/* Offset within area of entry point */

/* Private declarations */

typedef enum {TYPE_1, TYPE_2} reloctype;

/*
** 'relaction' is used when dealing with partially-linked AOF files. It says what
** can be done to the relocations in the original AOF files, that is, whether they
** can be resolved at this point, whether they are impossible and so on.
*/
typedef enum {
  NOTPOSS,		/* Relocation not possible */
  IGNORE,		/* Ignore relocation */
  POSSIBLE,		/* Relocation can be finished in its entirety */
  TYPE2_SA,		/* Relocate but extra type-2 symbol additive relocation needed */
  TYPE2_SP,		/* Relocate but extra type-2 symbol PC-relative relocation needed */
  TYPE2_AA,		/* Relocate but extra type-2 area additive relocation needed */
  TYPE2_AP,		/* Relocate but extra type-2 area PC-relative relocation needed */
  TYPE2_RP		/* Relocation not possible but offset in instruction must be altered */
} relaction;

/*
** 'areasrchlist' defines an entry in a symbol table of area names used
** when reading in the AOF files. It is used to speed up searching for a
** symbol's area.
*/
typedef struct areasrchlist {
  arealist *srcarea;		/* Pointer to area's list entry */
  int srchash;			/* Area's hashed name */
  struct areasrchlist *srcflink;	/* Next in search list */
} areasrchlist;

#define GCCAREA "C$$gnu"	/* Area names starting with this cannot be deleted */
#define GCCAREALEN (strlen(GCCAREA))	/* Length of name */

#define MAXSRCH 32		/* Size of area search table (must be a power of 2) */
#define SRCHMASK (MAXSRCH-1)	/* Mask for area search table */
#define RELOCSIZE 8		/* Size of a relocation entry in bytes in OBJ_AREA chunk */
#define ABSBASE 0x8000		/* Address of start of program code when loaded */

#ifdef TARGET_RISCOS
#define DEFAULTSIZE 0x8000	/* Default page size (32K to allow for pre-RiscPC machines) */
#else
#define DEFAULTSIZE 0x1000	/* Default page size (4K to confuse matters) */
#endif

/* The following constants are all used in the relocation code */

#define TYPE1_MASK 0xFFFF	/* Mask to extract symbol's OBJ_SYMT index from type-1 relocation */
#define TYPE2_MASK 0xFFFFFF	/* Mask to extract symbol's OBJ_SYMT index from type-2 relocation */
#define TYPE1_SHIFT 16		/* Register shift to extract type-1 relocation type code */
#define TYPE2_SHIFT 24		/* Register shift to extract type-2 relocation type code */
#define BYTEHIGH 0xFF		/* Highest value allowed in byte relocation */
#define HALFHIGH 0xFFFF		/* Highest value allowed in halfword relocation */
#define PCMODIFIER 8		/* Adjustment for PC-relative instructions for PC */
#define INSTMASK 0xE000000	/* Mask to identify instruction type */
#define IN_LDRSTR 0x4000000	/* Single data transfer - LDR Rx,[Ry,#n] */
#define IN_DATAPRO 0x2000000	/* Data processing (with 'immediate' bit set) */
#define IN_BRANCH 0xA000000	/* Branch or branch with link */
#define IN_OFFMASK 0xFFF	/* Mask to extract offset from LDR/STR */
#define MAX_OFFSET 0x1000	/* Maximum offset in LDR/STR */
#define IN_POSOFF 0x00800000	/* +ve offset bit mask (= 1 = +ve, = 0 = -ve) */
#define IN_ADDSUB 0x03E00000 	/* Mask to extract opcode from arithmetic instruction */
#define INST_ADD  0x02800000	/* ADD instruction */
#define INST_SUB  0x02400000	/* SUB instruction */
#define ADSB_MASK 0xFE1FF000	/* Mask for ADD/SUB instructions */
#define LDST_MASK 0xFF7FF000	/* Mask for LDR/STR instruction */
#define BR_MASK 0xFF000000	/* Branch instruction mask */
#define BROFF_MASK 0xFFFFFF	/* Branch offset mask */
#define SHIFT_MASK 0xF		/* Mask to extract shift bits from encoded constant */
#define SHIFT_SHIFT 8		/* Number of bits to shift shift bits in encoded constant */
#define NIBBLE_MASK 0xF		/* Mask to extract low four bits of a value */
#define BYTE_MASK 0xFF		/* Mask to extract a byte */
#define DEST_MASK 0xF000	/* Mask to extract the destination register in instructions */
#define RNRD_MASK 0xFF000	/* Mask to extract registers Rn and Rd from instructions */
#define LOW2_MASK 3		/* Mask for low-order two bits of word */

#define BIT23SIGN 0x800000
#define BIT24EXTEND 0xFF000000
#define BIT15SIGN 0x8000
#define BIT16EXTEND 0xFFFF0000

static unsigned int
  current_objsize,		/* Size of area containing current relocations */
  totalrelocs,			/* Total number of word additive relocations */
  symbolcount,			/* Count of symbols in OBJ_SYMT chunk */
  unused,			/* Number of unused areas */
  areapc,			/* Value of program counter at start of area */
  areatop;			/* Offset of end of last area */

static arealist *current_area;	/* Ptr to area list entry being worked on */
static filelist *current_file;	/* Ptr to file list entry of file being worked on */

static unsigned int
  *areastart,			/* Ptr to area being worked on */
  *headercode,			/* Ptr to AIF header to be added to executable image */
  *thisarea;			/* Pointer to arealist entry for area being processed */

static int entryareanum;	/* Number of area containing program entry point */

static bool
  gotcodearea,			/* TRUE if the first code area has been found */
  noheader;			/* TRUE if the program does not have a header on it */

static arealist
  *rocodelast,			/* Pointer to last R/O code entry added */
  *rwcodelast,			/* Pointer to last R/W code entry added */
  *rodatalast,			/* Pointer to last R/O data entry added */
  *rwdatalast,			/* Pointer to last R/W data entry added */
  *zidatalast,			/* Pointer to last ZI data entry added */
  *debuglast,			/* Pointer to last debug list (R/O) added */

  *firstarea,			/* Pointer to first area entry in OBJ_HEAD */
  *defaultarea,			/* Default entry point if no entry point given */
  *areablock;			/* Memory allocated to hold arealist structures */

typedef struct arealimits {
    const char *areaname;	/* Ptr to name of area */
    int areahash;		/* Hashed area name */
    symtentry *areabase;	/* Pointer to SYMT entry of area's 'base' symbol */
    symtentry *arealimit;	/* Pointer to SYMT entry of area's 'limit' symbol */
    struct arealimits *areaflink;	/* Pointer to next area limit pair */
  } arealimits;

static arealimits *arlimlist;

static areasrchlist *areatable[MAXSRCH];	/* Area symbol table */

/*
** 'extend24' sign extends the 24-bit value passed to it to 32 bits
*/
static unsigned int extend24(unsigned int x) {
  if ((x & BIT23SIGN)!=0) x = x | BIT24EXTEND;
  return  x;
}

/*
** 'decode_armconst' converts the constant value coded in an ARM
** arithmetic instruction passed to it to its proper value,
** returning that  value. It effectively does a 'rotate right' of
** the value held in the low eight bits of the instruction by two
** times the value stored in bits nine to twelve.
*/
static unsigned int decode_armconst(unsigned int value) {
  unsigned int shift, low2bits;
  shift = (value>>SHIFT_SHIFT) & SHIFT_MASK;	/* Extract no. of bits by which to shift value */
  value = value & BYTE_MASK;		/* Extract unshifted value */
  while (shift!=0) {
    low2bits = value & LOW2_MASK;
    value = value>>2 | low2bits<<30;
    shift-=1;
  }
  return value;
}

/*
** 'check_commondef' is called when a code or data area has the 'common
** definition' bit set. This is only used for AOF V3 object code files.
** The function checks to see if an area of the same name already
** exists in the relevant area list. It assumes there will be only one
** occurence at most, with the 'common definition' bit set. If the first
** area does not have this attribute then the linker complains. (Whether
** or not all areas with the same name should have the same attributes is
** not specified in the AOF docs but it makes sense to me that they should
** be.)
** The function returns either a pointer to the arealist entry if an area
** of that name already exists or 'nil' if it is a new area or an error
** occurs.
**
** Note that these areas do not appear in the common block symbol table
** as they are not seen as common blocks as such: they are instances
** of the same code or the same data across several AOF files where only
** one instance is to be kept in the image file. The 'common definition'
** attribute is being used to mark these areas.
*/
static arealist *check_commondef(filelist *fp, areaentry *aep, unsigned int atattr) {
  char *nameptr;
  arealist *ap;
  int hashval, count;
  unsigned int *p1, *p2;
  nameptr = strtbase+aep->areaname;
  hashval = hash(nameptr);
  if ((atattr & ATT_CODE)!=0) {		/* Figure out which list to check */
    ap = ((atattr & ATT_RDONLY)!=0 ? rocodelist : rwcodelist);
  }
  else {	/* Data area */
    ap = ((atattr & ATT_RDONLY)!=0 ? rodatalist : rwdatalist);
  }
  while (ap!=NIL && (hashval!=ap->arhash || strcmp(nameptr, ap->arname)!=0)) ap = ap->arflink;
  if (ap==NIL) return NIL;	/* Common block is unknown */
  if (ap->aratattr!=atattr) {	/* Known common area. Check attributes are the same */
    error("Warning: Attributes of common area '%s' in '%s' conflict with those in '%s'",
     nameptr, fp->chfilename, ap->arfileptr->chfilename);
    return NIL;		/* This area will be added to the area list as a normal area */
  }
  if (aep->arsize!=ap->arobjsize) {
    error("Error: Size of common area '%s' in '%s' differs from definition in '%s'",
     nameptr, fp->chfilename, ap->arfileptr->chfilename);
    return NIL;
  }
  else {
    count = ap->arobjsize;	/* Verify areas' contents are the same */
    p1 = ap->arobjdata;
    p2 = fp->objareaptr;
    while (count>0 && *p1==*p2) {
      p2++;
      p1++;
      count-=1;
    }
    if (count>0) {	/* Area contents do not match */
      error("Error: Contents of common area '%s' in '%s' differ from those in '%s'",
       nameptr, fp->chfilename, ap->arfileptr->chfilename);
      return NIL;
    }
  }
  return ap;
}

/*
** 'check_commonref' is called if the area is a common block reference or
** a definition with the 'no data' attribute. If the common block refers to
** one previously encountered, it returns a pointer to its area list entry
** after updating the size and/or attributes otherwise it returns 'nil'
**
** Note that under AOF V3, it is possible to have multiple definitions
** of a common block and not just a single one as in AOF V2. If a
** program being linked contains nothing but AOF V2 files then this
** restriction is maintained; otherwise the AOF V3 rule applies.
*/
static arealist *check_commonref(filelist *fp, areaentry *aep, unsigned int atattr) {
  char *nameptr;
  symbol *sp;
  arealist *ap;
  unsigned int newsize;
  nameptr = strtbase+aep->areaname;
  sp = find_common(nameptr);
  if (sp==NIL) return NIL;	/* Symbol not in table - Unknown common block */
  ap = sp->symtptr->symtarea.areaptr;
  newsize = aep->arsize;
  if ((atattr & ATT_COMDEF)!=0) {		/* This ref is a definition */
    if ((ap->aratattr & ATT_COMDEF)!=0) {		/* Previous ref was definition also */
      if (aofv3flag) {	/* AOF V3 rules apply */
        if (newsize!=ap->arobjsize) {
          error("Error: Size of Common Block '%s' in '%s' differs from its definition in '%s'",
           nameptr, fp->chfilename, ap->arfileptr->chfilename);
        }
      }
      else {	/* AOF V2 - Cannot have two definitions of a common block */
        error("Error: There is already a definition (in '%s') of Common Block '%s' in '%s'",
         ap->arfileptr->chfilename, nameptr, fp->chfilename);
      }
    }
    else if (newsize<ap->arobjsize) {	/* Definition of previously referenced common block */
      error("Error: Reference to Common Block '%s' in '%s' is larger than defined size in '%s'",
       nameptr, ap->arfileptr->chfilename, fp->chfilename);
    }
    ap->aratattr = ap->aratattr | ATT_COMDEF;
    ap->arobjsize = newsize;
  }
  else if (aep->arsize>ap->arobjsize) {		/* Common block reference */
    if ((ap->aratattr & ATT_COMDEF)!=0) {	/* Previous ref was definition */
      error("Error: Reference to Common Block '%s' in '%s' is larger than defined size in '%s'",
       nameptr, fp->chfilename, ap->arfileptr->chfilename);
    }
  }
  else {
    ap->arobjsize = newsize;
  }
  return ap;
}

static char *make_name(const char *s1, const char *s2) {
  char *p;
  if ((p = allocmem(strlen(s1)+strlen(s2)+sizeof(char)))==NIL) {
    error("Fatal: Out of memory in 'make_name'");
  }
  else {
    strcpy(p, s1);
    strcat(p, s2);
  }
  return p;
}

/*
** 'list_attributes' lists the area attributes of the area passed to it
*/
static void list_attributes(arealist *ap) {
  char text[MSGBUFLEN];
  unsigned int attr, count, n;
  struct {unsigned int atmask; const char *atname;} attributes[] = {
    {ATT_ABSOL,  "Absolute"},
    {ATT_CODE,   "Code"}   ,
    {ATT_COMDEF, "Common definition"},
    {ATT_COMMON, "Common reference"},
    {ATT_NOINIT, "Zero-initialised"},
    {ATT_RDONLY, "Read only"},
    {ATT_POSIND, "Position independent"},
    {ATT_SYMBOL, "Debugging tables"},
    {ATT_32BIT,  "32-bit APCS"},
    {ATT_REENT,  "Reentrant code"},
    {ATT_EXTFP,  "Extended FP instructions"},
    {ATT_NOSTAK, "No stack checking code"},
    {ATT_BASED,  "Based area"},
    {ATT_STUBS,  "Shared library stub data"},
    {0,          "*"}
  };

  attr = ap->aratattr;
  count = 0;
  sprintf(text, "    %s:  ", ap->arfileptr->chfilename);
  for (n = 0; *(attributes[n].atname)!='*'; n++) {
    if ((attr & attributes[n].atmask)!=0) {
      if (count!=0) strcat(text, ", ");
      strcat(text, attributes[n].atname);
      count++;
    }
  }
  error(text);
}

/*
** 'insert_area' inserts the area entry passed to it in the correct
** place (alphabetical order) in the area list passed to the routine.
** If there are duplicate names, these are added so that they appear
** in the order in which they were defined
**
** This code is very messy as it has to handle the case where there
** are a lot of areas with similar names, for example, C$$Code_123,
** C$$Code_124, C$$Code_125 and so on. As the entire area list has
** to be searched to find where to insert the new entry, this can
** be very time consuming, so two tricks are used to speed up the
** code. Firstly, the field 'arlast' is used to provide a link from
** the first area with a certain name to the last area with the same
** name, so that, for example, if there are twenty areas called
** 'C$$Code', the search will check the first and then skip the rest.
** The second trick is the use of 'lastentry'. This points at the
** last area entry **added** to a list (and not the last entry as
** used elsewhere). Using this, if the area name is greater than
** the last one added, the search will start partway down the
** list instead of at the beginning. In the case above, it gives
** immediately the place where the new area will be inserted.
*/
static void insert_area(arealist **list, arealist **lastentry, arealist *newarea) {
  int compres, lastres;
  arealist *ap, *lastarea;
  const char *name;
  arealimits *lp;
  lastarea = NIL;
  name = newarea->arname;
  lastres = compres = 1;
  ap = *lastentry;
  if (ap!=NIL && strcmp(name, ap->arname)<0) ap = *list;
  while (ap!=NIL && (compres = strcmp(name, ap->arname))>=0) {
    lastres = compres;
    if (ap->arlast!=NIL) ap = ap->arlast;	/* Skip areas with same name */
    lastarea = ap;
    ap = ap->arflink;
  }
  if (lastarea==NIL) {  /* List is empty or inserting new first entry */
    newarea->arflink = ap;
    *list = newarea;
  }
  else {
    newarea->arflink = lastarea->arflink;
    lastarea->arflink = newarea;
  }
  if (lastres==0) {	/* Name already in list. Propagate base pointer */
    if (newarea->aratattr!=lastarea->aratattr || newarea->aralign!=lastarea->aralign) {		/* Check attributes are the same */
      error("Warning: Attributes of area '%s' in '%s' conflict with those in '%s'",
       name, newarea->arfileptr->chfilename, lastarea->arfileptr->chfilename);
      list_attributes(newarea);
      list_attributes(lastarea);
    }
    newarea->arbase = lastarea->arbase;
    lastarea->arbase->arlast = newarea;
  }
  else {	/* New area name. Add to base/limit list and symbol table */
    newarea->arbase = newarea;
    lp = allocmem(sizeof(arealimits));
    if (lp==NIL) {
      error("Fatal: Out of memory in 'insert_area' handling area '%s' in file '%s'",
       name, newarea->arfileptr->chfilename);
    }
    else {
      lp->areaname = name;
      lp->areahash = hash(name);
      if (imagetype!=AOF) {
        lp->areabase = make_symbol(make_name(name, "$$Base"), SYM_GLOBAL)->symtptr;
        lp->arealimit = make_symbol(make_name(name, "$$Limit"), SYM_GLOBAL)->symtptr;
      }
      else {	/* Do not want base/limit pair when creating AOF file */
        lp->areabase = lp->arealimit = NIL;
      }
      lp->areaflink = arlimlist;
      arlimlist = lp;
    }
  }
  *lastentry = newarea->arbase;
}

/*
** 'keep_area' is called to check if the area 'ap' should be
** marked as 'non-deleteable' and therefore should not be removed
** when weeding out unreferenced areas. At present this only
** affects some GCC-specific areas
*/
static bool keep_area(arealist *ap) {
  return opt_gccareas && strncmp(ap->arname, GCCAREA, GCCAREALEN)==0;
}

/*
** 'add_newarea' creates a new area structure and adds it to the relevant
** area list. It returns a pointer to the entry if it was successfully
** created or nil
*/
static arealist *add_newarea(filelist *fp, areaentry *aep, unsigned int atattr, unsigned int alattr) {
  arealist *ap;
  symtentry *sp;
  ap = areablock;		/* Take memory for entry from block allocated for all areas in file */
  areablock++;
  ap->arname = strtbase+aep->areaname;
  ap->arhash = hash(ap->arname);
  ap->arfileptr = fp;
  ap->aratattr = atattr;
  ap->aralign = alattr;
  ap->arobjsize = aep->arsize;
  ap->arobjdata = thisarea;
  if ((atattr & ATT_NOINIT)==0) thisarea = COERCE(COERCE(thisarea, char *)+aep->arsize, unsigned int *);
  ap->areldata = COERCE(thisarea, relocation*);
  ap->arnumrelocs = aep->arelocs;
  thisarea = COERCE(COERCE(thisarea, char *)+aep->arelocs*RELOCSIZE, unsigned int *);
  if (!opt_nounused || keep_area(ap)) {
    ap->arefcount = 1;
  }
  else {
    ap->arefcount = 0;
  }
  ap->areflist = NIL;
  if (aofv3flag && (atattr & ATT_ABSOL)!=0) {	/* AOF 3 absolute address area */
    ap->arplace = aep->arlast.araddress;
  }
  else {
    ap->arplace = 0;
  }
  ap->arsymbol = NIL;
  ap->arflink = NIL;
  ap->arlast = NIL;
  if ((atattr & ATT_SYMBOL)!=0) {		/* Debugging info area */
    ap->arefcount = 1;		/* Debug areas are never deleted */
    insert_area(&debuglist, &debuglast, ap);
    debugsize+=aep->arsize;
  }
  else if ((atattr & ATT_CODE)!=0) {		/* Code areas */
    got_32bitapcs = got_32bitapcs || ((atattr & ATT_32BIT)!=0);
    got_26bitapcs = got_26bitapcs || ((atattr & ATT_32BIT)==0);
    if ((atattr & ATT_RDONLY)!=0) {
      insert_area(&rocodelist, &rocodelast, ap);
    }
    else {
      insert_area(&rwcodelist, &rwcodelast, ap);
    }
  }
  else if ((atattr & ATT_COMMON)!=0 || ((atattr & ATT_COMDEF)!=0 && (atattr & ATT_NOINIT)!=0)) {	/* Common block ref */
    if (imagetype!=AOF) {	/* Do not want symbol created when linking partially-linked AOF file */
      ap->arsymbol = make_symbol(ap->arname, SYM_COMMON);
      sp = ap->arsymbol->symtptr;	/* Fill in 'area' field of cb's SYMT entry */
      sp->symtattr = SYM_GLOBAL;	/* Watch this!!! Change attributes as symbol has area */
      sp->symtarea.areaptr = ap;
    }
    insert_area(&zidatalist, &zidatalast, ap);
  }
  else if ((atattr & ATT_RDONLY)!=0) {	/* Data area with R/O bit set */
    insert_area(&rodatalist, &rodatalast, ap);
  }
  else if ((atattr & ATT_NOINIT)==0) {		/* Data with R/O and 'zeroinit' bits clear */
    insert_area(&rwdatalist, &rwdatalast, ap);
  }
  else if ((atattr & ATT_NOINIT)!=0) {	/* Data with 'zeroinit' bit set */
    insert_area(&zidatalist, &zidatalast, ap);
  }
  else {
    error("Error: Illegal 'area' attribute value 0x%06x found in '%s'", atattr, fp->chfilename);
    return NIL;
  }
  if (!gotcodearea && (atattr & ATT_CODE)!=0) {
    gotcodearea = TRUE;
    defaultarea = ap;
  }
  return ap;
}

/*
** 'add_commonarea' is called to create an arealist structure for a common
** block that is referenced only by an entry in the OBJ_SYMT chunk with the
** 'common' attribute set. It returns a pointer to the structure created
*/
static arealist *add_commonarea(const char *name, unsigned int size) {
  arealist *ap;
  ap = allocmem(sizeof(arealist));
  if (ap==NIL) error("Fatal: Out of memory in 'add_commonarea'");
  ap->arname = name;
  ap->arhash = hash(name);
  ap->arfileptr = current_file;
  ap->aratattr = ATT_COMMON | ATT_NOINIT;
  ap->aralign = DEFALIGN;
  ap->arobjsize = size;
  ap->arobjdata = NIL;
  ap->areldata = NIL;
  ap->arnumrelocs = 0;
  ap->arefcount = (opt_nounused ? 0 : 1);
  ap->areflist = NIL;
  ap->arplace = 0;
  ap->arsymbol = NIL;
  ap->arflink = NIL;
  ap->arlast = NIL;
  return ap;
}

/*
** 'make_commonarea' is called when a symbol with the 'common' attribute is
** found in the OBJ_SYMT chunk of a file. It checks to see if the common
** block is known and if so just checks the size. If not known, it add it
** to the common block list. The function returns a pointer to the symbol
** table entry for the common block definition
*/
symtentry *make_commonarea(symbol *sp) {
  unsigned int size;
  symbol *cbsp;
  symtentry *stp;
  arealist *ap;
  stp = sp->symtptr;
  size = stp->symtvalue;
  cbsp = search_common(sp);
  if (cbsp==NIL) {	/* Common block is not known */
    ap = add_commonarea(stp->symtname, size);
    ap->arsymbol = make_symbol(ap->arname, SYM_COMMON);
    stp = ap->arsymbol->symtptr;	/* Point at dummy SYMT entry for CB definition */
    stp->symtarea.areaptr = ap;
    if (imagetype!=AOF) {	/* Only add area to list if not partially-linked AOF file */
      insert_area(&zidatalist, &zidatalast, ap);
    }
  }
  else {	/* Known common block */
    stp = cbsp->symtptr;
    ap = stp->symtarea.areaptr;
    if ((ap->aratattr & ATT_COMDEF)!=0) {	/* Common block size set by a 'definition' entry */
      if (size>ap->arobjsize) {
        error("Size of reference to Common Block '%s' in '%s' is greater than its defined value",
         sp->symtptr->symtname, current_file->chfilename);
      }
    }
    else {	/* Size not set */
      if (size>ap->arobjsize) ap->arobjsize = size;
    }
  }
  return stp;
}

/*
** 'add_srchlist' adds the area entry passed to it to the
** area search table
*/
static void add_srchlist(arealist *ap) {
  int hashval;
  areasrchlist *sp;
  if ((sp = allocmem(sizeof(areasrchlist)))==NIL) {
    error("Fatal: Out of memory in 'add_srchlist'");
  }
  sp->srcarea = ap;
  sp->srchash = hashval = ap->arhash;
  sp->srcflink = areatable[hashval & SRCHMASK];
  areatable[hashval & SRCHMASK] = sp;
}

/*
** 'free_srchlist' returns the memory used by the area search list
** to the heap
*/
void free_srchlist(void) {
  int i;
  areasrchlist *sp, *nextsp;
  for (i = 0; i<MAXSRCH; i++) {
    sp = areatable[i];
    while (sp!=NIL) {
      nextsp = sp->srcflink;
      freemem(sp, sizeof(areasrchlist));
      sp = nextsp;
    }
  }
}

/*
** 'scan_head' is called to scan through the OBJ_HEAD chunk and add
** entries from it to the various area linked lists. It returns 'true'
** if this was accomplished otherwise it returns 'false'. Note that
** memory is allocated to hold all the 'arealist' entries that will
** be generated for efficiency. Note also the 'clever' way of handling
** the area containing an entry point.
**
** Zero-length areas in the OBJ_AREA chunk are ignored. This has to be
** included to get round what looks like a bug in partially-linked AOF
** files produced by the Acorn linker as it creates zero-length areas
** with invalid area attributes.
**
** Another fudge is the check for the 'AL' attribute value. The AOF
** version 2 definition says this should always be set to two but
** the Acorn assembler, Objasm, sets it to zero in entries for debug
** tables. The code complains if it is greater than two in AOF V2
** files
*/
bool scan_head(filelist *fp) {
  arealist *ap = NULL;  /* Points at arealist entry created for entry being checked */
  objheadhdr *ahp;	/* Points at OBJ_HEAD chunk of file */
  areaentry
   *aep,			/* Points at OBJ_HEAD entry being checked */
   *headend;		/* Points at end of OBJ_HEAD chunk */
  int count, areaco;
  unsigned int totalsize, areasize, strtsize, atattr, alattr;
  bool ok;
  for (count = 0; count<MAXSRCH; count++) areatable[count] = NIL;
  current_file = fp;
  ok = TRUE;
  ahp = fp->objheadptr;
  if (ahp->areaheader.oftype!=OBJFILETYPE) {
    error("Error: '%s' does not appear to be an AOF file (type=%x)", fp->chfilename, ahp->areaheader.oftype);
    return FALSE;
  }
  fp->aofv3 = ahp->areaheader.aofversion>=AOFVER3;	/* Got an AOF version 3.1 file */
  aofv3flag = aofv3flag || fp->aofv3;
  if (ahp->areaheader.aofversion>AOFVERSION) {
    error("Error: The version of AOF used in '%s' (%d.%02d) is not supported",
     fp->chfilename, ahp->areaheader.aofversion/100, ahp->areaheader.aofversion%100);
    return FALSE;	/* This line was commented out for some reason... */
  }
  count = ahp->areaheader.numareas;
  if (sizeof(aofheader)+count*sizeof(areaentry)>fp->objheadsize) {
    error("Error: Area count in 'OBJ_HEAD' chunk in '%s' is too large. Is file corrupt?", fp->chfilename);
    return FALSE;
  }
  fp->areacount = count;
  fp->symtcount = ahp->areaheader.numsymbols;
  entryareanum = ahp->areaheader.eparea-1;  /* -1 as 'areaco' goes from 0, not 1 */
  if (entryareanum>=0) entryoffset = ahp->areaheader.epoffset;
  ok = TRUE;
  totalsize = 0;
  firstarea = NIL;
  thisarea = fp->objareaptr;
  areasize = fp->objareasize;
  strtsize = fp->objstrtsize;
  aep = &ahp->firstarea;
  headend = aep+count;
  areablock = allocmem(count*sizeof(arealist));	/* Allocate block to hold arealist entries for file */
  if (areablock==NIL) error("Fatal: Out of memory in 'scan_head' reading '%s'", fp->chfilename);
  for (areaco = 0; areaco<count && ok; areaco++) {
    if (aep->areaname>strtsize) {
      error("Error: Area name offset in area %d in '%s' is too big", areaco+1, fp->chfilename);
      ok = FALSE;
    }
    atattr = aep->attributes>>8;
    alattr = aep->attributes & 0xFF;
    if (!aofv3flag && alattr>ALBYTE) {	/* Check 'al' byte */
      error("Error: Found bad 'al' attribute byte in area %d in '%s'", areaco+1, fp->chfilename);
      ok = FALSE;
    }
    else if (aofv3flag) {		/* Checks for 'al' byte in AOF 3 */
      if (alattr<DEFALIGN || alattr>MAXV3AL) {
        error("Error: Area alignment value in area %d in '%s' is outside range 2 to 32", areaco+1, fp->chfilename);
        ok = FALSE;
      }
    }
    if (aofv3flag && (atattr & ATT_UNSUPP)!=0) {	/* Reject unsupported AOF 3 area attributes */
      error("Error: Area %d of '%s' contains unsupported area attributes (%06x)",
       areaco+1, fp->chfilename, atattr & ATT_UNSUPP);
      ok = FALSE;
    }
    if ((atattr & ATT_SYMBOL)!=0) {	/* Debugging area */
      if ((atattr & ATT_BADSYM)!=0) {	/* Check only legal symbol attributes */
        error("Error: Area %d of '%s' has illegal 'area' attributes (0x%06x)", areaco+1, fp->chfilename, atattr);
        ok = FALSE;
      }
    }
    else if ((atattr & ATT_CODE)!=0) {	/* code area */
      if ((atattr & ATT_BADCODE)!=0) {	/* Check only legal code attributes */
        error("Error: Area %d of '%s' has illegal 'area' attributes (0x%06x)", areaco+1, fp->chfilename, atattr);
        ok = FALSE;
      }
    }
    else {		/* Data area */
      if ((atattr & ATT_BADATA)!=0) {	/* Check only legal data attributes */
        error("Error: Area %d of '%s' has illegal 'area' attributes (0x%06x)", areaco+1, fp->chfilename, atattr);
        ok = FALSE;
      }
    }
    if ((atattr & ATT_NOINIT)==0) {	/* Area is present in AOF file */
      totalsize+=aep->arsize;
      if (totalsize>areasize) {
        error("Error: Size of area %d in 'OBJ_AREA' chunk in '%s' is too big", areaco+1, fp->chfilename);
        ok = FALSE;
      }
    }
    totalsize+=aep->arelocs*RELOCSIZE;
    if (totalsize>areasize) {
      error("Error: Number of relocations in area %d in '%s' is bad", areaco+1, fp->chfilename);
      ok = FALSE;
    }
    if (!fp->aofv3 && aep->arlast.arzero!=0) {		/* AOF 2 check only */
      error("Error: Last word of definition of area %d in '%s' is not zero", areaco+1, fp->chfilename);
      ok = FALSE;
    }
    if (ok) {
      ap = NIL;
      if ((atattr & ATT_COMMON)!=0 || atattr==(ATT_COMDEF|ATT_NOINIT)) {	/* Extra checks for common blocks */
        ap = check_commonref(fp, aep, atattr);
      }
      else if ((atattr & ATT_COMDEF)!=0) {	/* Common definition with code or data */
        ap = check_commondef(fp, aep, atattr);
        if (ap!=NIL) {		/* Known common area: free storage used by new version */
          add_srchlist(ap);
          freemem(thisarea, aep->arsize+aep->arelocs*RELOCSIZE);
        }
      }
      if (ap==NIL) {	/* Not a known common block or new area found */
        if ((atattr & ATT_SYMBOL)==0 || fp->keepdebug) {	/* Area or new common block */
          ap = add_newarea(fp, aep, atattr, alattr);
          ok = ap!=NIL;
        }
        else { /* Debug info when info not needed */
          freemem(thisarea, aep->arsize+aep->arelocs*RELOCSIZE);
          thisarea = COERCE(COERCE(thisarea, char *)+aep->arsize+aep->arelocs*RELOCSIZE, unsigned int *);
        }
      }
    }
    if (ap!=NIL) {
      aep->arlast.arlptr = ap;
      add_srchlist(ap);
      if (firstarea==NIL) firstarea = ap;
    }
    if (areaco==entryareanum) {
      if (entryarea!=NIL) {
        error("Error: Program has multiple entry points");
      }
      else {
        if ((atattr & ATT_CODE)==0) {
          error("Warning: Entry point for program in '%s' is in a data area, not code", fp->chfilename);
        }
        entryarea = ap;
      }
    }
    aep++;
  }
  return ok;
}

/*
** 'get_type1_type' returns the relocation type information from a
** type 1 relocation
*/
int get_type1_type(int reltype) {
  return reltype>>TYPE1_SHIFT;
}

/*
** 'get_type1_index' returns the index of a symbol's entry in the
** OBJ_SYMT chunk from a type 1 relocation
*/
int get_type1_index(int reltype) {
  return reltype & TYPE1_MASK;
}

/*
** 'get_type2_type' returns the relocation type information from a
** type 2 relocation
*/
int get_type2_type(int reltype) {
  return reltype>>TYPE2_SHIFT;
}

/*
** 'get_type2_index' returns the index of a symbol's entry in the
** OBJ_SYMT chunk or an area's index in the OBJ_HEAD chunk from a
** type 2 relocation
*/
int get_type2_index(int reltype) {
  return reltype & TYPE2_MASK;
}

/*
** 'check_strongrefs' is called if an AOF file contains any strong symbol
** definitions. It checks through all of the relocations in the file to
** see if there are any references to those symbols. If so, then it has to
** create 'external reference' OBJ_SYMT entries for those relocations so
** that the symbol resolution code can set up links to the correct symbols
*/
void check_strongrefs(filelist *fp) {
  areaentry *ap;		/* Points at entry in OBJ_HEAD chunk */
  relocation *rp;		/* Points at entry in relocation part of OBJ_AREA chunk */
  symtentry *stp;		/* Points at entry in OBJ_SYMT chunk */
  symbol *sp;
  unsigned int refcount, i, j, reltype;
  refcount = 0;
  ap = &fp->objheadptr->firstarea;
  rp = COERCE(fp->objareaptr, relocation*);
  for (i = 1; i<=fp->areacount; i++) {
    rp = COERCE(COERCE(rp, char *)+ap->arsize, relocation *);	/* Point at start of relocations */
    for (j = 1; j<=ap->arelocs; j++) {
      reltype = rp->reltypesym;
      stp = NIL;
      if ((reltype & REL_TYPE2)==0) {	/* Got a type-1 relocation */
        if ((get_type1_type(reltype) & (REL_SYM|REL_PC))!=0) {	/* Symbol relocation */
          stp = fp->objsymtptr+(get_type1_index(reltype));
        }
      }
      else {
        if ((get_type2_type(reltype) & REL_SYM)!=0) {
          stp = fp->objsymtptr+(get_type2_index(reltype));
        }
      }
      if (stp!=NIL && (stp->symtattr & SYM_STRONGDEF)==SYM_STRONGDEF) {
        refcount++;
        sp = create_externref(stp);
      }
      rp++;
    }
    ap++;
  }
}

/*
** 'find_area' is called to locate the area in which the area name 'np' is
** located. It returns a pointer to the 'arealist' entry of that area or
** 'NIL' if it cannot find the area
*/
arealist *find_area(const char *np) {
  arealist *ap;
  areasrchlist *sp;
  int hashval;
  hashval = hash(np);
  sp = areatable[hashval & SRCHMASK];
  while (sp!=NIL && (hashval!=sp->srchash || strcmp(sp->srcarea->arname, np)!=0)) sp = sp->srcflink;
  if (sp!=NIL) {
    ap = sp->srcarea;
  }
  else {
    if (opt_verbose) error("Warning: Found reference to non-existent area '%s'. Default used", np);
    ap = NIL;
  }
  return ap;
}

/*
** 'get_area' is called to return the area list entry for the area with
** area index 'index'. It is used when handling type-2 relocations
*/
static arealist *get_area(unsigned int index) {
  areaentry *temp;
  temp = &current_file->objheadptr->firstarea;
  return (temp+index)->arlast.arlptr;
}

/*
** 'decr_refcount' goes through an area's list of referenced areas and
** decrements the reference count for that area. If the count goes to
** zero then that area is not referenced and so the routine calls
** itself recursively to go through all the areas that that one
** references to decrement their use counts and so on
*/
static void decr_refcount(arearef *rp) {
  arealist *ap;
  while (rp!=NIL) {
    ap = rp->arefarea;
    if ((ap->arefcount-=rp->arefcount)==0) {
      decr_refcount(ap->areflist);
      ap->areflist = NIL;	/* So that mark_unused does not do them again */
    }
    rp = rp->arefnext;
  }
}

/*
** 'find_unused' goes through an area list and weeds out all the
** unreferenced areas.
*/
static void mark_unused(arealist *ap) {
  while (ap!=NIL) {
    if (ap->arefcount==0) {
      unused++;
      decr_refcount(ap->areflist);
      ap->areflist = NIL;
    }
    ap = ap->arflink;
  }
}

/*
** 'add_arearef' adds to reference to area 'to' from area 'from' and
** bumps up the reference count in area 'to'. If a symbol is
** absolute or one of the linker predefined symbols, it is not
** included.
*/
static void add_arearef(arealist *fromarea, arealist *toarea) {
  arearef *rp;
  if (fromarea!=toarea) {	/* Don't want refs to this area */
    rp = fromarea->areflist;
    while (rp!=NIL && rp->arefarea!=toarea) { /* First ref to this area? */
      rp = rp->arefnext;
    }
    if (rp!=NIL) {	/* Already seen this area */
      rp->arefcount+=1;
    }
    else {	/* new area ref */
      rp = allocmem(sizeof(arearef));
      if (rp==NIL) {
        error("Fatal: Out of memory in 'add_arearef'");
      }
      else {
        rp->arefarea = toarea;
        rp->arefcount = 1;
        rp->arefnext = fromarea->areflist;
        fromarea->areflist = rp;
      }
    }
    toarea->arefcount+=1;
  }
}

/*
** 'add_symref' adds reference 'symtindex' from area 'from' to
** the count of references to area containing the symbol. If
** a symbol is absolute or one of the linker predefined symbols,
** it is not included.
*/
static void add_symref(arealist *fromarea, int symtindex) {
  symtentry *sp;
  sp = symtbase+symtindex;
  if ((sp->symtattr & (SYM_WEAKREF|SYM_DEFN))==0) {		/* Was an external ref */
    sp = sp->symtarea.symdefptr;
  }
  else if ((sp->symtattr & SYM_STRONGDEF)==SYM_STRONGDEF) {	/* Direct reference to strong symbol */
    sp = find_nonstrong(sp);
  }
  if ((sp->symtattr & (SYM_LINKDEF|SYM_ABSVAL|SYM_WEAKREF))!=0) {	/* Symbol is absolute, predefined or weak external */
    return;
  }
  add_arearef(fromarea, sp->symtarea.areaptr);
}

/*
** 'find_arearefs' is used to determine which areas are referenced
** from other areas in one area list. It works this out from the
** relocation data for each area.
** There are two cases to consider, symbol references and area
** references. In a type-1 relocation, only symbol references have
** to be considered as area (interal) relocation always refer to the
** area containing the reference and we are not interested in these.
** In a type-2 relocation, the area referenced can be a different
** area, so both cases do have to be considered. Furthermore, with
** type-2 relocations, the relocation can be either PC-relative
** or additive. Of course, an area reference can refer to the same
** area as the one containing the relocation but add_arearef will
** weed out such relocations.
*/
static void find_arearefs(arealist *ap) {
  int n, numrelocs, reltype, relword;
  relocation *rp;
  while (ap!=NIL) {
    numrelocs = ap->arnumrelocs;
    if (numrelocs!=0) {
      current_file = ap->arfileptr;
      symtbase = ap->arfileptr->objsymtptr;
      symbolcount = ap->arfileptr->symtcount;
      rp = ap->areldata;
      for (n = 1; n<=numrelocs; n++) {
        relword = rp->reltypesym;
        if ((relword & REL_TYPE2)==0) {		/* Type-1 relocation */
          reltype = get_type1_type(relword);
          if ((reltype & (REL_SYM|REL_PC))!=0) add_symref(ap, get_type1_index(relword));
        }
        else {	/* Type-2 relocation */
          reltype = get_type2_type(relword);
          if ((reltype & REL_SYM)!=0) {		/* Symbol */
            add_symref(ap, get_type2_index(relword));
          }
          else {  /* Area */
            add_arearef(ap, get_area(get_type2_index(relword)));
          }
        }
        rp++;
      }
    }
    ap = ap->arflink;
  }
}

/*
** 'mark_area' is called to mark an area as non-deletable by bumping up
** its reference count
*/
void mark_area(arealist *ap) {
  ap->arefcount+=1;
}

/*
** 'find_unused' is called to find out which areas in the linked
** program are used and which are not and so can be left out of
** the image file
*/
void find_unused(void) {
  if (opt_verbose) error("Drlink: Finding unused areas...");
  mark_area(entryarea);		/* Don't want entry area to disappear */
  if (imagetype==RMOD) mark_area(rocodelist);	/* or any embarrasing problems with modules */
  if (opt_cpp) find_cdareas();	/* Or with C++ constructors and destructors */
  find_arearefs(rocodelist);
  find_arearefs(rodatalist);
  find_arearefs(rwcodelist);
  find_arearefs(rwdatalist);
  find_arearefs(zidatalist);
  mark_unused(rocodelist);
  mark_unused(rodatalist);
  mark_unused(rwcodelist);
  mark_unused(rwdatalist);
  mark_unused(zidatalist);
}

/*
** 'fillin_limits' fills in the area pointers and values of the base
** and limit pair for the given area
*/
static void fillin_limits(arealist *firstarea, arealist *lastarea) {
  arealimits *p;
  int hashval;
  const char *name;
  hashval = firstarea->arhash;
  name = firstarea->arname;
  p = arlimlist;
  while (p!=NIL && (hashval!=p->areahash || strcmp(name, p->areaname)!=0)) p = p->areaflink;
  if (p==NIL) error("Fatal: Possible linker error in 'fillin_limits'");
  p->areabase->symtarea.areaptr = firstarea;
  p->areabase->symtvalue = firstarea->arplace;
  p->arealimit->symtarea.areaptr = lastarea;
  p->arealimit->symtvalue = lastarea->arplace+lastarea->arobjsize;
}

/*
** 'align_page' aligns the end of the last area in the R/O portion of the
** image on a page boundary. This is for principly for debugging: DDT
** write-protects R/O areas up to a page boundary. At the moment this
** is kludged by simply extending the last area in the R/O part to a
** page boundary: Anything that follows the area will be written to the
** imagefile as part of the image. There is a chance this could lead to
** an addressing exception if this takes the area past the end of the
** wimp slot...
*/
static void align_page(void) {
  arealist *rolist, *lastarea;
  int pagesize;
  unsigned int padding;
#ifdef TARGET_RISCOS
  _kernel_oserror *swierror;
  _kernel_swi_regs regs;
#endif
  rolist = (rodatalist!=NIL ? rodatalist : rocodelist);
  if (rolist==NIL) return;	/* No read-only areas to protect */
  lastarea = NIL;
  do {
    lastarea = rolist;
    rolist = rolist->arflink;
  } while (rolist!=NIL);
#ifdef TARGET_RISCOS
  swierror = _kernel_swi(OS_ReadMemMapInfo, &regs, &regs);
  if (swierror==NIL) {
    pagesize = regs.r[0];		/* Page size is returned by SWI in R0 */
  }
  else {
    error("Warning: Could not determine machine's page size. Default of 32K used");
    pagesize = DEFAULTSIZE;
  }
#else
  pagesize = DEFAULTSIZE;
#endif
  padding = ((areapc+pagesize-1) & -pagesize)-areapc;
  lastarea->arobjsize+=padding;
  areapc+=padding;
}

/*
** 'calc_place' goes through a list of areas and fills in the address in
** the image file where the area is to be found. If the area has an entry
** in the symbol table, that is filled in with the value as well. The
** base/limit pairs for each area are also sorted out here. To make
** it even more complicated, the routine has to allow for areas that
** will not be included in the final image file. The last function it
** carries out is to add the size of the areas to the size of the image
** file.
*/
static void calc_place(arealist *p) {
  arealist
    *firstarea,		/* Points to first area with name 'x' */
    *lastarea,		/* Points to last area with name 'x' */
    *lastbase;
  unsigned int align;
  firstarea = lastarea = lastbase = NIL;
  while (p!=NIL) {
    if (p->arefcount>0) {
      if (firstarea==NIL) {	/* First ref to this area name */
        firstarea = lastarea = p;
        lastbase = p->arbase;
      }
      else if (lastbase==p->arbase) {	/* Same name as last entry */
        lastarea = p;
      }
      else {	/* New area name */
        fillin_limits(firstarea, lastarea);
        firstarea = lastarea = p;
        lastbase = p->arbase;
      }
      align = (1<<p->aralign)-1;	/* Align area address as necessary */
      areapc = (areapc+align) & ~align;
      p->arplace = areapc;
      if (p->arsymbol!=NIL) define_symbol(p->arsymbol, areapc);
      areapc+=p->arobjsize;
    }
    p = p->arflink;
  }
  if (firstarea!=NIL) fillin_limits(firstarea, lastarea);
}

/*
** 'relocate_areas' is called to work out the relocation constant
** of each area. It also defines the values of the pre-defined
** linker symbols and the base/limit pair values for each area.
*/
void relocate_areas(void) {
  unsigned int initpc, oldpc, baseaddr;
  segtype wantedtype;
  if (opt_codebase) {	/* Non-standard base-of-code used */
    progbase = baseaddr = codebase;
  }
  else {
    progbase = baseaddr = ABSBASE;
  }
  headersize = 0;
  if (!noheader) {	/* Executable image has a header on it */
    if (imagetype==BIN) {
      wantedtype = HDR_BIN;
    }
    else {	/* AIF or relocatable AIF */
      wantedtype = HDR_AIF;
    }
    get_hdrcode(wantedtype, &headercode, &headersize);
  }
  areatop = initpc = areapc = baseaddr+headersize;
  imagesize = headersize;
  define_symbol(image_robase, areapc);
  define_symbol(image_codebase, areapc);
  calc_place(rocodelist);
  calc_place(rodatalist);
  if (opt_pagealign) align_page();
  define_symbol(image_rolimit, areapc);
  define_symbol(image_codelimit, areapc);
  if (opt_database) {   /* Non-standard base-of-data used */
    if (database>=initpc && database<areapc) {
      error("Warning: Read/write code && data areas overlap read only areas");
    }
    if (rwcodelist!=NIL || rwdatalist!=NIL) {
      error("Warning: Image file contains code or constant data in read/write area and option '-data' has been used");
    }
    areapc = database;
  }
  define_symbol(image_rwbase, areapc);
  define_symbol(image_database, areapc);
  calc_place(rwcodelist);
  oldpc = areapc;
  calc_place(debuglist);
  areapc = oldpc;
  calc_place(rwdatalist);
  imagesize = imagesize+areapc-initpc;
  define_symbol(image_zibase, areapc);
  if (imagetype==RMOD) define_symbol(reloc_code, areapc);
  relocaddr = areapc;
  calc_place(zidatalist);
  define_symbol(image_zilimit, areapc);
  define_symbol(image_rwlimit, areapc);
  define_symbol(image_datalimit, areapc);
}

/*
** The next few functions deal with relocations. This section is complex
** and extreme care should be taken when modifying it
*/

/*
** 'flag_badreloc' is called to print an error message about a bad
** relocation
*/
static void flag_badreloc(unsigned int *relplace, const char *type) {
  error("Error: Relocated (%s) value is out of range at offset 0x%x in area '%s' in file '%s'",
   type, relplace-areastart, current_area->arname, current_file->chfilename);
}

/*
** 'fixup_adr' is called to relocate 'adr' instructions. Either
** single instructions or sequences of ADD and SUB can be dealt
** with by this code.
*/
static void fixup_adr(unsigned int *relplace, int inscount, unsigned int relvalue) {
  unsigned int regrnrd, inst;
  int addr, shift;
  unsigned int *relstart, *relbase;
  bool negative;
  if (inscount==0) inscount = 0x10000;	/* inscount=0 = do as many instructions as needed */
  addr = 0;
  relbase = relstart = relplace;
  inst = *relplace;
  regrnrd = inst & DEST_MASK;
  regrnrd = regrnrd+(regrnrd<<4);	/* Get mask for register Rn and Rd in ADDs and SUBs */
  do {		/* Extract the encoded offset */
    if ((inst & IN_ADDSUB)==INST_ADD) {
      addr+=decode_armconst(inst);
    }
    else {
      addr-=decode_armconst(inst);
    }
    relplace++;
    inst = *relplace;
    inscount-=1;
  } while (inscount!=0 && ((inst & IN_ADDSUB)==INST_SUB || (inst & IN_ADDSUB)==INST_ADD) && (inst & RNRD_MASK)==regrnrd);
  addr+=relvalue;
  negative = addr<0;
  if (negative) addr = -addr;
  shift = 16;
  do {		/* Encode constant for ARM instruction */
    if (addr!=0) {
      while ((addr & LOW2_MASK)==0) {
        addr = addr>>2;
        shift-=1;
      }
    }
    inst = (*relstart & ADSB_MASK) | (negative ? INST_SUB : INST_ADD);
    *relstart = (inst | ((shift & SHIFT_MASK)<<SHIFT_SHIFT)) + (addr & BYTE_MASK);
    addr = addr & ~BYTE_MASK;
    relstart++;
  } while (relstart!=relplace);
  if (addr!=0) flag_badreloc(relbase, "ADR");
}

/*
** 'fixup_additive' handles additive relocations, including
** instruction sequences
*/
static void fixup_additive(unsigned int *relplace, unsigned int reltype, unsigned int relvalue) {
  unsigned int data, inst;
  int addr;
  switch (reltype & FTMASK) {
  case REL_BYTE: case REL_HALF:		/* Relocate byte and halfword */
    if ((reltype & REL_SYM)==0) {
      error("Error: Found bad relocation type for a byte or halfword field in area '%s' in '%s'",
       current_area->arname, current_file->chfilename);
      return;
    }
    if ((reltype & FTMASK)==0) {	/* Byte relocation */
      data = *(COERCE(relplace, char*))+relvalue;
      if (data>BYTEHIGH) {
        flag_badreloc(relplace, "Byte");
      }
      else {
        *(COERCE(relplace, char*)) = data;
      }
    }
    else {	/* Half word  relocation */
      data = *(COERCE(relplace, char*))+(*(COERCE(relplace, char*)+1)<<8)+relvalue;
      if (data>HALFHIGH) {
        flag_badreloc(relplace, "Halfword");
      }
      else {
        *(COERCE(relplace, char*)) = data&0xFF;
        *(COERCE(relplace, char*)+1) = data>>8;
      }
    }
    break;
  case REL_WORD:
    *relplace+=relvalue;
    totalrelocs+=1;
    break;
  case REL_SEQ:	/* Instruction sequence */
    inst = *relplace;
    if ((inst & INSTMASK)==IN_LDRSTR) {	/* LDR/STR */
      addr = inst & IN_OFFMASK;
      if ((inst & IN_POSOFF)==0) addr = -addr;
      addr = addr+relvalue;
      if (addr<=-MAX_OFFSET || addr>=MAX_OFFSET) {
        flag_badreloc(relplace, "LDR/STR");
      }
      else {
        *relplace = (inst & LDST_MASK) | (addr<0 ? -addr : addr | IN_POSOFF);
      }
    }
    else if ((inst & INSTMASK)==IN_DATAPRO) {		/* Data processing instruction */
      fixup_adr(relplace, (reltype & REL_IIMASK)>>REL_IISHIFT, relvalue);
    }
    else {	/* Branch instruction. Will this happen? */
      *relplace = (inst & BR_MASK) | ((((extend24(inst)<<2)+relvalue)>>2) & BROFF_MASK);
    }
  }
}

/*
** 'fixup_pcrelative' is called to handle PC-relative relocations for both
** type-1 and type-2 relocations. Note that the code has to be able to
** cope with relocations in different instruction types and not just branch
** instructions.
*/
static bool fixup_pcrelative(unsigned int *relplace, reloctype reltype, int relvalue) {
  int addr;
  unsigned int inst;
  inst = *relplace;
  if ((inst & INSTMASK)==IN_BRANCH) {	/* Branch instruction */
    addr = extend24(inst)<<2;		/* Get address in inst, sign extend & convert to bytes */
    if (reltype==TYPE_1) {
      addr = addr+relvalue-current_area->arplace-(COERCE(relplace, char *)-COERCE(areastart, char *))-PCMODIFIER;
    }
    else {
      addr = addr+relvalue-current_area->arplace;
    }
    *relplace = (inst & BR_MASK) | ((addr>>2) & BROFF_MASK);
  }
  else if ((inst & INSTMASK)==IN_LDRSTR) {	/* LDR/STR */
    addr = inst & IN_OFFMASK;
    if ((inst & IN_POSOFF)==0) addr = -addr;
    addr = addr+relvalue-current_area->arplace;
    if (addr<=-MAX_OFFSET || addr>=MAX_OFFSET) {
      flag_badreloc(relplace, "PC relative");
    }
    else {
      *relplace = (inst & LDST_MASK) | (addr<0 ? -addr : addr | IN_POSOFF);
    }
  }
  else {		/* Assume data processing instruction (ADD or SUB) */
    fixup_adr(relplace, 0, relvalue-current_area->arplace);
  }
  return TRUE;
}

/*
** 'fixup_type1' is called to handle a type-1 relocation
*/
static void fixup_type1(unsigned int reltypesym, unsigned int *relplace) {
  unsigned int reltype, relvalue, symtindex;
  symtentry *sp;
  reltype = get_type1_type(reltypesym);
  if ((reltype & (REL_SYM|REL_PC))!=0) {  /* Symbol relocation */
    if ((symtindex = (get_type1_index(reltypesym)))>symbolcount) {
      error("Error: Found bad OBJ_SYMT index in relocation in area '%s' in '%s'",
       current_area->arname, current_file->chfilename);
      return;
    }
    sp = symtbase+symtindex;
    if ((sp->symtattr & SYM_DEFN)!=0) {	/* Nice, easy ref to symbol */
      if ((sp->symtattr & SYM_STRONG)!=0) {	/* Nasty, horrible ref to strong symbol in this file */
        sp = find_nonstrong(sp);
      }
      relvalue = sp->symtvalue;
    }
    else if ((sp->symtattr & (SYM_WEAKREF|SYM_ABSVAL))!=0)	/* Weak external ref or absolute */
      relvalue = 0;
    else {	/* Ref to defined external */
      relvalue = sp->symtarea.symdefptr->symtvalue;
    }
  }
  else {  /* Internal, that is, relative to this area, relocation */
    relvalue = current_area->arplace;
  }
  if ((reltype & REL_PC)!=0) {	/* PC-relative relocation */
    fixup_pcrelative(relplace, TYPE_1, relvalue);
  }
  else {  /* Additive relocation */
    fixup_additive(relplace, reltype, relvalue);
  }
}

/*
** 'fixup_type2' handles the relocation of a single type-2 relocation
*/
static void fixup_type2(unsigned int reltypesym, unsigned int *relplace) {
  unsigned int reltype, relvalue, symtindex;
  symtentry *sp = NULL;
  arealist *ap;
  reltype = get_type2_type(reltypesym);
  if ((reltype & REL_SYM)!=0) {	/* Symbol relocation */
    if ((symtindex = (get_type2_index(reltypesym)))>symbolcount) {
      error("Error: Found bad OBJ_SYMT index in relocation in area '%s' in '%s'",
       current_area->arname, current_file->chfilename);
      return;
    }
    sp = symtbase+symtindex;
    if ((sp->symtattr & SYM_DEFN)!=0) {		/* Nice, easy ref to symbol */
      if ((sp->symtattr & SYM_STRONG)!=0) {	/* Direct reference to strong symbol */
        sp = find_nonstrong(sp);
      }
      relvalue = sp->symtvalue;
    }
    else if ((sp->symtattr & (SYM_WEAKREF|SYM_ABSVAL))!=0) {	/* Weak external ref or absolute */
      relvalue = 0;
    }
    else {	/* Ref to defined external */
      relvalue = sp->symtarea.symdefptr->symtvalue;
    }
  }
  else {	/* Internal, that is, relative to some area, relocation */
    reltypesym = get_type2_index(reltypesym);
    if (reltypesym>=current_file->areacount) {
      error("Error: Type-2 relocation area index too high in area '%s' in '%s'",
       current_area->arname, current_file->chfilename);
      return;
    }
    if ((reltype & REL_BASED)==0) {		/* Ordinary area relocation */
      relvalue = get_area(reltypesym)->arplace;
    }
    else {	/* Based area relocation */
      ap = get_area(reltypesym);
      relvalue = ap->arplace-ap->arbase->arplace;	/* Addr of area - addr of 1st area of this name */
    }
  }
  if ((reltype & REL_PC)!=0) {	/* PC-relative relocation */
    /* From Linker document :
     *
     * If R is 1, B is usually 0.  A B value of 1 is used to denote that the
     * inter-link-unit value of a branch destination is to be used, rather than
     * the more usual intra-link-unit value.
     */
    if ((reltype & REL_BASED)!=0) {
      /* PC-relative and based - relocate using the inter-link-unit,
	 not intra-link unit */

      /* We need to check that the destination is a leaf or not.
       * If it is a leaf, then we branch to its start; otherwise we branch to
       * start + 4 :
       *  Bit 11 is the Simple Leaf Function attribute which is meaningful
       *  only if this symbol defines the entry point of a sufficiently
       *  simple leaf function (a leaf function is one which calls no other
       *  function).  For a reentrant leaf function it denotes that the
       *  function's inter-link-unit entry point is the same as its
       *  intra-link-unit entry point.
       */
      printf("PC-relative and based in area '%s' in '%s' to offset &%x\n", current_area->arname, current_file->chfilename, relvalue);
      if ((reltype & REL_SYM) == 0) {
        /* I don't know what to do if it's not symbol based; I can't look at
         * the symbol to check how to re-link it; this is (AFAICT) undefined.
         */
        error("Error: PC-relative, reentrant linkage of non-symbol data in area '%s' in '%s'",
       current_area->arname, current_file->chfilename);
        return;
      }
      if ((sp->symtattr & SYM_LEAF) == 0)
        relvalue+=4; /* Use inter-link entry point */
      fixup_pcrelative(relplace, TYPE_2, relvalue);
    }
    else {
      fixup_pcrelative(relplace, TYPE_2, relvalue);
    }
  }
  else {  /* Additive relocation */
    fixup_additive(relplace, reltype, relvalue);
  }
}

/*
** 'relocate_item' handles the relocation of a single item in the
** OBJ_AREA chunk.
*/
static void relocate_item(relocation *rp) {
  unsigned int offset, relts;
  unsigned int *relplace;
  offset = rp->reloffset;
  if (offset>current_objsize) {
    error("Error: Bad relocation offset (0x%x) found in area '%s' in '%s'",
     offset, current_area->arname, current_file->chfilename);
    return;
  }
  relplace = COERCE(COERCE(areastart, char *)+offset, unsigned int *);
  relts = rp->reltypesym;
  if ((relts & REL_TYPE2)==0) {
    fixup_type1(relts, relplace);
  }
  else {
    fixup_type2(relts, relplace);
  }
}

/*
** 'relocate_arealist' is called to deal with all of the relocations
** in all of the areas listed in the area list passed to it
*/
static void relocate_arealist(arealist *ap) {
  int n, numrelocs;
  relocation *rp;
  while (ap!=NIL) {
    if (ap->arefcount>0 && (numrelocs = ap->arnumrelocs)!=0) {
      current_area = ap;
      current_file = ap->arfileptr;
      current_objsize = ap->arobjsize;
      symtbase = ap->arfileptr->objsymtptr;
      symbolcount = ap->arfileptr->symtcount;
      areastart = ap->arobjdata;
      rp = ap->areldata;
      for (n = 1; n<=numrelocs; n++) {
        relocate_item(rp);
        rp++;
      }
    }
    ap = ap->arflink;
  }
}

/*
** 'relocate' is called to handle the relocation of the code and
** data in all the area lists
*/
bool relocate(void) {
  relocate_arealist(rocodelist);
  relocate_arealist(rodatalist);
  relocate_arealist(rwcodelist);
  relocate_arealist(rwdatalist);
  relocate_arealist(debuglist);
  return !got_errors();
}

/*
** 'check_entryarea' is called to check the program's entry point and to
** set up a default one if none has been specified
*/
void check_entryarea(void) {
  if (entryarea==NIL) {  /* No entry point specified */
    entryarea = defaultarea;
    entryoffset = 0;
    error("Warning: Program has no entry point. Default of first executable instruction assumed");
  }

  /* Default to suggesting that the program requires a standard
   * AIF or Binary header.
   */
  noheader = 0;

  /* Relocatable modules do not have a standard header.  */
  if (imagetype==RMOD)
    noheader = 1;

  /* Images where the user has supplied a base address do not have
   * a standard header.
   */
  if (imagetype==opt_codebase)
    noheader = 1;

  /* Both AIF and Binary (BIN) images can have headers.
   *
   * In the case of binary images, if the user explicitly set an ENTRY
   * attribute on either a read-only or a read-write CODE AREA, then we
   * drop the standard binary header and therefore assume that the
   * user knows better.
   */
  if (imagetype==BIN
      && (entryarea==rocodelist
	  || (rodatalist==NIL && entryarea==rwcodelist))
      && entryoffset==0)
    noheader = 1;
}

/*
** 'fillin_header' is called to fill in the executable image's header
*/
static void fillin_header(void) {
  unsigned int startaddr;
  startaddr = entryarea->arplace+entryoffset;
  if (imagetype==BIN) {
    setup_binhdr(headercode, startaddr);
  }
  else {
    setup_aifhdr(headercode, startaddr);
  }
}

/*
** 'write_reloc' is called when dealing with relocatable modules and
** relocatable AIF images to create the relocation table appended to
** image files of these types.
**
** It goes through the relocation information for one area in one of
** the area lists and picks out items that will change if the program
** moves. These are words that address areas or symbols that are not
** absolute values. In terms of AOF relocation types, these are
** word-sized additive relocations that are either internal relocations
** or symbol relocations where the symbol is not absolute. The function
** builds up a table of the offsets of any relocations that match these
** criteria within the image file and writes this table to the image file.
** Note that the table is built using the same memory as that used by
** the relocation data. This is okay, as the table will always be
** smaller than the relocation data (relocations are eight bytes
** long and the table entries are four bytes).
*/
#define WORDADDITIVE 0x00020000		/* Type-1 Word additive */
#define RELMASK 0x00060000
#define T2WORDADDITIVE 0x82000000	/* Type-2 word additive */
#define T2RELMASK 0x86000000

static void write_reloc(arealist *ap) {
  unsigned int n, numrelocs, areaoffset, newsize;
  relocation *rp;
  unsigned int *rip;
  numrelocs = ap->arnumrelocs;
  rp = ap->areldata;
  rip = COERCE(rp, unsigned int*);
  areaoffset = ap->arplace-progbase;
  symtbase = ap->arfileptr->objsymtptr;
  newsize = 0;
  for (n = 1; n<=numrelocs; n++) {
    if (((rp->reltypesym & RELMASK)==WORDADDITIVE || (rp->reltypesym & T2RELMASK)==T2WORDADDITIVE)
     && isrelocatable(rp)) {
      *rip = areaoffset+rp->reloffset;
      rip++;
      newsize+=sizeof(unsigned int);
    }
    rp++;
  }
  if (newsize!=0) {	/* Got some relocations */
    write_image(ap->areldata, newsize);
  }
}

/*
** 'write_areareloc' builds and writes the relocation information for
** each area in the area list passed to it
*/
static void write_areareloc(arealist *ap) {
  while (ap!=NIL) {
    if (ap->arnumrelocs!=0) write_reloc(ap);
    ap = ap->arflink;
  }
}

/*
** 'write_relocinfo' is called to add to the image file the relocation
** code and tables for relocatable programs and modules
*/
static void write_relocinfo(void) {
  unsigned int *relocode;
  unsigned int relocsize, relocend;
  struct {unsigned int reloc_offset, robase_offset;} lastbit;
  get_hdrcode((imagetype==RELOC ? HDR_RELOC : HDR_RMOD), &relocode, &relocsize);
  if (imagetype==RMOD) setup_modcode(relocode);
  write_image(relocode, relocsize);
  write_areareloc(rocodelist);
  write_areareloc(rodatalist);
  write_areareloc(rwcodelist);
  write_areareloc(rwdatalist);
  if (imagetype==RMOD) {   /* Write last two offsets required */
    relocend = reloc_code->symtptr->symtvalue+relocsize-2*sizeof(unsigned int)-progbase;
    lastbit.reloc_offset = relocend;
    lastbit.robase_offset = relocend+sizeof(unsigned int);
    write_image(&lastbit, sizeof(lastbit));
  }
  relocsize = 0xFFFFFFFF;   /* End marker */
  write_image(&relocsize, sizeof(unsigned int));
}

/*
** The next group of functions are concerned with relocations
** when creating a partially-linked AOF file. This lot really
** are nasty!
*/

/*
** 'alter_area_offset' is called to change the offset within an instruction
** (or word of data) used in a type-2 relocation. Note that this code has been
** modified to handle the case where a type-2 relocation that refers to code
** has the 'relocate word' attribute. With AOF 3, it should have a 'code'
** attribute but it looks as if earlier versions of AOF did not make the
** distinction. What is used to decide whether we are dealing with code or
** data is the type of relocation. If it is PC-relative then it is assumed
** to refer to code.
*/
static void alter_area_offset(relocation *rp) {
  unsigned int inst, reltype, offset;
  int addr;
  unsigned int *relplace;
  offset = rp->reloffset;
  if (offset>current_area->arobjsize) {
    error("Error: Bad relocation offset (0x%x) found in area '%s' in '%s'",
     offset, current_area->arname, current_file->chfilename);
    return;
  }
  reltype = get_type2_type(rp->reltypesym);	/* Assume type-2 relocation */
  relplace = COERCE(COERCE(areastart, char *)+offset, unsigned int *);
  inst = *relplace;
  if ((reltype & FTMASK)==REL_SEQ || (reltype & (FTMASK|REL_PC))==(REL_WORD|REL_PC)) {	/* Relocate instructions */
    if ((inst & INSTMASK)==IN_LDRSTR) {	/* LDR/STR */
      addr = inst & IN_OFFMASK;
      if ((inst & IN_POSOFF)==0) addr = -addr;
      addr = addr-current_area->arplace;	/* Subtract offset within 'super area' of area from inst offset */
      if (addr<=-MAX_OFFSET || addr>=MAX_OFFSET) {
        flag_badreloc(relplace, "Area Offset");
      }
      else {
        *relplace = (inst & LDST_MASK) | (addr<0 ? -addr : addr | IN_POSOFF);
      }
    }
    else if ((inst & INSTMASK)==IN_DATAPRO) {		/* Data processing instruction */
      fixup_adr(relplace, (reltype & REL_IIMASK)>>REL_IISHIFT, -current_area->arplace);
    }
    else {
      *relplace = (inst & BR_MASK) | ((((extend24(inst)<<2)-current_area->arplace)>>2) & BROFF_MASK);
    }
  }
  else {	/* Data */
    *relplace = inst-current_area->arplace;
  }
}

/*
** 'alter_type1_offset' is called to modify the area offset within a
** branch of branch with link instruction referenced by a type-1
** relocation, the only type of instruction to which PC-relative
** relocations can be applied using type-1. The instruction is
** modified to the value it would take if branching to the address
** *as if the address was defined in the area containing the
** reference*.
*/
static bool alter_type1_offset(relocation *rp) {
  unsigned int offset;
  unsigned int *relplace;
  symtentry *sp;
  int addr;
  offset = rp->reloffset;
  if (offset>current_area->arobjsize) {
    error("Error: Bad relocation offset (0x%x) found in area '%s' in '%s'",
     offset, current_area->arname, current_file->chfilename);
    return FALSE;
  }
  relplace = COERCE(COERCE(areastart, char *)+offset, unsigned int *);
  sp = symtbase+(get_type1_index(rp->reltypesym));
  if ((sp->symtattr & SYM_DEFN)==0) { /* Reference to another SYMT entry */
    sp = sp->symtarea.symdefptr;
  }
  else if ((sp->symtattr & SYM_STRONGDEF)==SYM_STRONGDEF) {	/* Direct reference to strong symbol */
    sp = find_nonstrong(sp);
  }
  addr = sp->symtvalue+(extend24(*relplace)<<2);
  addr = addr-offset-current_area->arplace-PCMODIFIER;
  *relplace = (*relplace & BR_MASK) | ((addr>>2) & BROFF_MASK);
  return TRUE;
}

/*
** 'alter_type2_offset' is called to modify the address part of instructions
** referenced by type-2 PC-relative relocations. Note that any type of
** instruction can be the target of such a relocation and that there are
** two classes of relocation, area and symbol, to be dealt with as well.
**
** In the case of a type-2 relocation, the address part of the instruction
** is modified to, as it were, move it to the start of the area. It is
** possible that in the link process, an area has been added to the area
** list before the area containing the relocation where the names of the
** areas are the same. In the partially-linked file, the areas will have
** been coalesced into one and the new area will precede the one containing
** the relocation. As a result, the offset in the instruction will be wrong
** and it is necessary to compensate for the new area in the address part
** of the instruction.
*/
static bool alter_type2_offset(relocation *rp) {
  unsigned int inst, offset, typesym;
  unsigned int *relplace;
  symtentry *sp;
  int addr, relvalue;
  offset = rp->reloffset;
  if (offset>current_area->arobjsize) {
    error("Error: Bad relocation offset (0x%x) found in area '%s' in '%s'",
     offset, current_area->arname, current_file->chfilename);
    return FALSE;
  }
  typesym = rp->reltypesym;
  relplace = COERCE(COERCE(areastart, char *)+offset, unsigned int *);
  inst = *relplace;
  addr = get_type2_index(typesym);		/* Extract symbol or area number */
  typesym = get_type2_type(typesym);
  if ((typesym & REL_SYM)!=0) {		/* Symbol reference */
    sp = symtbase+addr;
    if ((sp->symtattr & SYM_DEFN)==0) {		/* Reference to another SYMT entry */
      sp = sp->symtarea.symdefptr;
    }
    else if ((sp->symtattr & SYM_STRONGDEF)==SYM_STRONGDEF) {	/* Direct reference to strong symbol */
      sp = find_nonstrong(sp);
    }
    relvalue = sp->symtvalue;
  }
  else {		/* Area reference */
    relvalue = get_area(addr)->arplace;
  }
  if ((inst & INSTMASK)==IN_LDRSTR) {	/* LDR/STR */
    addr = inst & IN_OFFMASK;
    if ((inst & IN_POSOFF)==0) addr = -addr;
    addr = addr+relvalue-current_area->arplace;
    if (addr<=-MAX_OFFSET || addr>=MAX_OFFSET) {
      flag_badreloc(relplace, "Type 2 PC offset");
    }
    else {
      *relplace = (inst & LDST_MASK) | (addr<0 ? -addr : addr | IN_POSOFF);
    }
  }
  else if ((inst & INSTMASK)==IN_DATAPRO) {		/* Data processing instruction */
    fixup_adr(relplace, (typesym & REL_IIMASK)>>REL_IISHIFT, relvalue-current_area->arplace);
  }
  else {
    *relplace = (inst & BR_MASK) | ((((extend24(inst)<<2)+relvalue-current_area->arplace)>>2) & BROFF_MASK);
  }
  return TRUE;
}

/*
** 'decode_reloc' determines whether the relocation passed
** to it can be carried out at this stage or not. It also
** works out if a further relocation will be needed and if
** so, the type of relocation.
** If the relocation is additive and refers to a symbol whose
** value is known, it can be carried out but a further type-2
** internal relocation must be generated as well. PC-relative
** relocations within the same 'super area' can be dealt with
** with no further action required later. PC-relative
** relocation between different areas can be handled but an
** extra type-2 relocation has to be created.
**
** There is a slight kludge here needed to handle AOF files
** generated by the C compiler, where it attempts to relocate
** a symbol in a non-existent area. If there were no references
** to the symbol we would not have to bother, but there are...
*/
static relaction decode_reloc(relocation *rp) {
  unsigned int typesym, sid;
  symtentry *sp;
  bool istype1;
  typesym = rp->reltypesym;
  istype1 = (typesym & REL_TYPE2)==0;
  if (istype1) {	/* Type 1 relocation */
    sid = get_type1_index(typesym);
    typesym = get_type1_type(typesym);
  }
  else {	/* Type 2 relocation */
    sid = get_type2_index(typesym);
    typesym = get_type2_type(typesym);
  }
  if ((typesym & REL_SYM)!=0 || (istype1 && (typesym & REL_PC)!=0)) {	/* Symbol relocation */
    sp = symtbase+sid;
    if ((sp->symtattr & SYM_DEFN)==0) {	/* External reference */
      sp = sp->symtarea.symdefptr;
      if (sp==NIL || (sp->symtattr & SYM_COMMON)!=0) {	/* Not defined or ref to common block */
        if (istype1 || (typesym & REL_PC)==0) {	/* Type 1 or additive */
          return NOTPOSS;	/* Can't be done at this stage */
        }
        else {	/* Update instruction, PC-rel */
          return TYPE2_RP;
        }
      }
    }
    else if ((sp->symtattr & SYM_STRONGDEF)==SYM_STRONGDEF) {	/* Direct reference to strong symbol */
      sp = find_nonstrong(sp);
    }
    if ((sp->symtattr & SYM_ABSVAL)!=0) {	/* Absolute value? */
      return IGNORE;
    }
/*
** Here, the symbol referenced is known. There are four cases to consider:
** a)  PC-relative relocation, ref and symbol in same area
** b)  PC-relative relocation, ref and symbol in different areas
** c)  Additive relocation, ref and symbol in same area
** b)  Additive relocation, ref and symbol in different areas
**
** a) is the easiest and the relocation can be sorted out completely.
** b) The relocation can be partly resolved but a type-2 area, PC-rel
**    relocation is needed as well.
** c) The offset of the symbol within its area can be filled in, but a
**    type-2 internal relocation will need to be created
** d) The offset of the symbol within its area can be filled in, but a
**    type-2 internal relocation will need to be created
*/
    if ((typesym & REL_PC)!=0) {	/* PC-relative */
      if (sp->symtarea.areaptr->arbase==current_area->arbase) {	/* Same area, no problems */
        return POSSIBLE;	/* Can be done now */
      }
      else {  /* Different areas */
        return TYPE2_AP;	/* Need type-2 area PC-relative relocation */
      }
    }
    else {  /* Additive - Can do, but need a type-2 area additive reloc as well */
      return TYPE2_AA;		/* Need type-2 area additive relocation */
    }
  }
  else {  /* Internal relocation but excluding type-1 PC-rel */
/*
** Internal relocations, that is, ones that involve the addresses of
** areas rather than symbols, are easier to handle. There are four
** cases to consider:
** a) PC-relative, ref area and target area the same
** b) PC-relative, ref area and target area different
** c) Additive, ref area and target area the same
** d) Additive, ref area and target area are different
*/
    if ((typesym & REL_PC)!=0) {	/* PC-relative */
      return TYPE2_AP;		/* Type-2 area PC-relative */
    }
    else {
      return TYPE2_AA;		/* Type-2 area additive */
    }
  }
}

/*
** 'alter_reloc' is called to turn the relocation passed to it into a
** suitable type-2 relocation.
*/
static void alter_reloc(relocation *rp, relaction t2type) {
  unsigned int flags, sid;
  bool istype1;
  symtentry *sp;
  arealist *ap;
  flags = rp->reltypesym;
  istype1 = (flags & REL_TYPE2)==0;
  if (istype1) {
    sid = get_type1_index(flags);
    flags = get_type1_type(flags);
  }
  else {
    sid = get_type2_index(flags);
    flags = get_type2_type(flags);
  }
  if ((flags & REL_SYM)!=0 || (istype1 && (flags & REL_PC)!=0)) {	/* Got a symbol reference */
    sp = symtbase+sid;
    if ((sp->symtattr & SYM_DEFN)==0) {
      sp = sp->symtarea.symdefptr;
    }
    else if ((sp->symtattr & SYM_STRONGDEF)==SYM_STRONGDEF) {	/* Direct reference to strong symbol */
      sp = find_nonstrong(sp);
    }
    ap = sp->symtarea.areaptr;
  }
  else if (istype1) {	/* Got a type-1 area reference */
    ap = current_area;
  }
  else {	/* Got a type-2 area reference */
    ap = get_area(sid);
  }
  sid = find_areaindex(ap);
  switch (t2type) {
  case TYPE2_SA: /* Symbol, additive */
    flags = flags & REL_AOFMASK;
    break;
  case TYPE2_SP: /* Symbol, PC-rel  */
    flags = flags & (REL_AOFMASK | REL_SYM);
    break;
  case TYPE2_AA: /* Area, additive */
    flags = flags & REL_AOFMASK;
    break;
  case TYPE2_AP: /* Area, PC-rel */
    flags = flags & (REL_AOFMASK | REL_PC);
  default:
    break;
  }
  rp->reltypesym = flags<<24 | REL_TYPE2 | sid;
}

/*
** 'fixup_reloclist' goes through the list of external reference
** relocations and resolves any that can be handled at this stage.
*/
static void fixup_reloclist(arealist *ap) {
  int n, numrelocs;
  relocation *rp;
  relaction reltype;
  bool ok;
  ok = TRUE;
  while (ap!=NIL && ok) {
    if (ap->arefcount>0 && (numrelocs = ap->arnumrelocs)!=0) {
      current_area = ap;
      current_file = ap->arfileptr;
      current_objsize = ap->arobjsize;
      symtbase = ap->arfileptr->objsymtptr;
      symbolcount = ap->arfileptr->symtcount;
      areastart = ap->arobjdata;
      rp = ap->areldata;
      ok = TRUE;
      for (n = 1; ok && n<=numrelocs; n++) {
        reltype = decode_reloc(rp);
        switch (reltype) {
        case IGNORE:	/* Throw away relocation */
          rp->reloffset = ALLFS;
          break;
        case NOTPOSS:	/* Cannot do relocation */
          break;
        case POSSIBLE:	/* Relocate and that's all that's needed */
          relocate_item(rp);
          rp->reloffset = ALLFS;
          break;
        case TYPE2_AA:	/* Relocate and create a type-2 area additive as well */
          relocate_item(rp);
          alter_reloc(rp, reltype);
          break;
        case TYPE2_AP:	/* Relocate and create a type-2 area PC-relative as well */
          if ((rp->reltypesym & REL_TYPE2)==0) {
            ok = alter_type1_offset(rp);
          }
          else {
            ok = alter_type2_offset(rp);
          }
          if (ok) alter_reloc(rp, reltype);
          break;
        case TYPE2_RP:	/* Cannot do relocation, but modify offset in instruction */
          alter_area_offset(rp);
        default:
          break;
        }
        rp++;
      }
    }
    ap = ap->arflink;
  }
}

/*
** 'fixup_relocs' is called to try to handle relocations
** as far as it is possible to resolve them at this stage
*/
bool fixup_relocs(void) {
  fixup_reloclist(rocodelist);
  fixup_reloclist(rodatalist);
  fixup_reloclist(rwcodelist);
  fixup_reloclist(rwdatalist);
  fixup_reloclist(debuglist);
  return !got_errors();
}

/*
** The following functions are used to create the image file
*/

/*
** 'write_areas' dumps the areas in one list to the image file.
** Note that, if the area to be written to file has to be aligned
** on anything other than a word boundary, this routine also
** pads the image file with the required number of zeroes
** There is no error checking as the only errors at this stage
** cause the linker to give up on the spot.
*/
static void write_areas(arealist *ap) {
  int fillsize;
  while (ap!=NIL) {
    if (ap->arefcount>0 && ap->arobjsize>0) {
      if (ap->aralign!=DEFALIGN) {	/* Area has a funny alignment */
        fillsize = ap->arplace-areatop;
        if (fillsize!=0) write_zeroes(fillsize);
      }
      write_image(ap->arobjdata, ap->arobjsize);
      areatop = COERCE(ap->arplace+ap->arobjsize, unsigned int);
    }
    ap = ap->arflink;
  }
}

/*
** 'create_image' writes the final executable image file to disk
*/
void create_image(void) {
  link_state = CREATE_IMAGE;
  if (opt_verbose) error("Drlink: Creating executable file '%s'", imagename);
  if (imagetype==RELOC || imagetype==RMOD) imagesize+=totalrelocs*sizeof(unsigned int);
  open_image();
  if (headercode!=NIL) {
    fillin_header();
    write_image(headercode, headersize);
  }
  write_areas(rocodelist);
  write_areas(rodatalist);
  write_areas(rwcodelist);
  write_areas(rwdatalist);
  if (opt_keepdebug) {
    write_areas(debuglist);
    write_lldtable();
  }
  if (imagetype==RELOC || imagetype==RMOD) write_relocinfo();
  close_image();
}

/*
** 'print_arealist' is called to list the areas in an image file of a
** particular type, that is, read-only code, read/write code and so forth
*/
static void print_arealist(const char *areaclass, arealist *ap) {
  unsigned int offset;
  char text[MSGBUFLEN];
  offset = (imagetype==RMOD ? progbase : 0);
  while (ap!=NIL) {
    if (ap->arefcount>0 && ap->arobjsize>0) {
      sprintf(text, "%-6x  %-6x  %s %s from %s\n",
       ap->arplace-offset, ap->arobjsize, areaclass, ap->arname, ap->arfileptr->chfilename);
      if (map_open) {
        write_mapfile(text);
      }
      else {
        printf(text);
      }
    }
    ap = ap->arflink;
  }
}

/*
** 'print_areamap' is used to produce the area map at the end of the link if
** one has been requested via the '-map' option
*/
void print_areamap(void) {
  char text[500];
  sprintf(text, "Area map of image file '%s':\n\nStart   Size    Type      Name\n", imagename);
  if (map_open) {
    write_mapfile(text);
  }
  else {
    printf(text);
  }
  print_arealist("R/O code ", rocodelist);
  print_arealist("R/O data ", rodatalist);
  print_arealist("R/W code ", rwcodelist);
  print_arealist("R/W data ", rwdatalist);
  print_arealist("Zero-init", zidatalist);
  print_arealist("Debug    ", debuglist);
}

/*
** 'list_unused' is called to list the areas that have been removed
** from the image file as there were no references to them
*/
static void list_unused(arealist *ap, bool findsymbol) {
  const char *symname;
  char text[MSGBUFLEN];
  while (ap!=NIL) {
    if (ap->arefcount==0) {
      symname = (findsymbol ? find_areasymbol(ap) : NIL);
      if (symname!=NIL) {
        sprintf(text, "    '%s' (%s) from %s", ap->arname, symname, ap->arfileptr->chfilename);
      }
      else {
        sprintf(text, "    '%s' from %s", ap->arname, ap->arfileptr->chfilename);
      }
      if (map_open) {
        sprintf(&text[strlen(text)], " (%x bytes)\n", ap->arobjsize);
        write_mapfile(text);
      }
      else {
        error(text);
      }
    }
    ap = ap->arflink;
  }
}

/*
** 'print_unusedlist' is called to list the areas that were removed
** from the image file
*/
void print_unusedlist(void) {
  char text[64];
  if (unused==0) {
    if (map_open) {
      sprintf(text, "\nNo areas were omitted from the image file\n");
      write_mapfile(text);
    }
    return;
  }
  if (map_open) {
    sprintf(text, "\nThe following areas were omitted from the image file:\n");
    write_mapfile(text);
  }
  else {
    error("Drlink: The following areas were omitted from the image file:", text);
  }
  list_unused(rocodelist, TRUE);
  list_unused(rodatalist, FALSE);
  list_unused(rwcodelist, TRUE);
  list_unused(rwdatalist, FALSE);
  list_unused(zidatalist, FALSE);
}

/*
** 'print_mapfile' is called to write the area map to a file
*/
void print_mapfile(void) {
  open_mapfile();
  print_areamap();
  if (opt_nounused) print_unusedlist();
  close_mapfile();
}

/*
** 'init_areas' is called to initialise the area linked lists and other
** bits and pieces relating to areas
*/
void init_areas(void) {
  rocodelist = rwcodelist = rodatalist = rwdatalist = zidatalist = debuglist = NIL;
  rocodelast = rwcodelast = rodatalast = rwdatalast = zidatalast = debuglast = NIL;
  aofv3flag = FALSE;
  entryarea = NIL;
  gotcodearea = FALSE;
  unused = 0;
  totalrelocs = 0;
  relocaddr = 0;
  workspace = 0;
  headercode = NIL;
  arlimlist = NIL;
  codebase = database = 0;
  progbase = areapc = ABSBASE;
}
