;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_atan2.s,v $
; $Date: 2002/09/24 21:02:37 $
; $Revision: 1.3 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	atan2f
	NAME	atan2f
atan2f
	STMFD	sp!, {a1, a2}
	LDFS	f0, [sp], #4
	LDFS	f1, [sp], #4
	CMF	f0, #0
	CMFEQ	f1, #0
	POLNES	f0, f1, f0
	return	AL, pc, lr

	END
