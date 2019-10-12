@ som_handlers.s
@
@ Copyright 2019 GCCSDK Developers
@ Written by Lee Noar
@

.set USE_ABORT_HANDLER, 1
.set ABORT_HANDLER_MAPS_1_PAGE_ONLY, 1

.set XOS_ChangeEnvironment, 0x20040
.set XOS_Memory, 0x20068
.set XOS_ClaimProcessorVector, 0x20069
.set XOS_ValidateAddress, 0x2003A
.set OS_DynamicArea, 0x66
.set XOS_DynamicArea, 0x20066
.set XOS_ReadSysInfo, 0x20058
.set XOS_EnterOS, 0x20016
.set XOS_Memory, 0x20068

.set ReadSysInfo_read_abort, 7

.set DATA_ABORT_VECTOR, 0x3
.set VECTOR_CLAIM, (1 << 8)
.set VECTOR_RELEASE, (0 << 8)

.set RT_WORKSPACE_SIZE, 20

.set ABORT_HANDLER_PASS_ON,		0
.set ABORT_HANDLER_CLAIM,		1

			  .struct	0
CLIENT_PREV:			.skip	4
CLIENT_NEXT:			.skip	4
CLIENT_ID:			.skip	4
CLIENT_OBJECT_LIST_FIRST:	.skip	4
CLIENT_OBJECT_LIST_LAST:	.skip	4
CLIENT_OBJECT_LIST_COUNT:	.skip	4

@ This struct layout should be kept in sync with struct _som_client_object
@ in som.h
			  .struct	0
CLIENT_OBJECT_PREV:		.skip	4
CLIENT_OBJECT_NEXT:		.skip	4
CLIENT_OBJECT_VDATA_START:	.skip	4
CLIENT_OBJECT_VDATA_END:	.skip	4
CLIENT_OBJECT_DATA_SEG:		.skip	4
CLIENT_OBJECT_GOT_ADDR:		.skip	4
CLIENT_OBJECT_LIBRARY:		.skip	4
CLIENT_OBJECT_MAP:		.skip	4
CLIENT_OBJECT_UNMAP:		.skip	4
CLIENT_OBJECT_NUM_PAGES:	.skip	4
CLIENT_OBJECT_IS_MAPPED:	.skip	4
CLIENT_OBJECT_SIZE:

				.struct	0
ABORT_BLOCK_LIST:		.skip	4
ABORT_BLOCK_DA_NUM:		.skip	4
ABORT_BLOCK_SIZE:

				.struct	0
FILTER_BLOCK_LIST:		.skip	4
FILTER_BLOCK_DA_NUM:		.skip	4
FILTER_BLOCK_SIZE:

				.struct	0
@ All objects that are in use have an index used to access the runtime
@ and gott_base arrays.
rt_workspace_OBJECT_INDEX:	.skip	4
@ An array of Global Offset Table pointers used at runtime by the PIC
@ instruction sequence.
rt_workspace_GOTT_BASE:		.skip	4
@ An array used at runtime by the current client.
rt_workspace_RUNTIME_ARRAY: 	.skip	4
@ Pointer to the current client structure.
rt_workspace_CLIENT_STRUCT_PTR: .skip	4


	.macro	REPORT_HEX int
	STMFD	sp!,{r0-r2,r14}
	MOV	r0, \int
	SUB	sp, sp, #12
	MOV	r1, sp
	MOV	r2, #12
	SWI	0xd4
	MOV	r2, #0
	STRB	r2, [r1, #0]
	SWI	0x74c80
	ADD	sp, sp, #12

	LDMFD	sp!,{r0-r2,r14}
	.endm

	.macro	REPORT_DEC int
	STMFD	sp!,{r0-r2,r14}
	MOV	r0, \int
	SUB	sp, sp, #12
	MOV	r1, sp
	MOV	r2, #12
	SWI	0xdc
	MOV	r2, #0
	STRB	r2, [r1, #0]
	SWI	0x74c80
	ADD	sp, sp, #12

	LDMFD	sp!,{r0-r2,r14}
	.endm

	.macro REPORT_TEXT string

	SWI	0x74c81
	.asciz	"\string"
	.align

	.endm

	.macro REPORT_TEXT_R0

	STR	lr, [sp, #-4]!
	SWI	0x74c80
	LDR	lr, [sp], #4

	.endm

	.macro	PRINT_HEX int
	STMFD	sp!,{r0-r3,r14}
	MOV	r0,\int
	SUB	sp, sp, #12
	MOV	r1, sp
	MOV	r2,#12
	SWI	0xd4
	MOV	r2,#0
	STRB	r2,[r1,#0]
	SWI	0x2
	ADD	sp, sp, #12

	LDMFD	sp!,{r0-r3,r14}
	.endm

	.macro	PRINT_DEC int
	STMFD	sp!,{r0-r3,r14}
	MOV	r0,\int
	SUB	sp, sp, #12
	MOV	r1, sp
	MOV	r2,#12
	SWI	0xdc
	MOV	r2,#0
	STRB	r2,[r1,#0]
	SWI	0x2
	ADD	sp, sp, #12

	LDMFD	sp!,{r0-r3,r14}
	.endm

	.macro PRINT_NL

	SWI	0x10D
	SWI	0x10A

	.endm

	.macro PRINT_TEXT string

	STR	lr, [sp, #-4]!

	SWI	0x1
	.asciz	"\string"
	.align

	LDR	lr, [sp], #4

	.endm

	.text

@ Entry:
@  Called in SVC mode
@  r1 = address of fault
@  r11 = ptr to block of registers at point of abort (r0-r15, cpu flags)
@  r12 = ptr to user data block
@ Exit:
@  r0 = ABORT_HANDLER_CLAIM if able to handle, otherwise ABORT_HANDLER_PASS_ON
@  r12 can be corrupted
@  All other registers preserved
	.global som_abort_handler
som_abort_handler:
	STMFD	sp!, {r1-r3, r10, lr}

	LDR	r10, [r12, #ABORT_BLOCK_LIST]
0:
	TEQ	r10, #0
	MOVEQ	r0, #ABORT_HANDLER_PASS_ON
	LDMEQFD	sp!, {r1-r3, r10, pc}

	LDR	r2, [r10, #CLIENT_OBJECT_VDATA_START]
	LDR	r3, [r10, #CLIENT_OBJECT_VDATA_END]
	CMP	r1, r2
	LDRLO	r10, [r10, #CLIENT_OBJECT_NEXT]
	BLO	0b
	CMP	r1, r3
	LDRHS	r10, [r10, #CLIENT_OBJECT_NEXT]
	BHS	0b

	@ If the object is already mapped, then the data abort must be
	@ for another reason? Not sure if this is possible, but pass on
	@ to the default handler anyway.
	LDR	r0, [r10, #CLIENT_OBJECT_IS_MAPPED]
	TEQ	r0, #0
	MOVNE	r0, #ABORT_HANDLER_PASS_ON
	LDMNEFD	sp!, {r1-r3, r10, pc}

	MOV	r0, #22
	LDR	r1, [r12, #ABORT_BLOCK_DA_NUM]
	LDR	r2, [r10, #CLIENT_OBJECT_MAP]
	LDR	r3, [r10, #CLIENT_OBJECT_NUM_PAGES]
	SWI	XOS_DynamicArea
	MOVVS	r0, #ABORT_HANDLER_PASS_ON
	LDMVSFD	sp!, {r1-r3, r10, pc}

	MOV	r0, #1
	STR	r0, [r10, #CLIENT_OBJECT_IS_MAPPED]

	MOV	r0, #ABORT_HANDLER_CLAIM
	LDMFD	sp!, {r1-r3, r10, pc}

	.global	som_pre_wimp_filter
som_pre_wimp_filter:
	STMFD	sp!, {r0-r11, lr}
	MRS	r8, CPSR

	LDR	r10, [r12, #FILTER_BLOCK_LIST]
0:
	TEQ	r10, #0
	BEQ	1f

	LDR	r0, [r10, #CLIENT_OBJECT_IS_MAPPED]
	TEQ	r0, #0
	LDREQ	r10, [r10, #CLIENT_OBJECT_NEXT]
	BEQ	0b		@ Object not mapped, continue to next one
	MOV	r0, #22
	LDR	r1, [r12, #FILTER_BLOCK_DA_NUM]
	LDR	r2, [r10, #CLIENT_OBJECT_UNMAP]
	LDR	r3, [r10, #CLIENT_OBJECT_NUM_PAGES]
	SWI	XOS_DynamicArea
	@ Mark object as being unmapped
	MOVVC	r0, #0
	STRVC	r0, [r10, #CLIENT_OBJECT_IS_MAPPED]

	LDR	r10, [r10, #CLIENT_OBJECT_NEXT]
	B	0b
1:
	@ When we get here r10 is 0
	STR	r10, [r12, #FILTER_BLOCK_LIST]
99:
	MSR	CPSR_f, r8
	LDMFD	sp!, {r0-r11, pc}
ELFID:
	.word	0x464c457f

	.global	som_post_wimp_filter
som_post_wimp_filter:
	STMFD	sp!, {r0-r11, lr}
	MRS	r8, CPSR

	MOV	r0, #24
	MOV	r1, #0x8000
	ADD	r2, r1, #4
	SWI	XOS_Memory
	TST	r1, #1
	BEQ	99f

	MOV	r0, #0x8000
	LDR	r1, [r0, #0]
	LDR	r7, ELFID
	TEQ	r1, r7
	BNE	99f

	LDRH	r1, [r0, #40]
	SUB	r1, r1, #RT_WORKSPACE_SIZE
	ADD	r0, r0, r1
	LDR	r9, [r0, #rt_workspace_CLIENT_STRUCT_PTR]
	TEQ	r9, #0
	BEQ	99f

	LDR	r0, [r9, #CLIENT_OBJECT_LIST_FIRST]
	STR	r0, [r12, #FILTER_BLOCK_LIST]

.if USE_ABORT_HANDLER == 0
	MOV	r7, r0
0:
	TEQ	r7, #0
	BEQ	1f

	LDR	r0, [r7, #CLIENT_OBJECT_IS_MAPPED]
	TEQ	r0, #0
	LDRNE	r7, [r7, #CLIENT_OBJECT_NEXT]
	BNE	0b		@ Object mapped, continue to next one

	MOV	r0, #22
	LDR	r1, [r12, #FILTER_BLOCK_DA_NUM]
	LDR	r2, [r7, #CLIENT_OBJECT_MAP]
	LDR	r3, [r7, #CLIENT_OBJECT_NUM_PAGES]
	SWI	XOS_DynamicArea
	@ Mark object as being mapped
	MOVVC	r0, #1
	STRVC	r0, [r7, #CLIENT_OBJECT_IS_MAPPED]

	LDR	r7, [r7, #CLIENT_OBJECT_NEXT]
	B	0b
1:
.endif
99:
	MSR	CPSR_f, r8
	LDMFD	sp!, {r0-r11, pc}

@  r0 - Reason code 6
@  r1 - Requested change amount (signed page count)
@  r2 - Dynamic area number
@ r12 - Workspace pointer passed to OS_DynamicArea 0
	.global som_da_handler
som_da_handler:
REPORT_TEXT "som_da_handler"
	CMP     r0, #1<<31
	CMNVC   r0, #1<<31
	ADR	r0, error
	MOV	pc, lr

error:
	.word	0x16711671
	.asciz	"SOM DA handler called"
	.align

	.end
