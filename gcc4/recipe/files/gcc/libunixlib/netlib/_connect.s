@ _connect
@ Copyright (c) 2000-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	.global	_connect
	NAME	_connect
_connect:
	NetSWIsimple0	XSocket_Connect

	.end
