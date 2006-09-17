/* Configuration for GNU compilers for processor running RISC OS.
   Copyright (C) 1998, 2003, 2004 Free Software Foundation, Inc.
   Contributed by Nick Burrett (nick@sqrt.co.uk)

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

/* This file defines all the specific features for GCC when running
   on RISC OS.  */

/* For the throwback of GCC errors to a text editor.  */
extern void arm_error_throwback (const char *file, int line,
				 const char *prefix, const char *s);
extern void arm_throwback_finish (void);
extern void arm_gnat_error_throwback ();
extern void riscos_host_initialisation (void);
extern const char *riscos_convert_filename (void *obstack,
			const char *name, int do_exe, int do_obj);

#ifndef CROSS_COMPILE
/* Defines for the implementation of throwbacks.  */

#define ERROR_THROWBACK(FILE,LINE,PREFIX,S)		\
  do {							\
    if (TARGET_THROWBACK)				\
      arm_error_throwback (FILE, LINE, PREFIX, S);	\
    } while (0)

#define ERROR_THROWBACK_FINALISE arm_throwback_finish ()
#define GCC_DRIVER_HOST_INITIALIZATION riscos_host_initialisation ()
#define TARGET_CONVERT_FILENAME(a,b,c,d) return riscos_convert_filename (a,b,c,d)
#endif

/* Character constant used in separating components in paths.  */
#undef PATH_SEPARATOR
#define PATH_SEPARATOR ','

/* Directory name separator.  */
#define DIR_SEPARATOR '/'

/* We don't have STAB (or any) debugging info, at the moment.  */
#define NO_STAB_H

/* Maths operation domain error number, EDOM.  For CLib it is 1.  */
#define TARGET_EDOM 33
