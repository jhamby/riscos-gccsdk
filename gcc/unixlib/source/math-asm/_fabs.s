;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_fabs.s,v $
; $Date: 2001/01/29 15:10:20 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	; double fabs (double x)
	; returns the absolute value of x
	EXPORT  fabs
	NAME	fabs
fabs
	BIC	a1, a1, #&80000000
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	return	AL, pc, lr

	END
