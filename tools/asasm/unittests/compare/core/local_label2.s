	AREA	Code1, CODE

	[ :LNOT: REFERENCE
	MACRO
	Level1
101
	MOV	r0, #1
	Level2
	MOV	r1, #1

	; B	%b301
	; B	%b201
	B	%b101
	B	%b001
	MEND
	
	MACRO
	Level2
201
	MOV	r0, #2
	Level3
	MOV	r1, #2

	; B	%b301
	B	%b201
	B	%b101
	B	%b001
	
	B	%ba301	; All macro levels
	MEND
	
	MACRO
	Level3
301	
	MOV	r0, #3
	B	%b301	; This macro level and towards the top.
	B	%b201
	B	%b101
	B	%b001
	MEND

001	
	MOV	r0, #0
	Level1
	
	|

Lbl0	MOV      r0,#0
Lbl1	MOV      r0,#1
Lbl2	MOV      r0,#2
Lbl3	MOV      r0,#3
	B        Lbl3
	B        Lbl2
	B        Lbl1
	B        Lbl0
	MOV      r1,#2
	B        Lbl2
	B        Lbl1
	B        Lbl0
	B        Lbl3
	MOV      r1,#1
	B        Lbl1
	B        Lbl0
	
	]

	AREA	Code2, CODE
	[ :LNOT: REFERENCE
	MACRO
$label	Code2Test1
	MOV	r7, #7
$label	MOV	r8, #8
	MEND

	; Local label 20 should not defined when macro gets invoked but
	; taken along inside the macro and defined until it gets used there
	; as label.
20	Code2Test1
	B	%BA20

	MACRO
$label	Code2Test2
	MOV	r0, #1
Doh	ROUT
$label
	B %b$label
	MEND

	; Similar to Code2Test1, the Doh ROUT identifier is only checked
	; when 20Doh is going to be used as local label.
20Doh	Code2Test2

	|

	MOV	r7, #7
c2lbl1	MOV	r8, #8
	B	c2lbl1

	MOV	r0, #1
c2lbl2	B	c2lbl2
	]

	END
