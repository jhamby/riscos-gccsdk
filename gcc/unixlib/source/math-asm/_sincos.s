;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_sincos.s,v $
; $Date: 2004/06/05 14:00:08 $
; $Revision: 1.4 $
; $State: Exp $
; $Author: joty $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	; void sincos (double x, double *sinx, double *cosx)
	EXPORT	|__sincos|
	NAME	__sincos
|__sincos|
	STMFD	sp!,{a1, a2}
	LDFD	f2, [sp], #8
	SIND	f0, f2
	COSD	f1, f2
	STFD	f0, [a3, #0]
	STFD	f1, [a4, #0]
	MOV	pc, lr

	END
