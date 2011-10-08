; ASSERT tests:

	AREA	Code, CODE

	ASSERT {TRUE}

	NOP
lbl1
lbl2
	ASSERT lbl1=lbl2

	NOP
	ASSERT lbl3=lbl4
lbl3
lbl4

	END
