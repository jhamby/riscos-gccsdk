/* Copyright (c) 2002, 2003, 2004, 2005, 2006 UnixLib Developers
   Written by Martin Piper and Alex Waugh */

#include <pthread.h>

/* Return the thread id of the currently running thread */
pthread_t
pthread_self (void)
{
  return __pthread_running_thread;
}
