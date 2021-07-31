; Tests basic functionality of AREA.  Especially when the same area is
; declared multiple times.

	; AREA without any attributes should be a DATA AREA with other default
	; attributes.
	[ :LNOT: REFERENCE
		AREA	Data1
	|
		AREA	Data1, DATA, REL, READWRITE, ALIGN=2
	]

	[ :LNOT: REFERENCE
		AREA	Data2
		ORG &0
	|
		AREA	Data2, ABS
	]

	; COMDEF + COMMON => COMDEF
	[ :LNOT: REFERENCE
		AREA	Data3, COMMON, COMDEF
		AREA	Data3, COMDEF
	|
		AREA	Data3, COMDEF
	]

	; COMDEF + NOINT => COMMON
	[ :LNOT: REFERENCE
		AREA	Data4, COMDEF, NOINIT
		AREA	Data4, COMMON
	|
		AREA	Data4, COMMON
	]

	; Full option code area:
	AREA	CodeFull, ABS  , PIC, READONLY, COMDEF, ALIGN=5, VFP, REENTRANT, INTERWORK, HALFWORD, NOSWSTACKCHECK, CODEALIGN, CODE
	AREA	CodeFull

	; Full option data area:
	AREA	DataFull, REL  , PIC, READWRITE, ALIGN=6, VFP, BASED r10, DATA
	AREA	DataFull

	; Don't complain about AREA ABS difference (code area).
	AREA	Code5, CODE
	ORG	&500
	AREA	Code5, CODE

	; Don't complain about AREA ABS difference (data area).
	AREA	Data6
	ORG	&600
	AREA	Data6

	END

