;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_ftstexcpt.s,v $
; $Date: 2002/09/24 21:02:37 $
; $Revision: 1.3 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	EXPORT	fetestexcept
	NAME	fetestexcept
fetestexcept
	RFS	ip
	AND	ip, ip, a1
	AND	a1, ip, #15
	MOV	pc, lr

	END
