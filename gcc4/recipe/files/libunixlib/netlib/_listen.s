@ _listen
@ Copyright (c) 2000-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	.global	_listen
	NAME	_listen
_listen:
	NetSWIsimple0	XSocket_Listen

	.end
