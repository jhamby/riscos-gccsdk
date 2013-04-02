; Tests DMB, DSB and ISB.
; -RUNOPT: -CPU=7-A

	AREA	Code, CODE

	[ :LNOT: REFERENCE

	DMB
	DMB		; Same as DMB SY
	DMB	SY
	DMB	ST
	DMB	LD	; ARMv8
	DMB	ISH
	DMB	SH	; To be avoided.
	DMB	ISHST
	DMB	SHST	; To be avoided.
	DMB	ISHLD	; ARMv8
	DMB	NSH
	DMB	UN	; To be avoided.
	DMB	NSHST
	DMB	UNST	; To be avoided.
	DMB	NSHLD	; ARMv8
	DMB	OSH
	DMB	OSHST
	DMB	OSHLD	; ARMv8

	DSB
	DSB		; Same as DSB SY
	DSB	SY
	DSB	ST
	DSB	LD	; ARMv8
	DSB	ISH
	DSB	SH	; To be avoided.
	DSB	ISHST
	DSB	SHST	; To be avoided.
	DSB	ISHLD	; ARMv8
	DSB	NSH
	DSB	UN	; To be avoided.
	DSB	NSHST
	DSB	UNST	; To be avoided.
	DSB	NSHLD	; ARMv8
	DSB	OSH
	DSB	OSHST
	DSB	OSHLD	; ARMv8

	ISB
	ISB		; Same as ISB SY
	ISB	SY
	ISB	ST	; Reserved
	ISB	LD	; Reserved - ARMv8
	ISB	ISH	; Reserved
	ISB	SH	; Reserved
	ISB	ISHST	; Reserved
	ISB	SHST	; Reserved
	ISB	ISHLD	; Reserved - ARMv8
	ISB	NSH	; Reserved
	ISB	UN	; Reserved
	ISB	NSHST	; Reserved
	ISB	UNST	; Reserved
	ISB	NSHLD	; Reserved - ARMv8
	ISB	OSH	; Reserved
	ISB	OSHST	; Reserved
	ISB	OSHLD	; Reserved - ARMv8

	|

	DCI	&f57ff05f
	DCI	&f57ff05f
	DCI	&f57ff05f
	DCI	&f57ff05e
	DCI	&f57ff05d	; ARMv8
	DCI	&f57ff05b
	DCI	&f57ff05b
	DCI	&f57ff05a
	DCI	&f57ff05a
	DCI	&f57ff059	; ARMv8
	DCI	&f57ff057
	DCI	&f57ff057
	DCI	&f57ff056
	DCI	&f57ff056
	DCI	&f57ff055	; ARMv8
	DCI	&f57ff053
	DCI	&f57ff052
	DCI	&f57ff051	; ARMv8

	DCI	&f57ff04f
	DCI	&f57ff04f
	DCI	&f57ff04f
	DCI	&f57ff04e
	DCI	&f57ff04d	; ARMv8
	DCI	&f57ff04b
	DCI	&f57ff04b
	DCI	&f57ff04a
	DCI	&f57ff04a
	DCI	&f57ff049	; ARMv8
	DCI	&f57ff047
	DCI	&f57ff047
	DCI	&f57ff046
	DCI	&f57ff046
	DCI	&f57ff045	; ARMv8
	DCI	&f57ff043
	DCI	&f57ff042
	DCI	&f57ff041	; ARMv8

	DCI	&f57ff06f
	DCI	&f57ff06f
	DCI	&f57ff06f
	DCI	&f57ff06e
	DCI	&f57ff06d	; ARMv8
	DCI	&f57ff06b
	DCI	&f57ff06b
	DCI	&f57ff06a
	DCI	&f57ff06a
	DCI	&f57ff069	; ARMv8
	DCI	&f57ff067
	DCI	&f57ff067
	DCI	&f57ff066
	DCI	&f57ff066
	DCI	&f57ff065	; ARMv8
	DCI	&f57ff063
	DCI	&f57ff062
	DCI	&f57ff061	; ARMv8

	]

	END
