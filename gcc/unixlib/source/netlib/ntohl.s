;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/ntohl.s,v $
; $Date: 2001/01/29 15:10:20 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	ntohl
	NAME	ntohl
ntohl
	EOR	r1, r0, r0, ROR #16
	BIC	r1, r1, #&FF0000
	MOV	r0, r0, ROR #8
	EOR	r0, r0, r1, LSR #8
	return	AL, pc, lr

	END
