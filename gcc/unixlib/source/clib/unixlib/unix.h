/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/unixlib/unix.h,v $
 * $Date: 2005/04/08 22:37:55 $
 * $Revision: 1.26 $
 * $State: Exp $
 * $Author: alex $
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
  /* The return code of this process.  */
  unsigned int return_code : 8;
  /* The number of the signal we terminated with.  */
  unsigned int signal : 8;
  /* This process terminated through a signal handler.  */
  unsigned int signal_exit : 1;
  /* This process terminated with a core dump.  */
  unsigned int core_dump : 1;
  /* This process just stopped.  */
  unsigned int stopped : 1;
  /* This process has exited, but has not been reaped by it's parent.  */
  unsigned int zombie : 1;
  /* This process has already been reported to wait() whilst it is stopped.   */
  unsigned int reported : 1;
};

struct proc
{
  int argc;  /* Command line argument count.  */
  char **argv;  /* Command line arguments.  */
  struct itimerval itimers[__MAX_ITIMERS];  /* Defined interval timers.  */
  struct rlimit limit[RLIMIT_NLIMITS];  /* Process resource limits.  */
  struct rusage usage;  /* Resource usage of the process.  */
  /* The signal states for this process. */
  struct unixlib_sigstate sigstate;
  /* Set to 1 if this process is currently sleeping or stopped.
     These could change at any time.  */
  volatile int sleeping, stopped;
  /* DDEUtils_Prefix value at the beginning of this process.  */
  const char *dde_prefix;
};

/* Beware of backwards compatibility when altering this structure.
   It is referenced by SUL and other processes */
struct __sul_process
{
  int reserved1; /* Reserved for use by SUL */
  int reserved2;
  void *(*sul_malloc)(__pid_t pid, size_t size);
  void (*sul_free)(__pid_t pid, void *ptr);
  int (*sul_exit) (__pid_t pid, int status) __attribute__ ((__noreturn__));
  __pid_t pid;   /* Process ID. */
  __pid_t ppid;  /* Parent process ID. */
  __pid_t pgrp;  /* ID of process group that process belongs to.  */
  __uid_t uid;   /* Real UID.  */
  __uid_t euid;  /* Effective UID.  */
  __gid_t gid;   /* Real GID.  */
  __gid_t egid;  /* Effective GID.  */
  int ppri;  /* Process priority.  */
  int gpri;  /* Process group priority.  */
  int upri;  /* User process priority.  */
  __mode_t umask;   /* File creation mode mask.  */
  void *file_descriptors;
  int maxfd;           /* Number of entries in the file_descriptors array */
  int fdsize;          /* Size of each entry in the file_descriptors array */
  int fdhandlesize;    /* Size of each fdhandle */
  int tty_type;
  struct tty *console;
  struct tty *rs423;
  char **environ;
  struct __process status;
  struct __sul_process *children;   /* Linked list of all children of this process */
  struct __sul_process *next_child;
  __pid_t (*sul_fork) (__pid_t pid, struct __sul_process **proc, void *stacklimit, void *stack);
  int environ_size;
  void (*sul_exec) (__pid_t pid, char *cli, void *stacklimit, void *stack);
  void *(*sul_wimpslot) (__pid_t pid, void *newslot);
};

extern struct __sul_process *__proc;
extern struct proc *__u;	/* current process */

extern int __funcall_error (const char *, int, unsigned int);
#if __UNIXLIB_PARANOID > 0
#define __funcall(f,p) \
  ((((void *)(f) >= __image_ro_base) && (((unsigned int)(f) & ~3) == (unsigned int)(f)) \
   ? 0 : __funcall_error(__FILE__,__LINE__,(unsigned int)(f))), (f)p)
#else
#define __funcall(f,p) ((f)p)
#endif



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
extern int __dynamic_num;
extern unsigned int __dynamic_area_refcount;
extern void __dynamic_area_exit (void);

extern void __munmap_all (void);	/* Deallocate all mappings.  */

/* Zero if we are not executing within a TaskWindow.  Non-zero otherwise.  */
extern int __taskwindow;
/* Zero if we are running in command mode (not as a WIMP program).  Non-zero
   otherwise and equals current WIMP task handle.  */
extern int __taskhandle;
/* Zero if we are running in 26-bit, non-zero for 32-bit */
extern int __32bit;
/* Non-zero if the escape key was disabled on program initialisation */
extern int __escape_disabled;

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

/* Return unsigned int from STRING.  END, if non-null, points to character
   after number.  */
extern unsigned int __decstrtoui (const char *__string, char **__end);

/* If this variable is non-null then we allocated the current environment.  */
extern char **__last_environ;

/* UnixLib's prefix for global RISC OS environment variables.  */
#define __UNIX_ENV_PREFIX "UnixEnv$"

/* Add NAME=VALUE to the environment. If NAME is already in the environment,
   only add when replace is non-zero.  If string is non-NULL then it is
   used directly, otherwise a copy of name and value is used. */
extern int __addenv_to_env (char *string, const char *__name,
                            const char *__value, int __replace);

/* Get environment value from OS and copy into BUF and return a pointer to BUF.
   If no match found then return NULL.  If BUF is NULL, then allocate space
   as necessary, otherwise BUFLEN is length of supplied buffer.
   If buf is NULL and call is not NULL then an enumeration is taking place.  */
extern char *__getenv_from_os (const char *__name, char *__buf,
			       size_t __buflen, char **call);

/* Return the integer value of NAME, from the RISC OS global environment.  */
extern int __intenv (const char *__name);

/* Get the leaf name from the command line used to run the program
   or returns __program_name if defined.  */
extern char *get_program_name (const char *cli, char *fname_buf, size_t fname_buf_len);

/* Set runtime features according to system variables.  */
extern void __runtime_features (const char *__cmdline);

/* Print an error and exit the process. When MESSAGE is NULL, the printed
   error message is based on the current errno value.  */
extern void
__unixlib_fatal (const char *__message) __attribute__ ((__noreturn__));

/* Atomically increment or decrement a word of memory */
extern unsigned int __atomic_modify (unsigned int *addr, int incr);

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

/* Free any remaining memory and file descriptors associated with a process */
void __free_process(struct __sul_process *process);

int _main (void);

extern pid_t __fork_post (pid_t pid, int isfork);
extern int __fork_pre (int isfork, void **sul_fork, pid_t *pid);

#endif  /* __UNIXLIB_INTERNALS */

__END_DECLS

#endif
