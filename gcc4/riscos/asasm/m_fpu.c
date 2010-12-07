/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2000-2010 GCCSDK Developersrs
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
 * m_fpu.c
 */

#include "config.h"
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#elif HAVE_INTTYPES_H
#  include <inttypes.h>
#endif

#include "area.h"
#include "code.h"
#include "error.h"
#include "expr.h"
#include "fix.h"
#include "get.h"
#include "input.h"
#include "m_fpu.h"
#include "option.h"
#include "put.h"
#include "reloc.h"
#include "value.h"
#include "main.h"

ARMWord
fpuImm (ARMFloat d)
{
  if (d == 0.0)
    return 0;
  if (d == 1.0)
    return 1;
  if (d == 2.0)
    return 2;
  if (d == 3.0)
    return 3;
  if (d == 4.0)
    return 4;
  if (d == 5.0)
    return 5;
  if (d == 0.5)
    return 6;
  if (d == 10.0)
    return 7;

  return -1;
}

static ARMWord
getFloatRhs (ARMWord ir)
{
  if (Input_Match ('#', false))
    {
      ir |= 8; /* Immediate float.  */
      const Value *im = exprBuildAndEval (ValueInt | ValueFloat);
      switch (im->Tag)
	{
	  case ValueInt:
	    ir = fixImmFloat (0, ir, im->Data.Int.i);
	    break;
	  case ValueFloat:
	    ir = fixImmFloat (0, ir, im->Data.Float.f);
	    break;
	  default:
	    error (ErrorError, "Illegal float immediate");
	    break;
	}
    }
  else
    ir |= getFpuReg ();

  return ir;
}

/** DATA dyadic **/

static void
dstlhsrhs (ARMWord ir)
{
  if (option_apcs_softfloat)
    error (ErrorWarning, "soft-float code uses hard FP instructions");

  ARMWord op = getFpuReg ();
  ir |= DST_OP (op);
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%sdst", InsertCommaAfter);
  op = getFpuReg ();
  ir |= LHS_OP (op);
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%slhs", InsertCommaAfter);
  putIns (getFloatRhs (ir));
}


/**
 * Implements ADF.
 */
bool
m_adf (void)
{
  ARMWord cc = optionCondPrecRound ();
  if (cc == optionError)
    return true;
  dstlhsrhs (M_ADF | cc);
  return false;
}

/**
 * Implements DVF.
 */
bool
m_dvf (void)
{
  ARMWord cc = optionCondPrecRound ();
  if (cc == optionError)
    return true;
  dstlhsrhs (M_DVF | cc);
  return false;
}

/**
 * Implements FDV.
 */
bool
m_fdv (void)
{
  ARMWord cc = optionCondPrecRound ();
  if (cc == optionError)
    return true;
  dstlhsrhs (M_FDV | cc);
  return false;
}

/**
 * Implements FML.
 */
bool
m_fml (void)
{
  ARMWord cc = optionCondPrecRound ();
  if (cc == optionError)
    return true;
  dstlhsrhs (M_FML | cc);
  return false;
}

/**
 * Implements FRD.
 */
bool
m_frd (void)
{
  ARMWord cc = optionCondPrecRound ();
  if (cc == optionError)
    return true;
  dstlhsrhs (M_FRD | cc);
  return false;
}

/**
 * Implements MUF.
 */
bool
m_muf (void)
{
  ARMWord cc = optionCondPrecRound ();
  if (cc == optionError)
    return true;
  dstlhsrhs (M_MUF | cc);
  return false;
}

/**
 * Implements POL.
 */
bool
m_pol (void)
{
  ARMWord cc = optionCondPrecRound ();
  if (cc == optionError)
    return true;
  dstlhsrhs (M_POL | cc);
  return false;
}

/**
 * Implements POW.
 */
bool
m_pow (void)
{
  ARMWord cc = optionCondPrecRound ();
  if (cc == optionError)
    return true;
  dstlhsrhs (M_POW | cc);
  return false;
}

/**
 * Implements RDF.
 */
bool
m_rdf (void)
{
  ARMWord cc = optionCondPrecRound ();
  if (cc == optionError)
    return true;
  dstlhsrhs (M_RDF | cc);
  return false;
}

/**
 * Implements RMF.
 */
bool
m_rmf (void)
{
  ARMWord cc = optionCondPrecRound ();
  if (cc == optionError)
    return true;
  dstlhsrhs (M_RMF | cc);
  return false;
}

/**
 * Implements RPW.
 */
bool
m_rpw (void)
{
  ARMWord cc = optionCondPrecRound ();
  if (cc == optionError)
    return true;
  dstlhsrhs (M_RPW | cc);
  return false;
}

/**
 * Implements RSF.
 */
bool
m_rsf (void)
{
  ARMWord cc = optionCondPrecRound ();
  if (cc == optionError)
    return true;
  dstlhsrhs (M_RSF | cc);
  return false;
}

/**
 * Implements SUF.
 */
bool
m_suf (void)
{
  ARMWord cc = optionCondPrecRound ();
  if (cc == optionError)
    return true;
  dstlhsrhs (M_SUF | cc);
  return false;
}


/** DATA monadic **/

static void
dstrhs (ARMWord ir)
{
  if (option_apcs_softfloat)
    error (ErrorWarning, "soft-float code uses hard FP instructions");

  ARMWord op = getFpuReg ();
  ir |= DST_OP (op);
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%sdst", InsertCommaAfter);
  putIns (getFloatRhs (ir));
}

/**
 * Implements ABS.
 */
bool
m_abs (void)
{
  ARMWord cc = optionCondPrecRound ();
  if (cc == optionError)
    return true;
  dstrhs (M_ABS | cc);
  return false;
}

/**
 * Implements ACS.
 */
bool
m_acs (void)
{
  ARMWord cc = optionCondPrecRound ();
  if (cc == optionError)
    return true;
  dstrhs (M_ACS | cc);
  return false;
}

/**
 * Implements ASN.
 */
bool
m_asn (void)
{
  ARMWord cc = optionCondPrecRound ();
  if (cc == optionError)
    return true;
  dstrhs (M_ASN | cc);
  return false;
}

/**
 * Implements ATN.
 */
bool
m_atn (void)
{
  ARMWord cc = optionCondPrecRound ();
  if (cc == optionError)
    return true;
  dstrhs (M_ATN | cc);
  return false;
}

/**
 * Implements COS.
 */
bool
m_cos (void)
{
  ARMWord cc = optionCondPrecRound ();
  if (cc == optionError)
    return true;
  dstrhs (M_COS | cc);
  return false;
}

/**
 * Implements EXP.
 */
bool
m_exp (void)
{
  ARMWord cc = optionCondPrecRound ();
  if (cc == optionError)
    return true;
  dstrhs (M_EXP | cc);
  return false;
}

/**
 * Implements LGN.
 */
bool
m_lgn (void)
{
  ARMWord cc = optionCondPrecRound ();
  if (cc == optionError)
    return true;
  dstrhs (M_LGN | cc);
  return false;
}

/**
 * Implements LOG.
 */
bool
m_log (void)
{
  ARMWord cc = optionCondPrecRound ();
  if (cc == optionError)
    return true;
  dstrhs (M_LOG | cc);
  return false;
}

/**
 * Implements MNF.
 */
bool
m_mnf (void)
{
  ARMWord cc = optionCondPrecRound ();
  if (cc == optionError)
    return true;
  dstrhs (M_MNF | cc);
  return false;
}

/**
 * Implements MVF.
 */
bool
m_mvf (void)
{
  ARMWord cc = optionCondPrecRound ();
  if (cc == optionError)
    return true;
  dstrhs (M_MVF | cc);
  return false;
}

/**
 * Implements RND.
 */
bool
m_rnd (void)
{
  ARMWord cc = optionCondPrecRound ();
  if (cc == optionError)
    return true;
  dstrhs (M_RND | cc);
  return false;
}

/**
 * Implements SIN.
 */
bool
m_sin (void)
{
  ARMWord cc = optionCondPrecRound ();
  if (cc == optionError)
    return true;
  dstrhs (M_SIN | cc);
  return false;
}

/**
 * Implements SQT.
 */
bool
m_sqt (void)
{
  ARMWord cc = optionCondPrecRound ();
  if (cc == optionError)
    return true;
  dstrhs (M_SQT | cc);
  return false;
}

/**
 * Implements TAN.
 */
bool
m_tan (void)
{
  ARMWord cc = optionCondPrecRound ();
  if (cc == optionError)
    return true;
  dstrhs (M_TAN | cc);
  return false;
}

/**
 * Implements URD.
 */
bool
m_urd (void)
{
  ARMWord cc = optionCondPrecRound ();
  if (cc == optionError)
    return true;
  dstrhs (M_URD | cc);
  return false;
}

/**
 * Implements NRM.
 */
bool
m_nrm (void)
{
  ARMWord cc = optionCondPrecRound ();
  if (cc == optionError)
    return true;
  dstrhs (M_NRM | cc);
  return false;
}

static void
comparelow (ARMWord ir)		/* No precision and no rounding allowed ? */
{
  if (option_apcs_softfloat)
    error (ErrorWarning, "soft-float code uses hard FP instructions");

  ir |= LHS_OP (getFpuReg ());
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%slhs", InsertCommaAfter);
  putIns (getFloatRhs (ir));
}

/**
 * Implements CMF.
 */
bool
m_cmf (void)
{
  ARMWord cc = optionExceptionCond ();
  if (cc == optionError)
    return true;
  comparelow (M_CMF | cc);
  return false;
}

/**
 * Implements CNF.
 */
bool
m_cnf (void)
{
  ARMWord cc = optionExceptionCond ();
  if (cc == optionError)
    return true;
  comparelow (M_CNF | cc);
  return false;
}

/** REGISTER TRANSFER **/

/**
 * Implements FIX.
 */
bool
m_fix (void)
{
  ARMWord cc = optionCondOptRound ();
  if (cc == optionError)
    return true;

  if (option_apcs_softfloat)
    error (ErrorWarning, "soft-float code uses hard FP instructions");

  ARMWord ir = M_FIX | cc;
  ir |= DST_OP (getCpuReg ());
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%sdst", InsertCommaAfter);
  putIns (getFloatRhs (ir));
  return false;
}

/**
 * Implements FLT.
 */
bool
m_flt (void)
{
  ARMWord cc = optionCondPrecRound ();
  if (cc == optionError)
    return true;

  if (option_apcs_softfloat)
    error (ErrorWarning, "soft-float code uses hard FP instructions");

  ARMWord ir = M_FLT | cc;
  ir |= LHS_OP (getFpuReg ());
  skipblanks ();
  if (!Input_Match (',', true))
    error (ErrorError, "%sdst", InsertCommaAfter);
  ir |= DST_OP (getCpuReg ());
  putIns (ir);
  return false;
}

static void
flagtransfer (ARMWord ir)
{
  if (option_apcs_softfloat)
    error (ErrorWarning, "soft-float code uses hard FP instructions");

  ARMWord op = getCpuReg ();
  ir |= DST_OP (op);
  putIns (ir);
}

/**
 * Implements WFS.
 */
bool
m_wfs (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;
  flagtransfer (M_WFS | cc);
  return false;
}

/**
 * Implements RFS.
 */
bool
m_rfs (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;
  flagtransfer (M_RFS | cc);
  return false;
}

/**
 * Implements WFC.
 */
bool
m_wfc (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;
  flagtransfer (M_WFC | cc);
  return false;
}

/**
 * Implements RFC.
 */
bool
m_rfc (void)
{
  ARMWord cc = optionCond ();
  if (cc == optionError)
    return true;
  flagtransfer (M_RFC | cc);
  return false;
}
