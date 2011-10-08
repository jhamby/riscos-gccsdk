/* 
 * AS an assembler for ARM
 * Copyright (c) 1998 Nick Burrett
 * Copyright (c) 2001-2011 GCCSDK Developers
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

#ifndef __riscos__
#  include <error.h>
#  include <string.h>
/* UNIX specific information.  */
static inline const char *
CanonicalisePath (const char *path)
{
  const char *rsltP;
  if ((rsltP = strdup (path)) == NULL)
    errorOutOfMem ();
  return rsltP;
}
#else

#include <kernel.h>

int switonum (const char *swi);

_kernel_oserror *ThrowbackStart (void);
_kernel_oserror *ThrowbackSendStart (const char *filename);
_kernel_oserror *ThrowbackSendError (int level, int lineno, const char *error);
_kernel_oserror *ThrowbackEnd (void);

#define Throwback_ReasonProcessing	0
#define Throwback_ReasonErrorDetails	1
#define Throwback_ReasonInfoDetails	2

#define ThrowbackInfo			-1
#define ThrowbackWarning		0
#define ThrowbackError			1
#define ThrowbackSeriousError		2

const char *CanonicalisePath (const char *path);

#endif /* !__riscos__ */

#ifndef HAVE_STRNDUP
char *strndup (const char *str, size_t len);
#endif

#ifndef HAVE_STRDUP
char *strdup (const char *str);
#endif

#ifndef HAVE_STRNCASECMP
int strncasecmp(const char *str1, const char *str2, size_t n);
#endif

#endif
