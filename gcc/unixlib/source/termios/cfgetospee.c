/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/termios/cfgetospee.c,v $
 * $Date: 2000/07/15 14:52:35 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: cfgetospee.c,v 1.1.1.1 2000/07/15 14:52:35 nick Exp $";
#endif

#include <termios.h>

/* Return the output baud rate stored in the termios structure.  */
__speed_t
cfgetospeed (const struct termios *termios_p)
{
  return termios_p->__ospeed;
}
