;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_atan.s,v $
; $Date: 2002/09/24 21:02:37 $
; $Revision: 1.3 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	atan
	EXPORT	atanl
	NAME	atan
atan
atan1
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	ATND	f0, f0
	return	AL, pc, lr

	END
