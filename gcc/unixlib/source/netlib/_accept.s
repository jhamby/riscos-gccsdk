;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/_accept.s,v $
; $Date: 2002/07/19 13:12:30 $
; $Revision: 1.2.2.1 $
; $State: Exp $
; $Author: admin $
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
