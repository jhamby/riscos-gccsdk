;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_kernel.s,v $
; $Date: 2000/07/15 14:52:28 $
; $Revision: 1.1.1.1 $
; $State: Exp $
; $Author: nick $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$wrcode|,CODE

; _kernel_swi for UnixLib
; This version provides the same programming contract as the version in
; the Shared C Library and hence follows the definition of the function
; in C:kernel.h.
;
; This version differs by working not placing the generated code onto
; the stack, but using self-modifying code inlined in the function.
; This version will work on all current ARM chips including the ARM 8
; and StrongARM. The self-modifying code works because the only
; instruction modified is a SWI and the instruction cache only uses
; the condition code field and the SWI instruction field (the top
; byte) which is preset. The SWI decoding code will load the SWI `comment'
; field of the SWI from the data cache.
;
; Unfortunately, DDT doesn't seem to like even that bit of self-modifying
; code on a StrongARM, so we use OS_CallASWIR12 (as all good boys and
; girls should).


	IMPORT	|__seterr|
	EXPORT	|_kernel_swi|

|_kernel_swi|
	STMFD	sp!, {a3, v1-v6, lr}
	BICS	ip, a1, #&80000000
	ORRPL	ip, ip, #&20000		; X bit
	LDMIA	a2, {a1-v6}
	SWI	XOS_CallASWIR12
	LDR	ip, [sp]
	STMIA	ip, {a1-v6}
	MOVVC	a1, #0
	stackreturn	VC, "a3, v1-v6, pc"
	MOV	v1, a1
	BL	|__seterr|
	MOV	a1, v1
	stackreturn	AL, "a3, v1-v6, pc"

	END
