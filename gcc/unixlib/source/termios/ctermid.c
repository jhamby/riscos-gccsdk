/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/termios/ctermid.c,v $
 * $Date: 2000/07/15 14:52:35 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: ctermid.c,v 1.1.1.1 2000/07/15 14:52:35 nick Exp $";
#endif

#include <stdio.h>
#include <string.h>


/* Return the name of the controlling terminal.
   If S is not NULL, the name is copied into it (it should be at
   least L_ctermid bytes long), otherwise a static buffer is used.  */
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
