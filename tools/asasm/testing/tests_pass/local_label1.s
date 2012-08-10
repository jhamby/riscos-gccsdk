	AREA	Code1, CODE, READONLY

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

	; 'test' should be defined as a global label
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

	AREA	Code2, CODE

	[ :LNOT: REFERENCE

	MACRO
$lbl	Level2
21
	MOV	R0, #2
$lbl
	MOV	R0, #2
;	B	%bt01
;	B	%bt10
;	B	%bt11
	B	%bt20
	B	%bt21
	MEND

	MACRO
$lbl	Level1
20	Level2
11
	MOV	R0, #1
$lbl
	MOV	R0, #1
;	B	%bt01
	B	%bt10
	B	%bt11
;	B	%bt20
;	B	%bt21
	MEND

10	Level1

01
	B	%bt01
;	B	%bt10
;	B	%bt11
;	B	%bt20
;	B	%bt21

	|

c2lbl0	MOV	R0, #2
c2lbl1	MOV	R0, #2
	B	c2lbl1
	B	c2lbl0

c2lbl2	MOV	R0, #1
c2lbl3	MOV	R0, #1
	B	c2lbl3
	B	c2lbl2

c2lbl4	B	c2lbl4

	]

	AREA	Code3, CODE

	[ :LNOT: REFERENCE

	MACRO
	Test
10	MOV	r0, #1
	MEND

	B	%ft10
	Test
10	MOV	r0, #0

	|

	B	c3lbl1
	MOV	r0, #1
c3lbl1	MOV	r0, #0

	]

	; Use local labels for non-branch purposes.
	AREA	Code4, CODE
	[ :LNOT: REFERENCE
	ADR	r0, %ft01
	MOV	r1, #0
	MOV	r2, #0
	MOV	r3, #0
01	MOV	r4, #0

	BL	%ft01
	MOV	r1, #0
	MOV	r2, #0
	MOV	r3, #0
01	MOV	r4, #0

	ADD	r0, r0, #%ft03 - %ft02
02	MOV	r1, #0
	MOV	r2, #0
	MOV	r3, #0
	MOV	r4, #0
03

02	MOV	r5, #0
	|
	ADR	r0, c4lbl1
	MOV	r1, #0
	MOV	r2, #0
	MOV	r3, #0
c4lbl1	MOV	r4, #0

	BL	c4lbl2
	MOV	r1, #0
	MOV	r2, #0
	MOV	r3, #0
c4lbl2	MOV	r4, #0

	ADD	r0, r0, #c4lbl4-c4lbl3
c4lbl3	MOV	r1, #0
	MOV	r2, #0
	MOV	r3, #0
	MOV	r4, #0
c4lbl4

	MOV	r5, #0
	]

	END
