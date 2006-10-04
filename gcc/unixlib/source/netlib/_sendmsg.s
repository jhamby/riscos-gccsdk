; _sendmsg
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	|_sendmsg|
	NAME	_sendmsg
|_sendmsg|
	[ __UNIXLIB_COMPAT_INET4 > 0
	NetSWIsimple	XSocket_Sendmsg
	|
	NetSWIsimple	XSocket_Sendmsg_1
	]

	END
