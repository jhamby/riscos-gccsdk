;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_sinh.s,v $
; $Date: 2002/11/26 21:52:07 $
; $Revision: 1.4 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	; return (exp(x) - exp(-x)) / 2
	EXPORT	sinh
	EXPORT	sinhl
	NAME	sinh
sinh
sinhl
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	MNFD	f1, f0
	EXPD	f0, f0
	EXPD	f1, f1
	SUFD	f0, f0, f1
	DVFD	f0, f0, #2
	MOV	pc, lr

	END
