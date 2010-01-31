/* Internal UnixLib sys/select.h
 * Copyright (c) 2008 UnixLib Developers
 */

#ifndef __SYS_SELECT_H
#include_next <sys/select.h>
#endif

#if !defined(__INTERNAL_SYS_SELECT_H) && defined(__SYS_SELECT_H)
#define	__INTERNAL_SYS_SELECT_H

__BEGIN_DECLS

/* SWI veneer. Do not use directly.  */
extern int _select (int __nfds,
		    fd_set *__restrict __readfds,
		    fd_set *__restrict __writefds,
		    fd_set *__restrict __exceptfds,
		    const struct timeval *__restrict __timeout);

__END_DECLS

#endif
