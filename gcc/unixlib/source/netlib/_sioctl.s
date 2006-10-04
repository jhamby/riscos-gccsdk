; _sioctl
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	|_sioctl|
	NAME	_sioctl
|_sioctl|
	NetSWIsimple0	XSocket_Ioctl

	END
