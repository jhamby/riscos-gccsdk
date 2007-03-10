@ _gettsize
@ Copyright (c) 2000-2006 UnixLib Developers

#include "unixlib/asm_dec.s"

	.text

	.global	_gettsize
	NAME	_gettsize
_gettsize:
	NetSWIsimple	XSocket_Gettsize

	.end
