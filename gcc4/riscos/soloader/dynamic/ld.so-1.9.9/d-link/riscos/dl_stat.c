#include "syscall.h"
#include "../config.h"

extern int _dl_stat(char * filename, struct stat *st)
{
int ret;

#if LD_USE_SYMLINKS > 0
char *final_name;

  if ((final_name = _dl_resolve_symlinks(filename)) == 0)
    return -1;
#endif

  /* Only st_size is filled in */
  asm volatile ("mov	r1,%1;\n\t"
		"mov	r0,#17;\n\t"
		"swi	0x20008;\n\t"	/* SWI "XOS_File", 17 */
		"movvs	%0,#-1;\n\t"
		"bvs	0f;\n\t"
		"teq	r0,#1;\n\t"
		"movne	%0,#-1;\n\t"
		"bne	0f;\n\t"
		"teq	%2,#0;\n\t"
		"strne	v1,[%2,#28];\n\t"	/* st->st_size = r4 */
		"mov	%0,#0;\n"
		"0:\n"
		: "=r" (ret)
#if LD_USE_SYMLINKS > 0
		: "r" (final_name), "r" (st)
#else
		: "r" (filename), "r" (st)
#endif
		: "a1", "a2", "a3", "a4", "v1", "v2", "cc", "memory");

#if LD_USE_SYMLINKS > 0
  _dl_som_free(final_name);
#endif

  return ret;
}
