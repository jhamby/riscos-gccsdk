/* ctermid ()
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#include <stdio.h>
#include <string.h>


/* Return the name of the controlling terminal.
   If S is not NULL, the name is copied into it (it should be at
   least L_ctermid bytes long), otherwise a static buffer is used.  */

/* Defined by POSIX as not threadsafe when passed a NULL argument */
char *
ctermid (char *s)
{
  static char name[L_ctermid];

  if (name[0] == '\0')
    strcpy (name, "/dev/tty");

  if (s == NULL)
    return name;

  return strcpy (s, name);
}
