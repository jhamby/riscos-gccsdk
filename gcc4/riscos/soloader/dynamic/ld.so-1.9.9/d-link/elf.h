#ifndef _DLINK_ELF_H
#define _DLINK_ELF_H

#ifdef __riscos
#include <sys/elf.h>
#else
#include <linux/elf.h>
#endif

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
