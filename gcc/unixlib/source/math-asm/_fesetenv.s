;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_fesetenv.s,v $
; $Date: 2000/07/15 14:52:23 $
; $Revision: 1.1.1.1 $
; $State: Exp $
; $Author: nick $
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
