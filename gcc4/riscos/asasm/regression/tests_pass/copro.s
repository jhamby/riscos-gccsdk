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
		DCI	&EE010002
		DCI	&FE010002

		DCI	&EE4316B4
		DCI	&EE000010
		DCI	&EE000050
		DCI	&FE000010
		DCI	&FE000050

		DCI	&EE100010
		DCI	&EE100050
		DCI	&FE100010
		DCI	&FE100050
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
		DCI	&ED900001
		DCI	&ED100001

		DCI	&EDB00001
		DCI	&ED300001

		DCI	&ECB00001
		DCI	&EC300001

		DCI	&EC900010

		DCI	&EDD00001
		DCI	&ED500001

		DCI	&EDF00001
		DCI	&ED700001

		DCI	&ECF00001
		DCI	&EC700001

		DCI	&ECD00010

		DCI	&FD900001
		DCI	&FD100001

		DCI	&FDB00001
		DCI	&FD300001

		DCI	&FCB00001
		DCI	&FC300001

		DCI	&FC900010

		DCI	&FDD00001
		DCI	&FD500001

		DCI	&FDF00001
		DCI	&FD700001

		DCI	&FCF00001
		DCI	&FC700001

		DCI	&FCD00010
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
		DCI	&ED800001
		DCI	&ED000001

		DCI	&EDA00001
		DCI	&ED200001

		DCI	&ECA00001
		DCI	&EC200001

		DCI	&EC800010

		DCI	&EDC00001
		DCI	&ED400001

		DCI	&EDE00001
		DCI	&ED600001

		DCI	&ECE00001
		DCI	&EC600001

		DCI	&ECC00010

		DCI	&FD800001
		DCI	&FD000001

		DCI	&FDA00001
		DCI	&FD200001

		DCI	&FCA00001
		DCI	&FC200001

		DCI	&FC800010

		DCI	&FDC00001
		DCI	&FD400001

		DCI	&FDE00001
		DCI	&FD600001

		DCI	&FCE00001
		DCI	&FC600001

		DCI	&FCC00010
	]

		END
