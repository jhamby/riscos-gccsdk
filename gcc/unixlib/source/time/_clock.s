;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/time/_clock.s,v $
; $Date: 2001/01/29 15:10:22 $
; $Revision: 1.2 $
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
	return	AL, pc, lr

	END
