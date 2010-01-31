@ _send
@ Copyright (c) 2000-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	.global	_send
	NAME	_send
_send:
	NetSWIsimple	XSocket_Send

	.end
