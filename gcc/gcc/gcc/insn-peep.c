/* Generated automatically by the program `genpeep'
from the machine description file `md'.  */

#include "config.h"
#include "system.h"
#include "insn-config.h"
#include "rtl.h"
#include "tm_p.h"
#include "regs.h"
#include "output.h"
#include "real.h"
#include "recog.h"
#include "except.h"

#include "function.h"

#ifdef HAVE_peephole
extern rtx peep_operand[];

#define operands peep_operand

rtx
peephole (ins1)
     rtx ins1;
{
  rtx insn ATTRIBUTE_UNUSED, x ATTRIBUTE_UNUSED, pat ATTRIBUTE_UNUSED;

  if (NEXT_INSN (ins1)
      && GET_CODE (NEXT_INSN (ins1)) == BARRIER)
    return 0;

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L486;
  x = XEXP (pat, 0);
  if (GET_CODE (x) != MEM) goto L486;
  if (GET_MODE (x) != QImode) goto L486;
  x = XEXP (XEXP (pat, 0), 0);
  operands[0] = x;
  if (! s_register_operand (x, SImode)) goto L486;
  x = XEXP (pat, 1);
  operands[2] = x;
  if (! s_register_operand (x, QImode)) goto L486;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L486; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L486;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L486;
  x = XEXP (pat, 0);
  if (!rtx_equal_p (operands[0], x)) goto L486;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != PLUS) goto L486;
  if (GET_MODE (x) != SImode) goto L486;
  x = XEXP (XEXP (pat, 1), 0);
  if (!rtx_equal_p (operands[0], x)) goto L486;
  x = XEXP (XEXP (pat, 1), 1);
  operands[1] = x;
  if (! index_operand (x, SImode)) goto L486;
  if (! (TARGET_ARM
   && (REGNO (operands[2]) != REGNO (operands[0]))
   && (GET_CODE (operands[1]) != REG
       || (REGNO (operands[1]) != REGNO (operands[0]))))) goto L486;
  PATTERN (ins1) = gen_rtx_PARALLEL (VOIDmode, gen_rtvec_v (3, operands));
  INSN_CODE (ins1) = 486;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L486:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L487;
  x = XEXP (pat, 0);
  operands[0] = x;
  if (! s_register_operand (x, QImode)) goto L487;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != MEM) goto L487;
  if (GET_MODE (x) != QImode) goto L487;
  x = XEXP (XEXP (pat, 1), 0);
  operands[1] = x;
  if (! s_register_operand (x, SImode)) goto L487;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L487; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L487;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L487;
  x = XEXP (pat, 0);
  if (!rtx_equal_p (operands[1], x)) goto L487;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != PLUS) goto L487;
  if (GET_MODE (x) != SImode) goto L487;
  x = XEXP (XEXP (pat, 1), 0);
  if (!rtx_equal_p (operands[1], x)) goto L487;
  x = XEXP (XEXP (pat, 1), 1);
  operands[2] = x;
  if (! index_operand (x, SImode)) goto L487;
  if (! (TARGET_ARM
   && REGNO (operands[0]) != REGNO(operands[1])
   && (GET_CODE (operands[2]) != REG
       || REGNO(operands[0]) != REGNO (operands[2])))) goto L487;
  PATTERN (ins1) = gen_rtx_PARALLEL (VOIDmode, gen_rtvec_v (3, operands));
  INSN_CODE (ins1) = 487;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L487:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L488;
  x = XEXP (pat, 0);
  if (GET_CODE (x) != MEM) goto L488;
  if (GET_MODE (x) != SImode) goto L488;
  x = XEXP (XEXP (pat, 0), 0);
  operands[0] = x;
  if (! s_register_operand (x, SImode)) goto L488;
  x = XEXP (pat, 1);
  operands[2] = x;
  if (! s_register_operand (x, SImode)) goto L488;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L488; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L488;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L488;
  x = XEXP (pat, 0);
  if (!rtx_equal_p (operands[0], x)) goto L488;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != PLUS) goto L488;
  if (GET_MODE (x) != SImode) goto L488;
  x = XEXP (XEXP (pat, 1), 0);
  if (!rtx_equal_p (operands[0], x)) goto L488;
  x = XEXP (XEXP (pat, 1), 1);
  operands[1] = x;
  if (! index_operand (x, SImode)) goto L488;
  if (! (TARGET_ARM
   && (REGNO (operands[2]) != REGNO (operands[0]))
   && (GET_CODE (operands[1]) != REG
       || (REGNO (operands[1]) != REGNO (operands[0]))))) goto L488;
  PATTERN (ins1) = gen_rtx_PARALLEL (VOIDmode, gen_rtvec_v (3, operands));
  INSN_CODE (ins1) = 488;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L488:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L489;
  x = XEXP (pat, 0);
  operands[0] = x;
  if (! s_register_operand (x, HImode)) goto L489;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != MEM) goto L489;
  if (GET_MODE (x) != HImode) goto L489;
  x = XEXP (XEXP (pat, 1), 0);
  operands[1] = x;
  if (! s_register_operand (x, SImode)) goto L489;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L489; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L489;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L489;
  x = XEXP (pat, 0);
  if (!rtx_equal_p (operands[1], x)) goto L489;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != PLUS) goto L489;
  if (GET_MODE (x) != SImode) goto L489;
  x = XEXP (XEXP (pat, 1), 0);
  if (!rtx_equal_p (operands[1], x)) goto L489;
  x = XEXP (XEXP (pat, 1), 1);
  operands[2] = x;
  if (! index_operand (x, SImode)) goto L489;
  if (! (TARGET_ARM
   && !BYTES_BIG_ENDIAN
   && !TARGET_MMU_TRAPS
   && REGNO (operands[0]) != REGNO(operands[1])
   && (GET_CODE (operands[2]) != REG
       || REGNO(operands[0]) != REGNO (operands[2])))) goto L489;
  PATTERN (ins1) = gen_rtx_PARALLEL (VOIDmode, gen_rtvec_v (3, operands));
  INSN_CODE (ins1) = 489;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L489:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L490;
  x = XEXP (pat, 0);
  operands[0] = x;
  if (! s_register_operand (x, SImode)) goto L490;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != MEM) goto L490;
  if (GET_MODE (x) != SImode) goto L490;
  x = XEXP (XEXP (pat, 1), 0);
  operands[1] = x;
  if (! s_register_operand (x, SImode)) goto L490;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L490; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L490;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L490;
  x = XEXP (pat, 0);
  if (!rtx_equal_p (operands[1], x)) goto L490;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != PLUS) goto L490;
  if (GET_MODE (x) != SImode) goto L490;
  x = XEXP (XEXP (pat, 1), 0);
  if (!rtx_equal_p (operands[1], x)) goto L490;
  x = XEXP (XEXP (pat, 1), 1);
  operands[2] = x;
  if (! index_operand (x, SImode)) goto L490;
  if (! (TARGET_ARM
   && REGNO (operands[0]) != REGNO(operands[1])
   && (GET_CODE (operands[2]) != REG
       || REGNO(operands[0]) != REGNO (operands[2])))) goto L490;
  PATTERN (ins1) = gen_rtx_PARALLEL (VOIDmode, gen_rtvec_v (3, operands));
  INSN_CODE (ins1) = 490;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L490:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L491;
  x = XEXP (pat, 0);
  if (GET_CODE (x) != MEM) goto L491;
  if (GET_MODE (x) != QImode) goto L491;
  x = XEXP (XEXP (pat, 0), 0);
  if (GET_CODE (x) != PLUS) goto L491;
  if (GET_MODE (x) != SImode) goto L491;
  x = XEXP (XEXP (XEXP (pat, 0), 0), 0);
  operands[0] = x;
  if (! s_register_operand (x, SImode)) goto L491;
  x = XEXP (XEXP (XEXP (pat, 0), 0), 1);
  operands[1] = x;
  if (! index_operand (x, SImode)) goto L491;
  x = XEXP (pat, 1);
  operands[2] = x;
  if (! s_register_operand (x, QImode)) goto L491;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L491; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L491;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L491;
  x = XEXP (pat, 0);
  if (!rtx_equal_p (operands[0], x)) goto L491;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != PLUS) goto L491;
  if (GET_MODE (x) != SImode) goto L491;
  x = XEXP (XEXP (pat, 1), 0);
  if (!rtx_equal_p (operands[0], x)) goto L491;
  x = XEXP (XEXP (pat, 1), 1);
  if (!rtx_equal_p (operands[1], x)) goto L491;
  if (! (TARGET_ARM
   && (REGNO (operands[2]) != REGNO (operands[0]))
   && (GET_CODE (operands[1]) != REG
       || (REGNO (operands[1]) != REGNO (operands[0]))))) goto L491;
  PATTERN (ins1) = gen_rtx_PARALLEL (VOIDmode, gen_rtvec_v (3, operands));
  INSN_CODE (ins1) = 491;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L491:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L492;
  x = XEXP (pat, 0);
  if (GET_CODE (x) != MEM) goto L492;
  if (GET_MODE (x) != QImode) goto L492;
  x = XEXP (XEXP (pat, 0), 0);
  if (GET_CODE (x) != PLUS) goto L492;
  if (GET_MODE (x) != SImode) goto L492;
  x = XEXP (XEXP (XEXP (pat, 0), 0), 0);
  operands[4] = x;
  if (! shift_operator (x, SImode)) goto L492;
  x = XEXP (XEXP (XEXP (XEXP (pat, 0), 0), 0), 0);
  operands[0] = x;
  if (! s_register_operand (x, SImode)) goto L492;
  x = XEXP (XEXP (XEXP (XEXP (pat, 0), 0), 0), 1);
  operands[1] = x;
  if (! const_int_operand (x, SImode)) goto L492;
  x = XEXP (XEXP (XEXP (pat, 0), 0), 1);
  operands[2] = x;
  if (! s_register_operand (x, SImode)) goto L492;
  x = XEXP (pat, 1);
  operands[3] = x;
  if (! s_register_operand (x, QImode)) goto L492;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L492; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L492;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L492;
  x = XEXP (pat, 0);
  if (!rtx_equal_p (operands[2], x)) goto L492;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != PLUS) goto L492;
  if (GET_MODE (x) != SImode) goto L492;
  x = XEXP (XEXP (pat, 1), 0);
  if (GET_CODE (operands[4]) != GET_CODE (x)
      || GET_MODE (operands[4]) != GET_MODE (x)) goto L492;
  operands[4] = x;
  x = XEXP (XEXP (XEXP (pat, 1), 0), 0);
  if (!rtx_equal_p (operands[0], x)) goto L492;
  x = XEXP (XEXP (XEXP (pat, 1), 0), 1);
  if (!rtx_equal_p (operands[1], x)) goto L492;
  x = XEXP (XEXP (pat, 1), 1);
  if (!rtx_equal_p (operands[2], x)) goto L492;
  if (! (TARGET_ARM
   && (REGNO (operands[3]) != REGNO (operands[2]))
   && (REGNO (operands[0]) != REGNO (operands[2])))) goto L492;
  PATTERN (ins1) = gen_rtx_PARALLEL (VOIDmode, gen_rtvec_v (5, operands));
  INSN_CODE (ins1) = 492;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L492:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L494;
  x = XEXP (pat, 0);
  operands[0] = x;
  if (! s_register_operand (x, SImode)) goto L494;
  x = XEXP (pat, 1);
  operands[4] = x;
  if (! memory_operand (x, SImode)) goto L494;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L494; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L494;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L494;
  x = XEXP (pat, 0);
  operands[1] = x;
  if (! s_register_operand (x, SImode)) goto L494;
  x = XEXP (pat, 1);
  operands[5] = x;
  if (! memory_operand (x, SImode)) goto L494;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L494; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L494;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L494;
  x = XEXP (pat, 0);
  operands[2] = x;
  if (! s_register_operand (x, SImode)) goto L494;
  x = XEXP (pat, 1);
  operands[6] = x;
  if (! memory_operand (x, SImode)) goto L494;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L494; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L494;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L494;
  x = XEXP (pat, 0);
  operands[3] = x;
  if (! s_register_operand (x, SImode)) goto L494;
  x = XEXP (pat, 1);
  operands[7] = x;
  if (! memory_operand (x, SImode)) goto L494;
  if (! (TARGET_ARM && load_multiple_sequence (operands, 4, NULL, NULL, NULL))) goto L494;
  PATTERN (ins1) = gen_rtx_PARALLEL (VOIDmode, gen_rtvec_v (8, operands));
  INSN_CODE (ins1) = 494;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L494:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L495;
  x = XEXP (pat, 0);
  operands[0] = x;
  if (! s_register_operand (x, SImode)) goto L495;
  x = XEXP (pat, 1);
  operands[3] = x;
  if (! memory_operand (x, SImode)) goto L495;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L495; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L495;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L495;
  x = XEXP (pat, 0);
  operands[1] = x;
  if (! s_register_operand (x, SImode)) goto L495;
  x = XEXP (pat, 1);
  operands[4] = x;
  if (! memory_operand (x, SImode)) goto L495;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L495; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L495;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L495;
  x = XEXP (pat, 0);
  operands[2] = x;
  if (! s_register_operand (x, SImode)) goto L495;
  x = XEXP (pat, 1);
  operands[5] = x;
  if (! memory_operand (x, SImode)) goto L495;
  if (! (TARGET_ARM && load_multiple_sequence (operands, 3, NULL, NULL, NULL))) goto L495;
  PATTERN (ins1) = gen_rtx_PARALLEL (VOIDmode, gen_rtvec_v (6, operands));
  INSN_CODE (ins1) = 495;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L495:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L496;
  x = XEXP (pat, 0);
  operands[0] = x;
  if (! s_register_operand (x, SImode)) goto L496;
  x = XEXP (pat, 1);
  operands[2] = x;
  if (! memory_operand (x, SImode)) goto L496;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L496; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L496;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L496;
  x = XEXP (pat, 0);
  operands[1] = x;
  if (! s_register_operand (x, SImode)) goto L496;
  x = XEXP (pat, 1);
  operands[3] = x;
  if (! memory_operand (x, SImode)) goto L496;
  if (! (TARGET_ARM && load_multiple_sequence (operands, 2, NULL, NULL, NULL))) goto L496;
  PATTERN (ins1) = gen_rtx_PARALLEL (VOIDmode, gen_rtvec_v (4, operands));
  INSN_CODE (ins1) = 496;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L496:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L497;
  x = XEXP (pat, 0);
  operands[4] = x;
  if (! memory_operand (x, SImode)) goto L497;
  x = XEXP (pat, 1);
  operands[0] = x;
  if (! s_register_operand (x, SImode)) goto L497;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L497; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L497;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L497;
  x = XEXP (pat, 0);
  operands[5] = x;
  if (! memory_operand (x, SImode)) goto L497;
  x = XEXP (pat, 1);
  operands[1] = x;
  if (! s_register_operand (x, SImode)) goto L497;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L497; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L497;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L497;
  x = XEXP (pat, 0);
  operands[6] = x;
  if (! memory_operand (x, SImode)) goto L497;
  x = XEXP (pat, 1);
  operands[2] = x;
  if (! s_register_operand (x, SImode)) goto L497;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L497; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L497;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L497;
  x = XEXP (pat, 0);
  operands[7] = x;
  if (! memory_operand (x, SImode)) goto L497;
  x = XEXP (pat, 1);
  operands[3] = x;
  if (! s_register_operand (x, SImode)) goto L497;
  if (! (TARGET_ARM && store_multiple_sequence (operands, 4, NULL, NULL, NULL))) goto L497;
  PATTERN (ins1) = gen_rtx_PARALLEL (VOIDmode, gen_rtvec_v (8, operands));
  INSN_CODE (ins1) = 497;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L497:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L498;
  x = XEXP (pat, 0);
  operands[3] = x;
  if (! memory_operand (x, SImode)) goto L498;
  x = XEXP (pat, 1);
  operands[0] = x;
  if (! s_register_operand (x, SImode)) goto L498;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L498; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L498;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L498;
  x = XEXP (pat, 0);
  operands[4] = x;
  if (! memory_operand (x, SImode)) goto L498;
  x = XEXP (pat, 1);
  operands[1] = x;
  if (! s_register_operand (x, SImode)) goto L498;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L498; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L498;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L498;
  x = XEXP (pat, 0);
  operands[5] = x;
  if (! memory_operand (x, SImode)) goto L498;
  x = XEXP (pat, 1);
  operands[2] = x;
  if (! s_register_operand (x, SImode)) goto L498;
  if (! (TARGET_ARM && store_multiple_sequence (operands, 3, NULL, NULL, NULL))) goto L498;
  PATTERN (ins1) = gen_rtx_PARALLEL (VOIDmode, gen_rtvec_v (6, operands));
  INSN_CODE (ins1) = 498;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L498:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L499;
  x = XEXP (pat, 0);
  operands[2] = x;
  if (! memory_operand (x, SImode)) goto L499;
  x = XEXP (pat, 1);
  operands[0] = x;
  if (! s_register_operand (x, SImode)) goto L499;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L499; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L499;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L499;
  x = XEXP (pat, 0);
  operands[3] = x;
  if (! memory_operand (x, SImode)) goto L499;
  x = XEXP (pat, 1);
  operands[1] = x;
  if (! s_register_operand (x, SImode)) goto L499;
  if (! (TARGET_ARM && store_multiple_sequence (operands, 2, NULL, NULL, NULL))) goto L499;
  PATTERN (ins1) = gen_rtx_PARALLEL (VOIDmode, gen_rtvec_v (4, operands));
  INSN_CODE (ins1) = 499;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L499:

  return 0;
}

rtx peep_operand[8];
#endif
