	AREA |C$$code|, CODE, READONLY

	; Addressing Mode 2 - Load/Store Word or Unsigned Byte
	; Loads
	; 1
	ldr r0, [r1]
	ldr r0, [r1, #0]
	ldr r0, [r1, #4]
	ldr r0, [r1, #-4]
	; 2
	ldr r0, [r1, r2]
	ldr r0, [r1, -r2]
	; 3
	ldr r0, [r1, r2, LSL #2]
	ldr r0, [r1, -r2, ASR #4]
	; 4
	ldr r0, [r1]!
	ldr r0, [r1, #0]!
	ldr r0, [r1, #4]!
	ldr r0, [r1, #-4]!
	; 5
	ldr r0, [r1, r2]!
	ldr r0, [r1, -r2]!
	; 6
	ldr r0, [r1, r2, LSL #2]!
	ldr r0, [r1, -r2, ASR #4]!
	; 7
	ldr r0, [r1], #4
	ldr r0, [r1], #-4
	; 8
	ldr r0, [r1], r2
	ldr r0, [r1], -r2
	; 9
	ldr r0, [r1], r2, LSL #2
	ldr r0, [r1], -r2, ASR #4

	; 1
	ldrb r0, [r1]
	ldrb r0, [r1, #0]
	ldrb r0, [r1, #4]
	ldrb r0, [r1, #-4]
	; 2
	ldrb r0, [r1, r2]
	ldrb r0, [r1, -r2]
	; 3
	ldrb r0, [r1, r2, LSL #2]
	ldrb r0, [r1, -r2, ASR #4]
	; 4
	ldrb r0, [r1]!
	ldrb r0, [r1, #0]!
	ldrb r0, [r1, #4]!
	ldrb r0, [r1, #-4]!
	; 5
	ldrb r0, [r1, r2]!
	ldrb r0, [r1, -r2]!
	; 6
	ldrb r0, [r1, r2, LSL #2]!
	ldrb r0, [r1, -r2, ASR #4]!
	; 7
	ldrb r0, [r1], #4
	ldrb r0, [r1], #-4
	; 8
	ldrb r0, [r1], r2
	ldrb r0, [r1], -r2
	; 9
	ldrb r0, [r1], r2, LSL #2
	ldrb r0, [r1], -r2, ASR #4

	; 7
	ldrt r0, [r1], #4
	ldrt r0, [r1], #-4
	; 8
	ldrt r0, [r1], r2
	ldrt r0, [r1], -r2
	; 9
	ldrt r0, [r1], r2, LSL #2
	ldrt r0, [r1], -r2, ASR #4

	; 7
	ldrbt r0, [r1], #4
	ldrbt r0, [r1], #-4
	; 8
	ldrbt r0, [r1], r2
	ldrbt r0, [r1], -r2
	; 9
	ldrbt r0, [r1], r2, LSL #2
	ldrbt r0, [r1], -r2, ASR #4

	; Stores
	; 1
	str r0, [r1]
	str r0, [r1, #0]
	str r0, [r1, #4]
	str r0, [r1, #-4]
	; 2
	str r0, [r1, r2]
	str r0, [r1, -r2]
	; 3
	str r0, [r1, r2, LSL #2]
	str r0, [r1, -r2, ASR #4]
	; 4
	str r0, [r1]!
	str r0, [r1, #0]!
	str r0, [r1, #4]!
	str r0, [r1, #-4]!
	; 5
	str r0, [r1, r2]!
	str r0, [r1, -r2]!
	; 6
	str r0, [r1, r2, LSL #2]!
	str r0, [r1, -r2, ASR #4]!
	; 7
	str r0, [r1], #4
	str r0, [r1], #-4
	; 8
	str r0, [r1], r2
	str r0, [r1], -r2
	; 9
	str r0, [r1], r2, LSL #2
	str r0, [r1], -r2, ASR #4

	; 1
	strb r0, [r1]
	strb r0, [r1, #0]
	strb r0, [r1, #4]
	strb r0, [r1, #-4]
	; 2
	strb r0, [r1, r2]
	strb r0, [r1, -r2]
	; 3
	strb r0, [r1, r2, LSL #2]
	strb r0, [r1, -r2, ASR #4]
	; 4
	strb r0, [r1]!
	strb r0, [r1, #0]!
	strb r0, [r1, #4]!
	strb r0, [r1, #-4]!
	; 5
	strb r0, [r1, r2]!
	strb r0, [r1, -r2]!
	; 6
	strb r0, [r1, r2, LSL #2]!
	strb r0, [r1, -r2, ASR #4]!
	; 7
	strb r0, [r1], #4
	strb r0, [r1], #-4
	; 8
	strb r0, [r1], r2
	strb r0, [r1], -r2
	; 9
	strb r0, [r1], r2, LSL #2
	strb r0, [r1], -r2, ASR #4

	; 7
	strt r0, [r1], #4
	strt r0, [r1], #-4
	; 8
	strt r0, [r1], r2
	strt r0, [r1], -r2
	; 9
	strt r0, [r1], r2, LSL #2
	strt r0, [r1], -r2, ASR #4

	; 7
	strbt r0, [r1], #4
	strbt r0, [r1], #-4
	; 8
	strbt r0, [r1], r2
	strbt r0, [r1], -r2
	; 9
	strbt r0, [r1], r2, LSL #2
	strbt r0, [r1], -r2, ASR #4

	; Addressing Mode 3 - Miscellaneous Loads/Stores
	; Loads
	; 1
	ldrh r0, [r1]
	ldrh r0, [r1, #0]
	ldrh r0, [r1, #4]
	ldrh r0, [r1, #-4]
	; 2
	ldrh r0, [r1, r2]
	ldrh r0, [r1, -r2]
	; 3
	ldrh r0, [r1]!
	ldrh r0, [r1, #0]!
	ldrh r0, [r1, #4]!
	ldrh r0, [r1, #-4]!
	; 4
	ldrh r0, [r1, r2]!
	ldrh r0, [r1, -r2]!
	; 5
	ldrh r0, [r1], #4
	ldrh r0, [r1], #-4
	; 6
	ldrh r0, [r1], r2
	ldrh r0, [r1], -r2

	; 1
	ldrsh r0, [r1]
	ldrsh r0, [r1, #0]
	ldrsh r0, [r1, #4]
	ldrsh r0, [r1, #-4]
	; 2
	ldrsh r0, [r1, r2]
	ldrsh r0, [r1, -r2]
	; 3
	ldrsh r0, [r1]!
	ldrsh r0, [r1, #0]!
	ldrsh r0, [r1, #4]!
	ldrsh r0, [r1, #-4]!
	; 4
	ldrsh r0, [r1, r2]!
	ldrsh r0, [r1, -r2]!
	; 5
	ldrsh r0, [r1], #4
	ldrsh r0, [r1], #-4
	; 6
	ldrsh r0, [r1], r2
	ldrsh r0, [r1], -r2

	; 1
	ldrsb r0, [r1]
	ldrsb r0, [r1, #0]
	ldrsb r0, [r1, #4]
	ldrsb r0, [r1, #-4]
	; 2
	ldrsb r0, [r1, r2]
	ldrsb r0, [r1, -r2]
	; 3
	ldrsb r0, [r1]!
	ldrsb r0, [r1, #0]!
	ldrsb r0, [r1, #4]!
	ldrsb r0, [r1, #-4]!
	; 4
	ldrsb r0, [r1, r2]!
	ldrsb r0, [r1, -r2]!
	; 5
	ldrsb r0, [r1], #4
	ldrsb r0, [r1], #-4
	; 6
	ldrsb r0, [r1], r2
	ldrsb r0, [r1], -r2

	; 1
;	ldrd r0, [r1]
;	ldrd r0, [r1, #0]
;	ldrd r0, [r1, #4]
;	ldrd r0, [r1, #-4]
	; 2
;	ldrd r0, [r1, r2]
;	ldrd r0, [r1, -r2]
	; 3
;	ldrd r0, [r1]!
;	ldrd r0, [r1, #0]!
;	ldrd r0, [r1, #4]!
;	ldrd r0, [r1, #-4]!
	; 4
;	ldrd r0, [r1, r2]!
;	ldrd r0, [r1, -r2]!
	; 5
;	ldrd r0, [r1], #4
;	ldrd r0, [r1], #-4
	; 6
;	ldrd r0, [r1], r2
;	ldrd r0, [r1], -r2

	; Stores
	; 1
	strh r0, [r1]
	strh r0, [r1, #0]
	strh r0, [r1, #4]
	strh r0, [r1, #-4]
	; 2
	strh r0, [r1, r2]
	strh r0, [r1, -r2]
	; 3
	strh r0, [r1]!
	strh r0, [r1, #0]!
	strh r0, [r1, #4]!
	strh r0, [r1, #-4]!
	; 4
	strh r0, [r1, r2]!
	strh r0, [r1, -r2]!
	; 5
	strh r0, [r1], #4
	strh r0, [r1], #-4
	; 6
	strh r0, [r1], r2
	strh r0, [r1], -r2

	; 1
;	strd r0, [r1]
;	strd r0, [r1, #0]
;	strd r0, [r1, #4]
;	strd r0, [r1, #-4]
	; 2
;	strd r0, [r1, r2]
;	strd r0, [r1, -r2]
	; 3
;	strd r0, [r1]!
;	strd r0, [r1, #0]!
;	strd r0, [r1, #4]!
;	strd r0, [r1, #-4]!
	; 4
;	strd r0, [r1, r2]!
;	strd r0, [r1, -r2]!
	; 5
;	strd r0, [r1], #4
;	strd r0, [r1], #-4
	; 6
;	strd r0, [r1], r2
;	strd r0, [r1], -r2
