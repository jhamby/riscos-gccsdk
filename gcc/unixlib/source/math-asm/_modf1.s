;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/math-asm/s/_modf1,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
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
