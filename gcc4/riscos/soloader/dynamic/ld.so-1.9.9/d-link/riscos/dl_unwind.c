#include "hash.h"

/* Given an address, return the location and size of the
   exception tables of the object that contains it. The
   object could be a shared library or the executable.
   Used by the ARM stack unwinder.  */
_Unwind_Ptr
__gnu_Unwind_Find_exidx (_Unwind_Ptr addr, int *nrec)
{
  struct elf_resolve * tpnt;

  for (tpnt = _dl_loaded_modules;
       tpnt->next && (addr < (_Unwind_Ptr)tpnt->loadaddr
		   || addr >= (_Unwind_Ptr)tpnt->endaddr);
       tpnt = tpnt->next)
    /* Empty loop.  */;

  if (!tpnt)
  {
    *nrec = 0;
    return 0;
  }

  *nrec = tpnt->exidx_size / 8; /* sizeof(struct __EIT_entry) == 8 */
  return tpnt->exidx;
}
