/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/clib/machine/h/huge_val,v $
 * $Date: 1999/11/16 13:31:17 $
 * $Revision: 1.4 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

/* `HUGE_VAL' constant for IEEE 754. */

#ifndef __MACHINE_HUGE_VAL_H
#define __MACHINE_HUGE_VAL_H 1

/* IEEE positive infinity (-HUGE_VAL is negative infinity).  */

typedef union
{
  unsigned char __c[8];
  double __d;
} __huge_val_t;

extern __huge_val_t __huge_val;
#define HUGE_VAL (__huge_val.__d)


/* GNU extensions: (float) HUGE_VALF and (long double) HUGE_VALL.  */
typedef union
{
  unsigned char __c[4];
  float __f;
} __huge_valf_t;

extern __huge_valf_t __huge_valf;
#define HUGE_VALF (__huge_valf.__f)

typedef union
{
  unsigned char __c[12];
  long double __ld;
}__huge_vall_t;

extern __huge_vall_t __huge_vall;
#define HUGE_VALL (__huge_vall.__ld)

#endif
