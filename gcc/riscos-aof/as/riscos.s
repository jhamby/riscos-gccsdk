; os.s

a1	RN	0
a2	RN	1
a3	RN	2
a4	RN	3
v1	RN	4
v2	RN	5
v3	RN	6
v4	RN	7
v5	RN	8
v6	RN	9
sl	RN	10
fp	RN	11
ip	RN	12
sp	RN	13
lr	RN	14
pc	RN	15

	AREA	|C$$code|, CODE, READONLY

XOS_File			* &20008
XOS_FSControl			* &20029
XOS_SWINumberFromString		* &20039
XDDEUtils_ThrowbackRegister	* &62585
XDDEUtils_ThrowbackUnRegister	* &62586
XDDEUtils_ThrowbackStart	* &62587
XDDEUtils_ThrowbackSend		* &62588
XDDEUtils_ThrowbackEnd		* &62580

Throwback_ReasonProcessing	* 0
Throwback_ReasonErrorDetails	* 1
Throwback_ReasonInfoDetails	* 2

ThrowbackInfo			* -1
ThrowbackWarning		* 0
ThrowbackError			* 1
ThrowbackSeriousError		* 2

FSControl_CanonicalisePath	* 37


	IMPORT	|__rt_stkovf_split_small|
	IMPORT	strlen
	IMPORT	malloc
	IMPORT	strcpy


	= "switonum",0
	ALIGN
	& &FF00000C
|switonum|
	EXPORT switonum
	MOV	ip,sp
	STMDB	sp!,{fp,ip,lr,pc}
	SUB	fp,ip,#4
	MOV	a2,a1
	SWI	XOS_SWINumberFromString
	MVNVS	a1,#0
	LDMDB	fp,{fp,sp,pc}^


	= "cdir",0
	ALIGN
	& &FF000008
|cdir|
	EXPORT	cdir
	MOV	ip,sp
	STMDB	sp!,{v1-v2,fp,ip,lr,pc}
	SUB	fp,ip,#4
	MOV	a2,a1
	MOV	a1,#8
	MOV	a3,#77
	SWI	XOS_File
	MOVVC	a1,#0
	LDMDB	fp,{v1-v2,fp,sp,pc}^

	= "ThrowbackStart",0
	ALIGN
	& &FF000010
|ThrowbackStart|
	EXPORT	ThrowbackStart
	MOV	ip,sp
	STMDB	sp!,{v1,fp,ip,lr,pc}
	SUB	fp,ip,#4
	MOV	a1,#0
	MOV	a2,#0
	MOV	a3,#0
	MOV	a4,#0
	MOV	v1,#0
	SWI	XDDEUtils_ThrowbackStart
	MOVVC	a1,#0
	LDMDB	fp,{v1,fp,sp,pc}^

	= "ThrowbackSendStart",0
	ALIGN
	& &FF000014
|ThrowbackSendStart|
	EXPORT	ThrowbackSendStart
	MOV	ip,sp
	STMDB	sp!,{v1,fp,ip,lr,pc}
	SUB	fp,ip,#4
	LDR	a3,ErrorFilePtr
	STR	a1,[a3]
	MOV	a3,a1
	MOV	a2,#0
	MOV	a1,#Throwback_ReasonProcessing
	MOV	v1,#0
	SWI	XDDEUtils_ThrowbackSend
	MOVVC	a1,#0
	LDMDB	fp,{v1,fp,sp,pc}^

ErrorFilePtr
	&	ErrorFile

	= "ThrowbackSendError",0
	ALIGN
	& &FF000014
|ThrowbackSendError|
	EXPORT	ThrowbackSendError
	MOV	ip,sp
	STMDB	sp!,{v1,v2,fp,ip,lr,pc}
	SUB	fp,ip,#4
	MOVS	v2,a3
	ADRMI	v2,fatal
	MOV	a4,a2
	MOV	a2,#0
	LDR	a3,ErrorFilePtr
	LDR	a3,[a3]
	CMN	a1,#-ThrowbackInfo
	MOVEQ	v1,#0
	MOVEQ	a1,#Throwback_ReasonInfoDetails
	MOVNE	v1,a1
	MOVNE	a1,#Throwback_ReasonErrorDetails
	SWI	XDDEUtils_ThrowbackSend
	MOVVC	a1,#0
	LDMDB	fp,{v1,v2,fp,sp,pc}^
fatal	= "Invalid pointer passed to ThrowbackSendError",0
	ALIGN

	= "ThrowbackEnd",0
	ALIGN
	& &FF000010
|ThrowbackEnd|
	EXPORT	ThrowbackEnd
	MOV	ip,sp
	STMDB	sp!,{v1,fp,ip,lr,pc}
	SUB	fp,ip,#4
	MOV	a1,#0
	MOV	a2,#0
	MOV	a3,#0
	MOV	a4,#0
	MOV	v1,#0
	SWI	XDDEUtils_ThrowbackEnd
	MOVVC	a1,#0
	LDMDB	fp,{v1,fp,sp,pc}^

	= "OSCanonicalisePath",0
	ALIGN
	& &FF000014
|OSCanonicalisePath|
	EXPORT	OSCanonicalisePath
	MOV	ip,sp
	STMDB	sp!,{v1,v2,fp,ip,lr,pc}
	SUB	fp,ip,#4
; a2 <- a1   = char *path
; a3 <- a2   = char *buffer
; v2 <- a3   = int bufferSize
; a4 <- a4   = char *systemVar
; v1 <- [ip] = char *defaultPath
	MOV	v2,a3
	LDR	v1,[ip]
	MOV	a3,a2
	MOV	a2,a1
	MOV	a1,#FSControl_CanonicalisePath
	SWI	XOS_FSControl
	MOV	a1,v2
	LDMDB	fp,{v1,v2,fp,sp,pc}^


	AREA	|C$$data|, DATA

|ErrorFile|
	EXPORT	ErrorFile
	&	0

	END
