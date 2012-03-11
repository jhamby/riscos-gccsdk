; Tests DCFS, DCFSU, DCFD and DCFDU.

	AREA	Data, DATA, READONLY

	[ :LNOT: REFERENCE

	DCFS	0.,1.,2.,3.,  64.
	DCFS	.5
	DCFS	-0., -32., -32.23345
	DCFS	-0.E10, 32.E-8  , 64.54E3 , 9233.12094E-4

	; Floating point literal notation:
	DCFS	0f_3f000000, 0f_7FC00000

	DCFD	0.,1.,2.,3.,  64.
	DCFD	.5
	DCFD	-0., -32., -32.23345
	DCFD	-0.E10, 32.E-8  , 64.54E3 , 9233.12094E-4

	; Floating point literal notation:
	DCFD	0d_cdf766a13fed8bc5, 0d_FFF0000000000000

	; Unaligned floating point constants: DCFSU/DCFDU
	DCB	&11
	DCFSU	9233.12094E-4
	DCFDU	9233.12094E-4

	|

	; Note: objasm stores "-0." as "0." but this is IEEE wise wrong.
	; Float:
	DCD	0x00000000, 0x3f800000, 0x40000000, 0x40400000, 0x42800000
	DCD	0x3f000000
	DCD	0x80000000, 0xc2000000, 0xc200ef0e
	DCD	0x80000000, 0x34abcc77, 0x477c1c00, 0x3f6c5e2e

	; Floating point literal notation:
	DCD	0x3f000000, 0x7fc00000

	; Double:
	DCD	0x00000000, 0x00000000	; 0.
	DCD	0x3ff00000, 0x00000000	; 1.
	DCD	0x40000000, 0x00000000	; 2.
	DCD	0x40080000, 0x00000000	; 3.
	DCD	0x40500000, 0x00000000	; 64.

	DCD	0x3fe00000, 0x00000000	; .5

	DCD	0x80000000, 0x00000000	; -0.
	DCD	0xc0400000, 0x00000000	; -32.
	DCD	0xc0401de1, 0xb089a027	; -32.23345

	DCD	0x80000000, 0x00000000	; -0.E10
	DCD	0x3e95798e, 0xe2308c3a	; 32.E-8
	DCD	0x40ef8380, 0x00000000	; 64.54E3
	DCD	0x3fed8bc5, 0xcdf766a1	; 9233.12094E-4

	; Floating point literal notation:
	DCD	0x3fed8bc5, 0xcdf766a1
	DCD	0x00000000, 0xFFF00000

	; Unaligned floating point constants: DCFSU/DCFDU
	DCB	&11
	DCDU	0x3f6c5e2e
	DCDU	0x3fed8bc5, 0xcdf766a1

	]

	END
