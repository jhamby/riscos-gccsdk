		GET	Regs.s

		AREA	|C$$code|,CODE,READONLY

		EXPORT	OS_Word

		IMPORT	|__errno|

OS_Word
		STMFD	sp!,{r0-r1,lr}
		SWI	&07
		LDMFD	sp!,{r0-r1,pc}^

		END
