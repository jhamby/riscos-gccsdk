; Tests :INDEX: and :BASE:

	AREA Code, CODE, READONLY

	[ :LNOT:REFERENCE

	; Test 1
	DCD	:INDEX:5		; :INDEX: on integer is allowed.

	; Test 2
	^	0
foo1	#	4
foo2	#	8
foo3	#	16
max	*	:INDEX:@

	DCD	max
	DCD	:INDEX:{VAR}

	; Test 3
	^ 0, r12
field1	# 16		; Offset 0 - &10
field2	# 8		; Offset &10 - &18
field3	# 4		; Offset &18 - &1c

	^ 256, r7
field4	# 32
field5	# 16
field6	# 4

	^ 512, r15
field7	# 64

	^ 1024
field8	# 128

	DCD	:INDEX:field2
	DCD	:INDEX:field6

	DCD	:BASE:field2
	DCD	:BASE:field4
	DCD	:BASE:field7

	LDR	r0, field4
	LDR	r0, [r1, #field8]
	MOV	r0, #field8

	; Test 4 : late definition
	DCD	:INDEX:late_field1
	DCD	:INDEX:late_field2
	[ EXTENSION
	DCD	:BASE:late_field1	; Not supported by objasm.
	]
	LDR	r0, late_field1
	LDR	r0, [r1, #late_field2]
	MOV	r0, #late_field2

	^	12, r11
	#	8
late_field1 #	20
	^	20
	#	16
late_field2 #	32

	|

	; Test 1
	DCD	5

	; Test 2
	DCD	28
	DCD	28

	; Test 3
	DCD	&10
	DCD	&130

	DCD	12
	DCD	7
	DCD	15

	LDR	r0, [r7, #256]
	LDR	r0, [r1, #0x400]
	MOV	r0, #&400

	; Test 4 : late definition
	DCD	20
	DCD	36
	[ EXTENSION
	DCD	11
	]
	LDR	r0, [r11, #20]
	LDR	r0, [r1, #36]
	MOV	r0, #36

	]

	; Test 5 : on labels
	[ :LNOT: REFERENCE

	AREA	Data5a, DATA
	DCD	1, 2, 3
Tst5L1	DCD	4, 5, 6

	DCD	:BASE:Tst5L1
	[ EXTENSION
	DCD	:INDEX:Tst5L1
	]

	AREA	Data5b, DATA, BASED r3
	DCD	1, 2, 3
Tst5L2	DCD	4, 5, 6

	DCD	:BASE:Tst5L2
	DCD	:INDEX:Tst5L2

	AREA	Data5c, DATA

	DCD	:BASE:Tst5L2
	DCD	:INDEX:Tst5L2

	|

	AREA	Data5a, DATA
	DCD	1, 2, 3
	DCD	4, 5, 6
	DCD	15
	[ EXTENSION
	DCD	12
	]

	AREA	Data5b, DATA, BASED r3
	DCD	1, 2, 3
	DCD	4, 5, 6
	DCD	3
	DCD	12

	AREA	Data5c, DATA
	DCD	3
	DCD	12

	]

	END
