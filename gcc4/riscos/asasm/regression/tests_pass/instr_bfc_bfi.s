; Tests BFC/BFI
; RUNOPT: -CPU=7-A

	AREA	Code, CODE

	[ :LNOT: REFERENCE

	; BFC
	ARM
	BFC	r5, #3, #13
	BFCCC	r2, #0, #1
	BFCHI.W	r4, #1, #25
	
	THUMB
	BFC	r5, #3, #13
	BFCCC	r2, #0, #1
	BFCHI.W	r4, #1, #25

	; BFI
	ARM
	BFI	r5, r6, #3, #13
	BFICC	r2, r10, #0, #1
	BFIHI.W	r4, r2, #1, #25
	
	THUMB
	BFI	r5, r6, #3, #13
	BFICC	r2, r10, #0, #1
	BFIHI.W	r4, r2, #1, #25

	|

	ARM
	DCI	&e7cf519f
	DCI	&37c0201f
	DCI	&87d9409f
	
	THUMB
	DCI.W	&f36f05cf
	DCI.W	&f36f0200
	DCI.W	&f36f0459

	ARM
	DCI	&e7cf5196
	DCI	&37c0201a
	DCI	&87d94092

	THUMB
	DCI.W	&f36605cf
	DCI.W	&f36a0200
	DCI.W	&f3620459

	]

	END