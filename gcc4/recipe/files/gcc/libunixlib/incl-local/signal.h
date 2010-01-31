/* Internal UnixLib signal.h
 * Copyright (c) 2000-2006 UnixLib Developers
 */

#ifndef __SIGNAL_H
#include_next <signal.h>
#endif

#if !defined(__INTERNAL_SIGNAL_H) && defined(__SIGNAL_H)
#define __INTERNAL_SIGNAL_H

__BEGIN_DECLS

/* Inline versions for UnixLib library only. These are subject to change.  */

/* Clear all signals from set.  */
#define sigemptyset(set) ((*(set) = (__sigset_t) 0), 0)

/* Set all signals in set.  */
#define sigfillset(set) ((*(set) = ~(__sigset_t) 0), 0)

/* Constructs and returns a "signal mask" for signo.  */
#define sigmask(signo) (((__sigset_t) 1) << ((signo) - 1))

/* Add signo to set.  */
#define sigaddset(set,signo) ((*(set) |= sigmask (signo)), 0)

/* Remove signo from set.  */
#define sigdelset(set,signo) ((*(set) &= ~sigmask (signo)), 0)

/* Remove 1 if signo is in set, 0 if not.  */
#define sigismember(set,signo) ((*(set) & sigmask (signo)) ? 1 : 0)

__END_DECLS

#endif
