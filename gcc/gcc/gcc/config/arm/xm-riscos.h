/* Configuration for GNU compilers for processor running RISC OS.
   Copyright (C) 1998 Free Software Foundation, Inc.
   Contributed by Nick Burrett (nick.burrett@btinternet.com)

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
   on RISC OS.  It is intended for compilation with UnixLib 3.7c.  */

/* For the throwback of GCC errors to a text editor.  */
extern void arm_error_throwback ();
extern void arm_throwback_finish ();
extern void arm_gnat_error_throwback ();

#if !defined (__GNUC__) && defined(__riscos)
/* For Acorn C to compile a stage 1 compiler, these must be defined.  */
#define CODE_FIELD_BUG  1
#define ONLY_INT_FIELDS 1
#endif

#ifndef CROSS_COMPILE
/* Defines for the implementation of throwbacks.  */

#define ERROR_THROWBACK(FILE,LINE,PREFIX,S,AP)        \
{						      \
  if (TARGET_THROWBACK)				      \
    arm_error_throwback (FILE, LINE, PREFIX, S, AP);  \
}

#define ERROR_THROWBACK_FINALISE arm_throwback_finish ()
#endif

/* Character constant used in separating components in paths.  */
#define PATH_SEPARATOR ','

/* Directory name separator.  */
#define DIR_SEPARATOR '/'

/* We don't have STAB (or any) debugging info, at the moment.  */
#define NO_STAB_H

/* Maths operation domain error number, EDOM.  For CLib it is 1.  */
#define TARGET_EDOM 33

