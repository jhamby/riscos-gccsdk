/*
 * os-support.c
 *
 * Part of libriscosdotnet - a binding of the RISC OS API to C#.
 *
 * Author: Lee Noar (leenoar@sky.com)
*/

#include <kernel.h>

_kernel_oserror *rdn_OS_BPut (unsigned int byte,
			      unsigned int file_handle)
{
  register _kernel_oserror *err __asm ("r0");
  register unsigned int byte_reg __asm ("r0") = byte;
  register unsigned int file_handle_reg __asm ("r1") = file_handle;
  
  __asm__ volatile ("	swi	%[SWI_XOS_BPut];\n"
		    "	movvc	r0, #0;\n"
		    : "=r" (err)
		    : "r" (byte_reg),
		      "r" (file_handle_reg),
		      [SWI_XOS_BPut] "i" (0x2000b)
		    : "cc");
  return err;
}

_kernel_oserror *rdn_OS_BGet (unsigned int file_handle,
			      unsigned int *byte_out,
			      unsigned int *carry_flag_out)
{
  register _kernel_oserror *err __asm ("r0");
  register unsigned int file_handle_reg __asm ("r1") = file_handle;
  register unsigned int *byte_out_reg __asm ("r2") = byte_out;
  register unsigned int *carry_flag_out_reg __asm ("r3") = carry_flag_out;

  __asm__ volatile ("	swi	%[SWI_XOS_BGet];\n"
		    "	bvs	0f;\n"
		    "	str	r0, [r2];\n"
		    "	movcs	r0, #1;\n"
		    "	movcc	r0, #0;\n"
		    "	str	r0, [r3];\n"
		    "	mov	r0, #0;\n"
		    "0:\n"
		    : "=r" (err)
		    : "r" (byte_out_reg),
		      "r" (carry_flag_out_reg),
		      "r" (file_handle_reg),
		      [SWI_XOS_BGet] "i" (0x2000a)
		    : "cc");
  return err;
}
