; Test CBZ and CBNZ
; RUNOPT: -CPU=6T2

	AREA Code, CODE

	THUMB
	[ :LNOT: REFERENCE

	MACRO
	Inject $i
	;INFO 0, "\t$i"
	$i
	MEND

	MACRO
	Invoke $base
	LCLA idx
idx	SETA 0
	WHILE idx < 128
	Inject "$base r5  , {PC} + 4 + 0x$idx"
idx	SETA idx + 2
	WEND
	MEND

	Invoke CBZ
	Invoke CBNZ

	|

	DCI.N &b105
	DCI.N &b10d
	DCI.N &b115
	DCI.N &b11d
	DCI.N &b125
	DCI.N &b12d
	DCI.N &b135
	DCI.N &b13d
	DCI.N &b145
	DCI.N &b14d
	DCI.N &b155
	DCI.N &b15d
	DCI.N &b165
	DCI.N &b16d
	DCI.N &b175
	DCI.N &b17d
	DCI.N &b185
	DCI.N &b18d
	DCI.N &b195
	DCI.N &b19d
	DCI.N &b1a5
	DCI.N &b1ad
	DCI.N &b1b5
	DCI.N &b1bd
	DCI.N &b1c5
	DCI.N &b1cd
	DCI.N &b1d5
	DCI.N &b1dd
	DCI.N &b1e5
	DCI.N &b1ed
	DCI.N &b1f5
	DCI.N &b1fd
	DCI.N &b305
	DCI.N &b30d
	DCI.N &b315
	DCI.N &b31d
	DCI.N &b325
	DCI.N &b32d
	DCI.N &b335
	DCI.N &b33d
	DCI.N &b345
	DCI.N &b34d
	DCI.N &b355
	DCI.N &b35d
	DCI.N &b365
	DCI.N &b36d
	DCI.N &b375
	DCI.N &b37d
	DCI.N &b385
	DCI.N &b38d
	DCI.N &b395
	DCI.N &b39d
	DCI.N &b3a5
	DCI.N &b3ad
	DCI.N &b3b5
	DCI.N &b3bd
	DCI.N &b3c5
	DCI.N &b3cd
	DCI.N &b3d5
	DCI.N &b3dd
	DCI.N &b3e5
	DCI.N &b3ed
	DCI.N &b3f5
	DCI.N &b3fd

	DCI.N &b905
	DCI.N &b90d
	DCI.N &b915
	DCI.N &b91d
	DCI.N &b925
	DCI.N &b92d
	DCI.N &b935
	DCI.N &b93d
	DCI.N &b945
	DCI.N &b94d
	DCI.N &b955
	DCI.N &b95d
	DCI.N &b965
	DCI.N &b96d
	DCI.N &b975
	DCI.N &b97d
	DCI.N &b985
	DCI.N &b98d
	DCI.N &b995
	DCI.N &b99d
	DCI.N &b9a5
	DCI.N &b9ad
	DCI.N &b9b5
	DCI.N &b9bd
	DCI.N &b9c5
	DCI.N &b9cd
	DCI.N &b9d5
	DCI.N &b9dd
	DCI.N &b9e5
	DCI.N &b9ed
	DCI.N &b9f5
	DCI.N &b9fd
	DCI.N &bb05
	DCI.N &bb0d
	DCI.N &bb15
	DCI.N &bb1d
	DCI.N &bb25
	DCI.N &bb2d
	DCI.N &bb35
	DCI.N &bb3d
	DCI.N &bb45
	DCI.N &bb4d
	DCI.N &bb55
	DCI.N &bb5d
	DCI.N &bb65
	DCI.N &bb6d
	DCI.N &bb75
	DCI.N &bb7d
	DCI.N &bb85
	DCI.N &bb8d
	DCI.N &bb95
	DCI.N &bb9d
	DCI.N &bba5
	DCI.N &bbad
	DCI.N &bbb5
	DCI.N &bbbd
	DCI.N &bbc5
	DCI.N &bbcd
	DCI.N &bbd5
	DCI.N &bbdd
	DCI.N &bbe5
	DCI.N &bbed
	DCI.N &bbf5
	DCI.N &bbfd

	]

	; Test ELF relocation:
	[ OUTPUTTYPE == "ELF"
	AREA CodeELFReloc, CODE

	; IMPORT extSymbol_T01
	IMPORT extSymbol_T02

	[ :LNOT: REFERENCE
	CBZ r1, 40 + extSymbol_T01
	CBNZ r2, 80 + extSymbol_T02
	|
	DCI.N &b191	; cbz	r1, . + 4 + 40
	RELOC R_ARM_THM_JUMP6, extSymbol_T01
	DCI.N &bb32	; cbnz	r2, . + 4 + 80
	RELOC R_ARM_THM_JUMP6, extSymbol_T02
	]
	]

	END
