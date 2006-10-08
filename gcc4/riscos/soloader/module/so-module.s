@;* Module for managing GCC shared objects
@;* Written by Lee Noar
@;* Started 15/7/2004
@;* License: GPL v2

	.include	"../utils/macros.h"
	.include	"../utils/swis.h"
	.include	"so-module.h"
	.include	"../rt_workspace.h"
	.include	"../structs.h"

@;=====================================================================
@; Layout of global variables addressed from r12, prefixed with GV_
@;=====================================================================
		  	.struct	0
GV_DA_NUMBER:	  	.skip	4
GV_DA_MAX_SIZE:	  	.skip	4

GV_FLAGS:	  	.skip	4

GV_CLIENT_LIST:	  	.skip	LIST_STRUCT_SIZE @; List of clients known to system
GV_GLOBAL_OBJECT_LIST:	.skip	LIST_STRUCT_SIZE @; List of libraries loaded by system

GV_CACHED_CLIENT_PTR:	.skip	4	@; The last client struct ptr that was found when searching the list
GV_CACHED_CLIENT_ID:	.skip	4	@; The ID of the last client found when searching the list

GV_BADVALUE:		.skip	4	@; A constant that is returned by SOM_Prologue if something goes wrong
GV_RESOLVER_PLT_ADDR:	.skip	4
GV_RESOLVER_PLTGOT_ADDR:.skip	4
GV_LAST_ERROR_BLOCK_PTR:.skip	4
GV_CALL_EVERY_CS_DELAY:	.skip	4
GV_MAX_IDLE_TIME:	.skip	4	@; Number of centiseconds a library is idle in memory before being removed
GV_CALLBACK_PENDING:	.skip	4
GV_NO_CLIENT_CHECK:	.skip	4
GV_32BIT_HOST:		.skip	4	@; Flag indicating whether we're running on a 32bit OS

GV_LIBRARY_DA_NUMBER:	.skip	4	@; DA Number for library code
GV_LIBRARY_DA_BASE:	.skip	4	@; Base address of library DA

GV_SIZE:

	.text

@;=====================================================================
@; end of global variables
@;=====================================================================

.equ	LD_FLAG_CALL_EVERY_ENABLED,	1 << 0

.equ	N_Flag,		0x80000000	@; Negative flag
.equ	Z_Flag,		0x40000000	@; Zero flag
.equ	C_Flag,		0x20000000	@; Carry flag
.equ	V_Flag,		0x10000000	@; Overflow flag
.equ	I_Flag,		0x08000000	@; IRQ disable
.equ	F_Flag,		0x04000000	@; FIRQ disable

.equ	SOM_MAX_DA_SIZE,		1024 * 1024 @; 1Mb
.equ	SOM_INIT_DA_SIZE,		4 * 1024 @; 4Kb (multiple of 4Kb)

.equ	SOM_MAX_LIBDA_SIZE,		1024 * 1024 * 128 @; 128MB
.equ	SOM_INIT_LIBDA_SIZE,		4 * 1024 @; 4KB

.equ	SWI_CHUNK,			0x58580

.equ	SOM_CALL_EVERY_CS_DELAY,	(1 * 100 * 60) + 1	@; 1 minute
.equ	SOM_MAX_IDLE_TIME,		(30 * 100 * 60)		@; 30 minutes
.equ	SOM_BAD_VALUE,			0x1BADBAD1
.equ	SOM_HEAP_INC,			(64 * 1024)		@; 64Kb (multiple of 4Kb)
.equ	SOM_ALLOCATOR_ID1,		0x584D4F53		@; SOMX
.equ	SOM_ALLOCATOR_ID2,		0x444D4F53		@; SOMD
.equ	SOM_ALLOCATOR_ID3,		0x4C4D4F53		@; SOML

.equ	DEBUG,	0

.equ	LIBRARIES_IN_DA,	0

@; MACRO FIND_CLIENT:
@; entry:
@;  r0 = ID of required client
@; exit:
@;  r0 = client ptr
@;  corrupts r14
@;
@; If required ID is the same as the cached ID then return the cached ptr.
@; This saves a branch and search.
	.macro	FIND_CLIENT

	MOV	r14, #0x8000
	LDR	r0, [r14, #RT_WORKSPACE_CLIENT_ID]
	LDR	r14, [r12, #GV_CACHED_CLIENT_ID]
	TEQ	r0, r14
	LDREQ	r0, [r12, #GV_CACHED_CLIENT_PTR]
	BLNE	find_client	@; OK for 32 bit OS - not relying on flag status after conditional branch

	.endm

	.text

	.word	0			@; Module start code
	.word	init			@; Module initialisation code
	.word	final			@; Module finialisation code
	.word	0			@; Module service call code
	.word	title			@; Module title
	.word	help			@; Module help string
	.word	cmd_handler		@; Module * command table
	.word	SWI_CHUNK		@; Module SWI chunk number
	.word	swi_handler		@; Module SWI handler code
	.word	swi_table		@; Module SWI name table
	.word	0			@; Module SWI name code
	.word	0
	.word	flags

title:
	.asciz	"SoManager"

help:
	.asciz	"SO Manager	1.03 (29 Aug, 2006)  Lee Noar"

da_name:
	.asciz	"SO Manager"

	.align
flags:
	.word	1

@;##############################################################
@; Module initialisation code
@;##############################################################
init:
	STMFD	sp!,{r7,r8,lr}

	@;* create a dynamic area for memory allocation
	MOV	r0, #DYNAMIC_AREA_REASON_CREATE
	MOV	r1, #-1
	MOV	r2, #SOM_INIT_DA_SIZE
	MOV	r3, #-1
	MOV	r4, #0b10000000 @; not draggable
	MOV	r5, #SOM_MAX_DA_SIZE
	MOV	r6, #0
	MOV	r7, #0
	ADR	r8, da_name
	SWI	XOS_DynamicArea
	BVS	99f

	STR	r3, [r12]	@; store base address of DA in module private word
	MOV	r12, r3

	@; zero global vars
	MOV	r2, #0
	MOV	r3, #GV_SIZE
	MOV	r4, r12
0:
	STR	r2, [r4], #4
	SUBS	r3, r3, #4
	BNE	0b

	STR	r1, [r12, #GV_DA_NUMBER]
	STR	r5, [r12, #GV_DA_MAX_SIZE]

	.if	DEBUG == 1
	ADD	r0, r12, #GV_SIZE
	MOV	r1, #SOM_INIT_DA_SIZE
	SUB	r1, r1, #GV_SIZE
	MOV	r2, #0
	BL	fillmem
	.endif

	LDR	r0, .C0
	STR	r0, [r12, #GV_BADVALUE]

	LDR	r0, .C0 + 4
	STR	r0, [r12, #GV_CALL_EVERY_CS_DELAY]

	LDR	r0, .C0 + 8
	STR	r0, [r12, #GV_MAX_IDLE_TIME]

	@; create a heap in the dynamic area
	MOV	r0, #HEAP_REASON_INIT
	ADD	r1, r12, #GV_SIZE
	MOV	r3, #SOM_INIT_DA_SIZE
	SUB	r3, r3, #GV_SIZE
	SWI	XOS_Heap
	BVS	99f

	TEQ	pc, pc
	MOVNE	r0, #0
	MOVEQ	r0, #1
	STR	r0, [r12, #GV_32BIT_HOST]

	.if	LIBRARIES_IN_DA == 1

	TEQ	r0, #1
	BNE	1f

	BL	create_library_DA
	BVS	99f

	.endif
1:
	BL	som_start_call_every

	TEQ	pc, pc
	CMPNE	pc, #0
	MSREQ	cpsr_f, #C_Flag

	MOV	r0, #0
	LDMFD	sp!,{r7,r8,pc}
99:
	LDMFD	sp!,{r7,r8,lr}
	TEQ	pc, pc
	ORRNES	pc, lr, #V_Flag
	MSR	cpsr_f, #V_Flag
	MOV	pc, lr
.C0: @; Constants
	.word	SOM_BAD_VALUE
	.word	SOM_CALL_EVERY_CS_DELAY
	.word	SOM_MAX_IDLE_TIME
	.word	SOM_ALLOCATOR_ID1	@; +12 (SOMX)
	.word	SOM_ALLOCATOR_ID2	@; +16 (SOMD)
	.word	SOM_ALLOCATOR_ID3	@; +20 (SOML)

	.if	LIBRARIES_IN_DA == 1

libda_name:
	.asciz	"DSO Libraries"
	.align

create_library_DA:
	STMFD	sp!, {r1-r8,lr}

	MOV	r0, #DYNAMIC_AREA_REASON_CREATE
	MOV	r1, #-1
	MOV	r2, #SOM_INIT_LIBDA_SIZE
	MOV	r3, #-1
	MOV	r4, #0b10000000 @; not draggable
	MOV	r5, #SOM_MAX_LIBDA_SIZE
	MOV	r6, #0
	MOV	r7, #0
	ADR	r8, libda_name
	SWI	XOS_DynamicArea
	BVS	99f

	STR	r1, [r12, #GV_LIBRARY_DA_NUMBER]
	STR	r3, [r12, #GV_LIBRARY_DA_BASE]

	@; Create a heap in the DA
	MOV	r0, #HEAP_REASON_INIT
	MOV	r1, r3
	MOV	r3, #SOM_INIT_LIBDA_SIZE
	SWI	XOS_Heap
99:
	LDMFD	sp!, {r1-r8,pc}

	.endif

@;##############################################################
@; Module finalisation code
@;##############################################################

final:
	LDR	r12, [r12]

	LDR	r0, [r12, #GV_NO_CLIENT_CHECK]
	TEQ	r0, #0
	BNE	3f

	LDR	r0, [r12, #(GV_CLIENT_LIST + LIST_LINK_COUNT)]
	TEQ	r0, #0
	BEQ	3f

	ADRL	r0, error_10
	TEQ	r0, r0
	TEQ	pc, pc
	ORRNES	pc, lr, #V_Flag
	MSR	cpsr_f, #V_Flag
	MOV	pc, lr
3:
	STMFD	sp!,{lr}

	BL	som_stop_call_every

	.if	LIBRARIES_IN_DA == 1

	@; Don't bother individually freeing library memory on 32bit OS
	@; as deleting the DA will free them all in one go anyway.
	LDR	r0, [r12, #GV_32BIT_HOST]
	TEQ	r0, #0
	BNE	1f

	.endif

	LDR	r4, [r12, #(GV_GLOBAL_OBJECT_LIST + LIST_FIRST_LINK)]
0:
	TEQ	r4, #0
	BEQ	1f

	LDR	r2, [r4, #OBJECT_BASE_ADDR]
	TEQ	r2, #0
	BNE	2f

	CMP	r2, #0x8000
	MOVGT	r0, #7
	SWIGT	OS_Module
2:
	LDR	r4, [r4, #(OBJECT_LINK_HDR + LINK_NEXT)]
	B	0b
1:
	.if	LIBRARIES_IN_DA == 1

	LDR	r0, [r12, #GV_32BIT_HOST]
	TEQ	r0, #0
	BEQ	4f

	LDR	r1, [r12, #GV_LIBRARY_DA_NUMBER]
	TEQ	r1, #0
	MOVNE	r0, #DYNAMIC_AREA_REASON_REMOVE
	SWINE	XOS_DynamicArea

	.endif
4:
	MOV	r0, #DYNAMIC_AREA_REASON_REMOVE
	LDR	r1, [r12, #GV_DA_NUMBER]
	SWI	XOS_DynamicArea

	MOV	r0, #0
	LDMFD	sp!, {pc}

swi_handler:
	LDR	r12, [r12]
	CMP	r11, #(1f - 0f) / 4
	ADDCC	pc, pc, r11, lsl#2
	B	1f
0:
	B	som_prologue
	B	som_resolver
	B	som_alloc		@; memory allocation from dynamic area for data
	B	som_free
	B	som_alloc_lib		@; memory allocation for code
	B	som_register_object
	B	som_deregister_client
	B	som_deregister_sharedobject
	B	som_query_object
	B	som_iterate_objects
	B	som_got_from_addr
	B	som_handle_from_addr
	B	som_handle_from_name
	B	som_addr_to_offset
1:
	ADR	r0, bad_swi
	TEQ	pc, pc
	ORRNES	pc, lr, #V_Flag
	MSR	cpsr_f, #V_Flag
	MOV	pc, lr

bad_swi:
	.word	0
	.asciz	"Bad SoManager SWI"
	.align

@;* Memory allocation for data, this can be from a dynamic area
@;*
@;* entry:
@;*  r0 = size required
@;* exit:
@;*  r0 = pointer to block if successful (V flag clear) otherwise pointer to error (and V flag set)
som_alloc:
	STMFD	sp!,{r1-r5,lr}

	MOV	r5, #0		@; flag indicating whether we've already extended the heap

 	@; Add 4 bytes to allocation so we can add an identifier
	ADD	r4, r0, #4	@; Save arg for reuse
0:
	MOV	r3, r4
	ADD	r1, r12, #GV_SIZE
	MOV	r0, #HEAP_REASON_CLAIM
	SWI	XOS_Heap
	@; If this fails, then assume heap is full. The failure could be due to a corrupted heap
	@; or invalid r1, but we can't distinguish here.
	BVS	97f
	MOVS	r0, r2 @; I doubt we'll ever see 0 here, but just in case.
	BEQ	97f

	@; Store an ID word at the start of the memory to show that we allocated it
	LDR	r1, .C0 + 12
	STR	r1, [r0], #4	@; return ptr to address after ID word

	LDMFD	sp!,{r1-r5,pc}
97: @; Out of memory error
	@; If we've already extended the heap and it still fails, then don't try again
	TEQ	r5, #0
	BNE	98f

	@; Try to increase heap size and try again

	@; Double the allocation request
	ADD	r3, r4, r4

	@; Round up allocation request to multiple of 4Kb
	ADD	r3, r3, #0xf00
	ADD	r3, r3, #0x0ff
	BIC	r3, r3, #0xf00
	BIC	r3, r3, #0x0ff

	@; extend DA by (((allocation request * 2) + 0xfff) & ~0xfff)
	LDR	r0, [r12, #GV_DA_NUMBER]
	MOV	r1, r3
	SWI	XOS_ChangeDynamicArea
	BVS	99f

	@; extend heap by same amount
	MOV	r3, r1
	MOV	r0, #HEAP_REASON_CHANGE_HEAP_SIZE
	ADD	r1, r12, #GV_SIZE
	SWI	XOS_Heap
	BVC	0b
98: @; Really out of memory error
	LDMFD	sp!,{r1-r5,lr}
	MOV	r0, #9
	B	error_exit

99: @; SWI error
	LDMFD	sp!,{r1-r5,lr}
	TEQ	pc, pc
	ORRNES	pc, lr, #V_Flag
	MSR	cpsr_f, #V_Flag
	MOV	pc, lr

@;* entry:
@;*  r0 = ptr to block to free
@;* exit:
@;*  All registers preserved, flags corrupted
som_free:
	STMFD	sp!,{r0-r2,lr}

	TEQ	r0, #0
	BNE	1f
	.if DEBUG == 1
	PRINT_TEXT "Attempt to free NULL pointer\r\n"
	.endif
	B	0f
1:
 	@; Validate the ID word and ignore if we didn't allocate the memory
 	@; in the first case
 	LDR	r1, .C0 + 12
 	LDR	r2, [r0, #-4]!
 	TEQ	r1, r2
 	BNE	99f

	MOV	r2, #0
	STR	r2, [r0, #0]

	MOV	r2, r0
	ADD	r1, r12, #GV_SIZE
	MOV	r0, #HEAP_REASON_RELEASE
	SWI	XOS_Heap
0:
	LDMFD	sp!,{r0-r2,pc}
99:
	.if DEBUG == 1
	PRINT_TEXT "Not SOM allocated: "
	PRINT_HEX r0
	PRINT_TEXT "\r\n"
	.endif
	LDMFD	sp!,{r0-r2,pc}

@;* Memory allocation for code, this could be from a dynamic area on a 32 bit OS,
@;* otherwise from the RMA.
@;*
@;* entry:
@;*  r0 = size required
@;* exit:
@;*  r0 = pointer to block if successful, otherwise 0
som_alloc_lib:
	STMFD	sp!,{r1-r5,lr}

 	@; Add 4 bytes to allocation so we can add an identifier
	ADD	r5, r0, #4

	.if	LIBRARIES_IN_DA == 1

	LDR	r0, [r12, #GV_32BIT_HOST]
	TEQ	r0, #0
	BNE	1f

	.endif

	@; The 26bit OS code
	MOV	r3, r5
	MOV	r0, #6		@; Claim memory
	SWI	XOS_Module
	BVS	99f

	@; Store an ID word at the start of the memory to show that we allocated it
	MOV	r0, r2
	LDR	r1, .C0 + 20
	STR	r1, [r0], #4	@; return ptr to address after ID word

	LDMFD	sp!,{r1-r5,pc}

	.if	LIBRARIES_IN_DA == 1
1:
	@; The 32bit OS code
	MOV	r4, #0		@; flag indicating whether we've already extended the heap
0:
	MOV	r3, r5
	LDR	r1, [r12, #GV_LIBRARY_DA_BASE]
	MOV	r0, #HEAP_REASON_CLAIM
	SWI	XOS_Heap
	@; If this fails, then assume heap is full. The failure could be due to a corrupted heap
	@; or invalid r1, but we can't distinguish here.
	BVS	97f
	MOVS	r0 ,r2
	BEQ	97f

	@; Store an ID word at the start of the memory to show that we allocated it
	LDR	r1, .C0 + 20
	STR	r1, [r0], #4	@; return ptr to address after ID word

	LDMFD	sp!, {r1-r5,pc}
97: @; Out of memory error
	@; If we've already extended the heap and it still fails, then don't try again
	@; otherwise we could end up in an infinite loop
	TEQ	r4, #0
	BNE	98f

	@; Try to increase heap size and try again
	MOV	r3, r5

	@; Round up allocation request to multiple of 4KB
	ADD	r3, r3, #0xf00
	ADD	r3, r3, #0x0ff
	BIC	r3, r3, #0xf00
	BIC	r3, r3, #0x0ff

	@; extend DA by ((allocation request + 0xfff) & ~0xfff)
	LDR	r0, [r12, #GV_LIBRARY_DA_NUMBER]
	MOV	r1, r3
	SWI	XOS_ChangeDynamicArea
	BVS	99f

	@; extend heap by same amount
	MOV	r3, r1
	MOV	r0, #HEAP_REASON_CHANGE_HEAP_SIZE
	LDR	r1, [r12, #GV_LIBRARY_DA_BASE]
	SWI	XOS_Heap
	MOV	r4, #1	@; Flag the fact that we've extended the DA/heap
	BVC	0b
98: @; Really out of memory error
	LDMFD	sp!,{r1-r5,lr}
	MOV	r0, #9
	B	error_exit

	.endif

99: @; SWI error
	LDMFD	sp!,{r1-r5,lr}
	TEQ	pc, pc
	ORRNES	pc, lr, #V_Flag
	MSR	cpsr_f, #V_Flag
	MOV	pc, lr

@;* entry:
@;*  r0 = ptr to base addr of library
som_free_lib:
	STMFD	sp!,{r0-r2,lr}

	LDR	r1, .C0 + 20
	LDR	r2, [r0, #-4]!
	TEQ	r1, r2
	BNE	99f

	@; Clear the identifier
	MOV	r2, #0
	STR	r2, [r0, #0]

	.if	LIBRARIES_IN_DA == 1

	LDR	r1, [r12, #GV_32BIT_HOST]
	TEQ	r1, #0
	BNE	0f

	.endif

	MOV	r2, r0
	MOV	r0, #7
	SWI	XOS_Module
	LDMFD	sp!,{r0-r2,pc}

	.if	LIBRARIES_IN_DA == 1
0:
	MOV	r2, r0
	LDR	r1, [r12, #GV_LIBRARY_DA_BASE]
	MOV	r0, #HEAP_REASON_RELEASE
	SWI	XOS_Heap

	.endif
99:
	LDMFD	sp!,{r0-r2,pc}

@;* entry:
@;*  r8 = PLT address
@;* exit:
@;*  r8 = private GOT ptr
@;*
som_prologue:
	@; Fast case, check the cached values
	STR	r0, [sp, #-4]!

	MOV	r11, #0x8000
	LDR	r0, [r11, #RT_WORKSPACE_CLIENT_ID]
	LDR	r10, [r12, #GV_CACHED_CLIENT_ID]
	TEQ	r10, r0
	LDREQ	r0, [r11, #RT_WORKSPACE_CACHED_PLT]
	TEQEQ	r0, r8
	LDREQ	r8, [r11, #RT_WORKSPACE_CACHED_PLTGOT]
	LDREQ	r0, [sp], #4
	MOVEQ	pc, lr

	STR	lr, [sp, #-4]!

	FIND_CLIENT	@; corrupts r0 & r14

	TEQ	r0, #0
	BEQ	99f

	@; check libraries
	LDR	r0, [r0, #(CLIENT_OBJ_LIST + LIST_FIRST_LINK)]
0:
	TEQ	r0, #0
	BEQ	99f

	LDR	r14, [r0, #OBJECT_END_ADDR]
	CMP	r8, r14
	LDRHI	r0, [r0, #(OBJECT_LINK_HDR + LINK_NEXT)]
	BHI	0b
	@; fall through with r0 = object pointer if successful
	STR	r8, [r11, #RT_WORKSPACE_CACHED_PLT]
	LDR	r8, [r0, #OBJECT_PRIVATE_GOT_PTR]
	STR	r8, [r11, #RT_WORKSPACE_CACHED_PLTGOT]

	LDR	lr, [sp], #4
	LDR	r0, [sp], #4
	MOV	pc, lr
99:
 PRINT_TEXT "Library not found for address &"
 PRINT_HEX r8

	LDR	r8, [r12, #GV_BADVALUE]

	LDR	lr, [sp], #4
	LDR	r0, [sp], #4
	MOV	pc, lr

@; entry:
@;  r8 = &GOT[n+3] (pointer to function)
@; exit:
@;  r0 = &GOT[n+3] (pointer to function)
@;  r8 = &GOT[0]
som_resolver:
	MOV	r0, r8
	MOV	r12, #0x8000
	LDR	r8, [r12, #RT_WORKSPACE_CACHED_PLTGOT]

	MOV	pc, lr

@;* entry:
@;*  r0 = pointer to structure to be linked in
@;*  r1 = pointer to list structure
@;* exit:
@;*  all registers preserved
list_add_tail:
	STMFD	sp!,{r2,r3}

	LDR	r2, [r1, #LIST_LAST_LINK]	@; old_last_link
	MOV	r3, #0
	STR	r3, [r0, #LINK_NEXT]		@; r0->next = 0
	STR	r2, [r0, #LINK_PREV]		@; r0->prev = old_last_link

	TEQ	r2, #0
	STREQ	r0, [r1, #LIST_FIRST_LINK]	@; r1->first = r0
	STRNE	r0, [r2, #LINK_NEXT]		@; old_last_link->next = r0

	STR	r0, [r1, #LIST_LAST_LINK]	@; r1->last = r0

	LDR	r3, [r1, #LIST_LINK_COUNT]
	ADD	r3, r3, #1			@; r1->count++
	STR	r3, [r1, #LIST_LINK_COUNT]

	LDMFD	sp!,{r2,r3}
	MOV	pc, lr

@;* entry:
@;*  r0 = pointer to structure to be removed
@;*  r1 = pointer to list structure
@;* exit:
@;*  all registers preserved
list_remove:
	STMFD	sp!,{r2,r3}

	LDR	r2, [r0, #LINK_NEXT]	@; link->next
	LDR	r3, [r0, #LINK_PREV]	@; link->prev

	TEQ	r2, #0
	STREQ	r3, [r1, #LIST_LAST_LINK]
	STRNE	r3, [r2, #LINK_PREV]

	TEQ	r3, #0
	STREQ	r2, [r1, #LIST_FIRST_LINK]
	STRNE	r2, [r3, #LINK_NEXT]

	LDR	r2, [r1, #LIST_LINK_COUNT]
	SUB	r2, r2, #1
	STR	r2, [r1, #LIST_LINK_COUNT]

	LDMFD	sp!,{r2,r3}
	MOV	pc, lr

@;* Add a shared object to a linked list so that the list is ordered by increasing load address
@;* of library.
@;* entry:
@;*  r0 = pointer to OBJECT_* structure to be linked in
@;*  r1 = pointer to list structure
@;* exit:
@;*  all registers preserved
list_add_sharedobject:
	STMFD	sp!,{r2-r4,lr}

	LDR	r2, [r1, #LIST_FIRST_LINK]
	TEQ	r2, #0
	BNE	0f

	LDMFD	sp!,{r2-r4,lr}
	B	list_add_tail		@; list is empty
0:
	TEQ	r2, #0	@; not sure that this test & branch is needed
	BEQ	99f

	LDR	r3, [r0, #OBJECT_BASE_ADDR]	@; The address of the library being inserted
	LDR	r4, [r2, #OBJECT_BASE_ADDR]	@; The address of the library in the list
	CMP	r3,r4
	BGE	1f

	STR	r2, [r0, #(OBJECT_LINK_HDR + LINK_NEXT)]	@; r0(new_link)->next = r2(list_link)
	LDR	r3, [r2, #(OBJECT_LINK_HDR + LINK_PREV)]
	STR	r3, [r0, #(OBJECT_LINK_HDR + LINK_PREV)]	@; r0(new_link)->prev = r2(list_link)->prev
	TEQ	r3, #0					@; if inserting before 1st link then inserted becomes the 1st link
	STREQ	r0, [r1, #LIST_FIRST_LINK]
	STRNE	r0, [r3, #(OBJECT_LINK_HDR + LINK_NEXT)]	@; otherwise r2(list_link)->prev->next = r0(new_link)
	STR	r0, [r2, #(OBJECT_LINK_HDR + LINK_PREV)]	@; r2(list_link)->prev = r0(new_link)
	B	99f
1:
	LDR	r3, [r2, #(OBJECT_LINK_HDR + LINK_NEXT)]
	TEQ	r3, #0
	MOVNE	r2, r3
	BNE	0b					@; if this isn't the last link, then advance to next

	@; If this is the last link, then add link to end of list
	@; At this point, we know that r3 = 0
	STR	r0,[r2,#(OBJECT_LINK_HDR + LINK_NEXT)]	@; r2(list_link)->next = r0(new_link)
	STR	r3,[r0,#(OBJECT_LINK_HDR + LINK_NEXT)]	@; r0(new_link)->next = 0
	STR	r2,[r0,#(OBJECT_LINK_HDR + LINK_PREV)]	@; r0(new_link)->prev = r2(list_link)
99:
	LDR	r3, [r1, #LIST_LINK_COUNT]
	ADD	r3, r3, #1		@; r1->count++
	STR	r3, [r1, #LIST_LINK_COUNT]

	LDMFD	sp!,{r2-r4,pc}

@;* entry:
@;*  r0 = handle
@;*  r1 = ptr to OBJINFO_* struct
som_register_client:
	STMFD	sp!,{r0-r7,lr}

	MOV	r4, r0
	MOV	r5, r1

	@; Allocate memory for client structure
	MOV	r0, #CLIENT_STRUCT_SIZE
	BL	som_alloc
	BVS	99f

	@; r0 = client ptr
	MOV	r6, r0

	@; Find a unique ID that hasn't already been used. There is a danger that if
	@; multiple tasks are run in quick succession, they may get the same centisecond
	@; count (and therefore ID) from OS_ReadMonotonicTime. Make sure this doesn't
	@; happen.
	@; We do this here before the call to list_add_tail(), so that the new client
	@; is not considered by find_client().
	SWI	XOS_ReadMonotonicTime
0:
	MOV	r7, r0
	BL	find_client	@; should return 0 if the new ID is unique
	TEQ	r0, #0
	ADDNE	r0, r7, #1	@; if ID is not unique, add 1 and retry
	BNE	0b

	@; r7 contains unique ID

	MOV	r0, r6
	ADD	r1, r12, #GV_CLIENT_LIST
	BL	list_add_tail

	STR	r7, [r6, #CLIENT_UNIQUE_ID]
	MOV	r14, #0x8000
	STR	r7, [r14, #RT_WORKSPACE_CLIENT_ID]

	@; Cache the new client as it's likely to be used a lot in the near future
	STR	r7, [r12, #GV_CACHED_CLIENT_ID]
	STR	r6, [r12, #GV_CACHED_CLIENT_PTR]

	MOV	r2, #0
	STR	r2, [r14, #RT_WORKSPACE_CACHED_PLT]
	STR	r2, [r14, #RT_WORKSPACE_CACHED_PLTGOT]

	@; zero the library list structure
	STR	r2, [r6,#CLIENT_OBJ_LIST + LIST_FIRST_LINK]
	STR	r2, [r6,#CLIENT_OBJ_LIST + LIST_LAST_LINK]
	STR	r2, [r6,#CLIENT_OBJ_LIST + LIST_LINK_COUNT]

	@; Use the object name given in the OBJINFO_* struct as the client name
	LDR	r0, [r5, #OBJINFO_NAME]
	BL	strlen
	ADD	r0, r0, #1
	BL	som_alloc
	STR	r0, [r6, #CLIENT_NAME]
	LDR	r1, [r5, #OBJINFO_NAME]
	BL	strcpy

	@; Zero the object name for the client list object to stop init_object
	@; from making a copy. Rather than have the name copied twice, once for
	@; the client and once for the client object, we'll use the same copy
	@; for both.
	MOV	r0, #0
	STR	r0, [r5, #OBJINFO_NAME]

	@; Allocate an object struct to record the details of the client in its
	@; object list. This object is not in the global list - only library objects
	@; exist in the global list.
	MOV	r0, #OBJECT_STRUCT_SIZE
	BL	som_alloc
	BVS	99f

	STR	r4, [r0, #OBJECT_HANDLE]

	MOV	r1, r5
	BL	init_object

	@; Use the same copy of the client's name for it and its object.
	LDR	r1, [r6, #CLIENT_NAME]
	STR	r1, [r0, #OBJECT_NAME]

	@; The object list is ordered by base addr.
	ADD	r1, r6, #CLIENT_OBJ_LIST
	BL	list_add_sharedobject

	@; Although the list is kept ordered, the app object should always be the first,
	@; and so this value should never change throughout the life of the client.
	MOV	r1, #0x8000
	STR	r0, [r1, #RT_WORKSPACE_OBJECT_LIST]

	LDMFD	sp!,{r0-r7,pc}
99:
	LDMFD	sp!,{r0-r7,lr}
	MOV	r0, #0
	B	error_exit
98:
	LDMFD	sp!,{r0-r7,lr}
	MOV	r0, #4
	B	error_exit
97:
	LDMFD	sp!,{r0-r7,lr}
	MOV	r0, #5
	B	error_exit

@;* SWI "SOM_RegisterObject"
@;* entry:
@;*  r0 = Reason code
@;*  r1 = handle
@;*  r2 = ptr to OBJINFO_* struct
@;* exit:
@;*  all other registers preserved if no error,
@;*  otherwise r0 = ptr to error block

som_register_object:
	STMFD	sp!,{r0-r2,lr}

	TEQ	r0, #SOM_REGISTER_LOADER
	BNE	0f

	MOV	r0, r1
	MOV	r1, r2

	LDR	r2, [r1, #OBJINFO_FLAGS]
	BIC	r2, r2, #OBJECT_FLAG_TYPE_MASK
	ORR	r2, r2, #OBJECT_FLAG_TYPE_LOADER
	STR	r2, [r1, #OBJINFO_FLAGS]

	BL	som_register_sharedobject
	BVS	98f

	MOV	r14, #0x8000
	LDR	r0, [r0, #OBJECT_PRIVATE_GOT_PTR]
	STR	r0, [r14, #RT_WORKSPACE_LD_GOT]

	B	3f
0:
 	TEQ	r0, #SOM_REGISTER_CLIENT
	BNE	2f

	MOV	r0, r1
	MOV	r1, r2

	LDR	r2, [r1, #OBJINFO_FLAGS]
	BIC	r2, r2, #OBJECT_FLAG_TYPE_MASK
	ORR	r2, r2, #OBJECT_FLAG_TYPE_CLIENT
	STR	r2, [r1, #OBJINFO_FLAGS]

	BL	som_register_client
	BVS	98f

	B	3f
2:
	TEQ	r0, #SOM_REGISTER_LIBRARY
	BNE	99f

	MOV	r0, r1
	MOV	r1, r2

	LDR	r2, [r1, #OBJINFO_FLAGS]
	BIC	r2, r2, #OBJECT_FLAG_TYPE_MASK
	ORR	r2, r2, #OBJECT_FLAG_TYPE_SHARED
	STR	r2, [r1, #OBJINFO_FLAGS]

	BL	som_register_sharedobject
	BVS	98f
3:	@; normal exit
	LDMFD	sp!,{r0-r2,pc}
98:	@; error exit - error returned by another routine
	LDMFD	sp!,{r0-r2,lr}
	LDR	r0, [r12, #GV_LAST_ERROR_BLOCK_PTR]
	MOV	pc, lr
99:	@; error exit - error generated by this routine
	LDMFD	sp!,{r0-r2,lr}
	MOV	r0, #6
	B	error_exit

@;* exit:
@;*  all registers preserved
som_deregister_client:
	STMFD	sp!,{r0-r3,lr}

	FIND_CLIENT
	TEQ	r0, #0
	BEQ	99f

	MOV	r3, r0	@; keep client ptr safe for freeing later

	@; The first link in the client's object list should be the application itself.
	@; This is treated differently as it is not a shared object as such (ie. it
	@; doesn't exist in the global list). This means that it should not be
	@; deregistered like normal objects.
	LDR	r1, [r3, #(CLIENT_OBJ_LIST + LIST_FIRST_LINK)]

	@; Check that it is the application object.
	LDR	r2, [r1, #OBJECT_FLAGS]
	TST	r2, #OBJECT_FLAG_TYPE_CLIENT
	BEQ	1f

	@; Get the next object before freeing the memory, otherwise we may get undefined
	@; behaviour.
	LDR	r2, [r1, #(OBJECT_LINK_HDR + LINK_NEXT)]

	@; Free the object that describes the client. Note that name is borrowed from the
	@; client structure and so the memory is not freed here. It is freed below.
	MOV	r0, r1
	BL	som_free

	@; Now loop through the rest of the objects in the list and deregister them.
	MOV	r1, r2
1:
	TEQ	r1, #0
	BEQ	2f

	@; Get the next object before deregistering, otherwise we may get undefined
	@; behaviour.
	LDR	r2, [r1, #(OBJECT_LINK_HDR + LINK_NEXT)]

	LDR	r0, [r1, #OBJECT_HANDLE]
	BL	som_deregister_sharedobject
	BVS	98f

	MOV	r1, r2
	B	1b
2:
	@; Invalidate the cached values. This must be done after any calls to
	@; other functions they may reset them to other values.
	MOV	r0, #0
	STR	r0, [r12, #GV_CACHED_CLIENT_ID]
	STR	r0, [r12, #GV_CACHED_CLIENT_PTR]

	@; Unlink from the list of clients
	MOV	r0, r3
	ADD	r1, r12, #GV_CLIENT_LIST
	BL	list_remove

	LDR	r0, [r3, #CLIENT_NAME]
	BL	som_free

	MOV	r0, r3
	BL	som_free

	LDMFD	sp!,{r0-r3,pc}
98:	@; error exit - error returned by another routine
	LDMFD	sp!,{r0-r3,lr}
	LDR	r0, [r12, #GV_LAST_ERROR_BLOCK_PTR]
	MOV	pc, lr
99:	@; error exit - error generated by this routine
	LDMFD	sp!,{r0-r3,lr}
	MOV	r0, #7
	B	error_exit


@;* entry:
@;*  r0 = new OBJECT_* ptr
@;*  r1 = old OBJECT_* ptr
@;* exit:
@;* all registers preserved
copy_object:
	STR	r2, [sp, #-4]!

	LDR	r2, [r1, #OBJECT_HANDLE]
	STR	r2, [r0, #OBJECT_HANDLE]
	LDR	r2, [r1, #OBJECT_BASE_ADDR]
	STR	r2, [r0, #OBJECT_BASE_ADDR]
	LDR	r2, [r1, #OBJECT_END_ADDR]
	STR	r2, [r0, #OBJECT_END_ADDR]
	LDR	r2, [r1, #OBJECT_RW_PTR]
	STR	r2, [r0, #OBJECT_RW_PTR]
	LDR	r2, [r1, #OBJECT_RW_SIZE]
	STR	r2, [r0, #OBJECT_RW_SIZE]
	LDR	r2, [r1, #OBJECT_GOT_ADDR]
	STR	r2, [r0, #OBJECT_GOT_ADDR]
	LDR	r2, [r1, #OBJECT_BSS_ADDR]
	STR	r2, [r0, #OBJECT_BSS_ADDR]
	LDR	r2, [r1, #OBJECT_BSS_SIZE]
	STR	r2, [r0, #OBJECT_BSS_SIZE]
	LDR	r2, [r1, #OBJECT_DYNAMIC_ADDR]
	STR	r2, [r0, #OBJECT_DYNAMIC_ADDR]
	LDR	r2, [r1, #OBJECT_DYNAMIC_SIZE]
	STR	r2, [r0, #OBJECT_DYNAMIC_SIZE]
	LDR	r2, [r1, #OBJECT_NAME]
	STR	r2, [r0, #OBJECT_NAME]
	LDR	r2, [r1, #OBJECT_FLAGS]
	STR	r2, [r0, #OBJECT_FLAGS]

	MOV	r2, #0
	STR	r2, [r0, #OBJECT_USAGE_COUNT]
	STR	r2, [r0, #OBJECT_EXPIRE_TIME]
	STR	r2, [r0, #OBJECT_PRIVATE_RW_PTR]
	STR	r2, [r0, #OBJECT_PRIVATE_GOT_PTR]
	STR	r2, [r0, #(OBJECT_LINK_HDR + LINK_PREV)]
	STR	r2, [r0, #(OBJECT_LINK_HDR + LINK_NEXT)]

	LDR	r2, [sp], #4
	MOV	pc, lr

@;* Initialise an OBJECT_* structure from an OBJINFO_* structure
@;* entry:
@;*  r0 = OBJECT_* ptr
@;*  r1 = OBJINFO_* ptr;
@;* exit:
@;*  all registers preserved
init_object:
	STMFD	sp!,{r0-r6,lr}

	LDR	r3, [r1, #OBJINFO_BASE_ADDR]
	STR	r3, [r0, #OBJECT_BASE_ADDR]
	LDR	r2, [r1, #OBJINFO_PUBLIC_RW_PTR]
	STR	r2, [r0, #OBJECT_RW_PTR]
	STR	r2, [r0, #OBJECT_PRIVATE_RW_PTR]
	LDR	r3, [r1, #OBJINFO_RW_SIZE]
	STR	r3, [r0, #OBJECT_RW_SIZE]
	ADD	r3, r2, r3			@; should give the end of the library's address space
	STR	r3, [r0, #OBJECT_END_ADDR]
	LDR	r3, [r1, #OBJINFO_GOT_OFFSET]
	ADD	r3, r2, r3
	STR	r3, [r0, #OBJECT_GOT_ADDR]
	STR	r3, [r0, #OBJECT_PRIVATE_GOT_PTR]
	LDR	r3, [r1, #OBJINFO_BSS_OFFSET]
	ADD	r3, r2, r3
	STR	r3, [r0, #OBJECT_BSS_ADDR]
	LDR	r3, [r1, #OBJINFO_BSS_SIZE]
	STR	r3, [r0, #OBJECT_BSS_SIZE]
	LDR	r3, [r1, #OBJINFO_DYN_OFFSET]
	ADD	r3, r2, r3
	STR	r3, [r0, #OBJECT_DYNAMIC_ADDR]
	LDR	r3, [r1, #OBJINFO_DYN_SIZE]
	STR	r3, [r0, #OBJECT_DYNAMIC_SIZE]
	LDR	r3, [r1, #OBJINFO_FLAGS]
	STR	r3, [r0, #OBJECT_FLAGS]

	MOV	r3, #0
	STR	r3, [r0, #OBJECT_USAGE_COUNT]
	STR	r3, [r0, #OBJECT_EXPIRE_TIME]

	@; Copy the name if there is one
	MOV	r4, r0	@; save OBJECT_* struct pointer
	MOV	r5, r1	@; save OBJINFO_* struct pointer
	LDR	r6, [r5, #OBJINFO_NAME]
	MOVS	r0, r6
	STREQ	r0, [r4, #OBJECT_NAME]
	BEQ	99f

	BL	strlen

	ADD	r0, r0, #1	@; Make space for terminator
	BL	som_alloc
	STR	r0, [r4, #OBJECT_NAME]
	BVS	99f

	MOV	r1, r6
	BL	strcpy
99:
	LDMFD	sp!,{r0-r6,pc}

@;* Retrieve information about the library whose handle is
@;* given. The data is placed in a user supplied buffer.
@;*
@;* entry:
@;*  r0 = handle of library
@;*  r1 = pointer to buffer to return library information in
@;*   r1 + 0 = base address
@;*   r1 + 4 = pointer to library's read/write segment
@;*   r1 + 8 = pointer to client copy of read/write segment (if applicable)
@;*   r1 + 12 = size of read/write segment
@;*   r1 + 16 = offset of GOT from start of read/write segment
@;*   r1 + 20 = offset of bss area from start of read/write segment
@;*   r1 + 24 = size of bss area
@;*   r1 + 28 = pointer to name (Read only)
@;*   r1 + 32 = flags
@;*  r2 = flags
@;*   bit 0 - set to search current client object list
@;*           clear to search global object list
@;* exit:
@;*  all registers preserved if object found and no error, or r0 = pointer to error block
som_query_object:
	STMFD	sp!,{r2-r5,lr}

	TEQ	r0, #0
	TEQNE	r1, #0
	MOVEQ	r0, #3	@; Bad argument
	BEQ	99f

	MOV	r5, r0

	TST	r2, #1
	LDREQ	r2, [r12, #(GV_GLOBAL_OBJECT_LIST + LIST_FIRST_LINK)]
	BEQ	0f

	FIND_CLIENT
	LDR	r2, [r0, #(CLIENT_OBJ_LIST + LIST_FIRST_LINK)]
0:
	TEQ	r2, #0
	MOVEQ	r0, #8	@; Object not found
	BEQ	99f

	LDR	r3, [r2, #OBJECT_HANDLE]
	TEQ	r3, r5
	LDRNE	r2, [r2, #(OBJECT_LINK_HDR + LINK_NEXT)]
	BNE	0b

	@; library found
	LDR	r3, [r2, #OBJECT_BASE_ADDR]
	STR	r3, [r1, #OBJINFO_BASE_ADDR]

	LDR	r3, [r2, #OBJECT_RW_PTR]
	STR	r3, [r1, #OBJINFO_PUBLIC_RW_PTR]

	LDR	r4, [r2, #OBJECT_PRIVATE_RW_PTR]
	STR	r4, [r1, #OBJINFO_PRIVATE_RW_PTR]

	LDR	r4, [r2, #OBJECT_RW_SIZE]
	STR	r4, [r1, #OBJINFO_RW_SIZE]

	LDR	r4, [r2, #OBJECT_GOT_ADDR]
	SUB	r4, r4, r3			@; find offset from r/w segment start
	STR	r4, [r1, #OBJINFO_GOT_OFFSET]

	LDR	r4, [r2, #OBJECT_BSS_ADDR]
	SUB	r4, r4, r3			@; find offset from r/w segment start
	STR	r4, [r1, #OBJINFO_BSS_OFFSET]

	LDR	r4, [r2, #OBJECT_BSS_SIZE]
	STR	r4, [r1, #OBJINFO_BSS_SIZE]

	LDR	r4, [r2, #OBJECT_DYNAMIC_ADDR]
	SUB	r4, r4, r3			@; find offset from r/w segment start
	STR	r4, [r1, #OBJINFO_DYN_OFFSET]

	LDR	r4, [r2, #OBJECT_DYNAMIC_SIZE]
	STR	r4, [r1, #OBJINFO_DYN_SIZE]

	LDR	r4, [r2, #OBJECT_NAME]
	STR	r4, [r1, #OBJINFO_NAME]

	LDR	r4, [r2, #OBJECT_FLAGS]
	STR	r4, [r1, #OBJINFO_FLAGS]

	LDMFD	sp!,{r2-r5,pc}
99:
	LDMFD	sp!,{r2-r5,lr}
	B	error_exit

@;* Register a library for the current client. If the library is not already
@;* registered in the global object list, then an OBJECT_* structure is
@;* allocated and placed in the global list. Then regardless of whether the
@;* library was in the global list or not, its OBJECT_* structure is cloned
@;* and placed in the client's list.

@;* entry:
@;*  r0 = handle
@;*  r1 = pointer to structure used to pass data about library
@;* exit:
@;*  r0 = pointer to OBJECT_* structure created for the client list if success
@;*	otherwise error block pointer
@;*  all other registers preserved if successful
som_register_sharedobject:
	STMFD	sp!,{r1-r8,lr}

	MOV	r4, r0		@; keep handle safe
	MOV	r5, r1		@; keep data struct ptr safe

	FIND_CLIENT
	TEQ	r0, #0
	BEQ	99f

	MOV	r6, r0		@; keep client pointer safe

	@; see if the library is already registered in the global list
	LDR	r8, [r12, #(GV_GLOBAL_OBJECT_LIST + LIST_FIRST_LINK)]
0:
	TEQ	r8, #0
	BEQ	1f

	@; use the handle as the key
	LDR	r1, [r8, #OBJECT_HANDLE]
	TEQ	r1, r4
	LDRNE	r8, [r8, #(OBJECT_LINK_HDR + LINK_NEXT)]
	BNE	0b
1:
	TEQ	r8, #0	@; r8 != 0 indicates that the library is already registered globally
	BNE	2f

	@; Library is not in the global list, so create a link for it and add it

	@; Allocate memory for object structure
	MOV	r0, #OBJECT_STRUCT_SIZE
	BL	som_alloc
	BVS	98f

	MOV	r8, r0		@; save OBJECT_* struct pointer
	MOV	r1, r5
	BL	init_object

	ADD	r1, r12, #GV_GLOBAL_OBJECT_LIST	@; address of global library list
	BL	list_add_sharedobject

	STR	r4, [r8, #OBJECT_HANDLE]
2:
	LDR	r1, [r8, #OBJECT_USAGE_COUNT]
	ADD	r1, r1, #1
	STR	r1, [r8, #OBJECT_USAGE_COUNT]

	@; Make a copy of the object for the client's list
	MOV	r0, #OBJECT_STRUCT_SIZE
	BL	som_alloc
	BVS	98f

	MOV	r1, r8
	BL	copy_object

	ADD	r1, r6, #CLIENT_OBJ_LIST	@; address of client's object list
	BL	list_add_sharedobject

	LDR	r3, [r5, #OBJINFO_PRIVATE_RW_PTR]
	STR	r3, [r0, #OBJECT_PRIVATE_RW_PTR]
	LDR	r4, [r5, #OBJINFO_GOT_OFFSET]
	ADD	r4, r3, r4			@; convert offset to address
	STR	r4, [r0, #OBJECT_PRIVATE_GOT_PTR]

@; STR r0, [sp, #-4]!
@; LDR r0, [r8, #OBJECT_BASE_ADDR]
@; BL som_debug_hex
@; BL som_debug_space
@; LDR r0, [r8, #OBJECT_NAME]
@; BL som_debug_text
@; BL som_debug_nl
@; LDR r0, [sp], #4

	LDMFD	sp!,{r1-r8,pc}
99:
	LDMFD	sp!,{r1-r8,lr}
	MOV	r0, #2
	B	error_exit
98:
	LDMFD	sp!,{r1-r8,lr}
	MOV	r0, #1
	B	error_exit

@;* entry:
@;*  r0 = handle
@;* exit:
@;*  all registers preserved
som_deregister_sharedobject:
	STMFD	sp!,{r0-r5,lr}

	MOV	r4, r0		@; keep handle safe

	FIND_CLIENT
	TEQ	r0, #0
	BEQ	99f

	MOV	r5, r0		@; keep client pointer safe

	@; first find the object in the client's list
	LDR	r2, [r5, #(CLIENT_OBJ_LIST + LIST_FIRST_LINK)]
0:
	TEQ	r2, #0
	BEQ	98f

	LDR	r1, [r2, #OBJECT_HANDLE]
	TEQ	r1, r4
	LDRNE	r2, [r2, #(OBJECT_LINK_HDR + LINK_NEXT)]
	BNE	0b

	@; next find the object in the global list
	LDR	r3, [r12, #(GV_GLOBAL_OBJECT_LIST + LIST_FIRST_LINK)]
1:
	TEQ	r3, #0
	BEQ	98f

	LDR	r1, [r3, #OBJECT_HANDLE]
	TEQ	r1, r4
	LDRNE	r3, [r3, #(OBJECT_LINK_HDR + LINK_NEXT)]
	BNE	1b

	@; At this point:
	@;  r2 = ptr to object in client list
	@;  r3 = ptr to object in global list

	@; remove object from client list
	MOV	r0, r2
	ADD	r1, r5, #CLIENT_OBJ_LIST
	BL	list_remove

	@; Free memory used to store object in client list
	BL	som_free

	@; now deal with global object
	LDR	r0, [r3, #OBJECT_USAGE_COUNT]
	SUBS	r0, r0, #1
	STR	r0, [r3, #OBJECT_USAGE_COUNT]
	BNE	2f

	@; Usage count is zero, so object is no longer in use at all. We
	@; don't remove it from memory straight away, but mark it for
	@; expiry.

	SWI	XOS_ReadMonotonicTime
	LDR	r1, [r12, #GV_MAX_IDLE_TIME]
	ADD	r0, r0, r1	@; Calculate expiry time
	STR	r0, [r3, #OBJECT_EXPIRE_TIME]
2:
	LDMFD	sp!,{r0-r5,pc}
98:
	LDMFD	sp!,{r0-r5,lr}
	MOV	r0, #8
	B	error_exit
99:
	LDMFD	sp!,{r0-r5,lr}
	MOV	r0, #7
	B	error_exit

@; Return pointer to structure of given client
@; entry:
@;  r0  = client ID
@;  r12 = pointer to global variables
@; exit:
@;  r0  = pointer to client structure
find_client:
	STMFD	sp!,{r1,r2}

	MOV	r1, r0

	LDR	r0, [r12, #GV_CLIENT_LIST + LIST_FIRST_LINK]	@; get first client in list
0:
	TEQ	r0, #0			@; end of list?
	BEQ	2f			@; yes, exit with r0 = 0

	LDR	r2, [r0, #CLIENT_UNIQUE_ID]
	TEQ	r1, r2			@; IDs match?
	LDRNE	r0, [r0, #(CLIENT_LINK_HDR + LINK_NEXT)]	@; no get next client
	BNE	0b			@; and go round again

	STR	r0, [r12, #GV_CACHED_CLIENT_PTR]	@; cache the last client found and exit with r0 = client
	STR	r1, [r12, #GV_CACHED_CLIENT_ID]
2:
	LDMFD	sp!,{r1,r2}
	MOV	pc,lr

@; Given an address return the handle of the library that contains it
@; entry:
@;  r0 = address
@; exit:
@;  r0 = handle
som_handle_from_addr:
	STMFD	sp!,{r1,r2,lr}

	MOV	r1, r0		@; keep parameter safe
	FIND_CLIENT
	TEQ	r0, #0
	BEQ	0f	@; exit with r0 = 0

	@; check objects of client
	LDR	r0, [r0, #(CLIENT_OBJ_LIST + LIST_FIRST_LINK)]
1:
	TEQ	r0, #0
	BEQ	0f	@; exit with r0 = 0

	LDR	r2, [r0, #OBJECT_END_ADDR]
	CMP	r1, r2
	LDRHI	r0, [r0, #(OBJECT_LINK_HDR + LINK_NEXT)]
	BHI	1b
	LDR	r0, [r0, #OBJECT_HANDLE]
	@; fall through to exit with r0 = handle if successful
0:
	LDMFD	sp!,{r1,r2,pc}

@; Given an address return the client's private GOT of the library that contains it
@; entry:
@;  r0 = address
@; exit:
@;  r0 = ptr to GOT or 0 if failed
som_got_from_addr:
	STMFD	sp!,{lr}

	MOV	r10, r0		@; keep parameter safe
	FIND_CLIENT
	TEQ	r0, #0
	BEQ	0f	@; exit with r0 = 0

	@; check libraries of client
	LDR	r0, [r0, #(CLIENT_OBJ_LIST + LIST_FIRST_LINK)]
1:
	TEQ	r0, #0
	BEQ	0f	@; exit with r0 = 0

	LDR	r14, [r0, #OBJECT_END_ADDR]
	CMP	r10, r14
	LDRHI	r0, [r0, #(OBJECT_LINK_HDR + LINK_NEXT)]
	BHI	1b
	@; fall through with r0 = object pointer if successful
	LDR	r0, [r0, #OBJECT_PRIVATE_GOT_PTR]
0:
	LDMFD	sp!,{pc}

@; Given ptr to library name, return handle of library
@; from global list
@; entry:
@;  r0 = ptr to name (Read Only)
@; exit:
@;  r0 = handle or 0 for failure
som_handle_from_name:
	STMFD	sp!,{r1-r5,lr}

	MOV	r5, r0	@; keep parameter safe

	@; Search global list, not current client
	LDR	r4, [r12, #(GV_GLOBAL_OBJECT_LIST + LIST_FIRST_LINK)]
1:
	TEQ	r4, #0
	MOVEQ	r0, #0
	BEQ	0f

	MOV	r0, #-1	@; Use current territory
	LDR	r1, [r4, #OBJECT_NAME]
	MOV	r2, r5
	MOV	r3, #1	@; Ignore case
	SWI	XTerritory_Collate	@; Sets processor flags according to result
	LDRNE	r4, [r4, #(OBJECT_LINK_HDR + LINK_NEXT)]
	BNE	1b

	LDR	r0, [r4, #OBJECT_HANDLE]
0:
	LDMFD	sp!,{r1-r5,pc}

@; entry:
@;  r0 = Reason code:
@;	0 = return first object in client list
@;	1 = return next object in list after that given in r1
@;	2 = return previous object in list before that given in r1
@;	3 = return last object in client list
@;  r1 = for r0 = 1 or r0 = 2 - internal handle as returned in r1
@; exit:
@;  r0 = handle of requested object (NULL if there is no object to return)
@;  r1 = internal handle of requested object for use as input in subsequent calls
@;	(NULL if there is no object to return)
som_iterate_objects:
	STMFD	sp!,{lr}

	TEQ	r0, #SOM_ITERATE_REASON_NEXT
	BNE	2f

	TEQ	r1, #0
	BEQ	97f
	LDR	r1, [r1, #(OBJECT_LINK_HDR + LINK_NEXT)]
	B	0f
2:
	TEQ	r0, #SOM_ITERATE_REASON_PREV
	BNE	3f

	TEQ	r1, #0
	BEQ	97f
	LDR	r1, [r1, #(OBJECT_LINK_HDR + LINK_PREV)]
	B	0f
3:
	TEQ	r0, #SOM_ITERATE_REASON_FIRST
	BNE	1f

	FIND_CLIENT
	TEQ	r0, #0
	BEQ	99f

	LDR	r1, [r0, #(CLIENT_OBJ_LIST + LIST_FIRST_LINK)]
	B	0f
1:
	TEQ	r0, #SOM_ITERATE_REASON_LAST
	BNE	98f

	FIND_CLIENT
	TEQ	r0, #0
	BEQ	99f

	LDR	r1, [r0, #(CLIENT_OBJ_LIST + LIST_LAST_LINK)]
0:
	TEQ	r1, #0
	LDRNE	r0, [r1, #OBJECT_HANDLE]
	MOVEQ	r0, #0

	LDMFD	sp!,{pc}
97:
	LDMFD	sp!,{lr}
	MOV	r0, #11
	MOV	r1, #0
	B	error_exit
98:
	LDMFD	sp!,{lr}
	MOV	r0, #6
	MOV	r1, #0
	B	error_exit
99:
	LDMFD	sp!,{lr}
	MOV	r0, #7
	MOV	r1, #0
	B	error_exit

som_addr_to_offset:
	STMFD	sp!,{r1,r2,lr}

	MOV	r1, r0
	FIND_CLIENT
	TEQ	r0, #0
	BEQ	0f	@; exit with r0 = 0

	@; check objects of client
	LDR	r0, [r0, #(CLIENT_OBJ_LIST + LIST_FIRST_LINK)]
1:
	TEQ	r0, #0
	BEQ	0f	@; exit with r0 = 0

	LDR	r2, [r0, #OBJECT_END_ADDR]
	CMP	r1, r2
	LDRHI	r0, [r0, #(OBJECT_LINK_HDR + LINK_NEXT)]
	BHI	1b

	LDR	r0, [r0, #OBJECT_BASE_ADDR]
	SUB	r0, r1, r0
0:
	LDMFD	sp!,{r1,r2,pc}

@; entry:
@;  r12 = ptr to global variables
som_start_call_every:
	STMFD	sp!, {r0-r3, lr}

	LDR	r3, [r12, #GV_FLAGS]
	TST	r3, #LD_FLAG_CALL_EVERY_ENABLED
	BNE	0f

	LDR	r0, [r12, #GV_CALL_EVERY_CS_DELAY]
	ADR	r1, som_call_every
	MOV	r2, r12
	SWI	XOS_CallEvery
	ORRVC	r3, r3, #LD_FLAG_CALL_EVERY_ENABLED
	STRVC	r3, [r12, #GV_FLAGS]
0:
	LDMFD	sp!, {r0-r3, pc}

@; entry:
@;  r12 = ptr to global variables
som_stop_call_every:
	STMFD	sp!, {r0-r2, lr}

	LDR	r2, [r12, #GV_FLAGS]
	TST	r2, #LD_FLAG_CALL_EVERY_ENABLED
	BEQ	0f

	ADR	r0, som_call_every
	MOV	r1, r12
	SWI	XOS_RemoveTickerEvent
	BICVC	r2, r2, #LD_FLAG_CALL_EVERY_ENABLED
	STRVC	r2, [r12, #GV_FLAGS]

	@; Ensure a callback doesn't occur after the module has gone
	LDR	r0, [r12, #GV_CALLBACK_PENDING]
	TEQ	r0, #1
	ADREQ	r0, som_call_back
	MOVEQ	r1, r12
	SWIEQ	XOS_RemoveCallBack
0:
	LDMFD	sp!, {r0-r2, pc}

som_call_every:
	STMFD	sp!, {r0,r1,lr}

	ADR	r0, som_call_back
	MOV	r1, r12
	SWI	XOS_AddCallBack

	MOV	r0, #1
	STR	r0, [r12, #GV_CALLBACK_PENDING]

	LDMFD	sp!, {r0,r1,pc}

som_call_back:
	STMFD	sp!, {r0-r3,lr}

	MOV	r0, #0
	STR	r0, [r12, #GV_CALLBACK_PENDING]

	SWI	XOS_ReadMonotonicTime
	MOV	r2, r0

	LDR	r3, [r12, #(GV_GLOBAL_OBJECT_LIST + LIST_FIRST_LINK)]
0:
	TEQ	r3, #0
	BEQ	1f

	@; Ignore libraries that are still in use
	LDR	r1, [r3, #OBJECT_USAGE_COUNT]
	TEQ	r1, #0
	BNE	2f	@; Still in use, goto next

	@; The dynamic loader should never be removed from the global list
	@; UPDATE: Dynamic loader is now treated like any other library
@;	LDR	r1, [r3, #OBJECT_FLAGS]
@;	TST	r1, #OBJECT_FLAG_TYPE_LOADER
@;	BNE	2f	@; Ignore dynamic loader, goto next

	@; Check object expiry time against current time
	LDR	r1, [r3, #OBJECT_EXPIRE_TIME]
	CMP	r1, r2
	BGT	2f	@; Not time for this object to expire yet, goto next

	@; If we get here, object has expired, remove from global list
	MOV	r0, r3
	ADD	r1, r12, #GV_GLOBAL_OBJECT_LIST
	BL	list_remove
	LDR	r0, [r3, #OBJECT_NAME]
	BL	som_free			@; free memory used by name
	LDR	r0, [r3, #OBJECT_BASE_ADDR]
	BL	som_free_lib			@; free memory used by library code
	MOV	r0, r3
	BL	som_free			@; free memory holding object
2:
	LDR	r3, [r3, #(OBJECT_LINK_HDR + LINK_NEXT)]
	B	0b
1:
	LDMFD	sp!, {r0-r3,pc}

swi_table:					@; 0x58580 +
	.asciz	"SOM"				@; SWI name prefix
	.asciz	"Prologue"			@; 0x0
	.asciz	"Resolver"			@; 0x1
	.asciz	"Alloc"				@; 0x2
	.asciz	"Free"				@; 0x3
	.asciz	"AllocLib"			@; 0x4
	.asciz	"RegisterObject"		@; 0x5
	.asciz	"DeregisterClient"		@; 0x6
	.asciz	"DeregisterSharedObject"	@; 0x7
	.asciz	"QueryObject"			@; 0x8
	.asciz	"IterateObjects"		@; 0x9
	.asciz	"GOTFromAddr"			@; 0xA
	.asciz	"HandleFromAddr"		@; 0xB
	.asciz	"HandleFromName"		@; 0xC
	.asciz	"AddrToOffset"			@; 0xD
	.byte	0
	.align

@; Exit a module entry point correctly setting the V flag and returning
@; an error
@; entry:
@;  r0 = error index
error_exit:
	STR	r1, [sp, #-4]!
	ADR	r1, swi_error_table
	LDR	r0, [r1, r0, LSL#2]
	ADR	r1, swi_error_0
	ADD	r0, r1, r0
	STR	r0, [r12, #GV_LAST_ERROR_BLOCK_PTR]
	LDR	r1, [sp], #4
	TEQ	r0, r0
	TEQ	pc, pc
	ORRNES	pc, lr, #V_Flag
	MSR	cpsr_f, #V_Flag
	MOV	pc, lr

swi_error_0:
	.word	SOM_ERROR_BASE + 0
	.asciz	"Unable to register client, out of memory"
	.align
swi_error_1:
	.word	SOM_ERROR_BASE + 1
	.asciz	"Unable to register shared object, out of memory"
	.align
swi_error_2:
	.word	SOM_ERROR_BASE + 2
	.asciz	"Unable to register shared object, client not found"
	.align
swi_error_3:
	.word	SOM_ERROR_BASE + 3
	.asciz	"Could not query library, bad argument"
	.align
swi_error_4:
	.word	SOM_ERROR_BASE + 4
	.asciz	"Client already registered"
	.align
swi_error_5:
	.word	SOM_ERROR_BASE + 5
	.asciz	"Client registration failed"
	.align
swi_error_6:
	.word	SOM_ERROR_BASE + 6
	.asciz	"Bad reason code"
	.align
swi_error_7:
	.word	SOM_ERROR_BASE + 7
	.asciz	"Client not registered"
	.align
swi_error_8:
	.word	SOM_ERROR_BASE + 8
	.asciz	"Object not found"
	.align
swi_error_9:
	.word	SOM_ERROR_BASE + 9
	.asciz	"Out of memory"
	.align
error_10:
	.word	SOM_ERROR_BASE + 10
	.asciz	"There are still clients using SOManager."
	.align
error_11:
	.word	SOM_ERROR_BASE + 11
	.asciz	"Bad parameter"
	.align
swi_error_table:
	.word	swi_error_0 - swi_error_0
	.word	swi_error_1 - swi_error_0
	.word	swi_error_2 - swi_error_0
	.word	swi_error_3 - swi_error_0
	.word	swi_error_4 - swi_error_0
	.word	swi_error_5 - swi_error_0
	.word	swi_error_6 - swi_error_0
	.word	swi_error_7 - swi_error_0
	.word	swi_error_8 - swi_error_0
	.word	swi_error_9 - swi_error_0
	.word	error_10 - swi_error_0
	.word	error_11 - swi_error_0

@;:::::::::::::::::::::::::::::::::::::
@;	Star command handling
@;:::::::::::::::::::::::::::::::::::::

cmd_handler:
	.asciz	"SOMStatus"
	.align
	.word	cmd_status_code
	.byte	0		@; No min parameters
	.byte	0
	.byte	0		@; No max parameters
	.byte	0
	.word	cmd_status_syntax
	.word	cmd_status_help

	.asciz	"SOMAddress"
	.align
	.word	cmd_address_code
	.byte	1		@; 1 min parameter
	.byte	0
	.byte	1		@; 1 max parameter
	.byte	0
	.word	cmd_address_syntax
	.word	cmd_address_help

	.asciz	"SOMExpire"
	.align
	.word	cmd_expire_code
	.byte	0		@; 0 min parameter
	.byte	0
	.byte	2		@; 1 max parameter
	.byte	0
	.word	cmd_expire_syntax
	.word	cmd_expire_help

	.asciz	"SOMAllowQuit"
	.align
	.word	cmd_allowquit_code
	.byte	0		@; 1 min parameter
	.byte	0
	.byte	0		@; 1 max parameter
	.byte	0
	.word	cmd_allowquit_syntax
	.word	cmd_allowquit_help

	.byte	0

cmd_status_help:
	.ascii	"*SOMStatus displays information about the dynamic loader.\r\n"
cmd_status_syntax:
	.asciz	"Syntax: *SOMStatus"
cmd_address_help:
	.ascii	"*SOMAddress reports the shared library that contains the given address\r\n"
	.ascii	"and also the offset from the start of the library file where the address lies.\r\n"
	.ascii	"It is useful for debugging purposes.\r\n"
cmd_address_syntax:
	.asciz	"Syntax: *SOMAddress <address>"
cmd_expire_help:
	.ascii	"*SOMExpire allows the expiry time of any libraries used in the future\r\n"
	.ascii	"to be set. Specifiying no parameter returns the current setting.\r\n"
cmd_expire_syntax:
	.asciz	"Syntax: *SOMExpire [<n>h] [<n>m]"
cmd_allowquit_help:
	.ascii	"*SOMAllowQuit allows the module to be killed even if there are\r\n"
	.ascii	"still clients registered. It is used for developing the dynamic\r\n"
	.ascii	"loader and should generally be avoided.\r\n"
cmd_allowquit_syntax:
	.asciz	"Syntax: *SOMAllowQuit"

	.align

cmd_status_code:
	STMFD	sp!,{lr}

	LDR	r12,[r12]

	@;*
	@;* Display global library list
	@;*

	PRINT_TEXT "\n\n\rShared Object Manager Status\n\r----------------------------\n\r"

	PRINT_TEXT " Number of libraries loaded by system: "
	LDR	r0, [r12, #(GV_GLOBAL_OBJECT_LIST + LIST_LINK_COUNT)]
	BL	print_int
	PRINT_TEXT " - details follow:\n\n\r"

	LDR	r4, [r12, #(GV_GLOBAL_OBJECT_LIST + LIST_FIRST_LINK)]
0:
	TEQ	r4, #0
	BEQ	1f

	PRINT_TEXT "               Library name: "
	LDR	r0, [r4, #OBJECT_NAME]
	TEQ	r0, #0
	ADREQL	r0, null_lib_name
	SWI	XOS_Write0

	@; Address space: 0x00000000 -> 0x00000000 (size: 0x00000000)
	PRINT_TEXT "\n\r              Address space: "
	LDR	r3, [r4, #OBJECT_BASE_ADDR]
	MOV	r0, r3
	BL	print_hex
	PRINT_TEXT " -> "
	LDR	r0, [r4, #OBJECT_END_ADDR]
	BL	print_hex
	PRINT_TEXT " (size: "
	SUB	r0, r0, r3
	BL	print_hex

	PRINT_TEXT ")\n\r       R/W segment (public): "
	LDR	r0, [r4, #OBJECT_RW_PTR]
	BL	print_hex

	PRINT_TEXT " -> "
	LDR	r1, [r4, #OBJECT_RW_SIZE]
	ADD	r0, r0, r1
	BL	print_hex

	PRINT_TEXT " (size: "
	MOV	r0, r1
	BL	print_hex

	@; GOT ptr (public): 0x00000000
	PRINT_TEXT ")\n\r           GOT ptr (public): "
	LDR	r0, [r4, #OBJECT_GOT_ADDR]
	BL	print_hex

	@; Dynamic segment (public): 0x00000000
	PRINT_TEXT "\n\r   Dynamic segment (public): "
	LDR	r0, [r4, #OBJECT_DYNAMIC_ADDR]
	BL	print_hex

	PRINT_TEXT "\n\r             Time to expire: "
	LDR	r1, [r4, #OBJECT_USAGE_COUNT]
	TEQ	r1, #0
	BEQ	3f
	PRINT_TEXT "None, still in use"
	B	5f
3:
	LDR	r1, [r4, #OBJECT_EXPIRE_TIME]
	SWI	XOS_ReadMonotonicTime
	SUB	r0, r1, r0
	CMP	r0, #0
	BGT	6f
	PRINT_TEXT "Pending"
	B	5f
6:
	MOV	r1, #100
	BL	div	@; Convert from centi-seconds to seconds
	MOV	r1, #60
	BL	div	@; Convert from seconds to minutes
	MOV	r1, #60
	BL	div	@; Convert from minutes to hours and minutes
	BL	print_int
	PRINT_TEXT "h "
	MOV	r0, r1
	BL	print_int
	PRINT_TEXT "m"
5:
	PRINT_TEXT "\n\n\r"

	LDR	r4, [r4, #(OBJECT_LINK_HDR + LINK_NEXT)]
	B	0b
1:
	@;*
	@;* Display list of clients
	@;*

	PRINT_TEXT " Number of clients registered: "
	LDR	r0, [r12, #(GV_CLIENT_LIST + LIST_LINK_COUNT)]
	BL	print_int
	PRINT_TEXT "\n\r"

	LDR	r2, [r12, #(GV_CLIENT_LIST + LIST_FIRST_LINK)]
0:
	TEQ	r2, #0
	BEQ	1f

	PRINT_TEXT "\n\r           Client name: "
	LDR	r0, [r2, #CLIENT_NAME]
	SWI	XOS_Write0

	PRINT_TEXT "\n\r                    ID: "
	LDR	r0, [r2, #CLIENT_UNIQUE_ID]
	BL	print_hex

	;@ The 1st object in the client list is the client itself
	LDR	r1, [r2, #(CLIENT_OBJ_LIST + LIST_FIRST_LINK)]

	@; R/W segment: 0x00000000 -> 0x00000000 (size: 0x00000000)
	PRINT_TEXT "\n\r R/W segment (private): "
	LDR	r0, [r1, #OBJECT_PRIVATE_RW_PTR]
	BL	print_hex
	PRINT_TEXT " -> "
	LDR	r3, [r1, #OBJECT_RW_SIZE]
	ADD	r0, r0, r3
	BL	print_hex
	PRINT_TEXT " (size: "
	MOV	r0, r3
	BL	print_hex

	PRINT_TEXT ")\n\r     GOT ptr (private): "
	LDR	r0, [r1, #OBJECT_PRIVATE_GOT_PTR]
	BL	print_hex
	PRINT_TEXT "\n\n\r"

	@;*
	@;* Display details of libraries each client is using
	@;*

	LDR	r3, [r2, #(CLIENT_OBJ_LIST + LIST_FIRST_LINK)]
2:
	TEQ	r3, #0
	BEQ	4f

	LDR	r0, [r3, #OBJECT_BASE_ADDR]
	TEQ	r0, #0x8000
	LDREQ	r3, [r3, #(OBJECT_LINK_HDR + LINK_NEXT)]
	BEQ	2b

	PRINT_TEXT "            Library handle: "
	LDR	r0, [r3, #OBJECT_HANDLE]
	BL	print_hex
	PRINT_TEXT " ("
	LDR	r0, [r3, #OBJECT_NAME]
	SWI	XOS_Write0

	PRINT_TEXT ")\n\r     R/W segment (private): "
	LDR	r0, [r3, #OBJECT_PRIVATE_RW_PTR]
	BL	print_hex

	PRINT_TEXT " -> "
	LDR	r0, [r3, #OBJECT_PRIVATE_RW_PTR]
	LDR	r1, [r3, #OBJECT_RW_SIZE]
	ADD	r0, r0, r1
	BL	print_hex
	PRINT_TEXT " (size: "
	MOV	r0, r1
	BL	print_hex

	PRINT_TEXT ")\n\r         GOT ptr (private): "
	LDR	r0, [r3, #OBJECT_PRIVATE_GOT_PTR]
	BL	print_hex
	PRINT_TEXT "\n\r Dynamic segment (private): "
	LDR	r4, [r3, #OBJECT_DYNAMIC_ADDR]
	LDR	r0, [r3, #OBJECT_RW_PTR]
	SUB	r0, r4, r0
	LDR	r4, [r3, #OBJECT_PRIVATE_RW_PTR]
	ADD	r0, r4, r0
	BL	print_hex
	PRINT_TEXT "\n\n\r"

	LDR	r3, [r3, #(OBJECT_LINK_HDR + LINK_NEXT)]
	B	2b
4:
	LDR	r2, [r2, #(CLIENT_LINK_HDR + LINK_NEXT)]
	B	0b
1:
	LDMFD	sp!,{pc}

null_lib_name:
	.asciz	"Null library name - system may be corrupt"
	.align

print_hex:
	STMFD	sp!,{r0-r2,r14}
	SWI	OS_WriteI + '0'
	SWI	OS_WriteI + 'x'
	ADR	r1, conv_buffer
	MOV	r2, #20
	SWI	XOS_ConvertHex8
	MOV	r2, #0
	STRB	r2, [r1,#0]
	SWI	XOS_Write0

	LDMFD	sp!,{r0-r2,pc}
conv_buffer:
	.word	0,0,0,0,0

print_int:
	STMFD	sp!,{r0-r2,r14}
	ADR	r1, conv_buffer
	MOV	r2, #20
	SWI	XOS_ConvertCardinal4
	MOV	r2, #0
	STRB	r2, [r1,#0]
	SWI	XOS_Write0

	LDMFD	sp!,{r0-r2,pc}

cmd_address_code:
	STMFD	sp!,{lr}

	LDR	r12, [r12]

	MOV	r1, r0
	MOV	r0, #16
	SWI	XOS_ReadUnsigned

	LDR	r1, [r12,#(GV_GLOBAL_OBJECT_LIST + LIST_FIRST_LINK)]
1:
	TEQ	r1, #0
	BEQ	0f

	LDR	r0, [r1,#OBJECT_END_ADDR]
	CMP	r2, r0
	BGE	2f
	LDR	r0, [r1,#OBJECT_BASE_ADDR]
	CMP	r2, r0
	BLT	2f

	SUB	r0, r2, r0
	PRINT_TEXT "Address given is at offset "
	BL	print_hex
	PRINT_TEXT " of library:\n\r"
	LDR	r0, [r1,#OBJECT_NAME]
	SWI	XOS_Write0
	SWI	0x10d
	SWI	0x10a

	LDMFD	sp!,{pc}
2:
	LDR	r1, [r1,#(OBJECT_LINK_HDR + LINK_NEXT)]
	B	1b
0:
	PRINT_TEXT "Address given is not known to the Shared Object Manager\n\n\r"

	LDMFD	sp!,{pc}

cmd_expire_code:
	STMFD	sp!,{lr}

	LDR	r12, [r12]

	@; If no args given then return current expiry time
	TEQ	r1, #0
	BNE	0f
	PRINT_TEXT "Current expiry time is "
	LDR	r0, [r12, #GV_MAX_IDLE_TIME]
	MOV	r1, #100
	BL	div	@; Convert from centi-seconds to seconds
	MOV	r1, #60
	BL	div	@; Convert from seconds to minutes
	MOV	r1, #60
	BL	div	@; Convert from minutes to hours and minutes
	BL	print_int
	PRINT_TEXT "h "
	MOV	r0, r1
	BL	print_int
	PRINT_TEXT "m\r\n"

	B	1f
0:
	@; Parse the args, could be:
	@; <x>hr <y>m
	@; <x>hr
	@; <y>m
	@; Step through command tail searching for numbers, when one is found
	@; check the character after to identify it
	MOV	r4, r0
	MOV	r5, #0	@; Number of hours
	MOV	r6, #0	@; Number of minutes
2:
	LDRB	r0, [r4], #1
	CMP	r0, #' '
	BLT	3f
	CMP	r0, #'0'
	BLT	2b
	CMP	r0, #'9'
	BGT	2b

	MOV	r0, #10
	SUB	r1, r4, #1
	SWI	XOS_ReadUnsigned
	BVS	99f
	MOV	r4, r1
	LDRB	r0, [r4, #0]
	TEQ	r0, #'h'
	MOVEQ	r5, r2
	BEQ	2b
	TEQ	r0, #'m'
	MOVEQ	r6, r2
	BEQ	2b
99:
@;	ADR	r0, error_11
@;	PRINT_TEXT "Bad argument passed to *SOMExpire\r\n"
	LDMFD	sp!,{lr}
	MOV	r0, #11
	B	error_exit
3:
	MOV	r0, #60
	@; Convert hours to minutes and add minutes together
	MLA	r2, r5, r0, r6

	@; Convert to centiseconds
	MUL	r2, r0, r2	@; convert from minutes to seconds
	MOV	r0, #100
	MUL	r2, r0, r2	@; convert from seconds to centiseconds

	STR	r2, [r12, #GV_MAX_IDLE_TIME]
1:
	MOV	r0, #0
	LDMFD	sp!,{pc}

cmd_allowquit_code:
	STMFD	sp!,{r0, lr}

	LDR	r12, [r12]

	MOV	r0, #1
	STR	r0, [r12, #GV_NO_CLIENT_CHECK]

	LDMFD	sp!, {r0, pc}

	.if DEBUG == 1

@;============================
@; Debug code
@;============================
@;* entry:
@;*  r0 = ptr to text
@;* Dump text string to debugging file

som_debug_text:
	STMFD	sp!,{r0-r6,lr}

	MOV	r5, r0

	MOV	r0, #0xC3	@; Open existing file with r/w access
	ADR	r1, debug_filename
	SWI	XOS_Find
	BVS	99f
	MOVS	r6, r0
	BNE	0f

	MOV	r0, #0x83	@; Create new file with r/w access
	SWI	XOS_Find
	BVS	99f
	MOVS	r6, r0
	ADREQ	r0, filename_error
	BEQ	99f
0:
	MOV	r0, #2
	MOV	r1, r6
	SWI	XOS_Args
	BVS	99f
	MOV	r4, r2

	MOV	r0, r5
	BL	strlen
	MOV	r3, r0
	MOV	r0, #1
	MOV	r1, r6
	MOV	r2, r5
	SWI	XOS_GBPB

	MOV	r0, #0		@; Close file
	SWI	XOS_Find

	LDMFD	sp!,{r0-r6,pc}
99:
	SWI	XOS_Write0
	SWI	0x10D
	SWI	0x10A
	LDMFD	sp!,{r0-r6,pc}

@;* entry:
@;*  r0 = value to write to file
@;* Dump hex number as string to debugging file
som_debug_hex:
	STMFD	sp!, {r0-r6,lr}

	MOV	r5, r0

	MOV	r0, #0xC3	@; Open existing file with r/w access
	ADR	r1, debug_filename
	SWI	XOS_Find
	BVS	99f
	MOVs	r6, r0
	BNE	0f

	MOV	r0, #0x83	@; Create new file with r/w access
	SWI	XOS_Find
	BVS	99f
	MOVS	r6, r0
	ADREQ	r0, filename_error
	BEQ	99f
0:
	MOV	r0, #2
	MOV	r1, r6
	SWI	XOS_Args
	BVS	99f
	MOV	r4, r2

	MOV	r0, r5
	ADR	r1, hex_buffer
	MOV	r2, #20
	SWI	0x200D4
	BVS	99f

	MOV	r3, #8
	MOV	r0, #1
	MOV	r1, r6
	ADR	r2, hex_buffer
	SWI	XOS_GBPB

	MOV	r0, #0		@; Close file
	SWI	XOS_Find

	LDMFD	sp!, {r0-r6,pc}
99:
	SWI	XOS_Write0
	SWI	0x10D
	SWI	0x10A
	LDMFD	sp!, {r0-r6,pc}

debug_filename:
    .asciz	"RAM:$.SOM"
filename_error:
    .asciz	"Zero file handle returned"
    .align
hex_buffer:
    .word	0, 0, 0, 0

som_debug_space:
	STMFD	sp!, {r0-r3,lr}

	MOV	r0, #0xC3	@; Open existing file with r/w access
	ADR	r1, debug_filename
	SWI	XOS_Find
	BVS	99f
	MOVS	r3, r0
	BNE	0f

	MOV	r0, #0x83	@; Create new file with r/w access
	SWI	XOS_Find
	BVS	99f
	MOVS	r3, r0
	ADREQ	r0, filename_error
	BEQ	99f
0:
	MOV	r0, #2
	MOV	r1, r3
	SWI	XOS_Args	@; read extent
	MOVVC	r0, #1
	SWIVC	XOS_Args	@; write sequential file pointer
	BVS	99f

	MOV	r0, #' '
	SWI	XOS_BPut	@; write space character
	BVS	99f

	MOV	r0, #0		@; Close file
	SWI	XOS_Find
	BVS	99f

	LDMFD	sp!, {r0-r3,pc}
99:
	SWI	XOS_Write0
	SWI	0x10D
	SWI	0x10A
	LDMFD	sp!, {r0-r3,pc}

som_debug_nl:
	STMFD	sp!, {r0-r3,lr}

	MOV	r0, #0xC3	@; Open existing file with r/w access
	ADR	r1, debug_filename
	SWI	XOS_Find
	BVS	99f
	MOVS	r3, r0
	BNE	0f

	MOV	r0, #0x83	@; Create new file with r/w access
	SWI	XOS_Find
	BVS	99f
	MOVS	r3, r0
	ADREQ	r0, filename_error
	BEQ	99f
0:
	MOV	r0, #2
	MOV	r1, r3
	SWI	XOS_Args	@; read extent
	MOVVC	r0, #1
	SWIVC	XOS_Args	@; write sequential file pointer
	BVS	99f

	MOV	r0, #'\n'
	SWI	XOS_BPut	@; write space character

	MOV	r0, #0		@; Close file
	SWI	XOS_Find

	LDMFD	sp!, {r0-r3,pc}
99:
	SWI	XOS_Write0
	SWI	0x10D
	SWI	0x10A
	LDMFD	sp!, {r0-r3,pc}

	.endif @; DEBUG == 1

	.global	_start
_start:
	swi	XOS_WriteS
	.ascii	"This program is meant to be processed by OBJCOPY into\r\n"
	.asciz	"a RISCOS module and should not be run as an ELF executable."
	.align
	swi	XOS_Exit

	.end
