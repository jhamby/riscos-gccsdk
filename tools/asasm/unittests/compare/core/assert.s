; ASSERT tests:

	AREA	Code1, CODE

	ASSERT {TRUE}

	NOP
lbl1
lbl2
	ASSERT lbl1=lbl2

	NOP
	ASSERT lbl3=lbl4
lbl3
lbl4

	; Test if we can handle local lables in our assert expressions.
	AREA	Code2, CODE

	ASSERT %FT02 = . + 4
	B	.
02
	B	.
	ASSERT %BT02 = . - 4

	AREA	Code3, CODE
	ORG	&8000

	ASSERT %FT03 = . + 4
	B	.
03
	B	.
	ASSERT %BT03 = . - 4

	END