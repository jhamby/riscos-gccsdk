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

	EXPORT	|_sendmsg|
	NAME	_sendmsg
|_sendmsg|
	[ COMPAT_INET4 = 1
	NetSWIsimple	XSocket_Sendmsg
	|
	NetSWIsimple	XSocket_Sendmsg_1
	]

	END
