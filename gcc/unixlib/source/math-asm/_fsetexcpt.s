;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_fsetexcpt.s,v $
; $Date: 2000/07/15 14:52:24 $
; $Revision: 1.1.1.1 $
; $State: Exp $
; $Author: nick $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	EXPORT	fesetexceptflag
	; a1 = (const fexcept_t) *flagp, a2 = (int) excepts
fesetexceptflag
	; Get the current environment
	rfs	ip
	ldr	a4, [a1, #0] ; a4 = (flagp->cw)
	and	a3, a2, #15 ; a3 = (excepts & FE_ALL_EXCEPT)
	; Clear out old exception bits
	bic	ip, ip, a3, lsl #16

	; Add in new exception bits
	and	a4, a4, a2
	and	a4, a4, #15
	orr	ip, ip, a4, lsl #16

	; Set the new environment
	wfs	ip
	return	AL, pc, lr

	END
