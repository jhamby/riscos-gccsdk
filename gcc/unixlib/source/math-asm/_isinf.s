;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/math-asm/s/_isinf,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT  isinf
isinf
	BIC	a1, a1, #&80000000
	EOR	a1, a1, #&70000000
	EOR	a1, a1, #&0ff00000
	ORRS	a2, a1, a2
	MOVNE	a1, #0
	MOVEQ	a1, #1
	return	AL, pc, lr

	END
