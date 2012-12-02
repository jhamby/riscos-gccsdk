/*
 * wimp-support.c
 *
 * Part of libriscosdotnet - a binding of the RISC OS API to C#.
 *
 * Author: Lee Noar (leenoar@sky.com)
*/

#include <kernel.h>

#define WIMP_EVENT_BLOCK_SIZE (260 / 4)

/* The event code returned by Wimp_Poll/Wimp_PollIdle is stored in event[0],
 * the actual event data starts at &event[1].  */
static int event[WIMP_EVENT_BLOCK_SIZE];

_kernel_oserror *rdn_Wimp_Poll (int mask,
				int *pollword)
{
  register _kernel_oserror *err __asm ("r0");
  register int mask_reg __asm ("r0") = mask;
  void *block_reg = &event;
  register int *pollword_reg __asm ("r3") = pollword;

  __asm__ volatile ("	add	r1, %[block_reg], #4;\n"
		    "	swi	%[SWI_XWimp_Poll];\n"
		    "	strvc	r0, [%[block_reg], #0];\n"
		    "	movvc	r0, #0;\n"
		    : "=r" (err)
		    : "r" (mask_reg), [block_reg] "r" (block_reg), "r" (pollword_reg),
		    [SWI_XWimp_Poll] "i" (0x600c7)
		    : "r1", "cc");
  return err;
}

_kernel_oserror *rdn_Wimp_PollIdle (int mask,
				    unsigned int time,
				    int *pollword)
{
  register _kernel_oserror *err __asm ("r0");
  register int mask_reg __asm ("r0") = mask;
  void *block_reg = &event;
  register unsigned int time_reg __asm ("r2") = time;
  register int *pollword_reg __asm ("r3") = pollword;

  __asm__ volatile ("	add	r1, %[block_reg], #4;\n"
		    "	swi	%[SWI_XWimp_PollIdle];\n"
		    "	strvc	r0, [%[block_reg], #0];\n"
		    "	movvc	r0, #0;\n"
		    : "=r" (err)
		    : "r" (mask_reg), [block_reg] "r" (block_reg), "r" (time_reg), "r" (pollword_reg),
		    [SWI_XWimp_PollIdle] "i" (0x600e1)
		    : "r1", "cc");
  return err;
}

int rdn_Wimp_GetEventType (void)
{
  return event[0];
}

void *rdn_Wimp_GetEvent (void)
{
  return event + 1;
}
