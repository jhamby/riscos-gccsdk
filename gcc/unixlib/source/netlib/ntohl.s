;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/netlib/s/ntohl,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.5 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	ntohl
ntohl
	EOR	r1, r0, r0, ROR #16
	BIC	r1, r1, #&FF0000
	MOV	r0, r0, ROR #8
	EOR	r0, r0, r1, LSR #8
	return	AL, pc, lr

	END
