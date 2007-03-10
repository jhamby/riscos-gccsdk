/*
 * POSIX Standard 3.2.1: Wait for Process Termination <sys/wait.h>.
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#ifndef	__SYS_WAIT_H
#define	__SYS_WAIT_H 1

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#define __need_rusage
#include <sys/resource.h>

__BEGIN_DECLS

/* Bits in the third arguent to waitpid.  */
#define WNOHANG 1 /* Don't block waiting.  */
#define WUNTRACED 2 /* Report status of stopped children.  */

/* If WIFEXITED(STATUS), the low-order 8 bits of the status.  */
#define	__WEXITSTATUS(status)	(((status) & 0xff00) >> 8)

/* If WIFSIGNALED(STATUS), the terminating signal.  */
#define	__WTERMSIG(status)	((status) & 0x7f)

/* If WIFSTOPPED(STATUS), the signal that stopped the child.  */
#define	__WSTOPSIG(status)	__WEXITSTATUS(status)

/* Nonzero if STATUS indicates normal termination.  */
#define	__WIFEXITED(status)	(__WTERMSIG(status) == 0)

/* Nonzero if STATUS indicates termination by a signal.  */
#define	__WIFSIGNALED(status)	(!__WIFSTOPPED(status) && !__WIFEXITED(status))

/* Nonzero if STATUS indicates the child is stopped.  */
#define	__WIFSTOPPED(status)	(((status) & 0xff) == 0x7f)

/* Nonzero if STATUS indicates the child dumped core.  */
#define	__WCOREDUMP(status)	((status) & __WCOREFLAG)

/* Macros for constructing status values.  */
#define	__W_EXITCODE(ret, sig)	((ret) << 8 | (sig))
#define	__W_STOPCODE(sig)	((sig) << 8 | 0x7f)
#define	__WCOREFLAG		0x80


#define	WEXITSTATUS(status)	__WEXITSTATUS(status)
#define	WTERMSIG(status)	__WTERMSIG(status)
#define	WSTOPSIG(status)	__WSTOPSIG(status)
#define	WIFEXITED(status)	__WIFEXITED(status)
#define	WIFSIGNALED(status)	__WIFSIGNALED(status)
#define	WIFSTOPPED(status)	__WIFSTOPPED(status)

#define	WCOREFLAG		__WCOREFLAG
#define	WCOREDUMP(status)	__WCOREDUMP(status)
#define	W_EXITCODE(ret, sig)	__W_EXITCODE(ret, sig)
#define	W_STOPCODE(sig)		__W_STOPCODE(sig)


/* Wait for a child to die.  */
extern __pid_t wait (int *);

/* Special values for the pid argument to `waitpid' and `wait4'.  */
/* Any process.  */
#define	WAIT_ANY (-1)
/* Any process in my process group.  */
#define	WAIT_MYPGRP 0

/* Wait for a child matching PID to die.  */
extern __pid_t waitpid (__pid_t, int *, int) __THROW;

#ifndef __SYS_RESOURCE_H
struct rusage;
#endif

/* Wait for a child to exit.  */
extern __pid_t wait3 (int *, int, struct rusage *) __THROW;

/* Wait for a child matching pid_t to die and return its usage statistics. */
extern __pid_t wait4 (__pid_t, int *, int, struct rusage *) __THROW;

__END_DECLS

#endif /* sys/wait.h  */
