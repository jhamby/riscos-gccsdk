;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_feholdexc.s,v $
; $Date: 2002/09/24 21:02:37 $
; $Revision: 1.3 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	EXPORT	feholdexcept
	NAME	feholdexcept
feholdexcept
	RFS	ip
	STR	ip, [a1, #0]
	; ip &= ~(FE_ALL_EXCEPT << FE_EXCEPT_SHIFT)
	BIC	ip, ip, #&F0000
	WFS	ip
	MOV	a1, #1
	MOV	pc, lr

	END
