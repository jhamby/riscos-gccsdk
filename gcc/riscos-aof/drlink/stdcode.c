/*
** Drlink AOF linker
**
** Copyright © David Daniels 1993, 1994, 1995, 1996, 1997, 1998.
** All rights reserved.
**
** This module contains the procedures used to supply and modify
** the standard code segments the linker can add to the executable
** image
*/

#include <string.h>
#include "drlhdr.h"
#include "areahdr.h"
#include "symbolhdr.h"
#include "filehdr.h"
#include "procdefs.h"

#define NOOP 0xE1A00000			/* No-op instruction - MOV R0,R0 */

#define BRENTRY_OFFSET 0x0C		/* Offset in header of BL to entry point */
#define BRELOC_OFFSET 0x04		/* Offsey in header to BL to self-relocation code */
#define PCMODIFIER 8			/* Adjustment for PC-relative instructions for PC */

#define STRONGBIT 0x80000000		/* Bit to signify code is StrongARM compatible */
#define DBG_SOURCE 2			/* Source-level debug info present */

#define ADDRESS_32BIT 0x00000020	/* 32-bit address mode */

unsigned int bincode [] = {
  0xEA000000    /*  B       &00000000  Branch to entry point */
};

unsigned int aifcode [] = {
  0xE1A00000,	/* BL      decompress	*/
  0xEB000000,	/* BL      self-relocate */
  0xEB00000C,	/* BL      zero-init	*/
  0xEB000000,	/* BL      entrypoint	*/
  0xEF000011,	/* SWI     OS_Exit	*/
  0x00000000,	/* Image R/O size	*/
  0x00000000,	/* Image R/W size	*/
  0x00000000,	/* Image debug size	*/
  0x00000000,	/* Image zero-init size	*/
  0x00000000,	/* Image debug type	*/
  0x00008000,	/* Image base		*/
  0x00000000,	/* Workspace size	*/
  0x0000001A,	/* 26-bit addressing	*/
  0x00000000,	/* Data base		*/
  0x00000000,	/* Reserved		*/
  0x00000000,	/* Reserved		*/
  0xE1A00000,	/* MOV     R0,R0  -  No-op */
  0xE04EC00F,	/* SUB     R12,R14,PC	*/
  0xE08FC00C,	/* ADD     R12,PC,R12	*/
  0xE99C0017,	/* LDMIB   R12,{R0-R2,R4} */
  0xE24CC010,	/* SUB     R12,R12,#&10	*/
  0xE08CC000,	/* ADD     R12,R12,R0	*/
  0xE08CC001,	/* ADD     R12,R12,R1	*/
  0xE3A00000,	/* MOV     R0,#0	*/
  0xE3A01000,	/* MOV     R1,#0	*/
  0xE3A02000,	/* MOV     R2,#0	*/
  0xE3A03000,	/* MOV     R3,#0	*/
  0xE3540000,	/* CMP     R4,#0	*/
		/* zeroloop		*/
  0xD1A0F00E,	/* MOVLE   PC,R14	*/
  0xE8AC000F,	/* STMIA   R12!,{R0-R3}	*/
  0xE2544010,	/* SUBS    R4,R4,#16	*/
  0xEAFFFFFB	/* B       zeroloop	*/
};

unsigned int modcode [] = {
  0xE1A00000,	/* NO-OP		*/
  0xE59F1030,	/* LDR   R1,RelocCode	*/
  0xE24FB010,	/* SUB   R11,PC,#12	*/
  0xE05B1001,	/* SUBS  R1,R11,R1	*/
  0x01A0F00E,	/* MOVEQ PC,R14		*/
  0xE59FB024,	/* LDR   R11,ImageBase	*/
  0xE08BB001,	/* ADD   R11,R11,R1	*/
  0xE28F2020,	/* ADR   R2,end		*/
                /* relocloop		*/
  0xE4920004,	/* LDR   R0,[R2],#4	*/
  0xE3700001,	/* CMN   R0,#1		*/
  0xD1A0F00E,	/* MOVLE PC,R14		*/
  0xE79B3000,	/* LDR   R3,[R11,R0]	*/
  0xE0833001,	/* ADD   R3,R3,R1	*/
  0xE78B3000,	/* STR   R3,[R11,R0]	*/
  0xEAFFFFF8,	/* B     relocloop	*/
  0x00000000,   /* RelocCode  EQUD  __RelocCode */
  0x00000000    /* ImageBase  EQUD  Image$$RO$$Base */
                /* end */
};

unsigned int relocode [] = {
		/* RelocCode		*/
  0xE1A00000,	/* MOV     R0,R0	*/
  0xE04EC00F,	/* SUB     ip,lr,pc	*/
  0xE08FC00C,	/* ADD     ip,pc,ip	*/
  0xE24CC00C,	/* SUB     ip,ip,#&0C	*/
  0xE51F0018,	/* LDR     R0,RelocCode	*/
  0xE58C0004,	/* STR     R0,[ip,#4]	*/
  0xE59C902C,	/* LDR     R9,[ip,#44]	*/
  0xE3590000,	/* CMP     R9,#0	*/
  0x0A000018,	/* BEQ     RelocateOnly	*/
  0xE59C0020,	/* LDR     R0,[ip,#32]	*/
  0xE0899000,	/* ADD     R9,R9,R0	*/
  0xEF000010,	/* SWI     OS_GetEnv	*/
  0xE28F2080,	/* ADR     R2,end	*/
		/* l1			*/
  0xE4920004,	/* LDR     R0,[R2],#4	*/
  0xE3700001,	/* CMN     R0,#1	*/
  0x1AFFFFFC,	/* BNE     l1		*/
  0xE0413009,	/* SUB     R3,R1,R9	*/
  0xE0530002,	/* SUBS    R0,R3,R2	*/
  0xDA00000E,	/* BLE     RelocateOnly	*/
  0xE3C0000F,	/* BIC     R0,R0,#&0F	*/
  0xE0823000,	/* ADD     R3,R2,R0	*/
  0xE24F8004,	/* ADR     R8,l2	*/
		/* l2			*/
  0xE93200F0,	/* LDMDB   R2!,{R4-R7}	*/
  0xE92300F0,	/* STMDB   R3!,{R4-R7}	*/
  0xE1520008,	/* CMP     R2,R8	*/
  0xCAFFFFFB,	/* BGT     l2		*/
  0xE08F4000,	/* ADD     R4,pc,R0	*/
  0xE1A0F004,	/* MOV     pc,R4	*/
		/* l3			*/
  0xE93200F0,	/* LDMDB   R2!,{R4-R7}	*/
  0xE92300F0,	/* STMDB   R3!,{R4-R7}	*/
  0xE152000C,	/* CMP     R2,ip	*/
  0xCAFFFFFB,	/* BGT     l3		*/
  0xE08CC000,	/* ADD     ip,ip,R0	*/
  0xE08EE000,	/* ADD     lr,lr,R0	*/
		/* RelocateOnly		*/
  0xE59C1028,	/* LDR     R1,[ip,#40]	*/
  0xE05C1001,	/* SUBS    R1,ip,R1	*/
  0x01A0F00E,	/* MOVEQ   pc,lr	*/
  0xE58CC028,	/* STR     ip,[ip,#40]	*/
  0xE28F2018,	/* ADR     R2,&0000814C	*/
		/* l4			*/
  0xE4920004,	/* LDR     R0,[R2],#4	*/
  0xE3700001,	/* CMN     R0,#1	*/
  0x01A0F00E,	/* MOVEQ   pc,lr	*/
  0xE79C3000,	/* LDR     R3,[ip,R0]	*/
  0xE0833001,	/* ADD     R3,R3,R1	*/
  0xE78C3000,	/* STR     R3,[ip,R0]	*/
  0xEAFFFFF8	/* B       l4		*/
                /* end */
};

/*
** 'get_hdrcode' supplies a copy of the requested code segment to
** the calling routine. It does not simply return a pointer to the
** code as the code will be modified later
*/
void get_hdrcode(segtype hdrtype, unsigned int **hdrstart, unsigned int *hdrsize) {
  unsigned int *p, *cp;
  unsigned int size;
  p = cp = 0;	/* Stop gcc warnings */
  size = 0;  
  switch (hdrtype) {
  case HDR_BIN:		/* Binary image */
    cp = &bincode[0];
    size = sizeof(bincode);
    break;
  case HDR_AIF:		/* AIF image */
    cp = &aifcode[0];
    size = sizeof(aifcode);
    break;
  case HDR_RMOD:	/* Relocatable module */
    cp = &modcode[0];
    size = sizeof(modcode);
    break;
  case HDR_RELOC:	/* Relocatable AIF image */
    cp = &relocode[0];
    size = sizeof(relocode);
  }
  if ((p = allocmem(size))==NIL) {
    error("Fatal: Out of memory in 'get_hdrcode'");
  }
  else {
    memcpy(p, cp, size);
  }
  *hdrstart = p;
  *hdrsize = size;
}

/*
** 'setup_binhdr' is called to store the requisite values
** in the header code segment for a binary file
*/
void setup_binhdr(unsigned int *code, unsigned int startaddr) {
  *code = *code+((startaddr-progbase-PCMODIFIER)>>2);
}

/*
** 'setup_aifhdr' fills in the values needed in the header
** of an AIF file
*/
typedef struct {
  unsigned int aif_decompress;		/* BL to decompression code for compressed images */
  unsigned int aif_selfreloc;		/* BL to self-relocation code */
  unsigned int aif_zeroinit;		/* BL to zero init code */
  unsigned int aif_entrypoint;		/* BL to image entry point */
  unsigned int aif_exitinst;		/* Exit instruction */
  unsigned int aif_rosize;		/* R/O area size */
  unsigned int aif_rwsize;		/* R/W area size */
  unsigned int aif_debugsize;		/* Debug area size */
  unsigned int aif_zisize;		/* Zero init area sizer */
  unsigned int aif_debugtype;		/* Debug info type flag */
  unsigned int aif_imagebase;		/* Address of image base */
  unsigned int aif_workspace;		/* Relocatable image workspace size */
  unsigned int aif_amode;		/* 26/32 bit address mode and flags */
  unsigned int aif_data;		/* Address of image data base */
} aifheader;

void setup_aifhdr(unsigned int *code, unsigned int startaddr) {
  unsigned int zisize, imagebase;
  aifheader *base;
  base = COERCE(code, aifheader *);
  zisize = image_zilimit->symtptr->symtvalue-image_zibase->symtptr->symtvalue;
  imagebase = image_robase->symtptr->symtvalue-headersize;
  base->aif_imagebase = imagebase;
  base->aif_rosize = image_rolimit->symtptr->symtvalue-imagebase;
  base->aif_rwsize = image_rwlimit->symtptr->symtvalue-image_rwbase->symtptr->symtvalue-zisize;
  base->aif_zisize = zisize;
  base->aif_debugsize = debugsize;
  if (debugsize>0) base->aif_debugtype = DBG_SOURCE;
  base->aif_workspace = workspace;
  if (got_32bitapcs && !got_26bitapcs) base->aif_amode=ADDRESS_32BIT;       
  if (opt_strongarm) {	/* Set 'code is StrongARM compatible' flag in flag byte 1 */
    base->aif_amode|=STRONGBIT;
  }
  base->aif_entrypoint+=(startaddr-imagebase-BRENTRY_OFFSET-PCMODIFIER)>>2;
  if (zisize==0) {	/* No zero-init area. Change zero-init BL to NOOP */
    base->aif_zeroinit = NOOP;
  }
  if (imagetype==AIF) {	/* No relocation code */
    base->aif_selfreloc = NOOP;
  }
  else {	/* Relocatable image */
    base->aif_selfreloc+=(relocaddr-imagebase-BRELOC_OFFSET-PCMODIFIER)>>2;
  }
}

typedef struct {
  unsigned int mod_relocbase;		/* Address of relocation code */
  unsigned int mod_imagebase;		/* Address of entry point in image */
} modheader;

/*
** 'setup_modcode' stores values needed in the header for a relocatable
** module. Note the slightly hacky address calculation: the values go
** at the end of the header (Entries 'RelocCode' and 'ImageBase').
*/
void setup_modcode(unsigned int *code) {
  modheader *base;
  base = COERCE(COERCE(code, char *)+sizeof(modcode)-sizeof(modheader), modheader *);
  base->mod_relocbase = reloc_code->symtptr->symtvalue;
  base->mod_imagebase = image_robase->symtptr->symtvalue;
}
