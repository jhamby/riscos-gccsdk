	.file	"input.c"
	.version	"01.01"
gcc2_compiled.:
.globl inputName
.data
	.align 4
	.type	 inputName,@object
	.size	 inputName,4
inputName:
	.long 0
.globl inputExpand
	.align 4
	.type	 inputExpand,@object
	.size	 inputExpand,4
inputExpand:
	.long 1
.globl inputRewind
	.align 4
	.type	 inputRewind,@object
	.size	 inputRewind,4
inputRewind:
	.long 0
.globl input_buff
	.align 4
	.type	 input_buff,@object
	.size	 input_buff,4
input_buff:
	.long 0
.text
	.align 4
.globl inputLook
	.type	 inputLook,@function
inputLook:
	pushl %ebp
	movl %esp,%ebp
	movl input_pos,%eax
	movsbl (%eax),%eax
	leave
	ret
.Lfe1:
	.size	 inputLook,.Lfe1-inputLook
	.align 4
.globl inputLookLower
	.type	 inputLookLower,@function
inputLookLower:
	pushl %ebp
	movl %esp,%ebp
	movl input_pos,%ecx
	movsbl (%ecx),%edx
	movl __ctype_b,%eax
	testb $1,1(%eax,%edx,2)
	je .L178
	movb (%ecx),%al
	addb $32,%al
	movsbl %al,%eax
	jmp .L179
	.p2align 4,,7
.L178:
	movl %edx,%eax
.L179:
	leave
	ret
.Lfe2:
	.size	 inputLookLower,.Lfe2-inputLookLower
	.align 4
.globl inputLookUC
	.type	 inputLookUC,@function
inputLookUC:
	pushl %ebp
	movl %esp,%ebp
	pushl %ebx
	movl input_pos,%eax
	movb (%eax),%cl
	cmpl $0,uc
	je .L181
	movb %cl,%bl
	movsbl %cl,%edx
	movl __ctype_b,%eax
	testb $4,1(%eax,%edx,2)
	je .L183
	xorb $32,%bl
.L183:
	movsbl %bl,%eax
	jmp .L182
	.p2align 4,,7
.L181:
	movsbl %cl,%edx
	movl __ctype_b,%eax
	testb $1,1(%eax,%edx,2)
	je .L184
	addb $32,%cl
	movsbl %cl,%eax
	jmp .L182
	.p2align 4,,7
.L184:
	movl %edx,%eax
.L182:
	movl -4(%ebp),%ebx
	leave
	ret
.Lfe3:
	.size	 inputLookUC,.Lfe3-inputLookUC
	.align 4
.globl inputComment
	.type	 inputComment,@function
inputComment:
	pushl %ebp
	movl %esp,%ebp
	movl input_pos,%eax
	movsbl (%eax),%edx
	xorl %eax,%eax
	testl %edx,%edx
	je .L188
	cmpl $59,%edx
	je .L188
	cmpl $0,gcc_backend
	je .L187
	cmpl $64,%edx
	jne .L187
.L188:
	movl $1,%eax
.L187:
	leave
	ret
.Lfe4:
	.size	 inputComment,.Lfe4-inputComment
	.align 4
.globl inputLookN
	.type	 inputLookN,@function
inputLookN:
	pushl %ebp
	movl %esp,%ebp
	movl 8(%ebp),%edx
	movl input_pos,%eax
	movsbl (%edx,%eax),%eax
	leave
	ret
.Lfe5:
	.size	 inputLookN,.Lfe5-inputLookN
	.align 4
.globl inputLookNLower
	.type	 inputLookNLower,@function
inputLookNLower:
	pushl %ebp
	movl %esp,%ebp
	pushl %ebx
	movl 8(%ebp),%ebx
	movl input_pos,%ecx
	movsbl (%ebx,%ecx),%edx
	movl __ctype_b,%eax
	testb $1,1(%eax,%edx,2)
	je .L191
	movb (%ebx,%ecx),%al
	addb $32,%al
	movsbl %al,%eax
	jmp .L192
	.p2align 4,,7
.L191:
	movl %edx,%eax
.L192:
	movl -4(%ebp),%ebx
	leave
	ret
.Lfe6:
	.size	 inputLookNLower,.Lfe6-inputLookNLower
	.align 4
.globl inputLookNUC
	.type	 inputLookNUC,@function
inputLookNUC:
	pushl %ebp
	movl %esp,%ebp
	pushl %ebx
	movl 8(%ebp),%edx
	movl input_pos,%eax
	movb (%edx,%eax),%cl
	cmpl $0,uc
	je .L194
	movb %cl,%bl
	movsbl %cl,%edx
	movl __ctype_b,%eax
	testb $4,1(%eax,%edx,2)
	je .L196
	xorb $32,%bl
.L196:
	movsbl %bl,%eax
	jmp .L195
	.p2align 4,,7
.L194:
	movsbl %cl,%edx
	movl __ctype_b,%eax
	testb $1,1(%eax,%edx,2)
	je .L197
	addb $32,%cl
	movsbl %cl,%eax
	jmp .L195
	.p2align 4,,7
.L197:
	movl %edx,%eax
.L195:
	movl -4(%ebp),%ebx
	leave
	ret
.Lfe7:
	.size	 inputLookNUC,.Lfe7-inputLookNUC
	.align 4
.globl inputGet
	.type	 inputGet,@function
inputGet:
	pushl %ebp
	movl %esp,%ebp
	movl input_pos,%eax
	cmpb $0,(%eax)
	jne .L200
	xorl %eax,%eax
	jmp .L202
	.p2align 4,,7
.L200:
	movsbl (%eax),%eax
	incl input_pos
.L202:
	leave
	ret
.Lfe8:
	.size	 inputGet,.Lfe8-inputGet
	.align 4
.globl inputGetLower
	.type	 inputGetLower,@function
inputGetLower:
	pushl %ebp
	movl %esp,%ebp
	movl input_pos,%eax
	movb (%eax),%dl
	testb %dl,%dl
	je .L204
	incl input_pos
.L204:
	movsbl %dl,%ecx
	movl __ctype_b,%eax
	testb $1,1(%eax,%ecx,2)
	je .L205
	addb $32,%dl
	movsbl %dl,%eax
	jmp .L206
	.p2align 4,,7
.L205:
	movl %ecx,%eax
.L206:
	leave
	ret
.Lfe9:
	.size	 inputGetLower,.Lfe9-inputGetLower
	.align 4
.globl inputGetUC
	.type	 inputGetUC,@function
inputGetUC:
	pushl %ebp
	movl %esp,%ebp
	pushl %ebx
	movl input_pos,%eax
	movb (%eax),%cl
	testb %cl,%cl
	je .L208
	incl input_pos
.L208:
	cmpl $0,uc
	je .L209
	movb %cl,%bl
	movsbl %cl,%edx
	movl __ctype_b,%eax
	testb $4,1(%eax,%edx,2)
	je .L211
	xorb $32,%bl
.L211:
	movsbl %bl,%eax
	jmp .L210
	.p2align 4,,7
.L209:
	movsbl %cl,%edx
	movl __ctype_b,%eax
	testb $1,1(%eax,%edx,2)
	je .L212
	addb $32,%cl
	movsbl %cl,%eax
	jmp .L210
	.p2align 4,,7
.L212:
	movl %edx,%eax
.L210:
	movl -4(%ebp),%ebx
	leave
	ret
.Lfe10:
	.size	 inputGetUC,.Lfe10-inputGetUC
.section	.rodata
	.align 32
.LC0:
	.string	"Internal inputUnGet: illegal character"
.text
	.align 4
.globl inputUnGet
	.type	 inputUnGet,@function
inputUnGet:
	pushl %ebp
	movl %esp,%ebp
	movb 8(%ebp),%dl
	movl input_pos,%eax
	cmpl input_buff,%eax
	jbe .L215
	cmpb %dl,-1(%eax)
	jne .L215
	decl %eax
	movl %eax,input_pos
	jmp .L216
	.p2align 4,,7
.L215:
	cmpb $0,(%eax)
	jne .L218
	testb %dl,%dl
	je .L216
.L218:
	pushl $.LC0
	pushl $0
	pushl $3
	call error
.L216:
	leave
	ret
.Lfe11:
	.size	 inputUnGet,.Lfe11-inputUnGet
	.align 4
.globl inputSkip
	.type	 inputSkip,@function
inputSkip:
	pushl %ebp
	movl %esp,%ebp
	movl input_pos,%eax
	cmpb $0,(%eax)
	je .L220
	incl %eax
	movl %eax,input_pos
.L220:
	leave
	ret
.Lfe12:
	.size	 inputSkip,.Lfe12-inputSkip
.section	.rodata
	.align 32
.LC1:
	.string	"Internal inputSkipN: trying to skip more characters than are available"
.text
	.align 4
.globl inputSkipN
	.type	 inputSkipN,@function
inputSkipN:
	pushl %ebp
	movl %esp,%ebp
	pushl %ebx
	movl 8(%ebp),%ecx
	movl input_pos,%edx
	cmpb $0,(%edx)
	je .L228
	.p2align 4,,7
.L226:
	subl $1,%ecx
	jc .L223
	leal 1(%edx),%ebx
	movl %ebx,input_pos
	movb 1(%edx),%al
	movl %ebx,%edx
	testb %al,%al
	jne .L226
.L223:
	cmpb $0,(%edx)
	je .L228
	cmpl $-1,%ecx
	jg .L229
	jmp .L227
	.p2align 4,,7
.L228:
	testl %ecx,%ecx
	jle .L227
.L229:
	pushl $.LC1
	pushl $0
	pushl $3
	call error
.L227:
	movl -4(%ebp),%ebx
	leave
	ret
.Lfe13:
	.size	 inputSkipN,.Lfe13-inputSkipN
	.align 4
.globl inputRest
	.type	 inputRest,@function
inputRest:
	pushl %ebp
	movl %esp,%ebp
	movl input_pos,%edx
	movl %edx,%eax
	cmpb $0,(%edx)
	je .L232
	.p2align 4,,7
.L231:
	incl %eax
	cmpb $0,(%eax)
	jne .L231
.L232:
	movl %eax,input_pos
	movl %edx,%eax
	leave
	ret
.Lfe14:
	.size	 inputRest,.Lfe14-inputRest
	.align 4
.globl inputLine
	.type	 inputLine,@function
inputLine:
	pushl %ebp
	movl %esp,%ebp
	movl input_buff,%eax
	leave
	ret
.Lfe15:
	.size	 inputLine,.Lfe15-inputLine
	.align 4
.globl inputSkipLook
	.type	 inputSkipLook,@function
inputSkipLook:
	pushl %ebp
	movl %esp,%ebp
	movl input_pos,%eax
	cmpb $0,(%eax)
	jne .L237
	xorl %eax,%eax
	jmp .L238
	.p2align 4,,7
.L237:
	leal 1(%eax),%edx
	movl %edx,input_pos
	movsbl 1(%eax),%eax
.L238:
	leave
	ret
.Lfe16:
	.size	 inputSkipLook,.Lfe16-inputSkipLook
	.align 4
.globl skipblanks
	.type	 skipblanks,@function
skipblanks:
	pushl %ebp
	movl %esp,%ebp
	movl input_pos,%edx
	cmpb $0,(%edx)
	je .L241
	movl __ctype_b,%ecx
	.p2align 4,,7
.L244:
	movsbl (%edx),%eax
	testb $32,1(%ecx,%eax,2)
	je .L241
	incl %edx
	cmpb $0,(%edx)
	jne .L244
.L241:
	movl %edx,input_pos
	leave
	ret
.Lfe17:
	.size	 skipblanks,.Lfe17-skipblanks
	.align 4
.globl skiprest
	.type	 skiprest,@function
skiprest:
	pushl %ebp
	movl %esp,%ebp
	movl input_buff,%eax
	movl %eax,input_mark
	movl %eax,input_pos
	movb $0,(%eax)
	leave
	ret
.Lfe18:
	.size	 skiprest,.Lfe18-skiprest
	.align 4
.globl inputMark
	.type	 inputMark,@function
inputMark:
	pushl %ebp
	movl %esp,%ebp
	movl input_pos,%eax
	movl %eax,input_mark
	leave
	ret
.Lfe19:
	.size	 inputMark,.Lfe19-inputMark
	.align 4
.globl inputRollback
	.type	 inputRollback,@function
inputRollback:
	pushl %ebp
	movl %esp,%ebp
	movl input_mark,%eax
	movl %eax,input_pos
	leave
	ret
.Lfe20:
	.size	 inputRollback,.Lfe20-inputRollback
.section	.rodata
.LC2:
	.string	"-"
.LC3:
	.string	"r"
.LC4:
	.string	"AS can't read %s: %s"
.text
	.align 4
.globl inputInit
	.type	 inputInit,@function
inputInit:
	pushl %ebp
	movl %esp,%ebp
	pushl %ebx
	movl 8(%ebp),%ebx
	cmpl $0,input_buff
	jne .L249
	pushl $4352
	call malloc
	movl %eax,input_buff
	addl $4,%esp
.L249:
	testl %ebx,%ebx
	je .L273
	movzbl (%ebx),%eax
	addl $-45,%eax
	jne .L259
	movzbl 1(%ebx),%eax
.L259:
	testl %eax,%eax
	je .L273
	pushl $.LC3
	pushl %ebx
	call fopen
	movl %eax,asmfile
	addl $8,%esp
	testl %eax,%eax
	jne .L274
	call __errno_location
	pushl (%eax)
	call strerror
	pushl %eax
	pushl %ebx
	pushl $.LC4
	pushl $0
	pushl $4
	pushl $0
	pushl $0
	call errorLine
	addl $32,%esp
	jmp .L276
	.p2align 4,,7
.L274:
	movl %ebx,inputName
	pushl $16384
	call malloc
	pushl $16384
	pushl $0
	pushl %eax
	pushl asmfile
	call setvbuf
	addl $20,%esp
	jmp .L276
	.p2align 4,,7
.L273:
	movl stdin,%eax
	movl %eax,asmfile
.L276:
	movl $0,inputLineNo
	call skiprest
	movl -4(%ebp),%ebx
	leave
	ret
.Lfe21:
	.size	 inputInit,.Lfe21-inputInit
	.align 4
.globl inputFinish
	.type	 inputFinish,@function
inputFinish:
	pushl %ebp
	movl %esp,%ebp
	movl asmfile,%eax
	testl %eax,%eax
	je .L278
	cmpl stdin,%eax
	je .L278
	pushl %eax
	call fclose
.L278:
	leave
	ret
.Lfe22:
	.size	 inputFinish,.Lfe22-inputFinish
.section	.rodata
.LC5:
	.string	"No END found in this file"
.LC6:
	.string	"Line truncated"
.text
	.align 4
.globl inputNextLine
	.type	 inputNextLine,@function
inputNextLine:
	pushl %ebp
	movl %esp,%ebp
	subl $16,%esp
	pushl %edi
	pushl %esi
	pushl %ebx
	cmpl $0,inputRewind
	je .L280
	movl $0,inputRewind
	jmp .L281
	.p2align 4,,7
.L280:
	cmpl $0,asmfile
	je .L283
	incl inputLineNo
	cmpl $0,macroCurrent
	je .L285
	pushl $workBuff
	call macroGetLine
	addl $4,%esp
	testl %eax,%eax
	jne .L281
	jmp .L285
	.p2align 4,,7
.L287:
	cmpl $0,pedantic
	je .L288
	pushl $.LC5
	pushl $1
	pushl $1
	call error
	addl $12,%esp
.L288:
	call pop_file
	movl %eax,asmfile
	testl %eax,%eax
	jne .L285
.L283:
	negl inputLineNo
	xorl %eax,%eax
	jmp .L318
	.p2align 4,,7
.L285:
	pushl asmfile
	pushl $4096
	pushl $workBuff
	call fgets
	addl $12,%esp
	testl %eax,%eax
	je .L287
	movl $workBuff,%edx
	movl %edx,%edi
	xorl %eax,%eax
	cld
	movl $-1,%ecx
	repnz
	scasb
	notl %ecx
	leal -1(%ecx),%ebx
	testl %ebx,%ebx
	je .L281
	cmpb $35,workBuff
	jne .L292
	movl input_buff,%eax
	movl %eax,input_pos
	movb $0,(%eax)
.L319:
	movl $1,%eax
	jmp .L318
	.p2align 4,,7
.L292:
	cmpb $10,-2(%edx,%ecx)
	je .L293
	pushl asmfile
	call _IO_getc
	movl %eax,%ebx
	addl $4,%esp
	cmpl $-1,%ebx
	je .L281
	cmpl $10,%ebx
	je .L281
	pushl $.LC6
	pushl $1
	pushl $3
	call error
	addl $12,%esp
	jmp .L295
	.p2align 4,,7
.L299:
	cmpl $10,%ebx
	je .L281
.L295:
	pushl asmfile
	call _IO_getc
	movl %eax,%ebx
	addl $4,%esp
	cmpl $-1,%ebx
	je .L281
	jmp .L299
	.p2align 4,,7
.L293:
	movb $0,-1(%edx,%ebx)
.L281:
	cmpl $0,inputExpand
	jne .L301
	movl $workBuff,-4(%ebp)
	movl -4(%ebp),%edi
	xorl %eax,%eax
	cld
	movl $-1,%ecx
	repnz
	scasb
	notl %ecx
	cmpl $8,%ecx
	ja .L304
	movl input_buff,%esi
	movl %esi,input_pos
	movzbw workBuff+1,%ax
	salw $8,%ax
	movw %ax,-6(%ebp)
	movzbw workBuff,%ax
	orw %ax,-6(%ebp)
	movzbw workBuff+5,%ax
	salw $8,%ax
	movw %ax,-8(%ebp)
	movzbw workBuff+4,%ax
	orw %ax,-8(%ebp)
	movzbl workBuff+3,%edx
	sall $8,%edx
	movzbl workBuff+2,%eax
	orl %eax,%edx
	sall $8,%edx
	movzbl workBuff+1,%eax
	orl %eax,%edx
	sall $8,%edx
	movzbl workBuff,%eax
	orl %eax,%edx
	movzbl workBuff+7,%ebx
	sall $8,%ebx
	movzbl workBuff+6,%eax
	orl %eax,%ebx
	sall $8,%ebx
	movzbl workBuff+5,%eax
	orl %eax,%ebx
	sall $8,%ebx
	movzbl workBuff+4,%eax
	orl %eax,%ebx
	movl -4(%ebp),%edi
	xorl %eax,%eax
	cld
	movl $-1,%ecx
	repnz
	scasb
	notl %ecx
	decl %ecx
	cmpl $7,%ecx
	ja .L319
	jmp *.L307(,%ecx,4)
	.p2align 4,,7
.section	.rodata
	.align 4
	.align 4
.L307:
	.long .L308
	.long .L309
	.long .L310
	.long .L311
	.long .L312
	.long .L313
	.long .L314
	.long .L315
.text
	.p2align 4,,7
.L308:
	movb $0,(%esi)
	jmp .L319
	.p2align 4,,7
.L309:
	movw -6(%ebp),%ax
	movw %ax,(%esi)
	jmp .L319
	.p2align 4,,7
.L310:
	movw -6(%ebp),%ax
	movw %ax,(%esi)
	movb $0,2(%esi)
	jmp .L319
	.p2align 4,,7
.L311:
	movl %edx,(%esi)
	jmp .L319
	.p2align 4,,7
.L312:
	movl %edx,(%esi)
	movb $0,4(%esi)
	jmp .L319
	.p2align 4,,7
.L313:
	movl %edx,(%esi)
	movl -8(%ebp),%eax
	movw %ax,4(%esi)
	jmp .L319
	.p2align 4,,7
.L314:
	movl %edx,(%esi)
	movl -8(%ebp),%eax
	movw %ax,4(%esi)
	movb $0,6(%esi)
	jmp .L319
	.p2align 4,,7
.L315:
	movl %edx,(%esi)
	movl %ebx,4(%esi)
	jmp .L319
	.p2align 4,,7
.L304:
	movl input_buff,%eax
	movl %eax,input_pos
	movl -4(%ebp),%edi
	xorl %eax,%eax
	cld
	movl $-1,%ecx
	repnz
	scasb
	notl %ecx
	pushl %ecx
	movl -4(%ebp),%eax
	pushl %eax
	movl input_buff,%eax
	pushl %eax
	call memcpy
	jmp .L319
	.p2align 4,,7
.L301:
	call inputArgSub
.L318:
	leal -28(%ebp),%esp
	popl %ebx
	popl %esi
	popl %edi
	leave
	ret
.Lfe23:
	.size	 inputNextLine,.Lfe23-inputNextLine
.section	.rodata
.LC7:
	.string	"Non-ID in $ expansion"
.LC8:
	.string	"%i"
.LC9:
	.string	"%f"
.LC10:
	.string	"{TRUE}"
.LC11:
	.string	"{FALSE}"
.LC12:
	.string	"$ expansion '%s' is a pointer"
	.align 32
.LC13:
	.string	"Unknown value '%s' for $ expansion"
.LC14:
	.string	"Line expansion truncated"
.text
	.align 4
.globl inputArgSub
	.type	 inputArgSub,@function
inputArgSub:
	pushl %ebp
	movl %esp,%ebp
	subl $36,%esp
	pushl %edi
	pushl %esi
	pushl %ebx
	xorl %esi,%esi
	movl $0,-20(%ebp)
	cmpl $0,gcc_backend
	je .L321
	movb $64,-25(%ebp)
	jmp .L322
	.p2align 4,,7
.L515:
	movl $4096,%eax
	subl -24(%ebp),%eax
	subl %esi,%eax
	addl input_buff,%esi
	pushl %eax
	pushl %ebx
	pushl %esi
	call memcpy
	addl $12,%esp
	jmp .L337
	.p2align 4,,7
.L321:
	movb $0,-25(%ebp)
.L322:
	movl $workBuff,input_pos
	cmpb $0,workBuff
	je .L324
	.p2align 4,,7
.L510:
	cmpl $4095,%esi
	jg .L337
	jmp .L516
	.p2align 4,,7
.L332:
	cmpb $60,%dl
	jne .L331
	cmpl $0,objasm
	jne .L328
.L331:
	cmpb -25(%ebp),%dl
	je .L328
	cmpl $4095,%esi
	jg .L328
	movl input_buff,%eax
	movb %dl,(%esi,%eax)
	incl input_pos
	incl %esi
.L516:
	movl input_pos,%ebx
	movb (%ebx),%dl
	testb %dl,%dl
	je .L328
	cmpb $34,%dl
	je .L328
	cmpb $39,%dl
	je .L328
	cmpb $124,%dl
	je .L328
	cmpb $36,%dl
	je .L328
	cmpb $59,%dl
	jne .L332
.L328:
	movl %ebx,-32(%ebp)
	movb (%ebx),%cl
	cmpb $59,%cl
	je .L335
	jg .L507
	cmpb $36,%cl
	je .L381
	jg .L508
	cmpb $34,%cl
	je .L373
	jmp .L323
	.p2align 4,,7
.L508:
	cmpb $39,%cl
	je .L370
	jmp .L323
	.p2align 4,,7
.L507:
	cmpb $64,%cl
	je .L335
	jg .L509
	cmpb $60,%cl
	je .L355
	jmp .L323
	.p2align 4,,7
.L509:
	cmpb $124,%cl
	je .L363
	jmp .L323
	.p2align 4,,7
.L335:
	movl %ebx,%edi
	xorl %eax,%eax
	cld
	movl $-1,%ecx
	repnz
	scasb
	notl %ecx
	decl %ecx
	movl %ecx,-24(%ebp)
	addl %esi,%ecx
	cmpl $4095,%ecx
	jg .L515
	pushl %ebx
	movl %esi,%eax
	addl input_buff,%eax
	pushl %eax
	call strcpy
	jmp .L354
	.p2align 4,,7
.L355:
	leal 1(%ebx),%edx
	movb 1(%ebx),%cl
	testb %cl,%cl
	je .L357
	cmpb $62,%cl
	je .L361
	.p2align 4,,7
.L360:
	cmpb $32,%cl
	jle .L357
	incl %edx
	movb (%edx),%cl
	testb %cl,%cl
	je .L357
	cmpb $62,%cl
	jne .L360
.L357:
	cmpb $62,%cl
	je .L361
	movl input_buff,%edx
	movb (%ebx),%al
	movb %al,(%esi,%edx)
	incl input_pos
	incl %esi
	jmp .L517
	.p2align 4,,7
.L361:
	movb $62,%cl
	jmp .L363
	.p2align 4,,7
.L368:
	cmpl $4095,%esi
	jg .L364
.L363:
	movl input_buff,%edx
	movb (%ebx),%bl
	movb %bl,(%esi,%edx)
	incl input_pos
	incl %esi
	movl input_pos,%ebx
	movl %ebx,%edx
	movb (%ebx),%al
	movl %eax,%edi
	testb %al,%al
	je .L364
	cmpb %cl,%al
	jne .L368
.L364:
	cmpb %cl,(%edx)
	jne .L323
	movl input_buff,%eax
	movb %cl,(%esi,%eax)
	jmp .L518
	.p2align 4,,7
.L370:
	cmpl $0,objasm
	je .L373
	movl input_buff,%eax
	movb $39,(%esi,%eax)
	incl input_pos
	incl %esi
	movl input_pos,%ebx
	jmp .L373
	.p2align 4,,7
.L379:
	cmpl $4095,%esi
	jg .L374
.L373:
	movl input_buff,%edx
	movb (%ebx),%al
	movb %al,(%esi,%edx)
	incl input_pos
	incl %esi
	cmpb $92,%al
	jne .L375
	movl input_pos,%ebx
	movb (%ebx),%dl
	testb %dl,%dl
	je .L374
	movl input_buff,%eax
	movb %dl,(%esi,%eax)
	incl input_pos
	incl %esi
.L375:
	movl input_pos,%ebx
	movb (%ebx),%dl
	testb %dl,%dl
	je .L374
	cmpb %cl,%dl
	jne .L379
.L374:
	cmpb %cl,(%ebx)
	jne .L323
	movl input_buff,%eax
	movb %cl,(%esi,%eax)
	jmp .L518
	.p2align 4,,7
.L381:
	movl %esi,-24(%ebp)
	leal 1(%ebx),%eax
	movl %eax,input_pos
	cmpb $36,1(%ebx)
	jne .L382
	movl input_buff,%eax
	movb $36,(%esi,%eax)
.L518:
	incl %esi
	movl input_pos,%eax
	movl %eax,-36(%ebp)
	incl %eax
	movl %eax,input_pos
	movl -36(%ebp),%ebx
	incl %ebx
	jmp .L323
	.p2align 4,,7
.L382:
	leal -16(%ebp),%eax
	pushl %eax
	call lexGetId
	cmpl $0,-16(%ebp)
	je .L383
	pushl $.LC7
	pushl $1
	pushl $1
	call error
	movl input_buff,%eax
	movb $36,(%esi,%eax)
	incl %esi
	addl $12,%esp
	jmp .L517
	.p2align 4,,7
.L383:
	movl input_pos,%edx
	cmpb $46,(%edx)
	jne .L384
	incl %edx
	movl %edx,input_pos
.L384:
	cmpl $0,local
	je .L394
	cmpl $1,-8(%ebp)
	jne .L394
	movl -12(%ebp),%eax
	movsbl (%eax),%edx
	movl __ctype_toupper,%eax
	cmpl $76,(%eax,%edx,4)
	jne .L394
	movl input_buff,%eax
	movb $36,(%esi,%eax)
	incl %esi
	movl input_buff,%edx
	movl -12(%ebp),%eax
	movb (%eax),%al
	movb %al,(%esi,%edx)
	incl %esi
	jmp .L517
	.p2align 4,,7
.L394:
	pushl -4(%ebp)
	pushl -8(%ebp)
	pushl -12(%ebp)
	pushl -16(%ebp)
	call symbolFind
	movl %eax,%ebx
	addl $16,%esp
	testl %ebx,%ebx
	je .L479
	movl 12(%ebx),%edx
	decl %edx
	cmpl $63,%edx
	ja .L479
	jmp *.L480(,%edx,4)
	.p2align 4,,7
.section	.rodata
	.align 4
	.align 4
.L480:
	.long .L397
	.long .L398
	.long .L479
	.long .L399
	.long .L479
	.long .L479
	.long .L479
	.long .L418
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L477
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L477
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L479
	.long .L477
.text
	.p2align 4,,7
.L397:
	pushl 20(%ebx)
	pushl $.LC8
	movl %esi,-36(%ebp)
	movl input_buff,%eax
	addl %eax,-36(%ebp)
	movl -36(%ebp),%eax
	pushl %eax
	call sprintf
	movl input_buff,%edx
	leal (%esi,%edx),%edi
	xorl %eax,%eax
	cld
	movl $-1,%ecx
	repnz
	scasb
	notl %ecx
	leal -1(%esi,%ecx),%esi
	addl $12,%esp
	jmp .L396
	.p2align 4,,7
.L398:
	pushl 24(%ebx)
	pushl 20(%ebx)
	pushl $.LC9
	movl %esi,-36(%ebp)
	movl input_buff,%eax
	addl %eax,-36(%ebp)
	movl -36(%ebp),%eax
	pushl %eax
	call sprintf
	movl input_buff,%edx
	leal (%esi,%edx),%edi
	xorl %eax,%eax
	cld
	movl $-1,%ecx
	repnz
	scasb
	notl %ecx
	leal -1(%esi,%ecx),%esi
	addl $16,%esp
	jmp .L396
	.p2align 4,,7
.L399:
	movl %esi,%eax
	addl 20(%ebx),%eax
	cmpl $4095,%eax
	jle .L402
	movl $4097,%esi
	jmp .L519
	.p2align 4,,7
.L402:
	pushl 24(%ebx)
	movl %esi,%eax
	addl input_buff,%eax
	pushl %eax
	call strcpy
	addl $8,%esp
	addl 20(%ebx),%esi
	jmp .L519
	.p2align 4,,7
.L418:
	cmpl $0,20(%ebx)
	je .L473
	movl $.LC10,%edx
	jmp .L474
	.p2align 4,,7
.L473:
	movl $.LC11,%edx
.L474:
	pushl %edx
	movl %esi,-36(%ebp)
	movl input_buff,%eax
	addl %eax,-36(%ebp)
	movl -36(%ebp),%eax
	pushl %eax
	call strcpy
	addl $8,%esp
	movl input_buff,%edx
	leal (%esi,%edx),%edi
	xorl %eax,%eax
	cld
	movl $-1,%ecx
	repnz
	scasb
	notl %ecx
	leal -1(%esi,%ecx),%esi
	jmp .L396
	.p2align 4,,7
.L477:
	movl -12(%ebp),%edx
	movl -8(%ebp),%eax
	movb (%eax,%edx),%bl
	movb $0,(%eax,%edx)
	pushl -12(%ebp)
	pushl $.LC12
	pushl $1
	pushl $2
	call error
	movl -12(%ebp),%edx
	movl -8(%ebp),%eax
	movb %bl,(%eax,%edx)
	addl $16,%esp
.L519:
	movl input_buff,%edx
.L396:
	movl input_pos,%ebx
	cmpb $0,(%ebx)
	je .L482
	.p2align 4,,7
.L485:
	cmpl $4095,%esi
	jg .L514
	movb (%ebx),%al
	movb %al,(%esi,%edx)
	incl input_pos
	incl %esi
	movl input_pos,%ebx
	movl input_buff,%edx
	cmpb $0,(%ebx)
	jne .L485
.L482:
	cmpl $4095,%esi
	jle .L486
.L514:
	movl $1,-20(%ebp)
	movb $0,4095(%edx)
	jmp .L487
	.p2align 4,,7
.L486:
	movb $0,(%esi,%edx)
.L487:
	movl -24(%ebp),%esi
	movl %esi,-36(%ebp)
	movl input_buff,%eax
	addl %eax,-36(%ebp)
	movl -36(%ebp),%eax
	pushl %eax
	movl -32(%ebp),%eax
	movl %eax,input_pos
	pushl %eax
	call strcpy
	addl $8,%esp
	jmp .L517
	.p2align 4,,7
.L479:
	movl -12(%ebp),%edx
	movl -8(%ebp),%eax
	movb (%eax,%edx),%bl
	movb $0,(%eax,%edx)
	pushl -12(%ebp)
	pushl $.LC13
	pushl $1
	pushl $2
	call error
	movl -12(%ebp),%edx
	movl -8(%ebp),%eax
	movb %bl,(%eax,%edx)
	movl input_buff,%eax
	movb $36,(%esi,%eax)
	incl %esi
	addl $16,%esp
.L517:
	movl input_pos,%ebx
.L323:
	cmpb $0,(%ebx)
	jne .L510
.L324:
	cmpl $4095,%esi
	jg .L337
	cmpl $0,-20(%ebp)
	je .L354
.L337:
	movl input_buff,%eax
	movl %eax,input_pos
	movb $0,4095(%eax)
	pushl $.LC14
	pushl $1
	pushl $3
	call error
	xorl %eax,%eax
	jmp .L513
	.p2align 4,,7
.L354:
	movl input_buff,%eax
	movl %eax,input_pos
	movb $0,(%esi,%eax)
	movl $1,%eax
.L513:
	leal -48(%ebp),%esp
	popl %ebx
	popl %esi
	popl %edi
	leave
	ret
.Lfe24:
	.size	 inputArgSub,.Lfe24-inputArgSub
.section	.rodata
.LC15:
	.string	"*p = '%c', isalnum = %d\n"
	.align 32
.LC16:
	.string	"*p = '%c' (isalnum = %d, alpha = %d)\n"
.text
	.align 4
.globl inputSymbol
	.type	 inputSymbol,@function
inputSymbol:
	pushl %ebp
	movl %esp,%ebp
	subl $12,%esp
	pushl %edi
	pushl %esi
	pushl %ebx
	movl 12(%ebp),%eax
	movb %al,-5(%ebp)
	movl input_pos,%ebx
	movl %ebx,%esi
	testb %al,%al
	je .L521
	cmpl $0,objasm
	je .L522
	cmpb $39,%al
	je .L545
	cmpb $34,%al
	jne .L522
	jmp .L524
	.p2align 4,,7
.L545:
	incl %ebx
.L524:
	movsbl (%ebx),%edx
	testl %edx,%edx
	je .L537
	movsbl -5(%ebp),%eax
	.p2align 4,,7
.L529:
	cmpl %eax,%edx
	je .L537
	incl %ebx
	movsbl (%ebx),%edx
	testl %edx,%edx
	je .L537
	jmp .L529
	.p2align 4,,7
.L522:
	movsbl (%ebx),%edx
	testl %edx,%edx
	je .L537
	movsbl -5(%ebp),%eax
	.p2align 4,,7
.L536:
	cmpl %eax,%edx
	je .L537
	incl %ebx
	cmpl $92,%edx
	jne .L531
	cmpb $0,(%ebx)
	je .L531
	incl %ebx
.L531:
	movsbl (%ebx),%edx
	testl %edx,%edx
	je .L537
	jmp .L536
	.p2align 4,,7
.L521:
	leal -1(%ebx),%ecx
	movl %ecx,-12(%ebp)
	leal 2(%ebx),%esi
	jmp .L538
	.p2align 4,,7
.L544:
	cmpb $95,%al
	je .L539
	movsbl (%esi),%eax
	testb $4,1(%edx,%eax,2)
	jne .L539
.L540:
	movl -4(%ebp),%edi
	movw (%edx,%edi,2),%ax
	andl $8,%eax
	andl $65535,%eax
	pushl %eax
	movsbl (%ebx),%eax
	pushl %eax
	pushl $.LC15
	call printf
	incl %esi
	incl -12(%ebp)
	incl %ebx
	addl $12,%esp
	movl -12(%ebp),%ecx
	cmpb $92,(%ecx)
	jne .L538
	cmpb $0,(%ebx)
	je .L539
	incl %esi
	incl %ecx
	movl %ecx,-12(%ebp)
	incl %ebx
.L538:
	movb (%ebx),%al
	testb %al,%al
	je .L539
	movsbl %al,%edi
	movl %edi,-4(%ebp)
	movl __ctype_b,%edx
	testb $8,(%edx,%edi,2)
	jne .L540
	cmpb $95,%al
	je .L540
	cmpl $0,local
	je .L539
	cmpb $36,%al
	jne .L539
	movb -1(%esi),%al
	cmpb $108,%al
	je .L542
	cmpb $76,%al
	jne .L539
.L542:
	movb (%esi),%al
	cmpb $46,%al
	jne .L544
.L539:
	movsbl (%ebx),%eax
	movl __ctype_b,%edx
	movw (%edx,%eax,2),%ax
	andl $1024,%eax
	andl $65535,%eax
	pushl %eax
	movsbl (%ebx),%eax
	movw (%edx,%eax,2),%ax
	andl $8,%eax
	andl $65535,%eax
	pushl %eax
	movsbl (%ebx),%eax
	pushl %eax
	pushl $.LC16
	call printf
	movl input_pos,%esi
.L537:
	movl %ebx,%edi
	subl %esi,%edi
	movl 8(%ebp),%ecx
	movl %edi,(%ecx)
	movl %ebx,input_pos
	subl (%ecx),%ebx
	movl %ebx,%eax
	leal -24(%ebp),%esp
	popl %ebx
	popl %esi
	popl %edi
	leave
	ret
.Lfe25:
	.size	 inputSymbol,.Lfe25-inputSymbol
	.align 4
.globl inputThisInstead
	.type	 inputThisInstead,@function
inputThisInstead:
	pushl %ebp
	movl %esp,%ebp
	pushl 8(%ebp)
	movl input_buff,%eax
	movl %eax,input_mark
	movl %eax,input_pos
	pushl %eax
	call strcpy
	leave
	ret
.Lfe26:
	.size	 inputThisInstead,.Lfe26-inputThisInstead
	.comm	inputLineNo,4,4
	.comm	asmfile,4,4
	.local	workBuff
	.comm	workBuff,4097,32
	.comm	input_pos,4,4
	.comm	input_mark,4,4
	.ident	"GCC: (GNU) egcs-2.91.66 19990314/Linux (egcs-1.1.2 release)"
