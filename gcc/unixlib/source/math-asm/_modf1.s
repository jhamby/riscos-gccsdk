;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_modf1.s,v $
; $Date: 2000/07/15 14:52:24 $
; $Revision: 1.1.1.1 $
; $State: Exp $
; $Author: nick $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	modf
modf
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	RNDDZ	f1, f0
	STFD	f1, [a3]
	SUFD	f0, f0, f1
	return	AL, pc, lr

	END
