;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_modf1f.s,v $
; $Date: 2002/11/26 21:52:07 $
; $Revision: 1.1 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	modff
	NAME	modff
modff
	STR	a1, [sp, #-4]!
	LDFS	f0, [sp], #4
	RNDSZ	f1, f0
	STFS	f1, [a2, #0]
	SUFS	f0, f0, f1
	return	AL, pc, lr

	END
