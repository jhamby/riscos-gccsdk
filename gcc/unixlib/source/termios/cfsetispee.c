/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/termios/cfsetispee.c,v $
 * $Date: 2000/07/15 14:52:35 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: cfsetispee.c,v 1.1.1.1 2000/07/15 14:52:35 nick Exp $";
#endif

#include <termios.h>

/* Set input baud rate stored in the termios structure.
   Implementation defined as to whether to return an error if buad rate is
   not possible (eghardware can't support speed, or can't support split rates.
   NetBSD makes no effort to return errors, so we won't.  */
int
cfsetispeed (struct termios *termios_p, __speed_t speed)
{
  termios_p->__ispeed = speed;
  return 0;
}
