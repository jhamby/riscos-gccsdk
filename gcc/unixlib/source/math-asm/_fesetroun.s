;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_fesetroun.s,v $
; $Date: 2001/01/29 15:10:20 $
; $Revision: 1.2 $
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
	cmp	a1, #0
	movne	a1, #0
	moveq	a1, #1
	return	AL, pc, lr

	END
