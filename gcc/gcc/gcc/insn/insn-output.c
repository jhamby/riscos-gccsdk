/* Generated automatically by the program `genoutput'
from the machine description file `md'.  */

#include "config.h"
#include "system.h"
#include "flags.h"
#include "rtl.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "real.h"
#include "insn-config.h"

#include "conditions.h"
#include "insn-flags.h"
#include "insn-attr.h"

#include "insn-codes.h"

#include "recog.h"

#include "output.h"

static const char *
output_5 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_5[] = {
    "add%?\t%0, %1, %2",
    "sub%?\t%0, %1, #%n2",
    "#",
  };
  return strings_5[which_alternative];
}

static const char *
output_6 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_6[] = {
    "add%?s\t%0, %1, %2",
    "sub%?s\t%0, %1, #%n2",
  };
  return strings_6[which_alternative];
}

static const char *
output_7 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_7[] = {
    "cmn%?\t%0, %1",
    "cmp%?\t%0, #%n1",
  };
  return strings_7[which_alternative];
}

static const char *
output_8 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_8[] = {
    "add%?s\t%0, %1, %2",
    "sub%?s\t%0, %1, #%n2",
  };
  return strings_8[which_alternative];
}

static const char *
output_9 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_9[] = {
    "add%?s\t%0, %1, %2",
    "sub%?s\t%0, %1, #%n2",
  };
  return strings_9[which_alternative];
}

static const char *
output_10 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_10[] = {
    "cmn%?\t%0, %1",
    "cmp%?\t%0, #%n1",
  };
  return strings_10[which_alternative];
}

static const char *
output_11 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_11[] = {
    "cmn%?\t%0, %1",
    "cmp%?\t%0, #%n1",
  };
  return strings_11[which_alternative];
}

static const char *
output_16 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_16[] = {
    "add%d2\t%0, %1, #1",
    "mov%D2\t%0, %1\n\tadd%d2\t%0, %1, #1",
  };
  return strings_16[which_alternative];
}

static const char *
output_18 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_18[] = {
    "adf%?s\t%0, %1, %2",
    "suf%?s\t%0, %1, #%N2",
  };
  return strings_18[which_alternative];
}

static const char *
output_19 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_19[] = {
    "adf%?d\t%0, %1, %2",
    "suf%?d\t%0, %1, #%N2",
  };
  return strings_19[which_alternative];
}

static const char *
output_20 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_20[] = {
    "adf%?d\t%0, %1, %2",
    "suf%?d\t%0, %1, #%N2",
  };
  return strings_20[which_alternative];
}

static const char *
output_23 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_23[] = {
    "adf%?e\t%0, %1, %2",
    "suf%?e\t%0, %1, #%N2",
  };
  return strings_23[which_alternative];
}

static const char *
output_31 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_31[] = {
    "rsb%?\t%0, %2, %1",
    "#",
  };
  return strings_31[which_alternative];
}

static const char *
output_33 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_33[] = {
    "sub%?s\t%0, %1, %2",
    "rsb%?s\t%0, %2, %1",
  };
  return strings_33[which_alternative];
}

static const char *
output_34 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_34[] = {
    "sub%d2\t%0, %1, #1",
    "mov%D2\t%0, %1\n\tsub%d2\t%0, %1, #1",
  };
  return strings_34[which_alternative];
}

static const char *
output_35 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_35[] = {
    "suf%?s\t%0, %1, %2",
    "rsf%?s\t%0, %2, %1",
  };
  return strings_35[which_alternative];
}

static const char *
output_36 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_36[] = {
    "suf%?d\t%0, %1, %2",
    "rsf%?d\t%0, %2, %1",
  };
  return strings_36[which_alternative];
}

static const char *
output_38 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_38[] = {
    "suf%?d\t%0, %1, %2",
    "rsf%?d\t%0, %2, %1",
  };
  return strings_38[which_alternative];
}

static const char *
output_40 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_40[] = {
    "suf%?e\t%0, %1, %2",
    "rsf%?e\t%0, %2, %1",
  };
  return strings_40[which_alternative];
}

static const char *
output_57 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_57[] = {
    "fdv%?s\t%0, %1, %2",
    "frd%?s\t%0, %2, %1",
  };
  return strings_57[which_alternative];
}

static const char *
output_58 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_58[] = {
    "dvf%?d\t%0, %1, %2",
    "rdf%?d\t%0, %2, %1",
  };
  return strings_58[which_alternative];
}

static const char *
output_62 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_62[] = {
    "dvf%?e\t%0, %1, %2",
    "rdf%?e\t%0, %2, %1",
  };
  return strings_62[which_alternative];
}

static const char *
output_73 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_73[] = {
    "and%?\t%0, %1, %2",
    "bic%?\t%0, %1, #%B2",
    "#",
  };
  return strings_73[which_alternative];
}

static const char *
output_75 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_75[] = {
    "and%?s\t%0, %1, %2",
    "bic%?s\t%0, %1, #%B2",
  };
  return strings_75[which_alternative];
}

static const char *
output_76 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_76[] = {
    "tst%?\t%0, %1",
    "bic%?s\t%3, %0, #%B1",
  };
  return strings_76[which_alternative];
}

static const char *
output_77 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  operands[1] = GEN_INT (((1 << INTVAL (operands[1])) - 1)
			 << INTVAL (operands[2]));
  output_asm_insn ("tst%?\t%0, %1", operands);
  return "";

}

static const char *
output_78 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  operands[2] = GEN_INT (((1 << INTVAL (operands[2])) - 1)
			 << INTVAL (operands[3]));
  output_asm_insn ("ands\t%0, %1, %2", operands);
  return "movne\t%0, #1";

}

static const char *
output_81 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_81[] = {
    "bic%?\t%Q0, %Q1, %2",
    "bic%?\t%Q0, %Q1, %2\n\tmov%?\t%R0, %R1",
  };
  return strings_81[which_alternative];
}

static const char *
output_88 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_88[] = {
    "orr%?\t%Q0, %Q1, %2",
    "orr%?\t%Q0, %Q1, %2\n\tmov%?\t%R0, %R1",
  };
  return strings_88[which_alternative];
}

static const char *
output_91 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_91[] = {
    "orr%?\t%0, %1, %2",
    "#",
  };
  return strings_91[which_alternative];
}

static const char *
output_96 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_96[] = {
    "eor%?\t%Q0, %Q1, %2",
    "eor%?\t%Q0, %Q1, %2\n\tmov%?\t%R0, %R1",
  };
  return strings_96[which_alternative];
}

static const char *
output_103 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_103[] = {
    "cmp\t%1, %2\n\tmovlt\t%0, %2",
    "cmp\t%1, %2\n\tmovge\t%0, %1",
    "cmp\t%1, %2\n\tmovge\t%0, %1\n\tmovlt\t%0, %2",
  };
  return strings_103[which_alternative];
}

static const char *
output_104 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_104[] = {
    "cmp\t%1, %2\n\tmovge\t%0, %2",
    "cmp\t%1, %2\n\tmovlt\t%0, %1",
    "cmp\t%1, %2\n\tmovlt\t%0, %1\n\tmovge\t%0, %2",
  };
  return strings_104[which_alternative];
}

static const char *
output_105 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_105[] = {
    "cmp\t%1, %2\n\tmovcc\t%0, %2",
    "cmp\t%1, %2\n\tmovcs\t%0, %1",
    "cmp\t%1, %2\n\tmovcs\t%0, %1\n\tmovcc\t%0, %2",
  };
  return strings_105[which_alternative];
}

static const char *
output_106 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_106[] = {
    "cmp\t%1, %2\n\tmovcs\t%0, %2",
    "cmp\t%1, %2\n\tmovcc\t%0, %1",
    "cmp\t%1, %2\n\tmovcc\t%0, %1\n\tmovcs\t%0, %2",
  };
  return strings_106[which_alternative];
}

static const char *
output_107 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  operands[3] = gen_rtx (minmax_code (operands[3]), SImode, operands[1],
			 operands[2]);
  output_asm_insn ("cmp\t%1, %2", operands);
  output_asm_insn ("str%d3\t%1, %0", operands);
  output_asm_insn ("str%D3\t%2, %0", operands);
  return "";

}

static const char *
output_108 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

{
  enum rtx_code code = GET_CODE (operands[4]);

  operands[5] = gen_rtx (minmax_code (operands[5]), SImode, operands[2],
			 operands[3]);
  output_asm_insn ("cmp\t%2, %3", operands);
  output_asm_insn ("%i4%d5\t%0, %1, %2", operands);
  if (which_alternative != 0 || operands[3] != const0_rtx
      || (code != PLUS && code != MINUS && code != IOR && code != XOR))
    output_asm_insn ("%i4%D5\t%0, %1, %3", operands);
  return "";
}

}

static const char *
output_126 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_126[] = {
    "cmp\t%0, #0\n\trsblt\t%0, %0, #0",
    "eor%?\t%0, %1, %1, asr #31\n\tsub%?\t%0, %0, %1, asr #31",
  };
  return strings_126[which_alternative];
}

static const char *
output_127 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_127[] = {
    "cmp\t%0, #0\n\trsbgt\t%0, %0, #0",
    "eor%?\t%0, %1, %1, asr #31\n\trsb%?\t%0, %0, %1, asr #31",
  };
  return strings_127[which_alternative];
}

static const char *
output_149 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  if (REGNO (operands[1]) != REGNO (operands[0]) + (WORDS_BIG_ENDIAN ? 1 : 0))
    output_asm_insn ("mov%?\t%Q0, %1", operands);
  return "mov%?\t%R0, #0";

}

static const char *
output_150 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_150[] = {
    "and%?\t%Q0, %1, #255\n\tmov%?\t%R0, #0",
    "ldr%?b\t%Q0, %1\n\tmov%?\t%R0, #0",
  };
  return strings_150[which_alternative];
}

static const char *
output_151 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  if (REGNO (operands[1]) != REGNO (operands[0]) + (WORDS_BIG_ENDIAN ? 1 : 0))
    output_asm_insn ("mov%?\t%Q0, %1", operands);
  return "mov%?\t%R0, %Q0, asr #31";

}

static const char *
output_166 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  /* If the address is invalid, this will split the instruction into two. */
  if (bad_signed_byte_operand (operands[1], QImode))
    return "#";
  return "ldr%?sb\t%0, %1";

}

static const char *
output_169 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  /* If the address is invalid, this will split the instruction into two. */
  if (bad_signed_byte_operand (operands[1], QImode))
    return "#";
  return "ldr%?sb\t%0, %1";

}

static const char *
output_174 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  return (output_move_double (operands));

}

static const char *
output_176 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_176[] = {
    "mov%?\t%0, %1",
    "mvn%?\t%0, #%B1",
    "ldr%?\t%0, %1",
    "str%?\t%1, %0",
  };
  return strings_176[which_alternative];
}

static const char *
output_182 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

#ifdef AOF_ASSEMBLER
  operands[1] = aof_pic_entry1 (operands[1]);
#endif
  output_asm_insn ("ldr%?\t%0, [%|v6, %1]", operands);
  return "";

}

static const char *
output_183 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  ASM_OUTPUT_INTERNAL_LABEL (asm_out_file, "L",
			     CODE_LABEL_NUMBER (operands[1]));
  return "add%?\t%0, %|pc, %0";

}

static const char *
output_184 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_184[] = {
    "cmp%?\t%0, #0",
    "sub%?s\t%0, %1, #0",
  };
  return strings_184[which_alternative];
}

static const char *
output_190 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

{
  rtx ops[2];

  ops[0] = operands[0];
  ops[1] = gen_rtx_MEM (SImode, plus_constant (XEXP (operands[1], 0), 2));
  output_asm_insn ("ldr%?\t%0, %1\t%@ load-rotate", ops);
  return "";
}
}

static const char *
output_193 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_193[] = {
    "mov%?\t%0, %1\t%@ movhi",
    "mvn%?\t%0, #%B1\t%@ movhi",
    "ldr%?h\t%0, %1\t%@ movhi",
    "str%?h\t%1, %0\t%@ movhi",
  };
  return strings_193[which_alternative];
}

static const char *
output_194 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_194[] = {
    "mov%?\t%0, %1\t%@ movhi",
    "mvn%?\t%0, #%B1\t%@ movhi",
    "ldr%?\t%0, %1\t%@ movhi",
  };
  return strings_194[which_alternative];
}

static const char *
output_195 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_195[] = {
    "mov%?\t%0, %1\t%@ movhi",
    "mvn%?\t%0, #%B1\t%@ movhi",
    "ldr%?\t%0, %1\t%@ movhi_bigend\n\tmov%?\t%0, %0, asr #16",
  };
  return strings_195[which_alternative];
}

static const char *
output_197 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_197[] = {
    "mov%?\t%0, %1\t%@ movhi",
    "mvn%?\t%0, #%B1\t%@ movhi",
  };
  return strings_197[which_alternative];
}

static const char *
output_201 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_201[] = {
    "mov%?\t%0, %1",
    "mvn%?\t%0, #%B1",
    "ldr%?b\t%0, %1",
    "str%?b\t%1, %0",
  };
  return strings_201[which_alternative];
}

static const char *
output_203 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_203[] = {
    "mvf%?s\t%0, %1",
    "mnf%?s\t%0, #%N1",
    "ldf%?s\t%0, %1",
    "stf%?s\t%1, %0",
    "str%?\t%1, [%|sp, #-4]!\n\tldf%?s\t%0, [%|sp], #4",
    "stf%?s\t%1, [%|sp, #-4]!\n\tldr%?\t%0, [%|sp], #4",
    "mov%?\t%0, %1",
    "ldr%?\t%0, %1\t%@ float",
    "str%?\t%1, %0\t%@ float",
  };
  return strings_203[which_alternative];
}

static const char *
output_204 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_204[] = {
    "mov%?\t%0, %1",
    "ldr%?\t%0, %1\t%@ float",
    "str%?\t%1, %0\t%@ float",
  };
  return strings_204[which_alternative];
}

static const char *
output_207 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

{
  switch (which_alternative)
    {
    default:
    case 0: return "ldm%?ia\t%m1, %M0\t%@ double";
    case 1: return "stm%?ia\t%m0, %M1\t%@ double";
    case 2: case 3: case 4: return output_move_double (operands);
    case 5: return "mvf%?d\t%0, %1";
    case 6: return "mnf%?d\t%0, #%N1";
    case 7: return "ldf%?d\t%0, %1";
    case 8: return "stf%?d\t%1, %0";
    case 9: return output_mov_double_fpu_from_arm (operands);
    case 10: return output_mov_double_arm_from_fpu (operands);
    }
}

}

static const char *
output_208 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
 return output_move_double (operands);
}

static const char *
output_210 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  switch (which_alternative)
    {
    default:
    case 0: return "mvf%?e\t%0, %1";
    case 1: return "mnf%?e\t%0, #%N1";
    case 2: return "ldf%?e\t%0, %1";
    case 3: return "stf%?e\t%1, %0";
    case 4: return output_mov_long_double_fpu_from_arm (operands);
    case 5: return output_mov_long_double_arm_from_fpu (operands);
    case 6: return output_mov_long_double_arm_from_arm (operands);
    }

}

static const char *
output_212 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

{
  rtx ops[3];
  int count = XVECLEN (operands[0], 0);

  ops[0] = XEXP (SET_SRC (XVECEXP (operands[0], 0, 0)), 0);
  ops[1] = SET_DEST (XVECEXP (operands[0], 0, 1));
  ops[2] = SET_DEST (XVECEXP (operands[0], 0, count - 2));

  output_asm_insn ("ldm%?ia\t%0!, {%1-%2}\t%@ load multiple", ops);
  return "";
}

}

static const char *
output_213 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

{
  rtx ops[3];
  int count = XVECLEN (operands[0], 0);

  ops[0] = XEXP (SET_SRC (XVECEXP (operands[0], 0, 0)), 0);
  ops[1] = SET_DEST (XVECEXP (operands[0], 0, 0));
  ops[2] = SET_DEST (XVECEXP (operands[0], 0, count - 1));

  output_asm_insn ("ldm%?ia\t%0, {%1-%2}\t%@ load multiple", ops);
  return "";
}

}

static const char *
output_215 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

{
  rtx ops[3];
  int count = XVECLEN (operands[0], 0);

  ops[0] = XEXP (SET_SRC (XVECEXP (operands[0], 0, 0)), 0);
  ops[1] = SET_SRC (XVECEXP (operands[0], 0, 1));
  ops[2] = SET_SRC (XVECEXP (operands[0], 0, count - 2));

  output_asm_insn ("stm%?ia\t%0!, {%1-%2}\t%@ str multiple", ops);
  return "";
}

}

static const char *
output_216 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

{
  rtx ops[3];
  int count = XVECLEN (operands[0], 0);

  ops[0] = XEXP (SET_DEST (XVECEXP (operands[0], 0, 0)), 0);
  ops[1] = SET_SRC (XVECEXP (operands[0], 0, 0));
  ops[2] = SET_SRC (XVECEXP (operands[0], 0, count - 1));

  output_asm_insn ("stm%?ia\t%0, {%1-%2}\t%@ str multiple", ops);
  return "";
}

}

static const char *
output_222 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_222[] = {
    "cmp%?\t%0, %1",
    "cmn%?\t%0, #%n1",
  };
  return strings_222[which_alternative];
}

static const char *
output_226 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_226[] = {
    "cmf%?\t%0, %1",
    "cnf%?\t%0, #%N1",
  };
  return strings_226[which_alternative];
}

static const char *
output_227 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_227[] = {
    "cmf%?\t%0, %1",
    "cnf%?\t%0, #%N1",
  };
  return strings_227[which_alternative];
}

static const char *
output_228 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_228[] = {
    "cmf%?\t%0, %1",
    "cnf%?\t%0, #%N1",
  };
  return strings_228[which_alternative];
}

static const char *
output_230 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_230[] = {
    "cmf%?\t%0, %1",
    "cnf%?\t%0, #%N1",
  };
  return strings_230[which_alternative];
}

static const char *
output_231 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_231[] = {
    "cmf%?e\t%0, %1",
    "cnf%?e\t%0, #%N1",
  };
  return strings_231[which_alternative];
}

static const char *
output_232 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_232[] = {
    "cmf%?e\t%0, %1",
    "cnf%?e\t%0, #%N1",
  };
  return strings_232[which_alternative];
}

static const char *
output_233 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_233[] = {
    "cmf%?e\t%0, %1",
    "cnf%?e\t%0, #%N1",
  };
  return strings_233[which_alternative];
}

static const char *
output_235 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_235[] = {
    "cmf%?e\t%0, %1",
    "cnf%?e\t%0, #%N1",
  };
  return strings_235[which_alternative];
}

static const char *
output_247 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

{
  extern int arm_ccfsm_state;

  if (arm_ccfsm_state == 1 || arm_ccfsm_state == 2)
  {
    arm_ccfsm_state += 2;
    return "";
  }
  return "b%d1\t%l0";
}
}

static const char *
output_248 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

{
  extern int arm_ccfsm_state;

  if (arm_ccfsm_state == 1 || arm_ccfsm_state == 2)
  {
    arm_ccfsm_state += 2;
    return "";
  }
  return "b%D1\t%l0";
}
}

static const char *
output_265 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_265[] = {
    "mov%D3\t%0, %2",
    "mvn%D3\t%0, #%B2",
    "mov%d3\t%0, %1",
    "mvn%d3\t%0, #%B1",
    "mov%d3\t%0, %1\n\tmov%D3\t%0, %2",
    "mov%d3\t%0, %1\n\tmvn%D3\t%0, #%B2",
    "mvn%d3\t%0, #%B1\n\tmov%D3\t%0, %2",
    "mvn%d3\t%0, #%B1\n\tmvn%D3\t%0, #%B2",
  };
  return strings_265[which_alternative];
}

static const char *
output_266 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_266[] = {
    "mvf%D3s\t%0, %2",
    "mnf%D3s\t%0, #%N2",
    "mvf%d3s\t%0, %1",
    "mnf%d3s\t%0, #%N1",
    "mvf%d3s\t%0, %1\n\tmvf%D3s\t%0, %2",
    "mvf%d3s\t%0, %1\n\tmnf%D3s\t%0, #%N2",
    "mnf%d3s\t%0, #%N1\n\tmvf%D3s\t%0, %2",
    "mnf%d3s\t%0, #%N1\n\tmnf%D3s\t%0, #%N2",
  };
  return strings_266[which_alternative];
}

static const char *
output_267 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_267[] = {
    "mov%D3\t%0, %2",
    "mov%d3\t%0, %1",
  };
  return strings_267[which_alternative];
}

static const char *
output_268 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_268[] = {
    "mvf%D3d\t%0, %2",
    "mnf%D3d\t%0, #%N2",
    "mvf%d3d\t%0, %1",
    "mnf%d3d\t%0, #%N1",
    "mvf%d3d\t%0, %1\n\tmvf%D3d\t%0, %2",
    "mvf%d3d\t%0, %1\n\tmnf%D3d\t%0, #%N2",
    "mnf%d3d\t%0, #%N1\n\tmvf%D3d\t%0, %2",
    "mnf%d3d\t%0, #%N1\n\tmnf%D3d\t%0, #%N2",
  };
  return strings_268[which_alternative];
}

static const char *
output_269 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

{
  extern int arm_ccfsm_state;

  if (arm_ccfsm_state == 1 || arm_ccfsm_state == 2)
    {
      arm_ccfsm_state += 2;
      return "";
    }
  return "b%?\t%l0";
}
}

static const char *
output_271 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  return output_call (operands);

}

static const char *
output_272 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  return output_call_mem (operands);

}

static const char *
output_274 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  return output_call (&operands[1]);

}

static const char *
output_275 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  return output_call_mem (&operands[1]);

}

static const char *
output_276 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  {
    return NEED_PLT_GOT ? "bl%?\t%a0(PLT)" : "bl%?\t%a0";
  }
}

static const char *
output_277 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  {
    return NEED_PLT_GOT ? "bl%?\t%a1(PLT)" : "bl%?\t%a1";
  }
}

static const char *
output_278 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

{
  extern int arm_ccfsm_state;

  if (arm_ccfsm_state == 2)
  {
    arm_ccfsm_state += 2;
    return "";
  }
  return output_return_instruction (NULL, TRUE, FALSE);
}
}

static const char *
output_279 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

{
  extern int arm_ccfsm_state;

  if (arm_ccfsm_state == 2)
  {
    arm_ccfsm_state += 2;
    return "";
  }
  return output_return_instruction (operands[0], TRUE, FALSE);
}
}

static const char *
output_280 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

{
  extern int arm_ccfsm_state;

  if (arm_ccfsm_state == 2)
  {
    arm_ccfsm_state += 2;
    return "";
  }
  return output_return_instruction (operands[0], TRUE, TRUE);
}
}

static const char *
output_284 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  if (flag_pic)
    return "cmp\t%0, %1\n\taddls\t%|pc, %|pc, %0, asl #2\n\tb\t%l3";
  return "cmp\t%0, %1\n\tldrls\t%|pc, [%|pc, %0, asl #2]\n\tb\t%l3";

}

static const char *
output_294 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  output_asm_insn ("add%?\t%0, %2, %3%S5", operands);
  operands[2] = operands[1];
  operands[1] = operands[0];
  return output_add_immediate (operands);

}

static const char *
output_295 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  output_add_immediate (operands);
  return "add%?s\t%0, %0, %3%S5";

}

static const char *
output_296 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  output_add_immediate (operands);
  return "add%?s\t%0, %0, %3%S5";

}

static const char *
output_297 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  output_asm_insn ("mla%?\t%0, %2, %1, %3", operands);
  operands[2] = operands[4];
  operands[1] = operands[0];
  return output_add_immediate (operands);

}

static const char *
output_298 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  output_add_immediate (operands);
  output_asm_insn ("mla%?s\t%0, %3, %4, %0", operands);
  return "";

}

static const char *
output_299 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  output_add_immediate (operands);
  return "mla%?s\t%0, %3, %4, %0";

}

static const char *
output_301 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_301[] = {
    "orr%d2\t%0, %1, #1",
    "mov%D2\t%0, %1\n\torr%d2\t%0, %1, #1",
  };
  return strings_301[which_alternative];
}

static const char *
output_302 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  if (GET_CODE (operands[1]) == LT && operands[3] == const0_rtx)
    return "mov\t%0, %2, lsr #31";

  if (GET_CODE (operands[1]) == GE && operands[3] == const0_rtx)
    return "mvn\t%0, %2\n\tmov\t%0, %0, lsr #31";

  if (GET_CODE (operands[1]) == NE)
    {
      if (which_alternative == 1)
	return "adds\t%0, %2, #%n3\n\tmovne\t%0, #1";
      return "subs\t%0, %2, %3\n\tmovne\t%0, #1";
    }
  if (which_alternative == 1)
    output_asm_insn ("cmn\t%2, #%n3", operands);
  else
    output_asm_insn ("cmp\t%2, %3", operands);
  return "mov%D1\t%0, #0\n\tmov%d1\t%0, #1";

}

static const char *
output_303 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  if (GET_CODE (operands[3]) == NE)
    {
      if (which_alternative != 1)
	output_asm_insn ("mov%D4\t%0, %2", operands);
      if (which_alternative != 0)
	output_asm_insn ("mov%d4\t%0, %1", operands);
      return "";
    }
  if (which_alternative != 0)
    output_asm_insn ("mov%D4\t%0, %1", operands);
  if (which_alternative != 1)
    output_asm_insn ("mov%d4\t%0, %2", operands);
  return "";

}

static const char *
output_304 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  if (GET_CODE (operands[4]) == LT && operands[3] == const0_rtx)
    return "%i5\t%0, %1, %2, lsr #31";

  output_asm_insn ("cmp\t%2, %3", operands);
  if (GET_CODE (operands[5]) == AND)
    output_asm_insn ("mov%D4\t%0, #0", operands);
  else if (GET_CODE (operands[5]) == MINUS)
    output_asm_insn ("rsb%D4\t%0, %1, #0", operands);
  else if (which_alternative != 0)
    output_asm_insn ("mov%D4\t%0, %1", operands);
  return "%i5%d4\t%0, %1, #1";

}

static const char *
output_305 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  output_asm_insn ("cmp\t%2, %3", operands);
  if (which_alternative != 0)
    output_asm_insn ("mov%D4\t%0, %1", operands);
  return "sub%d4\t%0, %1, #1";

}

static const char *
output_306 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

{
  char* opcodes[4][2] =
  {
    {"cmp\t%2, %3\n\tcmp%d5\t%0, %1","cmp\t%0, %1\n\tcmp%d4\t%2, %3"},
    {"cmp\t%2, %3\n\tcmn%d5\t%0, #%n1", "cmn\t%0, #%n1\n\tcmp%d4\t%2, %3"},
    {"cmn\t%2, #%n3\n\tcmp%d5\t%0, %1", "cmp\t%0, %1\n\tcmn%d4\t%2, #%n3"},
    {"cmn\t%2, #%n3\n\tcmn%d5\t%0, #%n1",
     "cmn\t%0, #%n1\n\tcmn%d4\t%2, #%n3"}
  };
  int swap =
    comparison_dominates_p (GET_CODE (operands[5]), GET_CODE (operands[4]));

  return opcodes[which_alternative][swap];
}

}

static const char *
output_307 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

{
  char* opcodes[4][2] =
  {
    {"cmp\t%0, %1\n\tcmp%d4\t%2, %3", "cmp\t%2, %3\n\tcmp%D5\t%0, %1"},
    {"cmn\t%0, #%n1\n\tcmp%d4\t%2, %3", "cmp\t%2, %3\n\tcmn%D5\t%0, #%n1"},
    {"cmp\t%0, %1\n\tcmn%d4\t%2, #%n3", "cmn\t%2, #%n3\n\tcmp%D5\t%0, %1"},
    {"cmn\t%0, #%n1\n\tcmn%d4\t%2, #%n3",
     "cmn\t%2, #%n3\n\tcmn%D5\t%0, #%n1"}
  };
  int swap =
    comparison_dominates_p (GET_CODE (operands[5]),
			    reverse_condition (GET_CODE (operands[4])));

  return opcodes[which_alternative][swap];
}

}

static const char *
output_308 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  if (GET_CODE (operands[3]) == LT && operands[3] == const0_rtx)
    return "mov\t%0, %1, asr #31";

  if (GET_CODE (operands[3]) == NE)
    return "subs\t%0, %1, %2\n\tmvnne\t%0, #0";

  if (GET_CODE (operands[3]) == GT)
    return "subs\t%0, %1, %2\n\tmvnne\t%0, %0, asr #31";

  output_asm_insn ("cmp\t%1, %2", operands);
  output_asm_insn ("mov%D3\t%0, #0", operands);
  return "mvn%d3\t%0, #0";

}

static const char *
output_309 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  if (GET_CODE (operands[5]) == LT
      && (operands[4] == const0_rtx))
    {
      if (which_alternative != 1 && GET_CODE (operands[1]) == REG)
	{
	  if (operands[2] == const0_rtx)
	    return "and\t%0, %1, %3, asr #31";
	  return "ands\t%0, %1, %3, asr #32\n\tmovcc\t%0, %2";
	}
      else if (which_alternative != 0 && GET_CODE (operands[2]) == REG)
	{
	  if (operands[1] == const0_rtx)
	    return "bic\t%0, %2, %3, asr #31";
	  return "bics\t%0, %2, %3, asr #32\n\tmovcs\t%0, %1";
	}
      /* The only case that falls through to here is when both ops 1 & 2
	 are constants */
    }

  if (GET_CODE (operands[5]) == GE
      && (operands[4] == const0_rtx))
    {
      if (which_alternative != 1 && GET_CODE (operands[1]) == REG)
	{
	  if (operands[2] == const0_rtx)
	    return "bic\t%0, %1, %3, asr #31";
	  return "bics\t%0, %1, %3, asr #32\n\tmovcs\t%0, %2";
	}
      else if (which_alternative != 0 && GET_CODE (operands[2]) == REG)
	{
	  if (operands[1] == const0_rtx)
	    return "and\t%0, %2, %3, asr #31";
	  return "ands\t%0, %2, %3, asr #32\n\tmovcc\t%0, %1";
	}
      /* The only case that falls through to here is when both ops 1 & 2
	 are constants */
    }
  if (GET_CODE (operands[4]) == CONST_INT
      && !const_ok_for_arm (INTVAL (operands[4])))
    output_asm_insn ("cmn\t%3, #%n4", operands);
  else
    output_asm_insn ("cmp\t%3, %4", operands);
  if (which_alternative != 0)
    output_asm_insn ("mov%d5\t%0, %1", operands);
  if (which_alternative != 1)
    output_asm_insn ("mov%D5\t%0, %2", operands);
  return "";

}

static const char *
output_311 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_311[] = {
    "add%d4\t%0, %2, %3",
    "sub%d4\t%0, %2, #%n3",
    "add%d4\t%0, %2, %3\n\tmov%D4\t%0, %1",
    "sub%d4\t%0, %2, #%n3\n\tmov%D4\t%0, %1",
  };
  return strings_311[which_alternative];
}

static const char *
output_313 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_313[] = {
    "add%D4\t%0, %2, %3",
    "sub%D4\t%0, %2, #%n3",
    "add%D4\t%0, %2, %3\n\tmov%d4\t%0, %1",
    "sub%D4\t%0, %2, #%n3\n\tmov%d4\t%0, %1",
  };
  return strings_313[which_alternative];
}

static const char *
output_316 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  /* If we have an operation where (op x 0) is the identity operation and
     the conditional operator is LT or GE and we are comparing against zero and
     everything is in registers then we can do this in two instructions */
  if (operands[3] == const0_rtx
      && GET_CODE (operands[7]) != AND
      && GET_CODE (operands[5]) == REG
      && GET_CODE (operands[1]) == REG 
      && REGNO (operands[1]) == REGNO (operands[4])
      && REGNO (operands[4]) != REGNO (operands[0]))
    {
      if (GET_CODE (operands[6]) == LT)
	return "and\t%0, %5, %2, asr #31\n\t%I7\t%0, %4, %0";
      else if (GET_CODE (operands[6]) == GE)
	return "bic\t%0, %5, %2, asr #31\n\t%I7\t%0, %4, %0";
    }
  if (GET_CODE (operands[3]) == CONST_INT
      && !const_ok_for_arm (INTVAL (operands[3])))
    output_asm_insn ("cmn\t%2, #%n3", operands);
  else
    output_asm_insn ("cmp\t%2, %3", operands);
  output_asm_insn ("%I7%d6\t%0, %4, %5", operands);
  if (which_alternative != 0)
    return "mov%D6\t%0, %1";
  return "";

}

static const char *
output_317 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_317[] = {
    "%I5%d4\t%0, %2, %3",
    "%I5%d4\t%0, %2, %3\n\tmov%D4\t%0, %1",
  };
  return strings_317[which_alternative];
}

static const char *
output_318 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  /* If we have an operation where (op x 0) is the identity operation and
     the conditional operator is LT or GE and we are comparing against zero and
     everything is in registers then we can do this in two instructions */
  if (operands[5] == const0_rtx
      && GET_CODE (operands[7]) != AND
      && GET_CODE (operands[3]) == REG
      && GET_CODE (operands[1]) == REG 
      && REGNO (operands[1]) == REGNO (operands[2])
      && REGNO (operands[2]) != REGNO (operands[0]))
    {
      if (GET_CODE (operands[6]) == GE)
	return "and\t%0, %3, %4, asr #31\n\t%I7\t%0, %2, %0";
      else if (GET_CODE (operands[6]) == LT)
	return "bic\t%0, %3, %4, asr #31\n\t%I7\t%0, %2, %0";
    }

  if (GET_CODE (operands[5]) == CONST_INT
      && !const_ok_for_arm (INTVAL (operands[5])))
    output_asm_insn ("cmn\t%4, #%n5", operands);
  else
    output_asm_insn ("cmp\t%4, %5", operands);

  if (which_alternative != 0)
    output_asm_insn ("mov%d6\t%0, %1", operands);
  return "%I7%D6\t%0, %2, %3";

}

static const char *
output_319 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_319[] = {
    "%I5%D4\t%0, %2, %3",
    "%I5%D4\t%0, %2, %3\n\tmov%d4\t%0, %1",
  };
  return strings_319[which_alternative];
}

static const char *
output_321 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_321[] = {
    "mvn%D4\t%0, %2",
    "mov%d4\t%0, %1\n\tmvn%D4\t%0, %2",
    "mvn%d4\t%0, #%B1\n\tmvn%D4\t%0, %2",
  };
  return strings_321[which_alternative];
}

static const char *
output_323 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_323[] = {
    "mvn%d4\t%0, %2",
    "mov%D4\t%0, %1\n\tmvn%d4\t%0, %2",
    "mvn%D4\t%0, #%B1\n\tmvn%d4\t%0, %2",
  };
  return strings_323[which_alternative];
}

static const char *
output_325 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_325[] = {
    "mov%d5\t%0, %2%S4",
    "mov%D5\t%0, %1\n\tmov%d5\t%0, %2%S4",
    "mvn%D5\t%0, #%B1\n\tmov%d5\t%0, %2%S4",
  };
  return strings_325[which_alternative];
}

static const char *
output_327 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_327[] = {
    "mov%D5\t%0, %2%S4",
    "mov%d5\t%0, %1\n\tmov%D5\t%0, %2%S4",
    "mvn%d5\t%0, #%B1\n\tmov%D5\t%0, %2%S4",
  };
  return strings_327[which_alternative];
}

static const char *
output_335 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_335[] = {
    "rsb%d4\t%0, %2, #0",
    "mov%D4\t%0, %1\n\trsb%d4\t%0, %2, #0",
    "mvn%D4\t%0, #%B1\n\trsb%d4\t%0, %2, #0",
  };
  return strings_335[which_alternative];
}

static const char *
output_337 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  static const char *const strings_337[] = {
    "rsb%D4\t%0, %2, #0",
    "mov%d4\t%0, %1\n\trsb%D4\t%0, %2, #0",
    "mvn%d4\t%0, #%B1\n\trsb%D4\t%0, %2, #0",
  };
  return strings_337[which_alternative];
}

static const char *
output_338 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

{
  rtx ldm[3];
  rtx arith[4];
  int val1 = 0, val2 = 0;

  if (REGNO (operands[0]) > REGNO (operands[4]))
    {
      ldm[1] = operands[4];
      ldm[2] = operands[0];
    }
  else
    {
      ldm[1] = operands[0];
      ldm[2] = operands[4];
    }
  if (GET_CODE (XEXP (operands[2], 0)) != REG)
    val1 = INTVAL (XEXP (XEXP (operands[2], 0), 1));
  if (GET_CODE (XEXP (operands[3], 0)) != REG)
    val2 = INTVAL (XEXP (XEXP (operands[3], 0), 1));
  arith[0] = operands[0];
  arith[3] = operands[1];
  if (val1 < val2)
    {
      arith[1] = ldm[1];
      arith[2] = ldm[2];
    }
  else
    {
      arith[1] = ldm[2];
      arith[2] = ldm[1];
    }
  if (val1 && val2)
    {
      rtx ops[3];
      ldm[0] = ops[0] = operands[4];
      ops[1] = XEXP (XEXP (operands[2], 0), 0);
      ops[2] = XEXP (XEXP (operands[2], 0), 1);
      output_add_immediate (ops);
      if (val1 < val2)
	output_asm_insn ("ldm%?ia\t%0, {%1, %2}", ldm);
      else
	output_asm_insn ("ldm%?da\t%0, {%1, %2}", ldm);
    }
  else if (val1)
    {
      ldm[0] = XEXP (operands[3], 0);
      if (val1 < val2)
	output_asm_insn ("ldm%?da\t%0, {%1, %2}", ldm);
      else
	output_asm_insn ("ldm%?ia\t%0, {%1, %2}", ldm);
    }
  else
    {
      ldm[0] = XEXP (operands[2], 0);
      if (val1 < val2)
	output_asm_insn ("ldm%?ia\t%0, {%1, %2}", ldm);
      else
	output_asm_insn ("ldm%?da\t%0, {%1, %2}", ldm);
    }
  output_asm_insn ("%I3%?\t%0, %1, %2", arith);
  return "";
}

}

static const char *
output_369 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  return emit_ldm_seq (operands, 4);

}

static const char *
output_370 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  return emit_ldm_seq (operands, 3);

}

static const char *
output_371 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  return emit_ldm_seq (operands, 2);

}

static const char *
output_372 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  return emit_stm_seq (operands, 4);

}

static const char *
output_373 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  return emit_stm_seq (operands, 3);

}

static const char *
output_374 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  return emit_stm_seq (operands, 2);

}

static const char *
output_375 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

{
  extern rtx arm_target_insn;
  extern int arm_ccfsm_state;

  if (arm_ccfsm_state && arm_target_insn && INSN_DELETED_P (arm_target_insn))
  {
    arm_current_cc = ARM_INVERSE_CONDITION_CODE (arm_current_cc);
    output_return_instruction (NULL, TRUE, FALSE);
    arm_ccfsm_state = 0;
    arm_target_insn = NULL;
  }

  output_return_instruction (NULL, FALSE, FALSE);
  return NEED_PLT_GOT ? "b%?\t%a0(PLT)" : "b%?\t%a0";
}
}

static const char *
output_376 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

{
  extern rtx arm_target_insn;
  extern int arm_ccfsm_state;

  if (arm_ccfsm_state && arm_target_insn && INSN_DELETED_P (arm_target_insn))
  {
    arm_current_cc = ARM_INVERSE_CONDITION_CODE (arm_current_cc);
    output_return_instruction (NULL, TRUE, FALSE);
    arm_ccfsm_state = 0;
    arm_target_insn = NULL;
  }

  output_return_instruction (NULL, FALSE, FALSE);
  return NEED_PLT_GOT ? "b%?\t%a1(PLT)" : "b%?\t%a1";
}
}

static const char *
output_377 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

{
  extern rtx arm_target_insn;
  extern int arm_ccfsm_state;

  if (arm_ccfsm_state && arm_target_insn && INSN_DELETED_P (arm_target_insn))
  {
    arm_current_cc = ARM_INVERSE_CONDITION_CODE (arm_current_cc);
    output_return_instruction (NULL, TRUE, FALSE);
    arm_ccfsm_state = 0;
    arm_target_insn = NULL;
  }

  output_return_instruction (NULL, FALSE, FALSE);
  return "b%?\t%a1";
}
}

static const char *
output_382 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  operands[2] = GEN_INT (1 << INTVAL (operands[2]));
  output_asm_insn ("ands\t%0, %1, %2", operands);
  return "mvnne\t%0, #0";

}

static const char *
output_383 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  operands[2] = GEN_INT (1 << INTVAL (operands[2]));
  output_asm_insn ("tst\t%1, %2", operands);
  output_asm_insn ("mvneq\t%0, #0", operands);
  return "movne\t%0, #0";

}

static const char *
output_384 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

{
  char pattern[100];
  int i;
  extern int lr_save_eliminated;

  if (lr_save_eliminated)
    {
      if (XVECLEN (operands[2], 0) > 1)
	abort ();
      return "";
    }
  strcpy (pattern, "stmfd\t%m0!, {%1");
  for (i = 1; i < XVECLEN (operands[2], 0); i++)
    {
      strcat (pattern, ", %|");
      strcat (pattern, reg_names[REGNO (XEXP (XVECEXP (operands[2], 0, i),
					      0))]);
    }
  strcat (pattern, "}");
  output_asm_insn (pattern, operands);
  return "";
}
}

static const char *
output_385 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

{
  char pattern[100];

  sprintf (pattern, "sfmfd\t%%1, %d, [%%m0]!", XVECLEN (operands[2], 0));
  output_asm_insn (pattern, operands);
  return "";
}
}

static const char *
output_386 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

{
  making_const_table = TRUE;
  switch (GET_MODE_CLASS (GET_MODE (operands[0])))
    {
    case MODE_FLOAT:
    {
      union real_extract u;
      bcopy ((char *) &CONST_DOUBLE_LOW (operands[0]), (char *) &u, sizeof u);
      assemble_real (u.d, GET_MODE (operands[0]));
      break;
    }
    default:
      assemble_integer (operands[0], 4, 1);
      break;
    }
  return "";
}
}

static const char *
output_387 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

{
  making_const_table = TRUE;
  switch (GET_MODE_CLASS (GET_MODE (operands[0])))
    {
    case MODE_FLOAT:
    {
      union real_extract u;
      bcopy ((char *) &CONST_DOUBLE_LOW (operands[0]), (char *) &u, sizeof u);
      assemble_real (u.d, GET_MODE (operands[0]));
      break;
    }
    default:
      assemble_integer (operands[0], 8, 1);
      break;
    }
  return "";
}
}

static const char *
output_388 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  making_const_table = FALSE;
  return "";

}

static const char *
output_389 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  assemble_align (32);
  return "";

}

const char * const insn_template[] =
  {
    "adds\t%Q0, %Q1, %Q2\n\tadc\t%R0, %R1, %R2",
    "adds\t%Q0, %Q1, %2\n\tadc\t%R0, %R1, %2, asr #31",
    "adds\t%Q0, %Q1, %2\n\tadc\t%R0, %R1, #0",
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    "adc%?\t%0, %1, %2",
    "adc%?\t%0, %1, %2",
    "adc%?\t%0, %1, %2",
    "adc%?\t%0, %1, %2",
    0,
    0,
    0,
    0,
    0,
    "adf%?d\t%0, %1, %2",
    "adf%?d\t%0, %1, %2",
    0,
    "subs\t%Q0, %Q1, %Q2\n\tsbc\t%R0, %R1, %R2",
    "subs\t%Q0, %Q1, %2\n\tsbc\t%R0, %R1, #0",
    "subs\t%Q0, %Q1, %2\n\tsbc\t%R0, %R1, %2, asr #31",
    "rsbs\t%Q0, %Q1, %2\n\trsc\t%R0, %R1, #0",
    "rsbs\t%Q0, %Q1, %2\n\trsc\t%R0, %R1, %2, asr #31",
    "subs\t%Q0, %1, %2\n\trsc\t%R0, %1, %1",
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    "suf%?d\t%0, %1, %2",
    0,
    "suf%?d\t%0, %1, %2",
    0,
    "mul%?\t%0, %2, %1",
    "mul%?s\t%0, %2, %1",
    "mul%?s\t%0, %2, %1",
    "mla%?\t%0, %2, %1, %3",
    "mla%?s\t%0, %2, %1, %3",
    "mla%?s\t%0, %2, %1, %3",
    "smull%?\t%Q0, %R0, %1, %2",
    "umull%?\t%Q0, %R0, %1, %2",
    "smull%?\t%3, %0, %2, %1",
    "umull%?\t%3, %0, %2, %1",
    "fml%?s\t%0, %1, %2",
    "muf%?d\t%0, %1, %2",
    "muf%?d\t%0, %1, %2",
    "muf%?d\t%0, %1, %2",
    "muf%?d\t%0, %1, %2",
    "muf%?e\t%0, %1, %2",
    0,
    0,
    "dvf%?d\t%0, %1, %2",
    "rdf%?d\t%0, %2, %1",
    "dvf%?d\t%0, %1, %2",
    0,
    "rmf%?s\t%0, %1, %2",
    "rmf%?d\t%0, %1, %2",
    "rmf%?d\t%0, %1, %2",
    "rmf%?d\t%0, %1, %2",
    "rmf%?d\t%0, %1, %2",
    "rmf%?e\t%0, %1, %2",
    "and%?\t%Q0, %Q1, %Q2\n\tand%?\t%R0, %R1, %R2",
    "and%?\t%Q0, %Q1, %2\n\tmov%?\t%R0, #0",
    "and%?\t%Q0, %Q1, %2\n\tand%?\t%R0, %R1, %2, asr #31",
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    "bic%?\t%Q0, %Q1, %Q2\n\tbic%?\t%R0, %R1, %R2",
    0,
    "bic%?\t%Q0, %Q1, %2\n\tbic%?\t%R0, %R1, %2, asr #31",
    "bic%?\t%0, %1, %2",
    "bic%?\t%0, %1, %2%S4",
    "bic%?s\t%0, %1, %2",
    "bic%?s\t%0, %1, %2",
    "orr%?\t%Q0, %Q1, %Q2\n\torr%?\t%R0, %R1, %R2",
    0,
    "orr%?\t%Q0, %Q1, %2\n\torr%?\t%R0, %R1, %2, asr #31",
    0,
    0,
    0,
    "orr%?s\t%0, %1, %2",
    "orr%?s\t%0, %1, %2",
    "eor%?\t%Q0, %Q1, %Q2\n\teor%?\t%R0, %R1, %R2",
    0,
    "eor%?\t%Q0, %Q1, %2\n\teor%?\t%R0, %R1, %2, asr #31",
    "eor%?\t%0, %1, %2",
    "eor%?s\t%0, %1, %2",
    "teq%?\t%0, %1",
    0,
    "orr%?\t%0, %1, %2\n\tbic%?\t%0, %0, %3",
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    "mov%?\t%0, %1%S3",
    "mov%?s\t%0, %1%S3",
    "mov%?s\t%0, %1%S3",
    "mvn%?\t%0, %1%S3",
    "mvn%?s\t%0, %1%S3",
    "mvn%?s\t%0, %1%S3",
    "rsbs\t%Q0, %Q1, #0\n\trsc\t%R0, %R1, #0",
    "rsb%?\t%0, %1, #0",
    "mnf%?s\t%0, %1",
    "mnf%?d\t%0, %1",
    "mnf%?d\t%0, %1",
    "mnf%?e\t%0, %1",
    0,
    0,
    "abs%?s\t%0, %1",
    "abs%?d\t%0, %1",
    "abs%?d\t%0, %1",
    "abs%?e\t%0, %1",
    "sqt%?s\t%0, %1",
    "sqt%?d\t%0, %1",
    "sqt%?d\t%0, %1",
    "sqt%?e\t%0, %1",
    "mvn%?\t%Q0, %Q1\n\tmvn%?\t%R0, %R1",
    "mvn%?\t%0, %1",
    "mvn%?s\t%0, %1",
    "mvn%?s\t%0, %1",
    "flt%?s\t%0, %1",
    "flt%?d\t%0, %1",
    "flt%?e\t%0, %1",
    "fix%?z\t%0, %1",
    "fix%?z\t%0, %1",
    "fix%?z\t%0, %1",
    "mvf%?s\t%0, %1",
    "mvf%?s\t%0, %1",
    "mvf%?d\t%0, %1",
    0,
    0,
    0,
    0,
    "ldr%?h\t%0, %1",
    0,
    0,
    0,
    "ldr%?b\t%0, %1\t%@ zero_extendqisi2",
    0,
    "tst\t%0, #255",
    0,
    0,
    "ldr%?sh\t%0, %1",
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    "mvf%?d\t%0, %1",
    "mvf%?e\t%0, %1",
    "mvf%?e\t%0, %1",
    0,
    0,
    0,
    0,
    0,
    "adr%?\t%0, %a1",
    "ldr%?\t%0, %a1",
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    "ldr%?\t%0, %1\t%@ movhi_bigend",
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    "cmp%?\t%0, %1%S3",
    "cmp%?\t%0, %1%S3",
    "cmn%?\t%0, %1%S3",
    0,
    0,
    0,
    "cmf%?\t%0, %1",
    0,
    0,
    0,
    0,
    "cmf%?e\t%0, %1",
    0,
    "\t%@ deleted compare",
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    "mov%D1\t%0, #0\n\tmov%d1\t%0, #1",
    "mov%D1\t%0, #0\n\tmvn%d1\t%0, #0",
    "mov%D1\t%0, #0\n\tmvn%d1\t%0, #1",
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    "",
    0,
    0,
    "mov%?\t%|pc, %0\t%@ indirect jump",
    "ldr%?\t%|pc, %0\t%@ indirect jump",
    "mov%?\t%|r0, %|r0\t%@ nop",
    "%i1%?\t%0, %2, %4%S3",
    "%i1%?s\t%0, %2, %4%S3",
    "%i1%?s\t%0, %2, %4%S3",
    "sub%?\t%0, %1, %3%S2",
    "sub%?s\t%0, %1, %3%S2",
    "sub%?s\t%0, %1, %3%S2",
    0,
    0,
    0,
    0,
    0,
    0,
    "mov%D1\t%0, #0\n\tand%d1\t%0, %2, #1",
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    "#",
    0,
    "#",
    0,
    "#",
    "%I6%d5\t%0, %1, %2\n\t%I7%D5\t%0, %3, %4",
    0,
    0,
    0,
    0,
    "#",
    0,
    "#",
    0,
    "#",
    0,
    "#",
    0,
    "#",
    "mov%d5\t%0, %1%S6\n\tmov%D5\t%0, %3%S7",
    "#",
    "mvn%d5\t%0, %1\n\t%I6%D5\t%0, %2, %3",
    "#",
    "mvn%D5\t%0, %1\n\t%I6%d5\t%0, %2, %3",
    "#",
    0,
    "#",
    0,
    0,
    "str%?b\t%3, [%0, %2]!",
    "str%?b\t%3, [%0, -%2]!",
    "ldr%?b\t%3, [%0, %2]!",
    "ldr%?b\t%3, [%0, -%2]!",
    "ldr%?b\t%3, [%0, %2]!\t%@ z_extendqisi",
    "ldr%?b\t%3, [%0, -%2]!\t%@ z_extendqisi",
    "str%?\t%3, [%0, %2]!",
    "str%?\t%3, [%0, -%2]!",
    "ldr%?\t%3, [%0, %2]!",
    "ldr%?\t%3, [%0, -%2]!",
    "ldr%?\t%3, [%0, %2]!\t%@ loadhi",
    "ldr%?\t%3, [%0, -%2]!\t%@ loadhi",
    "str%?b\t%5, [%0, %3%S2]!",
    "str%?b\t%5, [%0, -%3%S2]!",
    "ldr%?b\t%5, [%0, %3%S2]!",
    "ldr%?b\t%5, [%0, -%3%S2]!",
    "str%?\t%5, [%0, %3%S2]!",
    "str%?\t%5, [%0, -%3%S2]!",
    "ldr%?\t%5, [%0, %3%S2]!",
    "ldr%?\t%5, [%0, -%3%S2]!",
    "ldr%?\t%5, [%0, %3%S2]!\t%@ loadhi",
    "ldr%?\t%5, [%0, -%3%S2]!\t%@ loadhi",
    "str%?b\t%2, [%0], %1",
    "ldr%?b\t%0, [%1], %2",
    "str%?\t%2, [%0], %1",
    "ldr%?\t%0, [%1], %2\t%@ loadhi",
    "ldr%?\t%0, [%1], %2",
    "str%?b\t%2, [%0, %1]!",
    "str%?b\t%3, [%2, %0%S4]!",
    "sub%?s\t%0, %1, #0",
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
  };

const char *(*const insn_outfun[])() =
  {
    0,
    0,
    0,
    0,
    0,
    output_5,
    output_6,
    output_7,
    output_8,
    output_9,
    output_10,
    output_11,
    0,
    0,
    0,
    0,
    output_16,
    0,
    output_18,
    output_19,
    output_20,
    0,
    0,
    output_23,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    output_31,
    0,
    output_33,
    output_34,
    output_35,
    output_36,
    0,
    output_38,
    0,
    output_40,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    output_57,
    output_58,
    0,
    0,
    0,
    output_62,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    output_73,
    0,
    output_75,
    output_76,
    output_77,
    output_78,
    0,
    0,
    output_81,
    0,
    0,
    0,
    0,
    0,
    0,
    output_88,
    0,
    0,
    output_91,
    0,
    0,
    0,
    0,
    output_96,
    0,
    0,
    0,
    0,
    0,
    0,
    output_103,
    output_104,
    output_105,
    output_106,
    output_107,
    output_108,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    output_126,
    output_127,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    output_149,
    output_150,
    output_151,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    output_166,
    0,
    0,
    output_169,
    0,
    0,
    0,
    0,
    output_174,
    0,
    output_176,
    0,
    0,
    0,
    0,
    0,
    output_182,
    output_183,
    output_184,
    0,
    0,
    0,
    0,
    0,
    output_190,
    0,
    0,
    output_193,
    output_194,
    output_195,
    0,
    output_197,
    0,
    0,
    0,
    output_201,
    0,
    output_203,
    output_204,
    0,
    0,
    output_207,
    output_208,
    0,
    output_210,
    0,
    output_212,
    output_213,
    0,
    output_215,
    output_216,
    0,
    0,
    0,
    0,
    0,
    output_222,
    0,
    0,
    0,
    output_226,
    output_227,
    output_228,
    0,
    output_230,
    output_231,
    output_232,
    output_233,
    0,
    output_235,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    output_247,
    output_248,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    output_265,
    output_266,
    output_267,
    output_268,
    output_269,
    0,
    output_271,
    output_272,
    0,
    output_274,
    output_275,
    output_276,
    output_277,
    output_278,
    output_279,
    output_280,
    0,
    0,
    0,
    output_284,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    output_294,
    output_295,
    output_296,
    output_297,
    output_298,
    output_299,
    0,
    output_301,
    output_302,
    output_303,
    output_304,
    output_305,
    output_306,
    output_307,
    output_308,
    output_309,
    0,
    output_311,
    0,
    output_313,
    0,
    0,
    output_316,
    output_317,
    output_318,
    output_319,
    0,
    output_321,
    0,
    output_323,
    0,
    output_325,
    0,
    output_327,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    output_335,
    0,
    output_337,
    output_338,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    output_369,
    output_370,
    output_371,
    output_372,
    output_373,
    output_374,
    output_375,
    output_376,
    output_377,
    0,
    0,
    0,
    0,
    output_382,
    output_383,
    output_384,
    output_385,
    output_386,
    output_387,
    output_388,
    output_389,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
  };

rtx (*const insn_gen_function[]) () =
  {
    gen_adddi3,
    0,
    0,
    gen_addsi3,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    gen_incscc,
    0,
    gen_addsf3,
    gen_adddf3,
    0,
    0,
    0,
    gen_addxf3,
    gen_subdi3,
    0,
    0,
    0,
    0,
    0,
    gen_subsi3,
    0,
    0,
    0,
    gen_decscc,
    gen_subsf3,
    gen_subdf3,
    0,
    0,
    0,
    gen_subxf3,
    gen_mulsi3,
    0,
    0,
    0,
    0,
    0,
    gen_mulsidi3,
    gen_umulsidi3,
    gen_smulsi3_highpart,
    gen_umulsi3_highpart,
    gen_mulsf3,
    gen_muldf3,
    0,
    0,
    0,
    gen_mulxf3,
    gen_divsf3,
    gen_divdf3,
    0,
    0,
    0,
    gen_divxf3,
    gen_modsf3,
    gen_moddf3,
    0,
    0,
    0,
    gen_modxf3,
    gen_anddi3,
    0,
    0,
    gen_andsi3,
    0,
    0,
    0,
    0,
    0,
    0,
    gen_insv,
    0,
    0,
    0,
    gen_andsi_notsi_si,
    gen_andsi_not_shiftsi_si,
    0,
    0,
    gen_iordi3,
    0,
    0,
    gen_iorsi3,
    0,
    0,
    0,
    0,
    gen_xordi3,
    0,
    0,
    gen_xorsi3,
    0,
    0,
    0,
    0,
    gen_smaxsi3,
    gen_sminsi3,
    gen_umaxsi3,
    gen_uminsi3,
    0,
    0,
    gen_ashlsi3,
    gen_ashrsi3,
    gen_lshrsi3,
    gen_rotlsi3,
    gen_rotrsi3,
    0,
    0,
    0,
    0,
    0,
    0,
    gen_negdi2,
    gen_negsi2,
    gen_negsf2,
    gen_negdf2,
    0,
    gen_negxf2,
    gen_abssi2,
    0,
    gen_abssf2,
    gen_absdf2,
    0,
    gen_absxf2,
    gen_sqrtsf2,
    gen_sqrtdf2,
    0,
    gen_sqrtxf2,
    gen_one_cmpldi2,
    gen_one_cmplsi2,
    0,
    0,
    gen_floatsisf2,
    gen_floatsidf2,
    gen_floatsixf2,
    gen_fix_truncsfsi2,
    gen_fix_truncdfsi2,
    gen_fix_truncxfsi2,
    gen_truncdfsf2,
    gen_truncxfsf2,
    gen_truncxfdf2,
    gen_zero_extendsidi2,
    gen_zero_extendqidi2,
    gen_extendsidi2,
    gen_zero_extendhisi2,
    0,
    0,
    0,
    gen_zero_extendqisi2,
    0,
    0,
    0,
    gen_extendhisi2,
    gen_extendhisi2_mem,
    0,
    0,
    0,
    gen_extendqihi2,
    0,
    0,
    gen_extendqisi2,
    0,
    0,
    gen_extendsfdf2,
    gen_extendsfxf2,
    gen_extenddfxf2,
    gen_movdi,
    gen_movsi,
    0,
    0,
    gen_movaddr,
    0,
    gen_pic_load_addr,
    gen_pic_load_addr_based,
    0,
    gen_pic_add_dot_plus_eight,
    0,
    gen_storehi,
    gen_storehi_bigend,
    gen_storeinthi,
    gen_storehi_single_op,
    gen_movhi,
    gen_rotated_loadsi,
    gen_movhi_bytes,
    gen_movhi_bigend,
    0,
    0,
    0,
    0,
    0,
    gen_reload_outhi,
    gen_reload_inhi,
    gen_movqi,
    0,
    gen_movsf,
    0,
    0,
    gen_movdf,
    gen_reload_outdf,
    0,
    0,
    gen_movxf,
    0,
    gen_load_multiple,
    0,
    0,
    gen_store_multiple,
    0,
    0,
    gen_movstrqi,
    gen_cmpsi,
    gen_cmpsf,
    gen_cmpdf,
    gen_cmpxf,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    gen_beq,
    gen_bne,
    gen_bgt,
    gen_ble,
    gen_bge,
    gen_blt,
    gen_bgtu,
    gen_bleu,
    gen_bgeu,
    gen_bltu,
    0,
    0,
    gen_seq,
    gen_sne,
    gen_sgt,
    gen_sle,
    gen_sge,
    gen_slt,
    gen_sgtu,
    gen_sleu,
    gen_sgeu,
    gen_sltu,
    0,
    0,
    0,
    gen_movsicc,
    gen_movsfcc,
    gen_movdfcc,
    0,
    0,
    0,
    0,
    gen_jump,
    gen_call,
    0,
    0,
    gen_call_value,
    0,
    0,
    0,
    0,
    gen_return,
    0,
    0,
    gen_untyped_call,
    gen_blockage,
    gen_casesi,
    gen_casesi_internal,
    gen_indirect_jump,
    0,
    gen_nop,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    gen_movcond,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    gen_prologue,
    0,
    0,
    0,
    0,
    0,
    gen_consttable_4,
    gen_consttable_8,
    gen_consttable_end,
    gen_align_4,
    gen_save_stack_function,
    gen_restore_stack_function,
    gen_save_stack_block,
    gen_save_stack_nonlocal,
    gen_restore_stack_block,
    gen_restore_stack_nonlocal,
    gen_allocate_stack,
  };

const char *insn_name[] =
  {
    "adddi3",
    "*adddi_sesidi_di",
    "*adddi_zesidi_di",
    "addsi3",
    "addsi3+1",
    "*addsi3_insn",
    "*addsi3_compare0",
    "*addsi3_compare0_scratch",
    "*addsi3_compare_op1",
    "*addsi3_compare_op2",
    "*compare_addsi2_op0",
    "*compare_addsi2_op1",
    "*addsi3_carryin",
    "*addsi3_carryin_alt1",
    "*addsi3_carryin_alt2",
    "*addsi3_carryin_alt3",
    "incscc",
    "incscc+1",
    "addsf3",
    "adddf3",
    "*adddf_esfdf_df",
    "*adddf_df_esfdf",
    "*adddf_esfdf_esfdf",
    "addxf3",
    "subdi3",
    "*subdi_di_zesidi",
    "*subdi_di_sesidi",
    "*subdi_zesidi_di",
    "*subdi_sesidi_di",
    "*subdi_zesidi_zesidi",
    "subsi3",
    "*subsi3_insn",
    "*subsi3_insn+1",
    "*subsi3_compare0",
    "decscc",
    "subsf3",
    "subdf3",
    "*subdf_esfdf_df",
    "*subdf_df_esfdf",
    "*subdf_esfdf_esfdf",
    "subxf3",
    "mulsi3",
    "*mulsi3_compare0",
    "*mulsi_compare0_scratch",
    "*mulsi3addsi",
    "*mulsi3addsi_compare0",
    "*mulsi3addsi_compare0_scratch",
    "mulsidi3",
    "umulsidi3",
    "smulsi3_highpart",
    "umulsi3_highpart",
    "mulsf3",
    "muldf3",
    "*muldf_esfdf_df",
    "*muldf_df_esfdf",
    "*muldf_esfdf_esfdf",
    "mulxf3",
    "divsf3",
    "divdf3",
    "*divdf_esfdf_df",
    "*divdf_df_esfdf",
    "*divdf_esfdf_esfdf",
    "divxf3",
    "modsf3",
    "moddf3",
    "*moddf_esfdf_df",
    "*moddf_df_esfdf",
    "*moddf_esfdf_esfdf",
    "modxf3",
    "anddi3",
    "*anddi_zesidi_di",
    "*anddi_sesdi_di",
    "andsi3",
    "*andsi3_insn",
    "*andsi3_insn+1",
    "*andsi3_compare0",
    "*andsi3_compare0_scratch",
    "*zeroextractsi_compare0_scratch",
    "*ne_zeroextractsi",
    "insv",
    "*anddi_notdi_di",
    "*anddi_notzesidi_di",
    "*anddi_notsesidi_di",
    "andsi_notsi_si",
    "andsi_not_shiftsi_si",
    "*andsi_notsi_si_compare0",
    "*andsi_notsi_si_compare0_scratch",
    "iordi3",
    "*iordi_zesidi_di",
    "*iordi_sesidi_di",
    "iorsi3",
    "*iorsi3_insn",
    "*iorsi3_insn+1",
    "*iorsi3_compare0",
    "*iorsi3_compare0_scratch",
    "xordi3",
    "*xordi_zesidi_di",
    "*xordi_sesidi_di",
    "xorsi3",
    "*xorsi3_compare0",
    "*xorsi3_compare0_scratch",
    "*xorsi3_compare0_scratch+1",
    "*andsi_iorsi3_notsi",
    "smaxsi3",
    "sminsi3",
    "umaxsi3",
    "uminsi3",
    "*store_minmaxsi",
    "*minmax_arithsi",
    "ashlsi3",
    "ashrsi3",
    "lshrsi3",
    "rotlsi3",
    "rotrsi3",
    "*shiftsi3",
    "*shiftsi3_compare0",
    "*shiftsi3_compare0_scratch",
    "*notsi_shiftsi",
    "*notsi_shiftsi_compare0",
    "*not_shiftsi_compare0_scratch",
    "negdi2",
    "negsi2",
    "negsf2",
    "negdf2",
    "*negdf_esfdf",
    "negxf2",
    "abssi2",
    "*neg_abssi2",
    "abssf2",
    "absdf2",
    "*absdf_esfdf",
    "absxf2",
    "sqrtsf2",
    "sqrtdf2",
    "*sqrtdf_esfdf",
    "sqrtxf2",
    "one_cmpldi2",
    "one_cmplsi2",
    "*notsi_compare0",
    "*notsi_compare0_scratch",
    "floatsisf2",
    "floatsidf2",
    "floatsixf2",
    "fix_truncsfsi2",
    "fix_truncdfsi2",
    "fix_truncxfsi2",
    "truncdfsf2",
    "truncxfsf2",
    "truncxfdf2",
    "zero_extendsidi2",
    "zero_extendqidi2",
    "extendsidi2",
    "zero_extendhisi2",
    "*zero_extendhisi_insn",
    "*zero_extendhisi_insn+1",
    "zero_extendqisi2-1",
    "zero_extendqisi2",
    "*load_extendqisi",
    "*load_extendqisi+1",
    "*compareqi_eq0",
    "extendhisi2",
    "extendhisi2_mem",
    "*extendhisi_insn",
    "*extendhisi_insn+1",
    "extendqihi2-1",
    "extendqihi2",
    "*extendqihi_insn",
    "*extendqihi_insn+1",
    "extendqisi2",
    "*extendqisi_insn",
    "*extendqisi_insn+1",
    "extendsfdf2",
    "extendsfxf2",
    "extenddfxf2",
    "movdi",
    "movsi",
    "*movsi_insn",
    "*movsi_insn+1",
    "movaddr",
    "*movaddr_insn",
    "pic_load_addr",
    "pic_load_addr_based",
    "*pic_load_addr_based_insn",
    "pic_add_dot_plus_eight",
    "*movsi_compare0",
    "storehi",
    "storehi_bigend",
    "storeinthi",
    "storehi_single_op",
    "movhi",
    "rotated_loadsi",
    "movhi_bytes",
    "movhi_bigend",
    "*movhi_insn_arch4",
    "*movhi_insn_littleend",
    "*movhi_insn_bigend",
    "*loadhi_si_bigend",
    "*movhi_bytes",
    "reload_outhi",
    "reload_inhi",
    "movqi",
    "*movqi_insn",
    "movsf",
    "*movsf_hard_insn",
    "*movsf_soft_insn",
    "movdf",
    "reload_outdf",
    "*movdf_hard_insn",
    "*movdf_soft_insn",
    "movxf",
    "*movxf_hard_insn",
    "load_multiple",
    "*ldmsi_postinc",
    "*ldmsi",
    "store_multiple",
    "*stmsi_postinc",
    "*stmsi",
    "movstrqi",
    "cmpsi",
    "cmpsf",
    "cmpdf",
    "cmpxf",
    "*cmpsi_insn",
    "*cmpsi_shiftsi",
    "*cmpsi_shiftsi_swp",
    "*cmpsi_neg_shiftsi",
    "*cmpsf_insn",
    "*cmpdf_insn",
    "*cmpesfdf_df",
    "*cmpdf_esfdf",
    "*cmpxf_insn",
    "*cmpsf_trap",
    "*cmpdf_trap",
    "*cmp_esfdf_df_trap",
    "*cmp_df_esfdf_trap",
    "*cmpxf_trap",
    "*deleted_compare",
    "beq",
    "bne",
    "bgt",
    "ble",
    "bge",
    "blt",
    "bgtu",
    "bleu",
    "bgeu",
    "bltu",
    "*condbranch",
    "*condbranch_reversed",
    "seq",
    "sne",
    "sgt",
    "sle",
    "sge",
    "slt",
    "sgtu",
    "sleu",
    "sgeu",
    "sltu",
    "*mov_scc",
    "*mov_negscc",
    "*mov_notscc",
    "movsicc",
    "movsfcc",
    "movdfcc",
    "*movsicc_insn",
    "*movsfcc_hard_insn",
    "*movsfcc_soft_insn",
    "*movdfcc_insn",
    "jump",
    "call",
    "*call_reg",
    "*call_mem",
    "call_value",
    "*call_value_reg",
    "*call_value_mem",
    "*call_symbol",
    "*call_value_symbol",
    "return",
    "*cond_return",
    "*cond_return_inverted",
    "untyped_call",
    "blockage",
    "casesi",
    "casesi_internal",
    "indirect_jump",
    "*load_indirect_jump",
    "nop",
    "*arith_shiftsi",
    "*arith_shiftsi_compare0",
    "*arith_shiftsi_compare0_scratch",
    "*sub_shiftsi",
    "*sub_shiftsi_compare0",
    "*sub_shiftsi_compare0_scratch",
    "*reload_mulsi3",
    "*reload_mulsi_compare0",
    "*reload_mulsi_compare0_scratch",
    "*reload_muladdsi",
    "*reload_muladdsi_compare0",
    "*reload_muladdsi_compare0_scratch",
    "*and_scc",
    "*ior_scc",
    "*compare_scc",
    "*cond_move",
    "*cond_arith",
    "*cond_sub",
    "*cmp_ite0",
    "*cmp_ite1",
    "*negscc",
    "movcond",
    "*ifcompare_plus_move",
    "*if_plus_move",
    "*ifcompare_move_plus",
    "*if_move_plus",
    "*ifcompare_arith_arith",
    "*if_arith_arith",
    "*ifcompare_arith_move",
    "*if_arith_move",
    "*ifcompare_move_arith",
    "*if_move_arith",
    "*ifcompare_move_not",
    "*if_move_not",
    "*ifcompare_not_move",
    "*if_not_move",
    "*ifcompare_shift_move",
    "*if_shift_move",
    "*ifcompare_move_shift",
    "*if_move_shift",
    "*ifcompare_shift_shift",
    "*if_shift_shift",
    "*ifcompare_not_arith",
    "*if_not_arith",
    "*ifcompare_arith_not",
    "*if_arith_not",
    "*ifcompare_neg_move",
    "*if_neg_move",
    "*ifcompare_move_neg",
    "*if_move_neg",
    "*arith_adjacentmem",
    "*strqi_preinc",
    "*strqi_predec",
    "*loadqi_preinc",
    "*loadqi_predec",
    "*loadqisi_preinc",
    "*loadqisi_predec",
    "*strsi_preinc",
    "*strsi_predec",
    "*loadsi_preinc",
    "*loadsi_predec",
    "*loadhi_preinc",
    "*loadhi_predec",
    "*strqi_shiftpreinc",
    "*strqi_shiftpredec",
    "*loadqi_shiftpreinc",
    "*loadqi_shiftpredec",
    "*strsi_shiftpreinc",
    "*strsi_shiftpredec",
    "*loadsi_shiftpreinc",
    "*loadsi_shiftpredec",
    "*loadhi_shiftpreinc",
    "*loadhi_shiftpredec",
    "*loadhi_shiftpredec+1",
    "*loadhi_shiftpredec+2",
    "*loadhi_shiftpredec+3",
    "*loadhi_shiftpredec+4",
    "*loadhi_shiftpredec+5",
    "*loadhi_shiftpredec+6",
    "*loadhi_shiftpredec+7",
    "*loadhi_shiftpredec+8",
    "*loadhi_shiftpredec+9",
    "*loadhi_shiftpredec+10",
    "prologue-9",
    "prologue-8",
    "prologue-7",
    "prologue-6",
    "prologue-5",
    "prologue-4",
    "prologue-3",
    "prologue-2",
    "prologue-1",
    "prologue",
    "prologue+1",
    "*sign_extract_onebit",
    "*not_signextract_onebit",
    "*push_multi",
    "*push_fp_multi",
    "consttable_4",
    "consttable_8",
    "consttable_end",
    "align_4",
    "save_stack_function",
    "restore_stack_function",
    "save_stack_block",
    "save_stack_nonlocal",
    "restore_stack_block",
    "restore_stack_nonlocal",
    "allocate_stack",
  };
const char **insn_name_ptr = insn_name;

const int insn_n_operands[] =
  {
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    2,
    3,
    3,
    2,
    2,
    3,
    3,
    3,
    3,
    4,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    4,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    4,
    4,
    4,
    3,
    3,
    4,
    4,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    4,
    3,
    4,
    4,
    3,
    3,
    3,
    3,
    5,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    3,
    2,
    5,
    4,
    3,
    3,
    3,
    3,
    4,
    6,
    3,
    3,
    3,
    3,
    3,
    4,
    4,
    4,
    4,
    4,
    4,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    3,
    5,
    2,
    2,
    3,
    1,
    2,
    2,
    2,
    3,
    5,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    3,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    3,
    3,
    2,
    2,
    2,
    2,
    2,
    2,
    3,
    2,
    2,
    2,
    2,
    0,
    4,
    3,
    0,
    4,
    3,
    4,
    2,
    2,
    2,
    2,
    2,
    4,
    4,
    4,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    3,
    3,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    3,
    3,
    3,
    4,
    4,
    4,
    5,
    5,
    5,
    5,
    1,
    2,
    2,
    2,
    3,
    3,
    3,
    2,
    3,
    0,
    2,
    2,
    3,
    0,
    5,
    4,
    1,
    1,
    0,
    6,
    6,
    6,
    5,
    5,
    5,
    6,
    6,
    6,
    5,
    5,
    5,
    4,
    4,
    4,
    6,
    6,
    5,
    7,
    7,
    4,
    6,
    7,
    6,
    7,
    6,
    10,
    9,
    8,
    7,
    8,
    7,
    6,
    5,
    6,
    5,
    8,
    7,
    8,
    7,
    10,
    9,
    8,
    7,
    8,
    7,
    6,
    5,
    6,
    5,
    5,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    6,
    6,
    6,
    6,
    6,
    6,
    6,
    6,
    6,
    6,
    3,
    3,
    3,
    3,
    3,
    3,
    5,
    2,
    8,
    6,
    4,
    8,
    6,
    4,
    2,
    3,
    3,
    6,
    3,
    0,
    6,
    3,
    3,
    3,
    3,
    1,
    1,
    0,
    0,
    0,
    0,
    2,
    2,
    2,
    2,
    2,
  };

const int insn_n_dups[] =
  {
    0,
    0,
    0,
    0,
    0,
    0,
    2,
    0,
    3,
    3,
    1,
    1,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    2,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    2,
    0,
    0,
    3,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    2,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    2,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    2,
    0,
    0,
    0,
    0,
    0,
    2,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    0,
    0,
    1,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    2,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    2,
    7,
    0,
    0,
    0,
    2,
    0,
    0,
    2,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    0,
    1,
    1,
    4,
    4,
    2,
    0,
    0,
    0,
    5,
    4,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    2,
    0,
    1,
    2,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    0,
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    2,
    0,
    0,
    0,
    0,
    1,
    0,
    0,
    2,
    0,
    0,
    3,
    0,
    0,
    4,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    4,
    0,
    1,
  };

const char *const insn_operand_constraint[][MAX_RECOG_OPERANDS] =
  {
    { "=&r,&r", "%0,0", "r,0", },
    { "=&r,&r", "r,0", "r,r", },
    { "=&r,&r", "r,0", "r,r", },
    { "", "", "", },
    { "", "", "", },
    { "=r,r,r", "r,r,r", "rI,L,?n", },
    { "=r,r", "r,r", "rI,L", },
    { "r,r", "rI,L", },
    { "=r,r", "r,r", "rI,L", },
    { "=r,r", "r,r", "rI,L", },
    { "r,r", "rI,L", },
    { "r,r", "rI,L", },
    { "=r", "r", "rI", },
    { "=r", "r", "rI", },
    { "=r", "r", "rI", },
    { "=r", "r", "rI", },
    { "=r,r", "0,?r", "", "", },
    { "", "", "", },
    { "=f,f", "f,f", "fG,H", },
    { "=f,f", "f,f", "fG,H", },
    { "=f,f", "f,f", "fG,H", },
    { "=f", "f", "f", },
    { "=f", "f", "f", },
    { "=f,f", "f,f", "fG,H", },
    { "=&r,&r,&r", "0,r,0", "r,0,0", },
    { "=&r,&r", "?r,0", "r,r", },
    { "=&r,&r", "r,0", "r,r", },
    { "=&r,&r", "?r,0", "r,r", },
    { "=&r,&r", "?r,0", "r,r", },
    { "=r", "r", "r", },
    { "", "", "", },
    { "=r,r", "rI,?n", "r,r", },
    { "", "", "", },
    { "=r,r", "r,I", "rI,r", },
    { "=r,r", "0,?r", "", "", },
    { "=f,f", "f,G", "fG,f", },
    { "=f,f", "f,G", "fG,f", },
    { "=f", "f", "fG", },
    { "=f,f", "f,G", "f,f", },
    { "=f", "f", "f", },
    { "=f,f", "f,G", "fG,f", },
    { "=&r,&r", "%?r,0", "r,r", },
    { "=&r,&r", "%?r,0", "r,r", },
    { "=&r,&r", "%?r,0", "r,r", },
    { "=&r,&r,&r,&r", "%r,0,r,0", "r,r,r,r", "?r,r,0,0", },
    { "=&r,&r,&r,&r", "%r,0,r,0", "r,r,r,r", "?r,r,0,0", },
    { "=&r,&r,&r,&r", "%r,0,r,0", "r,r,r,r", "?r,r,0,0", },
    { "=&r", "%r", "r", },
    { "=&r", "%r", "r", },
    { "=&r,&r", "%r,0", "r,r", "=&r,&r", },
    { "=&r,&r", "%r,0", "r,r", "=&r,&r", },
    { "=f", "f", "fG", },
    { "=f", "f", "fG", },
    { "=f", "f", "fG", },
    { "=f", "f", "f", },
    { "=f", "f", "f", },
    { "=f", "f", "fG", },
    { "=f,f", "f,G", "fG,f", },
    { "=f,f", "f,G", "fG,f", },
    { "=f", "f", "fG", },
    { "=f", "fG", "f", },
    { "=f", "f", "f", },
    { "=f,f", "f,G", "fG,f", },
    { "=f", "f", "fG", },
    { "=f", "f", "fG", },
    { "=f", "f", "fG", },
    { "=f", "f", "f", },
    { "=f", "f", "f", },
    { "=f", "f", "fG", },
    { "=&r,&r", "%0,0", "r,0", },
    { "=&r,&r", "?r,0", "r,r", },
    { "=&r,&r", "?r,0", "r,r", },
    { "", "", "", },
    { "=r,r,r", "r,r,r", "rI,K,?n", },
    { "", "", "", },
    { "=r,r", "r,r", "rI,K", },
    { "r,r", "rI,K", "", "=X,r", },
    { "r", "n", "n", },
    { "=r", "r", "n", "n", },
    { "", "", "", "", },
    { "=&r,&r", "0,r", "r,0", },
    { "=&r,&r", "0,?r", "r,r", },
    { "=&r,&r", "?r,0", "r,r", },
    { "=r", "r", "r", },
    { "=r", "r", "r", "rM", "", },
    { "=r", "r", "r", },
    { "=r", "r", "r", },
    { "=&r", "%0", "r", },
    { "=&r,&r", "0,?r", "r,r", },
    { "=&r,&r", "?r,0", "r,r", },
    { "", "", "", },
    { "=r,r", "r,r", "rI,?n", },
    { "", "", "", },
    { "=r", "%r", "rI", },
    { "=r", "%r", "rI", },
    { "=&r,&r", "%0,0", "r,0", },
    { "=&r,&r", "0,?r", "r,r", },
    { "=&r,&r", "?r,0", "r,r", },
    { "=r", "r", "rI", },
    { "=r", "r", "rI", },
    { "r", "rI", },
    { "", "", "", "", "", },
    { "=&r,&r,&r", "r,r,0", "rI,0,rI", "rI,rI,rI", },
    { "=r,r,r", "0,r,?r", "rI,0,rI", },
    { "=r,r,r", "0,r,?r", "rI,0,rI", },
    { "=r,r,r", "0,r,?r", "rI,0,rI", },
    { "=r,r,r", "0,r,?r", "rI,0,rI", },
    { "=m", "r", "r", "", },
    { "=r,r", "0,?r", "r,r", "rI,rI", "", "", },
    { "", "", "", },
    { "", "", "", },
    { "", "", "", },
    { "", "", "", },
    { "", "", "", },
    { "=r", "r", "rM", "", },
    { "=r", "r", "rM", "", },
    { "=r", "r", "rM", "", },
    { "=r", "r", "rM", "", },
    { "=r", "r", "rM", "", },
    { "=r", "r", "rM", "", },
    { "=&r,&r", "?r,0", },
    { "=r", "r", },
    { "=f", "f", },
    { "=f", "f", },
    { "=f", "f", },
    { "=f", "f", },
    { "=r,&r", "0,r", },
    { "=r,&r", "0,r", },
    { "=f", "f", },
    { "=f", "f", },
    { "=f", "f", },
    { "=f", "f", },
    { "=f", "f", },
    { "=f", "f", },
    { "=f", "f", },
    { "=f", "f", },
    { "=&r,&r", "?r,0", },
    { "=r", "r", },
    { "=r", "r", },
    { "=r", "r", },
    { "=f", "r", },
    { "=f", "r", },
    { "=f", "r", },
    { "=r", "f", },
    { "=r", "f", },
    { "=r", "f", },
    { "=f", "f", },
    { "=f", "f", },
    { "=f", "f", },
    { "=r", "r", },
    { "=r,r", "r,m", },
    { "=r", "r", },
    { "", "", },
    { "=r", "m", },
    { "", "", "", },
    { "", "", "", "", "", },
    { "=r,r", "r,m", },
    { "=r", "m", },
    { "", "", "", },
    { "r", },
    { "", "", },
    { "", "", },
    { "=r", "m", },
    { "", "", "", },
    { "", "", "", "", "", },
    { "", "", },
    { "=r", "m", },
    { "", "", },
    { "", "", },
    { "=r", "m", },
    { "", "", },
    { "=f", "f", },
    { "=f", "f", },
    { "=f", "f", },
    { "=r,r,o<>", "rIK,mi,r", },
    { "", "", },
    { "=r,r,r,m", "rI,K,mi,r", },
    { "", "", },
    { "", "", },
    { "=r", "p", },
    { "=r", "", },
    { "=r", "", },
    { "=r", "", "r", },
    { "+r", "", },
    { "=r,r", "0,r", },
    { "", "", },
    { "", "", },
    { "", "", },
    { "", "", },
    { "", "", },
    { "=r", "o", },
    { "", "", },
    { "", "", },
    { "=r,r,r,m", "rI,K,m,r", },
    { "=r,r,r", "rI,K,m", },
    { "=r,r,r", "rI,K,m", },
    { "=r", "m", },
    { "=r,r", "rI,K", },
    { "=o", "r", "=&r", },
    { "=r", "o", "=&r", },
    { "", "", },
    { "=r,r,r,m", "rI,K,m,r", },
    { "", "", },
    { "=f,f,f,m,f,r,r,r,m", "fG,H,mE,f,r,f,r,mE,r", },
    { "=r,r,m", "r,mE,r", },
    { "", "", },
    { "=o", "r", "=&r", },
    { "=r,Q,r,m,r,f,f,f,m,!f,!r", "Q,r,r,r,mF,fG,H,mF,f,r,f", },
    { "=r,r,m", "r,mF,r", },
    { "", "", },
    { "=f,f,f,m,f,r,r", "fG,H,m,f,r,f,r", },
    { 0 },
    { "", "+r", "n", "=r", },
    { "", "=r", "r", },
    { 0 },
    { "", "+r", "n", "r", },
    { "", "r", "r", },
    { "", "", "", "", },
    { "", "", },
    { "", "", },
    { "", "", },
    { "", "", },
    { "r,r", "rI,L", },
    { "r", "r", "rM", "", },
    { "r", "r", "rM", "", },
    { "r", "r", "rM", "", },
    { "f,f", "fG,H", },
    { "f,f", "fG,H", },
    { "f,f", "fG,H", },
    { "f", "f", },
    { "f,f", "fG,H", },
    { "f,f", "fG,H", },
    { "f,f", "fG,H", },
    { "f,f", "fG,H", },
    { "f", "f", },
    { "f,f", "fG,H", },
    { "", },
    { "", },
    { "", },
    { "", },
    { "", },
    { "", },
    { "", },
    { "", },
    { "", },
    { "", },
    { "", },
    { "", "", "", },
    { "", "", "", },
    { "=r", },
    { "=r", },
    { "=r", },
    { "=r", },
    { "=r", },
    { "=r", },
    { "=r", },
    { "=r", },
    { "=r", },
    { "=r", },
    { "=r", "", "", },
    { "=r", "", "", },
    { "=r", "", "", },
    { "", "", "", "", },
    { "", "", "", "", },
    { "", "", "", "", },
    { "=r,r,r,r,r,r,r,r", "0,0,rI,K,rI,rI,K,K", "rI,K,0,0,rI,K,rI,K", "", "", },
    { "=f,f,f,f,f,f,f,f", "0,0,fG,H,fG,fG,H,H", "fG,H,0,0,fG,H,fG,H", "", "", },
    { "=r,r", "0,r", "r,0", "", "", },
    { "=f,f,f,f,f,f,f,f", "0,0,fG,H,fG,fG,H,H", "fG,H,0,0,fG,H,fG,H", "", "", },
    { "", },
    { "", "", },
    { "r", "g", },
    { "m", "g", },
    { "=rf", "m", "g", },
    { "=rf", "r", "g", },
    { "=rf", "m", "g", },
    { "X", "g", },
    { "=rf", "X", "g", },
    { 0 },
    { "", "", },
    { "", "", },
    { "", "", "", },
    { 0 },
    { "", "", "", "", "", },
    { "r", "rI", "", "", },
    { "r", },
    { "m", },
    { 0 },
    { "=r", "", "r", "", "r", "rI", },
    { "=r", "", "r", "", "r", "rI", },
    { "=r", "", "r", "", "r", "rI", },
    { "=r", "r", "", "r", "rM", },
    { "=r", "r", "", "r", "rM", },
    { "=r", "r", "", "r", "rM", },
    { "=&r", "n", "r", "r", "rM", "", },
    { "=&r", "r", "n", "r", "rM", "", },
    { "=&r", "r", "n", "r", "rM", "", },
    { "=&r,&r", "%0,r", "r,r", "r,r", "n,n", },
    { "=&r", "r", "n", "r", "r", },
    { "=&r", "r", "n", "r", "r", },
    { "=r", "", "r", "", },
    { "=r,r", "0,?r", "", "", },
    { "=r,r", "", "r,r", "rI,L", },
    { "=r,r,r", "0,rI,?rI", "rI,0,rI", "", "", "", },
    { "=r,r", "0,?r", "r,r", "rI,rI", "", "", },
    { "=r,r", "0,?r", "r,r", "rI,rI", "", },
    { "r,r,r,r", "rI,L,rI,L", "r,r,r,r", "rI,rI,L,L", "", "", "", },
    { "r,r,r,r", "rI,L,rI,L", "r,r,r,r", "rI,rI,L,L", "", "", "", },
    { "=r", "r", "rI", "", },
    { "=r,r,r", "0,rI,?rI", "rI,0,rI", "r,r,r", "rIL,rIL,rIL", "", },
    { "=r,r", "0,?rI", "r,r", "rIL,rIL", "r,r", "rIL,rIL", "", },
    { "=r,r,r,r", "0,0,?rI,?rI", "r,r,r,r", "rI,L,rI,L", "", "", },
    { "=r,r", "0,?rI", "r,r", "rIL,rIL", "r,r", "rIL,rIL", "", },
    { "=r,r,r,r", "0,0,?rI,?rI", "r,r,r,r", "rI,L,rI,L", "", "", },
    { "=r", "r", "rI", "r", "rI", "r", "rIL", "", "", "", },
    { "=r", "r", "rI", "r", "rI", "", "", "", "", },
    { "=r,r", "0,?rI", "r,r", "rIL,rIL", "r,r", "rI,rI", "", "", },
    { "=r,r", "0,?rI", "r,r", "rI,rI", "", "", "", },
    { "=r,r", "0,?rI", "r,r", "rI,rI", "r,r", "rIL,rIL", "", "", },
    { "=r,r", "0,?rI", "r,r", "rI,rI", "", "", "", },
    { "=r,r", "0,?rIK", "r,r", "r,r", "rIL,rIL", "", },
    { "=r,r,r", "0,?rI,K", "r,r,r", "", "", },
    { "=r,r", "0,?rIK", "r,r", "r,r", "rIL,rIL", "", },
    { "=r,r,r", "0,?rI,K", "r,r,r", "", "", },
    { "=r,r", "0,?rIK", "r,r", "rM,rM", "r,r", "rIL,rIL", "", "", },
    { "=r,r,r", "0,?rI,K", "r,r,r", "rM,rM,rM", "", "", "", },
    { "=r,r", "0,?rIK", "r,r", "rM,rM", "r,r", "rIL,rIL", "", "", },
    { "=r,r,r", "0,?rI,K", "r,r,r", "rM,rM,rM", "", "", "", },
    { "=r", "r", "rM", "r", "rM", "r", "rIL", "", "", "", },
    { "=r", "r", "rM", "r", "rM", "", "", "", "", },
    { "=r", "r", "r", "rI", "r", "rIL", "", "", },
    { "=r", "r", "r", "rI", "", "", "", },
    { "=r", "r", "r", "rI", "r", "rIL", "", "", },
    { "=r", "r", "r", "rI", "", "", "", },
    { "=r,r", "0,?rIK", "r,r", "r,r", "rIL,rIL", "", },
    { "=r,r,r", "0,?rI,K", "r,r,r", "", "", },
    { "=r,r", "0,?rIK", "r,r", "r,r", "rIL,rIL", "", },
    { "=r,r,r", "0,?rI,K", "r,r,r", "", "", },
    { "=r", "", "m", "m", "=r", },
    { "=r", "%0", "rJ", "r", },
    { "=r", "0", "r", "r", },
    { "=r", "%0", "rJ", "=r", },
    { "=r", "0", "r", "=r", },
    { "=r", "%0", "rJ", "=r", },
    { "=r", "0", "r", "=r", },
    { "=r", "%0", "rJ", "r", },
    { "=r", "0", "r", "r", },
    { "=r", "%0", "rJ", "=r", },
    { "=r", "0", "r", "=r", },
    { "=r", "%0", "rJ", "=r", },
    { "=r", "0", "r", "=r", },
    { "=r", "0", "", "r", "n", "r", },
    { "=r", "0", "", "r", "n", "r", },
    { "=r", "0", "", "r", "n", "=r", },
    { "=r", "0", "", "r", "n", "=r", },
    { "=r", "0", "", "r", "n", "r", },
    { "=r", "0", "", "r", "n", "r", },
    { "=r", "0", "", "r", "n", "=r", },
    { "=r", "0", "", "r", "n", "=r", },
    { "=r", "0", "", "r", "n", "=r", },
    { "=r", "0", "", "r", "n", "=r", },
    { "+r", "rJ", "r", },
    { "=r", "+r", "rJ", },
    { "+r", "rJ", "r", },
    { "=r", "+r", "rJ", },
    { "=r", "+r", "rJ", },
    { "+r", "rJ", "r", },
    { "r", "n", "+r", "r", "", },
    { "=r", "r", },
    { "=r", "=r", "=r", "=r", "m", "m", "m", "m", },
    { "=r", "=r", "=r", "m", "m", "m", },
    { "=r", "=r", "m", "m", },
    { "r", "r", "r", "r", "=m", "=m", "=m", "=m", },
    { "r", "r", "r", "=m", "=m", "=m", },
    { "r", "r", "=m", "=m", },
    { "X", "g", },
    { "=rf", "X", "g", },
    { "=rf", "X", "g", },
    { "", "", "", "", "", "", },
    { "", "", "", },
    { 0 },
    { "", "", "", "", "", "", },
    { "=r", "r", "n", },
    { "=r", "r", "n", },
    { "=m", "r", "", },
    { "=m", "f", "", },
    { "", },
    { "", },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { "", "", },
    { "", "", },
    { "", "", },
    { "", "", },
    { "=r", "", },
  };

const enum machine_mode insn_operand_mode[][MAX_RECOG_OPERANDS] =
  {
    { DImode, DImode, DImode, },
    { DImode, DImode, SImode, },
    { DImode, DImode, SImode, },
    { SImode, SImode, SImode, },
    { VOIDmode, VOIDmode, VOIDmode, },
    { SImode, SImode, SImode, },
    { SImode, SImode, SImode, },
    { SImode, SImode, },
    { SImode, SImode, SImode, },
    { SImode, SImode, SImode, },
    { SImode, SImode, },
    { SImode, SImode, },
    { SImode, SImode, SImode, },
    { SImode, SImode, SImode, },
    { SImode, SImode, SImode, },
    { SImode, SImode, SImode, },
    { SImode, SImode, SImode, VOIDmode, },
    { VOIDmode, VOIDmode, VOIDmode, },
    { SFmode, SFmode, SFmode, },
    { DFmode, DFmode, DFmode, },
    { DFmode, SFmode, DFmode, },
    { DFmode, DFmode, SFmode, },
    { DFmode, SFmode, SFmode, },
    { XFmode, XFmode, XFmode, },
    { DImode, DImode, DImode, },
    { DImode, DImode, SImode, },
    { DImode, DImode, SImode, },
    { DImode, DImode, SImode, },
    { DImode, DImode, SImode, },
    { DImode, SImode, SImode, },
    { SImode, SImode, SImode, },
    { SImode, SImode, SImode, },
    { VOIDmode, VOIDmode, VOIDmode, },
    { SImode, SImode, SImode, },
    { SImode, SImode, SImode, VOIDmode, },
    { SFmode, SFmode, SFmode, },
    { DFmode, DFmode, DFmode, },
    { DFmode, SFmode, DFmode, },
    { DFmode, DFmode, SFmode, },
    { DFmode, SFmode, SFmode, },
    { XFmode, XFmode, XFmode, },
    { SImode, SImode, SImode, },
    { SImode, SImode, SImode, },
    { SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, },
    { DImode, SImode, SImode, },
    { DImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, },
    { SFmode, SFmode, SFmode, },
    { DFmode, DFmode, DFmode, },
    { DFmode, SFmode, DFmode, },
    { DFmode, DFmode, SFmode, },
    { DFmode, SFmode, SFmode, },
    { XFmode, XFmode, XFmode, },
    { SFmode, SFmode, SFmode, },
    { DFmode, DFmode, DFmode, },
    { DFmode, SFmode, DFmode, },
    { DFmode, DFmode, SFmode, },
    { DFmode, SFmode, SFmode, },
    { XFmode, XFmode, XFmode, },
    { SFmode, SFmode, SFmode, },
    { DFmode, DFmode, DFmode, },
    { DFmode, SFmode, DFmode, },
    { DFmode, DFmode, SFmode, },
    { DFmode, SFmode, SFmode, },
    { XFmode, XFmode, XFmode, },
    { DImode, DImode, DImode, },
    { DImode, DImode, SImode, },
    { DImode, DImode, SImode, },
    { SImode, SImode, SImode, },
    { SImode, SImode, SImode, },
    { VOIDmode, VOIDmode, VOIDmode, },
    { SImode, SImode, SImode, },
    { SImode, SImode, VOIDmode, SImode, },
    { SImode, VOIDmode, VOIDmode, },
    { SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, },
    { DImode, DImode, DImode, },
    { DImode, DImode, SImode, },
    { DImode, DImode, SImode, },
    { SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, },
    { SImode, SImode, SImode, },
    { DImode, DImode, DImode, },
    { DImode, DImode, SImode, },
    { DImode, DImode, SImode, },
    { SImode, SImode, SImode, },
    { SImode, SImode, SImode, },
    { VOIDmode, VOIDmode, VOIDmode, },
    { SImode, SImode, SImode, },
    { SImode, SImode, SImode, },
    { DImode, DImode, DImode, },
    { DImode, DImode, SImode, },
    { DImode, DImode, SImode, },
    { SImode, SImode, SImode, },
    { SImode, SImode, SImode, },
    { SImode, SImode, },
    { VOIDmode, VOIDmode, VOIDmode, VOIDmode, VOIDmode, },
    { SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, },
    { SImode, SImode, SImode, },
    { SImode, SImode, SImode, },
    { SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, },
    { SImode, SImode, SImode, },
    { SImode, SImode, SImode, },
    { SImode, SImode, SImode, },
    { SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, },
    { DImode, DImode, },
    { SImode, SImode, },
    { SFmode, SFmode, },
    { DFmode, DFmode, },
    { DFmode, SFmode, },
    { XFmode, XFmode, },
    { SImode, SImode, },
    { SImode, SImode, },
    { SFmode, SFmode, },
    { DFmode, DFmode, },
    { DFmode, SFmode, },
    { XFmode, XFmode, },
    { SFmode, SFmode, },
    { DFmode, DFmode, },
    { DFmode, SFmode, },
    { XFmode, XFmode, },
    { DImode, DImode, },
    { SImode, SImode, },
    { SImode, SImode, },
    { SImode, SImode, },
    { SFmode, SImode, },
    { DFmode, SImode, },
    { XFmode, SImode, },
    { SImode, SFmode, },
    { SImode, DFmode, },
    { SImode, XFmode, },
    { SFmode, DFmode, },
    { SFmode, XFmode, },
    { DFmode, XFmode, },
    { DImode, SImode, },
    { DImode, QImode, },
    { DImode, SImode, },
    { SImode, HImode, },
    { SImode, HImode, },
    { VOIDmode, VOIDmode, VOIDmode, },
    { VOIDmode, VOIDmode, VOIDmode, VOIDmode, VOIDmode, },
    { SImode, QImode, },
    { SImode, QImode, },
    { VOIDmode, VOIDmode, VOIDmode, },
    { QImode, },
    { SImode, HImode, },
    { SImode, HImode, },
    { SImode, HImode, },
    { VOIDmode, VOIDmode, VOIDmode, },
    { VOIDmode, VOIDmode, VOIDmode, VOIDmode, VOIDmode, },
    { HImode, QImode, },
    { HImode, QImode, },
    { VOIDmode, VOIDmode, },
    { SImode, QImode, },
    { SImode, QImode, },
    { VOIDmode, VOIDmode, },
    { DFmode, SFmode, },
    { XFmode, SFmode, },
    { XFmode, DFmode, },
    { DImode, DImode, },
    { SImode, SImode, },
    { SImode, SImode, },
    { VOIDmode, VOIDmode, },
    { SImode, DImode, },
    { SImode, DImode, },
    { SImode, VOIDmode, },
    { SImode, VOIDmode, },
    { SImode, VOIDmode, VOIDmode, },
    { VOIDmode, VOIDmode, },
    { SImode, SImode, },
    { VOIDmode, VOIDmode, },
    { VOIDmode, VOIDmode, },
    { VOIDmode, VOIDmode, },
    { HImode, HImode, },
    { HImode, HImode, },
    { SImode, SImode, },
    { SImode, HImode, },
    { HImode, HImode, },
    { HImode, HImode, },
    { HImode, HImode, },
    { HImode, HImode, },
    { SImode, HImode, },
    { HImode, HImode, },
    { HImode, HImode, DImode, },
    { HImode, HImode, DImode, },
    { QImode, QImode, },
    { QImode, QImode, },
    { SFmode, SFmode, },
    { SFmode, SFmode, },
    { SFmode, SFmode, },
    { DFmode, DFmode, },
    { DFmode, DFmode, SImode, },
    { DFmode, DFmode, },
    { DFmode, DFmode, },
    { XFmode, XFmode, },
    { XFmode, XFmode, },
    { VOIDmode },
    { VOIDmode, SImode, SImode, SImode, },
    { VOIDmode, SImode, SImode, },
    { VOIDmode },
    { VOIDmode, SImode, SImode, SImode, },
    { VOIDmode, SImode, SImode, },
    { BLKmode, BLKmode, SImode, SImode, },
    { SImode, SImode, },
    { SFmode, SFmode, },
    { DFmode, DFmode, },
    { XFmode, XFmode, },
    { SImode, SImode, },
    { SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, },
    { SFmode, SFmode, },
    { DFmode, DFmode, },
    { SFmode, DFmode, },
    { DFmode, SFmode, },
    { XFmode, XFmode, },
    { SFmode, SFmode, },
    { DFmode, DFmode, },
    { SFmode, DFmode, },
    { DFmode, SFmode, },
    { XFmode, XFmode, },
    { VOIDmode, },
    { VOIDmode, },
    { VOIDmode, },
    { VOIDmode, },
    { VOIDmode, },
    { VOIDmode, },
    { VOIDmode, },
    { VOIDmode, },
    { VOIDmode, },
    { VOIDmode, },
    { VOIDmode, },
    { VOIDmode, VOIDmode, VOIDmode, },
    { VOIDmode, VOIDmode, VOIDmode, },
    { SImode, },
    { SImode, },
    { SImode, },
    { SImode, },
    { SImode, },
    { SImode, },
    { SImode, },
    { SImode, },
    { SImode, },
    { SImode, },
    { SImode, SImode, VOIDmode, },
    { SImode, SImode, VOIDmode, },
    { SImode, SImode, VOIDmode, },
    { SImode, VOIDmode, SImode, SImode, },
    { SFmode, VOIDmode, SFmode, SFmode, },
    { DFmode, VOIDmode, DFmode, DFmode, },
    { SImode, SImode, SImode, VOIDmode, VOIDmode, },
    { SFmode, SFmode, SFmode, VOIDmode, VOIDmode, },
    { SFmode, SFmode, SFmode, VOIDmode, VOIDmode, },
    { DFmode, DFmode, DFmode, VOIDmode, VOIDmode, },
    { VOIDmode, },
    { VOIDmode, VOIDmode, },
    { SImode, VOIDmode, },
    { VOIDmode, VOIDmode, },
    { VOIDmode, VOIDmode, VOIDmode, },
    { VOIDmode, SImode, VOIDmode, },
    { VOIDmode, VOIDmode, VOIDmode, },
    { SImode, SImode, },
    { VOIDmode, SImode, SImode, },
    { VOIDmode },
    { VOIDmode, VOIDmode, },
    { VOIDmode, VOIDmode, },
    { VOIDmode, VOIDmode, VOIDmode, },
    { VOIDmode },
    { SImode, SImode, SImode, SImode, SImode, },
    { SImode, SImode, VOIDmode, VOIDmode, },
    { SImode, },
    { SImode, },
    { VOIDmode },
    { SImode, SImode, SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, SImode, },
    { SImode, VOIDmode, SImode, VOIDmode, },
    { SImode, SImode, VOIDmode, VOIDmode, },
    { SImode, VOIDmode, SImode, SImode, },
    { SImode, SImode, SImode, VOIDmode, VOIDmode, VOIDmode, },
    { SImode, SImode, SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, VOIDmode, SImode, VOIDmode, },
    { SImode, SImode, SImode, SImode, VOIDmode, SImode, VOIDmode, },
    { SImode, SImode, SImode, VOIDmode, },
    { SImode, SImode, SImode, SImode, SImode, VOIDmode, },
    { SImode, SImode, SImode, SImode, SImode, SImode, VOIDmode, },
    { SImode, SImode, SImode, SImode, VOIDmode, VOIDmode, },
    { SImode, SImode, SImode, SImode, SImode, SImode, VOIDmode, },
    { SImode, SImode, SImode, SImode, VOIDmode, VOIDmode, },
    { SImode, SImode, SImode, SImode, SImode, SImode, SImode, SImode, SImode, VOIDmode, },
    { SImode, SImode, SImode, SImode, SImode, VOIDmode, SImode, SImode, VOIDmode, },
    { SImode, SImode, SImode, SImode, SImode, SImode, VOIDmode, SImode, },
    { SImode, SImode, SImode, SImode, VOIDmode, SImode, VOIDmode, },
    { SImode, SImode, SImode, SImode, SImode, SImode, VOIDmode, SImode, },
    { SImode, SImode, SImode, SImode, VOIDmode, SImode, VOIDmode, },
    { SImode, SImode, SImode, SImode, SImode, VOIDmode, },
    { SImode, SImode, SImode, VOIDmode, VOIDmode, },
    { SImode, SImode, SImode, SImode, SImode, VOIDmode, },
    { SImode, SImode, SImode, VOIDmode, VOIDmode, },
    { SImode, SImode, SImode, SImode, SImode, SImode, VOIDmode, SImode, },
    { SImode, SImode, SImode, SImode, SImode, VOIDmode, VOIDmode, },
    { SImode, SImode, SImode, SImode, SImode, SImode, VOIDmode, SImode, },
    { SImode, SImode, SImode, SImode, SImode, VOIDmode, VOIDmode, },
    { SImode, SImode, SImode, SImode, SImode, SImode, SImode, VOIDmode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, SImode, VOIDmode, SImode, SImode, VOIDmode, },
    { SImode, SImode, SImode, SImode, SImode, SImode, VOIDmode, SImode, },
    { SImode, SImode, SImode, SImode, VOIDmode, VOIDmode, SImode, },
    { SImode, SImode, SImode, SImode, SImode, SImode, VOIDmode, SImode, },
    { SImode, SImode, SImode, SImode, VOIDmode, VOIDmode, SImode, },
    { SImode, SImode, SImode, SImode, SImode, VOIDmode, },
    { SImode, SImode, SImode, VOIDmode, VOIDmode, },
    { SImode, SImode, SImode, SImode, SImode, VOIDmode, },
    { SImode, SImode, SImode, VOIDmode, VOIDmode, },
    { SImode, SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, QImode, },
    { SImode, SImode, SImode, QImode, },
    { SImode, SImode, SImode, QImode, },
    { SImode, SImode, SImode, QImode, },
    { SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, HImode, },
    { SImode, SImode, SImode, HImode, },
    { SImode, SImode, SImode, SImode, SImode, QImode, },
    { SImode, SImode, SImode, SImode, SImode, QImode, },
    { SImode, SImode, SImode, SImode, SImode, QImode, },
    { SImode, SImode, SImode, SImode, SImode, QImode, },
    { SImode, SImode, SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, SImode, SImode, },
    { SImode, SImode, SImode, SImode, SImode, HImode, },
    { SImode, SImode, SImode, SImode, SImode, HImode, },
    { VOIDmode, VOIDmode, VOIDmode, },
    { VOIDmode, VOIDmode, VOIDmode, },
    { VOIDmode, VOIDmode, VOIDmode, },
    { VOIDmode, VOIDmode, VOIDmode, },
    { VOIDmode, VOIDmode, VOIDmode, },
    { VOIDmode, VOIDmode, VOIDmode, },
    { VOIDmode, VOIDmode, VOIDmode, VOIDmode, VOIDmode, },
    { VOIDmode, VOIDmode, },
    { VOIDmode, VOIDmode, VOIDmode, VOIDmode, VOIDmode, VOIDmode, VOIDmode, VOIDmode, },
    { VOIDmode, VOIDmode, VOIDmode, VOIDmode, VOIDmode, VOIDmode, },
    { VOIDmode, VOIDmode, VOIDmode, VOIDmode, },
    { VOIDmode, VOIDmode, VOIDmode, VOIDmode, VOIDmode, VOIDmode, VOIDmode, VOIDmode, },
    { VOIDmode, VOIDmode, VOIDmode, VOIDmode, VOIDmode, VOIDmode, },
    { VOIDmode, VOIDmode, VOIDmode, VOIDmode, },
    { VOIDmode, VOIDmode, },
    { VOIDmode, VOIDmode, VOIDmode, },
    { VOIDmode, VOIDmode, VOIDmode, },
    { VOIDmode, VOIDmode, VOIDmode, VOIDmode, VOIDmode, VOIDmode, },
    { VOIDmode, VOIDmode, VOIDmode, },
    { VOIDmode },
    { VOIDmode, VOIDmode, VOIDmode, VOIDmode, VOIDmode, VOIDmode, },
    { SImode, SImode, SImode, },
    { SImode, SImode, SImode, },
    { BLKmode, SImode, VOIDmode, },
    { BLKmode, XFmode, VOIDmode, },
    { VOIDmode, },
    { VOIDmode, },
    { VOIDmode },
    { VOIDmode },
    { VOIDmode },
    { VOIDmode },
    { SImode, SImode, },
    { SImode, SImode, },
    { SImode, SImode, },
    { SImode, SImode, },
    { SImode, SImode, },
  };

const char insn_operand_strict_low[][MAX_RECOG_OPERANDS] =
  {
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, },
    { 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, 0, },
    { 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0 },
    { 0, 0, 0, 0, },
    { 0, 0, 0, },
    { 0 },
    { 0, 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, },
    { 0, },
    { 0, },
    { 0, },
    { 0, },
    { 0, },
    { 0, },
    { 0, },
    { 0, },
    { 0, },
    { 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, },
    { 0, },
    { 0, },
    { 0, },
    { 0, },
    { 0, },
    { 0, },
    { 0, },
    { 0, },
    { 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, },
    { 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, },
    { 0, 0, 0, },
    { 0 },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, 0, },
    { 0 },
    { 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, },
    { 0, },
    { 0 },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, 0, 0, },
    { 0, 0, },
    { 0, 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, },
    { 0 },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, },
    { 0, },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
  };

extern int s_register_operand ();
extern int reg_or_int_operand ();
extern int arm_add_operand ();
extern int arm_rhs_operand ();
extern int comparison_operator ();
extern int cc_register ();
extern int fpu_add_operand ();
extern int fpu_rhs_operand ();
extern int scratch_operand ();
extern int arm_not_operand ();
extern int const_int_operand ();
extern int general_operand ();
extern int nonmemory_operand ();
extern int shift_operator ();
extern int memory_operand ();
extern int minmax_operator ();
extern int shiftable_operator ();
extern int nonimmediate_operand ();
extern int di_operand ();
extern int address_operand ();
extern int register_operand ();
extern int offsettable_memory_operand ();
extern int reload_memory_operand ();
extern int soft_df_operand ();
extern int load_multiple_operation ();
extern int store_multiple_operation ();
extern int equality_operator ();
extern int dominant_cc_register ();
extern int index_operand ();
extern int const_shift_operand ();
extern int multi_register_push ();
extern int f_register_operand ();

int (*const insn_operand_predicate[][MAX_RECOG_OPERANDS])() =
  {
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, reg_or_int_operand, },
    { 0, 0, 0, },
    { s_register_operand, s_register_operand, reg_or_int_operand, },
    { s_register_operand, s_register_operand, arm_add_operand, },
    { s_register_operand, arm_add_operand, },
    { s_register_operand, s_register_operand, arm_add_operand, },
    { s_register_operand, s_register_operand, arm_add_operand, },
    { s_register_operand, arm_add_operand, },
    { s_register_operand, arm_add_operand, },
    { s_register_operand, s_register_operand, arm_rhs_operand, },
    { s_register_operand, s_register_operand, arm_rhs_operand, },
    { s_register_operand, s_register_operand, arm_rhs_operand, },
    { s_register_operand, s_register_operand, arm_rhs_operand, },
    { s_register_operand, s_register_operand, comparison_operator, cc_register, },
    { 0, 0, 0, },
    { s_register_operand, s_register_operand, fpu_add_operand, },
    { s_register_operand, s_register_operand, fpu_add_operand, },
    { s_register_operand, s_register_operand, fpu_add_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, fpu_add_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, reg_or_int_operand, s_register_operand, },
    { s_register_operand, reg_or_int_operand, s_register_operand, },
    { 0, 0, 0, },
    { s_register_operand, arm_rhs_operand, arm_rhs_operand, },
    { s_register_operand, s_register_operand, comparison_operator, cc_register, },
    { s_register_operand, fpu_rhs_operand, fpu_rhs_operand, },
    { s_register_operand, fpu_rhs_operand, fpu_rhs_operand, },
    { s_register_operand, s_register_operand, fpu_rhs_operand, },
    { s_register_operand, fpu_rhs_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, fpu_rhs_operand, fpu_rhs_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { scratch_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, s_register_operand, },
    { scratch_operand, s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, scratch_operand, },
    { s_register_operand, s_register_operand, s_register_operand, scratch_operand, },
    { s_register_operand, s_register_operand, fpu_rhs_operand, },
    { s_register_operand, s_register_operand, fpu_rhs_operand, },
    { s_register_operand, s_register_operand, fpu_rhs_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, fpu_rhs_operand, },
    { s_register_operand, fpu_rhs_operand, fpu_rhs_operand, },
    { s_register_operand, fpu_rhs_operand, fpu_rhs_operand, },
    { s_register_operand, s_register_operand, fpu_rhs_operand, },
    { s_register_operand, fpu_rhs_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, fpu_rhs_operand, fpu_rhs_operand, },
    { s_register_operand, s_register_operand, fpu_rhs_operand, },
    { s_register_operand, s_register_operand, fpu_rhs_operand, },
    { s_register_operand, s_register_operand, fpu_rhs_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, fpu_rhs_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, reg_or_int_operand, },
    { s_register_operand, s_register_operand, reg_or_int_operand, },
    { 0, 0, 0, },
    { s_register_operand, s_register_operand, arm_not_operand, },
    { s_register_operand, arm_not_operand, 0, scratch_operand, },
    { s_register_operand, const_int_operand, const_int_operand, },
    { s_register_operand, s_register_operand, const_int_operand, const_int_operand, },
    { s_register_operand, general_operand, general_operand, nonmemory_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, arm_rhs_operand, shift_operator, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { scratch_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, reg_or_int_operand, },
    { s_register_operand, s_register_operand, reg_or_int_operand, },
    { 0, 0, 0, },
    { s_register_operand, s_register_operand, arm_rhs_operand, },
    { scratch_operand, s_register_operand, arm_rhs_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, arm_rhs_operand, },
    { s_register_operand, s_register_operand, arm_rhs_operand, },
    { s_register_operand, arm_rhs_operand, },
    { 0, 0, 0, 0, 0, },
    { s_register_operand, s_register_operand, arm_rhs_operand, arm_rhs_operand, },
    { s_register_operand, s_register_operand, arm_rhs_operand, },
    { s_register_operand, s_register_operand, arm_rhs_operand, },
    { s_register_operand, s_register_operand, arm_rhs_operand, },
    { s_register_operand, s_register_operand, arm_rhs_operand, },
    { memory_operand, s_register_operand, s_register_operand, minmax_operator, },
    { s_register_operand, s_register_operand, s_register_operand, arm_rhs_operand, shiftable_operator, minmax_operator, },
    { s_register_operand, s_register_operand, arm_rhs_operand, },
    { s_register_operand, s_register_operand, arm_rhs_operand, },
    { s_register_operand, s_register_operand, arm_rhs_operand, },
    { s_register_operand, s_register_operand, reg_or_int_operand, },
    { s_register_operand, s_register_operand, arm_rhs_operand, },
    { s_register_operand, s_register_operand, reg_or_int_operand, shift_operator, },
    { s_register_operand, s_register_operand, arm_rhs_operand, shift_operator, },
    { scratch_operand, s_register_operand, arm_rhs_operand, shift_operator, },
    { s_register_operand, s_register_operand, arm_rhs_operand, shift_operator, },
    { s_register_operand, s_register_operand, arm_rhs_operand, shift_operator, },
    { scratch_operand, s_register_operand, arm_rhs_operand, shift_operator, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, },
    { scratch_operand, s_register_operand, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, nonimmediate_operand, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, nonimmediate_operand, },
    { s_register_operand, memory_operand, },
    { 0, 0, 0, },
    { 0, 0, 0, 0, 0, },
    { s_register_operand, nonimmediate_operand, },
    { s_register_operand, memory_operand, },
    { 0, 0, 0, },
    { s_register_operand, },
    { s_register_operand, nonimmediate_operand, },
    { 0, 0, },
    { s_register_operand, memory_operand, },
    { 0, 0, 0, },
    { 0, 0, 0, 0, 0, },
    { s_register_operand, general_operand, },
    { s_register_operand, memory_operand, },
    { 0, 0, },
    { s_register_operand, general_operand, },
    { s_register_operand, memory_operand, },
    { 0, 0, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, },
    { di_operand, di_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { 0, 0, },
    { s_register_operand, address_operand, },
    { s_register_operand, address_operand, },
    { s_register_operand, 0, },
    { s_register_operand, 0, },
    { s_register_operand, 0, s_register_operand, },
    { register_operand, 0, },
    { s_register_operand, s_register_operand, },
    { 0, 0, },
    { 0, 0, },
    { 0, 0, },
    { memory_operand, general_operand, },
    { general_operand, general_operand, },
    { s_register_operand, offsettable_memory_operand, },
    { 0, 0, },
    { s_register_operand, memory_operand, },
    { general_operand, general_operand, },
    { s_register_operand, general_operand, },
    { s_register_operand, general_operand, },
    { s_register_operand, memory_operand, },
    { s_register_operand, arm_rhs_operand, },
    { reload_memory_operand, s_register_operand, s_register_operand, },
    { s_register_operand, reload_memory_operand, s_register_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { reload_memory_operand, s_register_operand, s_register_operand, },
    { general_operand, general_operand, },
    { soft_df_operand, soft_df_operand, },
    { general_operand, general_operand, },
    { general_operand, general_operand, },
    { 0 },
    { load_multiple_operation, s_register_operand, const_int_operand, s_register_operand, },
    { load_multiple_operation, s_register_operand, s_register_operand, },
    { 0 },
    { store_multiple_operation, s_register_operand, const_int_operand, s_register_operand, },
    { store_multiple_operation, s_register_operand, s_register_operand, },
    { general_operand, general_operand, const_int_operand, const_int_operand, },
    { s_register_operand, arm_add_operand, },
    { s_register_operand, fpu_rhs_operand, },
    { s_register_operand, fpu_rhs_operand, },
    { s_register_operand, fpu_rhs_operand, },
    { s_register_operand, arm_add_operand, },
    { s_register_operand, s_register_operand, arm_rhs_operand, shift_operator, },
    { s_register_operand, s_register_operand, reg_or_int_operand, shift_operator, },
    { s_register_operand, s_register_operand, arm_rhs_operand, shift_operator, },
    { s_register_operand, fpu_add_operand, },
    { s_register_operand, fpu_add_operand, },
    { s_register_operand, fpu_add_operand, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, fpu_add_operand, },
    { s_register_operand, fpu_add_operand, },
    { s_register_operand, fpu_add_operand, },
    { s_register_operand, fpu_add_operand, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, fpu_add_operand, },
    { cc_register, },
    { 0, },
    { 0, },
    { 0, },
    { 0, },
    { 0, },
    { 0, },
    { 0, },
    { 0, },
    { 0, },
    { 0, },
    { 0, comparison_operator, cc_register, },
    { 0, comparison_operator, cc_register, },
    { s_register_operand, },
    { s_register_operand, },
    { s_register_operand, },
    { s_register_operand, },
    { s_register_operand, },
    { s_register_operand, },
    { s_register_operand, },
    { s_register_operand, },
    { s_register_operand, },
    { s_register_operand, },
    { s_register_operand, comparison_operator, cc_register, },
    { s_register_operand, comparison_operator, cc_register, },
    { s_register_operand, comparison_operator, cc_register, },
    { s_register_operand, comparison_operator, arm_not_operand, arm_not_operand, },
    { s_register_operand, comparison_operator, s_register_operand, nonmemory_operand, },
    { s_register_operand, comparison_operator, s_register_operand, fpu_add_operand, },
    { s_register_operand, arm_not_operand, arm_not_operand, comparison_operator, cc_register, },
    { s_register_operand, fpu_add_operand, fpu_add_operand, comparison_operator, cc_register, },
    { s_register_operand, s_register_operand, s_register_operand, comparison_operator, cc_register, },
    { s_register_operand, fpu_add_operand, fpu_add_operand, comparison_operator, cc_register, },
    { 0, },
    { memory_operand, general_operand, },
    { s_register_operand, 0, },
    { memory_operand, general_operand, },
    { 0, memory_operand, general_operand, },
    { 0, s_register_operand, general_operand, },
    { 0, memory_operand, general_operand, },
    { 0, general_operand, },
    { s_register_operand, 0, general_operand, },
    { 0 },
    { comparison_operator, cc_register, },
    { comparison_operator, cc_register, },
    { 0, 0, 0, },
    { 0 },
    { s_register_operand, const_int_operand, const_int_operand, 0, 0, },
    { s_register_operand, arm_rhs_operand, 0, 0, },
    { s_register_operand, },
    { memory_operand, },
    { 0 },
    { s_register_operand, shiftable_operator, s_register_operand, shift_operator, s_register_operand, reg_or_int_operand, },
    { s_register_operand, shiftable_operator, s_register_operand, shift_operator, s_register_operand, reg_or_int_operand, },
    { scratch_operand, shiftable_operator, s_register_operand, shift_operator, s_register_operand, reg_or_int_operand, },
    { s_register_operand, s_register_operand, shift_operator, s_register_operand, reg_or_int_operand, },
    { s_register_operand, s_register_operand, shift_operator, s_register_operand, reg_or_int_operand, },
    { scratch_operand, s_register_operand, shift_operator, s_register_operand, reg_or_int_operand, },
    { 0, const_int_operand, 0, 0, 0, shift_operator, },
    { 0, 0, const_int_operand, 0, 0, shift_operator, },
    { scratch_operand, 0, const_int_operand, 0, 0, shift_operator, },
    { 0, 0, 0, 0, const_int_operand, },
    { 0, 0, const_int_operand, 0, 0, },
    { scratch_operand, 0, const_int_operand, 0, 0, },
    { s_register_operand, comparison_operator, s_register_operand, cc_register, },
    { s_register_operand, s_register_operand, comparison_operator, cc_register, },
    { s_register_operand, comparison_operator, s_register_operand, arm_add_operand, },
    { s_register_operand, arm_rhs_operand, arm_rhs_operand, equality_operator, comparison_operator, cc_register, },
    { s_register_operand, s_register_operand, s_register_operand, arm_rhs_operand, comparison_operator, shiftable_operator, },
    { s_register_operand, s_register_operand, s_register_operand, arm_rhs_operand, comparison_operator, },
    { s_register_operand, arm_add_operand, s_register_operand, arm_add_operand, comparison_operator, comparison_operator, dominant_cc_register, },
    { s_register_operand, arm_add_operand, s_register_operand, arm_add_operand, comparison_operator, comparison_operator, dominant_cc_register, },
    { s_register_operand, s_register_operand, arm_rhs_operand, comparison_operator, },
    { s_register_operand, arm_rhs_operand, arm_rhs_operand, s_register_operand, arm_add_operand, comparison_operator, },
    { s_register_operand, arm_rhs_operand, s_register_operand, arm_add_operand, s_register_operand, arm_add_operand, comparison_operator, },
    { s_register_operand, arm_rhs_operand, s_register_operand, arm_add_operand, comparison_operator, cc_register, },
    { s_register_operand, arm_rhs_operand, s_register_operand, arm_add_operand, s_register_operand, arm_add_operand, comparison_operator, },
    { s_register_operand, arm_rhs_operand, s_register_operand, arm_add_operand, comparison_operator, cc_register, },
    { s_register_operand, s_register_operand, arm_rhs_operand, s_register_operand, arm_rhs_operand, s_register_operand, arm_add_operand, shiftable_operator, shiftable_operator, comparison_operator, },
    { s_register_operand, s_register_operand, arm_rhs_operand, s_register_operand, arm_rhs_operand, comparison_operator, shiftable_operator, shiftable_operator, cc_register, },
    { s_register_operand, arm_rhs_operand, s_register_operand, arm_add_operand, s_register_operand, arm_rhs_operand, comparison_operator, shiftable_operator, },
    { s_register_operand, arm_rhs_operand, s_register_operand, arm_rhs_operand, comparison_operator, shiftable_operator, cc_register, },
    { s_register_operand, arm_rhs_operand, s_register_operand, arm_rhs_operand, s_register_operand, arm_add_operand, comparison_operator, shiftable_operator, },
    { s_register_operand, arm_rhs_operand, s_register_operand, arm_rhs_operand, comparison_operator, shiftable_operator, cc_register, },
    { s_register_operand, arm_not_operand, s_register_operand, s_register_operand, arm_add_operand, comparison_operator, },
    { s_register_operand, arm_not_operand, s_register_operand, cc_register, comparison_operator, },
    { s_register_operand, arm_not_operand, s_register_operand, s_register_operand, arm_add_operand, comparison_operator, },
    { s_register_operand, arm_not_operand, s_register_operand, cc_register, comparison_operator, },
    { s_register_operand, arm_not_operand, s_register_operand, arm_rhs_operand, s_register_operand, arm_add_operand, comparison_operator, shift_operator, },
    { s_register_operand, arm_not_operand, s_register_operand, arm_rhs_operand, shift_operator, comparison_operator, cc_register, },
    { s_register_operand, arm_not_operand, s_register_operand, arm_rhs_operand, s_register_operand, arm_add_operand, comparison_operator, shift_operator, },
    { s_register_operand, arm_not_operand, s_register_operand, arm_rhs_operand, shift_operator, comparison_operator, cc_register, },
    { s_register_operand, s_register_operand, arm_rhs_operand, s_register_operand, arm_rhs_operand, s_register_operand, arm_add_operand, comparison_operator, shift_operator, shift_operator, },
    { s_register_operand, s_register_operand, arm_rhs_operand, s_register_operand, arm_rhs_operand, comparison_operator, shift_operator, shift_operator, cc_register, },
    { s_register_operand, s_register_operand, s_register_operand, arm_rhs_operand, s_register_operand, arm_add_operand, comparison_operator, shiftable_operator, },
    { s_register_operand, s_register_operand, s_register_operand, arm_rhs_operand, cc_register, comparison_operator, shiftable_operator, },
    { s_register_operand, s_register_operand, s_register_operand, arm_rhs_operand, s_register_operand, arm_add_operand, comparison_operator, shiftable_operator, },
    { s_register_operand, s_register_operand, s_register_operand, arm_rhs_operand, cc_register, comparison_operator, shiftable_operator, },
    { s_register_operand, arm_not_operand, s_register_operand, s_register_operand, arm_add_operand, comparison_operator, },
    { s_register_operand, arm_not_operand, s_register_operand, cc_register, comparison_operator, },
    { s_register_operand, arm_not_operand, s_register_operand, s_register_operand, arm_add_operand, comparison_operator, },
    { s_register_operand, arm_not_operand, s_register_operand, cc_register, comparison_operator, },
    { s_register_operand, shiftable_operator, memory_operand, memory_operand, scratch_operand, },
    { s_register_operand, s_register_operand, index_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, index_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, index_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, index_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, index_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, index_operand, s_register_operand, },
    { s_register_operand, s_register_operand, s_register_operand, s_register_operand, },
    { s_register_operand, s_register_operand, shift_operator, s_register_operand, const_shift_operand, s_register_operand, },
    { s_register_operand, s_register_operand, shift_operator, s_register_operand, const_shift_operand, s_register_operand, },
    { s_register_operand, s_register_operand, shift_operator, s_register_operand, const_shift_operand, s_register_operand, },
    { s_register_operand, s_register_operand, shift_operator, s_register_operand, const_shift_operand, s_register_operand, },
    { s_register_operand, s_register_operand, shift_operator, s_register_operand, const_shift_operand, s_register_operand, },
    { s_register_operand, s_register_operand, shift_operator, s_register_operand, const_shift_operand, s_register_operand, },
    { s_register_operand, s_register_operand, shift_operator, s_register_operand, const_shift_operand, s_register_operand, },
    { s_register_operand, s_register_operand, shift_operator, s_register_operand, const_shift_operand, s_register_operand, },
    { s_register_operand, s_register_operand, shift_operator, s_register_operand, const_shift_operand, s_register_operand, },
    { s_register_operand, s_register_operand, shift_operator, s_register_operand, const_shift_operand, s_register_operand, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, 0, 0, },
    { 0, 0, },
    { 0, 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, },
    { 0, 0, 0, 0, 0, 0, },
    { 0, 0, 0, },
    { 0 },
    { 0, 0, 0, 0, 0, 0, },
    { s_register_operand, s_register_operand, const_int_operand, },
    { s_register_operand, s_register_operand, const_int_operand, },
    { memory_operand, s_register_operand, multi_register_push, },
    { memory_operand, f_register_operand, multi_register_push, },
    { 0, },
    { 0, },
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { memory_operand, s_register_operand, },
    { memory_operand, s_register_operand, },
    { s_register_operand, s_register_operand, },
    { s_register_operand, memory_operand, },
    { s_register_operand, reg_or_int_operand, },
  };

const int insn_n_alternatives[] =
  {
    2,
    2,
    2,
    0,
    0,
    3,
    2,
    2,
    2,
    2,
    2,
    2,
    1,
    1,
    1,
    1,
    2,
    0,
    2,
    2,
    2,
    1,
    1,
    2,
    3,
    2,
    2,
    2,
    2,
    1,
    0,
    2,
    0,
    2,
    2,
    2,
    2,
    1,
    2,
    1,
    2,
    2,
    2,
    2,
    4,
    4,
    4,
    1,
    1,
    2,
    2,
    1,
    1,
    1,
    1,
    1,
    1,
    2,
    2,
    1,
    1,
    1,
    2,
    1,
    1,
    1,
    1,
    1,
    1,
    2,
    2,
    2,
    0,
    3,
    0,
    2,
    2,
    1,
    1,
    0,
    2,
    2,
    2,
    1,
    1,
    1,
    1,
    1,
    2,
    2,
    0,
    2,
    0,
    1,
    1,
    2,
    2,
    2,
    1,
    1,
    1,
    0,
    3,
    3,
    3,
    3,
    3,
    1,
    2,
    0,
    0,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    2,
    1,
    1,
    1,
    1,
    1,
    2,
    2,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    2,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    2,
    1,
    0,
    1,
    0,
    0,
    2,
    1,
    0,
    1,
    0,
    0,
    1,
    0,
    0,
    0,
    1,
    0,
    0,
    1,
    0,
    1,
    1,
    1,
    3,
    0,
    4,
    0,
    0,
    1,
    1,
    1,
    1,
    1,
    2,
    0,
    0,
    0,
    0,
    0,
    1,
    0,
    0,
    4,
    3,
    3,
    1,
    2,
    1,
    1,
    0,
    4,
    0,
    9,
    3,
    0,
    1,
    11,
    3,
    0,
    7,
    0,
    1,
    1,
    0,
    1,
    1,
    0,
    0,
    0,
    0,
    0,
    2,
    1,
    1,
    1,
    2,
    2,
    2,
    1,
    2,
    2,
    2,
    2,
    1,
    2,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    0,
    0,
    0,
    8,
    8,
    2,
    8,
    0,
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    1,
    1,
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    2,
    1,
    1,
    1,
    2,
    2,
    3,
    2,
    2,
    4,
    4,
    1,
    3,
    2,
    4,
    2,
    4,
    1,
    1,
    2,
    2,
    2,
    2,
    2,
    3,
    2,
    3,
    2,
    3,
    2,
    3,
    1,
    1,
    1,
    1,
    1,
    1,
    2,
    3,
    2,
    3,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    0,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
  };
