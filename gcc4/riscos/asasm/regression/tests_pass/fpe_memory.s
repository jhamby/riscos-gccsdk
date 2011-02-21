; FPE (FPA10/FPA11/FPASC) memory access : LDF/STF/LFM/SFM
; Option autocast is specified as we want to test the integer -> float
; conversions.
; RUNOPT: -autocast

		AREA |C$$code|, CODE, READONLY

	[ :LNOT: REFERENCE
		LDFS	F1,[R2]
		LDFS	F1,[R2,#4]
		LDFS	F1,[R2,#-4]
		LDFS	F1,[R2]!
		LDFS	F1,[R2,#4]!
		LDFS	F1,[R2,#-4]!
		LDFS	F1,[R2],#4
		LDFS	F1,[R2],#-4

		LDFD	F1,[R2]
		LDFD	F1,[R2,#4]
		LDFD	F1,[R2,#-4]
		LDFD	F1,[R2]!
		LDFD	F1,[R2,#4]!
		LDFD	F1,[R2,#-4]!
		LDFD	F1,[R2],#4
		LDFD	F1,[R2],#-4

		LDFE	F1,[R2]
		LDFE	F1,[R2,#4]
		LDFE	F1,[R2,#-4]
		LDFE	F1,[R2]!
		LDFE	F1,[R2,#4]!
		LDFE	F1,[R2,#-4]!
		LDFE	F1,[R2],#4
		LDFE	F1,[R2],#-4

		LDFP	F1,[R2]
		LDFP	F1,[R2,#4]
		LDFP	F1,[R2,#-4]
		LDFP	F1,[R2]!
		LDFP	F1,[R2,#4]!
		LDFP	F1,[R2,#-4]!
		LDFP	F1,[R2],#4
		LDFP	F1,[R2],#-4
	|
		DCI	&ED921100	; LDFS    F1,[R2,#0]
		DCI	&ED921101	; LDFS    F1,[R2,#4]
		DCI	&ED121101	; LDFS    F1,[R2,#-4]
		DCI	&EDB21100	; LDFS    F1,[R2]!
		DCI	&EDB21101	; LDFS    F1,[R2,#4]!
		DCI	&ED321101	; LDFS    F1,[R2,#-4]!
		DCI	&ECB21101	; LDFS    F1,[R2],#4
		DCI	&EC321101	; LDFS    F1,[R2],#-4

		DCI	&ED929100	; LDFD    F1,[R2,#0]
		DCI	&ED929101	; LDFD    F1,[R2,#4]
		DCI	&ED129101	; LDFD    F1,[R2,#-4]
		DCI	&EDB29100	; LDFD    F1,[R2]!
		DCI	&EDB29101	; LDFD    F1,[R2,#4]!
		DCI	&ED329101	; LDFD    F1,[R2,#-4]!
		DCI	&ECB29101	; LDFD    F1,[R2],#4
		DCI	&EC329101	; LDFD    F1,[R2],#-4

		DCI	&EDD21100	; LDFE    F1,[R2,#0]
		DCI	&EDD21101	; LDFE    F1,[R2,#4]
		DCI	&ED521101	; LDFE    F1,[R2,#-4]
		DCI	&EDF21100	; LDFE    F1,[R2]!
		DCI	&EDF21101	; LDFE    F1,[R2,#4]!
		DCI	&ED721101	; LDFE    F1,[R2,#-4]!
		DCI	&ECF21101	; LDFE    F1,[R2],#4
		DCI	&EC721101	; LDFE    F1,[R2],#-4

		DCI	&EDD29100	; LDFP    F1,[R2,#0]
		DCI	&EDD29101	; LDFP    F1,[R2,#4]
		DCI	&ED529101	; LDFP    F1,[R2,#-4]
		DCI	&EDF29100	; LDFP    F1,[R2]!
		DCI	&EDF29101	; LDFP    F1,[R2,#4]!
		DCI	&ED729101	; LDFP    F1,[R2,#-4]!
		DCI	&ECF29101	; LDFP    F1,[R2],#4
		DCI	&EC729101	; LDFP    F1,[R2],#-4
	]

	[ :LNOT: REFERENCE
		STFS	F1,[R2]
		STFS	F1,[R2,#4]
		STFS	F1,[R2,#-4]
		STFS	F1,[R2]!
		STFS	F1,[R2,#4]!
		STFS	F1,[R2,#-4]!
		STFS	F1,[R2],#4
		STFS	F1,[R2],#-4

		STFD	F1,[R2]
		STFD	F1,[R2,#4]
		STFD	F1,[R2,#-4]
		STFD	F1,[R2]!
		STFD	F1,[R2,#4]!
		STFD	F1,[R2,#-4]!
		STFD	F1,[R2],#4
		STFD	F1,[R2],#-4

		STFE	F1,[R2]
		STFE	F1,[R2,#4]
		STFE	F1,[R2,#-4]
		STFE	F1,[R2]!
		STFE	F1,[R2,#4]!
		STFE	F1,[R2,#-4]!
		STFE	F1,[R2],#4
		STFE	F1,[R2],#-4

		STFP	F1,[R2]
		STFP	F1,[R2,#4]
		STFP	F1,[R2,#-4]
		STFP	F1,[R2]!
		STFP	F1,[R2,#4]!
		STFP	F1,[R2,#-4]!
		STFP	F1,[R2],#4
		STFP	F1,[R2],#-4
	|
		DCI	&ED821100	; STFS    F1,[R2,#0]
		DCI	&ED821101	; STFS    F1,[R2,#4]
		DCI	&ED021101	; STFS    F1,[R2,#-4]
		DCI	&EDA21100	; STFS    F1,[R2]!
		DCI	&EDA21101	; STFS    F1,[R2,#4]!
		DCI	&ED221101	; STFS    F1,[R2,#-4]!
		DCI	&ECA21101	; STFS    F1,[R2],#4
		DCI	&EC221101	; STFS    F1,[R2],#-4

		DCI	&ED829100	; STFD    F1,[R2,#0]
		DCI	&ED829101	; STFD    F1,[R2,#4]
		DCI	&ED029101	; STFD    F1,[R2,#-4]
		DCI	&EDA29100	; STFD    F1,[R2]!
		DCI	&EDA29101	; STFD    F1,[R2,#4]!
		DCI	&ED229101	; STFD    F1,[R2,#-4]!
		DCI	&ECA29101	; STFD    F1,[R2],#4
		DCI	&EC229101	; STFD    F1,[R2],#-4

		DCI	&EDC21100	; STFE    F1,[R2,#0]
		DCI	&EDC21101	; STFE    F1,[R2,#4]
		DCI	&ED421101	; STFE    F1,[R2,#-4]
		DCI	&EDE21100	; STFE    F1,[R2]!
		DCI	&EDE21101	; STFE    F1,[R2,#4]!
		DCI	&ED621101	; STFE    F1,[R2,#-4]!
		DCI	&ECE21101	; STFE    F1,[R2],#4
		DCI	&EC621101	; STFE    F1,[R2],#-4

		DCI	&EDC29100	; STFP    F1,[R2,#0]
		DCI	&EDC29101	; STFP    F1,[R2,#4]
		DCI	&ED429101	; STFP    F1,[R2,#-4]
		DCI	&EDE29100	; STFP    F1,[R2]!
		DCI	&EDE29101	; STFP    F1,[R2,#4]!
		DCI	&ED629101	; STFP    F1,[R2,#-4]!
		DCI	&ECE29101	; STFP    F1,[R2],#4
		DCI	&EC629101	; STFP    F1,[R2],#-4
	]

	[ :LNOT: REFERENCE
		; LFM form 1 : one register
		LFM	F1,1,[R2]
		LFM	F1,1,[R2,#4]
		LFM	F1,1,[R2,#-4]
		LFM	F1,1,[R2]!
		LFM	F1,1,[R2,#4]!
		LFM	F1,1,[R2,#-4]!
		LFM	F1,1,[R2],#4
		LFM	F1,1,[R2],#-4

		; LFM form 1 : more than one register
		LFM	F1,2,[R2]
		LFM	F1,3,[R2]
		LFM	F1,4,[R2]

		; LFM form 2 : one register
		LFMFD	F1,1,[R2]
		LFMFD	F1,1,[R2]!
		LFMEA	F1,1,[R2]
		LFMEA	F1,1,[R2]!

		; LFM form 2 : more than one register
		LFMFD	F1,2,[R2]
		LFMFD	F1,3,[R2]
		LFMFD	F1,4,[R2]
		LFMFD	F1,2,[R2]!
		LFMFD	F1,3,[R2]!
		LFMFD	F1,4,[R2]!
		LFMEA	F1,2,[R2]
		LFMEA	F1,3,[R2]
		LFMEA	F1,4,[R2]
		LFMEA	F1,2,[R2]!
		LFMEA	F1,3,[R2]!
		LFMEA	F1,4,[R2]!
	|
		; LFM form 1 : one register
		DCD	&ED929200	; LFM     F1,1,[R2,#0]
		DCD	&ED929201	; LFM     F1,1,[R2,#4]
		DCD	&ED129201	; LFM     F1,1,[R2,#-4]
		DCD	&EDB29200	; LFM     F1,1,[R2,#0]!
		DCD	&EDB29201	; LFM     F1,1,[R2,#4]!
		DCD	&ED329201	; LFM     F1,1,[R2,#-4]!
		DCD	&ECB29201	; LFM     F1,1,[R2],#4
		DCD	&EC329201	; LFM     F1,1,[R2],#-4

		; LFM form 1 : more than one register
		DCD	&EDD21200	; LFM     F1,2,[R2,#0]
		DCD	&EDD29200	; LFM     F1,3,[R2,#0]
		DCD	&ED921200	; LFM     F1,4,[R2,#0]

		; LFM form 2 : one register
		DCD	&ED929200	; LFM     F1,1,[R2,#0]
		DCD	&ECB29203	; LFM     F1,1,[R2],#&00C
		DCD	&ED129203	; LFM     F1,1,[R2,#-&00C]
		DCD	&ED329203	; LFM     F1,1,[R2,#-&00C]!

		; LFM form 2 : more than one register
		DCD	&EDD21200	; LFM     F1,2,[R2,#0]
		DCD	&EDD29200	; LFM     F1,3,[R2,#0]
		DCD	&ED921200	; LFM     F1,4,[R2,#0]
		DCD	&ECF21206	; LFM     F1,2,[R2],#&018
		DCD	&ECF29209	; LFM     F1,3,[R2],#&024
		DCD	&ECB2120C	; LFM     F1,4,[R2],#&030
		DCD	&ED521206	; LFM     F1,2,[R2,#-&018]
		DCD	&ED529209	; LFM     F1,3,[R2,#-&024]
		DCD	&ED12120C	; LFM     F1,4,[R2,#-&030]
		DCD	&ED721206	; LFM     F1,2,[R2,#-&018]!
		DCD	&ED729209	; LFM     F1,3,[R2,#-&024]!
		DCD	&ED32120C	; LFM     F1,4,[R2,#-&030]!
	]

	[ :LNOT: REFERENCE
		; SFM form 1 : one register
		SFM	F1,1,[R2]
		SFM	F1,1,[R2,#4]
		SFM	F1,1,[R2,#-4]
		SFM	F1,1,[R2]!
		SFM	F1,1,[R2,#4]!
		SFM	F1,1,[R2,#-4]!
		SFM	F1,1,[R2],#4
		SFM	F1,1,[R2],#-4

		; SFM form 1 : more than one register
		SFM	F1,2,[R2]
		SFM	F1,3,[R2]
		SFM	F1,4,[R2]

		; SFM form 2 : one register
		SFMFD	F1,1,[R2]
		SFMFD	F1,1,[R2]!
		SFMEA	F1,1,[R2]
		SFMEA	F1,1,[R2]!

		; SFM form 2 : more than one register
		SFMFD	F1,2,[R2]
		SFMFD	F1,3,[R2]
		SFMFD	F1,4,[R2]
		SFMFD	F1,2,[R2]!
		SFMFD	F1,3,[R2]!
		SFMFD	F1,4,[R2]!
		SFMEA	F1,2,[R2]
		SFMEA	F1,3,[R2]
		SFMEA	F1,4,[R2]
		SFMEA	F1,2,[R2]!
		SFMEA	F1,3,[R2]!
		SFMEA	F1,4,[R2]!
	|
		; SFM form 1 : one register
		DCD	&ED829200	; SFM     F1,1,[R2,#0]
		DCD	&ED829201	; SFM     F1,1,[R2,#4]
		DCD	&ED029201	; SFM     F1,1,[R2,#-4]
		DCD	&EDA29200	; SFM     F1,1,[R2,#0]!
		DCD	&EDA29201	; SFM     F1,1,[R2,#4]!
		DCD	&ED229201	; SFM     F1,1,[R2,#-4]!
		DCD	&ECA29201	; SFM     F1,1,[R2],#4
		DCD	&EC229201	; SFM     F1,1,[R2],#-4

		; SFM form 1 : more than one register
		DCD	&EDC21200	; SFM     F1,2,[R2,#0]
		DCD	&EDC29200	; SFM     F1,3,[R2,#0]
		DCD	&ED821200	; SFM     F1,4,[R2,#0]

		; SFM form 2 : one register
		DCD	&ED029203	; SFM     F1,1,[R2,#-&00C]
		DCD	&ED229203	; SFM     F1,1,[R2,#-&00C]!
		DCD	&ED829200	; SFM     F1,1,[R2,#0]
		DCD	&ECA29203	; SFM     F1,1,[R2],#&00C

		; SFM form 2 : more than one register
		DCD	&ED421206	; SFM     F1,2,[R2,#-&018]
		DCD	&ED429209	; SFM     F1,3,[R2,#-&024]
		DCD	&ED02120C	; SFM     F1,4,[R2,#-&030]
		DCD	&ED621206	; SFM     F1,2,[R2,#-&018]!
		DCD	&ED629209	; SFM     F1,3,[R2,#-&024]!
		DCD	&ED22120C	; SFM     F1,4,[R2,#-&030]!
		DCD	&EDC21200	; SFM     F1,2,[R2,#0]
		DCD	&EDC29200	; SFM     F1,3,[R2,#0]
		DCD	&ED821200	; SFM     F1,4,[R2,#0]
		DCD	&ECE21206	; SFM     F1,2,[R2],#&018
		DCD	&ECE29209	; SFM     F1,3,[R2],#&024
		DCD	&ECA2120C	; SFM     F1,4,[R2],#&030
	]

		END
