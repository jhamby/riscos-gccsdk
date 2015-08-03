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

/* Types and declarations that are referenced in the public VCOS headers. These
 * are not meant for use in user code.  */

#include <stdint.h>
#include <stdbool.h>
#include <kernel.h>
#include <stdio.h>
#include <stdlib.h>

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

struct thread_cond {
	uint32_t cond;
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

extern struct task_struct *kthread_create(int (*func)(void *),void *param,const char *name);
extern int kthread_key_create(uint32_t *, void (*)(void *));
extern void kthread_key_delete(uint32_t);
int kthread_setspecific(uint32_t key, void *value);
void *kthread_getspecific(uint32_t key);
extern struct task_struct *kthread_self(void);

#endif
