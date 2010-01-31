@ _gettsize
@ Copyright (c) 2000-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	.global	_gettsize
	NAME	_gettsize
_gettsize:
	NetSWIsimple	XSocket_Gettsize

	.end
