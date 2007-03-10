@ _send
@ Copyright (c) 2000-2006 UnixLib Developers

#include "unixlib/asm_dec.s"

	.text

	.global	_send
	NAME	_send
_send:
	NetSWIsimple	XSocket_Send

	.end
