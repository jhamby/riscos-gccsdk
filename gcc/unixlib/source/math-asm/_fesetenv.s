;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_fesetenv.s,v $
; $Date: 2001/01/29 15:10:20 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	EXPORT	fesetenv
	NAME	fesetenv
fesetenv
	cmn	a1, #1 ; if envp == FE_DFL_ENV
	; set FPSR to FPU_DEFAULT
	moveq	ip, #0
	ldrne	ip, [a1, #0]
	wfs	ip
	return	AL, pc, lr

	END
