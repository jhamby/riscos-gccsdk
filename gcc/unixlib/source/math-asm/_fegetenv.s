;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/math-asm/s/_fegetenv,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	EXPORT	fegetenv
fegetenv
	rfs	ip
	str	ip, [a1, #0]
	return	AL, pc, lr

	END
