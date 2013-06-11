; Tests MSR/MRS
; RUNOPT: -CPU=Cortex-A15

	; MRS tests:
		AREA Code1, CODE
	[ :LNOT: REFERENCE
		ARM
		MRS	R9,APSR
		MRS	R10,CPSR
		MRS	R11,SPSR

		THUMB
		MRS	R9,APSR
		MRS	R10,CPSR
		MRS	R11,SPSR
	|
		ARM
		DCI	&E10F9000
		DCI	&E10Fa000
		DCI	&E14Fb000

		THUMB
		DCI.W &f3ef8900	; mrs	r9, CPSR
		DCI.W &f3ef8a00	; mrs	sl, CPSR
		DCI.W &f3ff8b00	; mrs	fp, SPSR
	]

		AREA Code2, CODE
	[ :LNOT: REFERENCE
		MACRO
		MRSBankedTest
		MRS	R0, R8_usr
		MRS	R1, R9_usr
		MRS	R2, R10_usr
		MRS	R3, R11_usr
		MRS	R4, R12_usr
		MRS	R5, SP_usr
		MRS	R6, LR_usr
		MRS	R7, R8_fiq
		MRS	R8, R9_fiq
		MRS	R9, R10_fiq
		MRS	R10, R11_fiq
		MRS	R11, R12_fiq
		MRS	R12, SP_fiq
		MRS	R14, LR_fiq
		MRS	R0, LR_irq
		MRS	R1, SP_irq
		MRS	R2, LR_svc
		MRS	R3, SP_svc
		MRS	R4, LR_abt
		MRS	R5, SP_abt
		MRS	R6, LR_und
		MRS	R7, SP_und
		MRS	R8, LR_mon
		MRS	R9, SP_mon
		MRS	R10, ELR_hyp
		MRS	R11, SP_hyp
		MRS	R12, SPSR_fiq
		MRS	R14, SPSR_irq
		MRS	R0, SPSR_svc
		MRS	R1, SPSR_abt
		MRS	R2, SPSR_und
		MRS	R3, SPSR_mon
		MRS	R4, SPSR_hyp
		MEND

		ARM
		MRSBankedTest
		THUMB
		MRSBankedTest
	|
		ARM
		DCI &e1000200	; mrs	r0, R8_usr
		DCI &e1011200	; mrs	r1, R9_usr
		DCI &e1022200	; mrs	r2, R10_usr
		DCI &e1033200	; mrs	r3, R11_usr
		DCI &e1044200	; mrs	r4, R12_usr
		DCI &e1055200	; mrs	r5, SP_usr
		DCI &e1066200	; mrs	r6, LR_usr
		DCI &e1087200	; mrs	r7, R8_fiq
		DCI &e1098200	; mrs	r8, R9_fiq
		DCI &e10a9200	; mrs	r9, R10_fiq
		DCI &e10ba200	; mrs	sl, R11_fiq
		DCI &e10cb200	; mrs	fp, R12_fiq
		DCI &e10dc200	; mrs	ip, SP_fiq
		DCI &e10ee200	; mrs	lr, LR_fiq
		DCI &e1000300	; mrs	r0, LR_irq
		DCI &e1011300	; mrs	r1, SP_irq
		DCI &e1022300	; mrs	r2, LR_svc
		DCI &e1033300	; mrs	r3, SP_svc
		DCI &e1044300	; mrs	r4, LR_abt
		DCI &e1055300	; mrs	r5, SP_abt
		DCI &e1066300	; mrs	r6, LR_und
		DCI &e1077300	; mrs	r7, SP_und
		DCI &e10c8300	; mrs	r8, LR_mon
		DCI &e10d9300	; mrs	r9, SP_mon
		DCI &e10ea300	; mrs	sl, ELR_hyp
		DCI &e10fb300	; mrs	fp, SP_hyp
		DCI &e14ec200	; mrs	ip, SPSR_fiq
		DCI &e140e300	; mrs	lr, SPSR_irq
		DCI &e1420300	; mrs	r0, SPSR_svc
		DCI &e1441300	; mrs	r1, SPSR_abt
		DCI &e1462300	; mrs	r2, SPSR_und
		DCI &e14c3300	; mrs	r3, SPSR_mon
		DCI &e14e4300	; mrs	r4, SPSR_hyp

		THUMB
		DCI.W &f3e08020	; mrs	r0, R8_usr
		DCI.W &f3e18120	; mrs	r1, R9_usr
		DCI.W &f3e28220	; mrs	r2, R10_usr
		DCI.W &f3e38320	; mrs	r3, R11_usr
		DCI.W &f3e48420	; mrs	r4, R12_usr
		DCI.W &f3e58520	; mrs	r5, SP_usr
		DCI.W &f3e68620	; mrs	r6, LR_usr
		DCI.W &f3e88720	; mrs	r7, R8_fiq
		DCI.W &f3e98820	; mrs	r8, R9_fiq
		DCI.W &f3ea8920	; mrs	r9, R10_fiq
		DCI.W &f3eb8a20	; mrs	sl, R11_fiq
		DCI.W &f3ec8b20	; mrs	fp, R12_fiq
		DCI.W &f3ed8c20	; mrs	ip, SP_fiq
		DCI.W &f3ee8e20	; mrs	lr, LR_fiq
		DCI.W &f3e08030	; mrs	r0, LR_irq
		DCI.W &f3e18130	; mrs	r1, SP_irq
		DCI.W &f3e28230	; mrs	r2, LR_svc
		DCI.W &f3e38330	; mrs	r3, SP_svc
		DCI.W &f3e48430	; mrs	r4, LR_abt
		DCI.W &f3e58530	; mrs	r5, SP_abt
		DCI.W &f3e68630	; mrs	r6, LR_und
		DCI.W &f3e78730	; mrs	r7, SP_und
		DCI.W &f3ec8830	; mrs	r8, LR_mon
		DCI.W &f3ed8930	; mrs	r9, SP_mon
		DCI.W &f3ee8a30	; mrs	sl, ELR_hyp
		DCI.W &f3ef8b30	; mrs	fp, SP_hyp
		DCI.W &f3fe8c20	; mrs	ip, SPSR_fiq
		DCI.W &f3f08e30	; mrs	lr, SPSR_irq
		DCI.W &f3f28030	; mrs	r0, SPSR_svc
		DCI.W &f3f48130	; mrs	r1, SPSR_abt
		DCI.W &f3f68230	; mrs	r2, SPSR_und
		DCI.W &f3fc8330	; mrs	r3, SPSR_mon
		DCI.W &f3fe8430	; mrs	r4, SPSR_hyp
	]

	; MSR tests:
		AREA Code3, CODE
	[ :LNOT: REFERENCE
		ARM
		MSR	APSR_nzcvq, #4
		MSR	APSR_nzcvqg, #8
		MSR	APSR_g, #12

		MSR	CPSR_c,#&13
		MSR	CPSR_cs,#&13
		MSR	CPSR_csx,#&13
		MSR	CPSR_csxf,#&13

		MSR	SPSR_c,#&13
		MSR	SPSR_cs,#&13
		MSR	SPSR_csx,#&13
		MSR	SPSR_csxf,#&13

		MSR	APSR_nzcvq, R9
		MSR	APSR_nzcvqg , R10
		MSR	APSR_g   , R11

		MSR	CPSR_c,R9
		MSR	CPSR_cs,R10
		MSR	CPSR_csx,R11
		MSR	CPSR_csxf,R9

		MSR	SPSR_c,R9
		MSR	SPSR_cs,R10
		MSR	SPSR_csx,R11
		MSR	SPSR_csxf,R9

		; Legacy syntax:
		MSR	CPSR , #&13	; control & flag
		MSR	CPSR_all,#&13	; control & flag
		MSR	CPSR_ctl,#&13	; control
		MSR	CPSR_flg,#&13	; flg

		; Legacy syntax:
		MSR	SPSR,#&13	; control & flag
		MSR	SPSR_all,#&13	; control & flag
		MSR	SPSR_ctl,#&13	; control
		MSR	SPSR_flg,#&13	; flg

		THUMB
		MSR	APSR_nzcvq, R9
		MSR	APSR_nzcvqg , R10
		MSR	APSR_g   , R11

		MSR	CPSR_c,R9
		MSR	CPSR_cs,R10
		MSR	CPSR_csx,R11
		MSR	CPSR_csxf,R9

		MSR	SPSR_c,R9
		MSR	SPSR_cs,R10
		MSR	SPSR_csx,R11
		MSR	SPSR_csxf,R9
	|
		ARM
		DCI &e328f004	; msr	CPSR_f, #4
		DCI &e32cf008	; msr	CPSR_fs, #8
		DCI &e324f00c	; msr	CPSR_s, #12

		DCI &e321f013	; msr	CPSR_c, #19
		DCI &e325f013	; msr	CPSR_sc, #19
		DCI &e327f013	; msr	CPSR_sxc, #19
		DCI &e32ff013	; msr	CPSR_fsxc, #19

		DCI &e361f013	; msr	SPSR_c, #19
		DCI &e365f013	; msr	SPSR_sc, #19
		DCI &e367f013	; msr	SPSR_sxc, #19
		DCI &e36ff013	; msr	SPSR_fsxc, #19

		DCI &e128f009	; msr	CPSR_f, r9
		DCI &e12cf00a	; msr	CPSR_fs, sl
		DCI &e124f00b	; msr	CPSR_s, fp

		DCI &e121f009	; msr	CPSR_c, r9
		DCI &e125f00a	; msr	CPSR_sc, sl
		DCI &e127f00b	; msr	CPSR_sxc, fp
		DCI &e12ff009	; msr	CPSR_fsxc, r9

		DCI &e161f009	; msr	SPSR_c, r9
		DCI &e165f00a	; msr	SPSR_sc, sl
		DCI &e167f00b	; msr	SPSR_sxc, fp
		DCI &e16ff009	; msr	SPSR_fsxc, r9

		; Legacy syntax:
		DCI &e329f013	; msr	CPSR_fc, #19
		DCI &e329f013	; msr	CPSR_fc, #19
		DCI &e321f013	; msr	CPSR_c, #19
		DCI &e328f013	; msr	CPSR_f, #19

		; Legacy syntax:
		DCI &e369f013	; msr	SPSR_fc, #19
		DCI &e369f013	; msr	SPSR_fc, #19
		DCI &e361f013	; msr	SPSR_c, #19
		DCI &e368f013	; msr	SPSR_f, #19

		THUMB
		DCI.W &f3898800	; msr	CPSR_f, r9
		DCI.W &f38a8c00	; msr	CPSR_fs, sl
		DCI.W &f38b8400	; msr	CPSR_s, fp

		DCI.W &f3898100	; msr	CPSR_c, r9
		DCI.W &f38a8500	; msr	CPSR_sc, sl
		DCI.W &f38b8700	; msr	CPSR_sxc, fp
		DCI.W &f3898f00	; msr	CPSR_fsxc, r9

		DCI.W &f3998100	; msr	SPSR_c, r9
		DCI.W &f39a8500	; msr	SPSR_sc, sl
		DCI.W &f39b8700	; msr	SPSR_sxc, fp
		DCI.W &f3998f00	; msr	SPSR_fsxc, r9
	]

		AREA Code4, CODE
	[ :LNOT: REFERENCE
		MACRO
		MSRBankedTest
		MSR	R8_usr, R0
		MSR	R9_usr, R1
		MSR	R10_usr, R2
		MSR	R11_usr, R3
		MSR	R12_usr, R4
		MSR	SP_usr, R5
		MSR	LR_usr, R6
		MSR	R8_fiq, R7
		MSR	R9_fiq, R8
		MSR	R10_fiq, R9
		MSR	R11_fiq, R10
		MSR	R12_fiq, R11
		MSR	SP_fiq, R12
		MSR	LR_fiq, R14
		MSR	LR_irq, R0
		MSR	SP_irq, R1
		MSR	LR_svc, R2
		MSR	SP_svc, R3
		MSR	LR_abt, R4
		MSR	SP_abt, R5
		MSR	LR_und, R6
		MSR	SP_und, R7
		MSR	LR_mon, R8
		MSR	SP_mon, R9
		MSR	ELR_hyp, R10
		MSR	SP_hyp, R11
		MSR	SPSR_fiq, R12
		MSR	SPSR_irq, R14
		MSR	SPSR_svc, R0
		MSR	SPSR_abt, R1
		MSR	SPSR_und, R2
		MSR	SPSR_mon, R3
		MSR	SPSR_hyp, R4
		MEND

		ARM
		MSRBankedTest
		THUMB
		MSRBankedTest
	|
		ARM
		DCI &e120f200	; msr	R8_usr, r0
		DCI &e121f201	; msr	R9_usr, r1
		DCI &e122f202	; msr	R10_usr, r2
		DCI &e123f203	; msr	R11_usr, r3
		DCI &e124f204	; msr	R12_usr, r4
		DCI &e125f205	; msr	SP_usr, r5
		DCI &e126f206	; msr	LR_usr, r6
		DCI &e128f207	; msr	R8_fiq, r7
		DCI &e129f208	; msr	R9_fiq, r8
		DCI &e12af209	; msr	R10_fiq, r9
		DCI &e12bf20a	; msr	R11_fiq, sl
		DCI &e12cf20b	; msr	R12_fiq, fp
		DCI &e12df20c	; msr	SP_fiq, ip
		DCI &e12ef20e	; msr	LR_fiq, lr
		DCI &e120f300	; msr	LR_irq, r0
		DCI &e121f301	; msr	SP_irq, r1
		DCI &e122f302	; msr	LR_svc, r2
		DCI &e123f303	; msr	SP_svc, r3
		DCI &e124f304	; msr	LR_abt, r4
		DCI &e125f305	; msr	SP_abt, r5
		DCI &e126f306	; msr	LR_und, r6
		DCI &e127f307	; msr	SP_und, r7
		DCI &e12cf308	; msr	LR_mon, r8
		DCI &e12df309	; msr	SP_mon, r9
		DCI &e12ef30a	; msr	ELR_hyp, sl
		DCI &e12ff30b	; msr	SP_hyp, fp
		DCI &e16ef20c	; msr	SPSR_fiq, ip
		DCI &e160f30e	; msr	SPSR_irq, lr
		DCI &e162f300	; msr	SPSR_svc, r0
		DCI &e164f301	; msr	SPSR_abt, r1
		DCI &e166f302	; msr	SPSR_und, r2
		DCI &e16cf303	; msr	SPSR_mon, r3
		DCI &e16ef304	; msr	SPSR_hyp, r4

		THUMB
		DCI.W &f3808020	; msr	R8_usr, r0
		DCI.W &f3818120	; msr	R9_usr, r1
		DCI.W &f3828220	; msr	R10_usr, r2
		DCI.W &f3838320	; msr	R11_usr, r3
		DCI.W &f3848420	; msr	R12_usr, r4
		DCI.W &f3858520	; msr	SP_usr, r5
		DCI.W &f3868620	; msr	LR_usr, r6
		DCI.W &f3878820	; msr	R8_fiq, r7
		DCI.W &f3888920	; msr	R9_fiq, r8
		DCI.W &f3898a20	; msr	R10_fiq, r9
		DCI.W &f38a8b20	; msr	R11_fiq, sl
		DCI.W &f38b8c20	; msr	R12_fiq, fp
		DCI.W &f38c8d20	; msr	SP_fiq, ip
		DCI.W &f38e8e20	; msr	LR_fiq, lr
		DCI.W &f3808030	; msr	LR_irq, r0
		DCI.W &f3818130	; msr	SP_irq, r1
		DCI.W &f3828230	; msr	LR_svc, r2
		DCI.W &f3838330	; msr	SP_svc, r3
		DCI.W &f3848430	; msr	LR_abt, r4
		DCI.W &f3858530	; msr	SP_abt, r5
		DCI.W &f3868630	; msr	LR_und, r6
		DCI.W &f3878730	; msr	SP_und, r7
		DCI.W &f3888c30	; msr	LR_mon, r8
		DCI.W &f3898d30	; msr	SP_mon, r9
		DCI.W &f38a8e30	; msr	ELR_hyp, sl
		DCI.W &f38b8f30	; msr	SP_hyp, fp
		DCI.W &f39c8e20	; msr	SPSR_fiq, ip
		DCI.W &f39e8030	; msr	SPSR_irq, lr
		DCI.W &f3908230	; msr	SPSR_svc, r0
		DCI.W &f3918430	; msr	SPSR_abt, r1
		DCI.W &f3928630	; msr	SPSR_und, r2
		DCI.W &f3938c30	; msr	SPSR_mon, r3
		DCI.W &f3948e30	; msr	SPSR_hyp, r4
	]

		END
