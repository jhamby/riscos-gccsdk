	@ Simple example using GAS assembler
	.file	"asm_gas.s"

	.text

	.global	my_puts_gas
my_puts_gas:
	SWI	2
	MOV	PC, R14

	.end
