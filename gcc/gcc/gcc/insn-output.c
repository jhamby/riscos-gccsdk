/* Generated automatically by the program `genoutput'
   from the machine description file `md'.  */

#include "config.h"
#include "system.h"
#include "flags.h"
#include "ggc.h"
#include "rtl.h"
#include "expr.h"
#include "insn-codes.h"
#include "tm_p.h"
#include "function.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "real.h"
#include "insn-config.h"

#include "conditions.h"
#include "insn-attr.h"

#include "recog.h"

#include "toplev.h"
#include "output.h"

static const char * const output_4[] = {
  "add%?\t%0, %1, %2",
  "sub%?\t%0, %1, #%n2",
  "#",
};

static const char *output_5 PARAMS ((rtx *, rtx));

static const char *
output_5 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

   static const char * const asms[] = 
   {
     "add\t%0, %0, %2",
     "sub\t%0, %0, #%n2",
     "add\t%0, %1, %2",
     "add\t%0, %0, %2",
     "add\t%0, %0, %2",
     "add\t%0, %1, %2",
     "add\t%0, %1, %2"
   };
   if ((which_alternative == 2 || which_alternative == 6)
       && GET_CODE (operands[2]) == CONST_INT
       && INTVAL (operands[2]) < 0)
     return "sub\t%0, %1, #%n2";
   return asms[which_alternative];
  
}

static const char * const output_6[] = {
  "add%?s\t%0, %1, %2",
  "sub%?s\t%0, %1, #%n2",
};

static const char * const output_7[] = {
  "cmn%?\t%0, %1",
  "cmp%?\t%0, #%n1",
};

static const char * const output_8[] = {
  "add%?s\t%0, %1, %2",
  "sub%?s\t%0, %1, #%n2",
};

static const char * const output_9[] = {
  "cmn%?\t%0, %1",
  "cmp%?\t%0, #%n1",
};

static const char * const output_10[] = {
  "add%?s\t%0, %1, %2",
  "sub%?s\t%0, %1, #%n2",
};

static const char * const output_11[] = {
  "add%?s\t%0, %1, %2",
  "sub%?s\t%0, %1, #%n2",
};

static const char * const output_12[] = {
  "cmn%?\t%0, %1",
  "cmp%?\t%0, #%n1",
};

static const char * const output_13[] = {
  "cmn%?\t%0, %1",
  "cmp%?\t%0, #%n1",
};

static const char * const output_19[] = {
  "add%d2\t%0, %1, #1",
  "mov%D2\t%0, %1\n\tadd%d2\t%0, %1, #1",
};

static const char * const output_20[] = {
  "adf%?s\t%0, %1, %2",
  "suf%?s\t%0, %1, #%N2",
};

static const char * const output_21[] = {
  "adf%?d\t%0, %1, %2",
  "suf%?d\t%0, %1, #%N2",
};

static const char * const output_22[] = {
  "adf%?d\t%0, %1, %2",
  "suf%?d\t%0, %1, #%N2",
};

static const char * const output_25[] = {
  "adf%?e\t%0, %1, %2",
  "suf%?e\t%0, %1, #%N2",
};

static const char * const output_34[] = {
  "rsb%?\t%0, %2, %1",
  "#",
};

static const char * const output_35[] = {
  "sub%?s\t%0, %1, %2",
  "rsb%?s\t%0, %2, %1",
};

static const char * const output_36[] = {
  "sub%d2\t%0, %1, #1",
  "mov%D2\t%0, %1\n\tsub%d2\t%0, %1, #1",
};

static const char * const output_37[] = {
  "suf%?s\t%0, %1, %2",
  "rsf%?s\t%0, %2, %1",
};

static const char * const output_38[] = {
  "suf%?d\t%0, %1, %2",
  "rsf%?d\t%0, %2, %1",
};

static const char * const output_40[] = {
  "suf%?d\t%0, %1, %2",
  "rsf%?d\t%0, %2, %1",
};

static const char * const output_42[] = {
  "suf%?e\t%0, %1, %2",
  "rsf%?e\t%0, %2, %1",
};

static const char *output_44 PARAMS ((rtx *, rtx));

static const char *
output_44 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  if (which_alternative < 2)
    return "mov\t%0, %1\n\tmul\t%0, %0, %2";
  else
    return "mul\t%0, %0, %2";
  
}

static const char * const output_65[] = {
  "fdv%?s\t%0, %1, %2",
  "frd%?s\t%0, %2, %1",
};

static const char * const output_66[] = {
  "dvf%?d\t%0, %1, %2",
  "rdf%?d\t%0, %2, %1",
};

static const char * const output_70[] = {
  "dvf%?e\t%0, %1, %2",
  "rdf%?e\t%0, %2, %1",
};

static const char * const output_80[] = {
  "and%?\t%0, %1, %2",
  "bic%?\t%0, %1, #%B2",
  "#",
};

static const char * const output_82[] = {
  "and%?s\t%0, %1, %2",
  "bic%?s\t%0, %1, #%B2",
};

static const char * const output_83[] = {
  "tst%?\t%0, %1",
  "bic%?s\t%2, %0, #%B1",
};

static const char *output_84 PARAMS ((rtx *, rtx));

static const char *
output_84 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  operands[1] = GEN_INT (((1 << INTVAL (operands[1])) - 1)
			 << INTVAL (operands[2]));
  output_asm_insn ("tst%?\t%0, %1", operands);
  return "";
  
}

static const char *output_85 PARAMS ((rtx *, rtx));

static const char *
output_85 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  operands[2] = GEN_INT (((1 << INTVAL (operands[2])) - 1)
			 << INTVAL (operands[3]));
  output_asm_insn ("ands\t%0, %1, %2", operands);
  return "movne\t%0, #1";
  
}

static const char * const output_87[] = {
  "bic%?\t%Q0, %Q1, %2",
  "#",
};

static const char * const output_95[] = {
  "orr%?\t%Q0, %Q1, %2",
  "#",
};

static const char * const output_97[] = {
  "orr%?\t%0, %1, %2",
  "#",
};

static const char * const output_102[] = {
  "eor%?\t%Q0, %Q1, %2",
  "#",
};

static const char * const output_109[] = {
  "cmp\t%1, %2\n\tmovlt\t%0, %2",
  "cmp\t%1, %2\n\tmovge\t%0, %1",
  "cmp\t%1, %2\n\tmovge\t%0, %1\n\tmovlt\t%0, %2",
};

static const char * const output_110[] = {
  "cmp\t%1, %2\n\tmovge\t%0, %2",
  "cmp\t%1, %2\n\tmovlt\t%0, %1",
  "cmp\t%1, %2\n\tmovlt\t%0, %1\n\tmovge\t%0, %2",
};

static const char * const output_111[] = {
  "cmp\t%1, %2\n\tmovcc\t%0, %2",
  "cmp\t%1, %2\n\tmovcs\t%0, %1",
  "cmp\t%1, %2\n\tmovcs\t%0, %1\n\tmovcc\t%0, %2",
};

static const char * const output_112[] = {
  "cmp\t%1, %2\n\tmovcs\t%0, %2",
  "cmp\t%1, %2\n\tmovcc\t%0, %1",
  "cmp\t%1, %2\n\tmovcc\t%0, %1\n\tmovcs\t%0, %2",
};

static const char *output_113 PARAMS ((rtx *, rtx));

static const char *
output_113 (operands, insn)
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

static const char *output_114 PARAMS ((rtx *, rtx));

static const char *
output_114 (operands, insn)
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

static const char * const output_133[] = {
  "cmp\t%0, #0\n\trsblt\t%0, %0, #0",
  "eor%?\t%0, %1, %1, asr #31\n\tsub%?\t%0, %0, %1, asr #31",
};

static const char * const output_134[] = {
  "cmp\t%0, #0\n\trsbgt\t%0, %0, #0",
  "eor%?\t%0, %1, %1, asr #31\n\trsb%?\t%0, %0, %1, asr #31",
};

static const char *output_157 PARAMS ((rtx *, rtx));

static const char *
output_157 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

    if (REGNO (operands[1])
        != REGNO (operands[0]) + (WORDS_BIG_ENDIAN ? 1 : 0))
      output_asm_insn ("mov%?\t%Q0, %1", operands);
    return "mov%?\t%R0, #0";
  
}

static const char * const output_158[] = {
  "and%?\t%Q0, %1, #255\n\tmov%?\t%R0, #0",
  "ldr%?b\t%Q0, %1\n\tmov%?\t%R0, #0",
};

static const char *output_159 PARAMS ((rtx *, rtx));

static const char *
output_159 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

    if (REGNO (operands[1])
        != REGNO (operands[0]) + (WORDS_BIG_ENDIAN ? 1 : 0))
      output_asm_insn ("mov%?\t%Q0, %1", operands);
    return "mov%?\t%R0, %Q0, asr #31";
  
}

static const char *output_160 PARAMS ((rtx *, rtx));

static const char *
output_160 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  rtx mem = XEXP (operands[1], 0);

  if (GET_CODE (mem) == CONST)
    mem = XEXP (mem, 0);
    
  if (GET_CODE (mem) == LABEL_REF)
    return "ldr\t%0, %1";
    
  if (GET_CODE (mem) == PLUS)
    {
      rtx a = XEXP (mem, 0);
      rtx b = XEXP (mem, 1);

      /* This can happen due to bugs in reload.  */
      if (GET_CODE (a) == REG && REGNO (a) == SP_REGNUM)
        {
          rtx ops[2];
          ops[0] = operands[0];
          ops[1] = a;
      
          output_asm_insn ("mov	%0, %1", ops);

          XEXP (mem, 0) = operands[0];
       }

      else if (   GET_CODE (a) == LABEL_REF
	       && GET_CODE (b) == CONST_INT)
        return "ldr\t%0, %1";
    }
    
  return "ldrh\t%0, %1";
  
}

static const char *output_165 PARAMS ((rtx *, rtx));

static const char *
output_165 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  {
    rtx ops[4];
    rtx mem = XEXP (operands[1], 0);

    /* This code used to try to use 'V', and fix the address only if it was
       offsettable, but this fails for e.g. REG+48 because 48 is outside the
       range of QImode offsets, and offsettable_address_p does a QImode
       address check.  */
       
    if (GET_CODE (mem) == CONST)
      mem = XEXP (mem, 0);
    
    if (GET_CODE (mem) == LABEL_REF)
      return "ldr\t%0, %1";
    
    if (GET_CODE (mem) == PLUS)
      {
        rtx a = XEXP (mem, 0);
        rtx b = XEXP (mem, 1);

        if (GET_CODE (a) == LABEL_REF
	    && GET_CODE (b) == CONST_INT)
          return "ldr\t%0, %1";

        if (GET_CODE (b) == REG)
          return "ldrsh\t%0, %1";
	  
        ops[1] = a;
        ops[2] = b;
      }
    else
      {
        ops[1] = mem;
        ops[2] = const0_rtx;
      }
      
    if (GET_CODE (ops[1]) != REG)
      {
        debug_rtx (ops[1]);
        abort ();
      }

    ops[0] = operands[0];
    ops[3] = operands[2];
    output_asm_insn ("mov\t%3, %2\n\tldrsh\t%0, [%1, %3]", ops);
    return "";
  }
}

static const char *output_167 PARAMS ((rtx *, rtx));

static const char *
output_167 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  /* If the address is invalid, this will split the instruction into two. */
  if (bad_signed_byte_operand (operands[1], VOIDmode))
    return "#";
  return "ldr%?sb\t%0, %1";
  
}

static const char *output_168 PARAMS ((rtx *, rtx));

static const char *
output_168 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  /* If the address is invalid, this will split the instruction into two. */
  if (bad_signed_byte_operand (operands[1], VOIDmode))
    return "#";
  return "ldr%?sb\t%0, %1";
  
}

static const char *output_169 PARAMS ((rtx *, rtx));

static const char *
output_169 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  {
    rtx ops[3];
    rtx mem = XEXP (operands[1], 0);
    
    if (GET_CODE (mem) == CONST)
      mem = XEXP (mem, 0);
    
    if (GET_CODE (mem) == LABEL_REF)
      return "ldr\t%0, %1";

    if (GET_CODE (mem) == PLUS
        && GET_CODE (XEXP (mem, 0)) == LABEL_REF)
      return "ldr\t%0, %1";
      
    if (which_alternative == 0)
      return "ldrsb\t%0, %1";
      
    ops[0] = operands[0];
    
    if (GET_CODE (mem) == PLUS)
      {
        rtx a = XEXP (mem, 0);
	rtx b = XEXP (mem, 1);
	
        ops[1] = a;
        ops[2] = b;

        if (GET_CODE (a) == REG)
	  {
	    if (GET_CODE (b) == REG)
              output_asm_insn ("ldrsb\t%0, [%1, %2]", ops);
            else if (REGNO (a) == REGNO (ops[0]))
	      {
                output_asm_insn ("ldrb\t%0, [%1, %2]", ops);
		output_asm_insn ("lsl\t%0, %0, #24", ops);
		output_asm_insn ("asr\t%0, %0, #24", ops);
	      }
	    else
              output_asm_insn ("mov\t%0, %2\n\tldrsb\t%0, [%1, %0]", ops);
	  }
        else if (GET_CODE (b) != REG)
	  abort ();
	else
          {
            if (REGNO (b) == REGNO (ops[0]))
	      {
                output_asm_insn ("ldrb\t%0, [%2, %1]", ops);
		output_asm_insn ("lsl\t%0, %0, #24", ops);
		output_asm_insn ("asr\t%0, %0, #24", ops);
	      }
	    else
              output_asm_insn ("mov\t%0, %2\n\tldrsb\t%0, [%1, %0]", ops);
          }
      }
    else if (GET_CODE (mem) == REG && REGNO (ops[0]) == REGNO (mem))
      {
        output_asm_insn ("ldrb\t%0, [%0, #0]", ops);
	output_asm_insn ("lsl\t%0, %0, #24", ops);
	output_asm_insn ("asr\t%0, %0, #24", ops);
      }
    else
      {
        ops[1] = mem;
        ops[2] = const0_rtx;
	
        output_asm_insn ("mov\t%0, %2\n\tldrsb\t%0, [%1, %0]", ops);
      }
    return "";
  }
}

static const char *output_173 PARAMS ((rtx *, rtx));

static const char *
output_173 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  return (output_move_double (operands));
  
}

static const char *output_174 PARAMS ((rtx *, rtx));

static const char *
output_174 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  {
  switch (which_alternative)
    {
    default:
    case 0:
      if (REGNO (operands[1]) == REGNO (operands[0]) + 1)
	return "add\t%0,  %1,  #0\n\tadd\t%H0, %H1, #0";
      return   "add\t%H0, %H1, #0\n\tadd\t%0,  %1,  #0";
    case 1:
      return "mov\t%Q0, %1\n\tmov\t%R0, #0";
    case 2:
      operands[1] = GEN_INT (- INTVAL (operands[1]));
      return "mov\t%Q0, %1\n\tneg\t%Q0, %Q0\n\tasr\t%R0, %Q0, #31";
    case 3:
      return "ldmia\t%1, {%0, %H0}";
    case 4:
      return "stmia\t%0, {%1, %H1}";
    case 5:
      return thumb_load_double_from_address (operands);
    case 6:
      operands[2] = gen_rtx (MEM, SImode,
			     plus_constant (XEXP (operands[0], 0), 4));
      output_asm_insn ("str\t%1, %0\n\tstr\t%H1, %2", operands);
      return "";
    case 7:
      if (REGNO (operands[1]) == REGNO (operands[0]) + 1)
	return "mov\t%0, %1\n\tmov\t%H0, %H1";
      return "mov\t%H0, %H1\n\tmov\t%0, %1";
    }
  }
}

static const char * const output_175[] = {
  "mov%?\t%0, %1",
  "mvn%?\t%0, #%B1",
  "ldr%?\t%0, %1",
  "str%?\t%1, %0",
};

static const char * const output_176[] = {
  "mov	%0, %1",
  "mov	%0, %1",
  "#",
  "#",
  "ldmia\t%1, {%0}",
  "stmia\t%0, {%1}",
  "ldr\t%0, %1",
  "str\t%1, %0",
  "mov\t%0, %1",
};

static const char *output_180 PARAMS ((rtx *, rtx));

static const char *
output_180 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

#ifdef AOF_ASSEMBLER
  operands[1] = aof_pic_entry (operands[1]);
#endif
  output_asm_insn ("ldr%?\t%0, %a1", operands);
  return "";
  
}

static const char *output_181 PARAMS ((rtx *, rtx));

static const char *
output_181 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  ASM_OUTPUT_INTERNAL_LABEL (asm_out_file, "L",
			     CODE_LABEL_NUMBER (operands[1]));
  return "add\t%0, %|pc";
  
}

static const char *output_182 PARAMS ((rtx *, rtx));

static const char *
output_182 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

    ASM_OUTPUT_INTERNAL_LABEL (asm_out_file, "L",
			       CODE_LABEL_NUMBER (operands[1]));
    return "add%?\t%0, %|pc, %0";
  
}

static const char * const output_183[] = {
  "cmp%?\t%0, #0",
  "sub%?s\t%0, %1, #0",
};

static const char *output_184 PARAMS ((rtx *, rtx));

static const char *
output_184 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  switch (which_alternative)
    {
    case 0: return "add	%0, %1, #0";
    case 2: return "strh	%1, %0";
    case 3: return "mov	%0, %1";
    case 4: return "mov	%0, %1";
    case 5: return "mov	%0, %1";
    default: abort ();
    case 1:
      /* The stack pointer can end up being taken as an index register.
          Catch this case here and deal with it.  */
      if (GET_CODE (XEXP (operands[1], 0)) == PLUS
	  && GET_CODE (XEXP (XEXP (operands[1], 0), 0)) == REG
	  && REGNO    (XEXP (XEXP (operands[1], 0), 0)) == SP_REGNUM)
        {
	  rtx ops[2];
          ops[0] = operands[0];
          ops[1] = XEXP (XEXP (operands[1], 0), 0);
      
          output_asm_insn ("mov	%0, %1", ops);

          XEXP (XEXP (operands[1], 0), 0) = operands[0];
    
	}
      return "ldrh	%0, %1";
    }
}

static const char *output_185 PARAMS ((rtx *, rtx));

static const char *
output_185 (operands, insn)
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

static const char * const output_186[] = {
  "mov%?\t%0, %1\t%@ movhi",
  "mvn%?\t%0, #%B1\t%@ movhi",
  "str%?h\t%1, %0\t%@ movhi ",
  "ldr%?h\t%0, %1\t%@ movhi",
};

static const char * const output_187[] = {
  "mov%?\t%0, %1\t%@ movhi",
  "mvn%?\t%0, #%B1\t%@ movhi",
  "ldr%?\t%0, %1\t%@ movhi",
};

static const char * const output_188[] = {
  "mov%?\t%0, %1\t%@ movhi",
  "mvn%?\t%0, #%B1\t%@ movhi",
  "ldr%?\t%0, %1\t%@ movhi_bigend\n\tmov%?\t%0, %0, asr #16",
};

static const char * const output_190[] = {
  "mov%?\t%0, %1\t%@ movhi",
  "mvn%?\t%0, #%B1\t%@ movhi",
};

static const char *output_191 PARAMS ((rtx *, rtx));

static const char *
output_191 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  abort ();
}

static const char * const output_192[] = {
  "mov%?\t%0, %1",
  "mvn%?\t%0, #%B1",
  "ldr%?b\t%0, %1",
  "str%?b\t%1, %0",
};

static const char * const output_193[] = {
  "add\t%0, %1, #0",
  "ldrb\t%0, %1",
  "strb\t%1, %0",
  "mov\t%0, %1",
  "mov\t%0, %1",
  "mov\t%0, %1",
};

static const char * const output_194[] = {
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

static const char * const output_195[] = {
  "mov%?\t%0, %1",
  "ldr%?\t%0, %1\t%@ float",
  "str%?\t%1, %0\t%@ float",
};

static const char * const output_196[] = {
  "add\t%0, %1, #0",
  "ldmia\t%1, {%0}",
  "stmia\t%0, {%1}",
  "ldr\t%0, %1",
  "str\t%1, %0",
  "mov\t%0, %1",
  "mov\t%0, %1",
};

static const char *output_197 PARAMS ((rtx *, rtx));

static const char *
output_197 (operands, insn)
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

static const char *output_198 PARAMS ((rtx *, rtx));

static const char *
output_198 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
 return output_move_double (operands);
}

static const char *output_199 PARAMS ((rtx *, rtx));

static const char *
output_199 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  switch (which_alternative)
    {
    default:
    case 0:
      if (REGNO (operands[1]) == REGNO (operands[0]) + 1)
	return "add\t%0, %1, #0\n\tadd\t%H0, %H1, #0";
      return "add\t%H0, %H1, #0\n\tadd\t%0, %1, #0";
    case 1:
      return "ldmia\t%1, {%0, %H0}";
    case 2:
      return "stmia\t%0, {%1, %H1}";
    case 3:
      return thumb_load_double_from_address (operands);
    case 4:
      operands[2] = gen_rtx (MEM, SImode,
			     plus_constant (XEXP (operands[0], 0), 4));
      output_asm_insn ("str\t%1, %0\n\tstr\t%H1, %2", operands);
      return "";
    case 5:
      if (REGNO (operands[1]) == REGNO (operands[0]) + 1)
	return "mov\t%0, %1\n\tmov\t%H0, %H1";
      return "mov\t%H0, %H1\n\tmov\t%0, %1";
    }
  
}

static const char *output_200 PARAMS ((rtx *, rtx));

static const char *
output_200 (operands, insn)
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

static const char *output_213 PARAMS ((rtx *, rtx));

static const char *
output_213 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
 return thumb_output_move_mem_multiple (3, operands);
}

static const char *output_214 PARAMS ((rtx *, rtx));

static const char *
output_214 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
 return thumb_output_move_mem_multiple (2, operands);
}

static const char *output_215 PARAMS ((rtx *, rtx));

static const char *
output_215 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  output_asm_insn ("cmp\t%1, %2", operands);
  switch (get_attr_length (insn))
    {
    case 4:  return "b%d0\t%l3";
    case 6:  return "b%D0\t.LCB%=\n\tb\t%l3\t%@long jump\n.LCB%=:";
    default: return "b%D0\t.LCB%=\n\tbl\t%l3\t%@far jump\n.LCB%=:";
    }
  
}

static const char *output_216 PARAMS ((rtx *, rtx));

static const char *
output_216 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  output_asm_insn ("cmn\t%1, %2", operands);
  switch (get_attr_length (insn))
    {
    case 4:  return "b%d0\t%l3";
    case 6:  return "b%D0\t.LCB%=\n\tb\t%l3\t%@long jump\n.LCB%=:";
    default: return "b%D0\t.LCB%=\n\tbl\t%l3\t%@far jump\n.LCB%=:";
    }
  
}

static const char * const output_217[] = {
  "cmp%?\t%0, %1",
  "cmn%?\t%0, #%n1",
};

static const char * const output_221[] = {
  "cmf%?\t%0, %1",
  "cnf%?\t%0, #%N1",
};

static const char * const output_222[] = {
  "cmf%?\t%0, %1",
  "cnf%?\t%0, #%N1",
};

static const char * const output_223[] = {
  "cmf%?\t%0, %1",
  "cnf%?\t%0, #%N1",
};

static const char * const output_225[] = {
  "cmf%?\t%0, %1",
  "cnf%?\t%0, #%N1",
};

static const char * const output_226[] = {
  "cmf%?e\t%0, %1",
  "cnf%?e\t%0, #%N1",
};

static const char * const output_227[] = {
  "cmf%?e\t%0, %1",
  "cnf%?e\t%0, #%N1",
};

static const char * const output_228[] = {
  "cmf%?e\t%0, %1",
  "cnf%?e\t%0, #%N1",
};

static const char * const output_230[] = {
  "cmf%?e\t%0, %1",
  "cnf%?e\t%0, #%N1",
};

static const char *output_232 PARAMS ((rtx *, rtx));

static const char *
output_232 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  if (arm_ccfsm_state != 0)
    abort ();

  return "bvs\t%l0;beq\t%l0";
  
}

static const char *output_233 PARAMS ((rtx *, rtx));

static const char *
output_233 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  if (arm_ccfsm_state != 0)
    abort ();

  return "bmi\t%l0;bgt\t%l0";
  
}

static const char *output_234 PARAMS ((rtx *, rtx));

static const char *
output_234 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  if (arm_ccfsm_state == 1 || arm_ccfsm_state == 2)
    {
      arm_ccfsm_state += 2;
      return "";
    }
  return "b%d1\t%l0";
  
}

static const char *output_235 PARAMS ((rtx *, rtx));

static const char *
output_235 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  if (arm_ccfsm_state != 0)
    abort ();

  return "bmi\t%l0;bgt\t%l0";
  
}

static const char *output_236 PARAMS ((rtx *, rtx));

static const char *
output_236 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  if (arm_ccfsm_state != 0)
    abort ();

  return "bvs\t%l0;beq\t%l0";
  
}

static const char *output_237 PARAMS ((rtx *, rtx));

static const char *
output_237 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  if (arm_ccfsm_state == 1 || arm_ccfsm_state == 2)
    {
      arm_ccfsm_state += 2;
      return "";
    }
  return "b%D1\t%l0";
  
}

static const char * const output_241[] = {
  "mov%D3\t%0, %2",
  "mvn%D3\t%0, #%B2",
  "mov%d3\t%0, %1",
  "mvn%d3\t%0, #%B1",
  "mov%d3\t%0, %1\n\tmov%D3\t%0, %2",
  "mov%d3\t%0, %1\n\tmvn%D3\t%0, #%B2",
  "mvn%d3\t%0, #%B1\n\tmov%D3\t%0, %2",
  "mvn%d3\t%0, #%B1\n\tmvn%D3\t%0, #%B2",
};

static const char * const output_242[] = {
  "mvf%D3s\t%0, %2",
  "mnf%D3s\t%0, #%N2",
  "mvf%d3s\t%0, %1",
  "mnf%d3s\t%0, #%N1",
  "mvf%d3s\t%0, %1\n\tmvf%D3s\t%0, %2",
  "mvf%d3s\t%0, %1\n\tmnf%D3s\t%0, #%N2",
  "mnf%d3s\t%0, #%N1\n\tmvf%D3s\t%0, %2",
  "mnf%d3s\t%0, #%N1\n\tmnf%D3s\t%0, #%N2",
};

static const char * const output_243[] = {
  "mov%D3\t%0, %2",
  "mov%d3\t%0, %1",
};

static const char * const output_244[] = {
  "mvf%D3d\t%0, %2",
  "mnf%D3d\t%0, #%N2",
  "mvf%d3d\t%0, %1",
  "mnf%d3d\t%0, #%N1",
  "mvf%d3d\t%0, %1\n\tmvf%D3d\t%0, %2",
  "mvf%d3d\t%0, %1\n\tmnf%D3d\t%0, #%N2",
  "mnf%d3d\t%0, #%N1\n\tmvf%D3d\t%0, %2",
  "mnf%d3d\t%0, #%N1\n\tmnf%D3d\t%0, #%N2",
};

static const char *output_245 PARAMS ((rtx *, rtx));

static const char *
output_245 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  {
    if (arm_ccfsm_state == 1 || arm_ccfsm_state == 2)
      {
        arm_ccfsm_state += 2;
        return "";
      }
    return "b%?\t%l0";
  }
  
}

static const char *output_246 PARAMS ((rtx *, rtx));

static const char *
output_246 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  if (get_attr_length (insn) == 2)
    return "b\t%l0";
  return "bl\t%l0\t%@ far jump";
  
}

static const char *output_247 PARAMS ((rtx *, rtx));

static const char *
output_247 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  return output_call (operands);
  
}

static const char *output_248 PARAMS ((rtx *, rtx));

static const char *
output_248 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  return output_call_mem (operands);
  
}

static const char *output_249 PARAMS ((rtx *, rtx));

static const char *
output_249 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  {
    if (TARGET_CALLER_INTERWORKING)
      return "bl\t%__interwork_call_via_%0";
    else
      return "bl\t%__call_via_%0";
  }
}

static const char *output_250 PARAMS ((rtx *, rtx));

static const char *
output_250 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  {
    if (TARGET_CALLER_INTERWORKING)
      return "bl\t%__interwork_call_via_%1";
    else
      return "bl\t%__call_via_%1";
  }
}

static const char *output_251 PARAMS ((rtx *, rtx));

static const char *
output_251 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  return output_call (&operands[1]);
  
}

static const char *output_252 PARAMS ((rtx *, rtx));

static const char *
output_252 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  return output_call_mem (&operands[1]);
  
}

static const char *output_253 PARAMS ((rtx *, rtx));

static const char *
output_253 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  {
    return NEED_PLT_RELOC ? "bl%?\t%a0(PLT)" : "bl%?\t%a0";
  }
}

static const char *output_254 PARAMS ((rtx *, rtx));

static const char *
output_254 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  {
    return NEED_PLT_RELOC ? "bl%?\t%a1(PLT)" : "bl%?\t%a1";
  }
}

static const char *output_257 PARAMS ((rtx *, rtx));

static const char *
output_257 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  return NEED_PLT_RELOC ? "b%?\t%a0(PLT)" : "b%?\t%a0";
  
}

static const char *output_258 PARAMS ((rtx *, rtx));

static const char *
output_258 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  return NEED_PLT_RELOC ? "b%?\t%a1(PLT)" : "b%?\t%a1";
  
}

static const char *output_259 PARAMS ((rtx *, rtx));

static const char *
output_259 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  {
    if (arm_ccfsm_state == 2)
      {
        arm_ccfsm_state += 2;
        return "";
      }
    return output_return_instruction (NULL, TRUE, FALSE);
  }
}

static const char *output_260 PARAMS ((rtx *, rtx));

static const char *
output_260 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  {
    if (arm_ccfsm_state == 2)
      {
        arm_ccfsm_state += 2;
        return "";
      }
    return output_return_instruction (operands[0], TRUE, FALSE);
  }
}

static const char *output_261 PARAMS ((rtx *, rtx));

static const char *
output_261 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  {
    if (arm_ccfsm_state == 2)
      {
        arm_ccfsm_state += 2;
        return "";
      }
    return output_return_instruction (operands[0], TRUE, TRUE);
  }
}

static const char *output_263 PARAMS ((rtx *, rtx));

static const char *
output_263 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

    if (flag_pic)
      return "cmp\t%0, %1\n\taddls\t%|pc, %|pc, %0, asl #2\n\tb\t%l3";
    return   "cmp\t%0, %1\n\tldrls\t%|pc, [%|pc, %0, asl #2]\n\tb\t%l3";
  
}

static const char *output_267 PARAMS ((rtx *, rtx));

static const char *
output_267 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  if (TARGET_ARM)
    return "mov%?\t%|r0, %|r0\t%@ nop";
  return  "mov\tr8, r8";
  
}

static const char *output_274 PARAMS ((rtx *, rtx));

static const char *
output_274 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  output_asm_insn ("add%?\t%0, %2, %3%S5", operands);
  operands[2] = operands[1];
  operands[1] = operands[0];
  return output_add_immediate (operands);
  
}

static const char *output_275 PARAMS ((rtx *, rtx));

static const char *
output_275 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

    output_add_immediate (operands);
    return "add%?s\t%0, %0, %3%S5";
  
}

static const char *output_276 PARAMS ((rtx *, rtx));

static const char *
output_276 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

    output_add_immediate (operands);
    return "add%?s\t%0, %0, %3%S5";
  
}

static const char *output_277 PARAMS ((rtx *, rtx));

static const char *
output_277 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  output_asm_insn ("mla%?\t%0, %2, %1, %3", operands);
  operands[2] = operands[4];
  operands[1] = operands[0];
  return output_add_immediate (operands);
  
}

static const char *output_278 PARAMS ((rtx *, rtx));

static const char *
output_278 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

    output_add_immediate (operands);
    output_asm_insn ("mla%?s\t%0, %3, %4, %0", operands);
    return "";
  
}

static const char *output_279 PARAMS ((rtx *, rtx));

static const char *
output_279 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

    output_add_immediate (operands);
    return "mla%?s\t%0, %3, %4, %0";
  
}

static const char * const output_281[] = {
  "orr%d2\t%0, %1, #1",
  "mov%D2\t%0, %1\n\torr%d2\t%0, %1, #1",
};

static const char *output_282 PARAMS ((rtx *, rtx));

static const char *
output_282 (operands, insn)
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

static const char *output_283 PARAMS ((rtx *, rtx));

static const char *
output_283 (operands, insn)
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

static const char *output_284 PARAMS ((rtx *, rtx));

static const char *
output_284 (operands, insn)
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

static const char *output_285 PARAMS ((rtx *, rtx));

static const char *
output_285 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

    output_asm_insn ("cmp\t%2, %3", operands);
    if (which_alternative != 0)
      output_asm_insn ("mov%D4\t%0, %1", operands);
    return "sub%d4\t%0, %1, #1";
  
}

static const char *output_286 PARAMS ((rtx *, rtx));

static const char *
output_286 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  {
    static const char * const opcodes[4][2] =
    {
      {"cmp\t%2, %3\n\tcmp%d5\t%0, %1",
       "cmp\t%0, %1\n\tcmp%d4\t%2, %3"},
      {"cmp\t%2, %3\n\tcmn%d5\t%0, #%n1",
       "cmn\t%0, #%n1\n\tcmp%d4\t%2, %3"},
      {"cmn\t%2, #%n3\n\tcmp%d5\t%0, %1",
       "cmp\t%0, %1\n\tcmn%d4\t%2, #%n3"},
      {"cmn\t%2, #%n3\n\tcmn%d5\t%0, #%n1",
       "cmn\t%0, #%n1\n\tcmn%d4\t%2, #%n3"}
    };
    int swap =
      comparison_dominates_p (GET_CODE (operands[5]), GET_CODE (operands[4]));

    return opcodes[which_alternative][swap];
  }
}

static const char *output_287 PARAMS ((rtx *, rtx));

static const char *
output_287 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  {
    static const char * const opcodes[4][2] =
    {
      {"cmp\t%0, %1\n\tcmp%d4\t%2, %3",
       "cmp\t%2, %3\n\tcmp%D5\t%0, %1"},
      {"cmn\t%0, #%n1\n\tcmp%d4\t%2, %3",
       "cmp\t%2, %3\n\tcmn%D5\t%0, #%n1"},
      {"cmp\t%0, %1\n\tcmn%d4\t%2, #%n3",
       "cmn\t%2, #%n3\n\tcmp%D5\t%0, %1"},
      {"cmn\t%0, #%n1\n\tcmn%d4\t%2, #%n3",
       "cmn\t%2, #%n3\n\tcmn%D5\t%0, #%n1"}
    };
    int swap =
      comparison_dominates_p (GET_CODE (operands[5]),
			      reverse_condition (GET_CODE (operands[4])));

    return opcodes[which_alternative][swap];
  }
}

static const char *output_288 PARAMS ((rtx *, rtx));

static const char *
output_288 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  {
    static const char *const opcodes[4][2] =
    {
      {"cmp\t%2, %3\n\tcmp%d5\t%0, %1",
       "cmp\t%0, %1\n\tcmp%d4\t%2, %3"},
      {"cmp\t%2, %3\n\tcmn%d5\t%0, #%n1",
       "cmn\t%0, #%n1\n\tcmp%d4\t%2, %3"},
      {"cmn\t%2, #%n3\n\tcmp%d5\t%0, %1",
       "cmp\t%0, %1\n\tcmn%d4\t%2, #%n3"},
      {"cmn\t%2, #%n3\n\tcmn%d5\t%0, #%n1",
       "cmn\t%0, #%n1\n\tcmn%d4\t%2, #%n3"}
    };
    int swap =
      comparison_dominates_p (GET_CODE (operands[5]), GET_CODE (operands[4]));

    return opcodes[which_alternative][swap];
  }
}

static const char *output_289 PARAMS ((rtx *, rtx));

static const char *
output_289 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

{
  static const char *const opcodes[4][2] =
  {
    {"cmp\t%0, %1\n\tcmp%D4\t%2, %3",
     "cmp\t%2, %3\n\tcmp%D5\t%0, %1"},
    {"cmn\t%0, #%n1\n\tcmp%D4\t%2, %3",
     "cmp\t%2, %3\n\tcmn%D5\t%0, #%n1"},
    {"cmp\t%0, %1\n\tcmn%D4\t%2, #%n3",
     "cmn\t%2, #%n3\n\tcmp%D5\t%0, %1"},
    {"cmn\t%0, #%n1\n\tcmn%D4\t%2, #%n3",
     "cmn\t%2, #%n3\n\tcmn%D5\t%0, #%n1"}
  };
  int swap =
    comparison_dominates_p (GET_CODE (operands[5]), GET_CODE (operands[4]));

  return opcodes[which_alternative][swap];
}

}

static const char *output_290 PARAMS ((rtx *, rtx));

static const char *
output_290 (operands, insn)
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

static const char *output_291 PARAMS ((rtx *, rtx));

static const char *
output_291 (operands, insn)
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

static const char * const output_293[] = {
  "add%d4\t%0, %2, %3",
  "sub%d4\t%0, %2, #%n3",
  "add%d4\t%0, %2, %3\n\tmov%D4\t%0, %1",
  "sub%d4\t%0, %2, #%n3\n\tmov%D4\t%0, %1",
};

static const char * const output_295[] = {
  "add%D4\t%0, %2, %3",
  "sub%D4\t%0, %2, #%n3",
  "add%D4\t%0, %2, %3\n\tmov%d4\t%0, %1",
  "sub%D4\t%0, %2, #%n3\n\tmov%d4\t%0, %1",
};

static const char *output_298 PARAMS ((rtx *, rtx));

static const char *
output_298 (operands, insn)
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

static const char * const output_299[] = {
  "%I5%d4\t%0, %2, %3",
  "%I5%d4\t%0, %2, %3\n\tmov%D4\t%0, %1",
};

static const char *output_300 PARAMS ((rtx *, rtx));

static const char *
output_300 (operands, insn)
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

static const char * const output_301[] = {
  "%I5%D4\t%0, %2, %3",
  "%I5%D4\t%0, %2, %3\n\tmov%d4\t%0, %1",
};

static const char * const output_303[] = {
  "mvn%D4\t%0, %2",
  "mov%d4\t%0, %1\n\tmvn%D4\t%0, %2",
  "mvn%d4\t%0, #%B1\n\tmvn%D4\t%0, %2",
};

static const char * const output_305[] = {
  "mvn%d4\t%0, %2",
  "mov%D4\t%0, %1\n\tmvn%d4\t%0, %2",
  "mvn%D4\t%0, #%B1\n\tmvn%d4\t%0, %2",
};

static const char * const output_307[] = {
  "mov%d5\t%0, %2%S4",
  "mov%D5\t%0, %1\n\tmov%d5\t%0, %2%S4",
  "mvn%D5\t%0, #%B1\n\tmov%d5\t%0, %2%S4",
};

static const char * const output_309[] = {
  "mov%D5\t%0, %2%S4",
  "mov%d5\t%0, %1\n\tmov%D5\t%0, %2%S4",
  "mvn%d5\t%0, #%B1\n\tmov%D5\t%0, %2%S4",
};

static const char * const output_317[] = {
  "rsb%d4\t%0, %2, #0",
  "mov%D4\t%0, %1\n\trsb%d4\t%0, %2, #0",
  "mvn%D4\t%0, #%B1\n\trsb%d4\t%0, %2, #0",
};

static const char * const output_319[] = {
  "rsb%D4\t%0, %2, #0",
  "mov%d4\t%0, %1\n\trsb%D4\t%0, %2, #0",
  "mvn%d4\t%0, #%B1\n\trsb%D4\t%0, %2, #0",
};

static const char *output_320 PARAMS ((rtx *, rtx));

static const char *
output_320 (operands, insn)
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

static const char *output_343 PARAMS ((rtx *, rtx));

static const char *
output_343 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  output_asm_insn ("%@ Sibcall epilogue", operands);
  if (USE_RETURN_INSN (FALSE))
    return output_return_instruction (NULL, FALSE, FALSE);
  return arm_output_epilogue (FALSE);
  
}

static const char *output_344 PARAMS ((rtx *, rtx));

static const char *
output_344 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  if (TARGET_ARM)
    return arm_output_epilogue (TRUE);
  else /* TARGET_THUMB */
    return thumb_unexpanded_epilogue ();
  
}

static const char * const output_345[] = {
  "mvn%D4\t%0, %2",
  "mov%d4\t%0, %1\n\tmvn%D4\t%0, %2",
};

static const char *output_346 PARAMS ((rtx *, rtx));

static const char *
output_346 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

    operands[2] = GEN_INT (1 << INTVAL (operands[2]));
    output_asm_insn ("ands\t%0, %1, %2", operands);
    return "mvnne\t%0, #0";
  
}

static const char *output_347 PARAMS ((rtx *, rtx));

static const char *
output_347 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

    operands[2] = GEN_INT (1 << INTVAL (operands[2]));
    output_asm_insn ("tst\t%1, %2", operands);
    output_asm_insn ("mvneq\t%0, #0", operands);
    return "movne\t%0, #0";
  
}

static const char *output_348 PARAMS ((rtx *, rtx));

static const char *
output_348 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  {
    int num_saves = XVECLEN (operands[2], 0);
     
    /* For the StrongARM at least it is faster to
       use STR to store only a single register.  */
    if (num_saves == 1)
      output_asm_insn ("str\t%1, [%m0, #-4]!", operands);
    else
      {
	int i;
	char pattern[100];

	strcpy (pattern, "stmfd\t%m0!, {%1");

	for (i = 1; i < num_saves; i++)
	  {
	    strcat (pattern, ", %|");
	    strcat (pattern,
		    reg_names[REGNO (XEXP (XVECEXP (operands[2], 0, i), 0))]);
	  }

	strcat (pattern, "}");
	output_asm_insn (pattern, operands);
      }

    return "";
  }
}

static const char *output_349 PARAMS ((rtx *, rtx));

static const char *
output_349 (operands, insn)
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

static const char *output_350 PARAMS ((rtx *, rtx));

static const char *
output_350 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  assemble_align (32);
  return "";
  
}

static const char *output_351 PARAMS ((rtx *, rtx));

static const char *
output_351 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  making_const_table = FALSE;
  return "";
  
}

static const char *output_352 PARAMS ((rtx *, rtx));

static const char *
output_352 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  making_const_table = TRUE;
  assemble_integer (operands[0], 1, BITS_PER_WORD, 1);
  assemble_zeros (3);
  return "";
  
}

static const char *output_353 PARAMS ((rtx *, rtx));

static const char *
output_353 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  making_const_table = TRUE;
  assemble_integer (operands[0], 2, BITS_PER_WORD, 1);
  assemble_zeros (2);
  return "";
  
}

static const char *output_354 PARAMS ((rtx *, rtx));

static const char *
output_354 (operands, insn)
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
        memcpy (&u, &CONST_DOUBLE_LOW (operands[0]), sizeof u);
        assemble_real (u.d, GET_MODE (operands[0]), BITS_PER_WORD);
        break;
      }
      default:
        assemble_integer (operands[0], 4, BITS_PER_WORD, 1);
        break;
      }
    return "";
  }
}

static const char *output_355 PARAMS ((rtx *, rtx));

static const char *
output_355 (operands, insn)
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
          memcpy (&u, &CONST_DOUBLE_LOW (operands[0]), sizeof u);
          assemble_real (u.d, GET_MODE (operands[0]), BITS_PER_WORD);
          break;
        }
      default:
        assemble_integer (operands[0], 8, BITS_PER_WORD, 1);
        break;
      }
    return "";
  }
}

static const char *output_494 PARAMS ((rtx *, rtx));

static const char *
output_494 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  return emit_ldm_seq (operands, 4);
  
}

static const char *output_495 PARAMS ((rtx *, rtx));

static const char *
output_495 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  return emit_ldm_seq (operands, 3);
  
}

static const char *output_496 PARAMS ((rtx *, rtx));

static const char *
output_496 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  return emit_ldm_seq (operands, 2);
  
}

static const char *output_497 PARAMS ((rtx *, rtx));

static const char *
output_497 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  return emit_stm_seq (operands, 4);
  
}

static const char *output_498 PARAMS ((rtx *, rtx));

static const char *
output_498 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  return emit_stm_seq (operands, 3);
  
}

static const char *output_499 PARAMS ((rtx *, rtx));

static const char *
output_499 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  return emit_stm_seq (operands, 2);
  
}

static const char * const output_510[] = {
  "add%?\t%0, %1, %2",
  "sub%?\t%0, %1, #%n2",
  "#",
};

static const char * const output_511[] = {
  "adf%?s\t%0, %1, %2",
  "suf%?s\t%0, %1, #%N2",
};

static const char * const output_512[] = {
  "adf%?d\t%0, %1, %2",
  "suf%?d\t%0, %1, #%N2",
};

static const char * const output_513[] = {
  "adf%?d\t%0, %1, %2",
  "suf%?d\t%0, %1, #%N2",
};

static const char * const output_516[] = {
  "adf%?e\t%0, %1, %2",
  "suf%?e\t%0, %1, #%N2",
};

static const char * const output_517[] = {
  "rsb%?\t%0, %2, %1",
  "#",
};

static const char * const output_518[] = {
  "suf%?d\t%0, %1, %2",
  "rsf%?d\t%0, %2, %1",
};

static const char * const output_520[] = {
  "suf%?d\t%0, %1, %2",
  "rsf%?d\t%0, %2, %1",
};

static const char * const output_522[] = {
  "suf%?e\t%0, %1, %2",
  "rsf%?e\t%0, %2, %1",
};

static const char * const output_537[] = {
  "fdv%?s\t%0, %1, %2",
  "frd%?s\t%0, %2, %1",
};

static const char * const output_538[] = {
  "dvf%?d\t%0, %1, %2",
  "rdf%?d\t%0, %2, %1",
};

static const char * const output_542[] = {
  "dvf%?e\t%0, %1, %2",
  "rdf%?e\t%0, %2, %1",
};

static const char * const output_549[] = {
  "and%?\t%0, %1, %2",
  "bic%?\t%0, %1, #%B2",
  "#",
};

static const char * const output_551[] = {
  "bic%?\t%Q0, %Q1, %2",
  "#",
};

static const char * const output_556[] = {
  "orr%?\t%Q0, %Q1, %2",
  "#",
};

static const char * const output_558[] = {
  "orr%?\t%0, %1, %2",
  "#",
};

static const char * const output_560[] = {
  "eor%?\t%Q0, %Q1, %2",
  "#",
};

static const char *output_590 PARAMS ((rtx *, rtx));

static const char *
output_590 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

    if (REGNO (operands[1])
        != REGNO (operands[0]) + (WORDS_BIG_ENDIAN ? 1 : 0))
      output_asm_insn ("mov%?\t%Q0, %1", operands);
    return "mov%?\t%R0, #0";
  
}

static const char * const output_591[] = {
  "and%?\t%Q0, %1, #255\n\tmov%?\t%R0, #0",
  "ldr%?b\t%Q0, %1\n\tmov%?\t%R0, #0",
};

static const char *output_592 PARAMS ((rtx *, rtx));

static const char *
output_592 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

    if (REGNO (operands[1])
        != REGNO (operands[0]) + (WORDS_BIG_ENDIAN ? 1 : 0))
      output_asm_insn ("mov%?\t%Q0, %1", operands);
    return "mov%?\t%R0, %Q0, asr #31";
  
}

static const char *output_596 PARAMS ((rtx *, rtx));

static const char *
output_596 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  /* If the address is invalid, this will split the instruction into two. */
  if (bad_signed_byte_operand (operands[1], VOIDmode))
    return "#";
  return "ldr%?sb\t%0, %1";
  
}

static const char *output_597 PARAMS ((rtx *, rtx));

static const char *
output_597 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  /* If the address is invalid, this will split the instruction into two. */
  if (bad_signed_byte_operand (operands[1], VOIDmode))
    return "#";
  return "ldr%?sb\t%0, %1";
  
}

static const char * const output_601[] = {
  "mov%?\t%0, %1",
  "mvn%?\t%0, #%B1",
  "ldr%?\t%0, %1",
  "str%?\t%1, %0",
};

static const char *output_603 PARAMS ((rtx *, rtx));

static const char *
output_603 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

    ASM_OUTPUT_INTERNAL_LABEL (asm_out_file, "L",
			       CODE_LABEL_NUMBER (operands[1]));
    return "add%?\t%0, %|pc, %0";
  
}

static const char *output_604 PARAMS ((rtx *, rtx));

static const char *
output_604 (operands, insn)
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

static const char * const output_605[] = {
  "mov%?\t%0, %1\t%@ movhi",
  "mvn%?\t%0, #%B1\t%@ movhi",
  "str%?h\t%1, %0\t%@ movhi ",
  "ldr%?h\t%0, %1\t%@ movhi",
};

static const char * const output_606[] = {
  "mov%?\t%0, %1\t%@ movhi",
  "mvn%?\t%0, #%B1\t%@ movhi",
  "ldr%?\t%0, %1\t%@ movhi",
};

static const char * const output_607[] = {
  "mov%?\t%0, %1\t%@ movhi",
  "mvn%?\t%0, #%B1\t%@ movhi",
  "ldr%?\t%0, %1\t%@ movhi_bigend\n\tmov%?\t%0, %0, asr #16",
};

static const char * const output_609[] = {
  "mov%?\t%0, %1\t%@ movhi",
  "mvn%?\t%0, #%B1\t%@ movhi",
};

static const char * const output_610[] = {
  "mov%?\t%0, %1",
  "mvn%?\t%0, #%B1",
  "ldr%?b\t%0, %1",
  "str%?b\t%1, %0",
};

static const char * const output_611[] = {
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

static const char * const output_612[] = {
  "mov%?\t%0, %1",
  "ldr%?\t%0, %1\t%@ float",
  "str%?\t%1, %0\t%@ float",
};

static const char *output_613 PARAMS ((rtx *, rtx));

static const char *
output_613 (operands, insn)
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

static const char *output_614 PARAMS ((rtx *, rtx));

static const char *
output_614 (operands, insn)
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

static const char *output_627 PARAMS ((rtx *, rtx));

static const char *
output_627 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  {
    if (arm_ccfsm_state == 1 || arm_ccfsm_state == 2)
      {
        arm_ccfsm_state += 2;
        return "";
      }
    return "b%?\t%l0";
  }
  
}

static const char *output_628 PARAMS ((rtx *, rtx));

static const char *
output_628 (operands, insn)
     rtx *operands ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{

  {
    if (arm_ccfsm_state == 2)
      {
        arm_ccfsm_state += 2;
        return "";
      }
    return output_return_instruction (NULL, TRUE, FALSE);
  }
}

static const char *output_633 PARAMS ((rtx *, rtx));

static const char *
output_633 (operands, insn)
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


extern int register_operand PARAMS ((rtx, enum machine_mode));
extern int s_register_operand PARAMS ((rtx, enum machine_mode));
extern int reg_or_int_operand PARAMS ((rtx, enum machine_mode));
extern int nonmemory_operand PARAMS ((rtx, enum machine_mode));
extern int arm_add_operand PARAMS ((rtx, enum machine_mode));
extern int arm_rhs_operand PARAMS ((rtx, enum machine_mode));
extern int shift_operator PARAMS ((rtx, enum machine_mode));
extern int arm_comparison_operator PARAMS ((rtx, enum machine_mode));
extern int cc_register PARAMS ((rtx, enum machine_mode));
extern int fpu_add_operand PARAMS ((rtx, enum machine_mode));
extern int fpu_rhs_operand PARAMS ((rtx, enum machine_mode));
extern int scratch_operand PARAMS ((rtx, enum machine_mode));
extern int arm_not_operand PARAMS ((rtx, enum machine_mode));
extern int const_int_operand PARAMS ((rtx, enum machine_mode));
extern int memory_operand PARAMS ((rtx, enum machine_mode));
extern int minmax_operator PARAMS ((rtx, enum machine_mode));
extern int shiftable_operator PARAMS ((rtx, enum machine_mode));
extern int nonimmediate_operand PARAMS ((rtx, enum machine_mode));
extern int nonimmediate_di_operand PARAMS ((rtx, enum machine_mode));
extern int di_operand PARAMS ((rtx, enum machine_mode));
extern int general_operand PARAMS ((rtx, enum machine_mode));
extern int address_operand PARAMS ((rtx, enum machine_mode));
extern int offsettable_memory_operand PARAMS ((rtx, enum machine_mode));
extern int nonimmediate_soft_df_operand PARAMS ((rtx, enum machine_mode));
extern int soft_df_operand PARAMS ((rtx, enum machine_mode));
extern int load_multiple_operation PARAMS ((rtx, enum machine_mode));
extern int arm_hard_register_operand PARAMS ((rtx, enum machine_mode));
extern int store_multiple_operation PARAMS ((rtx, enum machine_mode));
extern int equality_operator PARAMS ((rtx, enum machine_mode));
extern int dominant_cc_register PARAMS ((rtx, enum machine_mode));
extern int index_operand PARAMS ((rtx, enum machine_mode));
extern int const_shift_operand PARAMS ((rtx, enum machine_mode));
extern int multi_register_push PARAMS ((rtx, enum machine_mode));
extern int f_register_operand PARAMS ((rtx, enum machine_mode));
extern int logical_binary_operator PARAMS ((rtx, enum machine_mode));
extern int alignable_memory_operand PARAMS ((rtx, enum machine_mode));
extern int bad_signed_byte_operand PARAMS ((rtx, enum machine_mode));
extern int arm_reload_memory_operand PARAMS ((rtx, enum machine_mode));
extern int immediate_operand PARAMS ((rtx, enum machine_mode));



static const struct insn_operand_data operand_data[] = 
{
  {
    0,
    "",
    VOIDmode,
    0,
    0
  },
  {
    register_operand,
    "=l",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "%0",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "l",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "=&r,&r",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "%0,0",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,0",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "=&r,&r",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,0",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r,r,r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "%r,r,r",
    SImode,
    0,
    1
  },
  {
    reg_or_int_operand,
    "rI,L,?n",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=l,l,l,*r,*h,l,!k",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "%0,0,l,*0,*0,!k,!k",
    SImode,
    0,
    1
  },
  {
    nonmemory_operand,
    "I,J,lL,*h,*r,!M,!O",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r,r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    SImode,
    0,
    1
  },
  {
    arm_add_operand,
    "rI,L",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rI",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    reg_or_int_operand,
    "",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r,r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "0,?r",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    SImode,
    0,
    0
  },
  {
    cc_register,
    "",
    CCmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f,f",
    SFmode,
    0,
    1
  },
  {
    s_register_operand,
    "%f,f",
    SFmode,
    0,
    1
  },
  {
    fpu_add_operand,
    "fG,H",
    SFmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f,f",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "%f,f",
    DFmode,
    0,
    1
  },
  {
    fpu_add_operand,
    "fG,H",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f,f",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f,f",
    SFmode,
    0,
    1
  },
  {
    fpu_add_operand,
    "fG,H",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f,f",
    XFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f,f",
    XFmode,
    0,
    1
  },
  {
    fpu_add_operand,
    "fG,H",
    XFmode,
    0,
    1
  },
  {
    s_register_operand,
    "=&r,&r,&r",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "0,r,0",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,0,0",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=l",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "0",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "l",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "=&r,&r",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "?r,0",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=l",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "l",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "l",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r,r",
    SImode,
    0,
    1
  },
  {
    reg_or_int_operand,
    "rI,?n",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r,r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "r,I",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rI,r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r,r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "0,?r",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    SImode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f,f",
    SFmode,
    0,
    1
  },
  {
    fpu_rhs_operand,
    "f,G",
    SFmode,
    0,
    1
  },
  {
    fpu_rhs_operand,
    "fG,f",
    SFmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f,f",
    DFmode,
    0,
    1
  },
  {
    fpu_rhs_operand,
    "f,G",
    DFmode,
    0,
    1
  },
  {
    fpu_rhs_operand,
    "fG,f",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    fpu_rhs_operand,
    "fG",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f,f",
    DFmode,
    0,
    1
  },
  {
    fpu_rhs_operand,
    "f,G",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f,f",
    SFmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f,f",
    XFmode,
    0,
    1
  },
  {
    fpu_rhs_operand,
    "f,G",
    XFmode,
    0,
    1
  },
  {
    fpu_rhs_operand,
    "fG,f",
    XFmode,
    0,
    1
  },
  {
    s_register_operand,
    "=&r,&r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "%?r,0",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=&l,&l,&l",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "%l,*h,0",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "l,l,l",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "=&r,&r",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "%?r,0",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=&r,&r,&r,&r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "%r,0,r,0",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r,r,r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "?r,r,0,0",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "=&r,&r,&r,&r",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "%r,0,r,0",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r,r,r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "?r,r,0,0",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=&r",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "0",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "%r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=&r",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "%r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=&r,&r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "%r,0",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "=&r,&r",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "%r",
    HImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    HImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "%r",
    HImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    HImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "0",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "%r",
    HImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    HImode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    SFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    fpu_rhs_operand,
    "fG",
    SFmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    DFmode,
    0,
    1
  },
  {
    fpu_rhs_operand,
    "fG",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    XFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    XFmode,
    0,
    1
  },
  {
    fpu_rhs_operand,
    "fG",
    XFmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    DFmode,
    0,
    1
  },
  {
    fpu_rhs_operand,
    "fG",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    s_register_operand,
    "=&r,&r",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "%0,r",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r,r,r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r,r",
    SImode,
    0,
    1
  },
  {
    reg_or_int_operand,
    "rI,K,?n",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=l",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "%0",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "l",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r,r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    SImode,
    0,
    1
  },
  {
    arm_not_operand,
    "rI,K",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "=X,r",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    const_int_operand,
    "n",
    VOIDmode,
    0,
    1
  },
  {
    const_int_operand,
    "n",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    const_int_operand,
    "n",
    SImode,
    0,
    1
  },
  {
    const_int_operand,
    "n",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=&r,&r",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,0",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "0,r",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "=&r,&r",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "0,?r",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=l",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "l",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "0",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rM",
    SImode,
    0,
    1
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    scratch_operand,
    "=r",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r,r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    SImode,
    0,
    1
  },
  {
    reg_or_int_operand,
    "rI,?n",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "%r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rI",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "=r",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "%r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rI",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=&r,&r,&r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r,0",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rI,0,rI",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rI,rI,rI",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r,r,r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "0,r,?r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rI,0,rI",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "=m",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    minmax_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "=r,r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "0,?r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rI,rI",
    SImode,
    0,
    1
  },
  {
    shiftable_operator,
    "",
    SImode,
    0,
    0
  },
  {
    minmax_operator,
    "",
    SImode,
    0,
    0
  },
  {
    register_operand,
    "=l,l",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "l,0",
    SImode,
    0,
    1
  },
  {
    nonmemory_operand,
    "N,l",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=l",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "0",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "l",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    reg_or_int_operand,
    "rM",
    SImode,
    0,
    1
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rM",
    SImode,
    0,
    1
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    scratch_operand,
    "=r",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rM",
    SImode,
    0,
    1
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "=&r,r",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "?r,0",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "=&l",
    DImode,
    0,
    1
  },
  {
    register_operand,
    "l",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r,&r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "0,r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    SFmode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    XFmode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    XFmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    SFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    SFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    XFmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    XFmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r,r",
    DImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "r,m",
    QImode,
    0,
    1
  },
  {
    register_operand,
    "=l",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    HImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    HImode,
    0,
    1
  },
  {
    register_operand,
    "=l",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    QImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    QImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    QImode,
    0,
    1
  },
  {
    register_operand,
    "=l",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    HImode,
    0,
    1
  },
  {
    scratch_operand,
    "=&l",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "=r",
    HImode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    QImode,
    0,
    1
  },
  {
    register_operand,
    "=l,l",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "V,m",
    QImode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    XFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    XFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    DFmode,
    0,
    1
  },
  {
    nonimmediate_di_operand,
    "=r,r,o<>",
    DImode,
    0,
    1
  },
  {
    di_operand,
    "rIK,mi,r",
    DImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=l,l,l,l,>,l,m,*r",
    DImode,
    0,
    1
  },
  {
    general_operand,
    "l,I,J,>,l,mi,l,*r",
    DImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=r,r,r,m",
    SImode,
    0,
    1
  },
  {
    general_operand,
    "rI,K,mi,r",
    SImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=l,l,l,l,l,>,l,m,*lh",
    SImode,
    0,
    1
  },
  {
    general_operand,
    "l,I,J,K,>,l,mi,l,*lh",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    address_operand,
    "p",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    0,
    "mX",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=l",
    SImode,
    0,
    1
  },
  {
    0,
    "mX",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "+r",
    SImode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r,r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "0,r",
    SImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=l,l,m,*r,*h,l",
    HImode,
    0,
    1
  },
  {
    general_operand,
    "l,mn,l,*h,*r,I",
    HImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    offsettable_memory_operand,
    "o",
    SImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=r,r,m,r",
    HImode,
    0,
    1
  },
  {
    general_operand,
    "rI,K,r,m",
    HImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r,r,r",
    HImode,
    0,
    1
  },
  {
    general_operand,
    "rI,K,m",
    HImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r,r",
    HImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rI,K",
    HImode,
    0,
    1
  },
  {
    memory_operand,
    "=m",
    HImode,
    0,
    1
  },
  {
    register_operand,
    "l",
    HImode,
    0,
    1
  },
  {
    register_operand,
    "=&l",
    SImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=r,r,r,m",
    QImode,
    0,
    1
  },
  {
    general_operand,
    "rI,K,m,r",
    QImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=l,l,m,*r,*h,l",
    QImode,
    0,
    1
  },
  {
    general_operand,
    "l,m,l,*h,*r,I",
    QImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=f,f,f,m,f,r,r,r,m",
    SFmode,
    0,
    1
  },
  {
    general_operand,
    "fG,H,mE,f,r,f,r,mE,r",
    SFmode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=r,r,m",
    SFmode,
    0,
    1
  },
  {
    general_operand,
    "r,mE,r",
    SFmode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=l,l,>,l,m,*r,*h",
    SFmode,
    0,
    1
  },
  {
    general_operand,
    "l,>,l,mF,l,*h,*r",
    SFmode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=r,Q,r,m,r,f,f,f,m,!f,!r",
    DFmode,
    0,
    1
  },
  {
    general_operand,
    "Q,r,r,r,mF,fG,H,mF,f,r,f",
    DFmode,
    0,
    1
  },
  {
    nonimmediate_soft_df_operand,
    "=r,r,m",
    DFmode,
    0,
    1
  },
  {
    soft_df_operand,
    "r,mF,r",
    DFmode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=l,l,>,l,m,*r",
    DFmode,
    0,
    1
  },
  {
    general_operand,
    "l,>,l,mF,l,*r",
    DFmode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=f,f,f,m,f,r,r",
    XFmode,
    0,
    1
  },
  {
    general_operand,
    "fG,H,m,f,r,f,r",
    XFmode,
    0,
    1
  },
  {
    load_multiple_operation,
    "",
    VOIDmode,
    0,
    0
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "1",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    load_multiple_operation,
    "",
    VOIDmode,
    0,
    0
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    store_multiple_operation,
    "",
    VOIDmode,
    0,
    0
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "1",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    store_multiple_operation,
    "",
    VOIDmode,
    0,
    0
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=l",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "=l",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "0",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "1",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "=&l",
    SImode,
    0,
    0
  },
  {
    scratch_operand,
    "=&l",
    SImode,
    0,
    0
  },
  {
    scratch_operand,
    "=&l",
    SImode,
    0,
    0
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    register_operand,
    "l,r",
    SImode,
    0,
    1
  },
  {
    nonmemory_operand,
    "rI,r",
    SImode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    register_operand,
    "l",
    SImode,
    0,
    1
  },
  {
    nonmemory_operand,
    "l",
    SImode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    reg_or_int_operand,
    "rM",
    SImode,
    0,
    1
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "f,f",
    SFmode,
    0,
    1
  },
  {
    fpu_add_operand,
    "fG,H",
    SFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f,f",
    DFmode,
    0,
    1
  },
  {
    fpu_add_operand,
    "fG,H",
    DFmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    SImode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r,r,r,r,r,r,r,r",
    SImode,
    0,
    1
  },
  {
    arm_not_operand,
    "0,0,rI,K,rI,rI,K,K",
    SImode,
    0,
    1
  },
  {
    arm_not_operand,
    "rI,K,0,0,rI,K,rI,K",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f,f,f,f,f,f,f,f",
    SFmode,
    0,
    1
  },
  {
    fpu_add_operand,
    "0,0,fG,H,fG,fG,H,H",
    SFmode,
    0,
    1
  },
  {
    fpu_add_operand,
    "fG,H,0,0,fG,H,fG,H",
    SFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r,r",
    SFmode,
    0,
    1
  },
  {
    s_register_operand,
    "0,r",
    SFmode,
    0,
    1
  },
  {
    s_register_operand,
    "r,0",
    SFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f,f,f,f,f,f,f,f",
    DFmode,
    0,
    1
  },
  {
    fpu_add_operand,
    "0,0,fG,H,fG,fG,H,H",
    DFmode,
    0,
    1
  },
  {
    fpu_add_operand,
    "fG,H,0,0,fG,H,fG,H",
    DFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    SImode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "l*r",
    SImode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "=l",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "l*r",
    SImode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "=r,f",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    SImode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "=r,f",
    VOIDmode,
    0,
    1
  },
  {
    memory_operand,
    "m,m",
    SImode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "X",
    SImode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r,f",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "X,X",
    SImode,
    0,
    1
  },
  {
    0,
    "",
    SImode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "X",
    SImode,
    0,
    1
  },
  {
    0,
    "",
    SImode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "=l",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "X",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r,f",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "X,X",
    SImode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rI",
    SImode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    shiftable_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    reg_or_int_operand,
    "rI",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "=r",
    SImode,
    0,
    0
  },
  {
    shiftable_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    reg_or_int_operand,
    "rI",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    reg_or_int_operand,
    "rM",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "=r",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    reg_or_int_operand,
    "rM",
    SImode,
    0,
    1
  },
  {
    0,
    "=&r",
    SImode,
    0,
    1
  },
  {
    const_int_operand,
    "n",
    SImode,
    0,
    1
  },
  {
    0,
    "r",
    SImode,
    0,
    1
  },
  {
    0,
    "r",
    SImode,
    0,
    1
  },
  {
    0,
    "rM",
    SImode,
    0,
    1
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    0,
    "=&r",
    SImode,
    0,
    1
  },
  {
    0,
    "r",
    SImode,
    0,
    1
  },
  {
    const_int_operand,
    "n",
    SImode,
    0,
    1
  },
  {
    0,
    "r",
    SImode,
    0,
    1
  },
  {
    0,
    "rM",
    SImode,
    0,
    1
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    scratch_operand,
    "=&r",
    SImode,
    0,
    0
  },
  {
    0,
    "r",
    SImode,
    0,
    1
  },
  {
    const_int_operand,
    "n",
    SImode,
    0,
    1
  },
  {
    0,
    "r",
    SImode,
    0,
    1
  },
  {
    0,
    "rM",
    SImode,
    0,
    1
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    0,
    "=&r,&r",
    SImode,
    0,
    1
  },
  {
    0,
    "%0,r",
    SImode,
    0,
    1
  },
  {
    0,
    "r,r",
    SImode,
    0,
    1
  },
  {
    0,
    "r,r",
    SImode,
    0,
    1
  },
  {
    const_int_operand,
    "n,n",
    SImode,
    0,
    1
  },
  {
    0,
    "=&r",
    SImode,
    0,
    1
  },
  {
    0,
    "r",
    SImode,
    0,
    1
  },
  {
    const_int_operand,
    "n",
    SImode,
    0,
    1
  },
  {
    0,
    "r",
    SImode,
    0,
    1
  },
  {
    0,
    "r",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "=&r",
    SImode,
    0,
    0
  },
  {
    0,
    "r",
    SImode,
    0,
    1
  },
  {
    const_int_operand,
    "n",
    SImode,
    0,
    1
  },
  {
    0,
    "r",
    SImode,
    0,
    1
  },
  {
    0,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r,r",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "r,r",
    SImode,
    0,
    1
  },
  {
    arm_add_operand,
    "rI,L",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r,r,r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "0,rI,?rI",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rI,0,rI",
    SImode,
    0,
    1
  },
  {
    equality_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r,r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "0,?r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rI,rI",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    SImode,
    0,
    0
  },
  {
    shiftable_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "r,r,r,r",
    SImode,
    0,
    1
  },
  {
    arm_add_operand,
    "rI,L,rI,L",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r,r,r",
    SImode,
    0,
    1
  },
  {
    arm_add_operand,
    "rI,rI,L,L",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    arm_comparison_operator,
    "",
    SImode,
    0,
    0
  },
  {
    dominant_cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rI",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    s_register_operand,
    "=r,r,r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "0,rI,?rI",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rI,0,rI",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r,r",
    SImode,
    0,
    1
  },
  {
    arm_add_operand,
    "rIL,rIL,rIL",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    s_register_operand,
    "=r,r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "0,?rI",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    SImode,
    0,
    1
  },
  {
    arm_add_operand,
    "rIL,rIL",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    SImode,
    0,
    1
  },
  {
    arm_add_operand,
    "rIL,rIL",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    s_register_operand,
    "=r,r,r,r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "0,0,?rI,?rI",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r,r,r",
    SImode,
    0,
    1
  },
  {
    arm_add_operand,
    "rI,L,rI,L",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rI",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rI",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_add_operand,
    "rIL",
    SImode,
    0,
    1
  },
  {
    shiftable_operator,
    "",
    SImode,
    0,
    0
  },
  {
    shiftable_operator,
    "",
    SImode,
    0,
    0
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rI",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rI",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    shiftable_operator,
    "",
    SImode,
    0,
    0
  },
  {
    shiftable_operator,
    "",
    SImode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r,r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "0,?rI",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    SImode,
    0,
    1
  },
  {
    arm_add_operand,
    "rIL,rIL",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rI,rI",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    shiftable_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "=r,r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "0,?rI",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rI,rI",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    shiftable_operator,
    "",
    SImode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r,r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "0,?rI",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rI,rI",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    SImode,
    0,
    1
  },
  {
    arm_add_operand,
    "rIL,rIL",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    shiftable_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "=r,r",
    SImode,
    0,
    1
  },
  {
    arm_not_operand,
    "0,?rIK",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    SImode,
    0,
    1
  },
  {
    arm_add_operand,
    "rIL,rIL",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    s_register_operand,
    "=r,r,r",
    SImode,
    0,
    1
  },
  {
    arm_not_operand,
    "0,?rI,K",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r,r",
    SImode,
    0,
    1
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    s_register_operand,
    "=r,r",
    SImode,
    0,
    1
  },
  {
    arm_not_operand,
    "0,?rIK",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rM,rM",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    SImode,
    0,
    1
  },
  {
    arm_add_operand,
    "rIL,rIL",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "=r,r,r",
    SImode,
    0,
    1
  },
  {
    arm_not_operand,
    "0,?rI,K",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r,r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rM,rM,rM",
    SImode,
    0,
    1
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rM",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rM",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_add_operand,
    "rIL",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rM",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rM",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rI",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_add_operand,
    "rIL",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    shiftable_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rI",
    SImode,
    0,
    1
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    shiftable_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    shiftable_operator,
    "",
    SImode,
    0,
    0
  },
  {
    memory_operand,
    "m",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "=r",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "%0",
    SImode,
    0,
    1
  },
  {
    index_operand,
    "rJ",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    QImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "0",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    QImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "%0",
    SImode,
    0,
    1
  },
  {
    index_operand,
    "rJ",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    QImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "0",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    QImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "%0",
    SImode,
    0,
    1
  },
  {
    index_operand,
    "rJ",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "0",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "%0",
    SImode,
    0,
    1
  },
  {
    index_operand,
    "rJ",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "0",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "%0",
    SImode,
    0,
    1
  },
  {
    index_operand,
    "rJ",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    HImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "0",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    HImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "0",
    SImode,
    0,
    1
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    const_shift_operand,
    "n",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    QImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "0",
    SImode,
    0,
    1
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    const_shift_operand,
    "n",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    QImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "0",
    SImode,
    0,
    1
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    const_shift_operand,
    "n",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "0",
    SImode,
    0,
    1
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    const_shift_operand,
    "n",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "0",
    SImode,
    0,
    1
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    const_shift_operand,
    "n",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    HImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r,r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "0,?rI",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    SImode,
    0,
    1
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    memory_operand,
    "=m",
    BLKmode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    multi_register_push,
    "",
    VOIDmode,
    0,
    0
  },
  {
    memory_operand,
    "=m",
    BLKmode,
    0,
    1
  },
  {
    f_register_operand,
    "f",
    XFmode,
    0,
    1
  },
  {
    multi_register_push,
    "",
    VOIDmode,
    0,
    0
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    reg_or_int_operand,
    "",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    const_int_operand,
    "",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "r",
    SImode,
    0,
    0
  },
  {
    register_operand,
    "=l",
    SImode,
    0,
    1
  },
  {
    const_int_operand,
    "M",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "k",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    const_int_operand,
    "",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "r",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    0
  },
  {
    0,
    "",
    VOIDmode,
    0,
    0
  },
  {
    0,
    "",
    VOIDmode,
    0,
    0
  },
  {
    logical_binary_operator,
    "",
    DImode,
    0,
    0
  },
  {
    s_register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    0
  },
  {
    0,
    "",
    VOIDmode,
    0,
    0
  },
  {
    0,
    "",
    VOIDmode,
    0,
    0
  },
  {
    logical_binary_operator,
    "",
    DImode,
    0,
    0
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    general_operand,
    "",
    SImode,
    0,
    1
  },
  {
    general_operand,
    "",
    SImode,
    0,
    1
  },
  {
    nonmemory_operand,
    "",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rI",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rI",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    const_int_operand,
    "",
    SImode,
    0,
    1
  },
  {
    const_int_operand,
    "",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "",
    HImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    alignable_memory_operand,
    "",
    HImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    shiftable_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "",
    QImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    0,
    "",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    0,
    "",
    SImode,
    0,
    1
  },
  {
    0,
    "",
    HImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    HImode,
    0,
    1
  },
  {
    general_operand,
    "",
    QImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    HImode,
    0,
    1
  },
  {
    bad_signed_byte_operand,
    "",
    QImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    general_operand,
    "",
    QImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    bad_signed_byte_operand,
    "",
    QImode,
    0,
    1
  },
  {
    general_operand,
    "",
    DImode,
    0,
    1
  },
  {
    general_operand,
    "",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    address_operand,
    "",
    DImode,
    0,
    1
  },
  {
    memory_operand,
    "",
    HImode,
    0,
    1
  },
  {
    general_operand,
    "",
    HImode,
    0,
    1
  },
  {
    general_operand,
    "",
    HImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    HImode,
    0,
    1
  },
  {
    memory_operand,
    "",
    HImode,
    0,
    1
  },
  {
    arm_reload_memory_operand,
    "=o",
    HImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    HImode,
    0,
    1
  },
  {
    s_register_operand,
    "=&l",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    HImode,
    0,
    1
  },
  {
    arm_reload_memory_operand,
    "o",
    HImode,
    0,
    1
  },
  {
    s_register_operand,
    "=&r",
    DImode,
    0,
    1
  },
  {
    general_operand,
    "",
    QImode,
    0,
    1
  },
  {
    general_operand,
    "",
    QImode,
    0,
    1
  },
  {
    general_operand,
    "",
    SFmode,
    0,
    1
  },
  {
    general_operand,
    "",
    SFmode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "",
    SFmode,
    0,
    1
  },
  {
    immediate_operand,
    "",
    SFmode,
    0,
    1
  },
  {
    general_operand,
    "",
    DFmode,
    0,
    1
  },
  {
    general_operand,
    "",
    DFmode,
    0,
    1
  },
  {
    arm_reload_memory_operand,
    "=o",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "=&r",
    SImode,
    0,
    1
  },
  {
    general_operand,
    "",
    XFmode,
    0,
    1
  },
  {
    general_operand,
    "",
    XFmode,
    0,
    1
  },
  {
    general_operand,
    "",
    BLKmode,
    0,
    1
  },
  {
    general_operand,
    "",
    BLKmode,
    0,
    1
  },
  {
    const_int_operand,
    "",
    SImode,
    0,
    1
  },
  {
    const_int_operand,
    "",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_add_operand,
    "",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SFmode,
    0,
    1
  },
  {
    fpu_rhs_operand,
    "",
    SFmode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    DFmode,
    0,
    1
  },
  {
    fpu_rhs_operand,
    "",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    XFmode,
    0,
    1
  },
  {
    fpu_rhs_operand,
    "",
    XFmode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    1
  },
  {
    arm_not_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_not_operand,
    "",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SFmode,
    0,
    1
  },
  {
    nonmemory_operand,
    "",
    SFmode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    DFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    DFmode,
    0,
    1
  },
  {
    fpu_add_operand,
    "",
    DFmode,
    0,
    1
  },
  {
    memory_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    general_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    memory_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    general_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    memory_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    general_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    const_int_operand,
    "",
    SImode,
    0,
    1
  },
  {
    const_int_operand,
    "",
    SImode,
    0,
    1
  },
  {
    0,
    "",
    SImode,
    0,
    1
  },
  {
    0,
    "",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "+r",
    SImode,
    0,
    1
  },
  {
    index_operand,
    "rJ",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    QImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    QImode,
    0,
    1
  },
  {
    s_register_operand,
    "+r",
    SImode,
    0,
    1
  },
  {
    index_operand,
    "rJ",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    HImode,
    0,
    1
  },
  {
    s_register_operand,
    "+r",
    SImode,
    0,
    1
  },
  {
    index_operand,
    "rJ",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "+r",
    SImode,
    0,
    1
  },
  {
    index_operand,
    "rJ",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    const_int_operand,
    "n",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "+r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    QImode,
    0,
    1
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "=m",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "=m",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "=m",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "=m",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "",
    QImode,
    0,
    1
  },
  {
    const_int_operand,
    "",
    VOIDmode,
    0,
    1
  },
  {
    scratch_operand,
    "",
    SImode,
    0,
    0
  },
  {
    register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_add_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r,r,r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "%r,r,r",
    SImode,
    0,
    1
  },
  {
    reg_or_int_operand,
    "rI,L,?n",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f,f",
    SFmode,
    0,
    1
  },
  {
    s_register_operand,
    "%f,f",
    SFmode,
    0,
    1
  },
  {
    fpu_add_operand,
    "fG,H",
    SFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f,f",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "%f,f",
    DFmode,
    0,
    1
  },
  {
    fpu_add_operand,
    "fG,H",
    DFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f,f",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f,f",
    SFmode,
    0,
    1
  },
  {
    fpu_add_operand,
    "fG,H",
    DFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f,f",
    XFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f,f",
    XFmode,
    0,
    1
  },
  {
    fpu_add_operand,
    "fG,H",
    XFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r,r",
    SImode,
    0,
    1
  },
  {
    reg_or_int_operand,
    "rI,?n",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f,f",
    DFmode,
    0,
    1
  },
  {
    fpu_rhs_operand,
    "f,G",
    DFmode,
    0,
    1
  },
  {
    fpu_rhs_operand,
    "fG,f",
    DFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    fpu_rhs_operand,
    "fG",
    DFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f,f",
    DFmode,
    0,
    1
  },
  {
    fpu_rhs_operand,
    "f,G",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f,f",
    SFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f,f",
    XFmode,
    0,
    1
  },
  {
    fpu_rhs_operand,
    "f,G",
    XFmode,
    0,
    1
  },
  {
    fpu_rhs_operand,
    "fG,f",
    XFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=&r,&r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "%?r,0",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=&r,&r,&r,&r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "%r,0,r,0",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r,r,r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "?r,r,0,0",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=&r",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "0",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "%r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=&r",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "%r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=&r,&r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "%r,0",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "=&r,&r",
    SImode,
    0,
    0
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    SFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    fpu_rhs_operand,
    "fG",
    SFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    DFmode,
    0,
    1
  },
  {
    fpu_rhs_operand,
    "fG",
    DFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    XFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    XFmode,
    0,
    1
  },
  {
    fpu_rhs_operand,
    "fG",
    XFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f,f",
    SFmode,
    0,
    1
  },
  {
    fpu_rhs_operand,
    "f,G",
    SFmode,
    0,
    1
  },
  {
    fpu_rhs_operand,
    "fG,f",
    SFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    DFmode,
    0,
    1
  },
  {
    fpu_rhs_operand,
    "fG",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r,r,r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r,r",
    SImode,
    0,
    1
  },
  {
    reg_or_int_operand,
    "rI,K,?n",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=&r,&r",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,0",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "0,r",
    DImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=&r,&r",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "0,?r",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=&r,&r",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "?r,0",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rM",
    SImode,
    0,
    1
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=&r,&r",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "%0,r",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    DImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r,r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r",
    SImode,
    0,
    1
  },
  {
    reg_or_int_operand,
    "rI,?n",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rI",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=&r,&r,&r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r,r,0",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rI,0,rI",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rI,rI,rI",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    reg_or_int_operand,
    "rM",
    SImode,
    0,
    1
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rM",
    SImode,
    0,
    1
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    SFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    DFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    XFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    XFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=&r,&r",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "?r,0",
    DImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    SFmode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    XFmode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    DFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    XFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    SFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    DFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    SFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    XFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    DFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    XFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    DImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r,r",
    DImode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "r,m",
    QImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    HImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    QImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    HImode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    QImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    XFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    SFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=f",
    XFmode,
    0,
    1
  },
  {
    s_register_operand,
    "f",
    DFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=r,r,r,m",
    SImode,
    0,
    1
  },
  {
    general_operand,
    "rI,K,mi,r",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    address_operand,
    "p",
    DImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    register_operand,
    "+r",
    SImode,
    0,
    1
  },
  {
    0,
    "",
    VOIDmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    offsettable_memory_operand,
    "o",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=r,r,m,r",
    HImode,
    0,
    1
  },
  {
    general_operand,
    "rI,K,r,m",
    HImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r,r,r",
    HImode,
    0,
    1
  },
  {
    general_operand,
    "rI,K,m",
    HImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r,r",
    HImode,
    0,
    1
  },
  {
    arm_rhs_operand,
    "rI,K",
    HImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=r,r,r,m",
    QImode,
    0,
    1
  },
  {
    general_operand,
    "rI,K,m,r",
    QImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=f,f,f,m,f,r,r,r,m",
    SFmode,
    0,
    1
  },
  {
    general_operand,
    "fG,H,mE,f,r,f,r,mE,r",
    SFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=r,r,m",
    SFmode,
    0,
    1
  },
  {
    general_operand,
    "r,mE,r",
    SFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=r,Q,r,m,r,f,f,f,m,!f,!r",
    DFmode,
    0,
    1
  },
  {
    general_operand,
    "Q,r,r,r,mF,fG,H,mF,f,r,f",
    DFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    nonimmediate_operand,
    "=f,f,f,m,f,r,r",
    XFmode,
    0,
    1
  },
  {
    general_operand,
    "fG,H,m,f,r,f,r",
    XFmode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    load_multiple_operation,
    "",
    VOIDmode,
    0,
    0
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "1",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    load_multiple_operation,
    "",
    VOIDmode,
    0,
    0
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "1",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    load_multiple_operation,
    "",
    VOIDmode,
    0,
    0
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "1",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    load_multiple_operation,
    "",
    VOIDmode,
    0,
    0
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    load_multiple_operation,
    "",
    VOIDmode,
    0,
    0
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    load_multiple_operation,
    "",
    VOIDmode,
    0,
    0
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    store_multiple_operation,
    "",
    VOIDmode,
    0,
    0
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "1",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    store_multiple_operation,
    "",
    VOIDmode,
    0,
    0
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "1",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    store_multiple_operation,
    "",
    VOIDmode,
    0,
    0
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "1",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    store_multiple_operation,
    "",
    VOIDmode,
    0,
    0
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    store_multiple_operation,
    "",
    VOIDmode,
    0,
    0
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    store_multiple_operation,
    "",
    VOIDmode,
    0,
    0
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_hard_register_operand,
    "",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    shiftable_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    reg_or_int_operand,
    "rI",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    reg_or_int_operand,
    "rM",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    shiftable_operator,
    "",
    SImode,
    0,
    0
  },
  {
    memory_operand,
    "m",
    SImode,
    0,
    1
  },
  {
    memory_operand,
    "m",
    SImode,
    0,
    1
  },
  {
    scratch_operand,
    "=r",
    SImode,
    0,
    0
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "%0",
    SImode,
    0,
    1
  },
  {
    index_operand,
    "rJ",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    QImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "0",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    QImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "%0",
    SImode,
    0,
    1
  },
  {
    index_operand,
    "rJ",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    QImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "0",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    QImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "%0",
    SImode,
    0,
    1
  },
  {
    index_operand,
    "rJ",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "0",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "%0",
    SImode,
    0,
    1
  },
  {
    index_operand,
    "rJ",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "0",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "%0",
    SImode,
    0,
    1
  },
  {
    index_operand,
    "rJ",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    HImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "0",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    HImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "0",
    SImode,
    0,
    1
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    const_shift_operand,
    "n",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    QImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "0",
    SImode,
    0,
    1
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    const_shift_operand,
    "n",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    QImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "0",
    SImode,
    0,
    1
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    const_shift_operand,
    "n",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "0",
    SImode,
    0,
    1
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    const_shift_operand,
    "n",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "0",
    SImode,
    0,
    1
  },
  {
    shift_operator,
    "",
    SImode,
    0,
    0
  },
  {
    s_register_operand,
    "r",
    SImode,
    0,
    1
  },
  {
    const_shift_operand,
    "n",
    SImode,
    0,
    1
  },
  {
    s_register_operand,
    "=r",
    HImode,
    0,
    1
  },
  {
    arm_comparison_operator,
    "",
    VOIDmode,
    0,
    0
  },
  {
    cc_register,
    "",
    VOIDmode,
    0,
    1
  },
};



const struct insn_data insn_data[] = 
{
  {
    "*thumb_adddi3",
    "add\t%Q0, %Q0, %Q2\n\tadc\t%R0, %R0, %R2",
    0,
    &operand_data[1],
    3,
    0,
    1,
    1
  },
  {
    "*arm_adddi3",
    "#",
    0,
    &operand_data[4],
    3,
    0,
    2,
    1
  },
  {
    "*adddi_sesidi_di",
    "#",
    0,
    &operand_data[7],
    3,
    0,
    2,
    1
  },
  {
    "*adddi_zesidi_di",
    "#",
    0,
    &operand_data[7],
    3,
    0,
    2,
    1
  },
  {
    "*arm_addsi3",
    (const PTR) output_4,
    0,
    &operand_data[10],
    3,
    0,
    3,
    2
  },
  {
    "*thumb_addsi3",
    (const PTR) output_5,
    0,
    &operand_data[13],
    3,
    0,
    7,
    3
  },
  {
    "*addsi3_compare0",
    (const PTR) output_6,
    0,
    &operand_data[16],
    3,
    2,
    2,
    2
  },
  {
    "*addsi3_compare0_scratch",
    (const PTR) output_7,
    0,
    &operand_data[17],
    2,
    0,
    2,
    2
  },
  {
    "*addsi3_compare0_for_combiner",
    (const PTR) output_8,
    0,
    &operand_data[16],
    3,
    2,
    2,
    2
  },
  {
    "*addsi3_compare0_scratch_for_combiner",
    (const PTR) output_9,
    0,
    &operand_data[17],
    2,
    0,
    2,
    2
  },
  {
    "*addsi3_compare_op1",
    (const PTR) output_10,
    0,
    &operand_data[16],
    3,
    3,
    2,
    2
  },
  {
    "*addsi3_compare_op2",
    (const PTR) output_11,
    0,
    &operand_data[16],
    3,
    3,
    2,
    2
  },
  {
    "*compare_addsi2_op0",
    (const PTR) output_12,
    0,
    &operand_data[17],
    2,
    1,
    2,
    2
  },
  {
    "*compare_addsi2_op1",
    (const PTR) output_13,
    0,
    &operand_data[17],
    2,
    1,
    2,
    2
  },
  {
    "*addsi3_carryin",
    "adc%?\t%0, %1, %2",
    0,
    &operand_data[19],
    3,
    0,
    1,
    1
  },
  {
    "*addsi3_carryin_shift",
    "adc%?\t%0, %1, %3%S2",
    0,
    &operand_data[22],
    5,
    0,
    0,
    1
  },
  {
    "*addsi3_carryin_alt1",
    "adc%?\t%0, %1, %2",
    0,
    &operand_data[19],
    3,
    0,
    1,
    1
  },
  {
    "*addsi3_carryin_alt2",
    "adc%?\t%0, %1, %2",
    0,
    &operand_data[19],
    3,
    0,
    1,
    1
  },
  {
    "*addsi3_carryin_alt3",
    "adc%?\t%0, %1, %2",
    0,
    &operand_data[19],
    3,
    0,
    1,
    1
  },
  {
    "incscc",
    (const PTR) output_19,
    (insn_gen_fn) gen_incscc,
    &operand_data[27],
    4,
    0,
    2,
    2
  },
  {
    "addsf3",
    (const PTR) output_20,
    (insn_gen_fn) gen_addsf3,
    &operand_data[31],
    3,
    0,
    2,
    2
  },
  {
    "adddf3",
    (const PTR) output_21,
    (insn_gen_fn) gen_adddf3,
    &operand_data[34],
    3,
    0,
    2,
    2
  },
  {
    "*adddf_esfdf_df",
    (const PTR) output_22,
    0,
    &operand_data[37],
    3,
    0,
    2,
    2
  },
  {
    "*adddf_df_esfdf",
    "adf%?d\t%0, %1, %2",
    0,
    &operand_data[40],
    3,
    0,
    1,
    1
  },
  {
    "*adddf_esfdf_esfdf",
    "adf%?d\t%0, %1, %2",
    0,
    &operand_data[43],
    3,
    0,
    1,
    1
  },
  {
    "addxf3",
    (const PTR) output_25,
    (insn_gen_fn) gen_addxf3,
    &operand_data[46],
    3,
    0,
    2,
    2
  },
  {
    "*arm_subdi3",
    "subs\t%Q0, %Q1, %Q2\n\tsbc\t%R0, %R1, %R2",
    0,
    &operand_data[49],
    3,
    0,
    3,
    1
  },
  {
    "*thumb_subdi3",
    "sub\t%Q0, %Q0, %Q2\n\tsbc\t%R0, %R0, %R2",
    0,
    &operand_data[52],
    3,
    0,
    1,
    1
  },
  {
    "*subdi_di_zesidi",
    "subs\t%Q0, %Q1, %2\n\tsbc\t%R0, %R1, #0",
    0,
    &operand_data[55],
    3,
    0,
    2,
    1
  },
  {
    "*subdi_di_sesidi",
    "subs\t%Q0, %Q1, %2\n\tsbc\t%R0, %R1, %2, asr #31",
    0,
    &operand_data[7],
    3,
    0,
    2,
    1
  },
  {
    "*subdi_zesidi_di",
    "rsbs\t%Q0, %Q1, %2\n\trsc\t%R0, %R1, #0",
    0,
    &operand_data[55],
    3,
    0,
    2,
    1
  },
  {
    "*subdi_sesidi_di",
    "rsbs\t%Q0, %Q1, %2\n\trsc\t%R0, %R1, %2, asr #31",
    0,
    &operand_data[55],
    3,
    0,
    2,
    1
  },
  {
    "*subdi_zesidi_zesidi",
    "subs\t%Q0, %1, %2\n\trsc\t%R0, %1, %1",
    0,
    &operand_data[58],
    3,
    0,
    1,
    1
  },
  {
    "*thumb_subsi3_insn",
    "sub\t%0, %1, %2",
    0,
    &operand_data[61],
    3,
    0,
    1,
    1
  },
  {
    "*arm_subsi3_insn",
    (const PTR) output_34,
    0,
    &operand_data[64],
    3,
    0,
    2,
    2
  },
  {
    "*subsi3_compare0",
    (const PTR) output_35,
    0,
    &operand_data[67],
    3,
    2,
    2,
    2
  },
  {
    "decscc",
    (const PTR) output_36,
    (insn_gen_fn) gen_decscc,
    &operand_data[70],
    4,
    0,
    2,
    2
  },
  {
    "subsf3",
    (const PTR) output_37,
    (insn_gen_fn) gen_subsf3,
    &operand_data[74],
    3,
    0,
    2,
    2
  },
  {
    "subdf3",
    (const PTR) output_38,
    (insn_gen_fn) gen_subdf3,
    &operand_data[77],
    3,
    0,
    2,
    2
  },
  {
    "*subdf_esfdf_df",
    "suf%?d\t%0, %1, %2",
    0,
    &operand_data[80],
    3,
    0,
    1,
    1
  },
  {
    "*subdf_df_esfdf",
    (const PTR) output_40,
    0,
    &operand_data[83],
    3,
    0,
    2,
    2
  },
  {
    "*subdf_esfdf_esfdf",
    "suf%?d\t%0, %1, %2",
    0,
    &operand_data[43],
    3,
    0,
    1,
    1
  },
  {
    "subxf3",
    (const PTR) output_42,
    (insn_gen_fn) gen_subxf3,
    &operand_data[86],
    3,
    0,
    2,
    2
  },
  {
    "*arm_mulsi3",
    "mul%?\t%0, %2, %1",
    0,
    &operand_data[89],
    3,
    0,
    2,
    1
  },
  {
    "*thumb_mulsi3",
    (const PTR) output_44,
    0,
    &operand_data[92],
    3,
    0,
    3,
    3
  },
  {
    "*mulsi3_compare0",
    "mul%?s\t%0, %2, %1",
    0,
    &operand_data[89],
    3,
    2,
    2,
    1
  },
  {
    "*mulsi_compare0_scratch",
    "mul%?s\t%0, %2, %1",
    0,
    &operand_data[95],
    3,
    0,
    2,
    1
  },
  {
    "*mulsi3addsi",
    "mla%?\t%0, %2, %1, %3",
    0,
    &operand_data[98],
    4,
    0,
    4,
    1
  },
  {
    "*mulsi3addsi_compare0",
    "mla%?s\t%0, %2, %1, %3",
    0,
    &operand_data[98],
    4,
    3,
    4,
    1
  },
  {
    "*mulsi3addsi_compare0_scratch",
    "mla%?s\t%0, %2, %1, %3",
    0,
    &operand_data[102],
    4,
    0,
    4,
    1
  },
  {
    "*mulsidi3adddi",
    "smlal%?\t%Q0, %R0, %3, %2",
    0,
    &operand_data[106],
    4,
    0,
    1,
    1
  },
  {
    "mulsidi3",
    "smull%?\t%Q0, %R0, %1, %2",
    (insn_gen_fn) gen_mulsidi3,
    &operand_data[110],
    3,
    0,
    1,
    1
  },
  {
    "umulsidi3",
    "umull%?\t%Q0, %R0, %1, %2",
    (insn_gen_fn) gen_umulsidi3,
    &operand_data[110],
    3,
    0,
    1,
    1
  },
  {
    "*umulsidi3adddi",
    "umlal%?\t%Q0, %R0, %3, %2",
    0,
    &operand_data[106],
    4,
    0,
    1,
    1
  },
  {
    "smulsi3_highpart",
    "smull%?\t%3, %0, %2, %1",
    (insn_gen_fn) gen_smulsi3_highpart,
    &operand_data[113],
    4,
    0,
    2,
    1
  },
  {
    "umulsi3_highpart",
    "umull%?\t%3, %0, %2, %1",
    (insn_gen_fn) gen_umulsi3_highpart,
    &operand_data[113],
    4,
    0,
    2,
    1
  },
  {
    "mulhisi3",
    "smulbb%?\t%0, %1, %2",
    (insn_gen_fn) gen_mulhisi3,
    &operand_data[117],
    3,
    0,
    1,
    1
  },
  {
    "*mulhisi3addsi",
    "smlabb%?\t%0, %2, %3, %1",
    0,
    &operand_data[120],
    4,
    0,
    1,
    1
  },
  {
    "*mulhidi3adddi",
    "smlalbb%?\t%Q0, %R0, %2, %3",
    0,
    &operand_data[124],
    4,
    0,
    1,
    1
  },
  {
    "mulsf3",
    "fml%?s\t%0, %1, %2",
    (insn_gen_fn) gen_mulsf3,
    &operand_data[128],
    3,
    0,
    1,
    1
  },
  {
    "muldf3",
    "muf%?d\t%0, %1, %2",
    (insn_gen_fn) gen_muldf3,
    &operand_data[131],
    3,
    0,
    1,
    1
  },
  {
    "*muldf_esfdf_df",
    "muf%?d\t%0, %1, %2",
    0,
    &operand_data[80],
    3,
    0,
    1,
    1
  },
  {
    "*muldf_df_esfdf",
    "muf%?d\t%0, %1, %2",
    0,
    &operand_data[40],
    3,
    0,
    1,
    1
  },
  {
    "*muldf_esfdf_esfdf",
    "muf%?d\t%0, %1, %2",
    0,
    &operand_data[43],
    3,
    0,
    1,
    1
  },
  {
    "mulxf3",
    "muf%?e\t%0, %1, %2",
    (insn_gen_fn) gen_mulxf3,
    &operand_data[134],
    3,
    0,
    1,
    1
  },
  {
    "divsf3",
    (const PTR) output_65,
    (insn_gen_fn) gen_divsf3,
    &operand_data[74],
    3,
    0,
    2,
    2
  },
  {
    "divdf3",
    (const PTR) output_66,
    (insn_gen_fn) gen_divdf3,
    &operand_data[77],
    3,
    0,
    2,
    2
  },
  {
    "*divdf_esfdf_df",
    "dvf%?d\t%0, %1, %2",
    0,
    &operand_data[80],
    3,
    0,
    1,
    1
  },
  {
    "*divdf_df_esfdf",
    "rdf%?d\t%0, %2, %1",
    0,
    &operand_data[137],
    3,
    0,
    1,
    1
  },
  {
    "*divdf_esfdf_esfdf",
    "dvf%?d\t%0, %1, %2",
    0,
    &operand_data[43],
    3,
    0,
    1,
    1
  },
  {
    "divxf3",
    (const PTR) output_70,
    (insn_gen_fn) gen_divxf3,
    &operand_data[86],
    3,
    0,
    2,
    2
  },
  {
    "modsf3",
    "rmf%?s\t%0, %1, %2",
    (insn_gen_fn) gen_modsf3,
    &operand_data[128],
    3,
    0,
    1,
    1
  },
  {
    "moddf3",
    "rmf%?d\t%0, %1, %2",
    (insn_gen_fn) gen_moddf3,
    &operand_data[131],
    3,
    0,
    1,
    1
  },
  {
    "*moddf_esfdf_df",
    "rmf%?d\t%0, %1, %2",
    0,
    &operand_data[80],
    3,
    0,
    1,
    1
  },
  {
    "*moddf_df_esfdf",
    "rmf%?d\t%0, %1, %2",
    0,
    &operand_data[40],
    3,
    0,
    1,
    1
  },
  {
    "*moddf_esfdf_esfdf",
    "rmf%?d\t%0, %1, %2",
    0,
    &operand_data[43],
    3,
    0,
    1,
    1
  },
  {
    "modxf3",
    "rmf%?e\t%0, %1, %2",
    (insn_gen_fn) gen_modxf3,
    &operand_data[134],
    3,
    0,
    1,
    1
  },
  {
    "anddi3",
    "#",
    (insn_gen_fn) gen_anddi3,
    &operand_data[140],
    3,
    0,
    2,
    1
  },
  {
    "*anddi_zesidi_di",
    "#",
    0,
    &operand_data[55],
    3,
    0,
    2,
    1
  },
  {
    "*anddi_sesdi_di",
    "#",
    0,
    &operand_data[55],
    3,
    0,
    2,
    1
  },
  {
    "*arm_andsi3_insn",
    (const PTR) output_80,
    0,
    &operand_data[143],
    3,
    0,
    3,
    2
  },
  {
    "*thumb_andsi3_insn",
    "and\t%0, %0, %2",
    0,
    &operand_data[146],
    3,
    0,
    1,
    1
  },
  {
    "*andsi3_compare0",
    (const PTR) output_82,
    0,
    &operand_data[149],
    3,
    2,
    2,
    2
  },
  {
    "*andsi3_compare0_scratch",
    (const PTR) output_83,
    0,
    &operand_data[150],
    3,
    0,
    2,
    2
  },
  {
    "*zeroextractsi_compare0_scratch",
    (const PTR) output_84,
    0,
    &operand_data[153],
    3,
    0,
    1,
    3
  },
  {
    "*ne_zeroextractsi",
    (const PTR) output_85,
    0,
    &operand_data[156],
    4,
    0,
    1,
    3
  },
  {
    "*anddi_notdi_di",
    "#",
    0,
    &operand_data[160],
    3,
    0,
    2,
    1
  },
  {
    "*anddi_notzesidi_di",
    (const PTR) output_87,
    0,
    &operand_data[163],
    3,
    0,
    2,
    2
  },
  {
    "*anddi_notsesidi_di",
    "#",
    0,
    &operand_data[55],
    3,
    0,
    2,
    1
  },
  {
    "andsi_notsi_si",
    "bic%?\t%0, %1, %2",
    (insn_gen_fn) gen_andsi_notsi_si,
    &operand_data[166],
    3,
    0,
    1,
    1
  },
  {
    "bicsi3",
    "bic\t%0, %0, %1",
    (insn_gen_fn) gen_bicsi3,
    &operand_data[169],
    3,
    0,
    1,
    1
  },
  {
    "andsi_not_shiftsi_si",
    "bic%?\t%0, %1, %2%S4",
    (insn_gen_fn) gen_andsi_not_shiftsi_si,
    &operand_data[172],
    5,
    0,
    1,
    1
  },
  {
    "*andsi_notsi_si_compare0",
    "bic%?s\t%0, %1, %2",
    0,
    &operand_data[166],
    3,
    2,
    1,
    1
  },
  {
    "*andsi_notsi_si_compare0_scratch",
    "bic%?s\t%0, %1, %2",
    0,
    &operand_data[177],
    3,
    0,
    1,
    1
  },
  {
    "iordi3",
    "#",
    (insn_gen_fn) gen_iordi3,
    &operand_data[140],
    3,
    0,
    2,
    1
  },
  {
    "*iordi_zesidi_di",
    (const PTR) output_95,
    0,
    &operand_data[163],
    3,
    0,
    2,
    2
  },
  {
    "*iordi_sesidi_di",
    "#",
    0,
    &operand_data[55],
    3,
    0,
    2,
    1
  },
  {
    "*arm_iorsi3",
    (const PTR) output_97,
    0,
    &operand_data[180],
    3,
    0,
    2,
    2
  },
  {
    "*thumb_iorsi3",
    "orr\t%0, %0, %2",
    0,
    &operand_data[146],
    3,
    0,
    1,
    1
  },
  {
    "*iorsi3_compare0",
    "orr%?s\t%0, %1, %2",
    0,
    &operand_data[183],
    3,
    2,
    1,
    1
  },
  {
    "*iorsi3_compare0_scratch",
    "orr%?s\t%0, %1, %2",
    0,
    &operand_data[186],
    3,
    0,
    1,
    1
  },
  {
    "xordi3",
    "#",
    (insn_gen_fn) gen_xordi3,
    &operand_data[140],
    3,
    0,
    2,
    1
  },
  {
    "*xordi_zesidi_di",
    (const PTR) output_102,
    0,
    &operand_data[163],
    3,
    0,
    2,
    2
  },
  {
    "*xordi_sesidi_di",
    "#",
    0,
    &operand_data[55],
    3,
    0,
    2,
    1
  },
  {
    "*arm_xorsi3",
    "eor%?\t%0, %1, %2",
    0,
    &operand_data[19],
    3,
    0,
    1,
    1
  },
  {
    "*thumb_xorsi3",
    "eor\t%0, %0, %2",
    0,
    &operand_data[146],
    3,
    0,
    1,
    1
  },
  {
    "*xorsi3_compare0",
    "eor%?s\t%0, %1, %2",
    0,
    &operand_data[19],
    3,
    2,
    1,
    1
  },
  {
    "*xorsi3_compare0_scratch",
    "teq%?\t%0, %1",
    0,
    &operand_data[20],
    2,
    0,
    1,
    1
  },
  {
    "*andsi_iorsi3_notsi",
    "orr%?\t%0, %1, %2\n\tbic%?\t%0, %0, %3",
    0,
    &operand_data[189],
    4,
    0,
    3,
    1
  },
  {
    "smaxsi3",
    (const PTR) output_109,
    (insn_gen_fn) gen_smaxsi3,
    &operand_data[193],
    3,
    0,
    3,
    2
  },
  {
    "sminsi3",
    (const PTR) output_110,
    (insn_gen_fn) gen_sminsi3,
    &operand_data[193],
    3,
    0,
    3,
    2
  },
  {
    "umaxsi3",
    (const PTR) output_111,
    (insn_gen_fn) gen_umaxsi3,
    &operand_data[193],
    3,
    0,
    3,
    2
  },
  {
    "uminsi3",
    (const PTR) output_112,
    (insn_gen_fn) gen_uminsi3,
    &operand_data[193],
    3,
    0,
    3,
    2
  },
  {
    "*store_minmaxsi",
    (const PTR) output_113,
    0,
    &operand_data[196],
    4,
    0,
    1,
    3
  },
  {
    "*minmax_arithsi",
    (const PTR) output_114,
    0,
    &operand_data[200],
    6,
    0,
    2,
    3
  },
  {
    "*thumb_ashlsi3",
    "lsl\t%0, %1, %2",
    0,
    &operand_data[206],
    3,
    0,
    2,
    1
  },
  {
    "*thumb_ashrsi3",
    "asr\t%0, %1, %2",
    0,
    &operand_data[206],
    3,
    0,
    2,
    1
  },
  {
    "*thumb_lshrsi3",
    "lsr\t%0, %1, %2",
    0,
    &operand_data[206],
    3,
    0,
    2,
    1
  },
  {
    "*thumb_rotrsi3",
    "ror\t%0, %0, %2",
    0,
    &operand_data[209],
    3,
    0,
    1,
    1
  },
  {
    "*arm_shiftsi3",
    "mov%?\t%0, %1%S3",
    0,
    &operand_data[212],
    4,
    0,
    1,
    1
  },
  {
    "*shiftsi3_compare0",
    "mov%?s\t%0, %1%S3",
    0,
    &operand_data[216],
    4,
    1,
    1,
    1
  },
  {
    "*shiftsi3_compare0_scratch",
    "mov%?s\t%0, %1%S3",
    0,
    &operand_data[220],
    4,
    0,
    1,
    1
  },
  {
    "*notsi_shiftsi",
    "mvn%?\t%0, %1%S3",
    0,
    &operand_data[216],
    4,
    0,
    1,
    1
  },
  {
    "*notsi_shiftsi_compare0",
    "mvn%?s\t%0, %1%S3",
    0,
    &operand_data[216],
    4,
    1,
    1,
    1
  },
  {
    "*not_shiftsi_compare0_scratch",
    "mvn%?s\t%0, %1%S3",
    0,
    &operand_data[220],
    4,
    0,
    1,
    1
  },
  {
    "*arm_negdi2",
    "rsbs\t%Q0, %Q1, #0\n\trsc\t%R0, %R1, #0",
    0,
    &operand_data[224],
    2,
    0,
    2,
    1
  },
  {
    "*thumb_negdi2",
    "mov\t%R0, #0\n\tneg\t%Q0, %Q1\n\tsbc\t%R0, %R1",
    0,
    &operand_data[226],
    2,
    0,
    1,
    1
  },
  {
    "*arm_negsi2",
    "rsb%?\t%0, %1, #0",
    0,
    &operand_data[19],
    2,
    0,
    1,
    1
  },
  {
    "*thumb_negsi2",
    "neg\t%0, %1",
    0,
    &operand_data[61],
    2,
    0,
    1,
    1
  },
  {
    "negsf2",
    "mnf%?s\t%0, %1",
    (insn_gen_fn) gen_negsf2,
    &operand_data[128],
    2,
    0,
    1,
    1
  },
  {
    "negdf2",
    "mnf%?d\t%0, %1",
    (insn_gen_fn) gen_negdf2,
    &operand_data[40],
    2,
    0,
    1,
    1
  },
  {
    "*negdf_esfdf",
    "mnf%?d\t%0, %1",
    0,
    &operand_data[43],
    2,
    0,
    1,
    1
  },
  {
    "negxf2",
    "mnf%?e\t%0, %1",
    (insn_gen_fn) gen_negxf2,
    &operand_data[134],
    2,
    0,
    1,
    1
  },
  {
    "abssi2",
    (const PTR) output_133,
    (insn_gen_fn) gen_abssi2,
    &operand_data[228],
    2,
    0,
    2,
    2
  },
  {
    "*neg_abssi2",
    (const PTR) output_134,
    0,
    &operand_data[228],
    2,
    0,
    2,
    2
  },
  {
    "abssf2",
    "abs%?s\t%0, %1",
    (insn_gen_fn) gen_abssf2,
    &operand_data[128],
    2,
    0,
    1,
    1
  },
  {
    "absdf2",
    "abs%?d\t%0, %1",
    (insn_gen_fn) gen_absdf2,
    &operand_data[40],
    2,
    0,
    1,
    1
  },
  {
    "*absdf_esfdf",
    "abs%?d\t%0, %1",
    0,
    &operand_data[43],
    2,
    0,
    1,
    1
  },
  {
    "absxf2",
    "abs%?e\t%0, %1",
    (insn_gen_fn) gen_absxf2,
    &operand_data[134],
    2,
    0,
    1,
    1
  },
  {
    "sqrtsf2",
    "sqt%?s\t%0, %1",
    (insn_gen_fn) gen_sqrtsf2,
    &operand_data[128],
    2,
    0,
    1,
    1
  },
  {
    "sqrtdf2",
    "sqt%?d\t%0, %1",
    (insn_gen_fn) gen_sqrtdf2,
    &operand_data[40],
    2,
    0,
    1,
    1
  },
  {
    "*sqrtdf_esfdf",
    "sqt%?d\t%0, %1",
    0,
    &operand_data[43],
    2,
    0,
    1,
    1
  },
  {
    "sqrtxf2",
    "sqt%?e\t%0, %1",
    (insn_gen_fn) gen_sqrtxf2,
    &operand_data[134],
    2,
    0,
    1,
    1
  },
  {
    "one_cmpldi2",
    "#",
    (insn_gen_fn) gen_one_cmpldi2,
    &operand_data[55],
    2,
    0,
    2,
    1
  },
  {
    "*arm_one_cmplsi2",
    "mvn%?\t%0, %1",
    0,
    &operand_data[19],
    2,
    0,
    1,
    1
  },
  {
    "*thumb_one_cmplsi2",
    "mvn\t%0, %1",
    0,
    &operand_data[61],
    2,
    0,
    1,
    1
  },
  {
    "*notsi_compare0",
    "mvn%?s\t%0, %1",
    0,
    &operand_data[19],
    2,
    1,
    1,
    1
  },
  {
    "*notsi_compare0_scratch",
    "mvn%?s\t%0, %1",
    0,
    &operand_data[177],
    2,
    0,
    1,
    1
  },
  {
    "floatsisf2",
    "flt%?s\t%0, %1",
    (insn_gen_fn) gen_floatsisf2,
    &operand_data[230],
    2,
    0,
    1,
    1
  },
  {
    "floatsidf2",
    "flt%?d\t%0, %1",
    (insn_gen_fn) gen_floatsidf2,
    &operand_data[232],
    2,
    0,
    1,
    1
  },
  {
    "floatsixf2",
    "flt%?e\t%0, %1",
    (insn_gen_fn) gen_floatsixf2,
    &operand_data[234],
    2,
    0,
    1,
    1
  },
  {
    "fix_truncsfsi2",
    "fix%?z\t%0, %1",
    (insn_gen_fn) gen_fix_truncsfsi2,
    &operand_data[236],
    2,
    0,
    1,
    1
  },
  {
    "fix_truncdfsi2",
    "fix%?z\t%0, %1",
    (insn_gen_fn) gen_fix_truncdfsi2,
    &operand_data[238],
    2,
    0,
    1,
    1
  },
  {
    "fix_truncxfsi2",
    "fix%?z\t%0, %1",
    (insn_gen_fn) gen_fix_truncxfsi2,
    &operand_data[240],
    2,
    0,
    1,
    1
  },
  {
    "truncdfsf2",
    "mvf%?s\t%0, %1",
    (insn_gen_fn) gen_truncdfsf2,
    &operand_data[242],
    2,
    0,
    1,
    1
  },
  {
    "truncxfsf2",
    "mvf%?s\t%0, %1",
    (insn_gen_fn) gen_truncxfsf2,
    &operand_data[244],
    2,
    0,
    1,
    1
  },
  {
    "truncxfdf2",
    "mvf%?d\t%0, %1",
    (insn_gen_fn) gen_truncxfdf2,
    &operand_data[246],
    2,
    0,
    1,
    1
  },
  {
    "zero_extendsidi2",
    (const PTR) output_157,
    (insn_gen_fn) gen_zero_extendsidi2,
    &operand_data[58],
    2,
    0,
    1,
    3
  },
  {
    "zero_extendqidi2",
    (const PTR) output_158,
    (insn_gen_fn) gen_zero_extendqidi2,
    &operand_data[248],
    2,
    0,
    2,
    2
  },
  {
    "extendsidi2",
    (const PTR) output_159,
    (insn_gen_fn) gen_extendsidi2,
    &operand_data[58],
    2,
    0,
    1,
    3
  },
  {
    "*thumb_zero_extendhisi2",
    (const PTR) output_160,
    0,
    &operand_data[250],
    2,
    0,
    1,
    3
  },
  {
    "*arm_zero_extendhisi2",
    "ldr%?h\t%0, %1",
    0,
    &operand_data[252],
    2,
    0,
    1,
    1
  },
  {
    "*thumb_zero_extendqisi2",
    "ldrb\t%0, %1",
    0,
    &operand_data[254],
    2,
    0,
    1,
    1
  },
  {
    "*arm_zero_extendqisi2",
    "ldr%?b\t%0, %1\t%@ zero_extendqisi2",
    0,
    &operand_data[256],
    2,
    0,
    1,
    1
  },
  {
    "*compareqi_eq0",
    "tst\t%0, #255",
    0,
    &operand_data[258],
    1,
    0,
    1,
    1
  },
  {
    "*thumb_extendhisi2_insn",
    (const PTR) output_165,
    0,
    &operand_data[259],
    3,
    0,
    1,
    3
  },
  {
    "*arm_extendhisi_insn",
    "ldr%?sh\t%0, %1",
    0,
    &operand_data[252],
    2,
    0,
    1,
    1
  },
  {
    "*extendqihi_insn",
    (const PTR) output_167,
    0,
    &operand_data[262],
    2,
    0,
    1,
    3
  },
  {
    "*arm_extendqisi_insn",
    (const PTR) output_168,
    0,
    &operand_data[256],
    2,
    0,
    1,
    3
  },
  {
    "*thumb_extendqisi2_insn",
    (const PTR) output_169,
    0,
    &operand_data[264],
    2,
    0,
    2,
    3
  },
  {
    "extendsfdf2",
    "mvf%?d\t%0, %1",
    (insn_gen_fn) gen_extendsfdf2,
    &operand_data[43],
    2,
    0,
    1,
    1
  },
  {
    "extendsfxf2",
    "mvf%?e\t%0, %1",
    (insn_gen_fn) gen_extendsfxf2,
    &operand_data[266],
    2,
    0,
    1,
    1
  },
  {
    "extenddfxf2",
    "mvf%?e\t%0, %1",
    (insn_gen_fn) gen_extenddfxf2,
    &operand_data[268],
    2,
    0,
    1,
    1
  },
  {
    "*arm_movdi",
    (const PTR) output_173,
    0,
    &operand_data[270],
    2,
    0,
    3,
    3
  },
  {
    "*thumb_movdi_insn",
    (const PTR) output_174,
    0,
    &operand_data[272],
    2,
    0,
    8,
    3
  },
  {
    "*arm_movsi_insn",
    (const PTR) output_175,
    0,
    &operand_data[274],
    2,
    0,
    4,
    2
  },
  {
    "*thumb_movsi_insn",
    (const PTR) output_176,
    0,
    &operand_data[276],
    2,
    0,
    9,
    2
  },
  {
    "*movaddr_insn",
    "adr%?\t%0, %a1",
    0,
    &operand_data[278],
    2,
    0,
    1,
    1
  },
  {
    "pic_load_addr_arm",
    "ldr%?\t%0, %1",
    (insn_gen_fn) gen_pic_load_addr_arm,
    &operand_data[280],
    2,
    0,
    1,
    1
  },
  {
    "pic_load_addr_thumb",
    "ldr\t%0, %1",
    (insn_gen_fn) gen_pic_load_addr_thumb,
    &operand_data[282],
    2,
    0,
    1,
    1
  },
  {
    "*pic_load_addr_based_insn",
    (const PTR) output_180,
    0,
    &operand_data[284],
    3,
    0,
    1,
    3
  },
  {
    "pic_add_dot_plus_four",
    (const PTR) output_181,
    (insn_gen_fn) gen_pic_add_dot_plus_four,
    &operand_data[287],
    2,
    1,
    1,
    3
  },
  {
    "pic_add_dot_plus_eight",
    (const PTR) output_182,
    (insn_gen_fn) gen_pic_add_dot_plus_eight,
    &operand_data[287],
    2,
    1,
    1,
    3
  },
  {
    "*movsi_compare0",
    (const PTR) output_183,
    0,
    &operand_data[289],
    2,
    1,
    2,
    2
  },
  {
    "*thumb_movhi_insn",
    (const PTR) output_184,
    0,
    &operand_data[291],
    2,
    0,
    6,
    3
  },
  {
    "rotated_loadsi",
    (const PTR) output_185,
    (insn_gen_fn) gen_rotated_loadsi,
    &operand_data[293],
    2,
    0,
    1,
    3
  },
  {
    "*movhi_insn_arch4",
    (const PTR) output_186,
    0,
    &operand_data[295],
    2,
    0,
    4,
    2
  },
  {
    "*movhi_insn_littleend",
    (const PTR) output_187,
    0,
    &operand_data[297],
    2,
    0,
    3,
    2
  },
  {
    "*movhi_insn_bigend",
    (const PTR) output_188,
    0,
    &operand_data[297],
    2,
    0,
    3,
    2
  },
  {
    "*loadhi_si_bigend",
    "ldr%?\t%0, %1\t%@ movhi_bigend",
    0,
    &operand_data[252],
    2,
    0,
    1,
    1
  },
  {
    "*movhi_bytes",
    (const PTR) output_190,
    0,
    &operand_data[299],
    2,
    0,
    2,
    2
  },
  {
    "thumb_movhi_clobber",
    (const PTR) output_191,
    (insn_gen_fn) gen_thumb_movhi_clobber,
    &operand_data[301],
    3,
    0,
    1,
    3
  },
  {
    "*arm_movqi_insn",
    (const PTR) output_192,
    0,
    &operand_data[304],
    2,
    0,
    4,
    2
  },
  {
    "*thumb_movqi_insn",
    (const PTR) output_193,
    0,
    &operand_data[306],
    2,
    0,
    6,
    2
  },
  {
    "*arm_movsf_hard_insn",
    (const PTR) output_194,
    0,
    &operand_data[308],
    2,
    0,
    9,
    2
  },
  {
    "*arm_movsf_soft_insn",
    (const PTR) output_195,
    0,
    &operand_data[310],
    2,
    0,
    3,
    2
  },
  {
    "*thumb_movsf_insn",
    (const PTR) output_196,
    0,
    &operand_data[312],
    2,
    0,
    7,
    2
  },
  {
    "*movdf_hard_insn",
    (const PTR) output_197,
    0,
    &operand_data[314],
    2,
    0,
    11,
    3
  },
  {
    "*movdf_soft_insn",
    (const PTR) output_198,
    0,
    &operand_data[316],
    2,
    0,
    3,
    3
  },
  {
    "*thumb_movdf_insn",
    (const PTR) output_199,
    0,
    &operand_data[318],
    2,
    0,
    6,
    3
  },
  {
    "*movxf_hard_insn",
    (const PTR) output_200,
    0,
    &operand_data[320],
    2,
    0,
    7,
    3
  },
  {
    "*ldmsi_postinc4",
    "ldm%?ia\t%1!, {%3, %4, %5, %6}",
    0,
    &operand_data[322],
    7,
    4,
    1,
    1
  },
  {
    "*ldmsi_postinc3",
    "ldm%?ia\t%1!, {%3, %4, %5}",
    0,
    &operand_data[322],
    6,
    3,
    1,
    1
  },
  {
    "*ldmsi_postinc2",
    "ldm%?ia\t%1!, {%3, %4}",
    0,
    &operand_data[322],
    5,
    2,
    1,
    1
  },
  {
    "*ldmsi4",
    "ldm%?ia\t%1, {%2, %3, %4, %5}",
    0,
    &operand_data[329],
    6,
    3,
    1,
    1
  },
  {
    "*ldmsi3",
    "ldm%?ia\t%1, {%2, %3, %4}",
    0,
    &operand_data[329],
    5,
    2,
    1,
    1
  },
  {
    "*ldmsi2",
    "ldm%?ia\t%1, {%2, %3}",
    0,
    &operand_data[329],
    4,
    1,
    1,
    1
  },
  {
    "*stmsi_postinc4",
    "stm%?ia\t%1!, {%3, %4, %5, %6}",
    0,
    &operand_data[335],
    7,
    4,
    1,
    1
  },
  {
    "*stmsi_postinc3",
    "stm%?ia\t%1!, {%3, %4, %5}",
    0,
    &operand_data[335],
    6,
    3,
    1,
    1
  },
  {
    "*stmsi_postinc2",
    "stm%?ia\t%1!, {%3, %4}",
    0,
    &operand_data[335],
    5,
    2,
    1,
    1
  },
  {
    "*stmsi4",
    "stm%?ia\t%1, {%2, %3, %4, %5}",
    0,
    &operand_data[342],
    6,
    3,
    1,
    1
  },
  {
    "*stmsi3",
    "stm%?ia\t%1, {%2, %3, %4}",
    0,
    &operand_data[342],
    5,
    2,
    1,
    1
  },
  {
    "*stmsi2",
    "stm%?ia\t%1, {%2, %3}",
    0,
    &operand_data[342],
    4,
    1,
    1,
    1
  },
  {
    "movmem12b",
    (const PTR) output_213,
    (insn_gen_fn) gen_movmem12b,
    &operand_data[348],
    7,
    6,
    1,
    3
  },
  {
    "movmem8b",
    (const PTR) output_214,
    (insn_gen_fn) gen_movmem8b,
    &operand_data[348],
    6,
    4,
    1,
    3
  },
  {
    "cbranchsi4",
    (const PTR) output_215,
    (insn_gen_fn) gen_cbranchsi4,
    &operand_data[355],
    4,
    0,
    2,
    3
  },
  {
    "*negated_cbranchsi4",
    (const PTR) output_216,
    0,
    &operand_data[359],
    4,
    0,
    1,
    3
  },
  {
    "*arm_cmpsi_insn",
    (const PTR) output_217,
    0,
    &operand_data[17],
    2,
    0,
    2,
    2
  },
  {
    "*cmpsi_shiftsi",
    "cmp%?\t%0, %1%S3",
    0,
    &operand_data[173],
    4,
    0,
    1,
    1
  },
  {
    "*cmpsi_shiftsi_swp",
    "cmp%?\t%0, %1%S3",
    0,
    &operand_data[363],
    4,
    0,
    1,
    1
  },
  {
    "*cmpsi_neg_shiftsi",
    "cmn%?\t%0, %1%S3",
    0,
    &operand_data[173],
    4,
    0,
    1,
    1
  },
  {
    "*cmpsf_insn",
    (const PTR) output_221,
    0,
    &operand_data[367],
    2,
    0,
    2,
    2
  },
  {
    "*cmpdf_insn",
    (const PTR) output_222,
    0,
    &operand_data[369],
    2,
    0,
    2,
    2
  },
  {
    "*cmpesfdf_df",
    (const PTR) output_223,
    0,
    &operand_data[38],
    2,
    0,
    2,
    2
  },
  {
    "*cmpdf_esfdf",
    "cmf%?\t%0, %1",
    0,
    &operand_data[41],
    2,
    0,
    1,
    1
  },
  {
    "*cmpxf_insn",
    (const PTR) output_225,
    0,
    &operand_data[47],
    2,
    0,
    2,
    2
  },
  {
    "*cmpsf_trap",
    (const PTR) output_226,
    0,
    &operand_data[367],
    2,
    0,
    2,
    2
  },
  {
    "*cmpdf_trap",
    (const PTR) output_227,
    0,
    &operand_data[369],
    2,
    0,
    2,
    2
  },
  {
    "*cmp_esfdf_df_trap",
    (const PTR) output_228,
    0,
    &operand_data[38],
    2,
    0,
    2,
    2
  },
  {
    "*cmp_df_esfdf_trap",
    "cmf%?e\t%0, %1",
    0,
    &operand_data[41],
    2,
    0,
    1,
    1
  },
  {
    "*cmpxf_trap",
    (const PTR) output_230,
    0,
    &operand_data[47],
    2,
    0,
    2,
    2
  },
  {
    "*deleted_compare",
    "\t%@ deleted compare",
    0,
    &operand_data[73],
    1,
    1,
    0,
    1
  },
  {
    "*arm_buneq",
    (const PTR) output_232,
    0,
    &operand_data[371],
    2,
    0,
    0,
    3
  },
  {
    "*arm_bltgt",
    (const PTR) output_233,
    0,
    &operand_data[371],
    2,
    0,
    0,
    3
  },
  {
    "*arm_cond_branch",
    (const PTR) output_234,
    0,
    &operand_data[373],
    3,
    0,
    0,
    3
  },
  {
    "*arm_buneq_reversed",
    (const PTR) output_235,
    0,
    &operand_data[371],
    2,
    0,
    0,
    3
  },
  {
    "*arm_bltgt_reversed",
    (const PTR) output_236,
    0,
    &operand_data[371],
    2,
    0,
    0,
    3
  },
  {
    "*arm_cond_branch_reversed",
    (const PTR) output_237,
    0,
    &operand_data[373],
    3,
    0,
    0,
    3
  },
  {
    "*mov_scc",
    "mov%D1\t%0, #0\n\tmov%d1\t%0, #1",
    0,
    &operand_data[376],
    3,
    0,
    1,
    1
  },
  {
    "*mov_negscc",
    "mov%D1\t%0, #0\n\tmvn%d1\t%0, #0",
    0,
    &operand_data[376],
    3,
    0,
    1,
    1
  },
  {
    "*mov_notscc",
    "mov%D1\t%0, #0\n\tmvn%d1\t%0, #1",
    0,
    &operand_data[376],
    3,
    0,
    1,
    1
  },
  {
    "*movsicc_insn",
    (const PTR) output_241,
    0,
    &operand_data[379],
    5,
    0,
    8,
    2
  },
  {
    "*movsfcc_hard_insn",
    (const PTR) output_242,
    0,
    &operand_data[384],
    5,
    0,
    8,
    2
  },
  {
    "*movsfcc_soft_insn",
    (const PTR) output_243,
    0,
    &operand_data[389],
    5,
    0,
    2,
    2
  },
  {
    "*movdfcc_insn",
    (const PTR) output_244,
    0,
    &operand_data[394],
    5,
    0,
    8,
    2
  },
  {
    "*arm_jump",
    (const PTR) output_245,
    0,
    &operand_data[285],
    1,
    0,
    0,
    3
  },
  {
    "*thumb_jump",
    (const PTR) output_246,
    0,
    &operand_data[285],
    1,
    0,
    0,
    3
  },
  {
    "*call_reg",
    (const PTR) output_247,
    0,
    &operand_data[399],
    3,
    0,
    1,
    3
  },
  {
    "*call_mem",
    (const PTR) output_248,
    0,
    &operand_data[402],
    3,
    0,
    1,
    3
  },
  {
    "*call_indirect",
    (const PTR) output_249,
    0,
    &operand_data[405],
    3,
    0,
    1,
    3
  },
  {
    "*call_value_indirect",
    (const PTR) output_250,
    0,
    &operand_data[408],
    4,
    0,
    1,
    3
  },
  {
    "*call_value_reg",
    (const PTR) output_251,
    0,
    &operand_data[412],
    4,
    0,
    2,
    3
  },
  {
    "*call_value_mem",
    (const PTR) output_252,
    0,
    &operand_data[416],
    4,
    0,
    2,
    3
  },
  {
    "*call_symbol",
    (const PTR) output_253,
    0,
    &operand_data[420],
    3,
    0,
    1,
    3
  },
  {
    "*call_value_symbol",
    (const PTR) output_254,
    0,
    &operand_data[423],
    4,
    0,
    2,
    3
  },
  {
    "*call_insn",
    "bl\t%a0",
    0,
    &operand_data[427],
    3,
    0,
    1,
    1
  },
  {
    "*call_value_insn",
    "bl\t%a1",
    0,
    &operand_data[430],
    4,
    0,
    1,
    1
  },
  {
    "*sibcall_insn",
    (const PTR) output_257,
    0,
    &operand_data[420],
    3,
    0,
    1,
    3
  },
  {
    "*sibcall_value_insn",
    (const PTR) output_258,
    0,
    &operand_data[434],
    4,
    0,
    2,
    3
  },
  {
    "return",
    (const PTR) output_259,
    (insn_gen_fn) gen_return,
    &operand_data[0],
    0,
    0,
    0,
    3
  },
  {
    "*cond_return",
    (const PTR) output_260,
    0,
    &operand_data[374],
    2,
    0,
    0,
    3
  },
  {
    "*cond_return_inverted",
    (const PTR) output_261,
    0,
    &operand_data[374],
    2,
    0,
    0,
    3
  },
  {
    "blockage",
    "",
    (insn_gen_fn) gen_blockage,
    &operand_data[0],
    0,
    0,
    0,
    1
  },
  {
    "casesi_internal",
    (const PTR) output_263,
    (insn_gen_fn) gen_casesi_internal,
    &operand_data[438],
    4,
    2,
    1,
    3
  },
  {
    "*arm_indirect_jump",
    "mov%?\t%|pc, %0\t%@ indirect register jump",
    0,
    &operand_data[20],
    1,
    0,
    1,
    1
  },
  {
    "*load_indirect_jump",
    "ldr%?\t%|pc, %0\t%@ indirect memory jump",
    0,
    &operand_data[402],
    1,
    0,
    1,
    1
  },
  {
    "*thumb_indirect_jump",
    "mov\tpc, %0",
    0,
    &operand_data[405],
    1,
    0,
    1,
    1
  },
  {
    "nop",
    (const PTR) output_267,
    (insn_gen_fn) gen_nop,
    &operand_data[0],
    0,
    0,
    0,
    3
  },
  {
    "*arith_shiftsi",
    "%i1%?\t%0, %2, %4%S3",
    0,
    &operand_data[442],
    6,
    0,
    1,
    1
  },
  {
    "*arith_shiftsi_compare0",
    "%i1%?s\t%0, %2, %4%S3",
    0,
    &operand_data[442],
    6,
    1,
    1,
    1
  },
  {
    "*arith_shiftsi_compare0_scratch",
    "%i1%?s\t%0, %2, %4%S3",
    0,
    &operand_data[448],
    6,
    0,
    1,
    1
  },
  {
    "*sub_shiftsi",
    "sub%?\t%0, %1, %3%S2",
    0,
    &operand_data[454],
    5,
    0,
    1,
    1
  },
  {
    "*sub_shiftsi_compare0",
    "sub%?s\t%0, %1, %3%S2",
    0,
    &operand_data[454],
    5,
    2,
    1,
    1
  },
  {
    "*sub_shiftsi_compare0_scratch",
    "sub%?s\t%0, %1, %3%S2",
    0,
    &operand_data[459],
    5,
    0,
    1,
    1
  },
  {
    "*reload_mulsi3",
    (const PTR) output_274,
    0,
    &operand_data[464],
    6,
    0,
    1,
    3
  },
  {
    "*reload_mulsi_compare0",
    (const PTR) output_275,
    0,
    &operand_data[470],
    6,
    3,
    1,
    3
  },
  {
    "*reload_mulsi_compare0_scratch",
    (const PTR) output_276,
    0,
    &operand_data[476],
    6,
    0,
    1,
    3
  },
  {
    "*reload_muladdsi",
    (const PTR) output_277,
    0,
    &operand_data[482],
    5,
    0,
    2,
    3
  },
  {
    "*reload_muladdsi_compare0",
    (const PTR) output_278,
    0,
    &operand_data[487],
    5,
    4,
    1,
    3
  },
  {
    "*reload_muladdsi_compare0_scratch",
    (const PTR) output_279,
    0,
    &operand_data[492],
    5,
    0,
    1,
    3
  },
  {
    "*and_scc",
    "mov%D1\t%0, #0\n\tand%d1\t%0, %2, #1",
    0,
    &operand_data[497],
    4,
    0,
    1,
    1
  },
  {
    "*ior_scc",
    (const PTR) output_281,
    0,
    &operand_data[70],
    4,
    0,
    2,
    2
  },
  {
    "*compare_scc",
    (const PTR) output_282,
    0,
    &operand_data[501],
    4,
    0,
    2,
    3
  },
  {
    "*cond_move",
    (const PTR) output_283,
    0,
    &operand_data[505],
    6,
    0,
    3,
    3
  },
  {
    "*cond_arith",
    (const PTR) output_284,
    0,
    &operand_data[511],
    6,
    0,
    2,
    3
  },
  {
    "*cond_sub",
    (const PTR) output_285,
    0,
    &operand_data[511],
    5,
    0,
    2,
    3
  },
  {
    "*cmp_ite0",
    (const PTR) output_286,
    0,
    &operand_data[517],
    7,
    0,
    4,
    3
  },
  {
    "*cmp_ite1",
    (const PTR) output_287,
    0,
    &operand_data[517],
    7,
    0,
    4,
    3
  },
  {
    "*cmp_and",
    (const PTR) output_288,
    0,
    &operand_data[517],
    7,
    0,
    4,
    3
  },
  {
    "*cmp_ior",
    (const PTR) output_289,
    0,
    &operand_data[517],
    7,
    0,
    4,
    3
  },
  {
    "*negscc",
    (const PTR) output_290,
    0,
    &operand_data[524],
    4,
    0,
    1,
    3
  },
  {
    "movcond",
    (const PTR) output_291,
    (insn_gen_fn) gen_movcond,
    &operand_data[528],
    6,
    0,
    3,
    3
  },
  {
    "*ifcompare_plus_move",
    "#",
    0,
    &operand_data[534],
    7,
    0,
    2,
    1
  },
  {
    "*if_plus_move",
    (const PTR) output_293,
    0,
    &operand_data[541],
    6,
    0,
    4,
    2
  },
  {
    "*ifcompare_move_plus",
    "#",
    0,
    &operand_data[534],
    7,
    0,
    2,
    1
  },
  {
    "*if_move_plus",
    (const PTR) output_295,
    0,
    &operand_data[541],
    6,
    0,
    4,
    2
  },
  {
    "*ifcompare_arith_arith",
    "#",
    0,
    &operand_data[547],
    10,
    0,
    1,
    1
  },
  {
    "*if_arith_arith",
    "%I6%d5\t%0, %1, %2\n\t%I7%D5\t%0, %3, %4",
    0,
    &operand_data[557],
    9,
    0,
    1,
    1
  },
  {
    "*ifcompare_arith_move",
    (const PTR) output_298,
    0,
    &operand_data[566],
    8,
    0,
    2,
    3
  },
  {
    "*if_arith_move",
    (const PTR) output_299,
    0,
    &operand_data[574],
    7,
    0,
    2,
    2
  },
  {
    "*ifcompare_move_arith",
    (const PTR) output_300,
    0,
    &operand_data[581],
    8,
    0,
    2,
    3
  },
  {
    "*if_move_arith",
    (const PTR) output_301,
    0,
    &operand_data[574],
    7,
    0,
    2,
    2
  },
  {
    "*ifcompare_move_not",
    "#",
    0,
    &operand_data[589],
    6,
    0,
    2,
    1
  },
  {
    "*if_move_not",
    (const PTR) output_303,
    0,
    &operand_data[595],
    5,
    0,
    3,
    2
  },
  {
    "*ifcompare_not_move",
    "#",
    0,
    &operand_data[589],
    6,
    0,
    2,
    1
  },
  {
    "*if_not_move",
    (const PTR) output_305,
    0,
    &operand_data[595],
    5,
    0,
    3,
    2
  },
  {
    "*ifcompare_shift_move",
    "#",
    0,
    &operand_data[600],
    8,
    0,
    2,
    1
  },
  {
    "*if_shift_move",
    (const PTR) output_307,
    0,
    &operand_data[608],
    7,
    0,
    3,
    2
  },
  {
    "*ifcompare_move_shift",
    "#",
    0,
    &operand_data[600],
    8,
    0,
    2,
    1
  },
  {
    "*if_move_shift",
    (const PTR) output_309,
    0,
    &operand_data[608],
    7,
    0,
    3,
    2
  },
  {
    "*ifcompare_shift_shift",
    "#",
    0,
    &operand_data[615],
    10,
    0,
    1,
    1
  },
  {
    "*if_shift_shift",
    "mov%d5\t%0, %1%S6\n\tmov%D5\t%0, %3%S7",
    0,
    &operand_data[625],
    9,
    0,
    1,
    1
  },
  {
    "*ifcompare_not_arith",
    "#",
    0,
    &operand_data[634],
    8,
    0,
    1,
    1
  },
  {
    "*if_not_arith",
    "mvn%d5\t%0, %1\n\t%I6%D5\t%0, %2, %3",
    0,
    &operand_data[642],
    7,
    0,
    1,
    1
  },
  {
    "*ifcompare_arith_not",
    "#",
    0,
    &operand_data[634],
    8,
    0,
    1,
    1
  },
  {
    "*if_arith_not",
    "mvn%D5\t%0, %1\n\t%I6%d5\t%0, %2, %3",
    0,
    &operand_data[642],
    7,
    0,
    1,
    1
  },
  {
    "*ifcompare_neg_move",
    "#",
    0,
    &operand_data[589],
    6,
    0,
    2,
    1
  },
  {
    "*if_neg_move",
    (const PTR) output_317,
    0,
    &operand_data[595],
    5,
    0,
    3,
    2
  },
  {
    "*ifcompare_move_neg",
    "#",
    0,
    &operand_data[589],
    6,
    0,
    2,
    1
  },
  {
    "*if_move_neg",
    (const PTR) output_319,
    0,
    &operand_data[595],
    5,
    0,
    3,
    2
  },
  {
    "*arith_adjacentmem",
    (const PTR) output_320,
    0,
    &operand_data[649],
    5,
    0,
    1,
    3
  },
  {
    "*strqi_preinc",
    "str%?b\t%3, [%0, %2]!",
    0,
    &operand_data[654],
    4,
    2,
    1,
    1
  },
  {
    "*strqi_predec",
    "str%?b\t%3, [%0, -%2]!",
    0,
    &operand_data[658],
    4,
    2,
    1,
    1
  },
  {
    "*loadqi_preinc",
    "ldr%?b\t%3, [%0, %2]!",
    0,
    &operand_data[662],
    4,
    2,
    1,
    1
  },
  {
    "*loadqi_predec",
    "ldr%?b\t%3, [%0, -%2]!",
    0,
    &operand_data[666],
    4,
    2,
    1,
    1
  },
  {
    "*loadqisi_preinc",
    "ldr%?b\t%3, [%0, %2]!\t%@ z_extendqisi",
    0,
    &operand_data[670],
    4,
    2,
    1,
    1
  },
  {
    "*loadqisi_predec",
    "ldr%?b\t%3, [%0, -%2]!\t%@ z_extendqisi",
    0,
    &operand_data[673],
    4,
    2,
    1,
    1
  },
  {
    "*strsi_preinc",
    "str%?\t%3, [%0, %2]!",
    0,
    &operand_data[676],
    4,
    2,
    1,
    1
  },
  {
    "*strsi_predec",
    "str%?\t%3, [%0, -%2]!",
    0,
    &operand_data[680],
    4,
    2,
    1,
    1
  },
  {
    "*loadsi_preinc",
    "ldr%?\t%3, [%0, %2]!",
    0,
    &operand_data[670],
    4,
    2,
    1,
    1
  },
  {
    "*loadsi_predec",
    "ldr%?\t%3, [%0, -%2]!",
    0,
    &operand_data[673],
    4,
    2,
    1,
    1
  },
  {
    "*loadhi_preinc",
    "ldr%?\t%3, [%0, %2]!\t%@ loadhi",
    0,
    &operand_data[684],
    4,
    2,
    1,
    1
  },
  {
    "*loadhi_predec",
    "ldr%?\t%3, [%0, -%2]!\t%@ loadhi",
    0,
    &operand_data[688],
    4,
    2,
    1,
    1
  },
  {
    "*strqi_shiftpreinc",
    "str%?b\t%5, [%0, %3%S2]!",
    0,
    &operand_data[692],
    6,
    2,
    1,
    1
  },
  {
    "*strqi_shiftpredec",
    "str%?b\t%5, [%0, -%3%S2]!",
    0,
    &operand_data[692],
    6,
    2,
    1,
    1
  },
  {
    "*loadqi_shiftpreinc",
    "ldr%?b\t%5, [%0, %3%S2]!",
    0,
    &operand_data[698],
    6,
    2,
    1,
    1
  },
  {
    "*loadqi_shiftpredec",
    "ldr%?b\t%5, [%0, -%3%S2]!",
    0,
    &operand_data[698],
    6,
    2,
    1,
    1
  },
  {
    "*strsi_shiftpreinc",
    "str%?\t%5, [%0, %3%S2]!",
    0,
    &operand_data[704],
    6,
    2,
    1,
    1
  },
  {
    "*strsi_shiftpredec",
    "str%?\t%5, [%0, -%3%S2]!",
    0,
    &operand_data[704],
    6,
    2,
    1,
    1
  },
  {
    "*loadsi_shiftpreinc",
    "ldr%?\t%5, [%0, %3%S2]!",
    0,
    &operand_data[710],
    6,
    2,
    1,
    1
  },
  {
    "*loadsi_shiftpredec",
    "ldr%?\t%5, [%0, -%3%S2]!",
    0,
    &operand_data[710],
    6,
    2,
    1,
    1
  },
  {
    "*loadhi_shiftpreinc",
    "ldr%?\t%5, [%0, %3%S2]!\t%@ loadhi",
    0,
    &operand_data[715],
    6,
    2,
    1,
    1
  },
  {
    "*loadhi_shiftpredec",
    "ldr%?\t%5, [%0, -%3%S2]!\t%@ loadhi",
    0,
    &operand_data[715],
    6,
    2,
    1,
    1
  },
  {
    "sibcall_epilogue",
    (const PTR) output_343,
    (insn_gen_fn) gen_sibcall_epilogue,
    &operand_data[0],
    0,
    0,
    0,
    3
  },
  {
    "*epilogue_insns",
    (const PTR) output_344,
    0,
    &operand_data[0],
    0,
    0,
    0,
    3
  },
  {
    "*cond_move_not",
    (const PTR) output_345,
    0,
    &operand_data[721],
    5,
    0,
    2,
    2
  },
  {
    "*sign_extract_onebit",
    (const PTR) output_346,
    0,
    &operand_data[156],
    3,
    0,
    1,
    3
  },
  {
    "*not_signextract_onebit",
    (const PTR) output_347,
    0,
    &operand_data[156],
    3,
    0,
    1,
    3
  },
  {
    "*push_multi",
    (const PTR) output_348,
    0,
    &operand_data[726],
    3,
    0,
    1,
    3
  },
  {
    "*push_fp_multi",
    (const PTR) output_349,
    0,
    &operand_data[729],
    3,
    0,
    1,
    3
  },
  {
    "align_4",
    (const PTR) output_350,
    (insn_gen_fn) gen_align_4,
    &operand_data[0],
    0,
    0,
    0,
    3
  },
  {
    "consttable_end",
    (const PTR) output_351,
    (insn_gen_fn) gen_consttable_end,
    &operand_data[0],
    0,
    0,
    0,
    3
  },
  {
    "consttable_1",
    (const PTR) output_352,
    (insn_gen_fn) gen_consttable_1,
    &operand_data[285],
    1,
    0,
    0,
    3
  },
  {
    "consttable_2",
    (const PTR) output_353,
    (insn_gen_fn) gen_consttable_2,
    &operand_data[285],
    1,
    0,
    0,
    3
  },
  {
    "consttable_4",
    (const PTR) output_354,
    (insn_gen_fn) gen_consttable_4,
    &operand_data[285],
    1,
    0,
    0,
    3
  },
  {
    "consttable_8",
    (const PTR) output_355,
    (insn_gen_fn) gen_consttable_8,
    &operand_data[285],
    1,
    0,
    0,
    3
  },
  {
    "tablejump",
    "mov	pc, %0",
    (insn_gen_fn) gen_tablejump,
    &operand_data[405],
    2,
    0,
    1,
    1
  },
  {
    "clz",
    "clz\t%0, %1",
    (insn_gen_fn) gen_clz,
    &operand_data[19],
    2,
    0,
    1,
    1
  },
  {
    "prologue_use",
    "%@ %0 needed for prologue",
    (insn_gen_fn) gen_prologue_use,
    &operand_data[732],
    1,
    0,
    0,
    1
  },
  {
    "adddi3",
    0,
    (insn_gen_fn) gen_adddi3,
    &operand_data[733],
    3,
    0,
    0,
    0
  },
  {
    "adddi3+1",
    0,
    0,
    &operand_data[733],
    3,
    0,
    0,
    0
  },
  {
    "adddi3+2",
    0,
    0,
    &operand_data[734],
    3,
    0,
    0,
    0
  },
  {
    "addsi3-1",
    0,
    0,
    &operand_data[734],
    3,
    0,
    0,
    0
  },
  {
    "addsi3",
    0,
    (insn_gen_fn) gen_addsi3,
    &operand_data[736],
    3,
    0,
    0,
    0
  },
  {
    "addsi3+1",
    0,
    0,
    &operand_data[739],
    4,
    0,
    0,
    0
  },
  {
    "addsi3+2",
    0,
    0,
    &operand_data[736],
    3,
    0,
    0,
    0
  },
  {
    "subdi3-1",
    0,
    0,
    &operand_data[743],
    3,
    0,
    0,
    0
  },
  {
    "subdi3",
    0,
    (insn_gen_fn) gen_subdi3,
    &operand_data[733],
    3,
    0,
    0,
    0
  },
  {
    "subsi3",
    0,
    (insn_gen_fn) gen_subsi3,
    &operand_data[737],
    3,
    0,
    0,
    0
  },
  {
    "subsi3+1",
    0,
    0,
    &operand_data[737],
    3,
    0,
    0,
    0
  },
  {
    "mulsi3-1",
    0,
    0,
    &operand_data[746],
    4,
    0,
    0,
    0
  },
  {
    "mulsi3",
    0,
    (insn_gen_fn) gen_mulsi3,
    &operand_data[750],
    3,
    0,
    0,
    0
  },
  {
    "mulsi3+1",
    0,
    0,
    &operand_data[753],
    7,
    0,
    0,
    0
  },
  {
    "mulsi3+2",
    0,
    0,
    &operand_data[760],
    7,
    0,
    0,
    0
  },
  {
    "mulsi3+3",
    0,
    0,
    &operand_data[734],
    3,
    0,
    0,
    0
  },
  {
    "andsi3-2",
    0,
    0,
    &operand_data[734],
    3,
    0,
    0,
    0
  },
  {
    "andsi3-1",
    0,
    0,
    &operand_data[734],
    3,
    0,
    0,
    0
  },
  {
    "andsi3",
    0,
    (insn_gen_fn) gen_andsi3,
    &operand_data[736],
    3,
    0,
    0,
    0
  },
  {
    "andsi3+1",
    0,
    0,
    &operand_data[736],
    3,
    0,
    0,
    0
  },
  {
    "insv",
    0,
    (insn_gen_fn) gen_insv,
    &operand_data[767],
    4,
    0,
    0,
    0
  },
  {
    "insv+1",
    0,
    0,
    &operand_data[733],
    3,
    0,
    0,
    0
  },
  {
    "insv+2",
    0,
    0,
    &operand_data[734],
    3,
    0,
    0,
    0
  },
  {
    "iorsi3-1",
    0,
    0,
    &operand_data[734],
    3,
    0,
    0,
    0
  },
  {
    "iorsi3",
    0,
    (insn_gen_fn) gen_iorsi3,
    &operand_data[736],
    3,
    0,
    0,
    0
  },
  {
    "iorsi3+1",
    0,
    0,
    &operand_data[736],
    3,
    0,
    0,
    0
  },
  {
    "xorsi3-1",
    0,
    0,
    &operand_data[739],
    4,
    0,
    0,
    0
  },
  {
    "xorsi3",
    0,
    (insn_gen_fn) gen_xorsi3,
    &operand_data[771],
    3,
    0,
    0,
    0
  },
  {
    "xorsi3+1",
    0,
    0,
    &operand_data[774],
    5,
    0,
    0,
    0
  },
  {
    "ashlsi3",
    0,
    (insn_gen_fn) gen_ashlsi3,
    &operand_data[771],
    3,
    0,
    0,
    0
  },
  {
    "ashrsi3",
    0,
    (insn_gen_fn) gen_ashrsi3,
    &operand_data[771],
    3,
    0,
    0,
    0
  },
  {
    "lshrsi3",
    0,
    (insn_gen_fn) gen_lshrsi3,
    &operand_data[771],
    3,
    0,
    0,
    0
  },
  {
    "rotlsi3",
    0,
    (insn_gen_fn) gen_rotlsi3,
    &operand_data[736],
    3,
    0,
    0,
    0
  },
  {
    "rotrsi3",
    0,
    (insn_gen_fn) gen_rotrsi3,
    &operand_data[771],
    3,
    0,
    0,
    0
  },
  {
    "extzv",
    0,
    (insn_gen_fn) gen_extzv,
    &operand_data[779],
    4,
    2,
    0,
    0
  },
  {
    "negdi2",
    0,
    (insn_gen_fn) gen_negdi2,
    &operand_data[733],
    2,
    0,
    0,
    0
  },
  {
    "negsi2",
    0,
    (insn_gen_fn) gen_negsi2,
    &operand_data[22],
    2,
    0,
    0,
    0
  },
  {
    "negsi2+1",
    0,
    0,
    &operand_data[733],
    2,
    0,
    0,
    0
  },
  {
    "one_cmplsi2",
    0,
    (insn_gen_fn) gen_one_cmplsi2,
    &operand_data[22],
    2,
    0,
    0,
    0
  },
  {
    "zero_extendhisi2",
    0,
    (insn_gen_fn) gen_zero_extendhisi2,
    &operand_data[783],
    2,
    2,
    0,
    0
  },
  {
    "zero_extendhisi2+1",
    0,
    0,
    &operand_data[785],
    3,
    0,
    0,
    0
  },
  {
    "zero_extendqisi2-1",
    0,
    0,
    &operand_data[785],
    5,
    0,
    0,
    0
  },
  {
    "zero_extendqisi2",
    0,
    (insn_gen_fn) gen_zero_extendqisi2,
    &operand_data[789],
    2,
    0,
    0,
    0
  },
  {
    "zero_extendqisi2+1",
    0,
    0,
    &operand_data[791],
    3,
    0,
    0,
    0
  },
  {
    "extendhisi2",
    0,
    (insn_gen_fn) gen_extendhisi2,
    &operand_data[783],
    2,
    2,
    0,
    0
  },
  {
    "extendhisi2_mem",
    0,
    (insn_gen_fn) gen_extendhisi2_mem,
    &operand_data[794],
    2,
    7,
    0,
    0
  },
  {
    "extendhisi2_mem+1",
    0,
    0,
    &operand_data[785],
    3,
    0,
    0,
    0
  },
  {
    "extendqihi2-1",
    0,
    0,
    &operand_data[785],
    5,
    0,
    0,
    0
  },
  {
    "extendqihi2",
    0,
    (insn_gen_fn) gen_extendqihi2,
    &operand_data[796],
    2,
    2,
    0,
    0
  },
  {
    "extendqihi2+1",
    0,
    0,
    &operand_data[798],
    2,
    0,
    0,
    0
  },
  {
    "extendqisi2",
    0,
    (insn_gen_fn) gen_extendqisi2,
    &operand_data[800],
    2,
    2,
    0,
    0
  },
  {
    "extendqisi2+1",
    0,
    0,
    &operand_data[802],
    2,
    0,
    0,
    0
  },
  {
    "movdi",
    0,
    (insn_gen_fn) gen_movdi,
    &operand_data[804],
    2,
    0,
    0,
    0
  },
  {
    "movsi",
    0,
    (insn_gen_fn) gen_movsi,
    &operand_data[768],
    2,
    0,
    0,
    0
  },
  {
    "movsi+1",
    0,
    0,
    &operand_data[740],
    2,
    0,
    0,
    0
  },
  {
    "movsi+2",
    0,
    0,
    &operand_data[780],
    2,
    0,
    0,
    0
  },
  {
    "movaddr-1",
    0,
    0,
    &operand_data[780],
    2,
    0,
    0,
    0
  },
  {
    "movaddr",
    0,
    (insn_gen_fn) gen_movaddr,
    &operand_data[806],
    2,
    0,
    0,
    0
  },
  {
    "pic_load_addr_based",
    0,
    (insn_gen_fn) gen_pic_load_addr_based,
    &operand_data[284],
    2,
    1,
    1,
    0
  },
  {
    "builtin_setjmp_receiver",
    0,
    (insn_gen_fn) gen_builtin_setjmp_receiver,
    &operand_data[285],
    1,
    0,
    0,
    0
  },
  {
    "storehi",
    0,
    (insn_gen_fn) gen_storehi,
    &operand_data[400],
    2,
    4,
    0,
    0
  },
  {
    "storehi_bigend",
    0,
    (insn_gen_fn) gen_storehi_bigend,
    &operand_data[400],
    2,
    4,
    0,
    0
  },
  {
    "storeinthi",
    0,
    (insn_gen_fn) gen_storeinthi,
    &operand_data[400],
    2,
    2,
    0,
    0
  },
  {
    "storehi_single_op",
    0,
    (insn_gen_fn) gen_storehi_single_op,
    &operand_data[808],
    2,
    0,
    0,
    0
  },
  {
    "movhi",
    0,
    (insn_gen_fn) gen_movhi,
    &operand_data[809],
    2,
    0,
    0,
    0
  },
  {
    "movhi_bytes",
    0,
    (insn_gen_fn) gen_movhi_bytes,
    &operand_data[794],
    2,
    5,
    0,
    0
  },
  {
    "movhi_bigend",
    0,
    (insn_gen_fn) gen_movhi_bigend,
    &operand_data[811],
    2,
    4,
    0,
    0
  },
  {
    "reload_outhi",
    0,
    (insn_gen_fn) gen_reload_outhi,
    &operand_data[813],
    3,
    0,
    1,
    0
  },
  {
    "reload_inhi",
    0,
    (insn_gen_fn) gen_reload_inhi,
    &operand_data[816],
    3,
    0,
    1,
    0
  },
  {
    "movqi",
    0,
    (insn_gen_fn) gen_movqi,
    &operand_data[819],
    2,
    0,
    0,
    0
  },
  {
    "movsf",
    0,
    (insn_gen_fn) gen_movsf,
    &operand_data[821],
    2,
    0,
    0,
    0
  },
  {
    "movsf+1",
    0,
    0,
    &operand_data[823],
    2,
    0,
    0,
    0
  },
  {
    "movdf",
    0,
    (insn_gen_fn) gen_movdf,
    &operand_data[825],
    2,
    0,
    0,
    0
  },
  {
    "reload_outdf",
    0,
    (insn_gen_fn) gen_reload_outdf,
    &operand_data[827],
    3,
    0,
    1,
    0
  },
  {
    "movxf",
    0,
    (insn_gen_fn) gen_movxf,
    &operand_data[830],
    2,
    0,
    0,
    0
  },
  {
    "load_multiple",
    0,
    (insn_gen_fn) gen_load_multiple,
    &operand_data[0],
    0,
    1,
    0,
    0
  },
  {
    "store_multiple",
    0,
    (insn_gen_fn) gen_store_multiple,
    &operand_data[0],
    0,
    1,
    0,
    0
  },
  {
    "movstrqi",
    0,
    (insn_gen_fn) gen_movstrqi,
    &operand_data[832],
    4,
    0,
    0,
    0
  },
  {
    "cmpsi",
    0,
    (insn_gen_fn) gen_cmpsi,
    &operand_data[836],
    2,
    0,
    0,
    0
  },
  {
    "cmpsf",
    0,
    (insn_gen_fn) gen_cmpsf,
    &operand_data[838],
    2,
    0,
    0,
    0
  },
  {
    "cmpdf",
    0,
    (insn_gen_fn) gen_cmpdf,
    &operand_data[840],
    2,
    0,
    0,
    0
  },
  {
    "cmpxf",
    0,
    (insn_gen_fn) gen_cmpxf,
    &operand_data[842],
    2,
    0,
    0,
    0
  },
  {
    "beq",
    0,
    (insn_gen_fn) gen_beq,
    &operand_data[285],
    1,
    1,
    0,
    0
  },
  {
    "bne",
    0,
    (insn_gen_fn) gen_bne,
    &operand_data[285],
    1,
    1,
    0,
    0
  },
  {
    "bgt",
    0,
    (insn_gen_fn) gen_bgt,
    &operand_data[285],
    1,
    1,
    0,
    0
  },
  {
    "ble",
    0,
    (insn_gen_fn) gen_ble,
    &operand_data[285],
    1,
    1,
    0,
    0
  },
  {
    "bge",
    0,
    (insn_gen_fn) gen_bge,
    &operand_data[285],
    1,
    1,
    0,
    0
  },
  {
    "blt",
    0,
    (insn_gen_fn) gen_blt,
    &operand_data[285],
    1,
    1,
    0,
    0
  },
  {
    "bgtu",
    0,
    (insn_gen_fn) gen_bgtu,
    &operand_data[285],
    1,
    1,
    0,
    0
  },
  {
    "bleu",
    0,
    (insn_gen_fn) gen_bleu,
    &operand_data[285],
    1,
    1,
    0,
    0
  },
  {
    "bgeu",
    0,
    (insn_gen_fn) gen_bgeu,
    &operand_data[285],
    1,
    1,
    0,
    0
  },
  {
    "bltu",
    0,
    (insn_gen_fn) gen_bltu,
    &operand_data[285],
    1,
    1,
    0,
    0
  },
  {
    "bunordered",
    0,
    (insn_gen_fn) gen_bunordered,
    &operand_data[285],
    1,
    1,
    0,
    0
  },
  {
    "bordered",
    0,
    (insn_gen_fn) gen_bordered,
    &operand_data[285],
    1,
    1,
    0,
    0
  },
  {
    "bungt",
    0,
    (insn_gen_fn) gen_bungt,
    &operand_data[285],
    1,
    1,
    0,
    0
  },
  {
    "bunlt",
    0,
    (insn_gen_fn) gen_bunlt,
    &operand_data[285],
    1,
    1,
    0,
    0
  },
  {
    "bunge",
    0,
    (insn_gen_fn) gen_bunge,
    &operand_data[285],
    1,
    1,
    0,
    0
  },
  {
    "bunle",
    0,
    (insn_gen_fn) gen_bunle,
    &operand_data[285],
    1,
    1,
    0,
    0
  },
  {
    "buneq",
    0,
    (insn_gen_fn) gen_buneq,
    &operand_data[285],
    1,
    1,
    0,
    0
  },
  {
    "bltgt",
    0,
    (insn_gen_fn) gen_bltgt,
    &operand_data[285],
    1,
    1,
    0,
    0
  },
  {
    "seq",
    0,
    (insn_gen_fn) gen_seq,
    &operand_data[19],
    1,
    1,
    1,
    0
  },
  {
    "sne",
    0,
    (insn_gen_fn) gen_sne,
    &operand_data[19],
    1,
    1,
    1,
    0
  },
  {
    "sgt",
    0,
    (insn_gen_fn) gen_sgt,
    &operand_data[19],
    1,
    1,
    1,
    0
  },
  {
    "sle",
    0,
    (insn_gen_fn) gen_sle,
    &operand_data[19],
    1,
    1,
    1,
    0
  },
  {
    "sge",
    0,
    (insn_gen_fn) gen_sge,
    &operand_data[19],
    1,
    1,
    1,
    0
  },
  {
    "slt",
    0,
    (insn_gen_fn) gen_slt,
    &operand_data[19],
    1,
    1,
    1,
    0
  },
  {
    "sgtu",
    0,
    (insn_gen_fn) gen_sgtu,
    &operand_data[19],
    1,
    1,
    1,
    0
  },
  {
    "sleu",
    0,
    (insn_gen_fn) gen_sleu,
    &operand_data[19],
    1,
    1,
    1,
    0
  },
  {
    "sgeu",
    0,
    (insn_gen_fn) gen_sgeu,
    &operand_data[19],
    1,
    1,
    1,
    0
  },
  {
    "sltu",
    0,
    (insn_gen_fn) gen_sltu,
    &operand_data[19],
    1,
    1,
    1,
    0
  },
  {
    "sunordered",
    0,
    (insn_gen_fn) gen_sunordered,
    &operand_data[19],
    1,
    1,
    1,
    0
  },
  {
    "sordered",
    0,
    (insn_gen_fn) gen_sordered,
    &operand_data[19],
    1,
    1,
    1,
    0
  },
  {
    "sungt",
    0,
    (insn_gen_fn) gen_sungt,
    &operand_data[19],
    1,
    1,
    1,
    0
  },
  {
    "sunge",
    0,
    (insn_gen_fn) gen_sunge,
    &operand_data[19],
    1,
    1,
    1,
    0
  },
  {
    "sunlt",
    0,
    (insn_gen_fn) gen_sunlt,
    &operand_data[19],
    1,
    1,
    1,
    0
  },
  {
    "sunle",
    0,
    (insn_gen_fn) gen_sunle,
    &operand_data[19],
    1,
    1,
    1,
    0
  },
  {
    "movsicc",
    0,
    (insn_gen_fn) gen_movsicc,
    &operand_data[844],
    4,
    0,
    0,
    0
  },
  {
    "movsfcc",
    0,
    (insn_gen_fn) gen_movsfcc,
    &operand_data[848],
    4,
    0,
    0,
    0
  },
  {
    "movdfcc",
    0,
    (insn_gen_fn) gen_movdfcc,
    &operand_data[852],
    4,
    0,
    0,
    0
  },
  {
    "jump",
    0,
    (insn_gen_fn) gen_jump,
    &operand_data[285],
    1,
    0,
    0,
    0
  },
  {
    "call",
    0,
    (insn_gen_fn) gen_call,
    &operand_data[856],
    3,
    0,
    0,
    0
  },
  {
    "call_value",
    0,
    (insn_gen_fn) gen_call_value,
    &operand_data[858],
    4,
    0,
    0,
    0
  },
  {
    "sibcall",
    0,
    (insn_gen_fn) gen_sibcall,
    &operand_data[856],
    3,
    0,
    0,
    0
  },
  {
    "sibcall_value",
    0,
    (insn_gen_fn) gen_sibcall_value,
    &operand_data[862],
    4,
    0,
    0,
    0
  },
  {
    "untyped_call",
    0,
    (insn_gen_fn) gen_untyped_call,
    &operand_data[865],
    3,
    0,
    0,
    0
  },
  {
    "casesi",
    0,
    (insn_gen_fn) gen_casesi,
    &operand_data[868],
    5,
    0,
    0,
    0
  },
  {
    "indirect_jump",
    0,
    (insn_gen_fn) gen_indirect_jump,
    &operand_data[22],
    1,
    0,
    0,
    0
  },
  {
    "indirect_jump+1",
    "str%?b\t%2, [%0], %1",
    0,
    &operand_data[873],
    3,
    0,
    1,
    1
  },
  {
    "indirect_jump+2",
    "ldr%?b\t%0, [%1], %2",
    0,
    &operand_data[876],
    3,
    0,
    1,
    1
  },
  {
    "indirect_jump+3",
    "str%?\t%2, [%0], %1",
    0,
    &operand_data[877],
    3,
    0,
    1,
    1
  },
  {
    "indirect_jump+4",
    "ldr%?\t%0, [%1], %2\t%@ loadhi",
    0,
    &operand_data[880],
    3,
    0,
    1,
    1
  },
  {
    "indirect_jump+5",
    "ldr%?\t%0, [%1], %2",
    0,
    &operand_data[883],
    3,
    0,
    1,
    1
  },
  {
    "indirect_jump+6",
    "str%?b\t%2, [%0, %1]!",
    0,
    &operand_data[873],
    3,
    0,
    1,
    1
  },
  {
    "indirect_jump+7",
    "str%?b\t%3, [%2, %0%S4]!",
    0,
    &operand_data[886],
    5,
    0,
    1,
    1
  },
  {
    "indirect_jump+8",
    0,
    0,
    &operand_data[22],
    2,
    0,
    0,
    0
  },
  {
    "prologue-8",
    (const PTR) output_494,
    0,
    &operand_data[891],
    8,
    0,
    1,
    3
  },
  {
    "prologue-7",
    (const PTR) output_495,
    0,
    &operand_data[892],
    6,
    0,
    1,
    3
  },
  {
    "prologue-6",
    (const PTR) output_496,
    0,
    &operand_data[893],
    4,
    0,
    1,
    3
  },
  {
    "prologue-5",
    (const PTR) output_497,
    0,
    &operand_data[899],
    8,
    0,
    1,
    3
  },
  {
    "prologue-4",
    (const PTR) output_498,
    0,
    &operand_data[900],
    6,
    0,
    1,
    3
  },
  {
    "prologue-3",
    (const PTR) output_499,
    0,
    &operand_data[901],
    4,
    0,
    1,
    3
  },
  {
    "prologue-2",
    0,
    0,
    &operand_data[907],
    6,
    0,
    0,
    0
  },
  {
    "prologue-1",
    0,
    0,
    &operand_data[913],
    3,
    0,
    0,
    0
  },
  {
    "prologue",
    0,
    (insn_gen_fn) gen_prologue,
    &operand_data[0],
    0,
    0,
    0,
    0
  },
  {
    "epilogue",
    0,
    (insn_gen_fn) gen_epilogue,
    &operand_data[0],
    0,
    0,
    0,
    0
  },
  {
    "eh_epilogue",
    0,
    (insn_gen_fn) gen_eh_epilogue,
    &operand_data[916],
    3,
    0,
    1,
    0
  },
  {
    "eh_epilogue+1",
    0,
    0,
    &operand_data[919],
    5,
    0,
    0,
    0
  },
  {
    "eh_epilogue+2",
    0,
    0,
    &operand_data[919],
    5,
    0,
    0,
    0
  },
  {
    "ffssi2-2",
    0,
    0,
    &operand_data[919],
    6,
    0,
    0,
    0
  },
  {
    "ffssi2-1",
    0,
    0,
    &operand_data[925],
    6,
    0,
    0,
    0
  },
  {
    "ffssi2",
    0,
    (insn_gen_fn) gen_ffssi2,
    &operand_data[22],
    2,
    0,
    0,
    0
  },
  {
    "ffssi2+1",
    (const PTR) output_510,
    0,
    &operand_data[931],
    5,
    0,
    3,
    2
  },
  {
    "ffssi2+2",
    (const PTR) output_511,
    0,
    &operand_data[936],
    5,
    0,
    2,
    2
  },
  {
    "ffssi2+3",
    (const PTR) output_512,
    0,
    &operand_data[941],
    5,
    0,
    2,
    2
  },
  {
    "ffssi2+4",
    (const PTR) output_513,
    0,
    &operand_data[946],
    5,
    0,
    2,
    2
  },
  {
    "ffssi2+5",
    "adf%?d\t%0, %1, %2",
    0,
    &operand_data[951],
    5,
    0,
    1,
    1
  },
  {
    "ffssi2+6",
    "adf%?d\t%0, %1, %2",
    0,
    &operand_data[956],
    5,
    0,
    1,
    1
  },
  {
    "ffssi2+7",
    (const PTR) output_516,
    0,
    &operand_data[961],
    5,
    0,
    2,
    2
  },
  {
    "ffssi2+8",
    (const PTR) output_517,
    0,
    &operand_data[966],
    5,
    0,
    2,
    2
  },
  {
    "ffssi2+9",
    (const PTR) output_518,
    0,
    &operand_data[971],
    5,
    0,
    2,
    2
  },
  {
    "ffssi2+10",
    "suf%?d\t%0, %1, %2",
    0,
    &operand_data[976],
    5,
    0,
    1,
    1
  },
  {
    "ffssi2+11",
    (const PTR) output_520,
    0,
    &operand_data[981],
    5,
    0,
    2,
    2
  },
  {
    "ffssi2+12",
    "suf%?d\t%0, %1, %2",
    0,
    &operand_data[956],
    5,
    0,
    1,
    1
  },
  {
    "ffssi2+13",
    (const PTR) output_522,
    0,
    &operand_data[986],
    5,
    0,
    2,
    2
  },
  {
    "ffssi2+14",
    "mul%?\t%0, %2, %1",
    0,
    &operand_data[991],
    5,
    0,
    2,
    1
  },
  {
    "ffssi2+15",
    "mla%?\t%0, %2, %1, %3",
    0,
    &operand_data[996],
    6,
    0,
    4,
    1
  },
  {
    "ffssi2+16",
    "smlal%?\t%Q0, %R0, %3, %2",
    0,
    &operand_data[1002],
    6,
    0,
    1,
    1
  },
  {
    "ffssi2+17",
    "smull%?\t%Q0, %R0, %1, %2",
    0,
    &operand_data[1008],
    5,
    0,
    1,
    1
  },
  {
    "ffssi2+18",
    "umull%?\t%Q0, %R0, %1, %2",
    0,
    &operand_data[1008],
    5,
    0,
    1,
    1
  },
  {
    "ffssi2+19",
    "umlal%?\t%Q0, %R0, %3, %2",
    0,
    &operand_data[1002],
    6,
    0,
    1,
    1
  },
  {
    "ffssi2+20",
    "smull%?\t%3, %0, %2, %1",
    0,
    &operand_data[1013],
    6,
    0,
    2,
    1
  },
  {
    "ffssi2+21",
    "umull%?\t%3, %0, %2, %1",
    0,
    &operand_data[1013],
    6,
    0,
    2,
    1
  },
  {
    "ffssi2+22",
    "fml%?s\t%0, %1, %2",
    0,
    &operand_data[1019],
    5,
    0,
    1,
    1
  },
  {
    "ffssi2+23",
    "muf%?d\t%0, %1, %2",
    0,
    &operand_data[1024],
    5,
    0,
    1,
    1
  },
  {
    "ffssi2+24",
    "muf%?d\t%0, %1, %2",
    0,
    &operand_data[976],
    5,
    0,
    1,
    1
  },
  {
    "ffssi2+25",
    "muf%?d\t%0, %1, %2",
    0,
    &operand_data[951],
    5,
    0,
    1,
    1
  },
  {
    "ffssi2+26",
    "muf%?d\t%0, %1, %2",
    0,
    &operand_data[956],
    5,
    0,
    1,
    1
  },
  {
    "ffssi2+27",
    "muf%?e\t%0, %1, %2",
    0,
    &operand_data[1029],
    5,
    0,
    1,
    1
  },
  {
    "ffssi2+28",
    (const PTR) output_537,
    0,
    &operand_data[1034],
    5,
    0,
    2,
    2
  },
  {
    "ffssi2+29",
    (const PTR) output_538,
    0,
    &operand_data[971],
    5,
    0,
    2,
    2
  },
  {
    "ffssi2+30",
    "dvf%?d\t%0, %1, %2",
    0,
    &operand_data[976],
    5,
    0,
    1,
    1
  },
  {
    "ffssi2+31",
    "rdf%?d\t%0, %2, %1",
    0,
    &operand_data[1039],
    5,
    0,
    1,
    1
  },
  {
    "ffssi2+32",
    "dvf%?d\t%0, %1, %2",
    0,
    &operand_data[956],
    5,
    0,
    1,
    1
  },
  {
    "ffssi2+33",
    (const PTR) output_542,
    0,
    &operand_data[986],
    5,
    0,
    2,
    2
  },
  {
    "ffssi2+34",
    "rmf%?s\t%0, %1, %2",
    0,
    &operand_data[1019],
    5,
    0,
    1,
    1
  },
  {
    "ffssi2+35",
    "rmf%?d\t%0, %1, %2",
    0,
    &operand_data[1024],
    5,
    0,
    1,
    1
  },
  {
    "ffssi2+36",
    "rmf%?d\t%0, %1, %2",
    0,
    &operand_data[976],
    5,
    0,
    1,
    1
  },
  {
    "ffssi2+37",
    "rmf%?d\t%0, %1, %2",
    0,
    &operand_data[951],
    5,
    0,
    1,
    1
  },
  {
    "ffssi2+38",
    "rmf%?d\t%0, %1, %2",
    0,
    &operand_data[956],
    5,
    0,
    1,
    1
  },
  {
    "ffssi2+39",
    "rmf%?e\t%0, %1, %2",
    0,
    &operand_data[1029],
    5,
    0,
    1,
    1
  },
  {
    "ffssi2+40",
    (const PTR) output_549,
    0,
    &operand_data[1044],
    5,
    0,
    3,
    2
  },
  {
    "ffssi2+41",
    "#",
    0,
    &operand_data[1049],
    5,
    0,
    2,
    1
  },
  {
    "ffssi2+42",
    (const PTR) output_551,
    0,
    &operand_data[1054],
    5,
    0,
    2,
    2
  },
  {
    "ffssi2+43",
    "#",
    0,
    &operand_data[1059],
    5,
    0,
    2,
    1
  },
  {
    "ffssi2+44",
    "bic%?\t%0, %1, %2",
    0,
    &operand_data[1064],
    5,
    0,
    1,
    1
  },
  {
    "ffssi2+45",
    "bic%?\t%0, %1, %2%S4",
    0,
    &operand_data[1069],
    7,
    0,
    1,
    1
  },
  {
    "ffssi2+46",
    "#",
    0,
    &operand_data[1076],
    5,
    0,
    2,
    1
  },
  {
    "ffssi2+47",
    (const PTR) output_556,
    0,
    &operand_data[1054],
    5,
    0,
    2,
    2
  },
  {
    "ffssi2+48",
    "#",
    0,
    &operand_data[1059],
    5,
    0,
    2,
    1
  },
  {
    "ffssi2+49",
    (const PTR) output_558,
    0,
    &operand_data[1081],
    5,
    0,
    2,
    2
  },
  {
    "ffssi2+50",
    "#",
    0,
    &operand_data[1076],
    5,
    0,
    2,
    1
  },
  {
    "ffssi2+51",
    (const PTR) output_560,
    0,
    &operand_data[1054],
    5,
    0,
    2,
    2
  },
  {
    "ffssi2+52",
    "#",
    0,
    &operand_data[1059],
    5,
    0,
    2,
    1
  },
  {
    "ffssi2+53",
    "eor%?\t%0, %1, %2",
    0,
    &operand_data[1086],
    5,
    0,
    1,
    1
  },
  {
    "ffssi2+54",
    "orr%?\t%0, %1, %2\n\tbic%?\t%0, %0, %3",
    0,
    &operand_data[1091],
    6,
    0,
    3,
    1
  },
  {
    "ffssi2+55",
    "mov%?\t%0, %1%S3",
    0,
    &operand_data[1097],
    6,
    0,
    1,
    1
  },
  {
    "ffssi2+56",
    "mvn%?\t%0, %1%S3",
    0,
    &operand_data[1103],
    6,
    0,
    1,
    1
  },
  {
    "ffssi2+57",
    "rsb%?\t%0, %1, #0",
    0,
    &operand_data[1109],
    4,
    0,
    1,
    1
  },
  {
    "ffssi2+58",
    "mnf%?s\t%0, %1",
    0,
    &operand_data[1113],
    4,
    0,
    1,
    1
  },
  {
    "ffssi2+59",
    "mnf%?d\t%0, %1",
    0,
    &operand_data[1117],
    4,
    0,
    1,
    1
  },
  {
    "ffssi2+60",
    "mnf%?d\t%0, %1",
    0,
    &operand_data[1121],
    4,
    0,
    1,
    1
  },
  {
    "ffssi2+61",
    "mnf%?e\t%0, %1",
    0,
    &operand_data[1125],
    4,
    0,
    1,
    1
  },
  {
    "ffssi2+62",
    "abs%?s\t%0, %1",
    0,
    &operand_data[1113],
    4,
    0,
    1,
    1
  },
  {
    "ffssi2+63",
    "abs%?d\t%0, %1",
    0,
    &operand_data[1117],
    4,
    0,
    1,
    1
  },
  {
    "ffssi2+64",
    "abs%?d\t%0, %1",
    0,
    &operand_data[1121],
    4,
    0,
    1,
    1
  },
  {
    "ffssi2+65",
    "abs%?e\t%0, %1",
    0,
    &operand_data[1125],
    4,
    0,
    1,
    1
  },
  {
    "ffssi2+66",
    "sqt%?s\t%0, %1",
    0,
    &operand_data[1113],
    4,
    0,
    1,
    1
  },
  {
    "ffssi2+67",
    "sqt%?d\t%0, %1",
    0,
    &operand_data[1117],
    4,
    0,
    1,
    1
  },
  {
    "ffssi2+68",
    "sqt%?d\t%0, %1",
    0,
    &operand_data[1121],
    4,
    0,
    1,
    1
  },
  {
    "ffssi2+69",
    "sqt%?e\t%0, %1",
    0,
    &operand_data[1125],
    4,
    0,
    1,
    1
  },
  {
    "ffssi2+70",
    "#",
    0,
    &operand_data[1129],
    4,
    0,
    2,
    1
  },
  {
    "ffssi2+71",
    "mvn%?\t%0, %1",
    0,
    &operand_data[1109],
    4,
    0,
    1,
    1
  },
  {
    "ffssi2+72",
    "flt%?s\t%0, %1",
    0,
    &operand_data[1133],
    4,
    0,
    1,
    1
  },
  {
    "ffssi2+73",
    "flt%?d\t%0, %1",
    0,
    &operand_data[1137],
    4,
    0,
    1,
    1
  },
  {
    "ffssi2+74",
    "flt%?e\t%0, %1",
    0,
    &operand_data[1141],
    4,
    0,
    1,
    1
  },
  {
    "ffssi2+75",
    "fix%?z\t%0, %1",
    0,
    &operand_data[1145],
    4,
    0,
    1,
    1
  },
  {
    "ffssi2+76",
    "fix%?z\t%0, %1",
    0,
    &operand_data[1149],
    4,
    0,
    1,
    1
  },
  {
    "ffssi2+77",
    "fix%?z\t%0, %1",
    0,
    &operand_data[1153],
    4,
    0,
    1,
    1
  },
  {
    "ffssi2+78",
    "mvf%?s\t%0, %1",
    0,
    &operand_data[1157],
    4,
    0,
    1,
    1
  },
  {
    "ffssi2+79",
    "mvf%?s\t%0, %1",
    0,
    &operand_data[1161],
    4,
    0,
    1,
    1
  },
  {
    "ffssi2+80",
    "mvf%?d\t%0, %1",
    0,
    &operand_data[1165],
    4,
    0,
    1,
    1
  },
  {
    "ffssi2+81",
    (const PTR) output_590,
    0,
    &operand_data[1169],
    4,
    0,
    1,
    3
  },
  {
    "ffssi2+82",
    (const PTR) output_591,
    0,
    &operand_data[1173],
    4,
    0,
    2,
    2
  },
  {
    "ffssi2+83",
    (const PTR) output_592,
    0,
    &operand_data[1169],
    4,
    0,
    1,
    3
  },
  {
    "ffssi2+84",
    "ldr%?h\t%0, %1",
    0,
    &operand_data[1177],
    4,
    0,
    1,
    1
  },
  {
    "ffssi2+85",
    "ldr%?b\t%0, %1\t%@ zero_extendqisi2",
    0,
    &operand_data[1181],
    4,
    0,
    1,
    1
  },
  {
    "ffssi2+86",
    "ldr%?sh\t%0, %1",
    0,
    &operand_data[1177],
    4,
    0,
    1,
    1
  },
  {
    "ffssi2+87",
    (const PTR) output_596,
    0,
    &operand_data[1185],
    4,
    0,
    1,
    3
  },
  {
    "ffssi2+88",
    (const PTR) output_597,
    0,
    &operand_data[1181],
    4,
    0,
    1,
    3
  },
  {
    "ffssi2+89",
    "mvf%?d\t%0, %1",
    0,
    &operand_data[1121],
    4,
    0,
    1,
    1
  },
  {
    "ffssi2+90",
    "mvf%?e\t%0, %1",
    0,
    &operand_data[1189],
    4,
    0,
    1,
    1
  },
  {
    "ffssi2+91",
    "mvf%?e\t%0, %1",
    0,
    &operand_data[1193],
    4,
    0,
    1,
    1
  },
  {
    "ffssi2+92",
    (const PTR) output_601,
    0,
    &operand_data[1197],
    4,
    0,
    4,
    2
  },
  {
    "ffssi2+93",
    "adr%?\t%0, %a1",
    0,
    &operand_data[1201],
    4,
    0,
    1,
    1
  },
  {
    "ffssi2+94",
    (const PTR) output_603,
    0,
    &operand_data[1205],
    4,
    1,
    1,
    3
  },
  {
    "ffssi2+95",
    (const PTR) output_604,
    0,
    &operand_data[1209],
    4,
    0,
    1,
    3
  },
  {
    "ffssi2+96",
    (const PTR) output_605,
    0,
    &operand_data[1213],
    4,
    0,
    4,
    2
  },
  {
    "ffssi2+97",
    (const PTR) output_606,
    0,
    &operand_data[1217],
    4,
    0,
    3,
    2
  },
  {
    "ffssi2+98",
    (const PTR) output_607,
    0,
    &operand_data[1217],
    4,
    0,
    3,
    2
  },
  {
    "ffssi2+99",
    "ldr%?\t%0, %1\t%@ movhi_bigend",
    0,
    &operand_data[1177],
    4,
    0,
    1,
    1
  },
  {
    "ffssi2+100",
    (const PTR) output_609,
    0,
    &operand_data[1221],
    4,
    0,
    2,
    2
  },
  {
    "ffssi2+101",
    (const PTR) output_610,
    0,
    &operand_data[1225],
    4,
    0,
    4,
    2
  },
  {
    "ffssi2+102",
    (const PTR) output_611,
    0,
    &operand_data[1229],
    4,
    0,
    9,
    2
  },
  {
    "ffssi2+103",
    (const PTR) output_612,
    0,
    &operand_data[1233],
    4,
    0,
    3,
    2
  },
  {
    "ffssi2+104",
    (const PTR) output_613,
    0,
    &operand_data[1237],
    4,
    0,
    11,
    3
  },
  {
    "ffssi2+105",
    (const PTR) output_614,
    0,
    &operand_data[1241],
    4,
    0,
    7,
    3
  },
  {
    "ffssi2+106",
    "ldm%?ia\t%1!, {%3, %4, %5, %6}",
    0,
    &operand_data[1245],
    9,
    4,
    1,
    1
  },
  {
    "ffssi2+107",
    "ldm%?ia\t%1!, {%3, %4, %5}",
    0,
    &operand_data[1254],
    8,
    3,
    1,
    1
  },
  {
    "ffssi2+108",
    "ldm%?ia\t%1!, {%3, %4}",
    0,
    &operand_data[1262],
    7,
    2,
    1,
    1
  },
  {
    "ffssi2+109",
    "ldm%?ia\t%1, {%2, %3, %4, %5}",
    0,
    &operand_data[1269],
    8,
    3,
    1,
    1
  },
  {
    "ffssi2+110",
    "ldm%?ia\t%1, {%2, %3, %4}",
    0,
    &operand_data[1277],
    7,
    2,
    1,
    1
  },
  {
    "ffssi2+111",
    "ldm%?ia\t%1, {%2, %3}",
    0,
    &operand_data[1284],
    6,
    1,
    1,
    1
  },
  {
    "ffssi2+112",
    "stm%?ia\t%1!, {%3, %4, %5, %6}",
    0,
    &operand_data[1290],
    9,
    4,
    1,
    1
  },
  {
    "ffssi2+113",
    "stm%?ia\t%1!, {%3, %4, %5}",
    0,
    &operand_data[1299],
    8,
    3,
    1,
    1
  },
  {
    "ffssi2+114",
    "stm%?ia\t%1!, {%3, %4}",
    0,
    &operand_data[1307],
    7,
    2,
    1,
    1
  },
  {
    "ffssi2+115",
    "stm%?ia\t%1, {%2, %3, %4, %5}",
    0,
    &operand_data[1314],
    8,
    3,
    1,
    1
  },
  {
    "ffssi2+116",
    "stm%?ia\t%1, {%2, %3, %4}",
    0,
    &operand_data[1322],
    7,
    2,
    1,
    1
  },
  {
    "ffssi2+117",
    "stm%?ia\t%1, {%2, %3}",
    0,
    &operand_data[1329],
    6,
    1,
    1,
    1
  },
  {
    "ffssi2+118",
    (const PTR) output_627,
    0,
    &operand_data[373],
    3,
    0,
    0,
    3
  },
  {
    "ffssi2+119",
    (const PTR) output_628,
    0,
    &operand_data[374],
    2,
    0,
    0,
    3
  },
  {
    "ffssi2+120",
    "mov%?\t%|pc, %0\t%@ indirect register jump",
    0,
    &operand_data[1005],
    3,
    0,
    1,
    1
  },
  {
    "ffssi2+121",
    "ldr%?\t%|pc, %0\t%@ indirect memory jump",
    0,
    &operand_data[1335],
    3,
    0,
    1,
    1
  },
  {
    "ffssi2+122",
    "%i1%?\t%0, %2, %4%S3",
    0,
    &operand_data[1338],
    8,
    0,
    1,
    1
  },
  {
    "ffssi2+123",
    "sub%?\t%0, %1, %3%S2",
    0,
    &operand_data[1346],
    7,
    0,
    1,
    1
  },
  {
    "ffssi2+124",
    (const PTR) output_633,
    0,
    &operand_data[1353],
    7,
    0,
    1,
    3
  },
  {
    "ffssi2+125",
    "str%?b\t%3, [%0, %2]!",
    0,
    &operand_data[1360],
    6,
    2,
    1,
    1
  },
  {
    "ffssi2+126",
    "str%?b\t%3, [%0, -%2]!",
    0,
    &operand_data[1366],
    6,
    2,
    1,
    1
  },
  {
    "ffssi2+127",
    "ldr%?b\t%3, [%0, %2]!",
    0,
    &operand_data[1372],
    6,
    2,
    1,
    1
  },
  {
    "ffssi2+128",
    "ldr%?b\t%3, [%0, -%2]!",
    0,
    &operand_data[1378],
    6,
    2,
    1,
    1
  },
  {
    "ffssi2+129",
    "ldr%?b\t%3, [%0, %2]!\t%@ z_extendqisi",
    0,
    &operand_data[1384],
    6,
    2,
    1,
    1
  },
  {
    "ffssi2+130",
    "ldr%?b\t%3, [%0, -%2]!\t%@ z_extendqisi",
    0,
    &operand_data[1390],
    6,
    2,
    1,
    1
  },
  {
    "ffssi2+131",
    "str%?\t%3, [%0, %2]!",
    0,
    &operand_data[1396],
    6,
    2,
    1,
    1
  },
  {
    "ffssi2+132",
    "str%?\t%3, [%0, -%2]!",
    0,
    &operand_data[1402],
    6,
    2,
    1,
    1
  },
  {
    "ffssi2+133",
    "ldr%?\t%3, [%0, %2]!",
    0,
    &operand_data[1384],
    6,
    2,
    1,
    1
  },
  {
    "ffssi2+134",
    "ldr%?\t%3, [%0, -%2]!",
    0,
    &operand_data[1390],
    6,
    2,
    1,
    1
  },
  {
    "ffssi2+135",
    "ldr%?\t%3, [%0, %2]!\t%@ loadhi",
    0,
    &operand_data[1408],
    6,
    2,
    1,
    1
  },
  {
    "ffssi2+136",
    "ldr%?\t%3, [%0, -%2]!\t%@ loadhi",
    0,
    &operand_data[1414],
    6,
    2,
    1,
    1
  },
  {
    "ffssi2+137",
    "str%?b\t%5, [%0, %3%S2]!",
    0,
    &operand_data[1420],
    8,
    2,
    1,
    1
  },
  {
    "ffssi2+138",
    "str%?b\t%5, [%0, -%3%S2]!",
    0,
    &operand_data[1420],
    8,
    2,
    1,
    1
  },
  {
    "ffssi2+139",
    "ldr%?b\t%5, [%0, %3%S2]!",
    0,
    &operand_data[1428],
    8,
    2,
    1,
    1
  },
  {
    "ffssi2+140",
    "ldr%?b\t%5, [%0, -%3%S2]!",
    0,
    &operand_data[1428],
    8,
    2,
    1,
    1
  },
  {
    "ffssi2+141",
    "str%?\t%5, [%0, %3%S2]!",
    0,
    &operand_data[1436],
    8,
    2,
    1,
    1
  },
  {
    "ffssi2+142",
    "str%?\t%5, [%0, -%3%S2]!",
    0,
    &operand_data[1436],
    8,
    2,
    1,
    1
  },
  {
    "ffssi2+143",
    "ldr%?\t%5, [%0, %3%S2]!",
    0,
    &operand_data[1444],
    8,
    2,
    1,
    1
  },
  {
    "ffssi2+144",
    "ldr%?\t%5, [%0, -%3%S2]!",
    0,
    &operand_data[1444],
    8,
    2,
    1,
    1
  },
  {
    "ffssi2+145",
    "ldr%?\t%5, [%0, %3%S2]!\t%@ loadhi",
    0,
    &operand_data[1452],
    8,
    2,
    1,
    1
  },
  {
    "ffssi2+146",
    "ldr%?\t%5, [%0, -%3%S2]!\t%@ loadhi",
    0,
    &operand_data[1452],
    8,
    2,
    1,
    1
  },
};


const char *
get_insn_name (code)
     int code;
{
  return insn_data[code].name;
}
