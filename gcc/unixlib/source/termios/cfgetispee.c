/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/termios/cfgetispee.c,v $
 * $Date: 2001/01/29 15:10:21 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: cfgetispee.c,v 1.2 2001/01/29 15:10:21 admin Exp $";
#endif

#include <errno.h>
#include <stdlib.h>
#include <termios.h>

/* Return the input baud rate stored in the termios structure.  */
__speed_t
cfgetispeed (const struct termios *termios_p)
{
  if (termios_p == NULL)
    return __set_errno (EINVAL);

  return termios_p->c_ispeed;
}
