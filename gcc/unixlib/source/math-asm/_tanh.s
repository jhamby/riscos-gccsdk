;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_tanh.s,v $
; $Date: 2002/11/26 21:52:07 $
; $Revision: 1.4 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	; return (exp(x) - exp(-x)) / (exp(x) + exp(-x))
	EXPORT	tanh
	EXPORT	tanhl
	NAME	tanh
tanh
tanhl
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	MNFD	f1, f0
	EXPD	f0, f0
	EXPD	f1, f1

	SUFD	f2, f0, f1
	ADFD	f3, f0, f1
	DVFD	f0, f2, f3
	MOV	pc, lr

	END
