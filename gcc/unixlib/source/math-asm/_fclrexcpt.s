;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/math-asm/s/_fclrexcpt,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
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
