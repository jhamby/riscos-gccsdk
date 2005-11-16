/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/termios/cfgetospee.c,v $
 * $Date: 2004/01/02 23:33:58 $
 * $Revision: 1.3 $
 * $State: Exp $
 * $Author: joty $
 *
 ***************************************************************************/

#include <errno.h>
#include <stdlib.h>
#include <termios.h>

/* Return the output baud rate stored in the termios structure.  */
__speed_t
cfgetospeed (const struct termios *termios_p)
{
  if (termios_p == NULL)
    return __set_errno (EINVAL);

  return termios_p->c_ospeed;
}
