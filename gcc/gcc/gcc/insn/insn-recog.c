/* Generated automatically by the program `genrecog'
from the machine description file `md'.  */

#include "config.h"
#include "system.h"
#include "rtl.h"
#include "insn-config.h"
#include "recog.h"
#include "real.h"
#include "output.h"
#include "flags.h"

extern rtx gen_split_4 ();
extern rtx gen_split_17 ();
extern rtx gen_split_32 ();
extern rtx gen_split_74 ();
extern rtx gen_split_92 ();
extern rtx gen_split_101 ();
extern rtx gen_split_154 ();
extern rtx gen_split_155 ();
extern rtx gen_split_158 ();
extern rtx gen_split_163 ();
extern rtx gen_split_164 ();
extern rtx gen_split_167 ();
extern rtx gen_split_170 ();
extern rtx gen_split_177 ();
extern rtx gen_split_378 ();
extern rtx gen_split_379 ();
extern rtx gen_split_381 ();

/* `recog' contains a decision tree
   that recognizes whether the rtx X0 is a valid instruction.

   recog returns -1 if the rtx is not valid.
   If the rtx is valid, recog returns a nonnegative number
   which is the insn code number for the pattern that matched.
   This is the same as the order in the machine description of
   the entry that matched.  This number can be used as an index into various
   insn_* tables, such as insn_templates, insn_outfun, and insn_n_operands
   (found in insn-output.c).

   The third argument to recog is an optional pointer to an int.
   If present, recog will accept a pattern if it matches except for
   missing CLOBBER expressions at the end.  In that case, the value
   pointed to by the optional pointer will be set to the number of
   CLOBBERs that need to be added (it should be initialized to zero by
   the caller).  If it is set nonzero, the caller should allocate a
   PARALLEL of the appropriate size, copy the initial entries, and call
   add_clobbers (found in insn-emit.c) to fill in the CLOBBERs.

   The function split_insns returns 0 if the rtl could not
   be split or the split rtl in a SEQUENCE if it can be.*/

#define operands recog_operand

int
recog_1 (x0, insn, pnum_clobbers)
     register rtx x0;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  register rtx *ro = &recog_operand[0];
  register rtx x1 ATTRIBUTE_UNUSED, x2 ATTRIBUTE_UNUSED, x3 ATTRIBUTE_UNUSED, x4 ATTRIBUTE_UNUSED, x5 ATTRIBUTE_UNUSED, x6 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;

  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) != SImode)
    goto ret0;
  switch (GET_CODE (x3))
    {
    default:
      break;
    case PLUS:
      goto L398;
    case MINUS:
      goto L302;
    case MULT:
      goto L361;
    case AND:
      goto L705;
    case IOR:
      goto L766;
    case XOR:
      goto L818;
    case NOT:
      goto L981;
    }
  L944: ATTRIBUTE_UNUSED_LABEL
  if (shift_operator (x3, SImode))
    {
      ro[3] = x3;
      goto L945;
    }
  goto ret0;

  L398: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) != SImode)
    goto ret0;
  if (GET_CODE (x4) == MULT && 1)
    goto L399;
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L59;
    }
  goto ret0;

  L399: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      ro[2] = x5;
      goto L400;
    }
  goto ret0;

  L400: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (s_register_operand (x5, SImode))
    {
      ro[1] = x5;
      goto L401;
    }
  goto ret0;

  L401: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (s_register_operand (x4, SImode))
    {
      ro[3] = x4;
      goto L402;
    }
  goto ret0;

  L402: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L403;
  goto ret0;

  L403: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  switch (GET_CODE (x1))
    {
    default:
      break;
    case SET:
      goto L404;
    case CLOBBER:
      goto L421;
    }
  goto ret0;

  L404: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L405;
    }
  goto ret0;

  L405: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == PLUS && 1)
    goto L406;
  goto ret0;

  L406: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode && GET_CODE (x3) == MULT && 1)
    goto L407;
  goto ret0;

  L407: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, ro[2]) && 1)
    goto L408;
  goto ret0;

  L408: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, ro[1]) && 1)
    goto L409;
  goto ret0;

  L409: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[3]) && 1)
    return 45;
  goto ret0;

  L421: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      ro[0] = x2;
      return 46;
    }
  goto ret0;

  L59: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_add_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L60;
    }
  goto ret0;

  L60: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L61;
  goto ret0;

  L61: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L62;
  goto ret0;

  L62: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L63;
    }
  goto ret0;

  L63: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == PLUS && 1)
    goto L64;
  goto ret0;

  L64: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L65;
  goto ret0;

  L65: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[2]) && 1)
    return 6;
  goto ret0;

  L302: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (arm_rhs_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L303;
    }
  goto ret0;

  L303: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L304;
    }
  goto ret0;

  L304: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L305;
  goto ret0;

  L305: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L306;
  goto ret0;

  L306: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L307;
    }
  goto ret0;

  L307: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == MINUS && 1)
    goto L308;
  goto ret0;

  L308: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L309;
  goto ret0;

  L309: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[2]) && 1)
    return 33;
  goto ret0;

  L361: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L362;
    }
  goto L944;

  L362: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L363;
    }
  goto L944;

  L363: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L364;
  x3 = XEXP (x2, 0);
  goto L944;

  L364: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  switch (GET_CODE (x1))
    {
    default:
      break;
    case SET:
      goto L365;
    case CLOBBER:
      goto L378;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L944;

  L365: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L366;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L944;

  L366: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == MULT && 1)
    goto L367;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L944;

  L367: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[2]) && 1)
    goto L368;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L944;

  L368: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[1]) && 1)
    return 42;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L944;

  L378: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      ro[0] = x2;
      return 43;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L944;

  L705: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) != SImode)
    goto ret0;
  if (GET_CODE (x4) == NOT && 1)
    goto L706;
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L626;
    }
  L638: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x4, SImode))
    {
      ro[0] = x4;
      goto L639;
    }
  goto ret0;

  L706: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      ro[2] = x5;
      goto L707;
    }
  goto ret0;

  L707: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L708;
    }
  goto ret0;

  L708: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L709;
  goto ret0;

  L709: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  switch (GET_CODE (x1))
    {
    default:
      break;
    case SET:
      goto L710;
    case CLOBBER:
      goto L725;
    }
  goto ret0;

  L710: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L711;
    }
  goto ret0;

  L711: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == AND && 1)
    goto L712;
  goto ret0;

  L712: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode && GET_CODE (x3) == NOT && 1)
    goto L713;
  goto ret0;

  L713: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, ro[2]) && 1)
    goto L714;
  goto ret0;

  L714: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[1]) && 1)
    return 85;
  goto ret0;

  L725: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      ro[0] = x2;
      return 86;
    }
  goto ret0;

  L626: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_not_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L627;
    }
  x4 = XEXP (x3, 0);
  goto L638;

  L627: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L628;
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L638;

  L628: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L629;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L638;

  L629: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L630;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L638;

  L630: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == AND && 1)
    goto L631;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L638;

  L631: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L632;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L638;

  L632: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[2]) && 1)
    return 75;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L638;

  L639: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_not_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L640;
    }
  goto ret0;

  L640: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L641;
  goto ret0;

  L641: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L642;
  goto ret0;

  L642: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      ro[3] = x2;
      return 76;
    }
  goto ret0;

  L766: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L767;
    }
  goto ret0;

  L767: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L768;
    }
  goto ret0;

  L768: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L769;
  goto ret0;

  L769: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  switch (GET_CODE (x1))
    {
    default:
      break;
    case SET:
      goto L770;
    case CLOBBER:
      goto L783;
    }
  goto ret0;

  L770: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L771;
    }
  goto ret0;

  L771: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == IOR && 1)
    goto L772;
  goto ret0;

  L772: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L773;
  goto ret0;

  L773: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[2]) && 1)
    return 93;
  goto ret0;

  L783: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      ro[0] = x2;
      return 94;
    }
  goto ret0;

  L818: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L819;
    }
  goto ret0;

  L819: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L820;
    }
  goto ret0;

  L820: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L821;
  goto ret0;

  L821: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L822;
  goto ret0;

  L822: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L823;
    }
  goto ret0;

  L823: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == XOR && 1)
    goto L824;
  goto ret0;

  L824: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L825;
  goto ret0;

  L825: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[2]) && 1)
    return 99;
  goto ret0;

  L981: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) != SImode)
    goto ret0;
  if (shift_operator (x4, SImode))
    {
      ro[3] = x4;
      goto L982;
    }
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L1114;
    }
  goto ret0;

  L982: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      ro[1] = x5;
      goto L983;
    }
  goto ret0;

  L983: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (arm_rhs_operand (x5, SImode))
    {
      ro[2] = x5;
      goto L984;
    }
  goto ret0;

  L984: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L985;
  goto ret0;

  L985: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  switch (GET_CODE (x1))
    {
    default:
      break;
    case SET:
      goto L986;
    case CLOBBER:
      goto L1001;
    }
  goto ret0;

  L986: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L987;
    }
  goto ret0;

  L987: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == NOT && 1)
    goto L988;
  goto ret0;

  L988: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[3]) && 1)
    {
      ro[3] = x3;
      goto L989;
    }
  goto ret0;

  L989: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, ro[1]) && 1)
    goto L990;
  goto ret0;

  L990: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, ro[2]) && 1)
    return 118;
  goto ret0;

  L1001: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      ro[0] = x2;
      return 119;
    }
  goto ret0;

  L1114: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L1115;
  goto ret0;

  L1115: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  switch (GET_CODE (x1))
    {
    default:
      break;
    case SET:
      goto L1116;
    case CLOBBER:
      goto L1127;
    }
  goto ret0;

  L1116: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L1117;
    }
  goto ret0;

  L1117: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == NOT && 1)
    goto L1118;
  goto ret0;

  L1118: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    return 138;
  goto ret0;

  L1127: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      ro[0] = x2;
      return 139;
    }
  goto ret0;

  L945: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L946;
    }
  goto ret0;

  L946: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L947;
    }
  goto ret0;

  L947: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L948;
  goto ret0;

  L948: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  switch (GET_CODE (x1))
    {
    default:
      break;
    case SET:
      goto L949;
    case CLOBBER:
      goto L962;
    }
  goto ret0;

  L949: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L950;
    }
  goto ret0;

  L950: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (rtx_equal_p (x2, ro[3]) && 1)
    {
      ro[3] = x2;
      goto L951;
    }
  goto ret0;

  L951: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L952;
  goto ret0;

  L952: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[2]) && 1)
    return 115;
  goto ret0;

  L962: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      ro[0] = x2;
      return 116;
    }
  goto ret0;
 ret0: return -1;
}

int
recog_2 (x0, insn, pnum_clobbers)
     register rtx x0;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  register rtx *ro = &recog_operand[0];
  register rtx x1 ATTRIBUTE_UNUSED, x2 ATTRIBUTE_UNUSED, x3 ATTRIBUTE_UNUSED, x4 ATTRIBUTE_UNUSED, x5 ATTRIBUTE_UNUSED, x6 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;

  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) != SImode)
    goto ret0;
  switch (GET_CODE (x2))
    {
    default:
      break;
    case TRUNCATE:
      goto L449;
    case SMAX:
      goto L857;
    case SMIN:
      goto L870;
    case UMAX:
      goto L883;
    case UMIN:
      goto L896;
    case ABS:
      goto L1046;
    case NEG:
      goto L1057;
    case PLUS:
    case MINUS:
    case AND:
    case IOR:
    case XOR:
      if (shiftable_operator (x2, SImode))
	{
	  ro[4] = x2;
	  goto L922;
	}
    }
  goto ret0;

  L449: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == DImode && GET_CODE (x3) == LSHIFTRT && 1)
    goto L450;
  goto ret0;

  L450: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == DImode && GET_CODE (x4) == MULT && 1)
    goto L451;
  goto ret0;

  L451: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (GET_MODE (x5) != DImode)
    goto ret0;
  switch (GET_CODE (x5))
    {
    default:
      break;
    case SIGN_EXTEND:
      goto L452;
    case ZERO_EXTEND:
      goto L475;
    }
  goto ret0;

  L452: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 0);
  if (s_register_operand (x6, SImode))
    {
      ro[1] = x6;
      goto L453;
    }
  goto ret0;

  L453: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_MODE (x5) == DImode && GET_CODE (x5) == SIGN_EXTEND && 1)
    goto L454;
  goto ret0;

  L454: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 0);
  if (s_register_operand (x6, SImode))
    {
      ro[2] = x6;
      goto L455;
    }
  goto ret0;

  L455: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT && XWINT (x4, 0) == 32 && 1)
    goto L456;
  goto ret0;

  L456: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L457;
  goto ret0;

  L457: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      ro[3] = x2;
      if (arm_fast_multiply)
	return 49;
      }
  goto ret0;

  L475: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 0);
  if (s_register_operand (x6, SImode))
    {
      ro[1] = x6;
      goto L476;
    }
  goto ret0;

  L476: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_MODE (x5) == DImode && GET_CODE (x5) == ZERO_EXTEND && 1)
    goto L477;
  goto ret0;

  L477: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 0);
  if (s_register_operand (x6, SImode))
    {
      ro[2] = x6;
      goto L478;
    }
  goto ret0;

  L478: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT && XWINT (x4, 0) == 32 && 1)
    goto L479;
  goto ret0;

  L479: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L480;
  goto ret0;

  L480: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      ro[3] = x2;
      if (arm_fast_multiply)
	return 50;
      }
  goto ret0;

  L857: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L858;
    }
  goto ret0;

  L858: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L859;
    }
  goto ret0;

  L859: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L860;
  goto ret0;

  L860: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 103;
  goto ret0;

  L870: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L871;
    }
  goto ret0;

  L871: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L872;
    }
  goto ret0;

  L872: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L873;
  goto ret0;

  L873: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 104;
  goto ret0;

  L883: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L884;
    }
  goto ret0;

  L884: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L885;
    }
  goto ret0;

  L885: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L886;
  goto ret0;

  L886: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 105;
  goto ret0;

  L896: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L897;
    }
  goto ret0;

  L897: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L898;
    }
  goto ret0;

  L898: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L899;
  goto ret0;

  L899: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 106;
  goto ret0;

  L1046: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L1047;
    }
  goto ret0;

  L1047: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1048;
  goto ret0;

  L1048: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 126;
  goto ret0;

  L1057: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode && GET_CODE (x3) == ABS && 1)
    goto L1058;
  goto ret0;

  L1058: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L1059;
    }
  goto ret0;

  L1059: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1060;
  goto ret0;

  L1060: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 127;
  goto ret0;

  L922: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (minmax_operator (x3, SImode))
    {
      ro[5] = x3;
      goto L923;
    }
  goto ret0;

  L923: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L924;
    }
  goto ret0;

  L924: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      ro[3] = x4;
      goto L925;
    }
  goto ret0;

  L925: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L926;
    }
  goto ret0;

  L926: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L927;
  goto ret0;

  L927: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    if (GET_CODE (operands[1]) != REG
   || (REGNO(operands[1]) != FRAME_POINTER_REGNUM
       && REGNO(operands[1]) != ARG_POINTER_REGNUM))
      return 108;
  goto ret0;
 ret0: return -1;
}

int
recog_3 (x0, insn, pnum_clobbers)
     register rtx x0;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  register rtx *ro = &recog_operand[0];
  register rtx x1 ATTRIBUTE_UNUSED, x2 ATTRIBUTE_UNUSED, x3 ATTRIBUTE_UNUSED, x4 ATTRIBUTE_UNUSED, x5 ATTRIBUTE_UNUSED, x6 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;

  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  switch (GET_MODE (x2))
    {
    default:
      break;
    case DImode:
      if (s_register_operand (x2, DImode))
	{
	  ro[0] = x2;
	  goto L3;
	}
      break;
    case CC_NOOVmode:
      switch (GET_CODE (x2))
	{
	default:
	  break;
	case REG:
	  if (XINT (x2, 0) == 24 && 1)
	    goto L56;
	}
      break;
    case CC_Cmode:
      if (GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
	goto L76;
      break;
    case SImode:
      if (s_register_operand (x2, SImode))
	{
	  ro[0] = x2;
	  goto L448;
	}
    L907: ATTRIBUTE_UNUSED_LABEL
      if (memory_operand (x2, SImode))
	{
	  ro[0] = x2;
	  goto L908;
	}
      break;
    case CCmode:
      if (GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
	goto L1303;
    }
  L1290: ATTRIBUTE_UNUSED_LABEL
  if (register_operand (x2, VOIDmode))
    {
      ro[0] = x2;
      goto L1291;
    }
  goto ret0;

  L3: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) != DImode)
    {
      x2 = XEXP (x1, 0);
      goto L1290;
    }
  switch (GET_CODE (x2))
    {
    default:
      break;
    case PLUS:
      goto L17;
    case MINUS:
      goto L244;
    case NEG:
      goto L1014;
    }
  x2 = XEXP (x1, 0);
  goto L1290;

  L17: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) != DImode)
    {
      x2 = XEXP (x1, 0);
      goto L1290;
    }
  switch (GET_CODE (x3))
    {
    default:
      break;
    case SIGN_EXTEND:
      goto L18;
    case ZERO_EXTEND:
      goto L33;
    case SUBREG:
    case REG:
      if (s_register_operand (x3, DImode))
	{
	  ro[1] = x3;
	  goto L5;
	}
    }
  x2 = XEXP (x1, 0);
  goto L1290;

  L18: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L19;
    }
  x2 = XEXP (x1, 0);
  goto L1290;

  L19: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, DImode))
    {
      ro[1] = x3;
      goto L20;
    }
  x2 = XEXP (x1, 0);
  goto L1290;

  L20: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L21;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1290;

  L21: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 1;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1290;

  L33: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L34;
    }
  x2 = XEXP (x1, 0);
  goto L1290;

  L34: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, DImode))
    {
      ro[1] = x3;
      goto L35;
    }
  x2 = XEXP (x1, 0);
  goto L1290;

  L35: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L36;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1290;

  L36: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 2;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1290;

  L5: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, DImode))
    {
      ro[2] = x3;
      goto L6;
    }
  x2 = XEXP (x1, 0);
  goto L1290;

  L6: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L7;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1290;

  L7: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 0;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1290;

  L244: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) != DImode)
    {
      x2 = XEXP (x1, 0);
      goto L1290;
    }
  switch (GET_CODE (x3))
    {
    default:
      break;
    case ZERO_EXTEND:
      goto L245;
    case SIGN_EXTEND:
      goto L260;
    case SUBREG:
    case REG:
      if (s_register_operand (x3, DImode))
	{
	  ro[1] = x3;
	  goto L215;
	}
    }
  x2 = XEXP (x1, 0);
  goto L1290;

  L245: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) != SImode)
    {
      x2 = XEXP (x1, 0);
      goto L1290;
    }
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L246;
    }
  L275: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L276;
    }
  x2 = XEXP (x1, 0);
  goto L1290;

  L246: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, DImode))
    {
      ro[1] = x3;
      goto L247;
    }
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L275;

  L247: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L248;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L275;

  L248: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 27;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L275;

  L276: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == DImode && GET_CODE (x3) == ZERO_EXTEND && 1)
    goto L277;
  x2 = XEXP (x1, 0);
  goto L1290;

  L277: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L278;
    }
  x2 = XEXP (x1, 0);
  goto L1290;

  L278: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L279;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1290;

  L279: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 29;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1290;

  L260: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L261;
    }
  x2 = XEXP (x1, 0);
  goto L1290;

  L261: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, DImode))
    {
      ro[1] = x3;
      goto L262;
    }
  x2 = XEXP (x1, 0);
  goto L1290;

  L262: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L263;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1290;

  L263: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 28;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1290;

  L215: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) != DImode)
    {
      x2 = XEXP (x1, 0);
      goto L1290;
    }
  switch (GET_CODE (x3))
    {
    default:
      break;
    case ZERO_EXTEND:
      goto L216;
    case SIGN_EXTEND:
      goto L231;
    case SUBREG:
    case REG:
      if (s_register_operand (x3, DImode))
	{
	  ro[2] = x3;
	  goto L203;
	}
    }
  x2 = XEXP (x1, 0);
  goto L1290;

  L216: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L217;
    }
  x2 = XEXP (x1, 0);
  goto L1290;

  L217: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L218;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1290;

  L218: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 25;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1290;

  L231: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L232;
    }
  x2 = XEXP (x1, 0);
  goto L1290;

  L232: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L233;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1290;

  L233: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 26;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1290;

  L203: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L204;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1290;

  L204: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 24;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1290;

  L1014: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, DImode))
    {
      ro[1] = x3;
      goto L1015;
    }
  x2 = XEXP (x1, 0);
  goto L1290;

  L1015: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1016;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1290;

  L1016: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 120;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1290;

  L56: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == CC_NOOVmode && GET_CODE (x2) == COMPARE && 1)
    goto L57;
  x2 = XEXP (x1, 0);
  goto L1290;
 L57: ATTRIBUTE_UNUSED_LABEL
  tem = recog_1 (x0, insn, pnum_clobbers);
  if (tem >= 0) return tem;
  x2 = XEXP (x1, 0);
  goto L1290;

  L76: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == CC_Cmode && GET_CODE (x2) == COMPARE && 1)
    goto L77;
  x2 = XEXP (x1, 0);
  goto L1290;

  L77: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode && GET_CODE (x3) == PLUS && 1)
    goto L78;
  x2 = XEXP (x1, 0);
  goto L1290;

  L78: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L79;
    }
  x2 = XEXP (x1, 0);
  goto L1290;

  L79: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_add_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L80;
    }
  x2 = XEXP (x1, 0);
  goto L1290;

  L80: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L81;
  L93: ATTRIBUTE_UNUSED_LABEL
  if (rtx_equal_p (x3, ro[2]) && 1)
    goto L94;
  x2 = XEXP (x1, 0);
  goto L1290;

  L81: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L82;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L93;

  L82: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L83;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L93;

  L83: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == PLUS && 1)
    goto L84;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L93;

  L84: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L85;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L93;

  L85: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[2]) && 1)
    return 8;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L93;

  L94: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L95;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1290;

  L95: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L96;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1290;

  L96: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == PLUS && 1)
    goto L97;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1290;

  L97: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L98;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1290;

  L98: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[2]) && 1)
    return 9;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1290;
 L448: ATTRIBUTE_UNUSED_LABEL
  tem = recog_2 (x0, insn, pnum_clobbers);
  if (tem >= 0) return tem;
  x2 = XEXP (x1, 0);
  goto L907;

  L908: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (minmax_operator (x2, SImode))
    {
      ro[3] = x2;
      goto L909;
    }
  x2 = XEXP (x1, 0);
  goto L1290;

  L909: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L910;
    }
  x2 = XEXP (x1, 0);
  goto L1290;

  L910: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L911;
    }
  x2 = XEXP (x1, 0);
  goto L1290;

  L911: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L912;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1290;

  L912: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 107;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1290;

  L1303: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == COMPARE && 1)
    goto L1304;
  x2 = XEXP (x1, 0);
  goto L1290;

  L1304: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L1305;
    }
  x2 = XEXP (x1, 0);
  goto L1290;

  L1305: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L1306;
  x2 = XEXP (x1, 0);
  goto L1290;

  L1306: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L1307;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1290;

  L1307: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L1308;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1290;

  L1308: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (rtx_equal_p (x2, ro[1]) && 1)
    return 184;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1290;

  L1291: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == PLUS && 1)
    goto L1292;
  goto ret0;

  L1292: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[0]) && 1)
    goto L1293;
  goto ret0;

  L1293: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST && 1)
    goto L1294;
  goto ret0;

  L1294: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode && GET_CODE (x4) == PLUS && 1)
    goto L1295;
  goto ret0;

  L1295: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (GET_CODE (x5) == PC && 1)
    goto L1296;
  goto ret0;

  L1296: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT && XWINT (x5, 0) == 8 && 1)
    goto L1297;
  goto ret0;

  L1297: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == USE && 1)
    goto L1298;
  goto ret0;

  L1298: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L1299;
  goto ret0;

  L1299: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  ro[1] = x3;
  if (flag_pic)
    return 183;
  goto ret0;
 ret0: return -1;
}

int
recog_4 (x0, insn, pnum_clobbers)
     register rtx x0;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  register rtx *ro = &recog_operand[0];
  register rtx x1 ATTRIBUTE_UNUSED, x2 ATTRIBUTE_UNUSED, x3 ATTRIBUTE_UNUSED, x4 ATTRIBUTE_UNUSED, x5 ATTRIBUTE_UNUSED, x6 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;

  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) != SImode)
    goto ret0;
  switch (GET_CODE (x3))
    {
    default:
      break;
    case MINUS:
      goto L1719;
    case PLUS:
      goto L1766;
    }
  L1673: ATTRIBUTE_UNUSED_LABEL
  if (shiftable_operator (x3, SImode))
    {
      ro[1] = x3;
      goto L1674;
    }
  goto ret0;

  L1719: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L1720;
    }
  goto L1673;

  L1720: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (shift_operator (x4, SImode))
    {
      ro[2] = x4;
      goto L1721;
    }
  goto L1673;

  L1721: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      ro[3] = x5;
      goto L1722;
    }
  goto L1673;

  L1722: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (reg_or_int_operand (x5, SImode))
    {
      ro[4] = x5;
      goto L1723;
    }
  goto L1673;

  L1723: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L1724;
  x3 = XEXP (x2, 0);
  goto L1673;

  L1724: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  switch (GET_CODE (x1))
    {
    default:
      break;
    case SET:
      goto L1725;
    case CLOBBER:
      goto L1742;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L1673;

  L1725: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L1726;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L1673;

  L1726: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == MINUS && 1)
    goto L1727;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L1673;

  L1727: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L1728;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L1673;

  L1728: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[2]) && 1)
    {
      ro[2] = x3;
      goto L1729;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L1673;

  L1729: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, ro[3]) && 1)
    goto L1730;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L1673;

  L1730: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, ro[4]) && 1)
    return 292;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L1673;

  L1742: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      ro[0] = x2;
      return 293;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L1673;

  L1766: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode && GET_CODE (x4) == PLUS && 1)
    goto L1767;
  goto L1673;

  L1767: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (GET_MODE (x5) != SImode)
    {
      goto L1673;
    }
  if (shift_operator (x5, SImode))
    {
      ro[5] = x5;
      goto L1768;
    }
  L1822: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x5) == MULT && 1)
    goto L1823;
  goto L1673;

  L1768: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 0);
  ro[3] = x6;
  goto L1769;

  L1769: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 1);
  ro[4] = x6;
  goto L1770;

  L1770: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  ro[1] = x5;
  goto L1771;

  L1771: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT && 1)
    {
      ro[2] = x4;
      goto L1772;
    }
  x4 = XEXP (x3, 0);
  x5 = XEXP (x4, 0);
  goto L1822;

  L1772: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L1773;
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  x5 = XEXP (x4, 0);
  goto L1822;

  L1773: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  switch (GET_CODE (x1))
    {
    default:
      break;
    case SET:
      goto L1774;
    case CLOBBER:
      goto L1795;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  x5 = XEXP (x4, 0);
  goto L1822;

  L1774: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && 1)
    {
      ro[0] = x2;
      goto L1775;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  x5 = XEXP (x4, 0);
  goto L1822;

  L1775: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == PLUS && 1)
    goto L1776;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  x5 = XEXP (x4, 0);
  goto L1822;

  L1776: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode && GET_CODE (x3) == PLUS && 1)
    goto L1777;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  x5 = XEXP (x4, 0);
  goto L1822;

  L1777: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, ro[5]) && 1)
    {
      ro[5] = x4;
      goto L1778;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  x5 = XEXP (x4, 0);
  goto L1822;

  L1778: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, ro[3]) && 1)
    goto L1779;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  x5 = XEXP (x4, 0);
  goto L1822;

  L1779: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (rtx_equal_p (x5, ro[4]) && 1)
    goto L1780;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  x5 = XEXP (x4, 0);
  goto L1822;

  L1780: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, ro[1]) && 1)
    goto L1781;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  x5 = XEXP (x4, 0);
  goto L1822;

  L1781: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[2]) && 1)
    if (reload_in_progress)
      return 295;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  x5 = XEXP (x4, 0);
  goto L1822;

  L1795: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      ro[0] = x2;
      if (reload_in_progress)
	return 296;
      }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  x5 = XEXP (x4, 0);
  goto L1822;

  L1823: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 0);
  ro[3] = x6;
  goto L1824;

  L1824: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 1);
  ro[4] = x6;
  goto L1825;

  L1825: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  ro[1] = x5;
  goto L1826;

  L1826: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT && 1)
    {
      ro[2] = x4;
      goto L1827;
    }
  goto L1673;

  L1827: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L1828;
  x3 = XEXP (x2, 0);
  goto L1673;

  L1828: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  switch (GET_CODE (x1))
    {
    default:
      break;
    case SET:
      goto L1829;
    case CLOBBER:
      goto L1850;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L1673;

  L1829: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && 1)
    {
      ro[0] = x2;
      goto L1830;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L1673;

  L1830: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == PLUS && 1)
    goto L1831;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L1673;

  L1831: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode && GET_CODE (x3) == PLUS && 1)
    goto L1832;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L1673;

  L1832: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode && GET_CODE (x4) == MULT && 1)
    goto L1833;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L1673;

  L1833: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, ro[3]) && 1)
    goto L1834;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L1673;

  L1834: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (rtx_equal_p (x5, ro[4]) && 1)
    goto L1835;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L1673;

  L1835: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, ro[1]) && 1)
    goto L1836;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L1673;

  L1836: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[2]) && 1)
    if (reload_in_progress)
      return 298;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L1673;

  L1850: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      ro[0] = x2;
      if (reload_in_progress)
	return 299;
      }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L1673;

  L1674: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (shift_operator (x4, SImode))
    {
      ro[3] = x4;
      goto L1675;
    }
  goto ret0;

  L1675: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      ro[4] = x5;
      goto L1676;
    }
  goto ret0;

  L1676: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (reg_or_int_operand (x5, SImode))
    {
      ro[5] = x5;
      goto L1677;
    }
  goto ret0;

  L1677: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L1678;
    }
  goto ret0;

  L1678: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L1679;
  goto ret0;

  L1679: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  switch (GET_CODE (x1))
    {
    default:
      break;
    case SET:
      goto L1680;
    case CLOBBER:
      goto L1697;
    }
  goto ret0;

  L1680: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L1681;
    }
  goto ret0;

  L1681: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (rtx_equal_p (x2, ro[1]) && 1)
    {
      ro[1] = x2;
      goto L1682;
    }
  goto ret0;

  L1682: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[3]) && 1)
    {
      ro[3] = x3;
      goto L1683;
    }
  goto ret0;

  L1683: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, ro[4]) && 1)
    goto L1684;
  goto ret0;

  L1684: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, ro[5]) && 1)
    goto L1685;
  goto ret0;

  L1685: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[2]) && 1)
    return 289;
  goto ret0;

  L1697: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      ro[0] = x2;
      return 290;
    }
  goto ret0;
 ret0: return -1;
}

int
recog_5 (x0, insn, pnum_clobbers)
     register rtx x0;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  register rtx *ro = &recog_operand[0];
  register rtx x1 ATTRIBUTE_UNUSED, x2 ATTRIBUTE_UNUSED, x3 ATTRIBUTE_UNUSED, x4 ATTRIBUTE_UNUSED, x5 ATTRIBUTE_UNUSED, x6 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;

  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) != SImode)
    goto ret0;
  if (s_register_operand (x4, SImode))
    {
      ro[4] = x4;
      goto L1997;
    }
  L2101: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L2102;
    }
  goto ret0;

  L1997: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_add_operand (x4, SImode))
    {
      ro[5] = x4;
      goto L1998;
    }
  x4 = XEXP (x3, 0);
  goto L2101;

  L1998: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  switch (GET_MODE (x3))
    {
    default:
      break;
    case SImode:
      switch (GET_CODE (x3))
	{
	default:
	  break;
	case PLUS:
	  goto L1999;
	case NOT:
	  goto L2335;
	case ASHIFT:
	case ASHIFTRT:
	case LSHIFTRT:
	case ROTATERT:
	case MULT:
	  if (shift_operator (x3, SImode))
	    {
	      ro[7] = x3;
	      goto L2230;
	    }
	}
    L2370: ATTRIBUTE_UNUSED_LABEL
      if (shiftable_operator (x3, SImode))
	{
	  ro[7] = x3;
	  goto L2371;
	}
    }
  if (arm_rhs_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L2032;
    }
  L2262: ATTRIBUTE_UNUSED_LABEL
  if (arm_not_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L2263;
    }
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L2101;

  L1999: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L2000;
    }
  goto L2370;

  L2000: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_add_operand (x4, SImode))
    {
      ro[3] = x4;
      goto L2001;
    }
  goto L2370;

  L2001: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L2002;
    }
  x3 = XEXP (x2, 1);
  goto L2370;

  L2002: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L2003;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L2370;

  L2003: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 310;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L2370;

  L2335: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L2336;
    }
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L2101;

  L2336: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (shiftable_operator (x3, SImode))
    {
      ro[7] = x3;
      goto L2337;
    }
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L2101;

  L2337: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L2338;
    }
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L2101;

  L2338: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      ro[3] = x4;
      goto L2339;
    }
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L2101;

  L2339: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L2340;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L2101;

  L2340: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 330;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L2101;

  L2230: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L2231;
    }
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L2101;

  L2231: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      ro[3] = x4;
      goto L2232;
    }
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L2101;

  L2232: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (arm_not_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L2233;
    }
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L2101;

  L2233: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L2234;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L2101;

  L2234: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 324;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L2101;

  L2371: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L2372;
    }
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L2101;

  L2372: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      ro[3] = x4;
      goto L2373;
    }
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L2101;

  L2373: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (GET_MODE (x3) == SImode && GET_CODE (x3) == NOT && 1)
    goto L2374;
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L2101;

  L2374: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L2375;
    }
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L2101;

  L2375: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L2376;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L2101;

  L2376: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 332;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L2101;

  L2032: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (GET_MODE (x3) != SImode)
    {
      x3 = XEXP (x2, 1);
      goto L2262;
    }
  if (GET_CODE (x3) == PLUS && 1)
    goto L2033;
  L2137: ATTRIBUTE_UNUSED_LABEL
  if (shiftable_operator (x3, SImode))
    {
      ro[7] = x3;
      goto L2138;
    }
  x3 = XEXP (x2, 1);
  goto L2262;

  L2033: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L2034;
    }
  goto L2137;

  L2034: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_add_operand (x4, SImode))
    {
      ro[3] = x4;
      goto L2035;
    }
  goto L2137;

  L2035: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L2036;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 2);
  goto L2137;

  L2036: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 312;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 2);
  goto L2137;

  L2138: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L2139;
    }
  x3 = XEXP (x2, 1);
  goto L2262;

  L2139: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      ro[3] = x4;
      goto L2140;
    }
  x3 = XEXP (x2, 1);
  goto L2262;

  L2140: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L2141;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L2262;

  L2141: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 318;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L2262;

  L2263: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (shift_operator (x3, SImode))
    {
      ro[7] = x3;
      goto L2264;
    }
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L2101;

  L2264: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L2265;
    }
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L2101;

  L2265: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      ro[3] = x4;
      goto L2266;
    }
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L2101;

  L2266: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L2267;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L2101;

  L2267: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 326;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L2101;

  L2102: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_add_operand (x4, SImode))
    {
      ro[3] = x4;
      goto L2103;
    }
  goto ret0;

  L2103: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (shiftable_operator (x3, SImode))
    {
      ro[7] = x3;
      goto L2104;
    }
  goto ret0;

  L2104: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[4] = x4;
      goto L2105;
    }
  goto ret0;

  L2105: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      ro[5] = x4;
      goto L2106;
    }
  goto ret0;

  L2106: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L2107;
    }
  goto ret0;

  L2107: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L2108;
  goto ret0;

  L2108: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 316;
  goto ret0;
 ret0: return -1;
}

int
recog_6 (x0, insn, pnum_clobbers)
     register rtx x0;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  register rtx *ro = &recog_operand[0];
  register rtx x1 ATTRIBUTE_UNUSED, x2 ATTRIBUTE_UNUSED, x3 ATTRIBUTE_UNUSED, x4 ATTRIBUTE_UNUSED, x5 ATTRIBUTE_UNUSED, x6 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;

  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  switch (GET_MODE (x2))
    {
    default:
      break;
    case SImode:
      switch (GET_CODE (x2))
	{
	default:
	  break;
	case MINUS:
	  goto L1920;
	case NEG:
	  goto L1961;
	case IF_THEN_ELSE:
	  goto L1976;
	}
    L1902: ATTRIBUTE_UNUSED_LABEL
      if (shiftable_operator (x2, SImode))
	{
	  ro[5] = x2;
	  goto L1903;
	}
    L2462: ATTRIBUTE_UNUSED_LABEL
      if (shiftable_operator (x2, SImode))
	{
	  ro[1] = x2;
	  goto L2463;
	}
    }
  if (comparison_operator (x2, VOIDmode))
    {
      ro[1] = x2;
      goto L1880;
    }
  goto ret0;

  L1920: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L1921;
    }
  goto L1902;

  L1921: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (comparison_operator (x3, SImode))
    {
      ro[4] = x3;
      goto L1922;
    }
  goto L1902;

  L1922: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L1923;
    }
  goto L1902;

  L1923: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      ro[3] = x4;
      goto L1924;
    }
  goto L1902;

  L1924: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1925;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L1902;

  L1925: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 305;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L1902;

  L1961: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (comparison_operator (x3, VOIDmode))
    {
      ro[3] = x3;
      goto L1962;
    }
  goto ret0;

  L1962: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L1963;
    }
  goto ret0;

  L1963: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L1964;
    }
  goto ret0;

  L1964: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1965;
  goto ret0;

  L1965: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 308;
  goto ret0;

  L1976: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (comparison_operator (x3, VOIDmode))
    {
      ro[5] = x3;
      goto L1977;
    }
  L1995: ATTRIBUTE_UNUSED_LABEL
  if (comparison_operator (x3, VOIDmode))
    {
      ro[6] = x3;
      goto L1996;
    }
  L2061: ATTRIBUTE_UNUSED_LABEL
  if (comparison_operator (x3, VOIDmode))
    {
      ro[9] = x3;
      goto L2062;
    }
  L2292: ATTRIBUTE_UNUSED_LABEL
  if (comparison_operator (x3, VOIDmode))
    {
      ro[7] = x3;
      goto L2293;
    }
  goto ret0;

  L1977: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[3] = x4;
      goto L1978;
    }
  goto L1995;

  L1978: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_add_operand (x4, SImode))
    {
      ro[4] = x4;
      goto L2199;
    }
  goto L1995;

  L2199: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  switch (GET_MODE (x3))
    {
    default:
      break;
    case SImode:
      switch (GET_CODE (x3))
	{
	default:
	  break;
	case NOT:
	  goto L2200;
	case NEG:
	  goto L2407;
	}
    }
  if (arm_rhs_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L1980;
    }
  L2169: ATTRIBUTE_UNUSED_LABEL
  if (arm_not_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L2170;
    }
  x3 = XEXP (x2, 0);
  goto L1995;

  L2200: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L2201;
    }
  x3 = XEXP (x2, 0);
  goto L1995;

  L2201: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (arm_not_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L2202;
    }
  x3 = XEXP (x2, 0);
  goto L1995;

  L2202: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L2203;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L1995;

  L2203: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 322;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L1995;

  L2407: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L2408;
    }
  x3 = XEXP (x2, 0);
  goto L1995;

  L2408: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (arm_not_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L2409;
    }
  x3 = XEXP (x2, 0);
  goto L1995;

  L2409: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L2410;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L1995;

  L2410: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 334;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L1995;

  L1980: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L1981;
    }
  x3 = XEXP (x2, 1);
  goto L2169;

  L1981: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1982;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L2169;

  L1982: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 309;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L2169;

  L2170: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (GET_MODE (x3) != SImode)
    {
      x3 = XEXP (x2, 0);
      goto L1995;
    }
  switch (GET_CODE (x3))
    {
    default:
      break;
    case NOT:
      goto L2171;
    case NEG:
      goto L2438;
    }
  x3 = XEXP (x2, 0);
  goto L1995;

  L2171: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L2172;
    }
  x3 = XEXP (x2, 0);
  goto L1995;

  L2172: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L2173;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L1995;

  L2173: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 320;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L1995;

  L2438: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L2439;
    }
  x3 = XEXP (x2, 0);
  goto L1995;

  L2439: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L2440;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L1995;

  L2440: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 336;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L1995;
 L1996: ATTRIBUTE_UNUSED_LABEL
  tem = recog_5 (x0, insn, pnum_clobbers);
  if (tem >= 0) return tem;
  goto L2061;

  L2062: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[5] = x4;
      goto L2063;
    }
  goto L2292;

  L2063: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_add_operand (x4, SImode))
    {
      ro[6] = x4;
      goto L2064;
    }
  goto L2292;

  L2064: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (shiftable_operator (x3, SImode))
    {
      ro[8] = x3;
      goto L2065;
    }
  x3 = XEXP (x2, 0);
  goto L2292;

  L2065: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L2066;
    }
  x3 = XEXP (x2, 0);
  goto L2292;

  L2066: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L2067;
    }
  x3 = XEXP (x2, 0);
  goto L2292;

  L2067: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (shiftable_operator (x3, SImode))
    {
      ro[7] = x3;
      goto L2068;
    }
  x3 = XEXP (x2, 0);
  goto L2292;

  L2068: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[3] = x4;
      goto L2069;
    }
  x3 = XEXP (x2, 0);
  goto L2292;

  L2069: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      ro[4] = x4;
      goto L2070;
    }
  x3 = XEXP (x2, 0);
  goto L2292;

  L2070: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L2071;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L2292;

  L2071: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 314;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L2292;

  L2293: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[5] = x4;
      goto L2294;
    }
  goto ret0;

  L2294: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_add_operand (x4, SImode))
    {
      ro[6] = x4;
      goto L2295;
    }
  goto ret0;

  L2295: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (shift_operator (x3, SImode))
    {
      ro[8] = x3;
      goto L2296;
    }
  goto ret0;

  L2296: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L2297;
    }
  goto ret0;

  L2297: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L2298;
    }
  goto ret0;

  L2298: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (shift_operator (x3, SImode))
    {
      ro[9] = x3;
      goto L2299;
    }
  goto ret0;

  L2299: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[3] = x4;
      goto L2300;
    }
  goto ret0;

  L2300: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      ro[4] = x4;
      goto L2301;
    }
  goto ret0;

  L2301: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L2302;
  goto ret0;

  L2302: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 328;
  goto ret0;

  L1903: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (comparison_operator (x3, SImode))
    {
      ro[4] = x3;
      goto L1904;
    }
  goto L2462;

  L1904: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L1905;
    }
  goto L2462;

  L1905: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      ro[3] = x4;
      goto L1906;
    }
  goto L2462;

  L1906: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L1907;
    }
  goto L2462;

  L1907: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1908;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L2462;

  L1908: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 304;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L2462;

  L2463: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (memory_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L2464;
    }
  goto ret0;

  L2464: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (memory_operand (x3, SImode))
    {
      ro[3] = x3;
      goto L2465;
    }
  goto ret0;

  L2465: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L2466;
  goto ret0;

  L2466: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      ro[4] = x2;
      if (adjacent_mem_locations (operands[2], operands[3]))
	return 338;
      }
  goto ret0;

  L1880: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L1881;
    }
  goto ret0;

  L1881: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_add_operand (x3, SImode))
    {
      ro[3] = x3;
      goto L1882;
    }
  goto ret0;

  L1882: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1883;
  goto ret0;

  L1883: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    return 302;
  goto ret0;
 ret0: return -1;
}

int
recog_7 (x0, insn, pnum_clobbers)
     register rtx x0;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  register rtx *ro = &recog_operand[0];
  register rtx x1 ATTRIBUTE_UNUSED, x2 ATTRIBUTE_UNUSED, x3 ATTRIBUTE_UNUSED, x4 ATTRIBUTE_UNUSED, x5 ATTRIBUTE_UNUSED, x6 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;

  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  ro[0] = x2;
  goto L1564;
  L1604: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, VOIDmode))
    {
      ro[0] = x2;
      goto L1605;
    }
  switch (GET_CODE (x2))
    {
    default:
      break;
    case PC:
      goto L1639;
    }
  L1671: ATTRIBUTE_UNUSED_LABEL
  switch (GET_MODE (x2))
    {
    default:
      break;
    case CC_NOOVmode:
      switch (GET_CODE (x2))
	{
	default:
	  break;
	case REG:
	  if (XINT (x2, 0) == 24 && 1)
	    goto L1672;
	}
      break;
    case SImode:
      if (GET_CODE (x2) == MEM && 1)
	goto L2549;
      if (s_register_operand (x2, SImode))
	{
	  ro[0] = x2;
	  goto L1919;
	}
    L2522: ATTRIBUTE_UNUSED_LABEL
      if (s_register_operand (x2, SImode))
	{
	  ro[3] = x2;
	  goto L2523;
	}
    L2716: ATTRIBUTE_UNUSED_LABEL
      if (s_register_operand (x2, SImode))
	{
	  ro[5] = x2;
	  goto L2717;
	}
      break;
    case QImode:
      if (GET_CODE (x2) == MEM && 1)
	goto L2475;
      if (s_register_operand (x2, QImode))
	{
	  ro[3] = x2;
	  goto L2499;
	}
    L2652: ATTRIBUTE_UNUSED_LABEL
      if (s_register_operand (x2, QImode))
	{
	  ro[5] = x2;
	  goto L2653;
	}
      break;
    case HImode:
      if (s_register_operand (x2, HImode))
	{
	  ro[3] = x2;
	  goto L2597;
	}
    L2748: ATTRIBUTE_UNUSED_LABEL
      if (s_register_operand (x2, HImode))
	{
	  ro[5] = x2;
	  goto L2749;
	}
    }
  goto ret0;

  L1564: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CALL && 1)
    goto L1565;
  x2 = XEXP (x1, 0);
  goto L1604;

  L1565: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode && GET_CODE (x3) == MEM && 1)
    goto L1566;
  x2 = XEXP (x1, 0);
  goto L1604;

  L1566: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L1567;
    }
  L1581: ATTRIBUTE_UNUSED_LABEL
  if (memory_operand (x4, VOIDmode))
    {
      ro[1] = x4;
      goto L1582;
    }
  x2 = XEXP (x1, 0);
  goto L1604;

  L1567: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (general_operand (x3, VOIDmode))
    {
      ro[2] = x3;
      goto L1568;
    }
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L1581;

  L1568: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1569;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L1581;

  L1569: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == REG && XINT (x2, 0) == 14 && 1)
    return 274;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L1581;

  L1582: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (general_operand (x3, VOIDmode))
    {
      ro[2] = x3;
      goto L1583;
    }
  x2 = XEXP (x1, 0);
  goto L1604;

  L1583: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1584;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1604;

  L1584: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == REG && XINT (x2, 0) == 14 && 1)
    if (! CONSTANT_ADDRESS_P (XEXP (operands[1], 0)))
      return 275;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1604;

  L1605: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CALL && 1)
    goto L1606;
  x2 = XEXP (x1, 0);
  goto L1671;

  L1606: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode && GET_CODE (x3) == MEM && 1)
    goto L1607;
  x2 = XEXP (x1, 0);
  goto L1671;

  L1607: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  ro[1] = x4;
  goto L1608;

  L1608: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (general_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L1609;
    }
  x2 = XEXP (x1, 0);
  goto L1671;

  L1609: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1610;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1671;

  L1610: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == REG && XINT (x2, 0) == 14 && 1)
    if (GET_CODE(operands[1]) == SYMBOL_REF)
      return 277;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1671;

  L1639: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == IF_THEN_ELSE && 1)
    goto L1640;
  goto ret0;

  L1640: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_CODE (x3) == LEU && 1)
    goto L1641;
  goto ret0;

  L1641: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[0] = x4;
      goto L1642;
    }
  goto ret0;

  L1642: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L1643;
    }
  goto ret0;

  L1643: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode && GET_CODE (x3) == MEM && 1)
    goto L1644;
  goto ret0;

  L1644: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode && GET_CODE (x4) == PLUS && 1)
    goto L1645;
  goto ret0;

  L1645: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (GET_MODE (x5) == SImode && GET_CODE (x5) == MULT && 1)
    goto L1646;
  goto ret0;

  L1646: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 0);
  if (rtx_equal_p (x6, ro[0]) && 1)
    goto L1647;
  goto ret0;

  L1647: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 1);
  if (GET_CODE (x6) == CONST_INT && XWINT (x6, 0) == 4 && 1)
    goto L1648;
  goto ret0;

  L1648: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == LABEL_REF && 1)
    goto L1649;
  goto ret0;

  L1649: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 0);
  ro[2] = x6;
  goto L1650;

  L1650: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (GET_CODE (x3) == LABEL_REF && 1)
    goto L1651;
  goto ret0;

  L1651: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  ro[3] = x4;
  goto L1652;

  L1652: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == USE && 1)
    goto L1653;
  goto ret0;

  L1653: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L1654;
  goto ret0;

  L1654: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[2]) && 1)
    return 284;
  goto ret0;

  L1672: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == CC_NOOVmode && GET_CODE (x2) == COMPARE && 1)
    goto L1718;
  goto ret0;
 L1718: ATTRIBUTE_UNUSED_LABEL
  return recog_4 (x0, insn, pnum_clobbers);

  L2549: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) != SImode)
    goto ret0;
  switch (GET_CODE (x3))
    {
    default:
      break;
    case PLUS:
      goto L2550;
    case MINUS:
      goto L2562;
    }
  goto ret0;

  L2550: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) != SImode)
    goto ret0;
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L2551;
    }
  if (shift_operator (x4, SImode))
    {
      ro[2] = x4;
      goto L2687;
    }
  goto ret0;

  L2551: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (index_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L2552;
    }
  goto ret0;

  L2552: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      ro[3] = x2;
      goto L2553;
    }
  goto ret0;

  L2553: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L2554;
  goto ret0;

  L2554: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L2555;
    }
  goto ret0;

  L2555: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == PLUS && 1)
    goto L2556;
  goto ret0;

  L2556: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L2557;
  goto ret0;

  L2557: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[2]) && 1)
    if (REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM))
      return 345;
  goto ret0;

  L2687: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      ro[3] = x5;
      goto L2688;
    }
  goto ret0;

  L2688: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT && const_shift_operand (x5, SImode))
    {
      ro[4] = x5;
      goto L2689;
    }
  goto ret0;

  L2689: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L2690;
    }
  goto ret0;

  L2690: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      ro[5] = x2;
      goto L2691;
    }
  goto ret0;

  L2691: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L2692;
  goto ret0;

  L2692: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L2693;
    }
  goto ret0;

  L2693: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == PLUS && 1)
    goto L2694;
  goto ret0;

  L2694: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[2]) && 1)
    {
      ro[2] = x3;
      goto L2695;
    }
  goto ret0;

  L2695: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, ro[3]) && 1)
    goto L2696;
  goto ret0;

  L2696: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, ro[4]) && 1)
    goto L2697;
  goto ret0;

  L2697: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[1]) && 1)
    if (REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && REGNO (operands[3]) != FRAME_POINTER_REGNUM)
      return 355;
  goto ret0;

  L2562: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L2563;
    }
  goto ret0;

  L2563: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_MODE (x4) != SImode)
    goto ret0;
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L2564;
    }
  if (shift_operator (x4, SImode))
    {
      ro[2] = x4;
      goto L2704;
    }
  goto ret0;

  L2564: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      ro[3] = x2;
      goto L2565;
    }
  goto ret0;

  L2565: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L2566;
  goto ret0;

  L2566: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L2567;
    }
  goto ret0;

  L2567: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == MINUS && 1)
    goto L2568;
  goto ret0;

  L2568: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L2569;
  goto ret0;

  L2569: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[2]) && 1)
    if (REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM))
      return 346;
  goto ret0;

  L2704: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      ro[3] = x5;
      goto L2705;
    }
  goto ret0;

  L2705: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT && const_shift_operand (x5, SImode))
    {
      ro[4] = x5;
      goto L2706;
    }
  goto ret0;

  L2706: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      ro[5] = x2;
      goto L2707;
    }
  goto ret0;

  L2707: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L2708;
  goto ret0;

  L2708: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L2709;
    }
  goto ret0;

  L2709: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == MINUS && 1)
    goto L2710;
  goto ret0;

  L2710: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L2711;
  goto ret0;

  L2711: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[2]) && 1)
    {
      ro[2] = x3;
      goto L2712;
    }
  goto ret0;

  L2712: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, ro[3]) && 1)
    goto L2713;
  goto ret0;

  L2713: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, ro[4]) && 1)
    if (REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && REGNO (operands[3]) != FRAME_POINTER_REGNUM)
      return 356;
  goto ret0;
 L1919: ATTRIBUTE_UNUSED_LABEL
  tem = recog_6 (x0, insn, pnum_clobbers);
  if (tem >= 0) return tem;
  x2 = XEXP (x1, 0);
  goto L2522;

  L2523: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) != SImode)
    {
      x2 = XEXP (x1, 0);
      goto L2716;
    }
  switch (GET_CODE (x2))
    {
    default:
      break;
    case ZERO_EXTEND:
      goto L2524;
    case MEM:
      goto L2574;
    }
  x2 = XEXP (x1, 0);
  goto L2716;

  L2524: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == QImode && GET_CODE (x3) == MEM && 1)
    goto L2525;
  x2 = XEXP (x1, 0);
  goto L2716;

  L2525: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) != SImode)
    {
      x2 = XEXP (x1, 0);
      goto L2716;
    }
  switch (GET_CODE (x4))
    {
    default:
      break;
    case PLUS:
      goto L2526;
    case MINUS:
      goto L2539;
    }
  x2 = XEXP (x1, 0);
  goto L2716;

  L2526: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      ro[1] = x5;
      goto L2527;
    }
  x2 = XEXP (x1, 0);
  goto L2716;

  L2527: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (index_operand (x5, SImode))
    {
      ro[2] = x5;
      goto L2528;
    }
  x2 = XEXP (x1, 0);
  goto L2716;

  L2528: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L2529;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2716;

  L2529: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L2530;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2716;

  L2530: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == PLUS && 1)
    goto L2531;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2716;

  L2531: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L2532;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2716;

  L2532: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[2]) && 1)
    if (REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM))
      return 343;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2716;

  L2539: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      ro[1] = x5;
      goto L2540;
    }
  x2 = XEXP (x1, 0);
  goto L2716;

  L2540: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (s_register_operand (x5, SImode))
    {
      ro[2] = x5;
      goto L2541;
    }
  x2 = XEXP (x1, 0);
  goto L2716;

  L2541: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L2542;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2716;

  L2542: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L2543;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2716;

  L2543: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == MINUS && 1)
    goto L2544;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2716;

  L2544: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L2545;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2716;

  L2545: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[2]) && 1)
    if (REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM))
      return 344;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2716;

  L2574: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) != SImode)
    {
      x2 = XEXP (x1, 0);
      goto L2716;
    }
  switch (GET_CODE (x3))
    {
    default:
      break;
    case PLUS:
      goto L2575;
    case MINUS:
      goto L2587;
    }
  x2 = XEXP (x1, 0);
  goto L2716;

  L2575: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L2576;
    }
  x2 = XEXP (x1, 0);
  goto L2716;

  L2576: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (index_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L2577;
    }
  x2 = XEXP (x1, 0);
  goto L2716;

  L2577: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L2578;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2716;

  L2578: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L2579;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2716;

  L2579: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == PLUS && 1)
    goto L2580;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2716;

  L2580: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L2581;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2716;

  L2581: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[2]) && 1)
    if (REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM))
      return 347;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2716;

  L2587: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L2588;
    }
  x2 = XEXP (x1, 0);
  goto L2716;

  L2588: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L2589;
    }
  x2 = XEXP (x1, 0);
  goto L2716;

  L2589: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L2590;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2716;

  L2590: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L2591;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2716;

  L2591: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == MINUS && 1)
    goto L2592;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2716;

  L2592: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L2593;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2716;

  L2593: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[2]) && 1)
    if (REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM))
      return 348;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2716;

  L2717: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == MEM && 1)
    goto L2718;
  goto ret0;

  L2718: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) != SImode)
    goto ret0;
  switch (GET_CODE (x3))
    {
    default:
      break;
    case PLUS:
      goto L2719;
    case MINUS:
      goto L2735;
    }
  goto ret0;

  L2719: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (shift_operator (x4, SImode))
    {
      ro[2] = x4;
      goto L2720;
    }
  goto ret0;

  L2720: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      ro[3] = x5;
      goto L2721;
    }
  goto ret0;

  L2721: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT && const_shift_operand (x5, SImode))
    {
      ro[4] = x5;
      goto L2722;
    }
  goto ret0;

  L2722: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L2723;
    }
  goto ret0;

  L2723: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L2724;
  goto ret0;

  L2724: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L2725;
    }
  goto ret0;

  L2725: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == PLUS && 1)
    goto L2726;
  goto ret0;

  L2726: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[2]) && 1)
    {
      ro[2] = x3;
      goto L2727;
    }
  goto ret0;

  L2727: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, ro[3]) && 1)
    goto L2728;
  goto ret0;

  L2728: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, ro[4]) && 1)
    goto L2729;
  goto ret0;

  L2729: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[1]) && 1)
    if (REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && REGNO (operands[3]) != FRAME_POINTER_REGNUM)
      return 357;
  goto ret0;

  L2735: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L2736;
    }
  goto ret0;

  L2736: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (shift_operator (x4, SImode))
    {
      ro[2] = x4;
      goto L2737;
    }
  goto ret0;

  L2737: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      ro[3] = x5;
      goto L2738;
    }
  goto ret0;

  L2738: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT && const_shift_operand (x5, SImode))
    {
      ro[4] = x5;
      goto L2739;
    }
  goto ret0;

  L2739: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L2740;
  goto ret0;

  L2740: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L2741;
    }
  goto ret0;

  L2741: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == MINUS && 1)
    goto L2742;
  goto ret0;

  L2742: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L2743;
  goto ret0;

  L2743: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[2]) && 1)
    {
      ro[2] = x3;
      goto L2744;
    }
  goto ret0;

  L2744: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, ro[3]) && 1)
    goto L2745;
  goto ret0;

  L2745: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, ro[4]) && 1)
    if (REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && REGNO (operands[3]) != FRAME_POINTER_REGNUM)
      return 358;
  goto ret0;

  L2475: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) != SImode)
    goto ret0;
  switch (GET_CODE (x3))
    {
    default:
      break;
    case PLUS:
      goto L2476;
    case MINUS:
      goto L2488;
    }
  goto ret0;

  L2476: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) != SImode)
    goto ret0;
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L2477;
    }
  if (shift_operator (x4, SImode))
    {
      ro[2] = x4;
      goto L2623;
    }
  goto ret0;

  L2477: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (index_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L2478;
    }
  goto ret0;

  L2478: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, QImode))
    {
      ro[3] = x2;
      goto L2479;
    }
  goto ret0;

  L2479: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L2480;
  goto ret0;

  L2480: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L2481;
    }
  goto ret0;

  L2481: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == PLUS && 1)
    goto L2482;
  goto ret0;

  L2482: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L2483;
  goto ret0;

  L2483: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[2]) && 1)
    if (REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM))
      return 339;
  goto ret0;

  L2623: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      ro[3] = x5;
      goto L2624;
    }
  goto ret0;

  L2624: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT && const_shift_operand (x5, SImode))
    {
      ro[4] = x5;
      goto L2625;
    }
  goto ret0;

  L2625: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L2626;
    }
  goto ret0;

  L2626: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, QImode))
    {
      ro[5] = x2;
      goto L2627;
    }
  goto ret0;

  L2627: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L2628;
  goto ret0;

  L2628: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L2629;
    }
  goto ret0;

  L2629: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == PLUS && 1)
    goto L2630;
  goto ret0;

  L2630: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[2]) && 1)
    {
      ro[2] = x3;
      goto L2631;
    }
  goto ret0;

  L2631: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, ro[3]) && 1)
    goto L2632;
  goto ret0;

  L2632: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, ro[4]) && 1)
    goto L2633;
  goto ret0;

  L2633: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[1]) && 1)
    if (REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && REGNO (operands[3]) != FRAME_POINTER_REGNUM)
      return 351;
  goto ret0;

  L2488: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L2489;
    }
  goto ret0;

  L2489: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_MODE (x4) != SImode)
    goto ret0;
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L2490;
    }
  if (shift_operator (x4, SImode))
    {
      ro[2] = x4;
      goto L2640;
    }
  goto ret0;

  L2490: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, QImode))
    {
      ro[3] = x2;
      goto L2491;
    }
  goto ret0;

  L2491: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L2492;
  goto ret0;

  L2492: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L2493;
    }
  goto ret0;

  L2493: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == MINUS && 1)
    goto L2494;
  goto ret0;

  L2494: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L2495;
  goto ret0;

  L2495: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[2]) && 1)
    if (REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM))
      return 340;
  goto ret0;

  L2640: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      ro[3] = x5;
      goto L2641;
    }
  goto ret0;

  L2641: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT && const_shift_operand (x5, SImode))
    {
      ro[4] = x5;
      goto L2642;
    }
  goto ret0;

  L2642: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, QImode))
    {
      ro[5] = x2;
      goto L2643;
    }
  goto ret0;

  L2643: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L2644;
  goto ret0;

  L2644: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L2645;
    }
  goto ret0;

  L2645: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == MINUS && 1)
    goto L2646;
  goto ret0;

  L2646: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L2647;
  goto ret0;

  L2647: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[2]) && 1)
    {
      ro[2] = x3;
      goto L2648;
    }
  goto ret0;

  L2648: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, ro[3]) && 1)
    goto L2649;
  goto ret0;

  L2649: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, ro[4]) && 1)
    if (REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && REGNO (operands[3]) != FRAME_POINTER_REGNUM)
      return 352;
  goto ret0;

  L2499: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == QImode && GET_CODE (x2) == MEM && 1)
    goto L2500;
  x2 = XEXP (x1, 0);
  goto L2652;

  L2500: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) != SImode)
    {
      x2 = XEXP (x1, 0);
      goto L2652;
    }
  switch (GET_CODE (x3))
    {
    default:
      break;
    case PLUS:
      goto L2501;
    case MINUS:
      goto L2513;
    }
  x2 = XEXP (x1, 0);
  goto L2652;

  L2501: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L2502;
    }
  x2 = XEXP (x1, 0);
  goto L2652;

  L2502: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (index_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L2503;
    }
  x2 = XEXP (x1, 0);
  goto L2652;

  L2503: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L2504;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2652;

  L2504: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L2505;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2652;

  L2505: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == PLUS && 1)
    goto L2506;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2652;

  L2506: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L2507;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2652;

  L2507: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[2]) && 1)
    if (REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM))
      return 341;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2652;

  L2513: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L2514;
    }
  x2 = XEXP (x1, 0);
  goto L2652;

  L2514: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L2515;
    }
  x2 = XEXP (x1, 0);
  goto L2652;

  L2515: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L2516;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2652;

  L2516: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L2517;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2652;

  L2517: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == MINUS && 1)
    goto L2518;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2652;

  L2518: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L2519;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2652;

  L2519: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[2]) && 1)
    if (REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM))
      return 342;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2652;

  L2653: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == QImode && GET_CODE (x2) == MEM && 1)
    goto L2654;
  goto ret0;

  L2654: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) != SImode)
    goto ret0;
  switch (GET_CODE (x3))
    {
    default:
      break;
    case PLUS:
      goto L2655;
    case MINUS:
      goto L2671;
    }
  goto ret0;

  L2655: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (shift_operator (x4, SImode))
    {
      ro[2] = x4;
      goto L2656;
    }
  goto ret0;

  L2656: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      ro[3] = x5;
      goto L2657;
    }
  goto ret0;

  L2657: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT && const_shift_operand (x5, SImode))
    {
      ro[4] = x5;
      goto L2658;
    }
  goto ret0;

  L2658: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L2659;
    }
  goto ret0;

  L2659: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L2660;
  goto ret0;

  L2660: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L2661;
    }
  goto ret0;

  L2661: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == PLUS && 1)
    goto L2662;
  goto ret0;

  L2662: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[2]) && 1)
    {
      ro[2] = x3;
      goto L2663;
    }
  goto ret0;

  L2663: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, ro[3]) && 1)
    goto L2664;
  goto ret0;

  L2664: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, ro[4]) && 1)
    goto L2665;
  goto ret0;

  L2665: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[1]) && 1)
    if (REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && REGNO (operands[3]) != FRAME_POINTER_REGNUM)
      return 353;
  goto ret0;

  L2671: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L2672;
    }
  goto ret0;

  L2672: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (shift_operator (x4, SImode))
    {
      ro[2] = x4;
      goto L2673;
    }
  goto ret0;

  L2673: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      ro[3] = x5;
      goto L2674;
    }
  goto ret0;

  L2674: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT && const_shift_operand (x5, SImode))
    {
      ro[4] = x5;
      goto L2675;
    }
  goto ret0;

  L2675: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L2676;
  goto ret0;

  L2676: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L2677;
    }
  goto ret0;

  L2677: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == MINUS && 1)
    goto L2678;
  goto ret0;

  L2678: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L2679;
  goto ret0;

  L2679: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[2]) && 1)
    {
      ro[2] = x3;
      goto L2680;
    }
  goto ret0;

  L2680: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, ro[3]) && 1)
    goto L2681;
  goto ret0;

  L2681: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, ro[4]) && 1)
    if (REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && REGNO (operands[3]) != FRAME_POINTER_REGNUM)
      return 354;
  goto ret0;

  L2597: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == HImode && GET_CODE (x2) == MEM && 1)
    goto L2598;
  x2 = XEXP (x1, 0);
  goto L2748;

  L2598: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) != SImode)
    {
      x2 = XEXP (x1, 0);
      goto L2748;
    }
  switch (GET_CODE (x3))
    {
    default:
      break;
    case PLUS:
      goto L2599;
    case MINUS:
      goto L2611;
    }
  x2 = XEXP (x1, 0);
  goto L2748;

  L2599: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L2600;
    }
  x2 = XEXP (x1, 0);
  goto L2748;

  L2600: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (index_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L2601;
    }
  x2 = XEXP (x1, 0);
  goto L2748;

  L2601: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L2602;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2748;

  L2602: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L2603;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2748;

  L2603: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == PLUS && 1)
    goto L2604;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2748;

  L2604: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L2605;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2748;

  L2605: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[2]) && 1)
    if ((! BYTES_BIG_ENDIAN)
   && ! TARGET_SHORT_BY_BYTES
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM))
      return 349;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2748;

  L2611: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L2612;
    }
  x2 = XEXP (x1, 0);
  goto L2748;

  L2612: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L2613;
    }
  x2 = XEXP (x1, 0);
  goto L2748;

  L2613: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L2614;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2748;

  L2614: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L2615;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2748;

  L2615: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == MINUS && 1)
    goto L2616;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2748;

  L2616: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L2617;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2748;

  L2617: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[2]) && 1)
    if ((!BYTES_BIG_ENDIAN)
   && ! TARGET_SHORT_BY_BYTES
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM))
      return 350;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2748;

  L2749: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == HImode && GET_CODE (x2) == MEM && 1)
    goto L2750;
  goto ret0;

  L2750: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) != SImode)
    goto ret0;
  switch (GET_CODE (x3))
    {
    default:
      break;
    case PLUS:
      goto L2751;
    case MINUS:
      goto L2767;
    }
  goto ret0;

  L2751: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (shift_operator (x4, SImode))
    {
      ro[2] = x4;
      goto L2752;
    }
  goto ret0;

  L2752: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      ro[3] = x5;
      goto L2753;
    }
  goto ret0;

  L2753: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT && const_shift_operand (x5, SImode))
    {
      ro[4] = x5;
      goto L2754;
    }
  goto ret0;

  L2754: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L2755;
    }
  goto ret0;

  L2755: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L2756;
  goto ret0;

  L2756: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L2757;
    }
  goto ret0;

  L2757: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == PLUS && 1)
    goto L2758;
  goto ret0;

  L2758: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[2]) && 1)
    {
      ro[2] = x3;
      goto L2759;
    }
  goto ret0;

  L2759: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, ro[3]) && 1)
    goto L2760;
  goto ret0;

  L2760: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, ro[4]) && 1)
    goto L2761;
  goto ret0;

  L2761: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[1]) && 1)
    if ((! BYTES_BIG_ENDIAN)
   && ! TARGET_SHORT_BY_BYTES
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && REGNO (operands[3]) != FRAME_POINTER_REGNUM)
      return 359;
  goto ret0;

  L2767: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L2768;
    }
  goto ret0;

  L2768: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (shift_operator (x4, SImode))
    {
      ro[2] = x4;
      goto L2769;
    }
  goto ret0;

  L2769: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      ro[3] = x5;
      goto L2770;
    }
  goto ret0;

  L2770: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT && const_shift_operand (x5, SImode))
    {
      ro[4] = x5;
      goto L2771;
    }
  goto ret0;

  L2771: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L2772;
  goto ret0;

  L2772: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L2773;
    }
  goto ret0;

  L2773: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == MINUS && 1)
    goto L2774;
  goto ret0;

  L2774: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L2775;
  goto ret0;

  L2775: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[2]) && 1)
    {
      ro[2] = x3;
      goto L2776;
    }
  goto ret0;

  L2776: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, ro[3]) && 1)
    goto L2777;
  goto ret0;

  L2777: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, ro[4]) && 1)
    if ((! BYTES_BIG_ENDIAN)
   && ! TARGET_SHORT_BY_BYTES
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && REGNO (operands[3]) != FRAME_POINTER_REGNUM)
      return 360;
  goto ret0;
 ret0: return -1;
}

int
recog_8 (x0, insn, pnum_clobbers)
     register rtx x0;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  register rtx *ro = &recog_operand[0];
  register rtx x1 ATTRIBUTE_UNUSED, x2 ATTRIBUTE_UNUSED, x3 ATTRIBUTE_UNUSED, x4 ATTRIBUTE_UNUSED, x5 ATTRIBUTE_UNUSED, x6 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;

  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) != DImode)
    goto ret0;
  switch (GET_CODE (x1))
    {
    default:
      break;
    case PLUS:
      goto L25;
    case MINUS:
      goto L252;
    case MULT:
      goto L434;
    case AND:
      goto L601;
    case IOR:
      goto L742;
    case XOR:
      goto L799;
    case NEG:
      goto L1020;
    case NOT:
      goto L1103;
    case ZERO_EXTEND:
      goto L1173;
    case SIGN_EXTEND:
      goto L1181;
    }
  goto ret0;

  L25: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) != DImode)
    goto ret0;
  switch (GET_CODE (x2))
    {
    default:
      break;
    case SIGN_EXTEND:
      goto L26;
    case ZERO_EXTEND:
      goto L41;
    case SUBREG:
    case REG:
      if (s_register_operand (x2, DImode))
	{
	  ro[1] = x2;
	  goto L12;
	}
    }
  goto ret0;

  L26: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L27;
    }
  goto ret0;

  L27: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (pnum_clobbers != 0 && s_register_operand (x2, DImode))
    {
      ro[1] = x2;
      *pnum_clobbers = 1;
      return 1;
    }
  goto ret0;

  L41: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L42;
    }
  goto ret0;

  L42: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (pnum_clobbers != 0 && s_register_operand (x2, DImode))
    {
      ro[1] = x2;
      *pnum_clobbers = 1;
      return 2;
    }
  goto ret0;

  L12: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (pnum_clobbers != 0 && s_register_operand (x2, DImode))
    {
      ro[2] = x2;
      *pnum_clobbers = 1;
      return 0;
    }
  goto ret0;

  L252: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) != DImode)
    goto ret0;
  switch (GET_CODE (x2))
    {
    default:
      break;
    case ZERO_EXTEND:
      goto L253;
    case SIGN_EXTEND:
      goto L268;
    case SUBREG:
    case REG:
      if (s_register_operand (x2, DImode))
	{
	  ro[1] = x2;
	  goto L223;
	}
    }
  goto ret0;

  L253: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) != SImode)
    goto ret0;
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L254;
    }
  L284: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L285;
    }
  goto ret0;

  L254: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (pnum_clobbers != 0 && s_register_operand (x2, DImode))
    {
      ro[1] = x2;
      *pnum_clobbers = 1;
      return 27;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L284;

  L285: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == DImode && GET_CODE (x2) == ZERO_EXTEND && 1)
    goto L286;
  goto ret0;

  L286: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (pnum_clobbers != 0 && s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      *pnum_clobbers = 1;
      return 29;
    }
  goto ret0;

  L268: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L269;
    }
  goto ret0;

  L269: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (pnum_clobbers != 0 && s_register_operand (x2, DImode))
    {
      ro[1] = x2;
      *pnum_clobbers = 1;
      return 28;
    }
  goto ret0;

  L223: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) != DImode)
    goto ret0;
  switch (GET_CODE (x2))
    {
    default:
      break;
    case ZERO_EXTEND:
      goto L224;
    case SIGN_EXTEND:
      goto L239;
    case SUBREG:
    case REG:
      if (pnum_clobbers != 0 && s_register_operand (x2, DImode))
	{
	  ro[2] = x2;
	  *pnum_clobbers = 1;
	  return 24;
	}
    }
  goto ret0;

  L224: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (pnum_clobbers != 0 && s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      *pnum_clobbers = 1;
      return 25;
    }
  goto ret0;

  L239: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (pnum_clobbers != 0 && s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      *pnum_clobbers = 1;
      return 26;
    }
  goto ret0;

  L434: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) != DImode)
    goto ret0;
  switch (GET_CODE (x2))
    {
    default:
      break;
    case SIGN_EXTEND:
      goto L435;
    case ZERO_EXTEND:
      goto L442;
    }
  goto ret0;

  L435: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L436;
    }
  goto ret0;

  L436: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == DImode && GET_CODE (x2) == SIGN_EXTEND && 1)
    goto L437;
  goto ret0;

  L437: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      if (arm_fast_multiply)
	return 47;
      }
  goto ret0;

  L442: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L443;
    }
  goto ret0;

  L443: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == DImode && GET_CODE (x2) == ZERO_EXTEND && 1)
    goto L444;
  goto ret0;

  L444: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      if (arm_fast_multiply)
	return 48;
      }
  goto ret0;

  L601: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) != DImode)
    goto ret0;
  switch (GET_CODE (x2))
    {
    default:
      break;
    case ZERO_EXTEND:
      goto L602;
    case SIGN_EXTEND:
      goto L608;
    case NOT:
      goto L676;
    case SUBREG:
    case REG:
      if (s_register_operand (x2, DImode))
	{
	  ro[1] = x2;
	  goto L597;
	}
    }
  goto ret0;

  L602: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L603;
    }
  goto ret0;

  L603: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      ro[1] = x2;
      return 70;
    }
  goto ret0;

  L608: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L609;
    }
  goto ret0;

  L609: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      ro[1] = x2;
      return 71;
    }
  goto ret0;

  L676: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) != DImode)
    goto ret0;
  switch (GET_CODE (x3))
    {
    default:
      break;
    case ZERO_EXTEND:
      goto L677;
    case SIGN_EXTEND:
      goto L684;
    case SUBREG:
    case REG:
      if (s_register_operand (x3, DImode))
	{
	  ro[2] = x3;
	  goto L671;
	}
    }
  goto ret0;

  L677: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L678;
    }
  goto ret0;

  L678: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      ro[1] = x2;
      return 81;
    }
  goto ret0;

  L684: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L685;
    }
  goto ret0;

  L685: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      ro[1] = x2;
      return 82;
    }
  goto ret0;

  L671: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      ro[1] = x2;
      return 80;
    }
  goto ret0;

  L597: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      ro[2] = x2;
      return 69;
    }
  goto ret0;

  L742: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) != DImode)
    goto ret0;
  switch (GET_CODE (x2))
    {
    default:
      break;
    case ZERO_EXTEND:
      goto L743;
    case SIGN_EXTEND:
      goto L749;
    case SUBREG:
    case REG:
      if (s_register_operand (x2, DImode))
	{
	  ro[1] = x2;
	  goto L738;
	}
    }
  goto ret0;

  L743: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L744;
    }
  goto ret0;

  L744: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      ro[1] = x2;
      return 88;
    }
  goto ret0;

  L749: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L750;
    }
  goto ret0;

  L750: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      ro[1] = x2;
      return 89;
    }
  goto ret0;

  L738: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      ro[2] = x2;
      return 87;
    }
  goto ret0;

  L799: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) != DImode)
    goto ret0;
  switch (GET_CODE (x2))
    {
    default:
      break;
    case ZERO_EXTEND:
      goto L800;
    case SIGN_EXTEND:
      goto L806;
    case SUBREG:
    case REG:
      if (s_register_operand (x2, DImode))
	{
	  ro[1] = x2;
	  goto L795;
	}
    }
  goto ret0;

  L800: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L801;
    }
  goto ret0;

  L801: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      ro[1] = x2;
      return 96;
    }
  goto ret0;

  L806: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L807;
    }
  goto ret0;

  L807: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      ro[1] = x2;
      return 97;
    }
  goto ret0;

  L795: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      ro[2] = x2;
      return 95;
    }
  goto ret0;

  L1020: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (pnum_clobbers != 0 && s_register_operand (x2, DImode))
    {
      ro[1] = x2;
      *pnum_clobbers = 1;
      return 120;
    }
  goto ret0;

  L1103: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, DImode))
    {
      ro[1] = x2;
      return 136;
    }
  goto ret0;

  L1173: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  switch (GET_MODE (x2))
    {
    default:
      break;
    case SImode:
      if (s_register_operand (x2, SImode))
	{
	  ro[1] = x2;
	  return 149;
	}
      break;
    case QImode:
      if (nonimmediate_operand (x2, QImode))
	{
	  ro[1] = x2;
	  return 150;
	}
    }
  goto ret0;

  L1181: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      return 151;
    }
  goto ret0;
 ret0: return -1;
}

int
recog_9 (x0, insn, pnum_clobbers)
     register rtx x0;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  register rtx *ro = &recog_operand[0];
  register rtx x1 ATTRIBUTE_UNUSED, x2 ATTRIBUTE_UNUSED, x3 ATTRIBUTE_UNUSED, x4 ATTRIBUTE_UNUSED, x5 ATTRIBUTE_UNUSED, x6 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;

  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) != SImode)
    goto ret0;
  switch (GET_CODE (x1))
    {
    default:
      break;
    case PLUS:
      goto L116;
    case MINUS:
      goto L313;
    case MULT:
      goto L354;
    case TRUNCATE:
      goto L461;
    case AND:
      goto L689;
    case NE:
      goto L661;
    case IOR:
      goto L754;
    case XOR:
      goto L811;
    case SMAX:
      goto L864;
    case SMIN:
      goto L877;
    case UMAX:
      goto L890;
    case UMIN:
      goto L903;
    case NOT:
      goto L973;
    case NEG:
      goto L1064;
    case ABS:
      goto L1052;
    case FIX:
      goto L1149;
    case ZERO_EXTEND:
      goto L1185;
    case SIGN_EXTEND:
      goto L1222;
    }
  L930: ATTRIBUTE_UNUSED_LABEL
  if (shiftable_operator (x1, SImode))
    {
      ro[4] = x1;
      goto L931;
    }
  L937: ATTRIBUTE_UNUSED_LABEL
  if (shift_operator (x1, SImode))
    {
      ro[3] = x1;
      goto L938;
    }
  goto ret0;

  L116: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) != SImode)
    {
      goto L930;
    }
  switch (GET_CODE (x2))
    {
    default:
      break;
    case LTU:
      goto L117;
    case PLUS:
      goto L135;
    case MULT:
      goto L390;
    case SUBREG:
    case REG:
      if (s_register_operand (x2, SImode))
	{
	  ro[1] = x2;
	  goto L52;
	}
    }
  L152: ATTRIBUTE_UNUSED_LABEL
  if (comparison_operator (x2, SImode))
    {
      ro[2] = x2;
      goto L153;
    }
  goto L930;

  L117: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == CC_Cmode && GET_CODE (x3) == REG && XINT (x3, 0) == 24 && 1)
    goto L118;
  goto L152;

  L118: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L119;
  goto L152;

  L119: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == PLUS && 1)
    goto L120;
  x2 = XEXP (x1, 0);
  goto L152;

  L120: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L121;
    }
  x2 = XEXP (x1, 0);
  goto L152;

  L121: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[2] = x3;
      return 12;
    }
  x2 = XEXP (x1, 0);
  goto L152;

  L135: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) != SImode)
    {
      goto L930;
    }
  if (GET_CODE (x3) == LTU && 1)
    goto L136;
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L127;
    }
  goto L930;

  L136: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == CC_Cmode && GET_CODE (x4) == REG && XINT (x4, 0) == 24 && 1)
    goto L137;
  goto L930;

  L137: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT && XWINT (x4, 0) == 0 && 1)
    goto L138;
  goto L930;

  L138: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L139;
    }
  L147: ATTRIBUTE_UNUSED_LABEL
  if (arm_rhs_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L148;
    }
  goto L930;

  L139: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_rhs_operand (x2, SImode))
    {
      ro[2] = x2;
      return 14;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 1);
  goto L147;

  L148: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      return 15;
    }
  goto L930;

  L127: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L128;
    }
  goto L930;

  L128: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == LTU && 1)
    goto L129;
  goto L930;

  L129: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == CC_Cmode && GET_CODE (x3) == REG && XINT (x3, 0) == 24 && 1)
    goto L130;
  goto L930;

  L130: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    return 13;
  goto L930;

  L390: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L391;
    }
  goto L930;

  L391: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L392;
    }
  goto L930;

  L392: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      ro[3] = x2;
      return 44;
    }
  goto L930;

  L52: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (reg_or_int_operand (x2, SImode))
    {
      ro[2] = x2;
      return 5;
    }
  goto L930;

  L153: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_CODE (x3) == REG && cc_register (x3, VOIDmode))
    {
      ro[3] = x3;
      goto L154;
    }
  goto L930;

  L154: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L155;
  goto L930;

  L155: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      return 16;
    }
  goto L930;

  L313: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L314;
    }
  L290: ATTRIBUTE_UNUSED_LABEL
  if (reg_or_int_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L291;
    }
  goto L930;

  L314: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (comparison_operator (x2, SImode))
    {
      ro[2] = x2;
      goto L315;
    }
  x2 = XEXP (x1, 0);
  goto L290;

  L315: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_CODE (x3) == REG && cc_register (x3, VOIDmode))
    {
      ro[3] = x3;
      goto L316;
    }
  x2 = XEXP (x1, 0);
  goto L290;

  L316: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    return 34;
  x2 = XEXP (x1, 0);
  goto L290;

  L291: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      ro[2] = x2;
      return 31;
    }
  goto L930;

  L354: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[2] = x2;
      goto L355;
    }
  goto L937;

  L355: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      return 41;
    }
  goto L937;

  L461: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == DImode && GET_CODE (x2) == LSHIFTRT && 1)
    goto L462;
  goto ret0;

  L462: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == DImode && GET_CODE (x3) == MULT && 1)
    goto L463;
  goto ret0;

  L463: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) != DImode)
    goto ret0;
  switch (GET_CODE (x4))
    {
    default:
      break;
    case SIGN_EXTEND:
      goto L464;
    case ZERO_EXTEND:
      goto L487;
    }
  goto ret0;

  L464: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      ro[1] = x5;
      goto L465;
    }
  goto ret0;

  L465: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_MODE (x4) == DImode && GET_CODE (x4) == SIGN_EXTEND && 1)
    goto L466;
  goto ret0;

  L466: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      ro[2] = x5;
      goto L467;
    }
  goto ret0;

  L467: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 32 && pnum_clobbers != 0 && 1)
    if (arm_fast_multiply)
      {
	*pnum_clobbers = 1;
	return 49;
      }
  goto ret0;

  L487: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      ro[1] = x5;
      goto L488;
    }
  goto ret0;

  L488: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_MODE (x4) == DImode && GET_CODE (x4) == ZERO_EXTEND && 1)
    goto L489;
  goto ret0;

  L489: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      ro[2] = x5;
      goto L490;
    }
  goto ret0;

  L490: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 32 && pnum_clobbers != 0 && 1)
    if (arm_fast_multiply)
      {
	*pnum_clobbers = 1;
	return 50;
      }
  goto ret0;

  L689: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) != SImode)
    {
      goto L930;
    }
  switch (GET_CODE (x2))
    {
    default:
      break;
    case NOT:
      goto L690;
    case IOR:
      goto L849;
    case SUBREG:
    case REG:
      if (s_register_operand (x2, SImode))
	{
	  ro[1] = x2;
	  goto L614;
	}
    }
  goto L930;

  L690: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) != SImode)
    {
      goto L930;
    }
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L691;
    }
  if (shift_operator (x3, SImode))
    {
      ro[4] = x3;
      goto L697;
    }
  goto L930;

  L691: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      return 83;
    }
  goto L930;

  L697: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L698;
    }
  goto L930;

  L698: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      ro[3] = x4;
      goto L699;
    }
  goto L930;

  L699: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      return 84;
    }
  goto L930;

  L849: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L850;
    }
  goto L930;

  L850: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L851;
    }
  goto L930;

  L851: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == NOT && 1)
    goto L852;
  goto L930;

  L852: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[3] = x3;
      return 102;
    }
  goto L930;

  L614: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (reg_or_int_operand (x2, SImode))
    {
      ro[2] = x2;
      return 73;
    }
  goto L930;

  L661: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == ZERO_EXTRACT && 1)
    goto L662;
  goto ret0;

  L662: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L663;
    }
  goto ret0;

  L663: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && 1)
    {
      ro[2] = x3;
      goto L664;
    }
  goto ret0;

  L664: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (GET_CODE (x3) == CONST_INT && 1)
    {
      ro[3] = x3;
      goto L665;
    }
  goto ret0;

  L665: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && 1)
    if (INTVAL (operands[3]) >= 0 && INTVAL (operands[3]) < 32
   && INTVAL (operands[2]) > 0 
   && INTVAL (operands[2]) + (INTVAL (operands[3]) & 1) <= 8
   && INTVAL (operands[2]) + INTVAL (operands[3]) <= 32)
      return 78;
  goto ret0;

  L754: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L755;
    }
  goto L930;

  L755: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (reg_or_int_operand (x2, SImode))
    {
      ro[2] = x2;
      return 91;
    }
  goto L930;

  L811: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L812;
    }
  goto L930;

  L812: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_rhs_operand (x2, SImode))
    {
      ro[2] = x2;
      return 98;
    }
  goto L930;

  L864: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L865;
    }
  goto ret0;

  L865: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (pnum_clobbers != 0 && arm_rhs_operand (x2, SImode))
    {
      ro[2] = x2;
      *pnum_clobbers = 1;
      return 103;
    }
  goto ret0;

  L877: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L878;
    }
  goto ret0;

  L878: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (pnum_clobbers != 0 && arm_rhs_operand (x2, SImode))
    {
      ro[2] = x2;
      *pnum_clobbers = 1;
      return 104;
    }
  goto ret0;

  L890: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L891;
    }
  goto ret0;

  L891: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (pnum_clobbers != 0 && arm_rhs_operand (x2, SImode))
    {
      ro[2] = x2;
      *pnum_clobbers = 1;
      return 105;
    }
  goto ret0;

  L903: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L904;
    }
  goto ret0;

  L904: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (pnum_clobbers != 0 && arm_rhs_operand (x2, SImode))
    {
      ro[2] = x2;
      *pnum_clobbers = 1;
      return 106;
    }
  goto ret0;

  L973: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) != SImode)
    goto ret0;
  if (shift_operator (x2, SImode))
    {
      ro[3] = x2;
      goto L974;
    }
  if (s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      return 137;
    }
  goto ret0;

  L974: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L975;
    }
  goto ret0;

  L975: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[2] = x3;
      return 117;
    }
  goto ret0;

  L1064: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) != SImode)
    goto ret0;
  if (GET_CODE (x2) == ABS && 1)
    goto L1065;
  if (s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      return 121;
    }
  goto ret0;

  L1065: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (pnum_clobbers != 0 && s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      *pnum_clobbers = 1;
      return 127;
    }
  goto ret0;

  L1052: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (pnum_clobbers != 0 && s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      *pnum_clobbers = 1;
      return 126;
    }
  goto ret0;

  L1149: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  switch (GET_MODE (x2))
    {
    default:
      break;
    case SFmode:
      if (s_register_operand (x2, SFmode))
	{
	  ro[1] = x2;
	  if (TARGET_HARD_FLOAT)
	    return 143;
	  }
      break;
    case DFmode:
      if (s_register_operand (x2, DFmode))
	{
	  ro[1] = x2;
	  if (TARGET_HARD_FLOAT)
	    return 144;
	  }
      break;
    case XFmode:
      if (s_register_operand (x2, XFmode))
	{
	  ro[1] = x2;
	  if (ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)
	    return 145;
	  }
    }
  goto ret0;

  L1185: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  switch (GET_MODE (x2))
    {
    default:
      break;
    case HImode:
      if (memory_operand (x2, HImode))
	{
	  ro[1] = x2;
	  if (arm_arch4 && ! ARM_ARCH_RISCPC)
	    return 153;
	  }
      break;
    case QImode:
      if (memory_operand (x2, QImode))
	{
	  ro[1] = x2;
	  return 157;
	}
    }
  goto ret0;

  L1222: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  switch (GET_MODE (x2))
    {
    default:
      break;
    case HImode:
      if (memory_operand (x2, HImode))
	{
	  ro[1] = x2;
	  if (arm_arch4 && ! ARM_ARCH_RISCPC)
	    return 162;
	  }
      break;
    case QImode:
      if (memory_operand (x2, QImode))
	{
	  ro[1] = x2;
	  if (arm_arch4)
	    return 169;
	  }
    }
  goto ret0;

  L931: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (minmax_operator (x2, SImode))
    {
      ro[5] = x2;
      goto L932;
    }
  goto ret0;

  L932: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L933;
    }
  goto ret0;

  L933: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[3] = x3;
      goto L934;
    }
  goto ret0;

  L934: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (pnum_clobbers != 0 && s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      if (GET_CODE (operands[1]) != REG
   || (REGNO(operands[1]) != FRAME_POINTER_REGNUM
       && REGNO(operands[1]) != ARG_POINTER_REGNUM))
	{
	  *pnum_clobbers = 1;
	  return 108;
	}
      }
  goto ret0;

  L938: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L939;
    }
  goto ret0;

  L939: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (reg_or_int_operand (x2, SImode))
    {
      ro[2] = x2;
      return 114;
    }
  goto ret0;
 ret0: return -1;
}

int
recog_10 (x0, insn, pnum_clobbers)
     register rtx x0;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  register rtx *ro = &recog_operand[0];
  register rtx x1 ATTRIBUTE_UNUSED, x2 ATTRIBUTE_UNUSED, x3 ATTRIBUTE_UNUSED, x4 ATTRIBUTE_UNUSED, x5 ATTRIBUTE_UNUSED, x6 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;

  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) != DFmode)
    goto ret0;
  switch (GET_CODE (x1))
    {
    default:
      break;
    case PLUS:
      goto L176;
    case MINUS:
      goto L330;
    case MULT:
      goto L504;
    case DIV:
      goto L538;
    case MOD:
      goto L572;
    case NEG:
      goto L1036;
    case ABS:
      goto L1077;
    case SQRT:
      goto L1094;
    case FLOAT:
      goto L1141;
    case FLOAT_TRUNCATE:
      goto L1169;
    case FLOAT_EXTEND:
      goto L1258;
    }
  goto ret0;

  L176: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) != DFmode)
    goto ret0;
  if (GET_CODE (x2) == FLOAT_EXTEND && 1)
    goto L177;
  if (s_register_operand (x2, DFmode))
    {
      ro[1] = x2;
      goto L183;
    }
  goto ret0;

  L177: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      ro[1] = x3;
      goto L190;
    }
  goto ret0;

  L190: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) != DFmode)
    goto ret0;
  if (GET_CODE (x2) == FLOAT_EXTEND && 1)
    goto L191;
  if (fpu_add_operand (x2, DFmode))
    {
      ro[2] = x2;
      if (TARGET_HARD_FLOAT)
	return 20;
      }
  goto ret0;

  L191: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      ro[2] = x3;
      if (TARGET_HARD_FLOAT)
	return 22;
      }
  goto ret0;

  L183: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) != DFmode)
    goto ret0;
  if (GET_CODE (x2) == FLOAT_EXTEND && 1)
    goto L184;
  if (fpu_add_operand (x2, DFmode))
    {
      ro[2] = x2;
      if (TARGET_HARD_FLOAT)
	return 19;
      }
  goto ret0;

  L184: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      ro[2] = x3;
      if (TARGET_HARD_FLOAT)
	return 21;
      }
  goto ret0;

  L330: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) != DFmode)
    goto ret0;
  if (GET_CODE (x2) == FLOAT_EXTEND && 1)
    goto L331;
  if (fpu_rhs_operand (x2, DFmode))
    {
      ro[1] = x2;
      goto L337;
    }
  goto ret0;

  L331: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      ro[1] = x3;
      goto L344;
    }
  goto ret0;

  L344: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) != DFmode)
    goto ret0;
  if (GET_CODE (x2) == FLOAT_EXTEND && 1)
    goto L345;
  if (fpu_rhs_operand (x2, DFmode))
    {
      ro[2] = x2;
      if (TARGET_HARD_FLOAT)
	return 37;
      }
  goto ret0;

  L345: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      ro[2] = x3;
      if (TARGET_HARD_FLOAT)
	return 39;
      }
  goto ret0;

  L337: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) != DFmode)
    goto ret0;
  if (GET_CODE (x2) == FLOAT_EXTEND && 1)
    goto L338;
  if (fpu_rhs_operand (x2, DFmode))
    {
      ro[2] = x2;
      if (TARGET_HARD_FLOAT)
	return 36;
      }
  goto ret0;

  L338: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      ro[2] = x3;
      if (TARGET_HARD_FLOAT)
	return 38;
      }
  goto ret0;

  L504: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) != DFmode)
    goto ret0;
  if (GET_CODE (x2) == FLOAT_EXTEND && 1)
    goto L505;
  if (s_register_operand (x2, DFmode))
    {
      ro[1] = x2;
      goto L511;
    }
  goto ret0;

  L505: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      ro[1] = x3;
      goto L518;
    }
  goto ret0;

  L518: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) != DFmode)
    goto ret0;
  if (GET_CODE (x2) == FLOAT_EXTEND && 1)
    goto L519;
  if (fpu_rhs_operand (x2, DFmode))
    {
      ro[2] = x2;
      if (TARGET_HARD_FLOAT)
	return 53;
      }
  goto ret0;

  L519: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      ro[2] = x3;
      if (TARGET_HARD_FLOAT)
	return 55;
      }
  goto ret0;

  L511: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) != DFmode)
    goto ret0;
  if (GET_CODE (x2) == FLOAT_EXTEND && 1)
    goto L512;
  if (fpu_rhs_operand (x2, DFmode))
    {
      ro[2] = x2;
      if (TARGET_HARD_FLOAT)
	return 52;
      }
  goto ret0;

  L512: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      ro[2] = x3;
      if (TARGET_HARD_FLOAT)
	return 54;
      }
  goto ret0;

  L538: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) != DFmode)
    goto ret0;
  if (GET_CODE (x2) == FLOAT_EXTEND && 1)
    goto L539;
  if (fpu_rhs_operand (x2, DFmode))
    {
      ro[1] = x2;
      goto L545;
    }
  goto ret0;

  L539: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      ro[1] = x3;
      goto L552;
    }
  goto ret0;

  L552: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) != DFmode)
    goto ret0;
  if (GET_CODE (x2) == FLOAT_EXTEND && 1)
    goto L553;
  if (fpu_rhs_operand (x2, DFmode))
    {
      ro[2] = x2;
      if (TARGET_HARD_FLOAT)
	return 59;
      }
  goto ret0;

  L553: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      ro[2] = x3;
      if (TARGET_HARD_FLOAT)
	return 61;
      }
  goto ret0;

  L545: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) != DFmode)
    goto ret0;
  if (GET_CODE (x2) == FLOAT_EXTEND && 1)
    goto L546;
  if (fpu_rhs_operand (x2, DFmode))
    {
      ro[2] = x2;
      if (TARGET_HARD_FLOAT)
	return 58;
      }
  goto ret0;

  L546: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      ro[2] = x3;
      if (TARGET_HARD_FLOAT)
	return 60;
      }
  goto ret0;

  L572: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) != DFmode)
    goto ret0;
  if (GET_CODE (x2) == FLOAT_EXTEND && 1)
    goto L573;
  if (s_register_operand (x2, DFmode))
    {
      ro[1] = x2;
      goto L579;
    }
  goto ret0;

  L573: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      ro[1] = x3;
      goto L586;
    }
  goto ret0;

  L586: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) != DFmode)
    goto ret0;
  if (GET_CODE (x2) == FLOAT_EXTEND && 1)
    goto L587;
  if (fpu_rhs_operand (x2, DFmode))
    {
      ro[2] = x2;
      if (TARGET_HARD_FLOAT)
	return 65;
      }
  goto ret0;

  L587: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      ro[2] = x3;
      if (TARGET_HARD_FLOAT)
	return 67;
      }
  goto ret0;

  L579: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) != DFmode)
    goto ret0;
  if (GET_CODE (x2) == FLOAT_EXTEND && 1)
    goto L580;
  if (fpu_rhs_operand (x2, DFmode))
    {
      ro[2] = x2;
      if (TARGET_HARD_FLOAT)
	return 64;
      }
  goto ret0;

  L580: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      ro[2] = x3;
      if (TARGET_HARD_FLOAT)
	return 66;
      }
  goto ret0;

  L1036: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) != DFmode)
    goto ret0;
  if (GET_CODE (x2) == FLOAT_EXTEND && 1)
    goto L1037;
  if (s_register_operand (x2, DFmode))
    {
      ro[1] = x2;
      if (TARGET_HARD_FLOAT)
	return 123;
      }
  goto ret0;

  L1037: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      ro[1] = x3;
      if (TARGET_HARD_FLOAT)
	return 124;
      }
  goto ret0;

  L1077: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) != DFmode)
    goto ret0;
  if (GET_CODE (x2) == FLOAT_EXTEND && 1)
    goto L1078;
  if (s_register_operand (x2, DFmode))
    {
      ro[1] = x2;
      if (TARGET_HARD_FLOAT)
	return 129;
      }
  goto ret0;

  L1078: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      ro[1] = x3;
      if (TARGET_HARD_FLOAT)
	return 130;
      }
  goto ret0;

  L1094: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) != DFmode)
    goto ret0;
  if (GET_CODE (x2) == FLOAT_EXTEND && 1)
    goto L1095;
  if (s_register_operand (x2, DFmode))
    {
      ro[1] = x2;
      if (TARGET_HARD_FLOAT)
	return 133;
      }
  goto ret0;

  L1095: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      ro[1] = x3;
      if (TARGET_HARD_FLOAT)
	return 134;
      }
  goto ret0;

  L1141: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      if (TARGET_HARD_FLOAT)
	return 141;
      }
  goto ret0;

  L1169: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, XFmode))
    {
      ro[1] = x2;
      if (ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)
	return 148;
      }
  goto ret0;

  L1258: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SFmode))
    {
      ro[1] = x2;
      if (TARGET_HARD_FLOAT)
	return 171;
      }
  goto ret0;
 ret0: return -1;
}

int
recog_11 (x0, insn, pnum_clobbers)
     register rtx x0;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  register rtx *ro = &recog_operand[0];
  register rtx x1 ATTRIBUTE_UNUSED, x2 ATTRIBUTE_UNUSED, x3 ATTRIBUTE_UNUSED, x4 ATTRIBUTE_UNUSED, x5 ATTRIBUTE_UNUSED, x6 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;

  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  if (equality_operator (x2, VOIDmode))
    {
      ro[3] = x2;
      goto L1893;
    }
  L1986: ATTRIBUTE_UNUSED_LABEL
  if (comparison_operator (x2, VOIDmode))
    {
      ro[5] = x2;
      goto L1987;
    }
  L2007: ATTRIBUTE_UNUSED_LABEL
  if (comparison_operator (x2, VOIDmode))
    {
      ro[6] = x2;
      goto L2008;
    }
  L2017: ATTRIBUTE_UNUSED_LABEL
  if (comparison_operator (x2, VOIDmode))
    {
      ro[4] = x2;
      goto L2018;
    }
  L2075: ATTRIBUTE_UNUSED_LABEL
  if (comparison_operator (x2, VOIDmode))
    {
      ro[9] = x2;
      goto L2076;
    }
  L2087: ATTRIBUTE_UNUSED_LABEL
  if (comparison_operator (x2, VOIDmode))
    {
      ro[5] = x2;
      goto L2178;
    }
  L2112: ATTRIBUTE_UNUSED_LABEL
  if (comparison_operator (x2, VOIDmode))
    {
      ro[6] = x2;
      goto L2113;
    }
  L2122: ATTRIBUTE_UNUSED_LABEL
  if (comparison_operator (x2, VOIDmode))
    {
      ro[4] = x2;
      goto L2123;
    }
  L2248: ATTRIBUTE_UNUSED_LABEL
  if (comparison_operator (x2, VOIDmode))
    {
      ro[5] = x2;
      goto L2249;
    }
  L2306: ATTRIBUTE_UNUSED_LABEL
  if (comparison_operator (x2, VOIDmode))
    {
      ro[7] = x2;
      goto L2307;
    }
  L2318: ATTRIBUTE_UNUSED_LABEL
  if (comparison_operator (x2, VOIDmode))
    {
      ro[5] = x2;
      goto L2415;
    }
  L2423: ATTRIBUTE_UNUSED_LABEL
  if (comparison_operator (x2, VOIDmode))
    {
      ro[4] = x2;
      goto L2424;
    }
  goto ret0;

  L1893: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (comparison_operator (x3, VOIDmode))
    {
      ro[4] = x3;
      goto L1894;
    }
  goto L1986;

  L1894: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_CODE (x4) == REG && cc_register (x4, VOIDmode))
    {
      ro[5] = x4;
      goto L1895;
    }
  goto L1986;

  L1895: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT && XWINT (x4, 0) == 0 && 1)
    goto L1896;
  goto L1986;

  L1896: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L1897;
  goto L1986;

  L1897: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_rhs_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L1898;
    }
  x2 = XEXP (x1, 0);
  goto L1986;

  L1898: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (arm_rhs_operand (x2, SImode))
    {
      ro[2] = x2;
      return 303;
    }
  x2 = XEXP (x1, 0);
  goto L1986;

  L1987: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[3] = x3;
      goto L1988;
    }
  goto L2007;

  L1988: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_add_operand (x3, SImode))
    {
      ro[4] = x3;
      goto L1989;
    }
  goto L2007;

  L1989: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_rhs_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L1990;
    }
  x2 = XEXP (x1, 0);
  goto L2007;

  L1990: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (pnum_clobbers != 0 && arm_rhs_operand (x2, SImode))
    {
      ro[2] = x2;
      *pnum_clobbers = 1;
      return 309;
    }
  x2 = XEXP (x1, 0);
  goto L2007;

  L2008: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[4] = x3;
      goto L2009;
    }
  goto L2017;

  L2009: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_add_operand (x3, SImode))
    {
      ro[5] = x3;
      goto L2010;
    }
  goto L2017;

  L2010: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == PLUS && 1)
    goto L2011;
  if (arm_rhs_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L2044;
    }
  x2 = XEXP (x1, 0);
  goto L2017;

  L2011: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L2012;
    }
  x2 = XEXP (x1, 0);
  goto L2017;

  L2012: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_add_operand (x3, SImode))
    {
      ro[3] = x3;
      goto L2013;
    }
  x2 = XEXP (x1, 0);
  goto L2017;

  L2013: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (pnum_clobbers != 0 && arm_rhs_operand (x2, SImode))
    {
      ro[1] = x2;
      *pnum_clobbers = 1;
      return 310;
    }
  x2 = XEXP (x1, 0);
  goto L2017;

  L2044: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == PLUS && 1)
    goto L2045;
  x2 = XEXP (x1, 0);
  goto L2017;

  L2045: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L2046;
    }
  x2 = XEXP (x1, 0);
  goto L2017;

  L2046: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (pnum_clobbers != 0 && arm_add_operand (x3, SImode))
    {
      ro[3] = x3;
      *pnum_clobbers = 1;
      return 312;
    }
  x2 = XEXP (x1, 0);
  goto L2017;

  L2018: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_CODE (x3) == REG && cc_register (x3, VOIDmode))
    {
      ro[5] = x3;
      goto L2019;
    }
  goto L2075;

  L2019: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L2020;
  goto L2075;

  L2020: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == PLUS && 1)
    goto L2021;
  if (arm_rhs_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L2054;
    }
  x2 = XEXP (x1, 0);
  goto L2075;

  L2021: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L2022;
    }
  x2 = XEXP (x1, 0);
  goto L2075;

  L2022: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_add_operand (x3, SImode))
    {
      ro[3] = x3;
      goto L2023;
    }
  x2 = XEXP (x1, 0);
  goto L2075;

  L2023: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (arm_rhs_operand (x2, SImode))
    {
      ro[1] = x2;
      return 311;
    }
  x2 = XEXP (x1, 0);
  goto L2075;

  L2054: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == PLUS && 1)
    goto L2055;
  x2 = XEXP (x1, 0);
  goto L2075;

  L2055: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L2056;
    }
  x2 = XEXP (x1, 0);
  goto L2075;

  L2056: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_add_operand (x3, SImode))
    {
      ro[3] = x3;
      return 313;
    }
  x2 = XEXP (x1, 0);
  goto L2075;

  L2076: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[5] = x3;
      goto L2077;
    }
  goto L2087;

  L2077: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_add_operand (x3, SImode))
    {
      ro[6] = x3;
      goto L2078;
    }
  goto L2087;

  L2078: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (shiftable_operator (x2, SImode))
    {
      ro[8] = x2;
      goto L2079;
    }
  x2 = XEXP (x1, 0);
  goto L2087;

  L2079: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L2080;
    }
  x2 = XEXP (x1, 0);
  goto L2087;

  L2080: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L2081;
    }
  x2 = XEXP (x1, 0);
  goto L2087;

  L2081: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (shiftable_operator (x2, SImode))
    {
      ro[7] = x2;
      goto L2082;
    }
  x2 = XEXP (x1, 0);
  goto L2087;

  L2082: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[3] = x3;
      goto L2083;
    }
  x2 = XEXP (x1, 0);
  goto L2087;

  L2083: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (pnum_clobbers != 0 && arm_rhs_operand (x3, SImode))
    {
      ro[4] = x3;
      *pnum_clobbers = 1;
      return 314;
    }
  x2 = XEXP (x1, 0);
  goto L2087;

  L2178: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[3] = x3;
      goto L2179;
    }
  L2088: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == REG && cc_register (x3, VOIDmode))
    {
      ro[8] = x3;
      goto L2089;
    }
  goto L2112;

  L2179: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_add_operand (x3, SImode))
    {
      ro[4] = x3;
      goto L2210;
    }
  x3 = XEXP (x2, 0);
  goto L2088;

  L2210: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == NOT && 1)
    goto L2211;
  if (arm_not_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L2181;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2088;

  L2211: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L2212;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2088;

  L2212: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (pnum_clobbers != 0 && arm_not_operand (x2, SImode))
    {
      ro[1] = x2;
      *pnum_clobbers = 1;
      return 322;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2088;

  L2181: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == NOT && 1)
    goto L2182;
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2088;

  L2182: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (pnum_clobbers != 0 && s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      *pnum_clobbers = 1;
      return 320;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2088;

  L2089: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L2090;
  goto L2112;

  L2090: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (shiftable_operator (x2, SImode))
    {
      ro[6] = x2;
      goto L2091;
    }
  x2 = XEXP (x1, 0);
  goto L2112;

  L2091: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L2092;
    }
  x2 = XEXP (x1, 0);
  goto L2112;

  L2092: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L2093;
    }
  x2 = XEXP (x1, 0);
  goto L2112;

  L2093: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (shiftable_operator (x2, SImode))
    {
      ro[7] = x2;
      goto L2094;
    }
  x2 = XEXP (x1, 0);
  goto L2112;

  L2094: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[3] = x3;
      goto L2095;
    }
  x2 = XEXP (x1, 0);
  goto L2112;

  L2095: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[4] = x3;
      return 315;
    }
  x2 = XEXP (x1, 0);
  goto L2112;

  L2113: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) != SImode)
    {
      goto L2122;
    }
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L2114;
    }
  L2146: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x3, SImode))
    {
      ro[4] = x3;
      goto L2147;
    }
  goto L2122;

  L2114: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_add_operand (x3, SImode))
    {
      ro[3] = x3;
      goto L2115;
    }
  x3 = XEXP (x2, 0);
  goto L2146;

  L2115: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (shiftable_operator (x2, SImode))
    {
      ro[7] = x2;
      goto L2116;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2146;

  L2116: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[4] = x3;
      goto L2117;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2146;

  L2117: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[5] = x3;
      goto L2118;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2146;

  L2118: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (pnum_clobbers != 0 && arm_rhs_operand (x2, SImode))
    {
      ro[1] = x2;
      *pnum_clobbers = 1;
      return 316;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2146;

  L2147: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_add_operand (x3, SImode))
    {
      ro[5] = x3;
      goto L2347;
    }
  goto L2122;

  L2347: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  switch (GET_MODE (x2))
    {
    default:
      break;
    case SImode:
      if (GET_CODE (x2) == NOT && 1)
	goto L2348;
      if (shift_operator (x2, SImode))
	{
	  ro[7] = x2;
	  goto L2242;
	}
      if (shiftable_operator (x2, SImode))
	{
	  ro[7] = x2;
	  goto L2384;
	}
    }
  if (arm_rhs_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L2149;
    }
  L2274: ATTRIBUTE_UNUSED_LABEL
  if (arm_not_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L2275;
    }
  x2 = XEXP (x1, 0);
  goto L2122;

  L2348: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L2349;
    }
  x2 = XEXP (x1, 0);
  goto L2122;

  L2349: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (shiftable_operator (x2, SImode))
    {
      ro[7] = x2;
      goto L2350;
    }
  x2 = XEXP (x1, 0);
  goto L2122;

  L2350: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L2351;
    }
  x2 = XEXP (x1, 0);
  goto L2122;

  L2351: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (pnum_clobbers != 0 && arm_rhs_operand (x3, SImode))
    {
      ro[3] = x3;
      *pnum_clobbers = 1;
      return 330;
    }
  x2 = XEXP (x1, 0);
  goto L2122;

  L2242: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L2243;
    }
  x2 = XEXP (x1, 0);
  goto L2122;

  L2243: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[3] = x3;
      goto L2244;
    }
  x2 = XEXP (x1, 0);
  goto L2122;

  L2244: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (pnum_clobbers != 0 && arm_not_operand (x2, SImode))
    {
      ro[1] = x2;
      *pnum_clobbers = 1;
      return 324;
    }
  x2 = XEXP (x1, 0);
  goto L2122;

  L2384: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L2385;
    }
  x2 = XEXP (x1, 0);
  goto L2122;

  L2385: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[3] = x3;
      goto L2386;
    }
  x2 = XEXP (x1, 0);
  goto L2122;

  L2386: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == NOT && 1)
    goto L2387;
  x2 = XEXP (x1, 0);
  goto L2122;

  L2387: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (pnum_clobbers != 0 && s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      *pnum_clobbers = 1;
      return 332;
    }
  x2 = XEXP (x1, 0);
  goto L2122;

  L2149: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (shiftable_operator (x2, SImode))
    {
      ro[7] = x2;
      goto L2150;
    }
  x2 = XEXP (x1, 1);
  goto L2274;

  L2150: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L2151;
    }
  x2 = XEXP (x1, 1);
  goto L2274;

  L2151: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (pnum_clobbers != 0 && arm_rhs_operand (x3, SImode))
    {
      ro[3] = x3;
      *pnum_clobbers = 1;
      return 318;
    }
  x2 = XEXP (x1, 1);
  goto L2274;

  L2275: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (shift_operator (x2, SImode))
    {
      ro[7] = x2;
      goto L2276;
    }
  x2 = XEXP (x1, 0);
  goto L2122;

  L2276: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L2277;
    }
  x2 = XEXP (x1, 0);
  goto L2122;

  L2277: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (pnum_clobbers != 0 && arm_rhs_operand (x3, SImode))
    {
      ro[3] = x3;
      *pnum_clobbers = 1;
      return 326;
    }
  x2 = XEXP (x1, 0);
  goto L2122;

  L2123: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_CODE (x3) != REG)
    {
    goto L2248;
    }
  if (cc_register (x3, VOIDmode))
    {
      ro[6] = x3;
      goto L2124;
    }
  L2187: ATTRIBUTE_UNUSED_LABEL
  if (cc_register (x3, VOIDmode))
    {
      ro[3] = x3;
      goto L2188;
    }
  goto L2248;

  L2124: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L2125;
  x3 = XEXP (x2, 0);
  goto L2187;

  L2125: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (shiftable_operator (x2, SImode))
    {
      ro[5] = x2;
      goto L2126;
    }
  if (arm_rhs_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L2159;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2187;

  L2126: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L2127;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2187;

  L2127: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[3] = x3;
      goto L2128;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2187;

  L2128: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (arm_rhs_operand (x2, SImode))
    {
      ro[1] = x2;
      return 317;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2187;

  L2159: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (shiftable_operator (x2, SImode))
    {
      ro[5] = x2;
      goto L2160;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2187;

  L2160: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L2161;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2187;

  L2161: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[3] = x3;
      return 319;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2187;

  L2188: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L2219;
  goto L2248;

  L2219: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == NOT && 1)
    goto L2220;
  if (arm_not_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L2190;
    }
  x2 = XEXP (x1, 0);
  goto L2248;

  L2220: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L2221;
    }
  x2 = XEXP (x1, 0);
  goto L2248;

  L2221: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (arm_not_operand (x2, SImode))
    {
      ro[1] = x2;
      return 323;
    }
  x2 = XEXP (x1, 0);
  goto L2248;

  L2190: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == NOT && 1)
    goto L2191;
  x2 = XEXP (x1, 0);
  goto L2248;

  L2191: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      return 321;
    }
  x2 = XEXP (x1, 0);
  goto L2248;

  L2249: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_CODE (x3) == REG && cc_register (x3, VOIDmode))
    {
      ro[6] = x3;
      goto L2250;
    }
  goto L2306;

  L2250: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L2251;
  goto L2306;

  L2251: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (shift_operator (x2, SImode))
    {
      ro[4] = x2;
      goto L2252;
    }
  if (arm_not_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L2285;
    }
  x2 = XEXP (x1, 0);
  goto L2306;

  L2252: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L2253;
    }
  x2 = XEXP (x1, 0);
  goto L2306;

  L2253: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[3] = x3;
      goto L2254;
    }
  x2 = XEXP (x1, 0);
  goto L2306;

  L2254: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (arm_not_operand (x2, SImode))
    {
      ro[1] = x2;
      return 325;
    }
  x2 = XEXP (x1, 0);
  goto L2306;

  L2285: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (shift_operator (x2, SImode))
    {
      ro[4] = x2;
      goto L2286;
    }
  x2 = XEXP (x1, 0);
  goto L2306;

  L2286: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L2287;
    }
  x2 = XEXP (x1, 0);
  goto L2306;

  L2287: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[3] = x3;
      return 327;
    }
  x2 = XEXP (x1, 0);
  goto L2306;

  L2307: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[5] = x3;
      goto L2308;
    }
  goto L2318;

  L2308: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_add_operand (x3, SImode))
    {
      ro[6] = x3;
      goto L2309;
    }
  goto L2318;

  L2309: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (shift_operator (x2, SImode))
    {
      ro[8] = x2;
      goto L2310;
    }
  x2 = XEXP (x1, 0);
  goto L2318;

  L2310: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L2311;
    }
  x2 = XEXP (x1, 0);
  goto L2318;

  L2311: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L2312;
    }
  x2 = XEXP (x1, 0);
  goto L2318;

  L2312: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (shift_operator (x2, SImode))
    {
      ro[9] = x2;
      goto L2313;
    }
  x2 = XEXP (x1, 0);
  goto L2318;

  L2313: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[3] = x3;
      goto L2314;
    }
  x2 = XEXP (x1, 0);
  goto L2318;

  L2314: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (pnum_clobbers != 0 && arm_rhs_operand (x3, SImode))
    {
      ro[4] = x3;
      *pnum_clobbers = 1;
      return 328;
    }
  x2 = XEXP (x1, 0);
  goto L2318;

  L2415: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[3] = x3;
      goto L2416;
    }
  L2319: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) != REG)
    {
    goto L2423;
    }
  if (cc_register (x3, VOIDmode))
    {
      ro[8] = x3;
      goto L2320;
    }
  L2356: ATTRIBUTE_UNUSED_LABEL
  if (cc_register (x3, VOIDmode))
    {
      ro[4] = x3;
      goto L2357;
    }
  goto L2423;

  L2416: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_add_operand (x3, SImode))
    {
      ro[4] = x3;
      goto L2417;
    }
  x3 = XEXP (x2, 0);
  goto L2319;

  L2417: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == NEG && 1)
    goto L2418;
  if (arm_not_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L2448;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2319;

  L2418: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L2419;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2319;

  L2419: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (pnum_clobbers != 0 && arm_not_operand (x2, SImode))
    {
      ro[1] = x2;
      *pnum_clobbers = 1;
      return 334;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2319;

  L2448: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == NEG && 1)
    goto L2449;
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2319;

  L2449: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (pnum_clobbers != 0 && s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      *pnum_clobbers = 1;
      return 336;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2319;

  L2320: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L2321;
  x3 = XEXP (x2, 0);
  goto L2356;

  L2321: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (shift_operator (x2, SImode))
    {
      ro[6] = x2;
      goto L2322;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2356;

  L2322: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L2323;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2356;

  L2323: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L2324;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2356;

  L2324: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (shift_operator (x2, SImode))
    {
      ro[7] = x2;
      goto L2325;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2356;

  L2325: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[3] = x3;
      goto L2326;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2356;

  L2326: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[4] = x3;
      return 329;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2356;

  L2357: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L2358;
  goto L2423;

  L2358: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) != SImode)
    {
      x2 = XEXP (x1, 0);
      goto L2423;
    }
  if (GET_CODE (x2) == NOT && 1)
    goto L2359;
  if (shiftable_operator (x2, SImode))
    {
      ro[6] = x2;
      goto L2395;
    }
  x2 = XEXP (x1, 0);
  goto L2423;

  L2359: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L2360;
    }
  x2 = XEXP (x1, 0);
  goto L2423;

  L2360: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (shiftable_operator (x2, SImode))
    {
      ro[6] = x2;
      goto L2361;
    }
  x2 = XEXP (x1, 0);
  goto L2423;

  L2361: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L2362;
    }
  x2 = XEXP (x1, 0);
  goto L2423;

  L2362: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[3] = x3;
      return 331;
    }
  x2 = XEXP (x1, 0);
  goto L2423;

  L2395: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L2396;
    }
  x2 = XEXP (x1, 0);
  goto L2423;

  L2396: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[3] = x3;
      goto L2397;
    }
  x2 = XEXP (x1, 0);
  goto L2423;

  L2397: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == NOT && 1)
    goto L2398;
  x2 = XEXP (x1, 0);
  goto L2423;

  L2398: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      return 333;
    }
  x2 = XEXP (x1, 0);
  goto L2423;

  L2424: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_CODE (x3) == REG && cc_register (x3, VOIDmode))
    {
      ro[3] = x3;
      goto L2425;
    }
  goto ret0;

  L2425: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L2426;
  goto ret0;

  L2426: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == NEG && 1)
    goto L2427;
  if (arm_not_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L2457;
    }
  goto ret0;

  L2427: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L2428;
    }
  goto ret0;

  L2428: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (arm_not_operand (x2, SImode))
    {
      ro[1] = x2;
      return 335;
    }
  goto ret0;

  L2457: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == NEG && 1)
    goto L2458;
  goto ret0;

  L2458: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      return 337;
    }
  goto ret0;
 ret0: return -1;
}

int
recog_12 (x0, insn, pnum_clobbers)
     register rtx x0;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  register rtx *ro = &recog_operand[0];
  register rtx x1 ATTRIBUTE_UNUSED, x2 ATTRIBUTE_UNUSED, x3 ATTRIBUTE_UNUSED, x4 ATTRIBUTE_UNUSED, x5 ATTRIBUTE_UNUSED, x6 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;

  x1 = XEXP (x0, 0);
  switch (GET_MODE (x1))
    {
    default:
      break;
    case DImode:
      if (s_register_operand (x1, DImode))
	{
	  ro[0] = x1;
	  goto L10;
	}
    L1268: ATTRIBUTE_UNUSED_LABEL
      if (di_operand (x1, DImode))
	{
	  ro[0] = x1;
	  goto L1269;
	}
      break;
    case SImode:
      if (s_register_operand (x1, SImode))
	{
	  ro[0] = x1;
	  goto L50;
	}
    L914: ATTRIBUTE_UNUSED_LABEL
      if (memory_operand (x1, SImode))
	{
	  ro[0] = x1;
	  goto L915;
	}
    L1271: ATTRIBUTE_UNUSED_LABEL
      if (general_operand (x1, SImode))
	{
	  ro[0] = x1;
	  goto L1272;
	}
    L1277: ATTRIBUTE_UNUSED_LABEL
      if (s_register_operand (x1, SImode))
	{
	  ro[0] = x1;
	  goto L1281;
	}
      break;
    case CC_NOOVmode:
      switch (GET_CODE (x1))
	{
	default:
	  break;
	case REG:
	  if (XINT (x1, 0) == 24 && 1)
	    goto L68;
	}
      break;
    case CC_Cmode:
      if (GET_CODE (x1) == REG && XINT (x1, 0) == 24 && 1)
	goto L101;
      break;
    case SFmode:
      if (s_register_operand (x1, SFmode))
	{
	  ro[0] = x1;
	  goto L165;
	}
    L1338: ATTRIBUTE_UNUSED_LABEL
      if (general_operand (x1, SFmode))
	{
	  ro[0] = x1;
	  goto L1339;
	}
      break;
    case DFmode:
      if (s_register_operand (x1, DFmode))
	{
	  ro[0] = x1;
	  goto L170;
	}
    L1346: ATTRIBUTE_UNUSED_LABEL
      if (general_operand (x1, DFmode))
	{
	  ro[0] = x1;
	  goto L1347;
	}
    L1349: ATTRIBUTE_UNUSED_LABEL
      if (soft_df_operand (x1, DFmode))
	{
	  ro[0] = x1;
	  goto L1350;
	}
      break;
    case XFmode:
      if (s_register_operand (x1, XFmode))
	{
	  ro[0] = x1;
	  goto L194;
	}
    L1352: ATTRIBUTE_UNUSED_LABEL
      if (general_operand (x1, XFmode))
	{
	  ro[0] = x1;
	  goto L1353;
	}
      break;
    case CC_Zmode:
      if (GET_CODE (x1) == REG && XINT (x1, 0) == 24 && 1)
	goto L1216;
      break;
    case HImode:
      if (s_register_operand (x1, HImode))
	{
	  ro[0] = x1;
	  goto L1241;
	}
    L1315: ATTRIBUTE_UNUSED_LABEL
      if (general_operand (x1, HImode))
	{
	  ro[0] = x1;
	  goto L1316;
	}
    L1318: ATTRIBUTE_UNUSED_LABEL
      if (s_register_operand (x1, HImode))
	{
	  ro[0] = x1;
	  goto L1319;
	}
      break;
    case QImode:
      if (general_operand (x1, QImode))
	{
	  ro[0] = x1;
	  goto L1336;
	}
      break;
    case CCmode:
      switch (GET_CODE (x1))
	{
	default:
	  break;
	case REG:
	  if (XINT (x1, 0) == 24 && 1)
	    goto L1386;
	}
      break;
    case CC_SWPmode:
      switch (GET_CODE (x1))
	{
	default:
	  break;
	case REG:
	  if (XINT (x1, 0) == 24 && 1)
	    goto L1398;
	}
      break;
    case CCFPmode:
      switch (GET_CODE (x1))
	{
	default:
	  break;
	case REG:
	  if (XINT (x1, 0) == 24 && 1)
	    goto L1413;
	}
      break;
    case CCFPEmode:
      switch (GET_CODE (x1))
	{
	default:
	  break;
	case REG:
	  if (XINT (x1, 0) == 24 && 1)
	    goto L1440;
	}
    }
  L1466: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x1))
    {
    default:
      break;
    case REG:
      if (cc_register (x1, VOIDmode))
	{
	  ro[0] = x1;
	  goto L1467;
	}
      break;
    case PC:
      goto L1470;
    }
  L1487: ATTRIBUTE_UNUSED_LABEL
  switch (GET_MODE (x1))
    {
    default:
      break;
    case SImode:
      if (s_register_operand (x1, SImode))
	{
	  ro[0] = x1;
	  goto L1493;
	}
      break;
    case SFmode:
      if (s_register_operand (x1, SFmode))
	{
	  ro[0] = x1;
	  goto L1513;
	}
      break;
    case DFmode:
      if (s_register_operand (x1, DFmode))
	{
	  ro[0] = x1;
	  goto L1529;
	}
    }
  L1571: ATTRIBUTE_UNUSED_LABEL
  ro[0] = x1;
  goto L1572;
  L1612: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x1, VOIDmode))
    {
      ro[0] = x1;
      goto L1613;
    }
  if (GET_CODE (x1) == PC && 1)
    goto L1657;
  L1663: ATTRIBUTE_UNUSED_LABEL
  switch (GET_MODE (x1))
    {
    default:
      break;
    case SImode:
      if (s_register_operand (x1, SImode))
	{
	  ro[0] = x1;
	  goto L1709;
	}
    }
  L1753: ATTRIBUTE_UNUSED_LABEL
  switch (GET_MODE (x1))
    {
    default:
      break;
    case SImode:
      ro[0] = x1;
      goto L1754;
    L1863: ATTRIBUTE_UNUSED_LABEL
      if (s_register_operand (x1, SImode))
	{
	  ro[0] = x1;
	  goto L1864;
	}
      break;
    case CC_NOOVmode:
      switch (GET_CODE (x1))
	{
	default:
	  break;
	case REG:
	  if (XINT (x1, 0) == 24 && 1)
	    goto L1700;
	}
    }
  L1934: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x1) == REG && dominant_cc_register (x1, VOIDmode))
    {
      ro[6] = x1;
      goto L1935;
    }
  goto ret0;
 L10: ATTRIBUTE_UNUSED_LABEL
  tem = recog_8 (x0, insn, pnum_clobbers);
  if (tem >= 0) return tem;
  x1 = XEXP (x0, 0);
  goto L1268;

  L1269: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (di_operand (x1, DImode))
    {
      ro[1] = x1;
      return 174;
    }
  x1 = XEXP (x0, 0);
  goto L1466;
 L50: ATTRIBUTE_UNUSED_LABEL
  tem = recog_9 (x0, insn, pnum_clobbers);
  if (tem >= 0) return tem;
  x1 = XEXP (x0, 0);
  goto L914;

  L915: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (minmax_operator (x1, SImode))
    {
      ro[3] = x1;
      goto L916;
    }
  x1 = XEXP (x0, 0);
  goto L1271;

  L916: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L917;
    }
  x1 = XEXP (x0, 0);
  goto L1271;

  L917: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (pnum_clobbers != 0 && s_register_operand (x2, SImode))
    {
      ro[2] = x2;
      *pnum_clobbers = 1;
      return 107;
    }
  x1 = XEXP (x0, 0);
  goto L1271;

  L1272: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (general_operand (x1, SImode))
    {
      ro[1] = x1;
      if (register_operand (operands[0], SImode)
   || register_operand (operands[1], SImode))
	return 176;
      }
  x1 = XEXP (x0, 0);
  goto L1277;

  L1281: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  switch (GET_MODE (x1))
    {
    default:
      break;
    case SImode:
      switch (GET_CODE (x1))
	{
	default:
	  break;
	case UNSPEC:
	  if (XINT (x1, 1) == 3 && XVECLEN (x1, 0) == 1 && 1)
	    goto L1282;
	  if (XINT (x1, 1) == 3 && XVECLEN (x1, 0) == 2 && 1)
	    goto L1286;
	  break;
	case ROTATE:
	  goto L1312;
	}
    }
  if (address_operand (x1, DImode))
    {
      ro[1] = x1;
      if (reload_completed
   && (GET_CODE (operands[1]) == LABEL_REF
       || (GET_CODE (operands[1]) == CONST
	   && GET_CODE (XEXP (operands[1], 0)) == PLUS
	   && GET_CODE (XEXP (XEXP (operands[1], 0), 0)) == LABEL_REF
	   && GET_CODE (XEXP (XEXP (operands[1], 0), 1)) == CONST_INT)))
	return 179;
      }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1282: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 0);
  ro[1] = x2;
  if (flag_pic)
    return 180;
  x1 = XEXP (x0, 0);
  goto L1466;

  L1286: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 0);
  ro[1] = x2;
  goto L1287;

  L1287: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (s_register_operand (x2, VOIDmode))
    {
      ro[2] = x2;
      if (flag_pic && operands[2] == pic_offset_table_rtx)
	return 182;
      }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1312: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) != SImode)
    {
      x1 = XEXP (x0, 0);
      goto L1466;
    }
  switch (GET_CODE (x2))
    {
    default:
      break;
    case MEM:
      if (offsettable_memory_operand (x2, SImode))
	{
	  ro[1] = x2;
	  goto L1313;
	}
      break;
    case SUBREG:
      if (XINT (x2, 1) == 0 && 1)
	goto L1329;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1313: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 16 && 1)
    if (! TARGET_SHORT_BY_BYTES)
      return 190;
  x1 = XEXP (x0, 0);
  goto L1466;

  L1329: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (memory_operand (x3, HImode))
    {
      ro[1] = x3;
      goto L1330;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1330: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 16 && 1)
    if (BYTES_BIG_ENDIAN
   && ! TARGET_SHORT_BY_BYTES)
      return 196;
  x1 = XEXP (x0, 0);
  goto L1466;

  L68: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == CC_NOOVmode && GET_CODE (x1) == COMPARE && 1)
    goto L69;
  x1 = XEXP (x0, 0);
  goto L1466;

  L69: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) != SImode)
    {
      x1 = XEXP (x0, 0);
      goto L1466;
    }
  switch (GET_CODE (x2))
    {
    default:
      break;
    case PLUS:
      goto L426;
    case MULT:
      goto L383;
    case AND:
      goto L730;
    case ZERO_EXTRACT:
      goto L654;
    case IOR:
      goto L788;
    case XOR:
      goto L830;
    case NOT:
      goto L1006;
    }
  L966: ATTRIBUTE_UNUSED_LABEL
  if (shift_operator (x2, SImode))
    {
      ro[3] = x2;
      goto L967;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L426: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) != SImode)
    {
      x1 = XEXP (x0, 0);
      goto L1466;
    }
  if (GET_CODE (x3) == MULT && 1)
    goto L427;
  if (s_register_operand (x3, SImode))
    {
      ro[0] = x3;
      goto L71;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L427: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L428;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L428: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L429;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L429: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      ro[3] = x3;
      goto L430;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L430: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && pnum_clobbers != 0 && 1)
    {
      *pnum_clobbers = 1;
      return 46;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L71: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_add_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L72;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L72: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && 1)
    return 7;
  x1 = XEXP (x0, 0);
  goto L1466;

  L383: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L384;
    }
  goto L966;

  L384: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L385;
    }
  goto L966;

  L385: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && pnum_clobbers != 0 && 1)
    {
      *pnum_clobbers = 1;
      return 43;
    }
  x2 = XEXP (x1, 0);
  goto L966;

  L730: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) != SImode)
    {
      x1 = XEXP (x0, 0);
      goto L1466;
    }
  if (GET_CODE (x3) == NOT && 1)
    goto L731;
  if (s_register_operand (x3, SImode))
    {
      ro[0] = x3;
      goto L648;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L731: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L732;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L732: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L733;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L733: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && pnum_clobbers != 0 && 1)
    {
      *pnum_clobbers = 1;
      return 86;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L648: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_not_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L649;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L649: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && pnum_clobbers != 0 && 1)
    {
      *pnum_clobbers = 1;
      return 76;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L654: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[0] = x3;
      goto L655;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L655: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && 1)
    {
      ro[1] = x3;
      goto L656;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L656: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (GET_CODE (x3) == CONST_INT && 1)
    {
      ro[2] = x3;
      goto L657;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L657: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && 1)
    if (INTVAL (operands[2]) >= 0 && INTVAL (operands[2]) < 32
   && INTVAL (operands[1]) > 0 
   && INTVAL (operands[1]) + (INTVAL (operands[2]) & 1) <= 8
   && INTVAL (operands[1]) + INTVAL (operands[2]) <= 32)
      return 77;
  x1 = XEXP (x0, 0);
  goto L1466;

  L788: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L789;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L789: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L790;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L790: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && pnum_clobbers != 0 && 1)
    {
      *pnum_clobbers = 1;
      return 94;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L830: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[0] = x3;
      goto L831;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L831: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L832;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L832: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && 1)
    return 100;
  x1 = XEXP (x0, 0);
  goto L1466;

  L1006: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) != SImode)
    {
      x1 = XEXP (x0, 0);
      goto L1466;
    }
  if (shift_operator (x3, SImode))
    {
      ro[3] = x3;
      goto L1007;
    }
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L1133;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1007: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L1008;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1008: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L1009;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1009: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && pnum_clobbers != 0 && 1)
    {
      *pnum_clobbers = 1;
      return 119;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1133: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && pnum_clobbers != 0 && 1)
    {
      *pnum_clobbers = 1;
      return 139;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L967: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L968;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L968: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L969;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L969: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && pnum_clobbers != 0 && 1)
    {
      *pnum_clobbers = 1;
      return 116;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L101: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == CC_Cmode && GET_CODE (x1) == COMPARE && 1)
    goto L102;
  x1 = XEXP (x0, 0);
  goto L1466;

  L102: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == PLUS && 1)
    goto L103;
  x1 = XEXP (x0, 0);
  goto L1466;

  L103: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[0] = x3;
      goto L104;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L104: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_add_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L105;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L105: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (rtx_equal_p (x2, ro[0]) && 1)
    return 10;
  L112: ATTRIBUTE_UNUSED_LABEL
  if (rtx_equal_p (x2, ro[1]) && 1)
    return 11;
  x1 = XEXP (x0, 0);
  goto L1466;

  L165: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) != SFmode)
    {
      x1 = XEXP (x0, 0);
      goto L1338;
    }
  switch (GET_CODE (x1))
    {
    default:
      break;
    case PLUS:
      goto L166;
    case MINUS:
      goto L320;
    case MULT:
      goto L494;
    case DIV:
      goto L528;
    case MOD:
      goto L562;
    case NEG:
      goto L1028;
    case ABS:
      goto L1069;
    case SQRT:
      goto L1086;
    case FLOAT:
      goto L1137;
    case FLOAT_TRUNCATE:
      goto L1161;
    }
  x1 = XEXP (x0, 0);
  goto L1338;

  L166: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SFmode))
    {
      ro[1] = x2;
      goto L167;
    }
  x1 = XEXP (x0, 0);
  goto L1338;

  L167: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_add_operand (x2, SFmode))
    {
      ro[2] = x2;
      if (TARGET_HARD_FLOAT)
	return 18;
      }
  x1 = XEXP (x0, 0);
  goto L1338;

  L320: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (fpu_rhs_operand (x2, SFmode))
    {
      ro[1] = x2;
      goto L321;
    }
  x1 = XEXP (x0, 0);
  goto L1338;

  L321: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_rhs_operand (x2, SFmode))
    {
      ro[2] = x2;
      if (TARGET_HARD_FLOAT)
	return 35;
      }
  x1 = XEXP (x0, 0);
  goto L1338;

  L494: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SFmode))
    {
      ro[1] = x2;
      goto L495;
    }
  x1 = XEXP (x0, 0);
  goto L1338;

  L495: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_rhs_operand (x2, SFmode))
    {
      ro[2] = x2;
      if (TARGET_HARD_FLOAT)
	return 51;
      }
  x1 = XEXP (x0, 0);
  goto L1338;

  L528: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (fpu_rhs_operand (x2, SFmode))
    {
      ro[1] = x2;
      goto L529;
    }
  x1 = XEXP (x0, 0);
  goto L1338;

  L529: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_rhs_operand (x2, SFmode))
    {
      ro[2] = x2;
      if (TARGET_HARD_FLOAT)
	return 57;
      }
  x1 = XEXP (x0, 0);
  goto L1338;

  L562: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SFmode))
    {
      ro[1] = x2;
      goto L563;
    }
  x1 = XEXP (x0, 0);
  goto L1338;

  L563: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_rhs_operand (x2, SFmode))
    {
      ro[2] = x2;
      if (TARGET_HARD_FLOAT)
	return 63;
      }
  x1 = XEXP (x0, 0);
  goto L1338;

  L1028: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SFmode))
    {
      ro[1] = x2;
      if (TARGET_HARD_FLOAT)
	return 122;
      }
  x1 = XEXP (x0, 0);
  goto L1338;

  L1069: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SFmode))
    {
      ro[1] = x2;
      if (TARGET_HARD_FLOAT)
	return 128;
      }
  x1 = XEXP (x0, 0);
  goto L1338;

  L1086: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SFmode))
    {
      ro[1] = x2;
      if (TARGET_HARD_FLOAT)
	return 132;
      }
  x1 = XEXP (x0, 0);
  goto L1338;

  L1137: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      if (TARGET_HARD_FLOAT)
	return 140;
      }
  x1 = XEXP (x0, 0);
  goto L1338;

  L1161: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  switch (GET_MODE (x2))
    {
    default:
      break;
    case DFmode:
      if (s_register_operand (x2, DFmode))
	{
	  ro[1] = x2;
	  if (TARGET_HARD_FLOAT)
	    return 146;
	  }
      break;
    case XFmode:
      if (s_register_operand (x2, XFmode))
	{
	  ro[1] = x2;
	  if (ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)
	    return 147;
	  }
    }
  x1 = XEXP (x0, 0);
  goto L1338;

  L1339: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (general_operand (x1, SFmode))
    goto L1343;
  x1 = XEXP (x0, 0);
  goto L1466;

  L1343: ATTRIBUTE_UNUSED_LABEL
  ro[1] = x1;
  if (TARGET_HARD_FLOAT
   && (GET_CODE (operands[0]) != MEM || register_operand (operands[1], SFmode)))
    return 203;
  L1344: ATTRIBUTE_UNUSED_LABEL
  ro[1] = x1;
  if (TARGET_SOFT_FLOAT
   && (GET_CODE (operands[0]) != MEM || register_operand (operands[1], SFmode)))
    return 204;
  x1 = XEXP (x0, 0);
  goto L1466;
 L170: ATTRIBUTE_UNUSED_LABEL
  tem = recog_10 (x0, insn, pnum_clobbers);
  if (tem >= 0) return tem;
  x1 = XEXP (x0, 0);
  goto L1346;

  L1347: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (general_operand (x1, DFmode))
    {
      ro[1] = x1;
      if (TARGET_HARD_FLOAT
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], DFmode)))
	return 207;
      }
  x1 = XEXP (x0, 0);
  goto L1349;

  L1350: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (soft_df_operand (x1, DFmode))
    {
      ro[1] = x1;
      if (TARGET_SOFT_FLOAT)
	return 208;
      }
  x1 = XEXP (x0, 0);
  goto L1466;

  L194: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) != XFmode)
    {
      x1 = XEXP (x0, 0);
      goto L1352;
    }
  switch (GET_CODE (x1))
    {
    default:
      break;
    case PLUS:
      goto L195;
    case MINUS:
      goto L349;
    case MULT:
      goto L523;
    case DIV:
      goto L557;
    case MOD:
      goto L591;
    case NEG:
      goto L1041;
    case ABS:
      goto L1082;
    case SQRT:
      goto L1099;
    case FLOAT:
      goto L1145;
    case FLOAT_EXTEND:
      goto L1262;
    }
  x1 = XEXP (x0, 0);
  goto L1352;

  L195: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, XFmode))
    {
      ro[1] = x2;
      goto L196;
    }
  x1 = XEXP (x0, 0);
  goto L1352;

  L196: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_add_operand (x2, XFmode))
    {
      ro[2] = x2;
      if (ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)
	return 23;
      }
  x1 = XEXP (x0, 0);
  goto L1352;

  L349: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (fpu_rhs_operand (x2, XFmode))
    {
      ro[1] = x2;
      goto L350;
    }
  x1 = XEXP (x0, 0);
  goto L1352;

  L350: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_rhs_operand (x2, XFmode))
    {
      ro[2] = x2;
      if (ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)
	return 40;
      }
  x1 = XEXP (x0, 0);
  goto L1352;

  L523: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, XFmode))
    {
      ro[1] = x2;
      goto L524;
    }
  x1 = XEXP (x0, 0);
  goto L1352;

  L524: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_rhs_operand (x2, XFmode))
    {
      ro[2] = x2;
      if (ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)
	return 56;
      }
  x1 = XEXP (x0, 0);
  goto L1352;

  L557: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (fpu_rhs_operand (x2, XFmode))
    {
      ro[1] = x2;
      goto L558;
    }
  x1 = XEXP (x0, 0);
  goto L1352;

  L558: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_rhs_operand (x2, XFmode))
    {
      ro[2] = x2;
      if (ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)
	return 62;
      }
  x1 = XEXP (x0, 0);
  goto L1352;

  L591: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, XFmode))
    {
      ro[1] = x2;
      goto L592;
    }
  x1 = XEXP (x0, 0);
  goto L1352;

  L592: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_rhs_operand (x2, XFmode))
    {
      ro[2] = x2;
      if (ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)
	return 68;
      }
  x1 = XEXP (x0, 0);
  goto L1352;

  L1041: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, XFmode))
    {
      ro[1] = x2;
      if (ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)
	return 125;
      }
  x1 = XEXP (x0, 0);
  goto L1352;

  L1082: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, XFmode))
    {
      ro[1] = x2;
      if (ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)
	return 131;
      }
  x1 = XEXP (x0, 0);
  goto L1352;

  L1099: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, XFmode))
    {
      ro[1] = x2;
      if (ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)
	return 135;
      }
  x1 = XEXP (x0, 0);
  goto L1352;

  L1145: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      if (ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)
	return 142;
      }
  x1 = XEXP (x0, 0);
  goto L1352;

  L1262: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  switch (GET_MODE (x2))
    {
    default:
      break;
    case SFmode:
      if (s_register_operand (x2, SFmode))
	{
	  ro[1] = x2;
	  if (ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)
	    return 172;
	  }
      break;
    case DFmode:
      if (s_register_operand (x2, DFmode))
	{
	  ro[1] = x2;
	  if (ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)
	    return 173;
	  }
    }
  x1 = XEXP (x0, 0);
  goto L1352;

  L1353: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (general_operand (x1, XFmode))
    {
      ro[1] = x1;
      if (TARGET_HARD_FLOAT && (ENABLE_XF_PATTERNS || reload_completed))
	return 210;
      }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1216: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == CC_Zmode && GET_CODE (x1) == COMPARE && 1)
    goto L1217;
  x1 = XEXP (x0, 0);
  goto L1466;

  L1217: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, QImode))
    {
      ro[0] = x2;
      goto L1218;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1218: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && 1)
    return 159;
  x1 = XEXP (x0, 0);
  goto L1466;

  L1241: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == HImode && GET_CODE (x1) == SIGN_EXTEND && 1)
    goto L1242;
  x1 = XEXP (x0, 0);
  goto L1315;

  L1242: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (memory_operand (x2, QImode))
    {
      ro[1] = x2;
      if (arm_arch4)
	return 166;
      }
  x1 = XEXP (x0, 0);
  goto L1315;

  L1316: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (general_operand (x1, HImode))
    {
      ro[1] = x1;
      if (arm_arch4 && ! ARM_ARCH_RISCPC
   && (GET_CODE (operands[1]) != CONST_INT
       || const_ok_for_arm (INTVAL (operands[1]))
       || const_ok_for_arm (~INTVAL (operands[1]))))
	return 193;
      }
  x1 = XEXP (x0, 0);
  goto L1318;

  L1319: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (general_operand (x1, HImode))
    goto L1323;
  L1333: ATTRIBUTE_UNUSED_LABEL
  if (arm_rhs_operand (x1, HImode))
    {
      ro[1] = x1;
      if (TARGET_SHORT_BY_BYTES)
	return 197;
      }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1323: ATTRIBUTE_UNUSED_LABEL
  ro[1] = x1;
  if ((! arm_arch4 || ARM_ARCH_RISCPC)
   && ! BYTES_BIG_ENDIAN
   && ! TARGET_SHORT_BY_BYTES
   && (GET_CODE (operands[1]) != CONST_INT
       || const_ok_for_arm (INTVAL (operands[1]))
       || const_ok_for_arm (~INTVAL (operands[1]))))
    return 194;
  L1324: ATTRIBUTE_UNUSED_LABEL
  ro[1] = x1;
  if ((! arm_arch4 || ARM_ARCH_RISCPC)
   && BYTES_BIG_ENDIAN
   && ! TARGET_SHORT_BY_BYTES
   && (GET_CODE (operands[1]) != CONST_INT
       || const_ok_for_arm (INTVAL (operands[1]))
       || const_ok_for_arm (~INTVAL (operands[1]))))
    return 195;
  goto L1333;

  L1336: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (general_operand (x1, QImode))
    {
      ro[1] = x1;
      if (register_operand (operands[0], QImode)
   || register_operand (operands[1], QImode))
	return 201;
      }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1386: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == CCmode && GET_CODE (x1) == COMPARE && 1)
    goto L1387;
  x1 = XEXP (x0, 0);
  goto L1466;

  L1387: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L1407;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1407: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  switch (GET_MODE (x2))
    {
    default:
      break;
    case SImode:
      if (GET_CODE (x2) == NEG && 1)
	goto L1408;
      if (shift_operator (x2, SImode))
	{
	  ro[3] = x2;
	  goto L1394;
	}
    }
  if (arm_add_operand (x2, SImode))
    {
      ro[1] = x2;
      return 222;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1408: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (shift_operator (x3, SImode))
    {
      ro[3] = x3;
      goto L1409;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1409: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L1410;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1410: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      ro[2] = x4;
      return 225;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1394: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L1395;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1395: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[2] = x3;
      return 223;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1398: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == CC_SWPmode && GET_CODE (x1) == COMPARE && 1)
    goto L1399;
  x1 = XEXP (x0, 0);
  goto L1466;

  L1399: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (shift_operator (x2, SImode))
    {
      ro[3] = x2;
      goto L1400;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1400: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L1401;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1401: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (reg_or_int_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L1402;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1402: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      return 224;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1413: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == CCFPmode && GET_CODE (x1) == COMPARE && 1)
    goto L1414;
  x1 = XEXP (x0, 0);
  goto L1466;

  L1414: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  switch (GET_MODE (x2))
    {
    default:
      break;
    case SFmode:
      if (s_register_operand (x2, SFmode))
	{
	  ro[0] = x2;
	  goto L1415;
	}
      break;
    case DFmode:
      if (GET_CODE (x2) == FLOAT_EXTEND && 1)
	goto L1425;
      if (s_register_operand (x2, DFmode))
	{
	  ro[0] = x2;
	  goto L1431;
	}
      break;
    case XFmode:
      if (s_register_operand (x2, XFmode))
	{
	  ro[0] = x2;
	  goto L1437;
	}
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1415: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_add_operand (x2, SFmode))
    {
      ro[1] = x2;
      if (TARGET_HARD_FLOAT)
	return 226;
      }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1425: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      ro[0] = x3;
      goto L1426;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1426: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_add_operand (x2, DFmode))
    {
      ro[1] = x2;
      if (TARGET_HARD_FLOAT)
	return 228;
      }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1431: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) != DFmode)
    {
      x1 = XEXP (x0, 0);
      goto L1466;
    }
  if (GET_CODE (x2) == FLOAT_EXTEND && 1)
    goto L1432;
  if (fpu_add_operand (x2, DFmode))
    {
      ro[1] = x2;
      if (TARGET_HARD_FLOAT)
	return 227;
      }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1432: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      ro[1] = x3;
      if (TARGET_HARD_FLOAT)
	return 229;
      }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1437: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_add_operand (x2, XFmode))
    {
      ro[1] = x2;
      if (ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)
	return 230;
      }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1440: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == CCFPEmode && GET_CODE (x1) == COMPARE && 1)
    goto L1441;
  x1 = XEXP (x0, 0);
  goto L1466;

  L1441: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  switch (GET_MODE (x2))
    {
    default:
      break;
    case SFmode:
      if (s_register_operand (x2, SFmode))
	{
	  ro[0] = x2;
	  goto L1442;
	}
      break;
    case DFmode:
      if (GET_CODE (x2) == FLOAT_EXTEND && 1)
	goto L1452;
      if (s_register_operand (x2, DFmode))
	{
	  ro[0] = x2;
	  goto L1458;
	}
      break;
    case XFmode:
      if (s_register_operand (x2, XFmode))
	{
	  ro[0] = x2;
	  goto L1464;
	}
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1442: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_add_operand (x2, SFmode))
    {
      ro[1] = x2;
      if (TARGET_HARD_FLOAT)
	return 231;
      }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1452: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      ro[0] = x3;
      goto L1453;
    }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1453: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_add_operand (x2, DFmode))
    {
      ro[1] = x2;
      if (TARGET_HARD_FLOAT)
	return 233;
      }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1458: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) != DFmode)
    {
      x1 = XEXP (x0, 0);
      goto L1466;
    }
  if (GET_CODE (x2) == FLOAT_EXTEND && 1)
    goto L1459;
  if (fpu_add_operand (x2, DFmode))
    {
      ro[1] = x2;
      if (TARGET_HARD_FLOAT)
	return 232;
      }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1459: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      ro[1] = x3;
      if (TARGET_HARD_FLOAT)
	return 234;
      }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1464: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_add_operand (x2, XFmode))
    {
      ro[1] = x2;
      if (ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)
	return 235;
      }
  x1 = XEXP (x0, 0);
  goto L1466;

  L1467: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (rtx_equal_p (x1, ro[0]) && 1)
    return 236;
  x1 = XEXP (x0, 0);
  goto L1487;

  L1470: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  switch (GET_CODE (x1))
    {
    default:
      break;
    case IF_THEN_ELSE:
      goto L1471;
    case LABEL_REF:
      goto L1538;
    }
  x1 = XEXP (x0, 0);
  goto L1571;

  L1471: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (comparison_operator (x2, VOIDmode))
    {
      ro[1] = x2;
      goto L1472;
    }
  x1 = XEXP (x0, 0);
  goto L1571;

  L1472: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_CODE (x3) == REG && cc_register (x3, VOIDmode))
    {
      ro[2] = x3;
      goto L1473;
    }
  x1 = XEXP (x0, 0);
  goto L1571;

  L1473: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L1474;
  x1 = XEXP (x0, 0);
  goto L1571;

  L1474: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  switch (GET_CODE (x2))
    {
    default:
      break;
    case LABEL_REF:
      goto L1475;
    case PC:
      goto L1484;
    }
  x1 = XEXP (x0, 0);
  goto L1571;

  L1475: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  ro[0] = x3;
  goto L1476;

  L1476: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == PC && 1)
    return 247;
  x1 = XEXP (x0, 0);
  goto L1571;

  L1484: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L1485;
  x1 = XEXP (x0, 0);
  goto L1571;

  L1485: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  ro[0] = x3;
  return 248;

  L1538: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  ro[0] = x2;
  return 269;

  L1493: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) != SImode)
    {
      x1 = XEXP (x0, 0);
      goto L1571;
    }
  switch (GET_CODE (x1))
    {
    default:
      break;
    case NEG:
      goto L1494;
    case NOT:
      goto L1500;
    case IF_THEN_ELSE:
      goto L1506;
    case EQ:
    case NE:
    case LE:
    case LT:
    case GE:
    case GT:
    case LEU:
    case LTU:
    case GEU:
    case GTU:
      if (comparison_operator (x1, SImode))
	{
	  ro[1] = x1;
	  goto L1489;
	}
    }
  x1 = XEXP (x0, 0);
  goto L1571;

  L1494: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (comparison_operator (x2, SImode))
    {
      ro[1] = x2;
      goto L1495;
    }
  x1 = XEXP (x0, 0);
  goto L1571;

  L1495: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_CODE (x3) == REG && cc_register (x3, VOIDmode))
    {
      ro[2] = x3;
      goto L1496;
    }
  x1 = XEXP (x0, 0);
  goto L1571;

  L1496: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    return 260;
  x1 = XEXP (x0, 0);
  goto L1571;

  L1500: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (comparison_operator (x2, SImode))
    {
      ro[1] = x2;
      goto L1501;
    }
  x1 = XEXP (x0, 0);
  goto L1571;

  L1501: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_CODE (x3) == REG && cc_register (x3, VOIDmode))
    {
      ro[2] = x3;
      goto L1502;
    }
  x1 = XEXP (x0, 0);
  goto L1571;

  L1502: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    return 261;
  x1 = XEXP (x0, 0);
  goto L1571;

  L1506: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (comparison_operator (x2, VOIDmode))
    {
      ro[3] = x2;
      goto L1507;
    }
  x1 = XEXP (x0, 0);
  goto L1571;

  L1507: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_CODE (x3) == REG && cc_register (x3, VOIDmode))
    {
      ro[4] = x3;
      goto L1508;
    }
  x1 = XEXP (x0, 0);
  goto L1571;

  L1508: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L1509;
  x1 = XEXP (x0, 0);
  goto L1571;

  L1509: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_not_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L1510;
    }
  x1 = XEXP (x0, 0);
  goto L1571;

  L1510: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (arm_not_operand (x2, SImode))
    {
      ro[2] = x2;
      return 265;
    }
  x1 = XEXP (x0, 0);
  goto L1571;

  L1489: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) == REG && cc_register (x2, VOIDmode))
    {
      ro[2] = x2;
      goto L1490;
    }
  x1 = XEXP (x0, 0);
  goto L1571;

  L1490: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && 1)
    return 259;
  x1 = XEXP (x0, 0);
  goto L1571;

  L1513: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == SFmode && GET_CODE (x1) == IF_THEN_ELSE && 1)
    goto L1514;
  x1 = XEXP (x0, 0);
  goto L1571;

  L1514: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (comparison_operator (x2, VOIDmode))
    {
      ro[3] = x2;
      goto L1515;
    }
  x1 = XEXP (x0, 0);
  goto L1571;

  L1515: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_CODE (x3) == REG && cc_register (x3, VOIDmode))
    {
      ro[4] = x3;
      goto L1516;
    }
  x1 = XEXP (x0, 0);
  goto L1571;

  L1516: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L1517;
  x1 = XEXP (x0, 0);
  goto L1571;

  L1517: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) != SFmode)
    {
      x1 = XEXP (x0, 0);
      goto L1571;
    }
  if (fpu_add_operand (x2, SFmode))
    {
      ro[1] = x2;
      goto L1518;
    }
  L1525: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, SFmode))
    {
      ro[1] = x2;
      goto L1526;
    }
  x1 = XEXP (x0, 0);
  goto L1571;

  L1518: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (fpu_add_operand (x2, SFmode))
    {
      ro[2] = x2;
      if (TARGET_HARD_FLOAT)
	return 266;
      }
  x2 = XEXP (x1, 1);
  goto L1525;

  L1526: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (s_register_operand (x2, SFmode))
    {
      ro[2] = x2;
      if (TARGET_SOFT_FLOAT)
	return 267;
      }
  x1 = XEXP (x0, 0);
  goto L1571;

  L1529: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == DFmode && GET_CODE (x1) == IF_THEN_ELSE && 1)
    goto L1530;
  x1 = XEXP (x0, 0);
  goto L1571;

  L1530: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (comparison_operator (x2, VOIDmode))
    {
      ro[3] = x2;
      goto L1531;
    }
  x1 = XEXP (x0, 0);
  goto L1571;

  L1531: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_CODE (x3) == REG && cc_register (x3, VOIDmode))
    {
      ro[4] = x3;
      goto L1532;
    }
  x1 = XEXP (x0, 0);
  goto L1571;

  L1532: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L1533;
  x1 = XEXP (x0, 0);
  goto L1571;

  L1533: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_add_operand (x2, DFmode))
    {
      ro[1] = x2;
      goto L1534;
    }
  x1 = XEXP (x0, 0);
  goto L1571;

  L1534: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (fpu_add_operand (x2, DFmode))
    {
      ro[2] = x2;
      if (TARGET_HARD_FLOAT)
	return 268;
      }
  x1 = XEXP (x0, 0);
  goto L1571;

  L1572: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_CODE (x1) == CALL && 1)
    goto L1573;
  x1 = XEXP (x0, 0);
  goto L1612;

  L1573: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == MEM && 1)
    goto L1574;
  x1 = XEXP (x0, 0);
  goto L1612;

  L1574: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L1575;
    }
  L1589: ATTRIBUTE_UNUSED_LABEL
  if (memory_operand (x3, VOIDmode))
    {
      ro[1] = x3;
      goto L1590;
    }
  x1 = XEXP (x0, 0);
  goto L1612;

  L1575: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (pnum_clobbers != 0 && general_operand (x2, VOIDmode))
    {
      ro[2] = x2;
      *pnum_clobbers = 1;
      return 274;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L1589;

  L1590: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (pnum_clobbers != 0 && general_operand (x2, VOIDmode))
    {
      ro[2] = x2;
      if (! CONSTANT_ADDRESS_P (XEXP (operands[1], 0)))
	{
	  *pnum_clobbers = 1;
	  return 275;
	}
      }
  x1 = XEXP (x0, 0);
  goto L1612;

  L1613: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_CODE (x1) == CALL && 1)
    goto L1614;
  x1 = XEXP (x0, 0);
  goto L1663;

  L1614: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == MEM && 1)
    goto L1615;
  x1 = XEXP (x0, 0);
  goto L1663;

  L1615: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  ro[1] = x3;
  goto L1616;

  L1616: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (pnum_clobbers != 0 && general_operand (x2, SImode))
    {
      ro[2] = x2;
      if (GET_CODE(operands[1]) == SYMBOL_REF)
	{
	  *pnum_clobbers = 1;
	  return 277;
	}
      }
  x1 = XEXP (x0, 0);
  goto L1663;

  L1657: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  switch (GET_MODE (x1))
    {
    default:
      break;
    case SImode:
      if (s_register_operand (x1, SImode))
	{
	  ro[0] = x1;
	  return 285;
	}
    L1660: ATTRIBUTE_UNUSED_LABEL
      if (memory_operand (x1, SImode))
	{
	  ro[0] = x1;
	  return 286;
	}
    }
  if (GET_CODE (x1) == IF_THEN_ELSE && 1)
    goto L1621;
  x1 = XEXP (x0, 0);
  goto L1753;

  L1621: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (comparison_operator (x2, VOIDmode))
    {
      ro[0] = x2;
      goto L1622;
    }
  x1 = XEXP (x0, 0);
  goto L1753;

  L1622: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_CODE (x3) == REG && cc_register (x3, VOIDmode))
    {
      ro[1] = x3;
      goto L1623;
    }
  x1 = XEXP (x0, 0);
  goto L1753;

  L1623: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L1624;
  x1 = XEXP (x0, 0);
  goto L1753;

  L1624: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  switch (GET_CODE (x2))
    {
    default:
      break;
    case RETURN:
      goto L1625;
    case PC:
      goto L1633;
    }
  x1 = XEXP (x0, 0);
  goto L1753;

  L1625: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == PC && 1)
    if (USE_RETURN_INSN (TRUE))
      return 279;
  x1 = XEXP (x0, 0);
  goto L1753;

  L1633: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == RETURN && 1)
    if (USE_RETURN_INSN (TRUE))
      return 280;
  x1 = XEXP (x0, 0);
  goto L1753;

  L1709: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) != SImode)
    {
      x1 = XEXP (x0, 0);
      goto L1753;
    }
  if (GET_CODE (x1) == MINUS && 1)
    goto L1710;
  L1664: ATTRIBUTE_UNUSED_LABEL
  if (shiftable_operator (x1, SImode))
    {
      ro[1] = x1;
      goto L1665;
    }
  x1 = XEXP (x0, 0);
  goto L1753;

  L1710: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L1711;
    }
  goto L1664;

  L1711: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (shift_operator (x2, SImode))
    {
      ro[2] = x2;
      goto L1712;
    }
  goto L1664;

  L1712: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[3] = x3;
      goto L1713;
    }
  goto L1664;

  L1713: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (reg_or_int_operand (x3, SImode))
    {
      ro[4] = x3;
      return 291;
    }
  goto L1664;

  L1665: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (shift_operator (x2, SImode))
    {
      ro[3] = x2;
      goto L1666;
    }
  x1 = XEXP (x0, 0);
  goto L1753;

  L1666: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[4] = x3;
      goto L1667;
    }
  x1 = XEXP (x0, 0);
  goto L1753;

  L1667: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (reg_or_int_operand (x3, SImode))
    {
      ro[5] = x3;
      goto L1668;
    }
  x1 = XEXP (x0, 0);
  goto L1753;

  L1668: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      ro[2] = x2;
      return 288;
    }
  x1 = XEXP (x0, 0);
  goto L1753;

  L1754: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == SImode && GET_CODE (x1) == PLUS && 1)
    goto L1755;
  x1 = XEXP (x0, 0);
  goto L1863;

  L1755: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == PLUS && 1)
    goto L1756;
  x1 = XEXP (x0, 0);
  goto L1863;

  L1756: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) != SImode)
    {
      x1 = XEXP (x0, 0);
      goto L1863;
    }
  if (shift_operator (x3, SImode))
    {
      ro[5] = x3;
      goto L1757;
    }
  L1811: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == MULT && 1)
    goto L1812;
  x1 = XEXP (x0, 0);
  goto L1863;

  L1757: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  ro[3] = x4;
  goto L1758;

  L1758: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  ro[4] = x4;
  goto L1759;

  L1759: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  ro[2] = x3;
  goto L1760;

  L1760: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && 1)
    {
      ro[1] = x2;
      if (reload_in_progress)
	return 294;
      }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L1811;

  L1812: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  ro[1] = x4;
  goto L1813;

  L1813: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  ro[2] = x4;
  goto L1814;

  L1814: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  ro[3] = x3;
  goto L1815;

  L1815: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && 1)
    {
      ro[4] = x2;
      if (reload_in_progress)
	return 297;
      }
  x1 = XEXP (x0, 0);
  goto L1863;

  L1864: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  switch (GET_MODE (x1))
    {
    default:
      break;
    case SImode:
      switch (GET_CODE (x1))
	{
	default:
	  break;
	case AND:
	  goto L1865;
	case IOR:
	  goto L1872;
	case IF_THEN_ELSE:
	  goto L1892;
	case MINUS:
	  goto L1929;
	case NEG:
	  goto L1969;
	case SIGN_EXTRACT:
	  goto L2816;
	case NOT:
	  goto L2822;
	}
    L1911: ATTRIBUTE_UNUSED_LABEL
      if (shiftable_operator (x1, SImode))
	{
	  ro[5] = x1;
	  goto L1912;
	}
    L2469: ATTRIBUTE_UNUSED_LABEL
      if (shiftable_operator (x1, SImode))
	{
	  ro[1] = x1;
	  goto L2470;
	}
    }
  if (comparison_operator (x1, VOIDmode))
    {
      ro[1] = x1;
      goto L1887;
    }
  x1 = XEXP (x0, 0);
  goto L1934;

  L1865: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (comparison_operator (x2, VOIDmode))
    {
      ro[1] = x2;
      goto L1866;
    }
  goto L1911;

  L1866: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_CODE (x3) == REG && cc_register (x3, VOIDmode))
    {
      ro[3] = x3;
      goto L1867;
    }
  goto L1911;

  L1867: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L1868;
  goto L1911;

  L1868: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      ro[2] = x2;
      return 300;
    }
  goto L1911;

  L1872: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (comparison_operator (x2, VOIDmode))
    {
      ro[2] = x2;
      goto L1873;
    }
  goto L1911;

  L1873: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_CODE (x3) == REG && cc_register (x3, VOIDmode))
    {
      ro[3] = x3;
      goto L1874;
    }
  goto L1911;

  L1874: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L1875;
  goto L1911;

  L1875: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      return 301;
    }
  goto L1911;
 L1892: ATTRIBUTE_UNUSED_LABEL
  tem = recog_11 (x0, insn, pnum_clobbers);
  if (tem >= 0) return tem;
  x1 = XEXP (x0, 0);
  goto L1934;

  L1929: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L1930;
    }
  goto L1911;

  L1930: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (comparison_operator (x2, SImode))
    {
      ro[4] = x2;
      goto L1931;
    }
  goto L1911;

  L1931: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L1932;
    }
  goto L1911;

  L1932: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (pnum_clobbers != 0 && arm_rhs_operand (x3, SImode))
    {
      ro[3] = x3;
      *pnum_clobbers = 1;
      return 305;
    }
  goto L1911;

  L1969: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (comparison_operator (x2, VOIDmode))
    {
      ro[3] = x2;
      goto L1970;
    }
  x1 = XEXP (x0, 0);
  goto L1934;

  L1970: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L1971;
    }
  x1 = XEXP (x0, 0);
  goto L1934;

  L1971: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (pnum_clobbers != 0 && arm_rhs_operand (x3, SImode))
    {
      ro[2] = x3;
      *pnum_clobbers = 1;
      return 308;
    }
  x1 = XEXP (x0, 0);
  goto L1934;

  L2816: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L2817;
    }
  x1 = XEXP (x0, 0);
  goto L1934;

  L2817: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 1 && 1)
    goto L2818;
  x1 = XEXP (x0, 0);
  goto L1934;

  L2818: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == CONST_INT && 1)
    {
      ro[2] = x2;
      return 382;
    }
  x1 = XEXP (x0, 0);
  goto L1934;

  L2822: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == SIGN_EXTRACT && 1)
    goto L2823;
  x1 = XEXP (x0, 0);
  goto L1934;

  L2823: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L2824;
    }
  x1 = XEXP (x0, 0);
  goto L1934;

  L2824: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 1 && 1)
    goto L2825;
  x1 = XEXP (x0, 0);
  goto L1934;

  L2825: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (GET_CODE (x3) == CONST_INT && 1)
    {
      ro[2] = x3;
      return 383;
    }
  x1 = XEXP (x0, 0);
  goto L1934;

  L1912: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (comparison_operator (x2, SImode))
    {
      ro[4] = x2;
      goto L1913;
    }
  goto L2469;

  L1913: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L1914;
    }
  goto L2469;

  L1914: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[3] = x3;
      goto L1915;
    }
  goto L2469;

  L1915: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (pnum_clobbers != 0 && s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      *pnum_clobbers = 1;
      return 304;
    }
  goto L2469;

  L2470: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (memory_operand (x2, SImode))
    {
      ro[2] = x2;
      goto L2471;
    }
  x1 = XEXP (x0, 0);
  goto L1934;

  L2471: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (pnum_clobbers != 0 && memory_operand (x2, SImode))
    {
      ro[3] = x2;
      if (adjacent_mem_locations (operands[2], operands[3]))
	{
	  *pnum_clobbers = 1;
	  return 338;
	}
      }
  x1 = XEXP (x0, 0);
  goto L1934;

  L1887: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[2] = x2;
      goto L1888;
    }
  x1 = XEXP (x0, 0);
  goto L1934;

  L1888: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (pnum_clobbers != 0 && arm_add_operand (x2, SImode))
    {
      ro[3] = x2;
      *pnum_clobbers = 1;
      return 302;
    }
  x1 = XEXP (x0, 0);
  goto L1934;

  L1700: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == CC_NOOVmode && GET_CODE (x1) == COMPARE && 1)
    goto L1746;
  x1 = XEXP (x0, 0);
  goto L1934;

  L1746: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) != SImode)
    {
      x1 = XEXP (x0, 0);
      goto L1934;
    }
  switch (GET_CODE (x2))
    {
    default:
      break;
    case MINUS:
      goto L1747;
    case PLUS:
      goto L1800;
    }
  L1701: ATTRIBUTE_UNUSED_LABEL
  if (shiftable_operator (x2, SImode))
    {
      ro[1] = x2;
      goto L1702;
    }
  x1 = XEXP (x0, 0);
  goto L1934;

  L1747: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L1748;
    }
  goto L1701;

  L1748: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (shift_operator (x3, SImode))
    {
      ro[2] = x3;
      goto L1749;
    }
  goto L1701;

  L1749: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[3] = x4;
      goto L1750;
    }
  goto L1701;

  L1750: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (reg_or_int_operand (x4, SImode))
    {
      ro[4] = x4;
      goto L1751;
    }
  goto L1701;

  L1751: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && pnum_clobbers != 0 && 1)
    {
      *pnum_clobbers = 1;
      return 293;
    }
  x2 = XEXP (x1, 0);
  goto L1701;

  L1800: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode && GET_CODE (x3) == PLUS && 1)
    goto L1801;
  goto L1701;

  L1801: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) != SImode)
    {
      goto L1701;
    }
  if (shift_operator (x4, SImode))
    {
      ro[5] = x4;
      goto L1802;
    }
  L1856: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x4) == MULT && 1)
    goto L1857;
  goto L1701;

  L1802: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  ro[3] = x5;
  goto L1803;

  L1803: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  ro[4] = x5;
  goto L1804;

  L1804: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  ro[1] = x4;
  goto L1805;

  L1805: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && 1)
    {
      ro[2] = x3;
      goto L1806;
    }
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L1856;

  L1806: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && pnum_clobbers != 0 && 1)
    if (reload_in_progress)
      {
	*pnum_clobbers = 1;
	return 296;
      }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L1856;

  L1857: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  ro[3] = x5;
  goto L1858;

  L1858: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  ro[4] = x5;
  goto L1859;

  L1859: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  ro[1] = x4;
  goto L1860;

  L1860: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && 1)
    {
      ro[2] = x3;
      goto L1861;
    }
  goto L1701;

  L1861: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && pnum_clobbers != 0 && 1)
    if (reload_in_progress)
      {
	*pnum_clobbers = 1;
	return 299;
      }
  x2 = XEXP (x1, 0);
  goto L1701;

  L1702: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (shift_operator (x3, SImode))
    {
      ro[3] = x3;
      goto L1703;
    }
  x1 = XEXP (x0, 0);
  goto L1934;

  L1703: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[4] = x4;
      goto L1704;
    }
  x1 = XEXP (x0, 0);
  goto L1934;

  L1704: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (reg_or_int_operand (x4, SImode))
    {
      ro[5] = x4;
      goto L1705;
    }
  x1 = XEXP (x0, 0);
  goto L1934;

  L1705: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L1706;
    }
  x1 = XEXP (x0, 0);
  goto L1934;

  L1706: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && pnum_clobbers != 0 && 1)
    {
      *pnum_clobbers = 1;
      return 290;
    }
  x1 = XEXP (x0, 0);
  goto L1934;

  L1935: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_CODE (x1) == COMPARE && 1)
    goto L1936;
  goto ret0;

  L1936: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == IF_THEN_ELSE && 1)
    goto L1937;
  goto ret0;

  L1937: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (comparison_operator (x3, VOIDmode))
    {
      ro[4] = x3;
      goto L1938;
    }
  goto ret0;

  L1938: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[0] = x4;
      goto L1939;
    }
  goto ret0;

  L1939: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_add_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L1940;
    }
  goto ret0;

  L1940: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (comparison_operator (x3, SImode))
    {
      ro[5] = x3;
      goto L1941;
    }
  goto ret0;

  L1941: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L1942;
    }
  goto ret0;

  L1942: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_add_operand (x4, SImode))
    {
      ro[3] = x4;
      goto L1943;
    }
  goto ret0;

  L1943: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (GET_CODE (x3) != CONST_INT)
    goto ret0;
  if (XWINT (x3, 0) == 0 && 1)
    goto L1944;
  if (XWINT (x3, 0) == 1 && 1)
    goto L1956;
  goto ret0;

  L1944: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && 1)
    return 306;
  goto ret0;

  L1956: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && 1)
    return 307;
  goto ret0;
 ret0: return -1;
}

int
recog (x0, insn, pnum_clobbers)
     register rtx x0;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  register rtx *ro = &recog_operand[0];
  register rtx x1 ATTRIBUTE_UNUSED, x2 ATTRIBUTE_UNUSED, x3 ATTRIBUTE_UNUSED, x4 ATTRIBUTE_UNUSED, x5 ATTRIBUTE_UNUSED, x6 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;

  L0: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x0))
    {
    default:
      break;
    case PARALLEL:
      if (XVECLEN (x0, 0) == 2 && 1)
	goto L1;
    L1354: ATTRIBUTE_UNUSED_LABEL
      if (load_multiple_operation (x0, VOIDmode))
	{
	  ro[0] = x0;
	  goto L1355;
	}
    L1369: ATTRIBUTE_UNUSED_LABEL
      if (store_multiple_operation (x0, VOIDmode))
	{
	  ro[0] = x0;
	  goto L1370;
	}
    L1561: ATTRIBUTE_UNUSED_LABEL
      if (XVECLEN (x0, 0) == 2 && 1)
	goto L1562;
    L2826: ATTRIBUTE_UNUSED_LABEL
      if (multi_register_push (x0, VOIDmode))
	{
	  ro[2] = x0;
	  goto L2827;
	}
      break;
    case SET:
      goto L9;
    case CALL:
      goto L1547;
    case RETURN:
      if (USE_RETURN_INSN (FALSE))
	return 278;
      break;
    case UNSPEC_VOLATILE:
      if (XINT (x0, 1) == 0 && XVECLEN (x0, 0) == 1 && 1)
	goto L1635;
      if (XINT (x0, 1) == 2 && XVECLEN (x0, 0) == 1 && 1)
	goto L2837;
      if (XINT (x0, 1) == 3 && XVECLEN (x0, 0) == 1 && 1)
	goto L2839;
      if (XINT (x0, 1) == 4 && XVECLEN (x0, 0) == 1 && 1)
	goto L2841;
      if (XINT (x0, 1) == 5 && XVECLEN (x0, 0) == 1 && 1)
	goto L2843;
      break;
    case CONST_INT:
      if (XWINT (x0, 0) == 0 && 1)
	return 287;
    }
  goto ret0;

  L1: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  switch (GET_CODE (x1))
    {
    default:
      break;
    case SET:
      goto L2;
    case CALL:
      goto L1541;
    }
  goto L1354;
 L2: ATTRIBUTE_UNUSED_LABEL
  tem = recog_3 (x0, insn, pnum_clobbers);
  if (tem >= 0) return tem;
  goto L1354;

  L1541: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == MEM && 1)
    goto L1542;
  goto L1354;

  L1542: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[0] = x3;
      goto L1543;
    }
  L1553: ATTRIBUTE_UNUSED_LABEL
  if (memory_operand (x3, VOIDmode))
    {
      ro[0] = x3;
      goto L1554;
    }
  goto L1354;

  L1543: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  ro[1] = x2;
  goto L1544;

  L1544: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1545;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L1553;

  L1545: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == REG && XINT (x2, 0) == 14 && 1)
    return 271;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L1553;

  L1554: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (general_operand (x2, VOIDmode))
    {
      ro[1] = x2;
      goto L1555;
    }
  goto L1354;

  L1555: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1556;
  goto L1354;

  L1556: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == REG && XINT (x2, 0) == 14 && 1)
    return 272;
  goto L1354;

  L1355: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  if (GET_CODE (x1) == SET && 1)
    goto L1356;
  goto L1369;

  L1356: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L1357;
    }
  goto L1369;

  L1357: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) != SImode)
    {
      goto L1369;
    }
  switch (GET_CODE (x2))
    {
    default:
      break;
    case PLUS:
      goto L1358;
    case MEM:
      goto L1368;
    }
  goto L1369;

  L1358: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L1359;
  goto L1369;

  L1359: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && 1)
    {
      ro[2] = x3;
      goto L1360;
    }
  goto L1369;

  L1360: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L1361;
  goto L1369;

  L1361: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[3] = x2;
      goto L1362;
    }
  goto L1369;

  L1362: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == MEM && 1)
    goto L1363;
  goto L1369;

  L1363: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    if ((INTVAL (operands[2])  == 4 * (XVECLEN (operands[0], 0) - 2)))
      return 212;
  goto L1369;

  L1368: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      return 213;
    }
  goto L1369;

  L1370: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  if (GET_CODE (x1) == SET && 1)
    goto L1381;
  goto L1561;

  L1381: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) != SImode)
    {
      goto L1561;
    }
  if (GET_CODE (x2) == MEM && 1)
    goto L1382;
  if (s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L1372;
    }
  goto L1561;

  L1382: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L1383;
    }
  goto L1561;

  L1383: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      return 216;
    }
  goto L1561;

  L1372: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == PLUS && 1)
    goto L1373;
  goto L1561;

  L1373: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L1374;
  goto L1561;

  L1374: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && 1)
    {
      ro[2] = x3;
      goto L1375;
    }
  goto L1561;

  L1375: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L1376;
  goto L1561;

  L1376: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == MEM && 1)
    goto L1377;
  goto L1561;

  L1377: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L1378;
  goto L1561;

  L1378: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      ro[3] = x2;
      if ((INTVAL (operands[2]) == 4 * (XVECLEN (operands[0], 0) - 2)))
	return 215;
      }
  goto L1561;

  L1562: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  switch (GET_CODE (x1))
    {
    default:
      break;
    case SET:
      goto L1563;
    case CALL:
      goto L1593;
    }
  goto L2826;
 L1563: ATTRIBUTE_UNUSED_LABEL
  tem = recog_7 (x0, insn, pnum_clobbers);
  if (tem >= 0) return tem;
  goto L2826;

  L1593: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == MEM && 1)
    goto L1594;
  goto L2826;

  L1594: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  ro[0] = x3;
  goto L1595;

  L1595: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L1596;
    }
  goto L2826;

  L1596: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1597;
  goto L2826;

  L1597: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == REG && XINT (x2, 0) == 14 && 1)
    if (GET_CODE (operands[0]) == SYMBOL_REF)
      return 276;
  goto L2826;

  L2827: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  if (GET_CODE (x1) == SET && 1)
    goto L2828;
  goto ret0;

  L2828: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (memory_operand (x2, BLKmode))
    {
      ro[0] = x2;
      goto L2829;
    }
  goto ret0;

  L2829: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == BLKmode && GET_CODE (x2) == UNSPEC && XINT (x2, 1) == 2 && XVECLEN (x2, 0) == 1 && 1)
    goto L2830;
  goto ret0;

  L2830: ATTRIBUTE_UNUSED_LABEL
  x3 = XVECEXP (x2, 0, 0);
  switch (GET_MODE (x3))
    {
    default:
      break;
    case SImode:
      if (s_register_operand (x3, SImode))
	{
	  ro[1] = x3;
	  return 384;
	}
      break;
    case XFmode:
      if (f_register_operand (x3, XFmode))
	{
	  ro[1] = x3;
	  return 385;
	}
    }
  goto ret0;
 L9: ATTRIBUTE_UNUSED_LABEL
  return recog_12 (x0, insn, pnum_clobbers);

  L1547: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 0);
  if (GET_MODE (x1) == SImode && GET_CODE (x1) == MEM && 1)
    goto L1548;
  goto ret0;

  L1548: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L1549;
    }
  L1559: ATTRIBUTE_UNUSED_LABEL
  if (memory_operand (x2, VOIDmode))
    {
      ro[0] = x2;
      goto L1560;
    }
  L1600: ATTRIBUTE_UNUSED_LABEL
  ro[0] = x2;
  goto L1601;

  L1549: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (pnum_clobbers != 0 && 1)
    {
      ro[1] = x1;
      *pnum_clobbers = 1;
      return 271;
    }
  x1 = XEXP (x0, 0);
  x2 = XEXP (x1, 0);
  goto L1559;

  L1560: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (pnum_clobbers != 0 && general_operand (x1, VOIDmode))
    {
      ro[1] = x1;
      *pnum_clobbers = 1;
      return 272;
    }
  x1 = XEXP (x0, 0);
  x2 = XEXP (x1, 0);
  goto L1600;

  L1601: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (pnum_clobbers != 0 && general_operand (x1, SImode))
    {
      ro[1] = x1;
      if (GET_CODE (operands[0]) == SYMBOL_REF)
	{
	  *pnum_clobbers = 1;
	  return 276;
	}
      }
  goto ret0;

  L1635: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  if (GET_CODE (x1) == CONST_INT && XWINT (x1, 0) == 0 && 1)
    return 282;
  goto ret0;

  L2837: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  ro[0] = x1;
  return 386;

  L2839: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  ro[0] = x1;
  return 387;

  L2841: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  if (GET_CODE (x1) == CONST_INT && XWINT (x1, 0) == 0 && 1)
    return 388;
  goto ret0;

  L2843: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  if (GET_CODE (x1) == CONST_INT && XWINT (x1, 0) == 0 && 1)
    return 389;
  goto ret0;
 ret0: return -1;
}

rtx
split_1 (x0, insn)
     register rtx x0;
     rtx insn ATTRIBUTE_UNUSED;
{
  register rtx *ro = &recog_operand[0];
  register rtx x1 ATTRIBUTE_UNUSED, x2 ATTRIBUTE_UNUSED, x3 ATTRIBUTE_UNUSED, x4 ATTRIBUTE_UNUSED, x5 ATTRIBUTE_UNUSED, x6 ATTRIBUTE_UNUSED;
  rtx tem ATTRIBUTE_UNUSED;

  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  switch (GET_MODE (x2))
    {
    default:
      break;
    case SImode:
      if (s_register_operand (x2, SImode))
	{
	  ro[0] = x2;
	  goto L836;
	}
      break;
    case CC_Zmode:
      if (GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
	goto L2794;
    }
  goto ret0;

  L836: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) != SImode)
    goto ret0;
  switch (GET_CODE (x2))
    {
    default:
      break;
    case IOR:
      goto L837;
    case ZERO_EXTEND:
      goto L1190;
    case SIGN_EXTEND:
      goto L1227;
    case AND:
      goto L2782;
    case IF_THEN_ELSE:
      goto L2806;
    }
  L1196: ATTRIBUTE_UNUSED_LABEL
  if (shiftable_operator (x2, SImode))
    {
      ro[3] = x2;
      goto L1197;
    }
  goto ret0;

  L837: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode && GET_CODE (x3) == AND && 1)
    goto L838;
  goto L1196;

  L838: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode && GET_CODE (x4) == NOT && 1)
    goto L839;
  goto L1196;

  L839: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      ro[1] = x5;
      goto L840;
    }
  goto L1196;

  L840: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_MODE (x4) == SImode && GET_CODE (x4) == NOT && 1)
    goto L841;
  goto L1196;

  L841: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (arm_rhs_operand (x5, SImode))
    {
      ro[2] = x5;
      goto L842;
    }
  goto L1196;

  L842: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      ro[3] = x3;
      goto L843;
    }
  goto L1196;

  L843: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L844;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L1196;

  L844: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[4] = x2;
      return gen_split_101 (operands);
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L1196;

  L1190: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  switch (GET_MODE (x3))
    {
    default:
      break;
    case HImode:
      switch (GET_CODE (x3))
	{
	default:
	  break;
	case MEM:
	  if (alignable_memory_operand (x3, HImode))
	    {
	      ro[1] = x3;
	      goto L1191;
	    }
	}
      break;
    case QImode:
      if (GET_CODE (x3) == SUBREG && XINT (x3, 1) == 0 && 1)
	goto L1211;
    }
  goto ret0;

  L1191: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1192;
  goto ret0;

  L1192: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[2] = x2;
      if (! arm_arch4 || ARM_ARCH_RISCPC)
	return gen_split_154 (operands);
      }
  goto ret0;

  L1211: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode && 1)
    {
      ro[1] = x4;
      goto L1212;
    }
  goto ret0;

  L1212: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1213;
  goto ret0;

  L1213: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[2] = x2;
      if (GET_CODE (operands[1]) != MEM)
	return gen_split_158 (operands);
      }
  goto ret0;

  L1227: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_CODE (x3) == MEM && alignable_memory_operand (x3, HImode))
    {
      ro[1] = x3;
      goto L1228;
    }
  goto ret0;

  L1228: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1229;
  goto ret0;

  L1229: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[2] = x2;
      if (! arm_arch4 || ARM_ARCH_RISCPC)
	return gen_split_163 (operands);
      }
  goto ret0;

  L2782: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode && GET_CODE (x3) == GE && 1)
    goto L2783;
  goto L1196;

  L2783: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L2784;
    }
  goto L1196;

  L2784: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT && XWINT (x4, 0) == 0 && 1)
    goto L2785;
  goto L1196;

  L2785: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode && GET_CODE (x3) == NEG && 1)
    goto L2786;
  goto L1196;

  L2786: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (comparison_operator (x4, SImode))
    {
      ro[2] = x4;
      goto L2787;
    }
  goto L1196;

  L2787: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      ro[3] = x5;
      goto L2788;
    }
  goto L1196;

  L2788: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (arm_rhs_operand (x5, SImode))
    {
      ro[4] = x5;
      goto L2789;
    }
  goto L1196;

  L2789: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L2790;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L1196;

  L2790: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[5] = x2;
      return gen_split_378 (operands);
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L1196;

  L2806: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (comparison_operator (x3, VOIDmode))
    {
      ro[1] = x3;
      goto L2807;
    }
  goto ret0;

  L2807: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  ro[2] = x4;
  goto L2808;

  L2808: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  ro[3] = x4;
  goto L2809;

  L2809: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  ro[4] = x3;
  goto L2810;

  L2810: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  ro[5] = x3;
  goto L2811;

  L2811: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L2812;
  goto ret0;

  L2812: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 24 && 1)
    if (reload_completed)
      return gen_split_381 (operands);
  goto ret0;

  L1197: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) != SImode)
    goto ret0;
  switch (GET_CODE (x3))
    {
    default:
      break;
    case ZERO_EXTEND:
      goto L1198;
    case SIGN_EXTEND:
      goto L1235;
    }
  goto ret0;

  L1198: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_CODE (x4) == MEM && alignable_memory_operand (x4, HImode))
    {
      ro[1] = x4;
      goto L1199;
    }
  goto ret0;

  L1199: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      ro[4] = x3;
      goto L1200;
    }
  goto ret0;

  L1200: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1201;
  goto ret0;

  L1201: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[2] = x2;
      if (! arm_arch4 || ARM_ARCH_RISCPC)
	return gen_split_155 (operands);
      }
  goto ret0;

  L1235: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_CODE (x4) == MEM && alignable_memory_operand (x4, HImode))
    {
      ro[1] = x4;
      goto L1236;
    }
  goto ret0;

  L1236: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      ro[4] = x3;
      goto L1237;
    }
  goto ret0;

  L1237: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1238;
  goto ret0;

  L1238: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[2] = x2;
      if (! arm_arch4 || ARM_ARCH_RISCPC)
	return gen_split_164 (operands);
      }
  goto ret0;

  L2794: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == CC_Zmode && GET_CODE (x2) == COMPARE && 1)
    goto L2795;
  goto ret0;

  L2795: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode && GET_CODE (x3) == ASHIFT && 1)
    goto L2796;
  goto ret0;

  L2796: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode && GET_CODE (x4) == SUBREG && XINT (x4, 1) == 0 && 1)
    goto L2797;
  goto ret0;

  L2797: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (memory_operand (x5, QImode))
    {
      ro[0] = x5;
      goto L2798;
    }
  goto ret0;

  L2798: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT && XWINT (x4, 0) == 24 && 1)
    goto L2799;
  goto ret0;

  L2799: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && 1)
    {
      ro[1] = x3;
      goto L2800;
    }
  goto ret0;

  L2800: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L2801;
  goto ret0;

  L2801: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      ro[2] = x2;
      if (((unsigned HOST_WIDE_INT) INTVAL (operands[1]))
   == (((unsigned HOST_WIDE_INT) INTVAL (operands[1])) >> 24) << 24)
	return gen_split_379 (operands);
      }
  goto ret0;
 ret0: return 0;
}

rtx
split_insns (x0, insn)
     register rtx x0;
     rtx insn ATTRIBUTE_UNUSED;
{
  register rtx *ro = &recog_operand[0];
  register rtx x1 ATTRIBUTE_UNUSED, x2 ATTRIBUTE_UNUSED, x3 ATTRIBUTE_UNUSED, x4 ATTRIBUTE_UNUSED, x5 ATTRIBUTE_UNUSED, x6 ATTRIBUTE_UNUSED;
  rtx tem ATTRIBUTE_UNUSED;

  L43: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x0))
    {
    default:
      break;
    case SET:
      goto L44;
    case PARALLEL:
      if (XVECLEN (x0, 0) == 2 && 1)
	goto L834;
    }
  goto ret0;

  L44: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 0);
  switch (GET_MODE (x1))
    {
    default:
      break;
    case SImode:
      if (s_register_operand (x1, SImode))
	{
	  ro[0] = x1;
	  goto L45;
	}
      break;
    case HImode:
      if (s_register_operand (x1, HImode))
	{
	  ro[0] = x1;
	  goto L1245;
	}
    }
  goto ret0;

  L45: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  switch (GET_MODE (x1))
    {
    default:
      break;
    case SImode:
      switch (GET_CODE (x1))
	{
	default:
	  break;
	case PLUS:
	  goto L46;
	case MINUS:
	  goto L295;
	case AND:
	  goto L618;
	case IOR:
	  goto L759;
	case SIGN_EXTEND:
	  goto L1254;
	}
    }
  if (GET_CODE (x1) == CONST_INT && 1)
    {
      ro[1] = x1;
      if (! (const_ok_for_arm (INTVAL (operands[1]))
      || const_ok_for_arm (~INTVAL (operands[1]))))
	return gen_split_177 (operands);
      }
  goto ret0;

  L46: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L47;
    }
  goto ret0;

  L47: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && 1)
    goto L161;
  goto ret0;

  L161: ATTRIBUTE_UNUSED_LABEL
  ro[2] = x2;
  if (! (const_ok_for_arm (INTVAL (operands[2]))
      || const_ok_for_arm (-INTVAL (operands[2]))))
    return gen_split_4 (operands);
  L162: ATTRIBUTE_UNUSED_LABEL
  ro[2] = x2;
  if (!(const_ok_for_arm (INTVAL (operands[2]))
     || const_ok_for_arm (-INTVAL (operands[2]))))
    return gen_split_17 (operands);
  goto ret0;

  L295: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) == CONST_INT && 1)
    {
      ro[1] = x2;
      goto L296;
    }
  goto ret0;

  L296: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      ro[2] = x2;
      if (! const_ok_for_arm (INTVAL (operands[1])))
	return gen_split_32 (operands);
      }
  goto ret0;

  L618: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L619;
    }
  goto ret0;

  L619: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && 1)
    {
      ro[2] = x2;
      if (! (const_ok_for_arm (INTVAL (operands[2]))
      || const_ok_for_arm (~ INTVAL (operands[2]))))
	return gen_split_74 (operands);
      }
  goto ret0;

  L759: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L760;
    }
  goto ret0;

  L760: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && 1)
    {
      ro[2] = x2;
      if (! const_ok_for_arm (INTVAL (operands[2])))
	return gen_split_92 (operands);
      }
  goto ret0;

  L1254: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) == MEM && bad_signed_byte_operand (x2, QImode))
    {
      ro[1] = x2;
      if (arm_arch4 && reload_completed)
	return gen_split_170 (operands);
      }
  goto ret0;

  L1245: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == HImode && GET_CODE (x1) == SIGN_EXTEND && 1)
    goto L1246;
  goto ret0;

  L1246: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) == MEM && bad_signed_byte_operand (x2, QImode))
    {
      ro[1] = x2;
      if (arm_arch4 && reload_completed)
	return gen_split_167 (operands);
      }
  goto ret0;

  L834: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  if (GET_CODE (x1) == SET && 1)
    goto L835;
  goto ret0;
 L835: ATTRIBUTE_UNUSED_LABEL
  return split_1 (x0, insn);
 ret0: return 0;
}

