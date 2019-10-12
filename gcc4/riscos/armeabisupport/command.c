/* command.c
 *
 * Copyright 2018 GCCSDK Developers
 * Written by Lee Noar
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <kernel.h>
#include "armeabisupport.h"
#include "stack.h"
#include "types.h"
#include "memory.h"
#include "debug.h"

static void
command_info(void)
{
  app_object *app;
  for (app = linklist_first_app_object (&global.app_list);
       app;
       app = linklist_next_app_object (app))
    {
      printf("App %X\n", app->ID);

      armeabisupport_allocator *allocator;
      for (allocator = linklist_first_armeabisupport_allocator(&app->allocator_list);
	   allocator;
	   allocator = linklist_next_armeabisupport_allocator(allocator))
	{
#if DEBUG_ALLOCATOR
	  dump_block_list(allocator);
#else
	  DEBUG_OUTPUT("  Allocator: '%s'", allocator->name);
#endif
	}
      stack_object *stack;
      for (stack = linklist_first_stack_object (&app->stack_list);
	   stack;
	   stack = linklist_next_stack_object (stack))
	{
	  printf("  Stack: %.32s %p -> %p\n", stack->name, stack->base, stack->top);
	}
    }
}

_kernel_oserror *
module_command (const char *arg_string, int argc, int number, void *pw)
{
  _kernel_oserror *err = NULL;

  switch (number)
    {
    case CMD_ARMEABISupport_Info:
      command_info ();
      break;
    }

  return err;
}
