;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_fclrexcpt.s,v $
; $Date: 2000/07/15 14:52:23 $
; $Revision: 1.1.1.1 $
; $State: Exp $
; $Author: nick $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	EXPORT	feclearexcept
feclearexcept
	rfs	ip
	and	a1, a1, #15
	eor	a1, a1, #15
	and	ip, ip, a1
	wfs	ip
	return	AL, pc, lr

	END
