	AREA Code, CODE

	[ :LNOT: REFERENCE
lbl1	= "t1", 0
	B lbl1

	= "t2", 0
	B lblE

;	= "t3", 0
;lbl3
;	B lbl3		; Unaligned destination

	= "t4", 0
lbl4	B lbl4

lblE
	|

lbl1	DCD      0x3174
	B        lbl1
	DCD      0x3274
	B        lblE
	DCD      0x3474
lbl4	B        lbl4
lblE

	]

	END
