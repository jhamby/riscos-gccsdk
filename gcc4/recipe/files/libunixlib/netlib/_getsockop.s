@ _getsockopt
@ Copyright (c) 2000-2006 UnixLib Developers

#include "unixlib/asm_dec.s"

	.text

	.global	_getsockopt
	NAME	_getsockopt
_getsockopt:
	STMFD	sp!, {r4, lr}
	LDR	r4, [sp, #8]
	NetSWI0	XSocket_Getsockopt
	LDMFD	sp!, {r4, pc}

	.end
