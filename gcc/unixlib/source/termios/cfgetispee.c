/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/termios/cfgetispee.c,v $
 * $Date: 2000/07/15 14:52:35 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: cfgetispee.c,v 1.1.1.1 2000/07/15 14:52:35 nick Exp $";
#endif

#include <termios.h>

/* Return the input baud rate stored in the termios structure.  */
__speed_t
cfgetispeed (const struct termios *termios_p)
{
  return termios_p->__ispeed;
}
