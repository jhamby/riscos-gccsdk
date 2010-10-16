		AREA |C$$code|, CODE, READONLY

		; Coprocessor instructions

	[ :LNOT: REFERENCE
		CDP	p0,0,c0,c1,c2,0
		CDP2	p0,0,c0,c1,c2,0

		MCR	p6,2,r1,c3,c4,5		; opcode1 test
		MCR	p0,0,r0,c0,c0
		MCR	p0,0,r0,c0,c0,2
		MCR2	p0,0,r0,c0,c0
		MCR2	p0,0,r0,c0,c0,2

		MRC	p0,0,r0,c0,c0
		MRC	p0,0,r0,c0,c0,2
		MRC2	p0,0,r0,c0,c0
		MRC2	p0,0,r0,c0,c0,2
	|
		DCD	&EE010002
		DCD	&FE010002

		DCD	&EE4316B4
		DCD	&EE000010
		DCD	&EE000050
		DCD	&FE000010
		DCD	&FE000050

		DCD	&EE100010
		DCD	&EE100050
		DCD	&FE100010
		DCD	&FE100050
	]


		; Addressing mode 5 - Load/Store Coprocessor
	[ :LNOT: REFERENCE
		; 1
		LDC	p0,c0,[r0,#4]
		LDC	p0,c0,[r0,#-4]
		; 2
		LDC	p0,c0,[r0,#4]!
		LDC	p0,c0,[r0,#-4]!
		; 3
		LDC	p0,c0,[r0],#4
		LDC	p0,c0,[r0],#-4
		; 4
		LDC	p0,c0,[r0],{16}

		; 1
		LDCL	p0,c0,[r0,#4]
		LDCL	p0,c0,[r0,#-4]
		; 2
		LDCL	p0,c0,[r0,#4]!
		LDCL	p0,c0,[r0,#-4]!
		; 3
		LDCL	p0,c0,[r0],#4
		LDCL	p0,c0,[r0],#-4
		; 4
		LDCL	p0,c0,[r0],{16}

		; 1
		LDC2	p0,c0,[r0,#4]
		LDC2	p0,c0,[r0,#-4]
		; 2
		LDC2	p0,c0,[r0,#4]!
		LDC2	p0,c0,[r0,#-4]!
		; 3
		LDC2	p0,c0,[r0],#4
		LDC2	p0,c0,[r0],#-4
		; 4
		LDC2	p0,c0,[r0],{16}

		; 1
		LDC2L	p0,c0,[r0,#4]
		LDC2L	p0,c0,[r0,#-4]
		; 2
		LDC2L	p0,c0,[r0,#4]!
		LDC2L	p0,c0,[r0,#-4]!
		; 3
		LDC2L	p0,c0,[r0],#4
		LDC2L	p0,c0,[r0],#-4
		; 4
		LDC2L	p0,c0,[r0],{16}
	|
		DCD	&ED900001
		DCD	&ED100001

		DCD	&EDB00001
		DCD	&ED300001

		DCD	&ECB00001
		DCD	&EC300001

		DCD	&EC900010

		DCD	&EDD00001
		DCD	&ED500001

		DCD	&EDF00001
		DCD	&ED700001

		DCD	&ECF00001
		DCD	&EC700001

		DCD	&ECD00010

		DCD	&FD900001
		DCD	&FD100001

		DCD	&FDB00001
		DCD	&FD300001

		DCD	&FCB00001
		DCD	&FC300001

		DCD	&FC900010

		DCD	&FDD00001
		DCD	&FD500001

		DCD	&FDF00001
		DCD	&FD700001

		DCD	&FCF00001
		DCD	&FC700001

		DCD	&FCD00010
	]

	[ :LNOT: REFERENCE
		; 1
		STC	p0,c0,[r0,#4]
		STC	p0,c0,[r0,#-4]
		; 2
		STC	p0,c0,[r0,#4]!
		STC	p0,c0,[r0,#-4]!
		; 3
		STC	p0,c0,[r0],#4
		STC	p0,c0,[r0],#-4
		; 4
		STC	p0,c0,[r0],{16}

		; 1
		STCL	p0,c0,[r0,#4]
		STCL	p0,c0,[r0,#-4]
		; 2
		STCL	p0,c0,[r0,#4]!
		STCL	p0,c0,[r0,#-4]!
		; 3
		STCL	p0,c0,[r0],#4
		STCL	p0,c0,[r0],#-4
		; 4
		STCL	p0,c0,[r0],{16}

		; 1
		STC2	p0,c0,[r0,#4]
		STC2	p0,c0,[r0,#-4]
		; 2
		STC2	p0,c0,[r0,#4]!
		STC2	p0,c0,[r0,#-4]!
		; 3
		STC2	p0,c0,[r0],#4
		STC2	p0,c0,[r0],#-4
		; 4
		STC2	p0,c0,[r0],{16}

		; 1
		STC2L	p0,c0,[r0,#4]
		STC2L	p0,c0,[r0,#-4]
		; 2
		STC2L	p0,c0,[r0,#4]!
		STC2L	p0,c0,[r0,#-4]!
		; 3
		STC2L	p0,c0,[r0],#4
		STC2L	p0,c0,[r0],#-4
		; 4
		STC2L	p0,c0,[r0],{16}
	|
		DCD	&ED800001
		DCD	&ED000001

		DCD	&EDA00001
		DCD	&ED200001

		DCD	&ECA00001
		DCD	&EC200001

		DCD	&EC800010

		DCD	&EDC00001
		DCD	&ED400001

		DCD	&EDE00001
		DCD	&ED600001

		DCD	&ECE00001
		DCD	&EC600001

		DCD	&ECC00010

		DCD	&FD800001
		DCD	&FD000001

		DCD	&FDA00001
		DCD	&FD200001

		DCD	&FCA00001
		DCD	&FC200001

		DCD	&FC800010

		DCD	&FDC00001
		DCD	&FD400001

		DCD	&FDE00001
		DCD	&FD600001

		DCD	&FCE00001
		DCD	&FC600001

		DCD	&FCC00010
	]

		END
