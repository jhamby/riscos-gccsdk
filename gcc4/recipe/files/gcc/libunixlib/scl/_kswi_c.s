@ _kernel_swi_c
@ Copyright (c) 2000-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	@ _kernel_oserror *_kernel_swi_c (int no, _kernel_swi_regs *in,
	@				  _kernel_swi_regs *out, int *carry)
	.global	_kernel_swi_c
	NAME	_kernel_swi_c
_kernel_swi_c:
	STMFD	sp!, {a3-a4, v1-v6, lr}
	BIC	ip, a1, #0x80000000
	TEQ	ip, a1
	ORREQ	ip, ip, #0x20000		@ X bit
	LDMIA	a2, {a1-v6}
	SWI	XOS_CallASWIR12
	LDR	ip, [sp, #0]
	STMIA	ip, {a1-v6}
	BVS	error
	MOV	a1, #0
	LDMIA	sp!, {a3-a4, v1-v6, lr}
	@ Store the carry flag status in `carry'
	MOVCC	a2, #0
	MOVCS	a2, #1
	STR	a2, [a4, #0]
	MOV	pc, lr

error:
	MOV	v1, a1
	MOV	a2, #1
	BL	__ul_seterr
	MOV	a1, v1
	LDMIA	sp!, {a3-a4, v1-v6, pc}
	DECLARE_FUNCTION _kernel_swi_c

	.end
