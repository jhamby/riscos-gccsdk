/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/termios/ctermid.c,v $
 * $Date: 2001/01/29 15:10:21 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: ctermid.c,v 1.2 2001/01/29 15:10:21 admin Exp $";
#endif

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
