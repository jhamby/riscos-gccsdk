;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_ldexp.s,v $
; $Date: 2002/09/24 21:02:37 $
; $Revision: 1.3 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	; scalb is the BSD name for ldexp
	EXPORT	ldexpf
	NAME	ldexpf
ldexpf
	; return x*pow(2, y)
	STR	a1, [sp, #-4]
	LDFS	f0, [sp, #-4]
	FLTS	f1, a2
	MVFS	f2, #2
	POWS	f3, f2, f1
	MUFS	f0, f0, f3
	return	AL, pc, lr

	END
