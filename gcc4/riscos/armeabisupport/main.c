/* main.c
 *
 * Copyright 2019 GCCSDK Developers
 * Written by Lee Noar
 */

#include <kernel.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include "armeabisupport.h"
#include "types.h"
#include "swi.h"
#include "abort.h"
#include "stack.h"
#include "memory.h"
#define DEBUG_PRINT 1
#define DEBUG_REPORT 1
#include "debug.h"

armeabisupport_globals global;

_kernel_oserror *
app_new(app_object **app_ret)
{
  _kernel_oserror *err;
  unsigned app_base;

  err = get_app_base(&app_base);
  if (err)
    return err;

  app_object *app;

  err = rma_claim(sizeof(*app), (void **)&app);
  if (err)
    return err;

  memset(app, 0, sizeof(*app));
  app->ID = app_base;

  void *handler = get_stack_abort_handler();
  abort_block *block = get_data_abort_support_block();
  abort_register_internal(app, handler, (unsigned)block);

  block->current_app = app;

  linklist_add_to_end(&global.app_list, &app->link);

  *app_ret = app;

  return NULL;
}

_kernel_oserror *
app_find(app_object **app_ret)
{
  _kernel_oserror *err;
  unsigned app_base;

  err = get_app_base(&app_base);
  if (err)
    {
      *app_ret = NULL;
      return armeabisupport_bad_process;
    }

  app_object *app;
  for (app = linklist_first_app_object(&global.app_list);
       app != NULL && app->ID != app_base;
       app = linklist_next_app_object(app))
    /* Empty loop */;

  *app_ret = app;

  if (app == NULL)
    return armeabisupport_bad_process;

  return NULL;
}

void app_remove_by_handle(app_object *app)
{
  if (app)
    {
      void *handler = get_stack_abort_handler();
      abort_block *r12 = get_data_abort_support_block();
      abort_deregister(handler, (unsigned)r12);
      linklist_remove(&global.app_list, &app->link);
      rma_free(app);
    }
}

_kernel_oserror *
armeabi_cleanup(void)
{
  _kernel_oserror *err;
  app_object *app;

  abort_remove();

  if ((err = app_find(&app)) != NULL)
    return err;

  stack_object *stack = linklist_first_stack_object(&app->stack_list);
  while (stack)
    {
      stack_object *next = linklist_next_stack_object(stack);
      stack_free_internal(stack, app);
      stack = next;
    }

  armeabisupport_allocator *allocator = linklist_first_armeabisupport_allocator(&app->allocator_list);
  while (allocator)
    {
      armeabisupport_allocator *next = linklist_next_armeabisupport_allocator(allocator);
      allocator_destroy_internal(allocator);
      allocator = next;
    }

  app_remove_by_handle(app);

  return NULL;
}

void report_swi_error(const char *func, int line, _kernel_oserror *err)
{
  char buffer[256];
  snprintf(buffer, sizeof(buffer) - 1, "%s:%d - %s", func, line, err->errmess);
  report_text(buffer);
}

static const char filename[] = "$.armeabisupport/log";

static void logwrite(const char *buffer)
{
  _kernel_oserror *err;
  unsigned handle;

  err = _swix(OS_Find, _INR(0,1)|_OUT(0),
		       0xC0 | (1 << 3), filename,
		       &handle);
  if (err || handle == 0)
  {
    err = _swix(OS_Find, _INR(0,1)|_OUT(0),
		       0x80 | (1 << 3), filename,
		       &handle);
  }
  if (!err && handle)
  {
    unsigned extent;
    _swix(OS_Args, _INR(0,1)|_OUT(2), 2, handle, &extent);
    _swix(OS_GBPB, _INR(0,4), 1, handle, buffer, strlen(buffer), extent);
    _swix(OS_Find, _INR(0,1), 0, handle);
  }
}

void log_printf(unsigned flags, const char *format, ...)
{
  if (flags)
  {
    char buffer[512];
    char *p = buffer;
    time_t t;
    struct tm *tmp;

    va_list ap;
    va_start (ap, format);

    t = time(NULL);
    tmp = localtime(&t);

    *p++ = '[';
    p += strftime(p, sizeof(buffer) - strlen(buffer) - 1, "%d/%m  %T", tmp);
    *p++ = ']';
    *p++ = ' ';
    *p++ = ':';
    *p++ = ' ';
    *p = '\0';

    p += vsnprintf(p, sizeof(buffer) - strlen(buffer) - 1, format, ap);
    *p++ = '\n';
    *p = '\0';

    if (flags & LOG_PRINT)
      logwrite(buffer);  
    
    if (flags & LOG_REPORT)
      report_text(buffer);
  }
}