@ Copyright (c) 2019 GCCSDK Developers
@ Written by Lee Noar

#include "decl-asm.h"
#include "debug-asm.h"


	.text

abort_handler:
	STR	r0, registers_0
	SUB	r0, r14, #8
	STR	r0, registers_PC

	MRS	r0, CPSR_all
	STR	r0, registers_CPSR		@ Store the handler CPU state

	MRS	r0, SPSR_all			@ Read aborting mode CPU state
	STR	r0, registers_SPSR		@ Store the aborting SPSR
	TST	r0, #0xF			@ is it USR?
	BEQ	USR_mode
	MSR	CPSR_all, r0			@ If not USR, switch to aborting CPU state
	ADR	r0, registers_0 + 4
	STMIA	r0, {r1-r14}			@ store the aborting state registers
	B	1f
USR_mode:
	ADR	r0, registers_0 + 4
	STMIA	r0!, {r1-r12}
	STMIA	r0, {sp, lr}^
	MRS	r6, SPSR_all
	STR	r6, registers_SPSR		@ store the aborting SPSR
1:
	ADR	r7, data_abort_support_block

	MRS	r9, CPSR
	BIC	r0, r9, #0x1F			@ Clear all mode bits
	ORR	r0, r0, #0x13			@ Set SVC mode bits
	MSR	CPSR_c, r0			@ Switch to SVC
	STR	lr, [sp, #-4]!			@ Preserve R14_svc
	@ No SWIs before this point, otherwise we corrupt SVC_lr

	@ Guard against an abort in this abort handler, as soon as possible at least
	LDR	r0, [r7, #ABORT_BLOCK_IN_HANDLER]
	TEQ	r0, #1
	.if DEBUG_HANDLER_DUMP_REGS == 1
	BEQ	handler__internal_abort
	.else
	BEQ	handler__pass_on
	.endif

	@ Read the address of the fault
	MRC	P15, 0, r8, C6, C0

	MOV	r0, #1
	STR	r0, [r7, #ABORT_BLOCK_IN_HANDLER]

	LDR	r10, [r7, #ABORT_BLOCK_FIRST_HANDLER]
	TEQ	r10, #0
	BEQ	handler__pass_on
	ADR	r11, registers_0
	MOV	r1, r8					@ address of fault
0:
	LDR	r12, [r10, #ABORT_HANDLER_R12_DATA]
	MOV	lr, pc
	LDR	pc, [r10, #ABORT_HANDLER_ROUTINE]
	TEQ	r0, #ABORT_HANDLER_CLAIM
	BEQ	handler__exit
	LDR	r10, [r10, #LINKHDR_NEXT]
	TEQ	r10, #0
	BNE	0b

	.if DEBUG_HANDLER_DUMP_REGS == 1
	B	handler__unhandled_abort
	.endif

handler__pass_on:
	LDR	lr, [sp], #4			@ Restore R14_svc

	LDR	r0, [r7, #ABORT_BLOCK_OLD_HANDLER]
	STR	r0, registers_PC

	LDR	r9, registers_CPSR

	MOV	r0, #0
	STR	r0, [r7, #ABORT_BLOCK_IN_HANDLER]

	ADR	r0, registers_0
	MSR	SPSR_cxsf, r9
	LDMIA	r0, {r0-pc}^			@ Restore all registers and return to old handler

	.if DEBUG_HANDLER_DUMP_REGS == 1
handler__internal_abort:
	REPORT_TEXT "Abort in abort handler:"
	B	handler__dump_regs

handler__unhandled_abort:
	REPORT_TEXT "Unhandled abort:"
	REPORT_HEX r6

handler__dump_regs:
	MRC	P15, 0, r12, C6, C0
	REPORT_TEXT "Fault address:"
	REPORT_HEX r12
	REPORT_TEXT "Fault instruction at:"
	LDR	r0, registers_PC
	REPORT_HEX r0
	MOV 	r0, #0
	ADR	r1, registers_0
2:
	LDR	r2, [r1, r0, LSL#2]
	REPORT_DEC r0
	REPORT_HEX r2
	ADD	r0, r0, #1
	CMP	r0, #18
	BLT	2b
	B	handler__pass_on
	.endif

handler__exit:
	.if ABORT_COLLECT_STATS == 1
	LDR	r9, [r7, #ABORT_BLOCK_CURRENT_APP]
	LDR	r0, [r9, #APP_ABORT_LAST_WIMP_COUNT]
	ADD	r0, r0, #1
	STR	r0, [r9, #APP_ABORT_LAST_WIMP_COUNT]
	LDR	r1, [r9, #APP_ABORT_HIGHEST_WIMP_COUNT]
	CMP	r0, r1
	STRGT	r0, [r9, #APP_ABORT_HIGHEST_WIMP_COUNT]
	LDR	r0, [r9, #APP_ABORT_TOTAL_COUNT]
	ADD	r0, r0, #1
	STR	R0, [R9, #APP_ABORT_TOTAL_COUNT]
	.endif

	LDR	lr, [sp], #4			@ Restore R14_svc

	LDR	r9, registers_SPSR

	MOV	r0, #0
	STR	r0, [r7, #ABORT_BLOCK_IN_HANDLER]

	ADR	lr, registers_0
	MSR	SPSR_cxsf, r9

	@ Restore all registers and return to aborting instruction
	TST	r9, #0xF
	LDMNEIA	lr, {r0-pc}^			@ Non USR mode return
	LDMIA	lr, {r0-lr}^			@ USR mode return
	LDR	lr, [lr, #15*4]
	MOVS	pc, lr

	.global	abort_install
abort_install:
	STMFD	sp!, {r1-r3, r12, lr}

	ADR	r12, data_abort_support_block

	MOV	r0, #3
	ADRL	r1, abort_handler
	MOV	r2, #0
	MOV	r3, #0
	SWI	XOS_ChangeEnvironment
	STRVC	r1, [r12, #ABORT_BLOCK_OLD_HANDLER]
	MOVVC	r0, #0

	LDMFD	sp!, {r1-r3, r12, pc}

	.global	abort_remove
abort_remove:
	STMFD	sp!, {r1-r3, r12, lr}

	ADR	r12, data_abort_support_block

	LDR	r1, [r12, #ABORT_BLOCK_OLD_HANDLER]

	TEQ	r1, #0
	LDMEQFD	sp!, {r1-r3, r12, pc}

	MOV	r0, #0
	STR	r0, [r12, #ABORT_BLOCK_OLD_HANDLER]

	MOV	r0, #SYSTEMCONTROLHANDLER_DATAABORT
	MOV	r2, #0
	MOV	r3, #0
	SWI	XOS_ChangeEnvironment
	MOVVC	r0, #0

	LDMFD	sp!, {r1-r3, r12, pc}

	.global	get_data_abort_support_block
get_data_abort_support_block:
	ADR	r0, data_abort_support_block
	MOV	pc, lr

data_abort_support_block:
	.space	ABORT_BLOCK_SIZE

registers_0:
	.space	15*4
registers_PC:
	.space	4
registers_SPSR:
	@ CPU mode of the aborting instruction
	.space	4
registers_CPSR:
	@ CPU mode on entry to abort handler
	.space	4

@ Entry:
@  r12 = ptr to module globals
pre_wimp_filter:
	STMFD	sp!, {r0, r8, r11, lr}
	MRS	r8, CPSR

	ADR	r11, data_abort_support_block
	MOV	r0, #0
	STR	r0, [r11, #ABORT_BLOCK_FIRST_HANDLER]
	STR	r0, [r11, #ABORT_BLOCK_FIRST_STACK]

	MSR	CPSR_f, r8
	LDMFD	sp!, {r0, r8, r11, pc}

@ Exit:
@  r0 = ptr to pre wimp_filter routine
	.global	abort_get_pre_filter
abort_get_pre_filter:
	ADR	r0, pre_wimp_filter
	MOV	pc, lr

@ Exit:
@  r0 = ptr to post wimp_filter routine
	.global	abort_get_post_filter
abort_get_post_filter:
	ADR	r0, post_wimp_filter
	MOV	pc, lr

@ Entry:
@  r12 = ptr to module globals
post_wimp_filter:
	STMFD	sp!, {r0-r2, r8, r9, r11, lr}
	MRS	r8, CPSR
	MOV	r9, r12

	@ Convert 0x8000 logical address to its physical address
	SUB	sp, sp, #12
	MOV	r0, #0
	STR	r0, [sp, #0]
	STR	r0, [sp, #8]
	MOV	r0, #0x8000
	STR	r0, [sp, #4]
	MOV	r0, #(1 << 9) | (1 << 13)
	MOV	r1, sp
	MOV	r2, #1
	SWI	XOS_Memory
	LDRVC	r2, [sp, #8]
	ADD	sp, sp, #12
	BVS	postfilter__exit

	LDR	r0, [r9, #GLOBALS_APP_LIST + LINKLIST_FIRST]
	TEQ	r0, #0
	BEQ	postfilter__exit
0:
	LDR	r1, [r0, #APP_ID]
	TEQ	r1, r2
	BEQ	postfilter__app_found
	LDR	r0, [r0, #APP_LINK + LINKHDR_NEXT]
	TEQ	r0, #0
	BNE	0b
	B	postfilter__exit
postfilter__app_found:
	ADR	r11, data_abort_support_block
	STR	r0, [r11, #ABORT_BLOCK_CURRENT_APP]
	LDR	r1, [r0, #APP_ABORT_HANDLER_LIST + LINKLIST_FIRST]
	STR	r1, [r11, #ABORT_BLOCK_FIRST_HANDLER]
	LDR	r1, [r0, #APP_STACK_LIST + LINKLIST_FIRST]
	STR	r1, [r11, #ABORT_BLOCK_FIRST_STACK]
	.if ABORT_COLLECT_STATS == 1
	@ r0 = ptr to APP object
	MOV	r1, #0
	STR	r1, [r0, #APP_ABORT_LAST_WIMP_COUNT]
	.endif
	@ Fall through
postfilter__exit:
	MSR	CPSR_f, r8
	LDMFD	sp!, {r0-r2, r8, r9, r11, pc}

@  r0 - Reason code 6
@  r1 - Requested change amount (signed page count)
@  r2 - Dynamic area number
@ r12 - Workspace pointer passed to OS_DynamicArea 0
	.global da_handler
da_handler:
@REPORT_TEXT "da_handler"
	CMP     r0, #1<<31
	CMNVC   r0, #1<<31
	ADR	r0, error
	MOV	pc, lr
error:
	.word	0
	.asciz	"DA handler called"
	.align

	.end
