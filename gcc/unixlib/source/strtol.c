/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/strtol.c,v $
 * $Date: 2000/07/15 14:52:10 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: strtol.c,v 1.1.1.1 2000/07/15 14:52:10 nick Exp $";
#endif

#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#undef atoi
#undef atol

int
atoi (const char *s)
{
  return (int) strtol (s, (char **) NULL, 10);
}

long
atol (const char *s)
{
  return (int) strtol (s, (char **) NULL, 10);
}

long
strtol (const char *nptr, char **end, int base)
{
  int negative = 0;
  int overflow;
  unsigned int digit;
  unsigned long result;
  unsigned long cutoff;
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

  cutoff = ULONG_MAX / (unsigned long) base;
  cutlim = ULONG_MAX % (unsigned long) base;

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
	    *end = (char *) s;
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
	  if (result > (unsigned long) LONG_MIN)
	    overflow = 1;
	}
      else
        {
	  if (result > (unsigned long) LONG_MAX)
	    overflow = 1;
	}
    }
  if (overflow)
    {
      (void) __set_errno (ERANGE);
      return (negative) ? LONG_MIN : LONG_MAX;
    }

  return (negative ? -result : result);
}
