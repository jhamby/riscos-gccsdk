/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/sys/resource.h,v $
 * $Date: 2000/07/15 14:52:16 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifndef	__SYS_RESOURCE_H
#define	__SYS_RESOURCE_H 1

/* Needed for the definition of struct timeval.  */
#ifndef __SYS_TIME_H
#include <sys/time.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

enum __rlimit_resource {
RLIMIT_CPU,
RLIMIT_FSIZE,
RLIMIT_CORE,
RLIMIT_DATA,
RLIMIT_STACK,
RLIMIT_RSS,
RLIMIT_MEMLOCK,
RLIMIT_NPROC,
RLIMIT_NOFILE,
RLIMIT_OFILE = RLIMIT_NOFILE,
RLIMIT_NLIMITS
};


struct rlimit
  {
    /* The current (soft) limit.  */
    int rlim_cur;
    /* The hard limit.  */
    int rlim_max;
  };

/* Value used to indicate that there is no limit.  */
#define RLIM_INFINITY 0x7fffffff

/* Put the soft and hard limits for RESOURCE in *RLIMITS.
   Returns 0 if successful, -1 if not (and sets errno).  */
extern int getrlimit (enum __rlimit_resource, struct rlimit *);

/* Set the soft and hard limits for RESOURCE to *RLIMITS.
   Only the super-user can increase hard limits.
   Return 0 if successful, -1 if not (and sets errno).  */

extern int setrlimit (enum __rlimit_resource, const struct rlimit *);

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

/* Return resource usage information on a process.  */
extern int getrusage (enum __rusage_who, struct rusage *);

/* Function depends on first parameter.
   1 = Return the limit on the size of a file, in units of 512 bytes.
   2 = Set the limit on the size of a file to new limit.  Only the
       super-user can increase the limit.
   3 = Return the maximum possible address of the data segment.
   4 = Return the maximum number of files that the calling process can open.  */
extern long int ulimit (int, int);


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

/* Return the highest priority of any process.  */
extern int getpriority (enum __priority_which, int);

/* Set the priority of all processes.  */
extern int setpriority (enum __priority_which, int, int);

/* Alter the priority of the current process by 'increment'.  */
extern int nice (int increment);

#ifdef __cplusplus
	}
#endif

#endif /* resource.h  */
