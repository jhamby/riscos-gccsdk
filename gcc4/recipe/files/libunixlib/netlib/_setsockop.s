@ _setsockopt
@ Copyright (c) 2000-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	.global	_setsockopt
	NAME	_setsockopt
_setsockopt:
	STMFD	sp!, {r4, lr}
	LDR	r4, [sp, #8]
	NetSWI0	XSocket_Setsockopt
	LDMFD	sp!, {r4, pc}

	.end
