/*
 * AsAsm an assembler for ARM
 * Copyright (c) 2012-2013 GCCSDK Developers
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */

#ifndef fpu_header_included
#define fpu_header_included

#include <stdint.h>

/* float ARM/FPA or ARM/VFP layout.  */
union arm_float
{
  uint32_t i;
  struct
    {
      unsigned int mantissa:23;
      unsigned int exponent:8;
      unsigned int negative:1;
    } flt;
};

/* double ARM/FPA layout.  */
union arm_double_fpa
{
  uint64_t i;
  struct
    {
      unsigned int mantissa0:20;
      unsigned int exponent:11;
      unsigned int negative:1;
      unsigned int mantissa1:32;
    } dbl;
};

/* double ARM/VFP layout.  */
union arm_double_vfp
{
  uint64_t i;
  struct
    {
      unsigned int mantissa1:32;
      unsigned int mantissa0:20;
      unsigned int exponent:11;
      unsigned int negative:1;
    } dbl;
};

#endif
