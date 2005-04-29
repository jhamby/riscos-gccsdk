/*
** Drlink AOF linker
**
** Copyright © David Daniels 1993, 1994, 1995, 1996, 1997, 1998.
** All rights reserved.
**
** This file contains the definitions of all attributes and structures
** concerned with areas
*/

#ifndef areahdr_h
#define areahdr_h

/* Area attribute bits */

#define ATT_CODE   0x02			/* Area contains code */
#define ATT_DATA   0x00			/* Area contains data */
#define ATT_COMDEF 0x04			/* Area is a common block definition */
#define ATT_COMMON 0x08			/* Area is a common block reference */
#define ATT_NOINIT 0x10			/* Area not present in file */
#define ATT_RDONLY 0x20			/* Area is read only */
#define ATT_SYMBOL 0x80			/* Area contains symbolic debugging information */

/* Following are the extra AOF V3 area attributes */

#define ATT_ABSOL       0x00001		/* Area has an absolute address                    */
#define ATT_POSIND      0x00040		/* Area is position independent                    */
#define ATT_32BIT       0x00100		/* Area contains 32-bit PC code                    */
#define ATT_REENT       0x00200		/* Area contains re-entrant code                   */
#define ATT_EXTFP       0x00400		/* Area contain instructions from extended FP set  */
#define ATT_NOSTAK      0x00800		/* Area contains no stack checking code            */
#define ATT_BASED       0x01000		/* Area contains based data                        */
#define ATT_STUBS       0x02000		/* Area contains shared library stub data          */
#define ATT_SOFTFLOAT 0x0080000		/* Uses soft-float                                 */
#define ATT_LINKONCE  0x0100000		/* GNU Linkonce section                            */
#define ATT_BRMASK      0xF0000		/* Mask for based area register number             */
#define ATT_A3ATTR      0x03F41		/* Mask for AOF 3 attributes                       */
#define ATT_UNSUPP      0x02001		/* Mask for unsupported AOF 3 attributes           */

/* Masks for checking legality of attribute settings */

#define ATT_BADCODE 0x7F090		/* Mask for 'code' area attributes */
#define ATT_BADATA  0x0CF80		/* Mask for 'data' attributes */
#define ATT_BADSYM  0xFF05D		/* Mask for 'debugging' attributes */

/* Alignment ('al') byte constants */

#define ALLFS 0xFFFFFFFF		/* Marks a relocation as to be ignored */
#define ALBYTE 2			/* Value of 'al' byte in OBJ_HEAD chunk area entry */
#define MAXV3AL 32			/* Maximum value of 'al' allowed in AOF V3 */
#define DEFALIGN 2			/* Default alignment for 'al' */

typedef struct areaentry {		/* Entry in OBJ_HEAD chunk in memory */
  unsigned int areaname;		/* Offset of area name in OBJ_STRT chunk */
  unsigned int attributes;		/* Attribute bytes */
  unsigned int arsize;			/* Size of area excluding relocations */
  unsigned int arelocs;			/* Number of relocations */
  union {
    unsigned int arzero;		/* 0 */
    struct arealist *arlptr;		/* Or pointer to arealist entry for area */
    unsigned int araddress;		/* Or base address if area has absolute address */
  } arlast;
} areaentry;

typedef struct aofheader {		/* OBJ_HEAD chunk header */
  unsigned int oftype;			/* Object file type */
  unsigned int aofversion;		/* Version of AOF in use */
  unsigned int numareas;		/* Number of areas in OBJ_AREA chunk */
  unsigned int numsymbols;		/* Number of symbols in OBJ_SYMT chunk */
  unsigned int eparea;			/* Area of program's entry point */
  unsigned int epoffset;		/* Offset in area of entry point */
} aofheader;

typedef struct objheadhdr {
  aofheader areaheader;			/* Header in OBJ_HEAD chunk */
  areaentry firstarea;			/* Entry for first area in OBJ_HEAD */
} objheadhdr;

typedef struct arearef {		/* Area's 'referenced area' list entry */
  struct arealist *arefarea;		/* Pointer to referenced area */
  int arefcount;			/* No. of times area is referenced by current area */
  struct arearef *arefnext;		/* Next area in list */
} arearef;

/*
** 'arealist' is the main structure used to hold information on areas in the
** AOF file.
*/
typedef struct arealist {
  struct arealist *left;		/* Left in binary tree                           */
  struct arealist *right;		/* Right in binary tree                          */
  int arhash;				/* Area name's hash value                        */
  const char *arname;			/* Pointer to area name in loaded OBJ_STRT chunk */
  struct arealist *arbase;		/* Pointer to first area with same name          */
  struct arealist *arlast;		/* Pointer to last area with same name or nil    */
  struct filelist *arfileptr;		/* Pointer to area's file's file list entry      */
  unsigned int aratattr;		/* Area's AT attribute value                     */
  unsigned int aralign;			/* Area's alignment mask                         */
  unsigned int *arobjdata;		/* Pointer to area in OBJ_AREA chunk             */
  unsigned int arobjsize;		/* Size of area in OBJ_AREA chunk                */
  struct relocation *areldata;		/* Pointer to relocation data in OBJ_AREA chunk  */
  unsigned int arnumrelocs;		/* Number of relocations in area                 */
  unsigned int arplace;			/* Offset of area in final image                 */
  struct symbol *arsymbol;		/* Pointer to area's symbol table entry or nil   */
  int arefcount;			/* Count of times area is referenced             */
  struct arearef *areflist;		/* Pointer to list of areas referenced           */
} arealist;

typedef enum {	/* type of image file being created */
  HDR_BIN, HDR_AIF, HDR_RMOD, HDR_RELOC} segtype;

extern arealist
  *rocodelist,				/* Pointer to R/O code list */
  *rwcodelist,				/* Pointer to R/W code list */
  *rodatalist,				/* Pointer to R/O data list */
  *rwdatalist,				/* Pointer to R/W data list */
  *zidatalist,				/* Pointer to zero-initialised (R/W) data list */
  *debuglist;				/* Pointer to debug list (R/O) */

extern unsigned int
  workspace,				/* Size of relocatable image workspace */
  progbase,				/* Address of start of program */
  codebase,				/* Start-of-code address */
  database,				/* Start-of-R/W data address */
  relocaddr;				/* Address of relocation code in image */

/** Image file entry point */

extern arealist *entryarea; 		/* Pointer to area containing entry point */
extern unsigned int entryoffset;    	/* Offset within area of entry point */

#endif
