;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_acosf.s,v $
; $Date: 2003/05/13 21:48:10 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: alex $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	acosf
	NAME	acosf
acosf
	[ __UNIXLIB_NORCROFT_BUILD > 0
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	|
	STR	a1, [sp, #-4]!
	LDFS	f0, [sp], #4
	]
	ACSS	f0, f0
	MOV	pc, lr

	END
