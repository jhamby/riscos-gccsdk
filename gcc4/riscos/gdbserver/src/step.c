#include "step.h"
#include "utils.h"

#define _ROR(x, n) (((x) >> (n)) | ((x) << (32 - (n))))

static uint32_t step_nv(uint32_t instruction, const cpu_registers *regs);

static uint32_t step_data_processing(uint32_t instruction, 
		const cpu_registers *regs);
static uint32_t step_misc_non_dp(uint32_t instruction, 
		const cpu_registers *regs);
static uint32_t step_mul_ld_st(uint32_t instruction, 
		const cpu_registers *regs);

static uint32_t step_ldr_str(uint32_t instruction, const cpu_registers *regs);
static uint32_t step_ldm_stm(uint32_t instruction, const cpu_registers *regs);
static uint32_t step_branch(uint32_t instruction, const cpu_registers *regs);
static uint32_t step_coprocessor(uint32_t instruction, 
		const cpu_registers *regs);

static uint32_t shift_value(uint32_t value, uint32_t type, uint32_t amount,
		uint32_t carry, uint32_t reg);

/**
 * Simulate a single-stepped instruction
 *
 * \param instruction  Instruction to simulate (assumed at regs->r[15])
 * \param regs         Current register states
 * \return Address of instruction after the one being stepped
 *
 * This does not actually execute (or emulate) the instruction, it simply
 * determines the PC-related side effects of it. This then allows the caller
 * to place a one-shot breakpoint at the returned address and run the CPU
 * normally.
 */
uint32_t step_instruction(uint32_t instruction, const cpu_registers *regs)
{
	enum { EQ, NE, CS, CC, MI, PL, VS, VC, 
	       HI, LS, GE, LT, GT, LE, AL, NV };

	/* Mask bits for status flags affecting condition codes */
	static const uint8_t cond_masks[16] = {
		0x4, 0x4, 0x2, 0x2, 0x8, 0x8, 0x1, 0x1,
		0x6, 0x6, 0x9, 0x9, 0xd, 0xd, 0x0, 0x0
	};

	/* Expected values for the first 8 condition codes */
	static const uint8_t cond_vals[8] = { 
		0x4, 0x0, 0x2, 0x0, 0x8, 0x0, 0x1, 0x0
	};

	uint32_t execute;
	const uint32_t cond = instruction >> 28;
	const uint32_t flags = (regs->cpsr >> 28) & cond_masks[cond];

	/* Consider the condition code first */
	if (cond < HI) {
		/* EQ, NE, HS/CS, LO/CC, MI, PL, VS, VC */
		execute = (flags == cond_vals[cond]);
	} else if (cond < GE) {
		/* HI, LS */
		execute = cond == HI ? (flags == 0x2) : (flags != 0x2);
	} else if (cond < GT) {
		/* GE, LT */
		const uint32_t nv = flags ^ (flags >> 3);

		execute = cond == GE ? (nv == 0) : (nv != 0);
	} else if (cond < AL) {
		/* GT, LE */
		const uint32_t z = flags & 0x4;
		const uint32_t nv = (flags & ~0x4) ^ ((flags & ~0x4) >> 3);

		execute = cond == GT ? (z == 0 && nv == 0) 
				     : (z != 0 || nv != 0);
	} else {
		/* AL, (NV) */
		execute = 1;
	}

	if (execute) {
		const uint32_t type = (instruction >> 24) & 0xf;

		if (cond == NV) {
			/* Magical NV space, most of which is undefined */
			return step_nv(instruction, regs);
		}

		if (type < 0x4)		
			return step_data_processing(instruction, regs);
		else if (type < 0x7)
			return step_ldr_str(instruction, regs);
		else if (type < 0x9)
			return step_ldm_stm(instruction, regs);
		else if (type < 0xc)
			return step_branch(instruction, regs);
		else if (type < 0xf)
			return step_coprocessor(instruction, regs);
	}

	/* Either unexecuted or an SWI. Either way, just increment the PC. */
	return regs->r[15] + 4;
}

uint32_t step_nv(uint32_t instruction, const cpu_registers *regs)
{
	/* 1111 xxxx xxxx xxxx xxxx xxxx xxxx xxxx */

	const uint32_t type = (instruction >> 25) & 0x7;

	if (type == 5) {
		/* BLX (imm) */
		int32_t imm = (instruction & ~0xff000000);

		imm = (imm << 8) >> (8 - 2);
		imm |= (instruction >> 23) & 0x2;

		return regs->r[15] + 8 + imm;
	}

	return regs->r[15] + 4;
}

uint32_t step_data_processing(uint32_t instruction, const cpu_registers *regs)
{
	/* xxxx 000o oooS nnnn dddd aaaa att0 mmmm - immediate shift
	 *      000o oooS nnnn dddd ssss 0tt1 mmmm - register shift
	 *      001o oooS nnnn dddd rrrr iiii iiii - immediate
	 *
	 *      000x xxxx xxxx xxxx xxxx 1xx1 xxxx - mul/load/store
	 *
	 *      0001 0xx0 xxxx xxxx xxxx xxx0 xxxx - misc instructions
	 *      0001 0xx0 xxxx xxxx xxxx 0xx1 xxxx - misc instructions
	 *      0011 0x00 xxxx xxxx xxxx xxxx xxxx - undefined
	 *      0011 0R10 mmmm 0000 rrrr iiii iiii - MSR immediate
	 *
	 *    oooo = opcode
	 *       S = set flags
	 *    nnnn = Rn
	 *    dddd = Rd
	 *   aaaaa = shift amount
	 *    rrrr = rotate amount
	 *    ssss = Rs
	 *      tt = shift type
	 *    mmmm = Rm
	 * iiiiiii = immediate
	 *
	 * TST, TEQ, CMP, CMN => S set. If S clear, then it's an extension.
	 */

	if ((instruction & 0x01900000) == 0x01000000) {
		/* Misc instructions | undefined | MSR immediate */
		return step_misc_non_dp(instruction, regs);
	} else if ((instruction & 0x02000090) == 0x90) {
		/* multiply | load | store */
		return step_mul_ld_st(instruction, regs);
	}

	/* Actually a data processing instruction */
	const uint32_t rd = (instruction >> 12) & 0xf;
	const uint32_t op = (instruction >> 21) & 0xf;
	const uint32_t imm = (instruction >> 24) & 0x2;
	uint32_t opcode2;

	if (rd != 15) {
		return regs->r[15] + 4;
	}

	/* Cannot be TST, TEQ, CMP, CMN, as Rd == 0 for those */

	if (imm) {
		const uint32_t immediate = instruction & 0xff;
		uint32_t rot = (instruction >> 8) & 0xf;

		opcode2 = _ROR(immediate, 2 * rot);
	} else {
		const uint32_t rm = instruction & 0xf;
		const uint32_t rs = (instruction >> 8) & 0xf;
		const uint32_t type = (instruction >> 5) & 0x3;
		const uint32_t reg = instruction & 0x10;
		const uint32_t carry = (regs->cpsr >> 29) & 0x1;

		uint32_t amount;

		if (reg)
			amount = regs->r[rs] & 0xff;
		else
			amount = (instruction >> 7) & 0x1f;

		opcode2 = regs->r[rm];

		if (rm == 15)
			opcode2 += 8;	/* PC + 8 */

		opcode2 = shift_value(opcode2, type, amount, carry, reg);
	}

	enum { AND, EOR, SUB, RSB, ADD, ADC, SBC, RSC, 
	       TST, TEQ, CMP, CMN, ORR, MOV, BIC, MVN };

	if (op == MOV) {
	} else if (op == MVN) {
		opcode2 = ~opcode2;
	} else {
		uint32_t rn = regs->r[(instruction >> 16) & 0xf];
		const uint32_t carry = (regs->cpsr >> 29) & 0x1;

		if (((instruction >> 16) & 0xf) == 15)
			rn += 8;	/* PC + 8 */

		if (op == AND) {
			opcode2 = rn & opcode2;
		} else if (op == EOR) {
			opcode2 = rn ^ opcode2;
		} else if (op == SUB) {
			opcode2 = rn - opcode2;
		} else if (op == RSB) {
			opcode2 = opcode2 - rn;
		} else if (op == ADD) {
			opcode2 = rn + opcode2;
		} else if (op == ADC) {
			opcode2 = rn + opcode2 + carry;
		} else if (op == SBC) {
			opcode2 = rn - opcode2 - ~carry;
		} else if (op == RSC) {
			opcode2 = opcode2 - rn - ~carry;
		} else if (op == ORR) {
			opcode2 = rn | opcode2;
		} else if (op == BIC) {
			opcode2 = rn & ~opcode2;
		}
	}

	return opcode2;
}

uint32_t step_misc_non_dp(uint32_t instruction, const cpu_registers *regs)
{
	/* xxxx 0001 0xx0 xxxx xxxx xxxx xxx0 xxxx - misc instructions
	 *      0001 0xx0 xxxx xxxx xxxx 0xx1 xxxx - misc instructions
	 *      0011 0x00 xxxx xxxx xxxx xxxx xxxx - undefined
	 *      0011 0R10 mmmm 0000 rrrr iiii iiii - MSR immediate
	 */

	const uint32_t type = (instruction >> 4) & 0xf;

	if ((type == 0x1 || type == 0x2) && 
			((instruction >> 20) & 0xf) == 0x2) {
		/* BX | BLX (reg) */
		uint32_t rm = regs->r[instruction & 0xf];

		if ((instruction & 0xf) == 15)
			rm += 8;

		return rm & ~0x1;
	}

	/* Don't care about the rest */
	return regs->r[15] + 4;
}

uint32_t step_mul_ld_st(uint32_t instruction, const cpu_registers *regs)
{
	/* xxxx 000x xxxx xxxx xxxx xxxx 1xx1 xxxx */

	UNUSED(instruction);

	/* None of these affect PC */
	return regs->r[15] + 4;
}

uint32_t step_ldr_str(uint32_t instruction, const cpu_registers *regs)
{
	/* xxxx 010P UBWL nnnn dddd iiii iiii iiii - immediate offset
	 *      011P UBWL nnnn dddd aaaa att0 mmmm - register offset
	 *      011x xxxx xxxx xxxx xxxx xxx1 xxxx - undefined
	 */

	if ((instruction & 0x02000010) == 0x02000010) {
		/* Undefined */
		return regs->r[15] + 4;
	}

	if ((instruction & 0x00500000) != 0x00100000 ||
			(instruction & 0x01200000) == 0x00200000) {
		/* Anything other than LDR */
		return regs->r[15] + 4;
	}

	const uint32_t rd = (instruction >> 12) & 0xf;

	if (rd != 15)
		return regs->r[15] + 4;

	const uint32_t reg = (instruction >> 24) & 0x2;
	uint32_t rn = regs->r[(instruction >> 16) & 0xf];
	uint32_t offset, address;

	if (((instruction >> 16) & 0xf) == 15)
		rn += 8;	/* PC + 8 */

	if (reg) {
		const uint32_t amount = (instruction >> 7) & 0x1f;
		const uint32_t type = (instruction >> 5) & 0x3;
		const uint32_t rm = regs->r[(instruction & 0xf)];

		if (amount != 0) {
			/* Shifted register offset */
			offset = shift_value(rm, type, amount, 
					(regs->cpsr >> 29) & 0x1, 0);
		} else {
			/* Unshifted register offset */
			offset = rm;
		}
	} else {
		/* Immediate offset */
		offset = (instruction & 0xfff);
	}

	const uint32_t p = (instruction >> 24) & 0x1;

	if (p) {
		/* Pre-indexed */
		const uint32_t u = (instruction >> 23) & 0x1;

		if (u)
			address = rn + offset;
		else
			address = rn - offset;
	} else {
		/* Post-indexed */
		address = rn;
	}

	/** \todo this may abort */
	address = *((uint32_t *) address);

	return address & ~0x3;
}

uint32_t step_ldm_stm(uint32_t instruction, const cpu_registers *regs)
{
	/* xxxx 100P USWL nnnn rrrr rrrr rrrr rrrr */

	enum { DA, IA, DB, IB };

	if (((instruction >> 20) & 0x1) == 0 || 
			((instruction >> 15) & 0x1) == 0) {
		/* STM, or LDM without PC */
		return regs->r[15] + 4;
	}

	const uint32_t rn = regs->r[(instruction >> 16) & 0xf];
	const uint32_t pu = (instruction >> 23) & 0x3;
	uint32_t rbits = (instruction & 0xffff);
	uint32_t n_regs = 0;
	uint32_t address;

	/* Count number of registers being loaded */
	for (; rbits > 0; rbits >>= 1) {
		if (rbits & 0x1)
			n_regs++;
	}

	/* Compute end address (which will be pointing to the new PC) */
	if (pu == IA) {
		address = rn + 4 * n_regs - 4;
	} else if (pu == IB) {
		address = rn + 4 * n_regs;
	} else if (pu == DA) {
		address = rn;
	} else {
		address = rn - 4;
	}

	/** \todo this may abort */
	return *((uint32_t *) address) & ~0x1;
}

uint32_t step_branch(uint32_t instruction, const cpu_registers *regs)
{
	/* xxxx 101L oooo oooo oooo oooo oooo oooo */
	int32_t imm = (instruction & ~0xff000000);

	/* Sign extend and shift left 2 bits */
	imm = (imm << 8) >> (8 - 2);

	return regs->r[15] + 8 + imm;
}

uint32_t step_coprocessor(uint32_t instruction, const cpu_registers *regs)
{
	/* xxxx 11xx xxxx xxxx xxxx xxxx xxxx xxxx */

	UNUSED(instruction);

	/* These don't affect PC */
	return regs->r[15] + 4;;
}

uint32_t shift_value(uint32_t value, uint32_t type, uint32_t amount,
		uint32_t carry, uint32_t reg)
{
	enum { LSL, LSR, ASR, ROR, RRX = ROR };

	if (type == LSL) {
		if (amount < 32)
			value <<= amount;
		else
			value = 0;
	} else if (type == LSR) {
		if (reg == 0)
			amount++;

		if (amount < 32)
			value >>= amount;
		else
			value = 0;
	} else if (type == ASR) {
		if (reg == 0)
			amount++;

		if (amount < 32)
			value = ((int32_t) value) >> amount;
		else
			value = value & 0x80000000 ? -1 : 0;
	} else {
		/* ROR/RRX */
		/* Clamp to [0,32] */
		amount &= 0x1f;

		if (reg == 0 && amount == 0) {
			value = (carry << 31) | (value >> 1);
		} else {
			value = _ROR(value, amount);
		}
	}

	return value;
}

