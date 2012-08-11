; Tests PKHBT/PKHTB.

	AREA Code, CODE

	[ :LNOT: REFERENCE

	; PKHBT
	ARM
	PKHBT r1, r2, r3
	PKHBT r1, r2, r3, LSL#0
	PKHBT r1, r2, r3, LSL#4
	PKHBTCS r2, r3, LSL#31

	PKHBT.W r1, r2, r3
	PKHBT.W r1, r2, r3, LSL#0
	PKHBT.W r1, r2, r3, LSL#4
	PKHBTCS.W r2, r3, LSL#31

	THUMB
	PKHBT r1, r2, r3
	PKHBT r1, r2, r3, LSL#0
	PKHBT r1, r2, r3, LSL#4
	PKHBTCS r2, r3, LSL#31

	PKHBT.W r1, r2, r3
	PKHBT.W r1, r2, r3, LSL#0
	PKHBT.W r1, r2, r3, LSL#4
	PKHBTCS.W r2, r3, LSL#31

	; PKHTB
	ARM
	PKHTB r1, r2, r3		; Becomes PKHBT
	PKHTB r1, r2, r3, ASR#4
	PKHTBCS r2, r3, ASR#31
	PKHTB r1, r2, r3, ASR#32

	PKHTB.W r1, r2, r3		; Becomes PKHBT
	PKHTB.W r1, r2, r3, ASR#4
	PKHTBCS.W r2, r3, ASR#31
	PKHTB.W r1, r2, r3, ASR#32

	THUMB
	PKHTB r1, r2, r3		; Becomes PKHBT
	PKHTB r1, r2, r3, ASR#4
	PKHTBCS r2, r3, ASR#31
	PKHTB r1, r2, r3, ASR#32

	PKHTB.W r1, r2, r3		; Becomes PKHBT
	PKHTB.W r1, r2, r3, ASR#4
	PKHTBCS.W r2, r3, ASR#31
	PKHTB.W r1, r2, r3, ASR#32

	|

	; PKHBT
	ARM
	DCI &e6821013	; 	pkhbt	r1, r2, r3
	DCI &e6821013	; 	pkhbt	r1, r2, r3
	DCI &e6821213	; 	pkhbt	r1, r2, r3, lsl #4
	DCI &26822f93	; 	pkhbtcs	r2, r2, r3, lsl #31

	DCI &e6821013	; 	pkhbt	r1, r2, r3
	DCI &e6821013	; 	pkhbt	r1, r2, r3
	DCI &e6821213	; 	pkhbt	r1, r2, r3, lsl #4
	DCI &26822f93	; 	pkhbtcs	r2, r2, r3, lsl #31

	THUMB
	DCI.W &eac20103	; 	pkhbt	r1, r2, r3
	DCI.W &eac20103	; 	pkhbt	r1, r2, r3
	DCI.W &eac21103	; 	pkhbt	r1, r2, r3, lsl #4
	DCI.W &eac272c3	; 	pkhbt	r2, r2, r3, lsl #31  FIXME: IT support missing

	DCI.W &eac20103	; 	pkhbt	r1, r2, r3
	DCI.W &eac20103	; 	pkhbt	r1, r2, r3
	DCI.W &eac21103	; 	pkhbt	r1, r2, r3, lsl #4
	DCI.W &eac272c3	; 	pkhbt	r2, r2, r3, lsl #31  FIXME: IT support missing

	; PKHTB
	ARM
	DCI &e6821013	; 	pkhbt	r1, r2, r3
	DCI &e6821253	; 	pkhtb	r1, r2, r3, asr #4
	DCI &26822fd3	; 	pkhtbcs	r2, r2, r3, asr #31
	DCI &e6821053	; 	pkhtb	r1, r2, r3, asr #32

	DCI &e6821013	; 	pkhbt	r1, r2, r3
	DCI &e6821253	; 	pkhtb	r1, r2, r3, asr #4
	DCI &26822fd3	; 	pkhtbcs	r2, r2, r3, asr #31
	DCI &e6821053	; 	pkhtb	r1, r2, r3, asr #32

	THUMB
	DCI.W &eac20103	; 	pkhbt	r1, r2, r3
	DCI.W &eac21123	; 	pkhtb	r1, r2, r3, asr #4
	DCI.W &eac272e3	; 	pkhtb	r2, r2, r3, asr #31  FIXME: IT support missing
	DCI.W &eac20123	; 	pkhtb	r1, r2, r3, asr #32

	DCI.W &eac20103	; 	pkhbt	r1, r2, r3
	DCI.W &eac21123	; 	pkhtb	r1, r2, r3, asr #4
	DCI.W &eac272e3	; 	pkhtb	r2, r2, r3, asr #31  FIXME: IT support missing
	DCI.W &eac20123	; 	pkhtb	r1, r2, r3, asr #32

	]

	END