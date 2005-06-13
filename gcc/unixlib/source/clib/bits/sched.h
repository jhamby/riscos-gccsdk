/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/sched.h,v $
 * $Date: 2005/03/31 12:59:19 $
 * $Revision: 1.2 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifndef __SCHED_BITS_H
#define __SCHED_BITS_H

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

__END_DECLS

#endif /* __SCHED_H */

