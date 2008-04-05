/* Internal UnixLib pthread.h
 * Written by Alex Waugh
 * Copyright (c) 2002-2008 UnixLib Developers
 */

#ifndef __PTHREAD_H
#include_next <pthread.h>
#endif

#if !defined(__INTERNAL_PTHREAD_H) && defined(__PTHREAD_H)
#define __INTERNAL_PTHREAD_H

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
  char fpregs[12*8]; /* Floating point registers */
  int fpstatus; /* Floating point status register */
};

extern pthread_t __pthread_thread_list; /* Linked list of all threads */

/* Called once early in program initialisation */
extern void __pthread_prog_init (void);

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
