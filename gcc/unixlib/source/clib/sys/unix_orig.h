/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/sys/unix_orig.h,v $
 * $Date: 2000/07/15 14:52:16 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 * UNIX is a registered trademark of AT&T Bell Laboratories
 *
 ***************************************************************************/

#ifndef __SYS_UNIX_H
#define __SYS_UNIX_H 1

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif
#ifndef __UNIXLIB_FD_H
#include <unixlib/fd.h>
#endif
#ifndef __UNIXLIB_SIGSTATE_H
#include <unixlib/sigstate.h>
#endif

#ifndef __SYS_TTY_H
#include <sys/tty.h>
#endif
#ifndef __SETJMP_H
#include <setjmp.h>
#endif
#ifndef __SYS_TIME_H
#include <sys/time.h>
#endif
#ifndef __SYS_RESOURCE_H
#include <sys/resource.h>
#endif
#ifndef __LIMITS_H
#include <limits.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __UNIXLIB_INTERNALS

#ifndef __GNUC__
#undef attribute
#define attribute(x) /* ignore */
#endif

/* the 2 functions below are automatically called in normal circumstances */

/* Initialise the UnixLib world */
extern void __unixinit (void);
/* Shutdown the UnixLib world */
extern void __unixexit (void);


struct __process
{
  /* This process has a parent process.  */
  unsigned int has_parent : 1;
  /* This process terminated through a signal handler.  */
  unsigned int signal_exit : 1;
  /* This process terminated with a core dump.  */
  unsigned int core_dump : 1;
  /* This process just stopped.  */
  unsigned int stopped : 1;
  /* This process is controlling the tty. */
  unsigned int tty_control : 1;
  /* The type of tty this process is controlling.  */
  unsigned int tty_type : 8;
  /* The number of the signal we terminated with.  */
  unsigned int signal : 8;
  /* The return code of this process.  */
  unsigned int return_code : 8;
};

struct __child_process
{
  __uid_t uid;
  __gid_t gid;
  __pid_t pid;
  int	ppri, gpri, upri;
  struct __process status;
  struct rusage usage;
  /* Process context.  */
  jmp_buf vreg;
};

/* UnixLib 3.7b == 0xfedcfa5f.  */
/* UnixLib 3.7c.beta == 0xfedcfa60.  */
/* UnixLib 3.7c.19970803 == 0xfedcfa61.  */
/* UnixLib 3.7c.19980504 == 0xfedcfa62  */
#define _PROCMAGIC 0xfedcfa62

struct proc
{
  /* Magic word */
  unsigned int __magic;
  /* main(argc,argv) */
  int argc;
  char **envp, *envb;
  char **argv, *argb;	/* Keep these 4 together if possible.  */
  /* Copy to use to initialise child argv which user program can't
     overwrite.  */
  size_t *clean_argv;	/* Stored as offsets into clean_argb.  */
  char *clean_argb;	/* Copy of *argb.  */
  /* File descriptors for this process.  */
  struct __unixlib_fd fd[MAXFD];
  struct tty *tty;
  /* real/effective user ID */
  __uid_t uid, euid;
  /* real/effective group ID */
  __gid_t gid, egid;
  /* process group ID */
  __pid_t pgrp;
  /* process id */
  __pid_t pid;
  /* parent process id */
  __pid_t ppid;
  /* priority: process, process group, user process */
  int ppri, gpri, upri;
  /* File creation mask.  */
  __mode_t umask;
  /* status for this particular process */
  struct __process status;
  /* interval timers */
  struct itimerval itimers[__MAX_ITIMERS];
  /* resource limits */
  struct rlimit limit[RLIMIT_NLIMITS];
  /* current resource usage */
  struct rusage usage;
  /* parent's struct proc */
  struct proc *pproc;
  /* Details recorded about stopped or terminated children.  */
  struct __child_process child[CHILD_MAX];
  /* The signal states for this process. */
  struct unixlib_sigstate sigstate;
  /* Set to 1 if this process is currently sleeping, orphaned or stopped.
     These could change at any time.  */
  volatile int sleeping, orphaned, stopped;
  /* Process context.  */
  jmp_buf vreg;
};

extern struct proc *__u;	/* current process */

struct pipe
{
  struct __unixlib_fd *p[2];
  char		*file;
  struct pipe	*next;
};

extern struct pipe *__pipe;	/* list of currently active pipes */

/* Return unsigned int from STRING.  END, if non-null, points to character
   after number.  */
extern unsigned int __decstrtoui (const char *__string, char **__end);

/* If this variable is non-null then we allocated the current environment.  */
extern char **__last_environ;

/* UnixLib's prefix for global RISC OS environment variables. I considered
   using Unix$ rather than UnixEnv$, but that means the environment variable
   PATH would get confused with Unix$Path which is used in makefiles.  */
#define __UNIX_ENV_PREFIX "UnixEnv$"

/* Add NAME=VALUE to the environment. If NAME is already in the environment,
   only add when REPLACE is non-zero. If NAME is added to the environment
   and ADD_TO_OS is non-zero, then also add it to the RISC OS global
   environment.  */
extern char *__addenv (const char *__name, const char *__value,
		       int __replace, int __add_to_os);

/* Lookup NAME in the environment and return value if found.  */
extern char *__chkenv (const char *__name);

/* Get environment value from OS and copy into BUF and return a pointer to BUF.
   If no match found then return NULL.  If BUF is NULL, then allocate space
   as necessary, otherwise BUFLEN is length of supplied buffer.  */
extern char *__getenv_from_os (const char *__name, char *__buf,
			       size_t __buflen);

/* If CACHE is non-zero, then lookup NAME in the program's environment first.
   If CACHE is zero or NAME was not found in the program's environment, then
   look it up in the RISC OS global environment.  If found there, then add
   it to the program's environment. Return found value or NULL.  */
extern char *__getenv (const char *__name, int __cache);

/* Return the integer value of NAME, from the RISC OS global environment.  */
extern int __intenv (const char *__name);

/* Remove NAME from the OS environment. Returns -1 on failure or not found.  */
extern int __remenv_from_os (const char *__name);

/* Set runtime features according to system variables.  */
extern void __runtime_features (const char *__cli);

/* Find first freely available child.  */
extern int __find_free_child (void);

/* Print an error and exit the process.  */
extern void
__unixlib_fatal (const char *__message) __attribute__ ((__noreturn__));

#endif  /* __UNIXLIB_INTERNALS */

#ifdef __cplusplus
}
#endif

#endif
