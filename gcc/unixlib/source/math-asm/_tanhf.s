;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_tanhf.s,v $
; $Date: 2003/05/13 21:48:10 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: alex $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	; return (exp(x) - exp(-x)) / (exp(x) + exp(-x))
	EXPORT	tanhf
	NAME	tanhf
tanhf
	[ __UNIXLIB_NORCROFT_BUILD > 0
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	|
	STR	a1, [sp, #-4]!
	LDFS	f0, [sp], #4
	]
	MNFS	f1, f0
	EXPS	f0, f0
	EXPS	f1, f1

	SUFS	f2, f0, f1
	ADFS	f3, f0, f1
	DVFS	f0, f2, f3
	MOV	pc, lr

	END
