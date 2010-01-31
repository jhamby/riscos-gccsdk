@ _sendtosm
@ Copyright (c) 2000-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	.global	_sendtosm
	NAME	_sendtosm
_sendtosm:
	NetSWIsimple	XSocket_Sendtosm

	.end
