;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/math-asm/s/_finite,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	; int finite (double x)
	; return 1 if x is finite, else 0
	EXPORT  finite
finite
	BIC	a1, a1, #&80000000
	ADD	a1, a1, #&100000
	ADD	a1, a1, #&80000000
	MOV	a1, a1, ASR #31
	return	AL, pc, lr

	END
