;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_feholdexc.s,v $
; $Date: 2000/07/15 14:52:23 $
; $Revision: 1.1.1.1 $
; $State: Exp $
; $Author: nick $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	EXPORT	feholdexcept
feholdexcept
	rfs	ip
	str	ip, [a1, #0]
	; ip &= ~(FE_ALL_EXCEPT << FE_EXCEPT_SHIFT)
	bic	ip, ip, #&F0000
	wfs	ip
	mov	a1, #1
	return	AL, pc, lr
	END
