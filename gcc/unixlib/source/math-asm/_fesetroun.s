;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/math-asm/s/_fesetroun,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	EXPORT	fesetround
fesetround
	; Only FE_TONEAREST is supported.
	cmp	a1, #0
	movne	a1, #0
	moveq	a1, #1
	return	AL, pc, lr

	END
