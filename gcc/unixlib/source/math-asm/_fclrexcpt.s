;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_fclrexcpt.s,v $
; $Date: 2001/01/29 15:10:20 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	EXPORT	feclearexcept
	NAME	feclearexcept
feclearexcept
	rfs	ip
	and	a1, a1, #15
	eor	a1, a1, #15
	and	ip, ip, a1
	wfs	ip
	return	AL, pc, lr

	END
