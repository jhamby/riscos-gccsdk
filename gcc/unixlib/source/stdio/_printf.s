; _printf, _fprintf, _sprintf, _vfprintf, _vprintf, _snprintf
; _vsprintf, _vsnprintf
; Copyright (c) 2000-2006 UnixLib Developers

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

	IMPORT	vprintf
	EXPORT	|_vprintf|
	NAME	_vprintf
|_vprintf|
	B	vprintf

	IMPORT	snprintf
	EXPORT	|_snprintf|
	NAME	_snprintf
|_snprintf|
	B	snprintf

	IMPORT	vsprintf
	EXPORT	|_vsprintf|
	NAME	_vsprintf
|_vsprintf|
	B	vsprintf

	IMPORT	vsnprintf
	EXPORT	|_vsnprintf|
	NAME	_vsnprintf
|_vsnprintf|
	B	vsnprintf
	
	END
