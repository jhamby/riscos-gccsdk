/* POSIX Semaphores.
   Copyright (c) 2004-2008 UnixLib Developers.  */

/*-
 * Copyright (c) 2003 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Jason R. Thorpe of Wasabi Systems, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *        This product includes software developed by the NetBSD
 *        Foundation, Inc. and its contributors.
 * 4. Neither the name of The NetBSD Foundation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Copyright (C) 2000 Jason Evans <jasone@freebsd.org>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice(s), this list of conditions and the following disclaimer as
 *    the first lines of this file unmodified other than the possible
 *    addition of one or more copyright notices.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice(s), this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER(S) BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* #define PTHREAD_DEBUG_SEMS */

#include <sys/cdefs.h>

#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdarg.h>

#include <pthread.h>
#include <internal/unix.h>

#ifdef PTHREAD_DEBUG_SEMS
#include <internal/os.h>
#endif

struct pthread_queue_t
{
  pthread_t thread; /* this thread */
  struct pthread_queue_t *next;    /* next element */
};

#define	USEM_MAGIC	0x09fa4012
#define	USEM_USER	0	/* assumes kernel does not use NULL */

/* Check validity of semaphore structure, return 0 if valid
   and non-zero if invalid.  */
static __inline int sem_check_validity (const sem_t *sem)
{
  if (sem != NULL && sem->usem_magic == USEM_MAGIC)
    return 0;

  return __set_errno (EINVAL);
}

static __inline void sem_free (sem_t *sem)
{
#ifdef PTHREAD_DEBUG_SEMS
  __os_print("-- sem_free: Freeing semaphore (");
  __os_prhex((int)sem);
  __os_print(")\r\n");
#endif
  sem->usem_magic = 0;
}

/* Initialise a semaphore structure.  User has already passed a valid
   piece of memory in 'sem'.  */
int sem_init (sem_t *sem, int pshared, unsigned int value)
{
  semid_t semid = USEM_USER;

  /* Semaphores shared between processes aren't supported on RISC OS */
  if (pshared)
    return __set_errno (ENOSYS);

  if (value > SEM_VALUE_MAX)
    return __set_errno (EINVAL);

#ifdef PTHREAD_DEBUG_SEMS
  __os_print("-- sem_alloc: Allocating semaphore (");
  __os_prhex((int)sem);
  __os_print(" id: ");
  __os_prhex((int)semid);
  __os_print(" value: ");
  __os_prdec((int)value);
  __os_print(")\r\n");
#endif
  sem->usem_magic = USEM_MAGIC;
  pthread_rwlock_init(&sem->usem_interlock, NULL);
  /*PTQ_INIT(&sem->usem_waiters);*/
  sem->usem_waiters = NULL;
  sem->usem_count = value;
  sem->usem_semid = semid;
  return 0;
}

int
sem_destroy(sem_t *sem)
{
  if (sem_check_validity (sem))
    return -1;

#ifdef PTHREAD_DEBUG_SEMS
  __os_print("-- sem_destroy: Attempting to destroy semaphore (");
  __os_prhex((int)sem);
  __os_print(")..........");
#endif

  pthread_rwlock_rdlock(&sem->usem_interlock);
  if (/*!PTQ_EMPTY(&(*sem)->usem_waiters)*/
      sem->usem_waiters != NULL)
    {
      pthread_rwlock_unlock(&sem->usem_interlock);
#ifdef PTHREAD_DEBUG_SEMS
      __os_print("failed\r\n");
#endif
      return __set_errno (EBUSY);
    }
  pthread_rwlock_unlock(&sem->usem_interlock);

#ifdef PTHREAD_DEBUG_SEMS
  __os_print("succeeded\r\n");
#endif

  sem_free (sem);
  return 0;
}

sem_t *
sem_open(const char *name, int oflag, ...)
{
  /* not supported under RISC OS */
  __set_errno (ENOSYS);
  return SEM_FAILED;
}

int
sem_close(sem_t *sem)
{
  /* not supported under RISC OS */
  return __set_errno (ENOSYS);
}

int
sem_unlink(const char *name)
{
  /* not supported under RISC OS */
  errno = ENOSYS;
  return (-1);
}

int sem_wait (sem_t *sem)
{
  pthread_t self;
  struct pthread_queue_t *queue_entry, *tmp;

  if (sem_check_validity (sem))
    return -1;

  PTHREAD_SAFE_CANCELLATION

  self = __pthread_running_thread;
  queue_entry = malloc(sizeof(struct pthread_queue_t));
  queue_entry->thread = self;
  queue_entry->next = NULL;

#ifdef PTHREAD_DEBUG_SEMS
  __os_print("-- sem_wait: waiting on semaphore (");
  __os_prhex((int)sem);
  __os_print(")\r\n");
#endif

  for (;;)
    {
      pthread_rwlock_wrlock(&sem->usem_interlock);
      __pthread_disable_ints();
      /* pthread_spinlock(self, &self->pt_statelock); */
      if (self->cancelpending)
	{
	  /* pthread_spinunlock(self, &self->pt_statelock); */
	  __pthread_enable_ints();
	  pthread_rwlock_unlock(&sem->usem_interlock);
	  pthread_exit(PTHREAD_CANCELED);
	}

      if (sem->usem_count > 0)
	{
	  /* pthread_spinunlock(self, &self->pt_statelock); */
	  __pthread_enable_ints();
	  sem->usem_count--;
	  pthread_rwlock_unlock(&sem->usem_interlock);
	  return 0;
	}

      if (sem->usem_waiters)
	{
	  /* PTQ_INSERT_TAIL(&(*sem)->usem_waiters, self, pt_sleep); */
	  for (tmp = sem->usem_waiters;
	       tmp->next && tmp != queue_entry; tmp = tmp->next)
	    ;
	  if (tmp != queue_entry)
	    tmp->next = queue_entry;
      
	  self->state = STATE_BLOCKED;
	  /* self->pt_sleepobj = *sem;
	     self->pt_sleepq = &(*sem)->usem_waiters;
	     self->pt_sleeplock = &(*sem)->usem_interlock;
	     pthread_spinunlock(self, &self->pt_statelock); */
	}

      __pthread_enable_ints();
      /* XXX What about signals? */

      pthread_rwlock_unlock(&sem->usem_interlock);

      /* pthread__block(self, &(*sem)->usem_interlock); */
      /* interlock is not held when we return */
      pthread_yield();
    }
  return 0;
}

int
sem_trywait(sem_t *sem)
{
  int retval = 0;

  if (sem_check_validity (sem))
    return -1;

#ifdef PTHREAD_DEBUG_SEMS
  __os_print("-- sem_trywait: trying to acquire semaphore (");
  __os_prhex((int)sem);
  __os_print(")..........");
#endif

  pthread_rwlock_wrlock(&sem->usem_interlock);

  if (sem->usem_count == 0)
    {
      errno = EAGAIN;
      retval = -1;
    }
  else
    {
      sem->usem_count --;
    }

  pthread_rwlock_unlock(&sem->usem_interlock);
#ifdef PTHREAD_DEBUG_SEMS
  __os_print ((retval == 0) ? "succeeded\r\n" : "failed\r\n");
#endif
  return retval;
}

int
sem_post(sem_t *sem)
{
  pthread_t self, blocked;
  struct pthread_queue_t *tmp;

  if (sem_check_validity (sem))
    return -1;

  self = __pthread_running_thread;

#ifdef PTHREAD_DEBUG_SEMS
  __os_print("-- sem_post: releasing semaphore (");
  __os_prhex((int)sem);
  __os_print(")\r\n");
#endif

  pthread_rwlock_wrlock(&sem->usem_interlock);
  sem->usem_count++;

  if (sem->usem_waiters != NULL)
    blocked = sem->usem_waiters->thread;
  else
    blocked = NULL;
  if (blocked)
    {
      /* PTQ_REMOVE(&(*sem)->usem_waiters, blocked, pt_sleep); */
      tmp = sem->usem_waiters;
      sem->usem_waiters = sem->usem_waiters->next;
      tmp->thread = tmp->thread = NULL;
      free(tmp);

      /* Give the head of the blocked queue another try. */
      /* pthread__sched(self, blocked); */
      __pthread_disable_ints();
      blocked->state = STATE_RUNNING;
      __pthread_enable_ints();
    }
  pthread_rwlock_unlock(&sem->usem_interlock);
  pthread_yield();
  return 0;
}

int
sem_getvalue(sem_t * __restrict sem, int * __restrict sval)
{
  if (sem_check_validity (sem))
    return -1;

#ifdef PTHREAD_DEBUG_SEMS
  __os_print("-- sem_getvalue: Retrieving value of semaphore\r\n");
#endif

  pthread_rwlock_rdlock(&sem->usem_interlock);
  *sval = (int) sem->usem_count;
  pthread_rwlock_unlock(&sem->usem_interlock);

  return 0;
}

int
sem_timedwait (sem_t *sem, const struct timespec *abstime)
{
#ifdef PTHREAD_DEBUG_SEMS
  __os_print(" -- sem_timedwait: Not supported\r\n");
#endif

  return __set_errno (ENOSYS);
}
