@ _swrite
@ Copyright (c) 2000-2006 UnixLib Developers

#include "unixlib/asm_dec.s"

	.text

	.global	_swrite
	NAME	_swrite
_swrite:
	NetSWIsimple	XSocket_Write

	.end
