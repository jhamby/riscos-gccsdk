;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/_bswap_32.s,v $
; $Date: 2002/12/22 18:22:29 $
; $Revision: 1.1 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	__bswap_32
	NAME	__bswap_32
__bswap_32
	EOR	r1, r0, r0, ROR #16
	BIC	r1, r1, #&FF0000
	MOV	r0, r0, ROR #8
	EOR	r0, r0, r1, LSR #8
	MOV	pc, lr

	END
