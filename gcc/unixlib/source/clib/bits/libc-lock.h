/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/bits/libc-lock.h,v $
 * $Date: 2003/04/28 21:04:35 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#ifndef _BITS_LIBC_LOCK_H
#define _BITS_LIBC_LOCK_H 1

#include <pthread.h>


#if __UNIXLIB_FEATURE_PTHREADS

#define __libc_lock_define_initialized(class, name) \
class pthread_mutex_t name = PTHREAD_MUTEX_INITIALIZER;
#define __libc_lock_lock(x) pthread_mutex_lock(&x)
#define __libc_lock_unlock(x) pthread_mutex_unlock(&x)

#else

#define __libc_lock_define_initialized(class, name)
#define __libc_lock_lock(x)
#define __libc_lock_unlock(x)

#endif


#endif
