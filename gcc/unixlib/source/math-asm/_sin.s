;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_sin.s,v $
; $Date: 2001/01/29 15:10:20 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	sin
	NAME	sin
sin
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	SIND	f0, f0
	return	AL, pc, lr

	END
