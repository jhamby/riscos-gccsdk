; Tests ADC, ADD, AND, BIC, EOR, MOV, MVN, ORR, RSB, RSC, SBC, SUB, CMN, CMP,
; TEQ, TST.

	; Addressing Mode 1 - Data processing operands

	AREA	Code, CODE
	[ :LNOT: REFERENCE

	; *** Part A:

	; <base>			; no conditional code
	; <base> "S"			; no conditional code
	; <base> <cond code>		: no S variant
	; <base> <cond code> "S"	; pre-UAL
	; <base> "S" <cond code>	; UAL
	MACRO
	Invoke $base, $threeArg
	; "ADD", etc. vs "MOV", etc. ?
	[ $threeArg
	SubInvoke3 $base
	SubInvoke3 $base.S
	|
	SubInvoke2 $base
	SubInvoke2 $base.S
	]
	LCLS	instr0
	LCLS	instr1
	LCLS	instr2
	LCLA	cnt
cnt	SETA	0
	WHILE	cnt < 17
	; 17 condition codes to be tested:
	;   - Condition code NV is left out from this test.
	;   - HS and LO are equivalents for CS and CC.
instr0	SETS	"$base" :CC: (("EQNECSCCMIPLVSVCHILSGELTGTLEALHSLO" :RIGHT: (34 - 2*cnt)) :LEFT: 2)
instr1	SETS	"$base" :CC: (("EQNECSCCMIPLVSVCHILSGELTGTLEALHSLO" :RIGHT: (34 - 2*cnt)) :LEFT: 2) :CC: "S"	; Pre-UAL
instr2	SETS	"$base.S" :CC: (("EQNECSCCMIPLVSVCHILSGELTGTLEALHSLO" :RIGHT: (34 - 2*cnt)) :LEFT: 2)	; UAL
	; "ADD", etc. vs "MOV", etc. ?
	[ $threeArg
	SubInvoke3 $instr0
	SubInvoke3 $instr1
	SubInvoke3 $instr2
	|
	SubInvoke2 $instr0
	SubInvoke2 $instr1
	SubInvoke2 $instr2
	]
cnt	SETA	cnt + 1
	WEND
	MEND

	; For ADC, ADD, AND, BIC, EOR, ORR, RSB, RSC, SBC, SUB.
	MACRO
	SubInvoke3 $base
	Inject "$base R1,R2,#0"			; 1
	Inject "$base R1,R2,#4"
	Inject "$base R1,R2,#&FF"
	Inject "$base R1,R2,#&3F0"
	Inject "$base R1,R2,R3"			; 2
	Inject "$base R1,R2,R3,LSL #2"		; 3
	Inject "$base R1,R2,R3,LSL #31"
	Inject "$base R1,R2,R3,LSL R4"		; 4
	Inject "$base R1,R2,R3,LSR #2"		; 5
	Inject "$base R1,R2,R3,LSR #32"
	Inject "$base R1,R2,R3,LSR R4"		; 6
	Inject "$base R1,R2,R3,ASR #2"		; 7
	Inject "$base R1,R2,R3,ASR #32"
	Inject "$base R1,R2,R3,ASR R4"		; 8
	Inject "$base R1,R2,R3,ROR #2"		; 9
	Inject "$base R1,R2,R3,ROR #31"
	Inject "$base R1,R2,R3,ROR R4"		; 10
	Inject "$base R1,R2,R3,RRX"		; 11
	MEND

	; For MOV, MVN, CMN, CMP, TEQ, TST.
	MACRO
	SubInvoke2 $base
	Inject "$base R1,#0"			; 1
	Inject "$base R1,#4"
	Inject "$base R1,#&FF"
	Inject "$base R1,#&3F0"
	Inject "$base R1,R2"			; 2
	Inject "$base R1,R2,LSL #2"		; 3
	Inject "$base R1,R2,LSL #31"
	Inject "$base R1,R2,LSL R3"		; 4
	Inject "$base R1,R2,LSR #2"		; 5
	Inject "$base R1,R2,LSR #32"
	Inject "$base R1,R2,LSR R3"		; 6
	Inject "$base R1,R2,ASR #2"		; 7
	Inject "$base R1,R2,ASR #32"
	Inject "$base R1,R2,ASR R3"		; 8
	Inject "$base R1,R2,ROR #2"		; 9
	Inject "$base R1,R2,ROR #31"
	Inject "$base R1,R2,ROR R3"		; 10
	Inject "$base R1,R2,RRX"		; 11
	MEND

	MACRO
	Inject $full
	$full
	; INFO	0, "\t$full"
	MEND

	Invoke	ADC, {TRUE}
	Invoke	ADD, {TRUE}
	Invoke	AND, {TRUE}
	Invoke	BIC, {TRUE}
	Invoke	EOR, {TRUE}
	Invoke	ORR, {TRUE}
	Invoke	RSB, {TRUE}
	Invoke	RSC, {TRUE}
	Invoke	SBC, {TRUE}
	Invoke	SUB, {TRUE}

	Invoke	MOV, {FALSE}
	Invoke	MVN, {FALSE}

	Invoke	CMN, {FALSE}
	Invoke	CMP, {FALSE}
	Invoke	TEQ, {FALSE}
	Invoke	TST, {FALSE}

	; *** Part B:

	; <base> = CMN/CMP/TST/TEQ
	; <base> <cond code> "P"
	; <base> <cond code> "SP" (a bit farfetched)
	; <base> <cond code> "PS" (a bit farfetched)
	MACRO
	InvokeP $base
	LCLS	instr0
	LCLS	instr1
	LCLS	instr2
	LCLA	cnt
cnt	SETA	0
	WHILE	cnt < 17
	; 17 condition codes to be tested:
	;   - Condition code NV is left out from this test.
	;   - HS and LO are equivalents for CS and CC.
instr0	SETS	"$base" :CC: (("EQNECSCCMIPLVSVCHILSGELTGTLEALHSLO" :RIGHT: (34 - 2*cnt)) :LEFT: 2) :CC: "P"
instr1	SETS	"$base" :CC: (("EQNECSCCMIPLVSVCHILSGELTGTLEALHSLO" :RIGHT: (34 - 2*cnt)) :LEFT: 2) :CC: "SP"
instr2	SETS	"$base" :CC: (("EQNECSCCMIPLVSVCHILSGELTGTLEALHSLO" :RIGHT: (34 - 2*cnt)) :LEFT: 2) :CC: "PS"
	Inject "$instr0 r1, #3"
	Inject "$instr1 r1, #3"
	Inject "$instr2 r1, #3"
cnt	SETA	cnt + 1
	WEND
	MEND

	InvokeP	CMN
	InvokeP	CMP
	InvokeP	TEQ
	InvokeP	TST

	|

	; *** Part A:

	; For ADC, ADD, AND, BIC, EOR, ORR, RSB, RSC, SBC, SUB.
	MACRO
	Base1 $instr
	DCI	$instr :OR: &02021000 ; xxx r1,r2,#0		; 1
	DCI	$instr :OR: &02021004 ; xxx r1,r2,#4
	DCI	$instr :OR: &020210ff ; xxx r1,r2,#0xff
	DCI	$instr :OR: &02021e3f ; xxx r1,r2,#0x3f0
	DCI	$instr :OR: &00021003 ; xxx r1,r2,r3		; 2
	DCI	$instr :OR: &00021103 ; xxx r1,r2,r3,LSL #2	; 3
	DCI	$instr :OR: &00021f83 ; xxx r1,r2,r3,LSL #31
	DCI	$instr :OR: &00021413 ; xxx r1,r2,r3,LSL r4	; 4
	DCI	$instr :OR: &00021123 ; xxx r1,r2,r3,LSR #2	; 5
	DCI	$instr :OR: &00021023 ; xxx r1,r2,r3,LSR #32
	DCI	$instr :OR: &00021433 ; xxx r1,r2,r3,LSR r4	; 6
	DCI	$instr :OR: &00021143 ; xxx r1,r2,r3,ASR #2	; 7
	DCI	$instr :OR: &00021043 ; xxx r1,r2,r3,ASR #32
	DCI	$instr :OR: &00021453 ; xxx r1,r2,r3,ASR r4	; 8
	DCI	$instr :OR: &00021163 ; xxx r1,r2,r3,ROR #2	; 9
	DCI	$instr :OR: &00021fe3 ; xxx r1,r2,r3,ROR #31
	DCI	$instr :OR: &00021473 ; xxx r1,r2,r3,ROR r4	; 10
	DCI	$instr :OR: &00021063 ; xxx r1,r2,r3,RRX	; 11
	MEND

	; For MOV, MVN
	MACRO
	Base2 $instr
	DCI	$instr :OR: &02001000 ; xxx R1,#0		; 1
	DCI	$instr :OR: &02001004 ; xxx R1,#4
	DCI	$instr :OR: &020010FF ; xxx R1,#&FF
	DCI	$instr :OR: &02001E3F ; xxx R1,#&3F0
	DCI	$instr :OR: &00001002 ; xxx R1,R2		; 2
	DCI	$instr :OR: &00001102 ; xxx R1,R2,LSL #2	; 3
	DCI	$instr :OR: &00001F82 ; xxx R1,R2,LSL #31
	DCI	$instr :OR: &00001312 ; xxx R1,R2,LSL R3	; 4
	DCI	$instr :OR: &00001122 ; xxx R1,R2,LSR #2	; 5
	DCI	$instr :OR: &00001022 ; xxx R1,R2,LSR #32
	DCI	$instr :OR: &00001332 ; xxx R1,R2,LSR R3	; 6
	DCI	$instr :OR: &00001142 ; xxx R1,R2,ASR #2	; 7
	DCI	$instr :OR: &00001042 ; xxx R1,R2,ASR #32
	DCI	$instr :OR: &00001352 ; xxx R1,R2,ASR R3	; 8
	DCI	$instr :OR: &00001162 ; xxx R1,R2,ROR #2	; 9
	DCI	$instr :OR: &00001FE2 ; xxx R1,R2,ROR #31
	DCI	$instr :OR: &00001372 ; xxx R1,R2,ROR R3	; 10
	DCI	$instr :OR: &00001062 ; xxx R1,R2,RRX		; 11
	MEND

	; For CMN, CMP, TEQ, TST.
	MACRO
	Base3 $instr
	DCI	$instr :OR: &02110000 ; xxx R1,#0		; 1
	DCI	$instr :OR: &02110004 ; xxx R1,#4
	DCI	$instr :OR: &021100FF ; xxx R1,#&FF
	DCI	$instr :OR: &02110E3F ; xxx R1,#&3F0
	DCI	$instr :OR: &00110002 ; xxx R1,R2		; 2
	DCI	$instr :OR: &00110102 ; xxx R1,R2,LSL #2	; 3
	DCI	$instr :OR: &00110F82 ; xxx R1,R2,LSL #31
	DCI	$instr :OR: &00110312 ; xxx R1,R2,LSL R3	; 4
	DCI	$instr :OR: &00110122 ; xxx R1,R2,LSR #2	; 5
	DCI	$instr :OR: &00110022 ; xxx R1,R2,LSR #32
	DCI	$instr :OR: &00110332 ; xxx R1,R2,LSR R3	; 6
	DCI	$instr :OR: &00110142 ; xxx R1,R2,ASR #2	; 7
	DCI	$instr :OR: &00110042 ; xxx R1,R2,ASR #32
	DCI	$instr :OR: &00110352 ; xxx R1,R2,ASR R3	; 8
	DCI	$instr :OR: &00110162 ; xxx R1,R2,ROR #2	; 9
	DCI	$instr :OR: &00110FE2 ; xxx R1,R2,ROR #31
	DCI	$instr :OR: &00110372 ; xxx R1,R2,ROR R3	; 10
	DCI	$instr :OR: &00110062 ; xxx R1,R2,RRX		; 11
	MEND

	MACRO
	Variants $instr, $flavour
	Base$flavour $instr
	Base$flavour $instr :OR: &100000	; S bit set
	Base$flavour $instr :OR: &100000	; S bit set
	MEND

	MACRO
	Invoke $instr, $flavour
	Base$flavour $instr :OR: (14 :SHL:28)		   ; <base>
	Base$flavour $instr :OR: &100000 :OR: (14 :SHL:28) ; <base> "S"

	Variants $instr :OR: (0 :SHL:28), $flavour ; EQ
	Variants $instr :OR: (1 :SHL:28), $flavour ; NE
	Variants $instr :OR: (2 :SHL:28), $flavour ; CS
	Variants $instr :OR: (3 :SHL:28), $flavour ; CC
	Variants $instr :OR: (4 :SHL:28), $flavour ; MI
	Variants $instr :OR: (5 :SHL:28), $flavour ; PL
	Variants $instr :OR: (6 :SHL:28), $flavour ; VS
	Variants $instr :OR: (7 :SHL:28), $flavour ; VC
	Variants $instr :OR: (8 :SHL:28), $flavour ; HI
	Variants $instr :OR: (9 :SHL:28), $flavour ; LS
	Variants $instr :OR: (10 :SHL:28), $flavour ; GE
	Variants $instr :OR: (11 :SHL:28), $flavour ; LT
	Variants $instr :OR: (12 :SHL:28), $flavour ; GT
	Variants $instr :OR: (13 :SHL:28), $flavour ; LE
	Variants $instr :OR: (14 :SHL:28), $flavour ; AL
	Variants $instr :OR: (2 :SHL:28), $flavour ; HS (= CS)
	Variants $instr :OR: (3 :SHL:28), $flavour ; LO (= CC)
	MEND

	Invoke 0x00a00000, 1 ; ADC
	Invoke 0x00800000, 1 ; ADD
	Invoke 0x00000000, 1 ; AND
	Invoke 0x01c00000, 1 ; BIC
	Invoke 0x00200000, 1 ; EOR
	Invoke 0x01800000, 1 ; ORR
	Invoke 0x00600000, 1 ; RSB
	Invoke 0x00e00000, 1 ; RSC
	Invoke 0x00c00000, 1 ; SBC
	Invoke 0x00400000, 1 ; SUB

	Invoke 0x01a00000, 2 ; MOV
	Invoke 0x01e00000, 2 ; MVN

	Invoke 0x01600000, 3 ; CMN
	Invoke 0x01400000, 3 ; CMP
	Invoke 0x01200000, 3 ; TEQ
	Invoke 0x01000000, 3 ; TST

	; *** Part B:

	MACRO
	VariantsP $instr
	DCI $instr :OR: &02110003 :OR: &F000		; P bit set
	DCI $instr :OR: &02110003 :OR: &F000		; P bit set
	DCI $instr :OR: &02110003 :OR: &F000		; P bit set
	MEND

	MACRO
	InvokeP $instr
	VariantsP $instr :OR: (0 :SHL:28) ; EQ
	VariantsP $instr :OR: (1 :SHL:28) ; NE
	VariantsP $instr :OR: (2 :SHL:28) ; CS
	VariantsP $instr :OR: (3 :SHL:28) ; CC
	VariantsP $instr :OR: (4 :SHL:28) ; MI
	VariantsP $instr :OR: (5 :SHL:28) ; PL
	VariantsP $instr :OR: (6 :SHL:28) ; VS
	VariantsP $instr :OR: (7 :SHL:28) ; VC
	VariantsP $instr :OR: (8 :SHL:28) ; HI
	VariantsP $instr :OR: (9 :SHL:28) ; LS
	VariantsP $instr :OR: (10 :SHL:28) ; GE
	VariantsP $instr :OR: (11 :SHL:28) ; LT
	VariantsP $instr :OR: (12 :SHL:28) ; GT
	VariantsP $instr :OR: (13 :SHL:28) ; LE
	VariantsP $instr :OR: (14 :SHL:28) ; AL
	VariantsP $instr :OR: (2 :SHL:28) ; HS (= CS)
	VariantsP $instr :OR: (3 :SHL:28) ; LO (= CC)
	MEND

	InvokeP 0x01600000 ; CMN
	InvokeP 0x01400000 ; CMP
	InvokeP 0x01200000 ; TEQ
	InvokeP 0x01000000 ; TST

	]

	END
