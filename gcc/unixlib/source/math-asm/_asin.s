;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_asin.s,v $
; $Date: 2000/07/15 14:52:23 $
; $Revision: 1.1.1.1 $
; $State: Exp $
; $Author: nick $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	asin
asin
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	ASND	f0, f0
	return	AL, pc, lr

	END
