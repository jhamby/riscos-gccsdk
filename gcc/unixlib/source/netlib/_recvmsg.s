; _recvmsg
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	|_recvmsg|
	NAME	_recvmsg
|_recvmsg|
	[ __UNIXLIB_COMPAT_INET4 > 0
	NetSWIsimple	XSocket_Recvmsg
	|
	NetSWIsimple	XSocket_Recvmsg_1
	]

	END
