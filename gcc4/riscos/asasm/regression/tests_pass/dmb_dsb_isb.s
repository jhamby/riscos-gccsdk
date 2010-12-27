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

	DCD	&f57ff05f
	DCD	&f57ff05f
	DCD	&f57ff05f
	DCD	&f57ff05e
	DCD	&f57ff05b
	DCD	&f57ff05b
	DCD	&f57ff05a
	DCD	&f57ff05a
	DCD	&f57ff057
	DCD	&f57ff057
	DCD	&f57ff056
	DCD	&f57ff056
	DCD	&f57ff053
	DCD	&f57ff052

	DCD	&f57ff04f
	DCD	&f57ff04f
	DCD	&f57ff04f
	DCD	&f57ff04e
	DCD	&f57ff04b
	DCD	&f57ff04b
	DCD	&f57ff04a
	DCD	&f57ff04a
	DCD	&f57ff047
	DCD	&f57ff047
	DCD	&f57ff046
	DCD	&f57ff046
	DCD	&f57ff043
	DCD	&f57ff042

	DCD	&f57ff06f
	DCD	&f57ff06f
	DCD	&f57ff06f
	DCD	&f57ff06e
	DCD	&f57ff06b
	DCD	&f57ff06b
	DCD	&f57ff06a
	DCD	&f57ff06a
	DCD	&f57ff067
	DCD	&f57ff067
	DCD	&f57ff066
	DCD	&f57ff066
	DCD	&f57ff063
	DCD	&f57ff062

	]

	END