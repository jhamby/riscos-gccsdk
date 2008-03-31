/* Structures for UnixLib's internal process management.
   Copyright (c) 2002-2007 UnixLib Developers.  */

#ifndef __UNIXLIB_UNIX_H
#define __UNIXLIB_UNIX_H 1

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

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif
#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#ifndef __INTERNAL_FD_H
#include <internal/fd.h>
#endif
#ifndef __INTERNAL_SIGSTATE_H
#include <internal/sigstate.h>
#endif
#ifndef __INTERNAL_TTY_H
#include <internal/tty.h>
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
  /* This process has exited, but has not been reaped by its parent.  */
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
   It is referenced by SUL and other processes.  */
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
  unsigned int maxfd;  /* Number of entries in the file_descriptors array */
  unsigned int fdsize; /* Size of each entry in the file_descriptors array */
  unsigned int fdhandlesize; /* Size of each fdhandle */
  int tty_type;
  struct tty *console;
  struct tty *rs423;
  char **environ;
  struct __process status;
  struct __sul_process *children;   /* Linked list of all children of this process */
  struct __sul_process *next_child;
  __pid_t (*sul_fork) (__pid_t pid, struct __sul_process **proc, void *stacklimit, void *stack);
  unsigned int environ_size;
  void (*sul_exec) (__pid_t pid, char *cli, void *stacklimit, void *stack) __attribute__ ((__noreturn__));
  void *(*sul_wimpslot) (__pid_t pid, void *newslot);
};

extern struct proc *__u;	/* current process */

/* This structure must be kept in perfect synchronisation with:

     a) GBL_* definitions in incl-local/internal/asm_dec.s
     b) The __ul_global structure at the end of sys/_syslib.s

   Functions wishing to use this structure can reduce the number of
   load/store operations by defining a local-variable i.e.

      void foo (void)
      {
        struct ul_global *gbl = &__ul_global;
        ...
      }

   Otherwise the compiler will force a lookup on __ul_global for each
   access.  */
struct ul_global
{
  const char *cli;
  unsigned int time[2];
  int rewindpipeoffset;
  int taskwindow;
  int taskhandle; /* consider using __get_taskhandle() to read this value.  */
  int dynamic_num;
  int changeredir0;
  int changeredir1;
  int panic_mode;
  struct __sul_process *sulproc;
  int pagesize;
  void *upcall_handler_addr;
  void *upcall_handler_r12;

  void *pthread_return_address;
  volatile int pthread_worksemaphore;
  volatile int pthread_callback_semaphore;
  int pthread_system_running;
  int pthread_callback_missed;
  int pthread_num_running_threads;

  int executing_signalhandler;
  void *signalhandler_sl;
  void *signalhandler_sp;

  void *__notused5;

  void *malloc_state;

#if defined(PIC)
  int (*main) (int, char *[], char **);
#else
  void *__unused3; /* To remain unused */
#endif

  int escape_disabled;
};

/* This structure must be kept in perfect synchronisation with:

     a) MEM_* definitions in incl-local/internal/asm_dec.s
     b) The __ul_memory structure at the end of sys/_syslib.s

   Functions wishing to use this structure can reduce the number of
   load/store operations by defining a local-variable i.e.

      void foo (void)
      {
        struct ul_memory *mem = &__ul_memory;
        ...
      }
   */
struct ul_memory
{
  /* Serialise access to this structure (opaque type).  */
  void *mutex;

  /* This variable sets the value of what is considered to be the
     maximum value of addressable memory within application space.
     This value can increase during program execution if there is a
     request for more memory.

     The value of 'appspace_himem' will always be less-than or equal-to
     the value of 'appspace_limit'.

     Note that when dynamic areas are in use, the only function that will
     try to increase 'appspace_himem' and 'appspace_limit' is the stack
     extension code.  */
  unsigned int appspace_himem;

  /* Current lowest position of the stack.  It lowers in memory address
     until it reaches 'stack_limit'.  Once reached, attempts to
     increase stack size will happen by increasing the wimpslot.  */
  unsigned int stack;

  /* This points to the start of application memory, usually 0x8000.
     Defined as Image$$RO$$Base for AOF.
     Defined as __executable_start for ELF.  */
  const unsigned int robase;

  /* This points to the end of the executable, including any read/write
     data that has been initialised before program start.

     It is called 'rwlomem' because it refers to the lowest memory address
     that is usable for run-time allocation of memory.

     Defined as Image$$RW$$Limit for AOF.
     Defined as __end__ for ELF.  */
  const unsigned int rwlomem;

  /* This points to the start of read/write data.
     Defined as Image$$RW$$Base for AOF.
     Defined as __data_start for ELF.  */
  const unsigned int rwbase;

  /* As data is requested from the system by 'brk' to satisfy allocation
     and deallocation requests and we're not using a dynamic area to store
     our heap, the value of 'rwbreak' will change.

     Initially it starts off equal to 'rwlomem', meaning that no data
     has been allocated at run-time.

     When a request is made for more space, then 'rwbreak' will increase.  */
  unsigned int rwbreak;

  /* This variable points to the lowest extent to which the stack can
     grow.  Its value increases in line with 'rwbreak' as more memory
     is taken from the system.  */
  unsigned int stack_limit;

  /* Holds the base memory address of the dynamic area.  It is used to
     ensure that any requests by 'brk' or 'sbrk' to change the data segment
     size, cannot reduce it past this value.

     This variable is used only when dynamic areas are in use.  */
  unsigned int dalomem;

  /* This variable is used only when dynamic areas are in use.  It records
     the used space in a dynamic area, which will increase or decrease
     as required according to the user's calls to 'brk'.  */
  unsigned int dabreak;

  /* This variable is used only when dynamic areas are in use.  It records
     the upper limit of the dynamic area and will increase/decrease as
     we claim or release memory.  */
  unsigned int dalimit;

  /* This variable is used by the non-contiguous stack chunk code.
     It records the upper limit of application space (or wimpslot)
     and will increase as more is required in multiples of 32K.  In such
     circumstances, this means that 'appspace_limit' will be larger than
     'appspace_himem'.  */
  unsigned int appspace_limit;

  /* Value of 'appspace_himem' last time the stack was increased.  */
  unsigned int old_himem;
};

extern struct ul_memory __ul_memory;

extern struct ul_global __ul_global;

/* Returns __ul_global.taskhandle and makes sure that this value is still
   up-to-date.  */
extern int __get_taskhandle (void);

extern int __funcall_error (const char *, int, unsigned int);
#if __UNIXLIB_PARANOID
#define __funcall(f, p) \
  ((((unsigned int)(f) >= __ul_memory.robase) && !((unsigned int)(f) & 3) && (unsigned int)(f) < __ul_memory.appspace_himem \
   ? 0 : __funcall_error(__FILE__, __LINE__, (unsigned int)(f))), (f)p)
#else
#define __funcall(f, p) ((f)p)
#endif

extern unsigned int __dynamic_area_refcount;
extern void __dynamic_area_exit (void);

extern void __munmap_all (void);	/* Deallocate all mappings.  */

/* Stop the interval timers.  */
extern void __stop_itimers (void);

/* OS_ChangeEnvironment is used to set up exception handlers. These
   handlers use OS_CallBack to raise signals in the foreground process.
   RTFM (RISC OS PRM - 'The Program Environment') for more info. */

/* Reset handlers, etc. back to original state.  */
extern void __env_riscos (void);
extern void __env_unixlib (void);

/* Return unsigned int from STRING.  END, if non-null, points to character
   after number.  */
extern unsigned int __decstrtoui (const char *__string, const char **__end)
	__nonnull ((1));

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
			       size_t __buflen, char **call)
	__nonnull ((1)) __wur;

/* Return the integer value of NAME, from the RISC OS global environment.  */
extern int __intenv (const char *__name)
	__nonnull ((1)) __wur;

/* Get the leaf name from the command line used to run the program
   or returns __program_name if defined.  */
extern char *__get_program_name (const char *cli, char *fname_buf,
                               size_t fname_buf_len)
	__nonnull ((1, 2)) __wur;

/* Set runtime features according to system variables.  */
extern void __runtime_features (const char *__cmdline)
	__nonnull ((1));

/* Print an error and exit the process. When MESSAGE is NULL, the printed
   error message is based on the current errno value.  */
extern void __unixlib_fatal (const char *__message)
	__attribute__ ((__noreturn__));

/* Atomically increment or decrement a word of memory */
extern unsigned int __atomic_modify (unsigned int *addr, int incr)
	__nonnull ((1));

/* Initialize the signal code.  */
extern void __unixlib_signal_initialise (struct proc *__p)
	__nonnull ((1));

/* Resource limit initialisation */
extern void __resource_initialise (struct proc *__p)
	__nonnull ((1));

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
extern void *__stackalloc (size_t __size) __wur;
extern void __stackfree (void *__ptr);
extern int __stackalloc_trim (void);
extern void __free_stack_chain (void *__ptr) __nonnull ((1));
extern unsigned int __stackalloc_incr_wimpslot (unsigned int __incr) __wur;

/* Initialise the UnixLib world.  */
extern void __unixinit (void);

/* Free any remaining memory and file descriptors associated with a
   process.  */
extern void __free_process (struct __sul_process *process)
	__nonnull ((1));

extern int _main (void);

extern pid_t __fork_post (pid_t pid, int isfork);
extern int __fork_pre (int isfork, void **sul_fork, pid_t *pid);

extern unsigned int __get_cpu_arch (void);

#if __UNIXLIB_SYMLINKS
extern int __resolve_symlinks (const char *filename_in, char *filename_out,
			       size_t fn_out_size);
#endif

#endif  /* __UNIXLIB_INTERNALS */

__END_DECLS

#endif
