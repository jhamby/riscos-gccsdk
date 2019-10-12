/* abort.h
 *
 * Copyright 2019 GCCSDK Developers
 * Written by Lee Noar
 */

#ifndef ABORT_H
#define ABORT_H

#include <kernel.h>
#include "armeabisupport.h"
#include "types.h"
#include "link_list.h"

struct _stack_object;
struct _app_object;
struct _armeabisupport_allocator;

#define ABORTOP_REGISTER		0
#define ABORTOP_DEREGISTER		1
#define ABORTOP_INSTALL			2
#define ABORTOP_REMOVE			3

typedef struct abort_handler
{
  link_hdr link;
  void *routine;
  unsigned r12;
} abort_handler;
LINKLIST_ACCESS_FUNCTIONS (abort_handler)

typedef struct abort_block
{
  abort_handler *first_abort;
  struct _stack_object *first_stack;
  struct _app_object *current_app;
  struct _armeabisupport_allocator *stack_allocator;
  bool in_handler;
  void *old_handler;

} abort_block;

extern _kernel_oserror *abort_op(_kernel_swi_regs *r);
extern _kernel_oserror *abort_register(void *routine, unsigned r12);
extern _kernel_oserror *abort_register_internal(app_object *app, void *routine, unsigned r12);
extern _kernel_oserror *abort_deregister(void *routine, unsigned r12);
extern _kernel_oserror *abort_install(void);
extern _kernel_oserror *abort_remove(void);

extern void *abort_get_pre_filter(void);
extern void *abort_get_post_filter(void);
extern abort_block *get_data_abort_support_block(void);

#endif
