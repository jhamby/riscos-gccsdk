;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_ldexp.s,v $
; $Date: 2000/07/15 14:52:24 $
; $Revision: 1.1.1.1 $
; $State: Exp $
; $Author: nick $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	; scalb is the BSD name for ldexp
	EXPORT	scalb
	EXPORT	ldexp
scalb
ldexp
	; return x*pow(2, y)
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	FLTD	f1, a3
	MVFD	f2, #2
	POWD	f3, f2, f1
	MUFD	f0, f0, f3
	return	AL, pc, lr

	END
