/*
 * AS an assembler for ARM
 * Copyright (c) 1997 Darren Salt
 * Copyright (c) 2003-2008 GCCSDK Developers
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * strdup.c
 */

#include "config.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"


#if !defined (HAVE_STRNDUP)
char *
strndup (const char *str, size_t len)
{
  char *c;
  if ((c = malloc (len + 1)) == NULL)
    return NULL;
  memcpy (c, str, len);
  c[len] = 0;

  return c;
}
#endif

#if !defined (HAVE_STRDUP)
char *
strdup (const char *str)
{
  size_t len = strlen(str) + 1;
  char *c;
  if ((c = malloc (len)) == NULL)
    return NULL;
  memcpy (c, str, len);

  return c;
}
#endif

#if !defined (HAVE_STRNCASECMP)
int
strncasecmp (const char *str1, const char *str2, size_t n)
{
  int i, j;

  if (n == 0)
    return 0;

  do
    {
      i = *str1++, j = *str2++;
      i = toupper (i);
      j = toupper (j);
    }
  while (i && i == j && --n);

  return i - j;
}
#endif
