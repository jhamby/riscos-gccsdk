		GET	Regs.s

		AREA	|C$$code|,CODE,READONLY

		EXPORT	OS_ConvertDataAndTime

		IMPORT	|__errno|

OS_ConvertDataAndTime
		STMFD	sp!,{r4,lr}
		STMFD	sp!,{r0}
		LDR	r1,[r0,#4]
		LDR	r2,[r0,#8]
		LDR	r3,[r0,#12]
		LDR	r0,[r0]
		SWI	&200C1
		LDMFD	sp!,{r4}
		STR	r0,[r4]
		STR	r1,[r4,#4]
		STR	r2,[r4,#8]
		STR	r3,[r4,#12]
		MOVVC	r0, #0
		LDMFD	sp!,{r4,pc}

		END
