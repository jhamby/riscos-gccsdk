/* setop.h -- public declarations for setop.c
   Copyright (C) 1997-2000 Free Software Foundation, Inc.

This file is part of GNU Pascal.

GNU Pascal is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU Pascal is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Pascal; see the file COPYING. If not, write to the
Free Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.

   Owning Modules:
      setop.c

   Modifications:
*/

/* Allow multiple inclusion to work. */

#ifndef _GPC_SETOP_H_
#define _GPC_SETOP_H_

/* Simple definitions and enumerations. */

#define CHECK_INSIDE  0
#define CLEAR_INSIDE  1
#define SET_INSIDE    2
#define CHECK_OUTSIDE 3
#define CLEAR_OUTSIDE 4
#define SET_OUTSIDE   5


/* Typedefs. */


/* Include files needed by this one. */


/* Structure definitions. */


/* Global objects accessed by users of this module. */

rtx expand_set_card PROTO((tree, rtx));
rtx expand_set_binop PROTO((tree, rtx));
rtx expand_set_comparison PROTO((tree, rtx));
rtx expand_set_in PROTO((tree, rtx));
void store_set_constructor PROTO((tree, rtx));

/* Define macros. */

/* Advance pointer to next word. */
#define NEXT_WORD(ptr)  expand_inc (ptr, GEN_INT (UNITS_PER_WORD))

#define WORD_IN_BITS  size_int (BITS_PER_WORD)
#define WORD_IN_UNITS size_int (UNITS_PER_WORD)
#define BITS_IN_UNIT  size_int (BITS_PER_UNIT)

#define CONV_INT(a)   convert (integer_type_node, a)

/* Get a mode from a or b trying to avoid VOIDmode */
#define GET_MODE_2(a, b) (GET_MODE(a) != VOIDmode ? GET_MODE(a) : GET_MODE(b))

/* End of #include file. */

#endif /* _GPC_SETOP_H_ */
