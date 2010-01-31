@ _sread
@ Copyright (c) 2000-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	.global	_sread
	NAME	_sread
_sread:
	NetSWIsimple	XSocket_Read


	.end
