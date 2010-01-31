@ _readv
@ Copyright (c) 2000-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	.global	_readv
	NAME	_readv
_readv:
	NetSWIsimple	XSocket_Readv

	.end
