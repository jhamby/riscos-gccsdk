;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_exp.s,v $
; $Date: 2002/09/24 21:02:37 $
; $Revision: 1.3 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	expf
	NAME	expf
expf
	STR	a1, [sp, #-4]
	LDFS	f0, [sp, #-4]
	EXPS	f0, f0
	return	AL, pc, lr

	END
