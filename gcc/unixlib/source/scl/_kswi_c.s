;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_kswi_c.s,v $
; $Date: 2002/09/24 21:02:37 $
; $Revision: 1.3 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY


	IMPORT	|__seterr|

	; _kernel_swi_c (int no, _kernel_swi_regs *in,
	;		 _kernel_swi_regs *out, int *carry);
	EXPORT	|_kernel_swi_c|
	NAME	_kernel_swi_c
|_kernel_swi_c|
	STMFD	sp!, {a3, a4, v1 - v6, lr}
	BICS	ip, a1, #&80000000
	ORRPL	ip, ip, #&20000		; X bit
	LDMIA	a2, {a1 - v6}
	SWI	XOS_CallASWIR12
	LDR	ip, [sp, #0]
	STMIA	ip, {a1 - v6}
	BVS	error
	MOV	a1, #0
	LDMIA	sp!, {a3, a4, v1-v6, lr}
	; Store the carry flag status in `carry'
	MOVCC	a2, #0
	MOVCS	a2, #1
	STR	a2, [a4, #0]
	MOV	pc, lr

error
	MOV	v1, a1
	BL	|__seterr|
	MOV	a1, v1
	LDMIA	sp!, {a3, a4, v1-v6, pc}

	END
