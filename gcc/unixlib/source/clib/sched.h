/****************************************************************************
 *
 * $Source: $
 * $Date: $
 * $Revision: $
 * $State: $
 * $Author: $
 *
 ***************************************************************************/

#ifndef __SCHED_H
#define __SCHED_H

#define SCHED_FIFO 0
#define SCHED_RR 1
#define SCHED_SPORADIC 2
#define SCHED_OTHER 3

struct sched_param
{
  int sched_priority;
};

#endif /* __SCHED_H */

