/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/sched.h,v $
 * $Date: 2002/12/15 13:16:55 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifndef __SCHED_H
#define __SCHED_H

#ifndef __UNIXLIB_FEATURES
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

extern int sched_yield (void);

__END_DECLS

#endif /* __SCHED_H */

