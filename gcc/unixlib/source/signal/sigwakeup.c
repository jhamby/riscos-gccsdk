/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/signal/c/sigwakeup,v $
 * $Date: 1997/10/09 20:00:31 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: sigwakeup,v 1.5 1997/10/09 20:00:31 unixlib Exp $";
#endif

/* Written by Nick Burrett, 26 August 1996.  */

#include <sys/unix.h>

/* Wake a task up.  */
void
sigwakeup (void)
{
  __u->sleeping = 0;
}
