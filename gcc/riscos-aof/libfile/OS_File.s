		GET	Regs.s

		AREA	|C$$code|,CODE,READONLY

		EXPORT	OS_File

		IMPORT	|__errno|

OS_File
		STMFD	sp!,{r0-r6,lr}
		STMFD	sp!,{r0}
		LDR	r1,[r0,#4]
		LDR	r2,[r0,#8]
		LDR	r3,[r0,#12]
		LDR	r4,[r0,#16]
		LDR	r5,[r0,#20]
		LDR	r0,[r0]
		SWI	&08
		LDMFD	sp!,{r6}
		STR	r0,[r6]
		STR	r1,[r6,#4]
		STR	r2,[r6,#8]
		STR	r3,[r6,#12]
		STR	r4,[r6,#16]
		STR	r5,[r6,#20]
		LDMFD	sp!,{r0-r6,pc}^

		END
