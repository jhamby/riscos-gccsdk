;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_powf.s,v $
; $Date: 2002/11/26 21:52:07 $
; $Revision: 1.1 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	powf
	NAME	powf
powf
	[ __UNIXLIB_NORCROFT_BUILD > 0
	STMFD	sp!, {a1, a2, a3, a4}
	LDFD	f0, [sp], #8
	LDFD	f1, [sp], #8
	|
	STMFD	sp!, {a1, a2}
	LDFS	f0, [sp], #4
	LDFS	f1, [sp], #4
	]
	POWS	f0, f0, f1
	MOV	pc, lr

	END
