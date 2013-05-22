; -RUNOPT: -CPU=7-A

		AREA |C$$code|, CODE, READONLY

		; Coprocessor instructions

	[ :LNOT: REFERENCE
		CDP	p8, 9, c7, c4, c12
		CDP	p8 , 9 , c7 , c4 , c12 , 4
		CDP	p8 ,9 ,c7 ,c4 ,c12 , # 7
		CDP2	p8, 9, c7, c4, c12
		CDP2	p8 , 9 , c7 , c4 , c12 , 4
		CDP2	p8 ,9 ,c7 ,c4 ,c12 , # 7

		MCR	p8, 5, r7, c4, c12
		MCR	p8 , 5 , r7 , c4 , c12 , 4
		MCR	p8 ,5 ,r7 ,c4 ,c12 , # 7
		MCR2	p8, 5, r7, c4, c12
		MCR2	p8 , 5 , r7 , c4 , c12 , 4
		MCR2	p8 ,5 ,r7 ,c4 ,c12 , # 7

		MRC	p8, 5, r7, c4, c12
		MRC	p8 , 5 , r7 , c4 , c12 , 4
		MRC	p8 ,5 ,r7 ,c4 ,c12 , # 7
		MRC2	p8, 5, r7, c4, c12
		MRC2	p8 , 5 , r7 , c4 , c12 , 4
		MRC2	p8 ,5 ,r7 ,c4 ,c12 , # 7
	|
		DCI	&EE94780c
		DCI	&EE94788c
		DCI	&EE9478Ec
		DCI	&FE94780c
		DCI	&FE94788c
		DCI	&FE9478Ec

		DCI	&EEa4781c
		DCI	&EEa4789c
		DCI	&EEa478Fc
		DCI	&FEa4781c
		DCI	&FEa4789c
		DCI	&FEa478Fc

		DCI	&EEb4781c
		DCI	&EEb4789c
		DCI	&EEb478Fc
		DCI	&FEb4781c
		DCI	&FEb4789c
		DCI	&FEb478Fc
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
