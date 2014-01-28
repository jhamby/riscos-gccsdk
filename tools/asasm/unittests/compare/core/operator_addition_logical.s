	[ :LNOT: :DEF:EXTENSION
	GBLL	EXTENSION
EXTENSION SETL	{FALSE}
	]

	AREA Code, CODE, READONLY

	^ 0, r12
field1	# 16		; Offset 0 - &10
field2	# 8		; Offset &10 - &18
field3	# 4		; Offset &18 - &1c

	[ :LNOT:REFERENCE

	; +
	DCD	5 + 3
	DCD	5 + -3
	DCD	-5 + 3
	DCD	-5 + -3
	[ EXTENSION
	DCFS	5. + 3.		; <float> + <float>
	DCFS	5. + -3.	; <float> + <float>
	DCFS	-5. + 3.	; <float> + <float>
	DCFS	-5. + -3.	; <float> + <float>
	DCFS	5 + 3.		; <signed int> + <float>
	DCFS	5. + -3		; <float> + <signed int>
	]
	DCD	:INDEX:@
	DCD	:INDEX:@ + 4
	DCD	4 + :INDEX:@
	DCD	:INDEX:@ + :INDEX:@
	DCD	"A" + 3
	DCD	3 + "A"
	LDR	r1, field2 + 4
	LDR	r2, 4 + field2

	; -
	DCD	5 - 3
	DCD	5 - -3
	DCD	-5 - 3
	DCD	-5 - -3
	DCD	:INDEX:@
	DCD	:INDEX:@ - 4
	DCD	4 - :INDEX:@
	DCD	:INDEX:@ - :INDEX:@
	[ EXTENSION
	DCFS	5. - 3.		; <float> + <float>
	DCFS	5. - -3.	; <float> + <float>
	DCFS	-5. - 3.	; <float> + <float>
	DCFS	-5. - -3.	; <float> + <float>
	DCFS	5 - 3.		; <signed int> + <float>
	DCFS	5. - -3		; <float> + <signed int>
	]
	DCD	"D" - 3
	DCD	"D" - "A"
	LDR	r1, field2 - 4
	DCD	field3 - field2

	; :AND:
	DCD	6 :AND: 3
	DCD	:INDEX:@ :AND: 12
	DCD	12 :AND: :INDEX:@
	DCD	:INDEX:@ :AND: :INDEX:@
	DCD	"A" :AND: 3
	DCD	3 :AND: "A"

	; &
	[ EXTENSION
	DCD	6 & 3
	DCD	:INDEX:@ & 12
	DCD	12 & :INDEX:@
	DCD	:INDEX:@ & :INDEX:@
	DCD	"A" :AND: 3
	DCD	3 :AND: "A"
	]

	; :OR:
	DCD	6 :OR: 3
	DCD	:INDEX:@ :OR: 12
	DCD	12 :OR: :INDEX:@
	DCD	:INDEX:@ :OR: :INDEX:@
	DCD	"A" :OR: 3
	DCD	3 :OR: "A"

	; |
	[ EXTENSION
	DCD	6 | 3
	DCD	:INDEX:@ | 12
	DCD	12 | :INDEX:@
	DCD	:INDEX:@ | :INDEX:@
	DCD	"A" :OR: 3
	DCD	3 :OR: "A"
	]

	; :EOR:
	DCD	6 :EOR: 3
	DCD	"A" :EOR: 3
	DCD	3 :EOR: "A"

	|

	; +
	DCD	8
	DCD	2
	DCD	-2
	DCD	-8
	[ EXTENSION
	DCFS	8.
	DCFS	2.
	DCFS	-2.
	DCFS	-8.
	DCFS	8.
	DCFS	2.
	]
	DCD	&1C
	DCD	&20
	DCD	&20
	DCD	&38
	DCD	&44
	DCD	&44
	LDR	r1, [r12, #20]
	LDR	r2, [r12, #20]

	; -
	DCD	2
	DCD	8
	DCD	-8
	DCD	-2
	DCD	&1C
	DCD	24
	DCD	-24
	DCD	0
	[ EXTENSION
	DCFS	2.
	DCFS	8.
	DCFS	-8.
	DCFS	-2.
	DCFS	2.
	DCFS	8.
	]
	DCD	&41
	DCD	3
	LDR	r1, [r12, #12]
	DCD	8

	; :AND:
	DCD	2
	DCD	&C
	DCD	&C
	DCD	&1c
	DCD	1
	DCD	1

	; &
	[ EXTENSION
	DCD	2
	DCD	&C
	DCD	&C
	DCD	&1c
	DCD	1
	DCD	1
	]

	; :OR:
	DCD	7
	DCD	&1c
	DCD	&1c
	DCD	&1c
	DCD	&43
	DCD	&43

	; |
	[ EXTENSION
	DCD	7
	DCD	&1c
	DCD	&1c
	DCD	&1c
	DCD	&43
	DCD	&43
	]

	; :EOR:
	DCD	5
	DCD	&42
	DCD	&42

	]

	END
