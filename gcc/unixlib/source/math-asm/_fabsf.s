;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_fabs.s,v $
; $Date: 2004/10/17 16:24:44 $
; $Revision: 1.4 $
; $State: Exp $
; $Author: joty $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	; float fabs (float x)
	; returns the absolute value of x
	EXPORT  fabsf
	NAME	fabsf
fabsf
	BIC	a1, a1, #&80000000
	STMFD	sp!, {a1}
	LDFS	f0, [sp], #4
	MOV	pc, lr

	END
