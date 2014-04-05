; Tests for ^, #, MAP, FIELD using register as base.

	; Test 1:
	AREA	Code1, CODE, READONLY

		^	0, r12
foo		#	4
bar		FIELD	4
sizeof_map	*	@

	[ :LNOT: REFERENCE
		LDR	r0, bar
		MOV	r1, #sizeof_map
		MOV	r1, #@
		LDR	r2, baz
		ADR	r3, bar
		ADRL	r4, bar		; Will detect ADR is sufficent.
	|
		LDR	r0,[r12,#4]
		MOV	r1,#8
		MOV	r1,#8
		LDR	r2,[r9]
		ADD	r3,r12,#4
		ADD	r4,r12,#4
	]

		MAP	0, r9
baz		#	4

	; Test 2:
	AREA	Data2a, DATA, NOINIT, BASED r3
		%	20
tst2_a		%	32
		^	tst2_a
field0		#	4
field1		#	8
field2		#	16

	AREA	Code2, CODE, READONLY
	[ :LNOT: REFERENCE
		LDR	r1, field1
		LDR	r2, field4

		LDR	r1, field1
		LDR	r2, field4
	|
		LDR     r1, Data2a + 24
		LDR     r2, Data2b + 24

	  IF OUTPUTTYPE == "AOF"
		LDR     r1, Data2a + 24
		LDR     r2, Data2b + 24
	  ELIF OUTPUTTYPE == "ELF"
		LDR	r1, [r3, #20 + 4]
		RELOC	R_ARM_LDR_SB_G0, Data2a
		LDR	r2, [r3, #20 + 4]
		RELOC	R_ARM_LDR_SB_G0, Data2b
	  ELSE
		Unsupported output type
	  ENDIF
	]

	AREA	Data2b, DATA, NOINIT, BASED r3
		%	20
tst2_b		%	32
		^	tst2_b
field3		#	4
field4		#	8
field5		#	16

	END
