;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/htonl.s,v $
; $Date: 2002/09/24 21:02:37 $
; $Revision: 1.3 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	htonl
	NAME	htonl
htonl
	EOR	r1, r0, r0, ROR #16
	BIC	r1, r1, #&FF0000
	MOV	r0, r0, ROR #8
	EOR	r0, r0, r1, LSR #8
	MOV	pc, lr

	END
