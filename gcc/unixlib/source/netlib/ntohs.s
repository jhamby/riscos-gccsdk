;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/netlib/s/ntohs,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.5 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	ntohs
ntohs
	MOV	r2, #255
	AND	r1, r2, r0, LSR #8
	AND	r0, r0, r2
	ORR	r0, r1, r0, LSL #8
	return	AL, pc, lr

	END
