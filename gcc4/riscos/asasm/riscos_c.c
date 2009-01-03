/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2001-2008 GCCSDK Developersrs
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
 * riscos.c
 */

/* These are functions that are specific to the RISC OS native version.  */

#include "config.h"

#ifdef __riscos__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#endif
#ifdef HAVE_INTTYPES_H
#  include <inttypes.h>
#endif
#include <kernel.h>
#include <swis.h>

#include "error.h"
#include "main.h"
#include "os.h"

#ifdef __TARGET_UNIXLIB__
#  include <unixlib/local.h>
#endif

/* The DDEUtils module throwback error category codes.  */
#define THROWBACK_INFORMATION         -1
#define THROWBACK_WARNING              0
#define THROWBACK_ERROR                1
#define THROWBACK_SERIOUS_ERROR        2

/* The DDEUtils module throwback reason codes.  */
#define THROWBACK_REASON_PROCESSING    0
#define THROWBACK_REASON_ERROR_DETAILS 1
#define THROWBACK_REASON_INFO_DETAILS  2

static char filename[1024];
static const char *ErrorFile;

extern _kernel_oserror *__os_swi (int swinum, int *regs);

char *CanonicalisePath (const char *path1)
{
  int size;
  char *buffer;
  char path[1024];
  __riscosify (path1, 0, 0, path, sizeof (path), NULL);
  if (dde && *path == '@')
    {				/* Replace @ with <Prefix$Dir> if dde flags */
      strcpy (filename, "<Prefix$Dir>");
      strcat (filename, path + 1);
    }
  else
    {
      strcpy (filename, path);
    }
  size = 1 - OSCanonicalisePath (filename, 0, 0, 0, 0);
  if ((buffer = malloc (size)) == NULL)
    {
      errorOutOfMem("CanonicalisePath");
      return NULL;
    }
  size = OSCanonicalisePath (filename, buffer, size, 0, 0);
  if (size == 1)
    return buffer;

  error (ErrorAbort, TRUE, "Internal error in CanonicalisePath");
  return NULL;			/* keep the compiler happy */
}

int switonum (const char *swiname)
{
  int regs[10];
  regs[1] = (int) swiname;
  __os_swi (OS_SWINumberFromString, regs);
  return regs[0];
}

_kernel_oserror *ThrowbackStart (void)
{
  int regs[10];
  _kernel_oserror *err = __os_swi (DDEUtils_ThrowbackStart, regs);
  return err;
}

_kernel_oserror *ThrowbackSendStart (const char *filename)
{
  int regs[10];
  ErrorFile = filename;
  regs[0] = THROWBACK_REASON_PROCESSING;
  regs[2] = (int) filename;
  return __os_swi (DDEUtils_ThrowbackSend, regs);
}

_kernel_oserror *ThrowbackSendError (int level,
				     long int lineno, const char *error)
{
  int regs[10];
  regs[0] = THROWBACK_REASON_ERROR_DETAILS;
  regs[1] = 0;
  regs[2] = (int) ErrorFile;
  regs[3] = lineno;
  regs[4] = level;
  regs[5] = (int) error;
  return __os_swi (DDEUtils_ThrowbackSend, regs);
}

_kernel_oserror *ThrowbackEnd (void)
{
  int regs[10];
  regs[0] = 0;
  regs[1] = 0;
  regs[2] = 0;
  regs[3] = 0;
  regs[4] = 0;
  return __os_swi (DDEUtils_ThrowbackEnd, regs);
}

int OSCanonicalisePath (const char *path, char *buffer, int buffersize,
			char *systemvar, char *defaultpath)
{
  int regs[10];
  regs[0] = 37;
  regs[1] = (int) path;
  regs[2] = (int) buffer;
  regs[3] = (int) systemvar;
  regs[4] = (int) defaultpath;
  regs[5] = buffersize;
  __os_swi (OS_FSControl, regs);
  return regs[5];
}

#endif /* __riscos__ */
