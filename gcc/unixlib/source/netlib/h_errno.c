/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/h_errno.c,v $
 * $Date: 2000/07/15 14:52:24 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: h_errno.c,v 1.1.1.1 2000/07/15 14:52:24 nick Exp $";
#endif

/* Please note that this should not be defined in any header files.  */

volatile int h_errno = 0;
