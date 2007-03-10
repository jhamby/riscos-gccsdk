@ _recvmsg
@ Copyright (c) 2000-2006 UnixLib Developers

#include "unixlib/asm_dec.s"

	.text

	.global	_recvmsg
	NAME	_recvmsg
_recvmsg:
	NetSWIsimple	XSocket_Recvmsg_1

	.end
