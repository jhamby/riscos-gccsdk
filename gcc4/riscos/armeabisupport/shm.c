/* shm.c
 *
 * Copyright 2021 GCCSDK Developers
 * Written by Lee Noar
 */

#include "armeabisupport.h"
#include "shm.h"
#include "memory.h"
#include "swi.h"
#include "link_list.h"
#include "types.h"
#include "debug.h"
#include <stddef.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>
#include <limits.h>

#define SHMOP_OPEN	0
#define SHMOP_UNLINK	1
#define SHMOP_CLOSE	2

#define LOGGING 0
#define TRACING 0

#if LOGGING
#define LOG(format,...) reporter_printf(format, ##__VA_ARGS__)
#else
#define LOG(format,...)
#endif

#if TRACING
#define TRACE(format,...) reporter_printf(format, ##__VA_ARGS__)
#else
#define TRACE(format,...)
#endif

/* r0 = reason code:
*	0 - shm_open
*	  Entry:
*	    r1 = name (beginning with a '/' and upto 256 chars)
*	    r2 = flags
*	    r3 = mode
*	  Exit:
*	    r0 = handle (not guarenteed to be lowest fd in current process) if success (V flag
*			 clear) or error pointer (V flag set).
*	1 - shm_unlink
*	  Entry:
*	    r1 = name
*	  Exit:
*	    r0 = 0 for success (V flag clear) or error pointer (V flag set).
*	2 - close
*	  Entry:
*	    r1 = fd
*	  Exit:
*	    r0 = 0 for success (V flag clear) or error pointer (V flag set).
*/

static _kernel_oserror *shm_open(const char *name, int flags, int mode, shm_object **result);
static _kernel_oserror *shm_unlink(const char *name);
static _kernel_oserror *shm_close(shm_object *obj);

_kernel_oserror *
shm_op(_kernel_swi_regs *r)
{
  _kernel_oserror *err = NULL;
  int reason = r->r[0];

  switch (reason)
  {
  case SHMOP_OPEN: {
    shm_object *result;
    err = shm_open((const char *)r->r[1], r->r[2], r->r[3], &result);
    if (!err)
      r->r[0] = (int)result;
    break;
  }
  case SHMOP_UNLINK:
    err = shm_unlink((const char *)r->r[1]);
    break;
  case SHMOP_CLOSE:
    err = shm_close((shm_object *)r->r[1]);
    break;
  }

  return err;
}

void
shm_dump_object_list(void)
{
  shm_object *shm;
  for (shm = linklist_first_shm_object(&global.shm_object_list);
       shm;
       shm = linklist_next_shm_object(shm))
    {
      printf("  %p \'%s\', linked=%d, ref=%d",
	     shm, shm->name, shm->linked, shm->ref_count);
      if (shm->block)
	printf(", addr=%p, size=0x%X\n",
	       page_to_addr(&shm->allocator->base, shm->block->start_page),
	       pages_to_bytes(shm->block->end_page - shm->block->start_page));
      else
	printf(" unallocated\n");
    }
}

static void
report_shm_object_list(void)
{
  shm_object *shm;
  for (shm = linklist_first_shm_object(&global.shm_object_list);
       shm;
       shm = linklist_next_shm_object(shm))
    {
      LOG("  %p \'%s\', linked=%d, ref=%d", shm, shm->name, shm->linked, shm->ref_count);
    }
}

static shm_object *
find_shm_object(const char *name)
{
  shm_object *shm;
  size_t name_len = strlen(name);
  for (shm = linklist_first_shm_object(&global.shm_object_list);
       shm && (!shm->linked || strncmp(name, shm->name, name_len));
       shm = linklist_next_shm_object(shm))
    /* Empty loop */;

  return shm;
}

static _kernel_oserror *
create_shm_object(const char *name, shm_object **result)
{
  _kernel_oserror *err;
  shm_object *shm = NULL;

  if ((err = rma_claim(sizeof(shm_object), (void **)&shm)) != NULL)
    goto error;
  memset(shm, 0, sizeof(*shm));

  if ((err = rma_claim(strlen(name) + 1, (void **)&shm->name)) != NULL)
    goto error;

  strcpy(shm->name, name);
  shm->valid = VALID_SHM;
  shm->fd_type = FD_TYPE_SHM;
  shm->ref_count = 2;
  shm->linked = true;

  linklist_add_to_end(&global.shm_object_list, &shm->link);

  LOG("Created shared memory object \"%s\" @ node %p", name, shm);

  *result = shm;

  return NULL;

error:
  if (shm)
    {
      if (shm->name)
	rma_free(shm->name);
      rma_free(shm);
    }

  LOG("create_shm_object: %s", err->errmess);

  return err;
}

static _kernel_oserror *
shm_open(const char *name, int oflag, int mode, shm_object **result)
{
  _kernel_oserror *err;
  shm_object *obj;

  TRACE("shm_open(\"%s\", %X, %X)", name, oflag, mode);

  if (strlen(name) > PATH_MAX)
    {
      LOG("shm_open: \"%s\" name too long", name);
      return armeabisupport_ENAMETOOLONG;
    }

  obj = find_shm_object(name);

  /* Fail if O_CREAT is not set and the named shared memory object does not exist.  */
  if (!obj && !(oflag & O_CREAT))
    {
      LOG("shm_open: \"%s\" does not exist and O_CREATE not set", name);
      return armeabisupport_ENOENT;
    }

  /* Fail if O_CREAT and O_EXCL are set and the named shared memory object already exists.  */
  if (obj && (oflag & (O_CREAT | O_EXCL)))
    {
      LOG("shm_open: \"%s\" already exists, but O_CREAT and O_EXCL are set", name);
      return armeabisupport_EEXIST;
    }

  if (obj)
    {
      LOG("Found existing shared memory object \"%s\"", name);
      obj->ref_count++;
      *result = obj;
      return NULL;
    }

  LOG("Creating shared memory object \"%s\"", name);
  err = create_shm_object(name, result);

  return err;
}

static _kernel_oserror *
shm_unlink(const char *name)
{
  shm_object *obj;

  TRACE("shm_unlink(\"%s\")", name);

  if (strlen(name) > PATH_MAX)
    return armeabisupport_ENAMETOOLONG;

  obj = find_shm_object(name);
  if (!obj || obj->valid != VALID_SHM)
    {
      LOG("shm_unlink: Shared memory object \"%s\" not found", name);
      return armeabisupport_ENOENT;
    }

  obj->linked = false;
/*  if (obj->name)
    {
      rma_free(obj->name);
      obj->name = NULL;
    }*/

  return NULL;
}

_kernel_oserror *
shm_close(shm_object *shm)
{
  if (shm->valid != VALID_SHM)
    return armeabisupport_EINVAL;

  TRACE("shm_close(%p) name=\"%s\"", shm, shm->name);

  shm_deref_object(shm);

  return NULL;
}

bool shm_deref_object(shm_object *shm)
{
  if (shm->valid != VALID_SHM)
    return false;

  if (shm->ref_count-- == 0)
    {
      TRACE("shm_deref_object: deleting shared memory object %p, \"%s\", unmapping memory", shm, shm->name);

      linklist_remove(&global.shm_object_list, &shm->link);
      allocator_munmap_block(shm->allocator, shm->block);

      if (shm->name)
	rma_free(shm->name);

      shm->valid = 0;
      shm->fd_type = FD_TYPE_INVALID;
      rma_free(shm);

      return true;
    }

    TRACE("shm_deref_object: shared memory object %p, \"%s\", ref=%d", shm, shm->name, shm->ref_count);

    return false;
}

/* Remove any shared memory objects for the given app.  */
void shm_cleanup_app(app_object *app)
{
  shm_object *shm = linklist_first_shm_object(&global.shm_object_list);
  while (shm)
    {
      shm_object *next = linklist_next_shm_object(shm);

      if (shm->block->owner == app)
	{
	  linklist_remove(&global.shm_object_list, &shm->link);
	  allocator_munmap_block(shm->allocator, shm->block);

	  if (shm->name)
	    rma_free(shm->name);
	  shm->valid = 0;
	  rma_free(shm);
	}

      shm = next;
    }
}
