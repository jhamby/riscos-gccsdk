;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/math-asm/s/_sincos,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	EXPORT	|__sincos|
	; void sincos (double x, double *sinx, double *cosx)
|__sincos|
	STMFD	sp!,{a1, a2}
	LDFD	f2, [sp], #8
	SIND	f0, f2
	COSD	f1, f2
	STFD	f0, [r1, #0]
	STFD	f1, [r2, #0]
	return	AL, pc, lr

	END
