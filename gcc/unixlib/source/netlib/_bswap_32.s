; __bswap_32, htonl, ntohl
; Copyright (c) 2000-2006 UnixLib Developers

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
