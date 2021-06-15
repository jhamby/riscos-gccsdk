/* shm.h
 *
 * Copyright 2021 GCCSDK Developers
 * Written by Lee Noar
 */

#ifndef SHM_H
#define SHM_H

#include "types.h"

#define VALID_SHM 0x304D4853	/* SHM0 */
#define FD_TYPE_INVALID 0
#define FD_TYPE_SHM 1

#define IS_SHM_FD(fd) ((fd) != -1 && (void *)fd > (void *)0x8000)

typedef struct shm_object
{
  link_hdr link;

  uint32_t valid;

  /* In case we wish to expand the shared fd idea in the future.  */
  int fd_type;

  armeabisupport_allocator_mmap *allocator;
  mmap_block *block;

  int ref_count;
  bool linked;

  char *name;
} shm_object;
LINKLIST_ACCESS_FUNCTIONS(shm_object)

extern _kernel_oserror *shm_op(_kernel_swi_regs * r);

/* Return true if the object was deleted.  */
extern bool shm_deref_object(shm_object *);

extern void shm_dump_object_list(void);
extern void shm_cleanup_app(app_object *app);

#endif
