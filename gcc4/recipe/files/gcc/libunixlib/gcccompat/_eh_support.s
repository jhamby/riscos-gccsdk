@ Exception handling support
@ This source is used by the call frame C++ exception handling code.
@ Copyright (c) 2009-2011 UnixLib Developers

@ The SharedCLibrary versions of these routines may be called in either
@ USR mode or, in the case of a module, SVC mode. In the latter case,
@ the stack is flat rather than chunked, so a runtime check on the
@ processor mode is used to determine which stack type to expect.

#include "internal/asm_dec.s"

	.text
 
#define STACKCHUNK_MAGIC_NUMBER 0xF60690FF

#if __TARGET_SCL__
	@ Find start and end address of SharedCLibrary module so that we
	@ can determine whether an address lies within it.
	@
	@ entry:
	@   ip = pointer to scl_pos_block
	@ exit:
	@   ip[0] = start address of SCL
	@   ip[1] = end address of SCL (actually start address of next module)
	NAME	__ehs_init
__ehs_init:
	STMFD	sp!, {r0-r5}

	MOV	r0, #18
	ADR	r1, scl_name
	SWI	XOS_Module
	STR	r3, [ip, #0]
	MOV	r0, #12
	ADD	r1, r1, #1
	MOV	r2, #0
	SWI	XOS_Module
	STR	r3, [ip, #4]

	LDMFD	sp!, {r0-r5}
	MOV	pc, lr
scl_name:
	.asciz	"SharedCLibrary"
	.align
	DECLARE_FUNCTION __ehs_init

	.data

scl_pos_block:
	@ Start address of SharedCLibrary module.
	.word	0
	@ End address of SharedCLibrary module (technically start address of
	@ module after it).
	.word	0

	.text

#endif

	@ void *__ehs_return_address (void *fp)
	@
	@ Given a frame pointer, return real return address
	.global	__ehs_return_address
	NAME	__ehs_return_address
#if __TARGET_UNIXLIB__
__ehs_return_address:
	MOV	ip, sp
 PICNE "STMFD	sp!, {v1, fp, ip, lr, pc}"
 PICEQ "STMFD	sp!, {v1, v4, fp, ip, lr, pc}"
	SUB	fp, ip, #4

	MOVS	v1, a1
	MOVEQ	a1, #0
 PICNE "LDMEQEA	fp, {v1, fp, sp, pc}"
 PICEQ "LDMEQEA	fp, {v1, v4, fp, sp, pc}"

 PICEQ "LDR	v4, .L3"
.LPIC0:
 PICEQ "ADD	v4, pc, v4"		@ v4 = Library public GOT
 PICEQ "LDMIA	v4, {v4, v5}"		@ v4 = Object index, v4 = GOT table location
 PICEQ "LDR	v5, [v5, #0]"		@ v5 = GOT table
 PICEQ "LDR	v4, [v5, v4, LSL#4]"	@ v4 = Library private GOT

	@ Retrieve return address from stack frame
	LDR	a1, [v1, #-4]
	TEQ	a1, a1			@ 32bit mode check
	TEQ	pc, pc
	BICNE	a1, a1, #0xfc000003	@ If running 26bit, clear PSR bits.

	LDR	lr, .L1			@ __gcc_alloca_free
 PICEQ "LDR	lr, [v4, lr]"
	TEQ	a1, lr
	MOVEQ	a1, v1
	BLEQ	__gcc_alloca_return_address

	LDR	lr, .L2			@ __free_stack_chunk
 PICEQ "LDR	lr, [v4, lr]"
	TEQ	a1, lr
 PICNE "LDMNEEA	fp, {v1, fp, sp, pc}"
 PICEQ "LDMNEEA	fp, {v1, v4, fp, sp, pc}"

	@ Starting at the current chunk, step back through the chunk list
	@ until we find the one containing the given frame pointer. The
	@ return address we're looking for is in the _next_ chunk after
	@ that one.
	SUB	a2, sl, #512 + CHUNK_OVERHEAD
	LDR	a4, =STACKCHUNK_MAGIC_NUMBER

0:	@ chunk_loop
	CMP	v1, a2
	BLT	1f	@ next_chunk

	LDR	a3, [a2, #CHUNK_SIZE]
	ADD	a3, a2, a3

	CMP	v1, a3
	BLE	2f	@ found_chunk

1:	@ next_chunk
	LDR	a2, [a2, #CHUNK_PREV]
	TEQ	a2, #0
	MOVEQ	a1, #0
 PICNE "LDMEQEA	fp, {v1, fp, sp, pc}"
 PICEQ "LDMEQEA	fp, {v1, v4, fp, sp, pc}"

	@ Make sure we've got a valid chunk
	LDR	a3, [a2, #CHUNK_MAGIC]
	TEQ	a3, a4
	MOVNE	a1, #0
 PICNE "LDMNEEA	fp, {v1, fp, sp, pc}"
 PICEQ "LDMNEEA	fp, {v1, v4, fp, sp, pc}"

	B	0b	@ chunk_loop

2:	@ found_chunk
	LDR	a1, [a2, #CHUNK_NEXT]
	TEQ	a1, #0
	LDRNE	a1, [a1, #CHUNK_RETURN]
	LDREQ	a1, [a2, #CHUNK_RETURN]
 PICNE "LDMEA	fp, {v1, fp, sp, pc}"
 PICEQ "LDMEA	fp, {v1, v4, fp, sp, pc}"
.L1:
	WORD	__gcc_alloca_free
.L2:
	WORD	__free_stack_chunk
.L3:
 PICEQ ".word	_GLOBAL_OFFSET_TABLE_-(.LPIC0+4)"
#elif __TARGET_SCL__
	@ There's no PIC in SCL code
__ehs_return_address:
	MOV	ip, sp
 	STMFD	sp!, {v1, fp, ip, lr, pc}
	SUB	fp, ip, #4

	MOVS	v1, a1
	MOVEQ	a1, #0
	LDMEQEA	fp, {v1, fp, sp, pc}

	@ Retrieve return address from stack frame
	LDR	a1, [v1, #-4]
	TEQ	a1, a1			@ 32bit mode check
	TEQ	pc, pc
	BICNE	a1, a1, #0xfc000003	@ If running 26bit, clear PSR bits.

	LDR	lr, .L1			@ __gcc_alloca_free
	TEQ	a1, lr
	MOVEQ	a1, v1
	BLEQ	__gcc_alloca_return_address

#if __TARGET_MODULE__
	@ Check the processor mode to see if we have a chunked stack. If we do,
	@ then there are more tests to be done.
	TEQ	a1, a1
	TEQ	pc, pc
	MRSEQ	ip, cpsr
	MOVNE	ip, pc
	TST	ip, #3
	LDMNEEA	fp, {v1, fp, sp, pc}
#endif

	@ We can't determine the exact address of the stack chunk freeing function
	@ in the SCL in order to test for it here as we do in the UnixLib code.
	@ Instead, we check if the address lies within the SCL at all and if it
	@ does, we assume that it's the stack chunk freeing function.
	LDR	ip, .L2			@ scl_pos_block
	LDMIA	ip, {a2, a3}
	TEQ	a2, #0
	BNE	0f

	@ __ehs_init is called only once on the first run through.
	BL	__ehs_init
	LDMIA	ip, {a2, a3}
0:
	CMP	a1, a2
	LDMLOEA	fp, {v1, fp, sp, pc}

	CMP	a1, a3
	LDMHSEA	fp, {v1, fp, sp, pc}

	@ If we get here, then the return address is in the SharedCLibrary
	@ and is likely to be the function for freeing the stack chunk. The
	@ SharedCLibrary appears to create a dummy frame to record the
	@ real return address.
	LDR	v1, [v1, #-12]		@ next frame
	LDR	a1, [v1, #-4]		@ retrieve return addresss

	TEQ	a1, a1			@ 32bit mode check
	TEQ	pc, pc
	BICNE	a1, a1, #0xfc000003	@ If running 26bit, clear PSR bits.
	LDMEA	fp, {v1, fp, sp, pc}
.L1:
	.word	__gcc_alloca_free
.L2:
	.word	scl_pos_block
#else
#  error "Unsupported runtime"
#endif
	DECLARE_FUNCTION __ehs_return_address


	@ void *__ehs_stack_limit (void *sp)
	@
	@ Given a stack pointer, return the stack limit of the chunk
	@ that contains it. Used by uw_install_context() in unwind-dw2.c.
	.global	__ehs_stack_limit
	NAME	__ehs_stack_limit
__ehs_stack_limit:
#if __TARGET_MODULE__
	@ Check the processor mode to see if we have a chunked stack.
	TEQ	a1, a1
	TEQ	pc, pc
	MRSEQ	a2, cpsr
	MOVNE	a2, pc
	TST	a2, #3
	MOVNE	a1, sl
	MOVNE	pc, lr
#endif
	SUB	a2, sl, #512 + CHUNK_OVERHEAD
	LDR	a4, =STACKCHUNK_MAGIC_NUMBER

0:	@ chunk_loop
	CMP	a1, a2
	BLT	1f	@ next_chunk

	LDR	a3, [a2, #CHUNK_SIZE]
	ADD	a3, a2, a3

	CMP	a1, a3
	BLE	2f	@ found_chunk

1:	@ next_chunk
	LDR	a2, [a2, #CHUNK_PREV]
	TEQ	a2, #0
	MOVEQ	a1, #0
	MOVEQ	pc, lr

	@ Make sure we've got a valid chunk
	LDR	a3, [a2, #CHUNK_MAGIC]
	TEQ	a3, a4
	MOVNE	a1, #0
	MOVNE	pc, lr

	B	0b	@ chunk_loop

2:	@ found_chunk
	ADD	a1, a2, #512 + CHUNK_OVERHEAD
	MOV	pc, lr
	DECLARE_FUNCTION __ehs_stack_limit


	@ void __ehs_trim_stack (void)
	@
	@ In UnixLib, this a wrapper function for __trim_stack, in SCL we
	@ do all the work ourselves. Used by uw_install_context()
	@ in unwind-dw2.c. The handler is stored at the sp of the context
	@ that is to be returned to, then __builtin_eh_return restores
	@ the registers for the context and jumps to __ehs_trim_stack.
	.global	__ehs_trim_stack
	NAME	__ehs_trim_stack
#if __TARGET_SCL__
__ehs_trim_stack:
        @ Retrieve address of exception handler from stack.
	@ Be careful not to write to the stack before the first instruction
	@ otherwise the handler will be overwritten.
	LDR	ip, [sp, #-4]
	STMFD	sp!, {a1-a4, v1, ip}

#if __TARGET_MODULE__
	@ Check the processor mode to see if we have a chunked stack. If not,
	@ then there are no chunks to be freed.
	TEQ	a1, a1
	TEQ	pc, pc
	MRSEQ	v1, cpsr
	MOVNE	v1, pc
	TST	v1, #3
	BNE	1f
#endif

	@ Find the base of the current chunk.
	SUB	v1, sl, #512 + CHUNK_OVERHEAD

	@ Make the current chunk the last one.
	MOV	a4, #0
	STR	a4, [v1, #CHUNK_NEXT]

	@ The next chunk, if there is one, is the first in a possible chain
	@ to delete.
	LDR	v1, [v1, #CHUNK_NEXT]
	TEQ	v1, #0
	BEQ	1f
0:
	MOV	a1, v1

	@ Invalidate the chunk by zero'ing the MAGIC number.
	MOV	a4, #0
	STR	a4, [a1, #CHUNK_MAGIC]

	@ Get the next chunk, if any, before freeing the current one.
	LDR	v1, [a1, #CHUNK_NEXT]

	@ Free the current chunk.
	MOV	lr, pc
	LDR	pc, [a1, #CHUNK_DEALLOC]
	TEQ	v1, #0
	BNE	0b
1:
	LDMFD	sp!, {a1-a4, v1, ip}
	@ Jump to the real handler.
	MOV	pc, ip
	DECLARE_FUNCTION __ehs_trim_stack
#elif __TARGET_UNIXLIB__
__ehs_trim_stack:
        @ Retrieve address of exception handler from stack.
	@ Be careful not to write to the stack before the first instruction
	@ otherwise the handler will be overwritten.
	LDR	ip, [sp, #-4]
	@ Save registers that __trim_stack may corrupt.
	STMFD	sp!, {a1-a4, ip}
	BL	__trim_stack
	LDMFD	sp!, {a1-a4, ip}
	@ Jump to the real handler.
	MOV	pc, ip
	DECLARE_FUNCTION __ehs_trim_stack
#else
#  error "Unsupported runtime"
#endif

	@ void __ehs_unwind_stack_chunk (void **fp, void **pc, void **sl)
	@
	@ Check if the return address in PC is either __gcc_alloca_free
	@ or __free_stack_chunk and return the real return address. In
	@ the latter case, step back to the previous stack chunk and
	@ update SL.
	@ This is used by the ARM stack unwinder (gcc/config/arm/unwind-arm.c)
	.global	__ehs_unwind_stack_chunk
	NAME	__ehs_unwind_stack_chunk
#if __TARGET_UNIXLIB__
__ehs_unwind_stack_chunk:
#if __UNIXLIB_CHUNKED_STACK
	MOV	ip, sp
 PICNE "STMFD	sp!, {v1-v3, fp, ip, lr, pc}"
 PICEQ "STMFD	sp!, {v1-v4, fp, ip, lr, pc}"
	SUB	fp, ip, #4

	MOV	v2, a2
	MOV	v3, a3

	LDR	v1, [a1, #0]
	TEQ	v1, #0
 PICNE "LDMEQEA	fp, {v1-v3, fp, sp, pc}"
 PICEQ "LDMEQEA	fp, {v1-v4, fp, sp, pc}"

 PICEQ "LDR	v4, .L4"
.LPIC1:
 PICEQ "ADD	v4, pc, v4"			@ v4 = Library public GOT
 PICEQ "LDMIA	v4, {v4, v5}"			@ v4 = Object index, v4 = GOT table location
 PICEQ "LDR	v5, [v5, #0]"			@ v5 = GOT table
 PICEQ "LDR	v4, [v5, v4, LSL#4]"		@ v4 = Library private GOT

	LDR	a1, [v2, #0]			@ Retrieve old return address (pc)
	TEQ	a1, a1				@ 32bit mode check
	TEQ	pc, pc
	BICNE	a1, a1, #0xfc000003		@ If running 26bit, clear PSR bits.

	LDR	lr, .L1				@ __gcc_alloca_free
 PICEQ "LDR	lr, [v4, lr]"
	TEQ	a1, lr
	BNE	0f

	@ Return address was __gcc_alloca_free, find real return address and store
	@ ready for return.
	MOV	a1, v1
	BL	__gcc_alloca_return_address
	STR	a1, [v2, #0]
	@ Fall through - alloca could have stored __free_stack_chunk.
0:
	LDR	lr, .L2				@ __free_stack_chunk
 PICEQ "LDR	lr, [v4, lr]"
	TEQ	a1, lr
 PICNE "LDMNEEA	fp, {v1-v3, fp, sp, pc}"
 PICEQ "LDMNEEA	fp, {v1-v4, fp, sp, pc}"

	@ Return address was __free_stack_chunk, unwind to previous stack chunk
	LDR	a2, [v3, #0]			@ Retrieve old stack limit
	SUB	a2, a2, #512 + CHUNK_OVERHEAD	@ Find start of stack chunk
	LDR	a1, [a2, #CHUNK_RETURN]		@ Find return address for this chunk
	LDR	a2, [a2, #CHUNK_PREV]		@ Unwind to previous chunk
	ADD	a2, a2, #512 + CHUNK_OVERHEAD	@ Calculate stack limit for new chunk
	STR	a1, [v2, #0]			@ Store new return address for return
	STR	a2, [v3, #0]			@ Store new stack limit for return

 PICNE "LDMEA	fp, {v1-v3, fp, sp, pc}"
 PICEQ "LDMEA	fp, {v1-v4, fp, sp, pc}"
.L4:
 PICEQ ".word	_GLOBAL_OFFSET_TABLE_-(.LPIC1+4)"
#else /* !__UNIXLIB_CHUNKED_STACK */
	@ Nothing to do for a flat stack.
	MOV	pc, lr
#endif
#elif __TARGET_SCL__
	@ void __ehs_unwind_stack_chunk (void **fp, void **pc, void **sl)
__ehs_unwind_stack_chunk:
	MOV	ip, sp
	STMFD	sp!, {v1-v3, fp, ip, lr, pc}
	SUB	fp, ip, #4

	MOV	v1, a1
	MOV	v2, a2
	MOV	v3, a3

	LDR	a1, [v2, #0]			@ Retrieve old return address (pc)
	TEQ	a1, a1				@ 32bit mode check
	TEQ	pc, pc
	BICNE	a1, a1, #0xfc000003		@ If running 26bit, clear PSR bits.

	LDR	lr, .L1				@ __gcc_alloca_free
	TEQ	a1, lr
	BNE	0f

	@ Return address was __gcc_alloca_free, find real return address and store
	@ ready for return.
	MOV	a1, v1
	BL	__gcc_alloca_return_address
	STR	a1, [v2, #0]
0:
#if __TARGET_MODULE__
	@ Check the processor mode to see if we have a chunked stack. If not,
	@ then we can exit now.
	TEQ	a1, a1
	TEQ	pc, pc
	MRSEQ	ip, cpsr
	MOVNE	ip, pc
	TST	ip, #3
	LDMNEEA	fp, {v1-v3, fp, sp, pc}
#endif

	@ We can't determine the exact address of the stack chunk freeing function
	@ in the SCL in order to test for it here as we do in the UnixLib code.
	@ Instead, we check if the address lies within the SCL at all and if it
	@ does, we assume that it's the stack chunk freeing function.
	LDR	ip, .L2			@ scl_pos_block
	LDMIA	ip, {a2, a3}
	TEQ	a2, #0
	BNE	0f

	@ __ehs_init is called only once during the program's lifetime.
	BL	__ehs_init
	LDMIA	ip, {a2, a3}
0:
	CMP	a1, a2
	LDMLOEA	fp, {v1-v3, fp, sp, pc}

	CMP	a1, a3
	LDMHSEA	fp, {v1-v3, fp, sp, pc}

	@ If we get here, then the return address is in the SharedCLibrary
	@ and is likely to be the function for freeing the stack chunk.
	LDR	ip, [v1, #0]			@ Retrieve frame pointer from arg1
	LDR	a1, [ip, #-4]			@ Retrieve real return address
	STR	a1, [v2, #0]			@ Store in arg2 for return
	LDR	a2, [v3, #0]			@ Retrieve old stack limit from arg3
	SUB	a2, a2, #512 + CHUNK_OVERHEAD	@ Find start of old stack chunk
	LDR	a2, [a2, #CHUNK_PREV]		@ Unwind to previous chunk
	ADD	a2, a2, #512 + CHUNK_OVERHEAD	@ Calculate stack limit for new chunk
	STR	a2, [v3, #0]			@ Store new stack limit in arg3 for return

	@ Update frame pointer so that we skip the current frame
	LDR	ip, [ip, #-12]
	STR	ip, [v1, #0]

	LDMEA	fp, {v1-v3, fp, sp, pc}
#else
#  error "Unsupported runtime"
#endif
	DECLARE_FUNCTION __ehs_unwind_stack_chunk

	.end
