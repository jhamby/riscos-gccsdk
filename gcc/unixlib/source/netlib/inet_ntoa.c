/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/inet_ntoa.c,v $
 * $Date: 2004/06/05 19:53:23 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: joty $
 *
 ***************************************************************************/

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
  sprintf (string, "%u.%u.%u.%u", in.s_addr & 0xff,
	   (in.s_addr >> 8) & 0xff, (in.s_addr >> 16) & 0xff,
	   (in.s_addr >> 24) & 0xff);

  return string;
}
