
/*
 * m_copmem.c
 * Copyright © 1992 Niklas Röjemo
 */

#include <stdint.h>
#include "mnemonics.h"
#include "error.h"
#include "option.h"
#include "input.h"
#include "expr.h"
#include "reloc.h"
#include "put.h"
#include "area.h"
#include "get.h"
#include "help_cop.h"

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
