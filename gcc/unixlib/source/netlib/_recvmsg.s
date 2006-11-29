; _recvmsg
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	|_recvmsg|
	NAME	_recvmsg
|_recvmsg|
	NetSWIsimple	XSocket_Recvmsg_1

	END
