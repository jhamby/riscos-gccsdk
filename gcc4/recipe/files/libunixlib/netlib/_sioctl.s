@ _sioctl
@ Copyright (c) 2000-2006 UnixLib Developers

#include "unixlib/asm_dec.s"

	.text

	.global	_sioctl
	NAME	_sioctl
_sioctl:
	NetSWIsimple0	XSocket_Ioctl

	.end
