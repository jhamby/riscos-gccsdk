@ _getpeername
@ Copyright (c) 2000-2006 UnixLib Developers

#include "unixlib/asm_dec.s"

	.text

	.global	_getpeername
	NAME	_getpeername
_getpeername:
	NetSWIsimple0	XSocket_Getpeername_1

	.end
