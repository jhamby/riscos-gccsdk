; __bswap_16, htons, ntohs
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	__bswap_16
	EXPORT	htons
	EXPORT	ntohs
	NAME	__bswap_16
__bswap_16
htons
ntohs
	MOV	r2, #255
	AND	r1, r2, r0, LSR #8
	AND	r0, r0, r2
	ORR	r0, r1, r0, LSL #8
	MOV	pc, lr

	END
