/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/netlib/c/inet_ntoa,v $
 * $Date: 1997/10/09 20:00:20 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: inet_ntoa,v 1.4 1997/10/09 20:00:20 unixlib Exp $";
#endif

#include <stdio.h>

#include <arpa/inet.h>
#include <netinet/in.h>

/*
 * Format an internet address in string form
 */
char *
inet_ntoa (struct in_addr in)
{
  static char string[16];

  /* Print the string into the buffer */
  sprintf (string, "%ld.%ld.%ld.%ld", in.s_addr & 0xff,
	   (in.s_addr >> 8) & 0xff, (in.s_addr >> 16) & 0xff,
	   (in.s_addr >> 24) & 0xff);

  return string;
}
