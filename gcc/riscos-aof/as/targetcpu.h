/*
 * targetcpu.h
 * Copyright © 1997 Darren Salt
 */

#ifndef targetcpu_header_included
#define targetcpu_header_included

#include "global.h"

typedef enum
{
  UNKNOWN,
  ARM2,
  ARM250,
  ARM3,
  ARM6,
  ARM7,
  ARM7M,
  ARM8,
  ARM9,
  SA1,
  ARM10,
  XSCALE
}
TargetCPU_t;

extern TargetCPU_t targetCPU;

int as_target (const char *target);

BOOL cpuWarn (TargetCPU_t);

#endif
