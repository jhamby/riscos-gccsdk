@ _getpeername
@ Copyright (c) 2000-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	.global	_getpeername
	NAME	_getpeername
_getpeername:
	NetSWIsimple0	XSocket_Getpeername_1

	.end
