@ __builtin_return_address
@ Copyright (c) 2002, 2003, 2004, 2005, 2006 UnixLib Developers

#include "unixlib/asm_dec.s"

#ifdef __ELF__
	@ BEWARE: this looks like GAS syntax but isn't, the labels don't
	@ have a colon.

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
__builtin_return_address:
	STMFD	sp!, {lr}
	BL	__builtin_frame_address

	MOVS	a2, a1
	LDMEQFD	sp!, {pc}

	LDR	a1, [a2, #-4]		@ Load return address from the stack frame.
	TEQ	a1, a1			@ 32bit mode check
	TEQ	pc, pc
	BICNE	a1, a1, #0xfc000003	@ If running 26bit, clear PSR bits.

	@ If the return address in the frame points to the '__gcc_alloca_free'
	@ function, then we have outstanding alloca blocks so the return
	@ address needs to be found in one of the 'struct alloca_chunk' blocks.
	LDR	lr, .L1
	TEQ	a1, lr
	BNE	__builtin_return_address_alloca_resolved

	MOV	a1, a2
	BL	__gcc_alloca_return_address

__builtin_return_address_alloca_resolved:
	@ If the return address in the frame points to the '__free_stack_chunk'
	@ function, then the real return address has to be found
	@ at sl[CHUNK_RETURN].
	LDR	lr, .L2
	TEQ	a1, lr
	LDREQ	a1, [a2, #CHUNK_RETURN]

	LDMFD	sp!, {pc}
.L1:
	WORD	__gcc_alloca_free
.L2:
	WORD	__free_stack_chunk
	DECLARE_FUNCTION __builtin_return_address

#endif

	.end
