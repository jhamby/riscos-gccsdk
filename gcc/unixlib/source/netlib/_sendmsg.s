;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/_sendmsg.s,v $
; $Date: 2002/07/19 13:12:30 $
; $Revision: 1.2.2.1 $
; $State: Exp $
; $Author: admin $
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
