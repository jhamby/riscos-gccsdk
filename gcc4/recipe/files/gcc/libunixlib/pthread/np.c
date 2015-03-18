/* NP thread functions (np = non-portable GNU extensions)
   Written by Lee Noar.
   Copyright (c) 2015 UnixLib Developers.  */

/* The functions contained within this file were written using the information
   given at <http://linux.die.net>.  */

#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <kernel.h>

/* <http://linux.die.net/man/3/pthread_getattr_np>
 * 
 * The pthread_getattr_np() function initializes the thread attributes
 * object referred to by 'attr' so that it contains actual attribute
 * values describing the running thread 'thread'.
 */
int
pthread_getattr_np(pthread_t thread,
		   pthread_attr_t *attr)
{
  _kernel_stack_chunk *stack;

  if (!thread || !attr)
    return EINVAL;

  stack = (_kernel_stack_chunk *)thread->stack;
  if (!stack || stack->sc_mark != 0xf60690ff)
    return ESRCH;

  /* FIXME: Should we ignore the 536? */
  attr->stackaddr = (char *)stack + 536;
  attr->stacksize = stack->sc_size - 536;
  attr->guardsize = 0;

  return 0;
}

/* <http://linux.die.net/man/3/pthread_setname_np>
 * 
 * By default, all the threads created using pthread_create() inherit the
 * program name. The pthread_setname_np() function can be used to set a
 * unique name for a thread, which can be useful for debugging
 * multithreaded applications. The thread name is a meaningful C language
 * string, whose length is restricted to PTHREAD_MAX_NAMELEN_NP characters,
 * including the terminating null byte. The thread argument specifies the
 * thread whose name is to be changed; name specifies the new name.
 */
int
pthread_setname_np (pthread_t thread,
		    const char *name)
{
  if (strlen (name) > PTHREAD_MAX_NAMELEN_NP - 1)
    return ERANGE;

  strcpy (thread->name, name);

  return 0;
}

/* The pthread_getname_np() function can be used to retrieve the name of
 * the thread. The thread argument specifies the thread whose name is to
 * be retrieved. The buffer name is used to return the thread name;
 * len specifies the number of bytes available in name.
 * The buffer specified by name should be at least PTHREAD_MAX_NAMELEN_NP
 * characters in length.
 * The returned thread name in the output buffer will be null terminated.
 */
int
pthread_getname_np (pthread_t thread,
		    char *name,
		    size_t len)
{
  if (len < PTHREAD_MAX_NAMELEN_NP)
    return ERANGE;

  strcpy (name, thread->name);

  return 0;
}
