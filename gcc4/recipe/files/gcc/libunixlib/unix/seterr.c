/* __ul_seterr
 * Copyright (c) 2010 UnixLib Developers
 */

#include <errno.h>
#ifndef __TARGET_SCL__
#  include <pthread.h>
#endif
#include <stdlib.h>

#include <internal/os.h>

#ifdef __TARGET_SCL__
extern int _stub_errorBuffer;
extern _kernel_oserror _stub_errorNumber;
#endif

/* Used to set errno because of a RISC OS error happened.
   err points to RISC OS error block or is NULL.
   new_errno is the new value of errno.
   Returns -1 when err is non-NULL, 0 otherwise.  */
int
__ul_seterr (const _kernel_oserror *err, int new_errno)
{
  if (err == NULL)
    return 0;

#ifdef __TARGET_SCL__
  errno = new_errno;

  _stub_errorNumber.errnum = err->errnum;
  char *dst = _stub_errorNumber.errmess;
#else
#  if __UNIXLIB_ERRNO_THREADED
  __pthread_running_thread->thread_errno = new_errno;
#  else
  errno = new_errno;
#  endif

  __pthread_running_thread->errbuf.errnum = err->errnum;
  char *dst = __pthread_running_thread->errbuf.errmess;
#endif
  const char *src = err->errmess;
  for (size_t maxlen = sizeof (err->errmess) - 1;
       maxlen != 0 && *src >= ' ';
       --maxlen)
    *dst++ = *src++;
  *dst = '\0';

#ifdef __TARGET_SCL__
  _stub_errorBuffer = 1;
#else
  __pthread_running_thread->errbuf_valid = 1;
#endif

  return -1;
}

