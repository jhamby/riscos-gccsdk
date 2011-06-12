; Tests DMB, DSB and ISB.

	AREA	Code, CODE

	[ :LNOT: REFERENCE

	DMB
	DMB		; Same as DMB SY
	DMB	SY
	DMB	ST
	DMB	ISH
	DMB	SH	; To be avoided.
	DMB	ISHST
	DMB	SHST	; To be avoided.
	DMB	NSH
	DMB	UN	; To be avoided.
	DMB	NSHST
	DMB	UNST	; To be avoided.
	DMB	OSH
	DMB	OSHST

	DSB
	DSB		; Same as DSB SY
	DSB	SY
	DSB	ST
	DSB	ISH
	DSB	SH	; To be avoided.
	DSB	ISHST
	DSB	SHST	; To be avoided.
	DSB	NSH
	DSB	UN	; To be avoided.
	DSB	NSHST
	DSB	UNST	; To be avoided.
	DSB	OSH
	DSB	OSHST

	ISB
	ISB		; Same as ISB SY
	ISB	SY
	ISB	ST	; Reserved
	ISB	ISH	; Reserved
	ISB	SH	; Reserved
	ISB	ISHST	; Reserved
	ISB	SHST	; Reserved
	ISB	NSH	; Reserved
	ISB	UN	; Reserved
	ISB	NSHST	; Reserved
	ISB	UNST	; Reserved
	ISB	OSH	; Reserved
	ISB	OSHST	; Reserved

	|

	DCI	&f57ff05f
	DCI	&f57ff05f
	DCI	&f57ff05f
	DCI	&f57ff05e
	DCI	&f57ff05b
	DCI	&f57ff05b
	DCI	&f57ff05a
	DCI	&f57ff05a
	DCI	&f57ff057
	DCI	&f57ff057
	DCI	&f57ff056
	DCI	&f57ff056
	DCI	&f57ff053
	DCI	&f57ff052

	DCI	&f57ff04f
	DCI	&f57ff04f
	DCI	&f57ff04f
	DCI	&f57ff04e
	DCI	&f57ff04b
	DCI	&f57ff04b
	DCI	&f57ff04a
	DCI	&f57ff04a
	DCI	&f57ff047
	DCI	&f57ff047
	DCI	&f57ff046
	DCI	&f57ff046
	DCI	&f57ff043
	DCI	&f57ff042

	DCI	&f57ff06f
	DCI	&f57ff06f
	DCI	&f57ff06f
	DCI	&f57ff06e
	DCI	&f57ff06b
	DCI	&f57ff06b
	DCI	&f57ff06a
	DCI	&f57ff06a
	DCI	&f57ff067
	DCI	&f57ff067
	DCI	&f57ff066
	DCI	&f57ff066
	DCI	&f57ff063
	DCI	&f57ff062

	]

	END
