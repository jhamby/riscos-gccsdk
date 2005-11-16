/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pthread/condattr.c,v $
 * $Date: 2002/12/15 13:16:55 $
 * $Revision: 1.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

/* Condition variable attributes manipulation */

/* Written by Alex Waugh */

#include <errno.h>
#include <pthread.h>

/* Initialise a condition variable attributes object */
int
pthread_condattr_init (pthread_condattr_t *attr)
{
  if (attr == NULL)
    return EINVAL;

  attr->pshared = PTHREAD_PROCESS_PRIVATE;

  return 0;
}

/* Destroy an attributes object */
int
pthread_condattr_destroy (pthread_condattr_t *attr)
{
  if (attr == NULL)
    return EINVAL;

  return 0;
}

/* Get process shared attribute */
int
pthread_condattr_getpshared (const pthread_condattr_t *attr, int *pshared)
{
  if (attr == NULL || pshared == NULL)
    return EINVAL;

  *pshared = attr->pshared;

  return 0;
}

/* Set process shared attribute (ignored under RISC OS) */
int
pthread_condattr_setpshared (pthread_condattr_t *attr, int pshared)
{
  if (attr == NULL)
    return EINVAL;

  if (pshared != PTHREAD_PROCESS_PRIVATE && pshared != PTHREAD_PROCESS_SHARED)
    return EINVAL;

  attr->pshared = pshared;

  return 0;
}
