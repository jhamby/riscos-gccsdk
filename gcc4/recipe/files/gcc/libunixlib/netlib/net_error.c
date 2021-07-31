/* __net_error ()
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#include <errno.h>
#include <stdlib.h>

#include <internal/os.h>

/* Function to record a networking (TCP/IP) SWI error.
   When err is non-NULL, it points to a RISC OS error block and errno is
   extracted from this and -1 will be returned.
   When err is NULL, value rtrn will be returned.  */
int
__net_error (int rtrn, const _kernel_oserror *err)
{
  if (err == NULL)
    return rtrn;

  return __ul_seterr (err, err->errnum & 0xFF);
}

