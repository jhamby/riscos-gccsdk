/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/signal/c/signame,v $
 * $Date: 2000/06/03 12:22:55 $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: signame,v 1.7 2000/06/03 12:22:55 admin Exp $";
#endif

/* signame.c: Define signals for sys_siglist.  */

#include <signal.h>
#include <unixlib/sigstate.h>

int sys_nsig = NSIG;

char *sys_siglist[NSIG] =
{
  "Signal 0"		       /* 0. NULL SIGNAL */,
  "Hangup"		       /* 1. SIGHUP */,
  "Interrupt"		       /* 2. SIGINT */,
  "Quit"		       /* 3. SIGQUIT */,
  "Illegal Instruction"	       /* 4. SIGILL */,
  "Trace/breakpoint trap"      /* 5. SIGTRAP */
  "Aborted"		       /* 6. SIGABRT/SIGIOT */,
  "EMT trap"		       /* 7. SIGEMT */,
  "Floating point exception"   /* 8. SIGFPE */,
  "Killed"		       /* 9. SIGKILL */,
  "Bus error"		       /* 10. SIGBUS */,
  "Segmentation fault"	       /* 11. SIGSEGV */,
  "Bad system call"	       /* 12. SIGSYS */,
  "Broken pipe"		       /* 13. SIGPIPE */,
  "Alarm clock"		       /* 14. SIGALRM */,
  "Terminated"		       /* 15. SIGTERM */,
  "Urgent I/O condition"       /* 16. SIGURG */,
  "Stopped (signal)"	       /* 17. SIGSTOP */,
  "Stopped"		       /* 18. SIGTSTP */,
  "Continued"		       /* 19. SIGCONT */,
  "Child exited"	       /* 20. SIGCHLD */,
  "Stopped (tty input)"        /* 21. SIGTTIN */,
  "Stopped (tty output)"       /* 22. SIGTTOU */,
  "I/O possible"	       /* 23. SIGIO */,
  "CPU time limit exceeded"    /* 24. SIGXCPU */,
  "File size limit exceeded"   /* 25. SIGXFSZ */,
  "Virtual timer expired"      /* 26. SIGVTALRM */,
  "Profiling timer expired"    /* 27. SIGPROF */,
  "Window changed"	       /* 28. SIGWINCH */,
  "Information requested"      /* 29. SIGINFO */,
  "User defined signal 1"      /* 30. SIGUSR1 */,
  "User defined signal 2"      /* 31. SIGUSR2 */,
  "Resource lost"	       /* 32. SIGLOST */,
  "RISC OS error"	       /* 33. SIGERR */
};
