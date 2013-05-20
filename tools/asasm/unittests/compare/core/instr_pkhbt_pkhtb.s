; Tests PKHBT/PKHTB.
; RUNOPT: -CPU=6T2

	AREA Code, CODE

	[ :LNOT: REFERENCE

	MACRO
	Inject $base, $cc, $q, $shift
	;INFO 0, "\t$base.$cc.$q R1, R2, R3 $shift"
	$base.$cc.$q R1, R2, R3 $shift
	MEND

	MACRO
	ExpandCC $base, $q, $shift
	; No CC:
	Inject $base,, $q, "$shift"
	; CC combinations:
	LCLA	cnt
	LCLS	cc
cnt	SETA	0
	WHILE	cnt < 17
	; 17 condition codes to be tested:
	;   - Condition code NV is left out from this test.
	;   - HS and LO are equivalents for CS and CC.
cc	SETS	("EQNECSCCMIPLVSVCHILSGELTGTLEALHSLO" :RIGHT: (34 - 2*cnt)) :LEFT: 2
	Inject $base, $cc, $q, "$shift"
cnt	SETA	cnt + 1
	WEND
	MEND

	MACRO
	Invoke $q
	ExpandCC PKHBT, $q
	ExpandCC PKHBT, $q, ", LSL#0"
	ExpandCC PKHBT, $q, ", LSL#1"
	ExpandCC PKHBT, $q, ", LSL#31"

	ExpandCC PKHTB, $q
	ExpandCC PKHTB, $q, ", ASR#1"
	ExpandCC PKHTB, $q, ", ASR#31"
	ExpandCC PKHTB, $q, ", ASR#32"
	MEND

	ARM
	;INFO 0, "\t.arm"
	Invoke
	Invoke .W

	THUMB
	;INFO 0, "\t.thumb"
	Invoke
	Invoke .W

	|

	ARM
	; No qualifier:
	DCI &e6821013		; pkhbt		r1, r2, r3
	DCI &06821013		; pkhbteq	r1, r2, r3
	DCI &16821013		; pkhbtne	r1, r2, r3
	DCI &26821013		; pkhbtcs	r1, r2, r3
	DCI &36821013		; pkhbtcc	r1, r2, r3
	DCI &46821013		; pkhbtmi	r1, r2, r3
	DCI &56821013		; pkhbtpl	r1, r2, r3
	DCI &66821013		; pkhbtvs	r1, r2, r3
	DCI &76821013		; pkhbtvc	r1, r2, r3
	DCI &86821013		; pkhbthi	r1, r2, r3
	DCI &96821013		; pkhbtls	r1, r2, r3
	DCI &a6821013		; pkhbtge	r1, r2, r3
	DCI &b6821013		; pkhbtlt	r1, r2, r3
	DCI &c6821013		; pkhbtgt	r1, r2, r3
	DCI &d6821013		; pkhbtle	r1, r2, r3
	DCI &e6821013		; pkhbt		r1, r2, r3
	DCI &26821013		; pkhbtcs	r1, r2, r3
	DCI &36821013		; pkhbtcc	r1, r2, r3
	DCI &e6821013		; pkhbt		r1, r2, r3
	DCI &06821013		; pkhbteq	r1, r2, r3
	DCI &16821013		; pkhbtne	r1, r2, r3
	DCI &26821013		; pkhbtcs	r1, r2, r3
	DCI &36821013		; pkhbtcc	r1, r2, r3
	DCI &46821013		; pkhbtmi	r1, r2, r3
	DCI &56821013		; pkhbtpl	r1, r2, r3
	DCI &66821013		; pkhbtvs	r1, r2, r3
	DCI &76821013		; pkhbtvc	r1, r2, r3
	DCI &86821013		; pkhbthi	r1, r2, r3
	DCI &96821013		; pkhbtls	r1, r2, r3
	DCI &a6821013		; pkhbtge	r1, r2, r3
	DCI &b6821013		; pkhbtlt	r1, r2, r3
	DCI &c6821013		; pkhbtgt	r1, r2, r3
	DCI &d6821013		; pkhbtle	r1, r2, r3
	DCI &e6821013		; pkhbt		r1, r2, r3
	DCI &26821013		; pkhbtcs	r1, r2, r3
	DCI &36821013		; pkhbtcc	r1, r2, r3
	DCI &e6821093		; pkhbt		r1, r2, r3, lsl #1
	DCI &06821093		; pkhbteq	r1, r2, r3, lsl #1
	DCI &16821093		; pkhbtne	r1, r2, r3, lsl #1
	DCI &26821093		; pkhbtcs	r1, r2, r3, lsl #1
	DCI &36821093		; pkhbtcc	r1, r2, r3, lsl #1
	DCI &46821093		; pkhbtmi	r1, r2, r3, lsl #1
	DCI &56821093		; pkhbtpl	r1, r2, r3, lsl #1
	DCI &66821093		; pkhbtvs	r1, r2, r3, lsl #1
	DCI &76821093		; pkhbtvc	r1, r2, r3, lsl #1
	DCI &86821093		; pkhbthi	r1, r2, r3, lsl #1
	DCI &96821093		; pkhbtls	r1, r2, r3, lsl #1
	DCI &a6821093		; pkhbtge	r1, r2, r3, lsl #1
	DCI &b6821093		; pkhbtlt	r1, r2, r3, lsl #1
	DCI &c6821093		; pkhbtgt	r1, r2, r3, lsl #1
	DCI &d6821093		; pkhbtle	r1, r2, r3, lsl #1
	DCI &e6821093		; pkhbt		r1, r2, r3, lsl #1
	DCI &26821093		; pkhbtcs	r1, r2, r3, lsl #1
	DCI &36821093		; pkhbtcc	r1, r2, r3, lsl #1
	DCI &e6821f93		; pkhbt		r1, r2, r3, lsl #31
	DCI &06821f93		; pkhbteq	r1, r2, r3, lsl #31
	DCI &16821f93		; pkhbtne	r1, r2, r3, lsl #31
	DCI &26821f93		; pkhbtcs	r1, r2, r3, lsl #31
	DCI &36821f93		; pkhbtcc	r1, r2, r3, lsl #31
	DCI &46821f93		; pkhbtmi	r1, r2, r3, lsl #31
	DCI &56821f93		; pkhbtpl	r1, r2, r3, lsl #31
	DCI &66821f93		; pkhbtvs	r1, r2, r3, lsl #31
	DCI &76821f93		; pkhbtvc	r1, r2, r3, lsl #31
	DCI &86821f93		; pkhbthi	r1, r2, r3, lsl #31
	DCI &96821f93		; pkhbtls	r1, r2, r3, lsl #31
	DCI &a6821f93		; pkhbtge	r1, r2, r3, lsl #31
	DCI &b6821f93		; pkhbtlt	r1, r2, r3, lsl #31
	DCI &c6821f93		; pkhbtgt	r1, r2, r3, lsl #31
	DCI &d6821f93		; pkhbtle	r1, r2, r3, lsl #31
	DCI &e6821f93		; pkhbt		r1, r2, r3, lsl #31
	DCI &26821f93		; pkhbtcs	r1, r2, r3, lsl #31
	DCI &36821f93		; pkhbtcc	r1, r2, r3, lsl #31
	DCI &e6831012		; pkhbt		r1, r3, r2
	DCI &06831012		; pkhbteq	r1, r3, r2
	DCI &16831012		; pkhbtne	r1, r3, r2
	DCI &26831012		; pkhbtcs	r1, r3, r2
	DCI &36831012		; pkhbtcc	r1, r3, r2
	DCI &46831012		; pkhbtmi	r1, r3, r2
	DCI &56831012		; pkhbtpl	r1, r3, r2
	DCI &66831012		; pkhbtvs	r1, r3, r2
	DCI &76831012		; pkhbtvc	r1, r3, r2
	DCI &86831012		; pkhbthi	r1, r3, r2
	DCI &96831012		; pkhbtls	r1, r3, r2
	DCI &a6831012		; pkhbtge	r1, r3, r2
	DCI &b6831012		; pkhbtlt	r1, r3, r2
	DCI &c6831012		; pkhbtgt	r1, r3, r2
	DCI &d6831012		; pkhbtle	r1, r3, r2
	DCI &e6831012		; pkhbt		r1, r3, r2
	DCI &26831012		; pkhbtcs	r1, r3, r2
	DCI &36831012		; pkhbtcc	r1, r3, r2
	DCI &e68210d3		; pkhtb		r1, r2, r3, asr #1
	DCI &068210d3		; pkhtbeq	r1, r2, r3, asr #1
	DCI &168210d3		; pkhtbne	r1, r2, r3, asr #1
	DCI &268210d3		; pkhtbcs	r1, r2, r3, asr #1
	DCI &368210d3		; pkhtbcc	r1, r2, r3, asr #1
	DCI &468210d3		; pkhtbmi	r1, r2, r3, asr #1
	DCI &568210d3		; pkhtbpl	r1, r2, r3, asr #1
	DCI &668210d3		; pkhtbvs	r1, r2, r3, asr #1
	DCI &768210d3		; pkhtbvc	r1, r2, r3, asr #1
	DCI &868210d3		; pkhtbhi	r1, r2, r3, asr #1
	DCI &968210d3		; pkhtbls	r1, r2, r3, asr #1
	DCI &a68210d3		; pkhtbge	r1, r2, r3, asr #1
	DCI &b68210d3		; pkhtblt	r1, r2, r3, asr #1
	DCI &c68210d3		; pkhtbgt	r1, r2, r3, asr #1
	DCI &d68210d3		; pkhtble	r1, r2, r3, asr #1
	DCI &e68210d3		; pkhtb		r1, r2, r3, asr #1
	DCI &268210d3		; pkhtbcs	r1, r2, r3, asr #1
	DCI &368210d3		; pkhtbcc	r1, r2, r3, asr #1
	DCI &e6821fd3		; pkhtb		r1, r2, r3, asr #31
	DCI &06821fd3		; pkhtbeq	r1, r2, r3, asr #31
	DCI &16821fd3		; pkhtbne	r1, r2, r3, asr #31
	DCI &26821fd3		; pkhtbcs	r1, r2, r3, asr #31
	DCI &36821fd3		; pkhtbcc	r1, r2, r3, asr #31
	DCI &46821fd3		; pkhtbmi	r1, r2, r3, asr #31
	DCI &56821fd3		; pkhtbpl	r1, r2, r3, asr #31
	DCI &66821fd3		; pkhtbvs	r1, r2, r3, asr #31
	DCI &76821fd3		; pkhtbvc	r1, r2, r3, asr #31
	DCI &86821fd3		; pkhtbhi	r1, r2, r3, asr #31
	DCI &96821fd3		; pkhtbls	r1, r2, r3, asr #31
	DCI &a6821fd3		; pkhtbge	r1, r2, r3, asr #31
	DCI &b6821fd3		; pkhtblt	r1, r2, r3, asr #31
	DCI &c6821fd3		; pkhtbgt	r1, r2, r3, asr #31
	DCI &d6821fd3		; pkhtble	r1, r2, r3, asr #31
	DCI &e6821fd3		; pkhtb		r1, r2, r3, asr #31
	DCI &26821fd3		; pkhtbcs	r1, r2, r3, asr #31
	DCI &36821fd3		; pkhtbcc	r1, r2, r3, asr #31
	DCI &e6821053		; pkhtb		r1, r2, r3, asr #32
	DCI &06821053		; pkhtbeq	r1, r2, r3, asr #32
	DCI &16821053		; pkhtbne	r1, r2, r3, asr #32
	DCI &26821053		; pkhtbcs	r1, r2, r3, asr #32
	DCI &36821053		; pkhtbcc	r1, r2, r3, asr #32
	DCI &46821053		; pkhtbmi	r1, r2, r3, asr #32
	DCI &56821053		; pkhtbpl	r1, r2, r3, asr #32
	DCI &66821053		; pkhtbvs	r1, r2, r3, asr #32
	DCI &76821053		; pkhtbvc	r1, r2, r3, asr #32
	DCI &86821053		; pkhtbhi	r1, r2, r3, asr #32
	DCI &96821053		; pkhtbls	r1, r2, r3, asr #32
	DCI &a6821053		; pkhtbge	r1, r2, r3, asr #32
	DCI &b6821053		; pkhtblt	r1, r2, r3, asr #32
	DCI &c6821053		; pkhtbgt	r1, r2, r3, asr #32
	DCI &d6821053		; pkhtble	r1, r2, r3, asr #32
	DCI &e6821053		; pkhtb		r1, r2, r3, asr #32
	DCI &26821053		; pkhtbcs	r1, r2, r3, asr #32
	DCI &36821053		; pkhtbcc	r1, r2, r3, asr #32


	ARM
	; Qualifier .W:
	DCI &e6821013		; pkhbt		r1, r2, r3
	DCI &06821013		; pkhbteq	r1, r2, r3
	DCI &16821013		; pkhbtne	r1, r2, r3
	DCI &26821013		; pkhbtcs	r1, r2, r3
	DCI &36821013		; pkhbtcc	r1, r2, r3
	DCI &46821013		; pkhbtmi	r1, r2, r3
	DCI &56821013		; pkhbtpl	r1, r2, r3
	DCI &66821013		; pkhbtvs	r1, r2, r3
	DCI &76821013		; pkhbtvc	r1, r2, r3
	DCI &86821013		; pkhbthi	r1, r2, r3
	DCI &96821013		; pkhbtls	r1, r2, r3
	DCI &a6821013		; pkhbtge	r1, r2, r3
	DCI &b6821013		; pkhbtlt	r1, r2, r3
	DCI &c6821013		; pkhbtgt	r1, r2, r3
	DCI &d6821013		; pkhbtle	r1, r2, r3
	DCI &e6821013		; pkhbt		r1, r2, r3
	DCI &26821013		; pkhbtcs	r1, r2, r3
	DCI &36821013		; pkhbtcc	r1, r2, r3
	DCI &e6821013		; pkhbt		r1, r2, r3
	DCI &06821013		; pkhbteq	r1, r2, r3
	DCI &16821013		; pkhbtne	r1, r2, r3
	DCI &26821013		; pkhbtcs	r1, r2, r3
	DCI &36821013		; pkhbtcc	r1, r2, r3
	DCI &46821013		; pkhbtmi	r1, r2, r3
	DCI &56821013		; pkhbtpl	r1, r2, r3
	DCI &66821013		; pkhbtvs	r1, r2, r3
	DCI &76821013		; pkhbtvc	r1, r2, r3
	DCI &86821013		; pkhbthi	r1, r2, r3
	DCI &96821013		; pkhbtls	r1, r2, r3
	DCI &a6821013		; pkhbtge	r1, r2, r3
	DCI &b6821013		; pkhbtlt	r1, r2, r3
	DCI &c6821013		; pkhbtgt	r1, r2, r3
	DCI &d6821013		; pkhbtle	r1, r2, r3
	DCI &e6821013		; pkhbt		r1, r2, r3
	DCI &26821013		; pkhbtcs	r1, r2, r3
	DCI &36821013		; pkhbtcc	r1, r2, r3
	DCI &e6821093		; pkhbt		r1, r2, r3, lsl #1
	DCI &06821093		; pkhbteq	r1, r2, r3, lsl #1
	DCI &16821093		; pkhbtne	r1, r2, r3, lsl #1
	DCI &26821093		; pkhbtcs	r1, r2, r3, lsl #1
	DCI &36821093		; pkhbtcc	r1, r2, r3, lsl #1
	DCI &46821093		; pkhbtmi	r1, r2, r3, lsl #1
	DCI &56821093		; pkhbtpl	r1, r2, r3, lsl #1
	DCI &66821093		; pkhbtvs	r1, r2, r3, lsl #1
	DCI &76821093		; pkhbtvc	r1, r2, r3, lsl #1
	DCI &86821093		; pkhbthi	r1, r2, r3, lsl #1
	DCI &96821093		; pkhbtls	r1, r2, r3, lsl #1
	DCI &a6821093		; pkhbtge	r1, r2, r3, lsl #1
	DCI &b6821093		; pkhbtlt	r1, r2, r3, lsl #1
	DCI &c6821093		; pkhbtgt	r1, r2, r3, lsl #1
	DCI &d6821093		; pkhbtle	r1, r2, r3, lsl #1
	DCI &e6821093		; pkhbt		r1, r2, r3, lsl #1
	DCI &26821093		; pkhbtcs	r1, r2, r3, lsl #1
	DCI &36821093		; pkhbtcc	r1, r2, r3, lsl #1
	DCI &e6821f93		; pkhbt		r1, r2, r3, lsl #31
	DCI &06821f93		; pkhbteq	r1, r2, r3, lsl #31
	DCI &16821f93		; pkhbtne	r1, r2, r3, lsl #31
	DCI &26821f93		; pkhbtcs	r1, r2, r3, lsl #31
	DCI &36821f93		; pkhbtcc	r1, r2, r3, lsl #31
	DCI &46821f93		; pkhbtmi	r1, r2, r3, lsl #31
	DCI &56821f93		; pkhbtpl	r1, r2, r3, lsl #31
	DCI &66821f93		; pkhbtvs	r1, r2, r3, lsl #31
	DCI &76821f93		; pkhbtvc	r1, r2, r3, lsl #31
	DCI &86821f93		; pkhbthi	r1, r2, r3, lsl #31
	DCI &96821f93		; pkhbtls	r1, r2, r3, lsl #31
	DCI &a6821f93		; pkhbtge	r1, r2, r3, lsl #31
	DCI &b6821f93		; pkhbtlt	r1, r2, r3, lsl #31
	DCI &c6821f93		; pkhbtgt	r1, r2, r3, lsl #31
	DCI &d6821f93		; pkhbtle	r1, r2, r3, lsl #31
	DCI &e6821f93		; pkhbt		r1, r2, r3, lsl #31
	DCI &26821f93		; pkhbtcs	r1, r2, r3, lsl #31
	DCI &36821f93		; pkhbtcc	r1, r2, r3, lsl #31
	DCI &e6831012		; pkhbt		r1, r3, r2
	DCI &06831012		; pkhbteq	r1, r3, r2
	DCI &16831012		; pkhbtne	r1, r3, r2
	DCI &26831012		; pkhbtcs	r1, r3, r2
	DCI &36831012		; pkhbtcc	r1, r3, r2
	DCI &46831012		; pkhbtmi	r1, r3, r2
	DCI &56831012		; pkhbtpl	r1, r3, r2
	DCI &66831012		; pkhbtvs	r1, r3, r2
	DCI &76831012		; pkhbtvc	r1, r3, r2
	DCI &86831012		; pkhbthi	r1, r3, r2
	DCI &96831012		; pkhbtls	r1, r3, r2
	DCI &a6831012		; pkhbtge	r1, r3, r2
	DCI &b6831012		; pkhbtlt	r1, r3, r2
	DCI &c6831012		; pkhbtgt	r1, r3, r2
	DCI &d6831012		; pkhbtle	r1, r3, r2
	DCI &e6831012		; pkhbt		r1, r3, r2
	DCI &26831012		; pkhbtcs	r1, r3, r2
	DCI &36831012		; pkhbtcc	r1, r3, r2
	DCI &e68210d3		; pkhtb		r1, r2, r3, asr #1
	DCI &068210d3		; pkhtbeq	r1, r2, r3, asr #1
	DCI &168210d3		; pkhtbne	r1, r2, r3, asr #1
	DCI &268210d3		; pkhtbcs	r1, r2, r3, asr #1
	DCI &368210d3		; pkhtbcc	r1, r2, r3, asr #1
	DCI &468210d3		; pkhtbmi	r1, r2, r3, asr #1
	DCI &568210d3		; pkhtbpl	r1, r2, r3, asr #1
	DCI &668210d3		; pkhtbvs	r1, r2, r3, asr #1
	DCI &768210d3		; pkhtbvc	r1, r2, r3, asr #1
	DCI &868210d3		; pkhtbhi	r1, r2, r3, asr #1
	DCI &968210d3		; pkhtbls	r1, r2, r3, asr #1
	DCI &a68210d3		; pkhtbge	r1, r2, r3, asr #1
	DCI &b68210d3		; pkhtblt	r1, r2, r3, asr #1
	DCI &c68210d3		; pkhtbgt	r1, r2, r3, asr #1
	DCI &d68210d3		; pkhtble	r1, r2, r3, asr #1
	DCI &e68210d3		; pkhtb		r1, r2, r3, asr #1
	DCI &268210d3		; pkhtbcs	r1, r2, r3, asr #1
	DCI &368210d3		; pkhtbcc	r1, r2, r3, asr #1
	DCI &e6821fd3		; pkhtb		r1, r2, r3, asr #31
	DCI &06821fd3		; pkhtbeq	r1, r2, r3, asr #31
	DCI &16821fd3		; pkhtbne	r1, r2, r3, asr #31
	DCI &26821fd3		; pkhtbcs	r1, r2, r3, asr #31
	DCI &36821fd3		; pkhtbcc	r1, r2, r3, asr #31
	DCI &46821fd3		; pkhtbmi	r1, r2, r3, asr #31
	DCI &56821fd3		; pkhtbpl	r1, r2, r3, asr #31
	DCI &66821fd3		; pkhtbvs	r1, r2, r3, asr #31
	DCI &76821fd3		; pkhtbvc	r1, r2, r3, asr #31
	DCI &86821fd3		; pkhtbhi	r1, r2, r3, asr #31
	DCI &96821fd3		; pkhtbls	r1, r2, r3, asr #31
	DCI &a6821fd3		; pkhtbge	r1, r2, r3, asr #31
	DCI &b6821fd3		; pkhtblt	r1, r2, r3, asr #31
	DCI &c6821fd3		; pkhtbgt	r1, r2, r3, asr #31
	DCI &d6821fd3		; pkhtble	r1, r2, r3, asr #31
	DCI &e6821fd3		; pkhtb		r1, r2, r3, asr #31
	DCI &26821fd3		; pkhtbcs	r1, r2, r3, asr #31
	DCI &36821fd3		; pkhtbcc	r1, r2, r3, asr #31
	DCI &e6821053		; pkhtb		r1, r2, r3, asr #32
	DCI &06821053		; pkhtbeq	r1, r2, r3, asr #32
	DCI &16821053		; pkhtbne	r1, r2, r3, asr #32
	DCI &26821053		; pkhtbcs	r1, r2, r3, asr #32
	DCI &36821053		; pkhtbcc	r1, r2, r3, asr #32
	DCI &46821053		; pkhtbmi	r1, r2, r3, asr #32
	DCI &56821053		; pkhtbpl	r1, r2, r3, asr #32
	DCI &66821053		; pkhtbvs	r1, r2, r3, asr #32
	DCI &76821053		; pkhtbvc	r1, r2, r3, asr #32
	DCI &86821053		; pkhtbhi	r1, r2, r3, asr #32
	DCI &96821053		; pkhtbls	r1, r2, r3, asr #32
	DCI &a6821053		; pkhtbge	r1, r2, r3, asr #32
	DCI &b6821053		; pkhtblt	r1, r2, r3, asr #32
	DCI &c6821053		; pkhtbgt	r1, r2, r3, asr #32
	DCI &d6821053		; pkhtble	r1, r2, r3, asr #32
	DCI &e6821053		; pkhtb		r1, r2, r3, asr #32
	DCI &26821053		; pkhtbcs	r1, r2, r3, asr #32
	DCI &36821053		; pkhtbcc	r1, r2, r3, asr #32

	THUMB
	; No qualifier:
	DCI.W	&eac20103	; pkhbt		r1, r2, r3
	DCI.N	&bf0c		; ite		eq
	DCI.W	&eac20103	; pkhbteq	r1, r2, r3
	DCI.W	&eac20103	; pkhbtne	r1, r2, r3
	DCI.N	&bf2c		; ite		cs
	DCI.W	&eac20103	; pkhbtcs	r1, r2, r3
	DCI.W	&eac20103	; pkhbtcc	r1, r2, r3
	DCI.N	&bf4c		; ite		mi
	DCI.W	&eac20103	; pkhbtmi	r1, r2, r3
	DCI.W	&eac20103	; pkhbtpl	r1, r2, r3
	DCI.N	&bf6c		; ite		vs
	DCI.W	&eac20103	; pkhbtvs	r1, r2, r3
	DCI.W	&eac20103	; pkhbtvc	r1, r2, r3
	DCI.N	&bf8c		; ite		hi
	DCI.W	&eac20103	; pkhbthi	r1, r2, r3
	DCI.W	&eac20103	; pkhbtls	r1, r2, r3
	DCI.N	&bfac		; ite		ge
	DCI.W	&eac20103	; pkhbtge	r1, r2, r3
	DCI.W	&eac20103	; pkhbtlt	r1, r2, r3
	DCI.N	&bfcc		; ite		gt
	DCI.W	&eac20103	; pkhbtgt	r1, r2, r3
	DCI.W	&eac20103	; pkhbtle	r1, r2, r3
	DCI.W	&eac20103	; pkhbt		r1, r2, r3
	DCI.N	&bf2c		; ite		cs
	DCI.W	&eac20103	; pkhbtcs	r1, r2, r3
	DCI.W	&eac20103	; pkhbtcc	r1, r2, r3
	DCI.W	&eac20103	; pkhbt		r1, r2, r3
	DCI.N	&bf0c		; ite		eq
	DCI.W	&eac20103	; pkhbteq	r1, r2, r3
	DCI.W	&eac20103	; pkhbtne	r1, r2, r3
	DCI.N	&bf2c		; ite		cs
	DCI.W	&eac20103	; pkhbtcs	r1, r2, r3
	DCI.W	&eac20103	; pkhbtcc	r1, r2, r3
	DCI.N	&bf4c		; ite		mi
	DCI.W	&eac20103	; pkhbtmi	r1, r2, r3
	DCI.W	&eac20103	; pkhbtpl	r1, r2, r3
	DCI.N	&bf6c		; ite		vs
	DCI.W	&eac20103	; pkhbtvs	r1, r2, r3
	DCI.W	&eac20103	; pkhbtvc	r1, r2, r3
	DCI.N	&bf8c		; ite		hi
	DCI.W	&eac20103	; pkhbthi	r1, r2, r3
	DCI.W	&eac20103	; pkhbtls	r1, r2, r3
	DCI.N	&bfac		; ite		ge
	DCI.W	&eac20103	; pkhbtge	r1, r2, r3
	DCI.W	&eac20103	; pkhbtlt	r1, r2, r3
	DCI.N	&bfcc		; ite		gt
	DCI.W	&eac20103	; pkhbtgt	r1, r2, r3
	DCI.W	&eac20103	; pkhbtle	r1, r2, r3
	DCI.W	&eac20103	; pkhbt		r1, r2, r3
	DCI.N	&bf2c		; ite		cs
	DCI.W	&eac20103	; pkhbtcs	r1, r2, r3
	DCI.W	&eac20103	; pkhbtcc	r1, r2, r3
	DCI.W	&eac20143	; pkhbt		r1, r2, r3, lsl #1
	DCI.N	&bf0c		; ite		eq
	DCI.W	&eac20143	; pkhbteq	r1, r2, r3, lsl #1
	DCI.W	&eac20143	; pkhbtne	r1, r2, r3, lsl #1
	DCI.N	&bf2c		; ite		cs
	DCI.W	&eac20143	; pkhbtcs	r1, r2, r3, lsl #1
	DCI.W	&eac20143	; pkhbtcc	r1, r2, r3, lsl #1
	DCI.N	&bf4c		; ite		mi
	DCI.W	&eac20143	; pkhbtmi	r1, r2, r3, lsl #1
	DCI.W	&eac20143	; pkhbtpl	r1, r2, r3, lsl #1
	DCI.N	&bf6c		; ite		vs
	DCI.W	&eac20143	; pkhbtvs	r1, r2, r3, lsl #1
	DCI.W	&eac20143	; pkhbtvc	r1, r2, r3, lsl #1
	DCI.N	&bf8c		; ite		hi
	DCI.W	&eac20143	; pkhbthi	r1, r2, r3, lsl #1
	DCI.W	&eac20143	; pkhbtls	r1, r2, r3, lsl #1
	DCI.N	&bfac		; ite		ge
	DCI.W	&eac20143	; pkhbtge	r1, r2, r3, lsl #1
	DCI.W	&eac20143	; pkhbtlt	r1, r2, r3, lsl #1
	DCI.N	&bfcc		; ite		gt
	DCI.W	&eac20143	; pkhbtgt	r1, r2, r3, lsl #1
	DCI.W	&eac20143	; pkhbtle	r1, r2, r3, lsl #1
	DCI.W	&eac20143	; pkhbt		r1, r2, r3, lsl #1
	DCI.N	&bf2c		; ite		cs
	DCI.W	&eac20143	; pkhbtcs	r1, r2, r3, lsl #1
	DCI.W	&eac20143	; pkhbtcc	r1, r2, r3, lsl #1
	DCI.W	&eac271c3	; pkhbt		r1, r2, r3, lsl #31
	DCI.N	&bf0c		; ite		eq
	DCI.W	&eac271c3	; pkhbteq	r1, r2, r3, lsl #31
	DCI.W	&eac271c3	; pkhbtne	r1, r2, r3, lsl #31
	DCI.N	&bf2c		; ite		cs
	DCI.W	&eac271c3	; pkhbtcs	r1, r2, r3, lsl #31
	DCI.W	&eac271c3	; pkhbtcc	r1, r2, r3, lsl #31
	DCI.N	&bf4c		; ite		mi
	DCI.W	&eac271c3	; pkhbtmi	r1, r2, r3, lsl #31
	DCI.W	&eac271c3	; pkhbtpl	r1, r2, r3, lsl #31
	DCI.N	&bf6c		; ite		vs
	DCI.W	&eac271c3	; pkhbtvs	r1, r2, r3, lsl #31
	DCI.W	&eac271c3	; pkhbtvc	r1, r2, r3, lsl #31
	DCI.N	&bf8c		; ite		hi
	DCI.W	&eac271c3	; pkhbthi	r1, r2, r3, lsl #31
	DCI.W	&eac271c3	; pkhbtls	r1, r2, r3, lsl #31
	DCI.N	&bfac		; ite		ge
	DCI.W	&eac271c3	; pkhbtge	r1, r2, r3, lsl #31
	DCI.W	&eac271c3	; pkhbtlt	r1, r2, r3, lsl #31
	DCI.N	&bfcc		; ite		gt
	DCI.W	&eac271c3	; pkhbtgt	r1, r2, r3, lsl #31
	DCI.W	&eac271c3	; pkhbtle	r1, r2, r3, lsl #31
	DCI.W	&eac271c3	; pkhbt		r1, r2, r3, lsl #31
	DCI.N	&bf2c		; ite		cs
	DCI.W	&eac271c3	; pkhbtcs	r1, r2, r3, lsl #31
	DCI.W	&eac271c3	; pkhbtcc	r1, r2, r3, lsl #31
	DCI.W	&eac30102	; pkhbt		r1, r3, r2
	DCI.N	&bf0c		; ite		eq
	DCI.W	&eac30102	; pkhbteq	r1, r3, r2
	DCI.W	&eac30102	; pkhbtne	r1, r3, r2
	DCI.N	&bf2c		; ite		cs
	DCI.W	&eac30102	; pkhbtcs	r1, r3, r2
	DCI.W	&eac30102	; pkhbtcc	r1, r3, r2
	DCI.N	&bf4c		; ite		mi
	DCI.W	&eac30102	; pkhbtmi	r1, r3, r2
	DCI.W	&eac30102	; pkhbtpl	r1, r3, r2
	DCI.N	&bf6c		; ite		vs
	DCI.W	&eac30102	; pkhbtvs	r1, r3, r2
	DCI.W	&eac30102	; pkhbtvc	r1, r3, r2
	DCI.N	&bf8c		; ite		hi
	DCI.W	&eac30102	; pkhbthi	r1, r3, r2
	DCI.W	&eac30102	; pkhbtls	r1, r3, r2
	DCI.N	&bfac		; ite		ge
	DCI.W	&eac30102	; pkhbtge	r1, r3, r2
	DCI.W	&eac30102	; pkhbtlt	r1, r3, r2
	DCI.N	&bfcc		; ite		gt
	DCI.W	&eac30102	; pkhbtgt	r1, r3, r2
	DCI.W	&eac30102	; pkhbtle	r1, r3, r2
	DCI.W	&eac30102	; pkhbt		r1, r3, r2
	DCI.N	&bf2c		; ite		cs
	DCI.W	&eac30102	; pkhbtcs	r1, r3, r2
	DCI.W	&eac30102	; pkhbtcc	r1, r3, r2
	DCI.W	&eac20163	; pkhtb		r1, r2, r3, asr #1
	DCI.N	&bf0c		; ite		eq
	DCI.W	&eac20163	; pkhtbeq	r1, r2, r3, asr #1
	DCI.W	&eac20163	; pkhtbne	r1, r2, r3, asr #1
	DCI.N	&bf2c		; ite		cs
	DCI.W	&eac20163	; pkhtbcs	r1, r2, r3, asr #1
	DCI.W	&eac20163	; pkhtbcc	r1, r2, r3, asr #1
	DCI.N	&bf4c		; ite		mi
	DCI.W	&eac20163	; pkhtbmi	r1, r2, r3, asr #1
	DCI.W	&eac20163	; pkhtbpl	r1, r2, r3, asr #1
	DCI.N	&bf6c		; ite		vs
	DCI.W	&eac20163	; pkhtbvs	r1, r2, r3, asr #1
	DCI.W	&eac20163	; pkhtbvc	r1, r2, r3, asr #1
	DCI.N	&bf8c		; ite		hi
	DCI.W	&eac20163	; pkhtbhi	r1, r2, r3, asr #1
	DCI.W	&eac20163	; pkhtbls	r1, r2, r3, asr #1
	DCI.N	&bfac		; ite		ge
	DCI.W	&eac20163	; pkhtbge	r1, r2, r3, asr #1
	DCI.W	&eac20163	; pkhtblt	r1, r2, r3, asr #1
	DCI.N	&bfcc		; ite		gt
	DCI.W	&eac20163	; pkhtbgt	r1, r2, r3, asr #1
	DCI.W	&eac20163	; pkhtble	r1, r2, r3, asr #1
	DCI.W	&eac20163	; pkhtb		r1, r2, r3, asr #1
	DCI.N	&bf2c		; ite		cs
	DCI.W	&eac20163	; pkhtbcs	r1, r2, r3, asr #1
	DCI.W	&eac20163	; pkhtbcc	r1, r2, r3, asr #1
	DCI.W	&eac271e3	; pkhtb		r1, r2, r3, asr #31
	DCI.N	&bf0c		; ite		eq
	DCI.W	&eac271e3	; pkhtbeq	r1, r2, r3, asr #31
	DCI.W	&eac271e3	; pkhtbne	r1, r2, r3, asr #31
	DCI.N	&bf2c		; ite		cs
	DCI.W	&eac271e3	; pkhtbcs	r1, r2, r3, asr #31
	DCI.W	&eac271e3	; pkhtbcc	r1, r2, r3, asr #31
	DCI.N	&bf4c		; ite		mi
	DCI.W	&eac271e3	; pkhtbmi	r1, r2, r3, asr #31
	DCI.W	&eac271e3	; pkhtbpl	r1, r2, r3, asr #31
	DCI.N	&bf6c		; ite		vs
	DCI.W	&eac271e3	; pkhtbvs	r1, r2, r3, asr #31
	DCI.W	&eac271e3	; pkhtbvc	r1, r2, r3, asr #31
	DCI.N	&bf8c		; ite		hi
	DCI.W	&eac271e3	; pkhtbhi	r1, r2, r3, asr #31
	DCI.W	&eac271e3	; pkhtbls	r1, r2, r3, asr #31
	DCI.N	&bfac		; ite		ge
	DCI.W	&eac271e3	; pkhtbge	r1, r2, r3, asr #31
	DCI.W	&eac271e3	; pkhtblt	r1, r2, r3, asr #31
	DCI.N	&bfcc		; ite		gt
	DCI.W	&eac271e3	; pkhtbgt	r1, r2, r3, asr #31
	DCI.W	&eac271e3	; pkhtble	r1, r2, r3, asr #31
	DCI.W	&eac271e3	; pkhtb		r1, r2, r3, asr #31
	DCI.N	&bf2c		; ite		cs
	DCI.W	&eac271e3	; pkhtbcs	r1, r2, r3, asr #31
	DCI.W	&eac271e3	; pkhtbcc	r1, r2, r3, asr #31
	DCI.W	&eac20123	; pkhtb		r1, r2, r3, asr #32
	DCI.N	&bf0c		; ite		eq
	DCI.W	&eac20123	; pkhtbeq	r1, r2, r3, asr #32
	DCI.W	&eac20123	; pkhtbne	r1, r2, r3, asr #32
	DCI.N	&bf2c		; ite		cs
	DCI.W	&eac20123	; pkhtbcs	r1, r2, r3, asr #32
	DCI.W	&eac20123	; pkhtbcc	r1, r2, r3, asr #32
	DCI.N	&bf4c		; ite		mi
	DCI.W	&eac20123	; pkhtbmi	r1, r2, r3, asr #32
	DCI.W	&eac20123	; pkhtbpl	r1, r2, r3, asr #32
	DCI.N	&bf6c		; ite		vs
	DCI.W	&eac20123	; pkhtbvs	r1, r2, r3, asr #32
	DCI.W	&eac20123	; pkhtbvc	r1, r2, r3, asr #32
	DCI.N	&bf8c		; ite		hi
	DCI.W	&eac20123	; pkhtbhi	r1, r2, r3, asr #32
	DCI.W	&eac20123	; pkhtbls	r1, r2, r3, asr #32
	DCI.N	&bfac		; ite		ge
	DCI.W	&eac20123	; pkhtbge	r1, r2, r3, asr #32
	DCI.W	&eac20123	; pkhtblt	r1, r2, r3, asr #32
	DCI.N	&bfcc		; ite		gt
	DCI.W	&eac20123	; pkhtbgt	r1, r2, r3, asr #32
	DCI.W	&eac20123	; pkhtble	r1, r2, r3, asr #32
	DCI.W	&eac20123	; pkhtb		r1, r2, r3, asr #32
	DCI.N	&bf2c		; ite		cs
	DCI.W	&eac20123	; pkhtbcs	r1, r2, r3, asr #32
	DCI.W	&eac20123	; pkhtbcc	r1, r2, r3, asr #32

	; Qualifier .W:
	DCI.W	&eac20103	; pkhbt		r1, r2, r3
	DCI.N	&bf0c		; ite		eq
	DCI.W	&eac20103	; pkhbteq	r1, r2, r3
	DCI.W	&eac20103	; pkhbtne	r1, r2, r3
	DCI.N	&bf2c		; ite		cs
	DCI.W	&eac20103	; pkhbtcs	r1, r2, r3
	DCI.W	&eac20103	; pkhbtcc	r1, r2, r3
	DCI.N	&bf4c		; ite		mi
	DCI.W	&eac20103	; pkhbtmi	r1, r2, r3
	DCI.W	&eac20103	; pkhbtpl	r1, r2, r3
	DCI.N	&bf6c		; ite		vs
	DCI.W	&eac20103	; pkhbtvs	r1, r2, r3
	DCI.W	&eac20103	; pkhbtvc	r1, r2, r3
	DCI.N	&bf8c		; ite		hi
	DCI.W	&eac20103	; pkhbthi	r1, r2, r3
	DCI.W	&eac20103	; pkhbtls	r1, r2, r3
	DCI.N	&bfac		; ite		ge
	DCI.W	&eac20103	; pkhbtge	r1, r2, r3
	DCI.W	&eac20103	; pkhbtlt	r1, r2, r3
	DCI.N	&bfcc		; ite		gt
	DCI.W	&eac20103	; pkhbtgt	r1, r2, r3
	DCI.W	&eac20103	; pkhbtle	r1, r2, r3
	DCI.W	&eac20103	; pkhbt		r1, r2, r3
	DCI.N	&bf2c		; ite		cs
	DCI.W	&eac20103	; pkhbtcs	r1, r2, r3
	DCI.W	&eac20103	; pkhbtcc	r1, r2, r3
	DCI.W	&eac20103	; pkhbt		r1, r2, r3
	DCI.N	&bf0c		; ite		eq
	DCI.W	&eac20103	; pkhbteq	r1, r2, r3
	DCI.W	&eac20103	; pkhbtne	r1, r2, r3
	DCI.N	&bf2c		; ite		cs
	DCI.W	&eac20103	; pkhbtcs	r1, r2, r3
	DCI.W	&eac20103	; pkhbtcc	r1, r2, r3
	DCI.N	&bf4c		; ite		mi
	DCI.W	&eac20103	; pkhbtmi	r1, r2, r3
	DCI.W	&eac20103	; pkhbtpl	r1, r2, r3
	DCI.N	&bf6c		; ite		vs
	DCI.W	&eac20103	; pkhbtvs	r1, r2, r3
	DCI.W	&eac20103	; pkhbtvc	r1, r2, r3
	DCI.N	&bf8c		; ite		hi
	DCI.W	&eac20103	; pkhbthi	r1, r2, r3
	DCI.W	&eac20103	; pkhbtls	r1, r2, r3
	DCI.N	&bfac		; ite		ge
	DCI.W	&eac20103	; pkhbtge	r1, r2, r3
	DCI.W	&eac20103	; pkhbtlt	r1, r2, r3
	DCI.N	&bfcc		; ite		gt
	DCI.W	&eac20103	; pkhbtgt	r1, r2, r3
	DCI.W	&eac20103	; pkhbtle	r1, r2, r3
	DCI.W	&eac20103	; pkhbt		r1, r2, r3
	DCI.N	&bf2c		; ite		cs
	DCI.W	&eac20103	; pkhbtcs	r1, r2, r3
	DCI.W	&eac20103	; pkhbtcc	r1, r2, r3
	DCI.W	&eac20143	; pkhbt		r1, r2, r3, lsl #1
	DCI.N	&bf0c		; ite		eq
	DCI.W	&eac20143	; pkhbteq	r1, r2, r3, lsl #1
	DCI.W	&eac20143	; pkhbtne	r1, r2, r3, lsl #1
	DCI.N	&bf2c		; ite		cs
	DCI.W	&eac20143	; pkhbtcs	r1, r2, r3, lsl #1
	DCI.W	&eac20143	; pkhbtcc	r1, r2, r3, lsl #1
	DCI.N	&bf4c		; ite		mi
	DCI.W	&eac20143	; pkhbtmi	r1, r2, r3, lsl #1
	DCI.W	&eac20143	; pkhbtpl	r1, r2, r3, lsl #1
	DCI.N	&bf6c		; ite		vs
	DCI.W	&eac20143	; pkhbtvs	r1, r2, r3, lsl #1
	DCI.W	&eac20143	; pkhbtvc	r1, r2, r3, lsl #1
	DCI.N	&bf8c		; ite		hi
	DCI.W	&eac20143	; pkhbthi	r1, r2, r3, lsl #1
	DCI.W	&eac20143	; pkhbtls	r1, r2, r3, lsl #1
	DCI.N	&bfac		; ite		ge
	DCI.W	&eac20143	; pkhbtge	r1, r2, r3, lsl #1
	DCI.W	&eac20143	; pkhbtlt	r1, r2, r3, lsl #1
	DCI.N	&bfcc		; ite		gt
	DCI.W	&eac20143	; pkhbtgt	r1, r2, r3, lsl #1
	DCI.W	&eac20143	; pkhbtle	r1, r2, r3, lsl #1
	DCI.W	&eac20143	; pkhbt		r1, r2, r3, lsl #1
	DCI.N	&bf2c		; ite		cs
	DCI.W	&eac20143	; pkhbtcs	r1, r2, r3, lsl #1
	DCI.W	&eac20143	; pkhbtcc	r1, r2, r3, lsl #1
	DCI.W	&eac271c3	; pkhbt		r1, r2, r3, lsl #31
	DCI.N	&bf0c		; ite		eq
	DCI.W	&eac271c3	; pkhbteq	r1, r2, r3, lsl #31
	DCI.W	&eac271c3	; pkhbtne	r1, r2, r3, lsl #31
	DCI.N	&bf2c		; ite		cs
	DCI.W	&eac271c3	; pkhbtcs	r1, r2, r3, lsl #31
	DCI.W	&eac271c3	; pkhbtcc	r1, r2, r3, lsl #31
	DCI.N	&bf4c		; ite		mi
	DCI.W	&eac271c3	; pkhbtmi	r1, r2, r3, lsl #31
	DCI.W	&eac271c3	; pkhbtpl	r1, r2, r3, lsl #31
	DCI.N	&bf6c		; ite		vs
	DCI.W	&eac271c3	; pkhbtvs	r1, r2, r3, lsl #31
	DCI.W	&eac271c3	; pkhbtvc	r1, r2, r3, lsl #31
	DCI.N	&bf8c		; ite		hi
	DCI.W	&eac271c3	; pkhbthi	r1, r2, r3, lsl #31
	DCI.W	&eac271c3	; pkhbtls	r1, r2, r3, lsl #31
	DCI.N	&bfac		; ite		ge
	DCI.W	&eac271c3	; pkhbtge	r1, r2, r3, lsl #31
	DCI.W	&eac271c3	; pkhbtlt	r1, r2, r3, lsl #31
	DCI.N	&bfcc		; ite		gt
	DCI.W	&eac271c3	; pkhbtgt	r1, r2, r3, lsl #31
	DCI.W	&eac271c3	; pkhbtle	r1, r2, r3, lsl #31
	DCI.W	&eac271c3	; pkhbt		r1, r2, r3, lsl #31
	DCI.N	&bf2c		; ite		cs
	DCI.W	&eac271c3	; pkhbtcs	r1, r2, r3, lsl #31
	DCI.W	&eac271c3	; pkhbtcc	r1, r2, r3, lsl #31
	DCI.W	&eac30102	; pkhbt		r1, r3, r2
	DCI.N	&bf0c		; ite		eq
	DCI.W	&eac30102	; pkhbteq	r1, r3, r2
	DCI.W	&eac30102	; pkhbtne	r1, r3, r2
	DCI.N	&bf2c		; ite		cs
	DCI.W	&eac30102	; pkhbtcs	r1, r3, r2
	DCI.W	&eac30102	; pkhbtcc	r1, r3, r2
	DCI.N	&bf4c		; ite		mi
	DCI.W	&eac30102	; pkhbtmi	r1, r3, r2
	DCI.W	&eac30102	; pkhbtpl	r1, r3, r2
	DCI.N	&bf6c		; ite		vs
	DCI.W	&eac30102	; pkhbtvs	r1, r3, r2
	DCI.W	&eac30102	; pkhbtvc	r1, r3, r2
	DCI.N	&bf8c		; ite		hi
	DCI.W	&eac30102	; pkhbthi	r1, r3, r2
	DCI.W	&eac30102	; pkhbtls	r1, r3, r2
	DCI.N	&bfac		; ite		ge
	DCI.W	&eac30102	; pkhbtge	r1, r3, r2
	DCI.W	&eac30102	; pkhbtlt	r1, r3, r2
	DCI.N	&bfcc		; ite		gt
	DCI.W	&eac30102	; pkhbtgt	r1, r3, r2
	DCI.W	&eac30102	; pkhbtle	r1, r3, r2
	DCI.W	&eac30102	; pkhbt		r1, r3, r2
	DCI.N	&bf2c		; ite		cs
	DCI.W	&eac30102	; pkhbtcs	r1, r3, r2
	DCI.W	&eac30102	; pkhbtcc	r1, r3, r2
	DCI.W	&eac20163	; pkhtb		r1, r2, r3, asr #1
	DCI.N	&bf0c		; ite		eq
	DCI.W	&eac20163	; pkhtbeq	r1, r2, r3, asr #1
	DCI.W	&eac20163	; pkhtbne	r1, r2, r3, asr #1
	DCI.N	&bf2c		; ite		cs
	DCI.W	&eac20163	; pkhtbcs	r1, r2, r3, asr #1
	DCI.W	&eac20163	; pkhtbcc	r1, r2, r3, asr #1
	DCI.N	&bf4c		; ite		mi
	DCI.W	&eac20163	; pkhtbmi	r1, r2, r3, asr #1
	DCI.W	&eac20163	; pkhtbpl	r1, r2, r3, asr #1
	DCI.N	&bf6c		; ite		vs
	DCI.W	&eac20163	; pkhtbvs	r1, r2, r3, asr #1
	DCI.W	&eac20163	; pkhtbvc	r1, r2, r3, asr #1
	DCI.N	&bf8c		; ite		hi
	DCI.W	&eac20163	; pkhtbhi	r1, r2, r3, asr #1
	DCI.W	&eac20163	; pkhtbls	r1, r2, r3, asr #1
	DCI.N	&bfac		; ite		ge
	DCI.W	&eac20163	; pkhtbge	r1, r2, r3, asr #1
	DCI.W	&eac20163	; pkhtblt	r1, r2, r3, asr #1
	DCI.N	&bfcc		; ite		gt
	DCI.W	&eac20163	; pkhtbgt	r1, r2, r3, asr #1
	DCI.W	&eac20163	; pkhtble	r1, r2, r3, asr #1
	DCI.W	&eac20163	; pkhtb		r1, r2, r3, asr #1
	DCI.N	&bf2c		; ite		cs
	DCI.W	&eac20163	; pkhtbcs	r1, r2, r3, asr #1
	DCI.W	&eac20163	; pkhtbcc	r1, r2, r3, asr #1
	DCI.W	&eac271e3	; pkhtb		r1, r2, r3, asr #31
	DCI.N	&bf0c		; ite		eq
	DCI.W	&eac271e3	; pkhtbeq	r1, r2, r3, asr #31
	DCI.W	&eac271e3	; pkhtbne	r1, r2, r3, asr #31
	DCI.N	&bf2c		; ite		cs
	DCI.W	&eac271e3	; pkhtbcs	r1, r2, r3, asr #31
	DCI.W	&eac271e3	; pkhtbcc	r1, r2, r3, asr #31
	DCI.N	&bf4c		; ite		mi
	DCI.W	&eac271e3	; pkhtbmi	r1, r2, r3, asr #31
	DCI.W	&eac271e3	; pkhtbpl	r1, r2, r3, asr #31
	DCI.N	&bf6c		; ite		vs
	DCI.W	&eac271e3	; pkhtbvs	r1, r2, r3, asr #31
	DCI.W	&eac271e3	; pkhtbvc	r1, r2, r3, asr #31
	DCI.N	&bf8c		; ite		hi
	DCI.W	&eac271e3	; pkhtbhi	r1, r2, r3, asr #31
	DCI.W	&eac271e3	; pkhtbls	r1, r2, r3, asr #31
	DCI.N	&bfac		; ite		ge
	DCI.W	&eac271e3	; pkhtbge	r1, r2, r3, asr #31
	DCI.W	&eac271e3	; pkhtblt	r1, r2, r3, asr #31
	DCI.N	&bfcc		; ite		gt
	DCI.W	&eac271e3	; pkhtbgt	r1, r2, r3, asr #31
	DCI.W	&eac271e3	; pkhtble	r1, r2, r3, asr #31
	DCI.W	&eac271e3	; pkhtb		r1, r2, r3, asr #31
	DCI.N	&bf2c		; ite		cs
	DCI.W	&eac271e3	; pkhtbcs	r1, r2, r3, asr #31
	DCI.W	&eac271e3	; pkhtbcc	r1, r2, r3, asr #31
	DCI.W	&eac20123	; pkhtb		r1, r2, r3, asr #32
	DCI.N	&bf0c		; ite		eq
	DCI.W	&eac20123	; pkhtbeq	r1, r2, r3, asr #32
	DCI.W	&eac20123	; pkhtbne	r1, r2, r3, asr #32
	DCI.N	&bf2c		; ite		cs
	DCI.W	&eac20123	; pkhtbcs	r1, r2, r3, asr #32
	DCI.W	&eac20123	; pkhtbcc	r1, r2, r3, asr #32
	DCI.N	&bf4c		; ite		mi
	DCI.W	&eac20123	; pkhtbmi	r1, r2, r3, asr #32
	DCI.W	&eac20123	; pkhtbpl	r1, r2, r3, asr #32
	DCI.N	&bf6c		; ite		vs
	DCI.W	&eac20123	; pkhtbvs	r1, r2, r3, asr #32
	DCI.W	&eac20123	; pkhtbvc	r1, r2, r3, asr #32
	DCI.N	&bf8c		; ite		hi
	DCI.W	&eac20123	; pkhtbhi	r1, r2, r3, asr #32
	DCI.W	&eac20123	; pkhtbls	r1, r2, r3, asr #32
	DCI.N	&bfac		; ite		ge
	DCI.W	&eac20123	; pkhtbge	r1, r2, r3, asr #32
	DCI.W	&eac20123	; pkhtblt	r1, r2, r3, asr #32
	DCI.N	&bfcc		; ite		gt
	DCI.W	&eac20123	; pkhtbgt	r1, r2, r3, asr #32
	DCI.W	&eac20123	; pkhtble	r1, r2, r3, asr #32
	DCI.W	&eac20123	; pkhtb		r1, r2, r3, asr #32
	DCI.N	&bf2c		; ite		cs
	DCI.W	&eac20123	; pkhtbcs	r1, r2, r3, asr #32
	DCI.W	&eac20123	; pkhtbcc	r1, r2, r3, asr #32

	]

	END