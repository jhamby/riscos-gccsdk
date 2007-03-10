@ _sockstat
@ Copyright (c) 2000-2006 UnixLib Developers

#include "unixlib/asm_dec.s"

	.text

	.global	_sockstat
	NAME	_sockstat
_sockstat:
	NetSWIsimple	XSocket_Stat

	.end
