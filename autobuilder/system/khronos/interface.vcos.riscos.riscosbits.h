/*
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

#ifndef RISCOSBITS_H
#define RISCOSBITS_H

/* Macros and other stuff required to make the new, less-portable VCHIQ work on RISC OS */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "kernel.h"

#define __init
#define __exit
#define __attribute__(X)
#define kzalloc(X,Y) malloc(X)
#define kmalloc(X,Y) malloc(X)
#define kfree free
#define EXPORT_SYMBOL(X) static char DUMMY_##X
#define flush_signals(X) (void) 0
#define ATOMIC_INIT(X) {(X)}
#define DEFINE_SPINLOCK(X) spinlock_t X
#define min(A,B) ((A)<(B)?(A):(B))
#define unlikely(X) X
#define set_user_nice(X,Y) (void) 0
#define fatal_signal_pending(X) 0

#define INIT_LIST_HEAD(HEAD) do { (HEAD)->next = (HEAD)->prev = HEAD; } while(0)
#define list_for_each_safe(POS,NEXT,HEAD) for((POS)=(HEAD)->next,(NEXT)=(POS)->next;(POS) != (HEAD);(POS)=(NEXT),(NEXT)=(POS)->next)
#define list_for_each(POS,HEAD) for((POS)=(HEAD)->next;(POS) != (HEAD);(POS)=(POS)->next)
#define list_entry(ITEM,TYPE,MEMBER) ((TYPE *) (((int)(ITEM)) - offsetof(TYPE,MEMBER)))
#define list_del(POS) do { (POS)->next->prev = (POS)->prev; (POS)->prev->next = (POS)->next; (POS)->next = (POS)->prev = (POS); } while(0)
#define list_add(ITEM,HEAD) do { (ITEM)->next = (HEAD)->next; (ITEM)->prev = (HEAD); (HEAD)->next = (ITEM); (ITEM)->next->prev = (ITEM); } while(0)

#ifdef DEBUGLIB
extern void vcos_log_impl(const char *fmt, ...);

#ifndef vchiq_log_error
#define vchiq_log_error(cat, ...) \
	do { if (cat >= VCHIQ_LOG_ERROR) \
		vcos_log_impl(__VA_ARGS__); } while (0)
#endif
#ifndef vchiq_log_warning
#define vchiq_log_warning(cat, ...) \
	do { if (cat >= VCHIQ_LOG_WARNING) \
		vcos_log_impl(__VA_ARGS__); } while (0)
#endif
#ifndef vchiq_log_info
#define vchiq_log_info(cat, ...) \
	do { if (cat >= VCHIQ_LOG_INFO) \
		vcos_log_impl(__VA_ARGS__); } while (0)
#endif
#ifndef vchiq_log_trace
#define vchiq_log_trace(cat, ...) \
	do { if (cat >= VCHIQ_LOG_TRACE) \
		vcos_log_impl(__VA_ARGS__); } while (0)
#endif

#define BUG() printk("BUG @ %s line %d",__FILE__,__LINE__)
#define BUG_ON(X) if(X) { printk("BUG: %s @ %s line %d",#X,__FILE__,__LINE__); }
#define WARN_ON(X) if(X) { printk("WARNING: %s @ %s line %d",#X,__FILE__,__LINE__); }
#define WARN_ON_ONCE(X) do { static bool warned = false; if(!warned && (X)) { warned = true; printk("WARNING: %s @ %s line %d",#X,__FILE__,__LINE__); } } while(0)
#define WARN(X,Y) if(X) { printk("WARNING: %s - %s @ %s line %d",#X,Y,__FILE__,__LINE__); }

extern void printk(const char *fmt,...);

#else

#ifndef vchiq_log_error
#define vchiq_log_error(cat, ...) (void) 0
#endif
#ifndef vchiq_log_warning
#define vchiq_log_warning(cat, ...) (void) 0
#endif
#ifndef vchiq_log_info
#define vchiq_log_info(cat, ...) (void) 0
#endif
#ifndef vchiq_log_trace
#define vchiq_log_trace(cat, ...) (void) 0
#endif

#define BUG() (void) 0
#define BUG_ON(X) (void) 0
#define WARN_ON(X) (void) 0
#define WARN_ON_ONCE(X) (void) 0
#define WARN(X,Y) (void) 0
#define printk(...) (void) 0

#endif

struct list_head {
	struct list_head *next;
	struct list_head *prev;
};

struct semaphore {
	uint32_t pollword;
};

struct mutex {
	uint32_t pollword; /* 1 if free, 0 if claimed */
	uint32_t rt_handle; /* Current owner. Not really needed, but might be useful for debugging? */
};

struct key {
    struct list_head link;
    uint32_t key;
    uint32_t value;
};

struct thread_cond {
	uint32_t cond;
};

struct completion {
	int dummy;
};

struct timer_list {
	int dummy;
};

struct task_struct {
	uint32_t rt_handle;
	bool stop;
	int retval;
	uint32_t stopped;
	_kernel_stack_chunk *stack;
	uint32_t wakeup;

	int (*thread_function)(void *);
	void *data;

	struct list_head keys;

	struct list_head link;
};

struct current_thing {
	int pid;
};

typedef struct { int32_t counter; } atomic_t;
typedef uint32_t spinlock_t;
typedef uint32_t rwlock_t;

/* Should really be a pointer to the current task_struct, but since we pretend everything is the same process it doesn't really matter */
extern struct current_thing *current;

extern void mutex_init(struct mutex *m);
extern int mutex_lock_interruptible(struct mutex *m);
extern int mutex_lock(struct mutex *m);
extern int mutex_trylock(struct mutex *m);
extern void mutex_unlock(struct mutex *m);

extern void sem_init(struct semaphore *s,int i);
extern int sem_destroy(struct semaphore *s);
extern int sem_wait(struct semaphore *s);
extern int sem_trywait(struct semaphore *s);
extern int sem_post(struct semaphore *s);
extern int sem_getvalue(struct semaphore *s,int *ret);

extern void msleep(uint32_t ms);
extern void udelay(uint32_t us);

extern void spin_lock(spinlock_t *s);
extern void spin_unlock(spinlock_t *s);

extern void dsb(void);
extern void wmb(void);
extern void rmb(void);

extern int32_t atomic_read(atomic_t *a);
extern void atomic_set(atomic_t *a, int32_t i);
extern int32_t atomic_inc_return(atomic_t *a);
extern int32_t atomic_dec_return(atomic_t *a);
extern int32_t atomic_xchg(atomic_t *a, int32_t i);
extern int32_t atomic_cmpxchg(atomic_t *a, int32_t old, int32_t new_);

extern struct task_struct *kthread_create(int (*func)(void *),void *param,const char *name);
extern int kthread_key_create(uint32_t *, void (*)(void *));
extern void kthread_key_delete(uint32_t);
int kthread_setspecific(uint32_t key, void *value);
void *kthread_getspecific(uint32_t key);
extern struct task_struct *kthread_self(void);
extern void wake_up_process(struct task_struct *t);

#endif
