;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math-asm/_log.s,v $
; $Date: 2002/11/26 21:52:07 $
; $Revision: 1.4 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	log
	EXPORT	logl
	NAME	log
log
logl
	STMFD	sp!, {a1, a2}
	LDFD	f0, [sp], #8
	LGND	f0, f0
	MOV	pc, lr

	END
