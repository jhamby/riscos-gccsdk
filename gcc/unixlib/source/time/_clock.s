;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/_clock.s,v $
; $Date: 2002/09/24 21:02:38 $
; $Revision: 1.3 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	; clock_t clock (void)
	; Return the elapsed processor time. The base time is
	; arbitary, but doesn't change.
	EXPORT	clock
	NAME	clock
clock
	SWI	XOS_ReadMonotonicTime
	MOV	pc, lr

	END
