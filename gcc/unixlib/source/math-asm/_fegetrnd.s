;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_fegetrnd.s,v $
; $Date: 2002/09/24 21:02:37 $
; $Revision: 1.3 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	EXPORT	fegetround
	NAME	fegetround
fegetround
	; The default rounding mode cannot be set on the ARM FPU without
	; handcoding the rounding mode into the instruction. We therefore
	; always default to round-to-nearest.
	MOV	a1, #0
	MOV	pc, lr

	END
