/*
 * targetcpu.c
 * Copyright © 1997 Darren Salt
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "targetcpu.h"
#include "error.h"

TargetCPU_t targetCPU = UNKNOWN;


extern char *ProgName;		/* from main.c */

void 
as_target (char *target)	/* only called from main() */
{
  static const struct
    {
      const char *name;
      TargetCPU_t type;
    }
  cpu[] =
  {
    {
      "ARM2", ARM2
    }
    ,
    {
      "ARM250", ARM250
    }
    ,
    {
      "ARM3", ARM3
    }
    ,
    {
      "ARM6", ARM6
    }
    ,
    {
      "ARM7", ARM7
    }
    ,
    {
      "ARM7M", ARM7M
    }
    ,
    {
      "ARM8", ARM8
    }
    ,
    {
      "ARM9", ARM9
    }
    ,
    {
      "SA1", SA1
    }
    ,
    {
      "SA110", SA1
    }
    ,
    {
      "STRONGARM", SA1
    }
    ,
    {
      "ARM10", ARM10
    }
    ,
    {
      0, UNKNOWN
    }
  };

  int n;
  char *c;

  if (targetCPU != UNKNOWN)
    {
      fprintf (stderr, "%s: Cannot redeclare target CPU\n", ProgName);
      return;
    }
  targetCPU = ARM2;
  if (target == 0)
    {
      fprintf (stderr, "%s: Missing target CPU\n", ProgName);
      exit (1);
    }
  c = strdup (target);
  n = 0;
  while (c[n])
    {
      c[n] = toupper (c[n]);
      n++;
    }
  n = 0;
  do
    {
      if (!strcmp (c, cpu[n].name))
	{
	  free (c);
	  targetCPU = cpu[n].type;
	  return;
	}
    }
  while (cpu[++n].name);
  free (c);
  fprintf (stderr, "%s: Unrecognised target CPU, assuming ARM2\n", ProgName);
}


BOOL 
cpuWarn (TargetCPU_t type)	/* true if warning is output */
{
  if (targetCPU >= type)
    return FALSE;
  error (ErrorWarning, TRUE, "Instruction unsupported on target CPU");
  return TRUE;
}
