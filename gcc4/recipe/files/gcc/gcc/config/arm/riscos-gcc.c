/* Functions for RISC OS as target machine for GNU C compiler.
   Copyright (C) 1997, 1999, 2003, 2004, 2005 Free Software Foundation, Inc.
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

#ifndef CROSS_COMPILE

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
  if (rout.r[0] < 6000 * 1024)
    {
      fprintf (stderr,
	       "Application requires a minimum of 6000K to run.\n");
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

#endif
