;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/math-asm/s/_fraiseexc,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	EXPORT	feraiseexcept
feraiseexcept
	rfs	ip
	and	a1, a1, #15
	orr	ip, ip, a1
	wfs	ip
	return	AL, pc, lr

	END
