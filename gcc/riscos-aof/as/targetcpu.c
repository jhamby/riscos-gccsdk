/*
 * targetcpu.c
 * Copyright © 1997 Darren Salt
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "error.h"
#include "main.h"
#include "targetcpu.h"

TargetCPU_t targetCPU = UNKNOWN;


int
as_target (const char *target)	/* only called from main() */
{
  static const struct
    {
      const char *name;
      TargetCPU_t type;
      size_t name_len;
    }
  cpu[] =
  {
    {
      "ARM2", ARM2, sizeof("ARM2")
    }
    ,
    {
      "ARM250", ARM250, sizeof("ARM250")
    }
    ,
    {
      "ARM3", ARM3, sizeof("ARM3")
    }
    ,
    {
      "ARM6", ARM6, sizeof("ARM6")
    }
    ,
    {
      "ARM7", ARM7, sizeof("ARM7")
    }
    ,
    {
      "ARM7M", ARM7M, sizeof("ARM7M")
    }
    ,
    {
      "ARM8", ARM8, sizeof("ARM8")
    }
    ,
    {
      "ARM9", ARM9, sizeof("ARM9")
    }
    ,
    {
      "SA1", SA1, sizeof("SA1")
    }
    ,
    {
      "SA110", SA1, sizeof("SA110")
    }
    ,
    {
      "STRONGARM", SA1, sizeof("STRONGARM")
    }
    ,
    {
      "ARM10", ARM10, sizeof("ARM10")
    }
    ,
    {
      "XSCALE", XSCALE, sizeof("XSCALE")
    }
    ,
    {
      NULL, UNKNOWN, 0
    }
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
  for (n = 0; cpu[n].name != NULL && strncasecmp (target, cpu[n].name, cpu[n].name_len); ++n)
    /* */;
  if (cpu[n].name != NULL)
    targetCPU = cpu[n].type;
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
  error (ErrorWarning, TRUE, "Instruction unsupported on target CPU");
  return TRUE;
}
