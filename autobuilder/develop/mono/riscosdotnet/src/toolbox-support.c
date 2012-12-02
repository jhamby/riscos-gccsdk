/*
 * toolbox-support.c
 *
 * Part of libriscosdotnet - a binding of the RISC OS API to C#.
 *
 * Author: Lee Noar (leenoar@sky.com)
*/

#include <kernel.h>

static int id_block [6];

static int messtrans_fd [4];

_kernel_oserror *rdn_Toolbox_Initialise (unsigned int flags,
					 unsigned int wimp_version,
					 int *message_list,
					 int *event_list,
					 char *dir_name,
					 int *version_out,
					 int *task_out,
					 int *sprite_out)
{
  register _kernel_oserror *err __asm ("r0");
  register unsigned int flags_reg __asm ("r0") = flags;
  register unsigned int wimp_version_reg __asm ("r1") = wimp_version;
  register int *message_list_reg __asm ("r2") = message_list;
  register int *event_list_reg __asm ("r3") = event_list;
  register char *dir_name_reg __asm ("r4") = dir_name;
  register void *messtrans_reg __asm ("r5") = &messtrans_fd;
  register void *id_block_reg __asm ("r6") = &id_block;

  __asm__ volatile ("	swi	%[SWI_XToolbox_Initialise];\n"
		    "	bvs	0f;\n"
		    "	ldr	lr, %[version_out];\n"
		    "	str	r0, [lr];\n"
		    "	ldr	lr, %[task_out];\n"
		    "	str	r1, [lr];\n"
		    "	ldr	lr, %[sprite_out];\n"
		    "	str	r2, [lr];\n"
		    "	mov	r0, #0;\n"
		    "0:\n"
		    : "=r" (err)
		    : [version_out] "m" (version_out),
		      [task_out] "m" (task_out),
		      [sprite_out] "m" (sprite_out),
		      "r" (flags_reg),
		      "r" (wimp_version_reg),
		      "r" (message_list_reg),
		      "r" (event_list_reg),
		      "r" (dir_name_reg),
		      "r" (messtrans_reg),
		      "r" (id_block_reg),
		      [SWI_XToolbox_Initialise] "i" (0x64ecf)
		    : "lr", "cc");
  return err;
}

void *rdn_Toolbox_GetIDBlock (void)
{
  return id_block;
}

void *rdn_Toolbox_GetMessTransFD (void)
{
  return messtrans_fd;
}
