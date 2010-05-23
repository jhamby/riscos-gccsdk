@ __builtin_return_address
@ This source is used by SCL and UnixLib libraries.
@ Copyright (c) 2002-2010 UnixLib Developers

#include "internal/asm_dec.s"

#ifndef __ARM_EABI__

	.text

	@ void * __builtin_return_adress (unsigned int level)
	@
	@ Return the return address of the current function (level == 0)
	@ or of one of its callers.  Return 0 if at the top of the stack
	@ or the address is unobtainable
	@
	@ This is an implementation of a GCC internal function.  It appears
	@ here because we need to cope with the function return address
	@ being modified in the frame by calls to stack extension code
	@ or calls to alloca.
	.global	__builtin_return_address
	NAME	__builtin_return_address
#if __TARGET_SCL__
	@ There's no PIC in SCL code.
__builtin_return_address:
	STMFD	sp!, {lr}

	BL	__builtin_frame_address

	MOVS	a2, a1
	LDMEQFD	sp!, {pc}

	LDR	a1, [a2, #-4]		@ Load return address from the stack frame.
	TEQ	a1, a1			@ 32bit mode check
	TEQ	pc, pc
	BICNE	a1, a1, #0xfc000003	@ If running 26bit, clear PSR bits.

	@ If the return address in the frame points to the
	@ '__gcc_alloca_free' function, then we have outstanding alloca
	@ blocks so the return address needs to be found in one of the
	@ 'struct alloca_chunk' blocks.
	LDR	lr, .L1
	TEQ	a1, lr
	MOVEQ	a1, a2
	BLEQ	__gcc_alloca_return_address

	LDMFD	sp!, {pc}
.L1:
	.word	__gcc_alloca_free
#elif __TARGET_UNIXLIB__
__builtin_return_address:
 PICNE "STMFD	sp!, {lr}"
 PICEQ "STMFD	sp!, {v4, lr}"
	@ Setup GOT for function call via PLT.
 PICEQ "LDR	v4, .L2+4"
.LPIC0:
 PICEQ "ADD	v4, pc, v4"		@ v4 = Library public GOT
 PICEQ "LDMIA	v4, {v4, v5}"		@ v4 = Object index, v4 = GOT table location
 PICEQ "LDR	v5, [v5, #0]"		@ v5 = GOT table
 PICEQ "LDR	v4, [v5, v4, LSL#4]"	@ v4 = Library private GOT

	BL	__builtin_frame_address

	MOVS	a2, a1
 PICNE "LDMEQFD	sp!, {pc}"
 PICEQ "LDMEQFD	sp!, {v4, pc}"

	LDR	a1, [a2, #-4]		@ Load return address from the stack frame.
	TEQ	a1, a1			@ 32bit mode check
	TEQ	pc, pc
	BICNE	a1, a1, #0xfc000003	@ If running 26bit, clear PSR bits.

	@ If the return address in the frame points to the
	@ '__gcc_alloca_free' function, then we have outstanding alloca
	@ blocks so the return address needs to be found in one of the
	@ 'struct alloca_chunk' blocks.
	LDR	lr, .L1
 PICEQ "LDR	lr, [v4, lr]"
	TEQ	a1, lr
	MOVEQ	a1, a2
	BLEQ	__gcc_alloca_return_address

	@ If the return address in the frame points to the
	@ '__free_stack_chunk' function, then the real return address has
	@ to be found at sl[CHUNK_RETURN].
	SUB	a2, sl, #512 + CHUNK_OVERHEAD
	LDR	lr, .L2
 PICEQ "LDR	lr, [v4, lr]"
	TEQ	a1, lr
	LDREQ	a1, [a2, #CHUNK_RETURN]

 PICNE "LDMFD	sp!, {pc}"
 PICEQ "LDMFD	sp!, {v4, pc}"
.L1:
	WORD	__gcc_alloca_free
.L2:
	WORD	__free_stack_chunk
 PICEQ ".word	_GLOBAL_OFFSET_TABLE_-(.LPIC0+4)"
#else
#  error "Unsupported runtime"
#endif
	DECLARE_FUNCTION __builtin_return_address

#endif

	.end
