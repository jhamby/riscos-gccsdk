;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_sincos.s,v $
; $Date: 2001/01/29 15:10:20 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
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
	STFD	f0, [r1, #0]
	STFD	f1, [r2, #0]
	return	AL, pc, lr

	END
