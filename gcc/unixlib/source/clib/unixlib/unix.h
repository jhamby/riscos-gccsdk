/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/unixlib/unix.h,v $
 * $Date: 2004/10/12 08:32:38 $
 * $Revision: 1.18 $
 * $State: Exp $
 * $Author: peter $
 *
 * UNIX is a registered trademark of AT&T Bell Laboratories
 *
 ***************************************************************************/

#ifndef __UNIXLIB_UNIX_H
#define __UNIXLIB_UNIX_H 1

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif
#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif
#ifndef __UNIXLIB_FD_H
#include <unixlib/fd.h>
#endif
#ifndef __UNIXLIB_SIGSTATE_H
#include <unixlib/sigstate.h>
#endif

#ifndef __UNIXLIB_TTY_H
#include <unixlib/tty.h>
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
#ifndef __SYS_TYPES_H
#include <sys/types.h>
#endif
#ifndef __LIMITS_H
#include <limits.h>
#endif
#ifndef __KERNEL_H
#include <kernel.h>
#endif

__BEGIN_DECLS

#ifdef __UNIXLIB_INTERNALS

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
  int ppri, gpri, upri;
  struct __process status;
  struct rusage usage;
  jmp_buf vreg; /* Process context.  */
};

/* UnixLib 3.7b == 0xfedcfa5f.  */
/* UnixLib 3.7c.beta == 0xfedcfa60.  */
/* UnixLib 3.7c.19970803 == 0xfedcfa61.  */
/* UnixLib 3.7c.19980504 == 0xfedcfa62.  */
/* UnixLib 4.0.20040113 == 0xfedcfa63.  */
#define _PROCMAGIC 0xfedcfa63

struct proc
{
  unsigned int __magic;  /* Magic word.  */
  int argc, envc;  /* Command line and environment argument count.  */
  char **envp;  /* Environment argument list.  */
  char **argv;  /* Command line arguments.  */
  struct __unixlib_fd fd[MAXFD];  /* File descriptors.  */
  struct tty *tty;
  __uid_t uid, euid;  /* Real and effective UID.  */
  __gid_t gid, egid;  /* Real and effective GID.  */
  __pid_t pgrp;  /* ID of process group that process belongs to.  */
  __pid_t pid;  /* Process ID.  */
  __pid_t ppid;  /* Parent process ID.  */
  int ppri, gpri, upri;  /* Process priority, process group priority
			    and user process priority.  */
  __mode_t umask;  /* File creation mode mask.  */
  struct __process status;  /* Process status.  */
  struct itimerval itimers[__MAX_ITIMERS];  /* Defined interval timers.  */
  struct rlimit limit[RLIMIT_NLIMITS];  /* Process resource limits.  */
  struct rusage usage;  /* Resource usage of the process.  */
  struct proc *pproc;  /* Pointer to the parent process's structure.  */
  /* Details recorded about stopped or terminated children.  */
  struct __child_process child[CHILD_MAX];
  /* The signal states for this process. */
  struct unixlib_sigstate sigstate;
  /* Set to 1 if this process is currently sleeping, orphaned or stopped.
     These could change at any time.  */
  volatile int sleeping, orphaned, stopped;
  jmp_buf vreg;  /* Process context.  */
  /* UGLY HACK: In execve() we store cli malloc pointer prior to calling
     child process.  The malloc store is then freed in __exret().  */
  void *cli;
  /* DDEUtils_Prefix value at the beginning of this process.  */
  const char *dde_prefix;
};

extern struct proc *__u;	/* current process */

struct pipe
{
  struct __unixlib_fd *p[2];
  char *file;
  struct pipe *next;
};

extern struct pipe *__pipe;	/* list of currently active pipes */


extern int __funcall_error (const char *, int, unsigned int);
#if __UNIXLIB_PARANOID > 0
#define __funcall(f,p) \
  ((((void *)(f) >= __image_ro_base) && (((unsigned int)(f) & ~3) == (unsigned int)(f)) \
   ? 0 : __funcall_error(__FILE__,__LINE__,(unsigned int)(f))), (f)p)
#else
#define __funcall(f,p) ((f)p)
#endif


/* Assembler exit.  */

/* __returncode is the final program's return code.
 */
extern void __exit (int __returncode) __attribute__ ((__noreturn__));
extern void __exit_no_code (void) __attribute__ ((__noreturn__));

/* __unixlib_break is initialised to __image_rw_lomem & __unixlib_stack to
   __image_rw_himem - STAKSIZ;
   __unixlib_stack is extended downwards in chunks by x$stack_overflow()
   and __unixlib_break is extended upwards by brk() and sbrk(). The sl
   register is kept equal to __unixlib_stack + 512. Should x$stack_overflow()
   attempt to extend __unixlib_stack below __unixlib_break then SIGEMT is
   raised.
   Should brk() or sbrk() be asked to extend __unixlib_break above
   __unixlib_stack then they return with ENOMEM.  */

extern char *__unixlib_cli;		/* command line from OS_GetEnv */
extern void *__image_ro_base;		/* BASE = Image$$RO$$Base */
extern void *__image_rw_lomem;		/* LOMEM = Image$$RW$$Limit */
extern void *__image_rw_himem;		/* HIMEM from OS_GetEnv */
extern void *__unixlib_real_himem;	/* Real HIMEM  - application space limit */
extern void *__unixlib_rwlimit;

extern void *__unixlib_break;		/* end of data area */
extern void *__unixlib_real_break;	/* real end of data area */
extern void *__unixlib_stack;		/* bottom of stack */
extern void *__unixlib_stack_limit;
extern int __codeshift;
extern int __dynamic_num;
extern void __dynamic_area_exit (void);

extern void __munmap_all (void);	/* Deallocate all mappings.  */

/* Zero if we are not executing within a TaskWindow.  Non-zero otherwise.  */
extern int __taskwindow;
/* Zero if we are running in command mode (not as a WIMP program).  Non-zero
   otherwise and equals current WIMP task handle.  */
extern int __taskhandle;
/* Zero if we are running in 26-bit, non-zero for 32-bit */
extern int __32bit;

extern unsigned int __time[2];	/* start time */

#define	__OS_ARTHUR	0xA0
#define	__OS_RISCOS_200	0xA1
#define	__OS_RISCOS_201	0xA2
#define	__OS_RISCOS_300	0xA3
#define	__OS_RISCOS_310	0xA4
#define	__OS_RISCOS_350	0xA5
#define	__OS_RISCOS_360	0xA6
#define	__OS_RISCOS_370	0xA7
#define	__OS_RISCOS_400	0xA8

/* FP flag reflecting Floating Point presence or not.  */
extern int __fpflag;

/* Stop the interval timers.  */
extern void __stop_itimers (void);

/* OS_ChangeEnvironment is used to set up exception handlers. These
 * handlers use OS_CallBack to raise signals in the foreground process.
 * RTFM (RISC OS PRM - 'The Program Environment') for more info. */

/* Reset handlers, etc. back to original state.  */
extern void __env_riscos (void);
extern void __env_unixlib (void);

/* vfork() & exec() */

extern void __vret (int);	/* return from child - calls __vexit() */

extern int *__vfork (void);	/* fork to child context */
extern int *__vexit (int);	/* restore to parent context */

extern void __exret (void);	/* return from __exec() - calls __vret() */

extern void (*__exptr) (char *); /* pointer to __exec() routine */
extern int __exlen;		/* length of __exec() routine */
extern int __exshift;		/* __exec() shift */
extern void __exec_cli (const char *) __attribute__ ((__noreturn__));

/* vfork weak link.  */

extern void (*___vret) (int);

/* Return unsigned int from STRING.  END, if non-null, points to character
   after number.  */
extern unsigned int __decstrtoui (const char *__string, char **__end);

/* If this variable is non-null then we allocated the current environment.  */
extern char **__last_environ;

/* UnixLib's prefix for global RISC OS environment variables.  */
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

/* Return the integer value of NAME, from the RISC OS global environment.  */
extern int __intenv (const char *__name);

/* Remove NAME from the OS environment. Returns -1 on failure or not found.  */
extern int __remenv_from_os (const char *__name);

/* Set runtime features according to system variables.  */
extern void __runtime_features (const char *__cmdline);

/* Print an error and exit the process. When MESSAGE is NULL, the printed
   error message is based on the current errno value.  */
extern void
__unixlib_fatal (const char *__message) __attribute__ ((__noreturn__));

/* Initialize the signal code.  */
extern void __unixlib_signal_initialise (struct proc *__p);

/* Resource limit initialisation */
extern void __resource_initialise (struct proc *__p);

/* Stack manipulation */

#define STACK_CHUNK_MAGIC 0xf60690ff

/* Structure representing the bottom part of a stack chunk */
/* See _syslib.s for more details */
struct __stack_chunk
{
  unsigned int magic;
  struct __stack_chunk *next;
  struct __stack_chunk *prev;
  unsigned int size;
  void (*dealloc) (void *);
  void (*returnaddr) (void);
};

extern void __stackalloc_init (void);
extern void *__stackalloc (size_t __size);
extern void __stackfree (void *__ptr);
extern int __stackalloc_trim (void);
extern void __free_stack_chain (void *__ptr);
extern void *__stackalloc_incr_wimpslot (int __incr);

/* Initialise the UnixLib world.  */
void __unixinit (void);

void _main (void);

/* Shared between sys/exec.c & sys/_exec.s */
extern _kernel_oserror *__exerr;

#endif  /* __UNIXLIB_INTERNALS */

__END_DECLS

#endif
