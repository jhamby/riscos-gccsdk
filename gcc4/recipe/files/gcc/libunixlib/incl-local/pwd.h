/* Internal UnixLib pwd.h
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#ifndef __PWD_H
#include_next <pwd.h>
#endif

#if !defined(__INTERNAL_PWD_H) && defined(__PWD_H)
#define __INTERNAL_PWD_H

__BEGIN_DECLS

/* UnixLib pwd implementation function.  */
extern struct passwd *__pwdread (FILE *__stream, struct passwd *__ppwd,
				 char *__buf, size_t __buflen);

/* Default values for passwd struct. */
extern struct passwd *__pwddefault (void);

__END_DECLS

#endif
