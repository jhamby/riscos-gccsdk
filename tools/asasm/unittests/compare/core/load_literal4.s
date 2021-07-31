; Tests literal reuse.

	; Test 1 : floating point literals (single precision)
	[ {TRUE}
	AREA	CodeTest1, CODE
	[ :LNOT: REFERENCE
DefTest1_Val1	* 1111.
	LDFS	F1, =DefTest1_Val1
	LDFS	F2, =DefTest1_Val2
	LDFS	F3, =DefTest1_Val3

	LDFS	F1, =DefTest1_Val1
	LDFS	F2, =DefTest1_Val2
	LDFS	F3, =DefTest1_Val3

DefTest1_Val2	* 2222.
	LTORG

	LDFS	F1, =DefTest1_Val1
	LDFS	F2, =DefTest1_Val2
	LDFS	F3, =DefTest1_Val3

DefTest1_Val3	* 3333.
	LTORG

	LDFS	F1, =DefTest1_Val1
	LDFS	F2, =DefTest1_Val2
	LDFS	F3, =DefTest1_Val3
	|
	LDFS	F1, LTTest1_Val1
	LDFS	F2, LTTest1_Val2
	LDFS	F3, LTTest1_Val3

	LDFS	F1, LTTest1_Val1
	LDFS	F2, LTTest1_Val2
	LDFS	F3, LTTest1_Val3

LTTest1_Val1	DCFS	1111.
LTTest1_Val2	DCFS	2222.
LTTest1_Val3	DCFS	3333.

	LDFS	F1, LTTest1_Val1
	LDFS	F2, LTTest1_Val2
	LDFS	F3, LTTest1_Val3

	LDFS	F1, LTTest1_Val1
	LDFS	F2, LTTest1_Val2
	LDFS	F3, LTTest1_Val3
	]
	]

	; Test 2 : floating point literals (double precision)
	[ {TRUE}
	AREA	CodeTest2, CODE
	[ :LNOT: REFERENCE
DefTest2_Val1	* 1111.
	LDFD	F1, =DefTest2_Val1
	LDFD	F2, =DefTest2_Val2
	LDFD	F3, =DefTest2_Val3

	LDFD	F1, =DefTest2_Val1
	LDFD	F2, =DefTest2_Val2
	LDFD	F3, =DefTest2_Val3

DefTest2_Val2	* 2222.
	LTORG

	LDFD	F1, =DefTest2_Val1
	LDFD	F2, =DefTest2_Val2
	LDFD	F3, =DefTest2_Val3

DefTest2_Val3	* 3333.
	LTORG

	LDFD	F1, =DefTest2_Val1
	LDFD	F2, =DefTest2_Val2
	LDFD	F3, =DefTest2_Val3
	|
	LDFD	F1, LTTest2_Val1
	LDFD	F2, LTTest2_Val2
	LDFD	F3, LTTest2_Val3

	LDFD	F1, LTTest2_Val1
	LDFD	F2, LTTest2_Val2
	LDFD	F3, LTTest2_Val3

LTTest2_Val1	DCFD	1111.
LTTest2_Val2	DCFD	2222.
LTTest2_Val3	DCFD	3333.

	LDFD	F1, LTTest2_Val1
	LDFD	F2, LTTest2_Val2
	LDFD	F3, LTTest2_Val3

	LDFD	F1, LTTest2_Val1
	LDFD	F2, LTTest2_Val2
	LDFD	F3, LTTest2_Val3
	]
	]

	; Test 3 : Literal floating point optimisation (single precision)
	[ {TRUE}
	AREA	CodeTest3, CODE
	[ :LNOT: REFERENCE
DefTest3_Val1	* 1.
	LDFS	F1, =DefTest3_Val1
	LDFS	F2, =DefTest3_Val2
	LDFS	F3, =DefTest3_Val3

	LDFS	F1, =DefTest3_Val1
	LDFS	F2, =DefTest3_Val2
	LDFS	F3, =DefTest3_Val3

DefTest3_Val2	* 2.
	LTORG			; This will result in 3. output although it is not used.

	LDFS	F1, =DefTest3_Val1
	LDFS	F2, =DefTest3_Val2
	LDFS	F3, =DefTest3_Val3
DefTest3_Val3	* 3.
	LTORG

	LDFS	F1, =DefTest3_Val1
	LDFS	F2, =DefTest3_Val2
	LDFS	F3, =DefTest3_Val3
	|
	MVFS	F1, #1.
	MVFS	F2, #2.
	MVFS	F3, #3.

	MVFS	F1, #1.
	MVFS	F2, #2.
	MVFS	F3, #3.
	
	DCFS	3.		; Too bad.

	MVFS	F1, #1.
	MVFS	F2, #2.
	MVFS	F3, #3.

	MVFS	F1, #1.
	MVFS	F2, #2.
	MVFS	F3, #3.
	]
	]

	; Test 4 : Literal floating point optimisation (double precision)
	[ {TRUE}
	AREA	CodeTest4, CODE
	[ :LNOT: REFERENCE
DefTest4_Val1	* 1.
	LDFD	F1, =DefTest4_Val1
	LDFD	F2, =DefTest4_Val2
	LDFD	F3, =DefTest4_Val3

	LDFD	F1, =DefTest4_Val1
	LDFD	F2, =DefTest4_Val2
	LDFD	F3, =DefTest4_Val3

DefTest4_Val2	* 2.
	LTORG			; This will result in 3. output although it is not used.

	LDFD	F1, =DefTest4_Val1
	LDFD	F2, =DefTest4_Val2
	LDFD	F3, =DefTest4_Val3
DefTest4_Val3	* 3.
	LTORG

	LDFD	F1, =DefTest4_Val1
	LDFD	F2, =DefTest4_Val2
	LDFD	F3, =DefTest4_Val3
	|
	MVFD	F1, #1.
	MVFD	F2, #2.
	MVFD	F3, #3.

	MVFD	F1, #1.
	MVFD	F2, #2.
	MVFD	F3, #3.
	
	DCFD	3.		; Too bad.

	MVFD	F1, #1.
	MVFD	F2, #2.
	MVFD	F3, #3.

	MVFD	F1, #1.
	MVFD	F2, #2.
	MVFD	F3, #3.
	]
	]

	; Test 5 : integer literals
	[ {TRUE}
	AREA	CodeTest5, CODE
	[ :LNOT: REFERENCE
DefTest5_Val1	* 0x1111
	LDR	R1, =DefTest5_Val1
	LDR	R2, =DefTest5_Val2
	LDR	R3, =DefTest5_Val3

	LDR	R1, =DefTest5_Val1
	LDR	R2, =DefTest5_Val2
	LDR	R3, =DefTest5_Val3

DefTest5_Val2	* 0x2222
	LTORG

	LDR	R1, =DefTest5_Val1
	LDR	R2, =DefTest5_Val2
	LDR	R3, =DefTest5_Val3

DefTest5_Val3	* 0x3333
	LTORG

	LDR	R1, =DefTest5_Val1
	LDR	R2, =DefTest5_Val2
	LDR	R3, =DefTest5_Val3
	|
	LDR	R1, LTTest5_Val1
	LDR	R2, LTTest5_Val2
	LDR	R3, LTTest5_Val3

	LDR	R1, LTTest5_Val1
	LDR	R2, LTTest5_Val2
	LDR	R3, LTTest5_Val3

LTTest5_Val1	DCD	0x1111
LTTest5_Val2	DCD	0x2222
LTTest5_Val3	DCD	0x3333

	LDR	R1, LTTest5_Val1
	LDR	R2, LTTest5_Val2
	LDR	R3, LTTest5_Val3

	LDR	R1, LTTest5_Val1
	LDR	R2, LTTest5_Val2
	LDR	R3, LTTest5_Val3
	]
	]

	; Test 6 : Literal integer optimisation
	[ {TRUE}	
	AREA	CodeTest5, CODE
	[ :LNOT: REFERENCE
DefTest6_Val1	* 0x11
	LDR	R1, =DefTest6_Val1
	LDR	R2, =DefTest6_Val2
	LDR	R3, =DefTest6_Val3

	LDR	R1, =DefTest6_Val1
	LDR	R2, =DefTest6_Val2
	LDR	R3, =DefTest6_Val3

DefTest6_Val2	* 0x22
	LTORG			; This will result in 3. output although it is not used.

	LDR	R1, =DefTest6_Val1
	LDR	R2, =DefTest6_Val2
	LDR	R3, =DefTest6_Val3

DefTest6_Val3	* 0x33
	LTORG

	LDR	R1, =DefTest6_Val1
	LDR	R2, =DefTest6_Val2
	LDR	R3, =DefTest6_Val3
	|
	MOV	R1, #&11
	MOV	R2, #&22
	MOV	R3, #&33

	MOV	R1, #&11
	MOV	R2, #&22
	MOV	R3, #&33

	DCD	&33		; Too bad.

	MOV	R1, #&11
	MOV	R2, #&22
	MOV	R3, #&33

	MOV	R1, #&11
	MOV	R2, #&22
	MOV	R3, #&33
	]
	]

	END
