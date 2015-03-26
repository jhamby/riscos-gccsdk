/* Internal UnixLib errno.h
 * Copyright (c) 2000-2014 UnixLib Developers
 */

#ifndef __ERRNO_H
#include_next <errno.h>
#endif

#if !defined(__INTERNAL_ERRNO_H) && defined(__ERRNO_H)
#define __INTERNAL_ERRNO_H

#ifndef __TARGET_SCL__
#  include <pthread.h>
#endif
#include <stdint.h>

/* Macro useful to test for a specific RISC OS filing system error number.  It
   ignores the RISC OS FS number encoded in the error number (see PRM 2-528
   and 2-590):

     &0001nnee with nn the FS number and ee file error number used by FileCore
     based filing systems.

   Note this is useful for non-FileCore based filing systems as well.  */

#define mask_fs_num(errnum) ((errnum) & 0xFFFF00FF)

__BEGIN_DECLS

/* This macro should be used whenever errno is to be set.
   We also give a return value so we can use return __set_errno() which can
   allow function tail calling.
   If errno needs to be set because of a RISC OS error (and we still have a
   ptr to the RISC OS error block, use __ul_seterr() instead.  */

#ifdef __TARGET_SCL__
/* SharedCLibrary */
extern int _stub_errorBuffer;
#  define __set_errno(val) (errno = (val), _stub_errorBuffer = 0, -1)
#else
/* Depending on __UNIXLIB_ERRNO_THREADED, errno is a global variable or an
   alias for __pthread_running_thread->thread_errno.  */
#  define __set_errno(val) (errno = (val), __pthread_running_thread->errbuf_valid = 0, -1)
#endif

extern struct
{
  void *pc;
  int errnum;
  char errmess[252];
} __ul_errbuf;

#ifndef __SOFTFP__
extern struct
{
#ifdef __VFP_FP__
  int fpscr;
  uint64_t d[32];
#else
  int fpsr;
  double f[8];
#endif
} __ul_fp_registers;
#endif

__END_DECLS

#endif
