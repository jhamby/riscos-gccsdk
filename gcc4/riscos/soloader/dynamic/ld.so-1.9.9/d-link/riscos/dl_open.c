#include "syscall.h"
#include "../config.h"

int _dl_open(char * filename, unsigned int flags)
{
int zfileno;

#if LD_USE_SYMLINKS > 0
char *final_name;

  if ((final_name = _dl_resolve_symlinks(filename)) == 0)
    return -1;
#endif

  /* assume files are always read only */

  asm volatile ("mov	a2,%1;\n\t"
		"mov	a1,#0x4F;\n\t"	/* open existing file for read access only, no errors generated */
		"swi	0x2000d;\n\t"	/* XOS_Find,0x4F */
		"movvc	%0,r0;\n\t"
		"movvs	%0,#-1;\n\t"
		"teq	r0,#0;\n\t"
		"moveq	%0,#-1;\n\t"
		: "=r" (zfileno)
#if LD_USE_SYMLINKS > 0
		: "r" (final_name)
#else
		: "r" (filename)
#endif
		: "a1", "a2", "cc");

#if LD_USE_SYMLINKS > 0
  _dl_som_free(final_name);
#endif

  return zfileno;
}
