; Tests SWP(B) instruction.

		AREA Code, CODE

	[ :LNOT: REFERENCE

		MACRO
		MkSWP	$p1, $p2
		LCLA	cc
		LCLS	ccs
ccs		SETS	"EQNECSCCMIPLVSVCHILSGELTGTLEALHSLO"
		LCLS	instr
		WHILE	cc <= :LEN:ccs
		[ cc = :LEN:ccs
instr		SETS	"SWP$p1$p2"
		|
instr		SETS	"SWP$p1" :CC: ((ccs:LEFT:(cc+2)):RIGHT:2) :CC: "$p2"
		]
		$instr	r2,r3,[r4]
cc		SETA	cc + 2
		WEND
		MEND

		MkSWP
		MkSWP	,B	; Pre-UAL syntax
		CODE32		; Allow both pre-UAL and UAL
		MkSWP	B	; UAL syntax
	|

		DCI &01042093 ; SWPEQ    r2,r3,[r4]
		DCI &11042093 ; SWPNE    r2,r3,[r4]
		DCI &21042093 ; SWPCS    r2,r3,[r4]
		DCI &31042093 ; SWPCC    r2,r3,[r4]
		DCI &41042093 ; SWPMI    r2,r3,[r4]
		DCI &51042093 ; SWPPL    r2,r3,[r4]
		DCI &61042093 ; SWPVS    r2,r3,[r4]
		DCI &71042093 ; SWPVC    r2,r3,[r4]
		DCI &81042093 ; SWPHI    r2,r3,[r4]
		DCI &91042093 ; SWPLS    r2,r3,[r4]
		DCI &a1042093 ; SWPGE    r2,r3,[r4]
		DCI &b1042093 ; SWPLT    r2,r3,[r4]
		DCI &c1042093 ; SWPGT    r2,r3,[r4]
		DCI &d1042093 ; SWPLE    r2,r3,[r4]
		DCI &e1042093 ; SWP      r2,r3,[r4]
		DCI &21042093 ; SWPCS    r2,r3,[r4]
		DCI &31042093 ; SWPCC    r2,r3,[r4]
		DCI &e1042093 ; SWP      r2,r3,[r4]

		DCI &01442093 ; SWPBEQ   r2,r3,[r4]
		DCI &11442093 ; SWPBNE   r2,r3,[r4]
		DCI &21442093 ; SWPBCS   r2,r3,[r4]
		DCI &31442093 ; SWPBCC   r2,r3,[r4]
		DCI &41442093 ; SWPBMI   r2,r3,[r4]
		DCI &51442093 ; SWPBPL   r2,r3,[r4]
		DCI &61442093 ; SWPBVS   r2,r3,[r4]
		DCI &71442093 ; SWPBVC   r2,r3,[r4]
		DCI &81442093 ; SWPBHI   r2,r3,[r4]
		DCI &91442093 ; SWPBLS   r2,r3,[r4]
		DCI &a1442093 ; SWPBGE   r2,r3,[r4]
		DCI &b1442093 ; SWPBLT   r2,r3,[r4]
		DCI &c1442093 ; SWPBGT   r2,r3,[r4]
		DCI &d1442093 ; SWPBLE   r2,r3,[r4]
		DCI &e1442093 ; SWPB     r2,r3,[r4]
		DCI &21442093 ; SWPBCS   r2,r3,[r4]
		DCI &31442093 ; SWPBCC   r2,r3,[r4]
		DCI &e1442093 ; SWPB     r2,r3,[r4]

		DCI &01442093 ; SWPBEQ   r2,r3,[r4]
		DCI &11442093 ; SWPBNE   r2,r3,[r4]
		DCI &21442093 ; SWPBCS   r2,r3,[r4]
		DCI &31442093 ; SWPBCC   r2,r3,[r4]
		DCI &41442093 ; SWPBMI   r2,r3,[r4]
		DCI &51442093 ; SWPBPL   r2,r3,[r4]
		DCI &61442093 ; SWPBVS   r2,r3,[r4]
		DCI &71442093 ; SWPBVC   r2,r3,[r4]
		DCI &81442093 ; SWPBHI   r2,r3,[r4]
		DCI &91442093 ; SWPBLS   r2,r3,[r4]
		DCI &a1442093 ; SWPBGE   r2,r3,[r4]
		DCI &b1442093 ; SWPBLT   r2,r3,[r4]
		DCI &c1442093 ; SWPBGT   r2,r3,[r4]
		DCI &d1442093 ; SWPBLE   r2,r3,[r4]
		DCI &e1442093 ; SWPB     r2,r3,[r4]
		DCI &21442093 ; SWPBCS   r2,r3,[r4]
		DCI &31442093 ; SWPBCC   r2,r3,[r4]
		DCI &e1442093 ; SWPB     r2,r3,[r4]

	]

		END
