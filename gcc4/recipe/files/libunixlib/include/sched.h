/*
 * Written by Alex Waugh
 * Copyright (c) 2002-2006 UnixLib Developers
 */

#ifndef __SCHED_H
#define __SCHED_H

#ifndef __UNIXLIB_FEATURES
#include <features.h>
#endif

__BEGIN_DECLS

#include <bits/sched.h>

extern int sched_yield (void);

__END_DECLS

#endif /* __SCHED_H */
