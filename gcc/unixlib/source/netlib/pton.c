/****************************************************************************
 *
 * $Source$
 * $Date$
 * $Revision$
 * $State$
 * $Author$
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id$";
#endif

/* Copyright (c) 1996 by Internet Software Consortium.

 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND INTERNET SOFTWARE CONSORTIUM DISCLAIMS
 * ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL INTERNET SOFTWARE
 * CONSORTIUM BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <arpa/nameser.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/types.h>

/* int
 * inet_pton4(src, dst)
 *	like inet_aton() but without all the hexadecimal and shorthand.
 * return:
 *	1 if `src' is a valid dotted quad, else 0.
 * notice:
 *	does not touch `dst' unless it's returning 1.
 * author:
 *	Paul Vixie, 1996.
 */
static int
inet_pton4 (const char *src, u_char *dst)
{
  u_int32_t val;
  int base, n;
  unsigned char c;
  u_int parts[4];
  u_int *pp = parts;

  c = *src;
  for (;;)
    {
      /* Collect number up to ``.''. Values are specified as for C:
	 0x=hex, 0=octal, isdigit=decimal.  */
      if (!isdigit(c))
	return 0;
      val = 0;
      base = 10;
      if (c == '0')
	{
	  c = *++src;
	  if (c == 'x' || c == 'X')
	    base = 16, c = *++src;
	  else
	    base = 8;
	}
      for (;;)
	{
	  if (isdigit(c))
	    {
	      val = (val * base) + (c - '0');
	      c = *++src;
	    }
	  else if (base == 16 && isxdigit(c))
	    {
	      val = (val << 4) | (c + 10 - (islower(c) ? 'a' : 'A'));
	      c = *++src;
	    }
	  else
	    break;
	}
      if (c == '.')
	{
	  /* Internet format:
	       a.b.c.d
	       a.b.c   (with c treated as 16 bits)
	       a.b     (with b treated as 24 bits)

	     We only store three parts in parts, hence check parts + 3.  */
	  if (pp >= parts + 3)
	    return 0;
	  *pp++ = val;
	  c = *++src;
	}
      else
	break;
    }

  /* Check for trailing characters.  */
  if (c != '\0' && !isspace(c))
    return 0;

  /* Concoct the address according to the number of parts specified.  */
  n = pp - parts + 1;
  switch (n)
    {
    case 0:
      return 0;	   /* Initial nondigit.  */

    case 1:	   /* a -- 32 bits.  */
      break;

    case 2:	   /* a.b -- 8.24 bits.  */
      if (val > 0xffffff)
	return 0;
      val |= parts[0] << 24;
      break;

    case 3:	   /* a.b.c -- 8.8.16 bits.  */
      if (val > 0xffff)
	return 0;
      val |= (parts[0] << 24) | (parts[1] << 16);
      break;

    case 4:	   /* a.b.c.d -- 8.8.8.8 bits.  */
      if (val > 0xff)
	return 0;
      val |= (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8);
      break;
    }

  if (dst != NULL)
    {
      val = (u_int32_t) htonl (val);
      memcpy (dst, &val, INADDRSZ);
    }
  return 1;
}

/* int
 * inet_pton6(src, dst)
 *	convert presentation level address to network order binary form.
 * return:
 *	1 if `src' is a valid [RFC1884 2.2] address, else 0.
 * notice:
 *	(1) does not touch `dst' unless it's returning 1.
 *	(2) :: in a full address is silently ignored.
 * credit:
 *	inspired by Mark Andrews.
 * author:
 *	Paul Vixie, 1996.
 */
static int
inet_pton6 (const char *src, u_char * dst)
{
  static const char xdigits_l[] = "0123456789abcdef";
  static const char xdigits_u[] = "0123456789ABCDEF";
  u_char tmp[IN6ADDRSZ], *tp = tmp, *endp, *colonp;
  const char *xdigits, *curtok;
  int ch, saw_xdigit;
  u_int val;

  memset (tp, 0, IN6ADDRSZ);
  endp = tp + IN6ADDRSZ;
  colonp = NULL;
  /* Leading :: requires some special handling.  */
  if (*src == ':')
    if (*++src != ':')
      return 0;
  curtok = src;
  saw_xdigit = 0;
  val = 0;
  while ((ch = *src++) != '\0')
    {
      const char *pch;

      if ((pch = strchr((xdigits = xdigits_l), ch)) == NULL)
	pch = strchr((xdigits = xdigits_u), ch);
      if (pch != NULL)
	{
	  val <<= 4;
	  val |= (pch - xdigits);
	  if (val > 0xffff)
	    return 0;
	  saw_xdigit = 1;
	  continue;
	}
      if (ch == ':')
	{
	  curtok = src;
	  if (!saw_xdigit)
	    {
	      if (colonp)
		return 0;
	      colonp = tp;
	      continue;
	    }
	  if (tp + INT16SZ > endp)
	    return 0;
	  *tp++ = (u_char) (val >> 8) & 0xff;
	  *tp++ = (u_char) val & 0xff;
	  saw_xdigit = 0;
	  val = 0;
	  continue;
	}
      if (ch == '.' && ((tp + INADDRSZ) <= endp) &&
	  inet_pton4 (curtok, tp) > 0)
	{
	  tp += INADDRSZ;
	  saw_xdigit = 0;
	  break;		/* '\0' was seen by inet_pton4().  */
	}
      return 0;
    }
  if (saw_xdigit)
    {
      if (tp + INT16SZ > endp)
	return 0;
      *tp++ = (u_char) (val >> 8) & 0xff;
      *tp++ = (u_char) val & 0xff;
    }
  if (colonp != NULL)
    {
      /* Since some memmove()'s erroneously fail to handle
	 overlapping regions, we'll do the shift by hand.  */
      const int n = tp - colonp;
      int i;

      for (i = 1; i <= n; i++)
	{
	  endp[-i] = colonp[n - i];
	  colonp[n - i] = 0;
	}
      tp = endp;
    }
  if (tp != endp)
    return 0;
  memcpy (dst, tmp, IN6ADDRSZ);
  return 1;
}

/* int
 * inet_pton(af, src, dst)
 *	convert from presentation format (which usually means ASCII printable)
 *	to network format (which is usually some kind of binary format).
 * return:
 *	1 if the address was valid for the specified address family
 *	0 if the address wasn't valid (`dst' is untouched in this case)
 *	-1 if some other error occurred (`dst' is untouched in this case, too)
 * author:
 *	Paul Vixie, 1996.
 */
int
inet_pton (int af, const char *src, void *dst)
{
  switch (af)
    {
    case AF_INET:
      return inet_pton4 (src, dst);
    case AF_INET6:
      return inet_pton6 (src, dst);
    default:
      return __set_errno (EAFNOSUPPORT);
    }
  /* NOTREACHED */
}
