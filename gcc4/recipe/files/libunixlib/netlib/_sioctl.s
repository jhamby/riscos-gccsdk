@ _sioctl
@ Copyright (c) 2000-2008 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	.global	_sioctl
	NAME	_sioctl
_sioctl:
	NetSWIsimple0	XSocket_Ioctl

	.end
