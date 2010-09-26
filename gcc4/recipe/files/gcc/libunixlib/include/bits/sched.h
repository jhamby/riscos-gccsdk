/*
 * Written by Alex Waugh
 * Copyright (c) 2002-2010 UnixLib Developers
 */

#ifndef __SCHED_BITS_H
#define __SCHED_BITS_H

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

__BEGIN_DECLS

#define SCHED_FIFO 0
#define SCHED_RR 1
#define SCHED_SPORADIC 2
#define SCHED_OTHER 3

struct sched_param
{
  int sched_priority;
};

__END_DECLS

#endif /* __SCHED_H */

