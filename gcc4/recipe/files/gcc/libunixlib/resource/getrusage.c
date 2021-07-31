/* getrusage ()
 * Copyright (c) 2000-2011 UnixLib Developers
 */

#include <sys/resource.h>
#include <errno.h>
#include <time.h>

#include <internal/unix.h>

/* Return resource usage information on process indicated by WHO
   and put it in *USAGE.  Returns 0 for success, -1 for failure.  */
int
getrusage (__rusage_who_t who, struct rusage *usage)
{
  int ticks;

  if (who != RUSAGE_SELF && who != RUSAGE_CHILDREN)
    return __set_errno (EINVAL);

  if (usage == 0)
    return -1;

  ticks = (int) clock ();
  /* Since clock () returns all time the current program has been
     running for, we shall not bother with the ru_stime structure.  */
  /* Time spend executing user instructions.  */
  usage->ru_utime.tv_sec = ticks / 100;
  /* Separate the centiseconds from the seconds and then convert
     to microseconds.  */
  usage->ru_utime.tv_usec = (ticks - ((int) (ticks / 100) * 100)) * 10000;
  /* Time spend in operating system code on behalf of 'who'.  */
  usage->ru_stime.tv_sec = 0;
  usage->ru_stime.tv_usec = 0;
  /* The maximum residet set size used, in kilobytes i.e. the
     maximum number of kilobytes that 'who' used in real memory
     simultaneously.  */
  usage->ru_maxrss = 0;
  /* Amount of memory used by text that was shared with other
     processes. Expressed in kilobytes * ticks of execution.  */
  usage->ru_ixrss = 0;
  /* The amount of unshared memory used in data.
     Expressed as above.  */
  usage->ru_idrss = 0;
  /* The amount of unshared memory used in stack space.
     Expressed as above.  */
  usage->ru_isrss = 0;
  /* The number of page faults which were serviced without
     requiring any I/O.  */
  usage->ru_minflt = 0;
  /* The number of page faults which were serviced by doing I/O.  */
  usage->ru_majflt = 0;
  /* The number of times 'who' was swapped entirely out of main
     memory.  */
  usage->ru_nswap = 0;
  /* The number of times the file system had to read from the disk
     on behalf of 'who'.  */
  usage->ru_inblock = __u->usage.ru_inblock;
  /* The number of times the file system had to write to the disk
     on behalf of 'who'.  */
  usage->ru_oublock = __u->usage.ru_oublock;
  /* Number of IPC messages sent.  */
  usage->ru_msgsnd = 0;
  /* Number of IPC messages received.  */
  usage->ru_msgrcv = 0;
  /* Number of signals received.  */
  usage->ru_nsignals = __u->usage.ru_nsignals;
  /* Number of times 'who' voluntarily invoked a context switch
     (usually to wait for some service).  */
  usage->ru_nvcsw = 0;
  /* The number of times an involuntary context switch took place
     (because the time slice expired, or another process of higher
     priority became runnable).  */
  usage->ru_nivcsw = 0;

  return 0;
}
