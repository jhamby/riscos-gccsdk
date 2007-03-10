@ _connect
@ Copyright (c) 2000-2006 UnixLib Developers

#include "unixlib/asm_dec.s"

	.text

	.global	_connect
	NAME	_connect
_connect:
	NetSWIsimple0	XSocket_Connect

	.end
