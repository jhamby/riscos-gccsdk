;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_fesetroun.s,v $
; $Date: 2002/09/24 21:02:37 $
; $Revision: 1.3 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	EXPORT	fesetround
	NAME	fesetround
fesetround
	; Only FE_TONEAREST is supported.
	CMP	a1, #0
	MOVNE	a1, #0
	MOVEQ	a1, #1
	MOV	pc, lr

	END
