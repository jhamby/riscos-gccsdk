; RUNOPT: -CPU=7-A

		AREA |C$$code|, CODE, READONLY

		; Miscellaneous instructions

	[ :LNOT: REFERENCE
		BKPT	1234

		CLZ	R0,R1

		MRS	R0,APSR
		MRS	R0,CPSR
		MRS	R0,SPSR

		MSR	APSR_nzcvq, #4
		MSR	APSR_nzcvqg, #8
		MSR	APSR_g, #12

		; Legacy syntax:
		MSR	CPSR,#&13	; control & flag
		MSR	CPSR_all,#&13	; control & flag
		MSR	CPSR_ctl,#&13	; control
		MSR	CPSR_flg,#&13	; flg

		MSR	CPSR_c,#&13
		MSR	CPSR_cs,#&13
		MSR	CPSR_csx,#&13
		MSR	CPSR_csxf,#&13

		MSR	CPSR_c,R0
		MSR	CPSR_cs,R0
		MSR	CPSR_csx,R0
		MSR	CPSR_csxf,R0

		; Legacy syntax:
		MSR	SPSR,#&13	; control & flag
		MSR	SPSR_all,#&13	; control & flag
		MSR	SPSR_ctl,#&13	; control
		MSR	SPSR_flg,#&13	; flg

		MSR	SPSR_c,#&13
		MSR	SPSR_cs,#&13
		MSR	SPSR_csx,#&13
		MSR	SPSR_csxf,#&13

		MSR	SPSR_c,R0
		MSR	SPSR_cs,R0
		MSR	SPSR_csx,R0
		MSR	SPSR_csxf,R0

		SWP	R0,R1,[R2]

		SWPB	R0,R1,[R2]
	|
		DCI	&E1204D72

		DCI	&E16F0F11

		DCI	&E10F0000
		DCI	&E10F0000
		DCI	&E14F0000

		MSR	CPSR_f, #4
		MSR	CPSR_fs, #8
		MSR	CPSR_s, #12

		; Legacy syntax:
		DCI	&E329F013
		DCI	&E329F013
		DCI	&E321F013
		DCI	&E328F013

		DCI	&E321F013
		DCI	&E325F013
		DCI	&E327F013
		DCI	&E32FF013

		DCI	&E121F000
		DCI	&E125F000
		DCI	&E127F000
		DCI	&E12FF000

		; Legacy:
		DCI	&E369F013
		DCI	&E369F013
		DCI	&E361F013
		DCI	&E368F013

		DCI	&E361F013
		DCI	&E365F013
		DCI	&E367F013
		DCI	&E36FF013

		DCI	&E161F000
		DCI	&E165F000
		DCI	&E167F000
		DCI	&E16FF000

		DCI	&E1020091

		DCI	&E1420091
	]

		END
