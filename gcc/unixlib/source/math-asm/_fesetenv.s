;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_fesetenv.s,v $
; $Date: 2002/09/24 21:02:37 $
; $Revision: 1.3 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	EXPORT	fesetenv
	NAME	fesetenv
fesetenv
	CMN	a1, #1 ; if envp == FE_DFL_ENV
	; set FPSR to FPU_DEFAULT
	MOVEQ	ip, #0
	LDRNE	ip, [a1, #0]
	WFS	ip
	MOV	pc, lr

	END
