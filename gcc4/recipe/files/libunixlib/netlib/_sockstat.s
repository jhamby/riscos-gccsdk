@ _sockstat
@ Copyright (c) 2000-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	.global	_sockstat
	NAME	_sockstat
_sockstat:
	NetSWIsimple	XSocket_Stat

	.end
