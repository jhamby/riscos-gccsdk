;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_fclrexcpt.s,v $
; $Date: 2002/09/24 21:02:37 $
; $Revision: 1.3 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	EXPORT	feclearexcept
	NAME	feclearexcept
feclearexcept
	RFS	ip
	AND	a1, a1, #15
	EOR	a1, a1, #15
	AND	ip, ip, a1
	WFS	ip
	MOV	pc, lr

	END
