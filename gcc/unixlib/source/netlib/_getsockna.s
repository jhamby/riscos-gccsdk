;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/_getsockna.s,v $
; $Date: 2002/12/22 18:22:29 $
; $Revision: 1.4 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	IMPORT	|__net_error_simple_entry|

	EXPORT	|_getsockname|
	NAME	_getsockname
|_getsockname|
	NetSWIsimple0	XSocket_Getsockname
	END
