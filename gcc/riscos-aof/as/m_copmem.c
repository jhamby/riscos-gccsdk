/*
 * m_copmem.c
 * Copyright © 1992 Niklas Röjemo
 */

#include "sdk-config.h"
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "area.h"
#include "error.h"
#include "expr.h"
#include "get.h"
#include "help_cop.h"
#include "input.h"
#include "mnemonics.h"
#include "option.h"
#include "put.h"
#include "reloc.h"

/** MEMMORY **/


static void
dstmem (WORD ir)
{
  ir |= CP_NUMBER (help_copInt (15, "coprocessor number"));
  skipblanks ();
  if (inputLook () == ',')
    {
      inputSkip ();
      skipblanks ();
    }
  else
    error (ErrorError, TRUE, "%scoprocessor number", InsertCommaAfter);
  ir |= CPDST_OP (getCopReg ());
  ir = help_copAddr (ir, 0);
  putIns (ir);
}

void
m_ldc (WORD cc)
{
  dstmem (cc | 0x0c100000);
}

void
m_stc (WORD cc)
{
  dstmem (cc | 0x0c000000);
}
