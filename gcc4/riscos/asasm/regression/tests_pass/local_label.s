	AREA	Code, CODE, READONLY

	[ :LNOT: REFERENCE

	B	%f01
01	MOV	r1, #1
	B	%b01

	B	%01	; First backward search, then forward.

	B	%f01
01	MOV	r1, #2
	B	%b01

	B	%01

xyz	ROUT
	B	%01

	B	%f01
01	MOV	r1, #3
	B	%b01

	B	%01

	B	%f01
01	MOV	r1, #4
	B	%b01

	B	%01xyz

	B	%f01
01xyz	MOV	r1, #5
	B	%b01

	B	%f01xyz
01xyz	MOV	r1, #6
	B	%b01xyz

	; 'test' should be defined a global label
test	ROUT
	MOV	r0, #1
2	MOV	r1, #2
	MOV	r2, #3
	B	%2test
	B	test
	ROUT
	B	test

	|

	B	lbl01
lbl01	MOV	r1, #1
	B	lbl01

	B	lbl01

	B	lbl02
lbl02	MOV	r1, #2
	B	lbl02

	B	lbl02

	B	lbl03

	B	lbl03
lbl03	MOV	r1, #3
	B	lbl03

	B	lbl03

	B	lbl04
lbl04	MOV	r1, #4
	B	lbl04

	B	lbl04

	B	lbl05
lbl05	MOV	r1, #5
	B	lbl05

	B	lbl06
lbl06	MOV	r1, #6
	B	lbl06

test
	MOV	r0, #1
	MOV	r1, #2
	MOV	r2, #3
	B	test + 4
	B	test
	B	test

	]

	END
