/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/netlib/c/h_errno,v $
 * $Date: 1997/10/09 20:00:18 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: h_errno,v 1.2 1997/10/09 20:00:18 unixlib Exp $";
#endif

/* Please note that this should not be defined in any header files.  */

volatile int h_errno = 0;
