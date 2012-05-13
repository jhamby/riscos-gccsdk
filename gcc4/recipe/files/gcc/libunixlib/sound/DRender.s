/*
 * DRender.S - DigitalRenderer interface
 *
 * Written by
 *  Andreas Dehmel <zarquon@t-online.de>
 *
 * Rewritten for inclusion in GCCSDK's libunixlib by
 *  Christopher Martin <belles@internode.on.net>
 *
 * (C) 1997-2000 Andreas Dehmel (zarquon@t-online.de)
 * (C) 2012 GCCSDK Developers
 *
 * This file was part of wimplib. wimplib is released under the Lesser
 * GNU Public License (LGPL). For more information see the License
 * provided with the library release.
 *
 * Currently part of the GCCSDK project.
 *
 * Note that the extern functions implemented herein and defined in DRender.h
 * are intended to be called **only** when the processor is in USR mode.
 * On the plus side, they will restore flags when called in 26-bit mode.
 */

#include "internal/asm_dec.s"

#define DigiRendChunk	0x6F700	/* X flag set */

	.text

	@ const _kernel_oserror * DRender_SetDefaults (int *channels, int *format,
	@					       int *period, int *buffsize,
	@					       int *numbuff, int *freq)
        .global DRender_SetDefaults
        NAME	DRender_SetDefaults
DRender_SetDefaults:
	stmfd	sp!, {r4-r10}		/* save 7 regs, r12 (ip) can be clobbered */
	mov	r6, r0
	mov	r7, r1
	mov	r8, r2
	mov	r9, r3
	ldr	r10, [sp, #7<<2]	/* fetch stached arg #5 */
	ldr	r12, [sp, #8<<2]	/* fetch stacked arg #6 */
	ldr	r0, [r6]
	ldr	r1, [r7]
	ldr	r2, [r8]
	ldr	r3, [r9]
	ldr	r4, [r10]
	ldr	r5, [r12]
	swi	DigiRendChunk + 14	/* will clobber r14 (lr) in SVC mode */
	bvs	9f
	str	r0, [r6]
	str	r1, [r7]
	str	r2, [r8]
	str	r3, [r9]
	str	r4, [r10]
	str	r5, [r12]
9:	ldmfd	sp!, {r4-r10}
	movvcs	r0, #0			/* Ensures a PSR26 flag is set */
	teq	pc, pc
	moveq	pc, lr
	movs	pc, lr
	DECLARE_FUNCTION DRender_SetDefaults

        @ const _kernel_oserror * DRender_LoadModule (const char * path)
        .global DRender_LoadModule
        NAME	DRender_LoadModule
DRender_LoadModule:
	movs	r1, r0
	adreq	r1, 9f
	mov	r0, #1			/* RMLoad */
	swi	XOS_Module		/* will clobber r14 (lr) in SVC mode */
	movvcs	r0, #0			/* Ensures a PSR26 flag is set */
	teq	pc, pc
	moveq	pc, lr
	movs	pc, lr
9:	.asciz	"System:Modules.DRenderer"
        .align
        DECLARE_FUNCTION DRender_LoadModule

	.end
