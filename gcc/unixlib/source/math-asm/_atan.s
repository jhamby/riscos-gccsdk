;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_atan.s,v $
; $Date: 2003/01/29 18:49:00 $
; $Revision: 1.5 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	atan
	EXPORT	atan1
	NAME	atan
atan
atan1
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	ATND	f0, f0
	MOV	pc, lr

	END
