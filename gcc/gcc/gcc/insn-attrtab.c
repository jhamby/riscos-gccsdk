/* Generated automatically by the program `genattrtab'
from the machine description file `md'.  */

#include "config.h"
#include "system.h"
#include "rtl.h"
#include "tm_p.h"
#include "insn-config.h"
#include "recog.h"
#include "regs.h"
#include "real.h"
#include "output.h"
#include "insn-attr.h"
#include "toplev.h"
#include "flags.h"

#define operands recog_data.operand

extern int insn_current_length PARAMS ((rtx));
int
insn_current_length (insn)
     rtx insn;
{
  switch (recog_memoized (insn))
    {
    case 246:
      extract_insn_cached (insn);
      if ((((INSN_ADDRESSES_SET_P () ? INSN_ADDRESSES (INSN_UID (GET_CODE (operands[0]) == LABEL_REF ? XEXP (operands[0], 0) : operands[0])) : 0) - (insn_current_reference_address (insn))) >= (-2048)) && (((INSN_ADDRESSES_SET_P () ? INSN_ADDRESSES (INSN_UID (GET_CODE (operands[0]) == LABEL_REF ? XEXP (operands[0], 0) : operands[0])) : 0) - (insn_current_reference_address (insn))) <= (2044)))
        {
	  return 2;
        }
      else
        {
	  return 4;
        }

    case 216:
      extract_insn_cached (insn);
      if ((((INSN_ADDRESSES_SET_P () ? INSN_ADDRESSES (INSN_UID (GET_CODE (operands[3]) == LABEL_REF ? XEXP (operands[3], 0) : operands[3])) : 0) - (insn_current_reference_address (insn))) >= (-250)) && (((INSN_ADDRESSES_SET_P () ? INSN_ADDRESSES (INSN_UID (GET_CODE (operands[3]) == LABEL_REF ? XEXP (operands[3], 0) : operands[3])) : 0) - (insn_current_reference_address (insn))) <= (256)))
        {
	  return 4;
        }
      else
        {
	  if ((((INSN_ADDRESSES_SET_P () ? INSN_ADDRESSES (INSN_UID (GET_CODE (operands[3]) == LABEL_REF ? XEXP (operands[3], 0) : operands[3])) : 0) - (insn_current_reference_address (insn))) >= (-2040)) && (((INSN_ADDRESSES_SET_P () ? INSN_ADDRESSES (INSN_UID (GET_CODE (operands[3]) == LABEL_REF ? XEXP (operands[3], 0) : operands[3])) : 0) - (insn_current_reference_address (insn))) <= (2048)))
	    {
	      return 6;
	    }
	  else
	    {
	      return 8;
	    }
        }

    case 215:
      extract_insn_cached (insn);
      if ((((INSN_ADDRESSES_SET_P () ? INSN_ADDRESSES (INSN_UID (GET_CODE (operands[3]) == LABEL_REF ? XEXP (operands[3], 0) : operands[3])) : 0) - (insn_current_reference_address (insn))) >= (-250)) && (((INSN_ADDRESSES_SET_P () ? INSN_ADDRESSES (INSN_UID (GET_CODE (operands[3]) == LABEL_REF ? XEXP (operands[3], 0) : operands[3])) : 0) - (insn_current_reference_address (insn))) <= (256)))
        {
	  return 4;
        }
      else
        {
	  if ((((INSN_ADDRESSES_SET_P () ? INSN_ADDRESSES (INSN_UID (GET_CODE (operands[3]) == LABEL_REF ? XEXP (operands[3], 0) : operands[3])) : 0) - (insn_current_reference_address (insn))) >= (-2040)) && (((INSN_ADDRESSES_SET_P () ? INSN_ADDRESSES (INSN_UID (GET_CODE (operands[3]) == LABEL_REF ? XEXP (operands[3], 0) : operands[3])) : 0) - (insn_current_reference_address (insn))) <= (2048)))
	    {
	      return 6;
	    }
	  else
	    {
	      return 8;
	    }
        }

    case -1:
      if (GET_CODE (PATTERN (insn)) != ASM_INPUT
          && asm_noperands (PATTERN (insn)) < 0)
        fatal_insn_not_found (insn);
    default:
      return 0;

    }
}

extern int insn_variable_length_p PARAMS ((rtx));
int
insn_variable_length_p (insn)
     rtx insn;
{
  switch (recog_memoized (insn))
    {
    case 246:
    case 216:
    case 215:
      return 1;

    case -1:
      if (GET_CODE (PATTERN (insn)) != ASM_INPUT
          && asm_noperands (PATTERN (insn)) < 0)
        fatal_insn_not_found (insn);
    default:
      return 0;

    }
}

extern int insn_default_length PARAMS ((rtx));
int
insn_default_length (insn)
     rtx insn;
{
  switch (recog_memoized (insn))
    {
    case 549:
    case 510:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  return 4;
        }
      else
        {
	  return 16 /* 0x10 */;
        }

    case 295:
    case 293:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  return 4;
        }
      else if (which_alternative == 2)
        {
	  return 8;
        }
      else
        {
	  return 8;
        }

    case 291:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  return 8;
        }
      else
        {
	  return 12 /* 0xc */;
        }

    case 244:
    case 242:
    case 241:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || ((which_alternative == 1) || ((which_alternative == 2) || (which_alternative == 3))))
        {
	  return 4;
        }
      else if ((which_alternative == 4) || ((which_alternative == 5) || (which_alternative == 6)))
        {
	  return 8;
        }
      else
        {
	  return 8;
        }

    case 614:
    case 200:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || ((which_alternative == 1) || ((which_alternative == 2) || (which_alternative == 3))))
        {
	  return 4;
        }
      else if ((which_alternative == 4) || (which_alternative == 5))
        {
	  return 8;
        }
      else
        {
	  return 12 /* 0xc */;
        }

    case 199:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  return 4;
        }
      else if ((which_alternative == 1) || (which_alternative == 2))
        {
	  return 2;
        }
      else if (which_alternative == 3)
        {
	  return 6;
        }
      else if (which_alternative == 4)
        {
	  return 4;
        }
      else
        {
	  return 4;
        }

    case 613:
    case 197:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  return 4;
        }
      else if ((which_alternative == 2) || ((which_alternative == 3) || (which_alternative == 4)))
        {
	  return 8;
        }
      else if ((which_alternative == 5) || ((which_alternative == 6) || ((which_alternative == 7) || (which_alternative == 8))))
        {
	  return 4;
        }
      else if (which_alternative == 9)
        {
	  return 8;
        }
      else
        {
	  return 8;
        }

    case 611:
    case 194:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || ((which_alternative == 1) || ((which_alternative == 2) || (which_alternative == 3))))
        {
	  return 4;
        }
      else if ((which_alternative == 4) || (which_alternative == 5))
        {
	  return 8;
        }
      else if ((which_alternative == 6) || (which_alternative == 7))
        {
	  return 4;
        }
      else
        {
	  return 4;
        }

    case 607:
    case 283:
    case 188:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  return 4;
        }
      else
        {
	  return 8;
        }

    case 184:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  return 2;
        }
      else if (which_alternative == 1)
        {
	  return 4;
        }
      else if ((which_alternative == 2) || ((which_alternative == 3) || (which_alternative == 4)))
        {
	  return 2;
        }
      else
        {
	  return 2;
        }

    case 176:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  return 2;
        }
      else if ((which_alternative == 2) || (which_alternative == 3))
        {
	  return 4;
        }
      else if ((which_alternative == 4) || ((which_alternative == 5) || ((which_alternative == 6) || (which_alternative == 7))))
        {
	  return 2;
        }
      else
        {
	  return 2;
        }

    case 174:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  return 4;
        }
      else if (which_alternative == 2)
        {
	  return 6;
        }
      else if ((which_alternative == 3) || (which_alternative == 4))
        {
	  return 2;
        }
      else if (which_alternative == 5)
        {
	  return 6;
        }
      else if (which_alternative == 6)
        {
	  return 4;
        }
      else
        {
	  return 4;
        }

    case 112:
    case 111:
    case 110:
    case 109:
      extract_constrain_insn_cached (insn);
      if (which_alternative != 2)
        {
	  return 8;
        }
      else
        {
	  return 12 /* 0xc */;
        }

    case 44:
      extract_constrain_insn_cached (insn);
      if (which_alternative != 2)
        {
	  return 4;
        }
      else
        {
	  return 2;
        }

    case 80:
    case 4:
      extract_constrain_insn_cached (insn);
      if (which_alternative != 2)
        {
	  return 4;
        }
      else
        {
	  return 16 /* 0x10 */;
        }

    case 633:
    case 347:
    case 320:
    case 314:
    case 312:
    case 310:
    case 296:
    case 290:
    case 284:
    case 282:
    case 263:
    case 252:
    case 251:
    case 248:
    case 247:
    case 114:
    case 113:
      return 12 /* 0xc */;

    case 560:
    case 556:
    case 551:
    case 345:
    case 301:
    case 299:
    case 281:
    case 102:
    case 95:
    case 87:
    case 36:
    case 19:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  return 4;
        }
      else
        {
	  return 8;
        }

    case 558:
    case 517:
    case 97:
    case 34:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  return 4;
        }
      else
        {
	  return 16 /* 0x10 */;
        }

    case 356:
    case 266:
    case 196:
    case 193:
    case 181:
    case 162:
    case 145:
    case 128:
    case 118:
    case 117:
    case 116:
    case 115:
    case 105:
    case 98:
    case 90:
    case 81:
    case 33:
    case 5:
      return 2;

    case 344:
    case 343:
      return 44 /* 0x2c */;

    case 319:
    case 317:
    case 309:
    case 307:
    case 305:
    case 303:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  return 4;
        }
      else if (which_alternative == 1)
        {
	  return 8;
        }
      else
        {
	  return 8;
        }

    case 318:
    case 316:
    case 308:
    case 306:
    case 304:
    case 302:
    case 300:
    case 298:
    case 294:
    case 292:
    case 285:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  return 8;
        }
      else
        {
	  return 12 /* 0xc */;
        }

    case 279:
    case 278:
    case 277:
    case 276:
    case 275:
    case 274:
      return 20 /* 0x14 */;

    case 267:
      if (((thumb_code) == (IS_THUMB_YES)))
        {
	  return 2;
        }
      else
        {
	  return 4;
        }

    case 262:
    case 231:
      return 0;

    case 597:
    case 596:
    case 592:
    case 591:
    case 590:
    case 579:
    case 563:
    case 561:
    case 559:
    case 557:
    case 555:
    case 552:
    case 550:
    case 355:
    case 346:
    case 315:
    case 313:
    case 311:
    case 297:
    case 289:
    case 288:
    case 287:
    case 286:
    case 280:
    case 240:
    case 239:
    case 238:
    case 236:
    case 235:
    case 233:
    case 232:
    case 198:
    case 173:
    case 168:
    case 167:
    case 159:
    case 158:
    case 157:
    case 143:
    case 134:
    case 133:
    case 125:
    case 108:
    case 103:
    case 101:
    case 96:
    case 94:
    case 88:
    case 86:
    case 85:
    case 79:
    case 78:
    case 77:
    case 32:
    case 31:
    case 30:
    case 29:
    case 28:
    case 26:
    case 3:
    case 2:
    case 1:
    case 216:
    case 215:
      return 8;

    case 169:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  return 2;
        }
      else
        {
	  return 6;
        }

    case 126:
      return 6;

    case -1:
      if (GET_CODE (PATTERN (insn)) != ASM_INPUT
          && asm_noperands (PATTERN (insn)) < 0)
        fatal_insn_not_found (insn);
    default:
      return 4;

    }
}

extern int result_ready_cost PARAMS ((rtx));
int
result_ready_cost (insn)
     rtx insn;
{
  switch (recog_memoized (insn))
    {
    case 607:
    case 606:
      extract_constrain_insn_cached (insn);
      if (((which_alternative != 0) && (which_alternative != 1)) && ((which_alternative == 0) || (which_alternative == 1)))
        {
	  return 32 /* 0x20 */;
        }
      else if (((which_alternative == 2) && (((arm_ld_sched) == (LDSCHED_YES)))) && (((arm_is_xscale) == (IS_XSCALE_YES))))
        {
	  return 3;
        }
      else if (((which_alternative == 2) && (((arm_ld_sched) == (LDSCHED_YES)))) || ((! (((arm_ld_sched) == (LDSCHED_YES)))) && ((which_alternative != 0) && (which_alternative != 1))))
        {
	  return 2;
        }
      else
        {
	  return 1;
        }

    case 605:
      extract_constrain_insn_cached (insn);
      if (((which_alternative != 0) && (which_alternative != 1)) && ((which_alternative == 0) || (which_alternative == 1)))
        {
	  return 32 /* 0x20 */;
        }
      else if ((((arm_is_6_or_7) == (MODEL_WBUF_YES))) && (which_alternative == 2))
        {
	  return 5;
        }
      else if (((which_alternative == 3) && (((arm_ld_sched) == (LDSCHED_YES)))) && (((arm_is_xscale) == (IS_XSCALE_YES))))
        {
	  return 3;
        }
      else if (((which_alternative == 3) && (((arm_ld_sched) == (LDSCHED_YES)))) || ((! (((arm_ld_sched) == (LDSCHED_YES)))) && ((which_alternative != 0) && (which_alternative != 1))))
        {
	  return 2;
        }
      else
        {
	  return 1;
        }

    case 610:
    case 601:
      extract_constrain_insn_cached (insn);
      if (((which_alternative != 0) && (which_alternative != 1)) && ((which_alternative == 0) || (which_alternative == 1)))
        {
	  return 32 /* 0x20 */;
        }
      else if ((which_alternative == 3) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))
        {
	  return 5;
        }
      else if (((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 2)) && (((arm_is_xscale) == (IS_XSCALE_YES))))
        {
	  return 3;
        }
      else if (((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 2)) || ((! (((arm_ld_sched) == (LDSCHED_YES)))) && ((which_alternative != 0) && (which_alternative != 1))))
        {
	  return 2;
        }
      else
        {
	  return 1;
        }

    case 591:
      extract_constrain_insn_cached (insn);
      if (((which_alternative == 1) && (((arm_ld_sched) == (LDSCHED_YES)))) && (((arm_is_xscale) == (IS_XSCALE_YES))))
        {
	  return 3;
        }
      else if (((which_alternative == 1) && (((arm_ld_sched) == (LDSCHED_YES)))) || ((! (((arm_ld_sched) == (LDSCHED_YES)))) && (which_alternative != 0)))
        {
	  return 2;
        }
      else
        {
	  return 1;
        }

    case 626:
    case 623:
    case 214:
    case 212:
    case 209:
      if (((arm_is_6_or_7) == (MODEL_WBUF_YES)))
        {
	  return 7;
        }
      else
        {
	  return 3;
        }

    case 625:
    case 622:
    case 213:
    case 211:
    case 208:
      if (((arm_is_6_or_7) == (MODEL_WBUF_YES)))
        {
	  return 9;
        }
      else
        {
	  return 4;
        }

    case 624:
    case 621:
    case 348:
    case 210:
    case 207:
      if (((arm_is_6_or_7) == (MODEL_WBUF_YES)))
        {
	  return 11 /* 0xb */;
        }
      else
        {
	  return 5;
        }

    case 614:
    case 200:
      extract_constrain_insn_cached (insn);
      if ((which_alternative != 6) && ((which_alternative != 0) && (which_alternative != 1)))
        {
	  return 32 /* 0x20 */;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 5))
        {
	  return 7;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 4))
        {
	  return 6;
        }
      else if ((((arm_is_6_or_7) == (MODEL_WBUF_YES))) && (which_alternative == 4))
        {
	  return 5;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 3))
        {
	  return 4;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 2))
        {
	  return 3;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && ((which_alternative == 0) || (which_alternative == 1)))
        {
	  return 2;
        }
      else
        {
	  return 1;
        }

    case 199:
      extract_constrain_insn_cached (insn);
      if (((which_alternative == 1) || ((which_alternative == 2) || ((which_alternative == 3) || (which_alternative == 4)))) && (((which_alternative != 1) && (which_alternative != 3)) && ((which_alternative != 2) && (which_alternative != 4))))
        {
	  return 32 /* 0x20 */;
        }
      else if ((((arm_is_6_or_7) == (MODEL_WBUF_YES))) && ((which_alternative == 2) || (which_alternative == 4)))
        {
	  return 7;
        }
      else if ((((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 1) || (which_alternative == 3))) && (((arm_is_xscale) == (IS_XSCALE_YES)))) || ((which_alternative == 2) || (which_alternative == 4)))
        {
	  return 3;
        }
      else if (((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 1) || (which_alternative == 3))) || ((! (((arm_ld_sched) == (LDSCHED_YES)))) && ((which_alternative == 1) || (which_alternative == 3))))
        {
	  return 2;
        }
      else
        {
	  return 1;
        }

    case 198:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 2) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))
        {
	  return 7;
        }
      else if ((((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 1)) && (((arm_is_xscale) == (IS_XSCALE_YES)))) || ((which_alternative != 0) && (which_alternative != 1)))
        {
	  return 3;
        }
      else if (((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 1)) || ((! (((arm_ld_sched) == (LDSCHED_YES)))) && (which_alternative == 1)))
        {
	  return 2;
        }
      else
        {
	  return 1;
        }

    case 613:
    case 197:
      extract_constrain_insn_cached (insn);
      if (((which_alternative != 2) && ((which_alternative != 5) && (which_alternative != 6))) && (((which_alternative != 0) && (which_alternative != 4)) && ((which_alternative != 1) && (which_alternative != 3))))
        {
	  return 32 /* 0x20 */;
        }
      else if (((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 10)) || ((((arm_is_6_or_7) == (MODEL_WBUF_YES))) && ((which_alternative == 1) || (which_alternative == 3))))
        {
	  return 7;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 9))
        {
	  return 6;
        }
      else if ((((arm_is_6_or_7) == (MODEL_WBUF_YES))) && (which_alternative == 9))
        {
	  return 5;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 8))
        {
	  return 4;
        }
      else if ((((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 0) || (which_alternative == 4))) && (((arm_is_xscale) == (IS_XSCALE_YES)))) || (((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 7)) || ((which_alternative == 1) || (which_alternative == 3))))
        {
	  return 3;
        }
      else if (((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 0) || (which_alternative == 4))) || (((! (((arm_ld_sched) == (LDSCHED_YES)))) && ((which_alternative == 0) || (which_alternative == 4))) || ((((arm_fpu_attr) == (FPU_FPA))) && ((which_alternative == 5) || (which_alternative == 6)))))
        {
	  return 2;
        }
      else
        {
	  return 1;
        }

    case 196:
      extract_constrain_insn_cached (insn);
      if (((which_alternative == 1) || ((which_alternative == 2) || ((which_alternative == 3) || (which_alternative == 4)))) && (((which_alternative != 1) && (which_alternative != 3)) && ((which_alternative != 2) && (which_alternative != 4))))
        {
	  return 32 /* 0x20 */;
        }
      else if ((((arm_is_6_or_7) == (MODEL_WBUF_YES))) && ((which_alternative == 2) || (which_alternative == 4)))
        {
	  return 5;
        }
      else if (((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 1) || (which_alternative == 3))) && (((arm_is_xscale) == (IS_XSCALE_YES))))
        {
	  return 3;
        }
      else if (((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 1) || (which_alternative == 3))) || ((! (((arm_ld_sched) == (LDSCHED_YES)))) && (((which_alternative == 1) || (which_alternative == 3)) || ((which_alternative == 2) || (which_alternative == 4)))))
        {
	  return 2;
        }
      else
        {
	  return 1;
        }

    case 612:
    case 195:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 2) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))
        {
	  return 5;
        }
      else if (((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 1)) && (((arm_is_xscale) == (IS_XSCALE_YES))))
        {
	  return 3;
        }
      else if (((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 1)) || ((! (((arm_ld_sched) == (LDSCHED_YES)))) && (which_alternative != 0)))
        {
	  return 2;
        }
      else
        {
	  return 1;
        }

    case 611:
    case 194:
      extract_constrain_insn_cached (insn);
      if (((which_alternative != 6) && ((which_alternative != 0) && (which_alternative != 1))) && ((which_alternative != 7) && (which_alternative != 8)))
        {
	  return 32 /* 0x20 */;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 5))
        {
	  return 7;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 4))
        {
	  return 6;
        }
      else if ((((arm_is_6_or_7) == (MODEL_WBUF_YES))) && ((which_alternative == 8) || (which_alternative == 4)))
        {
	  return 5;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 3))
        {
	  return 4;
        }
      else if ((((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 7)) && (((arm_is_xscale) == (IS_XSCALE_YES)))) || ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 2)))
        {
	  return 3;
        }
      else if (((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 7)) || (((! (((arm_ld_sched) == (LDSCHED_YES)))) && ((which_alternative == 7) || (which_alternative == 8))) || ((((arm_fpu_attr) == (FPU_FPA))) && ((which_alternative == 0) || (which_alternative == 1)))))
        {
	  return 2;
        }
      else
        {
	  return 1;
        }

    case 188:
    case 187:
      extract_constrain_insn_cached (insn);
      if (((which_alternative == 2) && (((arm_ld_sched) == (LDSCHED_YES)))) && (((arm_is_xscale) == (IS_XSCALE_YES))))
        {
	  return 3;
        }
      else if (which_alternative == 2)
        {
	  return 2;
        }
      else
        {
	  return 1;
        }

    case 186:
      extract_constrain_insn_cached (insn);
      if ((((arm_is_6_or_7) == (MODEL_WBUF_YES))) && (which_alternative == 2))
        {
	  return 5;
        }
      else if (((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 3)) && (((arm_is_xscale) == (IS_XSCALE_YES))))
        {
	  return 3;
        }
      else if (((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 3)) || ((! (((arm_ld_sched) == (LDSCHED_YES)))) && ((which_alternative == 3) || (which_alternative == 2))))
        {
	  return 2;
        }
      else
        {
	  return 1;
        }

    case 193:
    case 184:
      extract_constrain_insn_cached (insn);
      if (((which_alternative == 1) || (which_alternative == 2)) && ((which_alternative != 1) && (which_alternative != 2)))
        {
	  return 32 /* 0x20 */;
        }
      else if ((((arm_is_6_or_7) == (MODEL_WBUF_YES))) && (which_alternative == 2))
        {
	  return 5;
        }
      else if (((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 1)) && (((arm_is_xscale) == (IS_XSCALE_YES))))
        {
	  return 3;
        }
      else if (((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 1)) || ((! (((arm_ld_sched) == (LDSCHED_YES)))) && ((which_alternative == 1) || (which_alternative == 2))))
        {
	  return 2;
        }
      else
        {
	  return 1;
        }

    case 176:
      extract_constrain_insn_cached (insn);
      if ((((arm_is_6_or_7) == (MODEL_WBUF_YES))) && ((which_alternative == 5) || (which_alternative == 7)))
        {
	  return 5;
        }
      else if (((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 4) || (which_alternative == 6))) && (((arm_is_xscale) == (IS_XSCALE_YES))))
        {
	  return 3;
        }
      else if (((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 4) || (which_alternative == 6))) || ((! (((arm_ld_sched) == (LDSCHED_YES)))) && (((which_alternative == 4) || (which_alternative == 6)) || ((which_alternative == 5) || (which_alternative == 7)))))
        {
	  return 2;
        }
      else
        {
	  return 1;
        }

    case 192:
    case 175:
      extract_constrain_insn_cached (insn);
      if ((((arm_is_6_or_7) == (MODEL_WBUF_YES))) && (which_alternative == 3))
        {
	  return 5;
        }
      else if (((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 2)) && (((arm_is_xscale) == (IS_XSCALE_YES))))
        {
	  return 3;
        }
      else if (((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 2)) || ((! (((arm_ld_sched) == (LDSCHED_YES)))) && ((which_alternative == 2) || (which_alternative == 3))))
        {
	  return 2;
        }
      else
        {
	  return 1;
        }

    case 174:
      extract_constrain_insn_cached (insn);
      if ((((arm_is_6_or_7) == (MODEL_WBUF_YES))) && ((which_alternative == 4) || (which_alternative == 6)))
        {
	  return 7;
        }
      else if ((((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 3) || (which_alternative == 5))) && (((arm_is_xscale) == (IS_XSCALE_YES)))) || ((which_alternative == 4) || (which_alternative == 6)))
        {
	  return 3;
        }
      else if (((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 3) || (which_alternative == 5))) || ((! (((arm_ld_sched) == (LDSCHED_YES)))) && ((which_alternative == 3) || (which_alternative == 5))))
        {
	  return 2;
        }
      else
        {
	  return 1;
        }

    case 173:
      extract_constrain_insn_cached (insn);
      if ((((arm_is_6_or_7) == (MODEL_WBUF_YES))) && (which_alternative == 2))
        {
	  return 7;
        }
      else if ((((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 1)) && (((arm_is_xscale) == (IS_XSCALE_YES)))) || (which_alternative == 2))
        {
	  return 3;
        }
      else if (((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 1)) || ((! (((arm_ld_sched) == (LDSCHED_YES)))) && (which_alternative == 1)))
        {
	  return 2;
        }
      else
        {
	  return 1;
        }

    case 655:
    case 654:
    case 653:
    case 652:
    case 649:
    case 648:
    case 645:
    case 644:
    case 643:
    case 642:
    case 639:
    case 638:
    case 637:
    case 636:
    case 633:
    case 630:
    case 628:
    case 620:
    case 619:
    case 618:
    case 617:
    case 616:
    case 615:
    case 608:
    case 604:
    case 597:
    case 596:
    case 595:
    case 594:
    case 593:
    case 342:
    case 341:
    case 340:
    case 339:
    case 336:
    case 335:
    case 332:
    case 331:
    case 330:
    case 329:
    case 326:
    case 325:
    case 324:
    case 323:
    case 320:
    case 265:
    case 261:
    case 260:
    case 259:
    case 206:
    case 205:
    case 204:
    case 203:
    case 202:
    case 201:
    case 189:
    case 185:
    case 180:
    case 179:
    case 178:
    case 169:
    case 168:
    case 167:
    case 166:
    case 165:
    case 163:
    case 162:
    case 161:
    case 160:
      if ((((arm_ld_sched) == (LDSCHED_YES))) && (((arm_is_xscale) == (IS_XSCALE_YES))))
        {
	  return 3;
        }
      else
        {
	  return 2;
        }

    case 158:
      extract_constrain_insn_cached (insn);
      if (((which_alternative == 1) && (((arm_ld_sched) == (LDSCHED_YES)))) && (((arm_is_xscale) == (IS_XSCALE_YES))))
        {
	  return 3;
        }
      else if (which_alternative == 1)
        {
	  return 2;
        }
      else
        {
	  return 1;
        }

    case 586:
    case 585:
    case 584:
    case 583:
    case 582:
    case 581:
    case 578:
    case 577:
    case 576:
    case 575:
    case 349:
    case 344:
    case 343:
    case 262:
    case 258:
    case 257:
    case 256:
    case 255:
    case 254:
    case 253:
    case 252:
    case 251:
    case 250:
    case 249:
    case 248:
    case 247:
    case 230:
    case 229:
    case 228:
    case 227:
    case 226:
    case 225:
    case 224:
    case 223:
    case 222:
    case 221:
    case 153:
    case 152:
    case 151:
    case 150:
    case 149:
    case 148:
    case 142:
    case 141:
    case 140:
    case 139:
      return 32 /* 0x20 */;

    case 600:
    case 599:
    case 598:
    case 589:
    case 588:
    case 587:
    case 574:
    case 573:
    case 572:
    case 571:
    case 570:
    case 569:
    case 568:
    case 567:
    case 244:
    case 242:
    case 172:
    case 171:
    case 170:
    case 156:
    case 155:
    case 154:
    case 138:
    case 137:
    case 136:
    case 135:
    case 132:
    case 131:
    case 130:
    case 129:
      if (((arm_fpu_attr) == (FPU_FPA)))
        {
	  return 2;
        }
      else
        {
	  return 1;
        }

    case 651:
    case 650:
    case 647:
    case 646:
    case 641:
    case 640:
    case 635:
    case 634:
    case 338:
    case 337:
    case 334:
    case 333:
    case 328:
    case 327:
    case 322:
    case 321:
    case 113:
      if (((arm_is_6_or_7) == (MODEL_WBUF_YES)))
        {
	  return 5;
        }
      else if (! (((arm_ld_sched) == (LDSCHED_YES))))
        {
	  return 2;
        }
      else
        {
	  return 1;
        }

    case 548:
    case 542:
    case 76:
    case 70:
      if (((arm_fpu_attr) == (FPU_FPA)))
        {
	  return 71 /* 0x47 */;
        }
      else
        {
	  return 1;
        }

    case 547:
    case 546:
    case 545:
    case 544:
    case 541:
    case 540:
    case 539:
    case 538:
    case 75:
    case 74:
    case 73:
    case 72:
    case 69:
    case 68:
    case 67:
    case 66:
      if (((arm_fpu_attr) == (FPU_FPA)))
        {
	  return 59 /* 0x3b */;
        }
      else
        {
	  return 1;
        }

    case 543:
    case 537:
    case 71:
    case 65:
      if (((arm_fpu_attr) == (FPU_FPA)))
        {
	  return 31 /* 0x1f */;
        }
      else
        {
	  return 1;
        }

    case 536:
    case 535:
    case 534:
    case 533:
    case 532:
    case 64:
    case 63:
    case 62:
    case 61:
    case 60:
      if (((arm_fpu_attr) == (FPU_FPA)))
        {
	  return 9;
        }
      else
        {
	  return 1;
        }

    case 531:
    case 59:
      if (((arm_fpu_attr) == (FPU_FPA)))
        {
	  return 6;
        }
      else
        {
	  return 1;
        }

    case 530:
    case 529:
    case 528:
    case 527:
    case 526:
    case 525:
    case 524:
    case 523:
    case 279:
    case 278:
    case 277:
    case 58:
    case 57:
    case 56:
    case 55:
    case 54:
    case 53:
    case 52:
    case 51:
    case 50:
    case 49:
    case 48:
    case 47:
    case 46:
    case 45:
    case 44:
    case 43:
      if (((arm_ld_sched) == (LDSCHED_NO)))
        {
	  return 16 /* 0x10 */;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && (((arm_is_strong) == (IS_STRONGARM_NO))))
        {
	  return 4;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && (((arm_is_strong) == (IS_STRONGARM_YES))))
        {
	  return 3;
        }
      else
        {
	  return 1;
        }

    case 522:
    case 521:
    case 520:
    case 519:
    case 518:
    case 516:
    case 515:
    case 514:
    case 513:
    case 512:
    case 511:
    case 42:
    case 41:
    case 40:
    case 39:
    case 38:
    case 37:
    case 25:
    case 24:
    case 23:
    case 22:
    case 21:
    case 20:
      if (((arm_fpu_attr) == (FPU_FPA)))
        {
	  return 4;
        }
      else
        {
	  return 1;
        }

    case -1:
      if (GET_CODE (PATTERN (insn)) != ASM_INPUT
          && asm_noperands (PATTERN (insn)) < 0)
        fatal_insn_not_found (insn);
    default:
      return 1;

    }
}

extern int core_unit_ready_cost PARAMS ((rtx));
int
core_unit_ready_cost (insn)
     rtx insn;
{
  switch (recog_memoized (insn))
    {
    case 626:
    case 623:
    case 214:
    case 212:
    case 209:
      if (((((arm_is_xscale) == (IS_XSCALE_YES))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))) || ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))))
        {
	  return 3;
        }
      else
        {
	  return 32 /* 0x20 */;
        }

    case 625:
    case 622:
    case 213:
    case 211:
    case 208:
      if (((((arm_is_xscale) == (IS_XSCALE_YES))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))) || ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))))
        {
	  return 4;
        }
      else
        {
	  return 32 /* 0x20 */;
        }

    case 624:
    case 621:
    case 348:
    case 210:
    case 207:
      if (((((arm_is_xscale) == (IS_XSCALE_YES))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))) || ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))))
        {
	  return 5;
        }
      else
        {
	  return 32 /* 0x20 */;
        }

    case 614:
    case 200:
      extract_constrain_insn_cached (insn);
      if (((which_alternative != 5) && ((which_alternative != 4) && ((which_alternative != 3) && (which_alternative != 2)))) && (((((which_alternative != 0) && (which_alternative != 1)) && ((which_alternative != 2) && ((which_alternative != 3) && ((which_alternative != 4) && (which_alternative != 5))))) || ((which_alternative == 0) || (which_alternative == 1))) && ((((((arm_is_xscale) == (IS_XSCALE_YES))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && (((((arm_ld_sched) == (LDSCHED_NO))) && (((arm_fpu_attr) == (FPU_FPA)))) || (((((arm_ld_sched) == (LDSCHED_NO))) && (! (((arm_fpu_attr) == (FPU_FPA))))) || ((((arm_ld_sched) == (LDSCHED_YES))) && (((arm_fpu_attr) == (FPU_FPA))))))) || ((((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_YES))) && (! (((arm_fpu_attr) == (FPU_FPA)))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) && (((arm_fpu_attr) == (FPU_FPA)))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((((arm_ld_sched) == (LDSCHED_NO))) && (! (((arm_fpu_attr) == (FPU_FPA))))) || ((((arm_ld_sched) == (LDSCHED_YES))) && (((arm_fpu_attr) == (FPU_FPA))))))))) || (((((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_YES))) && (! (((arm_fpu_attr) == (FPU_FPA))))))) || ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) && (((arm_fpu_attr) == (FPU_FPA))))))) || ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((((arm_ld_sched) == (LDSCHED_NO))) && (! (((arm_fpu_attr) == (FPU_FPA))))) || ((((arm_ld_sched) == (LDSCHED_YES))) && (((arm_fpu_attr) == (FPU_FPA)))))))) || ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_YES))) && (! (((arm_fpu_attr) == (FPU_FPA)))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) && (((arm_fpu_attr) == (FPU_FPA)))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((((arm_ld_sched) == (LDSCHED_NO))) && (! (((arm_fpu_attr) == (FPU_FPA))))) || ((((arm_ld_sched) == (LDSCHED_YES))) && (((arm_fpu_attr) == (FPU_FPA)))))))))) || ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_YES))) && (! (((arm_fpu_attr) == (FPU_FPA))))))))))
        {
	  return 1;
        }
      else
        {
	  return 32 /* 0x20 */;
        }

    case 199:
      extract_constrain_insn_cached (insn);
      if (((which_alternative == 2) || (which_alternative == 4)) && ((((which_alternative == 1) || ((which_alternative == 2) || ((which_alternative == 3) || (which_alternative == 4)))) && (((((arm_is_xscale) == (IS_XSCALE_YES))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))) || ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))))) || (((which_alternative != 1) && ((which_alternative != 2) && ((which_alternative != 3) && (which_alternative != 4)))) && (((((arm_is_xscale) == (IS_XSCALE_YES))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))) || ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES)))))))))))
        {
	  return 3;
        }
      else if (((which_alternative == 1) || (which_alternative == 3)) && (((which_alternative == 1) || ((which_alternative == 2) || ((which_alternative == 3) || (which_alternative == 4)))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO)))))))
        {
	  return 2;
        }
      else if (((which_alternative == 1) || (which_alternative == 3)) && (((which_alternative == 1) || ((which_alternative == 2) || ((which_alternative == 3) || (which_alternative == 4)))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_YES)))))))
        {
	  return 3;
        }
      else if (((which_alternative == 1) || (which_alternative == 3)) && (((which_alternative == 1) || ((which_alternative == 2) || ((which_alternative == 3) || (which_alternative == 4)))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_NO)))))))
        {
	  return 2;
        }
      else if (((which_alternative == 1) || (which_alternative == 3)) && (((which_alternative == 1) || ((which_alternative == 2) || ((which_alternative == 3) || (which_alternative == 4)))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_YES)))))))
        {
	  return 3;
        }
      else if (((which_alternative == 1) || (which_alternative == 3)) && ((((which_alternative == 1) || ((which_alternative == 2) || ((which_alternative == 3) || (which_alternative == 4)))) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES)))))))) || (((which_alternative != 1) && ((which_alternative != 2) && ((which_alternative != 3) && (which_alternative != 4)))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO))))))))
        {
	  return 2;
        }
      else if ((((which_alternative != 2) && (which_alternative != 4)) && ((which_alternative != 1) && (which_alternative != 3))) && (((which_alternative != 1) && ((which_alternative != 2) && ((which_alternative != 3) && (which_alternative != 4)))) && (((((arm_is_xscale) == (IS_XSCALE_YES))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))) || ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))))))
        {
	  return 1;
        }
      else
        {
	  return 32 /* 0x20 */;
        }

    case 613:
    case 197:
      extract_constrain_insn_cached (insn);
      if (((which_alternative == 1) || (which_alternative == 3)) && ((((which_alternative != 2) && ((which_alternative != 5) && (which_alternative != 6))) && (((((arm_is_xscale) == (IS_XSCALE_YES))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))) || ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))))) || (((which_alternative == 2) || ((which_alternative == 5) || (which_alternative == 6))) && (((((arm_is_xscale) == (IS_XSCALE_YES))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))) || ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES)))))))))))
        {
	  return 3;
        }
      else if (((which_alternative == 0) || (which_alternative == 4)) && (((which_alternative != 2) && ((which_alternative != 5) && (which_alternative != 6))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO)))))))
        {
	  return 2;
        }
      else if (((which_alternative == 0) || (which_alternative == 4)) && (((which_alternative != 2) && ((which_alternative != 5) && (which_alternative != 6))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_YES)))))))
        {
	  return 3;
        }
      else if (((which_alternative == 0) || (which_alternative == 4)) && (((which_alternative != 2) && ((which_alternative != 5) && (which_alternative != 6))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_NO)))))))
        {
	  return 2;
        }
      else if (((which_alternative == 0) || (which_alternative == 4)) && (((which_alternative != 2) && ((which_alternative != 5) && (which_alternative != 6))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_YES)))))))
        {
	  return 3;
        }
      else if (((which_alternative == 0) || (which_alternative == 4)) && ((((which_alternative != 2) && ((which_alternative != 5) && (which_alternative != 6))) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES)))))))) || (((which_alternative == 2) || ((which_alternative == 5) || (which_alternative == 6))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO))))))))
        {
	  return 2;
        }
      else if ((((which_alternative != 1) && (which_alternative != 3)) && (((which_alternative != 0) && (which_alternative != 4)) && ((which_alternative != 10) && ((which_alternative != 9) && ((which_alternative != 8) && (which_alternative != 7)))))) && (((which_alternative == 2) || ((which_alternative == 5) || (which_alternative == 6))) && (((((arm_is_xscale) == (IS_XSCALE_YES))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))) || ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))))))
        {
	  return 1;
        }
      else
        {
	  return 32 /* 0x20 */;
        }

    case 196:
      extract_constrain_insn_cached (insn);
      if (((which_alternative == 2) || (which_alternative == 4)) && (((which_alternative == 1) || ((which_alternative == 2) || ((which_alternative == 3) || (which_alternative == 4)))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO)))))))
        {
	  return 2;
        }
      else if (((which_alternative == 2) || (which_alternative == 4)) && (((which_alternative == 1) || ((which_alternative == 2) || ((which_alternative == 3) || (which_alternative == 4)))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_YES)))))))
        {
	  return 1;
        }
      else if (((which_alternative == 2) || (which_alternative == 4)) && (((which_alternative == 1) || ((which_alternative == 2) || ((which_alternative == 3) || (which_alternative == 4)))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_NO)))))))
        {
	  return 2;
        }
      else if (((which_alternative == 2) || (which_alternative == 4)) && (((which_alternative == 1) || ((which_alternative == 2) || ((which_alternative == 3) || (which_alternative == 4)))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_YES)))))))
        {
	  return 1;
        }
      else if (((which_alternative == 2) || (which_alternative == 4)) && (((which_alternative == 1) || ((which_alternative == 2) || ((which_alternative == 3) || (which_alternative == 4)))) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO)))))))
        {
	  return 2;
        }
      else if (((which_alternative == 2) || (which_alternative == 4)) && (((which_alternative == 1) || ((which_alternative == 2) || ((which_alternative == 3) || (which_alternative == 4)))) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_YES)))))))
        {
	  return 1;
        }
      else if (((which_alternative == 2) || (which_alternative == 4)) && (((which_alternative == 1) || ((which_alternative == 2) || ((which_alternative == 3) || (which_alternative == 4)))) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_NO)))))))
        {
	  return 2;
        }
      else if (((which_alternative == 2) || (which_alternative == 4)) && (((which_alternative == 1) || ((which_alternative == 2) || ((which_alternative == 3) || (which_alternative == 4)))) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_YES)))))))
        {
	  return 1;
        }
      else if (((which_alternative == 1) || (which_alternative == 3)) && (((which_alternative == 1) || ((which_alternative == 2) || ((which_alternative == 3) || (which_alternative == 4)))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO)))))))
        {
	  return 2;
        }
      else if (((which_alternative == 1) || (which_alternative == 3)) && (((which_alternative == 1) || ((which_alternative == 2) || ((which_alternative == 3) || (which_alternative == 4)))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_YES)))))))
        {
	  return 3;
        }
      else if (((which_alternative == 1) || (which_alternative == 3)) && (((which_alternative == 1) || ((which_alternative == 2) || ((which_alternative == 3) || (which_alternative == 4)))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_NO)))))))
        {
	  return 2;
        }
      else if (((which_alternative == 1) || (which_alternative == 3)) && (((which_alternative == 1) || ((which_alternative == 2) || ((which_alternative == 3) || (which_alternative == 4)))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_YES)))))))
        {
	  return 3;
        }
      else if (((which_alternative == 1) || (which_alternative == 3)) && ((((which_alternative == 1) || ((which_alternative == 2) || ((which_alternative == 3) || (which_alternative == 4)))) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES)))))))) || (((which_alternative != 1) && ((which_alternative != 2) && ((which_alternative != 3) && (which_alternative != 4)))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO))))))))
        {
	  return 2;
        }
      else if ((((which_alternative != 2) && (which_alternative != 4)) && ((which_alternative != 1) && (which_alternative != 3))) && (((which_alternative != 1) && ((which_alternative != 2) && ((which_alternative != 3) && (which_alternative != 4)))) && (((((arm_is_xscale) == (IS_XSCALE_YES))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))) || ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))))))
        {
	  return 1;
        }
      else
        {
	  return 32 /* 0x20 */;
        }

    case 612:
    case 195:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 2) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 2) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 1;
        }
      else if ((which_alternative == 2) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 2) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 1;
        }
      else if ((which_alternative == 2) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 2) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 1;
        }
      else if ((which_alternative == 2) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 2) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 1;
        }
      else if ((which_alternative == 1) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 1) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 3;
        }
      else if ((which_alternative == 1) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 1) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 3;
        }
      else if ((which_alternative == 1) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))))
        {
	  return 2;
        }
      else if ((which_alternative == 0) && (((((arm_is_xscale) == (IS_XSCALE_YES))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))) || ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES)))))))))
        {
	  return 1;
        }
      else
        {
	  return 32 /* 0x20 */;
        }

    case 611:
    case 194:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 8) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 8) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 1;
        }
      else if ((which_alternative == 8) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 8) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 1;
        }
      else if ((which_alternative == 8) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 8) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 1;
        }
      else if ((which_alternative == 8) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 8) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 1;
        }
      else if ((which_alternative == 7) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 7) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 3;
        }
      else if ((which_alternative == 7) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 7) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 3;
        }
      else if ((which_alternative == 7) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))))
        {
	  return 2;
        }
      else if (((which_alternative != 8) && ((which_alternative != 7) && ((which_alternative != 5) && ((which_alternative != 4) && ((which_alternative != 3) && (which_alternative != 2)))))) && (((which_alternative == 6) || ((which_alternative == 0) || (which_alternative == 1))) && (((((arm_is_xscale) == (IS_XSCALE_YES))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))) || ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))))))
        {
	  return 1;
        }
      else
        {
	  return 32 /* 0x20 */;
        }

    case 607:
    case 606:
    case 188:
    case 187:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 2) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 2) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 3;
        }
      else if ((which_alternative == 2) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 2) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 3;
        }
      else if ((which_alternative == 2) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))))
        {
	  return 2;
        }
      else if (((which_alternative == 0) || (which_alternative == 1)) && (((((arm_is_xscale) == (IS_XSCALE_YES))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))) || ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES)))))))))
        {
	  return 1;
        }
      else
        {
	  return 32 /* 0x20 */;
        }

    case 605:
    case 186:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 2) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 2) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 1;
        }
      else if ((which_alternative == 2) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 2) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 1;
        }
      else if ((which_alternative == 2) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 2) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 1;
        }
      else if ((which_alternative == 2) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 2) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 1;
        }
      else if ((which_alternative == 3) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 3) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 3;
        }
      else if ((which_alternative == 3) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 3) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 3;
        }
      else if ((which_alternative == 3) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))))
        {
	  return 2;
        }
      else if (((which_alternative != 2) && (which_alternative != 3)) && (((which_alternative == 0) || (which_alternative == 1)) && (((((arm_is_xscale) == (IS_XSCALE_YES))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))) || ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))))))
        {
	  return 1;
        }
      else
        {
	  return 32 /* 0x20 */;
        }

    case 193:
    case 184:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 2) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 2) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 1;
        }
      else if ((which_alternative == 2) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 2) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 1;
        }
      else if ((which_alternative == 2) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 2) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 1;
        }
      else if ((which_alternative == 2) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 2) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 1;
        }
      else if ((which_alternative == 1) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 1) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 3;
        }
      else if ((which_alternative == 1) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 1) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 3;
        }
      else if ((which_alternative == 1) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))))
        {
	  return 2;
        }
      else if (((which_alternative != 2) && (which_alternative != 1)) && (((which_alternative != 1) && (which_alternative != 2)) && (((((arm_is_xscale) == (IS_XSCALE_YES))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))) || ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))))))
        {
	  return 1;
        }
      else
        {
	  return 32 /* 0x20 */;
        }

    case 176:
      extract_constrain_insn_cached (insn);
      if (((which_alternative == 5) || (which_alternative == 7)) && (((which_alternative != 0) && ((which_alternative != 1) && ((which_alternative != 2) && ((which_alternative != 3) && (which_alternative != 8))))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO)))))))
        {
	  return 2;
        }
      else if (((which_alternative == 5) || (which_alternative == 7)) && (((which_alternative != 0) && ((which_alternative != 1) && ((which_alternative != 2) && ((which_alternative != 3) && (which_alternative != 8))))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_YES)))))))
        {
	  return 1;
        }
      else if (((which_alternative == 5) || (which_alternative == 7)) && (((which_alternative != 0) && ((which_alternative != 1) && ((which_alternative != 2) && ((which_alternative != 3) && (which_alternative != 8))))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_NO)))))))
        {
	  return 2;
        }
      else if (((which_alternative == 5) || (which_alternative == 7)) && (((which_alternative != 0) && ((which_alternative != 1) && ((which_alternative != 2) && ((which_alternative != 3) && (which_alternative != 8))))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_YES)))))))
        {
	  return 1;
        }
      else if (((which_alternative == 5) || (which_alternative == 7)) && (((which_alternative != 0) && ((which_alternative != 1) && ((which_alternative != 2) && ((which_alternative != 3) && (which_alternative != 8))))) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO)))))))
        {
	  return 2;
        }
      else if (((which_alternative == 5) || (which_alternative == 7)) && (((which_alternative != 0) && ((which_alternative != 1) && ((which_alternative != 2) && ((which_alternative != 3) && (which_alternative != 8))))) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_YES)))))))
        {
	  return 1;
        }
      else if (((which_alternative == 5) || (which_alternative == 7)) && (((which_alternative != 0) && ((which_alternative != 1) && ((which_alternative != 2) && ((which_alternative != 3) && (which_alternative != 8))))) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_NO)))))))
        {
	  return 2;
        }
      else if (((which_alternative == 5) || (which_alternative == 7)) && (((which_alternative != 0) && ((which_alternative != 1) && ((which_alternative != 2) && ((which_alternative != 3) && (which_alternative != 8))))) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_YES)))))))
        {
	  return 1;
        }
      else if (((which_alternative == 5) || (which_alternative == 7)) && (((which_alternative == 0) || ((which_alternative == 1) || ((which_alternative == 2) || ((which_alternative == 3) || (which_alternative == 8))))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO)))))))
        {
	  return 2;
        }
      else if (((which_alternative == 4) || (which_alternative == 6)) && (((which_alternative != 0) && ((which_alternative != 1) && ((which_alternative != 2) && ((which_alternative != 3) && (which_alternative != 8))))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO)))))))
        {
	  return 2;
        }
      else if (((which_alternative == 4) || (which_alternative == 6)) && (((which_alternative != 0) && ((which_alternative != 1) && ((which_alternative != 2) && ((which_alternative != 3) && (which_alternative != 8))))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_YES)))))))
        {
	  return 3;
        }
      else if (((which_alternative == 4) || (which_alternative == 6)) && (((which_alternative != 0) && ((which_alternative != 1) && ((which_alternative != 2) && ((which_alternative != 3) && (which_alternative != 8))))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_NO)))))))
        {
	  return 2;
        }
      else if (((which_alternative == 4) || (which_alternative == 6)) && (((which_alternative != 0) && ((which_alternative != 1) && ((which_alternative != 2) && ((which_alternative != 3) && (which_alternative != 8))))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_YES)))))))
        {
	  return 3;
        }
      else if (((which_alternative == 4) || (which_alternative == 6)) && ((((which_alternative != 0) && ((which_alternative != 1) && ((which_alternative != 2) && ((which_alternative != 3) && (which_alternative != 8))))) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES)))))))) || (((which_alternative == 0) || ((which_alternative == 1) || ((which_alternative == 2) || ((which_alternative == 3) || (which_alternative == 8))))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO))))))))
        {
	  return 2;
        }
      else if ((((which_alternative != 5) && (which_alternative != 7)) && ((which_alternative != 4) && (which_alternative != 6))) && (((which_alternative == 0) || ((which_alternative == 1) || ((which_alternative == 2) || ((which_alternative == 3) || (which_alternative == 8))))) && (((((arm_is_xscale) == (IS_XSCALE_YES))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))) || ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))))))
        {
	  return 1;
        }
      else
        {
	  return 32 /* 0x20 */;
        }

    case 610:
    case 601:
    case 192:
    case 175:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 3) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 3) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 1;
        }
      else if ((which_alternative == 3) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 3) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 1;
        }
      else if ((which_alternative == 3) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 3) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 1;
        }
      else if ((which_alternative == 3) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 3) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 1;
        }
      else if ((which_alternative == 2) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 2) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 3;
        }
      else if ((which_alternative == 2) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 2) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 3;
        }
      else if ((which_alternative == 2) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))))
        {
	  return 2;
        }
      else if (((which_alternative != 3) && (which_alternative != 2)) && (((which_alternative == 0) || (which_alternative == 1)) && (((((arm_is_xscale) == (IS_XSCALE_YES))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))) || ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))))))
        {
	  return 1;
        }
      else
        {
	  return 32 /* 0x20 */;
        }

    case 174:
      extract_constrain_insn_cached (insn);
      if (((which_alternative == 4) || (which_alternative == 6)) && ((((which_alternative != 0) && ((which_alternative != 1) && ((which_alternative != 2) && (which_alternative != 7)))) && (((((arm_is_xscale) == (IS_XSCALE_YES))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))) || ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))))) || (((which_alternative == 0) || ((which_alternative == 1) || ((which_alternative == 2) || (which_alternative == 7)))) && (((((arm_is_xscale) == (IS_XSCALE_YES))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))) || ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES)))))))))))
        {
	  return 3;
        }
      else if (((which_alternative == 3) || (which_alternative == 5)) && (((which_alternative != 0) && ((which_alternative != 1) && ((which_alternative != 2) && (which_alternative != 7)))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO)))))))
        {
	  return 2;
        }
      else if (((which_alternative == 3) || (which_alternative == 5)) && (((which_alternative != 0) && ((which_alternative != 1) && ((which_alternative != 2) && (which_alternative != 7)))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_YES)))))))
        {
	  return 3;
        }
      else if (((which_alternative == 3) || (which_alternative == 5)) && (((which_alternative != 0) && ((which_alternative != 1) && ((which_alternative != 2) && (which_alternative != 7)))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_NO)))))))
        {
	  return 2;
        }
      else if (((which_alternative == 3) || (which_alternative == 5)) && (((which_alternative != 0) && ((which_alternative != 1) && ((which_alternative != 2) && (which_alternative != 7)))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_YES)))))))
        {
	  return 3;
        }
      else if (((which_alternative == 3) || (which_alternative == 5)) && ((((which_alternative != 0) && ((which_alternative != 1) && ((which_alternative != 2) && (which_alternative != 7)))) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES)))))))) || (((which_alternative == 0) || ((which_alternative == 1) || ((which_alternative == 2) || (which_alternative == 7)))) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO))))))))
        {
	  return 2;
        }
      else if ((((which_alternative != 4) && (which_alternative != 6)) && ((which_alternative != 3) && (which_alternative != 5))) && (((which_alternative == 0) || ((which_alternative == 1) || ((which_alternative == 2) || (which_alternative == 7)))) && (((((arm_is_xscale) == (IS_XSCALE_YES))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))) || ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))))))
        {
	  return 1;
        }
      else
        {
	  return 32 /* 0x20 */;
        }

    case 198:
    case 173:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 2) && (((((arm_is_xscale) == (IS_XSCALE_YES))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))) || ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES)))))))))
        {
	  return 3;
        }
      else if ((which_alternative == 1) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 1) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 3;
        }
      else if ((which_alternative == 1) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 1) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 3;
        }
      else if ((which_alternative == 1) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))))
        {
	  return 2;
        }
      else if ((which_alternative == 0) && (((((arm_is_xscale) == (IS_XSCALE_YES))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))) || ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES)))))))))
        {
	  return 1;
        }
      else
        {
	  return 32 /* 0x20 */;
        }

    case 655:
    case 654:
    case 653:
    case 652:
    case 649:
    case 648:
    case 645:
    case 644:
    case 643:
    case 642:
    case 639:
    case 638:
    case 637:
    case 636:
    case 633:
    case 630:
    case 628:
    case 620:
    case 619:
    case 618:
    case 617:
    case 616:
    case 615:
    case 608:
    case 604:
    case 597:
    case 596:
    case 595:
    case 594:
    case 593:
    case 342:
    case 341:
    case 340:
    case 339:
    case 336:
    case 335:
    case 332:
    case 331:
    case 330:
    case 329:
    case 326:
    case 325:
    case 324:
    case 323:
    case 320:
    case 265:
    case 261:
    case 260:
    case 259:
    case 206:
    case 205:
    case 204:
    case 203:
    case 202:
    case 201:
    case 189:
    case 185:
    case 180:
    case 179:
    case 178:
    case 169:
    case 168:
    case 167:
    case 166:
    case 165:
    case 163:
    case 162:
    case 161:
    case 160:
      if ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO)))))
        {
	  return 2;
        }
      else if ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_YES)))))
        {
	  return 3;
        }
      else if ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_NO)))))
        {
	  return 2;
        }
      else if ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_YES)))))
        {
	  return 3;
        }
      else if ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES)))))))
        {
	  return 2;
        }
      else
        {
	  return 32 /* 0x20 */;
        }

    case 591:
    case 158:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 1) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 1) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 3;
        }
      else if ((which_alternative == 1) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_NO))))))
        {
	  return 2;
        }
      else if ((which_alternative == 1) && ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_YES))))))
        {
	  return 3;
        }
      else if ((which_alternative == 1) && ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))))
        {
	  return 2;
        }
      else if ((which_alternative == 0) && (((((arm_is_xscale) == (IS_XSCALE_YES))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))))) || ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && (((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) || (((arm_ld_sched) == (LDSCHED_YES)))))))))
        {
	  return 1;
        }
      else
        {
	  return 32 /* 0x20 */;
        }

    case 586:
    case 585:
    case 584:
    case 583:
    case 582:
    case 581:
    case 578:
    case 577:
    case 576:
    case 575:
    case 349:
    case 344:
    case 343:
    case 262:
    case 258:
    case 257:
    case 256:
    case 255:
    case 254:
    case 253:
    case 252:
    case 251:
    case 250:
    case 249:
    case 248:
    case 247:
    case 230:
    case 229:
    case 228:
    case 227:
    case 226:
    case 225:
    case 224:
    case 223:
    case 222:
    case 221:
    case 153:
    case 152:
    case 151:
    case 150:
    case 149:
    case 148:
    case 142:
    case 141:
    case 140:
    case 139:
      return 32 /* 0x20 */;

    case 651:
    case 650:
    case 647:
    case 646:
    case 641:
    case 640:
    case 635:
    case 634:
    case 338:
    case 337:
    case 334:
    case 333:
    case 328:
    case 327:
    case 322:
    case 321:
    case 113:
      if ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO)))))
        {
	  return 2;
        }
      else if ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_YES)))))
        {
	  return 1;
        }
      else if ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_NO)))))
        {
	  return 2;
        }
      else if ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_YES)))))
        {
	  return 1;
        }
      else if ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO)))))
        {
	  return 2;
        }
      else if ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_YES)))))
        {
	  return 1;
        }
      else if ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_NO)))))
        {
	  return 2;
        }
      else if ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_YES)))))
        {
	  return 1;
        }
      else
        {
	  return 32 /* 0x20 */;
        }

    case 530:
    case 529:
    case 528:
    case 527:
    case 526:
    case 525:
    case 524:
    case 523:
    case 279:
    case 278:
    case 277:
    case 58:
    case 57:
    case 56:
    case 55:
    case 54:
    case 53:
    case 52:
    case 51:
    case 50:
    case 49:
    case 48:
    case 47:
    case 46:
    case 45:
    case 44:
    case 43:
      if ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO)))))
        {
	  return 16 /* 0x10 */;
        }
      else if ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_YES)))))
        {
	  return 4;
        }
      else if ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_NO)))))
        {
	  return 16 /* 0x10 */;
        }
      else if ((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_YES)))))
        {
	  return 3;
        }
      else if ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_NO)))))
        {
	  return 16 /* 0x10 */;
        }
      else if ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_NO))) && (((arm_ld_sched) == (LDSCHED_YES)))))
        {
	  return 4;
        }
      else if ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_NO)))))
        {
	  return 16 /* 0x10 */;
        }
      else if ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((arm_ld_sched) == (LDSCHED_YES)))))
        {
	  return 3;
        }
      else
        {
	  return 32 /* 0x20 */;
        }

    case -1:
      if (GET_CODE (PATTERN (insn)) != ASM_INPUT
          && asm_noperands (PATTERN (insn)) < 0)
        fatal_insn_not_found (insn);
    default:
      if (((((arm_is_xscale) == (IS_XSCALE_YES))) && ((((((arm_is_strong) == (IS_STRONGARM_NO))) && (((((arm_ld_sched) == (LDSCHED_NO))) && (((arm_fpu_attr) == (FPU_FPA)))) || (((((arm_ld_sched) == (LDSCHED_NO))) && (! (((arm_fpu_attr) == (FPU_FPA))))) || ((((arm_ld_sched) == (LDSCHED_YES))) && (((arm_fpu_attr) == (FPU_FPA))))))) || ((((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_YES))) && (! (((arm_fpu_attr) == (FPU_FPA)))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) && (((arm_fpu_attr) == (FPU_FPA)))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((((arm_ld_sched) == (LDSCHED_NO))) && (! (((arm_fpu_attr) == (FPU_FPA))))) || ((((arm_ld_sched) == (LDSCHED_YES))) && (((arm_fpu_attr) == (FPU_FPA)))))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_YES))) && (! (((arm_fpu_attr) == (FPU_FPA)))))))) || ((! (((arm_is_xscale) == (IS_XSCALE_YES)))) && ((((((arm_is_strong) == (IS_STRONGARM_NO))) && (((((arm_ld_sched) == (LDSCHED_NO))) && (((arm_fpu_attr) == (FPU_FPA)))) || (((((arm_ld_sched) == (LDSCHED_NO))) && (! (((arm_fpu_attr) == (FPU_FPA))))) || ((((arm_ld_sched) == (LDSCHED_YES))) && (((arm_fpu_attr) == (FPU_FPA))))))) || ((((((arm_is_strong) == (IS_STRONGARM_NO))) && ((((arm_ld_sched) == (LDSCHED_YES))) && (! (((arm_fpu_attr) == (FPU_FPA)))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_NO))) && (((arm_fpu_attr) == (FPU_FPA)))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && (((((arm_ld_sched) == (LDSCHED_NO))) && (! (((arm_fpu_attr) == (FPU_FPA))))) || ((((arm_ld_sched) == (LDSCHED_YES))) && (((arm_fpu_attr) == (FPU_FPA)))))))) || ((((arm_is_strong) == (IS_STRONGARM_YES))) && ((((arm_ld_sched) == (LDSCHED_YES))) && (! (((arm_fpu_attr) == (FPU_FPA)))))))))
        {
	  return 1;
        }
      else
        {
	  return 32 /* 0x20 */;
        }

    }
}

extern unsigned int core_unit_blockage_range PARAMS ((rtx));
unsigned int
core_unit_blockage_range (insn)
     rtx insn;
{
  switch (recog_memoized (insn))
    {
    case -1:
      if (GET_CODE (PATTERN (insn)) != ASM_INPUT
          && asm_noperands (PATTERN (insn)) < 0)
        fatal_insn_not_found (insn);
    default:
      return 65568 /* min 1, max 32 */;

    }
}

extern int write_blockage_unit_ready_cost PARAMS ((rtx));
int
write_blockage_unit_ready_cost (insn)
     rtx insn;
{
  switch (recog_memoized (insn))
    {
    case 626:
    case 623:
    case 214:
    case 212:
    case 209:
      if (((arm_is_6_or_7) == (MODEL_WBUF_YES)))
        {
	  return 7;
        }
      else
        {
	  return 11 /* 0xb */;
        }

    case 625:
    case 622:
    case 213:
    case 211:
    case 208:
      if (((arm_is_6_or_7) == (MODEL_WBUF_YES)))
        {
	  return 9;
        }
      else
        {
	  return 11 /* 0xb */;
        }

    case 614:
    case 200:
      extract_constrain_insn_cached (insn);
      if (((which_alternative == 2) || ((which_alternative == 3) || ((which_alternative == 5) || (which_alternative == 4)))) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))
        {
	  return 1;
        }
      else
        {
	  return 11 /* 0xb */;
        }

    case 199:
      extract_constrain_insn_cached (insn);
      if (((which_alternative == 2) || (which_alternative == 4)) && ((((which_alternative == 1) || (which_alternative == 3)) && (((arm_is_6_or_7) == (MODEL_WBUF_YES)))) || (((which_alternative != 1) && (which_alternative != 3)) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))))
        {
	  return 7;
        }
      else if (((which_alternative != 2) && (which_alternative != 4)) && (((which_alternative == 1) || (which_alternative == 3)) && (((arm_is_6_or_7) == (MODEL_WBUF_YES)))))
        {
	  return 1;
        }
      else
        {
	  return 11 /* 0xb */;
        }

    case 198:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 2) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))
        {
	  return 7;
        }
      else if (((which_alternative == 0) || (which_alternative == 1)) && ((which_alternative == 1) && (((arm_is_6_or_7) == (MODEL_WBUF_YES)))))
        {
	  return 1;
        }
      else
        {
	  return 11 /* 0xb */;
        }

    case 613:
    case 197:
      extract_constrain_insn_cached (insn);
      if (((which_alternative == 1) || (which_alternative == 3)) && ((((which_alternative == 7) || ((which_alternative == 8) || ((which_alternative == 10) || ((which_alternative == 9) || ((which_alternative == 0) || (which_alternative == 4)))))) && (((arm_is_6_or_7) == (MODEL_WBUF_YES)))) || (((which_alternative != 7) && ((which_alternative != 8) && ((which_alternative != 10) && ((which_alternative != 9) && ((which_alternative != 0) && (which_alternative != 4)))))) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))))
        {
	  return 7;
        }
      else if (((which_alternative != 1) && (which_alternative != 3)) && (((which_alternative == 7) || ((which_alternative == 8) || ((which_alternative == 10) || ((which_alternative == 9) || ((which_alternative == 0) || (which_alternative == 4)))))) && (((arm_is_6_or_7) == (MODEL_WBUF_YES)))))
        {
	  return 1;
        }
      else
        {
	  return 11 /* 0xb */;
        }

    case 196:
      extract_constrain_insn_cached (insn);
      if (((which_alternative == 2) || (which_alternative == 4)) && ((((which_alternative == 1) || (which_alternative == 3)) && (((arm_is_6_or_7) == (MODEL_WBUF_YES)))) || (((which_alternative != 1) && (which_alternative != 3)) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))))
        {
	  return 5;
        }
      else if (((which_alternative != 2) && (which_alternative != 4)) && (((which_alternative == 1) || (which_alternative == 3)) && (((arm_is_6_or_7) == (MODEL_WBUF_YES)))))
        {
	  return 1;
        }
      else
        {
	  return 11 /* 0xb */;
        }

    case 612:
    case 195:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 2) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))
        {
	  return 5;
        }
      else if (((which_alternative == 0) || (which_alternative == 1)) && ((which_alternative == 1) && (((arm_is_6_or_7) == (MODEL_WBUF_YES)))))
        {
	  return 1;
        }
      else
        {
	  return 11 /* 0xb */;
        }

    case 611:
    case 194:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 8) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))
        {
	  return 5;
        }
      else if (((which_alternative == 2) || ((which_alternative == 3) || ((which_alternative == 5) || ((which_alternative == 4) || (which_alternative == 7))))) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))
        {
	  return 1;
        }
      else
        {
	  return 11 /* 0xb */;
        }

    case 607:
    case 606:
    case 188:
    case 187:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 2) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))
        {
	  return 1;
        }
      else
        {
	  return 11 /* 0xb */;
        }

    case 605:
    case 186:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 2) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))
        {
	  return 5;
        }
      else if ((which_alternative == 3) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))
        {
	  return 1;
        }
      else
        {
	  return 11 /* 0xb */;
        }

    case 193:
    case 184:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 2) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))
        {
	  return 5;
        }
      else if ((which_alternative == 1) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))
        {
	  return 1;
        }
      else
        {
	  return 11 /* 0xb */;
        }

    case 176:
      extract_constrain_insn_cached (insn);
      if (((which_alternative == 5) || (which_alternative == 7)) && ((((which_alternative == 4) || (which_alternative == 6)) && (((arm_is_6_or_7) == (MODEL_WBUF_YES)))) || (((which_alternative != 4) && (which_alternative != 6)) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))))
        {
	  return 5;
        }
      else if (((which_alternative != 5) && (which_alternative != 7)) && (((which_alternative == 4) || (which_alternative == 6)) && (((arm_is_6_or_7) == (MODEL_WBUF_YES)))))
        {
	  return 1;
        }
      else
        {
	  return 11 /* 0xb */;
        }

    case 610:
    case 601:
    case 192:
    case 175:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 3) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))
        {
	  return 5;
        }
      else if ((which_alternative == 2) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))
        {
	  return 1;
        }
      else
        {
	  return 11 /* 0xb */;
        }

    case 174:
      extract_constrain_insn_cached (insn);
      if (((which_alternative == 4) || (which_alternative == 6)) && ((((which_alternative == 3) || (which_alternative == 5)) && (((arm_is_6_or_7) == (MODEL_WBUF_YES)))) || (((which_alternative != 3) && (which_alternative != 5)) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))))
        {
	  return 7;
        }
      else if (((which_alternative != 4) && (which_alternative != 6)) && (((which_alternative == 3) || (which_alternative == 5)) && (((arm_is_6_or_7) == (MODEL_WBUF_YES)))))
        {
	  return 1;
        }
      else
        {
	  return 11 /* 0xb */;
        }

    case 173:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 2) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))
        {
	  return 7;
        }
      else if ((which_alternative == 1) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))
        {
	  return 1;
        }
      else
        {
	  return 11 /* 0xb */;
        }

    case 591:
    case 158:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 1) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))
        {
	  return 1;
        }
      else
        {
	  return 11 /* 0xb */;
        }

    case 655:
    case 654:
    case 653:
    case 652:
    case 649:
    case 648:
    case 645:
    case 644:
    case 643:
    case 642:
    case 639:
    case 638:
    case 637:
    case 636:
    case 633:
    case 630:
    case 628:
    case 620:
    case 619:
    case 618:
    case 617:
    case 616:
    case 615:
    case 608:
    case 604:
    case 597:
    case 596:
    case 595:
    case 594:
    case 593:
    case 578:
    case 577:
    case 576:
    case 575:
    case 349:
    case 344:
    case 343:
    case 342:
    case 341:
    case 340:
    case 339:
    case 336:
    case 335:
    case 332:
    case 331:
    case 330:
    case 329:
    case 326:
    case 325:
    case 324:
    case 323:
    case 320:
    case 265:
    case 262:
    case 261:
    case 260:
    case 259:
    case 258:
    case 257:
    case 256:
    case 255:
    case 254:
    case 253:
    case 252:
    case 251:
    case 250:
    case 249:
    case 248:
    case 247:
    case 206:
    case 205:
    case 204:
    case 203:
    case 202:
    case 201:
    case 189:
    case 185:
    case 180:
    case 179:
    case 178:
    case 169:
    case 168:
    case 167:
    case 166:
    case 165:
    case 163:
    case 162:
    case 161:
    case 160:
    case 142:
    case 141:
    case 140:
    case 139:
      if (((arm_is_6_or_7) == (MODEL_WBUF_YES)))
        {
	  return 1;
        }
      else
        {
	  return 11 /* 0xb */;
        }

    case 651:
    case 650:
    case 647:
    case 646:
    case 641:
    case 640:
    case 635:
    case 634:
    case 338:
    case 337:
    case 334:
    case 333:
    case 328:
    case 327:
    case 322:
    case 321:
    case 113:
      if (((arm_is_6_or_7) == (MODEL_WBUF_YES)))
        {
	  return 5;
        }
      else
        {
	  return 11 /* 0xb */;
        }

    case -1:
      if (GET_CODE (PATTERN (insn)) != ASM_INPUT
          && asm_noperands (PATTERN (insn)) < 0)
        fatal_insn_not_found (insn);
    default:
      return 11 /* 0xb */;

    }
}

extern unsigned int write_blockage_unit_blockage_range PARAMS ((rtx));
unsigned int
write_blockage_unit_blockage_range (insn)
     rtx insn;
{
  switch (recog_memoized (insn))
    {
    case 613:
    case 197:
      extract_constrain_insn_cached (insn);
      if (((which_alternative == 1) || (which_alternative == 3)) && (((which_alternative != 7) && ((which_alternative != 8) && ((which_alternative != 10) && ((which_alternative != 9) && ((which_alternative != 0) && (which_alternative != 4)))))) && (((arm_is_6_or_7) == (MODEL_WBUF_YES)))))
        {
	  return 1 /* min 0, max 1 */;
        }
      else
        {
	  return 65547 /* min 1, max 11 */;
        }

    case 199:
    case 196:
      extract_constrain_insn_cached (insn);
      if (((which_alternative == 2) || (which_alternative == 4)) && (((which_alternative != 1) && (which_alternative != 3)) && (((arm_is_6_or_7) == (MODEL_WBUF_YES)))))
        {
	  return 1 /* min 0, max 1 */;
        }
      else
        {
	  return 65547 /* min 1, max 11 */;
        }

    case 611:
    case 194:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 8) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))
        {
	  return 1 /* min 0, max 1 */;
        }
      else
        {
	  return 65547 /* min 1, max 11 */;
        }

    case 176:
      extract_constrain_insn_cached (insn);
      if (((which_alternative == 5) || (which_alternative == 7)) && (((which_alternative != 4) && (which_alternative != 6)) && (((arm_is_6_or_7) == (MODEL_WBUF_YES)))))
        {
	  return 1 /* min 0, max 1 */;
        }
      else
        {
	  return 65547 /* min 1, max 11 */;
        }

    case 610:
    case 601:
    case 192:
    case 175:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 3) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))
        {
	  return 1 /* min 0, max 1 */;
        }
      else
        {
	  return 65547 /* min 1, max 11 */;
        }

    case 174:
      extract_constrain_insn_cached (insn);
      if (((which_alternative == 4) || (which_alternative == 6)) && (((which_alternative != 3) && (which_alternative != 5)) && (((arm_is_6_or_7) == (MODEL_WBUF_YES)))))
        {
	  return 1 /* min 0, max 1 */;
        }
      else
        {
	  return 65547 /* min 1, max 11 */;
        }

    case 612:
    case 605:
    case 198:
    case 195:
    case 193:
    case 186:
    case 184:
    case 173:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 2) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))
        {
	  return 1 /* min 0, max 1 */;
        }
      else
        {
	  return 65547 /* min 1, max 11 */;
        }

    case 651:
    case 650:
    case 647:
    case 646:
    case 641:
    case 640:
    case 635:
    case 634:
    case 626:
    case 625:
    case 624:
    case 623:
    case 622:
    case 621:
    case 348:
    case 338:
    case 337:
    case 334:
    case 333:
    case 328:
    case 327:
    case 322:
    case 321:
    case 214:
    case 213:
    case 212:
    case 211:
    case 210:
    case 209:
    case 208:
    case 207:
    case 113:
      if (((arm_is_6_or_7) == (MODEL_WBUF_YES)))
        {
	  return 1 /* min 0, max 1 */;
        }
      else
        {
	  return 65547 /* min 1, max 11 */;
        }

    case -1:
      if (GET_CODE (PATTERN (insn)) != ASM_INPUT
          && asm_noperands (PATTERN (insn)) < 0)
        fatal_insn_not_found (insn);
    default:
      return 65547 /* min 1, max 11 */;

    }
}

extern int write_buf_unit_ready_cost PARAMS ((rtx));
int
write_buf_unit_ready_cost (insn)
     rtx insn;
{
  switch (recog_memoized (insn))
    {
    case 626:
    case 623:
    case 214:
    case 212:
    case 209:
      if (((arm_is_6_or_7) == (MODEL_WBUF_YES)))
        {
	  return 7;
        }
      else
        {
	  return 11 /* 0xb */;
        }

    case 625:
    case 622:
    case 213:
    case 211:
    case 208:
      if (((arm_is_6_or_7) == (MODEL_WBUF_YES)))
        {
	  return 9;
        }
      else
        {
	  return 11 /* 0xb */;
        }

    case 614:
    case 200:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 4) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))
        {
	  return 5;
        }
      else
        {
	  return 11 /* 0xb */;
        }

    case 199:
      extract_constrain_insn_cached (insn);
      if (((which_alternative == 2) || (which_alternative == 4)) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))
        {
	  return 7;
        }
      else
        {
	  return 11 /* 0xb */;
        }

    case 613:
    case 197:
      extract_constrain_insn_cached (insn);
      if (((which_alternative == 1) || (which_alternative == 3)) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))
        {
	  return 7;
        }
      else if ((which_alternative == 9) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))
        {
	  return 5;
        }
      else
        {
	  return 11 /* 0xb */;
        }

    case 196:
      extract_constrain_insn_cached (insn);
      if (((which_alternative == 2) || (which_alternative == 4)) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))
        {
	  return 5;
        }
      else
        {
	  return 11 /* 0xb */;
        }

    case 611:
    case 194:
      extract_constrain_insn_cached (insn);
      if (((which_alternative == 4) && (((arm_is_6_or_7) == (MODEL_WBUF_YES)))) || ((which_alternative == 8) && (((arm_is_6_or_7) == (MODEL_WBUF_YES)))))
        {
	  return 5;
        }
      else
        {
	  return 11 /* 0xb */;
        }

    case 612:
    case 605:
    case 195:
    case 193:
    case 186:
    case 184:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 2) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))
        {
	  return 5;
        }
      else
        {
	  return 11 /* 0xb */;
        }

    case 176:
      extract_constrain_insn_cached (insn);
      if (((which_alternative == 5) || (which_alternative == 7)) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))
        {
	  return 5;
        }
      else
        {
	  return 11 /* 0xb */;
        }

    case 610:
    case 601:
    case 192:
    case 175:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 3) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))
        {
	  return 5;
        }
      else
        {
	  return 11 /* 0xb */;
        }

    case 174:
      extract_constrain_insn_cached (insn);
      if (((which_alternative == 4) || (which_alternative == 6)) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))
        {
	  return 7;
        }
      else
        {
	  return 11 /* 0xb */;
        }

    case 198:
    case 173:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 2) && (((arm_is_6_or_7) == (MODEL_WBUF_YES))))
        {
	  return 7;
        }
      else
        {
	  return 11 /* 0xb */;
        }

    case 651:
    case 650:
    case 647:
    case 646:
    case 641:
    case 640:
    case 635:
    case 634:
    case 338:
    case 337:
    case 334:
    case 333:
    case 328:
    case 327:
    case 322:
    case 321:
    case 113:
      if (((arm_is_6_or_7) == (MODEL_WBUF_YES)))
        {
	  return 5;
        }
      else
        {
	  return 11 /* 0xb */;
        }

    case -1:
      if (GET_CODE (PATTERN (insn)) != ASM_INPUT
          && asm_noperands (PATTERN (insn)) < 0)
        fatal_insn_not_found (insn);
    default:
      return 11 /* 0xb */;

    }
}

extern unsigned int write_buf_unit_blockage_range PARAMS ((rtx));
unsigned int
write_buf_unit_blockage_range (insn)
     rtx insn;
{
  switch (recog_memoized (insn))
    {
    case -1:
      if (GET_CODE (PATTERN (insn)) != ASM_INPUT
          && asm_noperands (PATTERN (insn)) < 0)
        fatal_insn_not_found (insn);
    default:
      return 196614 /* min 3, max 6 */;

    }
}

extern int fpa_mem_unit_ready_cost PARAMS ((rtx));
int
fpa_mem_unit_ready_cost (insn)
     rtx insn;
{
  switch (recog_memoized (insn))
    {
    case -1:
      if (GET_CODE (PATTERN (insn)) != ASM_INPUT
          && asm_noperands (PATTERN (insn)) < 0)
        fatal_insn_not_found (insn);
    default:
      return 3;

    }
}

extern int fpa_unit_ready_cost PARAMS ((rtx));
int
fpa_unit_ready_cost (insn)
     rtx insn;
{
  switch (recog_memoized (insn))
    {
    case 613:
    case 197:
      extract_constrain_insn_cached (insn);
      if (((which_alternative == 5) || (which_alternative == 6)) && (((arm_fpu_attr) == (FPU_FPA))))
        {
	  return 2;
        }
      else
        {
	  return 71 /* 0x47 */;
        }

    case 614:
    case 611:
    case 200:
    case 194:
      extract_constrain_insn_cached (insn);
      if (((which_alternative == 0) || (which_alternative == 1)) && (((arm_fpu_attr) == (FPU_FPA))))
        {
	  return 2;
        }
      else
        {
	  return 71 /* 0x47 */;
        }

    case 586:
    case 585:
    case 584:
    case 230:
    case 229:
    case 228:
    case 227:
    case 226:
    case 225:
    case 224:
    case 223:
    case 222:
    case 221:
    case 153:
    case 152:
    case 151:
      if (((arm_fpu_attr) == (FPU_FPA)))
        {
	  return 1;
        }
      else
        {
	  return 71 /* 0x47 */;
        }

    case 583:
    case 582:
    case 581:
    case 150:
    case 149:
    case 148:
      if (((arm_fpu_attr) == (FPU_FPA)))
        {
	  return 5;
        }
      else
        {
	  return 71 /* 0x47 */;
        }

    case 600:
    case 599:
    case 598:
    case 589:
    case 588:
    case 587:
    case 574:
    case 573:
    case 572:
    case 571:
    case 570:
    case 569:
    case 568:
    case 567:
    case 244:
    case 242:
    case 172:
    case 171:
    case 170:
    case 156:
    case 155:
    case 154:
    case 138:
    case 137:
    case 136:
    case 135:
    case 132:
    case 131:
    case 130:
    case 129:
      if (((arm_fpu_attr) == (FPU_FPA)))
        {
	  return 2;
        }
      else
        {
	  return 71 /* 0x47 */;
        }

    case 547:
    case 546:
    case 545:
    case 544:
    case 541:
    case 540:
    case 539:
    case 538:
    case 75:
    case 74:
    case 73:
    case 72:
    case 69:
    case 68:
    case 67:
    case 66:
      if (((arm_fpu_attr) == (FPU_FPA)))
        {
	  return 59 /* 0x3b */;
        }
      else
        {
	  return 71 /* 0x47 */;
        }

    case 543:
    case 537:
    case 71:
    case 65:
      if (((arm_fpu_attr) == (FPU_FPA)))
        {
	  return 31 /* 0x1f */;
        }
      else
        {
	  return 71 /* 0x47 */;
        }

    case 536:
    case 535:
    case 534:
    case 533:
    case 532:
    case 64:
    case 63:
    case 62:
    case 61:
    case 60:
      if (((arm_fpu_attr) == (FPU_FPA)))
        {
	  return 9;
        }
      else
        {
	  return 71 /* 0x47 */;
        }

    case 531:
    case 59:
      if (((arm_fpu_attr) == (FPU_FPA)))
        {
	  return 6;
        }
      else
        {
	  return 71 /* 0x47 */;
        }

    case 522:
    case 521:
    case 520:
    case 519:
    case 518:
    case 516:
    case 515:
    case 514:
    case 513:
    case 512:
    case 511:
    case 42:
    case 41:
    case 40:
    case 39:
    case 38:
    case 37:
    case 25:
    case 24:
    case 23:
    case 22:
    case 21:
    case 20:
      if (((arm_fpu_attr) == (FPU_FPA)))
        {
	  return 4;
        }
      else
        {
	  return 71 /* 0x47 */;
        }

    case -1:
      if (GET_CODE (PATTERN (insn)) != ASM_INPUT
          && asm_noperands (PATTERN (insn)) < 0)
        fatal_insn_not_found (insn);
    default:
      return 71 /* 0x47 */;

    }
}

extern unsigned int fpa_unit_blockage_range PARAMS ((rtx));
unsigned int
fpa_unit_blockage_range (insn)
     rtx insn;
{
  switch (recog_memoized (insn))
    {
    case -1:
      if (GET_CODE (PATTERN (insn)) != ASM_INPUT
          && asm_noperands (PATTERN (insn)) < 0)
        fatal_insn_not_found (insn);
    default:
      return 131141 /* min 2, max 69 */;

    }
}

extern int function_units_used PARAMS ((rtx));
int
function_units_used (insn)
     rtx insn;
{
  enum attr_core_cycles attr_core_cycles = get_attr_core_cycles (insn);
  enum attr_type attr_type = get_attr_type (insn);
  enum attr_write_conflict attr_write_conflict = get_attr_write_conflict (insn);
  unsigned long accum = 0;

  accum |= (((((arm_fpu_attr) == (FPU_FPA))) && (((((((((attr_type == TYPE_FDIVX) || (attr_type == TYPE_FDIVD)) || (attr_type == TYPE_FDIVS)) || (attr_type == TYPE_FMUL)) || (attr_type == TYPE_FFMUL)) || (attr_type == TYPE_FARITH)) || (attr_type == TYPE_FFARITH)) || (attr_type == TYPE_R_2_F)) || (attr_type == TYPE_F_2_R))) ? (1) : (0));
  accum |= (((((arm_fpu_attr) == (FPU_FPA))) && (attr_type == TYPE_F_LOAD)) ? (2) : (0));
  accum |= (((((arm_is_6_or_7) == (MODEL_WBUF_YES))) && (((((attr_type == TYPE_STORE1) || (attr_type == TYPE_R_MEM_F)) || (attr_type == TYPE_STORE2)) || (attr_type == TYPE_STORE3)) || (attr_type == TYPE_STORE4))) ? (4) : (0));
  accum |= (((attr_core_cycles == CORE_CYCLES_SINGLE) || (((((arm_ld_sched) == (LDSCHED_YES))) && ((attr_type == TYPE_STORE1) || (attr_type == TYPE_LOAD))) || ((((((arm_ld_sched) == (LDSCHED_YES))) && (attr_type == TYPE_LOAD)) && (((arm_is_xscale) == (IS_XSCALE_YES)))) || (((! (((arm_ld_sched) == (LDSCHED_YES)))) && ((attr_type == TYPE_LOAD) || (attr_type == TYPE_STORE1))) || (((((arm_fpu_attr) == (FPU_FPA))) && ((((attr_type == TYPE_F_LOAD) || (attr_type == TYPE_F_STORE)) || (attr_type == TYPE_R_MEM_F)) || (attr_type == TYPE_F_MEM_R))) || (((((arm_ld_sched) == (LDSCHED_NO))) && (attr_type == TYPE_MULT)) || ((((((arm_ld_sched) == (LDSCHED_YES))) && (((arm_is_strong) == (IS_STRONGARM_NO)))) && (attr_type == TYPE_MULT)) || ((((((arm_ld_sched) == (LDSCHED_YES))) && (((arm_is_strong) == (IS_STRONGARM_YES)))) && (attr_type == TYPE_MULT)) || ((attr_type == TYPE_STORE2) || ((attr_type == TYPE_STORE3) || ((attr_type == TYPE_STORE4) || ((attr_core_cycles == CORE_CYCLES_MULTI) && (! ((attr_type == TYPE_MULT) || ((attr_type == TYPE_LOAD) || ((attr_type == TYPE_STORE1) || ((attr_type == TYPE_STORE2) || ((attr_type == TYPE_STORE3) || (attr_type == TYPE_STORE4))))))))))))))))))) ? (16) : (0));
  accum |= (((((arm_is_6_or_7) == (MODEL_WBUF_YES))) && (((((attr_type == TYPE_STORE1) || (attr_type == TYPE_STORE2)) || (attr_type == TYPE_STORE3)) || (attr_type == TYPE_STORE4)) || (attr_write_conflict == WRITE_CONFLICT_YES))) ? (8) : (0));

  if (accum && accum == (accum & -accum))
    {
      int i;
      for (i = 0; accum >>= 1; ++i) continue;
      accum = i;
    }
  else
    accum = ~accum;
  return accum;
}

extern enum attr_core_cycles get_attr_core_cycles PARAMS ((rtx));
enum attr_core_cycles
get_attr_core_cycles (insn)
     rtx insn;
{
  switch (recog_memoized (insn))
    {
    case 614:
    case 200:
      extract_constrain_insn_cached (insn);
      if ((((which_alternative != 0) && (which_alternative != 1)) && ((which_alternative != 2) && ((which_alternative != 3) && ((which_alternative != 4) && (which_alternative != 5))))) || ((which_alternative == 0) || (which_alternative == 1)))
        {
	  return CORE_CYCLES_SINGLE;
        }
      else
        {
	  return CORE_CYCLES_MULTI;
        }

    case 613:
    case 197:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 2) || ((which_alternative == 5) || (which_alternative == 6)))
        {
	  return CORE_CYCLES_SINGLE;
        }
      else
        {
	  return CORE_CYCLES_MULTI;
        }

    case 199:
    case 196:
      extract_constrain_insn_cached (insn);
      if ((which_alternative != 1) && ((which_alternative != 2) && ((which_alternative != 3) && (which_alternative != 4))))
        {
	  return CORE_CYCLES_SINGLE;
        }
      else
        {
	  return CORE_CYCLES_MULTI;
        }

    case 611:
    case 194:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 6) || ((which_alternative == 0) || (which_alternative == 1)))
        {
	  return CORE_CYCLES_SINGLE;
        }
      else
        {
	  return CORE_CYCLES_MULTI;
        }

    case 193:
    case 184:
      extract_constrain_insn_cached (insn);
      if ((which_alternative != 1) && (which_alternative != 2))
        {
	  return CORE_CYCLES_SINGLE;
        }
      else
        {
	  return CORE_CYCLES_MULTI;
        }

    case 176:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || ((which_alternative == 1) || ((which_alternative == 2) || ((which_alternative == 3) || (which_alternative == 8)))))
        {
	  return CORE_CYCLES_SINGLE;
        }
      else
        {
	  return CORE_CYCLES_MULTI;
        }

    case 610:
    case 607:
    case 606:
    case 605:
    case 601:
    case 192:
    case 188:
    case 187:
    case 186:
    case 175:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  return CORE_CYCLES_SINGLE;
        }
      else
        {
	  return CORE_CYCLES_MULTI;
        }

    case 174:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || ((which_alternative == 1) || ((which_alternative == 2) || (which_alternative == 7))))
        {
	  return CORE_CYCLES_SINGLE;
        }
      else
        {
	  return CORE_CYCLES_MULTI;
        }

    case 612:
    case 591:
    case 198:
    case 195:
    case 173:
    case 158:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  return CORE_CYCLES_SINGLE;
        }
      else
        {
	  return CORE_CYCLES_MULTI;
        }

    case 655:
    case 654:
    case 653:
    case 652:
    case 651:
    case 650:
    case 649:
    case 648:
    case 647:
    case 646:
    case 645:
    case 644:
    case 643:
    case 642:
    case 641:
    case 640:
    case 639:
    case 638:
    case 637:
    case 636:
    case 635:
    case 634:
    case 633:
    case 630:
    case 628:
    case 626:
    case 625:
    case 624:
    case 623:
    case 622:
    case 621:
    case 620:
    case 619:
    case 618:
    case 617:
    case 616:
    case 615:
    case 608:
    case 604:
    case 597:
    case 596:
    case 595:
    case 594:
    case 593:
    case 586:
    case 585:
    case 584:
    case 583:
    case 582:
    case 581:
    case 578:
    case 577:
    case 576:
    case 575:
    case 530:
    case 529:
    case 528:
    case 527:
    case 526:
    case 525:
    case 524:
    case 523:
    case 349:
    case 348:
    case 344:
    case 343:
    case 342:
    case 341:
    case 340:
    case 339:
    case 338:
    case 337:
    case 336:
    case 335:
    case 334:
    case 333:
    case 332:
    case 331:
    case 330:
    case 329:
    case 328:
    case 327:
    case 326:
    case 325:
    case 324:
    case 323:
    case 322:
    case 321:
    case 320:
    case 279:
    case 278:
    case 277:
    case 265:
    case 262:
    case 261:
    case 260:
    case 259:
    case 258:
    case 257:
    case 256:
    case 255:
    case 254:
    case 253:
    case 252:
    case 251:
    case 250:
    case 249:
    case 248:
    case 247:
    case 230:
    case 229:
    case 228:
    case 227:
    case 226:
    case 225:
    case 224:
    case 223:
    case 222:
    case 221:
    case 214:
    case 213:
    case 212:
    case 211:
    case 210:
    case 209:
    case 208:
    case 207:
    case 206:
    case 205:
    case 204:
    case 203:
    case 202:
    case 201:
    case 189:
    case 185:
    case 180:
    case 179:
    case 178:
    case 169:
    case 168:
    case 167:
    case 166:
    case 165:
    case 163:
    case 162:
    case 161:
    case 160:
    case 153:
    case 152:
    case 151:
    case 150:
    case 149:
    case 148:
    case 142:
    case 141:
    case 140:
    case 139:
    case 113:
    case 58:
    case 57:
    case 56:
    case 55:
    case 54:
    case 53:
    case 52:
    case 51:
    case 50:
    case 49:
    case 48:
    case 47:
    case 46:
    case 45:
    case 44:
    case 43:
      return CORE_CYCLES_MULTI;

    case -1:
      if (GET_CODE (PATTERN (insn)) != ASM_INPUT
          && asm_noperands (PATTERN (insn)) < 0)
        fatal_insn_not_found (insn);
    default:
      return CORE_CYCLES_SINGLE;

    }
}

extern enum attr_conds get_attr_conds PARAMS ((rtx));
enum attr_conds
get_attr_conds (insn)
     rtx insn;
{
  switch (recog_memoized (insn))
    {
    case 258:
    case 257:
    case 256:
    case 255:
    case 254:
    case 253:
    case 252:
    case 251:
    case 250:
    case 249:
    case 248:
    case 247:
      if (((arm_prog_mode) == (PROG_MODE_PROG32)))
        {
	  return CONDS_CLOB;
        }
      else
        {
	  return CONDS_NOCOND;
        }

    case 134:
    case 133:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  return CONDS_CLOB;
        }
      else
        {
	  return CONDS_NOCOND;
        }

    case 232:
    case 233:
    case 235:
    case 236:
      return CONDS_JUMP_CLOB;

    case -1:
      if (GET_CODE (PATTERN (insn)) != ASM_INPUT
          && asm_noperands (PATTERN (insn)) < 0)
        fatal_insn_not_found (insn);
    case 1:
    case 2:
    case 3:
    case 26:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
    case 85:
    case 109:
    case 110:
    case 111:
    case 112:
    case 113:
    case 114:
    case 125:
    case 263:
    case 266:
    case 282:
    case 284:
    case 285:
    case 290:
    case 291:
    case 292:
    case 294:
    case 296:
    case 298:
    case 300:
    case 302:
    case 304:
    case 306:
    case 308:
    case 310:
    case 312:
    case 314:
    case 316:
    case 318:
    case 346:
    case 347:
      return CONDS_CLOB;

    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 35:
    case 45:
    case 46:
    case 48:
    case 49:
    case 82:
    case 83:
    case 84:
    case 92:
    case 93:
    case 99:
    case 100:
    case 106:
    case 107:
    case 120:
    case 121:
    case 123:
    case 124:
    case 146:
    case 147:
    case 164:
    case 183:
    case 217:
    case 218:
    case 219:
    case 220:
    case 221:
    case 222:
    case 223:
    case 224:
    case 225:
    case 226:
    case 227:
    case 228:
    case 229:
    case 230:
    case 231:
    case 269:
    case 270:
    case 272:
    case 273:
    case 275:
    case 276:
    case 278:
    case 279:
    case 286:
    case 287:
    case 288:
    case 289:
      return CONDS_SET;

    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 36:
    case 234:
    case 237:
    case 238:
    case 239:
    case 240:
    case 241:
    case 242:
    case 243:
    case 244:
    case 260:
    case 261:
    case 280:
    case 281:
    case 283:
    case 293:
    case 295:
    case 297:
    case 299:
    case 301:
    case 303:
    case 305:
    case 307:
    case 309:
    case 311:
    case 313:
    case 315:
    case 317:
    case 319:
    case 345:
      return CONDS_USE;

    default:
      return CONDS_NOCOND;

    }
}

extern enum attr_far_jump get_attr_far_jump PARAMS ((rtx));
enum attr_far_jump
get_attr_far_jump (insn)
     rtx insn;
{
  switch (recog_memoized (insn))
    {
    case 215:
    case 216:
      if (get_attr_length (insn) == 8)
        {
	  return FAR_JUMP_YES;
        }
      else
        {
	  return FAR_JUMP_NO;
        }

    case 246:
      if (get_attr_length (insn) == 4)
        {
	  return FAR_JUMP_YES;
        }
      else
        {
	  return FAR_JUMP_NO;
        }

    case -1:
      if (GET_CODE (PATTERN (insn)) != ASM_INPUT
          && asm_noperands (PATTERN (insn)) < 0)
        fatal_insn_not_found (insn);
    default:
      return FAR_JUMP_NO;

    }
}

extern int get_attr_neg_pool_range PARAMS ((rtx));
int
get_attr_neg_pool_range (insn)
     rtx insn;
{
  switch (recog_memoized (insn))
    {
    case 614:
    case 200:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  return 0;
        }
      else if (which_alternative == 2)
        {
	  return 1012 /* 0x3f4 */;
        }
      else
        {
	  return 0;
        }

    case 613:
    case 197:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || ((which_alternative == 1) || ((which_alternative == 2) || (which_alternative == 3))))
        {
	  return 0;
        }
      else if (which_alternative == 4)
        {
	  return 244 /* 0xf4 */;
        }
      else if ((which_alternative == 5) || (which_alternative == 6))
        {
	  return 0;
        }
      else if (which_alternative == 7)
        {
	  return 1012 /* 0x3f4 */;
        }
      else
        {
	  return 0;
        }

    case 611:
    case 194:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  return 0;
        }
      else if (which_alternative == 2)
        {
	  return 1012 /* 0x3f4 */;
        }
      else if ((which_alternative == 3) || ((which_alternative == 4) || ((which_alternative == 5) || (which_alternative == 6))))
        {
	  return 0;
        }
      else if (which_alternative == 7)
        {
	  return 4084 /* 0xff4 */;
        }
      else
        {
	  return 0;
        }

    case 607:
    case 188:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  return 0;
        }
      else
        {
	  return 4084 /* 0xff4 */;
        }

    case 605:
    case 186:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || ((which_alternative == 1) || (which_alternative == 2)))
        {
	  return 0;
        }
      else
        {
	  return 244 /* 0xf4 */;
        }

    case 601:
    case 175:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  return 0;
        }
      else if (which_alternative == 2)
        {
	  return 4084 /* 0xff4 */;
        }
      else
        {
	  return 0;
        }

    case 173:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  return 0;
        }
      else if (which_alternative == 1)
        {
	  return 1012 /* 0x3f4 */;
        }
      else
        {
	  return 0;
        }

    case 180:
      if (((thumb_code) == (IS_THUMB_YES)))
        {
	  return 0;
        }
      else
        {
	  return 4084 /* 0xff4 */;
        }

    case 158:
    case 591:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  return 0;
        }
      else
        {
	  return 4084 /* 0xff4 */;
        }

    case 161:
    case 166:
    case 167:
    case 168:
    case 198:
    case 593:
    case 595:
    case 596:
    case 597:
      return 244 /* 0xf4 */;

    case 195:
    case 612:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  return 0;
        }
      else if (which_alternative == 1)
        {
	  return 4084 /* 0xff4 */;
        }
      else
        {
	  return 0;
        }

    case 163:
    case 178:
    case 187:
    case 189:
    case 265:
    case 594:
    case 606:
    case 608:
    case 630:
      return 4084 /* 0xff4 */;

    case -1:
      if (GET_CODE (PATTERN (insn)) != ASM_INPUT
          && asm_noperands (PATTERN (insn)) < 0)
        fatal_insn_not_found (insn);
    default:
      return 0;

    }
}

extern enum attr_predicable get_attr_predicable PARAMS ((rtx));
enum attr_predicable
get_attr_predicable (insn)
     rtx insn;
{
  switch (recog_memoized (insn))
    {
    case -1:
      if (GET_CODE (PATTERN (insn)) != ASM_INPUT
          && asm_noperands (PATTERN (insn)) < 0)
        fatal_insn_not_found (insn);
    case 0:
    case 1:
    case 2:
    case 3:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
    case 33:
    case 35:
    case 36:
    case 37:
    case 44:
    case 45:
    case 46:
    case 48:
    case 49:
    case 56:
    case 57:
    case 58:
    case 77:
    case 78:
    case 79:
    case 81:
    case 82:
    case 83:
    case 84:
    case 85:
    case 90:
    case 92:
    case 93:
    case 98:
    case 99:
    case 100:
    case 105:
    case 106:
    case 107:
    case 109:
    case 110:
    case 111:
    case 112:
    case 113:
    case 114:
    case 115:
    case 116:
    case 117:
    case 118:
    case 120:
    case 121:
    case 123:
    case 124:
    case 125:
    case 126:
    case 128:
    case 133:
    case 134:
    case 145:
    case 146:
    case 147:
    case 160:
    case 162:
    case 164:
    case 165:
    case 169:
    case 173:
    case 174:
    case 176:
    case 178:
    case 179:
    case 180:
    case 181:
    case 183:
    case 184:
    case 191:
    case 193:
    case 196:
    case 198:
    case 199:
    case 213:
    case 214:
    case 215:
    case 216:
    case 217:
    case 218:
    case 219:
    case 220:
    case 221:
    case 222:
    case 223:
    case 224:
    case 225:
    case 226:
    case 227:
    case 228:
    case 229:
    case 230:
    case 231:
    case 232:
    case 233:
    case 234:
    case 235:
    case 236:
    case 237:
    case 238:
    case 239:
    case 240:
    case 241:
    case 242:
    case 243:
    case 244:
    case 246:
    case 247:
    case 248:
    case 249:
    case 250:
    case 251:
    case 252:
    case 253:
    case 254:
    case 255:
    case 256:
    case 257:
    case 258:
    case 260:
    case 261:
    case 262:
    case 263:
    case 266:
    case 267:
    case 269:
    case 270:
    case 272:
    case 273:
    case 274:
    case 275:
    case 276:
    case 277:
    case 278:
    case 279:
    case 280:
    case 281:
    case 282:
    case 283:
    case 284:
    case 285:
    case 286:
    case 287:
    case 288:
    case 289:
    case 290:
    case 291:
    case 292:
    case 293:
    case 294:
    case 295:
    case 296:
    case 297:
    case 298:
    case 299:
    case 300:
    case 301:
    case 302:
    case 303:
    case 304:
    case 305:
    case 306:
    case 307:
    case 308:
    case 309:
    case 310:
    case 311:
    case 312:
    case 313:
    case 314:
    case 315:
    case 316:
    case 317:
    case 318:
    case 319:
    case 343:
    case 344:
    case 345:
    case 346:
    case 347:
    case 348:
    case 349:
    case 350:
    case 351:
    case 352:
    case 353:
    case 354:
    case 355:
    case 356:
    case 357:
    case 358:
    case 486:
    case 487:
    case 488:
    case 489:
    case 490:
    case 491:
    case 492:
    case 494:
    case 495:
    case 496:
    case 497:
    case 498:
    case 499:
      return PREDICABLE_NO;

    default:
      return PREDICABLE_YES;

    }
}

extern int get_attr_pool_range PARAMS ((rtx));
int
get_attr_pool_range (insn)
     rtx insn;
{
  switch (recog_memoized (insn))
    {
    case 614:
    case 200:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  return 0;
        }
      else if (which_alternative == 2)
        {
	  return 1024 /* 0x400 */;
        }
      else
        {
	  return 0;
        }

    case 613:
    case 197:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || ((which_alternative == 1) || ((which_alternative == 2) || (which_alternative == 3))))
        {
	  return 0;
        }
      else if (which_alternative == 4)
        {
	  return 252 /* 0xfc */;
        }
      else if ((which_alternative == 5) || (which_alternative == 6))
        {
	  return 0;
        }
      else if (which_alternative == 7)
        {
	  return 1024 /* 0x400 */;
        }
      else
        {
	  return 0;
        }

    case 199:
    case 196:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || ((which_alternative == 1) || (which_alternative == 2)))
        {
	  return 0;
        }
      else if (which_alternative == 3)
        {
	  return 1020 /* 0x3fc */;
        }
      else
        {
	  return 0;
        }

    case 611:
    case 194:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  return 0;
        }
      else if (which_alternative == 2)
        {
	  return 1024 /* 0x400 */;
        }
      else if ((which_alternative == 3) || ((which_alternative == 4) || ((which_alternative == 5) || (which_alternative == 6))))
        {
	  return 0;
        }
      else if (which_alternative == 7)
        {
	  return 4096 /* 0x1000 */;
        }
      else
        {
	  return 0;
        }

    case 193:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  return 0;
        }
      else if (which_alternative == 1)
        {
	  return 32 /* 0x20 */;
        }
      else
        {
	  return 0;
        }

    case 607:
    case 188:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  return 0;
        }
      else
        {
	  return 4092 /* 0xffc */;
        }

    case 605:
    case 186:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || ((which_alternative == 1) || (which_alternative == 2)))
        {
	  return 0;
        }
      else
        {
	  return 256 /* 0x100 */;
        }

    case 184:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  return 0;
        }
      else if (which_alternative == 1)
        {
	  return 64 /* 0x40 */;
        }
      else
        {
	  return 0;
        }

    case 176:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || ((which_alternative == 1) || ((which_alternative == 2) || ((which_alternative == 3) || ((which_alternative == 4) || (which_alternative == 5))))))
        {
	  return 0;
        }
      else if (which_alternative == 6)
        {
	  return 1020 /* 0x3fc */;
        }
      else
        {
	  return 0;
        }

    case 601:
    case 175:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  return 0;
        }
      else if (which_alternative == 2)
        {
	  return 4096 /* 0x1000 */;
        }
      else
        {
	  return 0;
        }

    case 174:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || ((which_alternative == 1) || ((which_alternative == 2) || ((which_alternative == 3) || (which_alternative == 4)))))
        {
	  return 0;
        }
      else if (which_alternative == 5)
        {
	  return 1020 /* 0x3fc */;
        }
      else
        {
	  return 0;
        }

    case 160:
      return 60 /* 0x3c */;

    case 165:
      return 1020 /* 0x3fc */;

    case 162:
    case 169:
      return 32 /* 0x20 */;

    case 173:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  return 0;
        }
      else if (which_alternative == 1)
        {
	  return 1020 /* 0x3fc */;
        }
      else
        {
	  return 0;
        }

    case 179:
      return 1024 /* 0x400 */;

    case 180:
      if (((thumb_code) == (IS_THUMB_YES)))
        {
	  return 1024 /* 0x400 */;
        }
      else
        {
	  return 4096 /* 0x1000 */;
        }

    case 198:
      return 252 /* 0xfc */;

    case 158:
    case 591:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  return 0;
        }
      else
        {
	  return 4092 /* 0xffc */;
        }

    case 161:
    case 166:
    case 167:
    case 168:
    case 593:
    case 595:
    case 596:
    case 597:
      return 256 /* 0x100 */;

    case 195:
    case 612:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  return 0;
        }
      else if (which_alternative == 1)
        {
	  return 4096 /* 0x1000 */;
        }
      else
        {
	  return 0;
        }

    case 163:
    case 178:
    case 187:
    case 189:
    case 265:
    case 594:
    case 606:
    case 608:
    case 630:
      return 4096 /* 0x1000 */;

    case -1:
      if (GET_CODE (PATTERN (insn)) != ASM_INPUT
          && asm_noperands (PATTERN (insn)) < 0)
        fatal_insn_not_found (insn);
    default:
      return 0;

    }
}

extern int get_attr_shift PARAMS ((rtx));
int
get_attr_shift (insn)
     rtx insn;
{
  switch (recog_memoized (insn))
    {
    case 91:
    case 307:
    case 309:
    case 554:
      return 2;

    case 119:
    case 120:
    case 121:
    case 122:
    case 123:
    case 124:
    case 133:
    case 134:
    case 159:
    case 218:
    case 219:
    case 220:
    case 311:
    case 564:
    case 565:
    case 592:
      return 1;

    case 268:
    case 269:
    case 270:
    case 631:
      return 4;

    case 271:
    case 272:
    case 273:
    case 275:
    case 276:
    case 632:
      return 3;

    case -1:
      if (GET_CODE (PATTERN (insn)) != ASM_INPUT
          && asm_noperands (PATTERN (insn)) < 0)
        fatal_insn_not_found (insn);
    default:
      return 0;

    }
}

extern enum attr_type get_attr_type PARAMS ((rtx));
enum attr_type
get_attr_type (insn)
     rtx insn;
{
  switch (recog_memoized (insn))
    {
    case 614:
    case 200:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  return TYPE_FFARITH;
        }
      else if (which_alternative == 2)
        {
	  return TYPE_F_LOAD;
        }
      else if (which_alternative == 3)
        {
	  return TYPE_F_STORE;
        }
      else if (which_alternative == 4)
        {
	  return TYPE_R_MEM_F;
        }
      else if (which_alternative == 5)
        {
	  return TYPE_F_MEM_R;
        }
      else
        {
	  return TYPE_NORMAL;
        }

    case 613:
    case 197:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  return TYPE_LOAD;
        }
      else if (which_alternative == 1)
        {
	  return TYPE_STORE2;
        }
      else if (which_alternative == 2)
        {
	  return TYPE_NORMAL;
        }
      else if (which_alternative == 3)
        {
	  return TYPE_STORE2;
        }
      else if (which_alternative == 4)
        {
	  return TYPE_LOAD;
        }
      else if ((which_alternative == 5) || (which_alternative == 6))
        {
	  return TYPE_FFARITH;
        }
      else if (which_alternative == 7)
        {
	  return TYPE_F_LOAD;
        }
      else if (which_alternative == 8)
        {
	  return TYPE_F_STORE;
        }
      else if (which_alternative == 9)
        {
	  return TYPE_R_MEM_F;
        }
      else
        {
	  return TYPE_F_MEM_R;
        }

    case 196:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  return TYPE_NORMAL;
        }
      else if (which_alternative == 1)
        {
	  return TYPE_LOAD;
        }
      else if (which_alternative == 2)
        {
	  return TYPE_STORE1;
        }
      else if (which_alternative == 3)
        {
	  return TYPE_LOAD;
        }
      else if (which_alternative == 4)
        {
	  return TYPE_STORE1;
        }
      else
        {
	  return TYPE_NORMAL;
        }

    case 611:
    case 194:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  return TYPE_FFARITH;
        }
      else if (which_alternative == 2)
        {
	  return TYPE_F_LOAD;
        }
      else if (which_alternative == 3)
        {
	  return TYPE_F_STORE;
        }
      else if (which_alternative == 4)
        {
	  return TYPE_R_MEM_F;
        }
      else if (which_alternative == 5)
        {
	  return TYPE_F_MEM_R;
        }
      else if (which_alternative == 6)
        {
	  return TYPE_NORMAL;
        }
      else if (which_alternative == 7)
        {
	  return TYPE_LOAD;
        }
      else
        {
	  return TYPE_STORE1;
        }

    case 607:
    case 606:
    case 188:
    case 187:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  return TYPE_NORMAL;
        }
      else
        {
	  return TYPE_LOAD;
        }

    case 605:
    case 186:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  return TYPE_NORMAL;
        }
      else if (which_alternative == 2)
        {
	  return TYPE_STORE1;
        }
      else
        {
	  return TYPE_LOAD;
        }

    case 193:
    case 184:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  return TYPE_NORMAL;
        }
      else if (which_alternative == 1)
        {
	  return TYPE_LOAD;
        }
      else if (which_alternative == 2)
        {
	  return TYPE_STORE1;
        }
      else
        {
	  return TYPE_NORMAL;
        }

    case 176:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || ((which_alternative == 1) || ((which_alternative == 2) || (which_alternative == 3))))
        {
	  return TYPE_NORMAL;
        }
      else if (which_alternative == 4)
        {
	  return TYPE_LOAD;
        }
      else if (which_alternative == 5)
        {
	  return TYPE_STORE1;
        }
      else if (which_alternative == 6)
        {
	  return TYPE_LOAD;
        }
      else if (which_alternative == 7)
        {
	  return TYPE_STORE1;
        }
      else
        {
	  return TYPE_NORMAL;
        }

    case 610:
    case 601:
    case 192:
    case 175:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  return TYPE_NORMAL;
        }
      else if (which_alternative == 2)
        {
	  return TYPE_LOAD;
        }
      else
        {
	  return TYPE_STORE1;
        }

    case 174:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || ((which_alternative == 1) || (which_alternative == 2)))
        {
	  return TYPE_NORMAL;
        }
      else if (which_alternative == 3)
        {
	  return TYPE_LOAD;
        }
      else if (which_alternative == 4)
        {
	  return TYPE_STORE2;
        }
      else if (which_alternative == 5)
        {
	  return TYPE_LOAD;
        }
      else if (which_alternative == 6)
        {
	  return TYPE_STORE2;
        }
      else
        {
	  return TYPE_NORMAL;
        }

    case 173:
    case 198:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  return TYPE_NORMAL;
        }
      else if (which_alternative == 1)
        {
	  return TYPE_LOAD;
        }
      else
        {
	  return TYPE_STORE2;
        }

    case 199:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  return TYPE_NORMAL;
        }
      else if (which_alternative == 1)
        {
	  return TYPE_LOAD;
        }
      else if (which_alternative == 2)
        {
	  return TYPE_STORE2;
        }
      else if (which_alternative == 3)
        {
	  return TYPE_LOAD;
        }
      else if (which_alternative == 4)
        {
	  return TYPE_STORE2;
        }
      else
        {
	  return TYPE_NORMAL;
        }

    case 158:
    case 591:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  return TYPE_NORMAL;
        }
      else
        {
	  return TYPE_LOAD;
        }

    case 195:
    case 612:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  return TYPE_NORMAL;
        }
      else if (which_alternative == 1)
        {
	  return TYPE_LOAD;
        }
      else
        {
	  return TYPE_STORE1;
        }

    case 207:
    case 210:
    case 348:
    case 621:
    case 624:
      return TYPE_STORE4;

    case 208:
    case 211:
    case 213:
    case 622:
    case 625:
      return TYPE_STORE3;

    case 209:
    case 212:
    case 214:
    case 623:
    case 626:
      return TYPE_STORE2;

    case 113:
    case 321:
    case 322:
    case 327:
    case 328:
    case 333:
    case 334:
    case 337:
    case 338:
    case 634:
    case 635:
    case 640:
    case 641:
    case 646:
    case 647:
    case 650:
    case 651:
      return TYPE_STORE1;

    case 160:
    case 161:
    case 162:
    case 163:
    case 165:
    case 166:
    case 167:
    case 168:
    case 169:
    case 178:
    case 179:
    case 180:
    case 185:
    case 189:
    case 201:
    case 202:
    case 203:
    case 204:
    case 205:
    case 206:
    case 259:
    case 260:
    case 261:
    case 265:
    case 320:
    case 323:
    case 324:
    case 325:
    case 326:
    case 329:
    case 330:
    case 331:
    case 332:
    case 335:
    case 336:
    case 339:
    case 340:
    case 341:
    case 342:
    case 593:
    case 594:
    case 595:
    case 596:
    case 597:
    case 604:
    case 608:
    case 615:
    case 616:
    case 617:
    case 618:
    case 619:
    case 620:
    case 628:
    case 630:
    case 633:
    case 636:
    case 637:
    case 638:
    case 639:
    case 642:
    case 643:
    case 644:
    case 645:
    case 648:
    case 649:
    case 652:
    case 653:
    case 654:
    case 655:
      return TYPE_LOAD;

    case 247:
    case 248:
    case 249:
    case 250:
    case 251:
    case 252:
    case 253:
    case 254:
    case 255:
    case 256:
    case 257:
    case 258:
      return TYPE_CALL;

    case 148:
    case 149:
    case 150:
    case 581:
    case 582:
    case 583:
      return TYPE_R_2_F;

    case 151:
    case 152:
    case 153:
    case 221:
    case 222:
    case 223:
    case 224:
    case 225:
    case 226:
    case 227:
    case 228:
    case 229:
    case 230:
    case 584:
    case 585:
    case 586:
      return TYPE_F_2_R;

    case 349:
      return TYPE_F_STORE;

    case 139:
    case 140:
    case 141:
    case 142:
    case 575:
    case 576:
    case 577:
    case 578:
      return TYPE_FLOAT_EM;

    case 129:
    case 130:
    case 131:
    case 132:
    case 135:
    case 136:
    case 137:
    case 138:
    case 154:
    case 155:
    case 156:
    case 170:
    case 171:
    case 172:
    case 242:
    case 244:
    case 567:
    case 568:
    case 569:
    case 570:
    case 571:
    case 572:
    case 573:
    case 574:
    case 587:
    case 588:
    case 589:
    case 598:
    case 599:
    case 600:
      return TYPE_FFARITH;

    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 37:
    case 38:
    case 39:
    case 40:
    case 41:
    case 42:
    case 511:
    case 512:
    case 513:
    case 514:
    case 515:
    case 516:
    case 518:
    case 519:
    case 520:
    case 521:
    case 522:
      return TYPE_FARITH;

    case 59:
    case 531:
      return TYPE_FFMUL;

    case 60:
    case 61:
    case 62:
    case 63:
    case 64:
    case 532:
    case 533:
    case 534:
    case 535:
    case 536:
      return TYPE_FMUL;

    case 65:
    case 71:
    case 537:
    case 543:
      return TYPE_FDIVS;

    case 66:
    case 67:
    case 68:
    case 69:
    case 72:
    case 73:
    case 74:
    case 75:
    case 538:
    case 539:
    case 540:
    case 541:
    case 544:
    case 545:
    case 546:
    case 547:
      return TYPE_FDIVD;

    case 70:
    case 76:
    case 542:
    case 548:
      return TYPE_FDIVX;

    case 262:
    case 343:
    case 344:
      return TYPE_BLOCK;

    case 43:
    case 44:
    case 45:
    case 46:
    case 47:
    case 48:
    case 49:
    case 50:
    case 51:
    case 52:
    case 53:
    case 54:
    case 55:
    case 56:
    case 57:
    case 58:
    case 277:
    case 278:
    case 279:
    case 523:
    case 524:
    case 525:
    case 526:
    case 527:
    case 528:
    case 529:
    case 530:
      return TYPE_MULT;

    case -1:
      if (GET_CODE (PATTERN (insn)) != ASM_INPUT
          && asm_noperands (PATTERN (insn)) < 0)
        fatal_insn_not_found (insn);
    default:
      return TYPE_NORMAL;

    }
}

extern enum attr_write_conflict get_attr_write_conflict PARAMS ((rtx));
enum attr_write_conflict
get_attr_write_conflict (insn)
     rtx insn;
{
  switch (recog_memoized (insn))
    {
    case 614:
    case 200:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 2) || ((which_alternative == 3) || ((which_alternative == 5) || (which_alternative == 4))))
        {
	  return WRITE_CONFLICT_YES;
        }
      else
        {
	  return WRITE_CONFLICT_NO;
        }

    case 613:
    case 197:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 7) || ((which_alternative == 8) || ((which_alternative == 10) || ((which_alternative == 9) || ((which_alternative == 0) || (which_alternative == 4))))))
        {
	  return WRITE_CONFLICT_YES;
        }
      else
        {
	  return WRITE_CONFLICT_NO;
        }

    case 199:
    case 196:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 1) || (which_alternative == 3))
        {
	  return WRITE_CONFLICT_YES;
        }
      else
        {
	  return WRITE_CONFLICT_NO;
        }

    case 611:
    case 194:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 2) || ((which_alternative == 3) || ((which_alternative == 5) || ((which_alternative == 4) || (which_alternative == 7)))))
        {
	  return WRITE_CONFLICT_YES;
        }
      else
        {
	  return WRITE_CONFLICT_NO;
        }

    case 607:
    case 606:
    case 188:
    case 187:
      extract_constrain_insn_cached (insn);
      if ((which_alternative != 0) && (which_alternative != 1))
        {
	  return WRITE_CONFLICT_YES;
        }
      else
        {
	  return WRITE_CONFLICT_NO;
        }

    case 605:
    case 186:
      extract_constrain_insn_cached (insn);
      if ((which_alternative != 0) && ((which_alternative != 1) && (which_alternative != 2)))
        {
	  return WRITE_CONFLICT_YES;
        }
      else
        {
	  return WRITE_CONFLICT_NO;
        }

    case 176:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 4) || (which_alternative == 6))
        {
	  return WRITE_CONFLICT_YES;
        }
      else
        {
	  return WRITE_CONFLICT_NO;
        }

    case 610:
    case 601:
    case 192:
    case 175:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 2)
        {
	  return WRITE_CONFLICT_YES;
        }
      else
        {
	  return WRITE_CONFLICT_NO;
        }

    case 174:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 3) || (which_alternative == 5))
        {
	  return WRITE_CONFLICT_YES;
        }
      else
        {
	  return WRITE_CONFLICT_NO;
        }

    case 612:
    case 198:
    case 195:
    case 193:
    case 184:
    case 173:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 1)
        {
	  return WRITE_CONFLICT_YES;
        }
      else
        {
	  return WRITE_CONFLICT_NO;
        }

    case 591:
    case 158:
      extract_constrain_insn_cached (insn);
      if (which_alternative != 0)
        {
	  return WRITE_CONFLICT_YES;
        }
      else
        {
	  return WRITE_CONFLICT_NO;
        }

    case 655:
    case 654:
    case 653:
    case 652:
    case 649:
    case 648:
    case 645:
    case 644:
    case 643:
    case 642:
    case 639:
    case 638:
    case 637:
    case 636:
    case 633:
    case 630:
    case 628:
    case 620:
    case 619:
    case 618:
    case 617:
    case 616:
    case 615:
    case 608:
    case 604:
    case 597:
    case 596:
    case 595:
    case 594:
    case 593:
    case 578:
    case 577:
    case 576:
    case 575:
    case 349:
    case 344:
    case 343:
    case 342:
    case 341:
    case 340:
    case 339:
    case 336:
    case 335:
    case 332:
    case 331:
    case 330:
    case 329:
    case 326:
    case 325:
    case 324:
    case 323:
    case 320:
    case 265:
    case 262:
    case 261:
    case 260:
    case 259:
    case 258:
    case 257:
    case 256:
    case 255:
    case 254:
    case 253:
    case 252:
    case 251:
    case 250:
    case 249:
    case 248:
    case 247:
    case 206:
    case 205:
    case 204:
    case 203:
    case 202:
    case 201:
    case 189:
    case 185:
    case 180:
    case 179:
    case 178:
    case 169:
    case 168:
    case 167:
    case 166:
    case 165:
    case 163:
    case 162:
    case 161:
    case 160:
    case 142:
    case 141:
    case 140:
    case 139:
      return WRITE_CONFLICT_YES;

    case -1:
      if (GET_CODE (PATTERN (insn)) != ASM_INPUT
          && asm_noperands (PATTERN (insn)) < 0)
        fatal_insn_not_found (insn);
    default:
      return WRITE_CONFLICT_NO;

    }
}

static int core_unit_blockage PARAMS ((rtx, rtx));
static int
core_unit_blockage (executing_insn, candidate_insn)
     rtx executing_insn;
     rtx candidate_insn;
{
  rtx insn;
  int casenum;

  insn = executing_insn;
  switch (recog_memoized (insn))
    {
    case 607:
    case 606:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  casenum = 0;
        }
      else if ((which_alternative == 2) && (((arm_ld_sched) == (LDSCHED_YES))))
        {
	  casenum = 2;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 605:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  casenum = 0;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 2))
        {
	  casenum = 1;
        }
      else if ((which_alternative == 3) && (((arm_ld_sched) == (LDSCHED_YES))))
        {
	  casenum = 2;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 610:
    case 601:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  casenum = 0;
        }
      else if ((which_alternative == 3) && (((arm_ld_sched) == (LDSCHED_YES))))
        {
	  casenum = 1;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 2))
        {
	  casenum = 2;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 591:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  casenum = 0;
        }
      else if ((which_alternative == 1) && (((arm_ld_sched) == (LDSCHED_YES))))
        {
	  casenum = 2;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 349:
      if (((arm_fpu_attr) == (FPU_FPA)))
        {
	  casenum = 6;
        }
      else
        {
	  casenum = 15 /* 0xf */;
        }
      break;

    case 626:
    case 623:
    case 214:
    case 212:
    case 209:
      casenum = 12 /* 0xc */;
      break;

    case 625:
    case 622:
    case 213:
    case 211:
    case 208:
      casenum = 13 /* 0xd */;
      break;

    case 624:
    case 621:
    case 348:
    case 210:
    case 207:
      casenum = 14 /* 0xe */;
      break;

    case 614:
    case 200:
      extract_constrain_insn_cached (insn);
      if ((((which_alternative != 0) && (which_alternative != 1)) && ((which_alternative != 2) && ((which_alternative != 3) && ((which_alternative != 4) && (which_alternative != 5))))) || ((which_alternative == 0) || (which_alternative == 1)))
        {
	  casenum = 0;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 2))
        {
	  casenum = 5;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 3))
        {
	  casenum = 6;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 4))
        {
	  casenum = 7;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 5))
        {
	  casenum = 8;
        }
      else
        {
	  casenum = 15 /* 0xf */;
        }
      break;

    case 199:
      extract_constrain_insn_cached (insn);
      if ((which_alternative != 1) && ((which_alternative != 2) && ((which_alternative != 3) && (which_alternative != 4))))
        {
	  casenum = 0;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 1) || (which_alternative == 3)))
        {
	  casenum = 2;
        }
      else if (((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 1) || (which_alternative == 3))) && (((arm_is_xscale) == (IS_XSCALE_YES))))
        {
	  casenum = 3;
        }
      else if ((! (((arm_ld_sched) == (LDSCHED_YES)))) && ((which_alternative == 1) || (which_alternative == 3)))
        {
	  casenum = 4;
        }
      else if ((which_alternative == 2) || (which_alternative == 4))
        {
	  casenum = 12 /* 0xc */;
        }
      else
        {
	  casenum = 15 /* 0xf */;
        }
      break;

    case 198:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  casenum = 0;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 1))
        {
	  casenum = 2;
        }
      else if ((! (((arm_ld_sched) == (LDSCHED_YES)))) && (which_alternative == 1))
        {
	  casenum = 4;
        }
      else if (which_alternative != 1)
        {
	  casenum = 12 /* 0xc */;
        }
      else
        {
	  casenum = 15 /* 0xf */;
        }
      break;

    case 613:
    case 197:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 2) || ((which_alternative == 5) || (which_alternative == 6)))
        {
	  casenum = 0;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 0) || (which_alternative == 4)))
        {
	  casenum = 2;
        }
      else if (((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 0) || (which_alternative == 4))) && (((arm_is_xscale) == (IS_XSCALE_YES))))
        {
	  casenum = 3;
        }
      else if ((! (((arm_ld_sched) == (LDSCHED_YES)))) && ((which_alternative == 0) || (which_alternative == 4)))
        {
	  casenum = 4;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 7))
        {
	  casenum = 5;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 8))
        {
	  casenum = 6;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 9))
        {
	  casenum = 7;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 10))
        {
	  casenum = 8;
        }
      else if ((which_alternative == 1) || (which_alternative == 3))
        {
	  casenum = 12 /* 0xc */;
        }
      else
        {
	  casenum = 15 /* 0xf */;
        }
      break;

    case 196:
      extract_constrain_insn_cached (insn);
      if ((which_alternative != 1) && ((which_alternative != 2) && ((which_alternative != 3) && (which_alternative != 4))))
        {
	  casenum = 0;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 2) || (which_alternative == 4)))
        {
	  casenum = 1;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 1) || (which_alternative == 3)))
        {
	  casenum = 2;
        }
      else if (((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 1) || (which_alternative == 3))) && (((arm_is_xscale) == (IS_XSCALE_YES))))
        {
	  casenum = 3;
        }
      else if ((! (((arm_ld_sched) == (LDSCHED_YES)))) && (((which_alternative == 1) || (which_alternative == 3)) || ((which_alternative == 2) || (which_alternative == 4))))
        {
	  casenum = 4;
        }
      else
        {
	  casenum = 15 /* 0xf */;
        }
      break;

    case 612:
    case 195:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  casenum = 0;
        }
      else if ((which_alternative == 2) && (((arm_ld_sched) == (LDSCHED_YES))))
        {
	  casenum = 1;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 1))
        {
	  casenum = 2;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 611:
    case 194:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 6) || ((which_alternative == 0) || (which_alternative == 1)))
        {
	  casenum = 0;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 8))
        {
	  casenum = 1;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 7))
        {
	  casenum = 2;
        }
      else if ((! (((arm_ld_sched) == (LDSCHED_YES)))) && ((which_alternative == 7) || (which_alternative == 8)))
        {
	  casenum = 4;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 2))
        {
	  casenum = 5;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 3))
        {
	  casenum = 6;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 4))
        {
	  casenum = 7;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 5))
        {
	  casenum = 8;
        }
      else
        {
	  casenum = 15 /* 0xf */;
        }
      break;

    case 188:
    case 187:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  casenum = 0;
        }
      else if ((which_alternative == 2) && (((arm_ld_sched) == (LDSCHED_YES))))
        {
	  casenum = 2;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 186:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  casenum = 0;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 2))
        {
	  casenum = 1;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 3))
        {
	  casenum = 2;
        }
      else if ((! (((arm_ld_sched) == (LDSCHED_YES)))) && ((which_alternative == 3) || (which_alternative == 2)))
        {
	  casenum = 4;
        }
      else
        {
	  casenum = 15 /* 0xf */;
        }
      break;

    case 193:
    case 184:
      extract_constrain_insn_cached (insn);
      if ((which_alternative != 1) && (which_alternative != 2))
        {
	  casenum = 0;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 2))
        {
	  casenum = 1;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 1))
        {
	  casenum = 2;
        }
      else if (! (((arm_ld_sched) == (LDSCHED_YES))))
        {
	  casenum = 4;
        }
      else
        {
	  casenum = 15 /* 0xf */;
        }
      break;

    case 176:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || ((which_alternative == 1) || ((which_alternative == 2) || ((which_alternative == 3) || (which_alternative == 8)))))
        {
	  casenum = 0;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 5) || (which_alternative == 7)))
        {
	  casenum = 1;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 4) || (which_alternative == 6)))
        {
	  casenum = 2;
        }
      else if (((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 4) || (which_alternative == 6))) && (((arm_is_xscale) == (IS_XSCALE_YES))))
        {
	  casenum = 3;
        }
      else if ((! (((arm_ld_sched) == (LDSCHED_YES)))) && (((which_alternative == 4) || (which_alternative == 6)) || ((which_alternative == 5) || (which_alternative == 7))))
        {
	  casenum = 4;
        }
      else
        {
	  casenum = 15 /* 0xf */;
        }
      break;

    case 192:
    case 175:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  casenum = 0;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 3))
        {
	  casenum = 1;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 2))
        {
	  casenum = 2;
        }
      else if ((! (((arm_ld_sched) == (LDSCHED_YES)))) && ((which_alternative == 2) || (which_alternative == 3)))
        {
	  casenum = 4;
        }
      else
        {
	  casenum = 15 /* 0xf */;
        }
      break;

    case 174:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || ((which_alternative == 1) || ((which_alternative == 2) || (which_alternative == 7))))
        {
	  casenum = 0;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 3) || (which_alternative == 5)))
        {
	  casenum = 2;
        }
      else if (((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 3) || (which_alternative == 5))) && (((arm_is_xscale) == (IS_XSCALE_YES))))
        {
	  casenum = 3;
        }
      else if ((! (((arm_ld_sched) == (LDSCHED_YES)))) && ((which_alternative == 3) || (which_alternative == 5)))
        {
	  casenum = 4;
        }
      else if ((which_alternative == 4) || (which_alternative == 6))
        {
	  casenum = 12 /* 0xc */;
        }
      else
        {
	  casenum = 15 /* 0xf */;
        }
      break;

    case 173:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  casenum = 0;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 1))
        {
	  casenum = 2;
        }
      else if ((! (((arm_ld_sched) == (LDSCHED_YES)))) && (which_alternative == 1))
        {
	  casenum = 4;
        }
      else
        {
	  casenum = 12 /* 0xc */;
        }
      break;

    case 655:
    case 654:
    case 653:
    case 652:
    case 649:
    case 648:
    case 645:
    case 644:
    case 643:
    case 642:
    case 639:
    case 638:
    case 637:
    case 636:
    case 633:
    case 630:
    case 628:
    case 620:
    case 619:
    case 618:
    case 617:
    case 616:
    case 615:
    case 608:
    case 604:
    case 597:
    case 596:
    case 595:
    case 594:
    case 593:
    case 342:
    case 341:
    case 340:
    case 339:
    case 336:
    case 335:
    case 332:
    case 331:
    case 330:
    case 329:
    case 326:
    case 325:
    case 324:
    case 323:
    case 320:
    case 265:
    case 261:
    case 260:
    case 259:
    case 206:
    case 205:
    case 204:
    case 203:
    case 202:
    case 201:
    case 189:
    case 185:
    case 180:
    case 179:
    case 178:
    case 169:
    case 168:
    case 167:
    case 166:
    case 165:
    case 163:
    case 162:
    case 161:
    case 160:
      if (((arm_ld_sched) == (LDSCHED_YES)))
        {
	  casenum = 2;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 158:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  casenum = 0;
        }
      else if ((which_alternative == 1) && (((arm_ld_sched) == (LDSCHED_YES))))
        {
	  casenum = 2;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 586:
    case 585:
    case 584:
    case 583:
    case 582:
    case 581:
    case 578:
    case 577:
    case 576:
    case 575:
    case 344:
    case 343:
    case 262:
    case 258:
    case 257:
    case 256:
    case 255:
    case 254:
    case 253:
    case 252:
    case 251:
    case 250:
    case 249:
    case 248:
    case 247:
    case 230:
    case 229:
    case 228:
    case 227:
    case 226:
    case 225:
    case 224:
    case 223:
    case 222:
    case 221:
    case 153:
    case 152:
    case 151:
    case 150:
    case 149:
    case 148:
    case 142:
    case 141:
    case 140:
    case 139:
      casenum = 15 /* 0xf */;
      break;

    case 651:
    case 650:
    case 647:
    case 646:
    case 641:
    case 640:
    case 635:
    case 634:
    case 338:
    case 337:
    case 334:
    case 333:
    case 328:
    case 327:
    case 322:
    case 321:
    case 113:
      if (((arm_ld_sched) == (LDSCHED_YES)))
        {
	  casenum = 1;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 530:
    case 529:
    case 528:
    case 527:
    case 526:
    case 525:
    case 524:
    case 523:
    case 279:
    case 278:
    case 277:
    case 58:
    case 57:
    case 56:
    case 55:
    case 54:
    case 53:
    case 52:
    case 51:
    case 50:
    case 49:
    case 48:
    case 47:
    case 46:
    case 45:
    case 44:
    case 43:
      if (((arm_ld_sched) == (LDSCHED_NO)))
        {
	  casenum = 9;
        }
      else if (((arm_is_strong) == (IS_STRONGARM_NO)))
        {
	  casenum = 10 /* 0xa */;
        }
      else
        {
	  casenum = 11 /* 0xb */;
        }
      break;

    case -1:
      if (GET_CODE (PATTERN (insn)) != ASM_INPUT
          && asm_noperands (PATTERN (insn)) < 0)
        fatal_insn_not_found (insn);
    default:
      casenum = 0;
      break;

    }

  insn = candidate_insn;
  switch (casenum)
    {
    case 0:
      return 1;

    case 1:
      return 1;

    case 2:
      return 1;

    case 3:
      return 1;

    case 4:
      return 2;

    case 5:
      return 3;

    case 6:
      return 4;

    case 7:
      return 6;

    case 8:
      return 7;

    case 9:
      return 16 /* 0x10 */;

    case 10:
      return 4;

    case 11:
      return 2;

    case 12:
      return 3;

    case 13:
      return 4;

    case 14:
      return 5;

    case 15:
      return 32 /* 0x20 */;

    default:
      abort ();
    }
}

static int core_unit_conflict_cost PARAMS ((rtx, rtx));
static int
core_unit_conflict_cost (executing_insn, candidate_insn)
     rtx executing_insn;
     rtx candidate_insn;
{
  rtx insn;
  int casenum;

  insn = executing_insn;
  switch (recog_memoized (insn))
    {
    case 607:
    case 606:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  casenum = 0;
        }
      else if ((which_alternative == 2) && (((arm_ld_sched) == (LDSCHED_YES))))
        {
	  casenum = 2;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 605:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  casenum = 0;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 2))
        {
	  casenum = 1;
        }
      else if ((which_alternative == 3) && (((arm_ld_sched) == (LDSCHED_YES))))
        {
	  casenum = 2;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 610:
    case 601:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  casenum = 0;
        }
      else if ((which_alternative == 3) && (((arm_ld_sched) == (LDSCHED_YES))))
        {
	  casenum = 1;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 2))
        {
	  casenum = 2;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 591:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  casenum = 0;
        }
      else if ((which_alternative == 1) && (((arm_ld_sched) == (LDSCHED_YES))))
        {
	  casenum = 2;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 349:
      if (((arm_fpu_attr) == (FPU_FPA)))
        {
	  casenum = 6;
        }
      else
        {
	  casenum = 15 /* 0xf */;
        }
      break;

    case 626:
    case 623:
    case 214:
    case 212:
    case 209:
      casenum = 12 /* 0xc */;
      break;

    case 625:
    case 622:
    case 213:
    case 211:
    case 208:
      casenum = 13 /* 0xd */;
      break;

    case 624:
    case 621:
    case 348:
    case 210:
    case 207:
      casenum = 14 /* 0xe */;
      break;

    case 614:
    case 200:
      extract_constrain_insn_cached (insn);
      if ((((which_alternative != 0) && (which_alternative != 1)) && ((which_alternative != 2) && ((which_alternative != 3) && ((which_alternative != 4) && (which_alternative != 5))))) || ((which_alternative == 0) || (which_alternative == 1)))
        {
	  casenum = 0;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 2))
        {
	  casenum = 5;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 3))
        {
	  casenum = 6;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 4))
        {
	  casenum = 7;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 5))
        {
	  casenum = 8;
        }
      else
        {
	  casenum = 15 /* 0xf */;
        }
      break;

    case 199:
      extract_constrain_insn_cached (insn);
      if ((which_alternative != 1) && ((which_alternative != 2) && ((which_alternative != 3) && (which_alternative != 4))))
        {
	  casenum = 0;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 1) || (which_alternative == 3)))
        {
	  casenum = 2;
        }
      else if (((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 1) || (which_alternative == 3))) && (((arm_is_xscale) == (IS_XSCALE_YES))))
        {
	  casenum = 3;
        }
      else if ((! (((arm_ld_sched) == (LDSCHED_YES)))) && ((which_alternative == 1) || (which_alternative == 3)))
        {
	  casenum = 4;
        }
      else if ((which_alternative == 2) || (which_alternative == 4))
        {
	  casenum = 12 /* 0xc */;
        }
      else
        {
	  casenum = 15 /* 0xf */;
        }
      break;

    case 198:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  casenum = 0;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 1))
        {
	  casenum = 2;
        }
      else if ((! (((arm_ld_sched) == (LDSCHED_YES)))) && (which_alternative == 1))
        {
	  casenum = 4;
        }
      else if (which_alternative != 1)
        {
	  casenum = 12 /* 0xc */;
        }
      else
        {
	  casenum = 15 /* 0xf */;
        }
      break;

    case 613:
    case 197:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 2) || ((which_alternative == 5) || (which_alternative == 6)))
        {
	  casenum = 0;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 0) || (which_alternative == 4)))
        {
	  casenum = 2;
        }
      else if (((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 0) || (which_alternative == 4))) && (((arm_is_xscale) == (IS_XSCALE_YES))))
        {
	  casenum = 3;
        }
      else if ((! (((arm_ld_sched) == (LDSCHED_YES)))) && ((which_alternative == 0) || (which_alternative == 4)))
        {
	  casenum = 4;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 7))
        {
	  casenum = 5;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 8))
        {
	  casenum = 6;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 9))
        {
	  casenum = 7;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 10))
        {
	  casenum = 8;
        }
      else if ((which_alternative == 1) || (which_alternative == 3))
        {
	  casenum = 12 /* 0xc */;
        }
      else
        {
	  casenum = 15 /* 0xf */;
        }
      break;

    case 196:
      extract_constrain_insn_cached (insn);
      if ((which_alternative != 1) && ((which_alternative != 2) && ((which_alternative != 3) && (which_alternative != 4))))
        {
	  casenum = 0;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 2) || (which_alternative == 4)))
        {
	  casenum = 1;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 1) || (which_alternative == 3)))
        {
	  casenum = 2;
        }
      else if (((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 1) || (which_alternative == 3))) && (((arm_is_xscale) == (IS_XSCALE_YES))))
        {
	  casenum = 3;
        }
      else if ((! (((arm_ld_sched) == (LDSCHED_YES)))) && (((which_alternative == 1) || (which_alternative == 3)) || ((which_alternative == 2) || (which_alternative == 4))))
        {
	  casenum = 4;
        }
      else
        {
	  casenum = 15 /* 0xf */;
        }
      break;

    case 612:
    case 195:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  casenum = 0;
        }
      else if ((which_alternative == 2) && (((arm_ld_sched) == (LDSCHED_YES))))
        {
	  casenum = 1;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 1))
        {
	  casenum = 2;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 611:
    case 194:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 6) || ((which_alternative == 0) || (which_alternative == 1)))
        {
	  casenum = 0;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 8))
        {
	  casenum = 1;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 7))
        {
	  casenum = 2;
        }
      else if ((! (((arm_ld_sched) == (LDSCHED_YES)))) && ((which_alternative == 7) || (which_alternative == 8)))
        {
	  casenum = 4;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 2))
        {
	  casenum = 5;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 3))
        {
	  casenum = 6;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 4))
        {
	  casenum = 7;
        }
      else if ((((arm_fpu_attr) == (FPU_FPA))) && (which_alternative == 5))
        {
	  casenum = 8;
        }
      else
        {
	  casenum = 15 /* 0xf */;
        }
      break;

    case 188:
    case 187:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  casenum = 0;
        }
      else if ((which_alternative == 2) && (((arm_ld_sched) == (LDSCHED_YES))))
        {
	  casenum = 2;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 186:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  casenum = 0;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 2))
        {
	  casenum = 1;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 3))
        {
	  casenum = 2;
        }
      else if ((! (((arm_ld_sched) == (LDSCHED_YES)))) && ((which_alternative == 3) || (which_alternative == 2)))
        {
	  casenum = 4;
        }
      else
        {
	  casenum = 15 /* 0xf */;
        }
      break;

    case 193:
    case 184:
      extract_constrain_insn_cached (insn);
      if ((which_alternative != 1) && (which_alternative != 2))
        {
	  casenum = 0;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 2))
        {
	  casenum = 1;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 1))
        {
	  casenum = 2;
        }
      else if (! (((arm_ld_sched) == (LDSCHED_YES))))
        {
	  casenum = 4;
        }
      else
        {
	  casenum = 15 /* 0xf */;
        }
      break;

    case 176:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || ((which_alternative == 1) || ((which_alternative == 2) || ((which_alternative == 3) || (which_alternative == 8)))))
        {
	  casenum = 0;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 5) || (which_alternative == 7)))
        {
	  casenum = 1;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 4) || (which_alternative == 6)))
        {
	  casenum = 2;
        }
      else if (((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 4) || (which_alternative == 6))) && (((arm_is_xscale) == (IS_XSCALE_YES))))
        {
	  casenum = 3;
        }
      else if ((! (((arm_ld_sched) == (LDSCHED_YES)))) && (((which_alternative == 4) || (which_alternative == 6)) || ((which_alternative == 5) || (which_alternative == 7))))
        {
	  casenum = 4;
        }
      else
        {
	  casenum = 15 /* 0xf */;
        }
      break;

    case 192:
    case 175:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || (which_alternative == 1))
        {
	  casenum = 0;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 3))
        {
	  casenum = 1;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 2))
        {
	  casenum = 2;
        }
      else if ((! (((arm_ld_sched) == (LDSCHED_YES)))) && ((which_alternative == 2) || (which_alternative == 3)))
        {
	  casenum = 4;
        }
      else
        {
	  casenum = 15 /* 0xf */;
        }
      break;

    case 174:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 0) || ((which_alternative == 1) || ((which_alternative == 2) || (which_alternative == 7))))
        {
	  casenum = 0;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 3) || (which_alternative == 5)))
        {
	  casenum = 2;
        }
      else if (((((arm_ld_sched) == (LDSCHED_YES))) && ((which_alternative == 3) || (which_alternative == 5))) && (((arm_is_xscale) == (IS_XSCALE_YES))))
        {
	  casenum = 3;
        }
      else if ((! (((arm_ld_sched) == (LDSCHED_YES)))) && ((which_alternative == 3) || (which_alternative == 5)))
        {
	  casenum = 4;
        }
      else if ((which_alternative == 4) || (which_alternative == 6))
        {
	  casenum = 12 /* 0xc */;
        }
      else
        {
	  casenum = 15 /* 0xf */;
        }
      break;

    case 173:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  casenum = 0;
        }
      else if ((((arm_ld_sched) == (LDSCHED_YES))) && (which_alternative == 1))
        {
	  casenum = 2;
        }
      else if ((! (((arm_ld_sched) == (LDSCHED_YES)))) && (which_alternative == 1))
        {
	  casenum = 4;
        }
      else
        {
	  casenum = 12 /* 0xc */;
        }
      break;

    case 655:
    case 654:
    case 653:
    case 652:
    case 649:
    case 648:
    case 645:
    case 644:
    case 643:
    case 642:
    case 639:
    case 638:
    case 637:
    case 636:
    case 633:
    case 630:
    case 628:
    case 620:
    case 619:
    case 618:
    case 617:
    case 616:
    case 615:
    case 608:
    case 604:
    case 597:
    case 596:
    case 595:
    case 594:
    case 593:
    case 342:
    case 341:
    case 340:
    case 339:
    case 336:
    case 335:
    case 332:
    case 331:
    case 330:
    case 329:
    case 326:
    case 325:
    case 324:
    case 323:
    case 320:
    case 265:
    case 261:
    case 260:
    case 259:
    case 206:
    case 205:
    case 204:
    case 203:
    case 202:
    case 201:
    case 189:
    case 185:
    case 180:
    case 179:
    case 178:
    case 169:
    case 168:
    case 167:
    case 166:
    case 165:
    case 163:
    case 162:
    case 161:
    case 160:
      if (((arm_ld_sched) == (LDSCHED_YES)))
        {
	  casenum = 2;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 158:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 0)
        {
	  casenum = 0;
        }
      else if ((which_alternative == 1) && (((arm_ld_sched) == (LDSCHED_YES))))
        {
	  casenum = 2;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 586:
    case 585:
    case 584:
    case 583:
    case 582:
    case 581:
    case 578:
    case 577:
    case 576:
    case 575:
    case 344:
    case 343:
    case 262:
    case 258:
    case 257:
    case 256:
    case 255:
    case 254:
    case 253:
    case 252:
    case 251:
    case 250:
    case 249:
    case 248:
    case 247:
    case 230:
    case 229:
    case 228:
    case 227:
    case 226:
    case 225:
    case 224:
    case 223:
    case 222:
    case 221:
    case 153:
    case 152:
    case 151:
    case 150:
    case 149:
    case 148:
    case 142:
    case 141:
    case 140:
    case 139:
      casenum = 15 /* 0xf */;
      break;

    case 651:
    case 650:
    case 647:
    case 646:
    case 641:
    case 640:
    case 635:
    case 634:
    case 338:
    case 337:
    case 334:
    case 333:
    case 328:
    case 327:
    case 322:
    case 321:
    case 113:
      if (((arm_ld_sched) == (LDSCHED_YES)))
        {
	  casenum = 1;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 530:
    case 529:
    case 528:
    case 527:
    case 526:
    case 525:
    case 524:
    case 523:
    case 279:
    case 278:
    case 277:
    case 58:
    case 57:
    case 56:
    case 55:
    case 54:
    case 53:
    case 52:
    case 51:
    case 50:
    case 49:
    case 48:
    case 47:
    case 46:
    case 45:
    case 44:
    case 43:
      if (((arm_ld_sched) == (LDSCHED_NO)))
        {
	  casenum = 9;
        }
      else if (((arm_is_strong) == (IS_STRONGARM_NO)))
        {
	  casenum = 10 /* 0xa */;
        }
      else
        {
	  casenum = 11 /* 0xb */;
        }
      break;

    case -1:
      if (GET_CODE (PATTERN (insn)) != ASM_INPUT
          && asm_noperands (PATTERN (insn)) < 0)
        fatal_insn_not_found (insn);
    default:
      casenum = 0;
      break;

    }

  insn = candidate_insn;
  switch (casenum)
    {
    case 0:
      return 1;

    case 1:
      return 1;

    case 2:
      return 1;

    case 3:
      return 1;

    case 4:
      return 2;

    case 5:
      return 3;

    case 6:
      return 4;

    case 7:
      return 6;

    case 8:
      return 7;

    case 9:
      return 16 /* 0x10 */;

    case 10:
      return 4;

    case 11:
      return 2;

    case 12:
      return 3;

    case 13:
      return 4;

    case 14:
      return 5;

    case 15:
      return 32 /* 0x20 */;

    default:
      abort ();
    }
}

static int write_blockage_unit_blockage PARAMS ((rtx, rtx));
static int
write_blockage_unit_blockage (executing_insn, candidate_insn)
     rtx executing_insn;
     rtx candidate_insn;
{
  rtx insn;
  int casenum;

  insn = executing_insn;
  switch (recog_memoized (insn))
    {
    case 610:
    case 601:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 3)
        {
	  casenum = 0;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 626:
    case 623:
    case 214:
    case 212:
    case 209:
      casenum = 1;
      break;

    case 625:
    case 622:
    case 213:
    case 211:
    case 208:
      casenum = 2;
      break;

    case 624:
    case 621:
    case 348:
    case 210:
    case 207:
      casenum = 3;
      break;

    case 199:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 2) || (which_alternative == 4))
        {
	  casenum = 1;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 198:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 2)
        {
	  casenum = 1;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 613:
    case 197:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 1) || (which_alternative == 3))
        {
	  casenum = 1;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 196:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 2) || (which_alternative == 4))
        {
	  casenum = 0;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 612:
    case 195:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 2)
        {
	  casenum = 0;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 611:
    case 194:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 8)
        {
	  casenum = 0;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 605:
    case 193:
    case 186:
    case 184:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 2)
        {
	  casenum = 0;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 176:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 5) || (which_alternative == 7))
        {
	  casenum = 0;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 192:
    case 175:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 3)
        {
	  casenum = 0;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 174:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 4) || (which_alternative == 6))
        {
	  casenum = 1;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 173:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 2)
        {
	  casenum = 1;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 651:
    case 650:
    case 647:
    case 646:
    case 641:
    case 640:
    case 635:
    case 634:
    case 338:
    case 337:
    case 334:
    case 333:
    case 328:
    case 327:
    case 322:
    case 321:
    case 113:
      casenum = 0;
      break;

    case -1:
      if (GET_CODE (PATTERN (insn)) != ASM_INPUT
          && asm_noperands (PATTERN (insn)) < 0)
        fatal_insn_not_found (insn);
    default:
      casenum = 4;
      break;

    }

  insn = candidate_insn;
  switch (casenum)
    {
    case 0:
      switch (recog_memoized (insn))
	{
        case 613:
        case 197:
	  extract_constrain_insn_cached (insn);
	  if (((which_alternative == 1) || (which_alternative == 3)) && ((which_alternative != 7) && ((which_alternative != 8) && ((which_alternative != 10) && ((which_alternative != 9) && ((which_alternative != 0) && (which_alternative != 4)))))))
	    {
	      return 0;
	    }
	  else
	    {
	      return 5;
	    }

        case 199:
        case 196:
	  extract_constrain_insn_cached (insn);
	  if (((which_alternative == 2) || (which_alternative == 4)) && ((which_alternative != 1) && (which_alternative != 3)))
	    {
	      return 0;
	    }
	  else
	    {
	      return 5;
	    }

        case 611:
        case 194:
	  extract_constrain_insn_cached (insn);
	  if (which_alternative == 8)
	    {
	      return 0;
	    }
	  else
	    {
	      return 5;
	    }

        case 176:
	  extract_constrain_insn_cached (insn);
	  if (((which_alternative == 5) || (which_alternative == 7)) && ((which_alternative != 4) && (which_alternative != 6)))
	    {
	      return 0;
	    }
	  else
	    {
	      return 5;
	    }

        case 610:
        case 601:
        case 192:
        case 175:
	  extract_constrain_insn_cached (insn);
	  if (which_alternative == 3)
	    {
	      return 0;
	    }
	  else
	    {
	      return 5;
	    }

        case 174:
	  extract_constrain_insn_cached (insn);
	  if (((which_alternative == 4) || (which_alternative == 6)) && ((which_alternative != 3) && (which_alternative != 5)))
	    {
	      return 0;
	    }
	  else
	    {
	      return 5;
	    }

        case 612:
        case 605:
        case 198:
        case 195:
        case 193:
        case 186:
        case 184:
        case 173:
	  extract_constrain_insn_cached (insn);
	  if (which_alternative == 2)
	    {
	      return 0;
	    }
	  else
	    {
	      return 5;
	    }

        case 651:
        case 650:
        case 647:
        case 646:
        case 641:
        case 640:
        case 635:
        case 634:
        case 626:
        case 625:
        case 624:
        case 623:
        case 622:
        case 621:
        case 348:
        case 338:
        case 337:
        case 334:
        case 333:
        case 328:
        case 327:
        case 322:
        case 321:
        case 214:
        case 213:
        case 212:
        case 211:
        case 210:
        case 209:
        case 208:
        case 207:
        case 113:
	  return 0;

        case -1:
	  if (GET_CODE (PATTERN (insn)) != ASM_INPUT
	      && asm_noperands (PATTERN (insn)) < 0)
	    fatal_insn_not_found (insn);
        default:
	  return 5;

      }

    case 1:
      switch (recog_memoized (insn))
	{
        case 613:
        case 197:
	  extract_constrain_insn_cached (insn);
	  if (((which_alternative == 1) || (which_alternative == 3)) && ((which_alternative != 7) && ((which_alternative != 8) && ((which_alternative != 10) && ((which_alternative != 9) && ((which_alternative != 0) && (which_alternative != 4)))))))
	    {
	      return 0;
	    }
	  else
	    {
	      return 7;
	    }

        case 199:
        case 196:
	  extract_constrain_insn_cached (insn);
	  if (((which_alternative == 2) || (which_alternative == 4)) && ((which_alternative != 1) && (which_alternative != 3)))
	    {
	      return 0;
	    }
	  else
	    {
	      return 7;
	    }

        case 611:
        case 194:
	  extract_constrain_insn_cached (insn);
	  if (which_alternative == 8)
	    {
	      return 0;
	    }
	  else
	    {
	      return 7;
	    }

        case 176:
	  extract_constrain_insn_cached (insn);
	  if (((which_alternative == 5) || (which_alternative == 7)) && ((which_alternative != 4) && (which_alternative != 6)))
	    {
	      return 0;
	    }
	  else
	    {
	      return 7;
	    }

        case 610:
        case 601:
        case 192:
        case 175:
	  extract_constrain_insn_cached (insn);
	  if (which_alternative == 3)
	    {
	      return 0;
	    }
	  else
	    {
	      return 7;
	    }

        case 174:
	  extract_constrain_insn_cached (insn);
	  if (((which_alternative == 4) || (which_alternative == 6)) && ((which_alternative != 3) && (which_alternative != 5)))
	    {
	      return 0;
	    }
	  else
	    {
	      return 7;
	    }

        case 612:
        case 605:
        case 198:
        case 195:
        case 193:
        case 186:
        case 184:
        case 173:
	  extract_constrain_insn_cached (insn);
	  if (which_alternative == 2)
	    {
	      return 0;
	    }
	  else
	    {
	      return 7;
	    }

        case 651:
        case 650:
        case 647:
        case 646:
        case 641:
        case 640:
        case 635:
        case 634:
        case 626:
        case 625:
        case 624:
        case 623:
        case 622:
        case 621:
        case 348:
        case 338:
        case 337:
        case 334:
        case 333:
        case 328:
        case 327:
        case 322:
        case 321:
        case 214:
        case 213:
        case 212:
        case 211:
        case 210:
        case 209:
        case 208:
        case 207:
        case 113:
	  return 0;

        case -1:
	  if (GET_CODE (PATTERN (insn)) != ASM_INPUT
	      && asm_noperands (PATTERN (insn)) < 0)
	    fatal_insn_not_found (insn);
        default:
	  return 7;

      }

    case 2:
      switch (recog_memoized (insn))
	{
        case 613:
        case 197:
	  extract_constrain_insn_cached (insn);
	  if (((which_alternative == 1) || (which_alternative == 3)) && ((which_alternative != 7) && ((which_alternative != 8) && ((which_alternative != 10) && ((which_alternative != 9) && ((which_alternative != 0) && (which_alternative != 4)))))))
	    {
	      return 0;
	    }
	  else
	    {
	      return 9;
	    }

        case 199:
        case 196:
	  extract_constrain_insn_cached (insn);
	  if (((which_alternative == 2) || (which_alternative == 4)) && ((which_alternative != 1) && (which_alternative != 3)))
	    {
	      return 0;
	    }
	  else
	    {
	      return 9;
	    }

        case 611:
        case 194:
	  extract_constrain_insn_cached (insn);
	  if (which_alternative == 8)
	    {
	      return 0;
	    }
	  else
	    {
	      return 9;
	    }

        case 176:
	  extract_constrain_insn_cached (insn);
	  if (((which_alternative == 5) || (which_alternative == 7)) && ((which_alternative != 4) && (which_alternative != 6)))
	    {
	      return 0;
	    }
	  else
	    {
	      return 9;
	    }

        case 610:
        case 601:
        case 192:
        case 175:
	  extract_constrain_insn_cached (insn);
	  if (which_alternative == 3)
	    {
	      return 0;
	    }
	  else
	    {
	      return 9;
	    }

        case 174:
	  extract_constrain_insn_cached (insn);
	  if (((which_alternative == 4) || (which_alternative == 6)) && ((which_alternative != 3) && (which_alternative != 5)))
	    {
	      return 0;
	    }
	  else
	    {
	      return 9;
	    }

        case 612:
        case 605:
        case 198:
        case 195:
        case 193:
        case 186:
        case 184:
        case 173:
	  extract_constrain_insn_cached (insn);
	  if (which_alternative == 2)
	    {
	      return 0;
	    }
	  else
	    {
	      return 9;
	    }

        case 651:
        case 650:
        case 647:
        case 646:
        case 641:
        case 640:
        case 635:
        case 634:
        case 626:
        case 625:
        case 624:
        case 623:
        case 622:
        case 621:
        case 348:
        case 338:
        case 337:
        case 334:
        case 333:
        case 328:
        case 327:
        case 322:
        case 321:
        case 214:
        case 213:
        case 212:
        case 211:
        case 210:
        case 209:
        case 208:
        case 207:
        case 113:
	  return 0;

        case -1:
	  if (GET_CODE (PATTERN (insn)) != ASM_INPUT
	      && asm_noperands (PATTERN (insn)) < 0)
	    fatal_insn_not_found (insn);
        default:
	  return 9;

      }

    case 3:
      switch (recog_memoized (insn))
	{
        case 613:
        case 197:
	  extract_constrain_insn_cached (insn);
	  if (((which_alternative == 1) || (which_alternative == 3)) && ((which_alternative != 7) && ((which_alternative != 8) && ((which_alternative != 10) && ((which_alternative != 9) && ((which_alternative != 0) && (which_alternative != 4)))))))
	    {
	      return 0;
	    }
	  else
	    {
	      return 11 /* 0xb */;
	    }

        case 199:
        case 196:
	  extract_constrain_insn_cached (insn);
	  if (((which_alternative == 2) || (which_alternative == 4)) && ((which_alternative != 1) && (which_alternative != 3)))
	    {
	      return 0;
	    }
	  else
	    {
	      return 11 /* 0xb */;
	    }

        case 611:
        case 194:
	  extract_constrain_insn_cached (insn);
	  if (which_alternative == 8)
	    {
	      return 0;
	    }
	  else
	    {
	      return 11 /* 0xb */;
	    }

        case 176:
	  extract_constrain_insn_cached (insn);
	  if (((which_alternative == 5) || (which_alternative == 7)) && ((which_alternative != 4) && (which_alternative != 6)))
	    {
	      return 0;
	    }
	  else
	    {
	      return 11 /* 0xb */;
	    }

        case 610:
        case 601:
        case 192:
        case 175:
	  extract_constrain_insn_cached (insn);
	  if (which_alternative == 3)
	    {
	      return 0;
	    }
	  else
	    {
	      return 11 /* 0xb */;
	    }

        case 174:
	  extract_constrain_insn_cached (insn);
	  if (((which_alternative == 4) || (which_alternative == 6)) && ((which_alternative != 3) && (which_alternative != 5)))
	    {
	      return 0;
	    }
	  else
	    {
	      return 11 /* 0xb */;
	    }

        case 612:
        case 605:
        case 198:
        case 195:
        case 193:
        case 186:
        case 184:
        case 173:
	  extract_constrain_insn_cached (insn);
	  if (which_alternative == 2)
	    {
	      return 0;
	    }
	  else
	    {
	      return 11 /* 0xb */;
	    }

        case 651:
        case 650:
        case 647:
        case 646:
        case 641:
        case 640:
        case 635:
        case 634:
        case 626:
        case 625:
        case 624:
        case 623:
        case 622:
        case 621:
        case 348:
        case 338:
        case 337:
        case 334:
        case 333:
        case 328:
        case 327:
        case 322:
        case 321:
        case 214:
        case 213:
        case 212:
        case 211:
        case 210:
        case 209:
        case 208:
        case 207:
        case 113:
	  return 0;

        case -1:
	  if (GET_CODE (PATTERN (insn)) != ASM_INPUT
	      && asm_noperands (PATTERN (insn)) < 0)
	    fatal_insn_not_found (insn);
        default:
	  return 11 /* 0xb */;

      }

    case 4:
      return 1;

    default:
      abort ();
    }
}

static int write_blockage_unit_conflict_cost PARAMS ((rtx, rtx));
static int
write_blockage_unit_conflict_cost (executing_insn, candidate_insn)
     rtx executing_insn;
     rtx candidate_insn;
{
  rtx insn;
  int casenum;

  insn = executing_insn;
  switch (recog_memoized (insn))
    {
    case 610:
    case 601:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 3)
        {
	  casenum = 0;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 626:
    case 623:
    case 214:
    case 212:
    case 209:
      casenum = 1;
      break;

    case 625:
    case 622:
    case 213:
    case 211:
    case 208:
      casenum = 2;
      break;

    case 624:
    case 621:
    case 348:
    case 210:
    case 207:
      casenum = 3;
      break;

    case 199:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 2) || (which_alternative == 4))
        {
	  casenum = 1;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 198:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 2)
        {
	  casenum = 1;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 613:
    case 197:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 1) || (which_alternative == 3))
        {
	  casenum = 1;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 196:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 2) || (which_alternative == 4))
        {
	  casenum = 0;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 612:
    case 195:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 2)
        {
	  casenum = 0;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 611:
    case 194:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 8)
        {
	  casenum = 0;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 605:
    case 193:
    case 186:
    case 184:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 2)
        {
	  casenum = 0;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 176:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 5) || (which_alternative == 7))
        {
	  casenum = 0;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 192:
    case 175:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 3)
        {
	  casenum = 0;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 174:
      extract_constrain_insn_cached (insn);
      if ((which_alternative == 4) || (which_alternative == 6))
        {
	  casenum = 1;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 173:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 2)
        {
	  casenum = 1;
        }
      else
        {
	  casenum = 4;
        }
      break;

    case 651:
    case 650:
    case 647:
    case 646:
    case 641:
    case 640:
    case 635:
    case 634:
    case 338:
    case 337:
    case 334:
    case 333:
    case 328:
    case 327:
    case 322:
    case 321:
    case 113:
      casenum = 0;
      break;

    case -1:
      if (GET_CODE (PATTERN (insn)) != ASM_INPUT
          && asm_noperands (PATTERN (insn)) < 0)
        fatal_insn_not_found (insn);
    default:
      casenum = 4;
      break;

    }

  insn = candidate_insn;
  switch (casenum)
    {
    case 0:
      switch (recog_memoized (insn))
	{
        case 613:
        case 197:
	  extract_constrain_insn_cached (insn);
	  if (((which_alternative == 1) || (which_alternative == 3)) && ((which_alternative != 7) && ((which_alternative != 8) && ((which_alternative != 10) && ((which_alternative != 9) && ((which_alternative != 0) && (which_alternative != 4)))))))
	    {
	      return 0;
	    }
	  else
	    {
	      return 5;
	    }

        case 199:
        case 196:
	  extract_constrain_insn_cached (insn);
	  if (((which_alternative == 2) || (which_alternative == 4)) && ((which_alternative != 1) && (which_alternative != 3)))
	    {
	      return 0;
	    }
	  else
	    {
	      return 5;
	    }

        case 611:
        case 194:
	  extract_constrain_insn_cached (insn);
	  if (which_alternative == 8)
	    {
	      return 0;
	    }
	  else
	    {
	      return 5;
	    }

        case 176:
	  extract_constrain_insn_cached (insn);
	  if (((which_alternative == 5) || (which_alternative == 7)) && ((which_alternative != 4) && (which_alternative != 6)))
	    {
	      return 0;
	    }
	  else
	    {
	      return 5;
	    }

        case 610:
        case 601:
        case 192:
        case 175:
	  extract_constrain_insn_cached (insn);
	  if (which_alternative == 3)
	    {
	      return 0;
	    }
	  else
	    {
	      return 5;
	    }

        case 174:
	  extract_constrain_insn_cached (insn);
	  if (((which_alternative == 4) || (which_alternative == 6)) && ((which_alternative != 3) && (which_alternative != 5)))
	    {
	      return 0;
	    }
	  else
	    {
	      return 5;
	    }

        case 612:
        case 605:
        case 198:
        case 195:
        case 193:
        case 186:
        case 184:
        case 173:
	  extract_constrain_insn_cached (insn);
	  if (which_alternative == 2)
	    {
	      return 0;
	    }
	  else
	    {
	      return 5;
	    }

        case 651:
        case 650:
        case 647:
        case 646:
        case 641:
        case 640:
        case 635:
        case 634:
        case 626:
        case 625:
        case 624:
        case 623:
        case 622:
        case 621:
        case 348:
        case 338:
        case 337:
        case 334:
        case 333:
        case 328:
        case 327:
        case 322:
        case 321:
        case 214:
        case 213:
        case 212:
        case 211:
        case 210:
        case 209:
        case 208:
        case 207:
        case 113:
	  return 0;

        case -1:
	  if (GET_CODE (PATTERN (insn)) != ASM_INPUT
	      && asm_noperands (PATTERN (insn)) < 0)
	    fatal_insn_not_found (insn);
        default:
	  return 5;

      }

    case 1:
      switch (recog_memoized (insn))
	{
        case 613:
        case 197:
	  extract_constrain_insn_cached (insn);
	  if (((which_alternative == 1) || (which_alternative == 3)) && ((which_alternative != 7) && ((which_alternative != 8) && ((which_alternative != 10) && ((which_alternative != 9) && ((which_alternative != 0) && (which_alternative != 4)))))))
	    {
	      return 0;
	    }
	  else
	    {
	      return 7;
	    }

        case 199:
        case 196:
	  extract_constrain_insn_cached (insn);
	  if (((which_alternative == 2) || (which_alternative == 4)) && ((which_alternative != 1) && (which_alternative != 3)))
	    {
	      return 0;
	    }
	  else
	    {
	      return 7;
	    }

        case 611:
        case 194:
	  extract_constrain_insn_cached (insn);
	  if (which_alternative == 8)
	    {
	      return 0;
	    }
	  else
	    {
	      return 7;
	    }

        case 176:
	  extract_constrain_insn_cached (insn);
	  if (((which_alternative == 5) || (which_alternative == 7)) && ((which_alternative != 4) && (which_alternative != 6)))
	    {
	      return 0;
	    }
	  else
	    {
	      return 7;
	    }

        case 610:
        case 601:
        case 192:
        case 175:
	  extract_constrain_insn_cached (insn);
	  if (which_alternative == 3)
	    {
	      return 0;
	    }
	  else
	    {
	      return 7;
	    }

        case 174:
	  extract_constrain_insn_cached (insn);
	  if (((which_alternative == 4) || (which_alternative == 6)) && ((which_alternative != 3) && (which_alternative != 5)))
	    {
	      return 0;
	    }
	  else
	    {
	      return 7;
	    }

        case 612:
        case 605:
        case 198:
        case 195:
        case 193:
        case 186:
        case 184:
        case 173:
	  extract_constrain_insn_cached (insn);
	  if (which_alternative == 2)
	    {
	      return 0;
	    }
	  else
	    {
	      return 7;
	    }

        case 651:
        case 650:
        case 647:
        case 646:
        case 641:
        case 640:
        case 635:
        case 634:
        case 626:
        case 625:
        case 624:
        case 623:
        case 622:
        case 621:
        case 348:
        case 338:
        case 337:
        case 334:
        case 333:
        case 328:
        case 327:
        case 322:
        case 321:
        case 214:
        case 213:
        case 212:
        case 211:
        case 210:
        case 209:
        case 208:
        case 207:
        case 113:
	  return 0;

        case -1:
	  if (GET_CODE (PATTERN (insn)) != ASM_INPUT
	      && asm_noperands (PATTERN (insn)) < 0)
	    fatal_insn_not_found (insn);
        default:
	  return 7;

      }

    case 2:
      switch (recog_memoized (insn))
	{
        case 613:
        case 197:
	  extract_constrain_insn_cached (insn);
	  if (((which_alternative == 1) || (which_alternative == 3)) && ((which_alternative != 7) && ((which_alternative != 8) && ((which_alternative != 10) && ((which_alternative != 9) && ((which_alternative != 0) && (which_alternative != 4)))))))
	    {
	      return 0;
	    }
	  else
	    {
	      return 9;
	    }

        case 199:
        case 196:
	  extract_constrain_insn_cached (insn);
	  if (((which_alternative == 2) || (which_alternative == 4)) && ((which_alternative != 1) && (which_alternative != 3)))
	    {
	      return 0;
	    }
	  else
	    {
	      return 9;
	    }

        case 611:
        case 194:
	  extract_constrain_insn_cached (insn);
	  if (which_alternative == 8)
	    {
	      return 0;
	    }
	  else
	    {
	      return 9;
	    }

        case 176:
	  extract_constrain_insn_cached (insn);
	  if (((which_alternative == 5) || (which_alternative == 7)) && ((which_alternative != 4) && (which_alternative != 6)))
	    {
	      return 0;
	    }
	  else
	    {
	      return 9;
	    }

        case 610:
        case 601:
        case 192:
        case 175:
	  extract_constrain_insn_cached (insn);
	  if (which_alternative == 3)
	    {
	      return 0;
	    }
	  else
	    {
	      return 9;
	    }

        case 174:
	  extract_constrain_insn_cached (insn);
	  if (((which_alternative == 4) || (which_alternative == 6)) && ((which_alternative != 3) && (which_alternative != 5)))
	    {
	      return 0;
	    }
	  else
	    {
	      return 9;
	    }

        case 612:
        case 605:
        case 198:
        case 195:
        case 193:
        case 186:
        case 184:
        case 173:
	  extract_constrain_insn_cached (insn);
	  if (which_alternative == 2)
	    {
	      return 0;
	    }
	  else
	    {
	      return 9;
	    }

        case 651:
        case 650:
        case 647:
        case 646:
        case 641:
        case 640:
        case 635:
        case 634:
        case 626:
        case 625:
        case 624:
        case 623:
        case 622:
        case 621:
        case 348:
        case 338:
        case 337:
        case 334:
        case 333:
        case 328:
        case 327:
        case 322:
        case 321:
        case 214:
        case 213:
        case 212:
        case 211:
        case 210:
        case 209:
        case 208:
        case 207:
        case 113:
	  return 0;

        case -1:
	  if (GET_CODE (PATTERN (insn)) != ASM_INPUT
	      && asm_noperands (PATTERN (insn)) < 0)
	    fatal_insn_not_found (insn);
        default:
	  return 9;

      }

    case 3:
      switch (recog_memoized (insn))
	{
        case 613:
        case 197:
	  extract_constrain_insn_cached (insn);
	  if (((which_alternative == 1) || (which_alternative == 3)) && ((which_alternative != 7) && ((which_alternative != 8) && ((which_alternative != 10) && ((which_alternative != 9) && ((which_alternative != 0) && (which_alternative != 4)))))))
	    {
	      return 0;
	    }
	  else
	    {
	      return 11 /* 0xb */;
	    }

        case 199:
        case 196:
	  extract_constrain_insn_cached (insn);
	  if (((which_alternative == 2) || (which_alternative == 4)) && ((which_alternative != 1) && (which_alternative != 3)))
	    {
	      return 0;
	    }
	  else
	    {
	      return 11 /* 0xb */;
	    }

        case 611:
        case 194:
	  extract_constrain_insn_cached (insn);
	  if (which_alternative == 8)
	    {
	      return 0;
	    }
	  else
	    {
	      return 11 /* 0xb */;
	    }

        case 176:
	  extract_constrain_insn_cached (insn);
	  if (((which_alternative == 5) || (which_alternative == 7)) && ((which_alternative != 4) && (which_alternative != 6)))
	    {
	      return 0;
	    }
	  else
	    {
	      return 11 /* 0xb */;
	    }

        case 610:
        case 601:
        case 192:
        case 175:
	  extract_constrain_insn_cached (insn);
	  if (which_alternative == 3)
	    {
	      return 0;
	    }
	  else
	    {
	      return 11 /* 0xb */;
	    }

        case 174:
	  extract_constrain_insn_cached (insn);
	  if (((which_alternative == 4) || (which_alternative == 6)) && ((which_alternative != 3) && (which_alternative != 5)))
	    {
	      return 0;
	    }
	  else
	    {
	      return 11 /* 0xb */;
	    }

        case 612:
        case 605:
        case 198:
        case 195:
        case 193:
        case 186:
        case 184:
        case 173:
	  extract_constrain_insn_cached (insn);
	  if (which_alternative == 2)
	    {
	      return 0;
	    }
	  else
	    {
	      return 11 /* 0xb */;
	    }

        case 651:
        case 650:
        case 647:
        case 646:
        case 641:
        case 640:
        case 635:
        case 634:
        case 626:
        case 625:
        case 624:
        case 623:
        case 622:
        case 621:
        case 348:
        case 338:
        case 337:
        case 334:
        case 333:
        case 328:
        case 327:
        case 322:
        case 321:
        case 214:
        case 213:
        case 212:
        case 211:
        case 210:
        case 209:
        case 208:
        case 207:
        case 113:
	  return 0;

        case -1:
	  if (GET_CODE (PATTERN (insn)) != ASM_INPUT
	      && asm_noperands (PATTERN (insn)) < 0)
	    fatal_insn_not_found (insn);
        default:
	  return 11 /* 0xb */;

      }

    case 4:
      return 1;

    default:
      abort ();
    }
}

static int write_buf_unit_blockage PARAMS ((rtx, rtx));
static int
write_buf_unit_blockage (executing_insn, candidate_insn)
     rtx executing_insn;
     rtx candidate_insn;
{
  rtx insn;
  int casenum;

  insn = executing_insn;
  switch (recog_memoized (insn))
    {
    case 610:
    case 601:
      extract_constrain_insn_cached (insn);
      casenum = 0;
      break;

    case 626:
    case 623:
    case 214:
    case 212:
    case 209:
      casenum = 1;
      break;

    case 625:
    case 622:
    case 213:
    case 211:
    case 208:
      casenum = 2;
      break;

    case 614:
    case 200:
      extract_constrain_insn_cached (insn);
      casenum = 0;
      break;

    case 199:
      extract_constrain_insn_cached (insn);
      casenum = 1;
      break;

    case 198:
      extract_constrain_insn_cached (insn);
      casenum = 1;
      break;

    case 613:
    case 197:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 9)
        {
	  casenum = 0;
        }
      else
        {
	  casenum = 1;
        }
      break;

    case 196:
      extract_constrain_insn_cached (insn);
      casenum = 0;
      break;

    case 612:
    case 195:
      extract_constrain_insn_cached (insn);
      casenum = 0;
      break;

    case 611:
    case 194:
      extract_constrain_insn_cached (insn);
      casenum = 0;
      break;

    case 605:
    case 193:
    case 186:
    case 184:
      extract_constrain_insn_cached (insn);
      casenum = 0;
      break;

    case 176:
      extract_constrain_insn_cached (insn);
      casenum = 0;
      break;

    case 192:
    case 175:
      extract_constrain_insn_cached (insn);
      casenum = 0;
      break;

    case 174:
      extract_constrain_insn_cached (insn);
      casenum = 1;
      break;

    case 173:
      extract_constrain_insn_cached (insn);
      casenum = 1;
      break;

    case 651:
    case 650:
    case 647:
    case 646:
    case 641:
    case 640:
    case 635:
    case 634:
    case 338:
    case 337:
    case 334:
    case 333:
    case 328:
    case 327:
    case 322:
    case 321:
    case 113:
      casenum = 0;
      break;

    case -1:
      if (GET_CODE (PATTERN (insn)) != ASM_INPUT
          && asm_noperands (PATTERN (insn)) < 0)
        fatal_insn_not_found (insn);
    default:
      casenum = 3;
      break;

    }

  insn = candidate_insn;
  switch (casenum)
    {
    case 0:
      return 3;

    case 1:
      return 4;

    case 2:
      return 5;

    case 3:
      return 6;

    default:
      abort ();
    }
}

static int write_buf_unit_conflict_cost PARAMS ((rtx, rtx));
static int
write_buf_unit_conflict_cost (executing_insn, candidate_insn)
     rtx executing_insn;
     rtx candidate_insn;
{
  rtx insn;
  int casenum;

  insn = executing_insn;
  switch (recog_memoized (insn))
    {
    case 610:
    case 601:
      extract_constrain_insn_cached (insn);
      casenum = 0;
      break;

    case 626:
    case 623:
    case 214:
    case 212:
    case 209:
      casenum = 1;
      break;

    case 625:
    case 622:
    case 213:
    case 211:
    case 208:
      casenum = 2;
      break;

    case 614:
    case 200:
      extract_constrain_insn_cached (insn);
      casenum = 0;
      break;

    case 199:
      extract_constrain_insn_cached (insn);
      casenum = 1;
      break;

    case 198:
      extract_constrain_insn_cached (insn);
      casenum = 1;
      break;

    case 613:
    case 197:
      extract_constrain_insn_cached (insn);
      if (which_alternative == 9)
        {
	  casenum = 0;
        }
      else
        {
	  casenum = 1;
        }
      break;

    case 196:
      extract_constrain_insn_cached (insn);
      casenum = 0;
      break;

    case 612:
    case 195:
      extract_constrain_insn_cached (insn);
      casenum = 0;
      break;

    case 611:
    case 194:
      extract_constrain_insn_cached (insn);
      casenum = 0;
      break;

    case 605:
    case 193:
    case 186:
    case 184:
      extract_constrain_insn_cached (insn);
      casenum = 0;
      break;

    case 176:
      extract_constrain_insn_cached (insn);
      casenum = 0;
      break;

    case 192:
    case 175:
      extract_constrain_insn_cached (insn);
      casenum = 0;
      break;

    case 174:
      extract_constrain_insn_cached (insn);
      casenum = 1;
      break;

    case 173:
      extract_constrain_insn_cached (insn);
      casenum = 1;
      break;

    case 651:
    case 650:
    case 647:
    case 646:
    case 641:
    case 640:
    case 635:
    case 634:
    case 338:
    case 337:
    case 334:
    case 333:
    case 328:
    case 327:
    case 322:
    case 321:
    case 113:
      casenum = 0;
      break;

    case -1:
      if (GET_CODE (PATTERN (insn)) != ASM_INPUT
          && asm_noperands (PATTERN (insn)) < 0)
        fatal_insn_not_found (insn);
    default:
      casenum = 3;
      break;

    }

  insn = candidate_insn;
  switch (casenum)
    {
    case 0:
      return 3;

    case 1:
      return 4;

    case 2:
      return 5;

    case 3:
      return 6;

    default:
      abort ();
    }
}

static int fpa_unit_blockage PARAMS ((rtx, rtx));
static int
fpa_unit_blockage (executing_insn, candidate_insn)
     rtx executing_insn;
     rtx candidate_insn;
{
  rtx insn;
  int casenum;

  insn = executing_insn;
  switch (recog_memoized (insn))
    {
    case 613:
    case 197:
      extract_constrain_insn_cached (insn);
      casenum = 6;
      break;

    case 614:
    case 611:
    case 200:
    case 194:
      extract_constrain_insn_cached (insn);
      casenum = 6;
      break;

    case 583:
    case 582:
    case 581:
    case 150:
    case 149:
    case 148:
      casenum = 7;
      break;

    case 600:
    case 599:
    case 598:
    case 589:
    case 588:
    case 587:
    case 574:
    case 573:
    case 572:
    case 571:
    case 570:
    case 569:
    case 568:
    case 567:
    case 244:
    case 242:
    case 172:
    case 171:
    case 170:
    case 156:
    case 155:
    case 154:
    case 138:
    case 137:
    case 136:
    case 135:
    case 132:
    case 131:
    case 130:
    case 129:
      casenum = 6;
      break;

    case 548:
    case 542:
    case 76:
    case 70:
      casenum = 0;
      break;

    case 547:
    case 546:
    case 545:
    case 544:
    case 541:
    case 540:
    case 539:
    case 538:
    case 75:
    case 74:
    case 73:
    case 72:
    case 69:
    case 68:
    case 67:
    case 66:
      casenum = 1;
      break;

    case 543:
    case 537:
    case 71:
    case 65:
      casenum = 2;
      break;

    case 536:
    case 535:
    case 534:
    case 533:
    case 532:
    case 64:
    case 63:
    case 62:
    case 61:
    case 60:
      casenum = 3;
      break;

    case 531:
    case 59:
      casenum = 4;
      break;

    case 522:
    case 521:
    case 520:
    case 519:
    case 518:
    case 516:
    case 515:
    case 514:
    case 513:
    case 512:
    case 511:
    case 42:
    case 41:
    case 40:
    case 39:
    case 38:
    case 37:
    case 25:
    case 24:
    case 23:
    case 22:
    case 21:
    case 20:
      casenum = 5;
      break;

    case -1:
      if (GET_CODE (PATTERN (insn)) != ASM_INPUT
          && asm_noperands (PATTERN (insn)) < 0)
        fatal_insn_not_found (insn);
    default:
      casenum = 8;
      break;

    }

  insn = candidate_insn;
  switch (casenum)
    {
    case 0:
      return 69 /* 0x45 */;

    case 1:
      return 57 /* 0x39 */;

    case 2:
      return 29 /* 0x1d */;

    case 3:
      return 7;

    case 4:
      return 4;

    case 5:
      return 2;

    case 6:
      return 2;

    case 7:
      return 3;

    case 8:
      return 2;

    default:
      abort ();
    }
}

static int fpa_unit_conflict_cost PARAMS ((rtx, rtx));
static int
fpa_unit_conflict_cost (executing_insn, candidate_insn)
     rtx executing_insn;
     rtx candidate_insn;
{
  rtx insn;
  int casenum;

  insn = executing_insn;
  switch (recog_memoized (insn))
    {
    case 613:
    case 197:
      extract_constrain_insn_cached (insn);
      casenum = 6;
      break;

    case 614:
    case 611:
    case 200:
    case 194:
      extract_constrain_insn_cached (insn);
      casenum = 6;
      break;

    case 583:
    case 582:
    case 581:
    case 150:
    case 149:
    case 148:
      casenum = 7;
      break;

    case 600:
    case 599:
    case 598:
    case 589:
    case 588:
    case 587:
    case 574:
    case 573:
    case 572:
    case 571:
    case 570:
    case 569:
    case 568:
    case 567:
    case 244:
    case 242:
    case 172:
    case 171:
    case 170:
    case 156:
    case 155:
    case 154:
    case 138:
    case 137:
    case 136:
    case 135:
    case 132:
    case 131:
    case 130:
    case 129:
      casenum = 6;
      break;

    case 548:
    case 542:
    case 76:
    case 70:
      casenum = 0;
      break;

    case 547:
    case 546:
    case 545:
    case 544:
    case 541:
    case 540:
    case 539:
    case 538:
    case 75:
    case 74:
    case 73:
    case 72:
    case 69:
    case 68:
    case 67:
    case 66:
      casenum = 1;
      break;

    case 543:
    case 537:
    case 71:
    case 65:
      casenum = 2;
      break;

    case 536:
    case 535:
    case 534:
    case 533:
    case 532:
    case 64:
    case 63:
    case 62:
    case 61:
    case 60:
      casenum = 3;
      break;

    case 531:
    case 59:
      casenum = 4;
      break;

    case 522:
    case 521:
    case 520:
    case 519:
    case 518:
    case 516:
    case 515:
    case 514:
    case 513:
    case 512:
    case 511:
    case 42:
    case 41:
    case 40:
    case 39:
    case 38:
    case 37:
    case 25:
    case 24:
    case 23:
    case 22:
    case 21:
    case 20:
      casenum = 5;
      break;

    case -1:
      if (GET_CODE (PATTERN (insn)) != ASM_INPUT
          && asm_noperands (PATTERN (insn)) < 0)
        fatal_insn_not_found (insn);
    default:
      casenum = 8;
      break;

    }

  insn = candidate_insn;
  switch (casenum)
    {
    case 0:
      return 69 /* 0x45 */;

    case 1:
      return 57 /* 0x39 */;

    case 2:
      return 29 /* 0x1d */;

    case 3:
      return 7;

    case 4:
      return 4;

    case 5:
      return 2;

    case 6:
      return 2;

    case 7:
      return 3;

    case 8:
      return 2;

    default:
      abort ();
    }
}

const struct function_unit_desc function_units[] = {
  {"fpa", 1, 1, 0, 0, 69, fpa_unit_ready_cost, fpa_unit_conflict_cost, 69, fpa_unit_blockage_range, fpa_unit_blockage}, 
  {"fpa_mem", 2, 1, 0, 1, 1, fpa_mem_unit_ready_cost, 0, 1, 0, 0}, 
  {"write_buf", 4, 1, 2, 0, 6, write_buf_unit_ready_cost, write_buf_unit_conflict_cost, 6, write_buf_unit_blockage_range, write_buf_unit_blockage}, 
  {"write_blockage", 8, 1, 0, 0, 11, write_blockage_unit_ready_cost, write_blockage_unit_conflict_cost, 11, write_blockage_unit_blockage_range, write_blockage_unit_blockage}, 
  {"core", 16, 1, 0, 0, 32, core_unit_ready_cost, core_unit_conflict_cost, 32, core_unit_blockage_range, core_unit_blockage}, 
};

int length_unit_log = 1;
