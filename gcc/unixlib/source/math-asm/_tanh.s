;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_tanh.s,v $
; $Date: 2001/01/29 15:10:20 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	; return (exp(x) - exp(-x)) / (exp(x) + exp(-x))
	EXPORT	tanh
	NAME	tanh
tanh
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	MNFD	f1, f0
	EXPD	f0, f0
	EXPD	f1, f1

	SUFD	f2, f0, f1
	ADFD	f3, f0, f1
	DVFD	f0, f2, f3
	return	AL, pc, lr

	END
