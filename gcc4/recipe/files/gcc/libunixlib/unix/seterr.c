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

/* err points to RISC OS error block or is NULL.
   seterrno is non-zero when the thread's errno value needs to be set or zero
   when it should not be set.
   Returns -1 when err is non-NULL, 0 otherwise.  */
int
__ul_seterr (const _kernel_oserror *err, int seterrno)
{
  if (err == NULL)
    return 0;

  if (seterrno)
    __set_errno (EOPSYS);

#ifdef __TARGET_SCL__
  _stub_errorNumber.errnum = err->errnum;
  char *dst = _stub_errorNumber.errmess;
#else
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

