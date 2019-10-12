/* stack.h
 *
 * Copyright 2019 GCCSDK Developers
 * Written by Lee Noar
 */

#ifndef STACK_H
#define STACK_H

#include "link_list.h"
#include "types.h"

#define STACK_DA_MAX_SIZE	(1024 * 1024 * 512) /* 512MB */
#define STACK_NAME_MAX_LEN	32

#define STACKOP_ALLOC		0
#define STACKOP_FREE		1
#define STACKOP_GET_STACK	2
#define STACKOP_GET_BOUNDS	3
#define STACKOP_GET_SIZE	4

typedef struct _stack_object
{
  link_hdr link;
  eabi_PTR base;
  eabi_PTR top;
  unsigned size;			/* In pages.  */
  unsigned guard_size;			/* In pages.  */
  char name[STACK_NAME_MAX_LEN];
} stack_object;
LINKLIST_ACCESS_FUNCTIONS (stack_object)

extern _kernel_oserror *stack_op(_kernel_swi_regs * r);

extern _kernel_oserror *
stack_alloc(unsigned size, unsigned num_guard_pages, const char *name,
	    stack_object **handle_ret, eabi_PTR *base_ret, eabi_PTR *top_ret);
extern _kernel_oserror *
stack_free(stack_object *stack);
extern _kernel_oserror *stack_free_internal(stack_object *stack, app_object *app);
extern stack_object *find_stack(link_list *stack_list, eabi_PTR addr);
extern _kernel_oserror *stack_get_stack(eabi_PTR addr, stack_object **stack_ret);
extern void *get_stack_abort_handler(void);

#endif
