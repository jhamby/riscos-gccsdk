;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/time/s/_clock,v $
; $Date: 2000/12/21 15:09:13 $
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
clock
	SWI	XOS_ReadMonotonicTime
	return	AL, pc, lr

	END
