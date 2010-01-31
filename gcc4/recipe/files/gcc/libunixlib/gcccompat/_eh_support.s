@ Exception handling support
@ This source is used by the call frame C++ exception handling code.
@ Copyright (c) 2009 UnixLib Developers

#include "internal/asm_dec.s"

	.text
 
#define STACKCHUNK_MAGIC_NUMBER 0xF60690FF

	@ void *__ehs_return_address (void *fp)
	@
	@ Given a frame pointer, return real return address
	.global	__ehs_return_address
	NAME	__ehs_return_address
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

	@ SCL doesn't store the return address in the stack chunk
#if __TARGET_UNIXLIB__
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
#endif
 PICNE "LDMEA	fp, {v1, fp, sp, pc}"
 PICEQ "LDMEA	fp, {v1, v4, fp, sp, pc}"
.L1:
	WORD	__gcc_alloca_free
#if __TARGET_UNIXLIB__
.L2:
	WORD	__free_stack_chunk
#endif
.L3:
 PICEQ ".word	_GLOBAL_OFFSET_TABLE_-(.LPIC0+4)"
	DECLARE_FUNCTION __ehs_return_address


	@ void *__ehs_stack_limit (void *sp)
	@
	@ Given a stack pointer, return the stack limit of the chunk
	@ that contains it. Used by uw_install_context() in unwind-dw2.c.
	.global	__ehs_stack_limit
	NAME	__ehs_stack_limit
__ehs_stack_limit:
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
	@ Wrapper function for __trim_stack. Used by uw_install_context()
	@ in unwind-dw2.c. The handler is stored at the sp of the context
	@ that is to be returned to, then __builtin_eh_return restores
	@ the registers for the context and jumps to __ehs_trim_stack.
	.global	__ehs_trim_stack
	NAME	__ehs_trim_stack
__ehs_trim_stack:
        @ Retrieve address of exception handler from stack
	LDR	ip, [sp, #-4]

	@ Save registers that __trim_stack may corrupt
	STMFD	sp!, {a1-a4, ip}
	BL	__trim_stack
	LDMFD	sp!, {a1-a4, ip}
	MOV	pc, ip
	DECLARE_FUNCTION __ehs_trim_stack

	.end
