;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_finite.s,v $
; $Date: 2002/09/24 21:02:37 $
; $Revision: 1.3 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	; int finite (double x)
	; return 1 if x is finite, else 0
	EXPORT  finite
	NAME	finite
finite
	BIC	a1, a1, #&80000000
	ADD	a1, a1, #&100000
	ADD	a1, a1, #&80000000
	MOV	a1, a1, ASR #31
	MOV	pc, lr

	END
