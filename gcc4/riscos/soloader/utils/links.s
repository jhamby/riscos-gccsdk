	.include	"swis.h"
	.include	"macros.h"

.set	MAX_LINKS,	64

.set	LINK_ID,	0x4b4e494c

@; r0 = input filename
@; r1 = output buffer
@; r2 = size of output buffer
	.global	resolve_links
resolve_links:
	STMFD	sp!, {r4-r12,lr}

	MOV	r7, r0 @; file_in
	MOV	r8, r1 @; file_out
	MOV	r9, r2 @; file_out_len
	MOV	r10, #-1 @; fd

	@; strlen(file_in) + 1
	BL	strlen
	ADD	r0, r0, #1

	@; if > file_out_len return -1
	CMP	r0, r9
	BGT	99f

	@; strcpy(file_out, file_in);
	MOV	r0, r8
	MOV	r1, r7
	BL	strcpy

	MOV	r0, #23
	MOV	r1, r8
	SWI	XOS_File
	BVS	99f

	MOV	r12, #0	@; link_count
	ADR	r11, buffer
0:
	TEQ	r0, #0
	BEQ	1f
	LDR	r1, filetype
	TEQ	r6, r1
	BNE	1f

	CMP	r12, #MAX_LINKS
	BGE	99f

	MOV	r0, #0x4f
	MOV	r1, r8
	SWI	XOS_Find
	BVS	99f
	TEQ	r0, #0
	BEQ	99f

	MOV	r10, r0	@; fd

	MOV	r0, #4
	MOV	r1, r10
	MOV	r2, r11
	MOV	r3, #4
	SWI	XOS_GBPB
	BVS	99f

	@; Read file ID word and check for link file
	LDR	r2, [r11, #0]
	LDR	r3, =LINK_ID
	TEQ	r2, r3
	MOVNE	r0, #0
	SWINE	XOS_Find
	BNE	99f

	@; Read length of new filename
	MOV	r2, r11
	MOV	r3, #4
	SWI	XOS_GBPB
	BVS	99f

	LDR	r5, [r11, #0]

	@; Read new filename
	MOV	r2, r11
	MOV	r3, r5
	SWI	XOS_GBPB
	BVS	99f

	@; Finished reading file, so close
	MOV	r0, #0
	SWI	XOS_Find
	MOV	r10, #-1

	@; Terminate new filename
	MOV	r0, #0
	STRB	r0, [r11, r5]

	MOV	r0, r11
	MOV	r1, #':'
	BL	strchr
	TEQ	r0, #0
	BEQ	2f

	MOV	r0, r11
	BL	strlen
	ADD	r0, r0, #1
	CMP	r0, r9
	BGT	99f
	MOV	r0, r8
	MOV	r1, r11
	BL	strcpy
	B	3f
2:
	MOV	r4, #0
	MOV	r0, r8
	MOV	r1, #'.'
	BL	strrchr
	TEQ	r0, #0
	STRNEB	r4, [r0, #1]
	BNE	4f
	MOV	r0, r8
	MOV	r1, #':'
	TEQ	r0, #0
	STRNEB	r4, [r0, #1]
	STREQB	r4, [r8, #0]
4:
	MOV	r0, r8
	BL	strlen
	ADD	r4, r0, #2

	MOV	r0, r11
	BL	strlen
	ADD	r4, r4, r0

	CMP	r4, r9
	BGT	99f

	MOV	r0, r8
	MOV	r1, r11
	BL	strcat
3:
	MOV	r0, #23
	MOV	r1, r8
	SWI	XOS_File
	BVS	99f

	ADD	r12, r12, #1	@; link_count++
	B	0b
1:
	MOV	r0, #0
	LDMFD	sp!, {r4-r12,pc}
99:
	CMP	r10, #-1
	MOVEQ	r0, #0
	MOVEQ	r1, r10
	SWIEQ	XOS_Find

	MOV	r0, #-1
	LDMFD	sp!, {r4-r12,pc}
filetype:
	.word	0x1C8
buffer:
	.skip	256

	.end
