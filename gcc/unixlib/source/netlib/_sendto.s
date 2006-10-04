; _sendto
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	|_sendto|
	NAME	_sendto
|_sendto|
	STMFD	sp!, {r4-r5, lr}
	LDR	r4, [sp, #12]
	LDR	r5, [sp, #16]
	NetSWI	XSocket_Sendto
	LDMFD	sp!, {r4-r5, pc}

	END
