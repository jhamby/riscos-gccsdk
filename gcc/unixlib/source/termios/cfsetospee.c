/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/termios/c/cfsetospee,v $
 * $Date: 1998/06/25 21:35:31 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: cfsetospee,v 1.1 1998/06/25 21:35:31 unixlib Exp $";
#endif

#include <termios.h>

/* Set output baud rate stored in the termios structure.
   implementation defined as to whether to return an error if buad rate is
   not possible (eghardware can't support speed, or can't support split rates.
   NetBSD makes no effort to return errors, so we won't.  */
int
cfsetospeed (struct termios *termios_p, __speed_t speed)
{
  termios_p->__ospeed = speed;
  return 0;
}
