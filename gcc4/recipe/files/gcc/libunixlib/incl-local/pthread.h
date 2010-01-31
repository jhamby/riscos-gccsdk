/* Internal UnixLib pthread.h
 * Written by Alex Waugh
 * Copyright (c) 2002-2008, 2010 UnixLib Developers
 */

#ifndef __PTHREAD_H
#include_next <pthread.h>
#endif

#if !defined(__INTERNAL_PTHREAD_H) && defined(__PTHREAD_H)
#define __INTERNAL_PTHREAD_H

/* Hold all details about the thread. A pthread_t points to one of these structures */
struct __pthread_thread
{
  int magic; /* Magic number */

  /* Space for registers on the context switch */
  struct __pthread_saved_context *saved_context;

  void *alloca; /* Storage for alloca_chunk linked list */

  int thread_errno; /* Value of errno for this thread */

  struct
  {
    int errnum;
    char errmess[252];
  } errbuf; /* Last RISC OS error from error handler */

  /* WARNING: Various assembly files refer to this structure, using
     offsets defined in internal/asm_dec.s.
     If you change the ordering of any of the elements above this
     comment then make sure the offsets are kept in sync. */

  struct __pthread_thread *next; /* Linked list of all threads */
  enum __pthread_state state; /* Running/blocked/idle/etc. */
  void *ret; /* Value returned from thread */

  void *(*start_routine)(void *); /* Function to call as the new thread */
  void *arg; /* Argument to pass to the start_routine */

  /* Initial stack chunk allocated to thread */
  struct __stack_chunk *stack;

  /* Thread that wishes to join with this thread */
  struct __pthread_thread *joined;

  /* Linked list of keys associated with this thread */
  struct __pthread_key *keys;

  /* Mutex that the thread is waiting for */
  pthread_mutex_t *mutex;

  /* Type of mutex that the thread is waiting for */
  enum __pthread_locktype mutextype;

  /* Condition var that the thread is waiting for */
  pthread_cond_t *cond;

  /* Timeout value for condition var */
  clock_t condtimeout;

  /* Next thread that is waiting on the same mutex/condition var */
  struct __pthread_thread *nextwait;

  /* Linked list of cleanup functions to call when this thread exits */
  struct __pthread_cleanup *cleanupfns;

  /* Scheduling parameters. Currently ignored */
  struct sched_param __param;

  /* Scheduling policy.  Currently ignored.  */
  int __policy;

  /* Cancelability state of this thread (enabled or disabled) */
  unsigned int cancelstate : 1;

  /* Cancelability type (asynchronous or deferred) */
  unsigned int canceltype : 1;

  /* Should this thread be cancelled when it next reaches a
     cancellation point.  */
  unsigned int cancelpending : 1;

  /* Is the thread detached of can it still be joined to.  */
  unsigned int detachstate : 1;

  __sigset_t blocked; /* Signal mask for this thread. */
  __sigset_t pending; /* Pending signals for this thread */
  pthread_cond_t sigwait_cond;
};

extern pthread_t __pthread_running_thread; /* Currently running thread */

/* Print lots of general debugging info */
/*#define PTHREAD_DEBUG*/
/* Print debug info for the context switcher */
/*#define PTHREAD_DEBUG_CONTEXT*/

/* Magic number to check a pthread_t is valid */
#define PTHREAD_MAGIC 0x52485450

#define __pthread_invalid(thread) \
	(thread == NULL || thread->magic != PTHREAD_MAGIC)

/* Holds data about a thread specific key */
struct __pthread_key
{
  pthread_key_t keyid; /* Key that this value is attached to */
  union {
    const void *constvalue;
    void *nonconst;
  } value; /* Value of the key for this thread. Mess around with a union
              to get around const issues */
  void (*destructor) (void*); /* Destructor function to call for this value */
  struct __pthread_key *next; /* Linked list of all keys for this thread */
};

/* Holds all the state associated with a thread when it is switched out */
struct __pthread_saved_context
{
  int r[16]; /* User mode integer registers */
  int spsr;
#ifndef __SOFTFP__
  char fpregs[12*8]; /* Floating point registers */
  int fpstatus; /* Floating point status register */
#endif
};

/* A pointer to this structure (allocated from RMA) is passed to
   pthread_call_every in r12 so that it can validate the application
   without accessing its address space.
   As the filter name also needs to be RMA, it's convenient to use
   the same block.  */
struct __pthread_callevery_block
{
  unsigned int got;
  void *sul_upcall_addr;
  void *sul_upcall_r12;
  char filter_name[];
};

extern pthread_t __pthread_thread_list; /* Linked list of all threads */

/* Called once early in program initialisation */
extern void __pthread_prog_init (void);

extern void __pthread_prog_fini (void);

/* Called if a non recoverable error is detected */
extern void __pthread_fatal_error (const char *msg);

/* Initialise a new node, allocating it if node is NULL */
extern pthread_t __pthread_new_node (pthread_t node);

/* Common routines used by mutex/rwlock/cond functions */
extern int __pthread_lock_lock (pthread_mutex_t *mutex, const enum __pthread_locktype, const int trylock);
extern int __pthread_lock_unlock (pthread_mutex_t *mutex, const int yield);

/* Call all cleanup handlers registered for the current thread */
extern void __pthread_cleanup_callhandlers (void);

/* Call all destructors for remaining thread specific key values */
extern void __pthread_key_calldestructors (void);

/* Called by fork() just before the fork is about to happen */
extern void __pthread_atfork_callprepare (void);
/* Called by fork() once the fork has happened */
extern void __pthread_atfork_callparentchild (const int parent);

/* Allocate and initialise a stack chunk for a new thread */
extern struct __stack_chunk *__pthread_new_stack (void);

/* Main scheduling code */
extern void __pthread_context_switch (void);

/* Assembly functions */

/* Prevent the callevery interrupt from initialising a context switch.  */
extern int __pthread_disable_ints (void);
/* Allow the callevery interrupt from initialising a context switch.  */
extern int __pthread_enable_ints (void);
/* Initialise a context save area.  */
extern void __pthread_init_save_area (struct __pthread_saved_context *);
/* Calls alloca cleanup functions on thread exit.  */
extern void __pthread_exit (void);

/* Prevent the callevery interrupt from initialising a context switch,
   and register a function to reenable them when the current function
   returns.  */
extern void __pthread_protect_unsafe (void);

/* Should be placed at the beginning of a function body for a
   thread safe function that is defined as a cancellation point.  */
#define PTHREAD_SAFE_CANCELLATION \
  do \
    { \
      if (__ul_global.pthread_system_running) \
        pthread_testcancel (); \
    } while (0);

/* Should be placed at the beginning of a function body for a
   function that is not reentrant.  */
#define PTHREAD_UNSAFE \
  do \
    { \
      if (__ul_global.pthread_system_running) \
        __pthread_protect_unsafe (); \
    } while (0);

/* Should be placed at the beginning of a function body for a
   function that is not reentrant and is also defined as a cancellation
   point.  */
#define PTHREAD_UNSAFE_CANCELLATION \
  do \
    { \
      if (__ul_global.pthread_system_running) \
        { \
          pthread_testcancel (); \
          __pthread_protect_unsafe (); \
        } \
    } while (0);

__END_DECLS

#endif
