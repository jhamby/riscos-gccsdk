/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2010 GCCSDK Developers
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * decode.c
 */

#include "config.h"
#include <ctype.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif
#include <stdio.h>

#include "asm.h"
#include "commands.h"
#include "decode.h"
#include "error.h"
#include "filestack.h"
#include "input.h"
#include "local.h"
#include "macros.h"
#include "main.h"
#include "mnemonics.h"
#include "option.h"
#include "storage.h"
#include "variables.h"
#include "whileif.h"

bool ignoreInput = false;

bool
notinput (const char *str)
{
  for (; *str;)
    if (*str++ != inputGetUC ())
      return true;
  return false;
}


static bool
checkspace (void)
{
  if (inputLook () && !isspace (inputGet ()))
    return true;
  skipblanks ();
  return false;
}


static bool
checkstr (const char *str)
{
  if (notinput (str) || (inputLook () && !isspace (inputGet ())))
    return true;
  skipblanks ();
  return false;
}


static bool
checkchr (char chr)
{
  if (inputGetUC () != chr || (inputLook () && !isspace (inputGet ())))
    return true;
  skipblanks ();
  return false;
}


#define C_FINISH_STR(string, fun) \
  if (checkstr (string)) \
    goto illegal; \
  symbol = asm_label (label); \
  macro = false; \
  fun (); \
  break;

#define C_FINISH_FLOW(string, fun) \
  if (checkstr (string)) \
    goto illegal; \
  macro = false; \
  fun (label); \
  break;

#define C_FINISH_VAR(fun) \
  c = inputGetUC (); \
  if (checkspace ()) \
    goto illegal; \
  macro = false; \
  switch (c) \
    { \
      case 'a': \
        fun (ValueInt, label); \
	break; \
      case 'l': \
        fun (ValueBool, label); \
	break; \
      case 's': \
        fun (ValueString, label); \
	break; \
      default: \
        macro = true; \
	goto illegal; \
    } \
  break;

#define C_FINISH_CHR(chr, fun) \
  if (checkchr (chr)) \
    goto illegal; \
  symbol = asm_label (label); \
  macro = false; \
  fun (); \
  break;

#define C_FINISH(fun) \
  if (checkspace ()) \
    goto illegal; \
  symbol = asm_label (label); \
  macro = false; \
  fun (); \
  break;

#define C_FINISH_SYMBOL(fun) \
  if (checkspace ()) \
    goto illegal; \
  symbol = asm_label (label); \
  macro = false; \
  fun (symbol); \
  break;

#define C_FINISH_CHR_SYMBOL(chr, fun) \
  if (checkchr (chr)) \
    goto illegal; \
  symbol = asm_label (label); \
  macro = false; \
  fun (symbol); \
  break;

#define M_FINISH_STR(string, fun, opt) \
  if (notinput (string)) \
    goto illegal; \
  if (optionError == (option = opt ())) \
    goto illegal; \
  skipblanks(); \
  symbol = asm_label (label); \
  macro = false; \
  fun (option); \
  break;

#define M_FINISH_CHR(chr, fun, opt) \
  if (inputGetUC () != chr) \
    goto illegal; \
  if (optionError == (option = opt ())) \
    goto illegal; \
  skipblanks (); \
  symbol = asm_label (label); \
  macro = false; \
  fun (option); \
  break;

#define M_FINISH_LFM(fun) \
  if (inputGetUC () != 'm') \
    goto illegal; \
  option = optionCondLfmSfm (); \
  if (optionError == option) \
    goto illegal; \
  symbol = asm_label (label); \
  macro = false; \
  fun (option); \
  break;

#define M_FINISH(fun, opt) \
  if (optionError == (option = opt ())) \
    goto illegal; \
  skipblanks (); \
  symbol = asm_label (label); \
  macro = false; \
  fun (option); \
  break;

void
decode (const Lex *label)
{
  int c;
  bool macro = true;
  ARMWord option;
  Symbol *symbol;
  inputMark ();
  switch (inputGetUC ())
    {
    case '%':
      C_FINISH (c_reserve);	/* reserve space */
    case '*':
      C_FINISH_SYMBOL (c_equ);	/* equ */
    case '&':
      C_FINISH (c_dcd);		/* DCD    32 */
    case '=':
      C_FINISH (c_dcb);		/* DCB     8 */
    case '^':
      C_FINISH (c_record);	/* start of new record layout */
    case '#':
      C_FINISH_SYMBOL (c_alloc);	/* reserve space in the current record */
    case '!':
      C_FINISH (c_info);	/* INFO shorthand */
    case 'a':
      switch (inputGetUC ())
	{
	case 'b':
	  M_FINISH_CHR ('s', m_abs, optionCondPrecRound);	/* abs CC P R */
	case 'c':
	  M_FINISH_CHR ('s', m_acs, optionCondPrecRound);	/* acs CC P R */
	case 'd':
	  switch (inputGetUC ())
	    {
	    case 'c':
	      M_FINISH (m_adc, optionCondS);	/* adc CC s */
	    case 'd':
	      M_FINISH (m_add, optionCondS);	/* add CC s */
	    case 'f':
	      M_FINISH (m_adf, optionCondPrecRound);	/* adf CC P R */
	    case 'r':
	      M_FINISH (m_adr, optionAdrL);	/* adr CC */
	    default:
	      goto illegal;
	    }
	  break;
	case 'n':
	  M_FINISH_CHR ('d', m_and, optionCondS);	/* and CC S */
	case 'l':
	  C_FINISH_STR ("ign", c_align);	/* .align */
	case 'r':
	  C_FINISH_STR ("ea", c_area);	/* AREA */
	case 's':
	  switch (inputGetUC ())
	    {
	    case 's':
	      C_FINISH_STR ("ert", c_assert);	/* ASSERT */
	    case 'n':
	      M_FINISH (m_asn, optionCondPrecRound);	/* asn CC P R */
	    default:
	      goto illegal;
	    }
	  break;
	case 't':
	  M_FINISH_CHR ('n', m_atn, optionCondPrecRound);	/* atn CC P R */
	default:
	  goto illegal;
	}
      break;
    case 'b':
      switch (c = inputGet ())
	{
	case 'i':
	case 'I':
	  switch (inputGetUC ())
	    {
	    case 'c':
	      M_FINISH (m_bic, optionCondS);	/* bic CC s */
	    case 'n':
	      C_FINISH (c_bin);	/* bin */
	    default:
	      goto illegal;
	    }
	  break;
	case 'k':
	case 'K':
	  C_FINISH_STR("pt", m_bkpt);	/* bkpt */
	case 'l':
	case 'L':
	  if (inputLookUC () == 'x')
	    {
	      inputSkip ();
	      M_FINISH (m_blx, optionCond);	/* blx CC */
	    }
	  else
	    {
	      inputUnGet (c);
	      M_FINISH (m_branch, optionLinkCond);	/* bl CC */
	    }
	  break;
	case 'x':
	case 'X':
	  M_FINISH (m_bx, optionCond);	/* bx CC */
	default:
	  inputUnGet (c);
	  M_FINISH (m_branch, optionLinkCond);	/* b CC */
	  break;
	}
      break;
    case 'c':
      switch (inputGetUC ())
	{
	case 'd':
	  switch (inputGetUC ())
	    {
	    case 'p':
	      switch ((c = inputGet ()))
		{
		case '2':
		  C_FINISH (m_cdp2);	/* cdp2 */
		default:
		  inputUnGet (c);
		  M_FINISH (m_cdp, optionCond);	/* cdp CC */
		}
	      break;
	    default:
	      goto illegal;
	    }
	  break;
	case 'l':
	  M_FINISH_CHR ('z', m_clz, optionCond);	/* clz CC */
	case 'm':
	  switch (inputGetUC ())
	    {
	    case 'f':
	      M_FINISH (m_cmf, optionExceptionCond);	/* cmf CC  or cmfe CC */
	    case 'n':
	      M_FINISH (m_cmn, optionCondSP);	/* cmn CC SP */
	    case 'p':
	      M_FINISH (m_cmp, optionCondSP);	/* cmp CC sp */
	    default:
	      goto illegal;
	    }
	  break;
	case 'n':
	  switch (c = inputGet ())
	    {
	    case 'f':
	    case 'F':
	      M_FINISH (m_cnf, optionExceptionCond);	/* cnf CC or cnfe CC */
	    default:
	      inputUnGet (c);
	      C_FINISH_SYMBOL (c_cn);	/* CN */
	      break;
	    }
	  break;
	case 'o':
	  switch (inputGetUC ())
	    {
	    case 's':
	      M_FINISH (m_cos, optionCondPrecRound);	/* cos CC P R */
	    default:
	      goto illegal;
	    }
	  break;
	case 'p':
	  C_FINISH_SYMBOL (c_cp);	/* CP */
	default:
	  goto illegal;
	}
      break;
    case 'd':
      switch (inputGetUC ())
	{
	case 'v':
	  M_FINISH_CHR ('f', m_dvf, optionCondPrecRound);	/* dvf CC s */
	case 'c':
	  switch (inputGetUC ())
	    {
	    case 'b':
	      C_FINISH (c_dcb);	/* DCB     8 */
	    case 'd':
	      C_FINISH (c_dcd);	/* DCD     32 */
	    case 'f':
	      switch (inputGetUC ())
		{
		case 'd':
		  C_FINISH (c_dcfd);	/* DCFD   64 float */
		case 'e':
		  C_FINISH (c_dcfe);	/* DCFE   80 float */
		case 'p':
		  C_FINISH (c_dcfp);	/* DCFP   80 packed bcd */
		case 's':
		  C_FINISH (c_dcfs);	/* DCFS   32 float */
		}
	      break;
	    case 'w':
	      C_FINISH (c_dcw);	/* DCW     16 */
	    default:
	      goto illegal;
	    }
	  break;
	default:
	  goto illegal;
	}
      break;
    case 'e':
      switch (inputGetUC ())
	{
	case 'n':
	  switch (inputGetUC ())
	    {
	    case 'd':
	      C_FINISH (c_end);	/* END */
	    case 't':
	      inputRollback ();
	      if (inputGet () != 'E')
		goto illegal;
	      if (inputGet () != 'N')
		goto illegal;
	      if (inputGet () != 'T')
		goto illegal;
	      if (inputGet () != 'R')
		goto illegal;
	      if (inputGet () != 'Y')
		goto illegal;

              C_FINISH(c_entry);	/* ENTRY (must be all caps) */
	    default:
	      goto illegal;
	    }
	  break;
	case 'o':
	  M_FINISH_CHR ('r', m_eor, optionCondS);	/* eor CC s */
	case 'q':
	  C_FINISH_CHR_SYMBOL ('u', c_equ);	/* equ */
	case 'x':
	  switch (inputGetUC ())
	    {
	    case 'p':
	      switch (c = inputGet ())
		{
		case 'o':
		case 'O':
		  C_FINISH_STR ("rt", c_globl);		/* EXPORT */
		default:
		  inputUnGet (c);
		  M_FINISH (m_exp, optionCondPrecRound);	/* exp CC P R */
		  break;
		}
	      break;
	    default:
	      goto illegal;
	    }
	  break;
	default:
	  goto illegal;
	}
      break;
    case 'f':
      switch (inputGetUC ())
	{
	case 'd':
	  M_FINISH_CHR ('v', m_fdv, optionCondPrecRound);	/* fdv CC P R */
	case 'i':
	  M_FINISH_CHR ('x', m_fix, optionCondOptRound);	/* fix CC [P] R */
	case 'l':
	  M_FINISH_CHR ('t', m_flt, optionCondPrecRound);	/* flt CC P R */
	case 'm':
	  M_FINISH_CHR ('l', m_fml, optionCondPrecRound);	/* fml CC P R */
	case 'n':
	  C_FINISH_SYMBOL (c_fn);	/* FN */
	case 'r':
	  M_FINISH_CHR ('d', m_frd, optionCondPrecRound);	/* frd CC P R */
	default:
	  goto illegal;
	}
      break;
    case 'g':
      switch (inputGetUC ())
	{
	case 'b':		/* gbla, gbll, gbls */
	  if (inputGetUC () == 'l')
	    {
	      C_FINISH_VAR (c_gbl)
	    }
	  else
	    goto illegal;
	case 'e':
	  C_FINISH_CHR ('t', c_get); /* GET */
	case 'l':
	  C_FINISH_STR ("obl", c_globl);	/* globl  */
	default:
	  goto illegal;
	}
      break;
    case 'h':
      C_FINISH_STR ("ead", c_head);
      break;
    case 'i':
      switch (inputGetUC ())
	{
	case 'd':
	  C_FINISH_STR ("fn", c_idfn);	/* idfn */
	case 'n':
	  switch (inputGetUC ())
	    {
	    case 'c':
	      C_FINISH_STR ("clude", c_get);	/* INCLUDE */
	    case 'f':
	      C_FINISH_STR("o", c_info);	/* INFO */
	    default:
	      goto illegal;
	    }
	  break;
	case 'm':
	  C_FINISH_STR ("port", c_import);	/* IMPORT */
	default:
	  goto illegal;
	}
      break;
    case 'k':
      C_FINISH_STR ("eep", c_keep);	/* KEEP */
    case 'l':
      switch (inputGetUC ())
	{
	case 'c':		/* lcla, lcll, lcls */
	  if (inputGetUC () == 'l')
	    {
	      C_FINISH_VAR (c_lcl)
	    }
	  else
	    goto illegal;
	case 'd':
	  switch (inputGetUC ())
	    {
	    case 'c':
	      switch ((c = inputGet ()))
		{
		case '2':
		  M_FINISH (m_ldc2, optionCondL);	/* ldc2 l */
		default:
		  inputUnGet (c);
		  M_FINISH (m_ldc, optionCondL);	/* ldc CC l */
		}
	      break;
	    case 'f':
	      M_FINISH (m_ldf, optionCondPrec_P);	/* ldf CC P */
	    case 'm':
	      M_FINISH (m_ldm, optionCondDirLdm);	/* ldm CC TYPE */
	    case 'r':
	      M_FINISH (m_ldr, optionCondBT);	/* ldr CC BYTE */
	    default:
	      goto illegal;
	    }
	  break;
	case 'f':
	  M_FINISH_LFM (m_lfm);	/* lfm CC (TYPE) */
	case 'g':
	  M_FINISH_CHR ('n', m_lgn, optionCondPrecRound);	/* lgn CC P R */
	case 'n':
	  C_FINISH_CHR ('k', c_lnk);	/* lnk */
	case 'o':
	  switch (inputGetUC ())
	    {
	    case 'c':
	      if (!option_local)
		goto illegal;
	      C_FINISH_FLOW ("al", c_local);	/* local */
	    case 'g':
	      M_FINISH (m_log, optionCondPrecRound);	/* log CC P R */
	    default:
	      goto illegal;
	    }
	  break;
	case 't':
	  C_FINISH_STR ("org", c_ltorg);	/* ltorg */
	default:
	  goto illegal;
	}
      break;
    case 'm':
      switch (inputGetUC ())
	{
	case 'a':
	  C_FINISH_FLOW ("cro", c_macro);	/* macro */
	case 'c':
	  switch (inputGetUC ())
	    {
	    case 'r':
	      switch ((c = inputGet ()))
		{
		case '2':
		  C_FINISH (m_mcr2);	/* mcr2 */
		case 'r':
		case 'R':
		  M_FINISH (m_mcrr, optionCond);	/* mcrr CC */
		default:
		  inputUnGet (c);
		  M_FINISH (m_mcr, optionCond);	/* mcr CC */
		}
	      break;
	    default:
	      goto illegal;
	    }
	  break;
	case 'e':
	  C_FINISH_FLOW ("xit", c_mexit);	/* mexit */
	case 'l':
	  M_FINISH_CHR ('a', m_mla, optionCondS);	/* mla CC s */
	case 'n':
	  M_FINISH_CHR ('f', m_mnf, optionCondPrecRound);	/* mnf CC P R */
	case 'o':
	  M_FINISH_CHR ('v', m_mov, optionCondS);	/* mov CC s */
	case 'r':
	  switch (inputGetUC ())
	    {
	    case 'c':
	      switch ((c = inputGet ()))
		{
		case '2':
		  C_FINISH (m_mrc2);	/* mrc2 */
		default:
		  inputUnGet (c);
		  M_FINISH (m_mrc, optionCond);	/* mrc CC */
		}
	      break;
	    case 'r':
	      M_FINISH_CHR ('c', m_mrrc, optionCond);	/* mrrc CC */
	    case 's':
	      M_FINISH (m_mrs, optionCond);	/* mrs CC */
	    default:
	      goto illegal;
	    }
	  break;
	case 's':
	  M_FINISH_CHR ('r', m_msr, optionCond);	/* msr CC */
	case 'u':
	  switch (inputGetUC ())
	    {
	    case 'f':
	      M_FINISH (m_muf, optionCondPrecRound);	/* muf CC P R */
	    case 'l':
	      M_FINISH (m_mul, optionCondS);	/* mul CC s */
	    default:
	      goto illegal;
	    }
	  break;
	case 'v':
	  switch (inputGetUC ())
	    {
	    case 'f':
	      M_FINISH (m_mvf, optionCondPrecRound);	/* mvf CC P R */
	    case 'n':
	      M_FINISH (m_mvn, optionCondS);	/* mvn CC s */
	    default:
	      goto illegal;
	    }
	  break;
	default:
	  goto illegal;
	}
      break;
    case 'n':
      switch (inputGetUC ())
	{
	case 'o':
	  C_FINISH_STR ("p", m_nop);	/* nop */
	case 'r':
	  M_FINISH_STR ("m", m_nrm, optionCondPrecRound);	/* nrm CC P R */
	default:
	  goto illegal;
	}
      break;
    case 'o':
      switch (inputGetUC ())
	{
	case 'p':
	  C_FINISH_STR ("t", c_opt);	/* opt */
	case 'r':
	  M_FINISH_STR ("r", m_orr, optionCondS);	/* orr CC s */
	default:
	  goto illegal;
	}
      break;
    case 'p':
      switch (inputGetUC ())
	{
	case 'l':
	  C_FINISH_STR ("d", m_pld);	/* pld */
	case 'o':
	  switch (inputGetUC ())
	    {
	    case 'l':
	      M_FINISH (m_pol, optionCondPrecRound);	/* pol CC P R */
	    case 'w':
	      M_FINISH (m_pow, optionCondPrecRound);	/* pow CC P R */
	    default:
	      goto illegal;
	    }
	  break;
	default:
	  goto illegal;
	}
      break;
    case 'q':
      switch (inputGetUC ())
	{
	case 'a':
	  M_FINISH_STR ("dd", m_qadd, optionCond);	/* qadd CC */
	case 'd':
	  switch (inputGetUC ())
	    {
	    case 'a':
	      M_FINISH_STR ("dd", m_qdadd, optionCond);	/* qdadd CC */
	    case 's':
	      M_FINISH_STR ("ub", m_qdsub, optionCond);	/* qdsub CC */
	    default:
	      goto illegal;
	    }
	  break;
	case 's':
	  M_FINISH_STR("ub", m_qsub, optionCond);	/* qsub CC */
	}
      break;
    case 'r':
      switch (inputGetUC ())
	{
	case 'd':
	  M_FINISH_CHR ('f', m_rdf, optionCondPrecRound);	/* rdf CC P R */
	case 'e':
	  M_FINISH_CHR ('t', m_ret, optionCond);	/* ret CC */
	case 'f':
	  switch (inputGetUC ())
	    {
	    case 'c':
	      M_FINISH (m_rfc, optionCond);	/* rfc CC */
	    case 's':
	      M_FINISH (m_rfs, optionCond);	/* rfs CC */
	    default:
	      goto illegal;
	    }
	  break;
	case 'm':
	  M_FINISH_CHR ('f', m_rmf, optionCondPrecRound);	/* rmf CC P R */
	case 'n':
	  switch (c = inputGet ())
	    {
	    case 'd':
	    case 'D':
	      M_FINISH (m_rnd, optionCondPrecRound);	/* rnd CC P R */
	    default:
	      inputUnGet (c);
	      C_FINISH_SYMBOL (c_rn);	/* rn */
	      break;
	    }
	  break;
	case 'o':
	  C_FINISH_FLOW ("ut", c_rout);		/* rout */
	case 'p':
	  M_FINISH_CHR ('w', m_rpw, optionCondPrecRound);	/* rpw CC P R */
	case 's':
	  switch (inputGetUC ())
	    {
	    case 'b':
	      M_FINISH (m_rsb, optionCondS);	/* rsb CC s */
	    case 'c':
	      M_FINISH (m_rsc, optionCondS);	/* rsc CC s */
	    case 'f':
	      M_FINISH (m_rsf, optionCondPrecRound);	/* rsf CC P R */
	    default:
	      goto illegal;
	    }
	  break;
	default:
	  goto illegal;
	}
      break;
    case 's':
      switch (inputGetUC ())
	{
	case 'b':
	  M_FINISH_CHR ('c', m_sbc, optionCondS);	/* sbc CC s */
	case 'e':		/* seta, setl, sets */
	  if (inputGetUC () == 't')
	    {
	      C_FINISH_VAR (c_set)
	    }
	  else
	    goto illegal;
	case 'f':
	  M_FINISH_LFM (m_sfm);	/* sfm CC (TYPE) */
	case 'i':
	  M_FINISH_CHR ('n', m_sin, optionCondPrecRound);	/* sin CC P R */
	case 'm':
	  switch (inputGetUC ())
	    {
	    case 'u':
	      switch (inputGetUC ())
		{
		case 'l':
		  switch (inputGetUC ())
		    {
		    case 'l':
		      M_FINISH (m_smull, optionCondS);	/* smull CC */
		    case 'b':
		      switch (inputGetUC ())
			{
			case 'b':
			  M_FINISH (m_smulbb, optionCond); /* smulbb CC */
			case 't':
			  M_FINISH (m_smulbt, optionCond); /* smulbt CC */
			default:
			  goto illegal;
			}
		      break;
		    case 't':
		      switch (inputGetUC ())
			{
			case 'b':
			  M_FINISH (m_smultb, optionCond); /* smultb CC */
			case 't':
			  M_FINISH (m_smultt, optionCond); /* smultt CC */
			default:
			  goto illegal;
			}
		      break;
		    case 'w':
		      switch (inputGetUC ())
			{
			case 'b':
			  M_FINISH (m_smulwb, optionCond); /* smulwb CC */
			case 't':
			  M_FINISH (m_smulwt, optionCond); /* smulwt CC */
			default:
			  goto illegal;
			}
		      break;
		    }
		  break;
		}
	      break;
	    case 'l':
	      switch (inputGetUC ())
		{
		case 'a':
		  switch (inputGetUC ())
		    {
		    case 'l':
		      switch ((c = inputGet ()))
			{
			case 'b':
			case 'B':
			  switch (inputGetUC ())
			    {
			    case 'b':
			      M_FINISH (m_smlalbb, optionCond); /* smlalbb CC */
			    case 't':
			      M_FINISH (m_smlalbt, optionCond); /* smlalbt CC */
			    default:
			      goto illegal;
			    }
			  break;
			case 't':
			case 'T':
			  switch (inputGetUC ())
			    {
			    case 'b':
			      M_FINISH (m_smlaltb, optionCond); /* smlaltb CC */
			    case 't':
			      M_FINISH (m_smlaltt, optionCond); /* smlaltt CC */
			    default:
			      goto illegal;
			    }
			  break;
			default:
			  inputUnGet (c);
			  M_FINISH (m_smlal, optionCondS); /* smlal CC S */
			}
		      break;
		    case 'b':
		      switch (inputGetUC ())
			{
			case 'b':
			  M_FINISH (m_smlabb, optionCond); /* smlabb CC */
			case 't':
			  M_FINISH (m_smlabt, optionCond); /* smlabt CC */
			default:
			  goto illegal;
			}
		      break;
		    case 't':
		      switch (inputGetUC ())
			{
			case 'b':
			  M_FINISH (m_smlatb, optionCond); /* smlatb CC */
			case 't':
			  M_FINISH (m_smlatt, optionCond); /* smlatt CC */
			default:
			  goto illegal;
			}
		      break;
		    case 'w':
		      switch (inputGetUC ())
			{
			case 'b':
			  M_FINISH (m_smlawb, optionCond); /* smlawb CC */
			case 't':
			  M_FINISH (m_smlawt, optionCond); /* smlawt CC */
			default:
			  goto illegal;
			}
		      break;
		    }
		  break;
		default:
		  goto illegal;
		}
	      break;
	    default:
	      goto illegal;
	    }
	  break;
	case 'q':
	  M_FINISH_CHR ('t', m_sqt, optionCondPrecRound);	/* sqt CC P R */
	case 't':
	  switch (inputGetUC ())
	    {
	    case 'a':
	      C_FINISH_STR ("ck", m_stack);	/* stack CC */
	    case 'c':
	      switch ((c = inputGet ()))
		{
		case '2':
		  M_FINISH (m_stc2, optionCondL);	/* stc2 l */
		default:
		  inputUnGet (c);
		  M_FINISH (m_stc, optionCondL);	/* stc CC l */
		}
	      break;
	    case 'f':
	      M_FINISH (m_stf, optionCondPrec_P);	/* stf CC P */
	    case 'm':
	      M_FINISH (m_stm, optionCondDirStm);	/* stm CC TYPE */
	    case 'r':
	      switch (c = inputGet ())
		{
		case 'o':
		case 'O':
		  C_FINISH_STR ("ng", c_strong);	/* STRONG */
		default:
		  inputUnGet (c);
		  M_FINISH (m_str, optionCondBT);	/* str CC BYTE */
		  break;
		}
	      break;
	    default:
	      goto illegal;
	    }
	  break;
	case 'u':
	  switch (inputGetUC ())
	    {
	    case 'b':
               switch (c = inputGet ())
                 {
                 case 't':
                 case 'T':
                   C_FINISH (c_title);                  /* SUBT */
                 default:
                   inputUnGet (c);
                   M_FINISH (m_sub, optionCondS);	/* sub CC s */
                 }
               break;
	    case 'f':
	      M_FINISH (m_suf, optionCondPrecRound);	/* suf CC P R */
	    default:
	      goto illegal;
	    }
	  break;
	case 'w':
	  switch (inputGetUC ())
	    {
	    case 'i':
	      M_FINISH (m_swi, optionCond);	/* swi CC */
	    case 'p':
	      M_FINISH (m_swp, optionCondB);	/* swp CC BYTE */
	    default:
	      goto illegal;
	    }
	  break;
	default:
	  goto illegal;
	}
      break;
    case 't':
      switch (inputGetUC ())
	{
	case 'a':
	  switch (inputGetUC ())
	    {
	    case 'i':
	      M_FINISH_CHR ('l', m_tail, optionCond);	/* tail CC */
	    case 'n':
	      M_FINISH (m_tan, optionCondPrecRound);	/* tan CC P R */
	    default:
	      goto illegal;
	    }
	  break;
	case 'e':
	  M_FINISH_CHR ('q', m_teq, optionCondSP);	/* teq CC sp */
	case 's':
	  M_FINISH_CHR ('t', m_tst, optionCondSP);	/* tst CC sp */
	case 't':
	  C_FINISH_CHR ('l', c_title);			/* TTL */
	default:
	  goto illegal;
	}
      break;
    case 'u':
      switch (inputGetUC ())
	{
	case 'm':
	  switch (inputGetUC ())
	    {
	    case 'u':
	      M_FINISH_STR ("ll", m_umull, optionCondS);	/* umull CC */
	    case 'l':
	      M_FINISH_STR ("al", m_umlal, optionCondS);	/* umlal CC */
	    default:
	      goto illegal;
	    }
	  break;
	case 'r':
	  M_FINISH_CHR ('d', m_urd, optionCondPrecRound);	/* urd CC P R */
	default:
	  goto illegal;
	}
      break;
    case 'w':
      switch (inputGetUC ())
	{
	case 'e':
	  C_FINISH_FLOW ("nd", c_wend);		/* wend */
	case 'f':
	  switch (inputGetUC ())
	    {
	    case 'c':
	      M_FINISH (m_wfc, optionCond);	/* wfc CC */
	    case 's':
	      M_FINISH (m_wfs, optionCond);	/* wfs CC */
	    default:
	      goto illegal;
	    }
	  break;
	case 'h':
	  C_FINISH_FLOW ("ile", c_while);	/* while */
	default:
	  goto illegal;
	}
      break;
    case '[':
      if (inputLook () && !isspace (inputGet ()))
	goto illegal;
      skipblanks ();
      asm_label (label);
      c_if ();
      break;
    case '|':
      if (inputLook () && !isspace (inputGet ()))
	goto illegal;
      skipblanks ();
      c_else (label);
      break;
    case ']':
      if (inputLook () && !isspace (inputGet ()))
	goto illegal;
      skipblanks ();
      c_endif (label);
      break;
    default:
illegal:
      { /* Is it a macro call? */
	const Macro *m;
	if (macro)
	  {
	    inputRollback ();
	    int l;
	    char *ci;
	    if (inputLook () == '|')
	      {
		inputSkip ();
		ci = inputSymbol (&l, '|');
		if (inputGet () != '|')
		  error (ErrorError, "Identifier continues over newline");
	      }
	    else
	      ci = inputSymbol (&l, '\0');
	    m = macroFind (l, ci);
	  }
	else
	  m = NULL;
	if (m)
	  macroCall (m, label);
	else
	  {
	    inputRollback ();
	    errorAbort ("Illegal line \"%s\"", inputRest ());
	  }
      break;
      }
    }
  skipblanks ();
  if (!inputComment ())
    errorAbort ("Skipping extra characters '%s'", inputRest ());
}
