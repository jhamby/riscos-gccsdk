;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_atan2f.s,v $
; $Date: 2002/11/26 21:52:07 $
; $Revision: 1.1 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	atan2f
	NAME	atan2f
atan2f
	[ __UNIXLIB_NORCROFT_BUILD > 0
	STMFD	sp!, {a1, a2, a3, a4}
	LDFD	f0, [sp], #8
	LDFD	f1, [sp], #8
	|
	STMFD	sp!, {a1, a2}
	LDFS	f0, [sp], #4
	LDFS	f1, [sp], #4
	]
	CMF	f0, #0
	CMFEQ	f1, #0
	POLNES	f0, f1, f0
	MOV	pc, lr

	END
