/*
 * pthread.h - IEEE Std 1003.1-2001 threads
 * Written by Alex Waugh
 * Copyright (c) 2002-2015 UnixLib Developers
 */

#ifndef __PTHREAD_H

#if (! defined __need_pthread_t)
#define __PTHREAD_H
#endif

#define __need_size_t
#include <stddef.h>

#define __need_clock_t
#include <time.h>

#include <sched.h>
#include <bits/sched.h> /* for struct sched_param */

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

__BEGIN_DECLS

#if !defined __pthread_t_defined && (defined __PTHREAD_H || defined __need_pthread_t)
#define __pthread_t_defined

/* Mutex/rwlock/cond var attributes structure */
struct __pthread_lock_attr
{
  int pshared; /* can the mutex be shared between processes */
  int type; /* mutex type */
};

/* Mutex/rwlock type */
enum __pthread_locktype
{
  LOCK_INVALID = 0,
  LOCK_READ  = 'R',
  LOCK_WRITE = 'W',
  LOCK_MUTEX = 'M'
};

/* Mutex/rwlock object */
struct __pthread_lock
{
  /* Number of times the current thread has got this lock recursively */
  unsigned int count;
  /* Type of lock.  */
  enum __pthread_locktype type;
  /* Thread that currently owns the lock.  */
  struct __pthread_thread *owner;
  /* Linked list of threads that are blocked on this lock.  */
  struct __pthread_thread *waiting;
  /* Attributes of the lock.  */
  struct __pthread_lock_attr attr;
};

/* Condition var object */
struct __pthread_cond
{
  /* Linked list of threads that are blocked on this condition var.  */
  struct __pthread_thread *waiting;
};

typedef struct __pthread_lock pthread_mutex_t;
typedef struct __pthread_lock pthread_rwlock_t;
typedef struct __pthread_cond pthread_cond_t;

typedef struct __pthread_lock_attr pthread_mutexattr_t;
typedef struct __pthread_lock_attr pthread_rwlockattr_t;
typedef struct __pthread_lock_attr pthread_condattr_t;

typedef struct __pthread_thread *pthread_t; /* Thread ID type */

/* Thread attributes object */
struct __pthread_attr
{
  int detachstate;  /* Whether newly created threads should be joinable or detached */
  size_t guardsize; /* Amount of extra memory to allocate at the end of the stack, in case of overflow */
                    /* Not used under RISC OS as the stack is extensible */
  void *stackaddr;  /* Address of the stack to use for the thread */
                    /* Ignored under RISC OS, as a malloced area may be unsuitable for the stack */
  size_t stacksize; /* Size to use for the thread's stack (ignored) */
  struct sched_param param; /* Scheduling parameters. Currently ignored */
};

typedef struct __pthread_attr pthread_attr_t;

typedef int pthread_key_t; /* Key type for thread specific data */

typedef int pthread_once_t; /* Type for pthread_once */

/* Current schedulability state of the thread */
enum __pthread_state
{
  STATE_IDLE,
  STATE_MUTEX_WAIT, /* Waiting for a mutex to become available */
  STATE_COND_WAIT, /* Waiting for a condition variable to be signalled */
  STATE_COND_TIMED_WAIT, /* Waiting for a condition variable with a timeout */
  STATE_JOIN,
  STATE_UNALLOCED,
  STATE_BLOCKED, /* Waiting on a semaphore */

  /* All states below this point are for threads that may be switched in by the scheduler */
  STATE_RUNNING, /* A normally running thread */
  STATE_CLEANUP  /* A (possibly cancelled) thread that is running its cleanup handlers */
};

#endif /* !defined __pthread_t_defined && (defined __PTHREAD_H || defined __need_pthread_t) */
#undef __need_pthread_t

#ifdef __PTHREAD_H

/* Including pthread.h should make all symbols defined in time.h visible */
#include <time.h> /* for struct timespec */

/* Main thread routines */

/* Run start_routine as a new thread */
extern int pthread_create (pthread_t *__restrict thread,
			   const pthread_attr_t *__restrict attr,
			   void *(*start_routine) (void *),
			   void *__restrict arg) __THROW;

/* Exit from the current thread */
extern void pthread_exit (void *value_ptr) __attribute__ ((__noreturn__));

/* Wait for another thread to finish and get its exit status.
   This is a cancellation point.  */
extern int pthread_join (pthread_t thread, void **value_ptr);

/* Detach a thread so all its memory is freed when it exits */
extern int pthread_detach (pthread_t thread) __THROW;

/* Force a context switch as the current thread has nothing to do */
extern void pthread_yield (void) __THROW;

/* Cancellation */

/* Exit status returned by a thread that has been cancelled */
#define PTHREAD_CANCELED ((void*)1)

/* Enable a thread to be cancelled */
#define PTHREAD_CANCEL_ENABLE 0

/* Disable thread from being cancelled */
#define PTHREAD_CANCEL_DISABLE 1

/* Only cancel a thread when it reaches a cencellation point */
#define PTHREAD_CANCEL_DEFERRED 0

/* Cancel a thread at any point */
#define PTHREAD_CANCEL_ASYNCHRONOUS 1

/* Set whether the current thread can be cancelled or not */
extern int pthread_setcancelstate (int state, int *oldstate);

/* Set whether the current thread can be cancelled asynchonously or not */
extern int pthread_setcanceltype (int type, int *oldtype);

/* Introduce a cancellation point into the current thread */
extern void pthread_testcancel (void);

/* Cancel a thread */
extern int pthread_cancel (pthread_t thread);

/* Cleanup */

/* Record details of a cleanup handler */
struct __pthread_cleanup
{
  void (*routine) (void*); /* Cleanup routine */
  void *arg; /* Argument to call cleanup routine with */
  struct __pthread_cleanup *next; /* Linked list of other cleanup handlers */
};

/* Add a new cleanup handler */
/* For every cleanup_push there must be exactly one corresponding cleanup_pop in the same scope */
#define pthread_cleanup_push(cleanup_routine, cleanup_arg) \
{ \
  struct __pthread_cleanup __pthread_cleanupblk; \
  __pthread_cleanupblk.routine = cleanup_routine; \
  __pthread_cleanupblk.arg = cleanup_arg; \
\
  __pthread_cleanup_push (&__pthread_cleanupblk);

/* Remove a cleanup handler, running it first if execute is nonzero */
#define pthread_cleanup_pop(execute) \
  if (execute && __pthread_cleanupblk.routine != NULL) \
    __pthread_cleanupblk.routine(__pthread_cleanupblk.arg); \
\
  __pthread_cleanup_pop (); \
}

/* Internal functions used by the above macros */
extern void __pthread_cleanup_push (struct __pthread_cleanup *cleanup);
extern void __pthread_cleanup_pop (void);

/* Thread attributes */

/* The thread should be created so other threads may join with it */
#define PTHREAD_CREATE_JOINABLE 0
/* The thread should be created detached, so no threads may join with it */
#define PTHREAD_CREATE_DETACHED 1


/* Initialise a thread attributes object with default values */
extern int pthread_attr_init (pthread_attr_t *attr);

/* Destroy a thread attributes object (does nothing) */
extern int pthread_attr_destroy (pthread_attr_t *attr);

/* Sets the detach state attribute */
extern int pthread_attr_setdetachstate (pthread_attr_t *attr, int detachstate);

/* Gets the detach state attribute */
extern int pthread_attr_getdetachstate (const pthread_attr_t *__restrict attr,
					int *detachstate);

/* Sets the size of the stack guard attribute
   The stack guard is an area of memory at the end of the stack to
   provide protection against stack overflow. This is ignored as
   the stack is extensible. */
extern int pthread_attr_setguardsize (pthread_attr_t *attr, size_t guardsize);

/* Gets the size of the stack guard attribute */
extern int pthread_attr_getguardsize (const pthread_attr_t *__restrict attr,
				      size_t *guardsize);

/* Sets the address of a thread's stack
   This is ignored as there are limitations on what can be used
   for the stack. */
extern int pthread_attr_setstackaddr (pthread_attr_t *attr, void *stackaddr);
/* Gets the address of a thread's stack */
extern int pthread_attr_getstackaddr (const pthread_attr_t *__restrict attr,
				      void **__restrict stackaddr);

/* Sets the size of a thread's stack (ignored) */
extern int pthread_attr_setstacksize (pthread_attr_t *attr, size_t stacksize);

/* Gets the size of a thread's stack (as set with setstacksize) */
extern int pthread_attr_getstacksize (const pthread_attr_t *__restrict attr,
				      size_t *__restrict stacksize);

/* Set scheduling parameter attributes (currently ignored) */
extern int pthread_attr_setschedparam (pthread_attr_t *__restrict attr,
				       const struct sched_param *__restrict
				       param);

/* Get scheduling parameter attributes */
extern int pthread_attr_getschedparam (const pthread_attr_t *__restrict attr,
				       struct sched_param *__restrict param);

extern int pthread_attr_getstack(const pthread_attr_t *attr,
				 void **stackaddr,
				 size_t *stacksize);

/* Mutexes */

/* Static default initialiser for a pthread_mutex_t */
#define PTHREAD_MUTEX_INITIALIZER {0,LOCK_MUTEX,NULL,NULL,{PTHREAD_PROCESS_PRIVATE,PTHREAD_MUTEX_DEFAULT}}

/* A recursively attempt to lock a mutex will cause deadlock */
#define PTHREAD_MUTEX_NORMAL 0
/* A recursively attempt to lock a mutex will cause an error to be returned */
#define PTHREAD_MUTEX_ERRORCHECK 1
/* A recursively attempt to lock a mutex will succeed */
#define PTHREAD_MUTEX_RECURSIVE 2
/* The default of the above settings */
#define PTHREAD_MUTEX_DEFAULT 1

/* The mutex is private to the process */
#define PTHREAD_PROCESS_PRIVATE 0
/* The mutex is shared between processes (ignored) */
#define PTHREAD_PROCESS_SHARED 1

/* Initialise a mutex object with the specified attributes */
extern int pthread_mutex_init (pthread_mutex_t *__restrict mutex,
			       const pthread_mutexattr_t *__restrict attr);

/* Destroy a mutex that is finished with */
extern int pthread_mutex_destroy (pthread_mutex_t *mutex);

/* Lock a mutex, blocking if it is not available */
extern int pthread_mutex_lock (pthread_mutex_t *mutex);

/* Try to lock a mutex, but don't block if it is unavailable */
extern int pthread_mutex_trylock (pthread_mutex_t *mutex);

/* Unlock a previously locked mutex */
extern int pthread_mutex_unlock (pthread_mutex_t *mutex);

/* Initialise an attributes object */
extern int pthread_mutexattr_init (pthread_mutexattr_t *attr);

/* Destroy an attributes object */
extern int pthread_mutexattr_destroy (pthread_mutexattr_t *attr);

/* Set the process shared attribute (ignored) */
extern int pthread_mutexattr_setpshared (pthread_mutexattr_t *attr, int pshared);
/* Get the process shared attribute */
extern int pthread_mutexattr_getpshared (const pthread_mutexattr_t *__restrict
					 attr, int *__restrict pshared);

/* Set the type of mutex (specifies the behaviour when a thread recursively locks the mutex) */
extern int pthread_mutexattr_settype (pthread_mutexattr_t *attr, int type);

/* Get the type of mutex */
extern int pthread_mutexattr_gettype (const pthread_mutexattr_t *__restrict
				      attr, int *__restrict type);

/* Read/Write locks */

/* Static default initialiser for a pthread_rwlock_t */
#define PTHREAD_RWLOCK_INITIALIZER {0,LOCK_READ,NULL,NULL,{PTHREAD_PROCESS_PRIVATE,PTHREAD_MUTEX_DEFAULT}}

/* Initialise a rwlock object with the specified attributes */
extern int pthread_rwlock_init (pthread_rwlock_t *__restrict rwlock,
				const pthread_rwlockattr_t *__restrict attr);

/* Destroy a rwlock that is finished with */
extern int pthread_rwlock_destroy (pthread_rwlock_t *rwlock);

/* Try to lock a rwlock for reading, blocking if unavailable */
extern int pthread_rwlock_rdlock (pthread_rwlock_t *rwlock);

/* Try to lock a rwlock for reading, but don't block if it is unavailable */
extern int pthread_rwlock_tryrdlock (pthread_rwlock_t *rwlock);

/* Try to lock a rwlock for writing, blocking if unavailable */
extern int pthread_rwlock_wrlock (pthread_rwlock_t *rwlock);

/* Try to lock a rwlock for writing, but don't block if it is unavailable */
extern int pthread_rwlock_trywrlock (pthread_rwlock_t *rwlock);

/* Unlock a previously locked rwlock */
extern int pthread_rwlock_unlock (pthread_rwlock_t *rwlock);

/* Initialise an attributes object */
extern int pthread_rwlockattr_init (pthread_rwlockattr_t *attr);

/* Destroy an attributes object */
extern int pthread_rwlockattr_destroy (pthread_rwlockattr_t *attr);

/* Set the process shared attribute (ignored) */
extern int pthread_rwlockattr_setpshared (pthread_rwlockattr_t *attr,
					  int pshared);

/* Get the process shared attribute */
extern int pthread_rwlockattr_getpshared (const pthread_rwlockattr_t *
					  __restrict attr,
					  int *__restrict pshared);

/* Condition variables */

/* Static default initialiser for a pthread_cond_t */
#define PTHREAD_COND_INITIALIZER {NULL}

/* Initialise a condition variable with the given attributes */
extern int pthread_cond_init (pthread_cond_t *__restrict cond,
			      const pthread_condattr_t *__restrict attr);

/* Destroy a condition variable */
extern int pthread_cond_destroy (pthread_cond_t *cond);

/* Wait for a condition variable to be signalled */
extern int pthread_cond_wait (pthread_cond_t *__restrict cond,
			      pthread_mutex_t *__restrict mutex);

/* Wait for a condition variable to be signalled, with a timeout */
extern int pthread_cond_timedwait (pthread_cond_t *__restrict cond,
				   pthread_mutex_t *__restrict mutex,
				   const struct timespec *__restrict abstime);

/* Signal the specified condition var */
extern int pthread_cond_signal (pthread_cond_t *cond);

/* Broadcast a signal to all threads waiting on this conition var */
extern int pthread_cond_broadcast (pthread_cond_t *cond);

/* Initialise a condition variable attributes object */
extern int pthread_condattr_init (pthread_condattr_t *attr);

/* Destroy an attributes object */
extern int pthread_condattr_destroy (pthread_condattr_t *attr);

/* Set process shared attribute (ignored) */
extern int pthread_condattr_setpshared (pthread_condattr_t *attr, int pshared);

/* Get process shared attribute */
extern int pthread_condattr_getpshared (const pthread_condattr_t *
					__restrict attr,
					int *__restrict pshared);

/* Keys */

/* Maximum number of times a key destructor will be called */
#define PTHREAD_DESTRUCTOR_ITERATIONS 50
/* Maximum number of keys that can be created */
#define PTHREAD_KEYS_MAX 256

/* Create a key and associate a destructor function with it */
extern int pthread_key_create (pthread_key_t *key, void (*destructor) (void*));

/* Deletes a thread specific key (does not call the key's destructors) */
extern int pthread_key_delete (pthread_key_t key);

/* Set a thread specific value to associate with the given key */
extern int pthread_setspecific (pthread_key_t key, const void *value);

/* Read value associated with given key for the current thread
   Returns NULL if no value has been set for this thread yet */
extern void *pthread_getspecific (pthread_key_t key);

/* Miscellaneous */


/* Max munber of threads that may be active at once */
#define PTHREAD_THREADS_MAX 256

/* Size to allocate for a thread's initial stack chunk */
#define PTHREAD_STACK_MIN 4096

/* Static default initialiser for a pthread_once_t */
#define PTHREAD_ONCE_INIT 0

/* Ensure init_routine is called exactly once */
extern int pthread_once (pthread_once_t *once_control,
			 void (*init_routine) (void));

/* Return the thread id of the current thread */
extern pthread_t pthread_self (void);

/* Compare two thread ids */
extern int pthread_equal (pthread_t t1, pthread_t t2);

/* Set handlers to be called when a process forks */
extern int pthread_atfork (void (*prepare)(void), void (*parent)(void),
			   void (*child)(void));

/* Start the callevery interrupt */
extern void __pthread_start_ticker (void);
/* Remove the callevery interrupt */
extern void __pthread_stop_ticker (void);

extern int pthread_setschedparam (pthread_t __thr,
				  int __policy,
				  const struct sched_param *__param) __THROW;

extern int pthread_getschedparam (pthread_t __thr,
				  int *__policy,
				  struct sched_param *__param)
     __THROW;

#ifdef __USE_GNU
     
/* np functions
   Non portable/posix GNU extensions */

/*
 * Maximum length of a thread's name, including the terminating NUL.
 */
#define	PTHREAD_MAX_NAMELEN_NP	32

extern int pthread_setname_np (pthread_t thread_id,
			       const char *name);

extern int pthread_getname_np (pthread_t thread_id,
			       char *name,
			       size_t len);

extern int pthread_getattr_np (pthread_t thread_id,
			       pthread_attr_t *attr);

#endif /* __USE_GNU */

#endif /* __PTHREAD_H */

__END_DECLS

#endif /* ! __PTHREAD_H */
