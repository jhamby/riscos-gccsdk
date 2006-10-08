	.macro PRINT_TEXT string

	swi	0x1
	.ascii	"\string\0"
	.align

	.endm

	.macro PRINT_HEX int
	stmfd	sp!,{r0-r12,r14}
	mov	r12,\int
	b	9f
8:
	.word	0,0,0,0,0
9:
	mov	r0,r12
	adr	r1,8b
	mov	r2,#20
	swi	0xd4
	mov	r2,#0
	strb	r2,[r1,#0]
	swi	0x2
	swi	0x10a
	swi	0x10d

	ldmfd	sp!,{r0-r12,r14}
	.endm
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

@; This code is adapted from the loader supplied with version 2.3.2
@; of glibc which supports ARM

	.text

.globl RESOLVE
	.type	RESOLVE,%function
RESOLVE:
	@; we get called with
	@;	r0 contains &GOT[n+3] (pointer to function)
	@;	r8 points to &GOT[2]

	@; prepare to call fixup()
	@; change &GOT[n+3] into 8*n        NOTE: reloc are 8 bytes each
	sub	r1, r0, r8
	sub	r1, r1, #4
	add	r1, r1, r1

	@; get pointer to linker struct
	ldr	r0, [r8, #-4]

	@; call fixup routine
	ldr	r4, 0f
	add	r4, pc, r4
1:
	ldr	r3, 2f

	mov	lr, pc
	add	pc, r4, r3
	b	3f
0:
	.word	_GLOBAL_OFFSET_TABLE_ - 1b - 4
2:
	.word	RESOLVER(GOTOFF)
3:
	@; save the return
	mov	r8, r0

	@; restore the stack
	ldmfd	sp!,{r0-r4,r7,ip,lr}

	@; jump to the newly found address
	mov	pc, r8
.LFE2:
	.size RESOLVE,.LFE2-RESOLVE
