; Tests BFC/BFI/SBFX/USBFX
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

	; SBFX
	ARM
	SBFX	r5, r6, #3, #13
	SBFXCC	r2, r10, #0, #1
	SBFXHI.W	r4, r2, #1, #25
	
	THUMB
	SBFX	r5, r6, #3, #13
	SBFXCC	r2, r10, #0, #1
	SBFXHI.W	r4, r2, #1, #25

	; UBFX
	ARM
	UBFX	r5, r6, #3, #13
	UBFXCC	r2, r10, #0, #1
	UBFXHI.W	r4, r2, #1, #25
	
	THUMB
	UBFX	r5, r6, #3, #13
	UBFXCC	r2, r10, #0, #1
	UBFXHI.W	r4, r2, #1, #25

	|

	; BFC
	ARM
	DCI	&e7cf519f
	DCI	&37c0201f
	DCI	&87d9409f

	THUMB
	DCI.W	&f36f05cf
	DCI.W	&f36f0200 ; FIXME: IT support missing
	DCI.W	&f36f0459 ; FIXME: IT support missing

	; BFI
	ARM
	DCI	&e7cf5196
	DCI	&37c0201a
	DCI	&87d94092

	THUMB
	DCI.W	&f36605cf
	DCI.W	&f36a0200 ; FIXME: IT support missing
	DCI.W	&f3620459 ; FIXME: IT support missing

	; SBFX
	ARM
	DCI	&e7ac51d6
	DCI	&37a0205a
	DCI	&87b840d2

	THUMB
	DCI.W	&f34605cc
	DCI.W	&f34a0200 ; FIXME: IT support missing
	DCI.W	&f3420458 ; FIXME: IT support missing

	; UBFX
	ARM
	DCI	&e7ec51d6
	DCI	&37e0205a
	DCI	&87f840d2

	THUMB
	DCI.W	&f3c605cc
	DCI.W	&f3ca0200 ; FIXME: IT support missing
	DCI.W	&f3c20458 ; FIXME: IT support missing

	]

	END