@ _swrite
@ Copyright (c) 2000-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	.global	_swrite
	NAME	_swrite
_swrite:
	NetSWIsimple	XSocket_Write

	.end
