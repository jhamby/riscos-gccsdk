/****************************************************************************
 *
 * $Source: $
 * $Date: $
 * $Revision: $
 * $State: $
 * $Author: $
 *
 ***************************************************************************/

#ifndef _BITS_LIBC_LOCK_H
#define _BITS_LIBC_LOCK_H 1

#include <pthread.h>


#if __FEATURE_PTHREADS

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
