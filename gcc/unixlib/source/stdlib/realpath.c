/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdlib/Attic/realpath.c,v $
 * $Date: 2001/10/04 12:50:46 $
 * $Revision: 1.1.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#define _GNU_SOURCE
#include <errno.h>
#include <stdlib.h>
#include <kernel.h>
#include <swis.h>
#include <limits.h>
#include <unixlib/local.h>
#include <unixlib/os.h>

/* Canonicalise a filename */
/* resolved_name points to a buffer of at least PATH_MAX bytes */
char *
realpath (const char *file_name, char *resolved_name)
{
  char buffer[PATH_MAX];
  int r[10];

  if (file_name == NULL || resolved_name == NULL)
    {
      __set_errno (EINVAL);
      return NULL;
    }

  if (__riscosify_std (file_name, 0, resolved_name, PATH_MAX, NULL) == NULL)
    {
      __set_errno (ENAMETOOLONG);
      return NULL;
    }
  
  r[0] = 37; /* Canonicalise path */
  r[1] = (int) resolved_name;
  r[2] = (int) buffer;
  r[3] = 0;
  r[4] = 0;
  r[5] = PATH_MAX;
  if (__os_swi (OS_FSControl, r) != NULL)
    {
      __set_errno (EIO);
      return NULL;
    }

  if (__unixify_std (buffer, resolved_name, PATH_MAX,
		     __RISCOSIFY_FILETYPE_NOTSPECIFIED) == NULL)
    {
      __set_errno (ENAMETOOLONG);
      return NULL;
    }

  return resolved_name;
}
