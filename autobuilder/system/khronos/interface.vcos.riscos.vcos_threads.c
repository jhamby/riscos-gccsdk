/* A lot of the code in this file is borrowed from the VCHIQ module, so:
 *
 * Copyright (c) 2012, RISC OS Open Ltd
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of RISC OS Open Ltd nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include "kernel.h"
#include "swis.h"
#include "riscosbits.h"
#include "main.h"

#include "interface/vcos/vcos.h"
#include "interface/vcos/vcos_msgqueue.h"

#define RT_Register       0x575C0
#define RT_Deregister     0x575C1
#define RT_Yield          0x575C2
#define RT_TimedYield     0x575C3
#define RT_ChangePriority 0x575C4
#define RT_ReadInfo       0x575C5

#define RTReadInfo_Handle 0
#define RTReadInfo_Priority 1
#define RTReadInfo_SVCStk 2

#define THREAD_STACK_SIZE 16384

#ifndef VCOS_DEFAULT_STACK_SIZE
#define VCOS_DEFAULT_STACK_SIZE 4096
#endif

/* Flags for init/deinit components */
enum
{
   VCOS_INIT_NAMED_SEM   = (1 << 0),
   VCOS_INIT_PRINTF_LOCK = (1 << 1),
   VCOS_INIT_MAIN_SEM    = (1 << 2),
   VCOS_INIT_MSGQ        = (1 << 3),
   VCOS_INIT_ALL         = 0xffffffff
};

static const int dummy_pollword_1 = 1;
static const int dummy_pollword_0 = 0;

/* A VCOS wrapper for the thread which called vcos_init. */
static VCOS_THREAD_T vcos_thread_main;

static VCOS_THREAD_ATTR_T default_attrs = {
   .ta_stacksz = VCOS_DEFAULT_STACK_SIZE,
};

/** Singleton global lock used for vcos_global_lock/unlock(). */
static VCOS_MUTEX_T lock = { 1, 0 };

static inline uint32_t
ensure_irqs_off(void)
{
  uint32_t res;

  __asm__ volatile ("MRS	r0, CPSR;\n\t"
		    "ORR	r1, r0, #1 << 7;\n\t"
		    "TEQ	r0, r1;\n\t"
		    "MSRNE	CPSR_c, r1;\n\t"
		    "AND	%[result], r0, #1 << 7;\n"
		    : [result] "=r" (res)
		    :
		    : "r0", "r1");
  return res;
}

static inline void
restore_irqs(uint32_t v)
{
  __asm__ volatile ("MRS	r1, CPSR;\n\t"
		    "BIC	r2, r1, #1 << 7;\n\t"
		    "ORR	r2, r2, %[v];\n\t"
		    "TEQ	r1, r2;\n\t"
		    "MSRNE	CPSR_c, r2;\n"
		    :
		    : [v] "r" (v)
		    : "r1", "r2");
}

void vcos_riscos_logging_assert(const char *file, const char *func, unsigned int line, const char *fmt, ...)
{
   va_list ap;
   va_start(ap, fmt);
   fprintf(stderr, "assertion failure:%s:%d:%s():",
           file, line, func);
   vfprintf(stderr, fmt, ap);
   va_end(ap);
   fprintf(stderr, "\n");

#ifdef VCOS_HAVE_BACKTRACK
   if (vcos_backtrace_self)
      vcos_backtrace_self();
#endif
   abort();
}

extern VCOS_STATUS_T vcos_thread_at_exit(void (*pfn)(void*), void *cxt)
{
   int i;
   VCOS_THREAD_T *self = vcos_thread_current();
   if (!self)
   {
      vcos_assert(0);
      return VCOS_EINVAL;
   }
   for (i=0; i<VCOS_MAX_EXIT_HANDLERS; i++)
   {
      if (self->at_exit[i].pfn == NULL)
      {
         self->at_exit[i].pfn = pfn;
         self->at_exit[i].cxt = cxt;
         return VCOS_SUCCESS;
      }
   }
   return VCOS_ENOSPC;
}

/* Threads */

static void thread_wrapper(VCOS_TASK_T t)
{
	/* Initialise t->rt_handle */
	_swix(RT_ReadInfo,_IN(0)|_OUT(0),RTReadInfo_Handle,&t->rt_handle);
	if(!t->stop)
		t->retval = t->thread_function(t->data);
	ensure_irqs_off();
	t->stopped = 1;
	_swix(RT_Deregister,_INR(0,1),0,t->rt_handle);
}

VCOS_TASK_T
kthread_create(int (*func)(void *),void *param,const char *name)
{
	(void) name;
	
	VCOS_TASK_T thread = malloc(sizeof(struct __vcos_task_struct));
	if(!thread)
	{
		return 0;
	}
	memset(thread,0,sizeof(struct __vcos_task_struct));
	_kernel_stack_chunk *stack = malloc(THREAD_STACK_SIZE);
	if(!stack)
	{
		return 0;
	}
	thread->stop = false;
	thread->retval = NULL;
	thread->stack = stack;
	stack->sc_mark = 0xF60690FF;
	stack->sc_size = THREAD_STACK_SIZE;
	memcpy(stack+1, _kernel_current_stack_chunk()+1, 28);

	thread->thread_function = func;
	thread->data = param;

	int irq = ensure_irqs_off();
	_kernel_oserror *e = _swix(RT_Register, _INR(0,7)|_OUT(0), 0, thread_wrapper,
				   thread, globals->private_word, &thread->wakeup,
				   ((int) stack) + 560, ((int) stack) + THREAD_STACK_SIZE,
				   "VCHIQ:128", &thread->rt_handle); /* Normal priority */
	if(e)
	{
		restore_irqs(irq);
		free(stack);
		free(thread);
		return 0;
	}

	struct module_globals *glob = globals;
	list_add(&thread->link, &glob->tasks);
	INIT_LIST_HEAD(&thread->keys);

	restore_irqs(irq);
	return thread;
}

VCOS_TASK_T __attribute__((noinline))
kthread_self(void)
{
  int irq = ensure_irqs_off();

  uint32_t self_handle;
  struct module_globals *glob = globals;
  struct __vcos_list_head *l,*n;
  VCOS_TASK_T self = 0;

  _swix(RT_ReadInfo,_IN(0)|_OUT(0),RTReadInfo_Handle,&self_handle);

  list_for_each_safe(l, n, &glob->tasks) {
    if (list_entry(l, struct __vcos_task_struct, link)->rt_handle == self_handle) {
      self = list_entry(l, struct __vcos_task_struct, link);
      break;
    }
  }

  restore_irqs(irq);

  return self;
}

static VCOS_MUTEX_T once_mutex = { 1, 0 };

int kthread_once(VCOS_ONCE_T *once, void (*ctor)(void))
{
  if (once == NULL || ctor == NULL)
    return EINVAL;

  mutex_lock(&once_mutex);

  if (*once == 0) {
    ctor();
    *once = 1;
  }

  mutex_unlock(&once_mutex);

  return 0;
}

int kthread_key_create(VCOS_TLS_KEY_T *key, void (*dtor)(void *))
{
  static uint32_t next_key = -1;
  int irq = ensure_irqs_off(), ret = 0;
  struct key *k;
  VCOS_TASK_T self = kthread_self();

  if ((k = malloc(sizeof(struct key))) != NULL) {
    list_add(&k->link, &self->keys);
    k->key = next_key++;
    k->value = 0;
    *key = k->key;
    ret = 0;
  }

  restore_irqs(irq);
  
  return ret;
}

void kthread_key_delete(VCOS_TLS_KEY_T key)
{
  int irq = ensure_irqs_off();

  struct __vcos_list_head *l,*n;
  VCOS_TASK_T self = kthread_self();

  list_for_each_safe(l, n, &self->keys) {
    if (list_entry(l, struct key, link)->key == key) {
      list_del(l);
      free(list_entry(l, struct key, link));
    }
  }

  restore_irqs(irq);
}

int kthread_setspecific(VCOS_TLS_KEY_T key, void *value)
{
  int irq = ensure_irqs_off();

  struct __vcos_list_head *l;
  VCOS_TASK_T self = kthread_self();
  int rc = -1;
  
  list_for_each(l, &self->keys) {
    if (list_entry(l, struct key, link)->key == key) {
      list_entry(l, struct key, link)->value = value;
      rc = 0;
      break;
    }
  }

  restore_irqs(irq);

  return rc;
}

void *kthread_getspecific(VCOS_TLS_KEY_T key)
{
  int irq = ensure_irqs_off();

  struct __vcos_list_head *l;
  VCOS_TASK_T self = kthread_self();
  void *rc = NULL;
  
  list_for_each(l, &self->keys) {
    if (list_entry(l, struct key, link)->key == key) {
      rc = list_entry(l, struct key, link)->value;
      break;
    }
  }

  restore_irqs(irq);

  return rc;
}

void msleep(uint32_t ms)
{
  uint32_t csecs = (ms+9)/10;
  _swix(RT_TimedYield,_INR(1,2),&dummy_pollword_0,csecs+_swi(OS_ReadMonotonicTime,_RETURN(0)));
}

VCOS_STATUS_T vcos_thread_create(VCOS_THREAD_T *thread,
                                 const char *name,
                                 VCOS_THREAD_ATTR_T *attrs,
                                 VCOS_THREAD_ENTRY_FN_T entry,
                                 void *arg)
{
   VCOS_STATUS_T st;
//   pthread_attr_t pt_attrs;
//   VCOS_THREAD_ATTR_T *local_attrs = attrs ? attrs : &default_attrs;
   int rc;
   
   vcos_assert(thread);
   memset(thread, 0, sizeof(VCOS_THREAD_T));

//   rc = pthread_attr_init(&pt_attrs);
//   if (rc < 0)
//      return VCOS_ENOMEM;

   st = vcos_semaphore_create(&thread->suspend, NULL, 0);
   if (st != VCOS_SUCCESS)
   {
//      pthread_attr_destroy(&pt_attrs);
      return st;
   }

//   pthread_attr_setstacksize(&pt_attrs, local_attrs->ta_stacksz);
#if VCOS_CAN_SET_STACK_ADDR
//   if (local_attrs->ta_stackaddr)
//   {
//      pthread_attr_setstackaddr(&pt_attrs, local_attrs->ta_stackaddr);
 //  }
#else
//   vcos_demand(local_attrs->ta_stackaddr == 0);
#endif

   /* pthread_attr_setpriority(&pt_attrs, local_attrs->ta_priority); */

//   vcos_assert(local_attrs->ta_stackaddr == 0); /* Not possible */

   thread->entry = entry;
   thread->arg = arg;
//   thread->legacy = local_attrs->legacy;

   strncpy(thread->name, name, sizeof(thread->name));
   thread->name[sizeof(thread->name)-1] = '\0';
   memset(thread->at_exit, 0, sizeof(thread->at_exit));

   thread->thread = kthread_create(entry, arg, name);
   rc = thread->thread ? 0 : -1;
//   rc = pthread_create(&thread->thread, &pt_attrs, vcos_thread_entry, thread);

//   pthread_attr_destroy(&pt_attrs);

   if (rc < 0)
   {
      vcos_semaphore_delete(&thread->suspend);
      return VCOS_ENOMEM;
   }
   else
   {
      return VCOS_SUCCESS;
   }
}

/* Create a per-thread key for faking up vcos access
 * on non-vcos threads.
 */
VCOS_TLS_KEY_T _vcos_thread_current_key;

static VCOS_UNSIGNED _vcos_thread_current_key_created = 0;
static VCOS_ONCE_T current_thread_key_once;  /* init just once */

static void vcos_thread_cleanup(VCOS_THREAD_T *thread)
{
   vcos_semaphore_delete(&thread->suspend);
/*   if (thread->task_timer_created)
   {
      vcos_timer_delete(&thread->task_timer);
   }*/
}

static void vcos_dummy_thread_cleanup(void *cxt)
{
   VCOS_THREAD_T *thread = cxt;
   if (thread->dummy)
   {
      int i;
      /* call termination functions */
      for (i=0; thread->at_exit[i].pfn != NULL; i++)
      {
         thread->at_exit[i].pfn(thread->at_exit[i].cxt);
      }
      vcos_thread_cleanup(thread);
      vcos_free(thread);
   }
}

static void current_thread_key_init(void)
{
   vcos_assert(!_vcos_thread_current_key_created);
   kthread_key_create (&_vcos_thread_current_key, vcos_dummy_thread_cleanup);
   _vcos_thread_current_key_created = 1;
}

void vcos_thread_attr_init(VCOS_THREAD_ATTR_T *attrs)
{
   *attrs = default_attrs;
}

void vcos_thread_join(VCOS_THREAD_T *thread,
                             void **pData)
{
//   pthread_join(thread->thread, pData);
   while (!thread->thread->stopped) {
     _swix(RT_Yield,_IN(1),&dummy_pollword_1);
   }
   vcos_thread_cleanup(thread);
}

VCOS_THREAD_T *vcos_dummy_thread_create(void)
{
   VCOS_STATUS_T st;
   VCOS_THREAD_T *thread_hndl = NULL;
   int rc;

   thread_hndl = (VCOS_THREAD_T *)vcos_malloc(sizeof(VCOS_THREAD_T), NULL);
   vcos_assert(thread_hndl != NULL);

   memset(thread_hndl, 0, sizeof(VCOS_THREAD_T));

   thread_hndl->dummy = 1;
   thread_hndl->thread = kthread_self();

   st = vcos_semaphore_create(&thread_hndl->suspend, NULL, 0);
   if (st != VCOS_SUCCESS)
   {
      vcos_free(thread_hndl);
      return( thread_hndl );
   }

   vcos_once(&current_thread_key_once, current_thread_key_init);

   rc = kthread_setspecific(_vcos_thread_current_key,
                            thread_hndl);
   (void)rc;

   return( thread_hndl );
}

VCOS_STATUS_T vcos_once(VCOS_ONCE_T *once_control,
                        void (*init_routine)(void))
{
   int rc = kthread_once(once_control, init_routine);
   if (rc != 0)
   {
      switch (errno)
      {
      case EINVAL:
         return VCOS_EINVAL;
      default:
         vcos_assert(0);
         return VCOS_EACCESS;
      }
   }
   else
   {
      return VCOS_SUCCESS;
   }
}

/* Mutexes */

void mutex_init(VCOS_MUTEX_T *m)
{
	m->pollword = 1;
}

int mutex_trylock(VCOS_MUTEX_T *m)
{
  return mutex_lock(m);
}

int mutex_lock(VCOS_MUTEX_T *m)
{
	uint32_t rt_handle = _swi(RT_ReadInfo,_IN(0)|_RETURN(0),RTReadInfo_Handle);
	int irqs = ensure_irqs_off();
	while(!m->pollword && (m->rt_handle != rt_handle))
	{
		if(_swix(RT_Yield,_IN(1),&m->pollword))
		{
			restore_irqs(irqs);
			return -1;
		}
	}
	m->pollword = 0;
	m->rt_handle = rt_handle;
	restore_irqs(irqs);
	return 0;
}

void mutex_unlock(VCOS_MUTEX_T *m)
{
	m->pollword = 1;
	/* Wake up any waiting threads */
	_swix(RT_Yield,_IN(1),&dummy_pollword_1);
}

/* Semaphores */

void sem_init(VCOS_SEMAPHORE_T *s,int i)
{
	s->pollword = i;
}

int sem_destroy(VCOS_SEMAPHORE_T *s)
{
  return 0;
}

int sem_wait (VCOS_SEMAPHORE_T *s)
{
  int irqs = ensure_irqs_off();
  if (s->pollword > 0) {
    s->pollword--;
    restore_irqs(irqs);
    return 0;
  }
  
  while (!s->pollword) {
    restore_irqs(irqs);
//    printf(".");
    if(_swix(RT_Yield,_IN(1),&s->pollword)){
//printf("\n");
      return -1;}
    ensure_irqs_off();
  }
  s->pollword--;
  restore_irqs(irqs);
//printf("\n");

  return 0;
}

int sem_trywait (VCOS_SEMAPHORE_T *s)
{
  int irqs = ensure_irqs_off();
  if (s->pollword > 0) {
    s->pollword--;
    restore_irqs(irqs);
    return 0;
  }

  errno = EAGAIN;
  restore_irqs(irqs);

  return -1;
}

int sem_getvalue(VCOS_SEMAPHORE_T *s, int *ret)
{
  int irqs = ensure_irqs_off();
  *ret = s->pollword;
  restore_irqs(irqs);
  return 0;
}

int sem_post(VCOS_SEMAPHORE_T *s)
{
  int irqs = ensure_irqs_off();
  s->pollword++;
  restore_irqs(irqs);
  return 0;
}

int vcos_vsnprintf( char *buf, size_t buflen, const char *fmt, va_list ap )
{
   return vsnprintf( buf, buflen, fmt, ap );
}

int vcos_snprintf(char *buf, size_t buflen, const char *fmt, ...)
{
   int ret;
   va_list ap;
   va_start(ap,fmt);
   ret = vsnprintf(buf, buflen, fmt, ap);
   va_end(ap);
   return ret;
}

int vcos_log_to_file = 0;

static FILE * log_fhandle = NULL;

void vcos_vlog_default_impl(const VCOS_LOG_CAT_T *cat, VCOS_LOG_LEVEL_T _level, const char *fmt, va_list args)
{
   (void)_level;

   if(NULL != log_fhandle)
   {
      if (cat->flags.want_prefix)
         fprintf( log_fhandle, "%s: ", cat->name );
      vfprintf(log_fhandle, fmt, args);
      fputs("\n", log_fhandle);
      fflush(log_fhandle);
   }
}

void _vcos_log_platform_init(void)
{
   if(vcos_log_to_file)
   {
      char log_fname[100];
      snprintf(log_fname, 100, "/var/log/vcos_log%u.txt", vcos_process_id_current());
      log_fhandle = fopen(log_fname, "w");
   }
   else
      log_fhandle = stderr;
}

static void vcos_term(uint32_t flags)
{
   if (flags & VCOS_INIT_MSGQ)
      vcos_msgq_deinit();

   if (flags & VCOS_INIT_MAIN_SEM)
      vcos_semaphore_delete(&vcos_thread_main.suspend);

#ifdef ANDROID
   if (flags & VCOS_INIT_PRINTF_LOCK)
      vcos_mutex_delete(&printf_lock);
#endif

   if (flags & VCOS_INIT_NAMED_SEM)
      _vcos_named_semaphore_deinit();
}

VCOS_STATUS_T vcos_platform_init(void)
{
   VCOS_STATUS_T st;
   uint32_t flags = 0;
   int pst;

   st = _vcos_named_semaphore_init();
   if (!vcos_verify(st == VCOS_SUCCESS))
      goto end;

   flags |= VCOS_INIT_NAMED_SEM;

#ifdef HAVE_MTRACE
   /* enable glibc memory debugging, if the environment
    * variable MALLOC_TRACE names a valid file.
    */
   mtrace();
#endif

#ifdef ANDROID
   st = vcos_mutex_create(&printf_lock, "printf");
   if (!vcos_verify(st == VCOS_SUCCESS))
      goto end;

   flags |= VCOS_INIT_PRINTF_LOCK;
#endif

   st = vcos_once(&current_thread_key_once, current_thread_key_init);
   if (!vcos_verify(st == VCOS_SUCCESS))
      goto end;

   /* Initialise a VCOS wrapper for the thread which called vcos_init. */
   st = vcos_semaphore_create(&vcos_thread_main.suspend, NULL, 0);
   if (!vcos_verify(st == VCOS_SUCCESS))
      goto end;

   flags |= VCOS_INIT_MAIN_SEM;

   vcos_thread_main.thread = kthread_self();

   pst = kthread_setspecific(_vcos_thread_current_key, &vcos_thread_main);
   if (!vcos_verify(pst == 0))
   {
      st = VCOS_EINVAL;
      goto end;
   }

   st = vcos_msgq_init();
   if (!vcos_verify(st == VCOS_SUCCESS))
      goto end;

   flags |= VCOS_INIT_MSGQ;

   vcos_logging_init();

end:
   if (st != VCOS_SUCCESS)
      vcos_term(flags);

   return st;
}

void vcos_platform_deinit(void)
{
   vcos_term(VCOS_INIT_ALL);
}

void vcos_global_lock(void)
{
   mutex_lock(&lock);
}

void vcos_global_unlock(void)
{
   mutex_unlock(&lock);
}
