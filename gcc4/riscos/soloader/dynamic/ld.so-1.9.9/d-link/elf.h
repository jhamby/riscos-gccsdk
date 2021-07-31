#ifndef _DLINK_ELF_H
#define _DLINK_ELF_H

#ifdef __riscos
#include <sys/elf.h>
#else
#include <linux/elf.h>
#endif

#define PT_ARM_EXIDX	0x70000001

/* Processor specific flags for the ELF header e_flags field.  */
#define EF_ARM_RELEXEC     0x01
#define EF_ARM_HASENTRY    0x02
#define EF_ARM_INTERWORK   0x04
#define EF_ARM_APCS_26     0x08
#define EF_ARM_APCS_FLOAT  0x10
#define EF_ARM_PIC         0x20
#define EF_ARM_ALIGN8	   0x40		/* 8-bit structure alignment is in use.  */
#define EF_ARM_NEW_ABI     0x80
#define EF_ARM_OLD_ABI     0x100
#define EF_ARM_SOFT_FLOAT  0x200
#define EF_ARM_VFP_FLOAT   0x400
#define EF_ARM_MAVERICK_FLOAT 0x800

typedef unsigned _Unwind_Ptr;

struct nlist
{
  char			*n_name;	/* symbol name */
  long			n_value;	/* value of symbol */
  short			n_scnum;	/* section number */
  unsigned short	n_type;		/* type and derived type */
  char			n_sclass;	/* storage class */
  char			n_numaux;	/* number of aux. entries */
};

#endif /* _DLINK_ELF_H */
