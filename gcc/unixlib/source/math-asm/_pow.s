;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_pow.s,v $
; $Date: 2001/01/29 15:10:20 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	pow
	NAME	pow
pow
	STMFD	sp!, {a1, a2, a3, a4}
	LDFD	f0, [sp], #8
	LDFD	f1, [sp], #8
	POWD	f0, f0, f1
	return	AL, pc, lr

	END
