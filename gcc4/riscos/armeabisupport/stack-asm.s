@ Copyright (c) 2019 GCCSDK Developers
@ Written by Lee Noar

#include "decl-asm.h"
#include "debug-asm.h"

	.text

@ Entry:
@  r1 = address of fault
@  r11 = ptr to register block (r0-r15, cpu flags)
@  r12 = ptr to user data block
@ Exit:
@  r0 = ABORT_HANDLER_CLAIM if able to handle, otherwise ABORT_HANDLER_PASS_ON
@  r12 can be corrupted
@  All other registers preserved
stack_abort_handler:
	STMFD	sp!, {r1-r8, r10-r11, lr}

	LDR	r10, [r12, #ABORT_BLOCK_FIRST_STACK]
	@ r10 = stack object
0:
	TEQ	r10, #0
	MOVEQ	r0, #ABORT_HANDLER_PASS_ON
	LDMEQFD	sp!, {r1-r8, r10-r11, pc}

	LDR	r2, [r10, #STACK_BASE]
	LDR	r3, [r10, #STACK_TOP]
	CMP	r1, r2
	LDRLO	r10, [r10, #STACK_LINK + LINKHDR_NEXT]
	BLO	0b
	CMP	r1, r3
	LDRHS	r10, [r10, #STACK_LINK + LINKHDR_NEXT]
	BHS	0b

	@ We've found the stack, now check if we're in the guard page(s)
	LDR	r4, [r10, #STACK_GUARD_SIZE]		@ Add the guard page byte size
	ADD	r4, r2, r4, LSL#PAGE_SHIFT		@ to the stack base address
	CMP	r1, r4					@ If we're lower than that, then we really do
	BLO	stackabort_overflow			@ have a stack overflow

	LDR	r8, [r12, #ABORT_BLOCK_STACK_ALLOCATOR]
	@ r8 = stack allocator object
	LDR	r4, [r8, #ALLOCATOR_DA + DA_START]
	SUB	r4, r1, r4				@ fault address - da start
	MOV	r4, r4, LSR#PAGE_SHIFT			@ convert to page number
	SUB	sp, sp, #PMP_PHY_PAGE_ENTRY_SIZE	@ Create space on the stack for 1 entry
	MOV	r5, #PMP_PAGE_KERNEL_CHOICE		@ Use what pages the kernel chooses
	MOV	r6, #0					@ Page flags
	STMIA	sp, {r4-r6}				@ Create the page entry on the stack

	MOV	r0, #OSDYNAMICAREA_REASON_PHYSICALOP	@ Claim the relevent page
	LDR	r1, [r8, #ALLOCATOR_DA + DA_NUMBER]

	MOV	r2, sp
	MOV	r3, #1
	SWI	XOS_DynamicArea
	ADDVS	sp, sp, #PMP_PHY_PAGE_ENTRY_SIZE
	BVS	stackabort_pass_on

	@ We use the same stack space for physical and logical operations. Luckily, they're
	@ the same size.
	STR	r4, [sp, #PMP_LOG_PAGE_ENTRY_INDEX]	@ Fill in the page number we want mapped
	MOV	r0, #OSDYNAMICAREA_REASON_LOGICALOP	@ Map in the relevent page
	@ r1 should still be the dynamic area number

	MOV	r2, sp
	MOV	r3, #1
	SWI	XOS_DynamicArea
	ADD	sp, sp, #PMP_PHY_PAGE_ENTRY_SIZE
	BVS	stackabort_pass_on

stackabort_claim:
	MOV	r0, #ABORT_HANDLER_CLAIM
	LDMFD	sp!, {r1-r8, r10-r11, pc}
stackabort_pass_on:
	MOV	r0, #ABORT_HANDLER_PASS_ON
	LDMFD	sp!, {r1-r8, r10-r11, pc}
stackabort_overflow:
	@ FIXME: How can we propagate a stack overflow through Unixlib to the program?
	REPORT_TEXT "Stack overflow"
	REPORT_TEXT "Base:"
	REPORT_HEX r2
	REPORT_TEXT "Top:"
	REPORT_HEX r3

	MOV	r0, #ABORT_HANDLER_PASS_ON
	LDMFD	sp!, {r1-r8, r10-r11, pc}

	.global	get_stack_abort_handler
get_stack_abort_handler:
	ADR	r0, stack_abort_handler
	MOV	pc, lr
