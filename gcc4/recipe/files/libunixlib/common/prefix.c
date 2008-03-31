/* __get_dde_prefix ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <stdlib.h>
#include <string.h>
#include <swis.h>

#include <unixlib/local.h>
#include <internal/os.h>
#include <internal/unix.h>

/* Returns malloced block of memory holding DDEUtils_Prefix non zero-length
   string value or NULL when there isn't one set.  Caller needs to free
   memory after use.  */
const char *__get_dde_prefix (void)
{
  int regs[10];
  char *result;

  regs[0] = 0; /* Use the currently active object number.  */
  if (__os_swi (DDEUtils_ReadPrefix, regs) == NULL)
    {
      size_t size;
      const char *prefix;

      if ((prefix = (const char *)regs[0]) == NULL)
        return NULL;

      /* returned string can be control char terminated.  */
      while ((unsigned char)*prefix >= ' ')
        ++prefix;
      /* Filter out zero length results.  */
      if ((size = prefix - (const char *)regs[0]) == 0)
        return NULL;
      if ((result = (char *)malloc (size + 1)) == NULL)
        return NULL;
      memcpy (result, (const void *)regs[0], size);
      result[size] = '\0';
    }
  else
    {
      /* SWI DDEUtils_ReadPrefix is not known.  */
      result = __getenv_from_os ("Prefix$Dir", NULL, 0, NULL);

      /* Filter out zero length results.  */
      if (result[0] == '\0')
        {
          free (result);
          return NULL;
        }
    }

  return result;
}
