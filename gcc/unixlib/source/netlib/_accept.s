;----------------------------------------------------------------------------
;
; $Source$
; $Date$
; $Revision$
; $State$
; $Author$
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	IMPORT	|__net_error_simple_entry|

	EXPORT	|_accept|
	NAME	_accept
|_accept|
	[ COMPAT_INET4 = 1
	NetSWIsimple	XSocket_Accept
	|
	NetSWIsimple	XSocket_Accept_1
	]

	END
