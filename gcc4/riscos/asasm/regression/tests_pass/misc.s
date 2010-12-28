		AREA |C$$code|, CODE, READONLY

		; Miscellaneous instructions

	[ :LNOT: REFERENCE
		BKPT	1234

		CLZ	R0,R1

		MRS	R0,CPSR
		MRS	R0,SPSR

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
		DCD	&E1204D72

		DCD	&E16F0F11

		DCD	&E10F0000
		DCD	&E14F0000

		; Legacy syntax:
		DCD	&E329F013
		DCD	&E329F013
		DCD	&E321F013
		DCD	&E328F013

		DCD	&E321F013
		DCD	&E325F013
		DCD	&E327F013
		DCD	&E32FF013

		DCD	&E121F000
		DCD	&E125F000
		DCD	&E127F000
		DCD	&E12FF000

		; Legacy:
		DCD	&E369F013
		DCD	&E369F013
		DCD	&E361F013
		DCD	&E368F013

		DCD	&E361F013
		DCD	&E365F013
		DCD	&E367F013
		DCD	&E36FF013

		DCD	&E161F000
		DCD	&E165F000
		DCD	&E167F000
		DCD	&E16FF000

		DCD	&E1020091

		DCD	&E1420091
	]

		END
