/*
 * AS an assembler for ARM
 * Copyright © 1992 Niklas Röjemo
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
 * riscos.c
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#if defined(CROSS_COMPILE) || defined(UNIXLIB)
#include <unistd.h>
#endif
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "error.h"
#include "main.h"
#include "os.h"

#ifndef CROSS_COMPILE
# ifdef UNIXLIB
#  include <unixlib/local.h>
# endif /* UNIXLIB */

static char filename[1024];

char *CanonicalisePath (const char *path1)
{
  int size;
  char *buffer;
  char path[1024];
  __riscosify (path1, 0, 0, path, sizeof (path), NULL);
  if (dde && *path == '@')
    {				/* Replace @ with <Prefix$Dir> if dde flags */
      strcpy (filename, "<Prefix$Dir>");
      strcat (filename, path + 1);
    }
  else
    {
      strcpy (filename, path);
    }
  size = 1 - OSCanonicalisePath (filename, 0, 0, 0, 0);
  if ((buffer = malloc (size)) == NULL)
    {
      errorOutOfMem("CanonicalisePath");
      return NULL;
    }
  size = OSCanonicalisePath (filename, buffer, size, 0, 0);
  if (size == 1)
    return buffer;

  error (ErrorAbort, TRUE, "Internal error in CanonicalisePath");
  return NULL;			/* keep the compiler happy */
}

#endif /* ! CROSS_COMPILE */
