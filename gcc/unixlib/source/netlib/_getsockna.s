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

	EXPORT	|_getsockname|
	NAME	_getsockname
|_getsockname|
	[ COMPAT_INET4 = 1
	NetSWIsimple0	XSocket_Getsockname
	|
	NetSWIsimple0	XSocket_Getsockname_1
	]

	END
