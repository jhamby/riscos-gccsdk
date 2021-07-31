/* init-fini.c
 *
 * Copyright 2019-2021 GCCSDK Developers
 * Written by Lee Noar
 */

#include <stdlib.h>
#include <kernel.h>
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "swi.h"
#include "abort.h"
#include "memory.h"
#define DEBUG_PRINT 0
#include "debug.h"

static const char filter_name[] = "ARMEABISupport";

static _kernel_oserror *install_pre_filter(void)
{
  if (global.pre_filter_installed)
    return NULL;

  _kernel_oserror *err;

  if ((err = filter_register_pre_filter(filter_name,
				   abort_get_pre_filter(),
				   &global,
				   0)) == NULL)
    global.pre_filter_installed = true;

  return err;
}

static _kernel_oserror *remove_pre_filter (void)
{
  if (!global.pre_filter_installed)
    return NULL;

  _kernel_oserror *err;

  if ((err = filter_deregister_pre_filter (filter_name,
					   abort_get_pre_filter(),
					   &global,
					   0)) == NULL)
    global.pre_filter_installed = false;

  return err;
}

static _kernel_oserror *install_post_filter (void)
{
  if (global.post_filter_installed)
    return NULL;

  _kernel_oserror *err;

  if ((err = filter_register_post_filter (filter_name,
					  abort_get_post_filter(),
					  &global, 0)) == NULL)
    global.post_filter_installed = true;

  return err;
}

static _kernel_oserror *remove_post_filter (void)
{
  if (!global.post_filter_installed)
    return NULL;

  _kernel_oserror *err;

  if ((err = filter_deregister_post_filter (filter_name,
					    abort_get_post_filter(),
					    &global, 0)) == NULL)
    global.post_filter_installed = false;

  return err;
}

_kernel_oserror *module_initialise (const char *tail, int podule_base, void *pw)
{
  _kernel_oserror *err;

  memset (&global, 0, sizeof (armeabisupport_globals));

  // linklist_init_list (&global.app_list);
  // global.flags.no_client_check = false;

  if ((err = install_pre_filter()) != NULL
   || (err = install_post_filter()) != NULL)
    goto error;

  get_os_permissions(OS_PERMISSION_FLAG_USER_READ,
		     OS_PERMISSION_FLAG_USER_READ | OS_PERMISSION_FLAG_USER_WRITE | OS_PERMISSION_FLAG_USER_EXEC,
		     &global.os_memory_prot_read);
  get_os_permissions(OS_PERMISSION_FLAG_USER_READ | OS_PERMISSION_FLAG_USER_WRITE,
		     OS_PERMISSION_FLAG_USER_READ | OS_PERMISSION_FLAG_USER_WRITE | OS_PERMISSION_FLAG_USER_EXEC,
		     &global.os_memory_prot_read_write);
  get_os_permissions(OS_PERMISSION_FLAG_USER_EXEC,
		     OS_PERMISSION_FLAG_USER_READ | OS_PERMISSION_FLAG_USER_WRITE | OS_PERMISSION_FLAG_USER_EXEC,
		     &global.os_memory_prot_exec);
  get_os_permissions(OS_PERMISSION_FLAG_PRIV_READ,
		     OS_PERMISSION_FLAG_PRIV_READ | OS_PERMISSION_FLAG_PRIV_WRITE | OS_PERMISSION_FLAG_PRIV_EXEC |
		     OS_PERMISSION_FLAG_USER_READ | OS_PERMISSION_FLAG_USER_WRITE | OS_PERMISSION_FLAG_USER_EXEC,
		     &global.os_memory_prot_none);
  return NULL;

error:
  remove_pre_filter();
  remove_post_filter();

  return err;
}

_kernel_oserror *module_finalisation (int fatal, int podule_base, void *pw)
{
#if !DEBUG_ALLOCATOR
  if (global.app_list.count > 0)
    return armeabisupport_in_use;
#endif

  allocator_destroy_internal(global.stack_allocator);

  remove_pre_filter();
  remove_post_filter();

  return NULL;
}
