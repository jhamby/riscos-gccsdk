@ _accept
@ Copyright (c) 2000-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	.global	_accept
	NAME	_accept
_accept:
	NetSWIsimple	XSocket_Accept_1

	.end
