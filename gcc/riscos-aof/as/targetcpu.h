/*
 * targetcpu.h
 * Copyright © 1997 Darren Salt
 */

#ifndef __TARGETCPU_H
#define __TARGETCPU_H

#ifndef __GLOBAL_H
#include "global.h"
#endif

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

extern void as_target (const char *target);	/* only called from main() */

BOOL cpuWarn (TargetCPU_t);

#endif
