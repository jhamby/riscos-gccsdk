/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pthread/rwlockattr.c,v $
 * $Date: 2002/12/15 13:16:55 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

/* Read/write lock attribute manipulation routines */

/* Written by Alex Waugh */

#include <errno.h>
#include <pthread.h>

/* Initialise an attributes object */
int
pthread_rwlockattr_init (pthread_rwlockattr_t *attr)
{
  if (attr == NULL)
    return EINVAL;

  attr->pshared = PTHREAD_PROCESS_PRIVATE;
  attr->type = PTHREAD_MUTEX_DEFAULT;

  return 0;
}

/* Destroy an attributes object */
int
pthread_rwlockattr_destroy (pthread_rwlockattr_t *attr)
{
  if (attr == NULL)
    return EINVAL;

  return 0;
}

/* Get the process shared attribute */
int
pthread_rwlockattr_getpshared (const pthread_rwlockattr_t *attr, int *pshared)
{
  if (attr == NULL || pshared == NULL)
    return EINVAL;

  *pshared = attr->pshared;

  return 0;
}

/* Set the process shared attribute (Ignored under RISC OS) */
int
pthread_rwlockattr_setpshared (pthread_rwlockattr_t *attr, int pshared)
{
  if (attr == NULL)
    return EINVAL;

  if (pshared != PTHREAD_PROCESS_PRIVATE && pshared != PTHREAD_PROCESS_SHARED)
    return EINVAL;

  attr->pshared = pshared;

  return 0;
}
