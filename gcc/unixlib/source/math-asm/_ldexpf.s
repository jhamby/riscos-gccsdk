;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_ldexpf.s,v $
; $Date: 2003/05/13 21:48:10 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: alex $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	; scalb is the BSD name for ldexp
	EXPORT	ldexpf
	NAME	ldexpf
ldexpf
	; return x*pow(2, y)
	[ __UNIXLIB_NORCROFT_BUILD > 0
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	|
	STR	a1, [sp, #-4]!
	LDFS	f0, [sp], #4
	]
	FLTS	f1, a2
	MVFS	f2, #2
	POWS	f3, f2, f1
	MUFS	f0, f0, f3
	MOV	pc, lr

	END
