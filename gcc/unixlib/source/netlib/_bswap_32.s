;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/_bswap_32.s,v $
; $Date: 2004/10/17 16:24:44 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: joty $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	__bswap_32
	EXPORT	htonl
	EXPORT	ntohl
	NAME	__bswap_32
__bswap_32
htonl
ntohl
	EOR	r1, r0, r0, ROR #16
	BIC	r1, r1, #&FF0000
	MOV	r0, r0, ROR #8
	EOR	r0, r0, r1, LSR #8
	MOV	pc, lr

	END
