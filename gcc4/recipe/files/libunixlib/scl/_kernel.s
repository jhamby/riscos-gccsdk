@ _kernel_swi
@ Copyright (c) 2000-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

@ _kernel_swi for UnixLib
@ This version provides the same programming contract as the version in
@ the Shared C Library and hence follows the definition of the function
@ in C:kernel.h.
@
@ One possible implementation is using self-modifying code inlined in
@ the function. Such an implementation will work on all current ARM chips
@ including the ARM 8 and StrongARM.
@ The self-modifying code works because the only instruction modified
@ is a SWI and the instruction cache only uses the condition code field
@ and the SWI instruction field (the top byte) which is preset.
@ The SWI decoding code will load the SWI `comment' field of the SWI
@ from the data cache.
@
@ Unfortunately, DDT doesn't seem to like even that bit of self-modifying
@ code on a StrongARM, so we use OS_CallASWIR12 (as all good boys and
@ girls should).

	@ _kernel_oserror *_kernel_swi(int no, _kernel_swi_regs *in,
	@                              _kernel_swi_regs *out);
	.global	_kernel_swi
	NAME	_kernel_swi
_kernel_swi:
	STMFD	sp!, {a3, v1-v6, lr}
	BIC	ip, a1, #0x80000000
	TEQ	ip, a1
	ORREQ	ip, ip, #0x20000		@ X bit
	LDMIA	a2, {a1-v6}
	SWI	XOS_CallASWIR12
	LDR	ip, [sp, #0]
	STMIA	ip, {a1-v6}
	MOVVC	a1, #0
	LDMVCFD	sp!, {a3, v1-v6, pc}
	MOV	v1, a1
	MOV	a2, #1
	BL	__ul_seterr
	MOV	a1, v1
	LDMFD	sp!, {a3, v1-v6, pc}
	DECLARE_FUNCTION _kernel_swi

	.end
