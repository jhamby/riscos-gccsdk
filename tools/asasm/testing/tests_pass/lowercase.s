; Test lowercase support for instructions (directives are not supported
; in their lowercase version).
; -RUNOPT: -CPU=3

	AREA	Code, CODE
	[ :LNOT: REFERENCE
	b	{PC}+8
	bl	{PC}+8
	blvs	{PC}+8
	bhi	{PC}+8
	bne	{PC}+8
	beq	{PC}+8
	blo	{PC}+8
	bvs	{PC}+8

	cmp	r1, r2
	adr	r1, {PC}+8
	adrl	r1, {PC}+8000

	; Directives are not supported in their lowercase version.
	MACRO
	align
	DCI	42
	MEND
	align

	; Mixedcase instruction mnemonics are not supported either.
	MACRO
	bL	$where
	BLNV	$where
	MEND
	bL	{PC}+8

	|
	B	{PC}+8
	BL	{PC}+8
	BLVS	{PC}+8
	BHI	{PC}+8
	BNE	{PC}+8
	BEQ	{PC}+8
	BLO	{PC}+8
	BVS	{PC}+8

	CMP	r1, r2
	ADR	r1, {PC}+8
	ADRL	r1, {PC}+8000

	DCI	42
	
	BLNV	{PC}+8
	]
	END
