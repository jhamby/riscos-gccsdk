/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/strtoll.c,v $
 * $Date: 2002/02/11 13:16:40 $
 * $Revision: 1.2.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strtoll.c,v 1.2.2.1 2002/02/11 13:16:40 admin Exp $";
#endif

#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#ifdef __GNUC__
long long
strtoll (const char *nptr, char **end, int base)
{
  int negative = 0;
  int overflow;
  unsigned int digit;
  unsigned long long result;
  unsigned long long cutoff;
  unsigned long cutlim;
  const char *s, *save;

  if (!nptr)
    return 0;

  if (base < 0 || base == 1 || base > 36)
    return __set_errno (EINVAL);

  save = s = nptr;
  /* Skip white space.  */
  while (isspace (*s))
    s++;

  if (*s == '+' || *s == '-')
    {
      if (*s == '-')
	negative = 1;
      s++;
    }

  if (base == 16 && s[0] == '0' && toupper (s[1]) == 'X')
    s += 2;

  /* Try guessing the base.  */
  if (base == 0)
    {
      if (*s == '0')
	{
	  if (toupper (s[1]) == 'X')
	    {
	      s += 2;
	      base = 16;
	    }
	  else
	    base = 8;
	}
      else
	base = 10;
    }

  /* Save the pointer so we can check later if anything happened.  */
  save = s;

  cutoff = ULONG_LONG_MAX / (unsigned long) base;
  cutlim = ULONG_LONG_MAX % (unsigned long) base;

  overflow = 0;
  result = 0;
  for (digit = *s; digit != '\0'; digit = *++s)
    {
      if (digit >= '0' && digit <= '9')
	digit -= '0';
      else if (isalpha (digit))
	digit = toupper (digit) - 'A' + 10;
      else
	break;
      if (digit >= base)
	break;
      /* Overflow checks. */
      if (result > cutoff || (result == cutoff && digit > cutlim))
	overflow = 1;
      else
	{
	  result *= (unsigned long) base;
	  result += digit;
	}
    }
  if (s == save)
    {
      if (end != NULL)
	{
	  /* Special case, the base is zero or hexadecimal and the
	     first two chars are 0x, but nothing else follows. This
	     isn't an error.  */
	  if ((save - nptr) >= 2 && toupper (save[-1]) == 'X' && save[-2] == '0')
	    *end = (char *) &save[-1];
	  else
	    *end = (char *) nptr;
	}
      return 0;
    }

  /* Store the address of one character past the last we converted.  */
  if (end)
    *end = (char *) s;

  if (overflow == 0)
    {
      /* Check for a value that is within the range of
         an unsigned long but outside the range of long.  */
      if (negative)
        {
	  if (result > (unsigned long) LONG_LONG_MIN)
	    overflow = 1;
	}
      else
        {
	  if (result > (unsigned long) LONG_LONG_MAX)
	    overflow = 1;
	}
    }
  if (overflow)
    {
      (void) __set_errno (ERANGE);
      return (negative) ? LONG_LONG_MIN : LONG_LONG_MAX;
    }

  return (negative ? -result : result);
}
#endif
