@ _listen
@ Copyright (c) 2000-2006 UnixLib Developers

#include "unixlib/asm_dec.s"

	.text

	.global	_listen
	NAME	_listen
_listen:
	NetSWIsimple0	XSocket_Listen

	.end
