/* Generated automatically by the program `genrecog' from the target
   machine description file.  */

#include "config.h"
#include "system.h"
#include "rtl.h"
#include "tm_p.h"
#include "function.h"
#include "insn-config.h"
#include "recog.h"
#include "real.h"
#include "output.h"
#include "flags.h"
#include "hard-reg-set.h"
#include "resource.h"
#include "toplev.h"
#include "reload.h"



/* `recog' contains a decision tree that recognizes whether the rtx
   X0 is a valid instruction.

   recog returns -1 if the rtx is not valid.  If the rtx is valid, recog
   returns a nonnegative number which is the insn code number for the
   pattern that matched.  This is the same as the order in the machine
   description of the entry that matched.  This number can be used as an
   index into `insn_data' and other tables.

   The third argument to recog is an optional pointer to an int.  If
   present, recog will accept a pattern if it matches except for missing
   CLOBBER expressions at the end.  In that case, the value pointed to by
   the optional pointer will be set to the number of CLOBBERs that need
   to be added (it should be initialized to zero by the caller).  If it
   is set nonzero, the caller should allocate a PARALLEL of the
   appropriate size, copy the initial entries, and call add_clobbers
   (found in insn-emit.c) to fill in the CLOBBERs.


   The function split_insns returns 0 if the rtl could not
   be split or the split rtl in a SEQUENCE if it can be.

   The function peephole2_insns returns 0 if the rtl could not
   be matched. If there was a match, the new rtl is returned in a SEQUENCE,
   and LAST_INSN will point to the last recognized insn in the old sequence.
*/


extern rtx gen_split_360 PARAMS ((rtx *));
extern rtx gen_split_361 PARAMS ((rtx *));
extern rtx gen_split_362 PARAMS ((rtx *));
extern rtx gen_peephole2_364 PARAMS ((rtx, rtx *));
extern rtx gen_split_365 PARAMS ((rtx *));
extern rtx gen_peephole2_366 PARAMS ((rtx, rtx *));
extern rtx gen_split_369 PARAMS ((rtx *));
extern rtx gen_peephole2_370 PARAMS ((rtx, rtx *));
extern rtx gen_split_372 PARAMS ((rtx *));
extern rtx gen_split_373 PARAMS ((rtx *));
extern rtx gen_split_374 PARAMS ((rtx *));
extern rtx gen_split_375 PARAMS ((rtx *));
extern rtx gen_split_376 PARAMS ((rtx *));
extern rtx gen_split_378 PARAMS ((rtx *));
extern rtx gen_split_380 PARAMS ((rtx *));
extern rtx gen_split_381 PARAMS ((rtx *));
extern rtx gen_split_382 PARAMS ((rtx *));
extern rtx gen_split_384 PARAMS ((rtx *));
extern rtx gen_peephole2_385 PARAMS ((rtx, rtx *));
extern rtx gen_split_387 PARAMS ((rtx *));
extern rtx gen_split_396 PARAMS ((rtx *));
extern rtx gen_split_399 PARAMS ((rtx *));
extern rtx gen_split_400 PARAMS ((rtx *));
extern rtx gen_split_402 PARAMS ((rtx *));
extern rtx gen_split_405 PARAMS ((rtx *));
extern rtx gen_split_406 PARAMS ((rtx *));
extern rtx gen_split_408 PARAMS ((rtx *));
extern rtx gen_split_410 PARAMS ((rtx *));
extern rtx gen_split_413 PARAMS ((rtx *));
extern rtx gen_split_414 PARAMS ((rtx *));
extern rtx gen_split_415 PARAMS ((rtx *));
extern rtx gen_split_430 PARAMS ((rtx *));
extern rtx gen_peephole2_493 PARAMS ((rtx, rtx *));
extern rtx gen_split_500 PARAMS ((rtx *));
extern rtx gen_split_501 PARAMS ((rtx *));
extern rtx gen_split_505 PARAMS ((rtx *));
extern rtx gen_split_506 PARAMS ((rtx *));
extern rtx gen_split_507 PARAMS ((rtx *));
extern rtx gen_split_508 PARAMS ((rtx *));



static int recog_1 PARAMS ((rtx, rtx, int *));
static int
recog_1 (x0, insn, pnum_clobbers)
     rtx x0 ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  rtx * const operands ATTRIBUTE_UNUSED = &recog_data.operand[0];
  rtx x1 ATTRIBUTE_UNUSED;
  rtx x2 ATTRIBUTE_UNUSED;
  rtx x3 ATTRIBUTE_UNUSED;
  rtx x4 ATTRIBUTE_UNUSED;
  rtx x5 ATTRIBUTE_UNUSED;
  rtx x6 ATTRIBUTE_UNUSED;
  rtx x7 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;

  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  switch (GET_CODE (x3))
    {
    case PLUS:
      goto L77;
    case MINUS:
      goto L377;
    case MULT:
      goto L449;
    case AND:
      goto L791;
    case IOR:
      goto L951;
    case XOR:
      goto L1014;
    case NOT:
      goto L1199;
    default:
     break;
   }
 L5981: ATTRIBUTE_UNUSED_LABEL
  if (shift_operator (x3, SImode))
    {
      operands[3] = x3;
      goto L1161;
    }
  goto ret0;

 L77: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode)
    goto L5984;
  goto ret0;

 L5984: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x4) == MULT)
    goto L489;
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L78;
    }
  goto ret0;

 L489: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[2] = x5;
      goto L490;
    }
  goto ret0;

 L490: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (s_register_operand (x5, SImode))
    {
      operands[1] = x5;
      goto L491;
    }
  goto ret0;

 L491: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (s_register_operand (x4, SImode))
    {
      operands[3] = x4;
      goto L492;
    }
  goto ret0;

 L492: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L493;
  goto ret0;

 L493: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  switch (GET_CODE (x1))
    {
    case SET:
      goto L494;
    case CLOBBER:
      goto L511;
    default:
     break;
   }
  goto ret0;

 L494: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L495;
    }
  goto ret0;

 L495: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L496;
  goto ret0;

 L496: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MULT)
    goto L497;
  goto ret0;

 L497: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[2]))
    goto L498;
  goto ret0;

 L498: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[1]))
    goto L499;
  goto ret0;

 L499: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[3])
      && (TARGET_ARM && !arm_is_xscale))
    {
      return 48;
    }
  goto ret0;

 L511: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L512;
    }
  goto ret0;

 L512: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && !arm_is_xscale))
    {
      return 49;
    }
  goto ret0;

 L78: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_add_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L79;
    }
  goto ret0;

 L79: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L80;
  goto ret0;

 L80: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L81;
  goto ret0;

 L81: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L82;
    }
  goto ret0;

 L82: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L83;
  goto ret0;

 L83: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[1]))
    goto L84;
  goto ret0;

 L84: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[2])
      && (TARGET_ARM))
    {
      return 6;
    }
  goto ret0;

 L377: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (arm_rhs_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L378;
    }
  goto ret0;

 L378: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L379;
    }
  goto ret0;

 L379: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L380;
  goto ret0;

 L380: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L381;
  goto ret0;

 L381: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L382;
    }
  goto ret0;

 L382: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MINUS)
    goto L383;
  goto ret0;

 L383: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[1]))
    goto L384;
  goto ret0;

 L384: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[2])
      && (TARGET_ARM))
    {
      return 35;
    }
  goto ret0;

 L449: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L450;
    }
  goto L5981;

 L450: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L451;
    }
  goto L5981;

 L451: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L452;
  x3 = XEXP (x2, 0);
  goto L5981;

 L452: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  switch (GET_CODE (x1))
    {
    case SET:
      goto L453;
    case CLOBBER:
      goto L466;
    default:
     break;
   }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L5981;

 L453: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L454;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L5981;

 L454: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MULT)
    goto L455;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L5981;

 L455: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[2]))
    goto L456;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L5981;

 L456: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[1])
      && (TARGET_ARM && !arm_is_xscale))
    {
      return 45;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L5981;

 L466: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L467;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L5981;

 L467: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && !arm_is_xscale))
    {
      return 46;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L5981;

 L791: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode)
    goto L5987;
  goto ret0;

 L5987: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x4) == NOT)
    goto L885;
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L792;
    }
 L5986: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x4, SImode))
    {
      operands[0] = x4;
      goto L805;
    }
  goto ret0;

 L885: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[2] = x5;
      goto L886;
    }
  goto ret0;

 L886: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L887;
    }
  goto ret0;

 L887: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L888;
  goto ret0;

 L888: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  switch (GET_CODE (x1))
    {
    case SET:
      goto L889;
    case CLOBBER:
      goto L904;
    default:
     break;
   }
  goto ret0;

 L889: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L890;
    }
  goto ret0;

 L890: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == AND)
    goto L891;
  goto ret0;

 L891: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == NOT)
    goto L892;
  goto ret0;

 L892: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[2]))
    goto L893;
  goto ret0;

 L893: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[1])
      && (TARGET_ARM))
    {
      return 92;
    }
  goto ret0;

 L904: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L905;
    }
  goto ret0;

 L905: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 93;
    }
  goto ret0;

 L792: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_not_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L793;
    }
  x4 = XEXP (x3, 0);
  goto L5986;

 L793: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L794;
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L5986;

 L794: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L795;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L5986;

 L795: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L796;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L5986;

 L796: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == AND)
    goto L797;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L5986;

 L797: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[1]))
    goto L798;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L5986;

 L798: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[2])
      && (TARGET_ARM))
    {
      return 82;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L5986;

 L805: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_not_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L806;
    }
  goto ret0;

 L806: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L807;
  goto ret0;

 L807: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L808;
  goto ret0;

 L808: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L809;
    }
  goto ret0;

 L809: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 83;
    }
  goto ret0;

 L951: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L952;
    }
  goto ret0;

 L952: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L953;
    }
  goto ret0;

 L953: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L954;
  goto ret0;

 L954: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  switch (GET_CODE (x1))
    {
    case SET:
      goto L955;
    case CLOBBER:
      goto L968;
    default:
     break;
   }
  goto ret0;

 L955: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L956;
    }
  goto ret0;

 L956: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == IOR)
    goto L957;
  goto ret0;

 L957: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[1]))
    goto L958;
  goto ret0;

 L958: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[2])
      && (TARGET_ARM))
    {
      return 99;
    }
  goto ret0;

 L968: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L969;
    }
  goto ret0;

 L969: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 100;
    }
  goto ret0;

 L1014: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L1015;
    }
  goto ret0;

 L1015: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L1016;
    }
  goto ret0;

 L1016: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L1017;
  goto ret0;

 L1017: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L1018;
  goto ret0;

 L1018: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L1019;
    }
  goto ret0;

 L1019: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == XOR)
    goto L1020;
  goto ret0;

 L1020: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[1]))
    goto L1021;
  goto ret0;

 L1021: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[2])
      && (TARGET_ARM))
    {
      return 106;
    }
  goto ret0;

 L1199: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode)
    goto L5988;
  goto ret0;

 L5988: ATTRIBUTE_UNUSED_LABEL
  if (shift_operator (x4, SImode))
    {
      operands[3] = x4;
      goto L1200;
    }
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L1373;
    }
  goto ret0;

 L1200: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[1] = x5;
      goto L1201;
    }
  goto ret0;

 L1201: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (arm_rhs_operand (x5, SImode))
    {
      operands[2] = x5;
      goto L1202;
    }
  goto ret0;

 L1202: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L1203;
  goto ret0;

 L1203: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  switch (GET_CODE (x1))
    {
    case SET:
      goto L1204;
    case CLOBBER:
      goto L1219;
    default:
     break;
   }
  goto ret0;

 L1204: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L1205;
    }
  goto ret0;

 L1205: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == NOT)
    goto L1206;
  goto ret0;

 L1206: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[3]))
    {
      operands[3] = x3;
      goto L1207;
    }
  goto ret0;

 L1207: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[1]))
    goto L1208;
  goto ret0;

 L1208: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[2])
      && (TARGET_ARM))
    {
      return 123;
    }
  goto ret0;

 L1219: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L1220;
    }
  goto ret0;

 L1220: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 124;
    }
  goto ret0;

 L1373: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L1374;
  goto ret0;

 L1374: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  switch (GET_CODE (x1))
    {
    case SET:
      goto L1375;
    case CLOBBER:
      goto L1386;
    default:
     break;
   }
  goto ret0;

 L1375: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L1376;
    }
  goto ret0;

 L1376: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == NOT)
    goto L1377;
  goto ret0;

 L1377: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[1])
      && (TARGET_ARM))
    {
      return 146;
    }
  goto ret0;

 L1386: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L1387;
    }
  goto ret0;

 L1387: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 147;
    }
  goto ret0;

 L1161: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L1162;
    }
  goto ret0;

 L1162: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L1163;
    }
  goto ret0;

 L1163: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L1164;
  goto ret0;

 L1164: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  switch (GET_CODE (x1))
    {
    case SET:
      goto L1165;
    case CLOBBER:
      goto L1178;
    default:
     break;
   }
  goto ret0;

 L1165: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L1166;
    }
  goto ret0;

 L1166: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (rtx_equal_p (x2, operands[3]))
    {
      operands[3] = x2;
      goto L1167;
    }
  goto ret0;

 L1167: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[1]))
    goto L1168;
  goto ret0;

 L1168: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[2])
      && (TARGET_ARM))
    {
      return 120;
    }
  goto ret0;

 L1178: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L1179;
    }
  goto ret0;

 L1179: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 121;
    }
  goto ret0;
 ret0:
  return -1;
}

static int recog_2 PARAMS ((rtx, rtx, int *));
static int
recog_2 (x0, insn, pnum_clobbers)
     rtx x0 ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  rtx * const operands ATTRIBUTE_UNUSED = &recog_data.operand[0];
  rtx x1 ATTRIBUTE_UNUSED;
  rtx x2 ATTRIBUTE_UNUSED;
  rtx x3 ATTRIBUTE_UNUSED;
  rtx x4 ATTRIBUTE_UNUSED;
  rtx x5 ATTRIBUTE_UNUSED;
  rtx x6 ATTRIBUTE_UNUSED;
  rtx x7 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;

  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  switch (GET_MODE (x2))
    {
    case DImode:
      goto L5949;
    case CC_NOOVmode:
      goto L5951;
    case CCmode:
      goto L5952;
    case CC_Cmode:
      goto L5953;
    case SImode:
      goto L5955;
    case HImode:
      goto L5958;
    default:
      break;
    }
  goto ret0;

 L5949: ATTRIBUTE_UNUSED_LABEL
  if (register_operand (x2, DImode))
    {
      operands[0] = x2;
      goto L3;
    }
 L5950: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, DImode))
    {
      operands[0] = x2;
      goto L17;
    }
 L5954: ATTRIBUTE_UNUSED_LABEL
  if (register_operand (x2, DImode))
    {
      operands[0] = x2;
      goto L267;
    }
  goto ret0;

 L3: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == DImode
      && GET_CODE (x2) == PLUS)
    goto L4;
  x2 = XEXP (x1, 0);
  goto L5950;

 L4: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (register_operand (x3, DImode))
    {
      operands[1] = x3;
      goto L5;
    }
  x2 = XEXP (x1, 0);
  goto L5950;

 L5: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (register_operand (x3, DImode))
    {
      operands[2] = x3;
      goto L6;
    }
  x2 = XEXP (x1, 0);
  goto L5950;

 L6: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L7;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5950;

 L7: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_THUMB))
    {
      return 0;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5950;

 L17: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == DImode)
    goto L5959;
  x2 = XEXP (x1, 0);
  goto L5954;

 L5959: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x2))
    {
    case PLUS:
      goto L18;
    case MINUS:
      goto L254;
    case NEG:
      goto L1233;
    default:
     break;
   }
  x2 = XEXP (x1, 0);
  goto L5954;

 L18: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == DImode)
    goto L5963;
  x2 = XEXP (x1, 0);
  goto L5954;

 L5963: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x3))
    {
    case SIGN_EXTEND:
      goto L33;
    case ZERO_EXTEND:
      goto L49;
    case SUBREG:
    case REG:
      goto L5962;
    default:
      x2 = XEXP (x1, 0);
      goto L5954;
   }
 L5962: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x3, DImode))
    {
      operands[1] = x3;
      goto L19;
    }
  x2 = XEXP (x1, 0);
  goto L5954;

 L33: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L34;
    }
  x2 = XEXP (x1, 0);
  goto L5954;

 L34: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, DImode))
    {
      operands[1] = x3;
      goto L35;
    }
  x2 = XEXP (x1, 0);
  goto L5954;

 L35: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L36;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5954;

 L36: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 2;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5954;

 L49: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L50;
    }
  x2 = XEXP (x1, 0);
  goto L5954;

 L50: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, DImode))
    {
      operands[1] = x3;
      goto L51;
    }
  x2 = XEXP (x1, 0);
  goto L5954;

 L51: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L52;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5954;

 L52: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 3;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5954;

 L19: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, DImode))
    {
      operands[2] = x3;
      goto L20;
    }
  x2 = XEXP (x1, 0);
  goto L5954;

 L20: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L21;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5954;

 L21: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 1;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5954;

 L254: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == DImode)
    goto L5966;
  x2 = XEXP (x1, 0);
  goto L5954;

 L5966: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x3))
    {
    case ZERO_EXTEND:
      goto L315;
    case SIGN_EXTEND:
      goto L331;
    case SUBREG:
    case REG:
      goto L5965;
    default:
      x2 = XEXP (x1, 0);
      goto L5954;
   }
 L5965: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x3, DImode))
    {
      operands[1] = x3;
      goto L255;
    }
  x2 = XEXP (x1, 0);
  goto L5954;

 L315: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode)
    goto L5968;
  x2 = XEXP (x1, 0);
  goto L5954;

 L5968: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L316;
    }
 L5969: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L348;
    }
  x2 = XEXP (x1, 0);
  goto L5954;

 L316: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, DImode))
    {
      operands[1] = x3;
      goto L317;
    }
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L5969;

 L317: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L318;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L5969;

 L318: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 30;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L5969;

 L348: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == DImode
      && GET_CODE (x3) == ZERO_EXTEND)
    goto L349;
  x2 = XEXP (x1, 0);
  goto L5954;

 L349: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L350;
    }
  x2 = XEXP (x1, 0);
  goto L5954;

 L350: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L351;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5954;

 L351: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 32;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5954;

 L331: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L332;
    }
  x2 = XEXP (x1, 0);
  goto L5954;

 L332: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, DImode))
    {
      operands[1] = x3;
      goto L333;
    }
  x2 = XEXP (x1, 0);
  goto L5954;

 L333: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L334;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5954;

 L334: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 31;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5954;

 L255: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == DImode)
    goto L5971;
  x2 = XEXP (x1, 0);
  goto L5954;

 L5971: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x3))
    {
    case ZERO_EXTEND:
      goto L284;
    case SIGN_EXTEND:
      goto L300;
    case SUBREG:
    case REG:
      goto L5970;
    default:
      x2 = XEXP (x1, 0);
      goto L5954;
   }
 L5970: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x3, DImode))
    {
      operands[2] = x3;
      goto L256;
    }
  x2 = XEXP (x1, 0);
  goto L5954;

 L284: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L285;
    }
  x2 = XEXP (x1, 0);
  goto L5954;

 L285: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L286;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5954;

 L286: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 28;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5954;

 L300: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L301;
    }
  x2 = XEXP (x1, 0);
  goto L5954;

 L301: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L302;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5954;

 L302: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 29;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5954;

 L256: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L257;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5954;

 L257: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 26;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5954;

 L1233: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, DImode))
    {
      operands[1] = x3;
      goto L1234;
    }
  x2 = XEXP (x1, 0);
  goto L5954;

 L1234: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L1235;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5954;

 L1235: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 125;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5954;

 L267: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == DImode)
    goto L5973;
  goto ret0;

 L5973: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x2))
    {
    case MINUS:
      goto L268;
    case NEG:
      goto L1245;
    default:
     break;
   }
  goto ret0;

 L268: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (register_operand (x3, DImode))
    {
      operands[1] = x3;
      goto L269;
    }
  goto ret0;

 L269: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (register_operand (x3, DImode))
    {
      operands[2] = x3;
      goto L270;
    }
  goto ret0;

 L270: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L271;
  goto ret0;

 L271: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_THUMB))
    {
      return 27;
    }
  goto ret0;

 L1245: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (register_operand (x3, DImode))
    {
      operands[1] = x3;
      goto L1246;
    }
  goto ret0;

 L1246: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L1247;
  goto ret0;

 L1247: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_THUMB))
    {
      return 126;
    }
  goto ret0;

 L5951: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == REG
      && XINT (x2, 0) == 24)
    goto L75;
  goto ret0;

 L75: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == CC_NOOVmode
      && GET_CODE (x2) == COMPARE)
    goto L76;
  goto ret0;

 L76: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode)
    goto L5975;
  goto ret0;

 L5975: ATTRIBUTE_UNUSED_LABEL
  return recog_1 (x0, insn, pnum_clobbers);

 L5952: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == REG
      && XINT (x2, 0) == 24)
    goto L95;
  goto ret0;

 L95: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == COMPARE)
    goto L96;
  goto ret0;

 L96: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L97;
    }
  goto ret0;

 L97: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == NEG)
    goto L98;
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L1595;
  goto ret0;

 L98: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (arm_add_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L99;
    }
  goto ret0;

 L99: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L100;
  goto ret0;

 L100: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L101;
    }
  goto ret0;

 L101: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L102;
  goto ret0;

 L102: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[1]))
    goto L103;
  goto ret0;

 L103: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[2])
      && (TARGET_ARM))
    {
      return 8;
    }
  goto ret0;

 L1595: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L1596;
  goto ret0;

 L1596: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L1597;
    }
  goto ret0;

 L1597: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (rtx_equal_p (x2, operands[1])
      && (TARGET_ARM))
    {
      return 183;
    }
  goto ret0;

 L5953: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == REG
      && XINT (x2, 0) == 24)
    goto L114;
  goto ret0;

 L114: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == CC_Cmode
      && GET_CODE (x2) == COMPARE)
    goto L115;
  goto ret0;

 L115: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L116;
  goto ret0;

 L116: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L117;
    }
  goto ret0;

 L117: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_add_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L118;
    }
  goto ret0;

 L118: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[1]))
    goto L119;
 L131: ATTRIBUTE_UNUSED_LABEL
  if (rtx_equal_p (x3, operands[2]))
    goto L132;
  goto ret0;

 L119: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L120;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L131;

 L120: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L121;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L131;

 L121: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L122;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L131;

 L122: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[1]))
    goto L123;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L131;

 L123: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[2])
      && (TARGET_ARM))
    {
      return 10;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L131;

 L132: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L133;
  goto ret0;

 L133: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L134;
    }
  goto ret0;

 L134: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L135;
  goto ret0;

 L135: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[1]))
    goto L136;
  goto ret0;

 L136: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[2])
      && (TARGET_ARM))
    {
      return 11;
    }
  goto ret0;

 L5955: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L561;
    }
 L5956: ATTRIBUTE_UNUSED_LABEL
  if (memory_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L1097;
    }
 L5957: ATTRIBUTE_UNUSED_LABEL
  if (register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L1482;
    }
  goto ret0;

 L561: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode)
    goto L5990;
  x2 = XEXP (x1, 0);
  goto L5956;

 L5990: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x2))
    {
    case TRUNCATE:
      goto L562;
    case SMAX:
      goto L1042;
    case SMIN:
      goto L1056;
    case UMAX:
      goto L1070;
    case UMIN:
      goto L1084;
    case ABS:
      goto L1288;
    case NEG:
      goto L1300;
    case PLUS:
    case MINUS:
    case AND:
    case IOR:
    case XOR:
      goto L5995;
    default:
      x2 = XEXP (x1, 0);
      goto L5956;
   }
 L5995: ATTRIBUTE_UNUSED_LABEL
  if (shiftable_operator (x2, SImode))
    {
      operands[4] = x2;
      goto L1112;
    }
  x2 = XEXP (x1, 0);
  goto L5956;

 L562: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == DImode
      && GET_CODE (x3) == LSHIFTRT)
    goto L563;
  x2 = XEXP (x1, 0);
  goto L5956;

 L563: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == DImode
      && GET_CODE (x4) == MULT)
    goto L564;
  x2 = XEXP (x1, 0);
  goto L5956;

 L564: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (GET_MODE (x5) == DImode)
    goto L5998;
  x2 = XEXP (x1, 0);
  goto L5956;

 L5998: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x5))
    {
    case SIGN_EXTEND:
      goto L565;
    case ZERO_EXTEND:
      goto L589;
    default:
     break;
   }
  x2 = XEXP (x1, 0);
  goto L5956;

 L565: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 0);
  if (s_register_operand (x6, SImode))
    {
      operands[1] = x6;
      goto L566;
    }
  x2 = XEXP (x1, 0);
  goto L5956;

 L566: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_MODE (x5) == DImode
      && GET_CODE (x5) == SIGN_EXTEND)
    goto L567;
  x2 = XEXP (x1, 0);
  goto L5956;

 L567: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 0);
  if (s_register_operand (x6, SImode))
    {
      operands[2] = x6;
      goto L568;
    }
  x2 = XEXP (x1, 0);
  goto L5956;

 L568: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 32)
    goto L569;
  x2 = XEXP (x1, 0);
  goto L5956;

 L569: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L570;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5956;

 L570: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      operands[3] = x2;
      goto L571;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5956;

 L571: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && arm_fast_multiply))
    {
      return 54;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5956;

 L589: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 0);
  if (s_register_operand (x6, SImode))
    {
      operands[1] = x6;
      goto L590;
    }
  x2 = XEXP (x1, 0);
  goto L5956;

 L590: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_MODE (x5) == DImode
      && GET_CODE (x5) == ZERO_EXTEND)
    goto L591;
  x2 = XEXP (x1, 0);
  goto L5956;

 L591: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 0);
  if (s_register_operand (x6, SImode))
    {
      operands[2] = x6;
      goto L592;
    }
  x2 = XEXP (x1, 0);
  goto L5956;

 L592: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 32)
    goto L593;
  x2 = XEXP (x1, 0);
  goto L5956;

 L593: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L594;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5956;

 L594: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      operands[3] = x2;
      goto L595;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5956;

 L595: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && arm_fast_multiply))
    {
      return 55;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5956;

 L1042: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L1043;
    }
  x2 = XEXP (x1, 0);
  goto L5956;

 L1043: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L1044;
    }
  x2 = XEXP (x1, 0);
  goto L5956;

 L1044: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L1045;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5956;

 L1045: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 109;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5956;

 L1056: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L1057;
    }
  x2 = XEXP (x1, 0);
  goto L5956;

 L1057: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L1058;
    }
  x2 = XEXP (x1, 0);
  goto L5956;

 L1058: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L1059;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5956;

 L1059: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 110;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5956;

 L1070: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L1071;
    }
  x2 = XEXP (x1, 0);
  goto L5956;

 L1071: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L1072;
    }
  x2 = XEXP (x1, 0);
  goto L5956;

 L1072: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L1073;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5956;

 L1073: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 111;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5956;

 L1084: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L1085;
    }
  x2 = XEXP (x1, 0);
  goto L5956;

 L1085: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L1086;
    }
  x2 = XEXP (x1, 0);
  goto L5956;

 L1086: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L1087;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5956;

 L1087: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 112;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5956;

 L1288: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L1289;
    }
  x2 = XEXP (x1, 0);
  goto L5956;

 L1289: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L1290;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5956;

 L1290: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 133;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5956;

 L1300: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == ABS)
    goto L1301;
  x2 = XEXP (x1, 0);
  goto L5956;

 L1301: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L1302;
    }
  x2 = XEXP (x1, 0);
  goto L5956;

 L1302: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L1303;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5956;

 L1303: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 134;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5956;

 L1112: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (minmax_operator (x3, SImode))
    {
      operands[5] = x3;
      goto L1113;
    }
  x2 = XEXP (x1, 0);
  goto L5956;

 L1113: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L1114;
    }
  x2 = XEXP (x1, 0);
  goto L5956;

 L1114: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      operands[3] = x4;
      goto L1115;
    }
  x2 = XEXP (x1, 0);
  goto L5956;

 L1115: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L1116;
    }
  x2 = XEXP (x1, 0);
  goto L5956;

 L1116: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L1117;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5956;

 L1117: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM
   && (GET_CODE (operands[1]) != REG
       || (REGNO(operands[1]) != FRAME_POINTER_REGNUM
           && REGNO(operands[1]) != ARG_POINTER_REGNUM))))
    {
      return 114;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5956;

 L1097: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (minmax_operator (x2, SImode))
    {
      operands[3] = x2;
      goto L1098;
    }
  x2 = XEXP (x1, 0);
  goto L5957;

 L1098: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L1099;
    }
  x2 = XEXP (x1, 0);
  goto L5957;

 L1099: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L1100;
    }
  x2 = XEXP (x1, 0);
  goto L5957;

 L1100: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L1101;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5957;

 L1101: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 113;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L5957;

 L1482: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode)
    goto L6000;
  goto ret0;

 L6000: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x2))
    {
    case SIGN_EXTEND:
      goto L1483;
    case PLUS:
      goto L1567;
    default:
     break;
   }
  goto ret0;

 L1483: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (memory_operand (x3, HImode))
    {
      operands[1] = x3;
      goto L1484;
    }
  goto ret0;

 L1484: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L1485;
  goto ret0;

 L1485: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L1486;
    }
  goto ret0;

 L1486: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB))
    {
      return 165;
    }
  goto ret0;

 L1567: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[0]))
    goto L1568;
  goto ret0;

 L1568: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST)
    goto L1569;
  goto ret0;

 L1569: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode
      && GET_CODE (x4) == PLUS)
    goto L1570;
  goto ret0;

 L1570: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (GET_CODE (x5) == PC)
    goto L1571;
  goto ret0;

 L1571: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT)
    goto L6002;
  goto ret0;

 L6002: ATTRIBUTE_UNUSED_LABEL
  if ((int) XWINT (x5, 0) == XWINT (x5, 0))
    switch ((int) XWINT (x5, 0))
      {
      case 4:
        goto L1572;
      case 8:
        goto L1585;
      default:
        break;
      }
  goto ret0;

 L1572: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == USE)
    goto L1573;
  goto ret0;

 L1573: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) == LABEL_REF)
    goto L1574;
  goto ret0;

 L1574: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  operands[1] = x3;
  goto L1575;

 L1575: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB && flag_pic))
    {
      return 181;
    }
  goto ret0;

 L1585: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == USE)
    goto L1586;
  goto ret0;

 L1586: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) == LABEL_REF)
    goto L1587;
  goto ret0;

 L1587: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  operands[1] = x3;
  goto L1588;

 L1588: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && flag_pic))
    {
      return 182;
    }
  goto ret0;

 L5958: ATTRIBUTE_UNUSED_LABEL
  if (memory_operand (x2, HImode))
    {
      operands[0] = x2;
      goto L1632;
    }
  goto ret0;

 L1632: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (register_operand (x2, HImode))
    {
      operands[1] = x2;
      goto L1633;
    }
  goto ret0;

 L1633: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L1634;
  goto ret0;

 L1634: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L1635;
    }
  goto ret0;

 L1635: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB))
    {
      return 191;
    }
  goto ret0;
 ret0:
  return -1;
}

static int recog_3 PARAMS ((rtx, rtx, int *));
static int
recog_3 (x0, insn, pnum_clobbers)
     rtx x0 ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  rtx * const operands ATTRIBUTE_UNUSED = &recog_data.operand[0];
  rtx x1 ATTRIBUTE_UNUSED;
  rtx x2 ATTRIBUTE_UNUSED;
  rtx x3 ATTRIBUTE_UNUSED;
  rtx x4 ATTRIBUTE_UNUSED;
  rtx x5 ATTRIBUTE_UNUSED;
  rtx x6 ATTRIBUTE_UNUSED;
  rtx x7 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;

  x1 = XVECEXP (x0, 0, 0);
  switch (GET_CODE (x1))
    {
    case CALL:
      goto L2281;
    case SET:
      goto L2332;
    default:
     break;
   }
  goto ret0;

 L2281: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L2282;
  goto ret0;

 L2282: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode)
    goto L6015;
 L2396: ATTRIBUTE_UNUSED_LABEL
  operands[0] = x3;
  goto L2397;

 L6015: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L2283;
    }
 L6016: ATTRIBUTE_UNUSED_LABEL
  if (memory_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L2300;
    }
 L6017: ATTRIBUTE_UNUSED_LABEL
  if (register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L2317;
    }
  goto L2396;

 L2283: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  operands[1] = x2;
  goto L2284;

 L2284: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == USE)
    goto L2285;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6016;

 L2285: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  operands[2] = x2;
  goto L2286;

 L2286: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 2);
  if (GET_CODE (x1) == CLOBBER)
    goto L2287;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6016;

 L2287: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 14
      && (TARGET_ARM))
    {
      return 247;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6016;

 L2300: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  operands[1] = x2;
  goto L2301;

 L2301: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == USE)
    goto L2302;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6017;

 L2302: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  operands[2] = x2;
  goto L2303;

 L2303: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 2);
  if (GET_CODE (x1) == CLOBBER)
    goto L2304;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6017;

 L2304: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 14
      && (TARGET_ARM))
    {
      return 248;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6017;

 L2317: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  operands[1] = x2;
  goto L2318;

 L2318: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == USE)
    goto L2319;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2396;

 L2319: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  operands[2] = x2;
  goto L2320;

 L2320: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 2);
  if (GET_CODE (x1) == CLOBBER)
    goto L2321;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2396;

 L2321: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 14
      && (TARGET_THUMB))
    {
      return 249;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2396;

 L2397: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  operands[1] = x2;
  goto L2398;

 L2398: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == USE)
    goto L2399;
  goto ret0;

 L2399: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  operands[2] = x2;
  goto L2400;

 L2400: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 2);
  switch (GET_CODE (x1))
    {
    case CLOBBER:
      goto L2401;
    case USE:
      goto L2477;
    default:
     break;
   }
  goto ret0;

 L2401: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode)
    goto L6018;
  goto ret0;

 L6018: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == REG)
    goto L6020;
  goto ret0;

 L6020: ATTRIBUTE_UNUSED_LABEL
  if (XINT (x2, 0) == 14)
    goto L6022;
  goto ret0;

 L6022: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM
   && (GET_CODE (operands[0]) == SYMBOL_REF)
   && !arm_is_longcall_p (operands[0], INTVAL (operands[2]), 1)))
    {
      return 253;
    }
 L6023: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB
   && operands[2] == const0_rtx && (GET_CODE (operands[0]) == SYMBOL_REF)))
    {
      return 255;
    }
  goto ret0;

 L2477: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 14
      && (TARGET_ARM && GET_CODE (operands[0]) == SYMBOL_REF))
    {
      return 257;
    }
  goto ret0;

 L2332: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  operands[0] = x2;
  goto L2333;
 L2510: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == PC)
    goto L2511;
  if (s_register_operand (x2, VOIDmode))
    {
      operands[0] = x2;
      goto L2413;
    }
 L2450: ATTRIBUTE_UNUSED_LABEL
  if (register_operand (x2, VOIDmode))
    {
      operands[0] = x2;
      goto L2451;
    }
 L2480: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, VOIDmode))
    {
      operands[0] = x2;
      goto L2481;
    }
  goto ret0;

 L2333: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CALL)
    goto L2334;
  x2 = XEXP (x1, 0);
  goto L2510;

 L2334: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L2335;
  x2 = XEXP (x1, 0);
  goto L2510;

 L2335: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode)
    goto L6024;
  x2 = XEXP (x1, 0);
  goto L2510;

 L6024: ATTRIBUTE_UNUSED_LABEL
  if (register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L2336;
    }
 L6025: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L2357;
    }
 L6026: ATTRIBUTE_UNUSED_LABEL
  if (memory_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L2378;
    }
  x2 = XEXP (x1, 0);
  goto L2510;

 L2336: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  operands[2] = x3;
  goto L2337;

 L2337: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == USE)
    goto L2338;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L6025;

 L2338: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  operands[3] = x2;
  goto L2339;

 L2339: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 2);
  if (GET_CODE (x1) == CLOBBER)
    goto L2340;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L6025;

 L2340: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 14
      && (TARGET_THUMB))
    {
      return 250;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L6025;

 L2357: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  operands[2] = x3;
  goto L2358;

 L2358: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == USE)
    goto L2359;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L6026;

 L2359: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  operands[3] = x2;
  goto L2360;

 L2360: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 2);
  if (GET_CODE (x1) == CLOBBER)
    goto L2361;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L6026;

 L2361: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 14
      && (TARGET_ARM))
    {
      return 251;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L6026;

 L2378: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  operands[2] = x3;
  goto L2379;

 L2379: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == USE)
    goto L2380;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2510;

 L2380: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  operands[3] = x2;
  goto L2381;

 L2381: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 2);
  if (GET_CODE (x1) == CLOBBER)
    goto L2382;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2510;

 L2382: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 14
      && (TARGET_ARM && (!CONSTANT_ADDRESS_P (XEXP (operands[1], 0)))))
    {
      return 252;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2510;

 L2511: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == IF_THEN_ELSE)
    goto L2512;
  goto ret0;

 L2512: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_CODE (x3) == LEU)
    goto L2513;
  goto ret0;

 L2513: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[0] = x4;
      goto L2514;
    }
  goto ret0;

 L2514: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L2515;
    }
  goto ret0;

 L2515: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L2516;
  goto ret0;

 L2516: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode
      && GET_CODE (x4) == PLUS)
    goto L2517;
  goto ret0;

 L2517: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (GET_MODE (x5) == SImode
      && GET_CODE (x5) == MULT)
    goto L2518;
  goto ret0;

 L2518: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 0);
  if (rtx_equal_p (x6, operands[0]))
    goto L2519;
  goto ret0;

 L2519: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 1);
  if (GET_CODE (x6) == CONST_INT
      && XWINT (x6, 0) == 4)
    goto L2520;
  goto ret0;

 L2520: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == LABEL_REF)
    goto L2521;
  goto ret0;

 L2521: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 0);
  operands[2] = x6;
  goto L2522;

 L2522: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (GET_CODE (x3) == LABEL_REF)
    goto L2523;
  goto ret0;

 L2523: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  operands[3] = x4;
  goto L2524;

 L2524: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L2525;
  goto ret0;

 L2525: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24)
    goto L2526;
  goto ret0;

 L2526: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 2);
  if (GET_CODE (x1) == USE)
    goto L2527;
  goto ret0;

 L2527: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) == LABEL_REF)
    goto L2528;
  goto ret0;

 L2528: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[2])
      && (TARGET_ARM))
    {
      return 263;
    }
  goto ret0;

 L2413: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CALL)
    goto L2414;
  x2 = XEXP (x1, 0);
  goto L2450;

 L2414: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L2415;
  x2 = XEXP (x1, 0);
  goto L2450;

 L2415: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  operands[1] = x4;
  goto L2416;

 L2416: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  operands[2] = x3;
  goto L2417;

 L2417: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == USE)
    goto L2418;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2450;

 L2418: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  operands[3] = x2;
  goto L2419;

 L2419: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 2);
  if (GET_CODE (x1) == CLOBBER)
    goto L2420;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2450;

 L2420: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 14
      && (TARGET_ARM
   && (GET_CODE (operands[1]) == SYMBOL_REF)
   && !arm_is_longcall_p (operands[1], INTVAL (operands[3]), 1)))
    {
      return 254;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2450;

 L2451: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CALL)
    goto L2452;
  x2 = XEXP (x1, 0);
  goto L2480;

 L2452: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L2453;
  x2 = XEXP (x1, 0);
  goto L2480;

 L2453: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  operands[1] = x4;
  goto L2454;

 L2454: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  operands[2] = x3;
  goto L2455;

 L2455: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == USE)
    goto L2456;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2480;

 L2456: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  operands[3] = x2;
  goto L2457;

 L2457: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 2);
  if (GET_CODE (x1) == CLOBBER)
    goto L2458;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2480;

 L2458: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 14
      && (TARGET_THUMB
   && operands[3] == const0_rtx && (GET_CODE (operands[1]) == SYMBOL_REF)))
    {
      return 256;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2480;

 L2481: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CALL)
    goto L2482;
  goto ret0;

 L2482: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L2483;
  goto ret0;

 L2483: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  operands[1] = x4;
  goto L2484;

 L2484: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  operands[2] = x3;
  goto L2485;

 L2485: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == USE)
    goto L2486;
  goto ret0;

 L2486: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  operands[3] = x2;
  goto L2487;

 L2487: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 2);
  if (GET_CODE (x1) == USE)
    goto L2488;
  goto ret0;

 L2488: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 14
      && (TARGET_ARM && GET_CODE (operands[1]) == SYMBOL_REF))
    {
      return 258;
    }
  goto ret0;
 ret0:
  return -1;
}

static int recog_4 PARAMS ((rtx, rtx, int *));
static int
recog_4 (x0, insn, pnum_clobbers)
     rtx x0 ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  rtx * const operands ATTRIBUTE_UNUSED = &recog_data.operand[0];
  rtx x1 ATTRIBUTE_UNUSED;
  rtx x2 ATTRIBUTE_UNUSED;
  rtx x3 ATTRIBUTE_UNUSED;
  rtx x4 ATTRIBUTE_UNUSED;
  rtx x5 ATTRIBUTE_UNUSED;
  rtx x6 ATTRIBUTE_UNUSED;
  rtx x7 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;

  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  if (arm_comparison_operator (x3, VOIDmode))
    {
      operands[5] = x3;
      goto L2894;
    }
 L2913: ATTRIBUTE_UNUSED_LABEL
  if (arm_comparison_operator (x3, VOIDmode))
    {
      operands[6] = x3;
      goto L2914;
    }
 L2983: ATTRIBUTE_UNUSED_LABEL
  if (arm_comparison_operator (x3, VOIDmode))
    {
      operands[9] = x3;
      goto L2984;
    }
 L3228: ATTRIBUTE_UNUSED_LABEL
  if (arm_comparison_operator (x3, VOIDmode))
    {
      operands[7] = x3;
      goto L3229;
    }
  goto ret0;

 L2894: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[3] = x4;
      goto L2895;
    }
  goto L2913;

 L2895: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_add_operand (x4, SImode))
    {
      operands[4] = x4;
      goto L3129;
    }
  goto L2913;

 L3129: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode)
    goto L6057;
 L2896: ATTRIBUTE_UNUSED_LABEL
  if (arm_rhs_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L2897;
    }
 L3097: ATTRIBUTE_UNUSED_LABEL
  if (arm_not_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L3098;
    }
  x3 = XEXP (x2, 0);
  goto L2913;

 L6057: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x3))
    {
    case NOT:
      goto L3130;
    case NEG:
      goto L3349;
    default:
     break;
   }
  goto L2896;

 L3130: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L3131;
    }
  goto L2896;

 L3131: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (arm_not_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L3132;
    }
  x3 = XEXP (x2, 1);
  goto L2896;

 L3132: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L3133;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L2896;

 L3133: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 304;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L2896;

 L3349: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L3350;
    }
  goto L2896;

 L3350: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (arm_not_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L3351;
    }
  x3 = XEXP (x2, 1);
  goto L2896;

 L3351: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L3352;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L2896;

 L3352: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 316;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L2896;

 L2897: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L2898;
    }
  x3 = XEXP (x2, 1);
  goto L3097;

 L2898: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L2899;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L3097;

 L2899: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 291;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L3097;

 L3098: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (GET_MODE (x3) == SImode)
    goto L6059;
  x3 = XEXP (x2, 0);
  goto L2913;

 L6059: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x3))
    {
    case NOT:
      goto L3099;
    case NEG:
      goto L3382;
    default:
     break;
   }
  x3 = XEXP (x2, 0);
  goto L2913;

 L3099: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L3100;
    }
  x3 = XEXP (x2, 0);
  goto L2913;

 L3100: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L3101;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L2913;

 L3101: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 302;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L2913;

 L3382: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L3383;
    }
  x3 = XEXP (x2, 0);
  goto L2913;

 L3383: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L3384;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L2913;

 L3384: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 318;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L2913;

 L2914: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode)
    goto L6061;
  goto L2983;

 L6061: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x4, SImode))
    {
      operands[4] = x4;
      goto L2915;
    }
 L6062: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L3026;
    }
  goto L2983;

 L2915: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_add_operand (x4, SImode))
    {
      operands[5] = x4;
      goto L2916;
    }
  x4 = XEXP (x3, 0);
  goto L6062;

 L2916: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode)
    goto L6063;
 L2951: ATTRIBUTE_UNUSED_LABEL
  if (arm_rhs_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L2952;
    }
 L3196: ATTRIBUTE_UNUSED_LABEL
  if (arm_not_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L3197;
    }
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L6062;

 L6063: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x3))
    {
    case PLUS:
      goto L2917;
    case NOT:
      goto L3273;
    case ASHIFT:
    case ASHIFTRT:
    case LSHIFTRT:
    case ROTATERT:
    case MULT:
      goto L6064;
    default:
      goto L6066;
   }
 L6064: ATTRIBUTE_UNUSED_LABEL
  if (shift_operator (x3, SImode))
    {
      operands[7] = x3;
      goto L3162;
    }
 L6066: ATTRIBUTE_UNUSED_LABEL
  if (shiftable_operator (x3, SImode))
    {
      operands[7] = x3;
      goto L3311;
    }
  goto L2951;

 L2917: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L2918;
    }
  goto L6066;

 L2918: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_add_operand (x4, SImode))
    {
      operands[3] = x4;
      goto L2919;
    }
  goto L6066;

 L2919: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L2920;
    }
  x3 = XEXP (x2, 1);
  goto L6066;

 L2920: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L2921;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L6066;

 L2921: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 292;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L6066;

 L3273: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L3274;
    }
  goto L2951;

 L3274: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (shiftable_operator (x3, SImode))
    {
      operands[7] = x3;
      goto L3275;
    }
  x3 = XEXP (x2, 1);
  goto L2951;

 L3275: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L3276;
    }
  x3 = XEXP (x2, 1);
  goto L2951;

 L3276: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      operands[3] = x4;
      goto L3277;
    }
  x3 = XEXP (x2, 1);
  goto L2951;

 L3277: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L3278;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L2951;

 L3278: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 312;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L2951;

 L3162: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L3163;
    }
  goto L2951;

 L3163: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      operands[3] = x4;
      goto L3164;
    }
  goto L2951;

 L3164: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (arm_not_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L3165;
    }
  x3 = XEXP (x2, 1);
  goto L2951;

 L3165: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L3166;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L2951;

 L3166: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 306;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L2951;

 L3311: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L3312;
    }
  goto L2951;

 L3312: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      operands[3] = x4;
      goto L3313;
    }
  goto L2951;

 L3313: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == NOT)
    goto L3314;
  x3 = XEXP (x2, 1);
  goto L2951;

 L3314: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L3315;
    }
  x3 = XEXP (x2, 1);
  goto L2951;

 L3315: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L3316;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L2951;

 L3316: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 314;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L2951;

 L2952: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (GET_MODE (x3) == SImode)
    goto L6067;
  x3 = XEXP (x2, 1);
  goto L3196;

 L6067: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == PLUS)
    goto L2953;
 L6068: ATTRIBUTE_UNUSED_LABEL
  if (shiftable_operator (x3, SImode))
    {
      operands[7] = x3;
      goto L3064;
    }
  x3 = XEXP (x2, 1);
  goto L3196;

 L2953: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L2954;
    }
  goto L6068;

 L2954: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_add_operand (x4, SImode))
    {
      operands[3] = x4;
      goto L2955;
    }
  goto L6068;

 L2955: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L2956;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 2);
  goto L6068;

 L2956: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 294;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 2);
  goto L6068;

 L3064: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L3065;
    }
  x3 = XEXP (x2, 1);
  goto L3196;

 L3065: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      operands[3] = x4;
      goto L3066;
    }
  x3 = XEXP (x2, 1);
  goto L3196;

 L3066: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L3067;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L3196;

 L3067: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 300;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L3196;

 L3197: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (shift_operator (x3, SImode))
    {
      operands[7] = x3;
      goto L3198;
    }
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L6062;

 L3198: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L3199;
    }
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L6062;

 L3199: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      operands[3] = x4;
      goto L3200;
    }
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L6062;

 L3200: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L3201;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L6062;

 L3201: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 308;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L6062;

 L3026: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_add_operand (x4, SImode))
    {
      operands[3] = x4;
      goto L3027;
    }
  goto L2983;

 L3027: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (shiftable_operator (x3, SImode))
    {
      operands[7] = x3;
      goto L3028;
    }
  x3 = XEXP (x2, 0);
  goto L2983;

 L3028: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[4] = x4;
      goto L3029;
    }
  x3 = XEXP (x2, 0);
  goto L2983;

 L3029: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      operands[5] = x4;
      goto L3030;
    }
  x3 = XEXP (x2, 0);
  goto L2983;

 L3030: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L3031;
    }
  x3 = XEXP (x2, 0);
  goto L2983;

 L3031: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L3032;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L2983;

 L3032: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 298;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L2983;

 L2984: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[5] = x4;
      goto L2985;
    }
  goto L3228;

 L2985: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_add_operand (x4, SImode))
    {
      operands[6] = x4;
      goto L2986;
    }
  goto L3228;

 L2986: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (shiftable_operator (x3, SImode))
    {
      operands[8] = x3;
      goto L2987;
    }
  x3 = XEXP (x2, 0);
  goto L3228;

 L2987: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L2988;
    }
  x3 = XEXP (x2, 0);
  goto L3228;

 L2988: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L2989;
    }
  x3 = XEXP (x2, 0);
  goto L3228;

 L2989: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (shiftable_operator (x3, SImode))
    {
      operands[7] = x3;
      goto L2990;
    }
  x3 = XEXP (x2, 0);
  goto L3228;

 L2990: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[3] = x4;
      goto L2991;
    }
  x3 = XEXP (x2, 0);
  goto L3228;

 L2991: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      operands[4] = x4;
      goto L2992;
    }
  x3 = XEXP (x2, 0);
  goto L3228;

 L2992: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L2993;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L3228;

 L2993: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 296;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L3228;

 L3229: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[5] = x4;
      goto L3230;
    }
  goto ret0;

 L3230: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_add_operand (x4, SImode))
    {
      operands[6] = x4;
      goto L3231;
    }
  goto ret0;

 L3231: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (shift_operator (x3, SImode))
    {
      operands[8] = x3;
      goto L3232;
    }
  goto ret0;

 L3232: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L3233;
    }
  goto ret0;

 L3233: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L3234;
    }
  goto ret0;

 L3234: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (shift_operator (x3, SImode))
    {
      operands[9] = x3;
      goto L3235;
    }
  goto ret0;

 L3235: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[3] = x4;
      goto L3236;
    }
  goto ret0;

 L3236: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      operands[4] = x4;
      goto L3237;
    }
  goto ret0;

 L3237: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L3238;
  goto ret0;

 L3238: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 310;
    }
  goto ret0;
 ret0:
  return -1;
}

static int recog_5 PARAMS ((rtx, rtx, int *));
static int
recog_5 (x0, insn, pnum_clobbers)
     rtx x0 ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  rtx * const operands ATTRIBUTE_UNUSED = &recog_data.operand[0];
  rtx x1 ATTRIBUTE_UNUSED;
  rtx x2 ATTRIBUTE_UNUSED;
  rtx x3 ATTRIBUTE_UNUSED;
  rtx x4 ATTRIBUTE_UNUSED;
  rtx x5 ATTRIBUTE_UNUSED;
  rtx x6 ATTRIBUTE_UNUSED;
  rtx x7 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;

  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) == MEM)
    goto L3497;
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L2769;
    }
 L6031: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, SImode))
    {
      operands[3] = x2;
      goto L3471;
    }
 L6035: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, SImode))
    {
      operands[5] = x2;
      goto L3665;
    }
  goto ret0;

 L3497: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode)
    goto L6045;
  goto ret0;

 L6045: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x3))
    {
    case PLUS:
      goto L3498;
    case MINUS:
      goto L3510;
    default:
     break;
   }
  goto ret0;

 L3498: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode)
    goto L6047;
  goto ret0;

 L6047: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L3499;
    }
  if (shift_operator (x4, SImode))
    {
      operands[2] = x4;
      goto L3635;
    }
  goto ret0;

 L3499: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (index_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L3500;
    }
  goto ret0;

 L3500: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      operands[3] = x2;
      goto L3501;
    }
  goto ret0;

 L3501: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L3502;
  goto ret0;

 L3502: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L3503;
    }
  goto ret0;

 L3503: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L3504;
  goto ret0;

 L3504: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[1]))
    goto L3505;
  goto ret0;

 L3505: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[2])
      && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM)))
    {
      return 327;
    }
  goto ret0;

 L3635: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[3] = x5;
      goto L3636;
    }
  goto ret0;

 L3636: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (const_shift_operand (x5, SImode))
    {
      operands[4] = x5;
      goto L3637;
    }
  goto ret0;

 L3637: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L3638;
    }
  goto ret0;

 L3638: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      operands[5] = x2;
      goto L3639;
    }
  goto ret0;

 L3639: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L3640;
  goto ret0;

 L3640: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L3641;
    }
  goto ret0;

 L3641: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L3642;
  goto ret0;

 L3642: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[2]))
    {
      operands[2] = x3;
      goto L3643;
    }
  goto ret0;

 L3643: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[3]))
    goto L3644;
  goto ret0;

 L3644: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[4]))
    goto L3645;
  goto ret0;

 L3645: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[1])
      && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && REGNO (operands[3]) != FRAME_POINTER_REGNUM))
    {
      return 337;
    }
  goto ret0;

 L3510: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L3511;
    }
  goto ret0;

 L3511: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_MODE (x4) == SImode)
    goto L6049;
  goto ret0;

 L6049: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L3512;
    }
  if (shift_operator (x4, SImode))
    {
      operands[2] = x4;
      goto L3652;
    }
  goto ret0;

 L3512: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      operands[3] = x2;
      goto L3513;
    }
  goto ret0;

 L3513: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L3514;
  goto ret0;

 L3514: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L3515;
    }
  goto ret0;

 L3515: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MINUS)
    goto L3516;
  goto ret0;

 L3516: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[1]))
    goto L3517;
  goto ret0;

 L3517: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[2])
      && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM)))
    {
      return 328;
    }
  goto ret0;

 L3652: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[3] = x5;
      goto L3653;
    }
  goto ret0;

 L3653: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (const_shift_operand (x5, SImode))
    {
      operands[4] = x5;
      goto L3654;
    }
  goto ret0;

 L3654: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      operands[5] = x2;
      goto L3655;
    }
  goto ret0;

 L3655: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L3656;
  goto ret0;

 L3656: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L3657;
    }
  goto ret0;

 L3657: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MINUS)
    goto L3658;
  goto ret0;

 L3658: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[1]))
    goto L3659;
  goto ret0;

 L3659: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[2]))
    {
      operands[2] = x3;
      goto L3660;
    }
  goto ret0;

 L3660: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[3]))
    goto L3661;
  goto ret0;

 L3661: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[4])
      && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && REGNO (operands[3]) != FRAME_POINTER_REGNUM))
    {
      return 338;
    }
  goto ret0;

 L2769: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode)
    goto L6053;
  x2 = XEXP (x1, 0);
  goto L6031;

 L6053: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x2))
    {
    case MINUS:
      goto L2813;
    case NEG:
      goto L2877;
    case IF_THEN_ELSE:
      goto L2893;
    case EQ:
    case NE:
    case LE:
    case LT:
    case GE:
    case GT:
    case GEU:
    case GTU:
    case LEU:
    case LTU:
    case UNORDERED:
    case ORDERED:
    case UNLT:
    case UNLE:
    case UNGE:
    case UNGT:
      goto L6051;
    default:
      goto L6052;
   }
 L6051: ATTRIBUTE_UNUSED_LABEL
  if (arm_comparison_operator (x2, SImode))
    {
      operands[1] = x2;
      goto L2770;
    }
 L6052: ATTRIBUTE_UNUSED_LABEL
  if (shiftable_operator (x2, SImode))
    {
      operands[5] = x2;
      goto L2795;
    }
 L6056: ATTRIBUTE_UNUSED_LABEL
  if (shiftable_operator (x2, SImode))
    {
      operands[1] = x2;
      goto L3409;
    }
  x2 = XEXP (x1, 0);
  goto L6031;

 L2813: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L2814;
    }
  goto L6052;

 L2814: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_comparison_operator (x3, SImode))
    {
      operands[4] = x3;
      goto L2815;
    }
  goto L6052;

 L2815: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L2816;
    }
  goto L6052;

 L2816: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      operands[3] = x4;
      goto L2817;
    }
  goto L6052;

 L2817: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L2818;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L6052;

 L2818: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 285;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L6052;

 L2877: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (arm_comparison_operator (x3, VOIDmode))
    {
      operands[3] = x3;
      goto L2878;
    }
  x2 = XEXP (x1, 0);
  goto L6031;

 L2878: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L2879;
    }
  x2 = XEXP (x1, 0);
  goto L6031;

 L2879: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L2880;
    }
  x2 = XEXP (x1, 0);
  goto L6031;

 L2880: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L2881;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6031;

 L2881: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 290;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6031;

 L2893: ATTRIBUTE_UNUSED_LABEL
  tem = recog_4 (x0, insn, pnum_clobbers);
  if (tem >= 0)
    return tem;
  x2 = XEXP (x1, 0);
  goto L6031;

 L2770: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L2771;
    }
  x2 = XEXP (x1, 0);
  goto L6031;

 L2771: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_add_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L2772;
    }
  x2 = XEXP (x1, 0);
  goto L6031;

 L2772: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L2773;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6031;

 L2773: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 282;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6031;

 L2795: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (arm_comparison_operator (x3, SImode))
    {
      operands[4] = x3;
      goto L2796;
    }
  goto L6056;

 L2796: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L2797;
    }
  goto L6056;

 L2797: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      operands[3] = x4;
      goto L2798;
    }
  goto L6056;

 L2798: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L2799;
    }
  goto L6056;

 L2799: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L2800;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L6056;

 L2800: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM))
    {
      return 284;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L6056;

 L3409: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (memory_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L3410;
    }
  x2 = XEXP (x1, 0);
  goto L6031;

 L3410: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (memory_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L3411;
    }
  x2 = XEXP (x1, 0);
  goto L6031;

 L3411: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L3412;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6031;

 L3412: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      operands[4] = x2;
      goto L3413;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6031;

 L3413: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && adjacent_mem_locations (operands[2], operands[3])))
    {
      return 320;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6031;

 L3471: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode)
    goto L6069;
  x2 = XEXP (x1, 0);
  goto L6035;

 L6069: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x2))
    {
    case ZERO_EXTEND:
      goto L3472;
    case MEM:
      goto L3522;
    default:
     break;
   }
  x2 = XEXP (x1, 0);
  goto L6035;

 L3472: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == QImode
      && GET_CODE (x3) == MEM)
    goto L3473;
  x2 = XEXP (x1, 0);
  goto L6035;

 L3473: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode)
    goto L6071;
  x2 = XEXP (x1, 0);
  goto L6035;

 L6071: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x4))
    {
    case PLUS:
      goto L3474;
    case MINUS:
      goto L3487;
    default:
     break;
   }
  x2 = XEXP (x1, 0);
  goto L6035;

 L3474: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[1] = x5;
      goto L3475;
    }
  x2 = XEXP (x1, 0);
  goto L6035;

 L3475: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (index_operand (x5, SImode))
    {
      operands[2] = x5;
      goto L3476;
    }
  x2 = XEXP (x1, 0);
  goto L6035;

 L3476: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L3477;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6035;

 L3477: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L3478;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6035;

 L3478: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L3479;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6035;

 L3479: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[1]))
    goto L3480;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6035;

 L3480: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[2])
      && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM)))
    {
      return 325;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6035;

 L3487: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[1] = x5;
      goto L3488;
    }
  x2 = XEXP (x1, 0);
  goto L6035;

 L3488: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (s_register_operand (x5, SImode))
    {
      operands[2] = x5;
      goto L3489;
    }
  x2 = XEXP (x1, 0);
  goto L6035;

 L3489: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L3490;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6035;

 L3490: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L3491;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6035;

 L3491: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MINUS)
    goto L3492;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6035;

 L3492: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[1]))
    goto L3493;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6035;

 L3493: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[2])
      && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM)))
    {
      return 326;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6035;

 L3522: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode)
    goto L6073;
  x2 = XEXP (x1, 0);
  goto L6035;

 L6073: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x3))
    {
    case PLUS:
      goto L3523;
    case MINUS:
      goto L3535;
    default:
     break;
   }
  x2 = XEXP (x1, 0);
  goto L6035;

 L3523: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L3524;
    }
  x2 = XEXP (x1, 0);
  goto L6035;

 L3524: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (index_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L3525;
    }
  x2 = XEXP (x1, 0);
  goto L6035;

 L3525: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L3526;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6035;

 L3526: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L3527;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6035;

 L3527: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L3528;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6035;

 L3528: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[1]))
    goto L3529;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6035;

 L3529: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[2])
      && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM)))
    {
      return 329;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6035;

 L3535: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L3536;
    }
  x2 = XEXP (x1, 0);
  goto L6035;

 L3536: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L3537;
    }
  x2 = XEXP (x1, 0);
  goto L6035;

 L3537: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L3538;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6035;

 L3538: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L3539;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6035;

 L3539: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MINUS)
    goto L3540;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6035;

 L3540: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[1]))
    goto L3541;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6035;

 L3541: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[2])
      && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM)))
    {
      return 330;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6035;

 L3665: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L3666;
  goto ret0;

 L3666: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode)
    goto L6075;
  goto ret0;

 L6075: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x3))
    {
    case PLUS:
      goto L3667;
    case MINUS:
      goto L3683;
    default:
     break;
   }
  goto ret0;

 L3667: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (shift_operator (x4, SImode))
    {
      operands[2] = x4;
      goto L3668;
    }
  goto ret0;

 L3668: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[3] = x5;
      goto L3669;
    }
  goto ret0;

 L3669: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (const_shift_operand (x5, SImode))
    {
      operands[4] = x5;
      goto L3670;
    }
  goto ret0;

 L3670: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L3671;
    }
  goto ret0;

 L3671: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L3672;
  goto ret0;

 L3672: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L3673;
    }
  goto ret0;

 L3673: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L3674;
  goto ret0;

 L3674: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[2]))
    {
      operands[2] = x3;
      goto L3675;
    }
  goto ret0;

 L3675: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[3]))
    goto L3676;
  goto ret0;

 L3676: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[4]))
    goto L3677;
  goto ret0;

 L3677: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[1])
      && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && REGNO (operands[3]) != FRAME_POINTER_REGNUM))
    {
      return 339;
    }
  goto ret0;

 L3683: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L3684;
    }
  goto ret0;

 L3684: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (shift_operator (x4, SImode))
    {
      operands[2] = x4;
      goto L3685;
    }
  goto ret0;

 L3685: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[3] = x5;
      goto L3686;
    }
  goto ret0;

 L3686: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (const_shift_operand (x5, SImode))
    {
      operands[4] = x5;
      goto L3687;
    }
  goto ret0;

 L3687: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L3688;
  goto ret0;

 L3688: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L3689;
    }
  goto ret0;

 L3689: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MINUS)
    goto L3690;
  goto ret0;

 L3690: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[1]))
    goto L3691;
  goto ret0;

 L3691: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[2]))
    {
      operands[2] = x3;
      goto L3692;
    }
  goto ret0;

 L3692: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[3]))
    goto L3693;
  goto ret0;

 L3693: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[4])
      && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && REGNO (operands[3]) != FRAME_POINTER_REGNUM))
    {
      return 340;
    }
  goto ret0;
 ret0:
  return -1;
}

static int recog_6 PARAMS ((rtx, rtx, int *));
static int
recog_6 (x0, insn, pnum_clobbers)
     rtx x0 ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  rtx * const operands ATTRIBUTE_UNUSED = &recog_data.operand[0];
  rtx x1 ATTRIBUTE_UNUSED;
  rtx x2 ATTRIBUTE_UNUSED;
  rtx x3 ATTRIBUTE_UNUSED;
  rtx x4 ATTRIBUTE_UNUSED;
  rtx x5 ATTRIBUTE_UNUSED;
  rtx x6 ATTRIBUTE_UNUSED;
  rtx x7 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;

  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  operands[0] = x2;
  goto L2344;
 L2552: ATTRIBUTE_UNUSED_LABEL
  switch (GET_MODE (x2))
    {
    case CC_NOOVmode:
      goto L6027;
    case SImode:
      goto L6032;
    case QImode:
      goto L6029;
    case HImode:
      goto L6033;
    default:
      break;
    }
 L3785: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == PC)
    goto L3786;
  if (s_register_operand (x2, VOIDmode))
    {
      operands[0] = x2;
      goto L2424;
    }
 L2461: ATTRIBUTE_UNUSED_LABEL
  if (register_operand (x2, VOIDmode))
    {
      operands[0] = x2;
      goto L2462;
    }
  goto ret0;

 L2344: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CALL)
    goto L2345;
  x2 = XEXP (x1, 0);
  goto L2552;

 L2345: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L2346;
  x2 = XEXP (x1, 0);
  goto L2552;

 L2346: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode)
    goto L6037;
  x2 = XEXP (x1, 0);
  goto L2552;

 L6037: ATTRIBUTE_UNUSED_LABEL
  if (register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L2347;
    }
 L6038: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L2368;
    }
 L6039: ATTRIBUTE_UNUSED_LABEL
  if (memory_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L2389;
    }
  x2 = XEXP (x1, 0);
  goto L2552;

 L2347: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  operands[2] = x3;
  goto L2348;

 L2348: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == USE)
    goto L2349;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L6038;

 L2349: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  operands[3] = x2;
  goto L2350;

 L2350: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 250;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L6038;

 L2368: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  operands[2] = x3;
  goto L2369;

 L2369: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == USE)
    goto L2370;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L6039;

 L2370: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  operands[3] = x2;
  goto L2371;

 L2371: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 251;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L6039;

 L2389: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  operands[2] = x3;
  goto L2390;

 L2390: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == USE)
    goto L2391;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2552;

 L2391: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  operands[3] = x2;
  goto L2392;

 L2392: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && (!CONSTANT_ADDRESS_P (XEXP (operands[1], 0))))
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 252;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2552;

 L6027: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == REG
      && XINT (x2, 0) == 24)
    goto L2553;
  goto L3785;

 L2553: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == CC_NOOVmode
      && GET_CODE (x2) == COMPARE)
    goto L2554;
  x2 = XEXP (x1, 0);
  goto L3785;

 L2554: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode)
    goto L6041;
  x2 = XEXP (x1, 0);
  goto L3785;

 L6041: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x3))
    {
    case MINUS:
      goto L2602;
    case PLUS:
      goto L2651;
    default:
     break;
   }
 L6040: ATTRIBUTE_UNUSED_LABEL
  if (shiftable_operator (x3, SImode))
    {
      operands[1] = x3;
      goto L2555;
    }
  x2 = XEXP (x1, 0);
  goto L3785;

 L2602: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L2603;
    }
  goto L6040;

 L2603: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (shift_operator (x4, SImode))
    {
      operands[2] = x4;
      goto L2604;
    }
  goto L6040;

 L2604: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[3] = x5;
      goto L2605;
    }
  goto L6040;

 L2605: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (reg_or_int_operand (x5, SImode))
    {
      operands[4] = x5;
      goto L2606;
    }
  goto L6040;

 L2606: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L2607;
  x3 = XEXP (x2, 0);
  goto L6040;

 L2607: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  switch (GET_CODE (x1))
    {
    case SET:
      goto L2608;
    case CLOBBER:
      goto L2625;
    default:
     break;
   }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L6040;

 L2608: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L2609;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L6040;

 L2609: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MINUS)
    goto L2610;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L6040;

 L2610: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[1]))
    goto L2611;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L6040;

 L2611: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[2]))
    {
      operands[2] = x3;
      goto L2612;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L6040;

 L2612: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[3]))
    goto L2613;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L6040;

 L2613: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[4])
      && (TARGET_ARM))
    {
      return 272;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L6040;

 L2625: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L2626;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L6040;

 L2626: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 273;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L6040;

 L2651: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode
      && GET_CODE (x4) == PLUS)
    goto L2652;
  goto L6040;

 L2652: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (GET_MODE (x5) == SImode)
    goto L6043;
  goto L6040;

 L6043: ATTRIBUTE_UNUSED_LABEL
  if (shift_operator (x5, SImode))
    {
      operands[5] = x5;
      goto L2653;
    }
 L6044: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x5) == MULT)
    goto L2710;
  goto L6040;

 L2653: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 0);
  operands[3] = x6;
  goto L2654;

 L2654: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 1);
  operands[4] = x6;
  goto L2655;

 L2655: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  operands[1] = x5;
  goto L2656;

 L2656: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (const_int_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L2657;
    }
  x4 = XEXP (x3, 0);
  x5 = XEXP (x4, 0);
  goto L6044;

 L2657: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L2658;
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  x5 = XEXP (x4, 0);
  goto L6044;

 L2658: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  switch (GET_CODE (x1))
    {
    case SET:
      goto L2659;
    case CLOBBER:
      goto L2680;
    default:
     break;
   }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  x5 = XEXP (x4, 0);
  goto L6044;

 L2659: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  operands[0] = x2;
  goto L2660;

 L2660: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L2661;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  x5 = XEXP (x4, 0);
  goto L6044;

 L2661: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L2662;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  x5 = XEXP (x4, 0);
  goto L6044;

 L2662: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[5]))
    {
      operands[5] = x4;
      goto L2663;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  x5 = XEXP (x4, 0);
  goto L6044;

 L2663: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[3]))
    goto L2664;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  x5 = XEXP (x4, 0);
  goto L6044;

 L2664: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (rtx_equal_p (x5, operands[4]))
    goto L2665;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  x5 = XEXP (x4, 0);
  goto L6044;

 L2665: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[1]))
    goto L2666;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  x5 = XEXP (x4, 0);
  goto L6044;

 L2666: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[2])
      && (TARGET_ARM && reload_in_progress && !arm_is_xscale))
    {
      return 275;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  x5 = XEXP (x4, 0);
  goto L6044;

 L2680: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L2681;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  x5 = XEXP (x4, 0);
  goto L6044;

 L2681: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && reload_in_progress && !arm_is_xscale))
    {
      return 276;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  x5 = XEXP (x4, 0);
  goto L6044;

 L2710: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 0);
  operands[3] = x6;
  goto L2711;

 L2711: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 1);
  operands[4] = x6;
  goto L2712;

 L2712: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  operands[1] = x5;
  goto L2713;

 L2713: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (const_int_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L2714;
    }
  goto L6040;

 L2714: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L2715;
  x3 = XEXP (x2, 0);
  goto L6040;

 L2715: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  switch (GET_CODE (x1))
    {
    case SET:
      goto L2716;
    case CLOBBER:
      goto L2737;
    default:
     break;
   }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L6040;

 L2716: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  operands[0] = x2;
  goto L2717;

 L2717: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L2718;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L6040;

 L2718: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L2719;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L6040;

 L2719: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode
      && GET_CODE (x4) == MULT)
    goto L2720;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L6040;

 L2720: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[3]))
    goto L2721;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L6040;

 L2721: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (rtx_equal_p (x5, operands[4]))
    goto L2722;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L6040;

 L2722: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[1]))
    goto L2723;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L6040;

 L2723: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[2])
      && (TARGET_ARM && reload_in_progress && !arm_is_xscale))
    {
      return 278;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L6040;

 L2737: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L2738;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L6040;

 L2738: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && reload_in_progress))
    {
      return 279;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L6040;

 L2555: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (shift_operator (x4, SImode))
    {
      operands[3] = x4;
      goto L2556;
    }
  x2 = XEXP (x1, 0);
  goto L3785;

 L2556: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[4] = x5;
      goto L2557;
    }
  x2 = XEXP (x1, 0);
  goto L3785;

 L2557: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (reg_or_int_operand (x5, SImode))
    {
      operands[5] = x5;
      goto L2558;
    }
  x2 = XEXP (x1, 0);
  goto L3785;

 L2558: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L2559;
    }
  x2 = XEXP (x1, 0);
  goto L3785;

 L2559: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L2560;
  x2 = XEXP (x1, 0);
  goto L3785;

 L2560: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  switch (GET_CODE (x1))
    {
    case SET:
      goto L2561;
    case CLOBBER:
      goto L2578;
    default:
     break;
   }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L2561: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L2562;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L2562: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (rtx_equal_p (x2, operands[1]))
    {
      operands[1] = x2;
      goto L2563;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L2563: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[3]))
    {
      operands[3] = x3;
      goto L2564;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L2564: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[4]))
    goto L2565;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L2565: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[5]))
    goto L2566;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L2566: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[2])
      && (TARGET_ARM))
    {
      return 269;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L2578: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L2579;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L2579: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 270;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L6032: ATTRIBUTE_UNUSED_LABEL
  tem = recog_5 (x0, insn, pnum_clobbers);
  if (tem >= 0)
    return tem;
  goto L3785;

 L6029: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == MEM)
    goto L3423;
  if (s_register_operand (x2, QImode))
    {
      operands[3] = x2;
      goto L3447;
    }
 L6034: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, QImode))
    {
      operands[5] = x2;
      goto L3601;
    }
  goto L3785;

 L3423: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode)
    goto L6077;
  goto L3785;

 L6077: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x3))
    {
    case PLUS:
      goto L3424;
    case MINUS:
      goto L3436;
    default:
     break;
   }
  goto L3785;

 L3424: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode)
    goto L6079;
  goto L3785;

 L6079: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L3425;
    }
  if (shift_operator (x4, SImode))
    {
      operands[2] = x4;
      goto L3571;
    }
  goto L3785;

 L3425: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (index_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L3426;
    }
  goto L3785;

 L3426: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, QImode))
    {
      operands[3] = x2;
      goto L3427;
    }
  x2 = XEXP (x1, 0);
  goto L3785;

 L3427: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L3428;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3428: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L3429;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3429: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L3430;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3430: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[1]))
    goto L3431;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3431: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[2])
      && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM)))
    {
      return 321;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3571: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[3] = x5;
      goto L3572;
    }
  goto L3785;

 L3572: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (const_shift_operand (x5, SImode))
    {
      operands[4] = x5;
      goto L3573;
    }
  goto L3785;

 L3573: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L3574;
    }
  goto L3785;

 L3574: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, QImode))
    {
      operands[5] = x2;
      goto L3575;
    }
  x2 = XEXP (x1, 0);
  goto L3785;

 L3575: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L3576;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3576: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L3577;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3577: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L3578;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3578: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[2]))
    {
      operands[2] = x3;
      goto L3579;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3579: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[3]))
    goto L3580;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3580: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[4]))
    goto L3581;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3581: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[1])
      && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && REGNO (operands[3]) != FRAME_POINTER_REGNUM))
    {
      return 333;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3436: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L3437;
    }
  goto L3785;

 L3437: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_MODE (x4) == SImode)
    goto L6081;
  goto L3785;

 L6081: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L3438;
    }
  if (shift_operator (x4, SImode))
    {
      operands[2] = x4;
      goto L3588;
    }
  goto L3785;

 L3438: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, QImode))
    {
      operands[3] = x2;
      goto L3439;
    }
  x2 = XEXP (x1, 0);
  goto L3785;

 L3439: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L3440;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3440: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L3441;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3441: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MINUS)
    goto L3442;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3442: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[1]))
    goto L3443;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3443: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[2])
      && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM)))
    {
      return 322;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3588: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[3] = x5;
      goto L3589;
    }
  goto L3785;

 L3589: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (const_shift_operand (x5, SImode))
    {
      operands[4] = x5;
      goto L3590;
    }
  goto L3785;

 L3590: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, QImode))
    {
      operands[5] = x2;
      goto L3591;
    }
  x2 = XEXP (x1, 0);
  goto L3785;

 L3591: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L3592;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3592: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L3593;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3593: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MINUS)
    goto L3594;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3594: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[1]))
    goto L3595;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3595: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[2]))
    {
      operands[2] = x3;
      goto L3596;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3596: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[3]))
    goto L3597;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3597: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[4])
      && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && REGNO (operands[3]) != FRAME_POINTER_REGNUM))
    {
      return 334;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3447: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == QImode
      && GET_CODE (x2) == MEM)
    goto L3448;
  x2 = XEXP (x1, 0);
  goto L6034;

 L3448: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode)
    goto L6083;
  x2 = XEXP (x1, 0);
  goto L6034;

 L6083: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x3))
    {
    case PLUS:
      goto L3449;
    case MINUS:
      goto L3461;
    default:
     break;
   }
  x2 = XEXP (x1, 0);
  goto L6034;

 L3449: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L3450;
    }
  x2 = XEXP (x1, 0);
  goto L6034;

 L3450: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (index_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L3451;
    }
  x2 = XEXP (x1, 0);
  goto L6034;

 L3451: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L3452;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6034;

 L3452: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L3453;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6034;

 L3453: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L3454;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6034;

 L3454: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[1]))
    goto L3455;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6034;

 L3455: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[2])
      && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM)))
    {
      return 323;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6034;

 L3461: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L3462;
    }
  x2 = XEXP (x1, 0);
  goto L6034;

 L3462: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L3463;
    }
  x2 = XEXP (x1, 0);
  goto L6034;

 L3463: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L3464;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6034;

 L3464: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L3465;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6034;

 L3465: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MINUS)
    goto L3466;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6034;

 L3466: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[1]))
    goto L3467;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6034;

 L3467: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[2])
      && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM)))
    {
      return 324;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6034;

 L3601: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == QImode
      && GET_CODE (x2) == MEM)
    goto L3602;
  x2 = XEXP (x1, 0);
  goto L3785;

 L3602: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode)
    goto L6085;
  x2 = XEXP (x1, 0);
  goto L3785;

 L6085: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x3))
    {
    case PLUS:
      goto L3603;
    case MINUS:
      goto L3619;
    default:
     break;
   }
  x2 = XEXP (x1, 0);
  goto L3785;

 L3603: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (shift_operator (x4, SImode))
    {
      operands[2] = x4;
      goto L3604;
    }
  x2 = XEXP (x1, 0);
  goto L3785;

 L3604: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[3] = x5;
      goto L3605;
    }
  x2 = XEXP (x1, 0);
  goto L3785;

 L3605: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (const_shift_operand (x5, SImode))
    {
      operands[4] = x5;
      goto L3606;
    }
  x2 = XEXP (x1, 0);
  goto L3785;

 L3606: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L3607;
    }
  x2 = XEXP (x1, 0);
  goto L3785;

 L3607: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L3608;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3608: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L3609;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3609: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L3610;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3610: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[2]))
    {
      operands[2] = x3;
      goto L3611;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3611: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[3]))
    goto L3612;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3612: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[4]))
    goto L3613;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3613: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[1])
      && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && REGNO (operands[3]) != FRAME_POINTER_REGNUM))
    {
      return 335;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3619: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L3620;
    }
  x2 = XEXP (x1, 0);
  goto L3785;

 L3620: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (shift_operator (x4, SImode))
    {
      operands[2] = x4;
      goto L3621;
    }
  x2 = XEXP (x1, 0);
  goto L3785;

 L3621: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[3] = x5;
      goto L3622;
    }
  x2 = XEXP (x1, 0);
  goto L3785;

 L3622: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (const_shift_operand (x5, SImode))
    {
      operands[4] = x5;
      goto L3623;
    }
  x2 = XEXP (x1, 0);
  goto L3785;

 L3623: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L3624;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3624: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L3625;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3625: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MINUS)
    goto L3626;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3626: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[1]))
    goto L3627;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3627: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[2]))
    {
      operands[2] = x3;
      goto L3628;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3628: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[3]))
    goto L3629;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3629: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[4])
      && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && REGNO (operands[3]) != FRAME_POINTER_REGNUM))
    {
      return 336;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L6033: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, HImode))
    {
      operands[3] = x2;
      goto L3545;
    }
 L6036: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, HImode))
    {
      operands[5] = x2;
      goto L3697;
    }
  goto L3785;

 L3545: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == HImode
      && GET_CODE (x2) == MEM)
    goto L3546;
  x2 = XEXP (x1, 0);
  goto L6036;

 L3546: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode)
    goto L6087;
  x2 = XEXP (x1, 0);
  goto L6036;

 L6087: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x3))
    {
    case PLUS:
      goto L3547;
    case MINUS:
      goto L3559;
    default:
     break;
   }
  x2 = XEXP (x1, 0);
  goto L6036;

 L3547: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L3548;
    }
  x2 = XEXP (x1, 0);
  goto L6036;

 L3548: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (index_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L3549;
    }
  x2 = XEXP (x1, 0);
  goto L6036;

 L3549: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L3550;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6036;

 L3550: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L3551;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6036;

 L3551: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L3552;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6036;

 L3552: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[1]))
    goto L3553;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6036;

 L3553: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[2])
      && (TARGET_ARM
   && !BYTES_BIG_ENDIAN
   && !TARGET_MMU_TRAPS
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM)))
    {
      return 331;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6036;

 L3559: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L3560;
    }
  x2 = XEXP (x1, 0);
  goto L6036;

 L3560: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L3561;
    }
  x2 = XEXP (x1, 0);
  goto L6036;

 L3561: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L3562;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6036;

 L3562: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L3563;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6036;

 L3563: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MINUS)
    goto L3564;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6036;

 L3564: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[1]))
    goto L3565;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6036;

 L3565: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[2])
      && (TARGET_ARM
   && !BYTES_BIG_ENDIAN
   && !TARGET_MMU_TRAPS
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM)))
    {
      return 332;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6036;

 L3697: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == HImode
      && GET_CODE (x2) == MEM)
    goto L3698;
  x2 = XEXP (x1, 0);
  goto L3785;

 L3698: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode)
    goto L6089;
  x2 = XEXP (x1, 0);
  goto L3785;

 L6089: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x3))
    {
    case PLUS:
      goto L3699;
    case MINUS:
      goto L3715;
    default:
     break;
   }
  x2 = XEXP (x1, 0);
  goto L3785;

 L3699: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (shift_operator (x4, SImode))
    {
      operands[2] = x4;
      goto L3700;
    }
  x2 = XEXP (x1, 0);
  goto L3785;

 L3700: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[3] = x5;
      goto L3701;
    }
  x2 = XEXP (x1, 0);
  goto L3785;

 L3701: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (const_shift_operand (x5, SImode))
    {
      operands[4] = x5;
      goto L3702;
    }
  x2 = XEXP (x1, 0);
  goto L3785;

 L3702: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L3703;
    }
  x2 = XEXP (x1, 0);
  goto L3785;

 L3703: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L3704;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3704: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L3705;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3705: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L3706;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3706: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[2]))
    {
      operands[2] = x3;
      goto L3707;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3707: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[3]))
    goto L3708;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3708: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[4]))
    goto L3709;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3709: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[1])
      && (TARGET_ARM
   && !BYTES_BIG_ENDIAN
   && !TARGET_MMU_TRAPS
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && REGNO (operands[3]) != FRAME_POINTER_REGNUM))
    {
      return 341;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3715: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L3716;
    }
  x2 = XEXP (x1, 0);
  goto L3785;

 L3716: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (shift_operator (x4, SImode))
    {
      operands[2] = x4;
      goto L3717;
    }
  x2 = XEXP (x1, 0);
  goto L3785;

 L3717: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[3] = x5;
      goto L3718;
    }
  x2 = XEXP (x1, 0);
  goto L3785;

 L3718: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (const_shift_operand (x5, SImode))
    {
      operands[4] = x5;
      goto L3719;
    }
  x2 = XEXP (x1, 0);
  goto L3785;

 L3719: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L3720;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3720: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L3721;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3721: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MINUS)
    goto L3722;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3722: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[1]))
    goto L3723;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3723: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[2]))
    {
      operands[2] = x3;
      goto L3724;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3724: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[3]))
    goto L3725;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3725: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[4])
      && (TARGET_ARM
   && !BYTES_BIG_ENDIAN
   && !TARGET_MMU_TRAPS
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && REGNO (operands[3]) != FRAME_POINTER_REGNUM))
    {
      return 342;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L3785;

 L3786: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L3787;
    }
  goto ret0;

 L3787: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == USE)
    goto L3788;
  goto ret0;

 L3788: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) == LABEL_REF)
    goto L3789;
  goto ret0;

 L3789: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  operands[1] = x3;
  goto L3790;

 L3790: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB))
    {
      return 356;
    }
  goto ret0;

 L2424: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CALL)
    goto L2425;
  x2 = XEXP (x1, 0);
  goto L2461;

 L2425: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L2426;
  x2 = XEXP (x1, 0);
  goto L2461;

 L2426: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  operands[1] = x4;
  goto L2427;

 L2427: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  operands[2] = x3;
  goto L2428;

 L2428: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == USE)
    goto L2429;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2461;

 L2429: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  operands[3] = x2;
  goto L2430;

 L2430: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM
   && (GET_CODE (operands[1]) == SYMBOL_REF)
   && !arm_is_longcall_p (operands[1], INTVAL (operands[3]), 1))
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 254;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L2461;

 L2462: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CALL)
    goto L2463;
  goto ret0;

 L2463: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L2464;
  goto ret0;

 L2464: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  operands[1] = x4;
  goto L2465;

 L2465: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  operands[2] = x3;
  goto L2466;

 L2466: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == USE)
    goto L2467;
  goto ret0;

 L2467: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  operands[3] = x2;
  goto L2468;

 L2468: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB
   && operands[3] == const0_rtx && (GET_CODE (operands[1]) == SYMBOL_REF))
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 256;
    }
  goto ret0;
 ret0:
  return -1;
}

static int recog_7 PARAMS ((rtx, rtx, int *));
static int
recog_7 (x0, insn, pnum_clobbers)
     rtx x0 ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  rtx * const operands ATTRIBUTE_UNUSED = &recog_data.operand[0];
  rtx x1 ATTRIBUTE_UNUSED;
  rtx x2 ATTRIBUTE_UNUSED;
  rtx x3 ATTRIBUTE_UNUSED;
  rtx x4 ATTRIBUTE_UNUSED;
  rtx x5 ATTRIBUTE_UNUSED;
  rtx x6 ATTRIBUTE_UNUSED;
  rtx x7 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;

  switch (XVECLEN (x0, 0))
    {
    case 2:
      goto L1;
    case 8:
      goto L1913;
    case 5:
      goto L1954;
    case 6:
      goto L1988;
    case 4:
      goto L2018;
    default:
      break;
    }
 L5924: ATTRIBUTE_UNUSED_LABEL
  if (XVECLEN (x0, 0) >= 5
      && load_multiple_operation (x0, VOIDmode))
    {
      operands[0] = x0;
      goto L1673;
    }
 L5925: ATTRIBUTE_UNUSED_LABEL
  if (XVECLEN (x0, 0) >= 4
      && load_multiple_operation (x0, VOIDmode))
    {
      operands[0] = x0;
      goto L1701;
    }
 L5926: ATTRIBUTE_UNUSED_LABEL
  if (XVECLEN (x0, 0) >= 3
      && load_multiple_operation (x0, VOIDmode))
    {
      operands[0] = x0;
      goto L1723;
    }
 L5927: ATTRIBUTE_UNUSED_LABEL
  if (XVECLEN (x0, 0) >= 2
      && load_multiple_operation (x0, VOIDmode))
    {
      operands[0] = x0;
      goto L1779;
    }
 L5928: ATTRIBUTE_UNUSED_LABEL
  if (XVECLEN (x0, 0) >= 5
      && store_multiple_operation (x0, VOIDmode))
    {
      operands[0] = x0;
      goto L1790;
    }
 L5929: ATTRIBUTE_UNUSED_LABEL
  if (XVECLEN (x0, 0) >= 4
      && store_multiple_operation (x0, VOIDmode))
    {
      operands[0] = x0;
      goto L1819;
    }
 L5930: ATTRIBUTE_UNUSED_LABEL
  if (XVECLEN (x0, 0) >= 3
      && store_multiple_operation (x0, VOIDmode))
    {
      operands[0] = x0;
      goto L1842;
    }
 L5931: ATTRIBUTE_UNUSED_LABEL
  if (XVECLEN (x0, 0) >= 2
      && store_multiple_operation (x0, VOIDmode))
    {
      operands[0] = x0;
      goto L1901;
    }
 L5936: ATTRIBUTE_UNUSED_LABEL
  switch (XVECLEN (x0, 0))
    {
    case 3:
      goto L2280;
    case 2:
      goto L2342;
    default:
      break;
    }
 L5942: ATTRIBUTE_UNUSED_LABEL
  if (XVECLEN (x0, 0) >= 1
      && multi_register_push (x0, VOIDmode))
    {
      operands[2] = x0;
      goto L3756;
    }
  goto ret0;

 L1: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  switch (GET_CODE (x1))
    {
    case SET:
      goto L2;
    case CALL:
      goto L2290;
    default:
     break;
   }
  goto L5927;

 L2: ATTRIBUTE_UNUSED_LABEL
  tem = recog_2 (x0, insn, pnum_clobbers);
  if (tem >= 0)
    return tem;
  goto L5927;

 L2290: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L2291;
  goto L5927;

 L2291: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode)
    goto L6004;
  goto L5927;

 L6004: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L2292;
    }
 L6005: ATTRIBUTE_UNUSED_LABEL
  if (memory_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L2309;
    }
 L6006: ATTRIBUTE_UNUSED_LABEL
  if (register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L2326;
    }
  goto L5927;

 L2292: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  operands[1] = x2;
  goto L2293;

 L2293: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == USE)
    goto L2294;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6005;

 L2294: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  operands[2] = x2;
  goto L2295;

 L2295: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 247;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6005;

 L2309: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  operands[1] = x2;
  goto L2310;

 L2310: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == USE)
    goto L2311;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6006;

 L2311: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  operands[2] = x2;
  goto L2312;

 L2312: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 248;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6006;

 L2326: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  operands[1] = x2;
  goto L2327;

 L2327: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == USE)
    goto L2328;
  goto L5927;

 L2328: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  operands[2] = x2;
  goto L2329;

 L2329: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 249;
    }
  goto L5927;

 L1913: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  if (GET_CODE (x1) == SET)
    goto L1914;
  goto L5924;

 L1914: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1915;
  goto L5924;

 L1915: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L1916;
    }
  goto L5924;

 L1916: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1917;
  goto L5924;

 L1917: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L1918;
    }
  goto L5924;

 L1918: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L1919;
  goto L5924;

 L1919: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1920;
  goto L5924;

 L1920: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1921;
  goto L5924;

 L1921: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[2]))
    goto L1922;
  goto L5924;

 L1922: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 4)
    goto L1923;
  goto L5924;

 L1923: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1924;
  goto L5924;

 L1924: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1925;
  goto L5924;

 L1925: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[3]))
    goto L1926;
  goto L5924;

 L1926: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 4)
    goto L1927;
  goto L5924;

 L1927: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 2);
  if (GET_CODE (x1) == SET)
    goto L1928;
  goto L5924;

 L1928: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1929;
  goto L5924;

 L1929: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1930;
  goto L5924;

 L1930: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[2]))
    goto L1931;
  goto L5924;

 L1931: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 8)
    goto L1932;
  goto L5924;

 L1932: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1933;
  goto L5924;

 L1933: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1934;
  goto L5924;

 L1934: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[3]))
    goto L1935;
  goto L5924;

 L1935: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 8)
    goto L1936;
  goto L5924;

 L1936: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 3);
  if (GET_CODE (x1) == SET)
    goto L1937;
  goto L5924;

 L1937: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L1938;
    }
  goto L5924;

 L1938: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L1939;
  goto L5924;

 L1939: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[2]))
    goto L1940;
  goto L5924;

 L1940: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 12)
    goto L1941;
  goto L5924;

 L1941: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 4);
  if (GET_CODE (x1) == SET)
    goto L1942;
  goto L5924;

 L1942: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L1943;
    }
  goto L5924;

 L1943: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L1944;
  goto L5924;

 L1944: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[3]))
    goto L1945;
  goto L5924;

 L1945: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 12)
    goto L1946;
  goto L5924;

 L1946: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 5);
  if (GET_CODE (x1) == CLOBBER)
    goto L1947;
  goto L5924;

 L1947: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      operands[4] = x2;
      goto L1948;
    }
  goto L5924;

 L1948: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 6);
  if (GET_CODE (x1) == CLOBBER)
    goto L1949;
  goto L5924;

 L1949: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      operands[5] = x2;
      goto L1950;
    }
  goto L5924;

 L1950: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 7);
  if (GET_CODE (x1) == CLOBBER)
    goto L1951;
  goto L5924;

 L1951: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      operands[6] = x2;
      goto L1952;
    }
  goto L5924;

 L1952: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB))
    {
      return 213;
    }
  goto L5924;

 L1954: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  if (GET_CODE (x1) == SET)
    goto L1955;
  goto L5924;

 L1955: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1956;
  goto L5924;

 L1956: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L1957;
    }
  goto L5924;

 L1957: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1958;
  goto L5924;

 L1958: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L1959;
    }
  goto L5924;

 L1959: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L1960;
  goto L5924;

 L1960: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1961;
  goto L5924;

 L1961: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1962;
  goto L5924;

 L1962: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[2]))
    goto L1963;
  goto L5924;

 L1963: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 4)
    goto L1964;
  goto L5924;

 L1964: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1965;
  goto L5924;

 L1965: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1966;
  goto L5924;

 L1966: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[3]))
    goto L1967;
  goto L5924;

 L1967: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 4)
    goto L1968;
  goto L5924;

 L1968: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 2);
  if (GET_CODE (x1) == SET)
    goto L1969;
  goto L5924;

 L1969: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1970;
  goto L5924;

 L1970: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1971;
  goto L5924;

 L1971: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[2]))
    goto L1972;
  goto L5924;

 L1972: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 8)
    goto L1973;
  goto L5924;

 L1973: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1974;
  goto L5924;

 L1974: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1975;
  goto L5924;

 L1975: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[3]))
    goto L1976;
  goto L5924;

 L1976: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 8)
    goto L1977;
  goto L5924;

 L1977: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 3);
  if (GET_CODE (x1) == SET)
    goto L1978;
  goto L5924;

 L1978: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L1979;
    }
  goto L5924;

 L1979: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L1980;
  goto L5924;

 L1980: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[2]))
    goto L1981;
  goto L5924;

 L1981: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 12)
    goto L1982;
  goto L5924;

 L1982: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 4);
  if (GET_CODE (x1) == SET)
    goto L1983;
  goto L5924;

 L1983: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L1984;
    }
  goto L5924;

 L1984: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L1985;
  goto L5924;

 L1985: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[3]))
    goto L1986;
  goto L5924;

 L1986: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 12
      && (TARGET_THUMB)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 3;
      return 213;
    }
  goto L5924;

 L1988: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  if (GET_CODE (x1) == SET)
    goto L1989;
  goto L5924;

 L1989: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1990;
  goto L5924;

 L1990: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L1991;
    }
  goto L5924;

 L1991: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1992;
  goto L5924;

 L1992: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L1993;
    }
  goto L5924;

 L1993: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L1994;
  goto L5924;

 L1994: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1995;
  goto L5924;

 L1995: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1996;
  goto L5924;

 L1996: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[2]))
    goto L1997;
  goto L5924;

 L1997: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 4)
    goto L1998;
  goto L5924;

 L1998: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1999;
  goto L5924;

 L1999: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L2000;
  goto L5924;

 L2000: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[3]))
    goto L2001;
  goto L5924;

 L2001: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 4)
    goto L2002;
  goto L5924;

 L2002: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 2);
  if (GET_CODE (x1) == SET)
    goto L2003;
  goto L5924;

 L2003: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L2004;
    }
  goto L5924;

 L2004: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L2005;
  goto L5924;

 L2005: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[2]))
    goto L2006;
  goto L5924;

 L2006: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 8)
    goto L2007;
  goto L5924;

 L2007: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 3);
  if (GET_CODE (x1) == SET)
    goto L2008;
  goto L5924;

 L2008: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L2009;
    }
  goto L5924;

 L2009: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L2010;
  goto L5924;

 L2010: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[3]))
    goto L2011;
  goto L5924;

 L2011: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 8)
    goto L2012;
  goto L5924;

 L2012: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 4);
  if (GET_CODE (x1) == CLOBBER)
    goto L2013;
  goto L5924;

 L2013: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      operands[4] = x2;
      goto L2014;
    }
  goto L5924;

 L2014: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 5);
  if (GET_CODE (x1) == CLOBBER)
    goto L2015;
  goto L5924;

 L2015: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      operands[5] = x2;
      goto L2016;
    }
  goto L5924;

 L2016: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB))
    {
      return 214;
    }
  goto L5924;

 L2018: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  if (GET_CODE (x1) == SET)
    goto L2019;
  goto L5925;

 L2019: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L2020;
  goto L5925;

 L2020: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L2021;
    }
  goto L5925;

 L2021: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L2022;
  goto L5925;

 L2022: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L2023;
    }
  goto L5925;

 L2023: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L2024;
  goto L5925;

 L2024: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L2025;
  goto L5925;

 L2025: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L2026;
  goto L5925;

 L2026: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[2]))
    goto L2027;
  goto L5925;

 L2027: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 4)
    goto L2028;
  goto L5925;

 L2028: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L2029;
  goto L5925;

 L2029: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L2030;
  goto L5925;

 L2030: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[3]))
    goto L2031;
  goto L5925;

 L2031: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 4)
    goto L2032;
  goto L5925;

 L2032: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 2);
  if (GET_CODE (x1) == SET)
    goto L2033;
  goto L5925;

 L2033: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L2034;
    }
  goto L5925;

 L2034: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L2035;
  goto L5925;

 L2035: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[2]))
    goto L2036;
  goto L5925;

 L2036: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 8)
    goto L2037;
  goto L5925;

 L2037: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 3);
  if (GET_CODE (x1) == SET)
    goto L2038;
  goto L5925;

 L2038: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L2039;
    }
  goto L5925;

 L2039: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L2040;
  goto L5925;

 L2040: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[3]))
    goto L2041;
  goto L5925;

 L2041: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 8
      && (TARGET_THUMB)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 2;
      return 214;
    }
  goto L5925;

 L1673: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  if (GET_CODE (x1) == SET)
    goto L1674;
  goto L5925;

 L1674: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L1675;
    }
  goto L5925;

 L1675: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L1676;
  goto L5925;

 L1676: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L1677;
    }
  goto L5925;

 L1677: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 16)
    goto L1678;
  goto L5925;

 L1678: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L1679;
  goto L5925;

 L1679: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[3] = x2;
      goto L1680;
    }
  goto L5925;

 L1680: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1681;
  goto L5925;

 L1681: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[2]))
    goto L1682;
  goto L5925;

 L1682: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 2);
  if (GET_CODE (x1) == SET)
    goto L1683;
  goto L5925;

 L1683: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[4] = x2;
      goto L1684;
    }
  goto L5925;

 L1684: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1685;
  goto L5925;

 L1685: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1686;
  goto L5925;

 L1686: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[2]))
    goto L1687;
  goto L5925;

 L1687: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 4)
    goto L1688;
  goto L5925;

 L1688: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 3);
  if (GET_CODE (x1) == SET)
    goto L1689;
  goto L5925;

 L1689: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[5] = x2;
      goto L1690;
    }
  goto L5925;

 L1690: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1691;
  goto L5925;

 L1691: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1692;
  goto L5925;

 L1692: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[2]))
    goto L1693;
  goto L5925;

 L1693: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 8)
    goto L1694;
  goto L5925;

 L1694: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 4);
  if (GET_CODE (x1) == SET)
    goto L1695;
  goto L5925;

 L1695: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[6] = x2;
      goto L1696;
    }
  goto L5925;

 L1696: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1697;
  goto L5925;

 L1697: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1698;
  goto L5925;

 L1698: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[2]))
    goto L1699;
  goto L5925;

 L1699: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 12
      && (TARGET_ARM && XVECLEN (operands[0], 0) == 5))
    {
      return 201;
    }
  goto L5925;

 L1701: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  if (GET_CODE (x1) == SET)
    goto L1702;
  goto L5926;

 L1702: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode)
    goto L6008;
  goto L5926;

 L6008: ATTRIBUTE_UNUSED_LABEL
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L1741;
    }
 L6007: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L1703;
    }
  goto L5926;

 L1741: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1742;
  x2 = XEXP (x1, 0);
  goto L6007;

 L1742: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L1743;
    }
  x2 = XEXP (x1, 0);
  goto L6007;

 L1743: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L1744;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6007;

 L1744: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[3] = x2;
      goto L1745;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6007;

 L1745: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1746;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6007;

 L1746: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1747;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6007;

 L1747: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[1]))
    goto L1748;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6007;

 L1748: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 4)
    goto L1749;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6007;

 L1749: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 2);
  if (GET_CODE (x1) == SET)
    goto L1750;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6007;

 L1750: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[4] = x2;
      goto L1751;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6007;

 L1751: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1752;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6007;

 L1752: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1753;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6007;

 L1753: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[1]))
    goto L1754;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6007;

 L1754: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 8)
    goto L1755;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6007;

 L1755: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 3);
  if (GET_CODE (x1) == SET)
    goto L1756;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6007;

 L1756: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[5] = x2;
      goto L1757;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6007;

 L1757: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1758;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6007;

 L1758: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1759;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6007;

 L1759: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[1]))
    goto L1760;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6007;

 L1760: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 12
      && (TARGET_ARM && XVECLEN (operands[0], 0) == 4))
    {
      return 204;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6007;

 L1703: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L1704;
  goto L5926;

 L1704: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L1705;
    }
  goto L5926;

 L1705: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 12)
    goto L1706;
  goto L5926;

 L1706: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L1707;
  goto L5926;

 L1707: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[3] = x2;
      goto L1708;
    }
  goto L5926;

 L1708: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1709;
  goto L5926;

 L1709: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[2]))
    goto L1710;
  goto L5926;

 L1710: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 2);
  if (GET_CODE (x1) == SET)
    goto L1711;
  goto L5926;

 L1711: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[4] = x2;
      goto L1712;
    }
  goto L5926;

 L1712: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1713;
  goto L5926;

 L1713: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1714;
  goto L5926;

 L1714: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[2]))
    goto L1715;
  goto L5926;

 L1715: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 4)
    goto L1716;
  goto L5926;

 L1716: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 3);
  if (GET_CODE (x1) == SET)
    goto L1717;
  goto L5926;

 L1717: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[5] = x2;
      goto L1718;
    }
  goto L5926;

 L1718: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1719;
  goto L5926;

 L1719: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1720;
  goto L5926;

 L1720: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[2]))
    goto L1721;
  goto L5926;

 L1721: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 8
      && (TARGET_ARM && XVECLEN (operands[0], 0) == 4))
    {
      return 202;
    }
  goto L5926;

 L1723: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  if (GET_CODE (x1) == SET)
    goto L1724;
  goto L5927;

 L1724: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode)
    goto L6010;
  goto L5927;

 L6010: ATTRIBUTE_UNUSED_LABEL
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L1764;
    }
 L6009: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L1725;
    }
  goto L5927;

 L1764: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1765;
  x2 = XEXP (x1, 0);
  goto L6009;

 L1765: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L1766;
    }
  x2 = XEXP (x1, 0);
  goto L6009;

 L1766: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L1767;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6009;

 L1767: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[3] = x2;
      goto L1768;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6009;

 L1768: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1769;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6009;

 L1769: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1770;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6009;

 L1770: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[1]))
    goto L1771;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6009;

 L1771: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 4)
    goto L1772;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6009;

 L1772: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 2);
  if (GET_CODE (x1) == SET)
    goto L1773;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6009;

 L1773: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[4] = x2;
      goto L1774;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6009;

 L1774: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1775;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6009;

 L1775: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1776;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6009;

 L1776: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[1]))
    goto L1777;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6009;

 L1777: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 8
      && (TARGET_ARM && XVECLEN (operands[0], 0) == 3))
    {
      return 205;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L6009;

 L1725: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L1726;
  goto L5927;

 L1726: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L1727;
    }
  goto L5927;

 L1727: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 8)
    goto L1728;
  goto L5927;

 L1728: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L1729;
  goto L5927;

 L1729: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[3] = x2;
      goto L1730;
    }
  goto L5927;

 L1730: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1731;
  goto L5927;

 L1731: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[2]))
    goto L1732;
  goto L5927;

 L1732: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 2);
  if (GET_CODE (x1) == SET)
    goto L1733;
  goto L5927;

 L1733: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[4] = x2;
      goto L1734;
    }
  goto L5927;

 L1734: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1735;
  goto L5927;

 L1735: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1736;
  goto L5927;

 L1736: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[2]))
    goto L1737;
  goto L5927;

 L1737: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 4
      && (TARGET_ARM && XVECLEN (operands[0], 0) == 3))
    {
      return 203;
    }
  goto L5927;

 L1779: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  if (GET_CODE (x1) == SET)
    goto L1780;
  goto L5928;

 L1780: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L1781;
    }
  goto L5928;

 L1781: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1782;
  goto L5928;

 L1782: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L1783;
    }
  goto L5928;

 L1783: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L1784;
  goto L5928;

 L1784: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[3] = x2;
      goto L1785;
    }
  goto L5928;

 L1785: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1786;
  goto L5928;

 L1786: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1787;
  goto L5928;

 L1787: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[1]))
    goto L1788;
  goto L5928;

 L1788: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 4
      && (TARGET_ARM && XVECLEN (operands[0], 0) == 2))
    {
      return 206;
    }
  goto L5928;

 L1790: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  if (GET_CODE (x1) == SET)
    goto L1791;
  goto L5929;

 L1791: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L1792;
    }
  goto L5929;

 L1792: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L1793;
  goto L5929;

 L1793: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L1794;
    }
  goto L5929;

 L1794: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 16)
    goto L1795;
  goto L5929;

 L1795: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L1796;
  goto L5929;

 L1796: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1797;
  goto L5929;

 L1797: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[2]))
    goto L1798;
  goto L5929;

 L1798: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[3] = x2;
      goto L1799;
    }
  goto L5929;

 L1799: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 2);
  if (GET_CODE (x1) == SET)
    goto L1800;
  goto L5929;

 L1800: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1801;
  goto L5929;

 L1801: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1802;
  goto L5929;

 L1802: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[2]))
    goto L1803;
  goto L5929;

 L1803: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 4)
    goto L1804;
  goto L5929;

 L1804: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[4] = x2;
      goto L1805;
    }
  goto L5929;

 L1805: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 3);
  if (GET_CODE (x1) == SET)
    goto L1806;
  goto L5929;

 L1806: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1807;
  goto L5929;

 L1807: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1808;
  goto L5929;

 L1808: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[2]))
    goto L1809;
  goto L5929;

 L1809: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 8)
    goto L1810;
  goto L5929;

 L1810: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[5] = x2;
      goto L1811;
    }
  goto L5929;

 L1811: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 4);
  if (GET_CODE (x1) == SET)
    goto L1812;
  goto L5929;

 L1812: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1813;
  goto L5929;

 L1813: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1814;
  goto L5929;

 L1814: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[2]))
    goto L1815;
  goto L5929;

 L1815: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 12)
    goto L1816;
  goto L5929;

 L1816: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[6] = x2;
      goto L1817;
    }
  goto L5929;

 L1817: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && XVECLEN (operands[0], 0) == 5))
    {
      return 207;
    }
  goto L5929;

 L1819: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  if (GET_CODE (x1) == SET)
    goto L1820;
  goto L5930;

 L1820: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode)
    goto L6012;
  goto L5930;

 L6012: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == MEM)
    goto L1861;
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L1821;
    }
  goto L5930;

 L1861: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L1862;
    }
  goto L5930;

 L1862: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L1863;
    }
  goto L5930;

 L1863: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L1864;
  goto L5930;

 L1864: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1865;
  goto L5930;

 L1865: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1866;
  goto L5930;

 L1866: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[1]))
    goto L1867;
  goto L5930;

 L1867: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 4)
    goto L1868;
  goto L5930;

 L1868: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[3] = x2;
      goto L1869;
    }
  goto L5930;

 L1869: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 2);
  if (GET_CODE (x1) == SET)
    goto L1870;
  goto L5930;

 L1870: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1871;
  goto L5930;

 L1871: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1872;
  goto L5930;

 L1872: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[1]))
    goto L1873;
  goto L5930;

 L1873: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 8)
    goto L1874;
  goto L5930;

 L1874: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[4] = x2;
      goto L1875;
    }
  goto L5930;

 L1875: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 3);
  if (GET_CODE (x1) == SET)
    goto L1876;
  goto L5930;

 L1876: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1877;
  goto L5930;

 L1877: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1878;
  goto L5930;

 L1878: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[1]))
    goto L1879;
  goto L5930;

 L1879: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 12)
    goto L1880;
  goto L5930;

 L1880: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[5] = x2;
      goto L1881;
    }
  goto L5930;

 L1881: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && XVECLEN (operands[0], 0) == 4))
    {
      return 210;
    }
  goto L5930;

 L1821: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L1822;
  goto L5930;

 L1822: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L1823;
    }
  goto L5930;

 L1823: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 12)
    goto L1824;
  goto L5930;

 L1824: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L1825;
  goto L5930;

 L1825: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1826;
  goto L5930;

 L1826: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[2]))
    goto L1827;
  goto L5930;

 L1827: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[3] = x2;
      goto L1828;
    }
  goto L5930;

 L1828: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 2);
  if (GET_CODE (x1) == SET)
    goto L1829;
  goto L5930;

 L1829: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1830;
  goto L5930;

 L1830: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1831;
  goto L5930;

 L1831: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[2]))
    goto L1832;
  goto L5930;

 L1832: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 4)
    goto L1833;
  goto L5930;

 L1833: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[4] = x2;
      goto L1834;
    }
  goto L5930;

 L1834: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 3);
  if (GET_CODE (x1) == SET)
    goto L1835;
  goto L5930;

 L1835: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1836;
  goto L5930;

 L1836: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1837;
  goto L5930;

 L1837: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[2]))
    goto L1838;
  goto L5930;

 L1838: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 8)
    goto L1839;
  goto L5930;

 L1839: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[5] = x2;
      goto L1840;
    }
  goto L5930;

 L1840: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && XVECLEN (operands[0], 0) == 4))
    {
      return 208;
    }
  goto L5930;

 L1842: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  if (GET_CODE (x1) == SET)
    goto L1843;
  goto L5931;

 L1843: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode)
    goto L6014;
  goto L5931;

 L6014: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == MEM)
    goto L1885;
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L1844;
    }
  goto L5931;

 L1885: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L1886;
    }
  goto L5931;

 L1886: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L1887;
    }
  goto L5931;

 L1887: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L1888;
  goto L5931;

 L1888: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1889;
  goto L5931;

 L1889: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1890;
  goto L5931;

 L1890: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[1]))
    goto L1891;
  goto L5931;

 L1891: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 4)
    goto L1892;
  goto L5931;

 L1892: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[3] = x2;
      goto L1893;
    }
  goto L5931;

 L1893: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 2);
  if (GET_CODE (x1) == SET)
    goto L1894;
  goto L5931;

 L1894: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1895;
  goto L5931;

 L1895: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1896;
  goto L5931;

 L1896: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[1]))
    goto L1897;
  goto L5931;

 L1897: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 8)
    goto L1898;
  goto L5931;

 L1898: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[4] = x2;
      goto L1899;
    }
  goto L5931;

 L1899: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && XVECLEN (operands[0], 0) == 3))
    {
      return 211;
    }
  goto L5931;

 L1844: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L1845;
  goto L5931;

 L1845: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L1846;
    }
  goto L5931;

 L1846: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 8)
    goto L1847;
  goto L5931;

 L1847: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L1848;
  goto L5931;

 L1848: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1849;
  goto L5931;

 L1849: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[2]))
    goto L1850;
  goto L5931;

 L1850: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[3] = x2;
      goto L1851;
    }
  goto L5931;

 L1851: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 2);
  if (GET_CODE (x1) == SET)
    goto L1852;
  goto L5931;

 L1852: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1853;
  goto L5931;

 L1853: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1854;
  goto L5931;

 L1854: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[2]))
    goto L1855;
  goto L5931;

 L1855: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 4)
    goto L1856;
  goto L5931;

 L1856: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[4] = x2;
      goto L1857;
    }
  goto L5931;

 L1857: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && XVECLEN (operands[0], 0) == 3))
    {
      return 209;
    }
  goto L5931;

 L1901: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  if (GET_CODE (x1) == SET)
    goto L1902;
  goto L5936;

 L1902: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1903;
  goto L5936;

 L1903: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L1904;
    }
  goto L5936;

 L1904: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L1905;
    }
  goto L5936;

 L1905: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET)
    goto L1906;
  goto L5936;

 L1906: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L1907;
  goto L5936;

 L1907: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L1908;
  goto L5936;

 L1908: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[1]))
    goto L1909;
  goto L5936;

 L1909: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 4)
    goto L1910;
  goto L5936;

 L1910: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_hard_register_operand (x2, SImode))
    {
      operands[3] = x2;
      goto L1911;
    }
  goto L5936;

 L1911: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && XVECLEN (operands[0], 0) == 2))
    {
      return 212;
    }
  goto L5936;

 L2280: ATTRIBUTE_UNUSED_LABEL
  tem = recog_3 (x0, insn, pnum_clobbers);
  if (tem >= 0)
    return tem;
  goto L5942;

 L2342: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  switch (GET_CODE (x1))
    {
    case SET:
      goto L2343;
    case CALL:
      goto L2404;
    default:
     break;
   }
  goto L5942;

 L2343: ATTRIBUTE_UNUSED_LABEL
  tem = recog_6 (x0, insn, pnum_clobbers);
  if (tem >= 0)
    return tem;
  goto L5942;

 L2404: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MEM)
    goto L2405;
  goto L5942;

 L2405: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  operands[0] = x3;
  goto L2406;

 L2406: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  operands[1] = x2;
  goto L2407;

 L2407: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == USE)
    goto L2408;
  goto L5942;

 L2408: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  operands[2] = x2;
  goto L2409;

 L2409: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM
   && (GET_CODE (operands[0]) == SYMBOL_REF)
   && !arm_is_longcall_p (operands[0], INTVAL (operands[2]), 1))
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 253;
    }
 L2447: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB
   && operands[2] == const0_rtx && (GET_CODE (operands[0]) == SYMBOL_REF))
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 255;
    }
  goto L5942;

 L3756: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  if (GET_CODE (x1) == SET)
    goto L3757;
  goto ret0;

 L3757: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (memory_operand (x2, BLKmode))
    {
      operands[0] = x2;
      goto L3758;
    }
  goto ret0;

 L3758: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == BLKmode
      && GET_CODE (x2) == UNSPEC
      && XVECLEN (x2, 0) == 1
      && XINT (x2, 1) == 2)
    goto L3759;
  goto ret0;

 L3759: ATTRIBUTE_UNUSED_LABEL
  x3 = XVECEXP (x2, 0, 0);
  switch (GET_MODE (x3))
    {
    case SImode:
      goto L6091;
    case XFmode:
      goto L6092;
    default:
      break;
    }
  goto ret0;

 L6091: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L3760;
    }
  goto ret0;

 L3760: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 348;
    }
  goto ret0;

 L6092: ATTRIBUTE_UNUSED_LABEL
  if (f_register_operand (x3, XFmode))
    {
      operands[1] = x3;
      goto L3766;
    }
  goto ret0;

 L3766: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 349;
    }
  goto ret0;
 ret0:
  return -1;
}

static int recog_8 PARAMS ((rtx, rtx, int *));
static int
recog_8 (x0, insn, pnum_clobbers)
     rtx x0 ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  rtx * const operands ATTRIBUTE_UNUSED = &recog_data.operand[0];
  rtx x1 ATTRIBUTE_UNUSED;
  rtx x2 ATTRIBUTE_UNUSED;
  rtx x3 ATTRIBUTE_UNUSED;
  rtx x4 ATTRIBUTE_UNUSED;
  rtx x5 ATTRIBUTE_UNUSED;
  rtx x6 ATTRIBUTE_UNUSED;
  rtx x7 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;

  x1 = XEXP (x0, 1);
  switch (GET_CODE (x1))
    {
    case PLUS:
      goto L25;
    case MINUS:
      goto L261;
    case MULT:
      goto L535;
    case AND:
      goto L757;
    case IOR:
      goto L917;
    case XOR:
      goto L980;
    case NEG:
      goto L1239;
    case NOT:
      goto L1355;
    case ZERO_EXTEND:
      goto L1442;
    case SIGN_EXTEND:
      goto L1452;
    default:
     break;
   }
  goto ret0;

 L25: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == DImode)
    goto L6146;
  goto ret0;

 L6146: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x2))
    {
    case SIGN_EXTEND:
      goto L41;
    case ZERO_EXTEND:
      goto L57;
    case MULT:
      goto L526;
    case SUBREG:
    case REG:
      goto L6145;
    default:
      goto ret0;
   }
 L6145: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L26;
    }
  goto ret0;

 L41: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L42;
    }
  goto ret0;

 L42: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L43;
    }
  goto ret0;

 L43: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 2;
    }
  goto ret0;

 L57: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L58;
    }
  goto ret0;

 L58: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L59;
    }
  goto ret0;

 L59: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 3;
    }
  goto ret0;

 L526: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == DImode)
    goto L6149;
  goto ret0;

 L6149: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x3))
    {
    case SIGN_EXTEND:
      goto L527;
    case ZERO_EXTEND:
      goto L553;
    default:
     break;
   }
  goto ret0;

 L527: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L528;
    }
  goto ret0;

 L528: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == DImode
      && GET_CODE (x3) == SIGN_EXTEND)
    goto L529;
  goto ret0;

 L529: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[3] = x4;
      goto L530;
    }
  goto ret0;

 L530: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L531;
    }
  goto ret0;

 L531: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && arm_fast_multiply))
    {
      return 50;
    }
  goto ret0;

 L553: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L554;
    }
  goto ret0;

 L554: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == DImode
      && GET_CODE (x3) == ZERO_EXTEND)
    goto L555;
  goto ret0;

 L555: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[3] = x4;
      goto L556;
    }
  goto ret0;

 L556: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L557;
    }
  goto ret0;

 L557: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && arm_fast_multiply))
    {
      return 53;
    }
  goto ret0;

 L26: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == DImode)
    goto L6152;
  goto ret0;

 L6152: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == MULT)
    goto L629;
  if (s_register_operand (x2, DImode))
    {
      operands[2] = x2;
      goto L27;
    }
  goto ret0;

 L629: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == DImode
      && GET_CODE (x3) == SIGN_EXTEND)
    goto L630;
  goto ret0;

 L630: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, HImode))
    {
      operands[2] = x4;
      goto L631;
    }
  goto ret0;

 L631: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == DImode
      && GET_CODE (x3) == SIGN_EXTEND)
    goto L632;
  goto ret0;

 L632: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, HImode))
    {
      operands[3] = x4;
      goto L633;
    }
  goto ret0;

 L633: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && arm_is_xscale))
    {
      return 58;
    }
  goto ret0;

 L27: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 1;
    }
  goto ret0;

 L261: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == DImode)
    goto L6154;
  goto ret0;

 L6154: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x2))
    {
    case ZERO_EXTEND:
      goto L323;
    case SIGN_EXTEND:
      goto L339;
    case SUBREG:
    case REG:
      goto L6153;
    default:
      goto ret0;
   }
 L6153: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L262;
    }
  goto ret0;

 L323: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode)
    goto L6156;
  goto ret0;

 L6156: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L324;
    }
 L6157: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L357;
    }
  goto ret0;

 L324: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L325;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6157;

 L325: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 30;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6157;

 L357: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == DImode
      && GET_CODE (x2) == ZERO_EXTEND)
    goto L358;
  goto ret0;

 L358: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L359;
    }
  goto ret0;

 L359: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 32;
    }
  goto ret0;

 L339: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L340;
    }
  goto ret0;

 L340: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L341;
    }
  goto ret0;

 L341: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 31;
    }
  goto ret0;

 L262: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == DImode)
    goto L6159;
  goto ret0;

 L6159: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x2))
    {
    case ZERO_EXTEND:
      goto L292;
    case SIGN_EXTEND:
      goto L308;
    case SUBREG:
    case REG:
      goto L6158;
    default:
      goto ret0;
   }
 L6158: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, DImode))
    {
      operands[2] = x2;
      goto L263;
    }
  goto ret0;

 L292: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L293;
    }
  goto ret0;

 L293: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 28;
    }
  goto ret0;

 L308: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L309;
    }
  goto ret0;

 L309: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 29;
    }
  goto ret0;

 L263: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 26;
    }
  goto ret0;

 L535: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == DImode)
    goto L6161;
  goto ret0;

 L6161: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x2))
    {
    case SIGN_EXTEND:
      goto L536;
    case ZERO_EXTEND:
      goto L544;
    default:
     break;
   }
  goto ret0;

 L536: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L537;
    }
  goto ret0;

 L537: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == DImode
      && GET_CODE (x2) == SIGN_EXTEND)
    goto L538;
  goto ret0;

 L538: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L539;
    }
  goto ret0;

 L539: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && arm_fast_multiply))
    {
      return 51;
    }
  goto ret0;

 L544: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L545;
    }
  goto ret0;

 L545: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == DImode
      && GET_CODE (x2) == ZERO_EXTEND)
    goto L546;
  goto ret0;

 L546: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L547;
    }
  goto ret0;

 L547: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && arm_fast_multiply))
    {
      return 52;
    }
  goto ret0;

 L757: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == DImode)
    goto L6164;
  goto ret0;

 L6164: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x2))
    {
    case ZERO_EXTEND:
      goto L764;
    case SIGN_EXTEND:
      goto L771;
    case NOT:
      goto L837;
    case SUBREG:
    case REG:
      goto L6163;
    default:
      goto ret0;
   }
 L6163: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L758;
    }
  goto ret0;

 L764: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L765;
    }
  goto ret0;

 L765: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L766;
    }
  goto ret0;

 L766: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 78;
    }
  goto ret0;

 L771: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L772;
    }
  goto ret0;

 L772: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L773;
    }
  goto ret0;

 L773: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 79;
    }
  goto ret0;

 L837: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == DImode)
    goto L6168;
  goto ret0;

 L6168: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x3))
    {
    case ZERO_EXTEND:
      goto L845;
    case SIGN_EXTEND:
      goto L853;
    case SUBREG:
    case REG:
      goto L6167;
    default:
      goto ret0;
   }
 L6167: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x3, DImode))
    {
      operands[1] = x3;
      goto L838;
    }
  goto ret0;

 L845: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L846;
    }
  goto ret0;

 L846: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L847;
    }
  goto ret0;

 L847: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 87;
    }
  goto ret0;

 L853: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L854;
    }
  goto ret0;

 L854: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L855;
    }
  goto ret0;

 L855: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 88;
    }
  goto ret0;

 L838: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      operands[2] = x2;
      goto L839;
    }
  goto ret0;

 L839: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 86;
    }
  goto ret0;

 L758: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      operands[2] = x2;
      goto L759;
    }
  goto ret0;

 L759: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 77;
    }
  goto ret0;

 L917: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == DImode)
    goto L6171;
  goto ret0;

 L6171: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x2))
    {
    case ZERO_EXTEND:
      goto L924;
    case SIGN_EXTEND:
      goto L931;
    case SUBREG:
    case REG:
      goto L6170;
    default:
      goto ret0;
   }
 L6170: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L918;
    }
  goto ret0;

 L924: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L925;
    }
  goto ret0;

 L925: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L926;
    }
  goto ret0;

 L926: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 95;
    }
  goto ret0;

 L931: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L932;
    }
  goto ret0;

 L932: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L933;
    }
  goto ret0;

 L933: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 96;
    }
  goto ret0;

 L918: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      operands[2] = x2;
      goto L919;
    }
  goto ret0;

 L919: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 94;
    }
  goto ret0;

 L980: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == DImode)
    goto L6174;
  goto ret0;

 L6174: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x2))
    {
    case ZERO_EXTEND:
      goto L987;
    case SIGN_EXTEND:
      goto L994;
    case SUBREG:
    case REG:
      goto L6173;
    default:
      goto ret0;
   }
 L6173: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L981;
    }
  goto ret0;

 L987: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L988;
    }
  goto ret0;

 L988: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L989;
    }
  goto ret0;

 L989: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 102;
    }
  goto ret0;

 L994: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L995;
    }
  goto ret0;

 L995: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L996;
    }
  goto ret0;

 L996: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 103;
    }
  goto ret0;

 L981: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      operands[2] = x2;
      goto L982;
    }
  goto ret0;

 L982: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 101;
    }
  goto ret0;

 L1239: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L1240;
    }
  goto ret0;

 L1240: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 125;
    }
  goto ret0;

 L1355: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L1356;
    }
  goto ret0;

 L1356: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 143;
    }
  goto ret0;

 L1442: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  switch (GET_MODE (x2))
    {
    case SImode:
      goto L6176;
    case QImode:
      goto L6177;
    default:
      break;
    }
  goto ret0;

 L6176: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L1443;
    }
  goto ret0;

 L1443: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 157;
    }
  goto ret0;

 L6177: ATTRIBUTE_UNUSED_LABEL
  if (nonimmediate_operand (x2, QImode))
    {
      operands[1] = x2;
      goto L1448;
    }
  goto ret0;

 L1448: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 158;
    }
  goto ret0;

 L1452: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L1453;
    }
  goto ret0;

 L1453: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 159;
    }
  goto ret0;
 ret0:
  return -1;
}

static int recog_9 PARAMS ((rtx, rtx, int *));
static int
recog_9 (x0, insn, pnum_clobbers)
     rtx x0 ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  rtx * const operands ATTRIBUTE_UNUSED = &recog_data.operand[0];
  rtx x1 ATTRIBUTE_UNUSED;
  rtx x2 ATTRIBUTE_UNUSED;
  rtx x3 ATTRIBUTE_UNUSED;
  rtx x4 ATTRIBUTE_UNUSED;
  rtx x5 ATTRIBUTE_UNUSED;
  rtx x6 ATTRIBUTE_UNUSED;
  rtx x7 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;

  x1 = XEXP (x0, 1);
  switch (GET_CODE (x1))
    {
    case MINUS:
      goto L388;
    case MULT:
      goto L435;
    case PLUS:
      goto L478;
    case TRUNCATE:
      goto L575;
    case AND:
      goto L777;
    case NE:
      goto L828;
    case IOR:
      goto L937;
    case XOR:
      goto L1000;
    case SMAX:
      goto L1049;
    case SMIN:
      goto L1063;
    case UMAX:
      goto L1077;
    case UMIN:
      goto L1091;
    default:
     break;
   }
 L6202: ATTRIBUTE_UNUSED_LABEL
  if (shiftable_operator (x1, SImode))
    {
      operands[4] = x1;
      goto L1121;
    }
  goto ret0;

 L388: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L389;
    }
 L369: ATTRIBUTE_UNUSED_LABEL
  if (reg_or_int_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L370;
    }
  goto L6202;

 L389: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_comparison_operator (x2, SImode))
    {
      operands[2] = x2;
      goto L390;
    }
  x2 = XEXP (x1, 0);
  goto L369;

 L390: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (cc_register (x3, VOIDmode))
    {
      operands[3] = x3;
      goto L391;
    }
  x2 = XEXP (x1, 0);
  goto L369;

 L391: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0
      && (TARGET_ARM))
    {
      return 36;
    }
  x2 = XEXP (x1, 0);
  goto L369;

 L370: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L371;
    }
  goto L6202;

 L371: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 34;
    }
  x1 = XEXP (x0, 1);
  goto L6202;

 L435: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode)
    goto L6204;
  goto ret0;

 L6204: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == SIGN_EXTEND)
    goto L610;
  if (s_register_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L436;
    }
  goto ret0;

 L610: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, HImode))
    {
      operands[1] = x3;
      goto L611;
    }
  goto ret0;

 L611: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == SIGN_EXTEND)
    goto L612;
  goto ret0;

 L612: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, HImode))
    {
      operands[2] = x3;
      goto L613;
    }
  goto ret0;

 L613: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && arm_is_xscale))
    {
      return 56;
    }
  goto ret0;

 L436: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L437;
    }
  goto ret0;

 L437: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 43;
    }
  goto ret0;

 L478: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode)
    goto L6205;
  goto L6202;

 L6205: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == MULT)
    goto L479;
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L618;
    }
  goto L6202;

 L479: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L480;
    }
  goto L6202;

 L480: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L481;
    }
  goto L6202;

 L481: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      operands[3] = x2;
      goto L482;
    }
  goto L6202;

 L482: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 47;
    }
  x1 = XEXP (x0, 1);
  goto L6202;

 L618: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MULT)
    goto L619;
  goto L6202;

 L619: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == SIGN_EXTEND)
    goto L620;
  goto L6202;

 L620: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, HImode))
    {
      operands[2] = x4;
      goto L621;
    }
  goto L6202;

 L621: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == SIGN_EXTEND)
    goto L622;
  goto L6202;

 L622: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, HImode))
    {
      operands[3] = x4;
      goto L623;
    }
  goto L6202;

 L623: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && arm_is_xscale))
    {
      return 57;
    }
  x1 = XEXP (x0, 1);
  goto L6202;

 L575: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == DImode
      && GET_CODE (x2) == LSHIFTRT)
    goto L576;
  goto ret0;

 L576: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == DImode
      && GET_CODE (x3) == MULT)
    goto L577;
  goto ret0;

 L577: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == DImode)
    goto L6207;
  goto ret0;

 L6207: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x4))
    {
    case SIGN_EXTEND:
      goto L578;
    case ZERO_EXTEND:
      goto L602;
    default:
     break;
   }
  goto ret0;

 L578: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[1] = x5;
      goto L579;
    }
  goto ret0;

 L579: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_MODE (x4) == DImode
      && GET_CODE (x4) == SIGN_EXTEND)
    goto L580;
  goto ret0;

 L580: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[2] = x5;
      goto L581;
    }
  goto ret0;

 L581: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 32
      && (TARGET_ARM && arm_fast_multiply)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 54;
    }
  goto ret0;

 L602: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[1] = x5;
      goto L603;
    }
  goto ret0;

 L603: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_MODE (x4) == DImode
      && GET_CODE (x4) == ZERO_EXTEND)
    goto L604;
  goto ret0;

 L604: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[2] = x5;
      goto L605;
    }
  goto ret0;

 L605: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 32
      && (TARGET_ARM && arm_fast_multiply)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 55;
    }
  goto ret0;

 L777: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode)
    goto L6210;
  goto L6202;

 L6210: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x2))
    {
    case NOT:
      goto L860;
    case IOR:
      goto L1033;
    case SUBREG:
    case REG:
      goto L6209;
    default:
      goto L6202;
   }
 L6209: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L778;
    }
  goto L6202;

 L860: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode)
    goto L6212;
  goto L6202;

 L6212: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L861;
    }
  if (shift_operator (x3, SImode))
    {
      operands[4] = x3;
      goto L875;
    }
  goto L6202;

 L861: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L862;
    }
  goto L6202;

 L862: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 89;
    }
  x1 = XEXP (x0, 1);
  goto L6202;

 L875: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L876;
    }
  goto L6202;

 L876: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      operands[3] = x4;
      goto L877;
    }
  goto L6202;

 L877: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L878;
    }
  goto L6202;

 L878: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 91;
    }
  x1 = XEXP (x0, 1);
  goto L6202;

 L1033: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L1034;
    }
  goto L6202;

 L1034: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L1035;
    }
  goto L6202;

 L1035: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == NOT)
    goto L1036;
  goto L6202;

 L1036: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L1037;
    }
  goto L6202;

 L1037: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 108;
    }
  x1 = XEXP (x0, 1);
  goto L6202;

 L778: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (reg_or_int_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L779;
    }
  goto L6202;

 L779: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 80;
    }
  x1 = XEXP (x0, 1);
  goto L6202;

 L828: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == ZERO_EXTRACT)
    goto L829;
  goto ret0;

 L829: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L830;
    }
  goto ret0;

 L830: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (const_int_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L831;
    }
  goto ret0;

 L831: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (const_int_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L832;
    }
  goto ret0;

 L832: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 0
      && (TARGET_ARM
   && (INTVAL (operands[3]) >= 0 && INTVAL (operands[3]) < 32
       && INTVAL (operands[2]) > 0 
       && INTVAL (operands[2]) + (INTVAL (operands[3]) & 1) <= 8
       && INTVAL (operands[2]) + INTVAL (operands[3]) <= 32)))
    {
      return 85;
    }
  goto ret0;

 L937: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L938;
    }
  goto L6202;

 L938: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (reg_or_int_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L939;
    }
  goto L6202;

 L939: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 97;
    }
  x1 = XEXP (x0, 1);
  goto L6202;

 L1000: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L1001;
    }
  goto L6202;

 L1001: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_rhs_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L1002;
    }
  goto L6202;

 L1002: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 104;
    }
  x1 = XEXP (x0, 1);
  goto L6202;

 L1049: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L1050;
    }
  goto ret0;

 L1050: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_rhs_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L1051;
    }
  goto ret0;

 L1051: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 109;
    }
  goto ret0;

 L1063: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L1064;
    }
  goto ret0;

 L1064: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_rhs_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L1065;
    }
  goto ret0;

 L1065: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 110;
    }
  goto ret0;

 L1077: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L1078;
    }
  goto ret0;

 L1078: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_rhs_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L1079;
    }
  goto ret0;

 L1079: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 111;
    }
  goto ret0;

 L1091: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L1092;
    }
  goto ret0;

 L1092: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_rhs_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L1093;
    }
  goto ret0;

 L1093: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 112;
    }
  goto ret0;

 L1121: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (minmax_operator (x2, SImode))
    {
      operands[5] = x2;
      goto L1122;
    }
  goto ret0;

 L1122: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L1123;
    }
  goto ret0;

 L1123: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L1124;
    }
  goto ret0;

 L1124: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L1125;
    }
  goto ret0;

 L1125: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM
   && (GET_CODE (operands[1]) != REG
       || (REGNO(operands[1]) != FRAME_POINTER_REGNUM
           && REGNO(operands[1]) != ARG_POINTER_REGNUM)))
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 114;
    }
  goto ret0;
 ret0:
  return -1;
}

static int recog_10 PARAMS ((rtx, rtx, int *));
static int
recog_10 (x0, insn, pnum_clobbers)
     rtx x0 ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  rtx * const operands ATTRIBUTE_UNUSED = &recog_data.operand[0];
  rtx x1 ATTRIBUTE_UNUSED;
  rtx x2 ATTRIBUTE_UNUSED;
  rtx x3 ATTRIBUTE_UNUSED;
  rtx x4 ATTRIBUTE_UNUSED;
  rtx x5 ATTRIBUTE_UNUSED;
  rtx x6 ATTRIBUTE_UNUSED;
  rtx x7 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;

  x1 = XEXP (x0, 0);
  if (s_register_operand (x1, SImode))
    {
      operands[0] = x1;
      goto L62;
    }
 L6096: ATTRIBUTE_UNUSED_LABEL
  if (register_operand (x1, SImode))
    {
      operands[0] = x1;
      goto L68;
    }
 L6104: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x1, SImode))
    {
      operands[0] = x1;
      goto L368;
    }
 L6105: ATTRIBUTE_UNUSED_LABEL
  if (register_operand (x1, SImode))
    {
      operands[0] = x1;
      goto L440;
    }
 L6106: ATTRIBUTE_UNUSED_LABEL
  if (memory_operand (x1, SImode))
    {
      operands[0] = x1;
      goto L1104;
    }
 L6107: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x1, SImode))
    {
      operands[0] = x1;
      goto L1152;
    }
 L6108: ATTRIBUTE_UNUSED_LABEL
  if (register_operand (x1, SImode))
    {
      operands[0] = x1;
      goto L1260;
    }
 L6109: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x1, SImode))
    {
      operands[0] = x1;
      goto L1461;
    }
 L6112: ATTRIBUTE_UNUSED_LABEL
  if (register_operand (x1, SImode))
    {
      operands[0] = x1;
      goto L1509;
    }
 L6115: ATTRIBUTE_UNUSED_LABEL
  if (nonimmediate_operand (x1, SImode))
    {
      operands[0] = x1;
      goto L1537;
    }
 L6116: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x1, SImode))
    {
      operands[0] = x1;
      goto L1549;
    }
  goto ret0;

 L62: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == SImode
      && GET_CODE (x1) == PLUS)
    goto L63;
  x1 = XEXP (x0, 0);
  goto L6096;

 L63: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode)
    goto L6181;
  x1 = XEXP (x0, 0);
  goto L6096;

 L6181: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x2))
    {
    case LTU:
      goto L155;
    case PLUS:
      goto L177;
    case SUBREG:
    case REG:
      goto L6180;
    default:
      goto L6183;
   }
 L6180: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L64;
    }
 L6183: ATTRIBUTE_UNUSED_LABEL
  if (arm_comparison_operator (x2, SImode))
    {
      operands[2] = x2;
      goto L206;
    }
  x1 = XEXP (x0, 0);
  goto L6096;

 L155: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == CC_Cmode
      && GET_CODE (x3) == REG
      && XINT (x3, 0) == 24)
    goto L156;
  goto L6183;

 L156: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L157;
  goto L6183;

 L157: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L158;
  x2 = XEXP (x1, 0);
  goto L6183;

 L158: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode)
    goto L6184;
  x2 = XEXP (x1, 0);
  goto L6183;

 L6184: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L159;
    }
  if (shift_operator (x3, SImode))
    {
      operands[2] = x3;
      goto L169;
    }
  x2 = XEXP (x1, 0);
  goto L6183;

 L159: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L160;
    }
  x2 = XEXP (x1, 0);
  goto L6183;

 L160: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 14;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6183;

 L169: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[3] = x4;
      goto L170;
    }
  x2 = XEXP (x1, 0);
  goto L6183;

 L170: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (reg_or_int_operand (x4, SImode))
    {
      operands[4] = x4;
      goto L171;
    }
  x2 = XEXP (x1, 0);
  goto L6183;

 L171: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L172;
    }
  x2 = XEXP (x1, 0);
  goto L6183;

 L172: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 15;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6183;

 L177: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode)
    goto L6187;
  x1 = XEXP (x0, 0);
  goto L6096;

 L6187: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == LTU)
    goto L187;
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L178;
    }
  x1 = XEXP (x0, 0);
  goto L6096;

 L187: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == CC_Cmode
      && GET_CODE (x4) == REG
      && XINT (x4, 0) == 24)
    goto L188;
  x1 = XEXP (x0, 0);
  goto L6096;

 L188: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 0)
    goto L189;
  x1 = XEXP (x0, 0);
  goto L6096;

 L189: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L190;
    }
 L199: ATTRIBUTE_UNUSED_LABEL
  if (arm_rhs_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L200;
    }
  x1 = XEXP (x0, 0);
  goto L6096;

 L190: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_rhs_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L191;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 1);
  goto L199;

 L191: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 17;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 1);
  goto L199;

 L200: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L201;
    }
  x1 = XEXP (x0, 0);
  goto L6096;

 L201: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 18;
    }
  x1 = XEXP (x0, 0);
  goto L6096;

 L178: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L179;
    }
  x1 = XEXP (x0, 0);
  goto L6096;

 L179: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == LTU)
    goto L180;
  x1 = XEXP (x0, 0);
  goto L6096;

 L180: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == CC_Cmode
      && GET_CODE (x3) == REG
      && XINT (x3, 0) == 24)
    goto L181;
  x1 = XEXP (x0, 0);
  goto L6096;

 L181: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0
      && (TARGET_ARM))
    {
      return 16;
    }
  x1 = XEXP (x0, 0);
  goto L6096;

 L64: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (reg_or_int_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L65;
    }
  x1 = XEXP (x0, 0);
  goto L6096;

 L65: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 4;
    }
  x1 = XEXP (x0, 0);
  goto L6096;

 L206: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (cc_register (x3, CCmode))
    {
      operands[3] = x3;
      goto L207;
    }
  x1 = XEXP (x0, 0);
  goto L6096;

 L207: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L208;
  x1 = XEXP (x0, 0);
  goto L6096;

 L208: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L209;
    }
  x1 = XEXP (x0, 0);
  goto L6096;

 L209: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 19;
    }
  x1 = XEXP (x0, 0);
  goto L6096;

 L68: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == SImode)
    goto L6188;
  x1 = XEXP (x0, 0);
  goto L6104;

 L6188: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x1))
    {
    case PLUS:
      goto L69;
    case MINUS:
      goto L363;
    default:
     break;
   }
  x1 = XEXP (x0, 0);
  goto L6104;

 L69: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L70;
    }
  x1 = XEXP (x0, 0);
  goto L6104;

 L70: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (nonmemory_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L71;
    }
  x1 = XEXP (x0, 0);
  goto L6104;

 L71: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB))
    {
      return 5;
    }
  x1 = XEXP (x0, 0);
  goto L6104;

 L363: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L364;
    }
  x1 = XEXP (x0, 0);
  goto L6104;

 L364: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (register_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L365;
    }
  x1 = XEXP (x0, 0);
  goto L6104;

 L365: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB))
    {
      return 33;
    }
  x1 = XEXP (x0, 0);
  goto L6104;

 L368: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == SImode)
    goto L6190;
  x1 = XEXP (x0, 0);
  goto L6105;

 L6190: ATTRIBUTE_UNUSED_LABEL
  tem = recog_9 (x0, insn, pnum_clobbers);
  if (tem >= 0)
    return tem;
  x1 = XEXP (x0, 0);
  goto L6105;

 L440: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == SImode)
    goto L6214;
  x1 = XEXP (x0, 0);
  goto L6106;

 L6214: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x1))
    {
    case MULT:
      goto L441;
    case AND:
      goto L783;
    case IOR:
      goto L943;
    case XOR:
      goto L1006;
    case ASHIFT:
      goto L1129;
    case ASHIFTRT:
      goto L1135;
    case LSHIFTRT:
      goto L1141;
    case ROTATERT:
      goto L1147;
    default:
     break;
   }
  x1 = XEXP (x0, 0);
  goto L6106;

 L441: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L442;
    }
  x1 = XEXP (x0, 0);
  goto L6106;

 L442: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (register_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L443;
    }
  x1 = XEXP (x0, 0);
  goto L6106;

 L443: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB))
    {
      return 44;
    }
  x1 = XEXP (x0, 0);
  goto L6106;

 L783: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode)
    goto L6223;
  x1 = XEXP (x0, 0);
  goto L6106;

 L6223: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == NOT)
    goto L867;
  if (register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L784;
    }
  x1 = XEXP (x0, 0);
  goto L6106;

 L867: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L868;
    }
  x1 = XEXP (x0, 0);
  goto L6106;

 L868: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (register_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L869;
    }
  x1 = XEXP (x0, 0);
  goto L6106;

 L869: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB))
    {
      return 90;
    }
  x1 = XEXP (x0, 0);
  goto L6106;

 L784: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (register_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L785;
    }
  x1 = XEXP (x0, 0);
  goto L6106;

 L785: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB))
    {
      return 81;
    }
  x1 = XEXP (x0, 0);
  goto L6106;

 L943: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L944;
    }
  x1 = XEXP (x0, 0);
  goto L6106;

 L944: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (register_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L945;
    }
  x1 = XEXP (x0, 0);
  goto L6106;

 L945: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB))
    {
      return 98;
    }
  x1 = XEXP (x0, 0);
  goto L6106;

 L1006: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L1007;
    }
  x1 = XEXP (x0, 0);
  goto L6106;

 L1007: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (register_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L1008;
    }
  x1 = XEXP (x0, 0);
  goto L6106;

 L1008: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB))
    {
      return 105;
    }
  x1 = XEXP (x0, 0);
  goto L6106;

 L1129: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L1130;
    }
  x1 = XEXP (x0, 0);
  goto L6106;

 L1130: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (nonmemory_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L1131;
    }
  x1 = XEXP (x0, 0);
  goto L6106;

 L1131: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB))
    {
      return 115;
    }
  x1 = XEXP (x0, 0);
  goto L6106;

 L1135: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L1136;
    }
  x1 = XEXP (x0, 0);
  goto L6106;

 L1136: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (nonmemory_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L1137;
    }
  x1 = XEXP (x0, 0);
  goto L6106;

 L1137: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB))
    {
      return 116;
    }
  x1 = XEXP (x0, 0);
  goto L6106;

 L1141: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L1142;
    }
  x1 = XEXP (x0, 0);
  goto L6106;

 L1142: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (nonmemory_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L1143;
    }
  x1 = XEXP (x0, 0);
  goto L6106;

 L1143: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB))
    {
      return 117;
    }
  x1 = XEXP (x0, 0);
  goto L6106;

 L1147: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L1148;
    }
  x1 = XEXP (x0, 0);
  goto L6106;

 L1148: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (register_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L1149;
    }
  x1 = XEXP (x0, 0);
  goto L6106;

 L1149: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB))
    {
      return 118;
    }
  x1 = XEXP (x0, 0);
  goto L6106;

 L1104: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (minmax_operator (x1, SImode))
    {
      operands[3] = x1;
      goto L1105;
    }
  x1 = XEXP (x0, 0);
  goto L6107;

 L1105: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L1106;
    }
  x1 = XEXP (x0, 0);
  goto L6107;

 L1106: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L1107;
    }
  x1 = XEXP (x0, 0);
  goto L6107;

 L1107: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 113;
    }
  x1 = XEXP (x0, 0);
  goto L6107;

 L1152: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == SImode)
    goto L6225;
  x1 = XEXP (x0, 0);
  goto L6108;

 L6225: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x1))
    {
    case NOT:
      goto L1190;
    case NEG:
      goto L1256;
    case ABS:
      goto L1294;
    case FIX:
      goto L1412;
    case ASHIFT:
    case ASHIFTRT:
    case LSHIFTRT:
    case ROTATERT:
    case MULT:
      goto L6224;
    default:
      x1 = XEXP (x0, 0);
      goto L6108;
   }
 L6224: ATTRIBUTE_UNUSED_LABEL
  if (shift_operator (x1, SImode))
    {
      operands[3] = x1;
      goto L1153;
    }
  x1 = XEXP (x0, 0);
  goto L6108;

 L1190: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode)
    goto L6229;
  x1 = XEXP (x0, 0);
  goto L6108;

 L6229: ATTRIBUTE_UNUSED_LABEL
  if (shift_operator (x2, SImode))
    {
      operands[3] = x2;
      goto L1191;
    }
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L1361;
    }
  x1 = XEXP (x0, 0);
  goto L6108;

 L1191: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L1192;
    }
  x1 = XEXP (x0, 0);
  goto L6108;

 L1192: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L1193;
    }
  x1 = XEXP (x0, 0);
  goto L6108;

 L1193: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 122;
    }
  x1 = XEXP (x0, 0);
  goto L6108;

 L1361: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 144;
    }
  x1 = XEXP (x0, 0);
  goto L6108;

 L1256: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode)
    goto L6232;
  x1 = XEXP (x0, 0);
  goto L6108;

 L6232: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == ABS)
    goto L1308;
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L1257;
    }
  x1 = XEXP (x0, 0);
  goto L6108;

 L1308: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L1309;
    }
  x1 = XEXP (x0, 0);
  goto L6108;

 L1309: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 134;
    }
  x1 = XEXP (x0, 0);
  goto L6108;

 L1257: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 127;
    }
  x1 = XEXP (x0, 0);
  goto L6108;

 L1294: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L1295;
    }
  x1 = XEXP (x0, 0);
  goto L6108;

 L1295: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 133;
    }
  x1 = XEXP (x0, 0);
  goto L6108;

 L1412: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  switch (GET_MODE (x2))
    {
    case SFmode:
      goto L6233;
    case DFmode:
      goto L6234;
    case XFmode:
      goto L6235;
    default:
      break;
    }
  x1 = XEXP (x0, 0);
  goto L6108;

 L6233: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, SFmode))
    {
      operands[1] = x2;
      goto L1413;
    }
  x1 = XEXP (x0, 0);
  goto L6108;

 L1413: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 151;
    }
  x1 = XEXP (x0, 0);
  goto L6108;

 L6234: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, DFmode))
    {
      operands[1] = x2;
      goto L1418;
    }
  x1 = XEXP (x0, 0);
  goto L6108;

 L1418: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 152;
    }
  x1 = XEXP (x0, 0);
  goto L6108;

 L6235: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, XFmode))
    {
      operands[1] = x2;
      goto L1423;
    }
  x1 = XEXP (x0, 0);
  goto L6108;

 L1423: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT))
    {
      return 153;
    }
  x1 = XEXP (x0, 0);
  goto L6108;

 L1153: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L1154;
    }
  x1 = XEXP (x0, 0);
  goto L6108;

 L1154: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (reg_or_int_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L1155;
    }
  x1 = XEXP (x0, 0);
  goto L6108;

 L1155: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 119;
    }
  x1 = XEXP (x0, 0);
  goto L6108;

 L1260: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == SImode)
    goto L6236;
  x1 = XEXP (x0, 0);
  goto L6109;

 L6236: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x1))
    {
    case NEG:
      goto L1261;
    case NOT:
      goto L1365;
    case ZERO_EXTEND:
      goto L1457;
    case SIGN_EXTEND:
      goto L1490;
    default:
     break;
   }
  x1 = XEXP (x0, 0);
  goto L6109;

 L1261: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L1262;
    }
  x1 = XEXP (x0, 0);
  goto L6109;

 L1262: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB))
    {
      return 128;
    }
  x1 = XEXP (x0, 0);
  goto L6109;

 L1365: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L1366;
    }
  x1 = XEXP (x0, 0);
  goto L6109;

 L1366: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB))
    {
      return 145;
    }
  x1 = XEXP (x0, 0);
  goto L6109;

 L1457: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  switch (GET_MODE (x2))
    {
    case HImode:
      goto L6240;
    case QImode:
      goto L6241;
    default:
      break;
    }
  x1 = XEXP (x0, 0);
  goto L6109;

 L6240: ATTRIBUTE_UNUSED_LABEL
  if (memory_operand (x2, HImode))
    {
      operands[1] = x2;
      goto L1458;
    }
  x1 = XEXP (x0, 0);
  goto L6109;

 L1458: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB))
    {
      return 160;
    }
  x1 = XEXP (x0, 0);
  goto L6109;

 L6241: ATTRIBUTE_UNUSED_LABEL
  if (memory_operand (x2, QImode))
    {
      operands[1] = x2;
      goto L1468;
    }
  x1 = XEXP (x0, 0);
  goto L6109;

 L1468: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB))
    {
      return 162;
    }
  x1 = XEXP (x0, 0);
  goto L6109;

 L1490: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (memory_operand (x2, HImode))
    {
      operands[1] = x2;
      goto L1491;
    }
  x1 = XEXP (x0, 0);
  goto L6109;

 L1491: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 165;
    }
  x1 = XEXP (x0, 0);
  goto L6109;

 L1461: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == SImode)
    goto L6242;
  x1 = XEXP (x0, 0);
  goto L6112;

 L6242: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x1))
    {
    case ZERO_EXTEND:
      goto L1462;
    case SIGN_EXTEND:
      goto L1495;
    default:
     break;
   }
  x1 = XEXP (x0, 0);
  goto L6112;

 L1462: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  switch (GET_MODE (x2))
    {
    case HImode:
      goto L6244;
    case QImode:
      goto L6245;
    default:
      break;
    }
  x1 = XEXP (x0, 0);
  goto L6112;

 L6244: ATTRIBUTE_UNUSED_LABEL
  if (memory_operand (x2, HImode))
    {
      operands[1] = x2;
      goto L1463;
    }
  x1 = XEXP (x0, 0);
  goto L6112;

 L1463: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && arm_arch4))
    {
      return 161;
    }
  x1 = XEXP (x0, 0);
  goto L6112;

 L6245: ATTRIBUTE_UNUSED_LABEL
  if (memory_operand (x2, QImode))
    {
      operands[1] = x2;
      goto L1473;
    }
  x1 = XEXP (x0, 0);
  goto L6112;

 L1473: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 163;
    }
  x1 = XEXP (x0, 0);
  goto L6112;

 L1495: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  switch (GET_MODE (x2))
    {
    case HImode:
      goto L6246;
    case QImode:
      goto L6247;
    default:
      break;
    }
  x1 = XEXP (x0, 0);
  goto L6112;

 L6246: ATTRIBUTE_UNUSED_LABEL
  if (memory_operand (x2, HImode))
    {
      operands[1] = x2;
      goto L1496;
    }
  x1 = XEXP (x0, 0);
  goto L6112;

 L1496: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && arm_arch4))
    {
      return 166;
    }
  x1 = XEXP (x0, 0);
  goto L6112;

 L6247: ATTRIBUTE_UNUSED_LABEL
  if (memory_operand (x2, QImode))
    {
      operands[1] = x2;
      goto L1506;
    }
  x1 = XEXP (x0, 0);
  goto L6112;

 L1506: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && arm_arch4))
    {
      return 168;
    }
  x1 = XEXP (x0, 0);
  goto L6112;

 L1509: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == SImode
      && GET_CODE (x1) == SIGN_EXTEND)
    goto L1510;
  x1 = XEXP (x0, 0);
  goto L6115;

 L1510: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (memory_operand (x2, QImode))
    {
      operands[1] = x2;
      goto L1511;
    }
  x1 = XEXP (x0, 0);
  goto L6115;

 L1511: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB))
    {
      return 169;
    }
  x1 = XEXP (x0, 0);
  goto L6115;

 L1537: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (general_operand (x1, SImode))
    {
      operands[1] = x1;
      goto L1538;
    }
  x1 = XEXP (x0, 0);
  goto L6116;

 L1538: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM
   && (   register_operand (operands[0], SImode)
       || register_operand (operands[1], SImode))))
    {
      return 175;
    }
 L1542: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB
   && (   register_operand (operands[0], SImode) 
       || register_operand (operands[1], SImode))))
    {
      return 176;
    }
  x1 = XEXP (x0, 0);
  goto L6116;

 L1549: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == SImode)
    goto L6248;
 L1545: ATTRIBUTE_UNUSED_LABEL
  if (address_operand (x1, DImode))
    {
      operands[1] = x1;
      goto L1546;
    }
  goto ret0;

 L6248: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x1))
    {
    case UNSPEC:
      goto L6251;
    case ROTATE:
      goto L1605;
    default:
     break;
   }
  goto L1545;

 L6251: ATTRIBUTE_UNUSED_LABEL
  switch (XVECLEN (x1, 0))
    {
    case 1:
      goto L6253;
    case 2:
      goto L6254;
    default:
      break;
    }
  goto L1545;

 L6253: ATTRIBUTE_UNUSED_LABEL
  if (XINT (x1, 1) == 3)
    goto L1550;
  goto L1545;

 L1550: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 0);
  operands[1] = x2;
  goto L1551;

 L1551: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && flag_pic))
    {
      return 178;
    }
 L1556: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB && flag_pic))
    {
      return 179;
    }
  x1 = XEXP (x0, 1);
  goto L1545;

 L6254: ATTRIBUTE_UNUSED_LABEL
  if (XINT (x1, 1) == 3)
    goto L1560;
  goto L1545;

 L1560: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 0);
  operands[1] = x2;
  goto L1561;

 L1561: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (s_register_operand (x2, VOIDmode))
    {
      operands[2] = x2;
      goto L1562;
    }
  goto L1545;

 L1562: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_EITHER && flag_pic && operands[2] == pic_offset_table_rtx))
    {
      return 180;
    }
  x1 = XEXP (x0, 1);
  goto L1545;

 L1605: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode)
    goto L6255;
  goto L1545;

 L6255: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x2))
    {
    case MEM:
      goto L6257;
    case SUBREG:
      goto L6258;
    default:
     break;
   }
  goto L1545;

 L6257: ATTRIBUTE_UNUSED_LABEL
  if (offsettable_memory_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L1606;
    }
  goto L1545;

 L1606: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 16
      && (TARGET_ARM && (!TARGET_MMU_TRAPS)))
    {
      return 185;
    }
  goto L1545;

 L6258: ATTRIBUTE_UNUSED_LABEL
  if (XINT (x2, 1) == 0)
    goto L1623;
  goto L1545;

 L1623: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (memory_operand (x3, HImode))
    {
      operands[1] = x3;
      goto L1624;
    }
  goto L1545;

 L1624: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 16
      && (TARGET_ARM
   && BYTES_BIG_ENDIAN
   && !TARGET_MMU_TRAPS))
    {
      return 189;
    }
  goto L1545;

 L1546: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM
   && reload_completed
   && (GET_CODE (operands[1]) == LABEL_REF
       || (GET_CODE (operands[1]) == CONST
	   && GET_CODE (XEXP (operands[1], 0)) == PLUS
	   && GET_CODE (XEXP (XEXP (operands[1], 0), 0)) == LABEL_REF
	   && GET_CODE (XEXP (XEXP (operands[1], 0), 1)) == CONST_INT))))
    {
      return 177;
    }
  goto ret0;
 ret0:
  return -1;
}

static int recog_11 PARAMS ((rtx, rtx, int *));
static int
recog_11 (x0, insn, pnum_clobbers)
     rtx x0 ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  rtx * const operands ATTRIBUTE_UNUSED = &recog_data.operand[0];
  rtx x1 ATTRIBUTE_UNUSED;
  rtx x2 ATTRIBUTE_UNUSED;
  rtx x3 ATTRIBUTE_UNUSED;
  rtx x4 ATTRIBUTE_UNUSED;
  rtx x5 ATTRIBUTE_UNUSED;
  rtx x6 ATTRIBUTE_UNUSED;
  rtx x7 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;

  x1 = XEXP (x0, 1);
  switch (GET_CODE (x1))
    {
    case PLUS:
      goto L219;
    case MINUS:
      goto L401;
    case MULT:
      goto L643;
    case DIV:
      goto L683;
    case MOD:
      goto L723;
    case NEG:
      goto L1271;
    case ABS:
      goto L1318;
    case SQRT:
      goto L1339;
    case FLOAT:
      goto L1402;
    case FLOAT_TRUNCATE:
      goto L1437;
    case FLOAT_EXTEND:
      goto L1515;
    default:
     break;
   }
  goto ret0;

 L219: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == DFmode)
    goto L6299;
  goto ret0;

 L6299: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == FLOAT_EXTEND)
    goto L226;
  if (s_register_operand (x2, DFmode))
    {
      operands[1] = x2;
      goto L220;
    }
  goto ret0;

 L226: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      operands[1] = x3;
      goto L227;
    }
  goto ret0;

 L227: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == DFmode)
    goto L6301;
  goto ret0;

 L6301: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == FLOAT_EXTEND)
    goto L242;
  if (fpu_add_operand (x2, DFmode))
    {
      operands[2] = x2;
      goto L228;
    }
  goto ret0;

 L242: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      operands[2] = x3;
      goto L243;
    }
  goto ret0;

 L243: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 24;
    }
  goto ret0;

 L228: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 22;
    }
  goto ret0;

 L220: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == DFmode)
    goto L6303;
  goto ret0;

 L6303: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == FLOAT_EXTEND)
    goto L234;
  if (fpu_add_operand (x2, DFmode))
    {
      operands[2] = x2;
      goto L221;
    }
  goto ret0;

 L234: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      operands[2] = x3;
      goto L235;
    }
  goto ret0;

 L235: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 23;
    }
  goto ret0;

 L221: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 21;
    }
  goto ret0;

 L401: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == DFmode)
    goto L6305;
  goto ret0;

 L6305: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == FLOAT_EXTEND)
    goto L408;
  if (fpu_rhs_operand (x2, DFmode))
    {
      operands[1] = x2;
      goto L402;
    }
  goto ret0;

 L408: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      operands[1] = x3;
      goto L409;
    }
  goto ret0;

 L409: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == DFmode)
    goto L6307;
  goto ret0;

 L6307: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == FLOAT_EXTEND)
    goto L424;
  if (fpu_rhs_operand (x2, DFmode))
    {
      operands[2] = x2;
      goto L410;
    }
  goto ret0;

 L424: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      operands[2] = x3;
      goto L425;
    }
  goto ret0;

 L425: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 41;
    }
  goto ret0;

 L410: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 39;
    }
  goto ret0;

 L402: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == DFmode)
    goto L6309;
  goto ret0;

 L6309: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == FLOAT_EXTEND)
    goto L416;
  if (fpu_rhs_operand (x2, DFmode))
    {
      operands[2] = x2;
      goto L403;
    }
  goto ret0;

 L416: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      operands[2] = x3;
      goto L417;
    }
  goto ret0;

 L417: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 40;
    }
  goto ret0;

 L403: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 38;
    }
  goto ret0;

 L643: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == DFmode)
    goto L6311;
  goto ret0;

 L6311: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == FLOAT_EXTEND)
    goto L650;
  if (s_register_operand (x2, DFmode))
    {
      operands[1] = x2;
      goto L644;
    }
  goto ret0;

 L650: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      operands[1] = x3;
      goto L651;
    }
  goto ret0;

 L651: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == DFmode)
    goto L6313;
  goto ret0;

 L6313: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == FLOAT_EXTEND)
    goto L666;
  if (fpu_rhs_operand (x2, DFmode))
    {
      operands[2] = x2;
      goto L652;
    }
  goto ret0;

 L666: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      operands[2] = x3;
      goto L667;
    }
  goto ret0;

 L667: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 63;
    }
  goto ret0;

 L652: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 61;
    }
  goto ret0;

 L644: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == DFmode)
    goto L6315;
  goto ret0;

 L6315: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == FLOAT_EXTEND)
    goto L658;
  if (fpu_rhs_operand (x2, DFmode))
    {
      operands[2] = x2;
      goto L645;
    }
  goto ret0;

 L658: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      operands[2] = x3;
      goto L659;
    }
  goto ret0;

 L659: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 62;
    }
  goto ret0;

 L645: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 60;
    }
  goto ret0;

 L683: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == DFmode)
    goto L6317;
  goto ret0;

 L6317: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == FLOAT_EXTEND)
    goto L690;
  if (fpu_rhs_operand (x2, DFmode))
    {
      operands[1] = x2;
      goto L684;
    }
  goto ret0;

 L690: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      operands[1] = x3;
      goto L691;
    }
  goto ret0;

 L691: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == DFmode)
    goto L6319;
  goto ret0;

 L6319: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == FLOAT_EXTEND)
    goto L706;
  if (fpu_rhs_operand (x2, DFmode))
    {
      operands[2] = x2;
      goto L692;
    }
  goto ret0;

 L706: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      operands[2] = x3;
      goto L707;
    }
  goto ret0;

 L707: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 69;
    }
  goto ret0;

 L692: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 67;
    }
  goto ret0;

 L684: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == DFmode)
    goto L6321;
  goto ret0;

 L6321: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == FLOAT_EXTEND)
    goto L698;
  if (fpu_rhs_operand (x2, DFmode))
    {
      operands[2] = x2;
      goto L685;
    }
  goto ret0;

 L698: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      operands[2] = x3;
      goto L699;
    }
  goto ret0;

 L699: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 68;
    }
  goto ret0;

 L685: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 66;
    }
  goto ret0;

 L723: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == DFmode)
    goto L6323;
  goto ret0;

 L6323: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == FLOAT_EXTEND)
    goto L730;
  if (s_register_operand (x2, DFmode))
    {
      operands[1] = x2;
      goto L724;
    }
  goto ret0;

 L730: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      operands[1] = x3;
      goto L731;
    }
  goto ret0;

 L731: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == DFmode)
    goto L6325;
  goto ret0;

 L6325: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == FLOAT_EXTEND)
    goto L746;
  if (fpu_rhs_operand (x2, DFmode))
    {
      operands[2] = x2;
      goto L732;
    }
  goto ret0;

 L746: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      operands[2] = x3;
      goto L747;
    }
  goto ret0;

 L747: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 75;
    }
  goto ret0;

 L732: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 73;
    }
  goto ret0;

 L724: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == DFmode)
    goto L6327;
  goto ret0;

 L6327: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == FLOAT_EXTEND)
    goto L738;
  if (fpu_rhs_operand (x2, DFmode))
    {
      operands[2] = x2;
      goto L725;
    }
  goto ret0;

 L738: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      operands[2] = x3;
      goto L739;
    }
  goto ret0;

 L739: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 74;
    }
  goto ret0;

 L725: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 72;
    }
  goto ret0;

 L1271: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == DFmode)
    goto L6329;
  goto ret0;

 L6329: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == FLOAT_EXTEND)
    goto L1277;
  if (s_register_operand (x2, DFmode))
    {
      operands[1] = x2;
      goto L1272;
    }
  goto ret0;

 L1277: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      operands[1] = x3;
      goto L1278;
    }
  goto ret0;

 L1278: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 131;
    }
  goto ret0;

 L1272: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 130;
    }
  goto ret0;

 L1318: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == DFmode)
    goto L6331;
  goto ret0;

 L6331: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == FLOAT_EXTEND)
    goto L1324;
  if (s_register_operand (x2, DFmode))
    {
      operands[1] = x2;
      goto L1319;
    }
  goto ret0;

 L1324: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      operands[1] = x3;
      goto L1325;
    }
  goto ret0;

 L1325: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 137;
    }
  goto ret0;

 L1319: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 136;
    }
  goto ret0;

 L1339: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == DFmode)
    goto L6333;
  goto ret0;

 L6333: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == FLOAT_EXTEND)
    goto L1345;
  if (s_register_operand (x2, DFmode))
    {
      operands[1] = x2;
      goto L1340;
    }
  goto ret0;

 L1345: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      operands[1] = x3;
      goto L1346;
    }
  goto ret0;

 L1346: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 141;
    }
  goto ret0;

 L1340: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 140;
    }
  goto ret0;

 L1402: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L1403;
    }
  goto ret0;

 L1403: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 149;
    }
  goto ret0;

 L1437: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, XFmode))
    {
      operands[1] = x2;
      goto L1438;
    }
  goto ret0;

 L1438: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT))
    {
      return 156;
    }
  goto ret0;

 L1515: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SFmode))
    {
      operands[1] = x2;
      goto L1516;
    }
  goto ret0;

 L1516: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 170;
    }
  goto ret0;
 ret0:
  return -1;
}

static int recog_12 PARAMS ((rtx, rtx, int *));
static int
recog_12 (x0, insn, pnum_clobbers)
     rtx x0 ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  rtx * const operands ATTRIBUTE_UNUSED = &recog_data.operand[0];
  rtx x1 ATTRIBUTE_UNUSED;
  rtx x2 ATTRIBUTE_UNUSED;
  rtx x3 ATTRIBUTE_UNUSED;
  rtx x4 ATTRIBUTE_UNUSED;
  rtx x5 ATTRIBUTE_UNUSED;
  rtx x6 ATTRIBUTE_UNUSED;
  rtx x7 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;

  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  if (equality_operator (x2, VOIDmode))
    {
      operands[3] = x2;
      goto L2784;
    }
 L2903: ATTRIBUTE_UNUSED_LABEL
  if (arm_comparison_operator (x2, VOIDmode))
    {
      operands[5] = x2;
      goto L2904;
    }
 L2925: ATTRIBUTE_UNUSED_LABEL
  if (arm_comparison_operator (x2, VOIDmode))
    {
      operands[6] = x2;
      goto L2926;
    }
 L2936: ATTRIBUTE_UNUSED_LABEL
  if (arm_comparison_operator (x2, VOIDmode))
    {
      operands[4] = x2;
      goto L2937;
    }
 L2997: ATTRIBUTE_UNUSED_LABEL
  if (arm_comparison_operator (x2, VOIDmode))
    {
      operands[9] = x2;
      goto L2998;
    }
 L3010: ATTRIBUTE_UNUSED_LABEL
  if (arm_comparison_operator (x2, VOIDmode))
    {
      operands[5] = x2;
      goto L3106;
    }
 L3036: ATTRIBUTE_UNUSED_LABEL
  if (arm_comparison_operator (x2, VOIDmode))
    {
      operands[6] = x2;
      goto L3037;
    }
 L3047: ATTRIBUTE_UNUSED_LABEL
  if (arm_comparison_operator (x2, VOIDmode))
    {
      operands[4] = x2;
      goto L3048;
    }
 L3181: ATTRIBUTE_UNUSED_LABEL
  if (arm_comparison_operator (x2, VOIDmode))
    {
      operands[5] = x2;
      goto L3182;
    }
 L3242: ATTRIBUTE_UNUSED_LABEL
  if (arm_comparison_operator (x2, VOIDmode))
    {
      operands[7] = x2;
      goto L3243;
    }
 L3255: ATTRIBUTE_UNUSED_LABEL
  if (arm_comparison_operator (x2, VOIDmode))
    {
      operands[5] = x2;
      goto L3357;
    }
 L3366: ATTRIBUTE_UNUSED_LABEL
  if (arm_comparison_operator (x2, VOIDmode))
    {
      operands[4] = x2;
      goto L3367;
    }
  goto ret0;

 L2784: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (arm_comparison_operator (x3, VOIDmode))
    {
      operands[4] = x3;
      goto L2785;
    }
  goto L2903;

 L2785: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (cc_register (x4, VOIDmode))
    {
      operands[5] = x4;
      goto L2786;
    }
  goto L2903;

 L2786: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 0)
    goto L2787;
  goto L2903;

 L2787: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L2788;
  goto L2903;

 L2788: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_rhs_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L2789;
    }
  x2 = XEXP (x1, 0);
  goto L2903;

 L2789: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (arm_rhs_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L2790;
    }
  x2 = XEXP (x1, 0);
  goto L2903;

 L2790: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 283;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L2903;

 L2904: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L2905;
    }
  goto L2925;

 L2905: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_add_operand (x3, SImode))
    {
      operands[4] = x3;
      goto L2906;
    }
  goto L2925;

 L2906: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_rhs_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L2907;
    }
  x2 = XEXP (x1, 0);
  goto L2925;

 L2907: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (arm_rhs_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L2908;
    }
  x2 = XEXP (x1, 0);
  goto L2925;

 L2908: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 291;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L2925;

 L2926: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[4] = x3;
      goto L2927;
    }
  goto L2936;

 L2927: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_add_operand (x3, SImode))
    {
      operands[5] = x3;
      goto L2928;
    }
  goto L2936;

 L2928: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L2929;
  if (arm_rhs_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L2964;
    }
  x2 = XEXP (x1, 0);
  goto L2936;

 L2929: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L2930;
    }
  x2 = XEXP (x1, 0);
  goto L2936;

 L2930: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_add_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L2931;
    }
  x2 = XEXP (x1, 0);
  goto L2936;

 L2931: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (arm_rhs_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L2932;
    }
  x2 = XEXP (x1, 0);
  goto L2936;

 L2932: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 292;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L2936;

 L2964: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L2965;
  x2 = XEXP (x1, 0);
  goto L2936;

 L2965: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L2966;
    }
  x2 = XEXP (x1, 0);
  goto L2936;

 L2966: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_add_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L2967;
    }
  x2 = XEXP (x1, 0);
  goto L2936;

 L2967: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 294;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L2936;

 L2937: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (cc_register (x3, VOIDmode))
    {
      operands[5] = x3;
      goto L2938;
    }
  goto L2997;

 L2938: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L2939;
  goto L2997;

 L2939: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L2940;
  if (arm_rhs_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L2975;
    }
  x2 = XEXP (x1, 0);
  goto L2997;

 L2940: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L2941;
    }
  x2 = XEXP (x1, 0);
  goto L2997;

 L2941: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_add_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L2942;
    }
  x2 = XEXP (x1, 0);
  goto L2997;

 L2942: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (arm_rhs_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L2943;
    }
  x2 = XEXP (x1, 0);
  goto L2997;

 L2943: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 293;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L2997;

 L2975: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L2976;
  x2 = XEXP (x1, 0);
  goto L2997;

 L2976: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L2977;
    }
  x2 = XEXP (x1, 0);
  goto L2997;

 L2977: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_add_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L2978;
    }
  x2 = XEXP (x1, 0);
  goto L2997;

 L2978: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 295;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L2997;

 L2998: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[5] = x3;
      goto L2999;
    }
  goto L3010;

 L2999: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_add_operand (x3, SImode))
    {
      operands[6] = x3;
      goto L3000;
    }
  goto L3010;

 L3000: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (shiftable_operator (x2, SImode))
    {
      operands[8] = x2;
      goto L3001;
    }
  x2 = XEXP (x1, 0);
  goto L3010;

 L3001: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L3002;
    }
  x2 = XEXP (x1, 0);
  goto L3010;

 L3002: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L3003;
    }
  x2 = XEXP (x1, 0);
  goto L3010;

 L3003: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (shiftable_operator (x2, SImode))
    {
      operands[7] = x2;
      goto L3004;
    }
  x2 = XEXP (x1, 0);
  goto L3010;

 L3004: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L3005;
    }
  x2 = XEXP (x1, 0);
  goto L3010;

 L3005: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[4] = x3;
      goto L3006;
    }
  x2 = XEXP (x1, 0);
  goto L3010;

 L3006: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 296;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L3010;

 L3106: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L3107;
    }
 L3011: ATTRIBUTE_UNUSED_LABEL
  if (cc_register (x3, VOIDmode))
    {
      operands[8] = x3;
      goto L3012;
    }
  goto L3036;

 L3107: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_add_operand (x3, SImode))
    {
      operands[4] = x3;
      goto L3140;
    }
  x3 = XEXP (x2, 0);
  goto L3011;

 L3140: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == NOT)
    goto L3141;
  if (arm_not_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L3109;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L3011;

 L3141: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L3142;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L3011;

 L3142: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (arm_not_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L3143;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L3011;

 L3143: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 304;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L3011;

 L3109: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == NOT)
    goto L3110;
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L3011;

 L3110: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L3111;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L3011;

 L3111: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 302;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L3011;

 L3012: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L3013;
  goto L3036;

 L3013: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (shiftable_operator (x2, SImode))
    {
      operands[6] = x2;
      goto L3014;
    }
  x2 = XEXP (x1, 0);
  goto L3036;

 L3014: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L3015;
    }
  x2 = XEXP (x1, 0);
  goto L3036;

 L3015: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L3016;
    }
  x2 = XEXP (x1, 0);
  goto L3036;

 L3016: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (shiftable_operator (x2, SImode))
    {
      operands[7] = x2;
      goto L3017;
    }
  x2 = XEXP (x1, 0);
  goto L3036;

 L3017: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L3018;
    }
  x2 = XEXP (x1, 0);
  goto L3036;

 L3018: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[4] = x3;
      goto L3019;
    }
  x2 = XEXP (x1, 0);
  goto L3036;

 L3019: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 297;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L3036;

 L3037: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode)
    goto L6383;
  goto L3047;

 L6383: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L3038;
    }
 L6384: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x3, SImode))
    {
      operands[4] = x3;
      goto L3073;
    }
  goto L3047;

 L3038: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_add_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L3039;
    }
  x3 = XEXP (x2, 0);
  goto L6384;

 L3039: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (shiftable_operator (x2, SImode))
    {
      operands[7] = x2;
      goto L3040;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6384;

 L3040: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[4] = x3;
      goto L3041;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6384;

 L3041: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[5] = x3;
      goto L3042;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6384;

 L3042: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (arm_rhs_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L3043;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6384;

 L3043: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 298;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6384;

 L3073: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_add_operand (x3, SImode))
    {
      operands[5] = x3;
      goto L3173;
    }
  goto L3047;

 L3173: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode)
    goto L6386;
 L3074: ATTRIBUTE_UNUSED_LABEL
  if (arm_rhs_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L3075;
    }
 L3208: ATTRIBUTE_UNUSED_LABEL
  if (arm_not_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L3209;
    }
  x2 = XEXP (x1, 0);
  goto L3047;

 L6386: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == NOT)
    goto L3286;
  if (shift_operator (x2, SImode))
    {
      operands[7] = x2;
      goto L3174;
    }
  if (shiftable_operator (x2, SImode))
    {
      operands[7] = x2;
      goto L3324;
    }
  goto L3074;

 L3286: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L3287;
    }
  goto L3074;

 L3287: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (shiftable_operator (x2, SImode))
    {
      operands[7] = x2;
      goto L3288;
    }
  x2 = XEXP (x1, 1);
  goto L3074;

 L3288: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L3289;
    }
  x2 = XEXP (x1, 1);
  goto L3074;

 L3289: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L3290;
    }
  x2 = XEXP (x1, 1);
  goto L3074;

 L3290: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 312;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 1);
  goto L3074;

 L3174: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L3175;
    }
  goto L3074;

 L3175: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L3176;
    }
  goto L3074;

 L3176: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (arm_not_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L3177;
    }
  x2 = XEXP (x1, 1);
  goto L3074;

 L3177: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 306;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 1);
  goto L3074;

 L3324: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L3325;
    }
  goto L3074;

 L3325: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L3326;
    }
  goto L3074;

 L3326: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == NOT)
    goto L3327;
  x2 = XEXP (x1, 1);
  goto L3074;

 L3327: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L3328;
    }
  x2 = XEXP (x1, 1);
  goto L3074;

 L3328: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 314;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 1);
  goto L3074;

 L3075: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (shiftable_operator (x2, SImode))
    {
      operands[7] = x2;
      goto L3076;
    }
  x2 = XEXP (x1, 1);
  goto L3208;

 L3076: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L3077;
    }
  x2 = XEXP (x1, 1);
  goto L3208;

 L3077: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L3078;
    }
  x2 = XEXP (x1, 1);
  goto L3208;

 L3078: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 300;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 1);
  goto L3208;

 L3209: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (shift_operator (x2, SImode))
    {
      operands[7] = x2;
      goto L3210;
    }
  x2 = XEXP (x1, 0);
  goto L3047;

 L3210: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L3211;
    }
  x2 = XEXP (x1, 0);
  goto L3047;

 L3211: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L3212;
    }
  x2 = XEXP (x1, 0);
  goto L3047;

 L3212: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 308;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L3047;

 L3048: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_CODE (x3) == REG)
    goto L6388;
  goto L3181;

 L6388: ATTRIBUTE_UNUSED_LABEL
  if (cc_register (x3, VOIDmode))
    {
      operands[6] = x3;
      goto L3049;
    }
 L6389: ATTRIBUTE_UNUSED_LABEL
  if (cc_register (x3, VOIDmode))
    {
      operands[3] = x3;
      goto L3117;
    }
  goto L3181;

 L3049: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L3050;
  x3 = XEXP (x2, 0);
  goto L6389;

 L3050: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (shiftable_operator (x2, SImode))
    {
      operands[5] = x2;
      goto L3051;
    }
  if (arm_rhs_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L3086;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6389;

 L3051: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L3052;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6389;

 L3052: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L3053;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6389;

 L3053: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (arm_rhs_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L3054;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6389;

 L3054: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 299;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6389;

 L3086: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (shiftable_operator (x2, SImode))
    {
      operands[5] = x2;
      goto L3087;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6389;

 L3087: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L3088;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6389;

 L3088: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L3089;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6389;

 L3089: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 301;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6389;

 L3117: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L3150;
  goto L3181;

 L3150: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == NOT)
    goto L3151;
  if (arm_not_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L3119;
    }
  x2 = XEXP (x1, 0);
  goto L3181;

 L3151: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L3152;
    }
  x2 = XEXP (x1, 0);
  goto L3181;

 L3152: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (arm_not_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L3153;
    }
  x2 = XEXP (x1, 0);
  goto L3181;

 L3153: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 305;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L3181;

 L3119: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == NOT)
    goto L3120;
  x2 = XEXP (x1, 0);
  goto L3181;

 L3120: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L3121;
    }
  x2 = XEXP (x1, 0);
  goto L3181;

 L3121: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 303;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L3181;

 L3182: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (cc_register (x3, VOIDmode))
    {
      operands[6] = x3;
      goto L3183;
    }
  goto L3242;

 L3183: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L3184;
  goto L3242;

 L3184: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (shift_operator (x2, SImode))
    {
      operands[4] = x2;
      goto L3185;
    }
  if (arm_not_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L3220;
    }
  x2 = XEXP (x1, 0);
  goto L3242;

 L3185: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L3186;
    }
  x2 = XEXP (x1, 0);
  goto L3242;

 L3186: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L3187;
    }
  x2 = XEXP (x1, 0);
  goto L3242;

 L3187: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (arm_not_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L3188;
    }
  x2 = XEXP (x1, 0);
  goto L3242;

 L3188: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 307;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L3242;

 L3220: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (shift_operator (x2, SImode))
    {
      operands[4] = x2;
      goto L3221;
    }
  x2 = XEXP (x1, 0);
  goto L3242;

 L3221: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L3222;
    }
  x2 = XEXP (x1, 0);
  goto L3242;

 L3222: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L3223;
    }
  x2 = XEXP (x1, 0);
  goto L3242;

 L3223: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 309;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L3242;

 L3243: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[5] = x3;
      goto L3244;
    }
  goto L3255;

 L3244: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_add_operand (x3, SImode))
    {
      operands[6] = x3;
      goto L3245;
    }
  goto L3255;

 L3245: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (shift_operator (x2, SImode))
    {
      operands[8] = x2;
      goto L3246;
    }
  x2 = XEXP (x1, 0);
  goto L3255;

 L3246: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L3247;
    }
  x2 = XEXP (x1, 0);
  goto L3255;

 L3247: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L3248;
    }
  x2 = XEXP (x1, 0);
  goto L3255;

 L3248: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (shift_operator (x2, SImode))
    {
      operands[9] = x2;
      goto L3249;
    }
  x2 = XEXP (x1, 0);
  goto L3255;

 L3249: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L3250;
    }
  x2 = XEXP (x1, 0);
  goto L3255;

 L3250: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[4] = x3;
      goto L3251;
    }
  x2 = XEXP (x1, 0);
  goto L3255;

 L3251: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 310;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L3255;

 L3357: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L3358;
    }
 L3256: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == REG)
    goto L6390;
  goto L3366;

 L3358: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_add_operand (x3, SImode))
    {
      operands[4] = x3;
      goto L3359;
    }
  x3 = XEXP (x2, 0);
  goto L3256;

 L3359: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == NEG)
    goto L3360;
  if (arm_not_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L3392;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L3256;

 L3360: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L3361;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L3256;

 L3361: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (arm_not_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L3362;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L3256;

 L3362: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 316;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L3256;

 L3392: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == NEG)
    goto L3393;
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L3256;

 L3393: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L3394;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L3256;

 L3394: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 318;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L3256;

 L6390: ATTRIBUTE_UNUSED_LABEL
  if (cc_register (x3, VOIDmode))
    {
      operands[8] = x3;
      goto L3257;
    }
 L6391: ATTRIBUTE_UNUSED_LABEL
  if (cc_register (x3, VOIDmode))
    {
      operands[4] = x3;
      goto L3296;
    }
  goto L3366;

 L3257: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L3258;
  x3 = XEXP (x2, 0);
  goto L6391;

 L3258: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (shift_operator (x2, SImode))
    {
      operands[6] = x2;
      goto L3259;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6391;

 L3259: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L3260;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6391;

 L3260: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L3261;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6391;

 L3261: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (shift_operator (x2, SImode))
    {
      operands[7] = x2;
      goto L3262;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6391;

 L3262: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L3263;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6391;

 L3263: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[4] = x3;
      goto L3264;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6391;

 L3264: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 311;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L6391;

 L3296: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L3297;
  goto L3366;

 L3297: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode)
    goto L6392;
  x2 = XEXP (x1, 0);
  goto L3366;

 L6392: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == NOT)
    goto L3298;
  if (shiftable_operator (x2, SImode))
    {
      operands[6] = x2;
      goto L3336;
    }
  x2 = XEXP (x1, 0);
  goto L3366;

 L3298: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L3299;
    }
  x2 = XEXP (x1, 0);
  goto L3366;

 L3299: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (shiftable_operator (x2, SImode))
    {
      operands[6] = x2;
      goto L3300;
    }
  x2 = XEXP (x1, 0);
  goto L3366;

 L3300: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L3301;
    }
  x2 = XEXP (x1, 0);
  goto L3366;

 L3301: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L3302;
    }
  x2 = XEXP (x1, 0);
  goto L3366;

 L3302: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 313;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L3366;

 L3336: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L3337;
    }
  x2 = XEXP (x1, 0);
  goto L3366;

 L3337: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L3338;
    }
  x2 = XEXP (x1, 0);
  goto L3366;

 L3338: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == NOT)
    goto L3339;
  x2 = XEXP (x1, 0);
  goto L3366;

 L3339: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L3340;
    }
  x2 = XEXP (x1, 0);
  goto L3366;

 L3340: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 315;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L3366;

 L3367: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (cc_register (x3, VOIDmode))
    {
      operands[3] = x3;
      goto L3368;
    }
  goto ret0;

 L3368: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L3369;
  goto ret0;

 L3369: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == NEG)
    goto L3370;
  if (arm_not_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L3402;
    }
 L3736: ATTRIBUTE_UNUSED_LABEL
  if (arm_rhs_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L3737;
    }
  goto ret0;

 L3370: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L3371;
    }
  goto ret0;

 L3371: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (arm_not_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L3372;
    }
  goto ret0;

 L3372: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 317;
    }
  goto ret0;

 L3402: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == NEG)
    goto L3403;
  x2 = XEXP (x1, 1);
  goto L3736;

 L3403: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L3404;
    }
  x2 = XEXP (x1, 1);
  goto L3736;

 L3404: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 319;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 1);
  goto L3736;

 L3737: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == NOT)
    goto L3738;
  goto ret0;

 L3738: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L3739;
    }
  goto ret0;

 L3739: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 345;
    }
  goto ret0;
 ret0:
  return -1;
}

static int recog_13 PARAMS ((rtx, rtx, int *));
static int
recog_13 (x0, insn, pnum_clobbers)
     rtx x0 ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  rtx * const operands ATTRIBUTE_UNUSED = &recog_data.operand[0];
  rtx x1 ATTRIBUTE_UNUSED;
  rtx x2 ATTRIBUTE_UNUSED;
  rtx x3 ATTRIBUTE_UNUSED;
  rtx x4 ATTRIBUTE_UNUSED;
  rtx x5 ATTRIBUTE_UNUSED;
  rtx x6 ATTRIBUTE_UNUSED;
  rtx x7 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;

  x1 = XEXP (x0, 0);
  switch (GET_MODE (x1))
    {
    case DImode:
      goto L6093;
    case SImode:
      goto L6095;
    case CC_NOOVmode:
      goto L6097;
    case CCmode:
      goto L6098;
    case CC_Cmode:
      goto L6099;
    case SFmode:
      goto L6100;
    case DFmode:
      goto L6101;
    case XFmode:
      goto L6102;
    case CC_Zmode:
      goto L6110;
    case HImode:
      goto L6111;
    case QImode:
      goto L6119;
    case CC_SWPmode:
      goto L6125;
    case CCFPmode:
      goto L6126;
    case CCFPEmode:
      goto L6127;
    default:
      break;
    }
 L2043: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x1))
    {
    case PC:
      goto L2531;
    case REG:
      goto L6128;
    default:
     break;
   }
 L2217: ATTRIBUTE_UNUSED_LABEL
  switch (GET_MODE (x1))
    {
    case SImode:
      goto L6129;
    case SFmode:
      goto L6130;
    case DFmode:
      goto L6131;
    case CC_NOOVmode:
      goto L6132;
    default:
      break;
    }
 L2637: ATTRIBUTE_UNUSED_LABEL
  operands[0] = x1;
  goto L2638;
 L2683: ATTRIBUTE_UNUSED_LABEL
  if (GET_MODE (x1) == CC_NOOVmode
      && GET_CODE (x1) == REG
      && XINT (x1, 0) == 24)
    goto L2684;
 L2694: ATTRIBUTE_UNUSED_LABEL
  operands[0] = x1;
  goto L2695;
 L2740: ATTRIBUTE_UNUSED_LABEL
  switch (GET_MODE (x1))
    {
    case CC_NOOVmode:
      goto L6133;
    case SImode:
      goto L6134;
    default:
      break;
    }
 L2828: ATTRIBUTE_UNUSED_LABEL
  if (dominant_cc_register (x1, VOIDmode))
    {
      operands[6] = x1;
      goto L2829;
    }
  goto ret0;

 L6093: ATTRIBUTE_UNUSED_LABEL
  if (register_operand (x1, DImode))
    {
      operands[0] = x1;
      goto L10;
    }
 L6094: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x1, DImode))
    {
      operands[0] = x1;
      goto L24;
    }
 L6103: ATTRIBUTE_UNUSED_LABEL
  if (register_operand (x1, DImode))
    {
      operands[0] = x1;
      goto L274;
    }
 L6113: ATTRIBUTE_UNUSED_LABEL
  if (nonimmediate_di_operand (x1, DImode))
    {
      operands[0] = x1;
      goto L1529;
    }
 L6114: ATTRIBUTE_UNUSED_LABEL
  if (nonimmediate_operand (x1, DImode))
    {
      operands[0] = x1;
      goto L1533;
    }
  goto L2043;

 L10: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == DImode
      && GET_CODE (x1) == PLUS)
    goto L11;
  x1 = XEXP (x0, 0);
  goto L6094;

 L11: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (register_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L12;
    }
  x1 = XEXP (x0, 0);
  goto L6094;

 L12: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (register_operand (x2, DImode))
    {
      operands[2] = x2;
      goto L13;
    }
  x1 = XEXP (x0, 0);
  goto L6094;

 L13: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 0;
    }
  x1 = XEXP (x0, 0);
  goto L6094;

 L24: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == DImode)
    goto L6135;
  x1 = XEXP (x0, 0);
  goto L6103;

 L6135: ATTRIBUTE_UNUSED_LABEL
  tem = recog_8 (x0, insn, pnum_clobbers);
  if (tem >= 0)
    return tem;
  x1 = XEXP (x0, 0);
  goto L6103;

 L274: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == DImode)
    goto L6178;
  x1 = XEXP (x0, 0);
  goto L6113;

 L6178: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x1))
    {
    case MINUS:
      goto L275;
    case NEG:
      goto L1251;
    default:
     break;
   }
  x1 = XEXP (x0, 0);
  goto L6113;

 L275: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (register_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L276;
    }
  x1 = XEXP (x0, 0);
  goto L6113;

 L276: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (register_operand (x2, DImode))
    {
      operands[2] = x2;
      goto L277;
    }
  x1 = XEXP (x0, 0);
  goto L6113;

 L277: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 27;
    }
  x1 = XEXP (x0, 0);
  goto L6113;

 L1251: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (register_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L1252;
    }
  x1 = XEXP (x0, 0);
  goto L6113;

 L1252: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 126;
    }
  x1 = XEXP (x0, 0);
  goto L6113;

 L1529: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (di_operand (x1, DImode))
    {
      operands[1] = x1;
      goto L1530;
    }
  x1 = XEXP (x0, 0);
  goto L6114;

 L1530: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 173;
    }
  x1 = XEXP (x0, 0);
  goto L6114;

 L1533: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (general_operand (x1, DImode))
    {
      operands[1] = x1;
      goto L1534;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L1534: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB
   && (   register_operand (operands[0], DImode)
       || register_operand (operands[1], DImode))))
    {
      return 174;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L6095: ATTRIBUTE_UNUSED_LABEL
  tem = recog_10 (x0, insn, pnum_clobbers);
  if (tem >= 0)
    return tem;
  goto L2043;

 L6097: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x1) == REG
      && XINT (x1, 0) == 24)
    goto L87;
  goto L2043;

 L87: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == CC_NOOVmode
      && GET_CODE (x1) == COMPARE)
    goto L88;
  x1 = XEXP (x0, 0);
  goto L2043;

 L88: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode)
    goto L6259;
  x1 = XEXP (x0, 0);
  goto L2043;

 L6259: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x2))
    {
    case PLUS:
      goto L89;
    case MULT:
      goto L472;
    case AND:
      goto L814;
    case ZERO_EXTRACT:
      goto L821;
    case IOR:
      goto L974;
    case XOR:
      goto L1026;
    case NOT:
      goto L1225;
    default:
     break;
   }
 L6265: ATTRIBUTE_UNUSED_LABEL
  if (shift_operator (x2, SImode))
    {
      operands[3] = x2;
      goto L1184;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L89: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode)
    goto L6268;
  x1 = XEXP (x0, 0);
  goto L2043;

 L6268: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == MULT)
    goto L518;
  if (s_register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L90;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L518: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L519;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L519: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L520;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L520: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L521;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L521: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 0
      && (TARGET_ARM && !arm_is_xscale)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 49;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L90: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_add_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L91;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L91: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 0
      && (TARGET_ARM))
    {
      return 7;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L472: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L473;
    }
  goto L6265;

 L473: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L474;
    }
  goto L6265;

 L474: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 0
      && (TARGET_ARM && !arm_is_xscale)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 46;
    }
  x2 = XEXP (x1, 0);
  goto L6265;

 L814: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode)
    goto L6270;
  x1 = XEXP (x0, 0);
  goto L2043;

 L6270: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == NOT)
    goto L911;
  if (s_register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L815;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L911: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L912;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L912: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L913;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L913: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 0
      && (TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 93;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L815: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_not_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L816;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L816: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 0
      && (TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 83;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L821: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L822;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L822: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (const_int_operand (x3, VOIDmode))
    {
      operands[1] = x3;
      goto L823;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L823: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (const_int_operand (x3, VOIDmode))
    {
      operands[2] = x3;
      goto L824;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L824: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 0
      && (TARGET_ARM
  && (INTVAL (operands[2]) >= 0 && INTVAL (operands[2]) < 32
      && INTVAL (operands[1]) > 0 
      && INTVAL (operands[1]) + (INTVAL (operands[2]) & 1) <= 8
      && INTVAL (operands[1]) + INTVAL (operands[2]) <= 32)))
    {
      return 84;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L974: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L975;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L975: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L976;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L976: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 0
      && (TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 100;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L1026: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L1027;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L1027: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L1028;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L1028: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 0
      && (TARGET_ARM))
    {
      return 107;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L1225: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode)
    goto L6271;
  x1 = XEXP (x0, 0);
  goto L2043;

 L6271: ATTRIBUTE_UNUSED_LABEL
  if (shift_operator (x3, SImode))
    {
      operands[3] = x3;
      goto L1226;
    }
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L1393;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L1226: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L1227;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L1227: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L1228;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L1228: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 0
      && (TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 124;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L1393: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 0
      && (TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 147;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L1184: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L1185;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L1185: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L1186;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L1186: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 0
      && (TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 121;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L6098: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x1) == REG
      && XINT (x1, 0) == 24)
    goto L106;
  goto L2043;

 L106: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == CCmode
      && GET_CODE (x1) == COMPARE)
    goto L107;
  x1 = XEXP (x0, 0);
  goto L2043;

 L107: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L108;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L108: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode)
    goto L6273;
 L2065: ATTRIBUTE_UNUSED_LABEL
  if (arm_add_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L2066;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L6273: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == NEG)
    goto L2088;
  if (shift_operator (x2, SImode))
    {
      operands[3] = x2;
      goto L2072;
    }
  goto L2065;

 L2088: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (shift_operator (x3, SImode))
    {
      operands[3] = x3;
      goto L2089;
    }
  if (arm_add_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L110;
    }
  goto L2065;

 L2089: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L2090;
    }
  goto L2065;

 L2090: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L2091;
    }
  goto L2065;

 L2091: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 220;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 1);
  goto L2065;

 L110: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 9;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 1);
  goto L2065;

 L2072: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L2073;
    }
  goto L2065;

 L2073: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L2074;
    }
  goto L2065;

 L2074: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 218;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 1);
  goto L2065;

 L2066: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 217;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L6099: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x1) == REG
      && XINT (x1, 0) == 24)
    goto L139;
  goto L2043;

 L139: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == CC_Cmode
      && GET_CODE (x1) == COMPARE)
    goto L140;
  x1 = XEXP (x0, 0);
  goto L2043;

 L140: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L141;
  x1 = XEXP (x0, 0);
  goto L2043;

 L141: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L142;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L142: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_add_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L143;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L143: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (rtx_equal_p (x2, operands[0])
      && (TARGET_ARM))
    {
      return 12;
    }
 L150: ATTRIBUTE_UNUSED_LABEL
  if (rtx_equal_p (x2, operands[1])
      && (TARGET_ARM))
    {
      return 13;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L6100: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x1, SFmode))
    {
      operands[0] = x1;
      goto L212;
    }
 L6120: ATTRIBUTE_UNUSED_LABEL
  if (nonimmediate_operand (x1, SFmode))
    {
      operands[0] = x1;
      goto L1646;
    }
  goto L2043;

 L212: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == SFmode)
    goto L6275;
  x1 = XEXP (x0, 0);
  goto L6120;

 L6275: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x1))
    {
    case PLUS:
      goto L213;
    case MINUS:
      goto L395;
    case MULT:
      goto L637;
    case DIV:
      goto L677;
    case MOD:
      goto L717;
    case NEG:
      goto L1266;
    case ABS:
      goto L1313;
    case SQRT:
      goto L1334;
    case FLOAT:
      goto L1397;
    case FLOAT_TRUNCATE:
      goto L1427;
    default:
     break;
   }
  x1 = XEXP (x0, 0);
  goto L6120;

 L213: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SFmode))
    {
      operands[1] = x2;
      goto L214;
    }
  x1 = XEXP (x0, 0);
  goto L6120;

 L214: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_add_operand (x2, SFmode))
    {
      operands[2] = x2;
      goto L215;
    }
  x1 = XEXP (x0, 0);
  goto L6120;

 L215: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 20;
    }
  x1 = XEXP (x0, 0);
  goto L6120;

 L395: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (fpu_rhs_operand (x2, SFmode))
    {
      operands[1] = x2;
      goto L396;
    }
  x1 = XEXP (x0, 0);
  goto L6120;

 L396: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_rhs_operand (x2, SFmode))
    {
      operands[2] = x2;
      goto L397;
    }
  x1 = XEXP (x0, 0);
  goto L6120;

 L397: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 37;
    }
  x1 = XEXP (x0, 0);
  goto L6120;

 L637: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SFmode))
    {
      operands[1] = x2;
      goto L638;
    }
  x1 = XEXP (x0, 0);
  goto L6120;

 L638: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_rhs_operand (x2, SFmode))
    {
      operands[2] = x2;
      goto L639;
    }
  x1 = XEXP (x0, 0);
  goto L6120;

 L639: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 59;
    }
  x1 = XEXP (x0, 0);
  goto L6120;

 L677: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (fpu_rhs_operand (x2, SFmode))
    {
      operands[1] = x2;
      goto L678;
    }
  x1 = XEXP (x0, 0);
  goto L6120;

 L678: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_rhs_operand (x2, SFmode))
    {
      operands[2] = x2;
      goto L679;
    }
  x1 = XEXP (x0, 0);
  goto L6120;

 L679: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 65;
    }
  x1 = XEXP (x0, 0);
  goto L6120;

 L717: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SFmode))
    {
      operands[1] = x2;
      goto L718;
    }
  x1 = XEXP (x0, 0);
  goto L6120;

 L718: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_rhs_operand (x2, SFmode))
    {
      operands[2] = x2;
      goto L719;
    }
  x1 = XEXP (x0, 0);
  goto L6120;

 L719: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 71;
    }
  x1 = XEXP (x0, 0);
  goto L6120;

 L1266: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SFmode))
    {
      operands[1] = x2;
      goto L1267;
    }
  x1 = XEXP (x0, 0);
  goto L6120;

 L1267: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 129;
    }
  x1 = XEXP (x0, 0);
  goto L6120;

 L1313: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SFmode))
    {
      operands[1] = x2;
      goto L1314;
    }
  x1 = XEXP (x0, 0);
  goto L6120;

 L1314: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 135;
    }
  x1 = XEXP (x0, 0);
  goto L6120;

 L1334: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SFmode))
    {
      operands[1] = x2;
      goto L1335;
    }
  x1 = XEXP (x0, 0);
  goto L6120;

 L1335: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 139;
    }
  x1 = XEXP (x0, 0);
  goto L6120;

 L1397: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L1398;
    }
  x1 = XEXP (x0, 0);
  goto L6120;

 L1398: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 148;
    }
  x1 = XEXP (x0, 0);
  goto L6120;

 L1427: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  switch (GET_MODE (x2))
    {
    case DFmode:
      goto L6285;
    case XFmode:
      goto L6286;
    default:
      break;
    }
  x1 = XEXP (x0, 0);
  goto L6120;

 L6285: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, DFmode))
    {
      operands[1] = x2;
      goto L1428;
    }
  x1 = XEXP (x0, 0);
  goto L6120;

 L1428: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 154;
    }
  x1 = XEXP (x0, 0);
  goto L6120;

 L6286: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, XFmode))
    {
      operands[1] = x2;
      goto L1433;
    }
  x1 = XEXP (x0, 0);
  goto L6120;

 L1433: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT))
    {
      return 155;
    }
  x1 = XEXP (x0, 0);
  goto L6120;

 L1646: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (general_operand (x1, SFmode))
    {
      operands[1] = x1;
      goto L1647;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L1647: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM
   && TARGET_HARD_FLOAT
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode))))
    {
      return 194;
    }
 L1651: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM
   && TARGET_SOFT_FLOAT
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode))))
    {
      return 195;
    }
 L1655: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB
   && (   register_operand (operands[0], SFmode) 
       || register_operand (operands[1], SFmode))))
    {
      return 196;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L6101: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x1, DFmode))
    {
      operands[0] = x1;
      goto L218;
    }
 L6121: ATTRIBUTE_UNUSED_LABEL
  if (nonimmediate_operand (x1, DFmode))
    {
      operands[0] = x1;
      goto L1658;
    }
 L6122: ATTRIBUTE_UNUSED_LABEL
  if (nonimmediate_soft_df_operand (x1, DFmode))
    {
      operands[0] = x1;
      goto L1662;
    }
 L6123: ATTRIBUTE_UNUSED_LABEL
  if (nonimmediate_operand (x1, DFmode))
    {
      operands[0] = x1;
      goto L1666;
    }
  goto L2043;

 L218: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == DFmode)
    goto L6287;
  x1 = XEXP (x0, 0);
  goto L6121;

 L6287: ATTRIBUTE_UNUSED_LABEL
  tem = recog_11 (x0, insn, pnum_clobbers);
  if (tem >= 0)
    return tem;
  x1 = XEXP (x0, 0);
  goto L6121;

 L1658: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (general_operand (x1, DFmode))
    {
      operands[1] = x1;
      goto L1659;
    }
  x1 = XEXP (x0, 0);
  goto L6122;

 L1659: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM
   && TARGET_HARD_FLOAT
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], DFmode))))
    {
      return 197;
    }
  x1 = XEXP (x0, 0);
  goto L6122;

 L1662: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (soft_df_operand (x1, DFmode))
    {
      operands[1] = x1;
      goto L1663;
    }
  x1 = XEXP (x0, 0);
  goto L6123;

 L1663: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_SOFT_FLOAT
  ))
    {
      return 198;
    }
  x1 = XEXP (x0, 0);
  goto L6123;

 L1666: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (general_operand (x1, DFmode))
    {
      operands[1] = x1;
      goto L1667;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L1667: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB
   && (   register_operand (operands[0], DFmode)
       || register_operand (operands[1], DFmode))))
    {
      return 199;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L6102: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x1, XFmode))
    {
      operands[0] = x1;
      goto L246;
    }
 L6124: ATTRIBUTE_UNUSED_LABEL
  if (nonimmediate_operand (x1, XFmode))
    {
      operands[0] = x1;
      goto L1670;
    }
  goto L2043;

 L246: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == XFmode)
    goto L6334;
  x1 = XEXP (x0, 0);
  goto L6124;

 L6334: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x1))
    {
    case PLUS:
      goto L247;
    case MINUS:
      goto L429;
    case MULT:
      goto L671;
    case DIV:
      goto L711;
    case MOD:
      goto L751;
    case NEG:
      goto L1282;
    case ABS:
      goto L1329;
    case SQRT:
      goto L1350;
    case FLOAT:
      goto L1407;
    case FLOAT_EXTEND:
      goto L1520;
    default:
     break;
   }
  x1 = XEXP (x0, 0);
  goto L6124;

 L247: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, XFmode))
    {
      operands[1] = x2;
      goto L248;
    }
  x1 = XEXP (x0, 0);
  goto L6124;

 L248: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_add_operand (x2, XFmode))
    {
      operands[2] = x2;
      goto L249;
    }
  x1 = XEXP (x0, 0);
  goto L6124;

 L249: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT))
    {
      return 25;
    }
  x1 = XEXP (x0, 0);
  goto L6124;

 L429: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (fpu_rhs_operand (x2, XFmode))
    {
      operands[1] = x2;
      goto L430;
    }
  x1 = XEXP (x0, 0);
  goto L6124;

 L430: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_rhs_operand (x2, XFmode))
    {
      operands[2] = x2;
      goto L431;
    }
  x1 = XEXP (x0, 0);
  goto L6124;

 L431: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT))
    {
      return 42;
    }
  x1 = XEXP (x0, 0);
  goto L6124;

 L671: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, XFmode))
    {
      operands[1] = x2;
      goto L672;
    }
  x1 = XEXP (x0, 0);
  goto L6124;

 L672: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_rhs_operand (x2, XFmode))
    {
      operands[2] = x2;
      goto L673;
    }
  x1 = XEXP (x0, 0);
  goto L6124;

 L673: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT))
    {
      return 64;
    }
  x1 = XEXP (x0, 0);
  goto L6124;

 L711: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (fpu_rhs_operand (x2, XFmode))
    {
      operands[1] = x2;
      goto L712;
    }
  x1 = XEXP (x0, 0);
  goto L6124;

 L712: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_rhs_operand (x2, XFmode))
    {
      operands[2] = x2;
      goto L713;
    }
  x1 = XEXP (x0, 0);
  goto L6124;

 L713: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT))
    {
      return 70;
    }
  x1 = XEXP (x0, 0);
  goto L6124;

 L751: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, XFmode))
    {
      operands[1] = x2;
      goto L752;
    }
  x1 = XEXP (x0, 0);
  goto L6124;

 L752: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_rhs_operand (x2, XFmode))
    {
      operands[2] = x2;
      goto L753;
    }
  x1 = XEXP (x0, 0);
  goto L6124;

 L753: ATTRIBUTE_UNUSED_LABEL
  if ((ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT))
    {
      return 76;
    }
  x1 = XEXP (x0, 0);
  goto L6124;

 L1282: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, XFmode))
    {
      operands[1] = x2;
      goto L1283;
    }
  x1 = XEXP (x0, 0);
  goto L6124;

 L1283: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT))
    {
      return 132;
    }
  x1 = XEXP (x0, 0);
  goto L6124;

 L1329: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, XFmode))
    {
      operands[1] = x2;
      goto L1330;
    }
  x1 = XEXP (x0, 0);
  goto L6124;

 L1330: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT))
    {
      return 138;
    }
  x1 = XEXP (x0, 0);
  goto L6124;

 L1350: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, XFmode))
    {
      operands[1] = x2;
      goto L1351;
    }
  x1 = XEXP (x0, 0);
  goto L6124;

 L1351: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT))
    {
      return 142;
    }
  x1 = XEXP (x0, 0);
  goto L6124;

 L1407: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L1408;
    }
  x1 = XEXP (x0, 0);
  goto L6124;

 L1408: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT))
    {
      return 150;
    }
  x1 = XEXP (x0, 0);
  goto L6124;

 L1520: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  switch (GET_MODE (x2))
    {
    case SFmode:
      goto L6344;
    case DFmode:
      goto L6345;
    default:
      break;
    }
  x1 = XEXP (x0, 0);
  goto L6124;

 L6344: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, SFmode))
    {
      operands[1] = x2;
      goto L1521;
    }
  x1 = XEXP (x0, 0);
  goto L6124;

 L1521: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT))
    {
      return 171;
    }
  x1 = XEXP (x0, 0);
  goto L6124;

 L6345: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, DFmode))
    {
      operands[1] = x2;
      goto L1526;
    }
  x1 = XEXP (x0, 0);
  goto L6124;

 L1526: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT))
    {
      return 172;
    }
  x1 = XEXP (x0, 0);
  goto L6124;

 L1670: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (general_operand (x1, XFmode))
    {
      operands[1] = x1;
      goto L1671;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L1671: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT && (ENABLE_XF_PATTERNS || reload_completed)))
    {
      return 200;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L6110: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x1) == REG
      && XINT (x1, 0) == 24)
    goto L1476;
  goto L2043;

 L1476: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == CC_Zmode
      && GET_CODE (x1) == COMPARE)
    goto L1477;
  x1 = XEXP (x0, 0);
  goto L2043;

 L1477: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, QImode))
    {
      operands[0] = x2;
      goto L1478;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L1478: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 0
      && (TARGET_ARM))
    {
      return 164;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L6111: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x1, HImode))
    {
      operands[0] = x1;
      goto L1499;
    }
 L6117: ATTRIBUTE_UNUSED_LABEL
  if (nonimmediate_operand (x1, HImode))
    {
      operands[0] = x1;
      goto L1600;
    }
 L6118: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x1, HImode))
    {
      operands[0] = x1;
      goto L1613;
    }
  goto L2043;

 L1499: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == HImode
      && GET_CODE (x1) == SIGN_EXTEND)
    goto L1500;
  x1 = XEXP (x0, 0);
  goto L6117;

 L1500: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (memory_operand (x2, QImode))
    {
      operands[1] = x2;
      goto L1501;
    }
  x1 = XEXP (x0, 0);
  goto L6117;

 L1501: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && arm_arch4))
    {
      return 167;
    }
  x1 = XEXP (x0, 0);
  goto L6117;

 L1600: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (general_operand (x1, HImode))
    {
      operands[1] = x1;
      goto L1601;
    }
  x1 = XEXP (x0, 0);
  goto L6118;

 L1601: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB
   && (   register_operand (operands[0], HImode)
       || register_operand (operands[1], HImode))))
    {
      return 184;
    }
 L1610: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM
   && arm_arch4
   && (GET_CODE (operands[1]) != CONST_INT
       || const_ok_for_arm (INTVAL (operands[1]))
       || const_ok_for_arm (~INTVAL (operands[1])))))
    {
      return 186;
    }
  x1 = XEXP (x0, 0);
  goto L6118;

 L1613: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (general_operand (x1, HImode))
    {
      operands[1] = x1;
      goto L1614;
    }
 L1627: ATTRIBUTE_UNUSED_LABEL
  if (arm_rhs_operand (x1, HImode))
    {
      operands[1] = x1;
      goto L1628;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L1614: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM
   && !arm_arch4
   && !BYTES_BIG_ENDIAN
   && !TARGET_MMU_TRAPS
   && (GET_CODE (operands[1]) != CONST_INT
       || const_ok_for_arm (INTVAL (operands[1]))
       || const_ok_for_arm (~INTVAL (operands[1])))))
    {
      return 187;
    }
 L1618: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM
   && !arm_arch4
   && BYTES_BIG_ENDIAN
   && !TARGET_MMU_TRAPS
   && (GET_CODE (operands[1]) != CONST_INT
       || const_ok_for_arm (INTVAL (operands[1]))
       || const_ok_for_arm (~INTVAL (operands[1])))))
    {
      return 188;
    }
  x1 = XEXP (x0, 1);
  goto L1627;

 L1628: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_MMU_TRAPS))
    {
      return 190;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L6119: ATTRIBUTE_UNUSED_LABEL
  if (nonimmediate_operand (x1, QImode))
    {
      operands[0] = x1;
      goto L1638;
    }
  goto L2043;

 L1638: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (general_operand (x1, QImode))
    {
      operands[1] = x1;
      goto L1639;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L1639: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM
   && (   register_operand (operands[0], QImode)
       || register_operand (operands[1], QImode))))
    {
      return 192;
    }
 L1643: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB
   && (   register_operand (operands[0], QImode)
       || register_operand (operands[1], QImode))))
    {
      return 193;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L6125: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x1) == REG
      && XINT (x1, 0) == 24)
    goto L2077;
  goto L2043;

 L2077: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == CC_SWPmode
      && GET_CODE (x1) == COMPARE)
    goto L2078;
  x1 = XEXP (x0, 0);
  goto L2043;

 L2078: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (shift_operator (x2, SImode))
    {
      operands[3] = x2;
      goto L2079;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L2079: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L2080;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L2080: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (reg_or_int_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L2081;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L2081: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L2082;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L2082: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 219;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L6126: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x1) == REG
      && XINT (x1, 0) == 24)
    goto L2094;
  goto L2043;

 L2094: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == CCFPmode
      && GET_CODE (x1) == COMPARE)
    goto L2095;
  x1 = XEXP (x0, 0);
  goto L2043;

 L2095: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  switch (GET_MODE (x2))
    {
    case SFmode:
      goto L6346;
    case DFmode:
      goto L6348;
    case XFmode:
      goto L6349;
    default:
      break;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L6346: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, SFmode))
    {
      operands[0] = x2;
      goto L2096;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L2096: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_add_operand (x2, SFmode))
    {
      operands[1] = x2;
      goto L2097;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L2097: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 221;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L6348: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == FLOAT_EXTEND)
    goto L2108;
  if (s_register_operand (x2, DFmode))
    {
      operands[0] = x2;
      goto L2102;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L2108: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      operands[0] = x3;
      goto L2109;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L2109: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_add_operand (x2, DFmode))
    {
      operands[1] = x2;
      goto L2110;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L2110: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 223;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L2102: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == DFmode)
    goto L6351;
  x1 = XEXP (x0, 0);
  goto L2043;

 L6351: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == FLOAT_EXTEND)
    goto L2116;
  if (fpu_add_operand (x2, DFmode))
    {
      operands[1] = x2;
      goto L2103;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L2116: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      operands[1] = x3;
      goto L2117;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L2117: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 224;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L2103: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 222;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L6349: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, XFmode))
    {
      operands[0] = x2;
      goto L2122;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L2122: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_add_operand (x2, XFmode))
    {
      operands[1] = x2;
      goto L2123;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L2123: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT))
    {
      return 225;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L6127: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x1) == REG
      && XINT (x1, 0) == 24)
    goto L2126;
  goto L2043;

 L2126: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == CCFPEmode
      && GET_CODE (x1) == COMPARE)
    goto L2127;
  x1 = XEXP (x0, 0);
  goto L2043;

 L2127: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  switch (GET_MODE (x2))
    {
    case SFmode:
      goto L6352;
    case DFmode:
      goto L6354;
    case XFmode:
      goto L6355;
    default:
      break;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L6352: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, SFmode))
    {
      operands[0] = x2;
      goto L2128;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L2128: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_add_operand (x2, SFmode))
    {
      operands[1] = x2;
      goto L2129;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L2129: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 226;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L6354: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == FLOAT_EXTEND)
    goto L2140;
  if (s_register_operand (x2, DFmode))
    {
      operands[0] = x2;
      goto L2134;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L2140: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      operands[0] = x3;
      goto L2141;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L2141: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_add_operand (x2, DFmode))
    {
      operands[1] = x2;
      goto L2142;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L2142: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 228;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L2134: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == DFmode)
    goto L6357;
  x1 = XEXP (x0, 0);
  goto L2043;

 L6357: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == FLOAT_EXTEND)
    goto L2148;
  if (fpu_add_operand (x2, DFmode))
    {
      operands[1] = x2;
      goto L2135;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L2148: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      operands[1] = x3;
      goto L2149;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L2149: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 229;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L2135: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 227;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L6355: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, XFmode))
    {
      operands[0] = x2;
      goto L2154;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L2154: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_add_operand (x2, XFmode))
    {
      operands[1] = x2;
      goto L2155;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L2155: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT))
    {
      return 230;
    }
  x1 = XEXP (x0, 0);
  goto L2043;

 L2531: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == SImode)
    goto L6358;
 L2044: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x1))
    {
    case IF_THEN_ELSE:
      goto L2162;
    case LABEL_REF:
      goto L2272;
    default:
     break;
   }
  x1 = XEXP (x0, 0);
  goto L2217;

 L6358: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x1, SImode))
    {
      operands[0] = x1;
      goto L2532;
    }
 L6359: ATTRIBUTE_UNUSED_LABEL
  if (memory_operand (x1, SImode))
    {
      operands[0] = x1;
      goto L2536;
    }
 L6360: ATTRIBUTE_UNUSED_LABEL
  if (register_operand (x1, SImode))
    {
      operands[0] = x1;
      goto L2540;
    }
  goto L2044;

 L2532: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 264;
    }
  x1 = XEXP (x0, 1);
  goto L6359;

 L2536: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 265;
    }
  x1 = XEXP (x0, 1);
  goto L6360;

 L2540: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB))
    {
      return 266;
    }
  x1 = XEXP (x0, 1);
  goto L2044;

 L2162: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  switch (GET_CODE (x2))
    {
    case UNEQ:
      goto L2163;
    case LTGT:
      goto L2172;
    case EQ:
    case NE:
    case LE:
    case LT:
    case GE:
    case GT:
    case GEU:
    case GTU:
    case LEU:
    case LTU:
    case UNORDERED:
    case ORDERED:
    case UNLT:
    case UNLE:
    case UNGE:
    case UNGT:
      goto L2045;
    default:
      goto L2180;
   }
 L2045: ATTRIBUTE_UNUSED_LABEL
  if (arm_comparison_operator (x2, VOIDmode))
    {
      operands[0] = x2;
      goto L2046;
    }
 L2180: ATTRIBUTE_UNUSED_LABEL
  if (arm_comparison_operator (x2, VOIDmode))
    {
      operands[1] = x2;
      goto L2181;
    }
  x1 = XEXP (x0, 0);
  goto L2217;

 L2163: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (cc_register (x3, VOIDmode))
    {
      operands[1] = x3;
      goto L2164;
    }
  x1 = XEXP (x0, 0);
  goto L2217;

 L2164: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L2165;
  x1 = XEXP (x0, 0);
  goto L2217;

 L2165: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  switch (GET_CODE (x2))
    {
    case LABEL_REF:
      goto L2166;
    case PC:
      goto L2193;
    default:
     break;
   }
  x1 = XEXP (x0, 0);
  goto L2217;

 L2166: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  operands[0] = x3;
  goto L2167;

 L2167: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == PC
      && (TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 232;
    }
  x1 = XEXP (x0, 0);
  goto L2217;

 L2193: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == LABEL_REF)
    goto L2194;
  x1 = XEXP (x0, 0);
  goto L2217;

 L2194: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  operands[0] = x3;
  goto L2195;

 L2195: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 235;
    }
  x1 = XEXP (x0, 0);
  goto L2217;

 L2172: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (cc_register (x3, VOIDmode))
    {
      operands[1] = x3;
      goto L2173;
    }
  x1 = XEXP (x0, 0);
  goto L2217;

 L2173: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L2174;
  x1 = XEXP (x0, 0);
  goto L2217;

 L2174: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  switch (GET_CODE (x2))
    {
    case LABEL_REF:
      goto L2175;
    case PC:
      goto L2203;
    default:
     break;
   }
  x1 = XEXP (x0, 0);
  goto L2217;

 L2175: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  operands[0] = x3;
  goto L2176;

 L2176: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == PC
      && (TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 233;
    }
  x1 = XEXP (x0, 0);
  goto L2217;

 L2203: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == LABEL_REF)
    goto L2204;
  x1 = XEXP (x0, 0);
  goto L2217;

 L2204: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  operands[0] = x3;
  goto L2205;

 L2205: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 236;
    }
  x1 = XEXP (x0, 0);
  goto L2217;

 L2046: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L2056;
    }
 L2494: ATTRIBUTE_UNUSED_LABEL
  if (cc_register (x3, VOIDmode))
    {
      operands[1] = x3;
      goto L2495;
    }
  goto L2180;

 L2056: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == NEG)
    goto L2057;
  if (nonmemory_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L2048;
    }
  x3 = XEXP (x2, 0);
  goto L2494;

 L2057: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (nonmemory_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L2058;
    }
  x3 = XEXP (x2, 0);
  goto L2494;

 L2058: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == LABEL_REF)
    goto L2059;
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2494;

 L2059: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  operands[3] = x3;
  goto L2060;

 L2060: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == PC
      && (TARGET_THUMB))
    {
      return 216;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2494;

 L2048: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == LABEL_REF)
    goto L2049;
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2494;

 L2049: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  operands[3] = x3;
  goto L2050;

 L2050: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == PC
      && (TARGET_THUMB))
    {
      return 215;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L2494;

 L2495: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L2496;
  goto L2180;

 L2496: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  switch (GET_CODE (x2))
    {
    case RETURN:
      goto L2497;
    case PC:
      goto L2505;
    default:
     break;
   }
  x2 = XEXP (x1, 0);
  goto L2180;

 L2497: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == PC
      && (TARGET_ARM && USE_RETURN_INSN (TRUE)))
    {
      return 260;
    }
  x2 = XEXP (x1, 0);
  goto L2180;

 L2505: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == RETURN
      && (TARGET_ARM && USE_RETURN_INSN (TRUE)))
    {
      return 261;
    }
  x2 = XEXP (x1, 0);
  goto L2180;

 L2181: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (cc_register (x3, VOIDmode))
    {
      operands[2] = x3;
      goto L2182;
    }
  x1 = XEXP (x0, 0);
  goto L2217;

 L2182: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L2183;
  x1 = XEXP (x0, 0);
  goto L2217;

 L2183: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  switch (GET_CODE (x2))
    {
    case LABEL_REF:
      goto L2184;
    case PC:
      goto L2213;
    default:
     break;
   }
  x1 = XEXP (x0, 0);
  goto L2217;

 L2184: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  operands[0] = x3;
  goto L2185;

 L2185: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == PC
      && (TARGET_ARM))
    {
      return 234;
    }
  x1 = XEXP (x0, 0);
  goto L2217;

 L2213: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == LABEL_REF)
    goto L2214;
  x1 = XEXP (x0, 0);
  goto L2217;

 L2214: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  operands[0] = x3;
  goto L2215;

 L2215: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 237;
    }
  x1 = XEXP (x0, 0);
  goto L2217;

 L2272: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  operands[0] = x2;
  goto L2273;

 L2273: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 245;
    }
 L2278: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB))
    {
      return 246;
    }
  x1 = XEXP (x0, 0);
  goto L2217;

 L6128: ATTRIBUTE_UNUSED_LABEL
  if (cc_register (x1, VOIDmode))
    {
      operands[0] = x1;
      goto L2158;
    }
  goto L2217;

 L2158: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (rtx_equal_p (x1, operands[0])
      && (TARGET_ARM))
    {
      return 231;
    }
  x1 = XEXP (x0, 0);
  goto L2217;

 L6129: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x1, SImode))
    {
      operands[0] = x1;
      goto L2218;
    }
  goto L2637;

 L2218: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == SImode)
    goto L6362;
  x1 = XEXP (x0, 0);
  goto L2637;

 L6362: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x1))
    {
    case NEG:
      goto L2224;
    case NOT:
      goto L2230;
    case IF_THEN_ELSE:
      goto L2236;
    case MINUS:
      goto L2592;
    case EQ:
    case NE:
    case LE:
    case LT:
    case GE:
    case GT:
    case GEU:
    case GTU:
    case LEU:
    case LTU:
    case UNORDERED:
    case ORDERED:
    case UNLT:
    case UNLE:
    case UNGE:
    case UNGT:
      goto L6361;
    default:
      goto L6365;
   }
 L6361: ATTRIBUTE_UNUSED_LABEL
  if (arm_comparison_operator (x1, SImode))
    {
      operands[1] = x1;
      goto L2219;
    }
 L6365: ATTRIBUTE_UNUSED_LABEL
  if (shiftable_operator (x1, SImode))
    {
      operands[1] = x1;
      goto L2545;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L2224: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (arm_comparison_operator (x2, SImode))
    {
      operands[1] = x2;
      goto L2225;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L2225: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (cc_register (x3, VOIDmode))
    {
      operands[2] = x3;
      goto L2226;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L2226: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0
      && (TARGET_ARM))
    {
      return 239;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L2230: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (arm_comparison_operator (x2, SImode))
    {
      operands[1] = x2;
      goto L2231;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L2231: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (cc_register (x3, VOIDmode))
    {
      operands[2] = x3;
      goto L2232;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L2232: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0
      && (TARGET_ARM))
    {
      return 240;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L2236: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (arm_comparison_operator (x2, VOIDmode))
    {
      operands[3] = x2;
      goto L2237;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L2237: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (cc_register (x3, VOIDmode))
    {
      operands[4] = x3;
      goto L2238;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L2238: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L2239;
  x1 = XEXP (x0, 0);
  goto L2637;

 L2239: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_not_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L2240;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L2240: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (arm_not_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L2241;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L2241: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 241;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L2592: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L2593;
    }
  goto L6365;

 L2593: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (shift_operator (x2, SImode))
    {
      operands[2] = x2;
      goto L2594;
    }
  goto L6365;

 L2594: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L2595;
    }
  goto L6365;

 L2595: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (reg_or_int_operand (x3, SImode))
    {
      operands[4] = x3;
      goto L2596;
    }
  goto L6365;

 L2596: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 271;
    }
  x1 = XEXP (x0, 1);
  goto L6365;

 L2219: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (cc_register (x2, VOIDmode))
    {
      operands[2] = x2;
      goto L2220;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L2220: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 0
      && (TARGET_ARM))
    {
      return 238;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L2545: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (shift_operator (x2, SImode))
    {
      operands[3] = x2;
      goto L2546;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L2546: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[4] = x3;
      goto L2547;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L2547: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (reg_or_int_operand (x3, SImode))
    {
      operands[5] = x3;
      goto L2548;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L2548: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L2549;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L2549: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 268;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L6130: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x1, SFmode))
    {
      operands[0] = x1;
      goto L2244;
    }
  goto L2637;

 L2244: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == SFmode
      && GET_CODE (x1) == IF_THEN_ELSE)
    goto L2245;
  x1 = XEXP (x0, 0);
  goto L2637;

 L2245: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (arm_comparison_operator (x2, VOIDmode))
    {
      operands[3] = x2;
      goto L2246;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L2246: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (cc_register (x3, VOIDmode))
    {
      operands[4] = x3;
      goto L2247;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L2247: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L2248;
  x1 = XEXP (x0, 0);
  goto L2637;

 L2248: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SFmode)
    goto L6367;
  x1 = XEXP (x0, 0);
  goto L2637;

 L6367: ATTRIBUTE_UNUSED_LABEL
  if (fpu_add_operand (x2, SFmode))
    {
      operands[1] = x2;
      goto L2249;
    }
 L6368: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, SFmode))
    {
      operands[1] = x2;
      goto L2258;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L2249: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (fpu_add_operand (x2, SFmode))
    {
      operands[2] = x2;
      goto L2250;
    }
  x2 = XEXP (x1, 1);
  goto L6368;

 L2250: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 242;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 1);
  goto L6368;

 L2258: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (s_register_operand (x2, SFmode))
    {
      operands[2] = x2;
      goto L2259;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L2259: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_SOFT_FLOAT))
    {
      return 243;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L6131: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x1, DFmode))
    {
      operands[0] = x1;
      goto L2262;
    }
  goto L2637;

 L2262: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == DFmode
      && GET_CODE (x1) == IF_THEN_ELSE)
    goto L2263;
  x1 = XEXP (x0, 0);
  goto L2637;

 L2263: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (arm_comparison_operator (x2, VOIDmode))
    {
      operands[3] = x2;
      goto L2264;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L2264: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (cc_register (x3, VOIDmode))
    {
      operands[4] = x3;
      goto L2265;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L2265: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L2266;
  x1 = XEXP (x0, 0);
  goto L2637;

 L2266: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (fpu_add_operand (x2, DFmode))
    {
      operands[1] = x2;
      goto L2267;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L2267: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (fpu_add_operand (x2, DFmode))
    {
      operands[2] = x2;
      goto L2268;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L2268: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && TARGET_HARD_FLOAT))
    {
      return 244;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L6132: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x1) == REG
      && XINT (x1, 0) == 24)
    goto L2582;
  goto L2637;

 L2582: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == CC_NOOVmode
      && GET_CODE (x1) == COMPARE)
    goto L2583;
  x1 = XEXP (x0, 0);
  goto L2637;

 L2583: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode)
    goto L6370;
  x1 = XEXP (x0, 0);
  goto L2637;

 L6370: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == MINUS)
    goto L2631;
 L6369: ATTRIBUTE_UNUSED_LABEL
  if (shiftable_operator (x2, SImode))
    {
      operands[1] = x2;
      goto L2584;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L2631: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L2632;
    }
  goto L6369;

 L2632: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (shift_operator (x3, SImode))
    {
      operands[2] = x3;
      goto L2633;
    }
  goto L6369;

 L2633: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[3] = x4;
      goto L2634;
    }
  goto L6369;

 L2634: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (reg_or_int_operand (x4, SImode))
    {
      operands[4] = x4;
      goto L2635;
    }
  goto L6369;

 L2635: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 0
      && (TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 273;
    }
  x2 = XEXP (x1, 0);
  goto L6369;

 L2584: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (shift_operator (x3, SImode))
    {
      operands[3] = x3;
      goto L2585;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L2585: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[4] = x4;
      goto L2586;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L2586: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (reg_or_int_operand (x4, SImode))
    {
      operands[5] = x4;
      goto L2587;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L2587: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L2588;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L2588: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 0
      && (TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 270;
    }
  x1 = XEXP (x0, 0);
  goto L2637;

 L2638: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == SImode
      && GET_CODE (x1) == PLUS)
    goto L2639;
  x1 = XEXP (x0, 0);
  goto L2683;

 L2639: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L2640;
  x1 = XEXP (x0, 0);
  goto L2683;

 L2640: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (shift_operator (x3, SImode))
    {
      operands[5] = x3;
      goto L2641;
    }
  x1 = XEXP (x0, 0);
  goto L2683;

 L2641: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  operands[3] = x4;
  goto L2642;

 L2642: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  operands[4] = x4;
  goto L2643;

 L2643: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  operands[2] = x3;
  goto L2644;

 L2644: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (const_int_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L2645;
    }
  x1 = XEXP (x0, 0);
  goto L2683;

 L2645: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && reload_in_progress))
    {
      return 274;
    }
  x1 = XEXP (x0, 0);
  goto L2683;

 L2684: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == CC_NOOVmode
      && GET_CODE (x1) == COMPARE)
    goto L2685;
  x1 = XEXP (x0, 0);
  goto L2694;

 L2685: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L2686;
  x1 = XEXP (x0, 0);
  goto L2694;

 L2686: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L2687;
  x1 = XEXP (x0, 0);
  goto L2694;

 L2687: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (shift_operator (x4, SImode))
    {
      operands[5] = x4;
      goto L2688;
    }
  x1 = XEXP (x0, 0);
  goto L2694;

 L2688: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  operands[3] = x5;
  goto L2689;

 L2689: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  operands[4] = x5;
  goto L2690;

 L2690: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  operands[1] = x4;
  goto L2691;

 L2691: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (const_int_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L2692;
    }
  x1 = XEXP (x0, 0);
  goto L2694;

 L2692: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 0
      && (TARGET_ARM && reload_in_progress && !arm_is_xscale)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 276;
    }
  x1 = XEXP (x0, 0);
  goto L2694;

 L2695: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == SImode
      && GET_CODE (x1) == PLUS)
    goto L2696;
  x1 = XEXP (x0, 0);
  goto L2740;

 L2696: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L2697;
  x1 = XEXP (x0, 0);
  goto L2740;

 L2697: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MULT)
    goto L2698;
  x1 = XEXP (x0, 0);
  goto L2740;

 L2698: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  operands[1] = x4;
  goto L2699;

 L2699: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  operands[2] = x4;
  goto L2700;

 L2700: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  operands[3] = x3;
  goto L2701;

 L2701: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (const_int_operand (x2, SImode))
    {
      operands[4] = x2;
      goto L2702;
    }
  x1 = XEXP (x0, 0);
  goto L2740;

 L2702: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && reload_in_progress))
    {
      return 277;
    }
  x1 = XEXP (x0, 0);
  goto L2740;

 L6133: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x1) == REG
      && XINT (x1, 0) == 24)
    goto L2741;
  goto L2828;

 L2741: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == CC_NOOVmode
      && GET_CODE (x1) == COMPARE)
    goto L2742;
  x1 = XEXP (x0, 0);
  goto L2828;

 L2742: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L2743;
  x1 = XEXP (x0, 0);
  goto L2828;

 L2743: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L2744;
  x1 = XEXP (x0, 0);
  goto L2828;

 L2744: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode
      && GET_CODE (x4) == MULT)
    goto L2745;
  x1 = XEXP (x0, 0);
  goto L2828;

 L2745: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  operands[3] = x5;
  goto L2746;

 L2746: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  operands[4] = x5;
  goto L2747;

 L2747: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  operands[1] = x4;
  goto L2748;

 L2748: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (const_int_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L2749;
    }
  x1 = XEXP (x0, 0);
  goto L2828;

 L2749: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 0
      && (TARGET_ARM && reload_in_progress)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 279;
    }
  x1 = XEXP (x0, 0);
  goto L2828;

 L6134: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x1, SImode))
    {
      operands[0] = x1;
      goto L2752;
    }
  goto L2828;

 L2752: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == SImode)
    goto L6371;
  x1 = XEXP (x0, 0);
  goto L2828;

 L6371: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x1))
    {
    case AND:
      goto L2753;
    case IOR:
      goto L2761;
    case IF_THEN_ELSE:
      goto L2783;
    case MINUS:
      goto L2822;
    case NEG:
      goto L2885;
    case SIGN_EXTRACT:
      goto L3743;
    case NOT:
      goto L3750;
    case UNSPEC:
      goto L6382;
    case EQ:
    case NE:
    case LE:
    case LT:
    case GE:
    case GT:
    case GEU:
    case GTU:
    case LEU:
    case LTU:
    case UNORDERED:
    case ORDERED:
    case UNLT:
    case UNLE:
    case UNGE:
    case UNGT:
      goto L6373;
    default:
      goto L6375;
   }
 L6373: ATTRIBUTE_UNUSED_LABEL
  if (arm_comparison_operator (x1, SImode))
    {
      operands[1] = x1;
      goto L2777;
    }
 L6375: ATTRIBUTE_UNUSED_LABEL
  if (shiftable_operator (x1, SImode))
    {
      operands[5] = x1;
      goto L2804;
    }
 L6378: ATTRIBUTE_UNUSED_LABEL
  if (shiftable_operator (x1, SImode))
    {
      operands[1] = x1;
      goto L3417;
    }
  x1 = XEXP (x0, 0);
  goto L2828;

 L2753: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (arm_comparison_operator (x2, SImode))
    {
      operands[1] = x2;
      goto L2754;
    }
  goto L6375;

 L2754: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (cc_register (x3, VOIDmode))
    {
      operands[3] = x3;
      goto L2755;
    }
  goto L6375;

 L2755: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L2756;
  goto L6375;

 L2756: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L2757;
    }
  goto L6375;

 L2757: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 280;
    }
  x1 = XEXP (x0, 1);
  goto L6375;

 L2761: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (arm_comparison_operator (x2, SImode))
    {
      operands[2] = x2;
      goto L2762;
    }
  goto L6375;

 L2762: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (cc_register (x3, VOIDmode))
    {
      operands[3] = x3;
      goto L2763;
    }
  goto L6375;

 L2763: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0)
    goto L2764;
  goto L6375;

 L2764: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L2765;
    }
  goto L6375;

 L2765: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 281;
    }
  x1 = XEXP (x0, 1);
  goto L6375;

 L2783: ATTRIBUTE_UNUSED_LABEL
  tem = recog_12 (x0, insn, pnum_clobbers);
  if (tem >= 0)
    return tem;
  x1 = XEXP (x0, 0);
  goto L2828;

 L2822: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L2823;
    }
  goto L6375;

 L2823: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_comparison_operator (x2, SImode))
    {
      operands[4] = x2;
      goto L2824;
    }
  goto L6375;

 L2824: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L2825;
    }
  goto L6375;

 L2825: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L2826;
    }
  goto L6375;

 L2826: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 285;
    }
  x1 = XEXP (x0, 1);
  goto L6375;

 L2885: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (arm_comparison_operator (x2, VOIDmode))
    {
      operands[3] = x2;
      goto L2886;
    }
  x1 = XEXP (x0, 0);
  goto L2828;

 L2886: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L2887;
    }
  x1 = XEXP (x0, 0);
  goto L2828;

 L2887: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L2888;
    }
  x1 = XEXP (x0, 0);
  goto L2828;

 L2888: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 290;
    }
  x1 = XEXP (x0, 0);
  goto L2828;

 L3743: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L3744;
    }
  x1 = XEXP (x0, 0);
  goto L2828;

 L3744: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 1)
    goto L3745;
  x1 = XEXP (x0, 0);
  goto L2828;

 L3745: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 2);
  if (const_int_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L3746;
    }
  x1 = XEXP (x0, 0);
  goto L2828;

 L3746: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 346;
    }
  x1 = XEXP (x0, 0);
  goto L2828;

 L3750: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == SIGN_EXTRACT)
    goto L3751;
  x1 = XEXP (x0, 0);
  goto L2828;

 L3751: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L3752;
    }
  x1 = XEXP (x0, 0);
  goto L2828;

 L3752: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 1)
    goto L3753;
  x1 = XEXP (x0, 0);
  goto L2828;

 L3753: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (const_int_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L3754;
    }
  x1 = XEXP (x0, 0);
  goto L2828;

 L3754: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return 347;
    }
  x1 = XEXP (x0, 0);
  goto L2828;

 L6382: ATTRIBUTE_UNUSED_LABEL
  if (XVECLEN (x1, 0) == 1
      && XINT (x1, 1) == 5)
    goto L3794;
  x1 = XEXP (x0, 0);
  goto L2828;

 L3794: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L3795;
    }
  x1 = XEXP (x0, 0);
  goto L2828;

 L3795: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && arm_arch5))
    {
      return 357;
    }
  x1 = XEXP (x0, 0);
  goto L2828;

 L2777: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L2778;
    }
  x1 = XEXP (x0, 0);
  goto L2828;

 L2778: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (arm_add_operand (x2, SImode))
    {
      operands[3] = x2;
      goto L2779;
    }
  x1 = XEXP (x0, 0);
  goto L2828;

 L2779: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 282;
    }
  x1 = XEXP (x0, 0);
  goto L2828;

 L2804: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (arm_comparison_operator (x2, SImode))
    {
      operands[4] = x2;
      goto L2805;
    }
  goto L6378;

 L2805: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L2806;
    }
  goto L6378;

 L2806: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L2807;
    }
  goto L6378;

 L2807: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L2808;
    }
  goto L6378;

 L2808: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM)
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 284;
    }
  x1 = XEXP (x0, 1);
  goto L6378;

 L3417: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (memory_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L3418;
    }
  x1 = XEXP (x0, 0);
  goto L2828;

 L3418: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (memory_operand (x2, SImode))
    {
      operands[3] = x2;
      goto L3419;
    }
  x1 = XEXP (x0, 0);
  goto L2828;

 L3419: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && adjacent_mem_locations (operands[2], operands[3]))
      && pnum_clobbers != NULL)
    {
      *pnum_clobbers = 1;
      return 320;
    }
  x1 = XEXP (x0, 0);
  goto L2828;

 L2829: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_CODE (x1) == COMPARE)
    goto L2830;
  goto ret0;

 L2830: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode)
    goto L6394;
  goto ret0;

 L6394: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x2))
    {
    case IF_THEN_ELSE:
      goto L2831;
    case AND:
      goto L2855;
    case IOR:
      goto L2866;
    default:
     break;
   }
  goto ret0;

 L2831: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (arm_comparison_operator (x3, VOIDmode))
    {
      operands[4] = x3;
      goto L2832;
    }
  goto ret0;

 L2832: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[0] = x4;
      goto L2833;
    }
  goto ret0;

 L2833: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_add_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L2834;
    }
  goto ret0;

 L2834: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_comparison_operator (x3, SImode))
    {
      operands[5] = x3;
      goto L2835;
    }
  goto ret0;

 L2835: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L2836;
    }
  goto ret0;

 L2836: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_add_operand (x4, SImode))
    {
      operands[3] = x4;
      goto L2837;
    }
  goto ret0;

 L2837: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (GET_CODE (x3) == CONST_INT)
    goto L6397;
  goto ret0;

 L6397: ATTRIBUTE_UNUSED_LABEL
  if ((int) XWINT (x3, 0) == XWINT (x3, 0))
    switch ((int) XWINT (x3, 0))
      {
      case 0:
        goto L2838;
      case 1:
        goto L2850;
      default:
        break;
      }
  goto ret0;

 L2838: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 0
      && (TARGET_ARM))
    {
      return 286;
    }
  goto ret0;

 L2850: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 0
      && (TARGET_ARM))
    {
      return 287;
    }
  goto ret0;

 L2855: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (arm_comparison_operator (x3, VOIDmode))
    {
      operands[4] = x3;
      goto L2856;
    }
  goto ret0;

 L2856: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[0] = x4;
      goto L2857;
    }
  goto ret0;

 L2857: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_add_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L2858;
    }
  goto ret0;

 L2858: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_comparison_operator (x3, SImode))
    {
      operands[5] = x3;
      goto L2859;
    }
  goto ret0;

 L2859: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L2860;
    }
  goto ret0;

 L2860: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_add_operand (x4, SImode))
    {
      operands[3] = x4;
      goto L2861;
    }
  goto ret0;

 L2861: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 0
      && (TARGET_ARM))
    {
      return 288;
    }
  goto ret0;

 L2866: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (arm_comparison_operator (x3, VOIDmode))
    {
      operands[4] = x3;
      goto L2867;
    }
  goto ret0;

 L2867: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[0] = x4;
      goto L2868;
    }
  goto ret0;

 L2868: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_add_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L2869;
    }
  goto ret0;

 L2869: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_comparison_operator (x3, SImode))
    {
      operands[5] = x3;
      goto L2870;
    }
  goto ret0;

 L2870: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L2871;
    }
  goto ret0;

 L2871: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_add_operand (x4, SImode))
    {
      operands[3] = x4;
      goto L2872;
    }
  goto ret0;

 L2872: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 0
      && (TARGET_ARM))
    {
      return 289;
    }
  goto ret0;
 ret0:
  return -1;
}

static int recog_14 PARAMS ((rtx, rtx, int *));
static int
recog_14 (x0, insn, pnum_clobbers)
     rtx x0 ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  rtx * const operands ATTRIBUTE_UNUSED = &recog_data.operand[0];
  rtx x1 ATTRIBUTE_UNUSED;
  rtx x2 ATTRIBUTE_UNUSED;
  rtx x3 ATTRIBUTE_UNUSED;
  rtx x4 ATTRIBUTE_UNUSED;
  rtx x5 ATTRIBUTE_UNUSED;
  rtx x6 ATTRIBUTE_UNUSED;
  rtx x7 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;

  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  switch (GET_MODE (x2))
    {
    case SImode:
      goto L6409;
    case SFmode:
      goto L6410;
    case DFmode:
      goto L6411;
    case XFmode:
      goto L6412;
    case DImode:
      goto L6413;
    default:
      break;
    }
  goto ret0;

 L6409: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L4111;
    }
  goto ret0;

 L4111: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode)
    goto L6414;
  goto ret0;

 L6414: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x2))
    {
    case PLUS:
      goto L4112;
    case MINUS:
      goto L4186;
    case MULT:
      goto L4250;
    case AND:
      goto L4552;
    case IOR:
      goto L4653;
    case XOR:
      goto L4695;
    default:
     break;
   }
  goto ret0;

 L4112: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L4113;
    }
  goto ret0;

 L4113: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (reg_or_int_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L4114;
    }
  goto ret0;

 L4114: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 510;
    }
  goto ret0;

 L4186: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (reg_or_int_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L4187;
    }
  goto ret0;

 L4187: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L4188;
    }
  goto ret0;

 L4188: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 517;
    }
  goto ret0;

 L4250: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L4251;
    }
  goto ret0;

 L4251: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L4252;
    }
  goto ret0;

 L4252: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 523;
    }
  goto ret0;

 L4552: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode)
    goto L6421;
  goto ret0;

 L6421: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == NOT)
    goto L4598;
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L4553;
    }
  goto ret0;

 L4598: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L4599;
    }
  goto ret0;

 L4599: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L4600;
    }
  goto ret0;

 L4600: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 553;
    }
  goto ret0;

 L4553: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (reg_or_int_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L4554;
    }
  goto ret0;

 L4554: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 549;
    }
  goto ret0;

 L4653: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L4654;
    }
  goto ret0;

 L4654: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (reg_or_int_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L4655;
    }
  goto ret0;

 L4655: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 558;
    }
  goto ret0;

 L4695: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L4696;
    }
  goto ret0;

 L4696: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L4697;
    }
  goto ret0;

 L4697: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 562;
    }
  goto ret0;

 L6410: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, SFmode))
    {
      operands[0] = x2;
      goto L4121;
    }
  goto ret0;

 L4121: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SFmode)
    goto L6422;
  goto ret0;

 L6422: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x2))
    {
    case PLUS:
      goto L4122;
    case MULT:
      goto L4360;
    case DIV:
      goto L4424;
    case MOD:
      goto L4488;
    default:
     break;
   }
  goto ret0;

 L4122: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      operands[1] = x3;
      goto L4123;
    }
  goto ret0;

 L4123: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (fpu_add_operand (x3, SFmode))
    {
      operands[2] = x3;
      goto L4124;
    }
  goto ret0;

 L4124: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 511;
    }
  goto ret0;

 L4360: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      operands[1] = x3;
      goto L4361;
    }
  goto ret0;

 L4361: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (fpu_rhs_operand (x3, SFmode))
    {
      operands[2] = x3;
      goto L4362;
    }
  goto ret0;

 L4362: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 531;
    }
  goto ret0;

 L4424: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (fpu_rhs_operand (x3, SFmode))
    {
      operands[1] = x3;
      goto L4425;
    }
  goto ret0;

 L4425: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (fpu_rhs_operand (x3, SFmode))
    {
      operands[2] = x3;
      goto L4426;
    }
  goto ret0;

 L4426: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 537;
    }
  goto ret0;

 L4488: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      operands[1] = x3;
      goto L4489;
    }
  goto ret0;

 L4489: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (fpu_rhs_operand (x3, SFmode))
    {
      operands[2] = x3;
      goto L4490;
    }
  goto ret0;

 L4490: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 543;
    }
  goto ret0;

 L6411: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, DFmode))
    {
      operands[0] = x2;
      goto L4131;
    }
  goto ret0;

 L4131: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == DFmode)
    goto L6426;
  goto ret0;

 L6426: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x2))
    {
    case PLUS:
      goto L4132;
    case MINUS:
      goto L4196;
    case MULT:
      goto L4370;
    case DIV:
      goto L4434;
    case MOD:
      goto L4498;
    default:
     break;
   }
  goto ret0;

 L4132: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == DFmode)
    goto L6432;
  goto ret0;

 L6432: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == FLOAT_EXTEND)
    goto L4143;
  if (s_register_operand (x3, DFmode))
    {
      operands[1] = x3;
      goto L4133;
    }
  goto ret0;

 L4143: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SFmode))
    {
      operands[1] = x4;
      goto L4144;
    }
  goto ret0;

 L4144: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == DFmode)
    goto L6434;
  goto ret0;

 L6434: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == FLOAT_EXTEND)
    goto L4167;
  if (fpu_add_operand (x3, DFmode))
    {
      operands[2] = x3;
      goto L4145;
    }
  goto ret0;

 L4167: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SFmode))
    {
      operands[2] = x4;
      goto L4168;
    }
  goto ret0;

 L4168: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 515;
    }
  goto ret0;

 L4145: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 513;
    }
  goto ret0;

 L4133: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == DFmode)
    goto L6436;
  goto ret0;

 L6436: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == FLOAT_EXTEND)
    goto L4155;
  if (fpu_add_operand (x3, DFmode))
    {
      operands[2] = x3;
      goto L4134;
    }
  goto ret0;

 L4155: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SFmode))
    {
      operands[2] = x4;
      goto L4156;
    }
  goto ret0;

 L4156: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 514;
    }
  goto ret0;

 L4134: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 512;
    }
  goto ret0;

 L4196: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == DFmode)
    goto L6438;
  goto ret0;

 L6438: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == FLOAT_EXTEND)
    goto L4207;
  if (fpu_rhs_operand (x3, DFmode))
    {
      operands[1] = x3;
      goto L4197;
    }
  goto ret0;

 L4207: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SFmode))
    {
      operands[1] = x4;
      goto L4208;
    }
  goto ret0;

 L4208: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == DFmode)
    goto L6440;
  goto ret0;

 L6440: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == FLOAT_EXTEND)
    goto L4231;
  if (fpu_rhs_operand (x3, DFmode))
    {
      operands[2] = x3;
      goto L4209;
    }
  goto ret0;

 L4231: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SFmode))
    {
      operands[2] = x4;
      goto L4232;
    }
  goto ret0;

 L4232: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 521;
    }
  goto ret0;

 L4209: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 519;
    }
  goto ret0;

 L4197: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == DFmode)
    goto L6442;
  goto ret0;

 L6442: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == FLOAT_EXTEND)
    goto L4219;
  if (fpu_rhs_operand (x3, DFmode))
    {
      operands[2] = x3;
      goto L4198;
    }
  goto ret0;

 L4219: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SFmode))
    {
      operands[2] = x4;
      goto L4220;
    }
  goto ret0;

 L4220: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 520;
    }
  goto ret0;

 L4198: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 518;
    }
  goto ret0;

 L4370: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == DFmode)
    goto L6444;
  goto ret0;

 L6444: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == FLOAT_EXTEND)
    goto L4381;
  if (s_register_operand (x3, DFmode))
    {
      operands[1] = x3;
      goto L4371;
    }
  goto ret0;

 L4381: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SFmode))
    {
      operands[1] = x4;
      goto L4382;
    }
  goto ret0;

 L4382: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == DFmode)
    goto L6446;
  goto ret0;

 L6446: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == FLOAT_EXTEND)
    goto L4405;
  if (fpu_rhs_operand (x3, DFmode))
    {
      operands[2] = x3;
      goto L4383;
    }
  goto ret0;

 L4405: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SFmode))
    {
      operands[2] = x4;
      goto L4406;
    }
  goto ret0;

 L4406: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 535;
    }
  goto ret0;

 L4383: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 533;
    }
  goto ret0;

 L4371: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == DFmode)
    goto L6448;
  goto ret0;

 L6448: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == FLOAT_EXTEND)
    goto L4393;
  if (fpu_rhs_operand (x3, DFmode))
    {
      operands[2] = x3;
      goto L4372;
    }
  goto ret0;

 L4393: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SFmode))
    {
      operands[2] = x4;
      goto L4394;
    }
  goto ret0;

 L4394: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 534;
    }
  goto ret0;

 L4372: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 532;
    }
  goto ret0;

 L4434: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == DFmode)
    goto L6450;
  goto ret0;

 L6450: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == FLOAT_EXTEND)
    goto L4445;
  if (fpu_rhs_operand (x3, DFmode))
    {
      operands[1] = x3;
      goto L4435;
    }
  goto ret0;

 L4445: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SFmode))
    {
      operands[1] = x4;
      goto L4446;
    }
  goto ret0;

 L4446: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == DFmode)
    goto L6452;
  goto ret0;

 L6452: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == FLOAT_EXTEND)
    goto L4469;
  if (fpu_rhs_operand (x3, DFmode))
    {
      operands[2] = x3;
      goto L4447;
    }
  goto ret0;

 L4469: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SFmode))
    {
      operands[2] = x4;
      goto L4470;
    }
  goto ret0;

 L4470: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 541;
    }
  goto ret0;

 L4447: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 539;
    }
  goto ret0;

 L4435: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == DFmode)
    goto L6454;
  goto ret0;

 L6454: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == FLOAT_EXTEND)
    goto L4457;
  if (fpu_rhs_operand (x3, DFmode))
    {
      operands[2] = x3;
      goto L4436;
    }
  goto ret0;

 L4457: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SFmode))
    {
      operands[2] = x4;
      goto L4458;
    }
  goto ret0;

 L4458: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 540;
    }
  goto ret0;

 L4436: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 538;
    }
  goto ret0;

 L4498: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == DFmode)
    goto L6456;
  goto ret0;

 L6456: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == FLOAT_EXTEND)
    goto L4509;
  if (s_register_operand (x3, DFmode))
    {
      operands[1] = x3;
      goto L4499;
    }
  goto ret0;

 L4509: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SFmode))
    {
      operands[1] = x4;
      goto L4510;
    }
  goto ret0;

 L4510: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == DFmode)
    goto L6458;
  goto ret0;

 L6458: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == FLOAT_EXTEND)
    goto L4533;
  if (fpu_rhs_operand (x3, DFmode))
    {
      operands[2] = x3;
      goto L4511;
    }
  goto ret0;

 L4533: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SFmode))
    {
      operands[2] = x4;
      goto L4534;
    }
  goto ret0;

 L4534: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 547;
    }
  goto ret0;

 L4511: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 545;
    }
  goto ret0;

 L4499: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == DFmode)
    goto L6460;
  goto ret0;

 L6460: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == FLOAT_EXTEND)
    goto L4521;
  if (fpu_rhs_operand (x3, DFmode))
    {
      operands[2] = x3;
      goto L4500;
    }
  goto ret0;

 L4521: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SFmode))
    {
      operands[2] = x4;
      goto L4522;
    }
  goto ret0;

 L4522: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 546;
    }
  goto ret0;

 L4500: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 544;
    }
  goto ret0;

 L6412: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, XFmode))
    {
      operands[0] = x2;
      goto L4175;
    }
  goto ret0;

 L4175: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == XFmode)
    goto L6461;
  goto ret0;

 L6461: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x2))
    {
    case PLUS:
      goto L4176;
    case MINUS:
      goto L4240;
    case MULT:
      goto L4414;
    case DIV:
      goto L4478;
    case MOD:
      goto L4542;
    default:
     break;
   }
  goto ret0;

 L4176: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, XFmode))
    {
      operands[1] = x3;
      goto L4177;
    }
  goto ret0;

 L4177: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (fpu_add_operand (x3, XFmode))
    {
      operands[2] = x3;
      goto L4178;
    }
  goto ret0;

 L4178: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)))
    {
      return 516;
    }
  goto ret0;

 L4240: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (fpu_rhs_operand (x3, XFmode))
    {
      operands[1] = x3;
      goto L4241;
    }
  goto ret0;

 L4241: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (fpu_rhs_operand (x3, XFmode))
    {
      operands[2] = x3;
      goto L4242;
    }
  goto ret0;

 L4242: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)))
    {
      return 522;
    }
  goto ret0;

 L4414: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, XFmode))
    {
      operands[1] = x3;
      goto L4415;
    }
  goto ret0;

 L4415: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (fpu_rhs_operand (x3, XFmode))
    {
      operands[2] = x3;
      goto L4416;
    }
  goto ret0;

 L4416: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)))
    {
      return 536;
    }
  goto ret0;

 L4478: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (fpu_rhs_operand (x3, XFmode))
    {
      operands[1] = x3;
      goto L4479;
    }
  goto ret0;

 L4479: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (fpu_rhs_operand (x3, XFmode))
    {
      operands[2] = x3;
      goto L4480;
    }
  goto ret0;

 L4480: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)))
    {
      return 542;
    }
  goto ret0;

 L4542: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, XFmode))
    {
      operands[1] = x3;
      goto L4543;
    }
  goto ret0;

 L4543: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (fpu_rhs_operand (x3, XFmode))
    {
      operands[2] = x3;
      goto L4544;
    }
  goto ret0;

 L4544: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)))
    {
      return 548;
    }
  goto ret0;

 L6413: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, DImode))
    {
      operands[0] = x2;
      goto L4285;
    }
  goto ret0;

 L4285: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == DImode)
    goto L6466;
  goto ret0;

 L6466: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x2))
    {
    case MULT:
      goto L4286;
    case AND:
      goto L4562;
    case IOR:
      goto L4621;
    case XOR:
      goto L4663;
    default:
     break;
   }
  goto ret0;

 L4286: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == DImode)
    goto L6470;
  goto ret0;

 L6470: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x3))
    {
    case SIGN_EXTEND:
      goto L4287;
    case ZERO_EXTEND:
      goto L4299;
    default:
     break;
   }
  goto ret0;

 L4287: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L4288;
    }
  goto ret0;

 L4288: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == DImode
      && GET_CODE (x3) == SIGN_EXTEND)
    goto L4289;
  goto ret0;

 L4289: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L4290;
    }
  goto ret0;

 L4290: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && arm_fast_multiply)))
    {
      return 526;
    }
  goto ret0;

 L4299: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L4300;
    }
  goto ret0;

 L4300: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == DImode
      && GET_CODE (x3) == ZERO_EXTEND)
    goto L4301;
  goto ret0;

 L4301: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L4302;
    }
  goto ret0;

 L4302: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && arm_fast_multiply)))
    {
      return 527;
    }
  goto ret0;

 L4562: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == DImode
      && GET_CODE (x3) == NOT)
    goto L4563;
  goto ret0;

 L4563: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == DImode)
    goto L6473;
  goto ret0;

 L6473: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x4))
    {
    case ZERO_EXTEND:
      goto L4575;
    case SIGN_EXTEND:
      goto L4587;
    case SUBREG:
    case REG:
      goto L6472;
    default:
      goto ret0;
   }
 L6472: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x4, DImode))
    {
      operands[1] = x4;
      goto L4564;
    }
  goto ret0;

 L4575: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[2] = x5;
      goto L4576;
    }
  goto ret0;

 L4576: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, DImode))
    {
      operands[1] = x3;
      goto L4577;
    }
  goto ret0;

 L4577: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 551;
    }
  goto ret0;

 L4587: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[2] = x5;
      goto L4588;
    }
  goto ret0;

 L4588: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, DImode))
    {
      operands[1] = x3;
      goto L4589;
    }
  goto ret0;

 L4589: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 552;
    }
  goto ret0;

 L4564: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, DImode))
    {
      operands[2] = x3;
      goto L4565;
    }
  goto ret0;

 L4565: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 550;
    }
  goto ret0;

 L4621: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == DImode)
    goto L6476;
  goto ret0;

 L6476: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x3))
    {
    case ZERO_EXTEND:
      goto L4632;
    case SIGN_EXTEND:
      goto L4643;
    case SUBREG:
    case REG:
      goto L6475;
    default:
      goto ret0;
   }
 L6475: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x3, DImode))
    {
      operands[1] = x3;
      goto L4622;
    }
  goto ret0;

 L4632: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L4633;
    }
  goto ret0;

 L4633: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, DImode))
    {
      operands[1] = x3;
      goto L4634;
    }
  goto ret0;

 L4634: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 556;
    }
  goto ret0;

 L4643: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L4644;
    }
  goto ret0;

 L4644: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, DImode))
    {
      operands[1] = x3;
      goto L4645;
    }
  goto ret0;

 L4645: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 557;
    }
  goto ret0;

 L4622: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, DImode))
    {
      operands[2] = x3;
      goto L4623;
    }
  goto ret0;

 L4623: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 555;
    }
  goto ret0;

 L4663: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == DImode)
    goto L6479;
  goto ret0;

 L6479: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x3))
    {
    case ZERO_EXTEND:
      goto L4674;
    case SIGN_EXTEND:
      goto L4685;
    case SUBREG:
    case REG:
      goto L6478;
    default:
      goto ret0;
   }
 L6478: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x3, DImode))
    {
      operands[1] = x3;
      goto L4664;
    }
  goto ret0;

 L4674: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L4675;
    }
  goto ret0;

 L4675: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, DImode))
    {
      operands[1] = x3;
      goto L4676;
    }
  goto ret0;

 L4676: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 560;
    }
  goto ret0;

 L4685: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L4686;
    }
  goto ret0;

 L4686: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, DImode))
    {
      operands[1] = x3;
      goto L4687;
    }
  goto ret0;

 L4687: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 561;
    }
  goto ret0;

 L4664: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, DImode))
    {
      operands[2] = x3;
      goto L4665;
    }
  goto ret0;

 L4665: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 559;
    }
  goto ret0;
 ret0:
  return -1;
}

static int recog_15 PARAMS ((rtx, rtx, int *));
static int
recog_15 (x0, insn, pnum_clobbers)
     rtx x0 ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  rtx * const operands ATTRIBUTE_UNUSED = &recog_data.operand[0];
  rtx x1 ATTRIBUTE_UNUSED;
  rtx x2 ATTRIBUTE_UNUSED;
  rtx x3 ATTRIBUTE_UNUSED;
  rtx x4 ATTRIBUTE_UNUSED;
  rtx x5 ATTRIBUTE_UNUSED;
  rtx x6 ATTRIBUTE_UNUSED;
  rtx x7 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;

  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  switch (GET_MODE (x2))
    {
    case SImode:
      goto L6494;
    case SFmode:
      goto L6495;
    case DFmode:
      goto L6496;
    case XFmode:
      goto L6497;
    case DImode:
      goto L6498;
    case HImode:
      goto L6499;
    case QImode:
      goto L6504;
    default:
      break;
    }
  goto ret0;

 L6494: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L4738;
    }
 L6500: ATTRIBUTE_UNUSED_LABEL
  if (nonimmediate_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L5056;
    }
 L6501: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L5089;
    }
  goto ret0;

 L4738: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode)
    goto L6508;
  x2 = XEXP (x1, 0);
  goto L6500;

 L6508: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x2))
    {
    case NEG:
      goto L4739;
    case NOT:
      goto L4868;
    case FIX:
      goto L4904;
    case ZERO_EXTEND:
      goto L4985;
    case SIGN_EXTEND:
      goto L5003;
    default:
     break;
   }
  x2 = XEXP (x1, 0);
  goto L6500;

 L4739: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L4740;
    }
  x2 = XEXP (x1, 0);
  goto L6500;

 L4740: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 566;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6500;

 L4868: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L4869;
    }
  x2 = XEXP (x1, 0);
  goto L6500;

 L4869: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 580;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6500;

 L4904: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  switch (GET_MODE (x3))
    {
    case SFmode:
      goto L6513;
    case DFmode:
      goto L6514;
    case XFmode:
      goto L6515;
    default:
      break;
    }
  x2 = XEXP (x1, 0);
  goto L6500;

 L6513: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x3, SFmode))
    {
      operands[1] = x3;
      goto L4905;
    }
  x2 = XEXP (x1, 0);
  goto L6500;

 L4905: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 584;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6500;

 L6514: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x3, DFmode))
    {
      operands[1] = x3;
      goto L4914;
    }
  x2 = XEXP (x1, 0);
  goto L6500;

 L4914: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 585;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6500;

 L6515: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x3, XFmode))
    {
      operands[1] = x3;
      goto L4923;
    }
  x2 = XEXP (x1, 0);
  goto L6500;

 L4923: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)))
    {
      return 586;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6500;

 L4985: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  switch (GET_MODE (x3))
    {
    case HImode:
      goto L6516;
    case QImode:
      goto L6517;
    default:
      break;
    }
  x2 = XEXP (x1, 0);
  goto L6500;

 L6516: ATTRIBUTE_UNUSED_LABEL
  if (memory_operand (x3, HImode))
    {
      operands[1] = x3;
      goto L4986;
    }
  x2 = XEXP (x1, 0);
  goto L6500;

 L4986: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && arm_arch4)))
    {
      return 593;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6500;

 L6517: ATTRIBUTE_UNUSED_LABEL
  if (memory_operand (x3, QImode))
    {
      operands[1] = x3;
      goto L4995;
    }
  x2 = XEXP (x1, 0);
  goto L6500;

 L4995: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 594;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6500;

 L5003: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  switch (GET_MODE (x3))
    {
    case HImode:
      goto L6518;
    case QImode:
      goto L6519;
    default:
      break;
    }
  x2 = XEXP (x1, 0);
  goto L6500;

 L6518: ATTRIBUTE_UNUSED_LABEL
  if (memory_operand (x3, HImode))
    {
      operands[1] = x3;
      goto L5004;
    }
  x2 = XEXP (x1, 0);
  goto L6500;

 L5004: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && arm_arch4)))
    {
      return 595;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6500;

 L6519: ATTRIBUTE_UNUSED_LABEL
  if (memory_operand (x3, QImode))
    {
      operands[1] = x3;
      goto L5022;
    }
  x2 = XEXP (x1, 0);
  goto L6500;

 L5022: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && arm_arch4)))
    {
      return 597;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6500;

 L5056: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L5057;
    }
  x2 = XEXP (x1, 0);
  goto L6501;

 L5057: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM
   && (   register_operand (operands[0], SImode)
       || register_operand (operands[1], SImode)))))
    {
      return 601;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6501;

 L5089: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == ROTATE)
    goto L5090;
  if (address_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L5065;
    }
  goto ret0;

 L5090: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode)
    goto L6520;
  goto ret0;

 L6520: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x3))
    {
    case MEM:
      goto L6522;
    case SUBREG:
      goto L6523;
    default:
     break;
   }
  goto ret0;

 L6522: ATTRIBUTE_UNUSED_LABEL
  if (offsettable_memory_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L5091;
    }
  goto ret0;

 L5091: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 16
      && ((TARGET_ARM) && (TARGET_ARM && (!TARGET_MMU_TRAPS))))
    {
      return 604;
    }
  goto ret0;

 L6523: ATTRIBUTE_UNUSED_LABEL
  if (XINT (x3, 1) == 0)
    goto L5124;
  goto ret0;

 L5124: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (memory_operand (x4, HImode))
    {
      operands[1] = x4;
      goto L5125;
    }
  goto ret0;

 L5125: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 16
      && ((TARGET_ARM) && (TARGET_ARM
   && BYTES_BIG_ENDIAN
   && !TARGET_MMU_TRAPS)))
    {
      return 608;
    }
  goto ret0;

 L5065: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM
   && reload_completed
   && (GET_CODE (operands[1]) == LABEL_REF
       || (GET_CODE (operands[1]) == CONST
	   && GET_CODE (XEXP (operands[1], 0)) == PLUS
	   && GET_CODE (XEXP (XEXP (operands[1], 0), 0)) == LABEL_REF
	   && GET_CODE (XEXP (XEXP (operands[1], 0), 1)) == CONST_INT)))))
    {
      return 602;
    }
  goto ret0;

 L6495: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, SFmode))
    {
      operands[0] = x2;
      goto L4747;
    }
 L6505: ATTRIBUTE_UNUSED_LABEL
  if (nonimmediate_operand (x2, SFmode))
    {
      operands[0] = x2;
      goto L5148;
    }
  goto ret0;

 L4747: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SFmode)
    goto L6524;
  x2 = XEXP (x1, 0);
  goto L6505;

 L6524: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x2))
    {
    case NEG:
      goto L4748;
    case ABS:
      goto L4785;
    case SQRT:
      goto L4822;
    case FLOAT:
      goto L4877;
    case FLOAT_TRUNCATE:
      goto L4931;
    default:
     break;
   }
  x2 = XEXP (x1, 0);
  goto L6505;

 L4748: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      operands[1] = x3;
      goto L4749;
    }
  x2 = XEXP (x1, 0);
  goto L6505;

 L4749: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 567;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6505;

 L4785: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      operands[1] = x3;
      goto L4786;
    }
  x2 = XEXP (x1, 0);
  goto L6505;

 L4786: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 571;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6505;

 L4822: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      operands[1] = x3;
      goto L4823;
    }
  x2 = XEXP (x1, 0);
  goto L6505;

 L4823: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 575;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6505;

 L4877: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L4878;
    }
  x2 = XEXP (x1, 0);
  goto L6505;

 L4878: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 581;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6505;

 L4931: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  switch (GET_MODE (x3))
    {
    case DFmode:
      goto L6529;
    case XFmode:
      goto L6530;
    default:
      break;
    }
  x2 = XEXP (x1, 0);
  goto L6505;

 L6529: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x3, DFmode))
    {
      operands[1] = x3;
      goto L4932;
    }
  x2 = XEXP (x1, 0);
  goto L6505;

 L4932: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 587;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6505;

 L6530: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x3, XFmode))
    {
      operands[1] = x3;
      goto L4941;
    }
  x2 = XEXP (x1, 0);
  goto L6505;

 L4941: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)))
    {
      return 588;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6505;

 L5148: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SFmode))
    {
      operands[1] = x2;
      goto L5149;
    }
  goto ret0;

 L5149: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM
   && TARGET_HARD_FLOAT
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode)))))
    {
      return 611;
    }
 L5157: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM
   && TARGET_SOFT_FLOAT
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], SFmode)))))
    {
      return 612;
    }
  goto ret0;

 L6496: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, DFmode))
    {
      operands[0] = x2;
      goto L4756;
    }
 L6506: ATTRIBUTE_UNUSED_LABEL
  if (nonimmediate_operand (x2, DFmode))
    {
      operands[0] = x2;
      goto L5164;
    }
  goto ret0;

 L4756: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == DFmode)
    goto L6531;
  x2 = XEXP (x1, 0);
  goto L6506;

 L6531: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x2))
    {
    case NEG:
      goto L4757;
    case ABS:
      goto L4794;
    case SQRT:
      goto L4831;
    case FLOAT:
      goto L4886;
    case FLOAT_TRUNCATE:
      goto L4949;
    case FLOAT_EXTEND:
      goto L5030;
    default:
     break;
   }
  x2 = XEXP (x1, 0);
  goto L6506;

 L4757: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == DFmode)
    goto L6538;
  x2 = XEXP (x1, 0);
  goto L6506;

 L6538: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == FLOAT_EXTEND)
    goto L4767;
  if (s_register_operand (x3, DFmode))
    {
      operands[1] = x3;
      goto L4758;
    }
  x2 = XEXP (x1, 0);
  goto L6506;

 L4767: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SFmode))
    {
      operands[1] = x4;
      goto L4768;
    }
  x2 = XEXP (x1, 0);
  goto L6506;

 L4768: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 569;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6506;

 L4758: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 568;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6506;

 L4794: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == DFmode)
    goto L6540;
  x2 = XEXP (x1, 0);
  goto L6506;

 L6540: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == FLOAT_EXTEND)
    goto L4804;
  if (s_register_operand (x3, DFmode))
    {
      operands[1] = x3;
      goto L4795;
    }
  x2 = XEXP (x1, 0);
  goto L6506;

 L4804: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SFmode))
    {
      operands[1] = x4;
      goto L4805;
    }
  x2 = XEXP (x1, 0);
  goto L6506;

 L4805: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 573;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6506;

 L4795: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 572;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6506;

 L4831: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == DFmode)
    goto L6542;
  x2 = XEXP (x1, 0);
  goto L6506;

 L6542: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == FLOAT_EXTEND)
    goto L4841;
  if (s_register_operand (x3, DFmode))
    {
      operands[1] = x3;
      goto L4832;
    }
  x2 = XEXP (x1, 0);
  goto L6506;

 L4841: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SFmode))
    {
      operands[1] = x4;
      goto L4842;
    }
  x2 = XEXP (x1, 0);
  goto L6506;

 L4842: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 577;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6506;

 L4832: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 576;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6506;

 L4886: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L4887;
    }
  x2 = XEXP (x1, 0);
  goto L6506;

 L4887: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 582;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6506;

 L4949: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, XFmode))
    {
      operands[1] = x3;
      goto L4950;
    }
  x2 = XEXP (x1, 0);
  goto L6506;

 L4950: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)))
    {
      return 589;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6506;

 L5030: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SFmode))
    {
      operands[1] = x3;
      goto L5031;
    }
  x2 = XEXP (x1, 0);
  goto L6506;

 L5031: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT)))
    {
      return 598;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6506;

 L5164: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (general_operand (x2, DFmode))
    {
      operands[1] = x2;
      goto L5165;
    }
  goto ret0;

 L5165: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM
   && TARGET_HARD_FLOAT
   && (GET_CODE (operands[0]) != MEM
       || register_operand (operands[1], DFmode)))))
    {
      return 613;
    }
  goto ret0;

 L6497: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, XFmode))
    {
      operands[0] = x2;
      goto L4775;
    }
 L6507: ATTRIBUTE_UNUSED_LABEL
  if (nonimmediate_operand (x2, XFmode))
    {
      operands[0] = x2;
      goto L5172;
    }
  goto ret0;

 L4775: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == XFmode)
    goto L6543;
  x2 = XEXP (x1, 0);
  goto L6507;

 L6543: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x2))
    {
    case NEG:
      goto L4776;
    case ABS:
      goto L4813;
    case SQRT:
      goto L4850;
    case FLOAT:
      goto L4895;
    case FLOAT_EXTEND:
      goto L5039;
    default:
     break;
   }
  x2 = XEXP (x1, 0);
  goto L6507;

 L4776: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, XFmode))
    {
      operands[1] = x3;
      goto L4777;
    }
  x2 = XEXP (x1, 0);
  goto L6507;

 L4777: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)))
    {
      return 570;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6507;

 L4813: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, XFmode))
    {
      operands[1] = x3;
      goto L4814;
    }
  x2 = XEXP (x1, 0);
  goto L6507;

 L4814: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)))
    {
      return 574;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6507;

 L4850: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, XFmode))
    {
      operands[1] = x3;
      goto L4851;
    }
  x2 = XEXP (x1, 0);
  goto L6507;

 L4851: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)))
    {
      return 578;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6507;

 L4895: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L4896;
    }
  x2 = XEXP (x1, 0);
  goto L6507;

 L4896: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)))
    {
      return 583;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6507;

 L5039: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  switch (GET_MODE (x3))
    {
    case SFmode:
      goto L6548;
    case DFmode:
      goto L6549;
    default:
      break;
    }
  x2 = XEXP (x1, 0);
  goto L6507;

 L6548: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x3, SFmode))
    {
      operands[1] = x3;
      goto L5040;
    }
  x2 = XEXP (x1, 0);
  goto L6507;

 L5040: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)))
    {
      return 599;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6507;

 L6549: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x3, DFmode))
    {
      operands[1] = x3;
      goto L5049;
    }
  x2 = XEXP (x1, 0);
  goto L6507;

 L5049: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && ENABLE_XF_PATTERNS && TARGET_HARD_FLOAT)))
    {
      return 600;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6507;

 L5172: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (general_operand (x2, XFmode))
    {
      operands[1] = x2;
      goto L5173;
    }
  goto ret0;

 L5173: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_HARD_FLOAT && (ENABLE_XF_PATTERNS || reload_completed))))
    {
      return 614;
    }
  goto ret0;

 L6498: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, DImode))
    {
      operands[0] = x2;
      goto L4858;
    }
  goto ret0;

 L4858: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == DImode)
    goto L6550;
  goto ret0;

 L6550: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x2))
    {
    case NOT:
      goto L4859;
    case ZERO_EXTEND:
      goto L4958;
    case SIGN_EXTEND:
      goto L4976;
    default:
     break;
   }
  goto ret0;

 L4859: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, DImode))
    {
      operands[1] = x3;
      goto L4860;
    }
  goto ret0;

 L4860: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 579;
    }
  goto ret0;

 L4958: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  switch (GET_MODE (x3))
    {
    case SImode:
      goto L6553;
    case QImode:
      goto L6554;
    default:
      break;
    }
  goto ret0;

 L6553: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L4959;
    }
  goto ret0;

 L4959: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 590;
    }
  goto ret0;

 L6554: ATTRIBUTE_UNUSED_LABEL
  if (nonimmediate_operand (x3, QImode))
    {
      operands[1] = x3;
      goto L4968;
    }
  goto ret0;

 L4968: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 591;
    }
  goto ret0;

 L4976: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L4977;
    }
  goto ret0;

 L4977: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 592;
    }
  goto ret0;

 L6499: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, HImode))
    {
      operands[0] = x2;
      goto L5011;
    }
 L6502: ATTRIBUTE_UNUSED_LABEL
  if (nonimmediate_operand (x2, HImode))
    {
      operands[0] = x2;
      goto L5098;
    }
 L6503: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, HImode))
    {
      operands[0] = x2;
      goto L5106;
    }
  goto ret0;

 L5011: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == HImode
      && GET_CODE (x2) == SIGN_EXTEND)
    goto L5012;
  x2 = XEXP (x1, 0);
  goto L6502;

 L5012: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (memory_operand (x3, QImode))
    {
      operands[1] = x3;
      goto L5013;
    }
  x2 = XEXP (x1, 0);
  goto L6502;

 L5013: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && arm_arch4)))
    {
      return 596;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6502;

 L5098: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (general_operand (x2, HImode))
    {
      operands[1] = x2;
      goto L5099;
    }
  x2 = XEXP (x1, 0);
  goto L6503;

 L5099: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM
   && arm_arch4
   && (GET_CODE (operands[1]) != CONST_INT
       || const_ok_for_arm (INTVAL (operands[1]))
       || const_ok_for_arm (~INTVAL (operands[1]))))))
    {
      return 605;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
  goto L6503;

 L5106: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (general_operand (x2, HImode))
    {
      operands[1] = x2;
      goto L5107;
    }
 L5132: ATTRIBUTE_UNUSED_LABEL
  if (arm_rhs_operand (x2, HImode))
    {
      operands[1] = x2;
      goto L5133;
    }
  goto ret0;

 L5107: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM
   && !arm_arch4
   && !BYTES_BIG_ENDIAN
   && !TARGET_MMU_TRAPS
   && (GET_CODE (operands[1]) != CONST_INT
       || const_ok_for_arm (INTVAL (operands[1]))
       || const_ok_for_arm (~INTVAL (operands[1]))))))
    {
      return 606;
    }
 L5115: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM
   && !arm_arch4
   && BYTES_BIG_ENDIAN
   && !TARGET_MMU_TRAPS
   && (GET_CODE (operands[1]) != CONST_INT
       || const_ok_for_arm (INTVAL (operands[1]))
       || const_ok_for_arm (~INTVAL (operands[1]))))))
    {
      return 607;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 1);
  goto L5132;

 L5133: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && TARGET_MMU_TRAPS)))
    {
      return 609;
    }
  goto ret0;

 L6504: ATTRIBUTE_UNUSED_LABEL
  if (nonimmediate_operand (x2, QImode))
    {
      operands[0] = x2;
      goto L5140;
    }
  goto ret0;

 L5140: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (general_operand (x2, QImode))
    {
      operands[1] = x2;
      goto L5141;
    }
  goto ret0;

 L5141: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM
   && (   register_operand (operands[0], QImode)
       || register_operand (operands[1], QImode)))))
    {
      return 610;
    }
  goto ret0;
 ret0:
  return -1;
}

static int recog_16 PARAMS ((rtx, rtx, int *));
static int
recog_16 (x0, insn, pnum_clobbers)
     rtx x0 ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  rtx * const operands ATTRIBUTE_UNUSED = &recog_data.operand[0];
  rtx x1 ATTRIBUTE_UNUSED;
  rtx x2 ATTRIBUTE_UNUSED;
  rtx x3 ATTRIBUTE_UNUSED;
  rtx x4 ATTRIBUTE_UNUSED;
  rtx x5 ATTRIBUTE_UNUSED;
  rtx x6 ATTRIBUTE_UNUSED;
  rtx x7 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;

  x1 = XEXP (x0, 1);
  x2 = XVECEXP (x1, 0, 0);
  x3 = XEXP (x2, 0);
  switch (GET_MODE (x3))
    {
    case QImode:
      goto L6560;
    case SImode:
      goto L6562;
    case HImode:
      goto L6564;
    default:
      break;
    }
  goto ret0;

 L6560: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == MEM)
    goto L5730;
  if (s_register_operand (x3, QImode))
    {
      operands[5] = x3;
      goto L5770;
    }
  goto ret0;

 L5730: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode)
    goto L6565;
  goto ret0;

 L6565: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x4))
    {
    case PLUS:
      goto L5731;
    case MINUS:
      goto L5751;
    default:
     break;
   }
  goto ret0;

 L5731: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (shift_operator (x5, SImode))
    {
      operands[2] = x5;
      goto L5732;
    }
  goto ret0;

 L5732: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 0);
  if (s_register_operand (x6, SImode))
    {
      operands[3] = x6;
      goto L5733;
    }
  goto ret0;

 L5733: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 1);
  if (const_shift_operand (x6, SImode))
    {
      operands[4] = x6;
      goto L5734;
    }
  goto ret0;

 L5734: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (s_register_operand (x5, SImode))
    {
      operands[1] = x5;
      goto L5735;
    }
  goto ret0;

 L5735: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, QImode))
    {
      operands[5] = x3;
      goto L5736;
    }
  goto ret0;

 L5736: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5737;
  goto ret0;

 L5737: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L5738;
    }
  goto ret0;

 L5738: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L5739;
  goto ret0;

 L5739: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[2]))
    {
      operands[2] = x4;
      goto L5740;
    }
  goto ret0;

 L5740: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[3]))
    goto L5741;
  goto ret0;

 L5741: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (rtx_equal_p (x5, operands[4]))
    goto L5742;
  goto ret0;

 L5742: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[1])
      && ((TARGET_ARM) && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && REGNO (operands[3]) != FRAME_POINTER_REGNUM)))
    {
      return 646;
    }
  goto ret0;

 L5751: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[1] = x5;
      goto L5752;
    }
  goto ret0;

 L5752: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (shift_operator (x5, SImode))
    {
      operands[2] = x5;
      goto L5753;
    }
  goto ret0;

 L5753: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 0);
  if (s_register_operand (x6, SImode))
    {
      operands[3] = x6;
      goto L5754;
    }
  goto ret0;

 L5754: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 1);
  if (const_shift_operand (x6, SImode))
    {
      operands[4] = x6;
      goto L5755;
    }
  goto ret0;

 L5755: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, QImode))
    {
      operands[5] = x3;
      goto L5756;
    }
  goto ret0;

 L5756: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5757;
  goto ret0;

 L5757: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L5758;
    }
  goto ret0;

 L5758: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MINUS)
    goto L5759;
  goto ret0;

 L5759: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[1]))
    goto L5760;
  goto ret0;

 L5760: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[2]))
    {
      operands[2] = x4;
      goto L5761;
    }
  goto ret0;

 L5761: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[3]))
    goto L5762;
  goto ret0;

 L5762: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (rtx_equal_p (x5, operands[4])
      && ((TARGET_ARM) && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && REGNO (operands[3]) != FRAME_POINTER_REGNUM)))
    {
      return 647;
    }
  goto ret0;

 L5770: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == QImode
      && GET_CODE (x3) == MEM)
    goto L5771;
  goto ret0;

 L5771: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode)
    goto L6567;
  goto ret0;

 L6567: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x4))
    {
    case PLUS:
      goto L5772;
    case MINUS:
      goto L5792;
    default:
     break;
   }
  goto ret0;

 L5772: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (shift_operator (x5, SImode))
    {
      operands[2] = x5;
      goto L5773;
    }
  goto ret0;

 L5773: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 0);
  if (s_register_operand (x6, SImode))
    {
      operands[3] = x6;
      goto L5774;
    }
  goto ret0;

 L5774: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 1);
  if (const_shift_operand (x6, SImode))
    {
      operands[4] = x6;
      goto L5775;
    }
  goto ret0;

 L5775: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (s_register_operand (x5, SImode))
    {
      operands[1] = x5;
      goto L5776;
    }
  goto ret0;

 L5776: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5777;
  goto ret0;

 L5777: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L5778;
    }
  goto ret0;

 L5778: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L5779;
  goto ret0;

 L5779: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[2]))
    {
      operands[2] = x4;
      goto L5780;
    }
  goto ret0;

 L5780: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[3]))
    goto L5781;
  goto ret0;

 L5781: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (rtx_equal_p (x5, operands[4]))
    goto L5782;
  goto ret0;

 L5782: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[1])
      && ((TARGET_ARM) && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && REGNO (operands[3]) != FRAME_POINTER_REGNUM)))
    {
      return 648;
    }
  goto ret0;

 L5792: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[1] = x5;
      goto L5793;
    }
  goto ret0;

 L5793: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (shift_operator (x5, SImode))
    {
      operands[2] = x5;
      goto L5794;
    }
  goto ret0;

 L5794: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 0);
  if (s_register_operand (x6, SImode))
    {
      operands[3] = x6;
      goto L5795;
    }
  goto ret0;

 L5795: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 1);
  if (const_shift_operand (x6, SImode))
    {
      operands[4] = x6;
      goto L5796;
    }
  goto ret0;

 L5796: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5797;
  goto ret0;

 L5797: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L5798;
    }
  goto ret0;

 L5798: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MINUS)
    goto L5799;
  goto ret0;

 L5799: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[1]))
    goto L5800;
  goto ret0;

 L5800: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[2]))
    {
      operands[2] = x4;
      goto L5801;
    }
  goto ret0;

 L5801: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[3]))
    goto L5802;
  goto ret0;

 L5802: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (rtx_equal_p (x5, operands[4])
      && ((TARGET_ARM) && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && REGNO (operands[3]) != FRAME_POINTER_REGNUM)))
    {
      return 649;
    }
  goto ret0;

 L6562: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == MEM)
    goto L5810;
  if (s_register_operand (x3, SImode))
    {
      operands[5] = x3;
      goto L5850;
    }
  goto ret0;

 L5810: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode)
    goto L6569;
  goto ret0;

 L6569: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x4))
    {
    case PLUS:
      goto L5811;
    case MINUS:
      goto L5831;
    default:
     break;
   }
  goto ret0;

 L5811: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (shift_operator (x5, SImode))
    {
      operands[2] = x5;
      goto L5812;
    }
  goto ret0;

 L5812: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 0);
  if (s_register_operand (x6, SImode))
    {
      operands[3] = x6;
      goto L5813;
    }
  goto ret0;

 L5813: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 1);
  if (const_shift_operand (x6, SImode))
    {
      operands[4] = x6;
      goto L5814;
    }
  goto ret0;

 L5814: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (s_register_operand (x5, SImode))
    {
      operands[1] = x5;
      goto L5815;
    }
  goto ret0;

 L5815: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      operands[5] = x3;
      goto L5816;
    }
  goto ret0;

 L5816: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5817;
  goto ret0;

 L5817: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L5818;
    }
  goto ret0;

 L5818: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L5819;
  goto ret0;

 L5819: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[2]))
    {
      operands[2] = x4;
      goto L5820;
    }
  goto ret0;

 L5820: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[3]))
    goto L5821;
  goto ret0;

 L5821: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (rtx_equal_p (x5, operands[4]))
    goto L5822;
  goto ret0;

 L5822: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[1])
      && ((TARGET_ARM) && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && REGNO (operands[3]) != FRAME_POINTER_REGNUM)))
    {
      return 650;
    }
  goto ret0;

 L5831: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[1] = x5;
      goto L5832;
    }
  goto ret0;

 L5832: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (shift_operator (x5, SImode))
    {
      operands[2] = x5;
      goto L5833;
    }
  goto ret0;

 L5833: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 0);
  if (s_register_operand (x6, SImode))
    {
      operands[3] = x6;
      goto L5834;
    }
  goto ret0;

 L5834: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 1);
  if (const_shift_operand (x6, SImode))
    {
      operands[4] = x6;
      goto L5835;
    }
  goto ret0;

 L5835: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      operands[5] = x3;
      goto L5836;
    }
  goto ret0;

 L5836: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5837;
  goto ret0;

 L5837: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L5838;
    }
  goto ret0;

 L5838: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MINUS)
    goto L5839;
  goto ret0;

 L5839: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[1]))
    goto L5840;
  goto ret0;

 L5840: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[2]))
    {
      operands[2] = x4;
      goto L5841;
    }
  goto ret0;

 L5841: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[3]))
    goto L5842;
  goto ret0;

 L5842: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (rtx_equal_p (x5, operands[4])
      && ((TARGET_ARM) && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && REGNO (operands[3]) != FRAME_POINTER_REGNUM)))
    {
      return 651;
    }
  goto ret0;

 L5850: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5851;
  goto ret0;

 L5851: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode)
    goto L6571;
  goto ret0;

 L6571: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x4))
    {
    case PLUS:
      goto L5852;
    case MINUS:
      goto L5872;
    default:
     break;
   }
  goto ret0;

 L5852: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (shift_operator (x5, SImode))
    {
      operands[2] = x5;
      goto L5853;
    }
  goto ret0;

 L5853: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 0);
  if (s_register_operand (x6, SImode))
    {
      operands[3] = x6;
      goto L5854;
    }
  goto ret0;

 L5854: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 1);
  if (const_shift_operand (x6, SImode))
    {
      operands[4] = x6;
      goto L5855;
    }
  goto ret0;

 L5855: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (s_register_operand (x5, SImode))
    {
      operands[1] = x5;
      goto L5856;
    }
  goto ret0;

 L5856: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5857;
  goto ret0;

 L5857: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L5858;
    }
  goto ret0;

 L5858: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L5859;
  goto ret0;

 L5859: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[2]))
    {
      operands[2] = x4;
      goto L5860;
    }
  goto ret0;

 L5860: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[3]))
    goto L5861;
  goto ret0;

 L5861: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (rtx_equal_p (x5, operands[4]))
    goto L5862;
  goto ret0;

 L5862: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[1])
      && ((TARGET_ARM) && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && REGNO (operands[3]) != FRAME_POINTER_REGNUM)))
    {
      return 652;
    }
  goto ret0;

 L5872: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[1] = x5;
      goto L5873;
    }
  goto ret0;

 L5873: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (shift_operator (x5, SImode))
    {
      operands[2] = x5;
      goto L5874;
    }
  goto ret0;

 L5874: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 0);
  if (s_register_operand (x6, SImode))
    {
      operands[3] = x6;
      goto L5875;
    }
  goto ret0;

 L5875: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 1);
  if (const_shift_operand (x6, SImode))
    {
      operands[4] = x6;
      goto L5876;
    }
  goto ret0;

 L5876: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5877;
  goto ret0;

 L5877: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L5878;
    }
  goto ret0;

 L5878: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MINUS)
    goto L5879;
  goto ret0;

 L5879: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[1]))
    goto L5880;
  goto ret0;

 L5880: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[2]))
    {
      operands[2] = x4;
      goto L5881;
    }
  goto ret0;

 L5881: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[3]))
    goto L5882;
  goto ret0;

 L5882: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (rtx_equal_p (x5, operands[4])
      && ((TARGET_ARM) && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && REGNO (operands[3]) != FRAME_POINTER_REGNUM)))
    {
      return 653;
    }
  goto ret0;

 L6564: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x3, HImode))
    {
      operands[5] = x3;
      goto L5890;
    }
  goto ret0;

 L5890: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == HImode
      && GET_CODE (x3) == MEM)
    goto L5891;
  goto ret0;

 L5891: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode)
    goto L6573;
  goto ret0;

 L6573: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x4))
    {
    case PLUS:
      goto L5892;
    case MINUS:
      goto L5912;
    default:
     break;
   }
  goto ret0;

 L5892: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (shift_operator (x5, SImode))
    {
      operands[2] = x5;
      goto L5893;
    }
  goto ret0;

 L5893: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 0);
  if (s_register_operand (x6, SImode))
    {
      operands[3] = x6;
      goto L5894;
    }
  goto ret0;

 L5894: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 1);
  if (const_shift_operand (x6, SImode))
    {
      operands[4] = x6;
      goto L5895;
    }
  goto ret0;

 L5895: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (s_register_operand (x5, SImode))
    {
      operands[1] = x5;
      goto L5896;
    }
  goto ret0;

 L5896: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5897;
  goto ret0;

 L5897: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L5898;
    }
  goto ret0;

 L5898: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L5899;
  goto ret0;

 L5899: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[2]))
    {
      operands[2] = x4;
      goto L5900;
    }
  goto ret0;

 L5900: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[3]))
    goto L5901;
  goto ret0;

 L5901: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (rtx_equal_p (x5, operands[4]))
    goto L5902;
  goto ret0;

 L5902: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[1])
      && ((TARGET_ARM) && (TARGET_ARM
   && !BYTES_BIG_ENDIAN
   && !TARGET_MMU_TRAPS
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && REGNO (operands[3]) != FRAME_POINTER_REGNUM)))
    {
      return 654;
    }
  goto ret0;

 L5912: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[1] = x5;
      goto L5913;
    }
  goto ret0;

 L5913: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (shift_operator (x5, SImode))
    {
      operands[2] = x5;
      goto L5914;
    }
  goto ret0;

 L5914: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 0);
  if (s_register_operand (x6, SImode))
    {
      operands[3] = x6;
      goto L5915;
    }
  goto ret0;

 L5915: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 1);
  if (const_shift_operand (x6, SImode))
    {
      operands[4] = x6;
      goto L5916;
    }
  goto ret0;

 L5916: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5917;
  goto ret0;

 L5917: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L5918;
    }
  goto ret0;

 L5918: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MINUS)
    goto L5919;
  goto ret0;

 L5919: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[1]))
    goto L5920;
  goto ret0;

 L5920: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[2]))
    {
      operands[2] = x4;
      goto L5921;
    }
  goto ret0;

 L5921: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[3]))
    goto L5922;
  goto ret0;

 L5922: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (rtx_equal_p (x5, operands[4])
      && ((TARGET_ARM) && (TARGET_ARM
   && !BYTES_BIG_ENDIAN
   && !TARGET_MMU_TRAPS
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && REGNO (operands[3]) != FRAME_POINTER_REGNUM)))
    {
      return 655;
    }
  goto ret0;
 ret0:
  return -1;
}

static int recog_17 PARAMS ((rtx, rtx, int *));
static int
recog_17 (x0, insn, pnum_clobbers)
     rtx x0 ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  rtx * const operands ATTRIBUTE_UNUSED = &recog_data.operand[0];
  rtx x1 ATTRIBUTE_UNUSED;
  rtx x2 ATTRIBUTE_UNUSED;
  rtx x3 ATTRIBUTE_UNUSED;
  rtx x4 ATTRIBUTE_UNUSED;
  rtx x5 ATTRIBUTE_UNUSED;
  rtx x6 ATTRIBUTE_UNUSED;
  rtx x7 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;

  x1 = XEXP (x0, 1);
  switch (GET_CODE (x1))
    {
    case PARALLEL:
      goto L6559;
    case SET:
      goto L5497;
    default:
     break;
   }
  goto ret0;

 L6559: ATTRIBUTE_UNUSED_LABEL
  if (XVECLEN (x1, 0) == 2)
    goto L5728;
  if (XVECLEN (x1, 0) >= 4
      && load_multiple_operation (x1, VOIDmode))
    {
      operands[0] = x1;
      goto L5211;
    }
 L6558: ATTRIBUTE_UNUSED_LABEL
  if (XVECLEN (x1, 0) >= 4
      && store_multiple_operation (x1, VOIDmode))
    {
      operands[0] = x1;
      goto L5353;
    }
  goto ret0;

 L5728: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 0);
  if (GET_CODE (x2) == SET)
    goto L5729;
  goto ret0;

 L5729: ATTRIBUTE_UNUSED_LABEL
  return recog_16 (x0, insn, pnum_clobbers);

 L5211: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 0);
  if (GET_CODE (x2) == SET)
    goto L5212;
  goto L6558;

 L5212: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode)
    goto L6576;
  goto L6558;

 L6576: ATTRIBUTE_UNUSED_LABEL
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L5259;
    }
 L6575: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L5213;
    }
  goto L6558;

 L5259: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5260;
  x3 = XEXP (x2, 0);
  goto L6575;

 L5260: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L5261;
    }
  x3 = XEXP (x2, 0);
  goto L6575;

 L5261: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5262;
  x2 = XVECEXP (x1, 0, 0);
  x3 = XEXP (x2, 0);
  goto L6575;

 L5262: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L5263;
    }
  x2 = XVECEXP (x1, 0, 0);
  x3 = XEXP (x2, 0);
  goto L6575;

 L5263: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5264;
  x2 = XVECEXP (x1, 0, 0);
  x3 = XEXP (x2, 0);
  goto L6575;

 L5264: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode
      && GET_CODE (x4) == PLUS)
    goto L5265;
  x2 = XVECEXP (x1, 0, 0);
  x3 = XEXP (x2, 0);
  goto L6575;

 L5265: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[1]))
    goto L5266;
  x2 = XVECEXP (x1, 0, 0);
  x3 = XEXP (x2, 0);
  goto L6575;

 L5266: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT
      && XWINT (x5, 0) == 4)
    goto L5267;
  x2 = XVECEXP (x1, 0, 0);
  x3 = XEXP (x2, 0);
  goto L6575;

 L5267: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 2);
  if (GET_CODE (x2) == SET)
    goto L5268;
  x2 = XVECEXP (x1, 0, 0);
  x3 = XEXP (x2, 0);
  goto L6575;

 L5268: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[4] = x3;
      goto L5269;
    }
  x2 = XVECEXP (x1, 0, 0);
  x3 = XEXP (x2, 0);
  goto L6575;

 L5269: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5270;
  x2 = XVECEXP (x1, 0, 0);
  x3 = XEXP (x2, 0);
  goto L6575;

 L5270: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode
      && GET_CODE (x4) == PLUS)
    goto L5271;
  x2 = XVECEXP (x1, 0, 0);
  x3 = XEXP (x2, 0);
  goto L6575;

 L5271: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[1]))
    goto L5272;
  x2 = XVECEXP (x1, 0, 0);
  x3 = XEXP (x2, 0);
  goto L6575;

 L5272: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT
      && XWINT (x5, 0) == 8)
    goto L5273;
  x2 = XVECEXP (x1, 0, 0);
  x3 = XEXP (x2, 0);
  goto L6575;

 L5273: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 3);
  if (GET_CODE (x2) == SET)
    goto L5274;
  x2 = XVECEXP (x1, 0, 0);
  x3 = XEXP (x2, 0);
  goto L6575;

 L5274: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[5] = x3;
      goto L5275;
    }
  x2 = XVECEXP (x1, 0, 0);
  x3 = XEXP (x2, 0);
  goto L6575;

 L5275: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5276;
  x2 = XVECEXP (x1, 0, 0);
  x3 = XEXP (x2, 0);
  goto L6575;

 L5276: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode
      && GET_CODE (x4) == PLUS)
    goto L5277;
  x2 = XVECEXP (x1, 0, 0);
  x3 = XEXP (x2, 0);
  goto L6575;

 L5277: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[1]))
    goto L5278;
  x2 = XVECEXP (x1, 0, 0);
  x3 = XEXP (x2, 0);
  goto L6575;

 L5278: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT
      && XWINT (x5, 0) == 12
      && ((TARGET_ARM) && (TARGET_ARM && XVECLEN (operands[0], 0) == 4)))
    {
      return 618;
    }
  x2 = XVECEXP (x1, 0, 0);
  x3 = XEXP (x2, 0);
  goto L6575;

 L5213: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L5214;
  goto L6558;

 L5214: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L5215;
    }
  goto L6558;

 L5215: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 12)
    goto L5216;
  goto L6558;

 L5216: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5217;
  goto L6558;

 L5217: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L5218;
    }
  goto L6558;

 L5218: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5219;
  goto L6558;

 L5219: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[2]))
    goto L5220;
  goto L6558;

 L5220: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 2);
  if (GET_CODE (x2) == SET)
    goto L5221;
  goto L6558;

 L5221: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[4] = x3;
      goto L5222;
    }
  goto L6558;

 L5222: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5223;
  goto L6558;

 L5223: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode
      && GET_CODE (x4) == PLUS)
    goto L5224;
  goto L6558;

 L5224: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[2]))
    goto L5225;
  goto L6558;

 L5225: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT
      && XWINT (x5, 0) == 4)
    goto L5226;
  goto L6558;

 L5226: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 3);
  if (GET_CODE (x2) == SET)
    goto L5227;
  goto L6558;

 L5227: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[5] = x3;
      goto L5228;
    }
  goto L6558;

 L5228: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5229;
  goto L6558;

 L5229: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode
      && GET_CODE (x4) == PLUS)
    goto L5230;
  goto L6558;

 L5230: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[2]))
    goto L5231;
  goto L6558;

 L5231: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT
      && XWINT (x5, 0) == 8
      && ((TARGET_ARM) && (TARGET_ARM && XVECLEN (operands[0], 0) == 4)))
    {
      return 616;
    }
  goto L6558;

 L5353: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 0);
  if (GET_CODE (x2) == SET)
    goto L5354;
  goto ret0;

 L5354: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode)
    goto L6578;
  goto ret0;

 L6578: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == MEM)
    goto L5403;
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L5355;
    }
  goto ret0;

 L5403: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L5404;
    }
  goto ret0;

 L5404: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L5405;
    }
  goto ret0;

 L5405: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5406;
  goto ret0;

 L5406: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5407;
  goto ret0;

 L5407: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode
      && GET_CODE (x4) == PLUS)
    goto L5408;
  goto ret0;

 L5408: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[1]))
    goto L5409;
  goto ret0;

 L5409: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT
      && XWINT (x5, 0) == 4)
    goto L5410;
  goto ret0;

 L5410: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L5411;
    }
  goto ret0;

 L5411: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 2);
  if (GET_CODE (x2) == SET)
    goto L5412;
  goto ret0;

 L5412: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5413;
  goto ret0;

 L5413: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode
      && GET_CODE (x4) == PLUS)
    goto L5414;
  goto ret0;

 L5414: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[1]))
    goto L5415;
  goto ret0;

 L5415: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT
      && XWINT (x5, 0) == 8)
    goto L5416;
  goto ret0;

 L5416: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[4] = x3;
      goto L5417;
    }
  goto ret0;

 L5417: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 3);
  if (GET_CODE (x2) == SET)
    goto L5418;
  goto ret0;

 L5418: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5419;
  goto ret0;

 L5419: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode
      && GET_CODE (x4) == PLUS)
    goto L5420;
  goto ret0;

 L5420: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[1]))
    goto L5421;
  goto ret0;

 L5421: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT
      && XWINT (x5, 0) == 12)
    goto L5422;
  goto ret0;

 L5422: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[5] = x3;
      goto L5423;
    }
  goto ret0;

 L5423: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && XVECLEN (operands[0], 0) == 4)))
    {
      return 624;
    }
  goto ret0;

 L5355: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L5356;
  goto ret0;

 L5356: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L5357;
    }
  goto ret0;

 L5357: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 12)
    goto L5358;
  goto ret0;

 L5358: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5359;
  goto ret0;

 L5359: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5360;
  goto ret0;

 L5360: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[2]))
    goto L5361;
  goto ret0;

 L5361: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L5362;
    }
  goto ret0;

 L5362: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 2);
  if (GET_CODE (x2) == SET)
    goto L5363;
  goto ret0;

 L5363: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5364;
  goto ret0;

 L5364: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode
      && GET_CODE (x4) == PLUS)
    goto L5365;
  goto ret0;

 L5365: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[2]))
    goto L5366;
  goto ret0;

 L5366: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT
      && XWINT (x5, 0) == 4)
    goto L5367;
  goto ret0;

 L5367: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[4] = x3;
      goto L5368;
    }
  goto ret0;

 L5368: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 3);
  if (GET_CODE (x2) == SET)
    goto L5369;
  goto ret0;

 L5369: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5370;
  goto ret0;

 L5370: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode
      && GET_CODE (x4) == PLUS)
    goto L5371;
  goto ret0;

 L5371: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[2]))
    goto L5372;
  goto ret0;

 L5372: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT
      && XWINT (x5, 0) == 8)
    goto L5373;
  goto ret0;

 L5373: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[5] = x3;
      goto L5374;
    }
  goto ret0;

 L5374: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && XVECLEN (operands[0], 0) == 4)))
    {
      return 622;
    }
  goto ret0;

 L5497: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L5498;
    }
  goto ret0;

 L5498: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (shiftable_operator (x2, SImode))
    {
      operands[1] = x2;
      goto L5499;
    }
  goto ret0;

 L5499: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (shift_operator (x3, SImode))
    {
      operands[3] = x3;
      goto L5500;
    }
  goto ret0;

 L5500: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[4] = x4;
      goto L5501;
    }
  goto ret0;

 L5501: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (reg_or_int_operand (x4, SImode))
    {
      operands[5] = x4;
      goto L5502;
    }
  goto ret0;

 L5502: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L5503;
    }
  goto ret0;

 L5503: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 631;
    }
  goto ret0;
 ret0:
  return -1;
}

static int recog_18 PARAMS ((rtx, rtx, int *));
static int
recog_18 (x0, insn, pnum_clobbers)
     rtx x0 ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  rtx * const operands ATTRIBUTE_UNUSED = &recog_data.operand[0];
  rtx x1 ATTRIBUTE_UNUSED;
  rtx x2 ATTRIBUTE_UNUSED;
  rtx x3 ATTRIBUTE_UNUSED;
  rtx x4 ATTRIBUTE_UNUSED;
  rtx x5 ATTRIBUTE_UNUSED;
  rtx x6 ATTRIBUTE_UNUSED;
  rtx x7 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;

  x1 = XEXP (x0, 1);
  x2 = XVECEXP (x1, 0, 0);
  x3 = XEXP (x2, 0);
  switch (GET_MODE (x3))
    {
    case QImode:
      goto L6587;
    case SImode:
      goto L6590;
    case HImode:
      goto L6591;
    default:
      break;
    }
  goto ret0;

 L6587: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == MEM)
    goto L5536;
  if (s_register_operand (x3, QImode))
    {
      operands[3] = x3;
      goto L5568;
    }
  goto ret0;

 L5536: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode)
    goto L6592;
  goto ret0;

 L6592: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x4))
    {
    case PLUS:
      goto L5537;
    case MINUS:
      goto L5553;
    default:
     break;
   }
  goto ret0;

 L5537: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[1] = x5;
      goto L5538;
    }
  goto ret0;

 L5538: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (index_operand (x5, SImode))
    {
      operands[2] = x5;
      goto L5539;
    }
  goto ret0;

 L5539: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, QImode))
    {
      operands[3] = x3;
      goto L5540;
    }
  goto ret0;

 L5540: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5541;
  goto ret0;

 L5541: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L5542;
    }
  goto ret0;

 L5542: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L5543;
  goto ret0;

 L5543: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[1]))
    goto L5544;
  goto ret0;

 L5544: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[2])
      && ((TARGET_ARM) && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM))))
    {
      return 634;
    }
  goto ret0;

 L5553: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[1] = x5;
      goto L5554;
    }
  goto ret0;

 L5554: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (s_register_operand (x5, SImode))
    {
      operands[2] = x5;
      goto L5555;
    }
  goto ret0;

 L5555: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, QImode))
    {
      operands[3] = x3;
      goto L5556;
    }
  goto ret0;

 L5556: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5557;
  goto ret0;

 L5557: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L5558;
    }
  goto ret0;

 L5558: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MINUS)
    goto L5559;
  goto ret0;

 L5559: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[1]))
    goto L5560;
  goto ret0;

 L5560: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[2])
      && ((TARGET_ARM) && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM))))
    {
      return 635;
    }
  goto ret0;

 L5568: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == QImode
      && GET_CODE (x3) == MEM)
    goto L5569;
  goto ret0;

 L5569: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode)
    goto L6594;
  goto ret0;

 L6594: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x4))
    {
    case PLUS:
      goto L5570;
    case MINUS:
      goto L5586;
    default:
     break;
   }
  goto ret0;

 L5570: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[1] = x5;
      goto L5571;
    }
  goto ret0;

 L5571: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (index_operand (x5, SImode))
    {
      operands[2] = x5;
      goto L5572;
    }
  goto ret0;

 L5572: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5573;
  goto ret0;

 L5573: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L5574;
    }
  goto ret0;

 L5574: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L5575;
  goto ret0;

 L5575: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[1]))
    goto L5576;
  goto ret0;

 L5576: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[2])
      && ((TARGET_ARM) && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM))))
    {
      return 636;
    }
  goto ret0;

 L5586: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[1] = x5;
      goto L5587;
    }
  goto ret0;

 L5587: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (s_register_operand (x5, SImode))
    {
      operands[2] = x5;
      goto L5588;
    }
  goto ret0;

 L5588: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5589;
  goto ret0;

 L5589: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L5590;
    }
  goto ret0;

 L5590: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MINUS)
    goto L5591;
  goto ret0;

 L5591: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[1]))
    goto L5592;
  goto ret0;

 L5592: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[2])
      && ((TARGET_ARM) && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM))))
    {
      return 637;
    }
  goto ret0;

 L6590: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == MEM)
    goto L5634;
  if (s_register_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L5600;
    }
  goto ret0;

 L5634: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode)
    goto L6596;
  goto ret0;

 L6596: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x4))
    {
    case PLUS:
      goto L5635;
    case MINUS:
      goto L5651;
    default:
     break;
   }
  goto ret0;

 L5635: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[1] = x5;
      goto L5636;
    }
  goto ret0;

 L5636: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (index_operand (x5, SImode))
    {
      operands[2] = x5;
      goto L5637;
    }
  goto ret0;

 L5637: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L5638;
    }
  goto ret0;

 L5638: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5639;
  goto ret0;

 L5639: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L5640;
    }
  goto ret0;

 L5640: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L5641;
  goto ret0;

 L5641: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[1]))
    goto L5642;
  goto ret0;

 L5642: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[2])
      && ((TARGET_ARM) && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM))))
    {
      return 640;
    }
  goto ret0;

 L5651: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[1] = x5;
      goto L5652;
    }
  goto ret0;

 L5652: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (s_register_operand (x5, SImode))
    {
      operands[2] = x5;
      goto L5653;
    }
  goto ret0;

 L5653: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L5654;
    }
  goto ret0;

 L5654: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5655;
  goto ret0;

 L5655: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L5656;
    }
  goto ret0;

 L5656: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MINUS)
    goto L5657;
  goto ret0;

 L5657: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[1]))
    goto L5658;
  goto ret0;

 L5658: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[2])
      && ((TARGET_ARM) && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM))))
    {
      return 641;
    }
  goto ret0;

 L5600: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode)
    goto L6598;
  goto ret0;

 L6598: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x3))
    {
    case ZERO_EXTEND:
      goto L5601;
    case MEM:
      goto L5667;
    default:
     break;
   }
  goto ret0;

 L5601: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == QImode
      && GET_CODE (x4) == MEM)
    goto L5602;
  goto ret0;

 L5602: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (GET_MODE (x5) == SImode)
    goto L6600;
  goto ret0;

 L6600: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x5))
    {
    case PLUS:
      goto L5603;
    case MINUS:
      goto L5620;
    default:
     break;
   }
  goto ret0;

 L5603: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 0);
  if (s_register_operand (x6, SImode))
    {
      operands[1] = x6;
      goto L5604;
    }
  goto ret0;

 L5604: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 1);
  if (index_operand (x6, SImode))
    {
      operands[2] = x6;
      goto L5605;
    }
  goto ret0;

 L5605: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5606;
  goto ret0;

 L5606: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L5607;
    }
  goto ret0;

 L5607: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L5608;
  goto ret0;

 L5608: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[1]))
    goto L5609;
  goto ret0;

 L5609: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[2])
      && ((TARGET_ARM) && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM))))
    {
      return 638;
    }
  goto ret0;

 L5620: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 0);
  if (s_register_operand (x6, SImode))
    {
      operands[1] = x6;
      goto L5621;
    }
  goto ret0;

 L5621: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 1);
  if (s_register_operand (x6, SImode))
    {
      operands[2] = x6;
      goto L5622;
    }
  goto ret0;

 L5622: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5623;
  goto ret0;

 L5623: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L5624;
    }
  goto ret0;

 L5624: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MINUS)
    goto L5625;
  goto ret0;

 L5625: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[1]))
    goto L5626;
  goto ret0;

 L5626: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[2])
      && ((TARGET_ARM) && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM))))
    {
      return 639;
    }
  goto ret0;

 L5667: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode)
    goto L6602;
  goto ret0;

 L6602: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x4))
    {
    case PLUS:
      goto L5668;
    case MINUS:
      goto L5684;
    default:
     break;
   }
  goto ret0;

 L5668: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[1] = x5;
      goto L5669;
    }
  goto ret0;

 L5669: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (index_operand (x5, SImode))
    {
      operands[2] = x5;
      goto L5670;
    }
  goto ret0;

 L5670: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5671;
  goto ret0;

 L5671: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L5672;
    }
  goto ret0;

 L5672: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L5673;
  goto ret0;

 L5673: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[1]))
    goto L5674;
  goto ret0;

 L5674: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[2])
      && ((TARGET_ARM) && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM))))
    {
      return 642;
    }
  goto ret0;

 L5684: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[1] = x5;
      goto L5685;
    }
  goto ret0;

 L5685: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (s_register_operand (x5, SImode))
    {
      operands[2] = x5;
      goto L5686;
    }
  goto ret0;

 L5686: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5687;
  goto ret0;

 L5687: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L5688;
    }
  goto ret0;

 L5688: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MINUS)
    goto L5689;
  goto ret0;

 L5689: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[1]))
    goto L5690;
  goto ret0;

 L5690: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[2])
      && ((TARGET_ARM) && (TARGET_ARM
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM))))
    {
      return 643;
    }
  goto ret0;

 L6591: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x3, HImode))
    {
      operands[3] = x3;
      goto L5698;
    }
  goto ret0;

 L5698: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == HImode
      && GET_CODE (x3) == MEM)
    goto L5699;
  goto ret0;

 L5699: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode)
    goto L6604;
  goto ret0;

 L6604: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x4))
    {
    case PLUS:
      goto L5700;
    case MINUS:
      goto L5716;
    default:
     break;
   }
  goto ret0;

 L5700: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[1] = x5;
      goto L5701;
    }
  goto ret0;

 L5701: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (index_operand (x5, SImode))
    {
      operands[2] = x5;
      goto L5702;
    }
  goto ret0;

 L5702: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5703;
  goto ret0;

 L5703: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L5704;
    }
  goto ret0;

 L5704: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L5705;
  goto ret0;

 L5705: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[1]))
    goto L5706;
  goto ret0;

 L5706: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[2])
      && ((TARGET_ARM) && (TARGET_ARM
   && !BYTES_BIG_ENDIAN
   && !TARGET_MMU_TRAPS
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM))))
    {
      return 644;
    }
  goto ret0;

 L5716: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[1] = x5;
      goto L5717;
    }
  goto ret0;

 L5717: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (s_register_operand (x5, SImode))
    {
      operands[2] = x5;
      goto L5718;
    }
  goto ret0;

 L5718: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5719;
  goto ret0;

 L5719: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L5720;
    }
  goto ret0;

 L5720: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MINUS)
    goto L5721;
  goto ret0;

 L5721: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[1]))
    goto L5722;
  goto ret0;

 L5722: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, operands[2])
      && ((TARGET_ARM) && (TARGET_ARM
   && !BYTES_BIG_ENDIAN
   && !TARGET_MMU_TRAPS
   && REGNO (operands[0]) != FRAME_POINTER_REGNUM
   && REGNO (operands[1]) != FRAME_POINTER_REGNUM
   && (GET_CODE (operands[2]) != REG
       || REGNO (operands[2]) != FRAME_POINTER_REGNUM))))
    {
      return 645;
    }
  goto ret0;
 ret0:
  return -1;
}

static int recog_19 PARAMS ((rtx, rtx, int *));
static int
recog_19 (x0, insn, pnum_clobbers)
     rtx x0 ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  rtx * const operands ATTRIBUTE_UNUSED = &recog_data.operand[0];
  rtx x1 ATTRIBUTE_UNUSED;
  rtx x2 ATTRIBUTE_UNUSED;
  rtx x3 ATTRIBUTE_UNUSED;
  rtx x4 ATTRIBUTE_UNUSED;
  rtx x5 ATTRIBUTE_UNUSED;
  rtx x6 ATTRIBUTE_UNUSED;
  rtx x7 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;

  x1 = XEXP (x0, 0);
  if (arm_comparison_operator (x1, VOIDmode))
    {
      operands[3] = x1;
      goto L4107;
    }
 L4254: ATTRIBUTE_UNUSED_LABEL
  if (arm_comparison_operator (x1, VOIDmode))
    {
      operands[4] = x1;
      goto L4255;
    }
 L4602: ATTRIBUTE_UNUSED_LABEL
  if (arm_comparison_operator (x1, VOIDmode))
    {
      operands[5] = x1;
      goto L4603;
    }
 L4733: ATTRIBUTE_UNUSED_LABEL
  if (arm_comparison_operator (x1, VOIDmode))
    {
      operands[2] = x1;
      goto L4734;
    }
 L5175: ATTRIBUTE_UNUSED_LABEL
  if (arm_comparison_operator (x1, VOIDmode))
    {
      operands[7] = x1;
      goto L5176;
    }
 L5207: ATTRIBUTE_UNUSED_LABEL
  if (arm_comparison_operator (x1, VOIDmode))
    {
      operands[6] = x1;
      goto L5208;
    }
 L5280: ATTRIBUTE_UNUSED_LABEL
  if (arm_comparison_operator (x1, VOIDmode))
    {
      operands[5] = x1;
      goto L5281;
    }
 L5301: ATTRIBUTE_UNUSED_LABEL
  if (arm_comparison_operator (x1, VOIDmode))
    {
      operands[4] = x1;
      goto L5302;
    }
 L5463: ATTRIBUTE_UNUSED_LABEL
  if (arm_comparison_operator (x1, VOIDmode))
    {
      operands[1] = x1;
      goto L5464;
    }
 L5472: ATTRIBUTE_UNUSED_LABEL
  if (arm_comparison_operator (x1, VOIDmode))
    {
      operands[0] = x1;
      goto L5473;
    }
  goto ret0;

 L4107: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (cc_register (x2, VOIDmode))
    {
      operands[4] = x2;
      goto L4108;
    }
  goto L4254;

 L4108: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 0)
    goto L4109;
  goto L4254;

 L4109: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_CODE (x1) == SET)
    goto L4110;
  x1 = XEXP (x0, 0);
  goto L4254;

 L4110: ATTRIBUTE_UNUSED_LABEL
  tem = recog_14 (x0, insn, pnum_clobbers);
  if (tem >= 0)
    return tem;
  x1 = XEXP (x0, 0);
  goto L4254;

 L4255: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (cc_register (x2, VOIDmode))
    {
      operands[5] = x2;
      goto L4256;
    }
  goto L4602;

 L4256: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 0)
    goto L4257;
  goto L4602;

 L4257: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  switch (GET_CODE (x1))
    {
    case SET:
      goto L4258;
    case PARALLEL:
      goto L6481;
    default:
     break;
   }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4258: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  switch (GET_MODE (x2))
    {
    case SImode:
      goto L6482;
    case DImode:
      goto L6483;
    default:
      break;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L6482: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L4259;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4259: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode)
    goto L6484;
  x1 = XEXP (x0, 0);
  goto L4602;

 L6484: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x2))
    {
    case PLUS:
      goto L4260;
    case AND:
      goto L4705;
    case NOT:
      goto L4728;
    case ASHIFT:
    case ASHIFTRT:
    case LSHIFTRT:
    case ROTATERT:
    case MULT:
      goto L6486;
    default:
      x1 = XEXP (x0, 0);
      goto L4602;
   }
 L6486: ATTRIBUTE_UNUSED_LABEL
  if (shift_operator (x2, SImode))
    {
      operands[3] = x2;
      goto L4718;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4260: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MULT)
    goto L4261;
  x1 = XEXP (x0, 0);
  goto L4602;

 L4261: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L4262;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4262: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L4263;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4263: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L4264;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4264: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 524;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4705: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == IOR)
    goto L4706;
  x1 = XEXP (x0, 0);
  goto L4602;

 L4706: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L4707;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4707: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L4708;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4708: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == NOT)
    goto L4709;
  x1 = XEXP (x0, 0);
  goto L4602;

 L4709: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (arm_rhs_operand (x4, SImode))
    {
      operands[3] = x4;
      goto L4710;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4710: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 563;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4728: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (shift_operator (x3, SImode))
    {
      operands[3] = x3;
      goto L4729;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4729: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L4730;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4730: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_rhs_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L4731;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4731: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 565;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4718: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L4719;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4719: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (reg_or_int_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L4720;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4720: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 564;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L6483: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, DImode))
    {
      operands[0] = x2;
      goto L4271;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4271: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == DImode
      && GET_CODE (x2) == PLUS)
    goto L4272;
  x1 = XEXP (x0, 0);
  goto L4602;

 L4272: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == DImode
      && GET_CODE (x3) == MULT)
    goto L4273;
  x1 = XEXP (x0, 0);
  goto L4602;

 L4273: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == DImode)
    goto L6488;
  x1 = XEXP (x0, 0);
  goto L4602;

 L6488: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x4))
    {
    case SIGN_EXTEND:
      goto L4274;
    case ZERO_EXTEND:
      goto L4312;
    default:
     break;
   }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4274: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[2] = x5;
      goto L4275;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4275: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_MODE (x4) == DImode
      && GET_CODE (x4) == SIGN_EXTEND)
    goto L4276;
  x1 = XEXP (x0, 0);
  goto L4602;

 L4276: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[3] = x5;
      goto L4277;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4277: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, DImode))
    {
      operands[1] = x3;
      goto L4278;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4278: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && arm_fast_multiply)))
    {
      return 525;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4312: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[2] = x5;
      goto L4313;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4313: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_MODE (x4) == DImode
      && GET_CODE (x4) == ZERO_EXTEND)
    goto L4314;
  x1 = XEXP (x0, 0);
  goto L4602;

 L4314: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[3] = x5;
      goto L4315;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4315: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, DImode))
    {
      operands[1] = x3;
      goto L4316;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4316: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && arm_fast_multiply)))
    {
      return 528;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L6481: ATTRIBUTE_UNUSED_LABEL
  if (XVECLEN (x1, 0) == 2)
    goto L4322;
  x1 = XEXP (x0, 0);
  goto L4602;

 L4322: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 0);
  if (GET_CODE (x2) == SET)
    goto L4323;
  x1 = XEXP (x0, 0);
  goto L4602;

 L4323: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L4324;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4324: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == TRUNCATE)
    goto L4325;
  x1 = XEXP (x0, 0);
  goto L4602;

 L4325: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == DImode
      && GET_CODE (x4) == LSHIFTRT)
    goto L4326;
  x1 = XEXP (x0, 0);
  goto L4602;

 L4326: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (GET_MODE (x5) == DImode
      && GET_CODE (x5) == MULT)
    goto L4327;
  x1 = XEXP (x0, 0);
  goto L4602;

 L4327: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 0);
  if (GET_MODE (x6) == DImode)
    goto L6490;
  x1 = XEXP (x0, 0);
  goto L4602;

 L6490: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x6))
    {
    case SIGN_EXTEND:
      goto L4328;
    case ZERO_EXTEND:
      goto L4346;
    default:
     break;
   }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4328: ATTRIBUTE_UNUSED_LABEL
  x7 = XEXP (x6, 0);
  if (s_register_operand (x7, SImode))
    {
      operands[1] = x7;
      goto L4329;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4329: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 1);
  if (GET_MODE (x6) == DImode
      && GET_CODE (x6) == SIGN_EXTEND)
    goto L4330;
  x1 = XEXP (x0, 0);
  goto L4602;

 L4330: ATTRIBUTE_UNUSED_LABEL
  x7 = XEXP (x6, 0);
  if (s_register_operand (x7, SImode))
    {
      operands[2] = x7;
      goto L4331;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4331: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT
      && XWINT (x5, 0) == 32)
    goto L4332;
  x1 = XEXP (x0, 0);
  goto L4602;

 L4332: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == CLOBBER)
    goto L4333;
  x1 = XEXP (x0, 0);
  goto L4602;

 L4333: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (scratch_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L4334;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4334: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && arm_fast_multiply)))
    {
      return 529;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4346: ATTRIBUTE_UNUSED_LABEL
  x7 = XEXP (x6, 0);
  if (s_register_operand (x7, SImode))
    {
      operands[1] = x7;
      goto L4347;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4347: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 1);
  if (GET_MODE (x6) == DImode
      && GET_CODE (x6) == ZERO_EXTEND)
    goto L4348;
  x1 = XEXP (x0, 0);
  goto L4602;

 L4348: ATTRIBUTE_UNUSED_LABEL
  x7 = XEXP (x6, 0);
  if (s_register_operand (x7, SImode))
    {
      operands[2] = x7;
      goto L4349;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4349: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT
      && XWINT (x5, 0) == 32)
    goto L4350;
  x1 = XEXP (x0, 0);
  goto L4602;

 L4350: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == CLOBBER)
    goto L4351;
  x1 = XEXP (x0, 0);
  goto L4602;

 L4351: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (scratch_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L4352;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4352: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && arm_fast_multiply)))
    {
      return 530;
    }
  x1 = XEXP (x0, 0);
  goto L4602;

 L4603: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (cc_register (x2, VOIDmode))
    {
      operands[6] = x2;
      goto L4604;
    }
  goto L4733;

 L4604: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 0)
    goto L4605;
  goto L4733;

 L4605: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  switch (GET_CODE (x1))
    {
    case SET:
      goto L4606;
    case PARALLEL:
      goto L6492;
    default:
     break;
   }
  x1 = XEXP (x0, 0);
  goto L4733;

 L4606: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L4607;
    }
  x1 = XEXP (x0, 0);
  goto L4733;

 L4607: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == AND)
    goto L4608;
  x1 = XEXP (x0, 0);
  goto L4733;

 L4608: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == NOT)
    goto L4609;
  x1 = XEXP (x0, 0);
  goto L4733;

 L4609: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (shift_operator (x4, SImode))
    {
      operands[4] = x4;
      goto L4610;
    }
  x1 = XEXP (x0, 0);
  goto L4733;

 L4610: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[2] = x5;
      goto L4611;
    }
  x1 = XEXP (x0, 0);
  goto L4733;

 L4611: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (arm_rhs_operand (x5, SImode))
    {
      operands[3] = x5;
      goto L4612;
    }
  x1 = XEXP (x0, 0);
  goto L4733;

 L4612: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L4613;
    }
  x1 = XEXP (x0, 0);
  goto L4733;

 L4613: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 554;
    }
  x1 = XEXP (x0, 0);
  goto L4733;

 L6492: ATTRIBUTE_UNUSED_LABEL
  if (XVECLEN (x1, 0) >= 3
      && load_multiple_operation (x1, VOIDmode))
    {
      operands[0] = x1;
      goto L5237;
    }
  x1 = XEXP (x0, 0);
  goto L4733;

 L5237: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 0);
  if (GET_CODE (x2) == SET)
    goto L5238;
  x1 = XEXP (x0, 0);
  goto L4733;

 L5238: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L5239;
    }
  x1 = XEXP (x0, 0);
  goto L4733;

 L5239: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L5240;
  x1 = XEXP (x0, 0);
  goto L4733;

 L5240: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L5241;
    }
  x1 = XEXP (x0, 0);
  goto L4733;

 L5241: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 8)
    goto L5242;
  x1 = XEXP (x0, 0);
  goto L4733;

 L5242: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5243;
  x1 = XEXP (x0, 0);
  goto L4733;

 L5243: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L5244;
    }
  x1 = XEXP (x0, 0);
  goto L4733;

 L5244: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5245;
  x1 = XEXP (x0, 0);
  goto L4733;

 L5245: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[2]))
    goto L5246;
  x1 = XEXP (x0, 0);
  goto L4733;

 L5246: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 2);
  if (GET_CODE (x2) == SET)
    goto L5247;
  x1 = XEXP (x0, 0);
  goto L4733;

 L5247: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[4] = x3;
      goto L5248;
    }
  x1 = XEXP (x0, 0);
  goto L4733;

 L5248: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5249;
  x1 = XEXP (x0, 0);
  goto L4733;

 L5249: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode
      && GET_CODE (x4) == PLUS)
    goto L5250;
  x1 = XEXP (x0, 0);
  goto L4733;

 L5250: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[2]))
    goto L5251;
  x1 = XEXP (x0, 0);
  goto L4733;

 L5251: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT
      && XWINT (x5, 0) == 4
      && ((TARGET_ARM) && (TARGET_ARM && XVECLEN (operands[0], 0) == 3)))
    {
      return 617;
    }
  x1 = XEXP (x0, 0);
  goto L4733;

 L4734: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (cc_register (x2, VOIDmode))
    {
      operands[3] = x2;
      goto L4735;
    }
  goto L5175;

 L4735: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 0)
    goto L4736;
  goto L5175;

 L4736: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  switch (GET_CODE (x1))
    {
    case SET:
      goto L4737;
    case PARALLEL:
      goto L6493;
    default:
     break;
   }
  x1 = XEXP (x0, 0);
  goto L5175;

 L4737: ATTRIBUTE_UNUSED_LABEL
  tem = recog_15 (x0, insn, pnum_clobbers);
  if (tem >= 0)
    return tem;
  x1 = XEXP (x0, 0);
  goto L5175;

 L6493: ATTRIBUTE_UNUSED_LABEL
  if (XVECLEN (x1, 0) == 2)
    goto L5071;
  x1 = XEXP (x0, 0);
  goto L5175;

 L5071: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 0);
  if (GET_CODE (x2) == SET)
    goto L5072;
  x1 = XEXP (x0, 0);
  goto L5175;

 L5072: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L5073;
    }
  x1 = XEXP (x0, 0);
  goto L5175;

 L5073: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L5074;
  x1 = XEXP (x0, 0);
  goto L5175;

 L5074: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[0]))
    goto L5075;
  x1 = XEXP (x0, 0);
  goto L5175;

 L5075: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST)
    goto L5076;
  x1 = XEXP (x0, 0);
  goto L5175;

 L5076: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (GET_MODE (x5) == SImode
      && GET_CODE (x5) == PLUS)
    goto L5077;
  x1 = XEXP (x0, 0);
  goto L5175;

 L5077: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 0);
  if (GET_CODE (x6) == PC)
    goto L5078;
  x1 = XEXP (x0, 0);
  goto L5175;

 L5078: ATTRIBUTE_UNUSED_LABEL
  x6 = XEXP (x5, 1);
  if (GET_CODE (x6) == CONST_INT
      && XWINT (x6, 0) == 8)
    goto L5079;
  x1 = XEXP (x0, 0);
  goto L5175;

 L5079: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == USE)
    goto L5080;
  x1 = XEXP (x0, 0);
  goto L5175;

 L5080: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_CODE (x3) == LABEL_REF)
    goto L5081;
  x1 = XEXP (x0, 0);
  goto L5175;

 L5081: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  operands[1] = x4;
  goto L5082;

 L5082: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && flag_pic)))
    {
      return 603;
    }
  x1 = XEXP (x0, 0);
  goto L5175;

 L5176: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (cc_register (x2, VOIDmode))
    {
      operands[8] = x2;
      goto L5177;
    }
  goto L5207;

 L5177: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 0)
    goto L5178;
  goto L5207;

 L5178: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_CODE (x1) == PARALLEL)
    goto L6555;
  x1 = XEXP (x0, 0);
  goto L5207;

 L6555: ATTRIBUTE_UNUSED_LABEL
  if (XVECLEN (x1, 0) >= 5
      && load_multiple_operation (x1, VOIDmode))
    {
      operands[0] = x1;
      goto L5179;
    }
 L6556: ATTRIBUTE_UNUSED_LABEL
  if (XVECLEN (x1, 0) >= 5
      && store_multiple_operation (x1, VOIDmode))
    {
      operands[0] = x1;
      goto L5320;
    }
  x1 = XEXP (x0, 0);
  goto L5207;

 L5179: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 0);
  if (GET_CODE (x2) == SET)
    goto L5180;
  goto L6556;

 L5180: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L5181;
    }
  goto L6556;

 L5181: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L5182;
  goto L6556;

 L5182: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L5183;
    }
  goto L6556;

 L5183: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 16)
    goto L5184;
  goto L6556;

 L5184: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5185;
  goto L6556;

 L5185: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L5186;
    }
  goto L6556;

 L5186: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5187;
  goto L6556;

 L5187: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[2]))
    goto L5188;
  goto L6556;

 L5188: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 2);
  if (GET_CODE (x2) == SET)
    goto L5189;
  goto L6556;

 L5189: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[4] = x3;
      goto L5190;
    }
  goto L6556;

 L5190: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5191;
  goto L6556;

 L5191: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode
      && GET_CODE (x4) == PLUS)
    goto L5192;
  goto L6556;

 L5192: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[2]))
    goto L5193;
  goto L6556;

 L5193: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT
      && XWINT (x5, 0) == 4)
    goto L5194;
  goto L6556;

 L5194: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 3);
  if (GET_CODE (x2) == SET)
    goto L5195;
  goto L6556;

 L5195: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[5] = x3;
      goto L5196;
    }
  goto L6556;

 L5196: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5197;
  goto L6556;

 L5197: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode
      && GET_CODE (x4) == PLUS)
    goto L5198;
  goto L6556;

 L5198: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[2]))
    goto L5199;
  goto L6556;

 L5199: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT
      && XWINT (x5, 0) == 8)
    goto L5200;
  goto L6556;

 L5200: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 4);
  if (GET_CODE (x2) == SET)
    goto L5201;
  goto L6556;

 L5201: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[6] = x3;
      goto L5202;
    }
  goto L6556;

 L5202: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5203;
  goto L6556;

 L5203: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode
      && GET_CODE (x4) == PLUS)
    goto L5204;
  goto L6556;

 L5204: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[2]))
    goto L5205;
  goto L6556;

 L5205: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT
      && XWINT (x5, 0) == 12
      && ((TARGET_ARM) && (TARGET_ARM && XVECLEN (operands[0], 0) == 5)))
    {
      return 615;
    }
  goto L6556;

 L5320: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 0);
  if (GET_CODE (x2) == SET)
    goto L5321;
  x1 = XEXP (x0, 0);
  goto L5207;

 L5321: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L5322;
    }
  x1 = XEXP (x0, 0);
  goto L5207;

 L5322: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L5323;
  x1 = XEXP (x0, 0);
  goto L5207;

 L5323: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L5324;
    }
  x1 = XEXP (x0, 0);
  goto L5207;

 L5324: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 16)
    goto L5325;
  x1 = XEXP (x0, 0);
  goto L5207;

 L5325: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5326;
  x1 = XEXP (x0, 0);
  goto L5207;

 L5326: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5327;
  x1 = XEXP (x0, 0);
  goto L5207;

 L5327: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[2]))
    goto L5328;
  x1 = XEXP (x0, 0);
  goto L5207;

 L5328: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L5329;
    }
  x1 = XEXP (x0, 0);
  goto L5207;

 L5329: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 2);
  if (GET_CODE (x2) == SET)
    goto L5330;
  x1 = XEXP (x0, 0);
  goto L5207;

 L5330: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5331;
  x1 = XEXP (x0, 0);
  goto L5207;

 L5331: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode
      && GET_CODE (x4) == PLUS)
    goto L5332;
  x1 = XEXP (x0, 0);
  goto L5207;

 L5332: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[2]))
    goto L5333;
  x1 = XEXP (x0, 0);
  goto L5207;

 L5333: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT
      && XWINT (x5, 0) == 4)
    goto L5334;
  x1 = XEXP (x0, 0);
  goto L5207;

 L5334: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[4] = x3;
      goto L5335;
    }
  x1 = XEXP (x0, 0);
  goto L5207;

 L5335: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 3);
  if (GET_CODE (x2) == SET)
    goto L5336;
  x1 = XEXP (x0, 0);
  goto L5207;

 L5336: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5337;
  x1 = XEXP (x0, 0);
  goto L5207;

 L5337: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode
      && GET_CODE (x4) == PLUS)
    goto L5338;
  x1 = XEXP (x0, 0);
  goto L5207;

 L5338: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[2]))
    goto L5339;
  x1 = XEXP (x0, 0);
  goto L5207;

 L5339: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT
      && XWINT (x5, 0) == 8)
    goto L5340;
  x1 = XEXP (x0, 0);
  goto L5207;

 L5340: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[5] = x3;
      goto L5341;
    }
  x1 = XEXP (x0, 0);
  goto L5207;

 L5341: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 4);
  if (GET_CODE (x2) == SET)
    goto L5342;
  x1 = XEXP (x0, 0);
  goto L5207;

 L5342: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5343;
  x1 = XEXP (x0, 0);
  goto L5207;

 L5343: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode
      && GET_CODE (x4) == PLUS)
    goto L5344;
  x1 = XEXP (x0, 0);
  goto L5207;

 L5344: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[2]))
    goto L5345;
  x1 = XEXP (x0, 0);
  goto L5207;

 L5345: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT
      && XWINT (x5, 0) == 12)
    goto L5346;
  x1 = XEXP (x0, 0);
  goto L5207;

 L5346: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[6] = x3;
      goto L5347;
    }
  x1 = XEXP (x0, 0);
  goto L5207;

 L5347: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && XVECLEN (operands[0], 0) == 5)))
    {
      return 621;
    }
  x1 = XEXP (x0, 0);
  goto L5207;

 L5208: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (cc_register (x2, VOIDmode))
    {
      operands[7] = x2;
      goto L5209;
    }
  goto L5280;

 L5209: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 0)
    goto L5210;
  goto L5280;

 L5210: ATTRIBUTE_UNUSED_LABEL
  tem = recog_17 (x0, insn, pnum_clobbers);
  if (tem >= 0)
    return tem;
  x1 = XEXP (x0, 0);
  goto L5280;

 L5281: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (cc_register (x2, VOIDmode))
    {
      operands[6] = x2;
      goto L5282;
    }
  goto L5301;

 L5282: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 0)
    goto L5283;
  goto L5301;

 L5283: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  switch (GET_CODE (x1))
    {
    case PARALLEL:
      goto L6581;
    case SET:
      goto L5509;
    default:
     break;
   }
  x1 = XEXP (x0, 0);
  goto L5301;

 L6581: ATTRIBUTE_UNUSED_LABEL
  if (XVECLEN (x1, 0) == 2)
    goto L5521;
  if (XVECLEN (x1, 0) >= 3
      && load_multiple_operation (x1, VOIDmode))
    {
      operands[0] = x1;
      goto L5284;
    }
 L6580: ATTRIBUTE_UNUSED_LABEL
  if (XVECLEN (x1, 0) >= 3
      && store_multiple_operation (x1, VOIDmode))
    {
      operands[0] = x1;
      goto L5380;
    }
  x1 = XEXP (x0, 0);
  goto L5301;

 L5521: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 0);
  if (GET_CODE (x2) == SET)
    goto L5522;
  x1 = XEXP (x0, 0);
  goto L5301;

 L5522: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[0] = x3;
      goto L5523;
    }
  x1 = XEXP (x0, 0);
  goto L5301;

 L5523: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (shiftable_operator (x3, SImode))
    {
      operands[1] = x3;
      goto L5524;
    }
  x1 = XEXP (x0, 0);
  goto L5301;

 L5524: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (memory_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L5525;
    }
  x1 = XEXP (x0, 0);
  goto L5301;

 L5525: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (memory_operand (x4, SImode))
    {
      operands[3] = x4;
      goto L5526;
    }
  x1 = XEXP (x0, 0);
  goto L5301;

 L5526: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == CLOBBER)
    goto L5527;
  x1 = XEXP (x0, 0);
  goto L5301;

 L5527: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (scratch_operand (x3, SImode))
    {
      operands[4] = x3;
      goto L5528;
    }
  x1 = XEXP (x0, 0);
  goto L5301;

 L5528: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && adjacent_mem_locations (operands[2], operands[3]))))
    {
      return 633;
    }
  x1 = XEXP (x0, 0);
  goto L5301;

 L5284: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 0);
  if (GET_CODE (x2) == SET)
    goto L5285;
  goto L6580;

 L5285: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L5286;
    }
  goto L6580;

 L5286: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5287;
  goto L6580;

 L5287: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L5288;
    }
  goto L6580;

 L5288: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5289;
  goto L6580;

 L5289: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L5290;
    }
  goto L6580;

 L5290: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5291;
  goto L6580;

 L5291: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode
      && GET_CODE (x4) == PLUS)
    goto L5292;
  goto L6580;

 L5292: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[1]))
    goto L5293;
  goto L6580;

 L5293: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT
      && XWINT (x5, 0) == 4)
    goto L5294;
  goto L6580;

 L5294: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 2);
  if (GET_CODE (x2) == SET)
    goto L5295;
  goto L6580;

 L5295: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[4] = x3;
      goto L5296;
    }
  goto L6580;

 L5296: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5297;
  goto L6580;

 L5297: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode
      && GET_CODE (x4) == PLUS)
    goto L5298;
  goto L6580;

 L5298: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[1]))
    goto L5299;
  goto L6580;

 L5299: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT
      && XWINT (x5, 0) == 8
      && ((TARGET_ARM) && (TARGET_ARM && XVECLEN (operands[0], 0) == 3)))
    {
      return 619;
    }
  goto L6580;

 L5380: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 0);
  if (GET_CODE (x2) == SET)
    goto L5381;
  x1 = XEXP (x0, 0);
  goto L5301;

 L5381: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode)
    goto L6583;
  x1 = XEXP (x0, 0);
  goto L5301;

 L6583: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == MEM)
    goto L5431;
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L5382;
    }
  x1 = XEXP (x0, 0);
  goto L5301;

 L5431: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L5432;
    }
  x1 = XEXP (x0, 0);
  goto L5301;

 L5432: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L5433;
    }
  x1 = XEXP (x0, 0);
  goto L5301;

 L5433: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5434;
  x1 = XEXP (x0, 0);
  goto L5301;

 L5434: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5435;
  x1 = XEXP (x0, 0);
  goto L5301;

 L5435: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode
      && GET_CODE (x4) == PLUS)
    goto L5436;
  x1 = XEXP (x0, 0);
  goto L5301;

 L5436: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[1]))
    goto L5437;
  x1 = XEXP (x0, 0);
  goto L5301;

 L5437: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT
      && XWINT (x5, 0) == 4)
    goto L5438;
  x1 = XEXP (x0, 0);
  goto L5301;

 L5438: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L5439;
    }
  x1 = XEXP (x0, 0);
  goto L5301;

 L5439: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 2);
  if (GET_CODE (x2) == SET)
    goto L5440;
  x1 = XEXP (x0, 0);
  goto L5301;

 L5440: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5441;
  x1 = XEXP (x0, 0);
  goto L5301;

 L5441: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode
      && GET_CODE (x4) == PLUS)
    goto L5442;
  x1 = XEXP (x0, 0);
  goto L5301;

 L5442: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[1]))
    goto L5443;
  x1 = XEXP (x0, 0);
  goto L5301;

 L5443: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT
      && XWINT (x5, 0) == 8)
    goto L5444;
  x1 = XEXP (x0, 0);
  goto L5301;

 L5444: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[4] = x3;
      goto L5445;
    }
  x1 = XEXP (x0, 0);
  goto L5301;

 L5445: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && XVECLEN (operands[0], 0) == 3)))
    {
      return 625;
    }
  x1 = XEXP (x0, 0);
  goto L5301;

 L5382: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == PLUS)
    goto L5383;
  x1 = XEXP (x0, 0);
  goto L5301;

 L5383: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L5384;
    }
  x1 = XEXP (x0, 0);
  goto L5301;

 L5384: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 8)
    goto L5385;
  x1 = XEXP (x0, 0);
  goto L5301;

 L5385: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5386;
  x1 = XEXP (x0, 0);
  goto L5301;

 L5386: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5387;
  x1 = XEXP (x0, 0);
  goto L5301;

 L5387: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, operands[2]))
    goto L5388;
  x1 = XEXP (x0, 0);
  goto L5301;

 L5388: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L5389;
    }
  x1 = XEXP (x0, 0);
  goto L5301;

 L5389: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 2);
  if (GET_CODE (x2) == SET)
    goto L5390;
  x1 = XEXP (x0, 0);
  goto L5301;

 L5390: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5391;
  x1 = XEXP (x0, 0);
  goto L5301;

 L5391: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode
      && GET_CODE (x4) == PLUS)
    goto L5392;
  x1 = XEXP (x0, 0);
  goto L5301;

 L5392: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[2]))
    goto L5393;
  x1 = XEXP (x0, 0);
  goto L5301;

 L5393: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT
      && XWINT (x5, 0) == 4)
    goto L5394;
  x1 = XEXP (x0, 0);
  goto L5301;

 L5394: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[4] = x3;
      goto L5395;
    }
  x1 = XEXP (x0, 0);
  goto L5301;

 L5395: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && XVECLEN (operands[0], 0) == 3)))
    {
      return 623;
    }
  x1 = XEXP (x0, 0);
  goto L5301;

 L5509: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L5510;
    }
  x1 = XEXP (x0, 0);
  goto L5301;

 L5510: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == MINUS)
    goto L5511;
  x1 = XEXP (x0, 0);
  goto L5301;

 L5511: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[1] = x3;
      goto L5512;
    }
  x1 = XEXP (x0, 0);
  goto L5301;

 L5512: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (shift_operator (x3, SImode))
    {
      operands[2] = x3;
      goto L5513;
    }
  x1 = XEXP (x0, 0);
  goto L5301;

 L5513: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[3] = x4;
      goto L5514;
    }
  x1 = XEXP (x0, 0);
  goto L5301;

 L5514: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (reg_or_int_operand (x4, SImode))
    {
      operands[4] = x4;
      goto L5515;
    }
  x1 = XEXP (x0, 0);
  goto L5301;

 L5515: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 632;
    }
  x1 = XEXP (x0, 0);
  goto L5301;

 L5302: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (cc_register (x2, VOIDmode))
    {
      operands[5] = x2;
      goto L5303;
    }
  goto L5463;

 L5303: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 0)
    goto L5304;
  goto L5463;

 L5304: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_CODE (x1) == PARALLEL)
    goto L6586;
  x1 = XEXP (x0, 0);
  goto L5463;

 L6586: ATTRIBUTE_UNUSED_LABEL
  if (XVECLEN (x1, 0) == 2)
    goto L5534;
 L6584: ATTRIBUTE_UNUSED_LABEL
  if (XVECLEN (x1, 0) >= 2
      && load_multiple_operation (x1, VOIDmode))
    {
      operands[0] = x1;
      goto L5305;
    }
 L6585: ATTRIBUTE_UNUSED_LABEL
  if (XVECLEN (x1, 0) >= 2
      && store_multiple_operation (x1, VOIDmode))
    {
      operands[0] = x1;
      goto L5451;
    }
  x1 = XEXP (x0, 0);
  goto L5463;

 L5534: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 0);
  if (GET_CODE (x2) == SET)
    goto L5535;
  goto L6584;

 L5535: ATTRIBUTE_UNUSED_LABEL
  tem = recog_18 (x0, insn, pnum_clobbers);
  if (tem >= 0)
    return tem;
  goto L6584;

 L5305: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 0);
  if (GET_CODE (x2) == SET)
    goto L5306;
  goto L6585;

 L5306: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L5307;
    }
  goto L6585;

 L5307: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5308;
  goto L6585;

 L5308: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L5309;
    }
  goto L6585;

 L5309: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5310;
  goto L6585;

 L5310: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L5311;
    }
  goto L6585;

 L5311: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5312;
  goto L6585;

 L5312: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode
      && GET_CODE (x4) == PLUS)
    goto L5313;
  goto L6585;

 L5313: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[1]))
    goto L5314;
  goto L6585;

 L5314: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT
      && XWINT (x5, 0) == 4
      && ((TARGET_ARM) && (TARGET_ARM && XVECLEN (operands[0], 0) == 2)))
    {
      return 620;
    }
  goto L6585;

 L5451: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 0);
  if (GET_CODE (x2) == SET)
    goto L5452;
  x1 = XEXP (x0, 0);
  goto L5463;

 L5452: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5453;
  x1 = XEXP (x0, 0);
  goto L5463;

 L5453: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L5454;
    }
  x1 = XEXP (x0, 0);
  goto L5463;

 L5454: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L5455;
    }
  x1 = XEXP (x0, 0);
  goto L5463;

 L5455: ATTRIBUTE_UNUSED_LABEL
  x2 = XVECEXP (x1, 0, 1);
  if (GET_CODE (x2) == SET)
    goto L5456;
  x1 = XEXP (x0, 0);
  goto L5463;

 L5456: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == MEM)
    goto L5457;
  x1 = XEXP (x0, 0);
  goto L5463;

 L5457: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode
      && GET_CODE (x4) == PLUS)
    goto L5458;
  x1 = XEXP (x0, 0);
  goto L5463;

 L5458: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (rtx_equal_p (x5, operands[1]))
    goto L5459;
  x1 = XEXP (x0, 0);
  goto L5463;

 L5459: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (GET_CODE (x5) == CONST_INT
      && XWINT (x5, 0) == 4)
    goto L5460;
  x1 = XEXP (x0, 0);
  goto L5463;

 L5460: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_hard_register_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L5461;
    }
  x1 = XEXP (x0, 0);
  goto L5463;

 L5461: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM && XVECLEN (operands[0], 0) == 2)))
    {
      return 626;
    }
  x1 = XEXP (x0, 0);
  goto L5463;

 L5464: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (cc_register (x2, VOIDmode))
    {
      operands[2] = x2;
      goto L5465;
    }
  goto L5472;

 L5465: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 0)
    goto L5466;
  goto L5472;

 L5466: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_CODE (x1) == SET)
    goto L5467;
  x1 = XEXP (x0, 0);
  goto L5472;

 L5467: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) == PC)
    goto L5482;
  x1 = XEXP (x0, 0);
  goto L5472;

 L5482: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode)
    goto L6606;
 L5468: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == LABEL_REF)
    goto L5469;
  x1 = XEXP (x0, 0);
  goto L5472;

 L6606: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L5483;
    }
 L6607: ATTRIBUTE_UNUSED_LABEL
  if (memory_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L5491;
    }
  goto L5468;

 L5483: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 629;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 1);
  goto L6607;

 L5491: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 630;
    }
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 1);
  goto L5468;

 L5469: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  operands[0] = x3;
  goto L5470;

 L5470: ATTRIBUTE_UNUSED_LABEL
  if (((TARGET_ARM) && (TARGET_ARM)))
    {
      return 627;
    }
  x1 = XEXP (x0, 0);
  goto L5472;

 L5473: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (cc_register (x2, VOIDmode))
    {
      operands[1] = x2;
      goto L5474;
    }
  goto ret0;

 L5474: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT
      && XWINT (x2, 0) == 0)
    goto L5475;
  goto ret0;

 L5475: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_CODE (x1) == RETURN
      && ((TARGET_ARM) && (TARGET_ARM && USE_RETURN_INSN (FALSE))))
    {
      return 628;
    }
  goto ret0;
 ret0:
  return -1;
}

int recog PARAMS ((rtx, rtx, int *));
int
recog (x0, insn, pnum_clobbers)
     rtx x0 ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
     int *pnum_clobbers ATTRIBUTE_UNUSED;
{
  rtx * const operands ATTRIBUTE_UNUSED = &recog_data.operand[0];
  rtx x1 ATTRIBUTE_UNUSED;
  rtx x2 ATTRIBUTE_UNUSED;
  rtx x3 ATTRIBUTE_UNUSED;
  rtx x4 ATTRIBUTE_UNUSED;
  rtx x5 ATTRIBUTE_UNUSED;
  rtx x6 ATTRIBUTE_UNUSED;
  rtx x7 ATTRIBUTE_UNUSED;
  int tem ATTRIBUTE_UNUSED;
  recog_data.insn = NULL_RTX;

  if (GET_MODE (x0) == SImode
      && GET_CODE (x0) == UNSPEC
      && XVECLEN (x0, 0) == 1
      && XINT (x0, 1) == 6)
    goto L3797;
  switch (GET_CODE (x0))
    {
    case PARALLEL:
      goto L5923;
    case SET:
      goto L9;
    case RETURN:
      goto L5938;
    case UNSPEC_VOLATILE:
      goto L5939;
    case CONST_INT:
      goto L5940;
    case COND_EXEC:
      goto L4106;
    default:
     break;
   }
  goto ret0;

 L3797: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  if (register_operand (x1, SImode))
    {
      operands[0] = x1;
      return 358;
    }
  goto ret0;

 L5923: ATTRIBUTE_UNUSED_LABEL
  return recog_7 (x0, insn, pnum_clobbers);

 L9: ATTRIBUTE_UNUSED_LABEL
  return recog_13 (x0, insn, pnum_clobbers);

 L5938: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && USE_RETURN_INSN (FALSE)))
    {
      return 259;
    }
  goto ret0;

 L5939: ATTRIBUTE_UNUSED_LABEL
  if (XVECLEN (x0, 0) == 1)
    goto L6399;
  goto ret0;

 L6399: ATTRIBUTE_UNUSED_LABEL
  switch (XINT (x0, 1))
    {
    case 0:
      goto L2507;
    case 1:
      goto L3727;
    case 2:
      goto L3768;
    case 3:
      goto L3770;
    case 4:
      goto L3772;
    case 5:
      goto L3775;
    case 6:
      goto L3778;
    case 7:
      goto L3781;
    default:
      break;
    }
  goto ret0;

 L2507: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  if (GET_CODE (x1) == CONST_INT
      && XWINT (x1, 0) == 0
      && (TARGET_EITHER))
    {
      return 262;
    }
  goto ret0;

 L3727: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  switch (GET_CODE (x1))
    {
    case CONST_INT:
      goto L6407;
    case RETURN:
      goto L6408;
    default:
     break;
   }
  goto ret0;

 L6407: ATTRIBUTE_UNUSED_LABEL
  if (XWINT (x1, 0) == 0
      && (TARGET_ARM))
    {
      return 343;
    }
  goto ret0;

 L6408: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_EITHER))
    {
      return 344;
    }
  goto ret0;

 L3768: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  if (GET_CODE (x1) == CONST_INT
      && XWINT (x1, 0) == 0
      && (TARGET_EITHER))
    {
      return 350;
    }
  goto ret0;

 L3770: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  if (GET_CODE (x1) == CONST_INT
      && XWINT (x1, 0) == 0
      && (TARGET_EITHER))
    {
      return 351;
    }
  goto ret0;

 L3772: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  operands[0] = x1;
  goto L3773;

 L3773: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB))
    {
      return 352;
    }
  goto ret0;

 L3775: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  operands[0] = x1;
  goto L3776;

 L3776: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB))
    {
      return 353;
    }
  goto ret0;

 L3778: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  operands[0] = x1;
  goto L3779;

 L3779: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_EITHER))
    {
      return 354;
    }
  goto ret0;

 L3781: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  operands[0] = x1;
  goto L3782;

 L3782: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_EITHER))
    {
      return 355;
    }
  goto ret0;

 L5940: ATTRIBUTE_UNUSED_LABEL
  if (XWINT (x0, 0) == 0
      && (TARGET_EITHER))
    {
      return 267;
    }
  goto ret0;

 L4106: ATTRIBUTE_UNUSED_LABEL
  return recog_19 (x0, insn, pnum_clobbers);
 ret0:
  return -1;
}

static rtx split_1 PARAMS ((rtx, rtx));
static rtx
split_1 (x0, insn)
     rtx x0 ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  rtx * const operands ATTRIBUTE_UNUSED = &recog_data.operand[0];
  rtx x1 ATTRIBUTE_UNUSED;
  rtx x2 ATTRIBUTE_UNUSED;
  rtx x3 ATTRIBUTE_UNUSED;
  rtx x4 ATTRIBUTE_UNUSED;
  rtx x5 ATTRIBUTE_UNUSED;
  rtx x6 ATTRIBUTE_UNUSED;
  rtx x7 ATTRIBUTE_UNUSED;
  rtx tem ATTRIBUTE_UNUSED;

  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  switch (GET_MODE (x2))
    {
    case DImode:
      goto L6609;
    case SImode:
      goto L6610;
    case CC_Zmode:
      goto L6611;
    default:
      break;
    }
  goto ret0;

 L6609: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, DImode))
    {
      operands[0] = x2;
      goto L3801;
    }
  goto ret0;

 L3801: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == DImode
      && GET_CODE (x2) == PLUS)
    goto L3802;
  goto ret0;

 L3802: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == DImode)
    goto L6613;
  goto ret0;

 L6613: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x3))
    {
    case SIGN_EXTEND:
      goto L3811;
    case ZERO_EXTEND:
      goto L3820;
    case SUBREG:
    case REG:
      goto L6612;
    default:
      goto ret0;
   }
 L6612: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x3, DImode))
    {
      operands[1] = x3;
      goto L3803;
    }
  goto ret0;

 L3811: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L3812;
    }
  goto ret0;

 L3812: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, DImode))
    {
      operands[1] = x3;
      goto L3813;
    }
  goto ret0;

 L3813: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L3814;
  goto ret0;

 L3814: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM && reload_completed))
    {
      return gen_split_361 (operands);
    }
  goto ret0;

 L3820: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L3821;
    }
  goto ret0;

 L3821: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, DImode))
    {
      operands[1] = x3;
      goto L3822;
    }
  goto ret0;

 L3822: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L3823;
  goto ret0;

 L3823: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM && reload_completed))
    {
      return gen_split_362 (operands);
    }
  goto ret0;

 L3803: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, DImode))
    {
      operands[2] = x3;
      goto L3804;
    }
  goto ret0;

 L3804: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L3805;
  goto ret0;

 L3805: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM && reload_completed))
    {
      return gen_split_360 (operands);
    }
  goto ret0;

 L6610: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x2, SImode))
    {
      operands[0] = x2;
      goto L3939;
    }
  goto ret0;

 L3939: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode)
    goto L6615;
  goto ret0;

 L6615: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x2))
    {
    case IOR:
      goto L3940;
    case ZERO_EXTEND:
      goto L3958;
    case SIGN_EXTEND:
      goto L3985;
    case AND:
      goto L4038;
    case IF_THEN_ELSE:
      goto L4064;
    default:
     break;
   }
 L6617: ATTRIBUTE_UNUSED_LABEL
  if (shiftable_operator (x2, SImode))
    {
      operands[3] = x2;
      goto L3966;
    }
  goto ret0;

 L3940: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == AND)
    goto L3941;
  goto L6617;

 L3941: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode
      && GET_CODE (x4) == NOT)
    goto L3942;
  goto L6617;

 L3942: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[1] = x5;
      goto L3943;
    }
  goto L6617;

 L3943: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_MODE (x4) == SImode
      && GET_CODE (x4) == NOT)
    goto L3944;
  goto L6617;

 L3944: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (arm_rhs_operand (x5, SImode))
    {
      operands[2] = x5;
      goto L3945;
    }
  goto L6617;

 L3945: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[3] = x3;
      goto L3946;
    }
  goto L6617;

 L3946: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L3947;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L6617;

 L3947: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[4] = x2;
      goto L3948;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L6617;

 L3948: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return gen_split_387 (operands);
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L6617;

 L3958: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  switch (GET_MODE (x3))
    {
    case HImode:
      goto L6621;
    case QImode:
      goto L6622;
    default:
      break;
    }
  goto ret0;

 L6621: ATTRIBUTE_UNUSED_LABEL
  if (alignable_memory_operand (x3, HImode))
    {
      operands[1] = x3;
      goto L3959;
    }
  goto ret0;

 L3959: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L3960;
  goto ret0;

 L3960: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L3961;
    }
  goto ret0;

 L3961: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && (!arm_arch4)))
    {
      return gen_split_399 (operands);
    }
  goto ret0;

 L6622: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x3) == SUBREG
      && XINT (x3, 1) == 0)
    goto L3977;
  goto ret0;

 L3977: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  operands[1] = x4;
  goto L3978;

 L3978: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L3979;
  goto ret0;

 L3979: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L3980;
    }
  goto ret0;

 L3980: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && (GET_CODE (operands[1]) != MEM)))
    {
      return gen_split_402 (operands);
    }
  goto ret0;

 L3985: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (alignable_memory_operand (x3, HImode))
    {
      operands[1] = x3;
      goto L3986;
    }
  goto ret0;

 L3986: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L3987;
  goto ret0;

 L3987: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L3988;
    }
  goto ret0;

 L3988: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && (!arm_arch4)))
    {
      return gen_split_405 (operands);
    }
  goto ret0;

 L4038: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == GE)
    goto L4039;
  goto L6617;

 L4039: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[1] = x4;
      goto L4040;
    }
  goto L6617;

 L4040: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 0)
    goto L4041;
  goto L6617;

 L4041: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == NEG)
    goto L4042;
  goto L6617;

 L4042: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (arm_comparison_operator (x4, SImode))
    {
      operands[2] = x4;
      goto L4043;
    }
  goto L6617;

 L4043: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (s_register_operand (x5, SImode))
    {
      operands[3] = x5;
      goto L4044;
    }
  goto L6617;

 L4044: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 1);
  if (arm_rhs_operand (x5, SImode))
    {
      operands[4] = x5;
      goto L4045;
    }
  goto L6617;

 L4045: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L4046;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L6617;

 L4046: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[5] = x2;
      goto L4047;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L6617;

 L4047: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM))
    {
      return gen_split_500 (operands);
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L6617;

 L4064: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (arm_comparison_operator (x3, VOIDmode))
    {
      operands[1] = x3;
      goto L4065;
    }
  goto ret0;

 L4065: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  operands[2] = x4;
  goto L4066;
 L4098: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L4099;
    }
  goto ret0;

 L4066: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  operands[3] = x4;
  goto L4067;

 L4067: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, operands[0]))
    goto L4068;
 L4078: ATTRIBUTE_UNUSED_LABEL
  operands[4] = x3;
  goto L4079;

 L4068: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  operands[4] = x3;
  goto L4069;

 L4069: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L4070;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L4078;

 L4070: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM && reload_completed))
    {
      return gen_split_505 (operands);
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 1);
  goto L4078;

 L4079: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (rtx_equal_p (x3, operands[0]))
    goto L4080;
 L4090: ATTRIBUTE_UNUSED_LABEL
  operands[5] = x3;
  goto L4091;

 L4080: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L4081;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 2);
  goto L4090;

 L4081: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM && reload_completed))
    {
      return gen_split_506 (operands);
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 2);
  goto L4090;

 L4091: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L4092;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L4098;

 L4092: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM && reload_completed))
    {
      return gen_split_507 (operands);
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L4098;

 L4099: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (arm_add_operand (x4, SImode))
    {
      operands[3] = x4;
      goto L4100;
    }
  goto ret0;

 L4100: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (arm_rhs_operand (x3, SImode))
    {
      operands[4] = x3;
      goto L4101;
    }
  goto ret0;

 L4101: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 2);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == NOT)
    goto L4102;
  goto ret0;

 L4102: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[5] = x4;
      goto L4103;
    }
  goto ret0;

 L4103: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L4104;
  goto ret0;

 L4104: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24
      && (TARGET_ARM && reload_completed))
    {
      return gen_split_508 (operands);
    }
  goto ret0;

 L3966: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode)
    goto L6623;
  goto ret0;

 L6623: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x3))
    {
    case ZERO_EXTEND:
      goto L3967;
    case SIGN_EXTEND:
      goto L3994;
    default:
     break;
   }
  goto ret0;

 L3967: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (alignable_memory_operand (x4, HImode))
    {
      operands[1] = x4;
      goto L3968;
    }
  goto ret0;

 L3968: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      operands[4] = x3;
      goto L3969;
    }
  goto ret0;

 L3969: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L3970;
  goto ret0;

 L3970: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L3971;
    }
  goto ret0;

 L3971: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && (!arm_arch4)))
    {
      return gen_split_400 (operands);
    }
  goto ret0;

 L3994: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (alignable_memory_operand (x4, HImode))
    {
      operands[1] = x4;
      goto L3995;
    }
  goto ret0;

 L3995: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (s_register_operand (x3, SImode))
    {
      operands[4] = x3;
      goto L3996;
    }
  goto ret0;

 L3996: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L3997;
  goto ret0;

 L3997: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L3998;
    }
  goto ret0;

 L3998: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && (!arm_arch4)))
    {
      return gen_split_406 (operands);
    }
  goto ret0;

 L6611: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == REG
      && XINT (x2, 0) == 24)
    goto L4051;
  goto ret0;

 L4051: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == CC_Zmode
      && GET_CODE (x2) == COMPARE)
    goto L4052;
  goto ret0;

 L4052: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode
      && GET_CODE (x3) == ASHIFT)
    goto L4053;
  goto ret0;

 L4053: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode
      && GET_CODE (x4) == SUBREG
      && XINT (x4, 1) == 0)
    goto L4054;
  goto ret0;

 L4054: ATTRIBUTE_UNUSED_LABEL
  x5 = XEXP (x4, 0);
  if (memory_operand (x5, QImode))
    {
      operands[0] = x5;
      goto L4055;
    }
  goto ret0;

 L4055: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT
      && XWINT (x4, 0) == 24)
    goto L4056;
  goto ret0;

 L4056: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (const_int_operand (x3, VOIDmode))
    {
      operands[1] = x3;
      goto L4057;
    }
  goto ret0;

 L4057: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER)
    goto L4058;
  goto ret0;

 L4058: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L4059;
    }
  goto ret0;

 L4059: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM
   && (((unsigned HOST_WIDE_INT) INTVAL (operands[1]))
       == (((unsigned HOST_WIDE_INT) INTVAL (operands[1])) >> 24) << 24)))
    {
      return gen_split_501 (operands);
    }
  goto ret0;
 ret0:
  return 0;
}

rtx split_insns PARAMS ((rtx, rtx));
rtx
split_insns (x0, insn)
     rtx x0 ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
{
  rtx * const operands ATTRIBUTE_UNUSED = &recog_data.operand[0];
  rtx x1 ATTRIBUTE_UNUSED;
  rtx x2 ATTRIBUTE_UNUSED;
  rtx x3 ATTRIBUTE_UNUSED;
  rtx x4 ATTRIBUTE_UNUSED;
  rtx x5 ATTRIBUTE_UNUSED;
  rtx x6 ATTRIBUTE_UNUSED;
  rtx x7 ATTRIBUTE_UNUSED;
  rtx tem ATTRIBUTE_UNUSED;
  recog_data.insn = NULL_RTX;

  switch (GET_CODE (x0))
    {
    case PARALLEL:
      goto L6608;
    case SET:
      goto L3832;
    default:
     break;
   }
  goto ret0;

 L6608: ATTRIBUTE_UNUSED_LABEL
  if (XVECLEN (x0, 0) == 2)
    goto L3799;
  goto ret0;

 L3799: ATTRIBUTE_UNUSED_LABEL
  x1 = XVECEXP (x0, 0, 0);
  if (GET_CODE (x1) == SET)
    goto L3800;
  goto ret0;

 L3800: ATTRIBUTE_UNUSED_LABEL
  return split_1 (x0, insn);

 L3832: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 0);
  switch (GET_MODE (x1))
    {
    case SImode:
      goto L6625;
    case DImode:
      goto L6626;
    case HImode:
      goto L6627;
    case SFmode:
      goto L6629;
    default:
      break;
    }
  goto ret0;

 L6625: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x1, SImode))
    {
      operands[0] = x1;
      goto L3833;
    }
 L6628: ATTRIBUTE_UNUSED_LABEL
  if (register_operand (x1, SImode))
    {
      operands[0] = x1;
      goto L4015;
    }
  goto ret0;

 L3833: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == SImode)
    goto L6630;
 L4011: ATTRIBUTE_UNUSED_LABEL
  if (const_int_operand (x1, SImode))
    {
      operands[1] = x1;
      goto L4012;
    }
  x1 = XEXP (x0, 0);
  goto L6628;

 L6630: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x1))
    {
    case PLUS:
      goto L3834;
    case MINUS:
      goto L3850;
    case AND:
      goto L3897;
    case IOR:
      goto L3926;
    case SIGN_EXTEND:
      goto L4007;
    default:
     break;
   }
  goto L4011;

 L3834: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L3835;
    }
  goto L4011;

 L3835: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (reg_or_int_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L3836;
    }
  goto L4011;

 L3836: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM &&
   GET_CODE (operands[2]) == CONST_INT
   && !(const_ok_for_arm (INTVAL (operands[2]))
        || const_ok_for_arm (-INTVAL (operands[2])))))
    {
      return gen_split_365 (operands);
    }
  x1 = XEXP (x0, 1);
  goto L4011;

 L3850: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (reg_or_int_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L3851;
    }
  goto L4011;

 L3851: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L3852;
    }
  goto L4011;

 L3852: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM
   && GET_CODE (operands[1]) == CONST_INT
   && !const_ok_for_arm (INTVAL (operands[1]))))
    {
      return gen_split_369 (operands);
    }
  x1 = XEXP (x0, 1);
  goto L4011;

 L3897: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L3898;
    }
  goto L4011;

 L3898: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (reg_or_int_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L3899;
    }
  goto L4011;

 L3899: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM
   && GET_CODE (operands[2]) == CONST_INT
   && !(const_ok_for_arm (INTVAL (operands[2]))
	|| const_ok_for_arm (~INTVAL (operands[2])))))
    {
      return gen_split_378 (operands);
    }
  x1 = XEXP (x0, 1);
  goto L4011;

 L3926: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L3927;
    }
  goto L4011;

 L3927: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (reg_or_int_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L3928;
    }
  goto L4011;

 L3928: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM
   && GET_CODE (operands[2]) == CONST_INT
   && !const_ok_for_arm (INTVAL (operands[2]))))
    {
      return gen_split_384 (operands);
    }
  x1 = XEXP (x0, 1);
  goto L4011;

 L4007: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (bad_signed_byte_operand (x2, QImode))
    {
      operands[1] = x2;
      goto L4008;
    }
  goto L4011;

 L4008: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && arm_arch4 && reload_completed))
    {
      return gen_split_410 (operands);
    }
  x1 = XEXP (x0, 1);
  goto L4011;

 L4012: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM
  && (!(const_ok_for_arm (INTVAL (operands[1]))
        || const_ok_for_arm (~INTVAL (operands[1]))))))
    {
      return gen_split_413 (operands);
    }
  x1 = XEXP (x0, 0);
  goto L6628;

 L4015: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (const_int_operand (x1, SImode))
    {
      operands[1] = x1;
      goto L4016;
    }
  goto ret0;

 L4016: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB && CONST_OK_FOR_THUMB_LETTER (INTVAL (operands[1]), 'J')))
    {
      return gen_split_414 (operands);
    }
 L4020: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB && CONST_OK_FOR_THUMB_LETTER (INTVAL (operands[1]), 'K')))
    {
      return gen_split_415 (operands);
    }
  goto ret0;

 L6626: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x1, DImode))
    {
      operands[0] = x1;
      goto L3862;
    }
  goto ret0;

 L3862: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == DImode)
    goto L6636;
  goto ret0;

 L6636: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x1))
    {
    case IOR:
      goto L3876;
    case XOR:
      goto L3883;
    case AND:
      goto L3890;
    case NOT:
      goto L3952;
    default:
     break;
   }
 L6635: ATTRIBUTE_UNUSED_LABEL
  if (logical_binary_operator (x1, DImode))
    {
      operands[6] = x1;
      goto L3863;
    }
  goto ret0;

 L3876: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == DImode
      && GET_CODE (x2) == ZERO_EXTEND)
    goto L3877;
  goto L6635;

 L3877: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L3878;
    }
  goto L6635;

 L3878: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L3879;
    }
  goto L6635;

 L3879: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && operands[0] != operands[1] && reload_completed))
    {
      return gen_split_374 (operands);
    }
  x1 = XEXP (x0, 1);
  goto L6635;

 L3883: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == DImode
      && GET_CODE (x2) == ZERO_EXTEND)
    goto L3884;
  goto L6635;

 L3884: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L3885;
    }
  goto L6635;

 L3885: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L3886;
    }
  goto L6635;

 L3886: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && operands[0] != operands[1] && reload_completed))
    {
      return gen_split_375 (operands);
    }
  x1 = XEXP (x0, 1);
  goto L6635;

 L3890: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == DImode)
    goto L6640;
  goto L6635;

 L6640: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x2))
    {
    case ZERO_EXTEND:
      goto L3891;
    case NOT:
      goto L3904;
    default:
     break;
   }
  goto L6635;

 L3891: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L3892;
    }
  goto L6635;

 L3892: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L3893;
    }
  goto L6635;

 L3893: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && reload_completed))
    {
      return gen_split_376 (operands);
    }
  x1 = XEXP (x0, 1);
  goto L6635;

 L3904: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == DImode)
    goto L6643;
  goto L6635;

 L6643: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x3))
    {
    case ZERO_EXTEND:
      goto L3912;
    case SIGN_EXTEND:
      goto L3920;
    case SUBREG:
    case REG:
      goto L6642;
    default:
      goto L6635;
   }
 L6642: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x3, DImode))
    {
      operands[1] = x3;
      goto L3905;
    }
  goto L6635;

 L3912: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L3913;
    }
  goto L6635;

 L3913: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L3914;
    }
  goto L6635;

 L3914: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM
   && reload_completed
   && operands[0] != operands[1]))
    {
      return gen_split_381 (operands);
    }
  x1 = XEXP (x0, 1);
  goto L6635;

 L3920: ATTRIBUTE_UNUSED_LABEL
  x4 = XEXP (x3, 0);
  if (s_register_operand (x4, SImode))
    {
      operands[2] = x4;
      goto L3921;
    }
  goto L6635;

 L3921: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L3922;
    }
  goto L6635;

 L3922: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && reload_completed))
    {
      return gen_split_382 (operands);
    }
  x1 = XEXP (x0, 1);
  goto L6635;

 L3905: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      operands[2] = x2;
      goto L3906;
    }
  goto L6635;

 L3906: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && reload_completed))
    {
      return gen_split_380 (operands);
    }
  x1 = XEXP (x0, 1);
  goto L6635;

 L3952: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L3953;
    }
  goto ret0;

 L3953: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && reload_completed))
    {
      return gen_split_396 (operands);
    }
  goto ret0;

 L3863: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == DImode)
    goto L6646;
  goto ret0;

 L6646: ATTRIBUTE_UNUSED_LABEL
  if (GET_CODE (x2) == SIGN_EXTEND)
    goto L3870;
  if (s_register_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L3864;
    }
  goto ret0;

 L3870: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (s_register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L3871;
    }
  goto ret0;

 L3871: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      operands[1] = x2;
      goto L3872;
    }
  goto ret0;

 L3872: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && reload_completed))
    {
      return gen_split_373 (operands);
    }
  goto ret0;

 L3864: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, DImode))
    {
      operands[2] = x2;
      goto L3865;
    }
  goto ret0;

 L3865: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && reload_completed))
    {
      return gen_split_372 (operands);
    }
  goto ret0;

 L6627: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x1, HImode))
    {
      operands[0] = x1;
      goto L4001;
    }
  goto ret0;

 L4001: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == HImode
      && GET_CODE (x1) == SIGN_EXTEND)
    goto L4002;
  goto ret0;

 L4002: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (bad_signed_byte_operand (x2, QImode))
    {
      operands[1] = x2;
      goto L4003;
    }
  goto ret0;

 L4003: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM && arm_arch4 && reload_completed))
    {
      return gen_split_408 (operands);
    }
  goto ret0;

 L6629: ATTRIBUTE_UNUSED_LABEL
  if (nonimmediate_operand (x1, SFmode))
    {
      operands[0] = x1;
      goto L4023;
    }
  goto ret0;

 L4023: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (immediate_operand (x1, SFmode))
    {
      operands[1] = x1;
      goto L4024;
    }
  goto ret0;

 L4024: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_ARM
   && !TARGET_HARD_FLOAT
   && reload_completed
   && GET_CODE (operands[1]) == CONST_DOUBLE))
    {
      return gen_split_430 (operands);
    }
  goto ret0;
 ret0:
  return 0;
}

rtx peephole2_insns PARAMS ((rtx, rtx, int *));
rtx
peephole2_insns (x0, insn, _pmatch_len)
     rtx x0 ATTRIBUTE_UNUSED;
     rtx insn ATTRIBUTE_UNUSED;
     int *_pmatch_len ATTRIBUTE_UNUSED;
{
  rtx * const operands ATTRIBUTE_UNUSED = &recog_data.operand[0];
  rtx x1 ATTRIBUTE_UNUSED;
  rtx x2 ATTRIBUTE_UNUSED;
  rtx x3 ATTRIBUTE_UNUSED;
  rtx x4 ATTRIBUTE_UNUSED;
  rtx x5 ATTRIBUTE_UNUSED;
  rtx x6 ATTRIBUTE_UNUSED;
  rtx x7 ATTRIBUTE_UNUSED;
  rtx tem ATTRIBUTE_UNUSED;
  recog_data.insn = NULL_RTX;

  if (GET_CODE (x0) == SET)
    goto L3826;
  goto ret0;

 L3826: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 0);
  if (GET_MODE (x1) == SImode)
    goto L6647;
  goto ret0;

 L6647: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x1, SImode))
    {
      operands[0] = x1;
      goto L3827;
    }
 L6648: ATTRIBUTE_UNUSED_LABEL
  if (register_operand (x1, SImode))
    {
      operands[0] = x1;
      goto L3840;
    }
  goto ret0;

 L3827: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == SImode)
    goto L6649;
  x1 = XEXP (x0, 0);
  goto L6648;

 L6649: ATTRIBUTE_UNUSED_LABEL
  switch (GET_CODE (x1))
    {
    case PLUS:
      goto L3828;
    case MINUS:
      goto L3857;
    case IOR:
      goto L3933;
    case SUBREG:
    case REG:
      goto L6652;
    default:
      x1 = XEXP (x0, 0);
      goto L6648;
   }
 L6652: ATTRIBUTE_UNUSED_LABEL
  if (s_register_operand (x1, SImode))
    {
      operands[1] = x1;
      goto L4029;
    }
  x1 = XEXP (x0, 0);
  goto L6648;

 L3828: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L3829;
    }
  x1 = XEXP (x0, 0);
  goto L6648;

 L3829: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (const_int_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L3830;
    }
  x1 = XEXP (x0, 0);
  goto L6648;

 L3830: ATTRIBUTE_UNUSED_LABEL
  tem = peep2_next_insn (0);
  if (tem == NULL_RTX)
    goto L6648;
  x1 = PATTERN (tem);
  if ((TARGET_ARM &&
   !(const_ok_for_arm (INTVAL (operands[2]))
     || const_ok_for_arm (-INTVAL (operands[2])))
    && const_ok_for_arm (~INTVAL (operands[2]))))
    {
      *_pmatch_len = 0;
      tem = gen_peephole2_364 (insn, operands);
      if (tem != 0)
        return tem;
    }
  x1 = XEXP (x0, 0);
  goto L6648;

 L3857: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (const_int_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L3858;
    }
  x1 = XEXP (x0, 0);
  goto L6648;

 L3858: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (s_register_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L3859;
    }
  x1 = XEXP (x0, 0);
  goto L6648;

 L3859: ATTRIBUTE_UNUSED_LABEL
  tem = peep2_next_insn (0);
  if (tem == NULL_RTX)
    goto L6648;
  x1 = PATTERN (tem);
  if ((TARGET_ARM
   && !const_ok_for_arm (INTVAL (operands[1]))
   && const_ok_for_arm (~INTVAL (operands[1]))))
    {
      *_pmatch_len = 0;
      tem = gen_peephole2_370 (insn, operands);
      if (tem != 0)
        return tem;
    }
  x1 = XEXP (x0, 0);
  goto L6648;

 L3933: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (s_register_operand (x2, SImode))
    {
      operands[1] = x2;
      goto L3934;
    }
  x1 = XEXP (x0, 0);
  goto L6648;

 L3934: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (const_int_operand (x2, SImode))
    {
      operands[2] = x2;
      goto L3935;
    }
  x1 = XEXP (x0, 0);
  goto L6648;

 L3935: ATTRIBUTE_UNUSED_LABEL
  tem = peep2_next_insn (0);
  if (tem == NULL_RTX)
    goto L6648;
  x1 = PATTERN (tem);
  if ((TARGET_ARM
   && !const_ok_for_arm (INTVAL (operands[2]))
   && const_ok_for_arm (~INTVAL (operands[2]))))
    {
      *_pmatch_len = 0;
      tem = gen_peephole2_385 (insn, operands);
      if (tem != 0)
        return tem;
    }
  x1 = XEXP (x0, 0);
  goto L6648;

 L4029: ATTRIBUTE_UNUSED_LABEL
  tem = peep2_next_insn (1);
  if (tem == NULL_RTX)
    goto L6648;
  x1 = PATTERN (tem);
  if (GET_CODE (x1) == SET)
    goto L4030;
  x1 = XEXP (x0, 0);
  goto L6648;

 L4030: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == REG
      && XINT (x2, 0) == 24)
    goto L4031;
  x1 = XEXP (x0, 0);
  goto L6648;

 L4031: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == CCmode
      && GET_CODE (x2) == COMPARE)
    goto L4032;
  x1 = XEXP (x0, 0);
  goto L6648;

 L4032: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[1]))
    goto L4033;
  x1 = XEXP (x0, 0);
  goto L6648;

 L4033: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT
      && XWINT (x3, 0) == 0
      && (TARGET_ARM
  ))
    {
      *_pmatch_len = 1;
      tem = gen_peephole2_493 (insn, operands);
      if (tem != 0)
        return tem;
    }
  x1 = XEXP (x0, 0);
  goto L6648;

 L3840: ATTRIBUTE_UNUSED_LABEL
  x1 = XEXP (x0, 1);
  if (const_int_operand (x1, SImode))
    {
      operands[1] = x1;
      goto L3841;
    }
  goto ret0;

 L3841: ATTRIBUTE_UNUSED_LABEL
  tem = peep2_next_insn (1);
  if (tem == NULL_RTX)
    goto ret0;
  x1 = PATTERN (tem);
  if (GET_CODE (x1) == SET)
    goto L3842;
  goto ret0;

 L3842: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 0);
  if (rtx_equal_p (x2, operands[0]))
    goto L3843;
  goto ret0;

 L3843: ATTRIBUTE_UNUSED_LABEL
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode
      && GET_CODE (x2) == PLUS)
    goto L3844;
  goto ret0;

 L3844: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, operands[0]))
    goto L3845;
  goto ret0;

 L3845: ATTRIBUTE_UNUSED_LABEL
  x3 = XEXP (x2, 1);
  if (register_operand (x3, SImode))
    {
      operands[2] = x3;
      goto L3846;
    }
  goto ret0;

 L3846: ATTRIBUTE_UNUSED_LABEL
  if ((TARGET_THUMB
   && REGNO (operands[2]) == STACK_POINTER_REGNUM 
   && (unsigned HOST_WIDE_INT) (INTVAL (operands[1])) < 1024
   && (INTVAL (operands[1]) & 3) == 0))
    {
      *_pmatch_len = 1;
      tem = gen_peephole2_366 (insn, operands);
      if (tem != 0)
        return tem;
    }
  goto ret0;
 ret0:
  return 0;
}

