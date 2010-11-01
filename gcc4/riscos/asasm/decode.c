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

#include <assert.h>
#include <ctype.h>
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif
#include <stdio.h>
#include <string.h>

#include "area.h"
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


bool
notinput (const char *str)
{
  while (*str)
    if (*str++ != inputGet ())
      return true;
  return false;
}


/**
 * Checks there is still input remaining (comments are not considered as
 * valid input here) and if so, check there is at least one space at the
 * current input and if so, discard that and all subsequent spaces.
 * \return true when condition is not fulfilled, false otherwise.
 */
static bool
checkspace (void)
{
  if (Input_IsEolOrCommentStart ())
    return false;
  char c = inputGet ();
  if (!isspace ((unsigned int)c))
    return true;
  skipblanks ();
  return false;
}


/**
 * Checks there is at least the given string + space at the current input and
 * if so, discard that and all subsequent spaces.
 * \param str string to check for at current input.
 * \return true when condition is not fulfilled, false otherwise.
 */
static bool
checkstr (const char *str)
{
  if (notinput (str))
    return true;
  return checkspace ();
}


/**
 * Checks there is at least the given character + space at the current input
 * and if so, discard that and all subsequent spaces.
 * \param chr Character to check for at current input.
 * \return true when condition is not fulfilled, false otherwise.
 */
static bool
checkchr (char chr)
{
  if (inputGet () != chr)
    return true;
  return checkspace ();
}


#define C_FINISH_STR(string, fun) \
  do \
    { \
      if (checkstr (string)) \
        goto illegal; \
      labelSymbol = asm_label (label); \
      fun (); \
    } while (0)

#define C_FINISH_CHR(chr, fun) \
  do \
    { \
      if (checkchr (chr)) \
        goto illegal; \
      labelSymbol = asm_label (label); \
      fun (); \
    } while (0)

#define C_FINISH(fun) \
  do \
    { \
      if (checkspace ()) \
        goto illegal; \
      labelSymbol = asm_label (label); \
      fun (); \
    } while (0)

#define C_FINISH_FLOW(string, fun) \
  do \
    { \
      if (checkstr (string)) \
        goto illegal; \
      labelSymbol = NULL; \
      fun (label); \
    } while (0)

#define C_FINISH_VAR(fun) \
  do \
    { \
      int c = inputGet (); \
      if (checkspace ()) \
        goto illegal; \
      labelSymbol = NULL; \
      switch (c) \
        { \
          case 'A': \
            fun (ValueInt, label); \
            break; \
          case 'L': \
            fun (ValueBool, label); \
            break; \
          case 'S': \
            fun (ValueString, label); \
            break; \
          default: \
            goto illegal; \
        } \
    } while (0)

#define C_FINISH_SYMBOL(fun) \
  do \
    { \
      if (checkspace ()) \
        goto illegal; \
      labelSymbol = asm_label (label); \
      fun (labelSymbol); \
    } while (0)

#define C_FINISH_CHR_SYMBOL(chr, fun) \
  do \
    { \
      if (checkchr (chr)) \
        goto illegal; \
      labelSymbol = asm_label (label); \
      fun (labelSymbol); \
    } while (0)

#define M_FINISH_STR(string, fun, opt) \
  do \
    { \
      if (notinput (string)) \
        goto illegal; \
      ARMWord option; \
      if (optionError == (option = opt ())) \
        goto illegal; \
      skipblanks(); \
      labelSymbol = asm_label (label); \
      fun (option); \
    } while (0)

#define M_FINISH_CHR(chr, fun, opt) \
  do \
    { \
      if (inputGet () != chr) \
        goto illegal; \
      ARMWord option; \
      if (optionError == (option = opt ())) \
        goto illegal; \
      skipblanks (); \
      labelSymbol = asm_label (label); \
      fun (option); \
    } while (0)

#define M_FINISH_LFM(fun) \
  do \
    { \
      if (inputGet () != 'M') \
        goto illegal; \
      ARMWord option = optionCondLfmSfm (); \
      if (optionError == option) \
        goto illegal; \
      labelSymbol = asm_label (label); \
      fun (option); \
    } while (0)

#define M_FINISH(fun, opt) \
  do \
    { \
      ARMWord option; \
      if (optionError == (option = opt ())) \
        goto illegal; \
      skipblanks (); \
      labelSymbol = asm_label (label); \
      fun (option); \
    } while (0)

/**
 * Decode one assembler line.
 * \param label A non-NULL ptr to the starting label.  Its tag is LexId when
 * there was one found, it's LexNone when there wasn't one.
 */
void
decode (const Lex *label)
{
  const char * const inputMark = Input_GetMark ();

  const int startOffset = areaCurrentSymbol ? areaCurrentSymbol->value.Data.Int.i : 0;
  const Value startStorage = storageValue ();
  const int startStorageOffset = startStorage.Tag == ValueInt ? startStorage.Data.Int.i : startStorage.Data.Addr.i;

  Symbol *labelSymbol;
  switch (inputGet ())
    {
      case '%':
        C_FINISH (c_reserve); /* Reserve space.  */
        break;

      case '*':
        C_FINISH_SYMBOL (c_equ); /* EQU */
        break;

      case '&':
        C_FINISH (c_dcd); /* DCD (32 bits) */
        break;
        
      case '=':
        C_FINISH (c_dcb); /* DCB (8 bits) */
        break;
        
      case '^':
        C_FINISH (c_record); /* Start of new record layout.  */
        break;
        
      case '#':
        C_FINISH_SYMBOL (c_alloc); /* Reserve space in the current record.  */
        break;
        
      case '!':
        C_FINISH (c_info); /* INFO shorthand */
        break;
        
      case 'A':
        switch (inputGet ())
          {
            case 'B':
              M_FINISH_CHR ('S', m_abs, optionCondPrecRound); /* ABS CC P R */
              break;
            case 'C':
              M_FINISH_CHR ('S', m_acs, optionCondPrecRound); /* ACS CC P R */
              break;
            case 'D':
              switch (inputGet ())
                {
                  case 'C':
                    M_FINISH (m_adc, optionCondS); /* ADC CC s */
                    break;
                  case 'D':
                    M_FINISH (m_add, optionCondS); /* ADD CC s */
                    break;
                  case 'F':
                    M_FINISH (m_adf, optionCondPrecRound); /* ADF CC P R */
                    break;
                  case 'R':
                    M_FINISH (m_adr, optionAdrL); /* ADR CC */
                    break;
                  default:
                    goto illegal;
                }
              break;
            case 'N':
              M_FINISH_CHR ('D', m_and, optionCondS); /* AND CC S */
              break;
            case 'L':
              C_FINISH_STR ("IGN", c_align); /* ALIGN */
              break;
            case 'R':
              C_FINISH_STR ("EA", c_area); /* AREA */
              break;
            case 'S':
              switch (inputGet ())
                {
                  case 'S':
                    C_FINISH_STR ("ERT", c_assert); /* ASSERT */
                    break;
                  case 'N':
                    M_FINISH (m_asn, optionCondPrecRound); /* ASN CC P R */
                    break;
                  default:
                    goto illegal;
                }
              break;
            case 'T':
              M_FINISH_CHR ('N', m_atn, optionCondPrecRound); /* ATN CC P R */
              break;
            default:
              goto illegal;
          }
        break;

      case 'B':
        {
          int c;
          switch (c = inputGet ())
            {
              case 'I':
                switch (inputGet ())
                  {
                    case 'C':
                      M_FINISH (m_bic, optionCondS); /* BIC CC s */
                      break;
                    case 'N':
                      C_FINISH (c_bin); /* BIN */
                      break;
                    default:
                      goto illegal;
                  }
                break;
              case 'K':
                C_FINISH_STR ("PT", m_bkpt); /* BKPT */
                break;
              case 'L':
		{
		  if (inputLook () == 'X')
		    {
		      inputSkip ();
                      M_FINISH (m_blx, optionCond); /* BLX CC */
		    }
                  else
                    {
                      inputUnGet (c);
                      M_FINISH (m_branch, optionLinkCond); /* BL CC */
                    }
		}
                break;
              case 'X':
                M_FINISH (m_bx, optionCond);  /* BX CC */
                break;
              default:
                inputUnGet (c);
                M_FINISH (m_branch, optionCond); /* B CC */
                break;
            }
        }
        break;

      case 'C':
        switch (inputGet ())
          {
            case 'D':
              switch (inputGet ())
                {
                  case 'P':
                    {
                      int c;
                      switch ((c = inputGet ()))
                        {
                          case '2':
                            C_FINISH (m_cdp2); /* CDP2 */
                            break;
                          default:
                            inputUnGet (c);
                            M_FINISH (m_cdp, optionCond); /* CDP CC */
                            break;
                        }
                    }
                    break;
                  default:
                    goto illegal;
                }
              break;
            case 'L':
              M_FINISH_CHR ('Z', m_clz, optionCond); /* CLZ CC */
              break;
            case 'M':
              switch (inputGet ())
                {
                  case 'F':
                    M_FINISH (m_cmf, optionExceptionCond); /* CMF CC  or CMFE CC */
                    break;
                  case 'N':
                    M_FINISH (m_cmn, optionCondSP); /* CMN CC SP */
                    break;
                  case 'P':
                    M_FINISH (m_cmp, optionCondSP); /* CMP CC SP */
                    break;
                  default:
                    goto illegal;
                }
              break;
            case 'N':
              {
                int c;
                switch (c = inputGet ())
                  {
                    case 'F':
                      M_FINISH (m_cnf, optionExceptionCond); /* CNF CC or CNFE CC */
                      break;
                    default:
                      inputUnGet (c);
                      C_FINISH_SYMBOL (c_cn); /* CN */
                      break;
                  }
              }
              break;
            case 'O':
              switch (inputGet ())
                {
                  case 'S':
                    M_FINISH (m_cos, optionCondPrecRound); /* COS CC P R */
                    break;
                  default:
                    goto illegal;
                }
              break;
            case 'P':
              C_FINISH_SYMBOL (c_cp); /* CP */
              break;
            default:
              goto illegal;
          }
        break;

      case 'D':
        switch (inputGet ())
          {
            case 'V':
              M_FINISH_CHR ('F', m_dvf, optionCondPrecRound); /* DVF CC S */
              break;
            case 'C':
              switch (inputGet ())
                {
                  case 'B':
                    C_FINISH (c_dcb); /* DCB (8 bits) */
                    break;
                  case 'D':
                    C_FINISH (c_dcd); /* DCD (32 bits) */
                    break;
                  case 'F':
                    switch (inputGet ())
                      {
                        case 'D':
                          C_FINISH (c_dcfd); /* DCFD (64 bits float) */
                          break;
                        case 'S':
                          C_FINISH (c_dcfs); /* DCFS (32 bits float) */
                          break;
                        default:
                          goto illegal;
                      }
                    break;
                  case 'W':
                    C_FINISH (c_dcw); /* DCW (16 bits) */
                    break;
                  default:
                    goto illegal;
                }
              break;
            default:
              goto illegal;
          }
        break;

      case 'E':
        switch (inputGet ())
          {
            case 'N':
              switch (inputGet ())
                {
                  case 'D':
                    C_FINISH (c_end); /* END */
                    break;
                  case 'T':
                    C_FINISH_STR ("RY", c_entry); /* ENTRY */
                    break;
                  default:
                    goto illegal;
                }
              break;
            case 'O':
              M_FINISH_CHR ('R', m_eor, optionCondS); /* EOR CC S */
              break;
            case 'Q':
              C_FINISH_CHR_SYMBOL ('U', c_equ); /* EQU */
              break;
            case 'X':
              switch (inputGet ())
                {
                  case 'P':
                    {
                      int c;
                      switch (c = inputGet ())
                        {
                          case 'O':
                            C_FINISH_STR ("RT", c_globl); /* EXPORT */
                            break;
                          default:
                            inputUnGet (c);
                            M_FINISH (m_exp, optionCondPrecRound); /* EXP CC P R */
                            break;
                        }
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

      case 'F':
        switch (inputGet ())
          {
            case 'D':
              M_FINISH_CHR ('V', m_fdv, optionCondPrecRound); /* FDV CC P R */
              break;
            case 'I':
              M_FINISH_CHR ('X', m_fix, optionCondOptRound); /* FIX CC [P] R */
              break;
            case 'L':
              M_FINISH_CHR ('T', m_flt, optionCondPrecRound); /* FLT CC P R */
              break;
            case 'M':
              M_FINISH_CHR ('L', m_fml, optionCondPrecRound); /* FML CC P R */
              break;
            case 'N':
              C_FINISH_SYMBOL (c_fn); /* FN */
              break;
            case 'R':
              M_FINISH_CHR ('D', m_frd, optionCondPrecRound); /* FRD CC P R */
              break;
            default:
              goto illegal;
          }
        break;

      case 'G':
        switch (inputGet ())
          {
            case 'B': /* GBLA, GBLL, GBLS */
              if (inputGet () != 'L')
                goto illegal;
              C_FINISH_VAR (c_gbl);
              break;
            case 'E':
              C_FINISH_CHR ('T', c_get); /* GET */
              break;
            case 'L':
              C_FINISH_STR ("OBL", c_globl); /* GLOBL  */
              break;
            default:
              goto illegal;
          }
        break;

      case 'H':
        C_FINISH_STR ("EAD", c_head); /* HEAD */
        break;

      case 'I':
        switch (inputGet ())
          {
            case 'D':
              C_FINISH_STR ("FN", c_idfn); /* IDFN */
              break;
            case 'N':
              switch (inputGet ())
                {
                  case 'C':
                    C_FINISH_STR ("LUDE", c_get); /* INCLUDE */
                    break;
                  case 'F':
                    C_FINISH_STR("O", c_info); /* INFO */
                    break;
                  default:
                    goto illegal;
                }
              break;
            case 'M':
              C_FINISH_STR ("PORT", c_import); /* IMPORT */
              break;
            default:
              goto illegal;
          }
        break;

      case 'K':
        C_FINISH_STR ("EEP", c_keep); /* KEEP */
        break;        

      case 'L':
        switch (inputGet ())
          {
            case 'C': /* LCLA, LCLL, LCLS */
              if (inputGet () != 'L')
                goto illegal;
              C_FINISH_VAR (c_lcl);
              break;
            case 'D':
              switch (inputGet ())
                {
                  case 'C':
                    {
                      int c;
                      switch ((c = inputGet ()))
                        {
                          case '2':
                            M_FINISH (m_ldc2, optionCondL); /* LDC2 L */
                            break;
                          default:
                            inputUnGet (c);
                            M_FINISH (m_ldc, optionCondL); /* LDC CC l */
                            break;
                        }
                    }
                    break;
                  case 'F':
                    M_FINISH (m_ldf, optionCondPrec_P); /* LDF CC P */
                    break;
                  case 'M':
                    M_FINISH (m_ldm, optionCondDirLdm); /* LDM CC TYPE */
                    break;
                  case 'R':
                    M_FINISH (m_ldr, optionCondBT); /* LDR CC BYTE */
                    break;
                  default:
                    goto illegal;
                }
              break;
            case 'F':
              M_FINISH_LFM (m_lfm); /* LFM CC (TYPE) */
              break;
            case 'G':
              M_FINISH_CHR ('N', m_lgn, optionCondPrecRound); /* LGN CC P R */
              break;
            case 'N':
              C_FINISH_CHR ('K', c_lnk); /* LNK */
              break;
            case 'O':
              switch (inputGet ())
                {
                  case 'C':
                    if (!option_local)
                      goto illegal;
                    C_FINISH_FLOW ("AL", c_local); /* LOCAL */
                    break;
                  case 'G':
                    M_FINISH (m_log, optionCondPrecRound); /* LOG CC P R */
                    break;
                  default:
                    goto illegal;
                }
              break;
            case 'T':
              C_FINISH_STR ("ORG", c_ltorg); /* LTORG */
              break;
            default:
              goto illegal;
          }
        break;

      case 'M':
        switch (inputGet ())
          {
            case 'A':
              C_FINISH_FLOW ("CRO", c_macro); /* MACRO */
              break;
            case 'C':
              switch (inputGet ())
                {
                  case 'R':
                    {
                      int c;
                      switch ((c = inputGet ()))
                        {
                          case '2':
                            C_FINISH (m_mcr2); /* MCR2 */
                            break;
                          case 'R':
                            M_FINISH (m_mcrr, optionCond); /* MCRR CC */
                            break;
                          default:
                            inputUnGet (c);
                            M_FINISH (m_mcr, optionCond); /* MCR CC */
                            break;
                        }
                    }
                    break;
                  default:
                    goto illegal;
                }
              break;
            case 'E':
              C_FINISH_FLOW ("XIT", c_mexit); /* MEXIT */
              break;
            case 'L':
              M_FINISH_CHR ('A', m_mla, optionCondS); /* MLA CC s */
              break;
            case 'N':
              M_FINISH_CHR ('F', m_mnf, optionCondPrecRound); /* MNF CC P R */
              break;
            case 'O':
              M_FINISH_CHR ('V', m_mov, optionCondS); /* MOV CC s */
              break;
            case 'R':
              switch (inputGet ())
                {
                  case 'C':
                    {
                      int c;
                      switch ((c = inputGet ()))
                        {
                          case '2':
                            C_FINISH (m_mrc2); /* MRC2 */
                            break;
                          default:
                            inputUnGet (c);
                            M_FINISH (m_mrc, optionCond); /* MRC CC */
                            break;
                        }
                    }
                    break;
                  case 'R':
                    M_FINISH_CHR ('C', m_mrrc, optionCond); /* MRRC CC */
                    break;
                  case 'S':
                    M_FINISH (m_mrs, optionCond); /* MRS CC */
                    break;
                  default:
                    goto illegal;
                }
              break;
            case 'S':
              M_FINISH_CHR ('R', m_msr, optionCond); /* MSR CC */
              break;
            case 'U':
              switch (inputGet ())
                {
                  case 'F':
                    M_FINISH (m_muf, optionCondPrecRound); /* MUF CC P R */
                    break;
                  case 'L':
                    M_FINISH (m_mul, optionCondS); /* MUL CC s */
                    break;
                  default:
                    goto illegal;
                }
              break;
            case 'V':
              switch (inputGet ())
                {
                  case 'F':
                    M_FINISH (m_mvf, optionCondPrecRound); /* MVF CC P R */
                    break;
                  case 'N':
                    M_FINISH (m_mvn, optionCondS); /* MVN CC s */
                    break;
                  default:
                    goto illegal;
                }
              break;
            default:
              goto illegal;
          }
        break;

      case 'N':
        switch (inputGet ())
          {
            case 'O':
              C_FINISH_CHR ('P', m_nop); /* NOP */
              break;
            case 'R':
              M_FINISH_CHR ('M', m_nrm, optionCondPrecRound); /* NRM CC P R */
              break;
            default:
              goto illegal;
          }
        break;

      case 'O':
        switch (inputGet ())
          {
            case 'P':
              C_FINISH_CHR ('T', c_opt); /* OPT */
              break;
            case 'R':
              M_FINISH_CHR ('R', m_orr, optionCondS); /* ORR CC s */
              break;
            default:
              goto illegal;
          }
        break;

      case 'P':
        switch (inputGet ())
          {
            case 'L':
              C_FINISH_CHR ('D', m_pld); /* PLD */
              break;
            case 'O':
              switch (inputGet ())
                {
                  case 'L':
                    M_FINISH (m_pol, optionCondPrecRound); /* POL CC P R */
                    break;
                  case 'W':
                    M_FINISH (m_pow, optionCondPrecRound); /* POW CC P R */
                    break;
                  default:
                    goto illegal;
                }
              break;
            default:
              goto illegal;
          }
        break;

      case 'Q':
        switch (inputGet ())
          {
            case 'A':
              M_FINISH_STR ("DD", m_qadd, optionCond); /* QADD CC */
              break;
            case 'D':
              switch (inputGet ())
                {
                  case 'A':
                    M_FINISH_STR ("DD", m_qdadd, optionCond); /* QDADD CC */
                    break;
                  case 'S':
                    M_FINISH_STR ("UB", m_qdsub, optionCond); /* QDSUB CC */
                    break;
                  default:
                    goto illegal;
                }
              break;
            case 'S':
              M_FINISH_STR("UB", m_qsub, optionCond); /* QSUB CC */
              break;
            default:
              goto illegal;
          }
        break;

      case 'R':
        switch (inputGet ())
          {
            case 'D':
              M_FINISH_CHR ('F', m_rdf, optionCondPrecRound); /* RDF CC P R */
              break;
            case 'E':
              M_FINISH_CHR ('T', m_ret, optionCond); /* RET CC */
              break;
            case 'F':
              switch (inputGet ())
                {
                  case 'C':
                    M_FINISH (m_rfc, optionCond); /* RFC CC */
                    break;
                  case 'S':
                    M_FINISH (m_rfs, optionCond); /* RFS CC */
                    break;
                  default:
                    goto illegal;
                }
              break;
            case 'M':
              M_FINISH_CHR ('F', m_rmf, optionCondPrecRound); /* RMF CC P R */
              break;
            case 'N':
              {
                int c;
                switch (c = inputGet ())
                  {
                    case 'D':
                      M_FINISH (m_rnd, optionCondPrecRound); /* RND CC P R */
                      break;
                    default:
                      inputUnGet (c);
                      C_FINISH_SYMBOL (c_rn); /* RN */
                      break;
                  }
              }
              break;
            case 'O':
              C_FINISH_FLOW ("UT", c_rout); /* ROUT */
              break;
            case 'P':
              M_FINISH_CHR ('W', m_rpw, optionCondPrecRound); /* RPW CC P R */
              break;
            case 'S':
              switch (inputGet ())
                {
                  case 'B':
                    M_FINISH (m_rsb, optionCondS); /* RSB CC s */
                    break;
                  case 'C':
                    M_FINISH (m_rsc, optionCondS); /* RSC CC s */
                    break;
                  case 'F':
                    M_FINISH (m_rsf, optionCondPrecRound); /* RSF CC P R */
                    break;
                  default:
                    goto illegal;
                }
              break;
            default:
              goto illegal;
          }
        break;

      case 'S':
        switch (inputGet ())
          {
            case 'B':
              M_FINISH_CHR ('C', m_sbc, optionCondS); /* SBC CC s */
              break;
            case 'E': /* SETA, SETL, SETS */
              if (inputGet () != 'T')
                goto illegal;
              C_FINISH_VAR (c_set);
              break;
            case 'F':
              M_FINISH_LFM (m_sfm); /* SFM CC (TYPE) */
              break;
            case 'I':
              M_FINISH_CHR ('N', m_sin, optionCondPrecRound); /* SIN CC P R */
              break;
            case 'M':
              switch (inputGet ())
                {
                  case 'U': /* SMU... */
                    if (inputGet () != 'L')
                      goto illegal;
                    /* SMUL... */
                    switch (inputGet ())
                      {
                        case 'L':
                          M_FINISH (m_smull, optionCondS);  /* SMULL CC */
                          break;
                        case 'B':
                          switch (inputGet ())
                            {
                              case 'B':
                                M_FINISH (m_smulbb, optionCond); /* SMULBB CC */
                                break;
                              case 'T':
                                M_FINISH (m_smulbt, optionCond); /* SMULBT CC */
                                break;
                              default:
                                goto illegal;
                            }
                          break;
                        case 'T':
                          switch (inputGet ())
                            {
                              case 'B':
                                M_FINISH (m_smultb, optionCond); /* SMULTB CC */
                                break;
                              case 'T':
                                M_FINISH (m_smultt, optionCond); /* SMULTT CC */
                                break;
                              default:
                                goto illegal;
                            }
                          break;
                        case 'W':
                          switch (inputGet ())
                            {
                              case 'B':
                                M_FINISH (m_smulwb, optionCond); /* SMULWB CC */
                                break;
                              case 'T':
                                M_FINISH (m_smulwt, optionCond); /* SMULWT CC */
                                break;
                              default:
                                goto illegal;
                            }
                          break;
                        default:
                          goto illegal;
                      }
                    break;
                  case 'L': /* SML... */
                    if (inputGet () != 'A')
                      goto illegal;
                    switch (inputGet ())
                      {
                        case 'L': /* SMLAL... */
                          {
                            int c;
                            switch ((c = inputGet ()))
                              {
                                case 'B':
                                  switch (inputGet ())
                                    {
                                      case 'B':
                                        M_FINISH (m_smlalbb, optionCond); /* SMLALBB CC */
                                        break;
                                      case 'T':
                                        M_FINISH (m_smlalbt, optionCond); /* SMLALBT CC */
                                        break;
                                      default:
                                        goto illegal;
                                    }
                                  break;
                                case 'T':
                                  switch (inputGet ())
                                    {
                                      case 'B':
                                        M_FINISH (m_smlaltb, optionCond); /* SMLALTB CC */
                                        break;
                                      case 'T':
                                        M_FINISH (m_smlaltt, optionCond); /* SMLALTT CC */
                                        break;
                                      default:
                                        goto illegal;
                                    }
                                  break;
                                default:
                                  inputUnGet (c);
                                  M_FINISH (m_smlal, optionCondS); /* SMLAL CC S */
                                  break;
                              }
                          }
                          break;
                        case 'B': /* SMLAB... */
                          switch (inputGet ())
                            {
                              case 'B':
                                M_FINISH (m_smlabb, optionCond); /* SMLABB CC */
                                break;
                              case 'T':
                                M_FINISH (m_smlabt, optionCond); /* SMLABT CC */
                                break;
                              default:
                                goto illegal;
                            }
                          break;
                        case 'T': /* SMLAT... */
                          switch (inputGet ())
                            {
                              case 'B':
                                M_FINISH (m_smlatb, optionCond); /* SMLATB CC */
                                break;
                              case 'T':
                                M_FINISH (m_smlatt, optionCond); /* SMLATT CC */
                                break;
                              default:
                                goto illegal;
                            }
                          break;
                        case 'W': /* SMLAW... */
                          switch (inputGet ())
                            {
                              case 'B':
                                M_FINISH (m_smlawb, optionCond); /* SMLAWB CC */
                                break;
                              case 'T':
                                M_FINISH (m_smlawt, optionCond); /* SMLAWT CC */
                                break;
                              default:
                                goto illegal;
                            }
                          break;
                        default:
                          goto illegal;
                      }
                    break;
                }
              break;
            case 'Q':
              M_FINISH_CHR ('T', m_sqt, optionCondPrecRound); /* SQT CC P R */
              break;
            case 'T':
              switch (inputGet ())
                {
                  case 'A':
                    C_FINISH_STR ("CK", m_stack); /* STACK CC */
                    break;
                  case 'C':
                    {
                      int c;
                      switch ((c = inputGet ()))
                        {
                          case '2':
                            M_FINISH (m_stc2, optionCondL); /* STC2 l */
                            break;
                          default:
                            inputUnGet (c);
                            M_FINISH (m_stc, optionCondL); /* STC CC l */
                            break;
                        }
                    }
                    break;
                  case 'F':
                    M_FINISH (m_stf, optionCondPrec_P); /* STF CC P */
                    break;
                  case 'M':
                    M_FINISH (m_stm, optionCondDirStm); /* STM CC TYPE */
                    break;
                  case 'R':
                    {
                      int c;
                      switch (c = inputGet ())
                        {
                          case 'O':
                            C_FINISH_STR ("NG", c_strong); /* STRONG */
                            break;
                          default:
                            inputUnGet (c);
                            M_FINISH (m_str, optionCondBT); /* STR CC BYTE */
                            break;
                        }
                    }
                    break;
                  default:
                    goto illegal;
                }
              break;
            case 'U': /* SU... */
              switch (inputGet ())
                {
                  case 'B':
                    {
                      int c;
                      switch (c = inputGet ())
                        {
                          case 'T':
                            C_FINISH (c_title); /* SUBT */
                            break;
                          default:
                            inputUnGet (c);
                            M_FINISH (m_sub, optionCondS); /* SUB CC s */
                            break;
                        }
                    }
                    break;
                  case 'F':
                    M_FINISH (m_suf, optionCondPrecRound); /* SUF CC P R */
                    break;
                  default:
                    goto illegal;
                }
              break;
            case 'W': /* SW... */
              switch (inputGet ())
                {
                  case 'I':
                    M_FINISH (m_swi, optionCond); /* SWI CC */
                    break;
                  case 'P':
                    M_FINISH (m_swp, optionCondB); /* SWP CC BYTE */
                    break;
                  default:
                    goto illegal;
                }
              break;
            default:
              goto illegal;
          }
        break;

      case 'T':
        switch (inputGet ())
          {
            case 'A':
              switch (inputGet ())
                {
                  case 'I':
                    M_FINISH_CHR ('L', m_tail, optionCond); /* TAIL CC */
                    break;
                  case 'N':
                    M_FINISH (m_tan, optionCondPrecRound); /* TAN CC P R */
                    break;
                  default:
                    goto illegal;
                }
              break;
            case 'E':
              M_FINISH_CHR ('Q', m_teq, optionCondSP); /* TEQ CC sp */
              break;
            case 'S':
              M_FINISH_CHR ('T', m_tst, optionCondSP); /* TST CC sp */
              break;
            case 'T':
              C_FINISH_CHR ('L', c_title); /* TTL */
              break;
            default:
              goto illegal;
          }
        break;

      case 'U':
        switch (inputGet ())
          {
            case 'M':
              switch (inputGet ())
                {
                  case 'U':
                    M_FINISH_STR ("LL", m_umull, optionCondS); /* UMULL CC */
                    break;
                  case 'L':
                    M_FINISH_STR ("AL", m_umlal, optionCondS); /* UMLAL CC */
                    break;
                  default:
                    goto illegal;
                }
              break;
            case 'R':
              M_FINISH_CHR ('D', m_urd, optionCondPrecRound); /* URD CC P R */
              break;
            default:
              goto illegal;
          }
        break;

      case 'W':
        switch (inputGet ())
          {
            case 'E':
              C_FINISH_FLOW ("ND", c_wend); /* WEND */
              break;
            case 'F':
              switch (inputGet ())
                {
                  case 'C':
                    M_FINISH (m_wfc, optionCond); /* WFC CC */
                    break;
                  case 'S':
                    M_FINISH (m_wfs, optionCond); /* WFS CC */
                    break;
                  default:
                    goto illegal;
                }
              break;
            case 'H':
              C_FINISH_FLOW ("ILE", c_while); /* WHILE */
              break;
            default:
              goto illegal;
          }
        break;

      case '[':
	C_FINISH_FLOW ("", c_if); /* '[' */
        break;

      case '|':
	C_FINISH_FLOW ("", c_else); /* '|' */
        break;

      case ']':
	C_FINISH_FLOW ("", c_endif); /* ']' */
        break;

      default:
illegal:
        {
	  /* Mnemonic is not recognized, maybe it is a macro.  */
          Input_RollBackToMark (inputMark);
          size_t l;
          const char *ci;
          if (inputLook () == '|') /* FIXME: use lexGetId ! */
            {
              inputSkip ();
              ci = inputSymbol (&l, '|');
              if (inputGet () != '|')
                error (ErrorError, "Identifier continues over newline");
            }
          else
            ci = inputSymbol (&l, '\0');
          const Macro *m = macroFind (l, ci);
          if (m)
            macroCall (m, label);
          else
            {
              Input_RollBackToMark (inputMark);
              errorAbort ("Illegal line \"%s\"", inputRest ());
            }
        }
        break;
    }

  /* Determine the code size associated with the label on this line (if any).  */
  if (labelSymbol != NULL)
    {
      assert (labelSymbol->codeSize == 0);
      const Value currentStorage = storageValue ();
      const int currentStorageOffset = currentStorage.Tag == ValueInt ? currentStorage.Data.Int.i : currentStorage.Data.Addr.i;
      /* Either we have an increase in code/data in our current area, either
         we have an increase in storage map, either non of the previous (like
	 with "<lbl> * <value>" input).  */
      assert (areaCurrentSymbol->value.Data.Int.i - startOffset == 0 || currentStorageOffset - startStorageOffset == 0);
      labelSymbol->codeSize = areaCurrentSymbol->value.Data.Int.i - startOffset
				+ currentStorageOffset - startStorageOffset;
    }
  
  skipblanks ();
  if (!Input_IsEolOrCommentStart ())
    errorAbort ("Skipping extra characters '%s'", inputRest ());
}
