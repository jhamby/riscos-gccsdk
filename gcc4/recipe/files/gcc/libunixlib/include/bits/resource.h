/*
 * Copyright (c) 2000-2011 UnixLib Developers
 */

#ifndef	__SYS_RESOURCE_H
#  error "Never use <bits/resource.h> directly but include <sys/resource.h> instead."
#endif

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#define __need_timeval
#include <sys/time.h>

enum __rlimit_resource
  {
    RLIMIT_CPU,
#define RLIMIT_CPU RLIMIT_CPU
    RLIMIT_FSIZE,
#define RLIMIT_FSIZE RLIMIT_FSIZE
    RLIMIT_CORE,
#define RLIMIT_CORE RLIMIT_CORE
    RLIMIT_DATA,
#define RLIMIT_DATA RLIMIT_DATA
    RLIMIT_STACK,
#define RLIMIT_STACK RLIMIT_STACK
    RLIMIT_RSS,
#define RLIMIT_RSS RLIMIT_RSS
    RLIMIT_MEMLOCK,
#define RLIMIT_MEMLOCK RLIMIT_MEMLOCK
    RLIMIT_NPROC,
#define RLIMIT_NPROC RLIMIT_NPROC
    RLIMIT_NOFILE,
    RLIMIT_OFILE = RLIMIT_NOFILE,
#define RLIMIT_NOFILE RLIMIT_NOFILE
#define RLIMIT_OFILE RLIMIT_OFILE
    RLIMIT_NLIMITS
#define RLIMIT_NLIMITS RLIMIT_NLIMITS
  };

/* Value used to indicate that there is no limit.  */
#define RLIM_INFINITY 0x7fffffff

typedef __rlim_t rlim_t;

struct rlimit
  {
    /* The current (soft) limit.  */
    int rlim_cur;
    /* The hard limit.  */
    int rlim_max;
  };

/* Whose usage statistics do you want?  */
enum __rusage_who
/* The macro definitions are necessary because some programs want
   to test for operating system features with #ifdef RUSAGE_SELF.
   In ANSI C the reflexive definition is a no-op.  */
  {
    /* The calling process.  */
    RUSAGE_SELF = 0,
#define	RUSAGE_SELF	RUSAGE_SELF
    /* All of its terminated child processes.  */
    RUSAGE_CHILDREN = -1
#define	RUSAGE_CHILDREN	RUSAGE_CHILDREN
  };

/* Structure which says how much of each resource has been used.  */
struct rusage
  {
    /* Total amount of user time used.  */
    struct timeval ru_utime;
    /* Total amount of system time used.  */
    struct timeval ru_stime;
    /* Maximum resident set size (in kilobytes).  */
    int ru_maxrss;
    /* Amount of sharing of text segment memory
       with other processes (kilobyte-seconds).  */
    int ru_ixrss;
    /* Amount of data segment memory used (kilobyte-seconds).  */
    int ru_idrss;
    /* Amount of stack memory used (kilobyte-seconds).  */
    int ru_isrss;
    /* Number of soft page faults (i.e. those serviced by reclaiming
       a page from the list of pages awaiting reallocation.  */
    int ru_minflt;
    /* Number of hard page faults (i.e. those that required I/O).  */
    int ru_majflt;
    /* Number of times a process was swapped out of physical memory.  */
    int ru_nswap;
    /* Number of input operations via the file system.  Note: This
       and `ru_oublock' do not include operations with the cache.  */
    int ru_inblock;
    /* Number of output operations via the file system.  */
    int ru_oublock;
    /* Number of IPC messages sent.  */
    int ru_msgsnd;
    /* Number of IPC messages received.  */
    int ru_msgrcv;
    /* Number of signals delivered.  */
    int ru_nsignals;
    /* Number of voluntary context switches, i.e. because the process
       gave up the process before it had to (usually to wait for some
       resource to be available).  */
    int ru_nvcsw;
    /* Number of involuntary context switches, i.e. a higher priority process
       became runnable or the current process used up its time slice.  */
    int ru_nivcsw;
  };

/* Priority limits.  */
#define	PRIO_MIN	-20	/* Minimum priority a process can have.  */
#define	PRIO_MAX	20	/* Maximum priority a process can have.  */

/* The type of the 'which' argument to `getpriority' and `setpriority',
   indicating what flavor of entity the WHO argument specifies.  */
enum __priority_which
  {
    PRIO_PROCESS = 0,		/* WHO is a process ID.  */
    PRIO_PGRP = 1,		/* WHO is a process group ID.  */
    PRIO_USER = 2		/* WHO is a user ID.  */
  };

