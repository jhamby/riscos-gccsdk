;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_pow.s,v $
; $Date: 2002/09/24 21:02:37 $
; $Revision: 1.3 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	powf
	NAME	powf
powf
	STMFD	sp!, {a1, a2}
	LDFS	f0, [sp], #4
	LDFS	f1, [sp], #4
	POWS	f0, f0, f1
	return	AL, pc, lr

	END
