/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/termios/c/cfgetispee,v $
 * $Date: 1998/06/25 21:35:31 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: cfgetispee,v 1.1 1998/06/25 21:35:31 unixlib Exp $";
#endif

#include <termios.h>

/* Return the input baud rate stored in the termios structure.  */
__speed_t
cfgetispeed (const struct termios *termios_p)
{
  return termios_p->__ispeed;
}
