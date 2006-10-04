; _gettsize
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	|_gettsize|
	NAME	_gettsize
|_gettsize|
	NetSWIsimple	XSocket_Gettsize

	END
