/* GPC version information.
   Copyright (C) 1997, 2000 Free Software Foundation, Inc.
   Contributed by J.J. van der Heijden <j.j.vanderheijden@student.utwente.nl>.

This file is part of GNU Pascal.

GNU Pascal is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU Pascal is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Pascal; see the file COPYING. If not, write to the
Free Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

/* If you want to override the version date/time info with your own
   macros, e.g. for a consistent distribution when bootstrapping,
   go ahead!  */

/* Version numbers are in Make-lang.in and override these */
#ifndef GPC_VERSION
#define GPC_VERSION "[unknown]"
#endif

#ifndef GPC_DATE
#ifdef __DATE__
#define GPC_DATE __DATE__
#else   /* !defined (__DATE__) */
#define GPC_DATE "date unknown"
#endif  /* !defined (__DATE__) */
#endif  /* !defined (GPC_DATE) */

#ifndef GPC_TIME
#ifdef __TIME__
#define GPC_TIME __TIME__
#else   /* !defined (__TIME__) */
#define GPC_TIME "time unknown"
#endif  /* !defined (__TIME__) */
#endif  /* !defined (GPC_TIME) */

#include "../version.c"
char *gpc_version_string = GPC_VERSION;
char *gpc_date = GPC_DATE;
char *gpc_time = GPC_TIME;
