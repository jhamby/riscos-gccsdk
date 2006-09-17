/*
 * AS an assembler for ARM
 * Copyright (c) 1997 Darren Salt
 * Copyright (c) 2002-2006 GCCSDK Developers
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
 * targetcpu.h
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

void set_cpuvar (void);

#endif
