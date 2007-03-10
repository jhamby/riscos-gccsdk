@ _sread
@ Copyright (c) 2000-2006 UnixLib Developers

#include "unixlib/asm_dec.s"

	.text

	.global	_sread
	NAME	_sread
_sread:
	NetSWIsimple	XSocket_Read


	.end
