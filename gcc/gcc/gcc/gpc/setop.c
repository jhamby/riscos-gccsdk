/* SET_TYPE operations
   Copyright (C) 1992, 1993, 1998-2000 Free Software Foundation, Inc.
   Contributed by Jukka Virtanen <jtv@hut.fi>

This file is part of GNU Pascal.

GNU Pascal is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 1, or (at your option)
any later version.

GNU Pascal is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Pascal; see the file COPYING. If not, write to the
Free Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA. */

#include "gcc-version.h"
#include "config.h"
#include "gansidecl.h"
#ifdef EGCS
#include "system.h"
#endif

#include <stdio.h>
#include "gbe.h"
#include "obstack.h"
#include "insn-flags.h"
#include "setop.h"

/* Declare functions with prototypes. */

rtx expand_set_card PROTO((tree, rtx));
rtx expand_set_binop PROTO((tree, rtx));
rtx expand_set_comparison PROTO((tree, rtx));
rtx expand_set_in PROTO((tree, rtx));
void store_set_constructor PROTO((tree, rtx));

/* Local utility functions */

rtx expand_set_card PROTO((tree, rtx));
rtx expand_set_binop PROTO((tree, rtx));
rtx expand_set_comparison PROTO((tree, rtx));
rtx expand_set_in PROTO((tree, rtx));
void store_set_constructor PROTO((tree, rtx));

/* Local utility functions */

static int fixup_set_bounds PROTO((enum tree_code, tree, tree, tree*, tree*));
static tree long_int PROTO((long));
static tree setop_rlo PROTO((tree));
/* (unused) static tree setop_size PROTO((tree, tree)); */
static tree setop_word PROTO((tree, tree));
static tree setop_prev_word PROTO((tree, tree));
static tree setop_next_word PROTO((tree, tree));
static tree setop_bit PROTO((tree));
static tree setop_num_words PROTO((tree, tree));
static tree setop_vector_adjust PROTO((tree, tree));
static tree setop_num_bytes PROTO((tree, tree));
static int setop_empty PROTO((rtx, tree, tree));
static rtx setop_runtime PROTO((int, rtx, tree, tree, tree, tree, rtx, int));
static void setop_copy PROTO((rtx, tree, tree, rtx, tree, tree));
static rtx setop_card_one_word PROTO((rtx, rtx, int));

/* Note: All sets are word aligned.
 *       These routines do not support variable set bounds.
 *       (Not part of ISO-7185 Pascal)
 */

/* Sometimes when operands are constructors we do not know what
 * is the correct size of the constructed set. In this case I try
 * to allocate as small set as possible (depends on the operation)
 *
 * Find out LO and HI set bounds of the result based on the operand types
 * Returns 0 if TYPE0 was selected; 1 if TYPE1 was selected; 2 if combined.
 */

static int
fixup_set_bounds (code, type0, type1, lo, hi)
     enum tree_code code;
     tree type0;
     tree type1;
     tree *lo;
     tree *hi;
{
  tree result_lo, result_hi;

  int ret = 0;
  tree dom0 = TYPE_DOMAIN (type0);
  tree dom1 = TYPE_DOMAIN (type1);

  result_lo = TYPE_MIN_VALUE (dom0);
  result_hi = TYPE_MAX_VALUE (dom0);

  if (dom0 != dom1)
    switch (code) {
    case BIT_XOR_EXPR:
    case BIT_IOR_EXPR:
      /* Make the set large enough to hold both */
      if (tree_int_cst_lt (TYPE_MIN_VALUE (dom0), TYPE_MIN_VALUE (dom1)))
        result_lo = TYPE_MIN_VALUE (dom0);
      else
        {
          result_lo = TYPE_MIN_VALUE (dom1);
          ret = 1;
        }

      if (tree_int_cst_lt (TYPE_MAX_VALUE (dom0), TYPE_MAX_VALUE (dom1)))
        {
          result_hi = TYPE_MAX_VALUE (dom1);
          ret = ret ? 1 : 2;
        }
      else
        {
          result_hi = TYPE_MAX_VALUE (dom0);
          ret = ret ? 2 : 0;
        }
      break;

    case BIT_ANDTC_EXPR:
      /* Choose first set */
      break;

    case BIT_AND_EXPR:
      /* Return the smaller domain values */

      if ((TREE_INT_CST_LOW (TYPE_MAX_VALUE (dom1)) - TREE_INT_CST_LOW (TYPE_MIN_VALUE (dom1)))
          < (TREE_INT_CST_LOW (TYPE_MAX_VALUE (dom0)) - TREE_INT_CST_LOW (TYPE_MIN_VALUE (dom0))))
        {
          result_lo = TYPE_MIN_VALUE (dom1);
          result_hi = TYPE_MAX_VALUE (dom1);
          ret = 1;
        }
      break;

    default:
      abort ();
    }

  *lo = result_lo;
  *hi = result_hi;
  return ret;
}

/*
 * Construct an integer value from an integer (possibly negative) that
 * we are able to shift left more than 32 bits in a 64 bit machine.
 *
 */
static tree
long_int (value)
     long value;
{
  register tree t = make_node (INTEGER_CST);
  TREE_INT_CST_LOW (t) = value;
  TREE_INT_CST_HIGH (t) = 0L;
  TREE_TYPE (t) = long_integer_type_node;
  return t;
}

/* I first did the setop_* routines at rtl level, but
 * rewrote them because:
 *  - it is nice to fold() set bound calculations.
 *  - expand_shift() takes the shift count as a tree node.
 */

/* @@ make the following stuff macros after they have been debugged */

/* Return the element number of the first bit of LO's word.
   This bit does not necessarily belong to the set, but sets are word
   aligned.
 */

/* @@ Added conversions to integer type.
   @@ verify them.
   Boolean bound calculations fail otherwise.
 */

static tree
setop_rlo (lo0)
     tree lo0;
{
  tree lo = CONV_INT (lo0);
  tree result = fold (build (MINUS_EXPR, integer_type_node,
                             lo, fold (build (TRUNC_MOD_EXPR, integer_type_node,
                                              lo, WORD_IN_BITS))));
  STRIP_NOPS (result);
  return result;
}

/* Return the size of the set (number of elements it can have) */

/* unused */
#if 0
static tree
setop_size (lo, hi)
     tree lo, hi;
{
  return fold (build (PLUS_EXPR, integer_type_node,
                      integer_one_node,
                      convert (integer_type_node,
                               fold (build (MINUS_EXPR, integer_type_node,
                                            CONV_INT (hi), CONV_INT (lo))))));
}
#endif /* 0 */

/* Return the byte offset of the word that contains the A'th element */

static tree
setop_word (a, lo)
     tree a, lo;
{
  return fold (build (MULT_EXPR, integer_type_node,
                      fold (build (TRUNC_DIV_EXPR, integer_type_node,
                                   fold (build (MINUS_EXPR, integer_type_node,
                                                CONV_INT (a), setop_rlo (lo))),
                                   WORD_IN_BITS)),
                      WORD_IN_UNITS));
}

/* Return the byte offset of the word before the word
 * that contains the A'th element
 */

static tree
setop_prev_word (a, lo)
     tree a, lo;
{
  return setop_word (fold (build (MINUS_EXPR, integer_type_node,
                                  CONV_INT (a), WORD_IN_BITS)),
                     CONV_INT (lo));
}

/* Return the byte offset of the next word after the word
 * that contains the A'th element
 */

static tree
setop_next_word (a, lo)
     tree a, lo;
{
  return setop_word (fold (build (PLUS_EXPR, integer_type_node,
                                  CONV_INT (a), WORD_IN_BITS)),
                     CONV_INT (lo));
}

/* Returns the bit offset from start of word to A'th element
 * (in range 0..(BITS_PER_WORD-1))
 */

static tree
setop_bit (a)
     tree a;
{
  return fold (build (TRUNC_MOD_EXPR, integer_type_node, CONV_INT (a), WORD_IN_BITS));
}

/* Return the number of words allocated for the set */

static tree
setop_num_words (lo, hi)
     tree lo, hi;
{
  return fold (build (TRUNC_DIV_EXPR, integer_type_node,
                      fold (build (PLUS_EXPR, integer_type_node,
                                   CONV_INT (hi),
                                   fold (build (MINUS_EXPR, integer_type_node,
                                                WORD_IN_BITS,
                                                setop_rlo (lo))))),
                      WORD_IN_BITS));
}

/* Return the number of bytes needeed to adjust set A so that it's
 * element numbers match set B's element numbers. (Set A may be e.g.
 * a set constructor [ '0'..'9' ] that takes one word of storage while
 * set B may be a 'set of char' that takes 4 words (in 32 bit machine)
 * so set A word 0 must be adjusted to set B word 1)
 */

static tree
setop_vector_adjust (lo_a, lo_b)
     tree lo_a, lo_b;
{
  return fold (build (MULT_EXPR, integer_type_node,
                      fold (build (TRUNC_DIV_EXPR, integer_type_node,
                                   fold (build (MINUS_EXPR, integer_type_node,
                                                CONV_INT (lo_b), CONV_INT (lo_a))),
                                   WORD_IN_BITS)),
                      WORD_IN_UNITS));
}

/* Returns the number of bytes allocated for the set */

static tree
setop_num_bytes (lo, hi)
     tree lo, hi;
{
  return fold (build (MULT_EXPR, integer_type_node,
                      setop_num_words (lo, hi),
                      WORD_IN_UNITS));
}

/* Return 1 if the set is empty or the empty set node.
 *
 * @@ does not check contents!!
 * This is a compile time check only.
 */

static int
setop_empty (set, lo, hi)
     rtx set;
     tree lo, hi;
{
  if (TREE_CODE (lo) == INTEGER_CST
      && TREE_CODE (hi) == INTEGER_CST
      && TREE_INT_CST_LOW (lo) == 0
      && TREE_INT_CST_LOW (hi) == -1)
    return 1;
  if (set)
    {
      /* Should perhaps check constant sets here */
    }
  return 0;
}

/* Some runtime operations for a set:
 * WHAT
 *    CHECK_INSIDE : Check if set is empty from LO..HI (RLO,RHI unused)
 *    CLEAR_INSIDE : Clear set elements between LO..HI (RLO,RHI unused)
 *    SET_INSIDE   : Set all bits between LO..HI       (RLO,RHI unused)
 *    CHECK_OUTSIDE: Check if set is empty between RLO..(LO-1) and (HI+1)..RHI
 *    CLEAR_OUTSIDE: Clear set elements between RLO..(LO-1) and (HI+1)..RHI
 *    SET_OUTSIDE  : Set all bits between RLO..(LO-1) and (HI+1)..RHI
 *
 * SET          is the rtx of the set.
 * RLOW & RHIGH are the bounds of the set.
 * LO   & HI    are the bounds of a subset of the set from RLO..RHI
 * RESULT       is the rtx where result of CHECK_* is always placed (else NULL_RTX)
 * INVERT       if 0 RESULT will be const1_rtx when bits EQ zero.
 *              if 1 RESULT will be const0_rtx when bits EQ zero.
 *
 */

static rtx
setop_runtime (what, set, rlow, rhigh, lo, hi, result, invert)
     int what;
     rtx set;
     tree rlow, rhigh, lo, hi;
     rtx result;
     int invert;
{
  /* int bounds_match = rlow == lo && rhigh == hi; */
  int one_bit      = lo == hi;
  int one_word = 0;

  /* whole set bounds (byte offset to start of word) */
  tree owl = setop_word (rlow, rlow);
  tree owh = setop_word (rhigh, rlow);

  rtx rtx_owl = expand_expr (owl, NULL_RTX, VOIDmode, 0);
  rtx rtx_owh = expand_expr (owh, NULL_RTX, VOIDmode, 0);

  /* given set range in rtl */
  rtx rtx_lo = expand_expr (lo, NULL_RTX, VOIDmode, 0);
  rtx rtx_hi = expand_expr (hi, NULL_RTX, VOIDmode, 0);

  /* bits we are interested in (byte offset to start of word) */
  tree nwl = setop_word (lo, rlow);
  tree nwh = setop_word (hi, rlow);

  rtx rtx_nwl = expand_expr (nwl, NULL_RTX, VOIDmode, 0);
  rtx rtx_nwh = expand_expr (nwh, NULL_RTX, VOIDmode, 0);

  /* Bit indices inside the word the bound is in */
  tree nbl = setop_bit (lo);
  tree nbh = setop_bit (hi);

  /* This label is used as generic label to get out of the code generated
     in this routine */
  rtx label = gen_label_rtx ();

  rtx mask0;
  rtx mask1;
  rtx mask2 = NULL_RTX;
  rtx target;
  rtx temp;
  optab binoptab = and_optab;

  if (! set)
    abort ();

  if (GET_CODE (rtx_nwl) == CONST_INT && GET_CODE (rtx_nwh) == CONST_INT)
    one_word = INTVAL (rtx_nwl) == INTVAL (rtx_nwh);

  target = set;

  /* If we know range bounds, check that the low bound is not greater
     than the high bound. If it is, we do nothing. */
  if (GET_CODE (rtx_lo) == CONST_INT && GET_CODE (rtx_hi) == CONST_INT)
    {
      if (INTVAL (rtx_lo) > INTVAL (rtx_hi))
        {
          pedwarn ("Set range low bound greater than high bound");
          return target;
        }
    }
  else
    {
      /* Same check at runtime */
      /* Note: after this point must emit_label(label) before return */
      emit_cmp_insn (rtx_lo, rtx_hi, GT, 0, GET_MODE_2 (rtx_hi, rtx_lo), 0, 0);
      emit_jump_insn (gen_bgt (label));
    }

  /* One word set operation */
  if (GET_MODE (set) == word_mode || one_bit || one_word) /* BLKmode change */
    {
      rtx ptr;
      rtx word = set;

      if (GET_MODE (set) != word_mode)
        {
          ptr = gen_reg_rtx (Pmode);

          if (integer_zerop (nwl))
            emit_move_insn (ptr, memory_address (word_mode, XEXP (target, 0)));
          else
            {
              rtx ntarget = expand_binop (Pmode, add_optab,
                                          memory_address (word_mode,
                                                          XEXP (target, 0)),
                                          rtx_nwl, ptr, 1, OPTAB_LIB_WIDEN);
              if (ntarget != ptr)
                emit_move_insn (ptr, ntarget);
            }

          word = gen_rtx (MEM, word_mode, ptr);
        }

      /* mask0 = (~(-2 << (nbh - nbl))) << nbl; */

      /* @@ Should convert intermediate for int != long machines? */
      if (one_bit)
        mask0 = expand_expr (fold (build (LSHIFT_EXPR, long_integer_type_node,
                                          size_one_node, nbl)),
                             NULL_RTX, VOIDmode, 0);
      else
        mask0 = expand_expr
                (fold
                 (build
                  (LSHIFT_EXPR, long_integer_type_node,
                   fold (build1 (BIT_NOT_EXPR, long_integer_type_node,
                                 fold (build (LSHIFT_EXPR,
                                              long_integer_type_node,
                                              long_int (-2),
                                              fold (build (MINUS_EXPR,
                                                           integer_type_node,
                                                           nbh, nbl)))))),
                   nbl)),
                 NULL_RTX, word_mode, 0);

      switch (what) {
      case CHECK_INSIDE:
        break;
      case CLEAR_INSIDE:
        mask0 = expand_unop (word_mode, one_cmpl_optab, mask0, NULL_RTX, 1);
        break;
      case SET_INSIDE:
        binoptab = ior_optab;
        break;
      case CHECK_OUTSIDE:
        mask0 = expand_unop (word_mode, one_cmpl_optab, mask0, NULL_RTX, 1);
        break;
      case CLEAR_OUTSIDE:
        break;
      case SET_OUTSIDE:
        mask0 = expand_unop (word_mode, one_cmpl_optab, mask0, NULL_RTX, 1);
        binoptab = ior_optab;
        break;
      default:
        abort ();
      }

      temp = expand_binop (word_mode, binoptab, word, mask0,
                           word, 0, OPTAB_LIB_WIDEN);
      if (!result)
        {
          /* Make sure the resulting word is stored to the original target
           * (word points to the correct word)
           */
          if (word != temp)
            emit_move_insn (word, temp);

          /* Return the original target */
          emit_label (label);
          return target;
        }

      /* Doing a CHECK_* comparison operation */

      emit_move_insn (result, invert ? const1_rtx : const0_rtx);
      emit_cmp_insn (temp, const0_rtx, NE, NULL_RTX, word_mode, 0, 0);
      emit_jump_insn (gen_bne (label));
      emit_move_insn (result, invert ? const0_rtx : const1_rtx);

      /* this is also the exit label */
      emit_label (label);
      return result;
    }

  /* Multi word operation */
  {
    rtx word_value;

    rtx loop_start_label = NULL_RTX;
    rtx loop_end_label = NULL_RTX;
    rtx one_word_label = NULL_RTX;

    rtx ne_label = gen_label_rtx ();

    /* If LO & HI are in same word */
    int combined = tree_int_cst_equal (nwl, nwh);

    rtx ntarget;

    /* Word counter (word_mode crashes in alpha, take the
     * easy way out)
     */
    rtx count = gen_reg_rtx (SImode);

    /* Pointer to beginning of a word aligned set */
    rtx ptr = gen_reg_rtx (Pmode);

    /* word is a MEM where ptr points to */
    rtx word = gen_rtx (MEM, word_mode, ptr);

    /* @@@@@@@@@@@@@@@@@@@@@@@@
       CHECK_OUTSIDE is not implemented currently...
       now it returns target instead of true/false
       if we skip the operations with these.
       (too tired to do it now)
     */
    int skip_operation_lo = 0;
    int skip_operation_hi = 0;

    target = set;

    /* mask0       == mask of word containing LO
     * mask1       == mask of word containing HI
     * mask2       == mask of word containing both LO and HI (if fits)
     * word_value  == suitable mask for intermediate words
     * binoptab    == operation we apply to set words
     */

    /* @@ nbl needs conversion? */

    /* lsw mask: mask0 = -1 << nbl; */
    mask0 = expand_expr (fold (build (LSHIFT_EXPR, long_integer_type_node,
                                      long_int (-1), nbl)),
                         NULL_RTX, VOIDmode, 0);

    /* msw mask: mask1 = ~(-2 << nbh); */
    mask1 = expand_expr (fold (build1 (BIT_NOT_EXPR, long_integer_type_node,
                                       fold (build (LSHIFT_EXPR,
                                                    long_integer_type_node,
                                                    long_int (-2),
                                                    nbh)))),
                         NULL_RTX, VOIDmode, 0);

    /* mask2 = (~(-2 << (nbh - nbl))) << nbl; */

    if (GET_CODE (rtx_nwl) != CONST_INT /* lo or rlow varies */
        || GET_CODE (rtx_nwh) != CONST_INT      /* hi or rhigh varies */
        || combined)
      mask2 = expand_expr
                (fold
                 (build
                  (LSHIFT_EXPR, long_integer_type_node,
                   fold (build1 (BIT_NOT_EXPR, long_integer_type_node,
                                 fold (build (LSHIFT_EXPR,
                                              long_integer_type_node,
                                              long_int (-2),
                                              fold (build (MINUS_EXPR,
                                                           integer_type_node,
                                                           nbh, nbl)))))),
                   nbl)),
                 NULL_RTX, word_mode, 0);

    /* mask/comparison value for whole words in or out of set */
    word_value = GEN_INT (-1);

    switch (what) {
    case CHECK_INSIDE:
      word_value = const0_rtx;
      break;
    case CLEAR_INSIDE:
      mask0 = expand_unop (word_mode, one_cmpl_optab, mask0, NULL_RTX, 1);
      mask1 = expand_unop (word_mode, one_cmpl_optab, mask1, NULL_RTX, 1);
      if (mask2)
        mask2 = expand_unop (word_mode, one_cmpl_optab, mask2, NULL_RTX, 1);
      word_value = const0_rtx;
      break;
    case SET_INSIDE:
      binoptab = ior_optab;
      break;
    case CHECK_OUTSIDE:
      mask0 = expand_unop (word_mode, one_cmpl_optab, mask0, NULL_RTX, 1);
      mask1 = expand_unop (word_mode, one_cmpl_optab, mask1, NULL_RTX, 1);
      if (mask2)
        mask2 = expand_unop (word_mode, one_cmpl_optab, mask2, NULL_RTX, 1);
      word_value = const0_rtx;
      break;
    case CLEAR_OUTSIDE:
      word_value = const0_rtx;
      break;
    case SET_OUTSIDE:
      mask0 = expand_unop (word_mode, one_cmpl_optab, mask0, NULL_RTX, 1);
      mask1 = expand_unop (word_mode, one_cmpl_optab, mask1, NULL_RTX, 1);
      if (mask2)
        mask2 = expand_unop (word_mode, one_cmpl_optab, mask2, NULL_RTX, 1);
      binoptab = ior_optab;
      break;
    default:
      abort ();
    }

    /* Handle words: 0..(nwl-BYTES_PER_WORD)
     * i.e. whole words before the word where LO lives
     */

    switch (what) {
    case CHECK_INSIDE:
    case CLEAR_INSIDE:
    case SET_INSIDE:
      /* move pointer to word where LO lives */

      if (integer_zerop (nwl))
        emit_move_insn (ptr, memory_address (word_mode, XEXP (target, 0)));
      else
        {
          ntarget = expand_binop (Pmode, add_optab,
                                  memory_address (word_mode,
                                                  XEXP (target, 0)),
                                  rtx_nwl, ptr, 1, OPTAB_LIB_WIDEN);
          if (ntarget != ptr)
            emit_move_insn (ptr, ntarget);
        }
      break;

    case CHECK_OUTSIDE:
    case CLEAR_OUTSIDE:
    case SET_OUTSIDE:
      /* Any bits in allocated set words before LO? */
      if (GET_CODE (rtx_nwl) == CONST_INT
          && GET_CODE (rtx_owl) == CONST_INT
          && INTVAL (rtx_nwl) >= INTVAL (rtx_owl))
        skip_operation_lo = 1;

      /* How about HI then? */
      if (GET_CODE (rtx_nwh) == CONST_INT
          && GET_CODE (rtx_owh) == CONST_INT
          && INTVAL (rtx_nwh) <= INTVAL (rtx_owh))
        skip_operation_hi = 1;

      if (skip_operation_lo && skip_operation_hi)
        {
          /* No need to do anything, since the set set does not exist outside range */
          emit_label (label);
          return target;
        }

      if (skip_operation_lo)
        {
          if (integer_zerop (nwl))
            emit_move_insn (ptr, memory_address (word_mode, XEXP (target, 0)));
          else
            {
              ntarget = expand_binop (Pmode, add_optab,
                                      memory_address (word_mode,
                                                      XEXP (target, 0)),
                                      rtx_nwl, ptr, 1, OPTAB_LIB_WIDEN);
              if (ntarget != ptr)
                emit_move_insn (ptr, ntarget);
            }
        }
      else
        {
          tree prev;

          loop_start_label = gen_label_rtx ();
          loop_end_label = gen_label_rtx ();

          if (GET_CODE (rtx_nwl) != CONST_INT
              || GET_CODE (rtx_owl) != CONST_INT)
            {
              emit_cmp_insn (rtx_nwl, rtx_owl, LT,
                             NULL_RTX, GET_MODE_2 (rtx_nwl, rtx_owl), 0, 0);
              emit_jump_insn (gen_blt (loop_end_label));
            }
          prev = setop_prev_word (lo, rlow);

          /* Number of words to handle */
          emit_move_insn (count,
                          expand_expr (fold (build (TRUNC_DIV_EXPR,
                                                    integer_type_node,
                                                    prev, WORD_IN_UNITS)),
                                       NULL_RTX, VOIDmode, 0));

          if (integer_zerop (nwl))
            emit_move_insn (ptr, memory_address (word_mode, XEXP (target, 0)));
          else
            {
              ntarget = expand_binop (Pmode, add_optab,
                                      memory_address (word_mode,
                                                      XEXP (target, 0)),
                                      rtx_nwl, ptr, 1, OPTAB_LIB_WIDEN);
              if (ntarget != ptr)
                emit_move_insn (ptr, ntarget);
            }

          emit_label (loop_start_label);
          emit_cmp_insn (count, const0_rtx, LE,
                         NULL_RTX, GET_MODE (count), 0, 0);
          emit_jump_insn (gen_ble (loop_end_label));

          if (! result)
            {
              ntarget = expand_binop (word_mode, binoptab, word, word_value,
                                      word, 0, OPTAB_LIB_WIDEN);

              if (ntarget != word)
                emit_move_insn (word, ntarget);
            }
          else
            {
              emit_cmp_insn (word, word_value, NE, NULL_RTX, word_mode,
                             1, UNITS_PER_WORD);

              /* this word is not equal to zero -> jump */
              emit_jump_insn (gen_bne (ne_label));
            }

          NEXT_WORD (ptr);

          expand_dec (count, const1_rtx);

          emit_jump (loop_start_label);
          emit_label (loop_end_label);
        }
      break;
    }

    /* Now ptr points to the word where LO lives.
       Since HI may be in the same word, check this at runtime if
       not constants */

    if (mask2)
      {
        if (! combined)
          {
            /* not actually a loop here */
            loop_start_label = gen_label_rtx ();
            one_word_label = gen_label_rtx ();

            /* non-constant or different LO & HI, check at runtime */
            emit_cmp_insn (rtx_nwl, rtx_nwh, NE, NULL_RTX,
                           GET_MODE_2 (rtx_nwl, rtx_nwh), 1, UNITS_PER_WORD);
            emit_jump_insn (gen_bne (loop_start_label));
          }
        ntarget = expand_binop (word_mode, binoptab, word, mask2,
                                result ? NULL_RTX : word,
                                0, OPTAB_LIB_WIDEN);
        /* word contains both LO & HI */
        if (! result)
          {
            if (ntarget != word)
              emit_move_insn (word, ntarget);
          }
        else
          {
            emit_cmp_insn (ntarget, word_value, NE, NULL_RTX, word_mode,
                           1, UNITS_PER_WORD);

            /* this word is not equal to zero -> jump */
            emit_jump_insn (gen_bne (ne_label));
          }
      }

    if (! combined)     /* not constant or LO & HI in different words */
      {
        if (mask2)
          {
            emit_jump (one_word_label);
            emit_label (loop_start_label);
          }

        ntarget = expand_binop (word_mode, binoptab, word, mask0,
                                result ? NULL_RTX : word,
                                0, OPTAB_LIB_WIDEN);
        /* word contains the low bound, i.e. setop_word (lo, rlow) == nwl */
        if (! result)
          {
            if (ntarget != word)
              emit_move_insn (word, ntarget);
          }
        else
          {
            emit_cmp_insn (ntarget, word_value, NE, NULL_RTX, word_mode,
                           1, UNITS_PER_WORD);

            /* this word is not equal to zero -> jump */
            emit_jump_insn (gen_bne (ne_label));
          }

        /* words that are between the words that contain
           LO and HI (exclusive) */
        switch (what) {
        case CHECK_INSIDE:
        case CLEAR_INSIDE:
        case SET_INSIDE:
          if (GET_CODE (rtx_nwl) != CONST_INT
              || GET_CODE (rtx_nwh) != CONST_INT
              || INTVAL (rtx_nwl) < INTVAL (rtx_nwh))
            {
              tree next;

              loop_start_label = gen_label_rtx ();
              loop_end_label   = gen_label_rtx ();

              if (GET_CODE (rtx_nwl) != CONST_INT
                  || GET_CODE (rtx_nwh) != CONST_INT)
                {
                  emit_cmp_insn (rtx_nwl, rtx_nwh, GT,
                                 NULL_RTX, GET_MODE_2 (rtx_nwl, rtx_nwh), 0, 0);
                  emit_jump_insn (gen_bgt (loop_end_label));
                }
              next = setop_next_word (lo, rlow);

              /* Word after LO's word */
              NEXT_WORD (ptr);

              /* Number of words to handle */
              emit_move_insn (count,
                              expand_expr
                                  (fold (build (TRUNC_DIV_EXPR,
                                                integer_type_node,
                                                fold (build (MINUS_EXPR,
                                                             integer_type_node,
                                                             nwh, next)),
                                                WORD_IN_UNITS)),
                                   NULL_RTX, VOIDmode, 0));

              emit_label (loop_start_label);
              emit_cmp_insn (count, const0_rtx, LE,
                             NULL_RTX, GET_MODE (count), 0, 0);
              emit_jump_insn (gen_ble (loop_end_label));

              if (! result)
                {
                  ntarget = expand_binop (word_mode, binoptab, word,
                                          word_value,
                                          word, 0, OPTAB_LIB_WIDEN);
                  if (ntarget != word)
                    emit_move_insn (word, ntarget);
                }
              else
                {
                  emit_cmp_insn (word, word_value, NE, NULL_RTX, word_mode,
                                 1, UNITS_PER_WORD);

                  /* this word is not equal to zero -> jump */
                  emit_jump_insn (gen_bne (ne_label));
                }

              NEXT_WORD (ptr);

              expand_dec (count, const1_rtx);

              emit_jump (loop_start_label);
              emit_label (loop_end_label);
            }
          break;

        case CHECK_OUTSIDE:
        case CLEAR_OUTSIDE:
        case SET_OUTSIDE:
          if (skip_operation_hi)
            {
              emit_label (label);
              if (mask2)
                emit_label (one_word_label);
              return target;
            }

          /* move pointer to word where HI lives */
          if (integer_zerop (nwh))
            emit_move_insn (ptr, memory_address (word_mode,
                                                 XEXP (target, 0)));
          else
            {
              ntarget = expand_binop (Pmode, add_optab,
                                      memory_address (word_mode,
                                                      XEXP (target, 0)),
                                      rtx_nwh, ptr, 1, OPTAB_LIB_WIDEN);
              if (ntarget != ptr)
                emit_move_insn (ptr, ntarget);
            }
          break;
        }

        /* now ptr points to the word HI is in */
        /* Comparisons need to mask out unwanted bits */
        ntarget = expand_binop (word_mode, binoptab, word, mask1,
                                result ? NULL_RTX : word,
                                0, OPTAB_LIB_WIDEN);
        if (! result)
          {
            if (ntarget != word)
              emit_move_insn (word, ntarget);
          }
        else
          {
            emit_cmp_insn (ntarget, word_value, NE, NULL_RTX, word_mode,
                           1, UNITS_PER_WORD);

            /* this word is not equal to zero -> jump */
            emit_jump_insn (gen_bne (ne_label));
          }

        /* If LO and HI are in the same word and it was found out at
           runtime, jump here */
        if (mask2)
          emit_label (one_word_label);
      }

    /* Handle words: (nwh+BYTES_PER_WORD)..owh
     * i.e. words after the word where HI lives to the end of set
     */

    switch (what) {
    case CHECK_INSIDE:
    case CLEAR_INSIDE:
    case SET_INSIDE:
      break;

    case CHECK_OUTSIDE:
    case CLEAR_OUTSIDE:
    case SET_OUTSIDE:
      /* Words exist after the word that HI lives in? */
      if (GET_CODE (rtx_nwh) != CONST_INT
          || GET_CODE (rtx_owh) != CONST_INT
          || INTVAL (rtx_nwh) < INTVAL (rtx_owh))
        {
          tree next;

          loop_start_label = gen_label_rtx ();
          loop_end_label = gen_label_rtx ();

          if (GET_CODE (rtx_nwh) != CONST_INT
              || GET_CODE (rtx_owh) != CONST_INT)
            {
              emit_cmp_insn (rtx_nwh, rtx_owh, LT,
                             NULL_RTX, GET_MODE_2 (rtx_nwh, rtx_owh), 0, 0);
              emit_jump_insn (gen_blt (loop_end_label));
            }
          next = setop_next_word (hi, rlow);

          /* Number of words to handle */
          emit_move_insn (count,
                          expand_expr
                              (fold (build (TRUNC_DIV_EXPR,
                                            integer_type_node,
                                            fold (build (MINUS_EXPR,
                                                         integer_type_node,
                                                         owh, next)),
                                            WORD_IN_UNITS)),
                               NULL_RTX, VOIDmode, 0));

          emit_label (loop_start_label);
          emit_cmp_insn (count, const0_rtx, LE,
                         NULL_RTX, GET_MODE (count), 0, 0);
          emit_jump_insn (gen_ble (loop_end_label));

          NEXT_WORD (ptr);

          if (! result)
            {
              ntarget = expand_binop (word_mode, binoptab, word, word_value,
                                      word, 0, OPTAB_LIB_WIDEN);
              if (ntarget != word)
                emit_move_insn (word, ntarget);
            }
          else
            {
              emit_cmp_insn (word, word_value, NE, NULL_RTX, word_mode,
                             1, UNITS_PER_WORD);

              /* this word is not equal to zero -> jump */
              emit_jump_insn (gen_bne (ne_label));
            }

          expand_dec (count, const1_rtx);

          emit_jump (loop_start_label);
          emit_label (loop_end_label);
        }
      break;
    }

    if (! result)
      {
        emit_label (label);
        return target;
      }

    /* Expected sets to be empty in CHECK_* */

    emit_move_insn (result, invert ? const0_rtx : const1_rtx);
    emit_jump (label); /* EQ label */

    emit_label (ne_label);
    emit_move_insn (result, invert ? const1_rtx : const0_rtx);
  }

  emit_label (label);
  return result;
}

/* Copy SET with bound LO_A, HI_A to TARGET with bounds LO_R, HI_R
 * Always clobbers all bits in target
 */
static void
setop_copy (set, lo_a, hi_a, target, lo_r, hi_r)
     rtx set;
     tree lo_a, hi_a;
     rtx target;
     tree lo_r, hi_r;
{
  tree bytesA = setop_num_bytes (lo_a, hi_a);
  tree bytesT = setop_num_bytes (lo_r, hi_r);
  tree temp;
  int align = TYPE_ALIGN (TREE_TYPE (bytesT));

  /* @@ Variable bounds not implemented yet */
  int low_a = TREE_INT_CST_LOW (lo_a);
  int high_a = TREE_INT_CST_LOW (hi_a);
  int low_r = TREE_INT_CST_LOW (lo_r);
  int high_r = TREE_INT_CST_LOW (hi_r);
  int n_lo_a;
  int n_hi_a;
  tree n_lo_a_t;
  tree n_hi_a_t;

  if (TREE_CODE (bytesA) != INTEGER_CST || TREE_CODE (bytesT) != INTEGER_CST)
    abort ();

  if (setop_empty (set, lo_a, hi_a))
    {
      clear_storage (target, GEN_INT (TREE_INT_CST_LOW (bytesT)), align);
      return;
    }

  /* No common elements in sets */
  if (high_a < low_r || low_a > high_r)
    abort ();

  /* Copy only part of SET if TARGET is smaller than SET */
  if (low_a <= low_r)
    n_lo_a = low_r;
  else
    n_lo_a = low_a;

  if (high_a >= high_r)
    n_hi_a = high_r;
  else
    n_hi_a = high_a;

  n_lo_a_t = build_int_2 (n_lo_a, 0);
  n_hi_a_t = build_int_2 (n_hi_a, 0);

  /* Inform later passes that all of target will be altered */
  emit_insn (gen_rtx (CLOBBER, VOIDmode, target));

  /* Need block copy only if both are BLKmode */
  if (GET_MODE (set) != BLKmode || GET_MODE (target) != BLKmode)
    {
      rtx to   = target;
      rtx from = set;
      rtx ptr;

      if (GET_MODE (set) == BLKmode)
        {
          /* Word pointer PTR points to the correct word.
           * FROM represents the contents of that word.
           */

          /* set copied from n_lo_a element up */
          temp = setop_vector_adjust (n_lo_a_t, lo_a);

          if (integer_zerop (temp))
            ptr = memory_address (GET_MODE (target), XEXP (set, 0));
          else
            ptr = memory_address (GET_MODE (target),
                                  expand_binop (Pmode, add_optab,
                                                XEXP (set, 0),
                                                expand_expr (temp, NULL_RTX,
                                                             VOIDmode, 0),
                                                NULL_RTX, 1, OPTAB_LIB_WIDEN));

          from = gen_rtx (MEM, GET_MODE (target), ptr);
        }
      else if (GET_MODE (target) == BLKmode)
        {
          /* Make sure target bits will be ok */
          clear_storage (target, GEN_INT (TREE_INT_CST_LOW (bytesT)), align);

          /* Word pointer PTR points to the correct word.
           * TO represents the contents of that word.
           */

          /* target modified from n_lo_a_t element up */
          temp = setop_vector_adjust (n_lo_a_t, lo_r);

          if (integer_zerop (temp))
            ptr = memory_address (GET_MODE (set), XEXP (target, 0));
          else
            ptr = memory_address (GET_MODE (set),
                                  expand_binop (Pmode, add_optab,
                                                XEXP (target, 0),
                                                expand_expr (temp, NULL_RTX,
                                                             VOIDmode, 0),
                                                NULL_RTX, 1, OPTAB_LIB_WIDEN));

          to = gen_rtx (MEM, GET_MODE (set), ptr);
        }

      /* Copy it with a single move */
      emit_move_insn (to, from);
    }
  else /* Both are BLKmode sets. */
    {
      /* If the source is adjusted with the target do a block move
       * and possibly clear excess words
       */
      if (TREE_INT_CST_LOW (setop_rlo (lo_a))
          == TREE_INT_CST_LOW (setop_rlo (lo_r)))
        {
          temp = bytesT;
          /* Clear excess words */
          if (TREE_INT_CST_LOW (bytesA) < TREE_INT_CST_LOW (bytesT))
            {
              /* @@ for not clear the whole destination */
              clear_storage (target, GEN_INT (TREE_INT_CST_LOW (bytesT)), align);
              temp = bytesA;
            }
          emit_block_move (target, set,
                           expand_expr (temp, NULL_RTX, VOIDmode, 0),
                           UNITS_PER_WORD);
        }
      else
        {
          /* BLKmode sets are not aligned to the same low bound */

          /* But make sure all of target gets cleared */
          if (n_lo_a != low_r || n_hi_a != high_r)
            clear_storage (target, GEN_INT (TREE_INT_CST_LOW (bytesT)), align);

          emit_block_move
            (change_address
              (target, VOIDmode,
               plus_constant (XEXP (target, 0),
                              expand_expr (setop_word (n_lo_a_t, lo_r), NULL_RTX,
                                           VOIDmode, 0))),
             change_address
              (set, VOIDmode,
               plus_constant (XEXP (target, 0),
                              expand_expr (setop_word (n_lo_a_t, lo_a), NULL_RTX,
                                           VOIDmode, 0))),
             expand_expr (setop_num_bytes (n_lo_a_t, n_hi_a_t), NULL_RTX,
                          VOIDmode, 0),
             UNITS_PER_WORD);
        }
    }
}

/*
 * Sum the number of set bits in word (code REG) to TARGET
 * KEEP_WORD is nonzero if word cannot be destroyed
 */
static rtx
setop_card_one_word (source, target, keep_word)
     rtx source;
     rtx target;
     int keep_word;
{
  rtx word = source;
  rtx end_label = gen_label_rtx ();
  rtx start = gen_label_rtx ();

  if (keep_word)
    {
      word = gen_reg_rtx (word_mode);
      emit_move_insn (word, source);
    }

  emit_note (NULL_PTR, NOTE_INSN_LOOP_BEG);
  emit_note (NULL_PTR, NOTE_INSN_LOOP_CONT);
  emit_label (start);

  /* Loop if word not zero, unsigned, alignment in bytes */
  emit_cmp_insn (word, const0_rtx, EQ, NULL_RTX,
                 word_mode, 1, UNITS_PER_WORD);
  emit_jump_insn (gen_beq (end_label));

  /* add the least significand bit to the count */
  target = expand_binop (GET_MODE (target), add_optab,
                         target,
                         expand_binop (word_mode, and_optab,
                                       word, const1_rtx, NULL_RTX,
                                       0, OPTAB_LIB_WIDEN),
                         target, 0, OPTAB_LIB_WIDEN);

  /* Logical shift right 1 bit */
  word = expand_shift (RSHIFT_EXPR, word_mode, word,
                       integer_one_node, word, 1);
  emit_jump (start);
  emit_note (NULL_PTR, NOTE_INSN_LOOP_END);
  emit_label (end_label);
  emit_queue ();

  return target;
}


/* CARD_EXPR, returns target.
 *
 * Args: T_SET is the tree type set operand.
 *       TARGET is the suggested target.
 *
 * Returns: rtx where the result was placed.
 */

rtx
expand_set_card (t_set, target)
     tree t_set;
     rtx target;
{
  tree min = TYPE_MIN_VALUE (TYPE_DOMAIN (TREE_TYPE (t_set)));
  tree max = TYPE_MAX_VALUE (TYPE_DOMAIN (TREE_TYPE (t_set)));

  rtx  set = expand_expr (t_set, NULL_RTX, VOIDmode, 0);

  if (! target)
    target = gen_reg_rtx (SImode);

  emit_move_insn (target, const0_rtx);

  if (GET_MODE (set) == word_mode)
    target = setop_card_one_word (set, target, 1);
  else
    {
      rtx words = expand_expr (setop_num_words (min, max),
                               NULL_RTX, VOIDmode, 0);
      rtx count = gen_reg_rtx (word_mode);
      rtx start = gen_label_rtx ();
      rtx end_label = gen_label_rtx ();
      rtx ptr  = gen_reg_rtx (Pmode);
      rtx word = gen_rtx (MEM, word_mode, ptr);

      emit_move_insn (ptr,
                      memory_address (word_mode, XEXP (set, 0)));
      emit_move_insn (count, words);

      emit_note (NULL_PTR, NOTE_INSN_LOOP_BEG);
      emit_note (NULL_PTR, NOTE_INSN_LOOP_CONT);
      emit_label (start);
      emit_cmp_insn (count, const0_rtx, EQ,
                     NULL_RTX, GET_MODE (count), 0, 0);

      emit_jump_insn (gen_beq (end_label));

      /* Count bits in one word */
      target = setop_card_one_word (word, target, 0);

      NEXT_WORD (ptr);
      word = gen_rtx (MEM, word_mode, ptr);

      expand_dec (count, const1_rtx);
      emit_jump (start);
      emit_note (NULL_PTR, NOTE_INSN_LOOP_END);
      emit_label (end_label);
      emit_queue ();
    }
  return target;
}

/* Inlined set binop, returns rtx where the resulting set is stored */

rtx
expand_set_binop (exp, target)
     tree exp;
     rtx target;
{
  enum tree_code code = TREE_CODE (exp);
  optab binoptab;
  tree set0_t = TREE_OPERAND (exp, 0);
  tree set1_t = TREE_OPERAND (exp, 1);

  tree lo0 = TYPE_MIN_VALUE (TYPE_DOMAIN (TREE_TYPE (set0_t)));
  tree hi0 = TYPE_MAX_VALUE (TYPE_DOMAIN (TREE_TYPE (set0_t)));

  tree lo1 = TYPE_MIN_VALUE (TYPE_DOMAIN (TREE_TYPE (set1_t)));
  tree hi1 = TYPE_MAX_VALUE (TYPE_DOMAIN (TREE_TYPE (set1_t)));

  rtx set0 = expand_expr (set0_t, NULL_RTX, VOIDmode, 0);
  rtx set1 = expand_expr (set1_t, NULL_RTX, VOIDmode, 0);

  switch (code)
    {
      case BIT_AND_EXPR:      /* op1 * op2 */
        binoptab = and_optab;
        break;
      case BIT_ANDTC_EXPR:    /* op1 - op2  == op1 AND (NOT op2) */
        binoptab = and_optab;
        break;
      case BIT_IOR_EXPR:      /* op1 + op2 */
        binoptab = ior_optab;
        break;
      case BIT_XOR_EXPR:      /* op1 >< op2 (Extended Pascal symmetric difference) */
        binoptab = xor_optab;
        break;
      default:
        abort ();
    }

  /* Optimize single word binary operations */
  if (TREE_CODE (lo0) == INTEGER_CST && TREE_CODE (hi0) == INTEGER_CST
      && TREE_CODE (lo1) == INTEGER_CST && TREE_CODE (hi1) == INTEGER_CST
      && TREE_INT_CST_LOW (setop_rlo (lo0))
          == TREE_INT_CST_LOW (setop_rlo (lo1))  /* same set segment (low bound check) */
      && TREE_INT_CST_LOW (setop_rlo (hi0))
          == TREE_INT_CST_LOW (setop_rlo (hi1))  /* same set segment (high bound check) */
      && GET_MODE (set0) == GET_MODE (set1)
      && GET_MODE (set0) != BLKmode)
    {
      if (code == BIT_ANDTC_EXPR)
        set1 = expand_unop (GET_MODE (set1), one_cmpl_optab, set1, NULL_RTX, 1);

      target = expand_binop (GET_MODE (set1), binoptab, set0, set1,
                             target, 1, OPTAB_LIB_WIDEN);
    }
  else
    {
      /* @@@ Check if this code should work also for
         set that are e.g. DImode -> it now seems to convert:
         DImode set operation (s1 + s2) -> BLKmode
         This will crash later
       */
      rtx count = gen_reg_rtx (word_mode);
      rtx start_label = NULL_RTX;
      rtx end_label = NULL_RTX;

      rtx src;
      rtx src_p = gen_reg_rtx (Pmode);
      rtx dest;
      rtx dest_p= gen_reg_rtx (Pmode);

      tree lo_r;
      tree hi_r;

      rtx ntarget;
      int onex = 0;     /* If any single word sets */
      int selected = 0;
      tree temp;

      /* @@@ Target should always be the largest possible
       *     subrange type
       *     For "set of integer" there is a compile time size
       *     limit (0..255).
       *     @@It should be dynamic.
       */
      if (target)
        {
          if (TREE_CODE (TREE_TYPE (set0_t)) == INTEGER_TYPE
              && !TREE_TYPE (TREE_TYPE (set0_t)))
            {
              /* @@@@@ How to get the limits here language_independently??? */
              lo_r = integer_zero_node;
              lo_r = build_int_2 (255, 0);
            }
          else
            {
              tree dom = TYPE_DOMAIN (TREE_TYPE (set0_t));
              lo_r = TYPE_MIN_VALUE (dom);
              hi_r = TYPE_MAX_VALUE (dom);
            }
        }
      else
        {
          tree words;
          selected = fixup_set_bounds (code,
                                       TREE_TYPE (set0_t), TREE_TYPE (set1_t),
                                       &lo_r, &hi_r);

          if (selected == 1)
            {
              /* Swap sets for BIT_AND_EXPR (to get target smaller) */
              rtx tmp = set0; set0 = set1; set1 = tmp;

              temp = lo0; lo0 = lo1; lo1 = temp;
              temp = hi0; hi0 = hi1; hi1 = temp;
              temp = set0_t; set0_t = set1_t; set1_t = temp;
            }

          /* @@@ Should check for BLKmode sets (not one word sets).
             e.g. DImode sets could be optimized */
          words = setop_num_words (lo_r, hi_r);

          if (TREE_CODE (words) != INTEGER_CST)
            abort ();

          if (TREE_INT_CST_LOW (words) == 1)
            target = gen_reg_rtx (word_mode);
          else
            target = assign_stack_temp (BLKmode,
                                        TREE_INT_CST_LOW (words)
                                         * UNITS_PER_WORD,
                                        0);
        }

      if (TREE_CODE (lo0) != INTEGER_CST || TREE_CODE (hi0) != INTEGER_CST
          || TREE_CODE (lo1) != INTEGER_CST || TREE_CODE (hi1) != INTEGER_CST
          || TREE_CODE (lo_r) != INTEGER_CST
          || TREE_CODE (hi_r) != INTEGER_CST)
        abort ();

      /* Optimize and initialize target for operation */
      switch (code) {
      case BIT_AND_EXPR:
        /* handle empty sets and sets with no common elements */
        if (setop_empty (set1, lo1, hi1) || setop_empty (set0, lo0, hi0)
            || TREE_INT_CST_LOW (hi1) < TREE_INT_CST_LOW (lo0)
            || TREE_INT_CST_LOW (lo1) > TREE_INT_CST_LOW (hi0)
            || (GET_MODE (set1) != BLKmode
                && TREE_INT_CST_LOW (lo1) < TREE_INT_CST_LOW (lo0)))
          {
            tree bytesT = setop_num_bytes (lo_r, hi_r);
            int align = TYPE_ALIGN (TREE_TYPE (bytesT));
            clear_storage (target, GEN_INT (TREE_INT_CST_LOW (bytesT)),
                           align);
            return target;
          }
        setop_copy (set0, lo0, hi0, target, lo_r, hi_r);

        /* S * S == S; @@ is fold() too early to do this? */
        if (set0_t == set1_t)
          return target;
        break;

      case BIT_ANDTC_EXPR:

        /* [] - S == [], S - S == [] */
        if (setop_empty (set0, lo0, hi0) || set0_t == set1_t)
          {
            tree bytesT = setop_num_bytes (lo_r, hi_r);
            int align = TYPE_ALIGN (TREE_TYPE (bytesT));
            clear_storage (target, GEN_INT (TREE_INT_CST_LOW (bytesT)),
                           align);
            return target;
          }

        setop_copy (set0, lo0, hi0, target, lo_r, hi_r);

        /* handle empty set1 and sets with no common elements */
        if (setop_empty (set1, lo1, hi1)
            || TREE_INT_CST_LOW (hi1) < TREE_INT_CST_LOW (lo0)
            || TREE_INT_CST_LOW (lo1) > TREE_INT_CST_LOW (hi0)
            || (GET_MODE (set1) != BLKmode
                && TREE_INT_CST_LOW (lo1) < TREE_INT_CST_LOW (lo0)))
          return target;
        break;

      case BIT_IOR_EXPR:
        setop_copy (set0, lo0, hi0, target, lo_r, hi_r);

        /* S + [] == S, [] + S == S, [] + [] == [] */
        if (setop_empty (set1, lo1, hi1) || setop_empty (set0, lo0, hi0))
          return target;

        /* S + S == S */
        if (set0_t == set1_t)
          return target;
        break;

      case BIT_XOR_EXPR:
        /* S >< S == [] */
        if (set0_t == set1_t)
          {
            tree bytesT = setop_num_bytes (lo_r, hi_r);
            int align = TYPE_ALIGN (TREE_TYPE (bytesT));
            clear_storage (target, GEN_INT (TREE_INT_CST_LOW (bytesT)),
                           align);
            return target;
          }
        setop_copy (set0, lo0, hi0, target, lo_r, hi_r);

        /* S >< [] == S, [] >< S == S, [] >< [] == [] */
        if (setop_empty (set1, lo1, hi1) || setop_empty (set0, lo0, hi0))
          return target;

      default:
        /* NOTHING */ ;
      };

      if (GET_MODE (target) == BLKmode)
        {
          emit_move_insn (dest_p,
                          memory_address (word_mode, XEXP (target, 0)));
          dest = gen_rtx (MEM, word_mode, dest_p);
        }
      else
        {
          dest = target;
          onex++;
        }

      if (GET_MODE (set1) == BLKmode)
        {
          emit_move_insn (src_p,
                          memory_address (word_mode, XEXP (set1, 0)));
          src = gen_rtx (MEM, word_mode, src_p);
        }
      else
        {
          onex++;
          src = set1;
        }

      switch (code) {

      case BIT_AND_EXPR:        /* op1 * op2 */
        /* TARGET is possibly smaller than SET1 */

        if (TREE_INT_CST_LOW (lo1) < TREE_INT_CST_LOW (lo_r))
          {
            if (GET_CODE (set1) != MEM)
              abort ();

            temp = setop_vector_adjust (lo1, lo_r);

            if (! integer_zerop (temp))
              src_p = expand_binop (Pmode, add_optab, src_p,
                                    expand_expr (temp,
                                                 NULL_RTX,
                                                 VOIDmode, 0),
                                    src_p,
                                    1, OPTAB_LIB_WIDEN);
            lo1 = lo_r;
          }

        if (TREE_INT_CST_LOW (hi1) > TREE_INT_CST_LOW (hi_r))
          hi1 = hi_r;
        break;

      case BIT_ANDTC_EXPR:      /* op1 - op2  == op1 AND (NOT op2) */
        if (TREE_INT_CST_LOW (lo1) < TREE_INT_CST_LOW (lo_r))
          {
            if (GET_CODE (set1) != MEM)
              abort ();

            temp = setop_vector_adjust (lo1, lo_r);

            if (! integer_zerop (temp))
              src_p = expand_binop (Pmode, add_optab, src_p,
                                    expand_expr (temp,
                                                 NULL_RTX,
                                                 VOIDmode, 0),
                                    src_p,
                                    1, OPTAB_LIB_WIDEN);
            lo1 = lo_r;
          }

        if (TREE_INT_CST_LOW (hi1) > TREE_INT_CST_LOW (hi_r))
          hi1 = hi_r;
        break;

      /* Target is big enough to hold this, don't adjust hi bound */
      case BIT_IOR_EXPR:        /* op1 + op2 */
        if (TREE_INT_CST_LOW (lo1) > TREE_INT_CST_LOW (lo_r))
          {
            if (GET_CODE (set1) != MEM)
              abort ();

            temp = setop_vector_adjust (lo1, lo_r);

            if (! integer_zerop (temp))
              src_p = expand_binop (Pmode, add_optab, src_p,
                                    expand_expr (temp,
                                                 NULL_RTX,
                                                 VOIDmode, 0),
                                    src_p,
                                    1, OPTAB_LIB_WIDEN);
            lo1 = lo_r;
          }
        break;

      /* Target is big enough to hold this */
      case BIT_XOR_EXPR:        /* op1 >< op2 */
        if (TREE_INT_CST_LOW (lo1) > TREE_INT_CST_LOW (lo_r))
          {
            if (GET_CODE (set1) != MEM)
              abort ();

            temp = setop_vector_adjust (lo1, lo_r);

            if (! integer_zerop (temp))
              src_p = expand_binop (Pmode, add_optab, src_p,
                                    expand_expr (temp,
                                                 NULL_RTX,
                                                 VOIDmode, 0),
                                    src_p,
                                    1, OPTAB_LIB_WIDEN);
            lo1 = lo_r;
          }
        break;

      default:
        /* NOTHING */ ;
      };

      if (TREE_INT_CST_LOW (lo1) != TREE_INT_CST_LOW (lo_r))
        {
          if (GET_CODE (target) != MEM)
            abort ();
          temp = setop_word (lo1, lo_r);

          if (! integer_zerop (temp))
            dest_p = memory_address (word_mode,
                                     expand_binop (Pmode, add_optab, dest_p,
                                                   expand_expr (temp,
                                                                NULL_RTX,
                                                                VOIDmode, 0),
                                                   dest_p,
                                                   1, OPTAB_LIB_WIDEN));
        }

      /* Begin binop binary op generation, possibly loop it */

      if (! onex)
        {
          emit_move_insn (count,
                          expand_expr (setop_num_words (lo1, hi1),
                                       NULL_RTX, VOIDmode, 0));

          emit_queue ();

          start_label = gen_label_rtx ();
          end_label = gen_label_rtx ();

          emit_note (NULL_PTR, NOTE_INSN_LOOP_BEG);
          emit_note (NULL_PTR, NOTE_INSN_LOOP_CONT);
          emit_label (start_label);

          /* Exit loop if count is zero */
          emit_cmp_insn (count, const0_rtx, EQ, NULL_RTX,
                         GET_MODE (count), 0, 0);
          emit_jump_insn (gen_beq (end_label));
        }

      if (code == BIT_ANDTC_EXPR)
        src = expand_unop (GET_MODE (src), one_cmpl_optab, src, NULL_RTX, 1);

      /* loop through BLKmode set one word at the time */
      ntarget = expand_binop (word_mode, binoptab,
                              dest, src,
                              dest, 1, OPTAB_LIB_WIDEN);
      if (ntarget != dest)
        emit_move_insn (dest, ntarget);

      if (! onex)
        {
          expand_dec (count, const1_rtx);
          NEXT_WORD (src_p);
          NEXT_WORD (dest_p);

          emit_jump (start_label);
          emit_note (NULL_PTR, NOTE_INSN_LOOP_END);
          emit_label (end_label);
        }

      if (code == BIT_AND_EXPR)
        target = setop_runtime (CLEAR_OUTSIDE, target, lo_r, hi_r,
                                lo1, hi1, NULL_RTX, 0);

      emit_queue ();
    }
  return target;
}

/* @@@ FIX THIS:
   - comparison of two different word size segments has to be checked.
     (lo0 != lo1)
 */
rtx
expand_set_comparison (exp, target)
     tree exp;
     rtx target;
{
  enum tree_code code = TREE_CODE (exp);
  tree set0_t = TREE_OPERAND (exp, 0);
  tree set1_t = TREE_OPERAND (exp, 1);

  tree  lo0;
  tree  hi0;
  tree  lo1;
  tree  hi1;
  rtx  set0;
  rtx  set1;
  tree temp;
  int do_invert = FALSE;

  if (TREE_CODE (TREE_TYPE (set0_t)) != SET_TYPE
      || TREE_CODE (TREE_TYPE (set1_t)) != SET_TYPE)
    abort ();

  /* LT_EXPR and GT_EXPR are only generated as a result of
   * optimization of:
   *
   * LT_EXPR == NOT (set1 >= set2) optimized to (set1 < set2)
   * GT_EXPR == NOT (set1 <= set2) optimized to (set1 > set2)
   */
  switch (code) {
  case GT_EXPR:
    do_invert = TRUE;
    code = LE_EXPR;
    break;

  case LT_EXPR:
    do_invert = TRUE;
    /* FALLTHROUGH */

  case GE_EXPR:
    /* Swap sets, handle as LE_EXPR (set inclusion operator) */
    temp = set0_t;
    set0_t = set1_t;
    set1_t = temp;
    code = LE_EXPR;
    break;

  case NE_EXPR:
    do_invert = TRUE;
    code = EQ_EXPR;
    break;

  case LE_EXPR:
  case EQ_EXPR:
    break;

  default:
    abort ();
  }

  lo0 = TYPE_MIN_VALUE (TYPE_DOMAIN (TREE_TYPE (set0_t)));
  hi0 = TYPE_MAX_VALUE (TYPE_DOMAIN (TREE_TYPE (set0_t)));
  lo1 = TYPE_MIN_VALUE (TYPE_DOMAIN (TREE_TYPE (set1_t)));
  hi1 = TYPE_MAX_VALUE (TYPE_DOMAIN (TREE_TYPE (set1_t)));

  /* Take care of empty set constructors by changing the
   * type of the constructor to be the same as the
   * non-empty set constructor.
   *
   * Constant folding must have handled the case where both sets
   * are special empty set nodes.
   *
   */
  if (TREE_CODE (set0_t) == CONSTRUCTOR || TREE_CODE (set1_t) == CONSTRUCTOR)
    {
      if (TREE_CODE (TREE_TYPE (TREE_TYPE (set0_t))) == VOID_TYPE)
        {
          if (TREE_CODE (TREE_TYPE (TREE_TYPE (set1_t))) == VOID_TYPE)
            abort ();   /* Should have been folded */
          else
            TREE_TYPE (set0_t) = TREE_TYPE (set1_t);
        }
      else if (TREE_CODE (TREE_TYPE (TREE_TYPE (set1_t))) == VOID_TYPE)
        TREE_TYPE (set1_t) = TREE_TYPE (set0_t);
    }

  set0 = expand_expr (set0_t, NULL_RTX, VOIDmode, 0);
  set1 = expand_expr (set1_t, NULL_RTX, VOIDmode, 0);

  if (! target)
    target = gen_reg_rtx (TYPE_MODE (TREE_TYPE (exp)));

  if (setop_empty (set0, lo0, hi0) && setop_empty (set1, lo1, hi1))
    return const1_rtx;

  if (setop_empty (set0, lo0, hi0))
    return setop_runtime (CHECK_INSIDE, set1, lo1, hi1, lo1, hi1, target, do_invert);

  if (setop_empty (set1, lo1, hi1))
    return setop_runtime (CHECK_INSIDE, set0, lo0, hi0, lo0, hi0, target, do_invert);

  if (TREE_CODE (lo0) != INTEGER_CST || TREE_CODE (hi0) != INTEGER_CST
      || TREE_CODE (lo1) != INTEGER_CST || TREE_CODE (hi1) != INTEGER_CST)
    abort ();

  if (code == EQ_EXPR)
    {
      rtx success = gen_label_rtx ();

      if (GET_MODE (set0) == word_mode
          && GET_MODE (set1) == word_mode
          && TREE_INT_CST_LOW (lo0) == TREE_INT_CST_LOW (lo1))
        {
          emit_queue ();
          emit_move_insn (target, do_invert ? const0_rtx : const1_rtx);
          emit_cmp_insn (set0, set1, EQ, NULL_RTX, word_mode, 1, 0);
          emit_jump_insn (gen_beq (success));
          emit_move_insn (target, do_invert ? const1_rtx : const0_rtx);
          emit_label (success);
          emit_queue ();
          return target;
        }

      /* Either one/both set is BLKmode or the lower bound does not match */
      {
        rtx set0_p = gen_reg_rtx (Pmode);
        rtx set1_p = gen_reg_rtx (Pmode);
        rtx start = gen_label_rtx ();
        rtx end_label = gen_label_rtx ();
        rtx failed = gen_label_rtx ();
        rtx index = gen_reg_rtx (word_mode);
        int onex = 0;
        rtx word0;
        rtx word1;
        tree count;
        int smaller;

        emit_queue ();

        if (GET_MODE (set0) != BLKmode)
          {
            word0 = set0;
            onex++;
          }
        else
          {
            /* make word0 represent the first word of set0 */
            emit_move_insn (set0_p,
                            memory_address (word_mode, XEXP (set0, 0)));
            word0 = gen_rtx (MEM, word_mode, set0_p);
          }

        if (GET_MODE (set1) != BLKmode)
          {
            word1 = set1;
            onex++;
          }
        else
          {
            /* make word1 represent the first word of set1 */
            emit_move_insn (set1_p,
                            memory_address (word_mode, XEXP (set1, 0)));
            word1 = gen_rtx (MEM, word_mode, set1_p);
          }

        if (TREE_INT_CST_LOW (setop_rlo (lo0))
            < TREE_INT_CST_LOW (setop_rlo (lo1)))
          {
            /* @@@ set1 fits inside set0. FALSE ASSUMPTION! HI BOUNDS! */
            smaller = 1;
            if (GET_MODE (set0) == BLKmode)
              set0_p = expand_binop (Pmode, add_optab, set0_p,
                                     expand_expr (setop_word (lo1, lo0),
                                                  NULL_RTX, VOIDmode, 0),
                                     set0_p, 0, OPTAB_LIB_WIDEN);

            if (TREE_INT_CST_LOW (setop_rlo (hi0))
                >= TREE_INT_CST_LOW (setop_rlo (hi1)))
              count = setop_num_words (lo1, hi1);
            else
              count = fold (build
                            (TRUNC_DIV_EXPR, integer_type_node,
                             fold (build (MINUS_EXPR, integer_type_node,
                                          fold (build (PLUS_EXPR,
                                                       integer_type_node,
                                                       hi0,
                                                       WORD_IN_BITS)),
                                          setop_rlo (lo1))),
                             WORD_IN_BITS));
          }
        else
          {
            /* @@@ set0 fits inside set1 FALSE ASSUMPTION, HIBOUNDS!!! */
            smaller = 0;
            /* @@ does this also need SImode int? */
            if (GET_MODE (set1) == BLKmode)
              set1_p = expand_binop (Pmode, add_optab, set1_p,
                                     expand_expr (setop_word (lo0, lo1),
                                                  NULL_RTX, VOIDmode, 0),
                                     set1_p, 0, OPTAB_LIB_WIDEN);

            if (TREE_INT_CST_LOW (hi1) >= TREE_INT_CST_LOW (hi0))
              count = setop_num_words (lo0, hi0);
            else
              count = fold (build
                            (TRUNC_DIV_EXPR, integer_type_node,
                             fold (build (MINUS_EXPR, integer_type_node,
                                          fold (build (PLUS_EXPR,
                                                       integer_type_node,
                                                       hi1,
                                                       WORD_IN_BITS)),
                                          setop_rlo (lo0))),
                             WORD_IN_BITS));
          }

        /* Expect sets to be equal */
        emit_move_insn (target, do_invert ? const0_rtx : const1_rtx);
        if (! onex)
          {
            /* Word counter */
            emit_move_insn (index,
                            expand_expr (count,
                                         NULL_RTX, VOIDmode, 0));

            emit_note (NULL_PTR, NOTE_INSN_LOOP_BEG);
            emit_note (NULL_PTR, NOTE_INSN_LOOP_CONT);
            emit_label (start);

            /* Loop if count not zero */
            emit_cmp_insn (index, const0_rtx, EQ, NULL_RTX,
                           GET_MODE (index), 0, 0);
            emit_jump_insn (gen_beq (end_label));
          }

        emit_cmp_insn (word0, word1, NE, NULL_RTX,
                       GET_MODE (word0), 1, 0);

        /* this word is not equal -> jump */
        emit_jump_insn (gen_bne (failed));

        /* Advance to next word */
        if (! onex)
          {
            NEXT_WORD (set0_p);
            NEXT_WORD (set1_p);

            expand_dec (index, const1_rtx);

            emit_jump (start);
            emit_note (NULL_PTR, NOTE_INSN_LOOP_END);
            emit_label (end_label);
          }

        /* all compared words are now equal.
           Check if outside words are zero. */
        if (smaller == 0)
          {
            rtx tmp = set0; set0 = set1; set1 = tmp;

            temp = lo0; lo0 = lo1; lo1 = temp;
            temp = hi0; hi0 = hi1; hi1 = temp;
          }

        /* Set0 contains set1, so verify bits
         * lo0..(lo1-1) and (hi1+1)..hi0
         *
         * after setop_runtime(CHECK_OUTSIDE) target will contain the result
         * of the check if bits are zero outside the smaller set.
         */
        setop_runtime (CHECK_OUTSIDE,
                       set0, lo0, hi0, lo1, hi1, target, do_invert);
        emit_jump (success);
        emit_label (failed);
        emit_move_insn (target, do_invert ? const1_rtx : const0_rtx);
        emit_label (success);
        emit_queue ();
      }
      return target;
    }
  else  /* set0 <= set1 (LE_EXPR) */
    {

      /* Return const1_rtx if (set0 <= set1), otherwise const0_rtx
       * If do_invert is nonzero, the return value is inverted.
       */

      rtx failed = gen_label_rtx ();
      rtx tmp = NULL_RTX;
      rtx word0;
      rtx word1;

      emit_queue ();

      /* Default: 'set0 <= set1' is NOT TRUE */
      emit_move_insn (target, do_invert ? const1_rtx : const0_rtx);

      if (TREE_INT_CST_LOW (lo0) == TREE_INT_CST_LOW (lo1)
          && TREE_INT_CST_LOW (hi0) == TREE_INT_CST_LOW (hi1))
        {
          if (GET_MODE (set0) != BLKmode || GET_MODE (set1) != BLKmode)
            {
              /* `force_reg' cannot cope with BLKmode sets.  So we
                 perform the comparison by accessing the first word of
                 the set.  This should cause no problems because both
                 sets are of equal size - its just that one is contained
                 in a register and the other is in memory.  */
              if (GET_MODE (set0) != BLKmode)
                word0 = force_reg (GET_MODE (set0), set0);
              else
                {
                  rtx set0_p = gen_reg_rtx (Pmode);

                  emit_move_insn (set0_p, XEXP (set0, 0));
                  word0 = gen_rtx (MEM, word_mode, set0_p);
                }

              if (GET_MODE (set1) != BLKmode)
                word1 = force_reg (GET_MODE (set1), set1);
              else
                {
                  rtx set1_p = gen_reg_rtx (Pmode);

                  emit_move_insn (set1_p, XEXP (set1, 0));
                  word1 = gen_rtx (MEM, word_mode, set1_p);
                }

              /* *word0 & ~*word1 */
              tmp = expand_binop (GET_MODE (word0), and_optab, word0,
                                  expand_unop (GET_MODE (word1), one_cmpl_optab,
                                               word1, NULL_RTX, 1),
                                  NULL_RTX, 0, OPTAB_LIB_WIDEN);

              emit_cmp_insn (tmp, const0_rtx, NE, NULL_RTX, GET_MODE (tmp),
                             1, UNITS_PER_WORD);

              /* word does not satisfy inclusion rule if != 0 */
              emit_jump_insn (gen_bne (failed));
            }
          else /* Both are BLKmode.  */
            {
              rtx start = gen_label_rtx ();
              rtx end_label = gen_label_rtx ();
              rtx set0_p = gen_reg_rtx (Pmode);
              rtx set1_p = gen_reg_rtx (Pmode);
              rtx index = gen_reg_rtx (word_mode);

              /* Word counter */
              emit_move_insn (index,
                              expand_expr (setop_num_words (lo0, hi0),
                                           NULL_RTX, VOIDmode, 0));

              emit_move_insn (set0_p, XEXP (set0, 0));
              emit_move_insn (set1_p, XEXP (set1, 0));
              word0 = gen_rtx (MEM, word_mode, set0_p);
              word1 = gen_rtx (MEM, word_mode, set1_p);

              emit_note (NULL_PTR, NOTE_INSN_LOOP_BEG);
              emit_note (NULL_PTR, NOTE_INSN_LOOP_CONT);
              emit_label (start);

              /* Loop if count not zero */
              emit_cmp_insn (index, const0_rtx, EQ, NULL_RTX,
                             GET_MODE (index), 0, 0);
              emit_jump_insn (gen_beq (end_label));

              /* *word0 & ~*word1 */
              tmp = expand_binop (word_mode, and_optab, word0,
                                  expand_unop (word_mode, one_cmpl_optab,
                                                word1, NULL_RTX, 1),
                                   tmp, 0, OPTAB_LIB_WIDEN);

              emit_cmp_insn (tmp, const0_rtx, NE, NULL_RTX, GET_MODE (tmp),
                             1, UNITS_PER_WORD);

              /* word does not satisfy inclusion rule if != 0 */
              emit_jump_insn (gen_bne (failed));

              /* Advance to next word */
              NEXT_WORD (set0_p);
              NEXT_WORD (set1_p);

              expand_dec (index, const1_rtx);

              emit_jump (start);
              emit_note (NULL_PTR, NOTE_INSN_LOOP_END);
              emit_label (end_label);

              /* all compared words satisfy inclusion */
            }
        }
      else
        {
          /* @@@@@ INCOMPLETE! */
          /* i.e. set0 is different size than set1 *or* bounds differ */

          /* For the moment, solve it with tree nodes
           * by converting both sets to a common type.
           */

          abort ();
        }

      /* set0 <= set1 is TRUE */
      emit_move_insn (target, do_invert ? const0_rtx : const1_rtx);

      emit_label (failed);
      emit_queue ();
    }
  return target;
}

/* IN_EXPR: Inlined pascal set `in' expression.

   Algorithm:
    rlow      = set_low - (set_low%bits_per_word);
    the_word  = set [ (index - rlow)/bits_per_word ];
    bit_index = index % bits_per_word;
    return  (the_word >> bit_index) & 1;
*/

rtx
expand_set_in (exp, target)
     tree exp;
     rtx target;
{
  tree set = TREE_OPERAND (exp, 0);
  tree index = TREE_OPERAND (exp, 1);
  tree set_type = TREE_TYPE (set);
  int label_used = 0;

  tree set_low_bound = TYPE_MIN_VALUE (TYPE_DOMAIN (set_type));
  tree set_high_bound = TYPE_MAX_VALUE (TYPE_DOMAIN (set_type));
  tree shift_count;

  rtx index_val;
  rtx lo_r;
  rtx hi_r;
  rtx rlow;
  rtx diff, quo, addr, bit;
  rtx setval;
  enum machine_mode index_mode = TYPE_MODE (TREE_TYPE (index));
  rtx op0;
  rtx op1;

  if (target == 0)
    target = gen_reg_rtx (TYPE_MODE (TREE_TYPE (exp)));

  /* If domain is empty, answer is no.  */
  if (tree_int_cst_lt (set_high_bound, set_low_bound))
    return const0_rtx;

  index_val = expand_expr (index, 0, VOIDmode, 0);
  lo_r = expand_expr (set_low_bound, 0, VOIDmode, 0);
  hi_r = expand_expr (set_high_bound, 0, VOIDmode, 0);
  setval = expand_expr (set, 0, VOIDmode, 0);

  /* Compare index against bounds, if they are constant.  */
  if (GET_CODE (index_val) == CONST_INT
      && GET_CODE (lo_r) == CONST_INT)
    {
      if (INTVAL (index_val) < INTVAL (lo_r))
        return const0_rtx;
    }

  if (GET_CODE (index_val) == CONST_INT
      && GET_CODE (hi_r) == CONST_INT)
    {
      if (INTVAL (hi_r) < INTVAL (index_val))
        return const0_rtx;
    }

  /* If we get here, we have to generate the code for both cases
     (in range and out of range).  */

  op0 = gen_label_rtx ();
  op1 = gen_label_rtx ();

  if (! (GET_CODE (index_val) == CONST_INT
         && GET_CODE (lo_r) == CONST_INT))
    {
      emit_cmp_insn (index_val, lo_r, LT, 0, GET_MODE_2 (index_val, lo_r), 0, 0);
      emit_jump_insn (gen_blt (op1));
      label_used++;
    }

  if (! (GET_CODE (index_val) == CONST_INT
         && GET_CODE (hi_r) == CONST_INT))
    {
      emit_cmp_insn (index_val, hi_r, GT, 0, GET_MODE_2 (index_val, hi_r), 0, 0);
      emit_jump_insn (gen_bgt (op1));
      label_used++;
    }

  /* lo_r can be from 0..(BITS_PER_WORD-1).
   *
   * Calculate 'rlow', i.e. the index of bit 0 in the first
   * word of the set that can be in range (-BITS_PER_WORD+1)..0
   * rlow is 0 iff lo_r % BITS_PER_WORD == 0
   */
  if (GET_CODE (lo_r) == CONST_INT)
    rlow = GEN_INT (INTVAL (lo_r) - INTVAL (lo_r) % BITS_PER_WORD);
  else
    rlow = expand_expr
              (fold (build (MINUS_EXPR, integer_type_node,
                            set_low_bound,
                            fold (build (TRUNC_MOD_EXPR,
                                         integer_type_node,
                                         set_low_bound,
                                         WORD_IN_BITS)))),
               NULL_RTX, VOIDmode, 0);
  /* Here is the offset in bits from bit 0 of the first word of
   * the set to the bit we are interested in
   */
  diff = expand_binop (index_mode, sub_optab,
                       index_val, rlow, 0, 0, OPTAB_LIB_WIDEN);

  /* This is the WORD index from the first word of the set */
  quo = expand_divmod (0, TRUNC_DIV_EXPR, index_mode, diff,
                       GEN_INT (BITS_PER_WORD), 0, 0);

  /* This is the count to shift right to get the interesting bit
   * so that we can extract the bit by anding with 1.
   */
  shift_count = fold (build (TRUNC_MOD_EXPR, integer_type_node,
                             index, WORD_IN_BITS));


  /* @@@ ??? I try to calculate word offset, but instead I
     get a byte offset (in 386)
   */

  quo = expand_binop (index_mode, smul_optab, quo,
                      GEN_INT (UNITS_PER_WORD), 0, 0, OPTAB_LIB_WIDEN);

  if (GET_CODE (setval) == MEM)
    {
      /* This is the address of the word containing THE BIT */
      addr = memory_address (word_mode,
                             expand_binop (word_mode, add_optab,
                                           quo, XEXP (setval, 0),
                                           NULL_RTX,
                                           1, OPTAB_LIB_WIDEN));

      /* Shift right so that the bit comes to rightmost bit */
      bit = expand_shift (RSHIFT_EXPR, word_mode,
                          gen_rtx (MEM, word_mode, addr),
                          shift_count, 0, 1);

    }
  else
    {
      /* Set in a register, just shift it right to get the bit we want */
      bit = expand_shift (RSHIFT_EXPR, word_mode,
                          setval, shift_count, 0, 1);
    }

  /* Extract the result by anding the word with 1 */
  target = expand_binop (GET_MODE (target),
                         and_optab, bit, const1_rtx, target,
                         1, OPTAB_LIB_WIDEN);

  if (label_used)
    {
      /* Output the code to handle the out-of-range case.  */
      emit_jump (op0);
      emit_label (op1);
      emit_move_insn (target, const0_rtx);
      emit_label (op0);
    }
  return target;
}

/* Constructor elts comes here divided as two parts:
   - constant part that is a precalculated list of words
   - variable part that is a list of single elements and
     ranges for which one or both bounds vary.

   EXP is a CONSTRUCTOR node whose type is a SET_TYPE node of the
   set we are constructing.

   Call save_expr() for set elements/ranges to avoid computing
   their values multiple times.
 */

  /* For now: no constant part of precalculated list of words exist */

void
store_set_constructor (exp, target)
     tree exp;
     rtx target;
{
  /* enum tree_code code = TREE_CODE (exp); */
  tree elt   = CONSTRUCTOR_ELTS (exp);
  tree type  = TREE_TYPE (exp);

  tree lo0 = TYPE_MIN_VALUE (TYPE_DOMAIN (type));
  tree hi0 = TYPE_MAX_VALUE (TYPE_DOMAIN (type));

  /* Calculate bit zero element number */
  /* tree rlow = setop_rlo (lo0); */

  /* Pointer to the correct word */
  rtx ptr  = gen_reg_rtx (Pmode);

  /* Use this to access the word we put the current constructor values in */
  rtx word;

  if (! target)
    abort ();

  clear_storage (target, expr_size (exp), TYPE_ALIGN (type) / BITS_PER_UNIT);

  /* No need to do anything else for empty set. */
  if (!elt)
    return;

  if (GET_MODE (target) == BLKmode)
    {
      word = gen_rtx (MEM, word_mode, ptr);
      emit_move_insn (ptr, XEXP (target, 0));
    }
  else
    word = target;

  for (; elt; elt = TREE_CHAIN (elt))
    {
      tree p = TREE_PURPOSE (elt);
      tree v = TREE_VALUE   (elt);
      /* Avoid evaluating the set element more than once */
      tree saved_p = save_expr (p);
      rtx dest;


      if (GET_MODE (target) == BLKmode)
        {
          /* Pointer to word that contains the element */

          ptr = expand_binop
                  (Pmode, add_optab,
                   XEXP (target, 0),
                   expand_expr (setop_word (convert (integer_type_node, saved_p), lo0),
                                NULL_RTX, VOIDmode, 0),
                   ptr, 1, OPTAB_LIB_WIDEN);
        }

      /* Set a single member or X..X */
      /* @@@ Missing checks (element inside set bounds)? */
      if (v == NULL_TREE || v == p)
        {
          rtx mask =
            expand_expr (fold (build (LSHIFT_EXPR, long_integer_type_node,
                                      size_one_node,
                                      fold (build (TRUNC_MOD_EXPR,
                                                   integer_type_node,
                                                   convert (integer_type_node,
                                                            saved_p),
                                                   WORD_IN_BITS)))),
                         NULL_RTX, VOIDmode, 0);

          dest = expand_binop (word_mode, ior_optab, word, mask,
                               word, 1, OPTAB_LIB_WIDEN);
          if (dest != word)
            emit_move_insn (word, dest);
        }
      else
        {
          rtx result = setop_runtime (SET_INSIDE, target, lo0, hi0,
                                      saved_p, save_expr (v), NULL_RTX, 0);
          if (result != target)
            abort ();           /* @@ fix */
        }
    }
}
