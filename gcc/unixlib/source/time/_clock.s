;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/_clock.s,v $
; $Date: 2000/07/15 14:52:43 $
; $Revision: 1.1.1.1 $
; $State: Exp $
; $Author: nick $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	; clock_t clock (void)
	; Return the elapsed processor time. The base time is
	; arbitary, but doesn't change.
	EXPORT	clock
clock
	SWI	XOS_ReadMonotonicTime
	return	AL, pc, lr

	END
