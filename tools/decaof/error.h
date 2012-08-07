/*
 * error/warning report function prototypes
 *
 * Copyright (c) 1992 Andy Duplain, andy.duplain@dsl.pipex.com
 * Copyright (c) 2005-2012 GCCSDK Developers
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#ifndef ERROR_HEADER_INCLUDED
#define ERROR_HEADER_INCLUDED

#include <stdarg.h>

extern int nrerrs;

extern void error (const char *fmt, ...)
  __attribute__ ((__format__ (__printf__, 1, 2)));
extern void warning (const char *fmt, ...)
  __attribute__ ((__format__ (__printf__, 1, 2)));

#endif
