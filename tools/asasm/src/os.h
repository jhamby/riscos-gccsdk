/* 
 * AS an assembler for ARM
 * Copyright (c) 1998 Nick Burrett
 * Copyright (c) 2001-2013 GCCSDK Developers
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
 * os.h
 * Operating system specific details.
 */

#ifndef os_header_included
#define os_header_included

#include "config.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

typedef struct
{
  const char *canonName; /* Canonical filename.  */
  off_t size; /* File size.  */
  uint8_t attribs; /* RISC OS file attributes.  */
  uint32_t execAddress; /* RISC OS exec address.  */
  uint32_t loadAddress; /* RISC OS load address.  */
} ASFile;

bool ASFile_Create (const char *fileName, ASFile *asFileP);
void ASFile_Free (ASFile *asFileP);

const char *OS_GetCWD (void);

#ifdef __riscos__

#include <kernel.h>

int OS_SWINameToNum (const char *swi);

const _kernel_oserror *OS_ThrowbackStart (void);
const _kernel_oserror *OS_ThrowbackSendStart (const char *fileName);
const _kernel_oserror *OS_ThrowbackSendError (int level, unsigned lineNum, const char *error);
const _kernel_oserror *OS_ThrowbackEnd (void);

#define Throwback_ReasonProcessing	0
#define Throwback_ReasonErrorDetails	1
#define Throwback_ReasonInfoDetails	2

#define ThrowbackInfo			-1
#define ThrowbackWarning		0
#define ThrowbackError			1
#define ThrowbackSeriousError		2

#endif /* !__riscos__ */

#endif
