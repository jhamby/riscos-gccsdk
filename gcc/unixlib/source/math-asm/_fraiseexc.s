;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_fraiseexc.s,v $
; $Date: 2002/09/24 21:02:37 $
; $Revision: 1.3 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	EXPORT	feraiseexcept
	NAME	feraiseexcept
feraiseexcept
	RFS	ip
	AND	a1, a1, #15
	ORR	ip, ip, a1
	WFS	ip
	MOV	pc, lr

	END
