/* Canonicalise handle.
   Copyright (c) 2011 UnixLib Developers.  */

#include <errno.h>
#include <stdlib.h>

#include <internal/local.h>
#include <internal/os.h>

char *
__canonicalise_handle (int fhandle)
{
  size_t bufsize;
  const _kernel_oserror *err;
  if ((err = SWI_OS_Args_Canonicalise (fhandle, NULL, 0,
				       &bufsize)) != NULL)
    {
      __ul_seterr (err, EOPSYS);
      return NULL;
    }

  char *rslt = malloc (bufsize);
  if (rslt == NULL)
    return NULL;

  if ((err = SWI_OS_Args_Canonicalise (fhandle, rslt, bufsize, NULL)) != NULL)
    {
      free (rslt);
      __ul_seterr (err, EOPSYS);
      return NULL;
    }

  return rslt;
}

