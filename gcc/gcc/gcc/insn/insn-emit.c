/* Generated automatically by the program `genemit'
from the machine description file `md'.  */

#include "config.h"
#include "system.h"
#include "rtl.h"
#include "expr.h"
#include "real.h"
#include "flags.h"
#include "output.h"
#include "insn-config.h"
#include "insn-flags.h"
#include "insn-codes.h"
#include "recog.h"
#include "reload.h"

extern rtx recog_operand[];
#define operands emit_operand

#define FAIL return (end_sequence (), _val)
#define DONE return (_val = gen_sequence (), end_sequence (), _val)
rtx
gen_adddi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (DImode,
	operand1,
	operand2)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_REG (CCmode,
	24))));
}

rtx
gen_addsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx operands[3];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;
  operands[2] = operand2;

  if (GET_CODE (operands[2]) == CONST_INT)
    {
      arm_split_constant (PLUS, SImode, INTVAL (operands[2]), operands[0],
			  operands[1],
			  (reload_in_progress || reload_completed ? 0
			   : preserve_subexpressions_p ()));
      DONE;
    }

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (SImode,
	operand1,
	operand2)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_split_4 (operands)
     rtx *operands;
{
  rtx _val = 0;
  start_sequence ();

  arm_split_constant (PLUS, SImode, INTVAL (operands[2]), operands[0],
		      operands[1], 0);
  DONE;

  emit_insn (gen_rtx_CLOBBER (VOIDmode,
	const0_rtx));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_incscc (operand0, operand1, operand2, operand3)
     rtx operand0;
     rtx operand1;
     rtx operand2;
     rtx operand3;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (SImode,
	gen_rtx (GET_CODE (operand2), SImode,
		operand3,
		const0_rtx),
	operand1));
}

rtx
gen_split_17 (operands)
     rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx _val = 0;
  start_sequence ();

{
  unsigned int val = (unsigned) INTVAL (operands[2]);
  int i;
  unsigned int temp;

  /* this code is similar to the approach followed in movsi, but it must
     generate exactly two insns */

  for (i = 30; i >= 0; i -= 2)
    {
      if (val & (3 << i))
	{
	  i -= 6;
	  if (i < 0) i = 0;
	  if (const_ok_for_arm (temp = (val & ~(255 << i))))
	    {
	      val &= 255 << i;
	      break;
	    }
	  /* we might be able to do this as (larger number - small number) */
	  temp = ((val >> i) & 255) + 1;
	  if (temp > 255 && i < 24)
	    {
	      i += 2;
	      temp = ((val >> i) & 255) + 1;
	    }
	  if (const_ok_for_arm ((temp << i) - val))
	    {
	      i = temp << i;
	      temp = (unsigned) - (int) (i - val);
	      val = i;
	      break;
	    }
	  FAIL;
	}
    }
  /* if we got here, we have found a way of doing it in two instructions.
     the two constants are in val and temp */
  operands[2] = GEN_INT ((int)val);
  operands[3] = GEN_INT ((int)temp);
}

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (SImode,
	operand1,
	operand2)));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (SImode,
	operand0,
	operand3)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_addsf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (SFmode,
	operand1,
	operand2));
}

rtx
gen_adddf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (DFmode,
	operand1,
	operand2));
}

rtx
gen_addxf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (XFmode,
	operand1,
	operand2));
}

rtx
gen_subdi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (DImode,
	operand1,
	operand2)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_REG (CCmode,
	24))));
}

rtx
gen_subsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx operands[3];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;
  operands[2] = operand2;

  if (GET_CODE (operands[1]) == CONST_INT)
    {
      arm_split_constant (MINUS, SImode, INTVAL (operands[1]), operands[0],
			  operands[2],
			  (reload_in_progress || reload_completed ? 0
			   : preserve_subexpressions_p ()));
      DONE;
    }

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (SImode,
	operand1,
	operand2)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_split_32 (operands)
     rtx *operands;
{
  rtx _val = 0;
  start_sequence ();

  arm_split_constant (MINUS, SImode, INTVAL (operands[1]), operands[0],
		      operands[2], 0);
  DONE;

  emit_insn (gen_rtx_CLOBBER (VOIDmode,
	const0_rtx));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_decscc (operand0, operand1, operand2, operand3)
     rtx operand0;
     rtx operand1;
     rtx operand2;
     rtx operand3;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (SImode,
	operand1,
	gen_rtx (GET_CODE (operand2), SImode,
		operand3,
		const0_rtx)));
}

rtx
gen_subsf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (SFmode,
	operand1,
	operand2));
}

rtx
gen_subdf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (DFmode,
	operand1,
	operand2));
}

rtx
gen_subxf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (XFmode,
	operand1,
	operand2));
}

rtx
gen_mulsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MULT (SImode,
	operand2,
	operand1));
}

rtx
gen_mulsidi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MULT (DImode,
	gen_rtx_SIGN_EXTEND (DImode,
	operand1),
	gen_rtx_SIGN_EXTEND (DImode,
	operand2)));
}

rtx
gen_umulsidi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MULT (DImode,
	gen_rtx_ZERO_EXTEND (DImode,
	operand1),
	gen_rtx_ZERO_EXTEND (DImode,
	operand2)));
}

rtx
gen_smulsi3_highpart (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_TRUNCATE (SImode,
	gen_rtx_LSHIFTRT (DImode,
	gen_rtx_MULT (DImode,
	gen_rtx_SIGN_EXTEND (DImode,
	operand1),
	gen_rtx_SIGN_EXTEND (DImode,
	operand2)),
	GEN_INT (32)))),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode))));
}

rtx
gen_umulsi3_highpart (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_TRUNCATE (SImode,
	gen_rtx_LSHIFTRT (DImode,
	gen_rtx_MULT (DImode,
	gen_rtx_ZERO_EXTEND (DImode,
	operand1),
	gen_rtx_ZERO_EXTEND (DImode,
	operand2)),
	GEN_INT (32)))),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode))));
}

rtx
gen_mulsf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MULT (SFmode,
	operand1,
	operand2));
}

rtx
gen_muldf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MULT (DFmode,
	operand1,
	operand2));
}

rtx
gen_mulxf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MULT (XFmode,
	operand1,
	operand2));
}

rtx
gen_divsf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_DIV (SFmode,
	operand1,
	operand2));
}

rtx
gen_divdf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_DIV (DFmode,
	operand1,
	operand2));
}

rtx
gen_divxf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_DIV (XFmode,
	operand1,
	operand2));
}

rtx
gen_modsf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MOD (SFmode,
	operand1,
	operand2));
}

rtx
gen_moddf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MOD (DFmode,
	operand1,
	operand2));
}

rtx
gen_modxf3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MOD (XFmode,
	operand1,
	operand2));
}

rtx
gen_anddi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_AND (DImode,
	operand1,
	operand2));
}

rtx
gen_andsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx operands[3];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;
  operands[2] = operand2;

  if (GET_CODE (operands[2]) == CONST_INT)
    {
      arm_split_constant (AND, SImode, INTVAL (operands[2]), operands[0],
			  operands[1],
			  (reload_in_progress || reload_completed
			   ? 0 : preserve_subexpressions_p ()));
      DONE;
    }

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_AND (SImode,
	operand1,
	operand2)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_split_74 (operands)
     rtx *operands;
{
  rtx _val = 0;
  start_sequence ();

  arm_split_constant  (AND, SImode, INTVAL (operands[2]), operands[0],
		       operands[1], 0);
  DONE;

  emit_insn (gen_rtx_CLOBBER (VOIDmode,
	const0_rtx));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_insv (operand0, operand1, operand2, operand3)
     rtx operand0;
     rtx operand1;
     rtx operand2;
     rtx operand3;
{
  rtx operands[4];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;
  operands[2] = operand2;
  operands[3] = operand3;

{
  int start_bit = INTVAL (operands[2]);
  int width = INTVAL (operands[1]);
  HOST_WIDE_INT mask = (((HOST_WIDE_INT)1) << width) - 1;
  rtx target, subtarget;
  
  target = operands[0];
  /* Avoid using a subreg as a subtarget, and avoid writing a paradoxical 
     subreg as the final target.  */
  if (GET_CODE (target) == SUBREG)
    {
      subtarget = gen_reg_rtx (SImode);
      if (GET_MODE_SIZE (GET_MODE (SUBREG_REG (target)))
	  < GET_MODE_SIZE (SImode))
        target = SUBREG_REG (target);
    }
  else
    subtarget = target;    
    
  if (GET_CODE (operands[3]) == CONST_INT)
    {
      /* Since we are inserting a known constant, we may be able to
	 reduce the number of bits that we have to clear so that
	 the mask becomes simple.  */
      /* ??? This code does not check to see if the new mask is actually
	 simpler.  It may not be.  */
      rtx op1 = gen_reg_rtx (SImode);
      /* ??? Truncate operand3 to fit in the bitfield.  See comment before
	 start of this pattern.  */
      HOST_WIDE_INT op3_value = mask & INTVAL (operands[3]);
      HOST_WIDE_INT mask2 = ((mask & ~op3_value) << start_bit);

      emit_insn (gen_andsi3 (op1, operands[0], GEN_INT (~mask2)));
      emit_insn (gen_iorsi3 (subtarget, op1,
			     GEN_INT (op3_value << start_bit)));
    }
  else if (start_bit == 0
	   && ! (const_ok_for_arm (mask)
		 || const_ok_for_arm (~mask)))
    {
      /* A Trick, since we are setting the bottom bits in the word,
	 we can shift operand[3] up, operand[0] down, OR them together
	 and rotate the result back again.  This takes 3 insns, and
	 the third might be mergable into another op.  */
      /* The shift up copes with the possibility that operand[3] is
         wider than the bitfield.  */
      rtx op0 = gen_reg_rtx (SImode);
      rtx op1 = gen_reg_rtx (SImode);

      emit_insn (gen_ashlsi3 (op0, operands[3], GEN_INT (32 - width)));
      emit_insn (gen_iorsi3 (op1, gen_rtx_LSHIFTRT (SImode, operands[0],
							    operands[1]),
			     op0));
      emit_insn (gen_rotlsi3 (subtarget, op1, operands[1]));
    }
  else if ((width + start_bit == 32)
	   && ! (const_ok_for_arm (mask)
		 || const_ok_for_arm (~mask)))
    {
      /* Similar trick, but slightly less efficient.  */

      rtx op0 = gen_reg_rtx (SImode);
      rtx op1 = gen_reg_rtx (SImode);

      emit_insn (gen_ashlsi3 (op0, operands[3], GEN_INT (32 - width)));
      emit_insn (gen_ashlsi3 (op1, operands[0], operands[1]));
      emit_insn (gen_iorsi3 (subtarget,
			     gen_rtx_LSHIFTRT (SImode, op1, operands[1]),
			     op0));
    }
  else
    {
      rtx op0 = GEN_INT (mask);
      rtx op1 = gen_reg_rtx (SImode);
      rtx op2 = gen_reg_rtx (SImode);

      if (! (const_ok_for_arm (mask) || const_ok_for_arm (~mask)))
	{
	  rtx tmp = gen_reg_rtx (SImode);

	  emit_insn (gen_movsi (tmp, op0));
	  op0 = tmp;
	}

      /* Mask out any bits in operand[3] that are not needed.  */
      emit_insn (gen_andsi3 (op1, operands[3], op0));

      if (GET_CODE (op0) == CONST_INT
	  && (const_ok_for_arm (mask << start_bit)
	      || const_ok_for_arm (~ (mask << start_bit))))
	{
	  op0 = GEN_INT (~(mask << start_bit));
	  emit_insn (gen_andsi3 (op2, operands[0], op0));
	}
      else
	{
	  if (GET_CODE (op0) == CONST_INT)
	    {
	      rtx tmp = gen_reg_rtx (SImode);

	      emit_insn (gen_movsi (tmp, op0));
	      op0 = tmp;
	    }

	  if (start_bit != 0)
	    op0 = gen_rtx_ASHIFT (SImode, op0, operands[2]);
	    
	  emit_insn (gen_andsi_notsi_si (op2, operands[0], op0));
	}

      if (start_bit != 0)
	op1 = gen_rtx_ASHIFT (SImode, op1, operands[2]);

      emit_insn (gen_iorsi3 (subtarget, op1, op2));
    }

  if (subtarget != target)
    {
      /* If TARGET is still a SUBREG, then it must be wider than a word,
	 so we must be careful only to set the subword we were asked to. */
      if (GET_CODE (target) == SUBREG)
	emit_move_insn (target, subtarget);
      else
	emit_move_insn (target, gen_lowpart (GET_MODE (target), subtarget));
    }

  DONE;
}

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	gen_rtx_ZERO_EXTRACT (SImode,
	operand0,
	operand1,
	operand2),
	operand3));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_andsi_notsi_si (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_AND (SImode,
	gen_rtx_NOT (SImode,
	operand2),
	operand1));
}

rtx
gen_andsi_not_shiftsi_si (operand0, operand1, operand2, operand3, operand4)
     rtx operand0;
     rtx operand1;
     rtx operand2;
     rtx operand3;
     rtx operand4;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_AND (SImode,
	gen_rtx_NOT (SImode,
	gen_rtx (GET_CODE (operand4), SImode,
		operand2,
		operand3)),
	operand1));
}

rtx
gen_iordi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_IOR (DImode,
	operand1,
	operand2));
}

rtx
gen_iorsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx operands[3];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;
  operands[2] = operand2;

  if (GET_CODE (operands[2]) == CONST_INT)
    {
      arm_split_constant (IOR, SImode, INTVAL (operands[2]), operands[0],
			  operands[1],
			  (reload_in_progress || reload_completed
			   ? 0 : preserve_subexpressions_p ()));
      DONE;
    }

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_IOR (SImode,
	operand1,
	operand2)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_split_92 (operands)
     rtx *operands;
{
  rtx _val = 0;
  start_sequence ();

  arm_split_constant (IOR, SImode, INTVAL (operands[2]), operands[0],
		      operands[1], 0);
  DONE;

  emit_insn (gen_rtx_CLOBBER (VOIDmode,
	const0_rtx));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_xordi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_XOR (DImode,
	operand1,
	operand2));
}

rtx
gen_xorsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_XOR (SImode,
	operand1,
	operand2));
}

rtx
gen_split_101 (operands)
     rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx _val = 0;
  start_sequence ();

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand4,
	gen_rtx_AND (SImode,
	gen_rtx_IOR (SImode,
	operand1,
	operand2),
	gen_rtx_NOT (SImode,
	operand3))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_NOT (SImode,
	operand4)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_smaxsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SMAX (SImode,
	operand1,
	operand2)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_REG (CCmode,
	24))));
}

rtx
gen_sminsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SMIN (SImode,
	operand1,
	operand2)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_REG (CCmode,
	24))));
}

rtx
gen_umaxsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UMAX (SImode,
	operand1,
	operand2)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_REG (CCmode,
	24))));
}

rtx
gen_uminsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UMIN (SImode,
	operand1,
	operand2)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_REG (CCmode,
	24))));
}

rtx
gen_ashlsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx operands[3];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;
  operands[2] = operand2;

  if (GET_CODE (operands[2]) == CONST_INT
      && ((unsigned HOST_WIDE_INT) INTVAL (operands[2])) > 31)
    {
      emit_insn (gen_movsi (operands[0], const0_rtx));
      DONE;
    }

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ASHIFT (SImode,
	operand1,
	operand2)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_ashrsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx operands[3];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;
  operands[2] = operand2;

  if (GET_CODE (operands[2]) == CONST_INT
      && ((unsigned HOST_WIDE_INT) INTVAL (operands[2])) > 31)
    operands[2] = GEN_INT (31);

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ASHIFTRT (SImode,
	operand1,
	operand2)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_lshrsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx operands[3];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;
  operands[2] = operand2;

  if (GET_CODE (operands[2]) == CONST_INT
      && ((unsigned HOST_WIDE_INT) INTVAL (operands[2])) > 31)
    {
      emit_insn (gen_movsi (operands[0], const0_rtx));
      DONE;
    }

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_LSHIFTRT (SImode,
	operand1,
	operand2)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_rotlsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx operands[3];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;
  operands[2] = operand2;

  if (GET_CODE (operands[2]) == CONST_INT)
    operands[2] = GEN_INT ((32 - INTVAL (operands[2])) % 32);
  else
    {
      rtx reg = gen_reg_rtx (SImode);
      emit_insn (gen_subsi3 (reg, GEN_INT (32), operands[2]));
      operands[2] = reg;
    }

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ROTATERT (SImode,
	operand1,
	operand2)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_rotrsi3 (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx operands[3];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;
  operands[2] = operand2;

  if (GET_CODE (operands[2]) == CONST_INT
      && ((unsigned HOST_WIDE_INT) INTVAL (operands[2])) > 31)
    operands[2] = GEN_INT (INTVAL (operands[2]) % 32);

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ROTATERT (SImode,
	operand1,
	operand2)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_negdi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_NEG (DImode,
	operand1)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_REG (CCmode,
	24))));
}

rtx
gen_negsi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_NEG (SImode,
	operand1));
}

rtx
gen_negsf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_NEG (SFmode,
	operand1));
}

rtx
gen_negdf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_NEG (DFmode,
	operand1));
}

rtx
gen_negxf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_NEG (XFmode,
	operand1));
}

rtx
gen_abssi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ABS (SImode,
	operand1)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_REG (CCmode,
	24))));
}

rtx
gen_abssf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ABS (SFmode,
	operand1));
}

rtx
gen_absdf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ABS (DFmode,
	operand1));
}

rtx
gen_absxf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ABS (XFmode,
	operand1));
}

rtx
gen_sqrtsf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SQRT (SFmode,
	operand1));
}

rtx
gen_sqrtdf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SQRT (DFmode,
	operand1));
}

rtx
gen_sqrtxf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SQRT (XFmode,
	operand1));
}

rtx
gen_one_cmpldi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_NOT (DImode,
	operand1));
}

rtx
gen_one_cmplsi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_NOT (SImode,
	operand1));
}

rtx
gen_floatsisf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FLOAT (SFmode,
	operand1));
}

rtx
gen_floatsidf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FLOAT (DFmode,
	operand1));
}

rtx
gen_floatsixf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FLOAT (XFmode,
	operand1));
}

rtx
gen_fix_truncsfsi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FIX (SImode,
	operand1));
}

rtx
gen_fix_truncdfsi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FIX (SImode,
	operand1));
}

rtx
gen_fix_truncxfsi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FIX (SImode,
	operand1));
}

rtx
gen_truncdfsf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FLOAT_TRUNCATE (SFmode,
	operand1));
}

rtx
gen_truncxfsf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FLOAT_TRUNCATE (SFmode,
	operand1));
}

rtx
gen_truncxfdf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FLOAT_TRUNCATE (DFmode,
	operand1));
}

rtx
gen_zero_extendsidi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ZERO_EXTEND (DImode,
	operand1));
}

rtx
gen_zero_extendqidi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ZERO_EXTEND (DImode,
	operand1));
}

rtx
gen_extendsidi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SIGN_EXTEND (DImode,
	operand1));
}

rtx
gen_zero_extendhisi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operand2;
  rtx operands[3];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;

{
  if ((arm_arch4 && ! ARM_ARCH_RISCPC) && GET_CODE (operands[1]) == MEM)
    {
     /* Note: We do not have to worry about TARGET_SHORT_BY_BYTES
	here because the insn below will generate an LDRH instruction
	rather than an LDR instruction, so we cannot get an unaligned
	word access.  */
      emit_insn (gen_rtx_SET (VOIDmode, operands[0],
			      gen_rtx_ZERO_EXTEND (SImode, operands[1])));
      DONE;
    }
  if (TARGET_SHORT_BY_BYTES && GET_CODE (operands[1]) == MEM)
    {
      emit_insn (gen_movhi_bytes (operands[0], operands[1]));
      DONE;
    }
  if (! s_register_operand (operands[1], HImode))
    operands[1] = copy_to_mode_reg (HImode, operands[1]);
  operands[1] = gen_lowpart (SImode, operands[1]);
  operands[2] = gen_reg_rtx (SImode); 
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	gen_rtx_ASHIFT (SImode,
	operand1,
	GEN_INT (16))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_LSHIFTRT (SImode,
	operand2,
	GEN_INT (16))));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_split_154 (operands)
     rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();

{
  if ((operands[1] = gen_rotated_half_load (operands[1])) == NULL)
    FAIL;
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	operand1));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_LSHIFTRT (SImode,
	operand2,
	GEN_INT (16))));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_split_155 (operands)
     rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx _val = 0;
  start_sequence ();

{
  if ((operands[1] = gen_rotated_half_load (operands[1])) == NULL)
    FAIL;
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	operand1));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx (GET_CODE (operand3), GET_MODE (operand3),
		gen_rtx_LSHIFTRT (SImode,
	operand2,
	GEN_INT (16)),
		operand4)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_zero_extendqisi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;

  if (GET_CODE (operands[1]) != MEM)
    {
      emit_insn (gen_andsi3 (operands[0], gen_lowpart (SImode, operands[1]),
			     GEN_INT (255)));
      DONE;
    }

  operand0 = operands[0];
  operand1 = operands[1];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ZERO_EXTEND (SImode,
	operand1)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_split_158 (operands)
     rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	operand1));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_AND (SImode,
	operand2,
	GEN_INT (255))));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_extendhisi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operand2;
  rtx operands[3];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;

{
  if (arm_arch4 && ! ARM_ARCH_RISCPC && GET_CODE (operands[1]) == MEM)
    {
     /* Note: We do not have to worry about TARGET_SHORT_BY_BYTES
	here because the insn below will generate an LDRH instruction
	rather than an LDR instruction, so we cannot get an unaligned
	word access.  */
      emit_insn (gen_rtx_SET (VOIDmode, operands[0],
		 gen_rtx_SIGN_EXTEND (SImode, operands[1])));
      DONE;
    }

  if (TARGET_SHORT_BY_BYTES && GET_CODE (operands[1]) == MEM)
    {
      emit_insn (gen_extendhisi2_mem (operands[0], operands[1]));
      DONE;
    }
  if (! s_register_operand (operands[1], HImode))
    operands[1] = copy_to_mode_reg (HImode, operands[1]);
  operands[1] = gen_lowpart (SImode, operands[1]);
  operands[2] = gen_reg_rtx (SImode);
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	gen_rtx_ASHIFT (SImode,
	operand1,
	GEN_INT (16))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ASHIFTRT (SImode,
	operand2,
	GEN_INT (16))));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_extendhisi2_mem (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx operand5;
  rtx operand6;
  rtx operand7;
  rtx operands[8];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;

{
  rtx mem1, mem2;
  rtx addr = copy_to_mode_reg (SImode, XEXP (operands[1], 0));

  mem1 = gen_rtx_MEM (QImode, addr);
  MEM_COPY_ATTRIBUTES (mem1, operands[1]);
  RTX_UNCHANGING_P (mem1) = RTX_UNCHANGING_P (operands[1]);
  mem2 = gen_rtx_MEM (QImode, plus_constant (addr, 1));
  MEM_COPY_ATTRIBUTES (mem2, operands[1]);
  RTX_UNCHANGING_P (mem2) = RTX_UNCHANGING_P (operands[1]);
  operands[0] = gen_lowpart (SImode, operands[0]);
  operands[1] = mem1;
  operands[2] = gen_reg_rtx (SImode);
  operands[3] = gen_reg_rtx (SImode);
  operands[6] = gen_reg_rtx (SImode);
  operands[7] = mem2;

  if (BYTES_BIG_ENDIAN)
    {
      operands[4] = operands[2];
      operands[5] = operands[3];
    }
  else
    {
      operands[4] = operands[3];
      operands[5] = operands[2];
    }
}

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  operand5 = operands[5];
  operand6 = operands[6];
  operand7 = operands[7];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	gen_rtx_ZERO_EXTEND (SImode,
	operand1)));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand3,
	gen_rtx_ZERO_EXTEND (SImode,
	operand7)));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand6,
	gen_rtx_ASHIFT (SImode,
	operand4,
	GEN_INT (24))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_IOR (SImode,
	gen_rtx_ASHIFTRT (SImode,
	operand6,
	GEN_INT (16)),
	operand5)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_split_163 (operands)
     rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();

{
  if ((operands[1] = gen_rotated_half_load (operands[1])) == NULL)
    FAIL;
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	operand1));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ASHIFTRT (SImode,
	operand2,
	GEN_INT (16))));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_split_164 (operands)
     rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx _val = 0;
  start_sequence ();

{
  if ((operands[1] = gen_rotated_half_load (operands[1])) == NULL)
    FAIL;
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	operand1));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx (GET_CODE (operand3), GET_MODE (operand3),
		gen_rtx_ASHIFTRT (SImode,
	operand2,
	GEN_INT (16)),
		operand4)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_extendqihi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operand2;
  rtx operands[3];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;

{
  if (arm_arch4 && GET_CODE (operands[1]) == MEM)
    {
      emit_insn (gen_rtx_SET (VOIDmode,
			      operands[0],
			      gen_rtx_SIGN_EXTEND (HImode, operands[1])));
      DONE;
    }
  if (! s_register_operand (operands[1], QImode))
    operands[1] = copy_to_mode_reg (QImode, operands[1]);
  operands[0] = gen_lowpart (SImode, operands[0]);
  operands[1] = gen_lowpart (SImode, operands[1]);
  operands[2] = gen_reg_rtx (SImode);
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	gen_rtx_ASHIFT (SImode,
	operand1,
	GEN_INT (24))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ASHIFTRT (SImode,
	operand2,
	GEN_INT (24))));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_split_167 (operands)
     rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx _val = 0;
  start_sequence ();

  {
    HOST_WIDE_INT offset;

    operands[3] = gen_rtx_REG (SImode, REGNO (operands[0]));
    operands[2] = gen_rtx_MEM (QImode, operands[3]);
    MEM_COPY_ATTRIBUTES (operands[2], operands[1]);
    RTX_UNCHANGING_P (operands[2]) = RTX_UNCHANGING_P (operands[1]);
    operands[1] = XEXP (operands[1], 0);
    if (GET_CODE (operands[1]) == PLUS
	&& GET_CODE (XEXP (operands[1], 1)) == CONST_INT
	&& ! (const_ok_for_arm (offset = INTVAL (XEXP (operands[1], 1)))
	      || const_ok_for_arm (-offset)))
      {
	HOST_WIDE_INT low = (offset > 0
			     ? (offset & 0xff) : -((-offset) & 0xff));
	XEXP (operands[2], 0) = plus_constant (operands[3], low);
	operands[1] = plus_constant (XEXP (operands[1], 0), offset - low);
      }
    /* Ensure the sum is in correct canonical form */
    else if (GET_CODE (operands[1]) == PLUS
	     && GET_CODE (XEXP (operands[1], 1)) != CONST_INT
	     && ! s_register_operand (XEXP (operands[1], 1), VOIDmode))
      operands[1] = gen_rtx_PLUS (GET_MODE (operands[1]),
					   XEXP (operands[1], 1),
					   XEXP (operands[1], 0));
  }

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand3,
	operand1));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SIGN_EXTEND (HImode,
	operand2)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_extendqisi2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operand2;
  rtx operands[3];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;

{
  if (arm_arch4 && GET_CODE (operands[1]) == MEM)
    {
      emit_insn (gen_rtx_SET (VOIDmode,
			      operands[0],
			      gen_rtx_SIGN_EXTEND (SImode, operands[1])));
      DONE;
    }
  if (! s_register_operand (operands[1], QImode))
    operands[1] = copy_to_mode_reg (QImode, operands[1]);
  operands[1] = gen_lowpart (SImode, operands[1]);
  operands[2] = gen_reg_rtx (SImode);
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	gen_rtx_ASHIFT (SImode,
	operand1,
	GEN_INT (24))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ASHIFTRT (SImode,
	operand2,
	GEN_INT (24))));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_split_170 (operands)
     rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();

  {
    HOST_WIDE_INT offset;

    operands[2] = gen_rtx_MEM (QImode, operands[0]);
    MEM_COPY_ATTRIBUTES (operands[2], operands[1]);
    RTX_UNCHANGING_P (operands[2]) = RTX_UNCHANGING_P (operands[1]);
    operands[1] = XEXP (operands[1], 0);
    if (GET_CODE (operands[1]) == PLUS
	&& GET_CODE (XEXP (operands[1], 1)) == CONST_INT
	&& ! (const_ok_for_arm (offset = INTVAL (XEXP (operands[1], 1)))
	      || const_ok_for_arm (-offset)))
      {
	HOST_WIDE_INT low = (offset > 0
			     ? (offset & 0xff) : -((-offset) & 0xff));
	XEXP (operands[2], 0) = plus_constant (operands[0], low);
	operands[1] = plus_constant (XEXP (operands[1], 0), offset - low);
      }
    /* Ensure the sum is in correct canonical form */
    else if (GET_CODE (operands[1]) == PLUS
	     && GET_CODE (XEXP (operands[1], 1)) != CONST_INT
	     && ! s_register_operand (XEXP (operands[1], 1), VOIDmode))
      operands[1] = gen_rtx_PLUS (GET_MODE (operands[1]),
					   XEXP (operands[1], 1),
					   XEXP (operands[1], 0));
  }

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SIGN_EXTEND (SImode,
	operand2)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_extendsfdf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FLOAT_EXTEND (DFmode,
	operand1));
}

rtx
gen_extendsfxf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FLOAT_EXTEND (XFmode,
	operand1));
}

rtx
gen_extenddfxf2 (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_FLOAT_EXTEND (XFmode,
	operand1));
}

rtx
gen_movdi (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	operand1);
}

rtx
gen_movsi (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;

  /* Everything except mem = const or mem = mem can be done easily */
  if (GET_CODE (operands[0]) == MEM)
    operands[1] = force_reg (SImode, operands[1]);
  if (GET_CODE (operands[1]) == CONST_INT
      && !(const_ok_for_arm (INTVAL (operands[1]))
           || const_ok_for_arm (~INTVAL (operands[1]))))
    {
      arm_split_constant (SET, SImode, INTVAL (operands[1]), operands[0],
			  NULL_RTX,
			  (reload_in_progress || reload_completed ? 0
			   : preserve_subexpressions_p ()));
      DONE;
    }
  if (CONSTANT_P (operands[1]) && flag_pic)
    operands[1] = legitimize_pic_address (operands[1], SImode,
					  ((reload_in_progress
					    || reload_completed)
					   ? operands[0] : 0));

  operand0 = operands[0];
  operand1 = operands[1];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_split_177 (operands)
     rtx *operands;
{
  rtx _val = 0;
  start_sequence ();

  arm_split_constant (SET, SImode, INTVAL (operands[1]), operands[0],
		      NULL_RTX, 0);
  DONE;

  emit_insn (gen_rtx_CLOBBER (VOIDmode,
	const0_rtx));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_movaddr (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	operand1);
}

rtx
gen_pic_load_addr (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (1,
		operand1),
	3));
}

rtx
gen_pic_load_addr_based (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operand2;
  rtx operands[3];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;
operands[2] = pic_offset_table_rtx;
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_UNSPEC (SImode,
	gen_rtvec (2,
		operand1,
		operand2),
	3)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_pic_add_dot_plus_eight (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_PLUS (SImode,
	operand0,
	gen_rtx_CONST (VOIDmode,
	gen_rtx_PLUS (SImode,
	pc_rtx,
	GEN_INT (8))))),
		gen_rtx_USE (VOIDmode,
	gen_rtx_LABEL_REF (VOIDmode,
	operand1))));
}

rtx
gen_storehi (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx operands[5];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;

{
  rtx addr = XEXP (operands[1], 0);
  enum rtx_code code = GET_CODE (addr);

  if ((code == PLUS && GET_CODE (XEXP (addr, 1)) != CONST_INT)
      || code == MINUS)
    addr = force_reg (SImode, addr);

  operands[4] = change_address (operands[1], QImode, plus_constant (addr, 1));
  operands[1] = change_address (operands[1], QImode, NULL_RTX);
  operands[3] = gen_lowpart (QImode, operands[0]);
  operands[0] = gen_lowpart (SImode, operands[0]);
  operands[2] = gen_reg_rtx (SImode); 
}

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand1,
	operand3));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	gen_rtx_ASHIFTRT (SImode,
	operand0,
	GEN_INT (8))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand4,
	gen_rtx_SUBREG (QImode,
	operand2,
	0)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_storehi_bigend (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx operands[5];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;

{
  rtx addr = XEXP (operands[1], 0);
  enum rtx_code code = GET_CODE (addr);

  if ((code == PLUS && GET_CODE (XEXP (addr, 1)) != CONST_INT)
      || code == MINUS)
    addr = force_reg (SImode, addr);

  operands[4] = change_address (operands[1], QImode, plus_constant (addr, 1));
  operands[1] = change_address (operands[1], QImode, NULL_RTX);
  operands[3] = gen_lowpart (QImode, operands[0]);
  operands[0] = gen_lowpart (SImode, operands[0]);
  operands[2] = gen_reg_rtx (SImode);
}

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand4,
	operand3));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	gen_rtx_ASHIFTRT (SImode,
	operand0,
	GEN_INT (8))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand1,
	gen_rtx_SUBREG (QImode,
	operand2,
	0)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_storeinthi (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operand2;
  rtx operand3;
  rtx operands[4];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;

{
  HOST_WIDE_INT value = INTVAL (operands[1]);
  rtx addr = XEXP (operands[0], 0);
  enum rtx_code code = GET_CODE (addr);

  if ((code == PLUS && GET_CODE (XEXP (addr, 1)) != CONST_INT)
      || code == MINUS)
    addr = force_reg (SImode, addr);

  operands[1] = gen_reg_rtx (SImode);
  if (BYTES_BIG_ENDIAN)
    {
      emit_insn (gen_movsi (operands[1], GEN_INT ((value >> 8) & 255)));
      if ((value & 255) == ((value >> 8) & 255))
	operands[2] = operands[1];
      else
	{
	  operands[2] = gen_reg_rtx (SImode);
	  emit_insn (gen_movsi (operands[2], GEN_INT (value & 255)));
	}
    }
  else
    {
      emit_insn (gen_movsi (operands[1], GEN_INT (value & 255)));
      if ((value & 255) == ((value >> 8) & 255))
	operands[2] = operands[1];
      else
	{
	  operands[2] = gen_reg_rtx (SImode);
	  emit_insn (gen_movsi (operands[2], GEN_INT ((value >> 8) & 255)));
	}
    }

  operands[3] = change_address (operands[0], QImode, plus_constant (addr, 1));
  operands[0] = change_address (operands[0], QImode, NULL_RTX);
}

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SUBREG (QImode,
	operand1,
	0)));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand3,
	gen_rtx_SUBREG (QImode,
	operand2,
	0)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_storehi_single_op (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;

  if (! s_register_operand (operands[1], HImode))
    operands[1] = copy_to_mode_reg (HImode, operands[1]);

  operand0 = operands[0];
  operand1 = operands[1];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_movhi (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;

{
  if (! (reload_in_progress || reload_completed))
    {
      if (GET_CODE (operands[0]) == MEM)
	{
	  if (arm_arch4 && ! ARM_ARCH_RISCPC)
	    {
	      emit_insn (gen_storehi_single_op (operands[0], operands[1]));
	      DONE;
	    }
	  if (GET_CODE (operands[1]) == CONST_INT)
	    emit_insn (gen_storeinthi (operands[0], operands[1]));
	  else
	    {
	      if (GET_CODE (operands[1]) == MEM)
		operands[1] = force_reg (HImode, operands[1]);
	      if (BYTES_BIG_ENDIAN)
		emit_insn (gen_storehi_bigend (operands[1], operands[0]));
	      else
		emit_insn (gen_storehi (operands[1], operands[0]));
	    }
	  DONE;
	}
      /* Sign extend a constant, and keep it in an SImode reg.  */
      else if (GET_CODE (operands[1]) == CONST_INT)
	{
	  rtx reg = gen_reg_rtx (SImode);
	  HOST_WIDE_INT val = INTVAL (operands[1]) & 0xffff;

	  /* If the constant is already valid, leave it alone.  */
	  if (! const_ok_for_arm (val))
	    {
	      /* If setting all the top bits will make the constant 
		 loadable in a single instruction, then set them.  
		 Otherwise, sign extend the number.  */

	      if (const_ok_for_arm (~ (val | ~0xffff)))
		val |= ~0xffff;
	      else if (val & 0x8000)
		val |= ~0xffff;
	    }

	  emit_insn (gen_movsi (reg, GEN_INT (val)));
	  operands[1] = gen_rtx_SUBREG (HImode, reg, 0);
	}
      else if (! arm_arch4 || ARM_ARCH_RISCPC)
	{
	 /* Note: We do not have to worry about TARGET_SHORT_BY_BYTES
	    for v4 and up architectures because LDRH instructions will
	    be used to access the HI values, and these cannot generate
	    unaligned word access faults in the MMU.  */
	  if (GET_CODE (operands[1]) == MEM)
	    {
	      if (TARGET_SHORT_BY_BYTES)
		{
		  rtx base;
		  rtx offset = const0_rtx;
		  rtx reg = gen_reg_rtx (SImode);

		  if ((GET_CODE (base = XEXP (operands[1], 0)) == REG
		       || (GET_CODE (base) == PLUS
			   && GET_CODE (offset = XEXP (base, 1)) == CONST_INT
                           && ((INTVAL(offset) & 1) != 1)
			   && GET_CODE (base = XEXP (base, 0)) == REG))
		      && REGNO_POINTER_ALIGN (REGNO (base)) >= 4)
		    {
		      HOST_WIDE_INT new_offset = INTVAL (offset) & ~3;
		      rtx new;

		      new = gen_rtx_MEM (SImode,
					 plus_constant (base, new_offset));
	              MEM_COPY_ATTRIBUTES (new, operands[1]);
		      RTX_UNCHANGING_P (new) = RTX_UNCHANGING_P (operands[1]);
		      emit_insn (gen_movsi (reg, new));
		      if (((INTVAL (offset) & 2) != 0)
			  ^ (BYTES_BIG_ENDIAN ? 1 : 0))
			{
			  rtx reg2 = gen_reg_rtx (SImode);

			  emit_insn (gen_lshrsi3 (reg2, reg, GEN_INT (16)));
			  reg = reg2;
			}
		    }
		  else
		    emit_insn (gen_movhi_bytes (reg, operands[1]));

		  operands[1] = gen_lowpart (HImode, reg);
		}
	      else if (BYTES_BIG_ENDIAN)
		{
		  rtx base;
		  rtx offset = const0_rtx;

		  if ((GET_CODE (base = XEXP (operands[1], 0)) == REG
		       || (GET_CODE (base) == PLUS
			   && GET_CODE (offset = XEXP (base, 1)) == CONST_INT
			   && GET_CODE (base = XEXP (base, 0)) == REG))
		      && REGNO_POINTER_ALIGN (REGNO (base)) >= 4)
		    {
		      rtx reg = gen_reg_rtx (SImode);
		      rtx new;

		      if ((INTVAL (offset) & 2) == 2)
			{
			  HOST_WIDE_INT new_offset = INTVAL (offset) ^ 2;
			  new = gen_rtx_MEM (SImode,
					     plus_constant (base, new_offset));
                          MEM_COPY_ATTRIBUTES (new, operands[1]);
			  RTX_UNCHANGING_P (new) = RTX_UNCHANGING_P (operands[1]);
			  emit_insn (gen_movsi (reg, new));
			}
		      else
			{
			  new = gen_rtx_MEM (SImode, XEXP (operands[1], 0));
	                  MEM_COPY_ATTRIBUTES (new, operands[1]);
			  RTX_UNCHANGING_P (new)
			    = RTX_UNCHANGING_P (operands[1]);
			  emit_insn (gen_rotated_loadsi (reg, new));
			}

		      operands[1] = gen_lowpart (HImode, reg);
		    }
		  else
		    {
		      emit_insn (gen_movhi_bigend (operands[0], operands[1]));
		      DONE;
		    }
		}
	    }
	}
    }
  /* Handle loading a large integer during reload */
  else if (GET_CODE (operands[1]) == CONST_INT
	   && ! const_ok_for_arm (INTVAL (operands[1]))
	   && ! const_ok_for_arm (~INTVAL (operands[1])))
    {
      /* Writing a constant to memory needs a scratch, which should
	 be handled with SECONDARY_RELOADs.  */
      if (GET_CODE (operands[0]) != REG)
	abort ();

      operands[0] = gen_rtx_SUBREG (SImode, operands[0], 0);
      emit_insn (gen_movsi (operands[0], operands[1]));
      DONE;
    }
}

  operand0 = operands[0];
  operand1 = operands[1];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_rotated_loadsi (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_ROTATE (SImode,
	operand1,
	GEN_INT (16)));
}

rtx
gen_movhi_bytes (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx operand5;
  rtx operand6;
  rtx operands[7];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;

{
  rtx mem1, mem2;
  rtx addr = copy_to_mode_reg (SImode, XEXP (operands[1], 0));

  mem1 = gen_rtx_MEM (QImode, addr);
  MEM_COPY_ATTRIBUTES (mem1, operands[1]);
  RTX_UNCHANGING_P (mem1) = RTX_UNCHANGING_P (operands[1]);
  mem2 = gen_rtx_MEM (QImode, plus_constant (addr, 1));
  MEM_COPY_ATTRIBUTES (mem2, operands[1]);
  RTX_UNCHANGING_P (mem2) = RTX_UNCHANGING_P (operands[1]);
  operands[0] = gen_lowpart (SImode, operands[0]);
  operands[1] = mem1;
  operands[2] = gen_reg_rtx (SImode);
  operands[3] = gen_reg_rtx (SImode);
  operands[6] = mem2;

  if (BYTES_BIG_ENDIAN)
    {
      operands[4] = operands[2];
      operands[5] = operands[3];
    }
  else
    {
      operands[4] = operands[3];
      operands[5] = operands[2];
    }
}

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  operand5 = operands[5];
  operand6 = operands[6];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	gen_rtx_ZERO_EXTEND (SImode,
	operand1)));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand3,
	gen_rtx_ZERO_EXTEND (SImode,
	operand6)));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_IOR (SImode,
	gen_rtx_ASHIFT (SImode,
	operand4,
	GEN_INT (8)),
	operand5)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_movhi_bigend (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operand2;
  rtx operand3;
  rtx operands[4];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;

  operands[2] = gen_reg_rtx (SImode);
  operands[3] = gen_reg_rtx (SImode);

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	gen_rtx_ROTATE (SImode,
	gen_rtx_SUBREG (SImode,
	operand1,
	0),
	GEN_INT (16))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand3,
	gen_rtx_ASHIFTRT (SImode,
	operand2,
	GEN_INT (16))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_SUBREG (HImode,
	operand3,
	0)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_reload_outhi (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx operands[3];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;
  operands[2] = operand2;

  arm_reload_out_hi (operands);
  DONE;

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		operand0,
		operand1,
		operand2)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_reload_inhi (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx operands[3];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;
  operands[2] = operand2;

  arm_reload_in_hi (operands);
  DONE;

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		operand0,
		operand1,
		operand2)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_movqi (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;

  /* Everything except mem = const or mem = mem can be done easily */

  if (!(reload_in_progress || reload_completed))
    {
      if (GET_CODE (operands[1]) == CONST_INT)
	{
	  rtx reg = gen_reg_rtx (SImode);

	  emit_insn (gen_movsi (reg, operands[1]));
	  operands[1] = gen_rtx_SUBREG (QImode, reg, 0);
	}
      if (GET_CODE (operands[0]) == MEM)
	operands[1] = force_reg (QImode, operands[1]);
    }

  operand0 = operands[0];
  operand1 = operands[1];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_movsf (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;

  if (GET_CODE (operands[0]) == MEM)
    operands[1] = force_reg (SFmode, operands[1]);

  operand0 = operands[0];
  operand1 = operands[1];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_movdf (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;

  if (GET_CODE (operands[0]) == MEM)
    operands[1] = force_reg (DFmode, operands[1]);

  operand0 = operands[0];
  operand1 = operands[1];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_reload_outdf (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx operands[3];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;
  operands[2] = operand2;

{
  enum rtx_code code = GET_CODE (XEXP (operands[0], 0));

  if (code == REG)
    operands[2] = XEXP (operands[0], 0);
  else if (code == POST_INC || code == PRE_DEC)
    {
      operands[0] = gen_rtx_SUBREG (DImode, operands[0], 0);
      operands[1] = gen_rtx_SUBREG (DImode, operands[1], 0);
      emit_insn (gen_movdi (operands[0], operands[1]));
      DONE;
    }
  else if (code == PRE_INC)
    {
      rtx reg = XEXP (XEXP (operands[0], 0), 0);
      emit_insn (gen_addsi3 (reg, reg, GEN_INT (8)));
      operands[2] = reg;
    }
  else if (code == POST_DEC)
    operands[2] = XEXP (XEXP (operands[0], 0), 0);
  else
    emit_insn (gen_addsi3 (operands[2], XEXP (XEXP (operands[0], 0), 0),
			   XEXP (XEXP (operands[0], 0), 1)));

  emit_insn (gen_rtx_SET (VOIDmode, gen_rtx_MEM (DFmode, operands[2]),
				    operands[1]));

  if (code == POST_DEC)
    emit_insn (gen_addsi3 (operands[2], operands[2], GEN_INT (-8)));

  DONE;
}

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit (operand0);
  emit (operand1);
  emit (operand2);
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_movxf (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_SET (VOIDmode,
	operand0,
	operand1);
}

rtx
gen_load_multiple (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx operand3;
  rtx operands[4];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;
  operands[2] = operand2;

  /* Support only fixed point registers */
  if (GET_CODE (operands[2]) != CONST_INT
      || INTVAL (operands[2]) > 14
      || INTVAL (operands[2]) < 2
      || GET_CODE (operands[1]) != MEM
      || GET_CODE (operands[0]) != REG
      || REGNO (operands[0]) > 14
      || REGNO (operands[0]) + INTVAL (operands[2]) > 15)
    FAIL;

  operands[3]
    = arm_gen_load_multiple (REGNO (operands[0]), INTVAL (operands[2]),
			     force_reg (SImode, XEXP (operands[1], 0)),
			     TRUE, FALSE, RTX_UNCHANGING_P(operands[1]),
			     MEM_IN_STRUCT_P(operands[1]),
	                     MEM_SCALAR_P (operands[1]));

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (operand3);
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_store_multiple (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx operand3;
  rtx operands[4];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;
  operands[2] = operand2;

  /* Support only fixed point registers */
  if (GET_CODE (operands[2]) != CONST_INT
      || INTVAL (operands[2]) > 14
      || INTVAL (operands[2]) < 2
      || GET_CODE (operands[1]) != REG
      || GET_CODE (operands[0]) != MEM
      || REGNO (operands[1]) > 14
      || REGNO (operands[1]) + INTVAL (operands[2]) > 15)
    FAIL;

  operands[3]
    = arm_gen_store_multiple (REGNO (operands[1]), INTVAL (operands[2]),
			      force_reg (SImode, XEXP (operands[0], 0)),
			      TRUE, FALSE, RTX_UNCHANGING_P (operands[0]),
			      MEM_IN_STRUCT_P(operands[0]), 
	                      MEM_SCALAR_P (operands[0]));

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (operand3);
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_movstrqi (operand0, operand1, operand2, operand3)
     rtx operand0;
     rtx operand1;
     rtx operand2;
     rtx operand3;
{
  rtx operands[4];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;
  operands[2] = operand2;
  operands[3] = operand3;

  if (arm_gen_movstrqi (operands))
    DONE;
  FAIL;

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit (operand0);
  emit (operand1);
  emit (operand2);
  emit (operand3);
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_cmpsi (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;

{
  arm_compare_op0 = operands[0];
  arm_compare_op1 = operands[1];
  DONE;
}

  operand0 = operands[0];
  operand1 = operands[1];
  emit (operand0);
  emit (operand1);
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_cmpsf (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;

{
  arm_compare_op0 = operands[0];
  arm_compare_op1 = operands[1];
  DONE;
}

  operand0 = operands[0];
  operand1 = operands[1];
  emit (operand0);
  emit (operand1);
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_cmpdf (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;

{
  arm_compare_op0 = operands[0];
  arm_compare_op1 = operands[1];
  DONE;
}

  operand0 = operands[0];
  operand1 = operands[1];
  emit (operand0);
  emit (operand1);
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_cmpxf (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;

{
  arm_compare_op0 = operands[0];
  arm_compare_op1 = operands[1];
  DONE;
}

  operand0 = operands[0];
  operand1 = operands[1];
  emit (operand0);
  emit (operand1);
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_beq (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;

{
  operands[1] = gen_compare_reg (EQ, arm_compare_op0, arm_compare_op1);
}

  operand0 = operands[0];
  operand1 = operands[1];
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_EQ (VOIDmode,
	operand1,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_bne (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;

{
  operands[1] = gen_compare_reg (NE, arm_compare_op0, arm_compare_op1);
}

  operand0 = operands[0];
  operand1 = operands[1];
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_NE (VOIDmode,
	operand1,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_bgt (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;

{
  operands[1] = gen_compare_reg (GT, arm_compare_op0, arm_compare_op1);
}

  operand0 = operands[0];
  operand1 = operands[1];
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_GT (VOIDmode,
	operand1,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_ble (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;

{
  operands[1] = gen_compare_reg (LE, arm_compare_op0, arm_compare_op1);
}

  operand0 = operands[0];
  operand1 = operands[1];
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_LE (VOIDmode,
	operand1,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_bge (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;

{
  operands[1] = gen_compare_reg (GE, arm_compare_op0, arm_compare_op1);
}

  operand0 = operands[0];
  operand1 = operands[1];
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_GE (VOIDmode,
	operand1,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_blt (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;

{
  operands[1] = gen_compare_reg (LT, arm_compare_op0, arm_compare_op1);
}

  operand0 = operands[0];
  operand1 = operands[1];
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_LT (VOIDmode,
	operand1,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_bgtu (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;

{
  operands[1] = gen_compare_reg (GTU, arm_compare_op0, arm_compare_op1);
}

  operand0 = operands[0];
  operand1 = operands[1];
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_GTU (VOIDmode,
	operand1,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_bleu (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;

{
  operands[1] = gen_compare_reg (LEU, arm_compare_op0, arm_compare_op1);
}

  operand0 = operands[0];
  operand1 = operands[1];
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_LEU (VOIDmode,
	operand1,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_bgeu (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;

{
  operands[1] = gen_compare_reg (GEU, arm_compare_op0, arm_compare_op1);
}

  operand0 = operands[0];
  operand1 = operands[1];
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_GEU (VOIDmode,
	operand1,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_bltu (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;

{
  operands[1] = gen_compare_reg (LTU, arm_compare_op0, arm_compare_op1);
}

  operand0 = operands[0];
  operand1 = operands[1];
  emit_jump_insn (gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_LTU (VOIDmode,
	operand1,
	const0_rtx),
	gen_rtx_LABEL_REF (VOIDmode,
	operand0),
	pc_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_seq (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;

{
  operands[1] = gen_compare_reg (EQ, arm_compare_op0, arm_compare_op1);
}

  operand0 = operands[0];
  operand1 = operands[1];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_EQ (SImode,
	operand1,
	const0_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_sne (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;

{
  operands[1] = gen_compare_reg (NE, arm_compare_op0, arm_compare_op1);
}

  operand0 = operands[0];
  operand1 = operands[1];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_NE (SImode,
	operand1,
	const0_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_sgt (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;

{
  operands[1] = gen_compare_reg (GT, arm_compare_op0, arm_compare_op1);
}

  operand0 = operands[0];
  operand1 = operands[1];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_GT (SImode,
	operand1,
	const0_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_sle (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;

{
  operands[1] = gen_compare_reg (LE, arm_compare_op0, arm_compare_op1);
}

  operand0 = operands[0];
  operand1 = operands[1];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_LE (SImode,
	operand1,
	const0_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_sge (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;

{
  operands[1] = gen_compare_reg (GE, arm_compare_op0, arm_compare_op1);
}

  operand0 = operands[0];
  operand1 = operands[1];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_GE (SImode,
	operand1,
	const0_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_slt (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;

{
  operands[1] = gen_compare_reg (LT, arm_compare_op0, arm_compare_op1);
}

  operand0 = operands[0];
  operand1 = operands[1];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_LT (SImode,
	operand1,
	const0_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_sgtu (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;

{
  operands[1] = gen_compare_reg (GTU, arm_compare_op0, arm_compare_op1);
}

  operand0 = operands[0];
  operand1 = operands[1];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_GTU (SImode,
	operand1,
	const0_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_sleu (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;

{
  operands[1] = gen_compare_reg (LEU, arm_compare_op0, arm_compare_op1);
}

  operand0 = operands[0];
  operand1 = operands[1];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_LEU (SImode,
	operand1,
	const0_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_sgeu (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;

{
  operands[1] = gen_compare_reg (GEU, arm_compare_op0, arm_compare_op1);
}

  operand0 = operands[0];
  operand1 = operands[1];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_GEU (SImode,
	operand1,
	const0_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_sltu (operand0)
     rtx operand0;
{
  rtx operand1;
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;

{
  operands[1] = gen_compare_reg (LTU, arm_compare_op0, arm_compare_op1);
}

  operand0 = operands[0];
  operand1 = operands[1];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_LTU (SImode,
	operand1,
	const0_rtx)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_movsicc (operand0, operand1, operand2, operand3)
     rtx operand0;
     rtx operand1;
     rtx operand2;
     rtx operand3;
{
  rtx operands[4];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;
  operands[2] = operand2;
  operands[3] = operand3;

{
  enum rtx_code code = GET_CODE (operands[1]);
  rtx ccreg = gen_compare_reg (code, arm_compare_op0, arm_compare_op1);

  operands[1] = gen_rtx (code, VOIDmode, ccreg, const0_rtx);
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_IF_THEN_ELSE (SImode,
	operand1,
	operand2,
	operand3)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_movsfcc (operand0, operand1, operand2, operand3)
     rtx operand0;
     rtx operand1;
     rtx operand2;
     rtx operand3;
{
  rtx operands[4];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;
  operands[2] = operand2;
  operands[3] = operand3;

{
  enum rtx_code code = GET_CODE (operands[1]);
  rtx ccreg;

  /* When compiling for SOFT_FLOAT, ensure both arms are in registers. 
     Otherwise, ensure it is a valid FP add operand */
  if ((! TARGET_HARD_FLOAT)
      || (! fpu_add_operand (operands[3], SFmode)))
    operands[3] = force_reg (SFmode, operands[3]);

  ccreg = gen_compare_reg (code, arm_compare_op0, arm_compare_op1);

  operands[1] = gen_rtx (code, VOIDmode, ccreg, const0_rtx);
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_IF_THEN_ELSE (SFmode,
	operand1,
	operand2,
	operand3)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_movdfcc (operand0, operand1, operand2, operand3)
     rtx operand0;
     rtx operand1;
     rtx operand2;
     rtx operand3;
{
  rtx operands[4];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;
  operands[2] = operand2;
  operands[3] = operand3;

{
  enum rtx_code code = GET_CODE (operands[1]);
  rtx ccreg = gen_compare_reg (code, arm_compare_op0, arm_compare_op1);

  operands[1] = gen_rtx (code, VOIDmode, ccreg, const0_rtx);
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_IF_THEN_ELSE (DFmode,
	operand1,
	operand2,
	operand3)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_jump (operand0)
     rtx operand0;
{
  return gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_LABEL_REF (VOIDmode,
	operand0));
}

rtx
gen_call (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_CALL (VOIDmode,
	operand0,
	operand1),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_REG (SImode,
	14))));
}

rtx
gen_call_value (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_CALL (VOIDmode,
	operand1,
	operand2)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_REG (SImode,
	14))));
}

rtx
gen_return ()
{
  return gen_rtx_RETURN (VOIDmode);
}

rtx
gen_untyped_call (operand0, operand1, operand2)
     rtx operand0;
     rtx operand1;
     rtx operand2;
{
  rtx operands[3];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;
  operands[2] = operand2;

{
  int i;

  emit_call_insn (gen_call (operands[0], const0_rtx, NULL, const0_rtx));

  for (i = 0; i < XVECLEN (operands[2], 0); i++)
    {
      rtx set = XVECEXP (operands[2], 0, i);
      emit_move_insn (SET_DEST (set), SET_SRC (set));
    }

  /* The optimizer does not know that the call sets the function value
     registers we stored in the result block.  We avoid problems by
     claiming that all hard registers are used and clobbered at this
     point.  */
  emit_insn (gen_blockage ());

  DONE;
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_call_insn (gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (3,
		gen_rtx_CALL (VOIDmode,
	operand0,
	const0_rtx),
		operand1,
		operand2)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_blockage ()
{
  return gen_rtx_UNSPEC_VOLATILE (VOIDmode,
	gen_rtvec (1,
		const0_rtx),
	0);
}

rtx
gen_casesi (operand0, operand1, operand2, operand3, operand4)
     rtx operand0;
     rtx operand1;
     rtx operand2;
     rtx operand3;
     rtx operand4;
{
  rtx operands[5];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;
  operands[2] = operand2;
  operands[3] = operand3;
  operands[4] = operand4;

{
  rtx reg;
  if (operands[1] != const0_rtx)
    {
      reg = gen_reg_rtx (SImode);
      emit_insn (gen_addsi3 (reg, operands[0],
			     GEN_INT (-INTVAL (operands[1]))));
      operands[0] = reg;
    }

  if (! const_ok_for_arm (INTVAL (operands[2])))
    operands[2] = force_reg (SImode, operands[2]);

  emit_jump_insn (gen_casesi_internal (operands[0], operands[2], operands[3],
				       operands[4]));
  DONE;
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  emit (operand0);
  emit (operand1);
  emit (operand2);
  emit (operand3);
  emit (operand4);
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_casesi_internal (operand0, operand1, operand2, operand3)
     rtx operand0;
     rtx operand1;
     rtx operand2;
     rtx operand3;
{
  return gen_rtx_PARALLEL (VOIDmode,
	gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	pc_rtx,
	gen_rtx_IF_THEN_ELSE (VOIDmode,
	gen_rtx_LEU (VOIDmode,
	operand0,
	operand1),
	gen_rtx_MEM (SImode,
	gen_rtx_PLUS (SImode,
	gen_rtx_MULT (SImode,
	operand0,
	GEN_INT (4)),
	gen_rtx_LABEL_REF (VOIDmode,
	operand2))),
	gen_rtx_LABEL_REF (VOIDmode,
	operand3))),
		gen_rtx_USE (VOIDmode,
	gen_rtx_LABEL_REF (VOIDmode,
	operand2))));
}

rtx
gen_indirect_jump (operand0)
     rtx operand0;
{
  return gen_rtx_SET (VOIDmode,
	pc_rtx,
	operand0);
}

rtx
gen_nop ()
{
  return const0_rtx;
}

rtx
gen_movcond (operand0, operand1, operand2, operand3, operand4, operand5)
     rtx operand0;
     rtx operand1;
     rtx operand2;
     rtx operand3;
     rtx operand4;
     rtx operand5;
{
  return gen_rtx_PARALLEL (VOIDmode, gen_rtvec (2,
		gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_IF_THEN_ELSE (SImode,
	gen_rtx (GET_CODE (operand5), VOIDmode,
		operand3,
		operand4),
	operand1,
	operand2)),
		gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_REG (CCmode,
	24))));
}

rtx
gen_split_378 (operands)
     rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx operand5;
  rtx _val = 0;
  start_sequence ();

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  operand5 = operands[5];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand5,
	gen_rtx_NOT (SImode,
	gen_rtx_ASHIFTRT (SImode,
	operand1,
	GEN_INT (31)))));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_AND (SImode,
	gen_rtx (GET_CODE (operand2), GET_MODE (operand2),
		operand3,
		operand4),
	operand5)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_split_379 (operands)
     rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx _val = 0;
  start_sequence ();

  operands[1] = GEN_INT (((unsigned long) INTVAL (operands[1])) >> 24);

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	gen_rtx_ZERO_EXTEND (SImode,
	operand0)));
  emit_insn (gen_rtx_SET (VOIDmode,
	gen_rtx_REG (CCmode,
	24),
	gen_rtx_COMPARE (CCmode,
	operand2,
	operand1)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_prologue ()
{
  rtx _val = 0;
  start_sequence ();

  arm_expand_prologue ();
  DONE;

  emit_insn (gen_rtx_CLOBBER (VOIDmode,
	const0_rtx));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_split_381 (operands)
     rtx *operands;
{
  rtx operand0;
  rtx operand1;
  rtx operand2;
  rtx operand3;
  rtx operand4;
  rtx operand5;
  rtx operand6;
  rtx operand7;
  rtx _val = 0;
  start_sequence ();

{
  enum machine_mode mode = SELECT_CC_MODE (GET_CODE (operands[1]), operands[2],
					   operands[3]);

  operands[6] = gen_rtx_REG (mode, 24);
  operands[7] = gen_rtx_COMPARE (mode, operands[2], operands[3]);
}

  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  operand3 = operands[3];
  operand4 = operands[4];
  operand5 = operands[5];
  operand6 = operands[6];
  operand7 = operands[7];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand6,
	operand7));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_IF_THEN_ELSE (SImode,
	gen_rtx (GET_CODE (operand1), GET_MODE (operand1),
		operand6,
		const0_rtx),
	operand4,
	operand5)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_consttable_4 (operand0)
     rtx operand0;
{
  return gen_rtx_UNSPEC_VOLATILE (VOIDmode,
	gen_rtvec (1,
		operand0),
	2);
}

rtx
gen_consttable_8 (operand0)
     rtx operand0;
{
  return gen_rtx_UNSPEC_VOLATILE (VOIDmode,
	gen_rtvec (1,
		operand0),
	3);
}

rtx
gen_consttable_end ()
{
  return gen_rtx_UNSPEC_VOLATILE (VOIDmode,
	gen_rtvec (1,
		const0_rtx),
	4);
}

rtx
gen_align_4 ()
{
  return gen_rtx_UNSPEC_VOLATILE (VOIDmode,
	gen_rtvec (1,
		const0_rtx),
	5);
}

rtx
gen_save_stack_function ()
{
  return gen_rtx_USE (VOIDmode,
	const0_rtx);
}

rtx
gen_restore_stack_function ()
{
  return gen_rtx_USE (VOIDmode,
	const0_rtx);
}

rtx
gen_save_stack_block (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;

{
#if 1
  /* Inline version of __arm_alloca_block_init.  */
  rtx scratch_reg = gen_reg_rtx (SImode);
  rtx label = gen_rtx_SYMBOL_REF (Pmode, "__arm_alloca_st");

  emit_move_insn (scratch_reg, gen_rtx_MEM (SImode, label));
  emit_insn (gen_addsi3 (scratch_reg, scratch_reg, const1_rtx));
  emit_move_insn (gen_rtx_MEM (SImode, label), scratch_reg);
#else
  rtx r0_rtx = gen_rtx (REG, SImode, 0);
  rtx funexp = gen_rtx (SYMBOL_REF, Pmode, "___arm_alloca_block_init");

  emit_call_insn (gen_call_value (r0_rtx,
  		 		  gen_rtx (MEM, FUNCTION_MODE, funexp),
  		 		  const0_rtx));
  emit_move_insn (operands[0], r0_rtx);
#endif
  DONE;
}
  operand0 = operands[0];
  operand1 = operands[1];
  emit (operand0);
  emit (operand1);
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_save_stack_nonlocal (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;

{
#if 1
  /* Inline version of __arm_alloca_block_init.  */
  rtx scratch_reg = gen_reg_rtx (SImode);
  rtx label = gen_rtx_SYMBOL_REF (Pmode, "__arm_alloca_st");

  emit_move_insn (scratch_reg, gen_rtx_MEM (SImode, label));
  emit_insn (gen_addsi3 (scratch_reg, scratch_reg, const1_rtx));
  emit_move_insn (gen_rtx_MEM (SImode, label), scratch_reg);
#else
  rtx r0_rtx = gen_rtx (REG, SImode, 0);
  rtx funexp = gen_rtx (SYMBOL_REF, Pmode, "___arm_alloca_block_init");

  emit_call_insn (gen_call_value (r0_rtx,
  		 		  gen_rtx (MEM, FUNCTION_MODE, funexp),
  		 		  const0_rtx));
  emit_move_insn (operands[0], r0_rtx);
#endif
  DONE;
}
  operand0 = operands[0];
  operand1 = operands[1];
  emit (operand0);
  emit (operand1);
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_restore_stack_block (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operand2;
  rtx operands[3];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;

{
  rtx r0_rtx = gen_rtx (REG, SImode, 0);
  rtx funexp = gen_rtx (SYMBOL_REF, Pmode, "___arm_alloca_block_free");

  emit_move_insn (r0_rtx, operands[1]);
  emit_insn (gen_rtx (USE, VOIDmode, r0_rtx));
  emit_call_insn (gen_call (gen_rtx (MEM, FUNCTION_MODE, funexp), GEN_INT (4)));
  DONE;
}
  operand0 = operands[0];
  operand1 = operands[1];
  operand2 = operands[2];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand2,
	gen_rtx_MEM (SImode,
	operand0)));
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	operand1));
  emit_insn (gen_rtx_SET (VOIDmode,
	gen_rtx_MEM (SImode,
	operand0),
	operand2));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_restore_stack_nonlocal (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;

{
  rtx r0_rtx = gen_rtx (REG, SImode, 0);
  rtx funexp = gen_rtx (SYMBOL_REF, Pmode, "___arm_alloca_block_free");

  emit_move_insn (r0_rtx, operands[1]);
  emit_insn (gen_rtx (USE, VOIDmode, r0_rtx));
  emit_call_insn (gen_call (gen_rtx (MEM, FUNCTION_MODE, funexp), GEN_INT (4)));
  DONE;
}
  operand0 = operands[0];
  operand1 = operands[1];
  emit (operand0);
  emit (operand1);
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}

rtx
gen_allocate_stack (operand0, operand1)
     rtx operand0;
     rtx operand1;
{
  rtx operands[2];
  rtx _val = 0;
  start_sequence ();
  operands[0] = operand0;
  operands[1] = operand1;

{
  rtx r0_rtx = gen_rtx (REG, SImode, 0);
  rtx funexp = gen_rtx (SYMBOL_REF, Pmode, "___arm_alloca_alloc");

  emit_move_insn (r0_rtx, operands[1]);
  emit_insn (gen_rtx (USE, VOIDmode, r0_rtx));
  emit_call_insn (gen_call_value (r0_rtx,
				  gen_rtx (MEM, FUNCTION_MODE, funexp),
				  GEN_INT (4)));
  emit_move_insn (operands[0], r0_rtx);

  DONE;
}
  operand0 = operands[0];
  operand1 = operands[1];
  emit_insn (gen_rtx_SET (VOIDmode,
	operand0,
	gen_rtx_MINUS (SImode,
	gen_rtx_REG (SImode,
	13),
	operand1)));
  emit_insn (gen_rtx_SET (VOIDmode,
	gen_rtx_REG (SImode,
	13),
	gen_rtx_MINUS (SImode,
	gen_rtx_REG (SImode,
	13),
	operand1)));
  emit_insn (gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_REG (SImode,
	0)));
  emit_insn (gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_REG (SImode,
	14)));
  _val = gen_sequence ();
  end_sequence ();
  return _val;
}



void
add_clobbers (pattern, insn_code_number)
     rtx pattern;
     int insn_code_number;
{
  switch (insn_code_number)
    {
    case 277:
    case 276:
    case 275:
    case 274:
    case 272:
    case 271:
      XVECEXP (pattern, 0, 1) = gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_REG (SImode,
	14));
      break;

    case 338:
    case 299:
    case 296:
    case 293:
    case 290:
    case 139:
    case 119:
    case 116:
    case 94:
    case 86:
    case 76:
    case 50:
    case 49:
    case 46:
    case 43:
      XVECEXP (pattern, 0, 1) = gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_SCRATCH (SImode));
      break;

    case 336:
    case 334:
    case 332:
    case 330:
    case 328:
    case 326:
    case 324:
    case 322:
    case 320:
    case 318:
    case 316:
    case 314:
    case 312:
    case 310:
    case 309:
    case 308:
    case 305:
    case 304:
    case 302:
    case 127:
    case 126:
    case 120:
    case 108:
    case 107:
    case 106:
    case 105:
    case 104:
    case 103:
    case 29:
    case 28:
    case 27:
    case 26:
    case 25:
    case 24:
    case 2:
    case 1:
    case 0:
      XVECEXP (pattern, 0, 1) = gen_rtx_CLOBBER (VOIDmode,
	gen_rtx_REG (CCmode,
	24));
      break;

    default:
      abort ();
    }
}

void
init_mov_optab ()
{
#ifdef HAVE_movcc_noov
  if (HAVE_movcc_noov)
    mov_optab->handlers[(int) CC_NOOVmode].insn_code = CODE_FOR_movcc_noov;
#endif
#ifdef HAVE_movcc_z
  if (HAVE_movcc_z)
    mov_optab->handlers[(int) CC_Zmode].insn_code = CODE_FOR_movcc_z;
#endif
#ifdef HAVE_movcc_swp
  if (HAVE_movcc_swp)
    mov_optab->handlers[(int) CC_SWPmode].insn_code = CODE_FOR_movcc_swp;
#endif
#ifdef HAVE_movccfp
  if (HAVE_movccfp)
    mov_optab->handlers[(int) CCFPmode].insn_code = CODE_FOR_movccfp;
#endif
#ifdef HAVE_movccfpe
  if (HAVE_movccfpe)
    mov_optab->handlers[(int) CCFPEmode].insn_code = CODE_FOR_movccfpe;
#endif
#ifdef HAVE_movcc_dne
  if (HAVE_movcc_dne)
    mov_optab->handlers[(int) CC_DNEmode].insn_code = CODE_FOR_movcc_dne;
#endif
#ifdef HAVE_movcc_deq
  if (HAVE_movcc_deq)
    mov_optab->handlers[(int) CC_DEQmode].insn_code = CODE_FOR_movcc_deq;
#endif
#ifdef HAVE_movcc_dle
  if (HAVE_movcc_dle)
    mov_optab->handlers[(int) CC_DLEmode].insn_code = CODE_FOR_movcc_dle;
#endif
#ifdef HAVE_movcc_dlt
  if (HAVE_movcc_dlt)
    mov_optab->handlers[(int) CC_DLTmode].insn_code = CODE_FOR_movcc_dlt;
#endif
#ifdef HAVE_movcc_dge
  if (HAVE_movcc_dge)
    mov_optab->handlers[(int) CC_DGEmode].insn_code = CODE_FOR_movcc_dge;
#endif
#ifdef HAVE_movcc_dgt
  if (HAVE_movcc_dgt)
    mov_optab->handlers[(int) CC_DGTmode].insn_code = CODE_FOR_movcc_dgt;
#endif
#ifdef HAVE_movcc_dleu
  if (HAVE_movcc_dleu)
    mov_optab->handlers[(int) CC_DLEUmode].insn_code = CODE_FOR_movcc_dleu;
#endif
#ifdef HAVE_movcc_dltu
  if (HAVE_movcc_dltu)
    mov_optab->handlers[(int) CC_DLTUmode].insn_code = CODE_FOR_movcc_dltu;
#endif
#ifdef HAVE_movcc_dgeu
  if (HAVE_movcc_dgeu)
    mov_optab->handlers[(int) CC_DGEUmode].insn_code = CODE_FOR_movcc_dgeu;
#endif
#ifdef HAVE_movcc_dgtu
  if (HAVE_movcc_dgtu)
    mov_optab->handlers[(int) CC_DGTUmode].insn_code = CODE_FOR_movcc_dgtu;
#endif
#ifdef HAVE_movcc_c
  if (HAVE_movcc_c)
    mov_optab->handlers[(int) CC_Cmode].insn_code = CODE_FOR_movcc_c;
#endif
}
