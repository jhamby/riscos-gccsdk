#if 0
#include <sysdep.h>
#endif
/*
 * These are various helper routines that are needed to run an ELF image.
 */
#ifndef ALIGN
#define ALIGN 2
#endif

#ifndef NO_UNDERSCORE
#define RESOLVE __dl_riscos_resolve
#define RESOLVER __dl_riscos_resolver
#else
#define RESOLVE _dl_riscos_resolve
#define RESOLVER _dl_riscos_resolver
#endif

@ This code is adapted from the loader supplied with version 2.3.2
@ of glibc which supports ARM

	.text

.globl RESOLVE
	.type	RESOLVE,%function
RESOLVE:
	@ we get called with
	@	r8 contains &GOT[n+5] (pointer to function - private GOT)
	@	lr points to &GOT[5] (private GOT)

	@ We have to be careful which registers we use here. We can't use
	@ any that the stack extension routines may require to be preserved
	@ (eg, sl, fp, ip).

	@ lr was already stacked by the PLT
	@ ip is stacked for the sake of the stack extension routines, as
	@ the fixup routine may corrupt it.
	stmfd	sp!, {r0-r3, ip}

	@ prepare to call fixup()
	@ change &GOT[n+5] into 8*n        NOTE: reloc are 8 bytes each
	sub	r1, r8, lr
	sub	r1, r1, #4
	add	r1, r1, r1

	@ Get pointer to linker struct
	ldr	r0, [lr, #-4]

	@ Call fixup routine
	bl	_dl_riscos_resolver

	@ Save the return
	mov	r8, r0

	@ Restore the stack
	ldmfd	sp!, {r0-r3, ip, lr}

	@ Jump to the newly found address
	mov	pc, r8
.LFE2:
	.size RESOLVE,.LFE2-RESOLVE
