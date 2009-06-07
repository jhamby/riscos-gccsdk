@ Exception handling support
@ This source is used by the call frame C++ exception handling code.
@ Copyright (c) 2009 UnixLib Developers

#include "internal/asm_dec.s"

	.text
 
	@ void *__ehs_return_address (void *fp)
	@
	@ Given a frame pointer, return real return address
	.global	__ehs_return_address
	NAME	__ehs_return_address
__ehs_return_address:
	MOV	ip, sp
	STMFD	sp!, {fp, ip, lr, pc}
	SUB	fp, ip, #4

	MOVS	ip, a1
	MOVEQ	a1, #0
	LDMEQEA	fp, {fp, sp, pc}

	@ Retrieve return address from stack frame
	LDR	a1, [ip, #-4]
	TEQ	a1, a1			@ 32bit mode check
	TEQ	pc, pc
	BICNE	a1, a1, #0xfc000003	@ If running 26bit, clear PSR bits.

	LDR	lr, .L1			@ __gcc_alloca_free
	TEQ	a1, lr
	MOVEQ	a1, ip
	BLEQ	__gcc_alloca_return_address

	@ SCL doesn't store the return address in the stack chunk
#if __TARGET_UNIXLIB__
	LDR	lr, .L2			@ __free_stack_chunk
	TEQ	a1, lr
	LDMNEEA	fp, {fp, sp, pc}

	@ The logical thing to do here would be to find the stack chunk that
	@ contains the fp arg and extract the return address from that as the
	@ result. However, that doesn't work as far as C++ call frame
	@ exception handling is concerned. Extracting the return address from
	@ the current chunk does work.

	SUB	a2, sl, #512 + CHUNK_OVERHEAD

	LDR	a1, [a2, #CHUNK_RETURN]
#endif
	LDMEA	fp, {fp, sp, pc}
.L1:
	WORD	__gcc_alloca_free
#if __TARGET_UNIXLIB__
.L2:
	WORD	__free_stack_chunk
#endif
	DECLARE_FUNCTION __ehs_return_address

	.end
