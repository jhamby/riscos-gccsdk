/* Thread cancellation functions.
   Copyright (c) 2002, 2003, 2004, 2005, 2006, 2008 UnixLib Developers
   Written by Alex Waugh */

#include <errno.h>

#include <internal/os.h>
#include <pthread.h>


/* Set the cancelability state of the current thread */
int
pthread_setcancelstate (int state, int *oldstate)
{
  if (state != PTHREAD_CANCEL_ENABLE && state != PTHREAD_CANCEL_DISABLE)
    return EINVAL;

  __pthread_disable_ints ();

  if (oldstate != NULL)
    *oldstate = __pthread_running_thread->cancelstate;

  __pthread_running_thread->cancelstate = state;

  __pthread_enable_ints ();
  return 0;
}

/* Set the type of cancellation that may occur on the current thread */
int
pthread_setcanceltype (int type, int *oldtype)
{
  if (type != PTHREAD_CANCEL_DEFERRED && type != PTHREAD_CANCEL_ASYNCHRONOUS)
    return EINVAL;

  __pthread_disable_ints ();

  if (oldtype != NULL)
    *oldtype = __pthread_running_thread->canceltype;

  __pthread_running_thread->canceltype = type;

  __pthread_enable_ints ();
  return 0;
}

/* Cancel a specific thread */
int
pthread_cancel (pthread_t thread)
{
  if (__pthread_invalid (thread))
    return ESRCH;

  thread->cancelpending = 1;

  return 0;
}

#ifndef PIC

/* This function is never called or referenced. Its only purpose is to ensure
   that the enclosed functions are pulled in by the static linker for the sake
   of the gthread implementation in libstdc++. */
static void __attribute__((used))
__pthread_pull_gthr_weak(void)
{
  pthread_once(NULL, NULL);
  pthread_key_create(NULL, NULL);
  pthread_key_delete(0);
  pthread_getspecific(0);
  pthread_setspecific(0, NULL);
  pthread_mutex_lock(NULL);
  pthread_mutex_trylock(NULL);
  pthread_mutex_unlock(NULL);
}

#endif
