/* Internal UnixLib errno.h
 * Copyright (c) 2000-2010 UnixLib Developers
 */

#ifndef __ERRNO_H
#include_next <errno.h>
#endif

#if !defined(__INTERNAL_ERRNO_H) && defined(__ERRNO_H)
#define __INTERNAL_ERRNO_H

__BEGIN_DECLS

/* This macro should be used whenever errno is to be set. This permits
   us to easily make setting the errno call a function if necessary if
   threads ever appear.  We also give a return value so we can use
   return __set_errno () which can allow function tail calling.  */

#if __UNIXLIB_ERRNO_THREADED
#define __set_errno(val) (__pthread_running_thread->thread_errno = (val), -1)
#else
#define __set_errno(val) (errno = (val), -1)
#endif

extern struct
{
  void *pc;
  int errnum;
  char errmess[252];
  int valid;
} __ul_errbuf;

#ifndef __SOFTFP__
extern struct
{
  int fpsr;
  double f[8];
} __ul_fp_registers;
#endif

__END_DECLS

#endif
