;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_ftstexcpt.s,v $
; $Date: 2001/01/29 15:10:20 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	EXPORT	fetestexcept
	NAME	fetestexcept
fetestexcept
	rfs	ip
	and	ip, ip, a1
	and	a1, ip, #15
	return	AL, pc, lr

	END
