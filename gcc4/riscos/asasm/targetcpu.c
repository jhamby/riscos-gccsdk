/*
 * AS an assembler for ARM
 * Copyright (c) 2010 GCCSDK Developers
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
#include "targetcpu.h"

/* Keep this in sync with enum ARM_eArchitectures.  */
static const char * const oARMArch[] =
{
  "1",
  "2",
  "2a",
  "3",
  "3M",
  "4",
  "4T",
  "5T",
  "5TE",
  "5TEJ",
  "6",
  "6J",
  "6K",
  "6KZ",
  "6Z",
  "6M",
  "6T2",
  "7-A",
  "7-R",
  "7E-M",
  "7-M"
};

typedef struct
{
  const char *core;
  ARM_eArchitectures arch;
} Core2Arch_t;

/* Maps ARM cores to architectures: */
const Core2Arch_t oCore2Arch[] =
{
  /* V1 */
  { "ARM1", ARCH_ARMv1 },

  /* V2 / V2A */
  { "ARM2", ARCH_ARMv2 },
  { "ARM250", ARCH_ARMv2a },
  { "ARM3", ARCH_ARMv2a },

  /* V3 */
  { "ARM6", ARCH_ARMv3 },
  { "ARM60", ARCH_ARMv3 },
  { "ARM600", ARCH_ARMv3 },
  { "ARM610", ARCH_ARMv3 },
  { "ARM620", ARCH_ARMv3 },
  { "ARM7", ARCH_ARMv3 },
  { "ARM7D", ARCH_ARMv3 },
  { "ARM7DI", ARCH_ARMv3 },
  { "ARM70", ARCH_ARMv3 },
  { "ARM700", ARCH_ARMv3 },
  { "ARM700I", ARCH_ARMv3 },
  { "ARM710", ARCH_ARMv3 },
  { "ARM720", ARCH_ARMv3 },
  { "ARM710C", ARCH_ARMv3 },
  { "ARM7100", ARCH_ARMv3 },
  { "ARM7500", ARCH_ARMv3 },

  /* V3M */
  { "ARM7M", ARCH_ARMv3M },
  { "ARM7DM", ARCH_ARMv3M },
  { "ARM7DMI", ARCH_ARMv3M },

  /* V4 */
  { "ARM8", ARCH_ARMv4 },
  { "ARM810", ARCH_ARMv4 },
  { "StrongARM", ARCH_ARMv4 },
  { "StrongARM110", ARCH_ARMv4 },
  { "StrongARM1100", ARCH_ARMv4 },
  { "StrongARM1110", ARCH_ARMv4 },
  { "FA526", ARCH_ARMv4 },
  { "FA626", ARCH_ARMv4 },

  /* V4T */
  { "ARM7TDMI", ARCH_ARMv4T },
  { "ARM7TDMI-S", ARCH_ARMv4T },
  { "ARM710T", ARCH_ARMv4T },
  { "ARM720T", ARCH_ARMv4T },
  { "ARM740T", ARCH_ARMv4T },
  { "ARM9", ARCH_ARMv4T },
  { "ARM9TDMI", ARCH_ARMv4T },
  { "ARM920", ARCH_ARMv4T },
  { "ARM920T", ARCH_ARMv4T },
  { "ARM922T", ARCH_ARMv4T },
  { "ARM940T", ARCH_ARMv4T },
  { "EP9312", ARCH_ARMv4T },

  /* V5T */
  { "ARM10TDMI", ARCH_ARMv5T },
  { "ARM1020T", ARCH_ARMv5T },

  /* V5TE */
  { "ARM9E", ARCH_ARMv5TE },
  { "ARM946E-S", ARCH_ARMv5TE },
  { "ARM966E-S", ARCH_ARMv5TE },
  { "ARM968E-S", ARCH_ARMv5TE },
  { "ARM10E", ARCH_ARMv5TE },
  { "ARM1020E", ARCH_ARMv5TE },
  { "ARM1022E", ARCH_ARMv5TE },
  { "XScale", ARCH_ARMv5TE },
  { "IWMMXT", ARCH_ARMv5TE },
  { "IWMMXT2", ARCH_ARMv5TE },
  { "FA606TE", ARCH_ARMv5TE },
  { "FA626TE", ARCH_ARMv5TE },
  { "FMP626", ARCH_ARMv5TE },
  { "FA726TE", ARCH_ARMv5TE },

  /* V5TEJ */
  { "ARM926EJ-S", ARCH_ARMv5TEJ },
  { "ARM1026EJ-S", ARCH_ARMv5TEJ },

  /* V6 */
  { "ARM1136J-S", ARCH_ARMv6J },
  { "ARM1136JF-S", ARCH_ARMv6J },
  { "ARM1176JZ-S", ARCH_ARMv6KZ },
  { "ARM1176JZF-S", ARCH_ARMv6KZ },
  { "ARM11MPCore", ARCH_ARMv6K },
  { "ARM1156T2-S", ARCH_ARMv6T2 },
  { "ARM1156T2F-S", ARCH_ARMv6T2 },
  { "Cortex-M0", ARCH_ARMv6M },
  { "Cortex-M1", ARCH_ARMv6M },

  /* V7 */
  { "Cortex-A5", ARCH_ARMv7A },
  { "Cortex-A8", ARCH_ARMv7A },
  { "Cortex-A9", ARCH_ARMv7A },
  { "Cortex-A15", ARCH_ARMv7A },
  { "Cortex-R4", ARCH_ARMv7R },
  { "Cortex-R4F", ARCH_ARMv7R },
  { "Cortex-M3", ARCH_ARMv7M },
  { "Cortex-M4", ARCH_ARMv7EM }
};

const Core2Arch_t *oCoreArchSelectionP = NULL;


/**
 * Given an ARM architecture, return a CPU implementing this.
 */
static const Core2Arch_t *
GetCPUForArchitecture (ARM_eArchitectures arch)
{
  for (size_t j = 0; j != sizeof (oCore2Arch)/sizeof (oCore2Arch[0]); ++j)
    {
      if (oCore2Arch[j].arch == arch)
	return &oCore2Arch[j];
    }
  return NULL;
}


/**
 * Sets the CPU/architecture to target for.
 * \param cpu CPU/architecture to target for.  When "list" it will output the
 * list of supported CPU and architectures.
 * \return true when failure (or after "list"), false otherwise.
 */
bool
Target_SetCPU (const char *cpu)
{
  if (!strcmp (cpu, "list"))
    {
      fprintf (stderr, "Supported CPUs : ");
      for (size_t i = 0; i != sizeof (oCore2Arch)/sizeof (oCore2Arch[0]); ++i)
	fprintf (stderr, "%s ", oCore2Arch[i].core);
      fprintf (stderr, "\nSupported architectures : ");
      for (size_t i = 0; i != sizeof (oARMArch)/sizeof (oARMArch[0]); ++i)
	fprintf (stderr, "%s ", oARMArch[i]);
      fprintf (stderr, "\n");
      return true;
    }
  /* Try to identify as CPU.  */
  for (size_t i = 0; i != sizeof (oCore2Arch)/sizeof (oCore2Arch[0]); ++i)
    {
      if (!strcasecmp (oCore2Arch[i].core, cpu))
	{
	  oCoreArchSelectionP = &oCore2Arch[i];
	  return false;
	}
    }
  /* Try to identify as architecture.  */
  for (size_t i = 0; i != sizeof (oARMArch)/sizeof (oARMArch[0]); ++i)
    {
      if (!strcasecmp (oARMArch[i], cpu))
	{
	  oCoreArchSelectionP = GetCPUForArchitecture ((ARM_eArchitectures)i);
	  if (oCoreArchSelectionP != NULL)
	    return false;
	  fprintf (stderr, "Unable to find CPU core for architecture %s\n", oARMArch[i]);
	  return true;
	}
    }
  fprintf (stderr, "Unable to find CPU or architecture matching %s\n", cpu);
  return true;
}


bool
Target_NeedAtLeastArch (ARM_eArchitectures arch)
{
  if (oCoreArchSelectionP->arch < arch)
    {
      error (ErrorWarning, "Instruction is not supported on selected CPU %s (needs e.g. %s)",
	     oCoreArchSelectionP->core, GetCPUForArchitecture (arch)->core);
      return true;
    }
  return false;
}


const char *
Target_GetCPU (void)
{
  return oCoreArchSelectionP->core;
}


const char *
Target_GetArchAsString (void)
{
  return oARMArch[oCoreArchSelectionP->arch];
}


ARM_eArchitectures
Target_GetArch (void)
{
  return oCoreArchSelectionP->arch;
}
