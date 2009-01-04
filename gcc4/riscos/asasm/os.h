/* 
 * AS an assembler for ARM
 * Copyright (c) 1998 Nick Burrett
 * Copyright (c) 2001-2006 GCCSDK Developers
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
/* UNIX specific information.  */
#define OSCanonicalisePath(a,b,c,d,e) strdup(a)
#else

#include <kernel.h>

extern int switonum (const char *swi);

extern int OSCanonicalisePath (const char *path,
			       char *buffer, int bufferSize,
			       char *systemVar, char *defaultPath);

extern _kernel_oserror *ThrowbackStart (void);
extern _kernel_oserror *ThrowbackSendStart (const char *filename);
extern _kernel_oserror *ThrowbackSendError (int level, long int lineno,
					    const char *error);
extern _kernel_oserror *ThrowbackEnd (void);

#define Throwback_ReasonProcessing	0
#define Throwback_ReasonErrorDetails	1
#define Throwback_ReasonInfoDetails	2

#define ThrowbackInfo			-1
#define ThrowbackWarning		0
#define ThrowbackError			1
#define ThrowbackSeriousError		2

extern char *CanonicalisePath (const char *path);

#endif /* !__riscos__ */

#ifndef HAVE_STRNDUP
extern char *strndup (const char *str, size_t len);
#endif

#ifndef HAVE_STRDUP
extern char *strdup (const char *str);
#endif

#ifndef HAVE_STRNCASECMP
extern int strncasecmp(const char *str1, const char *str2, size_t n);
#endif

#endif
