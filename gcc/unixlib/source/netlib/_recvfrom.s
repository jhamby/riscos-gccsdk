; _recvfrom
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	|_recvfrom|
	NAME	_recvfrom
|_recvfrom|
	STMFD	sp!, {r4-r5, lr}
	LDR	r4, [sp, #12]
	LDR	r5, [sp, #16]
	NetSWI	XSocket_Recvfrom_1
	LDMFD	sp!, {r4-r5, pc}

	END
