;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/math-asm/s/_fesetenv,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	EXPORT	fesetenv
fesetenv
	cmn	a1, #1 ; if envp == FE_DFL_ENV
	; set FPSR to FPU_DEFAULT
	moveq	ip, #0
	ldrne	ip, [a1, #0]
	wfs	ip
	return	AL, pc, lr

	END
