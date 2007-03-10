@ _sendmsg
@ Copyright (c) 2000-2006 UnixLib Developers

#include "unixlib/asm_dec.s"

	.text

	.global	_sendmsg
	NAME	_sendmsg
_sendmsg:
	NetSWIsimple	XSocket_Sendmsg_1

	.end
