@ _sendto
@ Copyright (c) 2000-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	.global	_sendto
	NAME	_sendto
_sendto:
	STMFD	sp!, {r4-r5, lr}
	LDR	r4, [sp, #12]
	LDR	r5, [sp, #16]
	NetSWI	XSocket_Sendto
	LDMFD	sp!, {r4-r5, pc}

	.end
