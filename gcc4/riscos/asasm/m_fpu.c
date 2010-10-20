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
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "area.h"
#include "code.h"
#include "error.h"
#include "expr.h"
#include "fix.h"
#include "get.h"
#include "input.h"
#include "mnemonics.h"
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
  if (inputLook () == '#')
    {
      inputSkip ();
      exprBuild ();
      ir |= 8;			/* Immediate Float */
      Value im = exprEval (ValueInt | ValueFloat | ValueLateLabel | ValueCode);
      switch (im.Tag)
	{
	case ValueInt:
	  ir = fixImmFloat (0, ir, im.ValueInt.i);
	  break;
	case ValueFloat:
	  ir = fixImmFloat (0, ir, im.ValueFloat.f);
	  break;
	case ValueLateLabel:
	case ValueCode:
	  relocImmFloat (ir, &im);
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
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, "%sdst", InsertCommaAfter);
  op = getFpuReg ();
  ir |= LHS_OP (op);
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, "%slhs", InsertCommaAfter);
  putIns (getFloatRhs (ir));
}


void
m_adf (ARMWord cc)
{
  dstlhsrhs (M_ADF | cc);
}

void
m_dvf (ARMWord cc)
{
  dstlhsrhs (M_DVF | cc);
}

void
m_fdv (ARMWord cc)
{
  dstlhsrhs (M_FDV | cc);
}

void
m_fml (ARMWord cc)
{
  dstlhsrhs (M_FML | cc);
}

void
m_frd (ARMWord cc)
{
  dstlhsrhs (M_FRD | cc);
}

void
m_muf (ARMWord cc)
{
  dstlhsrhs (M_MUF | cc);
}

void
m_pol (ARMWord cc)
{
  dstlhsrhs (M_POL | cc);
}

void
m_pow (ARMWord cc)
{
  dstlhsrhs (M_POW | cc);
}

void
m_rdf (ARMWord cc)
{
  dstlhsrhs (M_RDF | cc);
}

void
m_rmf (ARMWord cc)
{
  dstlhsrhs (M_RMF | cc);
}

void
m_rpw (ARMWord cc)
{
  dstlhsrhs (M_RPW | cc);
}

void
m_rsf (ARMWord cc)
{
  dstlhsrhs (M_RSF | cc);
}

void
m_suf (ARMWord cc)
{
  dstlhsrhs (M_SUF | cc);
}


/** DATA monadic **/

static void
dstrhs (ARMWord ir)
{
  ARMWord op;

  if (option_apcs_softfloat)
    error (ErrorWarning, "soft-float code uses hard FP instructions");

  op = getFpuReg ();
  ir |= DST_OP (op);
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, "%sdst", InsertCommaAfter);
  putIns (getFloatRhs (ir));
}

void
m_abs (ARMWord cc)
{
  dstrhs (M_ABS | cc);
}

void
m_acs (ARMWord cc)
{
  dstrhs (M_ACS | cc);
}

void
m_asn (ARMWord cc)
{
  dstrhs (M_ASN | cc);
}

void
m_atn (ARMWord cc)
{
  dstrhs (M_ATN | cc);
}

void
m_cos (ARMWord cc)
{
  dstrhs (M_COS | cc);
}

void
m_exp (ARMWord cc)
{
  dstrhs (M_EXP | cc);
}

void
m_lgn (ARMWord cc)
{
  dstrhs (M_LGN | cc);
}

void
m_log (ARMWord cc)
{
  dstrhs (M_LOG | cc);
}

void
m_mnf (ARMWord cc)
{
  dstrhs (M_MNF | cc);
}

void
m_mvf (ARMWord cc)
{
  dstrhs (M_MVF | cc);
}

void
m_rnd (ARMWord cc)
{
  dstrhs (M_RND | cc);
}

void
m_sin (ARMWord cc)
{
  dstrhs (M_SIN | cc);
}

void
m_sqt (ARMWord cc)
{
  dstrhs (M_SQT | cc);
}

void
m_tan (ARMWord cc)
{
  dstrhs (M_TAN | cc);
}

void
m_urd (ARMWord cc)
{
  dstrhs (M_URD | cc);
}

void
m_nrm (ARMWord cc)
{
  dstrhs (M_NRM | cc);
}

/** REGISTER TRANSFER **/

void
m_fix (ARMWord cc)
{
  if (option_apcs_softfloat)
    error (ErrorWarning, "soft-float code uses hard FP instructions");

  ARMWord ir = M_FIX | cc;
  ir |= DST_OP (getCpuReg ());
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, "%sdst", InsertCommaAfter);
  putIns (getFloatRhs (ir));
}

void
m_flt (ARMWord cc)
{
  if (option_apcs_softfloat)
    error (ErrorWarning, "soft-float code uses hard FP instructions");

  ARMWord ir = M_FLT | cc;
  ir |= LHS_OP (getFpuReg ());
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, "%sdst", InsertCommaAfter);
  ir |= DST_OP (getCpuReg ());
  putIns (ir);
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

void
m_wfs (ARMWord cc)
{
  flagtransfer (M_WFS | cc);
}

void
m_rfs (ARMWord cc)
{
  flagtransfer (M_RFS | cc);
}

void
m_wfc (ARMWord cc)
{
  flagtransfer (M_WFC | cc);
}

void
m_rfc (ARMWord cc)
{
  flagtransfer (M_RFC | cc);
}

static void
comparelow (ARMWord ir)		/* No precision and no rounding allowed ? */
{
  if (option_apcs_softfloat)
    error (ErrorWarning, "soft-float code uses hard FP instructions");

  ir |= LHS_OP (getFpuReg ());
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, "%slhs", InsertCommaAfter);
  putIns (getFloatRhs (ir));
}

void
m_cmf (ARMWord cc)
{
  comparelow (M_CMF | cc);
}

void
m_cnf (ARMWord cc)
{
  comparelow (M_CNF | cc);
}
