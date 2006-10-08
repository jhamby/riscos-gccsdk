	.include "../rt_workspace.h"
	.include "../structs.h"
	.include "../utils/swis.h"
	.include "../utils/macros.h"

	.text

	.global	__rt_load_pic
	.hidden __rt_load_pic
__rt_load_pic:
	STMFD	sp!, {r0,r1,r14}

	ADR	r7, __rt_load_pic
	MOV	r14, #0x8000
	LDR	r1, [r14, #RT_WORKSPACE_CACHED_R7]
	TEQ	r1, r7
	LDREQ	r7, [r14, #RT_WORKSPACE_CACHED_GOT]
	BEQ	1f
@;2:
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
	@; Check if there's an object list for this client
	LDR	r1, [r14, #RT_WORKSPACE_OBJECT_LIST]
	TEQ	r1, #0
	@; If not, then the dynamic loader is still initialising, so give it the
	@; benefit of the doubt and return the loader's private GOT ptr.
	LDREQ	r7, [r14, #RT_WORKSPACE_LD_GOT]
	@; Otherwise, we really can't find the library, so return a bad value.
	LDRNE	r7, bad_value
	LDMFD	sp!, {r0,r1,lr}
	MOV	pc, lr
bad_value:
	.word	0x7BADBAD7
	.type	__rt_load_pic, %function
	.size	__rt_load_pic, . - __rt_load_pic

	.end
