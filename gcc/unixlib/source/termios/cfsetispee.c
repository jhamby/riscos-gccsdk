/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/termios/cfsetispee.c,v $
 * $Date: 2004/01/02 23:33:58 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: joty $
 *
 ***************************************************************************/

#include <errno.h>
#include <stdlib.h>
#include <termios.h>

/* Set input baud rate stored in the termios structure.
   Implementation defined as to whether to return an error if baud rate is
   not possible (e.g. hardware can't support speed, or can't support split
   rates.
   NetBSD makes no effort to return errors, so we won't.  */
int
cfsetispeed (struct termios *termios_p, __speed_t speed)
{
  if (termios_p == NULL || speed > __MAX_BAUD)
    return __set_errno (EINVAL);

  termios_p->c_ispeed = speed;
  return 0;
}
