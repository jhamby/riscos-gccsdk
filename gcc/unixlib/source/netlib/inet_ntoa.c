/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/inet_ntoa.c,v $
 * $Date: 2000/07/15 14:52:25 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: inet_ntoa.c,v 1.1.1.1 2000/07/15 14:52:25 nick Exp $";
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
