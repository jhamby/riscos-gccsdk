@ _select
@ Copyright (c) 2000-2006 UnixLib Developers

#include "unixlib/asm_dec.s"

	.text

	.global	_select
	NAME	_select
_select:
	STMFD	sp!, {r4, lr}
	LDR	r4, [sp, #8]
	NetSWI	XSocket_Select
	LDMFD	sp!, {r4, pc}

	.end
