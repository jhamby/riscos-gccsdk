;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/_printf.s,v $
; $Date: 2001/01/29 15:10:21 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	printf
	EXPORT	|_printf|
	NAME	_printf
|_printf|
	B	printf

	IMPORT	fprintf
	EXPORT	|_fprintf|
	NAME	_fprintf
|_fprintf|
	B	fprintf

	IMPORT	sprintf
	EXPORT	|_sprintf|
	NAME	_sprintf
|_sprintf|
	B	sprintf

	IMPORT	vfprintf
	EXPORT	|_vfprintf|
	NAME	_vfprintf
|_vfprintf|
	B	vfprintf

	END
