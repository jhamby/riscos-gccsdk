/*
 * decode.c
 * Copyright © 1992 Niklas Röjemo
 */
#include "sdk-config.h"
#include <ctype.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include <stdio.h>
#include "decode.h"
#include "input.h"
#include "commands.h"
#include "whileif.h"
#include "variables.h"
#include "mnemonics.h"
#include "storage.h"
#include "option.h"
#include "error.h"
#include "filestack.h"
#include "macros.h"
#include "asm.h"
#include "local.h"


extern FILE *asmfile;		/* in input.c */
extern int local;		/* in main.c */

BOOL ignoreInput = FALSE;
int returnvalue = 0;


BOOL 
notinput (char *str)
{
  for (; *str;)
    if (*str++ != inputGetUC ())
      return TRUE;
  return FALSE;
}


static BOOL 
checkspace (void)
{
  if (inputLook () && !isspace (inputGet ()))
    return TRUE;
  skipblanks ();
  return FALSE;
}


static BOOL 
checkstr (char *str)
{
  if (notinput (str) || (inputLook () && !isspace (inputGet ())))
    return TRUE;
  skipblanks ();
  return FALSE;
}


static BOOL 
checkchr (char chr)
{
  if (inputGetUC () != chr || (inputLook () && !isspace (inputGet ())))
    return TRUE;
  skipblanks ();
  return FALSE;
}


#define C_FINISH_STR(string,fun)	    \
	if (checkstr(string)) goto illegal; \
	symbol=asm_label(label);	    \
	macro=FALSE; fun();		    \
	break;

#define C_FINISH_FLOW(string,fun)	   \
	if(checkstr(string)) goto illegal; \
	macro=FALSE; fun(label);	   \
	break;

#define C_FINISH_VAR(fun)			 \
	c=inputGetUC();			 \
	if (checkspace()) goto illegal;		 \
	macro=FALSE;				 \
	switch (c) {				 \
	  case 'a': fun(ValueInt,label); break;	 \
	  case 'l': fun(ValueBool,label); break; \
	  case 's': fun(ValueString,label);	 \
	} break;

#define C_FINISH_CHR(chr,fun)		 \
	if (checkchr(chr)) goto illegal; \
	symbol=asm_label(label);	 \
	macro=FALSE; fun();		 \
	break;

#define C_FINISH(fun)			\
	if (checkspace()) goto illegal;	\
	symbol=asm_label(label);	\
	macro=FALSE; fun();		\
	break;

#define C_FINISH_SYMBOL(fun)		\
	if (checkspace()) goto illegal;	\
	symbol=asm_label(label);	\
	macro=FALSE; fun(symbol);	\
	break;

#define C_FINISH_CHR_SYMBOL(chr,fun)	 \
	if (checkchr(chr)) goto illegal; \
	symbol=asm_label(label);	 \
	macro=FALSE; fun(symbol);	 \
	break;

#define M_FINISH_STR(string,fun,opt)	    \
	if (notinput(string)) goto illegal; \
	if (optionError==(option=opt()))    \
	  goto illegal;			    \
	skipblanks();			    \
	symbol=asm_label(label);	    \
	macro=FALSE; fun(option);	    \
	break;

#define M_FINISH_CHR(chr,fun,opt)	   \
	if (inputGetUC()!=chr) goto illegal; \
	if (optionError==(option=opt()))   \
	  goto illegal;			   \
	skipblanks();			   \
	symbol=asm_label(label);	   \
	macro=FALSE; fun(option);	   \
	break;

#define M_FINISH_LFM(fun)	   \
	if (inputGetUC()!='m')	   \
	  goto illegal;		   \
	option=optionCondLfmSfm(); \
	if(optionError==option)	   \
	  goto illegal;		   \
	symbol=asm_label(label);   \
	macro=FALSE; fun(option);  \
	break;

#define M_FINISH(fun,opt)		 \
	if (optionError==(option=opt())) \
	  goto illegal;			 \
	skipblanks();			 \
	symbol=asm_label(label);	 \
	macro=FALSE; fun(option);	 \
	break;

int 
decode (Lex * label)
{
  int c;
  BOOL macro = TRUE;
  WORD option;
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
	default:
	  inputUnGet (c);
	  M_FINISH (m_branch, optionLinkCond);	/* b, bl */
	}
      break;
    case 'c':
      switch (inputGetUC ())
	{
	case 'd':
	  M_FINISH_CHR ('p', m_cdp, optionCond);	/* cdp CC */
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
	      C_FINISH_STR ("ry", c_entry);	/* entry */
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
	  C_FINISH_CHR ('t', c_get);
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
	case 'm':
	  C_FINISH_STR ("port", c_import);	/*IMPORT */
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
	      M_FINISH (m_ldc, optionCondL);	/* ldc CC l */
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
	      if (!local)
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
	  M_FINISH_CHR ('r', m_mcr, optionCond);	/* mcr CC */
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
	      M_FINISH (m_mrc, optionCond);	/* mrc CC */
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
      M_FINISH_STR ("rr", m_orr, optionCondS);	/* orr CC s */
    case 'p':
      switch (inputGetUC ())
	{
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
	      M_FINISH_STR ("ll", m_smull, optionCondS);	/* smull CC */
	    case 'l':
	      M_FINISH_STR ("al", m_smlal, optionCondS);	/* smlal CC */
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
	      M_FINISH (m_stc, optionCondL);	/* stc CC l */
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
	      M_FINISH (m_sub, optionCondS);	/* sub CC s */
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
      symbol = asm_label (label);
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
      {
	int l;			/* Is it a macro call? */
	char *c;
	Macro *m;
	if (macro)
	  {
	    inputRollback ();
	    if (inputLook () == '|')
	      {
		inputSkip ();
		c = inputSymbol (&l, '|', 1);
		if (inputGet () != '|')
		  error (ErrorError, TRUE,
			 "Identifier continues over newline");
	      }
	    else
	      c = inputSymbol (&l, '\0', 1);
	    m = macroFind (l, c);
	  }
	if (macro && m)
	  macroCall (m, label);
	else
	  {
	    inputRollback ();
	    error (ErrorError, FALSE, "Illegal line \"%s\"", inputRest ());
	  }
      }
    }
  skipblanks ();
  if (!inputComment ())
    {
      error (ErrorError, FALSE, "Skipping extra characters '%s'", inputRest ());
    }
  return returnvalue;
}
