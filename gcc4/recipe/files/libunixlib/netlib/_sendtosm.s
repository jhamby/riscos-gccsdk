@ _sendtosm
@ Copyright (c) 2000-2006 UnixLib Developers

#include "unixlib/asm_dec.s"

	.text

	.global	_sendtosm
	NAME	_sendtosm
_sendtosm:
	NetSWIsimple	XSocket_Sendtosm

	.end
