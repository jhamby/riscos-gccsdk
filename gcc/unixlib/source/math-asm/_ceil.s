;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_ceil.s,v $
; $Date: 2002/11/26 21:52:07 $
; $Revision: 1.4 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	ceil
	EXPORT	ceill
	NAME	ceil
ceil
ceill
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	RNDDP	f0, f0
	MOV	pc, lr

	END
