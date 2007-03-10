@ _readv
@ Copyright (c) 2000-2006 UnixLib Developers

#include "unixlib/asm_dec.s"

	.text

	.global	_readv
	NAME	_readv
_readv:
	NetSWIsimple	XSocket_Readv

	.end
