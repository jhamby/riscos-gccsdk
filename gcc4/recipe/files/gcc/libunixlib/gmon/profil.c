/* Low-level statistical profiling support function.  RISC OS version.
   Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <internal/machine-gmon.h>
#include <kernel.h>
#include <internal/unix.h>

/*#define DEBUG 1*/
#ifdef DEBUG
#  include <sys/debug.h>
#endif

/* Enable statistical profiling, writing samples of the PC into at most
   SIZE bytes of SAMPLE_BUFFER; every processor clock tick while profiling
   is enabled, the system examines the user PC and increments
   SAMPLE_BUFFER[((PC - OFFSET) / 2) * SCALE / 65536].  If SCALE is zero,
   disable profiling.  Returns zero on success, -1 on error.  */

int
__profil (u_short *sample_buffer, size_t size, size_t offset, u_int scale)
{
  struct ul_global *gbl = &__ul_global;
  profiler_data_t *prof = &profiler;
  _kernel_oserror *err;

  if (sample_buffer == NULL || scale == 0)
    {
      /* Disable profiling.  */
      if (prof->handler_data->flags & INT_HANDLER_FLAG_DISABLED_BY_USER)
	/* Wasn't turned on.  */
	return 0;

      prof->handler_data->flags |= INT_HANDLER_FLAG_DISABLED_BY_USER;

#ifdef DEBUG
      debug_printf("Profiling stopped\n");
#endif

      return 0;
    }

  if (!(prof->handler_data->flags & INT_HANDLER_FLAG_DISABLED_BY_USER))
    {
      /* Was already turned on.  */
      return 0;
    }

  if (prof->flags & PROFILER_FLAG_FIRST_TIME)
    {
      /* Set up the data that the interrupt handler requires.  */
      prof->handler_data->samples = (unsigned int)sample_buffer;
      prof->handler_data->nsamples = size / sizeof *sample_buffer;
      prof->handler_data->pc_offset = offset;
      prof->handler_data->pc_scale = scale;

      if ((err = __profile_enable (gbl, prof)) != NULL)
	{
#ifdef DEBUG
	  debug_printf ("Could not start profiling: %s\n", err->errmess);
#endif
	  return -1;
	}

      prof->flags &= (~PROFILER_FLAG_FIRST_TIME);
    }

  prof->handler_data->flags &= (~INT_HANDLER_FLAG_DISABLED_BY_USER);

#ifdef DEBUG
  debug_printf("Profiling started\n");
#endif

  return 0;
}
weak_alias (__profil, profil)
