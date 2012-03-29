; Calling sequence:
;                MHigh
;              /   |   \
;        MMid1    70    MMid2
;       /  |  \        /  |  \
;   MLow1 50  MLow2 MLow3 60 MLow4
;      |        |     |        |
;     10       20    30       40

	AREA	Code, CODE, READONLY

	MACRO
	MLow1
	MOV	r1, #0x0
	B	%10
;	B	%b10
	B	%f10
	B	%t10
;	B	%bt10
	B	%ft10
	B	%a10
;	B	%ba10
	B	%fa10
	MOV	r1, #0x1
	B	%20
;	B	%b20
	B	%f20
	B	%t20
;	B	%bt20
	B	%ft20
	B	%a20
;	B	%ba20
	B	%fa20
	MOV	r1, #0x2
	B	%30
;	B	%b30
	B	%f30
	B	%t30
;	B	%bt30
	B	%ft30
	B	%a30
;	B	%ba30
	B	%fa30
	MOV	r1, #0x3
	B	%40
;	B	%b40
	B	%f40
	B	%t40
;	B	%bt40
	B	%ft40
	B	%a40
;	B	%ba40
	B	%fa40
	MOV	r1, #0x4
	B	%50
;	B	%b50
	B	%f50
;;	B	%t50
;	B	%bt50
;;	B	%ft50
	B	%a50
;	B	%ba50
	B	%fa50
	MOV	r1, #0x5
	B	%60
;	B	%b60
	B	%f60
;;	B	%t60
;	B	%bt60
;;	B	%ft60
	B	%a60
;	B	%ba60
	B	%fa60
	MOV	r1, #0x6
	B	%70
;	B	%b70
	B	%f70
;;	B	%t70
;	B	%bt70
;;	B	%ft70
	B	%a70
;	B	%ba70
	B	%fa70
	MOV	r1, #0x7
	B	%99
;	B	%b99
	B	%f99
	B	%t99
;	B	%bt99
	B	%ft99
	B	%a99
;	B	%ba99
	B	%fa99
99
10	MOV	r1, #0x8
	B	%10
	B	%b10
;	B	%f10
	B	%t10
	B	%bt10
;	B	%ft10
	B	%a10
	B	%ba10
;	B	%fa10
	MOV	r1, #0x9
	B	%20
;	B	%b20
	B	%f20
	B	%t20
;	B	%bt20
	B	%ft20
	B	%a20
;	B	%ba20
	B	%fa20
	MOV	r1, #0xA
	B	%30
;	B	%b30
	B	%f30
	B	%t30
;	B	%bt30
	B	%ft30
	B	%a30
;	B	%ba30
	B	%fa30
	MOV	r1, #0xB
	B	%40
;	B	%b40
	B	%f40
	B	%t40
;	B	%bt40
	B	%ft40
	B	%a40
;	B	%ba40
	B	%fa40
	MOV	r1, #0xC
	B	%50
;	B	%b50
	B	%f50
;	B	%t50
;	B	%bt50
;	B	%ft50
	B	%a50
;	B	%ba50
	B	%fa50
	MOV	r1, #0xD
	B	%60
;	B	%b60
	B	%f60
;	B	%t60
;	B	%bt60
;	B	%ft60
	B	%a60
;	B	%ba60
	B	%fa60
	MOV	r1, #0xE
	B	%70
;	B	%b70
	B	%f70
;	B	%t70
;	B	%bt70
;	B	%ft70
	B	%a70
;	B	%ba70
	B	%fa70
	MOV	r1, #0xF
	B	%99
	B	%b99
	B	%f99
	B	%t99
	B	%bt99
	B	%ft99
	B	%a99
	B	%ba99
	B	%fa99
	MOV	r1, #0x10
	MEND

	MACRO
	MLow2
	MOV	r2, #0x0
	B	%10
	B	%b10
;	B	%f10
	B	%t10
	B	%bt10
;	B	%ft10
	B	%a10
	B	%ba10
;	B	%fa10
	MOV	r2, #0x1
	B	%20
;	B	%b20
	B	%f20
	B	%t20
;	B	%bt20
	B	%ft20
	B	%a20
;	B	%ba20
	B	%fa20
	MOV	r2, #0x2
	B	%30
;	B	%b30
	B	%f30
	B	%t30
;	B	%bt30
	B	%ft30
	B	%a30
;	B	%ba30
	B	%fa30
	MOV	r2, #0x3
	B	%40
;	B	%b40
	B	%f40
	B	%t40
;	B	%bt40
	B	%ft40
	B	%a40
;	B	%ba40
	B	%fa40
	MOV	r2, #0x4
	B	%50
	B	%b50
;	B	%f50
;	B	%t50
;	B	%bt50
;	B	%ft50
	B	%a50
	B	%ba50
;	B	%fa50
	MOV	r2, #0x5
	B	%60
;	B	%b60
	B	%f60
;	B	%t60
;	B	%bt60
;	B	%ft60
	B	%a60
;	B	%ba60
	B	%fa60
	MOV	r2, #0x6
	B	%70
;	B	%b70
	B	%f70
;	B	%t70
;	B	%bt70
;	B	%ft70
	B	%a70
;	B	%ba70
	B	%fa70
	MOV	r2, #0x7
	B	%99
	B	%b99
	B	%f99
	B	%t99
	B	%bt99
	B	%ft99
	B	%a99
	B	%ba99
	B	%fa99
99
20	MOV	r2, #0x8
	B	%10
	B	%b10
;	B	%f10
	B	%t10
	B	%bt10
;	B	%ft10
	B	%a10
	B	%ba10
;	B	%fa10
	MOV	r2, #0x9
	B	%20
	B	%b20
;	B	%f20
	B	%t20
	B	%bt20
;	B	%ft20
	B	%a20
	B	%ba20
;	B	%fa20
	MOV	r2, #0xA
	B	%30
;	B	%b30
	B	%f30
	B	%t30
;	B	%bt30
	B	%ft30
	B	%a30
;	B	%ba30
	B	%fa30
	MOV	r2, #0xB
	B	%40
;	B	%b40
	B	%f40
	B	%t40
;	B	%bt40
	B	%ft40
	B	%a40
;	B	%ba40
	B	%fa40
	MOV	r2, #0xC
	B	%50
	B	%b50
;	B	%f50
;	B	%t50
;	B	%bt50
;	B	%ft50
	B	%a50
	B	%ba50
;	B	%fa50
	MOV	r2, #0xD
	B	%60
;	B	%b60
	B	%f60
;	B	%t60
;	B	%bt60
;	B	%ft60
	B	%a60
;	B	%ba60
	B	%fa60
	MOV	r2, #0xE
	B	%70
;	B	%b70
	B	%f70
;	B	%t70
;	B	%bt70
;	B	%ft70
	B	%a70
;	B	%ba70
	B	%fa70
	MOV	r2, #0xF
	B	%99
	B	%b99
	B	%f99
	B	%t99
	B	%bt99
	B	%ft99
	B	%a99
	B	%ba99
	B	%fa99
	MOV	r2, #0x10
	MEND

	MACRO
	MLow3
	MOV	r3, #0x0
	B	%10
	B	%b10
;	B	%f10
	B	%t10
	B	%bt10
;	B	%ft10
	B	%a10
	B	%ba10
;	B	%fa10
	MOV	r3, #0x1
	B	%20
	B	%b20
;	B	%f20
	B	%t20
	B	%bt20
;	B	%ft20
	B	%a20
	B	%ba20
;	B	%fa20
	MOV	r3, #0x2
	B	%30
;	B	%b30
	B	%f30
	B	%t30
;	B	%bt30
	B	%ft30
	B	%a30
;	B	%ba30
	B	%fa30
	MOV	r3, #0x3
	B	%40
;	B	%b40
	B	%f40
	B	%t40
;	B	%bt40
	B	%ft40
	B	%a40
;	B	%ba40
	B	%fa40
	MOV	r3, #0x4
	B	%50
	B	%b50
;	B	%f50
;	B	%t50
;	B	%bt50
;	B	%ft50
	B	%a50
	B	%ba50
;	B	%fa50
	MOV	r3, #0x5
	B	%60
;	B	%b60
	B	%f60
;	B	%t60
;	B	%bt60
;	B	%ft60
	B	%a60
;	B	%ba60
	B	%fa60
	MOV	r3, #0x6
	B	%70
	B	%b70
;	B	%f70
;	B	%t70
;	B	%bt70
;	B	%ft70
	B	%a70
	B	%ba70
;	B	%fa70
	MOV	r3, #0x7
	B	%99
	B	%b99
	B	%f99
	B	%t99
	B	%bt99
	B	%ft99
	B	%a99
	B	%ba99
	B	%fa99
99
30	MOV	r3, #0x8
	B	%10
	B	%b10
;	B	%f10
	B	%t10
	B	%bt10
;	B	%ft10
	B	%a10
	B	%ba10
;	B	%fa10
	MOV	r3, #0x9
	B	%20
	B	%b20
;	B	%f20
	B	%t20
	B	%bt20
;	B	%ft20
	B	%a20
	B	%ba20
;	B	%fa20
	MOV	r3, #0xA
	B	%30
	B	%b30
;	B	%f30
	B	%t30
	B	%bt30
;	B	%ft30
	B	%a30
	B	%ba30
;	B	%fa30
	MOV	r3, #0xB
	B	%40
;	B	%b40
	B	%f40
	B	%t40
;	B	%bt40
	B	%ft40
	B	%a40
;	B	%ba40
	B	%fa40
	MOV	r3, #0xC
	B	%50
	B	%b50
;	B	%f50
;	B	%t50
;	B	%bt50
;	B	%ft50
	B	%a50
	B	%ba50
;	B	%fa50
	MOV	r3, #0xD
	B	%60
;	B	%b60
	B	%f60
;	B	%t60
;	B	%bt60
;	B	%ft60
	B	%a60
;	B	%ba60
	B	%fa60
	MOV	r3, #0xE
	B	%70
	B	%b70
;	B	%f70
;	B	%t70
;	B	%bt70
;	B	%ft70
	B	%a70
	B	%ba70
;	B	%fa70
	MOV	r3, #0xF
	B	%99
	B	%b99
	B	%f99
	B	%t99
	B	%bt99
	B	%ft99
	B	%a99
	B	%ba99
	B	%fa99
	MOV	r3, #0x10
	MEND

	MACRO
	MLow4
	MOV	r4, #0x0
	B	%10
	B	%b10
;	B	%f10
	B	%t10
	B	%bt10
;	B	%ft10
	B	%a10
	B	%ba10
;	B	%fa10
	MOV	r4, #0x1
	B	%20
	B	%b20
;	B	%f20
	B	%t20
	B	%bt20
;	B	%ft20
	B	%a20
	B	%ba20
;	B	%fa20
	MOV	r4, #0x2
	B	%30
	B	%b30
;	B	%f30
	B	%t30
	B	%bt30
;	B	%ft30
	B	%a30
	B	%ba30
;	B	%fa30
	MOV	r4, #0x3
	B	%40
;	B	%b40
	B	%f40
	B	%t40
;	B	%bt40
	B	%ft40
	B	%a40
;	B	%ba40
	B	%fa40
	MOV	r4, #0x4
	B	%50
	B	%b50
;	B	%f50
;	B	%t50
;	B	%bt50
;	B	%ft50
	B	%a50
	B	%ba50
;	B	%fa50
	MOV	r4, #0x5
	B	%60
	B	%b60
;	B	%f60
;	B	%t60
;	B	%bt60
;	B	%ft60
	B	%a60
	B	%ba60
;	B	%fa60
	MOV	r4, #0x6
	B	%70
	B	%b70
;	B	%f70
;	B	%t70
;	B	%bt70
;	B	%ft70
	B	%a70
	B	%ba70
;	B	%fa70
	MOV	r4, #0x7
	B	%99
	B	%b99
	B	%f99
	B	%t99
	B	%bt99
	B	%ft99
	B	%a99
	B	%ba99
	B	%fa99
99
40	MOV	r4, #0x8
	B	%10
	B	%b10
;	B	%f10
	B	%t10
	B	%bt10
;	B	%ft10
	B	%a10
	B	%ba10
;	B	%fa10
	MOV	r4, #0x9
	B	%20
	B	%b20
;	B	%f20
	B	%t20
	B	%bt20
;	B	%ft20
	B	%a20
	B	%ba20
;	B	%fa20
	MOV	r4, #0xA
	B	%30
	B	%b30
;	B	%f30
	B	%t30
	B	%bt30
;	B	%ft30
	B	%a30
	B	%ba30
;	B	%fa30
	MOV	r4, #0xB
	B	%40
	B	%b40
;	B	%f40
	B	%t40
	B	%bt40
;	B	%ft40
	B	%a40
	B	%ba40
;	B	%fa40
	MOV	r4, #0xC
	B	%50
	B	%b50
;	B	%f50
;	B	%t50
;	B	%bt50
;	B	%ft50
	B	%a50
	B	%ba50
;	B	%fa50
	MOV	r4, #0xD
	B	%60
	B	%b60
;	B	%f60
;	B	%t60
;	B	%bt60
;	B	%ft60
	B	%a60
	B	%ba60
;	B	%fa60
	MOV	r4, #0xE
	B	%70
	B	%b70
;	B	%f70
;	B	%t70
;	B	%bt70
;	B	%ft70
	B	%a70
	B	%ba70
;	B	%fa70
	MOV	r4, #0xF
	B	%99
	B	%b99
;	B	%f99
	B	%t99
	B	%bt99
;	B	%ft99
	B	%a99
	B	%ba99
;	B	%fa99
	MOV	r4, #0x10
	MEND

	MACRO
	MMid1
	MOV	r5, #0x0
	MLow1
99
50	MOV	r5, #0x8
	MLow2
	MOV	r5, #0xF
	MEND

	MACRO
	MMid2
	MOV	r6, #0x0
	MLow3
99
60	MOV	r6, #0x8
	MLow4
	MOV	r6, #0xF
	MEND

	MACRO
	MHigh
	MOV	r7, #0x0
	MMid1
99	MOV	r7, #0x8
70	MOV	r7, #0x9
	MMid2
	MOV	r7, #0xF
	MEND

	MHigh

	END
