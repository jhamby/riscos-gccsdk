@ _getsockname
@ Copyright (c) 2000-2006 UnixLib Developers

#include "unixlib/asm_dec.s"

	.text

	.global	_getsockname
	NAME	_getsockname
_getsockname:
	NetSWIsimple0	XSocket_Getsockname_1

	.end
