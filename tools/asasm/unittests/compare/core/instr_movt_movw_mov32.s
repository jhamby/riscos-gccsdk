; Tests MOVW, MOVT and MOV32
; RUNOPT: -cpu=6T2

	AREA	Code, CODE

	[ :LNOT:REFERENCE

	MACRO
	Expand	$instr, $numBitPos
	LCLA	bitPos
	WHILE	bitPos <> $numBitPos
	$instr	r9, #1 :SHL: bitPos
bitPos	SETA	bitPos + 1
	WEND
	MEND

	ARM
	Expand	MOVW,16
	Expand	MOVT,16
	Expand	MOV32,32

	THUMB
	Expand	MOVW,16
	Expand	MOVT,16
	Expand	MOV32,32

	|

	ARM
	; MOVW
	DCI.W &e3009001 	; movw	r9, #1
	DCI.W &e3009002 	; movw	r9, #2
	DCI.W &e3009004 	; movw	r9, #4
	DCI.W &e3009008 	; movw	r9, #8
	DCI.W &e3009010 	; movw	r9, #16
	DCI.W &e3009020 	; movw	r9, #32
	DCI.W &e3009040 	; movw	r9, #64		; 0x40
	DCI.W &e3009080 	; movw	r9, #128	; 0x80
	DCI.W &e3009100 	; movw	r9, #256	; 0x100
	DCI.W &e3009200 	; movw	r9, #512	; 0x200
	DCI.W &e3009400 	; movw	r9, #1024	; 0x400
	DCI.W &e3009800 	; movw	r9, #2048	; 0x800
	DCI.W &e3019000 	; movw	r9, #4096	; 0x1000
	DCI.W &e3029000 	; movw	r9, #8192	; 0x2000
	DCI.W &e3049000 	; movw	r9, #16384	; 0x4000
	DCI.W &e3089000 	; movw	r9, #32768	; 0x8000
	; MOVT
	DCI.W &e3409001 	; movt	r9, #1
	DCI.W &e3409002 	; movt	r9, #2
	DCI.W &e3409004 	; movt	r9, #4
	DCI.W &e3409008 	; movt	r9, #8
	DCI.W &e3409010 	; movt	r9, #16
	DCI.W &e3409020 	; movt	r9, #32
	DCI.W &e3409040 	; movt	r9, #64		; 0x40
	DCI.W &e3409080 	; movt	r9, #128	; 0x80
	DCI.W &e3409100 	; movt	r9, #256	; 0x100
	DCI.W &e3409200 	; movt	r9, #512	; 0x200
	DCI.W &e3409400 	; movt	r9, #1024	; 0x400
	DCI.W &e3409800 	; movt	r9, #2048	; 0x800
	DCI.W &e3419000 	; movt	r9, #4096	; 0x1000
	DCI.W &e3429000 	; movt	r9, #8192	; 0x2000
	DCI.W &e3449000 	; movt	r9, #16384	; 0x4000
	DCI.W &e3489000 	; movt	r9, #32768	; 0x8000
	; MOV32
	DCI.W &e3009001 	; movw	r9, #1
	DCI.W &e3409000 	; movt	r9, #0
	DCI.W &e3009002 	; movw	r9, #2
	DCI.W &e3409000 	; movt	r9, #0
	DCI.W &e3009004 	; movw	r9, #4
	DCI.W &e3409000 	; movt	r9, #0
	DCI.W &e3009008 	; movw	r9, #8
	DCI.W &e3409000 	; movt	r9, #0
	DCI.W &e3009010 	; movw	r9, #16
	DCI.W &e3409000 	; movt	r9, #0
	DCI.W &e3009020 	; movw	r9, #32
	DCI.W &e3409000 	; movt	r9, #0
	DCI.W &e3009040 	; movw	r9, #64		; 0x40
	DCI.W &e3409000 	; movt	r9, #0
	DCI.W &e3009080 	; movw	r9, #128	; 0x80
	DCI.W &e3409000 	; movt	r9, #0
	DCI.W &e3009100 	; movw	r9, #256	; 0x100
	DCI.W &e3409000 	; movt	r9, #0
	DCI.W &e3009200 	; movw	r9, #512	; 0x200
	DCI.W &e3409000 	; movt	r9, #0
	DCI.W &e3009400 	; movw	r9, #1024	; 0x400
	DCI.W &e3409000 	; movt	r9, #0
	DCI.W &e3009800 	; movw	r9, #2048	; 0x800
	DCI.W &e3409000 	; movt	r9, #0
	DCI.W &e3019000 	; movw	r9, #4096	; 0x1000
	DCI.W &e3409000 	; movt	r9, #0
	DCI.W &e3029000 	; movw	r9, #8192	; 0x2000
	DCI.W &e3409000 	; movt	r9, #0
	DCI.W &e3049000 	; movw	r9, #16384	; 0x4000
	DCI.W &e3409000 	; movt	r9, #0
	DCI.W &e3089000 	; movw	r9, #32768	; 0x8000
	DCI.W &e3409000 	; movt	r9, #0
	DCI.W &e3009000 	; movw	r9, #0
	DCI.W &e3409001 	; movt	r9, #1
	DCI.W &e3009000 	; movw	r9, #0
	DCI.W &e3409002 	; movt	r9, #2
	DCI.W &e3009000 	; movw	r9, #0
	DCI.W &e3409004 	; movt	r9, #4
	DCI.W &e3009000 	; movw	r9, #0
	DCI.W &e3409008 	; movt	r9, #8
	DCI.W &e3009000 	; movw	r9, #0
	DCI.W &e3409010 	; movt	r9, #16
	DCI.W &e3009000 	; movw	r9, #0
	DCI.W &e3409020 	; movt	r9, #32
	DCI.W &e3009000 	; movw	r9, #0
	DCI.W &e3409040 	; movt	r9, #64		; 0x40
	DCI.W &e3009000 	; movw	r9, #0
	DCI.W &e3409080 	; movt	r9, #128	; 0x80
	DCI.W &e3009000 	; movw	r9, #0
	DCI.W &e3409100 	; movt	r9, #256	; 0x100
	DCI.W &e3009000 	; movw	r9, #0
	DCI.W &e3409200 	; movt	r9, #512	; 0x200
	DCI.W &e3009000 	; movw	r9, #0
	DCI.W &e3409400 	; movt	r9, #1024	; 0x400
	DCI.W &e3009000 	; movw	r9, #0
	DCI.W &e3409800 	; movt	r9, #2048	; 0x800
	DCI.W &e3009000 	; movw	r9, #0
	DCI.W &e3419000 	; movt	r9, #4096	; 0x1000
	DCI.W &e3009000 	; movw	r9, #0
	DCI.W &e3429000 	; movt	r9, #8192	; 0x2000
	DCI.W &e3009000 	; movw	r9, #0
	DCI.W &e3449000 	; movt	r9, #16384	; 0x4000
	DCI.W &e3009000 	; movw	r9, #0
	DCI.W &e3489000 	; movt	r9, #32768	; 0x8000

	THUMB
	; MOVW
	DCI.W &f2400901 	; movw	r9, #1
	DCI.W &f2400902 	; movw	r9, #2
	DCI.W &f2400904 	; movw	r9, #4
	DCI.W &f2400908 	; movw	r9, #8
	DCI.W &f2400910 	; movw	r9, #16
	DCI.W &f2400920 	; movw	r9, #32
	DCI.W &f2400940 	; movw	r9, #64		; 0x40
	DCI.W &f2400980 	; movw	r9, #128	; 0x80
	DCI.W &f2401900 	; movw	r9, #256	; 0x100
	DCI.W &f2402900 	; movw	r9, #512	; 0x200
	DCI.W &f2404900 	; movw	r9, #1024	; 0x400
	DCI.W &f6400900 	; movw	r9, #2048	; 0x800
	DCI.W &f2410900 	; movw	r9, #4096	; 0x1000
	DCI.W &f2420900 	; movw	r9, #8192	; 0x2000
	DCI.W &f2440900 	; movw	r9, #16384	; 0x4000
	DCI.W &f2480900 	; movw	r9, #32768	; 0x8000
	; MOVT
	DCI.W &f2c00901 	; movt	r9, #1
	DCI.W &f2c00902 	; movt	r9, #2
	DCI.W &f2c00904 	; movt	r9, #4
	DCI.W &f2c00908 	; movt	r9, #8
	DCI.W &f2c00910 	; movt	r9, #16
	DCI.W &f2c00920 	; movt	r9, #32
	DCI.W &f2c00940 	; movt	r9, #64		; 0x40
	DCI.W &f2c00980 	; movt	r9, #128	; 0x80
	DCI.W &f2c01900 	; movt	r9, #256	; 0x100
	DCI.W &f2c02900 	; movt	r9, #512	; 0x200
	DCI.W &f2c04900 	; movt	r9, #1024	; 0x400
	DCI.W &f6c00900 	; movt	r9, #2048	; 0x800
	DCI.W &f2c10900 	; movt	r9, #4096	; 0x1000
	DCI.W &f2c20900 	; movt	r9, #8192	; 0x2000
	DCI.W &f2c40900 	; movt	r9, #16384	; 0x4000
	DCI.W &f2c80900 	; movt	r9, #32768	; 0x8000
	; MOV32
	DCI.W &f2400901 	; movw	r9, #1
	DCI.W &f2c00900 	; movt	r9, #0
	DCI.W &f2400902 	; movw	r9, #2
	DCI.W &f2c00900 	; movt	r9, #0
	DCI.W &f2400904 	; movw	r9, #4
	DCI.W &f2c00900 	; movt	r9, #0
	DCI.W &f2400908 	; movw	r9, #8
	DCI.W &f2c00900 	; movt	r9, #0
	DCI.W &f2400910 	; movw	r9, #16
	DCI.W &f2c00900 	; movt	r9, #0
	DCI.W &f2400920 	; movw	r9, #32
	DCI.W &f2c00900 	; movt	r9, #0
	DCI.W &f2400940 	; movw	r9, #64		; 0x40
	DCI.W &f2c00900 	; movt	r9, #0
	DCI.W &f2400980 	; movw	r9, #128	; 0x80
	DCI.W &f2c00900 	; movt	r9, #0
	DCI.W &f2401900 	; movw	r9, #256	; 0x100
	DCI.W &f2c00900 	; movt	r9, #0
	DCI.W &f2402900 	; movw	r9, #512	; 0x200
	DCI.W &f2c00900 	; movt	r9, #0
	DCI.W &f2404900 	; movw	r9, #1024	; 0x400
	DCI.W &f2c00900 	; movt	r9, #0
	DCI.W &f6400900 	; movw	r9, #2048	; 0x800
	DCI.W &f2c00900 	; movt	r9, #0
	DCI.W &f2410900 	; movw	r9, #4096	; 0x1000
	DCI.W &f2c00900 	; movt	r9, #0
	DCI.W &f2420900 	; movw	r9, #8192	; 0x2000
	DCI.W &f2c00900 	; movt	r9, #0
	DCI.W &f2440900 	; movw	r9, #16384	; 0x4000
	DCI.W &f2c00900 	; movt	r9, #0
	DCI.W &f2480900 	; movw	r9, #32768	; 0x8000
	DCI.W &f2c00900 	; movt	r9, #0
	DCI.W &f2400900 	; movw	r9, #0
	DCI.W &f2c00901 	; movt	r9, #1
	DCI.W &f2400900 	; movw	r9, #0
	DCI.W &f2c00902 	; movt	r9, #2
	DCI.W &f2400900 	; movw	r9, #0
	DCI.W &f2c00904 	; movt	r9, #4
	DCI.W &f2400900 	; movw	r9, #0
	DCI.W &f2c00908 	; movt	r9, #8
	DCI.W &f2400900 	; movw	r9, #0
	DCI.W &f2c00910 	; movt	r9, #16
	DCI.W &f2400900 	; movw	r9, #0
	DCI.W &f2c00920 	; movt	r9, #32
	DCI.W &f2400900 	; movw	r9, #0
	DCI.W &f2c00940 	; movt	r9, #64		; 0x40
	DCI.W &f2400900 	; movw	r9, #0
	DCI.W &f2c00980 	; movt	r9, #128	; 0x80
	DCI.W &f2400900 	; movw	r9, #0
	DCI.W &f2c01900 	; movt	r9, #256	; 0x100
	DCI.W &f2400900 	; movw	r9, #0
	DCI.W &f2c02900 	; movt	r9, #512	; 0x200
	DCI.W &f2400900 	; movw	r9, #0
	DCI.W &f2c04900 	; movt	r9, #1024	; 0x400
	DCI.W &f2400900 	; movw	r9, #0
	DCI.W &f6c00900 	; movt	r9, #2048	; 0x800
	DCI.W &f2400900 	; movw	r9, #0
	DCI.W &f2c10900 	; movt	r9, #4096	; 0x1000
	DCI.W &f2400900 	; movw	r9, #0
	DCI.W &f2c20900 	; movt	r9, #8192	; 0x2000
	DCI.W &f2400900 	; movw	r9, #0
	DCI.W &f2c40900 	; movt	r9, #16384	; 0x4000
	DCI.W &f2400900 	; movw	r9, #0
	DCI.W &f2c80900 	; movt	r9, #32768	; 0x8000

	]

	; Check one-character string support:
	[ :LNOT: REFERENCE
	ARM
	MOVW	r9, #" "
	MOVT	r9, #" "
	MOV32	r9, #" "

	THUMB
	MOVW	r9, #" "
	MOVT	r9, #" "
	MOV32	r9, #" "

	|

	ARM
	DCI.W &e3009020 	; movw	r9, #32
	DCI.W &e3409020 	; movt	r9, #32
	DCI.W &e3009020 	; movw	r9, #32
	DCI.W &e3409000 	; movt	r9, #0

	THUMB
	DCI.W &f2400920 	; movw	r9, #32
	DCI.W &f2c00920 	; movt	r9, #32
	DCI.W &f2400920 	; movw	r9, #32
	DCI.W &f2c00900 	; movt	r9, #0
	]

	END
