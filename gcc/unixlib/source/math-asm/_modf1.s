;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_modf1.s,v $
; $Date: 2001/01/29 15:10:20 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	modf
	NAME	modf
modf
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	RNDDZ	f1, f0
	STFD	f1, [a3]
	SUFD	f0, f0, f1
	return	AL, pc, lr

	END
