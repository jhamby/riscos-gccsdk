/* Generated automatically by the program `genpeep'
from the machine description file `md'.  */

#include "config.h"
#include "system.h"
#include "insn-config.h"
#include "rtl.h"
#include "regs.h"
#include "output.h"
#include "real.h"
#include "recog.h"
#include "except.h"

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
  if (GET_CODE (x) != SET) goto L361;
  x = XEXP (pat, 0);
  if (GET_CODE (x) != MEM) goto L361;
  if (GET_MODE (x) != QImode) goto L361;
  x = XEXP (XEXP (pat, 0), 0);
  operands[0] = x;
  if (! s_register_operand (x, SImode)) goto L361;
  x = XEXP (pat, 1);
  operands[2] = x;
  if (! s_register_operand (x, QImode)) goto L361;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L361; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L361;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L361;
  x = XEXP (pat, 0);
  if (!rtx_equal_p (operands[0], x)) goto L361;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != PLUS) goto L361;
  if (GET_MODE (x) != SImode) goto L361;
  x = XEXP (XEXP (pat, 1), 0);
  if (!rtx_equal_p (operands[0], x)) goto L361;
  x = XEXP (XEXP (pat, 1), 1);
  operands[1] = x;
  if (! index_operand (x, SImode)) goto L361;
  PATTERN (ins1) = gen_rtx_PARALLEL (VOIDmode, gen_rtvec_v (3, operands));
  INSN_CODE (ins1) = 361;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L361:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L362;
  x = XEXP (pat, 0);
  operands[0] = x;
  if (! s_register_operand (x, QImode)) goto L362;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != MEM) goto L362;
  if (GET_MODE (x) != QImode) goto L362;
  x = XEXP (XEXP (pat, 1), 0);
  operands[1] = x;
  if (! s_register_operand (x, SImode)) goto L362;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L362; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L362;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L362;
  x = XEXP (pat, 0);
  if (!rtx_equal_p (operands[1], x)) goto L362;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != PLUS) goto L362;
  if (GET_MODE (x) != SImode) goto L362;
  x = XEXP (XEXP (pat, 1), 0);
  if (!rtx_equal_p (operands[1], x)) goto L362;
  x = XEXP (XEXP (pat, 1), 1);
  operands[2] = x;
  if (! index_operand (x, SImode)) goto L362;
  if (! (REGNO(operands[0]) != REGNO(operands[1])
   && (GET_CODE (operands[2]) != REG
       || REGNO(operands[0]) != REGNO (operands[2])))) goto L362;
  PATTERN (ins1) = gen_rtx_PARALLEL (VOIDmode, gen_rtvec_v (3, operands));
  INSN_CODE (ins1) = 362;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L362:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L363;
  x = XEXP (pat, 0);
  if (GET_CODE (x) != MEM) goto L363;
  if (GET_MODE (x) != SImode) goto L363;
  x = XEXP (XEXP (pat, 0), 0);
  operands[0] = x;
  if (! s_register_operand (x, SImode)) goto L363;
  x = XEXP (pat, 1);
  operands[2] = x;
  if (! s_register_operand (x, SImode)) goto L363;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L363; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L363;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L363;
  x = XEXP (pat, 0);
  if (!rtx_equal_p (operands[0], x)) goto L363;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != PLUS) goto L363;
  if (GET_MODE (x) != SImode) goto L363;
  x = XEXP (XEXP (pat, 1), 0);
  if (!rtx_equal_p (operands[0], x)) goto L363;
  x = XEXP (XEXP (pat, 1), 1);
  operands[1] = x;
  if (! index_operand (x, SImode)) goto L363;
  PATTERN (ins1) = gen_rtx_PARALLEL (VOIDmode, gen_rtvec_v (3, operands));
  INSN_CODE (ins1) = 363;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L363:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L364;
  x = XEXP (pat, 0);
  operands[0] = x;
  if (! s_register_operand (x, HImode)) goto L364;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != MEM) goto L364;
  if (GET_MODE (x) != HImode) goto L364;
  x = XEXP (XEXP (pat, 1), 0);
  operands[1] = x;
  if (! s_register_operand (x, SImode)) goto L364;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L364; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L364;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L364;
  x = XEXP (pat, 0);
  if (!rtx_equal_p (operands[1], x)) goto L364;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != PLUS) goto L364;
  if (GET_MODE (x) != SImode) goto L364;
  x = XEXP (XEXP (pat, 1), 0);
  if (!rtx_equal_p (operands[1], x)) goto L364;
  x = XEXP (XEXP (pat, 1), 1);
  operands[2] = x;
  if (! index_operand (x, SImode)) goto L364;
  if (! ((! BYTES_BIG_ENDIAN)
   && ! TARGET_SHORT_BY_BYTES
   && REGNO(operands[0]) != REGNO(operands[1])
   && (GET_CODE (operands[2]) != REG
       || REGNO(operands[0]) != REGNO (operands[2])))) goto L364;
  PATTERN (ins1) = gen_rtx_PARALLEL (VOIDmode, gen_rtvec_v (3, operands));
  INSN_CODE (ins1) = 364;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L364:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L365;
  x = XEXP (pat, 0);
  operands[0] = x;
  if (! s_register_operand (x, SImode)) goto L365;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != MEM) goto L365;
  if (GET_MODE (x) != SImode) goto L365;
  x = XEXP (XEXP (pat, 1), 0);
  operands[1] = x;
  if (! s_register_operand (x, SImode)) goto L365;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L365; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L365;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L365;
  x = XEXP (pat, 0);
  if (!rtx_equal_p (operands[1], x)) goto L365;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != PLUS) goto L365;
  if (GET_MODE (x) != SImode) goto L365;
  x = XEXP (XEXP (pat, 1), 0);
  if (!rtx_equal_p (operands[1], x)) goto L365;
  x = XEXP (XEXP (pat, 1), 1);
  operands[2] = x;
  if (! index_operand (x, SImode)) goto L365;
  if (! (REGNO(operands[0]) != REGNO(operands[1])
   && (GET_CODE (operands[2]) != REG
       || REGNO(operands[0]) != REGNO (operands[2])))) goto L365;
  PATTERN (ins1) = gen_rtx_PARALLEL (VOIDmode, gen_rtvec_v (3, operands));
  INSN_CODE (ins1) = 365;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L365:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L366;
  x = XEXP (pat, 0);
  if (GET_CODE (x) != MEM) goto L366;
  if (GET_MODE (x) != QImode) goto L366;
  x = XEXP (XEXP (pat, 0), 0);
  if (GET_CODE (x) != PLUS) goto L366;
  if (GET_MODE (x) != SImode) goto L366;
  x = XEXP (XEXP (XEXP (pat, 0), 0), 0);
  operands[0] = x;
  if (! s_register_operand (x, SImode)) goto L366;
  x = XEXP (XEXP (XEXP (pat, 0), 0), 1);
  operands[1] = x;
  if (! index_operand (x, SImode)) goto L366;
  x = XEXP (pat, 1);
  operands[2] = x;
  if (! s_register_operand (x, QImode)) goto L366;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L366; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L366;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L366;
  x = XEXP (pat, 0);
  if (!rtx_equal_p (operands[0], x)) goto L366;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != PLUS) goto L366;
  if (GET_MODE (x) != SImode) goto L366;
  x = XEXP (XEXP (pat, 1), 0);
  if (!rtx_equal_p (operands[0], x)) goto L366;
  x = XEXP (XEXP (pat, 1), 1);
  if (!rtx_equal_p (operands[1], x)) goto L366;
  PATTERN (ins1) = gen_rtx_PARALLEL (VOIDmode, gen_rtvec_v (3, operands));
  INSN_CODE (ins1) = 366;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L366:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L367;
  x = XEXP (pat, 0);
  if (GET_CODE (x) != MEM) goto L367;
  if (GET_MODE (x) != QImode) goto L367;
  x = XEXP (XEXP (pat, 0), 0);
  if (GET_CODE (x) != PLUS) goto L367;
  if (GET_MODE (x) != SImode) goto L367;
  x = XEXP (XEXP (XEXP (pat, 0), 0), 0);
  operands[4] = x;
  if (! shift_operator (x, SImode)) goto L367;
  x = XEXP (XEXP (XEXP (XEXP (pat, 0), 0), 0), 0);
  operands[0] = x;
  if (! s_register_operand (x, SImode)) goto L367;
  x = XEXP (XEXP (XEXP (XEXP (pat, 0), 0), 0), 1);
  operands[1] = x;
  if (! const_int_operand (x, SImode)) goto L367;
  x = XEXP (XEXP (XEXP (pat, 0), 0), 1);
  operands[2] = x;
  if (! s_register_operand (x, SImode)) goto L367;
  x = XEXP (pat, 1);
  operands[3] = x;
  if (! s_register_operand (x, QImode)) goto L367;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L367; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L367;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L367;
  x = XEXP (pat, 0);
  if (!rtx_equal_p (operands[2], x)) goto L367;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != PLUS) goto L367;
  if (GET_MODE (x) != SImode) goto L367;
  x = XEXP (XEXP (pat, 1), 0);
  if (GET_CODE (operands[4]) != GET_CODE (x)
      || GET_MODE (operands[4]) != GET_MODE (x)) goto L367;
  operands[4] = x;
  x = XEXP (XEXP (XEXP (pat, 1), 0), 0);
  if (!rtx_equal_p (operands[0], x)) goto L367;
  x = XEXP (XEXP (XEXP (pat, 1), 0), 1);
  if (!rtx_equal_p (operands[1], x)) goto L367;
  x = XEXP (XEXP (pat, 1), 1);
  if (!rtx_equal_p (operands[2], x)) goto L367;
  PATTERN (ins1) = gen_rtx_PARALLEL (VOIDmode, gen_rtvec_v (5, operands));
  INSN_CODE (ins1) = 367;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L367:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L368;
  x = XEXP (pat, 0);
  operands[0] = x;
  if (! s_register_operand (x, SImode)) goto L368;
  x = XEXP (pat, 1);
  operands[1] = x;
  if (! s_register_operand (x, SImode)) goto L368;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L368; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L368;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L368;
  x = XEXP (pat, 0);
  if (GET_CODE (x) != REG) goto L368;
  if (GET_MODE (x) != CCmode) goto L368;
  if (XINT (x, 0) != 24) goto L368;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != COMPARE) goto L368;
  if (GET_MODE (x) != CCmode) goto L368;
  x = XEXP (XEXP (pat, 1), 0);
  if (!rtx_equal_p (operands[1], x)) goto L368;
  x = XEXP (XEXP (pat, 1), 1);
  if (GET_CODE (x) != CONST_INT) goto L368;
  if (XWINT (x, 0) != 0) goto L368;
  PATTERN (ins1) = gen_rtx_PARALLEL (VOIDmode, gen_rtvec_v (2, operands));
  INSN_CODE (ins1) = 368;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L368:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L369;
  x = XEXP (pat, 0);
  operands[0] = x;
  if (! s_register_operand (x, SImode)) goto L369;
  x = XEXP (pat, 1);
  operands[4] = x;
  if (! memory_operand (x, SImode)) goto L369;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L369; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L369;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L369;
  x = XEXP (pat, 0);
  operands[1] = x;
  if (! s_register_operand (x, SImode)) goto L369;
  x = XEXP (pat, 1);
  operands[5] = x;
  if (! memory_operand (x, SImode)) goto L369;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L369; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L369;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L369;
  x = XEXP (pat, 0);
  operands[2] = x;
  if (! s_register_operand (x, SImode)) goto L369;
  x = XEXP (pat, 1);
  operands[6] = x;
  if (! memory_operand (x, SImode)) goto L369;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L369; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L369;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L369;
  x = XEXP (pat, 0);
  operands[3] = x;
  if (! s_register_operand (x, SImode)) goto L369;
  x = XEXP (pat, 1);
  operands[7] = x;
  if (! memory_operand (x, SImode)) goto L369;
  if (! (load_multiple_sequence (operands, 4, NULL, NULL, NULL))) goto L369;
  PATTERN (ins1) = gen_rtx_PARALLEL (VOIDmode, gen_rtvec_v (8, operands));
  INSN_CODE (ins1) = 369;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L369:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L370;
  x = XEXP (pat, 0);
  operands[0] = x;
  if (! s_register_operand (x, SImode)) goto L370;
  x = XEXP (pat, 1);
  operands[3] = x;
  if (! memory_operand (x, SImode)) goto L370;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L370; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L370;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L370;
  x = XEXP (pat, 0);
  operands[1] = x;
  if (! s_register_operand (x, SImode)) goto L370;
  x = XEXP (pat, 1);
  operands[4] = x;
  if (! memory_operand (x, SImode)) goto L370;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L370; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L370;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L370;
  x = XEXP (pat, 0);
  operands[2] = x;
  if (! s_register_operand (x, SImode)) goto L370;
  x = XEXP (pat, 1);
  operands[5] = x;
  if (! memory_operand (x, SImode)) goto L370;
  if (! (load_multiple_sequence (operands, 3, NULL, NULL, NULL))) goto L370;
  PATTERN (ins1) = gen_rtx_PARALLEL (VOIDmode, gen_rtvec_v (6, operands));
  INSN_CODE (ins1) = 370;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L370:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L371;
  x = XEXP (pat, 0);
  operands[0] = x;
  if (! s_register_operand (x, SImode)) goto L371;
  x = XEXP (pat, 1);
  operands[2] = x;
  if (! memory_operand (x, SImode)) goto L371;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L371; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L371;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L371;
  x = XEXP (pat, 0);
  operands[1] = x;
  if (! s_register_operand (x, SImode)) goto L371;
  x = XEXP (pat, 1);
  operands[3] = x;
  if (! memory_operand (x, SImode)) goto L371;
  if (! (load_multiple_sequence (operands, 2, NULL, NULL, NULL))) goto L371;
  PATTERN (ins1) = gen_rtx_PARALLEL (VOIDmode, gen_rtvec_v (4, operands));
  INSN_CODE (ins1) = 371;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L371:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L372;
  x = XEXP (pat, 0);
  operands[4] = x;
  if (! memory_operand (x, SImode)) goto L372;
  x = XEXP (pat, 1);
  operands[0] = x;
  if (! s_register_operand (x, SImode)) goto L372;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L372; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L372;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L372;
  x = XEXP (pat, 0);
  operands[5] = x;
  if (! memory_operand (x, SImode)) goto L372;
  x = XEXP (pat, 1);
  operands[1] = x;
  if (! s_register_operand (x, SImode)) goto L372;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L372; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L372;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L372;
  x = XEXP (pat, 0);
  operands[6] = x;
  if (! memory_operand (x, SImode)) goto L372;
  x = XEXP (pat, 1);
  operands[2] = x;
  if (! s_register_operand (x, SImode)) goto L372;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L372; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L372;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L372;
  x = XEXP (pat, 0);
  operands[7] = x;
  if (! memory_operand (x, SImode)) goto L372;
  x = XEXP (pat, 1);
  operands[3] = x;
  if (! s_register_operand (x, SImode)) goto L372;
  if (! (store_multiple_sequence (operands, 4, NULL, NULL, NULL))) goto L372;
  PATTERN (ins1) = gen_rtx_PARALLEL (VOIDmode, gen_rtvec_v (8, operands));
  INSN_CODE (ins1) = 372;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L372:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L373;
  x = XEXP (pat, 0);
  operands[3] = x;
  if (! memory_operand (x, SImode)) goto L373;
  x = XEXP (pat, 1);
  operands[0] = x;
  if (! s_register_operand (x, SImode)) goto L373;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L373; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L373;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L373;
  x = XEXP (pat, 0);
  operands[4] = x;
  if (! memory_operand (x, SImode)) goto L373;
  x = XEXP (pat, 1);
  operands[1] = x;
  if (! s_register_operand (x, SImode)) goto L373;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L373; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L373;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L373;
  x = XEXP (pat, 0);
  operands[5] = x;
  if (! memory_operand (x, SImode)) goto L373;
  x = XEXP (pat, 1);
  operands[2] = x;
  if (! s_register_operand (x, SImode)) goto L373;
  if (! (store_multiple_sequence (operands, 3, NULL, NULL, NULL))) goto L373;
  PATTERN (ins1) = gen_rtx_PARALLEL (VOIDmode, gen_rtvec_v (6, operands));
  INSN_CODE (ins1) = 373;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L373:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L374;
  x = XEXP (pat, 0);
  operands[2] = x;
  if (! memory_operand (x, SImode)) goto L374;
  x = XEXP (pat, 1);
  operands[0] = x;
  if (! s_register_operand (x, SImode)) goto L374;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L374; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L374;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L374;
  x = XEXP (pat, 0);
  operands[3] = x;
  if (! memory_operand (x, SImode)) goto L374;
  x = XEXP (pat, 1);
  operands[1] = x;
  if (! s_register_operand (x, SImode)) goto L374;
  if (! (store_multiple_sequence (operands, 2, NULL, NULL, NULL))) goto L374;
  PATTERN (ins1) = gen_rtx_PARALLEL (VOIDmode, gen_rtvec_v (4, operands));
  INSN_CODE (ins1) = 374;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L374:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != PARALLEL) goto L375;
  if (XVECLEN (x, 0) != 2) goto L375;
  x = XVECEXP (pat, 0, 0);
  if (GET_CODE (x) != CALL) goto L375;
  x = XEXP (XVECEXP (pat, 0, 0), 0);
  if (GET_CODE (x) != MEM) goto L375;
  if (GET_MODE (x) != SImode) goto L375;
  x = XEXP (XEXP (XVECEXP (pat, 0, 0), 0), 0);
  operands[0] = x;
  x = XEXP (XVECEXP (pat, 0, 0), 1);
  operands[1] = x;
  if (! general_operand (x, SImode)) goto L375;
  x = XVECEXP (pat, 0, 1);
  if (GET_CODE (x) != CLOBBER) goto L375;
  x = XEXP (XVECEXP (pat, 0, 1), 0);
  if (GET_CODE (x) != REG) goto L375;
  if (GET_MODE (x) != SImode) goto L375;
  if (XINT (x, 0) != 14) goto L375;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L375; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L375;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != RETURN) goto L375;
  if (! ((GET_CODE (operands[0]) == SYMBOL_REF && USE_RETURN_INSN (FALSE)
    && !get_frame_size () && !current_function_calls_alloca
    && !current_function_outgoing_args_size && !current_function_args_size))) goto L375;
  PATTERN (ins1) = gen_rtx_PARALLEL (VOIDmode, gen_rtvec_v (2, operands));
  INSN_CODE (ins1) = 375;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L375:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != PARALLEL) goto L376;
  if (XVECLEN (x, 0) != 2) goto L376;
  x = XVECEXP (pat, 0, 0);
  if (GET_CODE (x) != SET) goto L376;
  x = XEXP (XVECEXP (pat, 0, 0), 0);
  operands[0] = x;
  if (! s_register_operand (x, VOIDmode)) goto L376;
  x = XEXP (XVECEXP (pat, 0, 0), 1);
  if (GET_CODE (x) != CALL) goto L376;
  x = XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0);
  if (GET_CODE (x) != MEM) goto L376;
  if (GET_MODE (x) != SImode) goto L376;
  x = XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0);
  operands[1] = x;
  x = XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1);
  operands[2] = x;
  if (! general_operand (x, SImode)) goto L376;
  x = XVECEXP (pat, 0, 1);
  if (GET_CODE (x) != CLOBBER) goto L376;
  x = XEXP (XVECEXP (pat, 0, 1), 0);
  if (GET_CODE (x) != REG) goto L376;
  if (GET_MODE (x) != SImode) goto L376;
  if (XINT (x, 0) != 14) goto L376;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L376; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L376;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != RETURN) goto L376;
  if (! ((GET_CODE (operands[1]) == SYMBOL_REF && USE_RETURN_INSN (FALSE)
    && !get_frame_size () && !current_function_calls_alloca
    && !current_function_outgoing_args_size && !current_function_args_size))) goto L376;
  PATTERN (ins1) = gen_rtx_PARALLEL (VOIDmode, gen_rtvec_v (3, operands));
  INSN_CODE (ins1) = 376;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L376:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != PARALLEL) goto L377;
  if (XVECLEN (x, 0) != 2) goto L377;
  x = XVECEXP (pat, 0, 0);
  if (GET_CODE (x) != SET) goto L377;
  x = XEXP (XVECEXP (pat, 0, 0), 0);
  operands[0] = x;
  if (! s_register_operand (x, VOIDmode)) goto L377;
  x = XEXP (XVECEXP (pat, 0, 0), 1);
  if (GET_CODE (x) != CALL) goto L377;
  x = XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0);
  if (GET_CODE (x) != MEM) goto L377;
  if (GET_MODE (x) != SImode) goto L377;
  x = XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0);
  operands[1] = x;
  x = XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1);
  operands[2] = x;
  if (! general_operand (x, SImode)) goto L377;
  x = XVECEXP (pat, 0, 1);
  if (GET_CODE (x) != CLOBBER) goto L377;
  x = XEXP (XVECEXP (pat, 0, 1), 0);
  if (GET_CODE (x) != REG) goto L377;
  if (GET_MODE (x) != SImode) goto L377;
  if (XINT (x, 0) != 14) goto L377;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L377; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L377;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != USE) goto L377;
  x = XEXP (pat, 0);
  if (!rtx_equal_p (operands[0], x)) goto L377;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L377; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L377;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != RETURN) goto L377;
  if (! ((GET_CODE (operands[1]) == SYMBOL_REF && USE_RETURN_INSN (FALSE)
    && !get_frame_size () && !current_function_calls_alloca
    && !current_function_outgoing_args_size && !current_function_args_size))) goto L377;
  PATTERN (ins1) = gen_rtx_PARALLEL (VOIDmode, gen_rtvec_v (3, operands));
  INSN_CODE (ins1) = 377;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L377:

  return 0;
}

rtx peep_operand[8];
