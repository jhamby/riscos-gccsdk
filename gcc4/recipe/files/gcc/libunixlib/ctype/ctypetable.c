/* __build_ctype_tables ()
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#include <ctype.h>
#include <swis.h>
#include <locale.h>

#include <pthread.h>
#include <internal/os.h>
#include <internal/unix.h>

/* Global containing current locale settings. */
struct _locale __locale_global;
/* Offset 1 as the first entry is reserved for EOF.  */
const unsigned char * const __ctype = &__locale_global.ctype[1];
const short * const __ctype_upper = &__locale_global.ctype_upper[1];
const short * const __ctype_lower = &__locale_global.ctype_lower[1];

void
__build_ctype_tables (locale_t locobj, int territory)
{
  PTHREAD_UNSAFE

  if (territory == -2)
    {
      /* Initialise the array. This is only done by __unixinit()/newlocale().  */
      territory = -1;
      for (int x = 0; x < LC_ALL; x++)
        locobj->locale_territory[x] = -1;
      __localeconv_lconv_init(&locobj->lc);
      locobj->lc_needs_refresh = 1;
    }

  /* Initialise ctype_upper/ctype_lower tables.  */
  for (int x = 0; x < 257; x++)
    {
      /* In the C/POSIX locale, tolower(top bit set char)
	 should return the character unchanged.  */
      locobj->ctype_lower[x] = locobj->ctype_upper[x] = x - 1;
      locobj->ctype[x] = 0;
    }

  int regs[10];
  for (int code = 0; code <= 7; code ++)
    {
      /* This SWI produces a 256-bit table of a particular
	 character type.  Our goal is to merge 8 of these
	 256-bit tables into one 256-byte table.  */
      regs[0] = territory;
      regs[1] = code;
      __os_swi (Territory_CharacterPropertyTable, regs);
      const unsigned int *table = (const unsigned int *)regs[0];
      int y = (territory == -1) ? 4 : 8; /* See C/POSIX comment below.  */
      int pos = 0;
      for (int x = 0; x < y; x++)
	{
	  /* Process 32 characters.  */
	  unsigned int bits = *table++;
	  for (int offset = 1; bits; bits = bits >> 1, offset += 1)
	    {
	      if (bits & 1)
		locobj->ctype[pos + offset] |= 1 << code;
	    }
	  pos += 32;
	}
    }

  /* Create the lower-case and upper-case tables.  */
  regs[0] = territory;
  __os_swi (Territory_LowerCaseTable, regs);
  const char *p = (const char *)regs[0];
  regs[0] = territory;
  __os_swi (Territory_UpperCaseTable, regs);
  const char *q = (const char *)regs[0];
  /* In the C/POSIX locale, top-bit set characters aren't supported.
     The isgraph(), isalpha() etc. functions will return false.
     toupper() and tolower() will return the character unchanged.  */
  int y = (territory == -1) ? 128 : 256;
  for (int x = 1; x <= y; x++)
    {
      locobj->ctype_lower[x] = (short)*p++;
      locobj->ctype_upper[x] = (short)*q++;
    }
}
