/* Functions for RISC OS as target machine for GNU C compiler.
   Copyright (C) 2005-2010 Free Software Foundation, Inc.
   Contributed by Nick Burrett (nick@sqrt.co.uk)
   and John Tytgat (John.Tytgat@aaug.net).

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#ifndef CROSS_DIRECTORY_STRUCTURE

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"

#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

#include <swis.h>
#include <kernel.h>
#include <unixlib/local.h>

#include "obstack.h"

static const char shared_libs_path_option[] = "-L/GCCSOLib:";
static const char riscos_abi_version[] = 
#include "riscos-abi.h"
  ;

/* Function is used by the GCC_DRIVER_HOST_INITIALIZATION macro
   in gcc.c.  */
void
riscos_host_initialisation (void)
{
  /* Perform a simple memory check.  Notify the user that there is
     not enough space in the `next' slot for a task and die.  */
  _kernel_swi_regs rin, rout;

  rin.r[0] = -1;
  rin.r[1] = -1;
  if (_kernel_swi (Wimp_SlotSize, &rin, &rout))
    {
      fprintf (stderr,
	       "Failed to determine available memory for application.\n");
      exit (EXIT_FAILURE);
    }
  if (rout.r[0] < 16000 * 1024)
    {
      fprintf (stderr,
	       "Application requires a minimum of 16000K to run.\n");
      exit (EXIT_FAILURE);
    }
}

/* Called from the gcc driver to convert a RISC OS filename into a
   Unix format name.  When do_exe is non-zero we do *not* convert the
   filename as we know the executable output filename will be passed on
   to collect2 cmd line. */
const char *
riscos_convert_filename (void *obstack, const char *name,
                         int do_exe, int do_obj ATTRIBUTE_UNUSED)
{
  char tmp[1024];

  if (do_exe)
    return name;

  if (!__unixify_std (name, tmp, sizeof (tmp), 0))
    return name;

  return obstack_copy0 ((struct obstack *) obstack, tmp, strlen (tmp));
}

/* This will be called by the spec parser in gcc.c when it sees
   a %:riscos_multilib_dir(args) construct in SUBTARGET_EXTRA_LINK_SPEC
   defined in riscos-elf.h.
   Returns command line parameters that allow the linker to find the
   multilib shared libraries.  */
const char *
riscos_multilib_dir (int argc, const char **argv)
{
  return concat (shared_libs_path_option,
		 "/",
		 riscos_abi_version,
		 " ",
		 shared_libs_path_option,
		 NULL);
}

#endif
