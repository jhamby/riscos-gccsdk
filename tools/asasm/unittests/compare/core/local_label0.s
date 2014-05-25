	; Basic ROUT and local label test.

	AREA	Code1, CODE, READONLY
	[ :LNOT: REFERENCE

xyz	ROUT
01	MOV	r1, #1
02	MOV	r1, #2
	B	%01
	B	%02xyz

|.xyz|	ROUT
03	MOV	r1, #3
	B	%03|.xyz|
	
	|

c1lbl1	MOV	r1, #1
c1lbl2	MOV	r1, #2
	B	c1lbl1
	B	c1lbl2

c1lbl3	MOV	r1, #3
	B	c1lbl3

	]
	
	END
