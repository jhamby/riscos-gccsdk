;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_fsetexcpt.s,v $
; $Date: 2002/09/24 21:02:37 $
; $Revision: 1.3 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	; a1 = (const fexcept_t) *flagp, a2 = (int) excepts
	EXPORT	fesetexceptflag
	NAME	fesetexceptflag
fesetexceptflag
	; Get the current environment
	RFS	ip
	LDR	a4, [a1, #0] ; a4 = (flagp->cw)
	AND	a3, a2, #15 ; a3 = (excepts & FE_ALL_EXCEPT)
	; Clear out old exception bits
	BIC	ip, ip, a3, lsl #16

	; Add in new exception bits
	AND	a4, a4, a2
	AND	a4, a4, #15
	ORR	ip, ip, a4, lsl #16

	; Set the new environment
	WFS	ip
	MOV	pc, lr

	END
