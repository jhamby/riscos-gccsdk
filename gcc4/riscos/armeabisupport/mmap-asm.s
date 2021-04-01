@ Copyright (c) 2021 GCCSDK Developers
@ Written by Lee Noar

#include "decl-asm.h"
#include "debug-asm.h"

	.text

@ Entry:
@  r1 = address of fault
@  r11 = ptr to register block (r0-r15, cpu flags)
@  r12 = ptr to user data
@ Exit:
@  r0 = bits 0-7: ABORT_HANDLER_CLAIM if able to handle, ABORT_HANDLER_ERROR if there was an
@	error otherwise ABORT_HANDLER_PASS_ON
@	bits 8-15: If ABORT_HANDLER_ERROR, then error code otherwise 0.
@  r12 can be corrupted
@  All other registers preserved
mmap_abort_handler:
	STMFD	sp!, {r1-r8, r10-r11, lr}

	LDR	r10, [r12, #LINKLIST_FIRST]
	@ r10 = mmap object
0:
	TEQ	r10, #0
	MOVEQ	r0, #ABORT_HANDLER_PASS_ON
	LDMEQFD	sp!, {r1-r8, r10-r11, pc}	@ End of allocator list, pass on

	@ Check if the addresss is lower than the dynamic area base address
	LDR	r7, [r10, #MMAP_ALLOCATOR + ALLOCATOR_DA + DA_START]
	CMP	r1, r7
	LDRLO	r10, [r10, #MMAP_ALLOCATOR + ALLOCATOR_LINK + LINKHDR_NEXT]
	BLO	0b	@ It is, continue to the next allocator in the pool
	@ Check if the address is higher than the last addressable page in the dynamic area
	LDR	r2, [r10, #MMAP_ALLOCATOR + ALLOCATOR_DA + DA_SIZE]
	ADD	r2, r7, r2, LSL#PAGE_SHIFT	@ End address of dynamic area
	CMP	r1, r2
	LDRHS	r10, [r10, #MMAP_ALLOCATOR + ALLOCATOR_LINK + LINKHDR_NEXT]
	BHS	0b	@ It is, continue to the next allocator in the pool

	@ We've found the mmap allocator (r10), now find the block
	LDR	r8, [r10, #MMAP_ALLOCATOR_BLOCK_LIST + LINKLIST_FIRST]
1:
	@ If we reach the end of the block list, then that could indicate
	@ a program trying to use a freed block.
	TEQ	r8, #0
	MOVEQ	r0, #ABORT_HANDLER_PASS_ON
	LDMEQFD	sp!, {r1-r8, r10-r11, pc}	@ End of block list, pass on

	@ r7 = Base address of dynamic area
	@ r8 = MMap block
	LDR	r2, [r8, #MMAP_BLOCK_END_PAGE]
	ADD	r2, r7, r2, LSL#PAGE_SHIFT		@ Convert page number to address
	CMP	r1, r2
	LDRHI	r8, [r8, #MMAP_BLOCK_LINK + LINKHDR_NEXT]
	BHI	1b

	@ We know that the fault address is lower than the end of the current block
	@ and we've already eliminated all blocks before the current block. If the
	@ fault address is below the start of the current block, then it must be within
	@ an unallocated region.
	LDR	r2, [r8, #MMAP_BLOCK_START_PAGE]
	ADD	r2, r7, r2, LSL#PAGE_SHIFT
	CMP	r1, r2
	BLO	mapabort_mmap_area

	@ Found a valid mmap block, map the page in
	SUB	r4, r1, r7				@ fault address - da base address
	MOV	r4, r4, LSR#PAGE_SHIFT			@ convert to page number
	SUB	sp, sp, #PMP_PHY_PAGE_ENTRY_SIZE	@ Create space on the stack for 1 entry
	MOV	r5, #PMP_PAGE_KERNEL_CHOICE		@ Use what pages the kernel chooses
	MOV	r6, #0					@ Page flags
	STMIA	sp, {r4-r6}				@ Create the page entry on the stack
	MOV	r0, #OSDYNAMICAREA_REASON_PHYSICALOP	@ Claim the relevent page
	LDR	r1, [r10, #MMAP_ALLOCATOR + ALLOCATOR_DA + DA_NUMBER]
	MOV	r2, sp
	MOV	r3, #1
	SWI	XOS_DynamicArea
	ADDVS	sp, sp, #PMP_PHY_PAGE_ENTRY_SIZE
	BVS	mmapabort_swi_error

	@ We use the same stack space for physical and logical operations. Luckily, they're
	@ the same size.
	STR	r4, [sp, #PMP_LOG_PAGE_ENTRY_INDEX]	@ Fill in the page number we want mapped
	MOV	r0, #OSDYNAMICAREA_REASON_LOGICALOP	@ Map in the relevent page
	@ r1 should still be the dynamic area number

	MOV	r2, sp
	MOV	r3, #1
	SWI	XOS_DynamicArea
	ADD	sp, sp, #PMP_PHY_PAGE_ENTRY_SIZE
	BVS	mmapabort_swi_error

	@ Zero fill the newly mapped page
	MOV	r0, #0
	MOV	r1, #0
	MOV	r2, #0
	MOV	r3, #0
	MOV	r5, #0
	MOV	r6, #0
	MOV	r8, #0
	MOV	r10, #0
	ADD	r4, r7, r4, LSL#PAGE_SHIFT		@ Address of page to zero: da base + page number
	MOV	r7, #16
2:
	STMIA	r4!, {r0,r1,r2,r3,r5,r6,r8,r10}		@ 32 bytes
	STMIA	r4!, {r0,r1,r2,r3,r5,r6,r8,r10}		@ 64 bytes
	STMIA	r4!, {r0,r1,r2,r3,r5,r6,r8,r10}		@ 96 bytes
	STMIA	r4!, {r0,r1,r2,r3,r5,r6,r8,r10}		@ 128 bytes
	STMIA	r4!, {r0,r1,r2,r3,r5,r6,r8,r10}		@ 160 bytes
	STMIA	r4!, {r0,r1,r2,r3,r5,r6,r8,r10}		@ 192 bytes
	STMIA	r4!, {r0,r1,r2,r3,r5,r6,r8,r10}		@ 224 bytes
	STMIA	r4!, {r0,r1,r2,r3,r5,r6,r8,r10}		@ 256 bytes
	SUBS	r7, r7, #1
	BNE	2b

	@ FIXME: Use mmap block access flags to apply correct protection

mmapabort_claim:
	MOV	r0, #ABORT_HANDLER_CLAIM
	LDMFD	sp!, {r1-r8, r10-r11, pc}
mmapabort_pass_on:
	MOV	r0, #ABORT_HANDLER_PASS_ON
	LDMFD	sp!, {r1-r8, r10-r11, pc}
mmapabort_swi_error:
	MOV	r0, #ABORT_HANDLER_ERROR
	ORR	r0, r0, #(ABORT_ERROR_MMAP_INTERNAL << 8)
	LDMFD	sp!, {r1-r8, r10-r11, pc}
mapabort_mmap_area:
	MOV	r0, #ABORT_HANDLER_ERROR
	ORR	r0, r0, #(ABORT_ERROR_MMAP_AREA << 8)
	LDMFD	sp!, {r1-r8, r10-r11, pc}

	.global	get_mmap_abort_handler
get_mmap_abort_handler:
	ADR	r0, mmap_abort_handler
	MOV	pc, lr
