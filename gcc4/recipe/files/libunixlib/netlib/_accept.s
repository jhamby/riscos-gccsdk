@ _accept
@ Copyright (c) 2000-2006 UnixLib Developers

#include "unixlib/asm_dec.s"

	.text

	.global	_accept
	NAME	_accept
_accept:
	NetSWIsimple	XSocket_Accept_1

	.end
