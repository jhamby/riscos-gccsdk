	; Simple example using ASASM assembler

	AREA	|Asm$$Code|, CODE

	EXPORT	|my_puts_asasm|
my_puts_asasm
	SWI	2
	MOV	PC, R14

	END
