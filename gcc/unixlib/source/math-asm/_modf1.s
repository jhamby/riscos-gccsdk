;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_modf1.s,v $
; $Date: 2002/11/26 21:52:07 $
; $Revision: 1.4 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	modf
	EXPORT	modfl
	NAME	modf
modf
modfl
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	RNDDZ	f1, f0
	STFD	f1, [a3]
	SUFD	f0, f0, f1
	MOV	pc, lr

	END
