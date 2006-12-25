@ _rt_pic.asm for RISC OS
@ Runtime routines that form libpic.a which is linked into every shared
@ library.
@ Copyright (c) 2006 GCCSDK Developers

#include "rt_workspace.h"
#include "som_structs.h"

	.text

@ A call to __rt_load_pic is added by GCC to the prologue of any shared library
@ function that accesses global data.
	.global	__rt_load_pic
	.hidden __rt_load_pic
__rt_load_pic:
	STMFD	sp!, {r0,r1,lr}

	ADR	r7, __rt_load_pic
	MOV	r14, #0x8000
	LDR	r1, [r14, #RT_WORKSPACE_CACHED_R7]
	TEQ	r1, r7
	LDREQ	r7, [r14, #RT_WORKSPACE_CACHED_GOT]
	BEQ	1f

	LDR	r0, [r14, #RT_WORKSPACE_OBJECT_LIST]
0:
	TEQ	r0, #0
	BEQ	99f

	LDR	r1, [r0, #OBJECT_END_ADDR]
	CMP	r7, r1
	LDRHI	r0, [r0, #(OBJECT_LINK_HDR + LINK_NEXT)]
	BHI	0b

	STR	r7, [r14, #RT_WORKSPACE_CACHED_R7]
	LDR	r7, [r0, #OBJECT_PRIVATE_GOT_PTR]
	STR	r7, [r14, #RT_WORKSPACE_CACHED_GOT]
1:
	LDMFD	sp!, {r0,r1,lr}
	MOV	pc, lr
99:
	@ Check if there's an object list for this client
	LDR	r1, [r14, #RT_WORKSPACE_OBJECT_LIST]
	TEQ	r1, #0
	@ If not, then the dynamic loader is still initialising, so give it the
	@ benefit of the doubt and return the loader's private GOT ptr.
	LDREQ	r7, [r14, #RT_WORKSPACE_LD_GOT]
	@ Otherwise, we really can't find the library, so return a bad value.
	LDRNE	r7, bad_value
	LDMFD	sp!, {r0,r1,lr}
	MOV	pc, lr
bad_value:
	.word	0x7BADBAD7
	.type	__rt_load_pic, %function
	.size	__rt_load_pic, . - __rt_load_pic

@ __rt_public_got()
@ Used by the dynamic loader to find a library's GOT during initialisation.
@ This was originally done simply by looking up the symbol __GLOBAL_OFFSET_TABLE__,
@ however, at some point in the life of binutils, this symbol became hidden.

@ We can't assume _GLOBAL_OFFSET_TABLE_ will be exported, so we have to be
@ a bit cleverer to find it. Return the dynamic segment position as an
@ offset from the GOT. When the dynamic segment address is known, this offset
@ can be used to work backwards to find the GOT.
	.global	__rt_public_got
__rt_public_got:
	LDR	r0, 2f
	MOV	pc, lr
2:
	.word	_DYNAMIC(GOTOFF)
	.type	__rt_public_got, %function
	.size	__rt_public_got, . - __rt_public_got

	.end
