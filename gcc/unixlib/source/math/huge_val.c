/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/math/huge_val.c,v $
 * $Date: 2000/07/15 14:52:21 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#include <machine/endian.h>
#include <machine/huge_val.h>
#include <machine/nan.h>

/* IEEE positive infinity (-HUGE_VAL is negative infinity).  */

#define __HUGE_VAL_bytes { 0, 0, 0xf0, 0x7f, 0, 0, 0, 0 }
#define __HUGE_VALf_bytes { 0, 0, 0x80, 0x7f }

__huge_val_t __huge_val = { __HUGE_VAL_bytes };
__huge_valf_t __huge_valf = { __HUGE_VALf_bytes };

#if 0
#define __HUGE_VALl_bytes { 0, 0, 0x7f, 0xff, 0, 0, 0, 0, 0, 0, 0, 0 }
__huge_vall_t __huge_vall = { __HUGE_VALl_bytes };
#endif

#define __nan_bytes { 0, 0, 0xf8, 0x7f, 0, 0, 0, 0 }

const char __nan[8] = __nan_bytes;
