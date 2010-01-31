/* __build_ctype_tables ()
 * Copyright (c) 2000-2009 UnixLib Developers
 */

#include <ctype.h>
#include <swis.h>
#include <locale.h>

#include <pthread.h>
#include <internal/os.h>
#include <internal/unix.h>

void
__build_ctype_tables (int territory)
{
  int regs[10], x, y, code, pos, offset;
  unsigned int *table, bits;
  char *p, *q;

  PTHREAD_UNSAFE

  if (territory == -2)
    {
      /* Initialise the array. This is only done by __unixinit().  */
      territory = -1;
      for (x = 0; x <= LC_ALL; x++)
        __locale_territory[x] = -1;
    }

  /* Initialise ctype tables.. */
  for (x = 0; x <= 255; x++)
    {
      __ctype[x] = 0;
      /* In the C/POSIX locate, tolower(top bit set char)
	 should return the character unchanged.  */
      __ctype_lower[x] = __ctype_upper[x] = x;
    }

  /* Support EOF.  This is not a bug, the arrays must
     start at minus 1.  See <ctype.h>.  */
  __ctype[-1] = 0;
  __ctype_lower[-1] = __ctype_upper[-1] = -1;

  for (code = 0; code <= 7; code ++)
    {
      /* This SWI produces a 256-bit table of a particular
	 character type.  Our goal is to merge 8 of these
	 256-bit tables into one 256-byte table.  */
      regs[0] = territory;
      regs[1] = code;
      __os_swi (Territory_CharacterPropertyTable, regs);
      pos = 0;
      table = (unsigned int *)regs[0];
      y = (territory == -1) ? 4 : 8; /* See C/POSIX comment below.  */
      for (x = 0; x <= y; x++)
	{
	  /* Process 32 characters.  */
	  bits = *table++;
	  offset = 0;
	  while (bits)
	    {
	      if (bits & 1)
		__ctype[pos + offset] |= 1 << code;
	      bits = bits >> 1;
	      offset += 1;
	    }
	  pos += 32;
	}
    }

  /* Create the lower-case and upper-case tables.  */
  regs[0] = territory;
  __os_swi (Territory_LowerCaseTable, regs);
  p = (char *)regs[0];
  regs[0] = territory;
  __os_swi (Territory_UpperCaseTable, regs);
  q = (char *)regs[0];
  /* In the C/POSIX locale, top-bit set characters aren't supported.
     The isgraph(), isalpha() etc. functions will return false.
     toupper() and tolower() will return the character unchanged.  */
  y = (territory == -1) ? 127 : 255;
  for (x = 0; x <= y; x++)
    {
      __ctype_lower[x] = (short)*p++;
      __ctype_upper[x] = (short)*q++;
    }
}
