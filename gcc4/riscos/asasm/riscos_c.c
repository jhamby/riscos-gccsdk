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

static const char *ErrorFile;

/**
 * Canonicalise filename.
 * \param path1 Filename.
 * \returns When non-NULL, pointer to malloced buffer holding the canonicalised
 * filename (caller needs to free).  NULL in case of an error.
 */
char *
CanonicalisePath (const char *path1)
{
  int size;
  char *buffer;
  char filename[1024];
#if __TARGET_UNIXLIB__
  char path[1024];
  __riscosify (path1, 0, 0, path, sizeof (path), NULL);
#else
  const char *path = path1;
#endif
  
  if (option_dde && *path == '@')
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

int
switonum (const char *swiname)
{
  _kernel_swi_regs regs;
  regs.r[1] = (int) swiname;
  _kernel_swi (OS_SWINumberFromString, &regs, &regs); /* TODO: handle error ? */
  return regs.r[0];
}

_kernel_oserror *
ThrowbackStart (void)
{
  _kernel_swi_regs regs;
  return _kernel_swi (DDEUtils_ThrowbackStart, &regs, &regs);
}

_kernel_oserror *
ThrowbackSendStart (const char *filename)
{
  _kernel_swi_regs regs;

  ErrorFile = filename;
  regs.r[0] = THROWBACK_REASON_PROCESSING;
  regs.r[2] = (int) filename;
  return _kernel_swi (DDEUtils_ThrowbackSend, &regs, &regs);
}

/**
 * \param level 0 for warning, 1 for error, 2 for serious error.
 * \param error nul terminated description of error
 */
_kernel_oserror *
ThrowbackSendError (int level, long int lineno, const char *error)
{
  _kernel_swi_regs regs;

  regs.r[0] = THROWBACK_REASON_ERROR_DETAILS;
  regs.r[1] = 0;
  regs.r[2] = (int) ErrorFile;
  regs.r[3] = lineno;
  regs.r[4] = level;
  regs.r[5] = (int) error;
  return _kernel_swi (DDEUtils_ThrowbackSend, &regs, &regs);
}

_kernel_oserror *
ThrowbackEnd (void)
{
  _kernel_swi_regs regs;

  return _kernel_swi (DDEUtils_ThrowbackEnd, &regs, &regs);
}

int
OSCanonicalisePath (const char *path, char *buffer, int buffersize,
		    char *systemvar, char *defaultpath)
{
  _kernel_swi_regs regs;

  regs.r[0] = 37;
  regs.r[1] = (int) path;
  regs.r[2] = (int) buffer;
  regs.r[3] = (int) systemvar;
  regs.r[4] = (int) defaultpath;
  regs.r[5] = buffersize;
  _kernel_swi (OS_FSControl, &regs, &regs); /* TODO: error handling */
  return regs.r[5];
}

#endif /* __riscos__ */
