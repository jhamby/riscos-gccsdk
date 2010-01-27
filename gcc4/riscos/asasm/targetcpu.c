/*
 * AS an assembler for ARM
 * Copyright (c) 1997 Darren Salt
 * Copyright (c) 2002-2010 GCCSDK Developers
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
 * targetcpu.c
 */

#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "error.h"
#include "main.h"
#include "targetcpu.h"
#include "os.h"
#include "variables.h"

TargetCPU_t targetCPU = UNKNOWN;
static const char *targetName = "";


/**
 * Only called from main()
 */
int
as_target (const char *target)
{
  static const struct
    {
      const char *name;
      TargetCPU_t type;
      size_t name_len;
    }
  cpu[] =
  {
    { "ARM2", ARM2, sizeof("ARM2") },
    { "ARM250", ARM250, sizeof("ARM250") },
    { "ARM3", ARM3, sizeof("ARM3") },
    { "ARM6", ARM6, sizeof("ARM6") },
    { "ARM7", ARM7, sizeof("ARM7") },
    { "ARM7M", ARM7M, sizeof("ARM7M") },
    { "ARM8", ARM8, sizeof("ARM8") },
    { "ARM9", ARM9, sizeof("ARM9") },
    { "SA1", SA1, sizeof("SA1") },
    { "SA110", SA1, sizeof("SA110") },
    { "STRONGARM", SA1, sizeof("STRONGARM") },
    { "ARM10", ARM10, sizeof("ARM10") },
    { "XSCALE", XSCALE, sizeof("XSCALE") },
    { NULL, UNKNOWN, 0 }
  };

  int n;
  if (targetCPU != UNKNOWN)
    {
      fprintf (stderr, "%s: Cannot redeclare target CPU\n", ProgName);
      return -1;
    }
  if (target == NULL)
    {
      fprintf (stderr, "%s: Missing target CPU\n", ProgName);
      return -1;
    }
  for (n = 0; cpu[n].name != NULL
	 && strncasecmp (target, cpu[n].name, cpu[n].name_len); ++n)
    /* */;
  if (cpu[n].name != NULL)
    {
      targetCPU = cpu[n].type;
      targetName = cpu[n].name;
    }
  else
    {
      targetCPU = ARM2;
      fprintf (stderr, "%s: Unrecognised target CPU <%s>, assuming ARM2\n", ProgName, target);
    }
  return 0;
}


BOOL
cpuWarn (TargetCPU_t type)	/* true if warning is output */
{
  if (targetCPU >= type)
    return FALSE;
  error (ErrorWarning, "Instruction unsupported on target CPU");
  return TRUE;
}

void
set_cpuvar (void)
{
   char buffer[40];
   snprintf (buffer, sizeof(buffer), "TARGET_CPU=%s", targetName);
   var_define (buffer);
}
