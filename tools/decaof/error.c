/*
 * report errors and warnings
 *
 * Copyright (c) 1992 Andy Duplain, andy.duplain@dsl.pipex.com
 * Copyright (c) 2005-2013 GCCSDK Developers
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */

#include "config.h"

#include <stdio.h>

#include "error.h"
#include "main.h"

int nrerrs = 0;

void
error (const char *fmt, ...)
{
  va_list ap;
  va_start (ap, fmt);
  fprintf (stderr, PACKAGE " error: ");
  vfprintf (stderr, fmt, ap);
  putc ('\n', stderr);
  va_end (ap);

  ++nrerrs;
}

void
warning (const char *fmt, ...)
{
  va_list ap;
  va_start (ap, fmt);
  fprintf (stderr, PACKAGE " warning: ");
  vfprintf (stderr, fmt, ap);
  putc ('\n', stderr);
  va_end (ap);
}
