		GET	Regs.s

		AREA	|C$$code|,CODE,READONLY

		EXPORT	OS_Word

		IMPORT	|__errno|

OS_Word
		STMFD	sp!,{lr}
		SWI	&20007
		MOVVC	r0, #0
		LDMFD	sp!,{pc}^

		END
