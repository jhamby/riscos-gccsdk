;  This header file for GETting from assembler source defines register names & other bits


;  Use the RN directive to define ARM register names

r0		RN	0
r1		RN	1
r2		RN	2
r3		RN	3
r4		RN	4
r5		RN	5
r6		RN	6
r7		RN	7
r8		RN	8
r9		RN	9
r10		RN	10
r11		RN	11
r12		RN	12
r13		RN	13
r14		RN	14
r15		RN	15

;  Define APCS standard register names. Note: This is APCS_R

a1		RN	r0	; Note: register names can be defined from each other
a2		RN	r1
a3		RN	r2
a4		RN	r3
v1		RN	r4
v2		RN	r5
v3		RN	r6
v4		RN	r7
v5		RN	r8
v6		RN	r9
sl		RN	r10
fp		RN	r11
ip		RN	r12
sp		RN	r13
lr		RN	r14
pc		RN	r15


;  Use the FN directive to define floating point register names

f0		FN	0
f1		FN	1
f2		FN	2
f3		FN	3
f4		FN	4
f5		FN	5
f6		FN	6
f7		FN	7


;  Use the CN directive to define co-processor register names

c0		CN	0
c1		CN	1
c2		CN	2
c3		CN	3
c4		CN	4
c5		CN	5
c6		CN	6
c7		CN	7
c8		CN	8
c9		CN	9
c10		CN	10
c11		CN	11
c12		CN	12
c13		CN	13
c14		CN	14
c15		CN	15


;  Use the CP directive to define co-processor names

;cp0		CP	0
;cp1		CP	1
;cp2		CP	2
;cp3		CP	3
;cp4		CP	4
;cp5		CP	5
;cp6		CP	6
;cp7		CP	7
;cp8		CP	8
;cp9		CP	9
;cp10		CP	10
;cp11		CP	11
;cp12		CP	12
;;cp13		CP	13
;cp14		CP	14
;cp15		CP	15


;  Status register masks

Fbit		EQU	1:SHL:26
Ibit		EQU	1:SHL:27
Vbit		EQU	1:SHL:28
Cbit		EQU	1:SHL:29
Zbit		EQU	1:SHL:30
Nbit		EQU	1:SHL:31


;  Processor modes

Usermode	EQU	2_00
FIQmode		EQU	2_01
IRQmode		EQU	2_10
SVCmode		EQU	2_11

		END
