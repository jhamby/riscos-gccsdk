;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/math-asm/s/_feholdexc,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
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
