; Tests LDM, STM, PUSH and POP.
; LDM/STM/PUSH/POP support for RLIST is tested in directive_rlist.s.
; -RUNOPT: -CPU=Cortex-A8

	AREA Code1, CODE, READONLY

	; When NStrict is {TRUE}, there can be calls to LDM* and STM*
	; macros.
	GBLL	NStrict
NStrict	SETL	{FALSE}
	GBLL	TestARM
TestARM	SETL	{TRUE}
	GBLL	TestCODE16
TestCODE16 SETL	{TRUE}
	GBLL	TestTHUMB
TestTHUMB SETL	{TRUE}
	GBLL	TestPP
TestPP	SETL	{TRUE}
	GBLL	Test1R
Test1R	SETL	{TRUE}

	[ :LNOT: REFERENCE
	; Pre-UAL/UAL support catcher
	MACRO
	LDM$rest $param1, $param2
	NOP	; not available, was LDM$rest $param1, $param2
	;INFO	0, "\tLDM$rest $param1, $param2\t;Unavailable"
	MEND
	MACRO
	STM$rest $param1, $param2
	NOP	; not available, was LDM$rest $param1, $param2
	;INFO	0, "\tSTM$rest $param1, $param2\t;Unavailable"
	MEND

	MACRO
	Inject $full
	;INFO	0, "\t$full"
	$full
	MEND

	; Test:
	;  - LDM/STM + [<.N>|<.W>] $baseReg, $regList
	;  - LDM/STM + <cc> + [<.N>|<.W>] $baseReg, $regList
	; Only supported for ARM and THUMB syntax mode (i.e. UAL syntax is ok),
	; so not CODE16.
	MACRO
	InvokeCC $instr, $baseReg, $regList
	LCLL	narOK
narOK	SETL	{UAL}
	LCLL	wideOK
wideOK	SETL	{CODESIZE}=32 :LOR: {UAL}
	; Only when UAL syntax is allowed.
	[ NStrict :LOR: wideOK
	; 1. No <cc>
	Inject	"$instr $baseReg, $regList"
	[ narOK
	Inject	"$instr..N $baseReg, $regList"
	]
	[ wideOK
	Inject	"$instr..W $baseReg, $regList"
	]

	; 2. Iterate over all <cc> possibilities:
	LCLS	cc
	LCLA	cnt
	WHILE	cnt < 17
	; 17 condition codes to be tested:
	;   - Condition code NV is left out from this test.
	;   - HS and LO are equivalents for CS and CC.
cc	SETS	("EQNECSCCMIPLVSVCHILSGELTGTLEALHSLO" :RIGHT: (34 - 2*cnt)) :LEFT: 2
	Inject	"$instr.$cc $baseReg, $regList"
	[ narOK
	Inject	"$instr.$cc..N $baseReg, $regList"
	]
	[ wideOK
	Inject	"$instr.$cc..W $baseReg, $regList"
	]
cnt	SETA	cnt + 1
	WEND
	]
	MEND

	; Test:
	;  - LDM/STM + <stack type> + [<.N>|<.W>] $baseReg, $regList (pre-UAL and UAL)
	;  - LDM/STM + <cc> + <stack type> + [<.N>|<.W>] $baseReg, $regList (pre-UAL only)
	;  - LDM/STM + <stack type> + <cc> + [<.N>|<.W>] $baseReg, $regList (UAL only)
	; For given LDM/STM instruction, $stackType1 is equivalent to $stackType2.
	MACRO
	InvokeCCnStackType $instr, $stackType1, $stackType2, $baseReg, $regList
	LCLL	narOK
narOK	SETL	{UAL}
	LCLL	wideOK
wideOK	SETL	{CODESIZE}=32 :LOR: {UAL}
	; In CODE16 syntax mode, we only test IA stackmode.
	; In THUMB syntax mode, we only test IA and DB stackmode.
	; In ARM syntax mode, we test all 4 stack modes.
	; 1. No <cc>
	[ {TRUE} :LAND: (NStrict :LOR: ("$stackType1"="IA" :LAND: {CODESIZE}=16) :LOR: ("$stackType1"="DB" :LAND: {UAL}) :LOR: {CODESIZE}=32)
	Inject	"$instr.$stackType1 $baseReg, $regList"
	]
	[ narOK :LAND: (NStrict :LOR: "$stackType1"="IA")
	Inject	"$instr.$stackType1..N $baseReg, $regList"
	]
	[ wideOK :LAND: (NStrict :LOR: {CODESIZE}=32 :LOR: "$stackType1"="IA" :LOR: "$stackType1"="DB")
	Inject	"$instr.$stackType1..W $baseReg, $regList"
	]

	; We only test $stackType2 when in ARM or THUMB ("IA" or "DB" equivalent only) syntax mode.
	[ {TRUE} :LAND: (NStrict :LOR: (("$stackType1"="IA" :LOR: "$stackType1"="DB") :LAND: {UAL}) :LOR: {CODESIZE}=32)
	Inject	"$instr.$stackType2 $baseReg, $regList"
	]
	[ narOK :LAND: (NStrict :LOR: ("$stackType1"="IA" :LAND: {UAL}) :LOR: {CODESIZE}=32)
	Inject	"$instr.$stackType2..N $baseReg, $regList"
	]
	[ wideOK :LAND: (NStrict :LOR: (("$stackType1"="IA" :LOR: "$stackType1"="DB") :LAND: {UAL}) :LOR: {CODESIZE}=32)
	Inject	"$instr.$stackType2..W $baseReg, $regList"
	]

	; 2. Iterate over all <cc> possibilities:
	LCLS	cc
	LCLA	cnt
	WHILE	cnt < 17
	; 17 condition codes to be tested:
	;   - Condition code NV is left out from this test.
	;   - HS and LO are equivalents for CS and CC.
cc	SETS	"EQNECSCCMIPLVSVCHILSGELTGTLEALHSLO" :RIGHT: (34 - 2*cnt) :LEFT: 2
	[ ({CODESIZE}=32 :LOR: {UAL}) :LAND: ((cnt < 14 :LAND: (cnt :MOD: 2)=0) :LOR: cnt=15)	; FIXME: REMOVE !
	;Inject "ITTTT	$cc"
	]
	; In CODE16 syntax mode, we only test IA stackmode.
	; No THUMB syntax mode as this is pre-UAL.
	; In ARM syntax mode, we test all 4 stack modes.
	; 2a. Pre-UAL:
	[ NStrict :LOR: {CODESIZE}=32 :LOR: :LNOT:{UAL}
	[ {TRUE} :LAND: (NStrict :LOR: ("$stackType1"="IA" :LAND: {CODESIZE}=16) :LOR: ("$stackType1"="DB" :LAND: {UAL}) :LOR: {CODESIZE}=32)
	Inject	"$instr.$cc.$stackType1 $baseReg, $regList"
	]
	[ narOK :LAND: (NStrict :LOR: "$stackType1"="IA")
	Inject	"$instr.$cc.$stackType1..N $baseReg, $regList"
	]
	[ wideOK :LAND: (NStrict :LOR: {CODESIZE}=32 :LOR: "$stackType1"="IA" :LOR: "$stackType1"="DB")
	Inject	"$instr.$cc.$stackType1..W $baseReg, $regList"
	]

	; We only test $stackType2 when in ARM syntax mode (no THUMB syntax mode, as this is pre-UAL).
	[ wideOK :LAND: (NStrict :LOR: (("$stackType1"="IA" :LOR: "$stackType1"="DB") :LAND: {UAL}) :LOR: {CODESIZE}=32)
	Inject	"$instr.$cc.$stackType2 $baseReg, $regList"
	]
	[ narOK :LAND: (NStrict :LOR: ("$stackType1"="IA" :LAND: {UAL}) :LOR: {CODESIZE}=32)
	Inject	"$instr.$cc.$stackType2..N $baseReg, $regList"
	]
	[ wideOK :LAND: (NStrict :LOR: (("$stackType1"="IA" :LOR: "$stackType1"="DB") :LAND: {UAL}) :LOR: {CODESIZE}=32)
	Inject	"$instr.$cc.$stackType2..W $baseReg, $regList"
	]
	]	; Pre-UAL check

	; 2b. UAL:
	[ NStrict :LOR: {CODESIZE}=32 :LOR: {UAL}
	; We only test $stackType + $cc in ARM or THUMB syntax mode.
	[ {TRUE} :LAND: (NStrict :LOR: ("$stackType1"="IA" :LAND: {CODESIZE}=16) :LOR: ("$stackType1"="DB" :LAND: {UAL}) :LOR: {CODESIZE}=32)
	Inject	"$instr.$stackType1.$cc $baseReg, $regList"
	]
	[ narOK :LAND: (NStrict :LOR: "$stackType1"="IA")
	Inject	"$instr.$stackType1.$cc..N $baseReg, $regList"
	]
	[ wideOK :LAND: (NStrict :LOR: {CODESIZE}=32 :LOR: "$stackType1"="IA" :LOR: "$stackType1"="DB")
	Inject	"$instr.$stackType1.$cc..W $baseReg, $regList"
	]

	; We only test $stackType2 when in ARM or THUMB syntax mode.
	[ wideOK :LAND: (NStrict :LOR: (("$stackType1"="IA" :LOR: "$stackType1"="DB") :LAND: {UAL}) :LOR: {CODESIZE}=32)
	Inject	"$instr.$stackType2.$cc $baseReg, $regList"
	]
	[ narOK :LAND: (NStrict :LOR: ("$stackType1"="IA" :LAND: {UAL}) :LOR: {CODESIZE}=32)
	Inject	"$instr.$stackType2.$cc..N $baseReg, $regList"
	]
	[ wideOK :LAND: (NStrict :LOR: (("$stackType1"="IA" :LOR: "$stackType1"="DB") :LAND: {UAL}) :LOR: {CODESIZE}=32)
	Inject	"$instr.$stackType2.$cc..W $baseReg, $regList"
	]
	]
cnt	SETA	cnt + 1
	WEND
	MEND

	; Do all syntax combinations:
	MACRO
	InvokeFullSyntax $baseReg, $regList
	; Test without stack type:
	InvokeCC LDM, $baseReg, $regList
	InvokeCC STM, $baseReg, $regList

	; LDM: DA=FA, DB=EA, IA=FD, IB=ED
	InvokeCCnStackType LDM, IA, FD, $baseReg, $regList
	InvokeCCnStackType LDM, IB, ED, $baseReg, $regList
	InvokeCCnStackType LDM, DA, FA, $baseReg, $regList
	InvokeCCnStackType LDM, DB, EA, $baseReg, $regList

	; STM: DA=ED, DB=FD, IA=EA, IB=FA
	InvokeCCnStackType STM, IA, EA, $baseReg, $regList
	InvokeCCnStackType STM, IB, FA, $baseReg, $regList
	InvokeCCnStackType STM, DA, ED, $baseReg, $regList
	InvokeCCnStackType STM, DB, FD, $baseReg, $regList
	MEND

RListA	RLIST {r1, r14}
RListT1	RLIST {r0, r4, r7}
RListT2	RLIST {r1, r3, r6}
	[ TestARM
	ARM
	InvokeFullSyntax r9, RListA
	]
	[ TestCODE16
	CODE16
	InvokeFullSyntax r3!, RListT1
	]
	[ TestTHUMB
	THUMB
	InvokeFullSyntax r2!, RListT2
	]

	|

	[ TestARM
	ARM
	; Test:
	;  - LDM/STM + [<.W>]
	;  - LDM/STM + <cc> + [<.W>]
	MACRO
	InvokeCC $instVal, $stackTypeVal
	DCI	$instVal + $stackTypeVal + &E0000000
	DCI	$instVal + $stackTypeVal + &E0000000

	; Maps <cc> codes 0 - 0xe + 2 (HS/CS) + 3 (LO/CC).
	LCLS	map
map	SETS	"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x02\x03"
	LCLA	cc
	WHILE	cc < 17
	DCI	$instVal + $stackTypeVal + (map:RIGHT:(17-cc):LEFT:1):SHL:28
	DCI	$instVal + $stackTypeVal + (map:RIGHT:(17-cc):LEFT:1):SHL:28
cc	SETA	cc + 1
	WEND
	MEND

	; Test:
	;  - LDM/STM + <stack type> + [<.W>] (pre-UAL and UAL)
	;  - LDM/STM + <cc> + <stack type> + [<.W>] (pre-UAL only)
	;  - LDM/STM + <stack type> + <cc> + [<.W>] (UAL only)
	; For given LDM/STM instruction, $stackType1 is equivalent to $stackType2.
	MACRO
	InvokeCCnStackType $instVal, $stackTypeVal
	DCI	$instVal + $stackTypeVal + &E0000000
	DCI	$instVal + $stackTypeVal + &E0000000
	DCI	$instVal + $stackTypeVal + &E0000000
	DCI	$instVal + $stackTypeVal + &E0000000

	; Maps <cc> codes 0 - 0xe + 2 (HS/CS) + 3 (LO/CC).
	LCLS	map
map	SETS	"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x02\x03"
	LCLA	cc
	WHILE	cc < 17
	LCLA	i
	WHILE	i < 8
	DCI	$instVal + $stackTypeVal + (map:RIGHT:(17-cc):LEFT:1):SHL:28
i	SETA	i + 1
	WEND
cc	SETA	cc + 1
	WEND
	MEND

	; ARM:
	; LDM / LDM.W / LDM<cc> / LDM<cc>.W
	InvokeCC &08194002, &00800000			; IA
	; STM / STM.W / STM<cc> / STM<cc>.W
	InvokeCC &08094002, &00800000			; IA

	; LDM
	InvokeCCnStackType &08194002, &00800000		; IA
	InvokeCCnStackType &08194002, &01800000		; IB
	InvokeCCnStackType &08194002, 0			; DA
	InvokeCCnStackType &08194002, &01000000		; DB
	; STM
	InvokeCCnStackType &08094002, &00800000		; IA
	InvokeCCnStackType &08094002, &01800000		; IB
	InvokeCCnStackType &08094002, 0			; DA
	InvokeCCnStackType &08094002, &01000000		; DB
	]

	[ TestCODE16
	CODE16
	DCI &cb91		; ldmia		r3!, {r0, r4, r7}
	DCI &bf0c		; ite		eq
	DCI &cb91		; ldmiaeq	r3!, {r0, r4, r7}
	DCI &cb91		; ldmiane	r3!, {r0, r4, r7}
	DCI &bf2c		; ite		cs
	DCI &cb91		; ldmiacs	r3!, {r0, r4, r7}
	DCI &cb91		; ldmiacc	r3!, {r0, r4, r7}
	DCI &bf4c		; ite		mi
	DCI &cb91		; ldmiami	r3!, {r0, r4, r7}
	DCI &cb91		; ldmiapl	r3!, {r0, r4, r7}
	DCI &bf6c		; ite		vs
	DCI &cb91		; ldmiavs	r3!, {r0, r4, r7}
	DCI &cb91		; ldmiavc	r3!, {r0, r4, r7}
	DCI &bf8c		; ite		hi
	DCI &cb91		; ldmiahi	r3!, {r0, r4, r7}
	DCI &cb91		; ldmials	r3!, {r0, r4, r7}
	DCI &bfac		; ite		ge
	DCI &cb91		; ldmiage	r3!, {r0, r4, r7}
	DCI &cb91		; ldmialt	r3!, {r0, r4, r7}
	DCI &bfcc		; ite		gt
	DCI &cb91		; ldmiagt	r3!, {r0, r4, r7}
	DCI &cb91		; ldmiale	r3!, {r0, r4, r7}
	DCI &cb91		; ldmia		r3!, {r0, r4, r7}
	DCI &bf2c		; ite		cs
	DCI &cb91		; ldmiacs	r3!, {r0, r4, r7}
	DCI &cb91		; ldmiacc	r3!, {r0, r4, r7}
	DCI &c391		; stmia		r3!, {r0, r4, r7}
	DCI &bf0c		; ite		eq
	DCI &c391		; stmiaeq	r3!, {r0, r4, r7}
	DCI &c391		; stmiane	r3!, {r0, r4, r7}
	DCI &bf2c		; ite		cs
	DCI &c391		; stmiacs	r3!, {r0, r4, r7}
	DCI &c391		; stmiacc	r3!, {r0, r4, r7}
	DCI &bf4c		; ite		mi
	DCI &c391		; stmiami	r3!, {r0, r4, r7}
	DCI &c391		; stmiapl	r3!, {r0, r4, r7}
	DCI &bf6c		; ite		vs
	DCI &c391		; stmiavs	r3!, {r0, r4, r7}
	DCI &c391		; stmiavc	r3!, {r0, r4, r7}
	DCI &bf8c		; ite		hi
	DCI &c391		; stmiahi	r3!, {r0, r4, r7}
	DCI &c391		; stmials	r3!, {r0, r4, r7}
	DCI &bfac		; ite		ge
	DCI &c391		; stmiage	r3!, {r0, r4, r7}
	DCI &c391		; stmialt	r3!, {r0, r4, r7}
	DCI &bfcc		; ite		gt
	DCI &c391		; stmiagt	r3!, {r0, r4, r7}
	DCI &c391		; stmiale	r3!, {r0, r4, r7}
	DCI &c391		; stmia		r3!, {r0, r4, r7}
	DCI &bf2c		; ite		cs
	DCI &c391		; stmiacs	r3!, {r0, r4, r7}
	DCI &c391		; stmiacc	r3!, {r0, r4, r7}
	]

	[ TestTHUMB
	THUMB
	DCI.N &ca4a		; ldmia		r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmia		r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmia.w	r2!, {r1, r3, r6}
	DCI.N &bf03		; ittte		eq
	DCI.N &ca4a		; ldmiaeq	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiaeq	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiaeq.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiane	r2!, {r1, r3, r6}
	DCI.N &bf1c		; itt		ne
	DCI.N &ca4a		; ldmiane	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiane.w	r2!, {r1, r3, r6}
	DCI.N &bf23		; ittte		cs
	DCI.N &ca4a		; ldmiacs	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiacs	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiacs.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiacc	r2!, {r1, r3, r6}
	DCI.N &bf3c		; itt		cc
	DCI.N &ca4a		; ldmiacc	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiacc.w	r2!, {r1, r3, r6}
	DCI.N &bf43		; ittte		mi
	DCI.N &ca4a		; ldmiami	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiami	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiami.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiapl	r2!, {r1, r3, r6}
	DCI.N &bf5c		; itt		pl
	DCI.N &ca4a		; ldmiapl	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiapl.w	r2!, {r1, r3, r6}
	DCI.N &bf63		; ittte		vs
	DCI.N &ca4a		; ldmiavs	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiavs	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiavs.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiavc	r2!, {r1, r3, r6}
	DCI.N &bf7c		; itt		vc
	DCI.N &ca4a		; ldmiavc	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiavc.w	r2!, {r1, r3, r6}
	DCI.N &bf83		; ittte		hi
	DCI.N &ca4a		; ldmiahi	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiahi	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiahi.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmials	r2!, {r1, r3, r6}
	DCI.N &bf9c		; itt		ls
	DCI.N &ca4a		; ldmials	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmials.w	r2!, {r1, r3, r6}
	DCI.N &bfa3		; ittte		ge
	DCI.N &ca4a		; ldmiage	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiage	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiage.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmialt	r2!, {r1, r3, r6}
	DCI.N &bfbc		; itt		lt
	DCI.N &ca4a		; ldmialt	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmialt.w	r2!, {r1, r3, r6}
	DCI.N &bfc3		; ittte		gt
	DCI.N &ca4a		; ldmiagt	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiagt	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiagt.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiale	r2!, {r1, r3, r6}
	DCI.N &bfdc		; itt		le
	DCI.N &ca4a		; ldmiale	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiale.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmia		r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmia		r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmia.w	r2!, {r1, r3, r6}
	DCI.N &bf23		; ittte		cs
	DCI.N &ca4a		; ldmiacs	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiacs	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiacs.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiacc	r2!, {r1, r3, r6}
	DCI.N &bf3c		; itt		cc
	DCI.N &ca4a		; ldmiacc	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiacc.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmia		r2!, {r1, r3, r6}
	DCI.N &c24a		; stmia		r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmia.w	r2!, {r1, r3, r6}
	DCI.N &bf03		; ittte		eq
	DCI.N &c24a		; stmiaeq	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiaeq	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiaeq.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiane	r2!, {r1, r3, r6}
	DCI.N &bf1c		; itt		ne
	DCI.N &c24a		; stmiane	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiane.w	r2!, {r1, r3, r6}
	DCI.N &bf23		; ittte		cs
	DCI.N &c24a		; stmiacs	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiacs	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiacs.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiacc	r2!, {r1, r3, r6}
	DCI.N &bf3c		; itt		cc
	DCI.N &c24a		; stmiacc	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiacc.w	r2!, {r1, r3, r6}
	DCI.N &bf43		; ittte		mi
	DCI.N &c24a		; stmiami	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiami	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiami.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiapl	r2!, {r1, r3, r6}
	DCI.N &bf5c		; itt		pl
	DCI.N &c24a		; stmiapl	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiapl.w	r2!, {r1, r3, r6}
	DCI.N &bf63		; ittte		vs
	DCI.N &c24a		; stmiavs	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiavs	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiavs.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiavc	r2!, {r1, r3, r6}
	DCI.N &bf7c		; itt		vc
	DCI.N &c24a		; stmiavc	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiavc.w	r2!, {r1, r3, r6}
	DCI.N &bf83		; ittte		hi
	DCI.N &c24a		; stmiahi	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiahi	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiahi.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmials	r2!, {r1, r3, r6}
	DCI.N &bf9c		; itt		ls
	DCI.N &c24a		; stmials	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmials.w	r2!, {r1, r3, r6}
	DCI.N &bfa3		; ittte		ge
	DCI.N &c24a		; stmiage	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiage	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiage.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmialt	r2!, {r1, r3, r6}
	DCI.N &bfbc		; itt		lt
	DCI.N &c24a		; stmialt	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmialt.w	r2!, {r1, r3, r6}
	DCI.N &bfc3		; ittte		gt
	DCI.N &c24a		; stmiagt	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiagt	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiagt.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiale	r2!, {r1, r3, r6}
	DCI.N &bfdc		; itt		le
	DCI.N &c24a		; stmiale	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiale.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmia		r2!, {r1, r3, r6}
	DCI.N &c24a		; stmia		r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmia.w	r2!, {r1, r3, r6}
	DCI.N &bf23		; ittte		cs
	DCI.N &c24a		; stmiacs	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiacs	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiacs.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiacc	r2!, {r1, r3, r6}
	DCI.N &bf3c		; itt		cc
	DCI.N &c24a		; stmiacc	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiacc.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmia		r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmia		r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmia.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmia		r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmia		r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmia.w	r2!, {r1, r3, r6}
	DCI.N &bf01		; itttt		eq
	DCI.N &ca4a		; ldmiaeq	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiaeq	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiaeq.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiaeq	r2!, {r1, r3, r6}
	DCI.N &bf07		; ittee		eq
	DCI.N &ca4a		; ldmiaeq	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiaeq.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiane	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiane	r2!, {r1, r3, r6}
	DCI.N &bf1f		; itttt		ne
	DCI.W &e8b2004a		; ldmiane.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiane	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiane	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiane.w	r2!, {r1, r3, r6}
	DCI.N &bf21		; itttt		cs
	DCI.N &ca4a		; ldmiacs	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiacs	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiacs.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiacs	r2!, {r1, r3, r6}
	DCI.N &bf27		; ittee		cs
	DCI.N &ca4a		; ldmiacs	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiacs.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiacc	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiacc	r2!, {r1, r3, r6}
	DCI.N &bf3f		; itttt		cc
	DCI.W &e8b2004a		; ldmiacc.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiacc	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiacc	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiacc.w	r2!, {r1, r3, r6}
	DCI.N &bf41		; itttt		mi
	DCI.N &ca4a		; ldmiami	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiami	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiami.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiami	r2!, {r1, r3, r6}
	DCI.N &bf47		; ittee		mi
	DCI.N &ca4a		; ldmiami	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiami.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiapl	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiapl	r2!, {r1, r3, r6}
	DCI.N &bf5f		; itttt		pl
	DCI.W &e8b2004a		; ldmiapl.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiapl	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiapl	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiapl.w	r2!, {r1, r3, r6}
	DCI.N &bf61		; itttt		vs
	DCI.N &ca4a		; ldmiavs	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiavs	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiavs.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiavs	r2!, {r1, r3, r6}
	DCI.N &bf67		; ittee		vs
	DCI.N &ca4a		; ldmiavs	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiavs.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiavc	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiavc	r2!, {r1, r3, r6}
	DCI.N &bf7f		; itttt		vc
	DCI.W &e8b2004a		; ldmiavc.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiavc	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiavc	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiavc.w	r2!, {r1, r3, r6}
	DCI.N &bf81		; itttt		hi
	DCI.N &ca4a		; ldmiahi	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiahi	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiahi.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiahi	r2!, {r1, r3, r6}
	DCI.N &bf87		; ittee		hi
	DCI.N &ca4a		; ldmiahi	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiahi.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmials	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmials	r2!, {r1, r3, r6}
	DCI.N &bf9f		; itttt		ls
	DCI.W &e8b2004a		; ldmials.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmials	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmials	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmials.w	r2!, {r1, r3, r6}
	DCI.N &bfa1		; itttt		ge
	DCI.N &ca4a		; ldmiage	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiage	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiage.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiage	r2!, {r1, r3, r6}
	DCI.N &bfa7		; ittee		ge
	DCI.N &ca4a		; ldmiage	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiage.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmialt	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmialt	r2!, {r1, r3, r6}
	DCI.N &bfbf		; itttt		lt
	DCI.W &e8b2004a		; ldmialt.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmialt	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmialt	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmialt.w	r2!, {r1, r3, r6}
	DCI.N &bfc1		; itttt		gt
	DCI.N &ca4a		; ldmiagt	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiagt	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiagt.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiagt	r2!, {r1, r3, r6}
	DCI.N &bfc7		; ittee		gt
	DCI.N &ca4a		; ldmiagt	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiagt.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiale	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiale	r2!, {r1, r3, r6}
	DCI.N &bfdf		; itttt		le
	DCI.W &e8b2004a		; ldmiale.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiale	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiale	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiale.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmia		r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmia		r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmia.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmia		r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmia		r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmia.w	r2!, {r1, r3, r6}
	DCI.N &bf21		; itttt		cs
	DCI.N &ca4a		; ldmiacs	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiacs	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiacs.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiacs	r2!, {r1, r3, r6}
	DCI.N &bf27		; ittee		cs
	DCI.N &ca4a		; ldmiacs	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiacs.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiacc	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiacc	r2!, {r1, r3, r6}
	DCI.N &bf3f		; itttt		cc
	DCI.W &e8b2004a		; ldmiacc.w	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiacc	r2!, {r1, r3, r6}
	DCI.N &ca4a		; ldmiacc	r2!, {r1, r3, r6}
	DCI.W &e8b2004a		; ldmiacc.w	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdb		r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdb		r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdb		r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdb		r2!, {r1, r3, r6}
	DCI.N &bf01		; itttt		eq
	DCI.W &e932004a		; ldmdbeq	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbeq	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbeq	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbeq	r2!, {r1, r3, r6}
	DCI.N &bf1f		; itttt		ne
	DCI.W &e932004a		; ldmdbne	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbne	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbne	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbne	r2!, {r1, r3, r6}
	DCI.N &bf21		; itttt		cs
	DCI.W &e932004a		; ldmdbcs	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbcs	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbcs	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbcs	r2!, {r1, r3, r6}
	DCI.N &bf3f		; itttt		cc
	DCI.W &e932004a		; ldmdbcc	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbcc	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbcc	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbcc	r2!, {r1, r3, r6}
	DCI.N &bf41		; itttt		mi
	DCI.W &e932004a		; ldmdbmi	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbmi	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbmi	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbmi	r2!, {r1, r3, r6}
	DCI.N &bf5f		; itttt		pl
	DCI.W &e932004a		; ldmdbpl	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbpl	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbpl	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbpl	r2!, {r1, r3, r6}
	DCI.N &bf61		; itttt		vs
	DCI.W &e932004a		; ldmdbvs	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbvs	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbvs	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbvs	r2!, {r1, r3, r6}
	DCI.N &bf7f		; itttt		vc
	DCI.W &e932004a		; ldmdbvc	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbvc	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbvc	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbvc	r2!, {r1, r3, r6}
	DCI.N &bf81		; itttt		hi
	DCI.W &e932004a		; ldmdbhi	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbhi	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbhi	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbhi	r2!, {r1, r3, r6}
	DCI.N &bf9f		; itttt		ls
	DCI.W &e932004a		; ldmdbls	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbls	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbls	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbls	r2!, {r1, r3, r6}
	DCI.N &bfa1		; itttt		ge
	DCI.W &e932004a		; ldmdbge	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbge	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbge	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbge	r2!, {r1, r3, r6}
	DCI.N &bfbf		; itttt		lt
	DCI.W &e932004a		; ldmdblt	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdblt	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdblt	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdblt	r2!, {r1, r3, r6}
	DCI.N &bfc1		; itttt		gt
	DCI.W &e932004a		; ldmdbgt	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbgt	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbgt	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbgt	r2!, {r1, r3, r6}
	DCI.N &bfdf		; itttt		le
	DCI.W &e932004a		; ldmdble	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdble	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdble	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdble	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdb		r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdb		r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdb		r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdb		r2!, {r1, r3, r6}
	DCI.N &bf21		; itttt		cs
	DCI.W &e932004a		; ldmdbcs	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbcs	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbcs	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbcs	r2!, {r1, r3, r6}
	DCI.N &bf3f		; itttt		cc
	DCI.W &e932004a		; ldmdbcc	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbcc	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbcc	r2!, {r1, r3, r6}
	DCI.W &e932004a		; ldmdbcc	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmia		r2!, {r1, r3, r6}
	DCI.N &c24a		; stmia		r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmia.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmia		r2!, {r1, r3, r6}
	DCI.N &c24a		; stmia		r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmia.w	r2!, {r1, r3, r6}
	DCI.N &bf01		; itttt		eq
	DCI.N &c24a		; stmiaeq	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiaeq	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiaeq.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiaeq	r2!, {r1, r3, r6}
	DCI.N &bf07		; ittee		eq
	DCI.N &c24a		; stmiaeq	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiaeq.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiane	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiane	r2!, {r1, r3, r6}
	DCI.N &bf1f		; itttt		ne
	DCI.W &e8a2004a		; stmiane.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiane	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiane	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiane.w	r2!, {r1, r3, r6}
	DCI.N &bf21		; itttt		cs
	DCI.N &c24a		; stmiacs	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiacs	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiacs.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiacs	r2!, {r1, r3, r6}
	DCI.N &bf27		; ittee		cs
	DCI.N &c24a		; stmiacs	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiacs.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiacc	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiacc	r2!, {r1, r3, r6}
	DCI.N &bf3f		; itttt		cc
	DCI.W &e8a2004a		; stmiacc.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiacc	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiacc	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiacc.w	r2!, {r1, r3, r6}
	DCI.N &bf41		; itttt		mi
	DCI.N &c24a		; stmiami	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiami	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiami.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiami	r2!, {r1, r3, r6}
	DCI.N &bf47		; ittee		mi
	DCI.N &c24a		; stmiami	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiami.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiapl	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiapl	r2!, {r1, r3, r6}
	DCI.N &bf5f		; itttt		pl
	DCI.W &e8a2004a		; stmiapl.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiapl	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiapl	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiapl.w	r2!, {r1, r3, r6}
	DCI.N &bf61		; itttt		vs
	DCI.N &c24a		; stmiavs	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiavs	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiavs.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiavs	r2!, {r1, r3, r6}
	DCI.N &bf67		; ittee		vs
	DCI.N &c24a		; stmiavs	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiavs.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiavc	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiavc	r2!, {r1, r3, r6}
	DCI.N &bf7f		; itttt		vc
	DCI.W &e8a2004a		; stmiavc.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiavc	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiavc	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiavc.w	r2!, {r1, r3, r6}
	DCI.N &bf81		; itttt		hi
	DCI.N &c24a		; stmiahi	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiahi	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiahi.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiahi	r2!, {r1, r3, r6}
	DCI.N &bf87		; ittee		hi
	DCI.N &c24a		; stmiahi	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiahi.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmials	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmials	r2!, {r1, r3, r6}
	DCI.N &bf9f		; itttt		ls
	DCI.W &e8a2004a		; stmials.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmials	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmials	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmials.w	r2!, {r1, r3, r6}
	DCI.N &bfa1		; itttt		ge
	DCI.N &c24a		; stmiage	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiage	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiage.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiage	r2!, {r1, r3, r6}
	DCI.N &bfa7		; ittee		ge
	DCI.N &c24a		; stmiage	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiage.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmialt	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmialt	r2!, {r1, r3, r6}
	DCI.N &bfbf		; itttt		lt
	DCI.W &e8a2004a		; stmialt.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmialt	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmialt	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmialt.w	r2!, {r1, r3, r6}
	DCI.N &bfc1		; itttt		gt
	DCI.N &c24a		; stmiagt	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiagt	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiagt.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiagt	r2!, {r1, r3, r6}
	DCI.N &bfc7		; ittee		gt
	DCI.N &c24a		; stmiagt	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiagt.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiale	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiale	r2!, {r1, r3, r6}
	DCI.N &bfdf		; itttt		le
	DCI.W &e8a2004a		; stmiale.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiale	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiale	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiale.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmia		r2!, {r1, r3, r6}
	DCI.N &c24a		; stmia		r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmia.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmia		r2!, {r1, r3, r6}
	DCI.N &c24a		; stmia		r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmia.w	r2!, {r1, r3, r6}
	DCI.N &bf21		; itttt		cs
	DCI.N &c24a		; stmiacs	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiacs	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiacs.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiacs	r2!, {r1, r3, r6}
	DCI.N &bf27		; ittee		cs
	DCI.N &c24a		; stmiacs	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiacs.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiacc	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiacc	r2!, {r1, r3, r6}
	DCI.N &bf3f		; itttt		cc
	DCI.W &e8a2004a		; stmiacc.w	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiacc	r2!, {r1, r3, r6}
	DCI.N &c24a		; stmiacc	r2!, {r1, r3, r6}
	DCI.W &e8a2004a		; stmiacc.w	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdb		r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdb		r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdb		r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdb		r2!, {r1, r3, r6}
	DCI.N &bf01		; itttt		eq
	DCI.W &e922004a		; stmdbeq	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbeq	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbeq	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbeq	r2!, {r1, r3, r6}
	DCI.N &bf1f		; itttt		ne
	DCI.W &e922004a		; stmdbne	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbne	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbne	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbne	r2!, {r1, r3, r6}
	DCI.N &bf21		; itttt		cs
	DCI.W &e922004a		; stmdbcs	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbcs	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbcs	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbcs	r2!, {r1, r3, r6}
	DCI.N &bf3f		; itttt		cc
	DCI.W &e922004a		; stmdbcc	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbcc	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbcc	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbcc	r2!, {r1, r3, r6}
	DCI.N &bf41		; itttt		mi
	DCI.W &e922004a		; stmdbmi	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbmi	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbmi	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbmi	r2!, {r1, r3, r6}
	DCI.N &bf5f		; itttt		pl
	DCI.W &e922004a		; stmdbpl	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbpl	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbpl	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbpl	r2!, {r1, r3, r6}
	DCI.N &bf61		; itttt		vs
	DCI.W &e922004a		; stmdbvs	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbvs	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbvs	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbvs	r2!, {r1, r3, r6}
	DCI.N &bf7f		; itttt		vc
	DCI.W &e922004a		; stmdbvc	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbvc	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbvc	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbvc	r2!, {r1, r3, r6}
	DCI.N &bf81		; itttt		hi
	DCI.W &e922004a		; stmdbhi	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbhi	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbhi	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbhi	r2!, {r1, r3, r6}
	DCI.N &bf9f		; itttt		ls
	DCI.W &e922004a		; stmdbls	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbls	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbls	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbls	r2!, {r1, r3, r6}
	DCI.N &bfa1		; itttt		ge
	DCI.W &e922004a		; stmdbge	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbge	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbge	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbge	r2!, {r1, r3, r6}
	DCI.N &bfbf		; itttt		lt
	DCI.W &e922004a		; stmdblt	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdblt	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdblt	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdblt	r2!, {r1, r3, r6}
	DCI.N &bfc1		; itttt		gt
	DCI.W &e922004a		; stmdbgt	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbgt	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbgt	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbgt	r2!, {r1, r3, r6}
	DCI.N &bfdf		; itttt		le
	DCI.W &e922004a		; stmdble	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdble	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdble	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdble	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdb		r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdb		r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdb		r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdb		r2!, {r1, r3, r6}
	DCI.N &bf21		; itttt		cs
	DCI.W &e922004a		; stmdbcs	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbcs	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbcs	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbcs	r2!, {r1, r3, r6}
	DCI.N &bf3f		; itttt		cc
	DCI.W &e922004a		; stmdbcc	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbcc	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbcc	r2!, {r1, r3, r6}
	DCI.W &e922004a		; stmdbcc	r2!, {r1, r3, r6}
	]

	]

	; POP/PUSH tests:
	[ TestPP
	[ :LNOT: REFERENCE
	MACRO
	InvokePP
	PUSH	{r1}		; becomes str r1, [r13, #-4]! (ARM only)
	PUSH	{r9}		; becomes str r9, [r13, #-4]! (Thumb2, ARM)
	[ {CODESIZE}=32 :LOR: {UAL}
	PUSH.W	{r9}		; becomes str r9, [r13, #-4]! (Thumb2, ARM)
	]
	PUSH	{r1,r7,lr}
	PUSH	{r1,r7,r10,lr}
	PUSH	{lr}		; becomes str lr, [r13, #-4]! (ARM only)

	POP	{r1}		; becomes ldr r1, [r13], #4 (ARM only)
	POP	{r9}		; becomes ldr r9, [r13], #4 (Thumb2, ARM)
	[ {CODESIZE}=32 :LOR: {UAL}
	POP.W	{r9}		; becomes ldr r9, [r13], #4 (Thumb2, ARM)
	]
	POP	{r1,r7,pc}
	POP	{r1,r7,r10,pc}
	POP	{r1,r7,r10,lr}
	POP	{pc}		; becomes ldr pc, [r13], #4 (ARM only)
	MEND

	ARM
	InvokePP

	CODE16
	InvokePP

	THUMB
	InvokePP
	|
	ARM
	DCI &e52d1004		; push		{r1}	; (str r1, [sp, #-4]!)
	DCI &e52d9004		; push		{r9}	; (str r9, [sp, #-4]!)
	DCI &e52d9004		; push		{r9}	; (str r9, [sp, #-4]!)
	DCI &e92d4082		; push		{r1, r7, lr}
	DCI &e92d4482		; push		{r1, r7, sl, lr}
	DCI &e52de004		; push		{lr}	; (str lr, [sp, #-4]!)

	DCI &e49d1004		; pop		{r1}	; (ldr r1, [sp], #4)
	DCI &e49d9004		; pop		{r9}	; (ldr r9, [sp], #4)
	DCI &e49d9004		; pop		{r9}	; (ldr r9, [sp], #4)
	DCI &e8bd8082		; pop		{r1, r7, pc}
	DCI &e8bd8482		; pop		{r1, r7, sl, pc}
	DCI &e8bd4482		; pop		{r1, r7, sl, lr}
	DCI &e49df004		; pop		{pc}	; (ldr pc, [sp], #4)

	THUMB
	DCI.N &b402		; push		{r1}
	DCI.W &f84d9d04		; str.w		r9, [sp, #-4]!
	DCI.N &b582		; push		{r1, r7, lr}
	DCI.W &e92d4482		; stmdb		sp!, {r1, r7, sl, lr}
	DCI.N &b500		; push		{lr}

	DCI.N &bc02		; pop		{r1}
	DCI.W &f85d9b04		; ldr.w		r9, [sp], #4
	DCI.N &bd82		; pop		{r1, r7, pc}
	DCI.W &e8bd8482		; ldmia.w	sp!, {r1, r7, sl, pc}
	DCI.W &e8bd4482		; ldmia.w	sp!, {r1, r7, sl, lr}
	DCI.N &bd00		; pop		{pc}

	THUMB
	DCI.N &b402		; push		{r1}
	DCI.W &f84d9d04		; str.w		r9, [sp, #-4]!
	DCI.W &f84d9d04		; str.w		r9, [sp, #-4]!
	DCI.N &b582		; push		{r1, r7, lr}
	DCI.W &e92d4482		; stmdb		sp!, {r1, r7, sl, lr}
	DCI.N &b500		; push		{lr}

	DCI.N &bc02		; pop		{r1}
	DCI.W &f85d9b04		; ldr.w		r9, [sp], #4
	DCI.W &f85d9b04		; ldr.w		r9, [sp], #4
	DCI.N &bd82		; pop		{r1, r7, pc}
	DCI.W &e8bd8482		; ldmia.w	sp!, {r1, r7, sl, pc}
	DCI.W &e8bd4482		; ldmia.w	sp!, {r1, r7, sl, lr}
	DCI.N &bd00		; pop		{pc}
	]
	] ; TestPP

	; Thumb2 LDM/STM may not have one register in register list.
	; Test its substitution by Thumb1 LDM/STM or LDR/STR.
	[ Test1R
	[ :LNOT: REFERENCE
	THUMB
	LDMIA	r3, {r2}
	LDMIA.W	r3, {r2}
	LDMIA	r3!, {r2}
	LDMIA.W	r3!, {r2}
	LDMIA	r9, {r2}
	LDMIA.W	r9, {r2}
	LDMIA	r9!, {r2}
	LDMIA.W	r9!, {r2}
	LDMIA	r3, {r9}
	LDMIA.W	r3, {r9}
	LDMIA	r3!, {r9}
	LDMIA.W	r3!, {r9}

	LDMDB	r3, {r2}
	LDMDB.W	r3, {r2}
	LDMDB	r3!, {r2}
	LDMDB.W	r3!, {r2}
	LDMDB	r9, {r2}
	LDMDB.W	r9, {r2}
	LDMDB	r9!, {r2}
	LDMDB.W	r9!, {r2}
	LDMDB	r3, {r9}
	LDMDB.W	r3, {r9}
	LDMDB	r3!, {r9}
	LDMDB.W	r3!, {r9}

	STMIA	r3, {r2}
	STMIA.W	r3, {r2}
	STMIA	r3!, {r2}
	STMIA.W	r3!, {r2}
	STMIA	r9, {r2}
	STMIA.W	r9, {r2}
	STMIA	r9!, {r2}
	STMIA.W	r9!, {r2}
	STMIA	r3, {r9}
	STMIA.W	r3, {r9}
	STMIA	r3!, {r9}
	STMIA.W	r3!, {r9}

	STMDB	r3, {r2}
	STMDB.W	r3, {r2}
	STMDB	r3!, {r2}
	STMDB.W	r3!, {r2}
	STMDB	r9, {r2}
	STMDB.W	r9, {r2}
	STMDB	r9!, {r2}
	STMDB.W	r9!, {r2}
	STMDB	r3, {r9}
	STMDB.W	r3, {r9}
	STMDB	r3!, {r9}
	STMDB.W	r3!, {r9}
	|
	THUMB
	DCI.N &681a		; ldr	r2, [r3, #0]
	DCI.W &f8d32000		; ldr.w	r2, [r3]
	DCI.N &cb04		; ldmia	r3!, {r2}
	DCI.W &f8532b04		; ldr.w	r2, [r3], #4
	DCI.W &f8d92000		; ldr.w	r2, [r9]
	DCI.W &f8d92000		; ldr.w	r2, [r9]
	DCI.W &f8592b04 	; ldr.w	r2, [r9], #4
	DCI.W &f8592b04 	; ldr.w	r2, [r9], #4
	DCI.W &f8d39000		; ldr.w	r9, [r3]
	DCI.W &f8d39000		; ldr.w	r9, [r3]
	DCI.W &f8539b04 	; ldr.w	r9, [r3], #4
	DCI.W &f8539b04 	; ldr.w	r9, [r3], #4

	DCI.W &f8532c04 	; ldr.w	r2, [r3, #-4]
	DCI.W &f8532c04 	; ldr.w	r2, [r3, #-4]
	DCI.W &f8532d04 	; ldr.w	r2, [r3, #-4]!
	DCI.W &f8532d04 	; ldr.w	r2, [r3, #-4]!
	DCI.W &f8592c04 	; ldr.w	r2, [r9, #-4]
	DCI.W &f8592c04 	; ldr.w	r2, [r9, #-4]
	DCI.W &f8592d04 	; ldr.w	r2, [r9, #-4]!
	DCI.W &f8592d04 	; ldr.w	r2, [r9, #-4]!
	DCI.W &f8539c04 	; ldr.w	r9, [r3, #-4]
	DCI.W &f8539c04 	; ldr.w	r9, [r3, #-4]
	DCI.W &f8539d04 	; ldr.w	r9, [r3, #-4]!
	DCI.W &f8539d04 	; ldr.w	r9, [r3, #-4]!

	DCI.N &601a		; str	r2, [r3, #0]
	DCI.W &f8c32000		; str.w	r2, [r3]
	DCI.N &c304		; stmia	r3!, {r2}
	DCI.W &f8432b04		; str.w	r2, [r3], #4
	DCI.W &f8c92000		; str.w	r2, [r9]
	DCI.W &f8c92000		; str.w	r2, [r9]
	DCI.W &f8492b04 	; str.w	r2, [r9], #4
	DCI.W &f8492b04 	; str.w	r2, [r9], #4
	DCI.W &f8c39000		; str.w	r9, [r3]
	DCI.W &f8c39000		; str.w	r9, [r3]
	DCI.W &f8439b04 	; str.w	r9, [r3], #4
	DCI.W &f8439b04 	; str.w	r9, [r3], #4

	DCI.W &f8432c04 	; str.w	r2, [r3, #-4]
	DCI.W &f8432c04 	; str.w	r2, [r3, #-4]
	DCI.W &f8432d04 	; str.w	r2, [r3, #-4]!
	DCI.W &f8432d04 	; str.w	r2, [r3, #-4]!
	DCI.W &f8492c04 	; str.w	r2, [r9, #-4]
	DCI.W &f8492c04 	; str.w	r2, [r9, #-4]
	DCI.W &f8492d04 	; str.w	r2, [r9, #-4]!
	DCI.W &f8492d04 	; str.w	r2, [r9, #-4]!
	DCI.W &f8439c04 	; str.w	r9, [r3, #-4]
	DCI.W &f8439c04 	; str.w	r9, [r3, #-4]
	DCI.W &f8439d04 	; str.w	r9, [r3, #-4]!
	DCI.W &f8439d04 	; str.w	r9, [r3, #-4]!
	]
	] ; Test1R

	; Writeback base register test:
	[ :LNOT: REFERENCE
	THUMB
	LDM	r3!, {r0, r4, r7}	; T1
	LDM	r3, {r0, r3-r4, r7}	; T1
	LDM	r3, {r0, r4, r7}	; T2

	LDM	r3!, {r0, r4, r7-r9}	; T2
	LDM	r3, {r0, r3-r4, r7-r9}	; T2

	STM	r3!, {r0, r3, r7}	; T1
	|
	THUMB
	DCI.N &cb91      	; ldmia	r3!, {r0, r4, r7}
	DCI.N &cb99      	; ldmia	r3, {r0, r3, r4, r7}
	DCI.W &e8930091 	; ldmia.w	r3, {r0, r4, r7}
	DCI.W &e8b30391 	; ldmia.w	r3!, {r0, r4, r7, r8, r9}
	DCI.W &e8930399 	; ldmia.w	r3, {r0, r3, r4, r7, r8, r9}
	DCI.N &c389      	; stmia	r3!, {r0, r3, r7}
	]

	; Test hat:
	[ :LNOT:REFERENCE
	ARM
	LDM	r3, {r0, r4, r7}^
	LDM	r3!, {r0, r4, r7, pc}^
	STM	r3, {r0, r4, r7}^
	|
	ARM
	DCI &e8d30091		; ldm	r3, {r0, r4, r7}^
	DCI &e8f38091		; ldm	r3!, {r0, r4, r7, pc}^
	DCI &e8c30091		; stm	r3, {r0, r4, r7}^
	]

	; Test syntax macro failback:
	[ :LNOT:REFERENCE
	ARM
	; Pre-UAL and UAL:
	LDMCCIA	r9, {r1}	; Is pre-UAL
	LDMIACC	r9, {r1}	; Is UAL
	LDMCC	r9, {r1}	; Is UAL
	LDM	r9, {r1}	; Is UAL

	CODE16
	; Pre-UAL only:
	LDMCCIA	r9, {r1}
	LDMIACC	r9, {r1}	; Should call LDM* macro
	LDMCC	r9, {r1}	; Should call LDM* macro
	LDM	r9, {r1}	; Should call LDM* macro

	THUMB
	; UAL only:
	LDMCCIA	r9, {r1}	; Should call LDM* macro
	LDMIACC	r9, {r1}
	LDMCC	r9, {r1}
	LDM	r9, {r1}

	|

	DCI &38990002		; ldmcc	r9, {r1}
	DCI &38990002		; ldmcc	r9, {r1}
	DCI &38990002		; ldmcc	r9, {r1}
	DCI &e8990002		; ldm	r9, {r1}

	THUMB
	DCI.N &bf38		; it	cc
	DCI.W &f8d91000		; ldrcc.w	r1, [r9]
	DCI.N &bf00		; nop
	DCI.N &bf00		; nop
	DCI.N &bf00		; nop

	DCI.N &bf00		; nop
	DCI.N &bf3c		; itt	cc
	DCI.W &f8d91000		; ldrcc.w	r1, [r9]
	DCI.W &f8d91000		; ldrcc.w	r1, [r9]
	DCI.W &f8d91000		; ldr.w	r1, [r9]
	]

	AREA Code2, CODE
	THUMB
	[ :LNOT: REFERENCE
	POPCC.W {r4, pc}	; Forces the end of the implicit IT block.
	SWICC 0

	LDMNE.W r3!, {r4, pc}	; Forces the end of the implicit IT block.
	SWINE 0

	LDMDBCC.W r3!, {r4, pc}	; Forces the end of the implicit IT block.
	SWICC 0
	|
	DCI.N &bf38	; it	cc
	DCI.W &e8bd8010	; ldmiacc.w	sp!, {r4, pc}
	DCI.N &bf38	; it	cc
	DCI.N &df00	; svccc	0

	DCI.N &bf18	; it	ne
	DCI.W &e8b38010	; ldmiane.w	r3!, {r4, pc}
	DCI.N &bf18	; it	ne
	DCI.N &df00	; svcne	0

	DCI.N &bf38	; it	cc
	DCI.W &e9338010	; ldmdbcc	r3!, {r4, pc}
	DCI.N &bf38	; it	cc
	DCI.N &df00	; svccc	0
	]

	END
