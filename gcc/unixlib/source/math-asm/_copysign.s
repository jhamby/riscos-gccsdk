;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/math-asm/s/_copysign,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	; double copysign (double x, double y)
	; returns a value with the magnitude of x and with
	; the sign bit of y.
        EXPORT  copysign
copysign
	TST	a3, #&80000000
	ORRNE	a1, a1, #&80000000
	BICEQ	a1, a1, #&80000000
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	return	AL, pc, lr

	END
