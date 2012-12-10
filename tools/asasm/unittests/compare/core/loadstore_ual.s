; Tests UAL parsing of LDR/STR
; RUNOPT: -CPU 7-A

	AREA	Code, CODE, HALFWORD

	[ :LNOT: REFERENCE
	; <LDR|STR> <type>
	; <LDR|STR> <cond code> <type>
	; <LDR|STR> <type> <cond code>
	MACRO
	Invoke $base, $type, $addr
	Inject "$base.$type $addr"
	LCLS	instr1
	LCLS	instr2
	LCLA	cnt
cnt	SETA	0
	WHILE	cnt < 17
	; 17 condition codes to be tested:
	;   - Condition code NV is left out from this test.
	;   - HS and LO are equivalents for CS and CC.
instr1	SETS	"$base" :CC: (("EQNECSCCMIPLVSVCHILSGELTGTLEALHSLO" :RIGHT: (34 - 2*cnt)) :LEFT: 2) :CC: "$type"	; Pre-UAL
instr2	SETS	"$base" :CC: "$type" :CC: (("EQNECSCCMIPLVSVCHILSGELTGTLEALHSLO" :RIGHT: (34 - 2*cnt)) :LEFT: 2)	; UAL
	Inject "$instr1 $addr"
cnt	SETA	cnt + 1
	WEND
	MEND

	MACRO
	Inject $full
	$full
	; INFO	0, "\t$full"
	MEND

	Invoke	LDR, "", "r1, [r2]"
	Invoke	LDR, "T", "r1, [r2]"
	Invoke	LDR, "B", "r1, [r2]"
	Invoke	LDR, "BT", "r1, [r2]"
	Invoke	LDR, "D", "r2, r3, [r4]"	; Explicitely test the two transfer register notation.
	Invoke	LDR, "H", "r1, [r2]"
	Invoke	LDR, "SB", "r1, [r2]"
	Invoke	LDR, "SH", "r1, [r2]"

	Invoke	STR, "", "r1, [r2]"
	Invoke	STR, "T", "r1, [r2]"
	Invoke	STR, "B", "r1, [r2]"
	Invoke	STR, "BT", "r1, [r2]"
	Invoke	STR, "D", "r2, r3, [r4]"	; Explicitely test the two transfer register notation.
	Invoke	STR, "H", "r1, [r2]"

	|

	DCI &e5921000	; LDR      r1,[r2]	; LDR, ""
	DCI &05921000	; LDREQ    r1,[r2]
	DCI &15921000	; LDRNE    r1,[r2]
	DCI &25921000	; LDRCS    r1,[r2]
	DCI &35921000	; LDRCC    r1,[r2]
	DCI &45921000	; LDRMI    r1,[r2]
	DCI &55921000	; LDRPL    r1,[r2]
	DCI &65921000	; LDRVS    r1,[r2]
	DCI &75921000	; LDRVC    r1,[r2]
	DCI &85921000	; LDRHI    r1,[r2]
	DCI &95921000	; LDRLS    r1,[r2]
	DCI &a5921000	; LDRGE    r1,[r2]
	DCI &b5921000	; LDRLT    r1,[r2]
	DCI &c5921000	; LDRGT    r1,[r2]
	DCI &d5921000	; LDRLE    r1,[r2]
	DCI &e5921000	; LDRAL    r1,[r2]
	DCI &25921000	; LDRCS    r1,[r2]
	DCI &35921000	; LDRCC    r1,[r2]

	DCI &e4b21000	; LDRT     r1,[r2]	; LDR, "T"
	DCI &04b21000	; LDRTEQ   r1,[r2]
	DCI &14b21000	; LDRTNE   r1,[r2]
	DCI &24b21000	; LDRTCS   r1,[r2]
	DCI &34b21000	; LDRTCC   r1,[r2]
	DCI &44b21000	; LDRTMI   r1,[r2]
	DCI &54b21000	; LDRTPL   r1,[r2]
	DCI &64b21000	; LDRTVS   r1,[r2]
	DCI &74b21000	; LDRTVC   r1,[r2]
	DCI &84b21000	; LDRTHI   r1,[r2]
	DCI &94b21000	; LDRTLS   r1,[r2]
	DCI &a4b21000	; LDRTGE   r1,[r2]
	DCI &b4b21000	; LDRTLT   r1,[r2]
	DCI &c4b21000	; LDRTGT   r1,[r2]
	DCI &d4b21000	; LDRTLE   r1,[r2]
	DCI &e4b21000	; LDRTAL   r1,[r2]
	DCI &24b21000	; LDRTCS   r1,[r2]
	DCI &34b21000	; LDRTCC   r1,[r2]

	DCI &e5d21000	; LDRB     r1,[r2]	; LDR, "B"
	DCI &05d21000	; LDRBEQ   r1,[r2]
	DCI &15d21000	; LDRBNE   r1,[r2]
	DCI &25d21000	; LDRBCS   r1,[r2]
	DCI &35d21000	; LDRBCC   r1,[r2]
	DCI &45d21000	; LDRBMI   r1,[r2]
	DCI &55d21000	; LDRBPL   r1,[r2]
	DCI &65d21000	; LDRBVS   r1,[r2]
	DCI &75d21000	; LDRBVC   r1,[r2]
	DCI &85d21000	; LDRBHI   r1,[r2]
	DCI &95d21000	; LDRBLS   r1,[r2]
	DCI &a5d21000	; LDRBGE   r1,[r2]
	DCI &b5d21000	; LDRBLT   r1,[r2]
	DCI &c5d21000	; LDRBGT   r1,[r2]
	DCI &d5d21000	; LDRBLE   r1,[r2]
	DCI &e5d21000	; LDRBAL   r1,[r2]
	DCI &25d21000	; LDRBCS   r1,[r2]
	DCI &35d21000	; LDRBCC   r1,[r2]

	DCI &e4f21000	; LDRBT    r1,[r2]	; LDR, "BT"
	DCI &04f21000	; LDRBTEQ  r1,[r2]
	DCI &14f21000	; LDRBTNE  r1,[r2]
	DCI &24f21000	; LDRBTCS  r1,[r2]
	DCI &34f21000	; LDRBTCC  r1,[r2]
	DCI &44f21000	; LDRBTMI  r1,[r2]
	DCI &54f21000	; LDRBTPL  r1,[r2]
	DCI &64f21000	; LDRBTVS  r1,[r2]
	DCI &74f21000	; LDRBTVC  r1,[r2]
	DCI &84f21000	; LDRBTHI  r1,[r2]
	DCI &94f21000	; LDRBTLS  r1,[r2]
	DCI &a4f21000	; LDRBTGE  r1,[r2]
	DCI &b4f21000	; LDRBTLT  r1,[r2]
	DCI &c4f21000	; LDRBTGT  r1,[r2]
	DCI &d4f21000	; LDRBTLE  r1,[r2]
	DCI &e4f21000	; LDRBTAL  r1,[r2]
	DCI &24f21000	; LDRBTCS  r1,[r2]
	DCI &34f21000	; LDRBTCC  r1,[r2]

	DCI &e1c420d0	; LDRD     r2,r3,[r4]	; LDR, "D"
	DCI &01c420d0	; LDRDEQ   r2,r3,[r4]
	DCI &11c420d0	; LDRDNE   r2,r3,[r4]
	DCI &21c420d0	; LDRDCS   r2,r3,[r4]
	DCI &31c420d0	; LDRDCC   r2,r3,[r4]
	DCI &41c420d0	; LDRDMI   r2,r3,[r4]
	DCI &51c420d0	; LDRDPL   r2,r3,[r4]
	DCI &61c420d0	; LDRDVS   r2,r3,[r4]
	DCI &71c420d0	; LDRDVC   r2,r3,[r4]
	DCI &81c420d0	; LDRDHI   r2,r3,[r4]
	DCI &91c420d0	; LDRDLS   r2,r3,[r4]
	DCI &a1c420d0	; LDRDGE   r2,r3,[r4]
	DCI &b1c420d0	; LDRDLT   r2,r3,[r4]
	DCI &c1c420d0	; LDRDGT   r2,r3,[r4]
	DCI &d1c420d0	; LDRDLE   r2,r3,[r4]
	DCI &e1c420d0	; LDRDAL   r2,r3,[r4]
	DCI &21c420d0	; LDRDCS   r2,r3,[r4]
	DCI &31c420d0	; LDRDCC   r2,r3,[r4]

	DCI &e1d210b0	; LDRH     r1,[r2]	; LDR, "H"
	DCI &01d210b0	; LDRHEQ   r1,[r2]
	DCI &11d210b0	; LDRHNE   r1,[r2]
	DCI &21d210b0	; LDRHCS   r1,[r2]
	DCI &31d210b0	; LDRHCC   r1,[r2]
	DCI &41d210b0	; LDRHMI   r1,[r2]
	DCI &51d210b0	; LDRHPL   r1,[r2]
	DCI &61d210b0	; LDRHVS   r1,[r2]
	DCI &71d210b0	; LDRHVC   r1,[r2]
	DCI &81d210b0	; LDRHHI   r1,[r2]
	DCI &91d210b0	; LDRHLS   r1,[r2]
	DCI &a1d210b0	; LDRHGE   r1,[r2]
	DCI &b1d210b0	; LDRHLT   r1,[r2]
	DCI &c1d210b0	; LDRHGT   r1,[r2]
	DCI &d1d210b0	; LDRHLE   r1,[r2]
	DCI &e1d210b0	; LDRHAL   r1,[r2]
	DCI &21d210b0	; LDRHCS   r1,[r2]
	DCI &31d210b0	; LDRHCC   r1,[r2]

	DCI &e1d210d0	; LDRSB    r1,[r2]	; LDR, "SB"
	DCI &01d210d0	; LDRSBEQ  r1,[r2]
	DCI &11d210d0	; LDRSBNE  r1,[r2]
	DCI &21d210d0	; LDRSBCS  r1,[r2]
	DCI &31d210d0	; LDRSBCC  r1,[r2]
	DCI &41d210d0	; LDRSBMI  r1,[r2]
	DCI &51d210d0	; LDRSBPL  r1,[r2]
	DCI &61d210d0	; LDRSBVS  r1,[r2]
	DCI &71d210d0	; LDRSBVC  r1,[r2]
	DCI &81d210d0	; LDRSBHI  r1,[r2]
	DCI &91d210d0	; LDRSBLS  r1,[r2]
	DCI &a1d210d0	; LDRSBGE  r1,[r2]
	DCI &b1d210d0	; LDRSBLT  r1,[r2]
	DCI &c1d210d0	; LDRSBGT  r1,[r2]
	DCI &d1d210d0	; LDRSBLE  r1,[r2]
	DCI &e1d210d0	; LDRSBAL  r1,[r2]
	DCI &21d210d0	; LDRSBCS  r1,[r2]
	DCI &31d210d0	; LDRSBCC  r1,[r2]

	DCI &e1d210f0	; LDRSH    r1,[r2]	; LDR, "SH"
	DCI &01d210f0	; LDRSHEQ  r1,[r2]
	DCI &11d210f0	; LDRSHNE  r1,[r2]
	DCI &21d210f0	; LDRSHCS  r1,[r2]
	DCI &31d210f0	; LDRSHCC  r1,[r2]
	DCI &41d210f0	; LDRSHMI  r1,[r2]
	DCI &51d210f0	; LDRSHPL  r1,[r2]
	DCI &61d210f0	; LDRSHVS  r1,[r2]
	DCI &71d210f0	; LDRSHVC  r1,[r2]
	DCI &81d210f0	; LDRSHHI  r1,[r2]
	DCI &91d210f0	; LDRSHLS  r1,[r2]
	DCI &a1d210f0	; LDRSHGE  r1,[r2]
	DCI &b1d210f0	; LDRSHLT  r1,[r2]
	DCI &c1d210f0	; LDRSHGT  r1,[r2]
	DCI &d1d210f0	; LDRSHLE  r1,[r2]
	DCI &e1d210f0	; LDRSHAL  r1,[r2]
	DCI &21d210f0	; LDRSHCS  r1,[r2]
	DCI &31d210f0	; LDRSHCC  r1,[r2]

	DCI &e5821000	; STR      r1,[r2]	; STR, ""
	DCI &05821000	; STREQ    r1,[r2]
	DCI &15821000	; STRNE    r1,[r2]
	DCI &25821000	; STRCS    r1,[r2]
	DCI &35821000	; STRCC    r1,[r2]
	DCI &45821000	; STRMI    r1,[r2]
	DCI &55821000	; STRPL    r1,[r2]
	DCI &65821000	; STRVS    r1,[r2]
	DCI &75821000	; STRVC    r1,[r2]
	DCI &85821000	; STRHI    r1,[r2]
	DCI &95821000	; STRLS    r1,[r2]
	DCI &a5821000	; STRGE    r1,[r2]
	DCI &b5821000	; STRLT    r1,[r2]
	DCI &c5821000	; STRGT    r1,[r2]
	DCI &d5821000	; STRLE    r1,[r2]
	DCI &e5821000	; STRAL    r1,[r2]
	DCI &25821000	; STRCS    r1,[r2]
	DCI &35821000	; STRCC    r1,[r2]

	DCI &e4a21000	; STRT     r1,[r2]	; STR, "T"
	DCI &04a21000	; STRTEQ   r1,[r2]
	DCI &14a21000	; STRTNE   r1,[r2]
	DCI &24a21000	; STRTCS   r1,[r2]
	DCI &34a21000	; STRTCC   r1,[r2]
	DCI &44a21000	; STRTMI   r1,[r2]
	DCI &54a21000	; STRTPL   r1,[r2]
	DCI &64a21000	; STRTVS   r1,[r2]
	DCI &74a21000	; STRTVC   r1,[r2]
	DCI &84a21000	; STRTHI   r1,[r2]
	DCI &94a21000	; STRTLS   r1,[r2]
	DCI &a4a21000	; STRTGE   r1,[r2]
	DCI &b4a21000	; STRTLT   r1,[r2]
	DCI &c4a21000	; STRTGT   r1,[r2]
	DCI &d4a21000	; STRTLE   r1,[r2]
	DCI &e4a21000	; STRTAL   r1,[r2]
	DCI &24a21000	; STRTCS   r1,[r2]
	DCI &34a21000	; STRTCC   r1,[r2]

	DCI &e5c21000	; STRB     r1,[r2]	; STR, "B"
	DCI &05c21000	; STRBEQ   r1,[r2]
	DCI &15c21000	; STRBNE   r1,[r2]
	DCI &25c21000	; STRBCS   r1,[r2]
	DCI &35c21000	; STRBCC   r1,[r2]
	DCI &45c21000	; STRBMI   r1,[r2]
	DCI &55c21000	; STRBPL   r1,[r2]
	DCI &65c21000	; STRBVS   r1,[r2]
	DCI &75c21000	; STRBVC   r1,[r2]
	DCI &85c21000	; STRBHI   r1,[r2]
	DCI &95c21000	; STRBLS   r1,[r2]
	DCI &a5c21000	; STRBGE   r1,[r2]
	DCI &b5c21000	; STRBLT   r1,[r2]
	DCI &c5c21000	; STRBGT   r1,[r2]
	DCI &d5c21000	; STRBLE   r1,[r2]
	DCI &e5c21000	; STRBAL   r1,[r2]
	DCI &25c21000	; STRBCS   r1,[r2]
	DCI &35c21000	; STRBCC   r1,[r2]

	DCI &e4e21000	; STRBT    r1,[r2]	; STR, "BT"
	DCI &04e21000	; STRBTEQ  r1,[r2]
	DCI &14e21000	; STRBTNE  r1,[r2]
	DCI &24e21000	; STRBTCS  r1,[r2]
	DCI &34e21000	; STRBTCC  r1,[r2]
	DCI &44e21000	; STRBTMI  r1,[r2]
	DCI &54e21000	; STRBTPL  r1,[r2]
	DCI &64e21000	; STRBTVS  r1,[r2]
	DCI &74e21000	; STRBTVC  r1,[r2]
	DCI &84e21000	; STRBTHI  r1,[r2]
	DCI &94e21000	; STRBTLS  r1,[r2]
	DCI &a4e21000	; STRBTGE  r1,[r2]
	DCI &b4e21000	; STRBTLT  r1,[r2]
	DCI &c4e21000	; STRBTGT  r1,[r2]
	DCI &d4e21000	; STRBTLE  r1,[r2]
	DCI &e4e21000	; STRBTAL  r1,[r2]
	DCI &24e21000	; STRBTCS  r1,[r2]
	DCI &34e21000	; STRBTCC  r1,[r2]

	DCI &e1c420f0	; STRD     r2,r3,[r4]	; STR, "D"
	DCI &01c420f0	; STRDEQ   r2,r3,[r4]
	DCI &11c420f0	; STRDNE   r2,r3,[r4]
	DCI &21c420f0	; STRDCS   r2,r3,[r4]
	DCI &31c420f0	; STRDCC   r2,r3,[r4]
	DCI &41c420f0	; STRDMI   r2,r3,[r4]
	DCI &51c420f0	; STRDPL   r2,r3,[r4]
	DCI &61c420f0	; STRDVS   r2,r3,[r4]
	DCI &71c420f0	; STRDVC   r2,r3,[r4]
	DCI &81c420f0	; STRDHI   r2,r3,[r4]
	DCI &91c420f0	; STRDLS   r2,r3,[r4]
	DCI &a1c420f0	; STRDGE   r2,r3,[r4]
	DCI &b1c420f0	; STRDLT   r2,r3,[r4]
	DCI &c1c420f0	; STRDGT   r2,r3,[r4]
	DCI &d1c420f0	; STRDLE   r2,r3,[r4]
	DCI &e1c420f0	; STRDAL   r2,r3,[r4]
	DCI &21c420f0	; STRDCS   r2,r3,[r4]
	DCI &31c420f0	; STRDCC   r2,r3,[r4]

	DCI &e1c210b0	; STRH     r1,[r2]	; STR, "H"
	DCI &01c210b0	; STRHEQ   r1,[r2]
	DCI &11c210b0	; STRHNE   r1,[r2]
	DCI &21c210b0	; STRHCS   r1,[r2]
	DCI &31c210b0	; STRHCC   r1,[r2]
	DCI &41c210b0	; STRHMI   r1,[r2]
	DCI &51c210b0	; STRHPL   r1,[r2]
	DCI &61c210b0	; STRHVS   r1,[r2]
	DCI &71c210b0	; STRHVC   r1,[r2]
	DCI &81c210b0	; STRHHI   r1,[r2]
	DCI &91c210b0	; STRHLS   r1,[r2]
	DCI &a1c210b0	; STRHGE   r1,[r2]
	DCI &b1c210b0	; STRHLT   r1,[r2]
	DCI &c1c210b0	; STRHGT   r1,[r2]
	DCI &d1c210b0	; STRHLE   r1,[r2]
	DCI &e1c210b0	; STRHAL   r1,[r2]
	DCI &21c210b0	; STRHCS   r1,[r2]
	DCI &31c210b0	; STRHCC   r1,[r2]

	]

	END
