;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_isinf.s,v $
; $Date: 2002/09/24 21:02:37 $
; $Revision: 1.3 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT  isinf
	NAME	isinf
isinf
	BIC	a1, a1, #&80000000
	EOR	a1, a1, #&70000000
	EOR	a1, a1, #&0ff00000
	ORRS	a2, a1, a2
	MOVNE	a1, #0
	MOVEQ	a1, #1
	MOV	pc, lr

	END
