/* Type construction and checking for GNU Pascal.
   Copyright (C) 1987-89, 1995-2000 Free Software Foundation, Inc.

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

*/

/*
 * Authors: Jukka Virtanen <jtv@hut.fi>
 *          Peter Gerwinski <peter@gerwinski.de>
 */

#include "gcc-version.h"
#include "config.h"
#include "gansidecl.h"
#ifdef EGCS
#include "system.h"
#endif

#include <stdio.h>

#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif

#include "gbe.h"
#include "gpc-defs.h"
#include "parse.h"
#include "objects.h"
#include "rts.h"
#include "util.h"
#include "types.h"


/* Declare functions with prototypes. */

void clear_set PROTO ((tree));
static tree build_array_slice_ref PROTO ((tree, tree));
static int guess_precision PROTO ((tree, tree));


void cstring_inform ()
{
  static int cstring_informed = 0;
  if (! cstring_informed)
    {
      error (" (Use `{$x+}' or `--extended-syntax' to treat `CString'");
      error (" as a string.)");
      cstring_informed++;
    }
}

void ptrarith_inform ()
{
  static int ptrarith_informed = 0;
  if (! ptrarith_informed)
    {
      error (" (Use `{$x+}' or `--extended-syntax' to enable");
      error (" pointer arithmetic.)");
      ptrarith_informed++;
    }
}

/*
 * Sets.
 */

/* Clear the target set.
 */
void
clear_set (the_set)
     tree the_set;
{
  tree type  = TREE_TYPE (the_set);
  rtx target = expand_expr (the_set,
                            NULL_RTX,
                            TYPE_MODE (type),
                            0);
  clear_storage (target, expr_size (the_set),
                 TYPE_ALIGN (type) / BITS_PER_UNIT);
}

/* Construct one set member from the LOWER and UPPER as range bounds.
 * If UPPER is NULL_TREE, then the member is not a closed interval,
 * but rather a single member.
 *
 * Returns a TREE_LIST node with TREE_PURPOSE as the lower bound and
 * TREE_VALUE as the upper bound of each set member.
 */

tree
construct_set_member (lower, upper)
     tree lower, upper;
{
  tree t1;

  if (lower == error_mark_node || upper == error_mark_node)
    return build_tree_list (NULL_TREE, NULL_TREE);

  lower = string_may_be_char (lower);
  upper = string_may_be_char (upper);

  t1 = TREE_TYPE (lower);

  if (! ORDINAL_TYPE (TREE_CODE (t1)))
    {
      error ("set constructor elements must be of ordinal type");
      return build_tree_list (NULL_TREE, NULL_TREE);
    }

  if (upper == NULL_TREE)
    return build_tree_list (lower, upper);
  else
    {
      if (TREE_CODE (t1) != TREE_CODE (TREE_TYPE (upper)))
        {
          error ("set range upper and lower bound are of incompatible type");
          return build_tree_list (NULL_TREE, NULL_TREE);
        }
      if (TREE_CODE (lower) == INTEGER_CST &&
          TREE_CODE (upper) == INTEGER_CST &&
          tree_int_cst_lt (upper, lower))
        {
          warning ("specified set member range is empty");
          return NULL_TREE;
        }
    }
  return build_tree_list (lower, upper);
}

/* Build a constructor node for set elements.
 * This is later converted to a set with construct_set ()
 */
tree
build_set_constructor (members)
     tree members;
{
  tree node;
  tree m;
  int is_constant = 1;

  if (members == error_mark_node)
    return error_mark_node;

  for (m = members; m ; m = TREE_CHAIN(m))
    {
      if (!TREE_PURPOSE (m) && !TREE_VALUE (m))
        return error_mark_node;                 /* Error in elements. */
      if (! TREE_VALUE (m))
        TREE_VALUE (m) = TREE_PURPOSE (m);   /* Back-end needs it like this. */
      if (! TREE_CONSTANT (TREE_VALUE (m))
          || ! TREE_CONSTANT (TREE_PURPOSE (m)))
        is_constant = 0;
    }

  node = make_node (CONSTRUCTOR);

  /* Mark it as a SET_TYPE constructor.
   * The correct type is not known yet, so this is as good as any other.
   */
  TREE_TYPE (node) =
    build_set_type (build_tree_list (void_type_node,
                                     INT_RANGE_TYPE (integer_zero_node, -1)));

  CONSTRUCTOR_ELTS (node) = members;
  TREE_CONSTANT (node) = is_constant;
  TREE_STATIC (node) = is_constant;
  TREE_ADDRESSABLE (node) = 1;
  return node;
}

/* Build a SET_TYPE node from a set constructor.
 *
 * CONSTRUCTOR is a CONSTRUCTOR type node whose CONSTRUCTOR_ELTS
 * contains a TREE_LIST of the elements(/pairs) of the set.
 *
 * If ARG_TYPE == 0, TARGET_OR_TYPE is a VAR_DECL node where we should
 *      construct our set.
 * If ARG_TYPE == 1, TARGET_OR_TYPE is a SET_TYPE node which we should
 *      model our new set after.
 * If ARG_TYPE == 2, TARGET_OR_TYPE is a SET_TYPE node passed as a
 *      parameter to a function. (Special case for empty set constructor
 *      passing)
 *
 * TARGET_OR_TYPE is NULL_TREE if we don't know the destination
 * where we should put the constructed set, nor the type we should
 * be cloning to our constructed set.
 *
 * Return NULL_TREE if we assigned the set to the existing
 * TARGET_SET, else return the constructor whose TREE_TYPE
 * type we now set.
 */
tree
construct_set (constructor, target_or_type, arg_type)
     tree constructor;
     tree target_or_type;
     int  arg_type;
{
  tree elements;
  tree elem;
  tree set_low, set_high;
  int set_low_value, set_high_value;
  tree basetype;

  tree this_set_type = NULL;

  if (constructor == error_mark_node)
    return error_mark_node;

  elements = CONSTRUCTOR_ELTS (constructor);

  if (! elements && arg_type != 2)
    {
      if (arg_type == 0 && target_or_type)  /* Clear storage in the target */
        {
          clear_set (target_or_type);
          return NULL_TREE;
        }
      else
        return constructor;
    }
  if (arg_type == 0 && target_or_type)
    {
      tree domain;
      this_set_type = TREE_TYPE (target_or_type);
      if (TYPE_DOMAIN (this_set_type))
        domain = TYPE_DOMAIN (this_set_type);
      else
        domain = this_set_type;
      set_low = TYPE_MIN_VALUE (domain);
      set_high = TYPE_MAX_VALUE (domain);
    }
  else
    {
      int  set_is_ready = 0;
      tree type;
      enum tree_code tcode;

      if (elements)
        type = base_type (TREE_TYPE (TREE_PURPOSE (elements)));
      else if (! target_or_type)
        /* Empty set constructor to unknown set? */
        abort ();
      else
        type = TREE_TYPE (target_or_type);

      tcode = TREE_CODE (type);

      if (target_or_type)
        {
          tree domain;
          this_set_type = target_or_type;
          if (TYPE_DOMAIN (this_set_type))
            domain = TYPE_DOMAIN (this_set_type);
          else
            domain = this_set_type;
          set_low = TYPE_MIN_VALUE (domain);
          set_high = TYPE_MAX_VALUE (domain);
          set_is_ready = 1;
        }
      else if (tcode == INTEGER_TYPE && ! TREE_TYPE (type))
        {
          /* avoids [ -maxint .. maxint ] storage request :-) */
          tree min = NULL_TREE;
          tree max = NULL_TREE;
          int varies = 0;

          /* Scan for the min and max values */
          for (elem = elements; elem; elem = TREE_CHAIN (elem))
            {
              tree min_c = TREE_PURPOSE (elem);
              tree max_c = TREE_VALUE (elem);

              STRIP_NOPS (min_c);
              STRIP_NOPS (max_c);

              /* If this is a single element, not a range.
               * @@@ Should not happen - handled in `build_set_constructor()'.
               */
              if (! max_c)
                abort ();

              if (TREE_CODE (min_c) != INTEGER_CST)
                {
                  /* Non-constant lower bound.  Try to rescue it by looking
                   * whether its type has reasonable bounds.
                   */
                  min_c = TYPE_MIN_VALUE (TREE_TYPE (min_c));
                }

              if (TREE_CODE (min_c) != INTEGER_CST)
                varies++;
              else
                if (!min || tree_int_cst_lt (min_c, min))
                  min = min_c;

              if (TREE_CODE (max_c) != INTEGER_CST)
                max_c = TYPE_MAX_VALUE (TREE_TYPE (max_c));

              if (TREE_CODE (max_c) != INTEGER_CST)
                varies++;
              else
                if (!max || tree_int_cst_lt (max, max_c))
                  max = max_c;
            }

          /* @@@ We do not yet check if the type of the variable part
           * is a subrange whose allowed ranges would
           * change the calculated set bounds, or prevent
           * the warnings since it could not be outside bounds.
           */

          if (! varies)  /* Constant low bound */
            {
              tree range = NULL_TREE;

              set_low = min;
              set_high = max;

              if (min && max)
                range = (fold (build (PLUS_EXPR,
                                      integer_type_node,
                                      integer_one_node,
                                      fold (build (MINUS_EXPR,
                                            integer_type_node,
                                            max, min)))));

              /* If constant bounds, but too big, construct a limited set.
               */
              if (range && (TREE_OVERFLOW (range)
                            || int_cst_lt_always (integer_set_size, range)))
                range = NULL_TREE;

              if (! range)
                {
                  if (INT_CST_LT (set_low, integer_zero_node))
                    set_low = integer_zero_node;
                  set_high = build_int_2 (TREE_INT_CST_LOW (set_low)
                                          + TREE_INT_CST_LOW (integer_set_size)
                                          - 1, 0);
                  warning ("constructing limited integer set `%d..%d'",
                           TREE_INT_CST_LOW (set_low),
                           TREE_INT_CST_LOW (set_high));
                }
            }
          else
            {
              tree domain;
              this_set_type = integer_set_type_node;
              if (TYPE_DOMAIN (this_set_type))
                domain = TYPE_DOMAIN (this_set_type);
              else
                domain = this_set_type;

              set_low = TYPE_MIN_VALUE (domain);
              set_high = TYPE_MAX_VALUE (domain);
              set_is_ready = 1;
              warning ("constructing limited integer set `%d..%d'",
                       TREE_INT_CST_LOW (set_low),
                       TREE_INT_CST_LOW (set_high));
            }

          if (varies)
            warning ("limited integer set contains variable elements");
        }
      else
        {
          tree domain;
          if (tcode == INTEGER_TYPE) /* subrange type */
            domain = TREE_TYPE (type);
          else
            domain = type;
          /* domain = TYPE_DOMAIN (domain); */
          set_low = TYPE_MIN_VALUE (domain);
          set_high = TYPE_MAX_VALUE (domain);
        }

      if (! set_is_ready)
        this_set_type =
          build_set_type
            (build_tree_list
             (type,
              build_range_type (TREE_TYPE (set_low),
                                set_low, set_high)));

      if (int_size_in_bytes (type) == -1)
        abort ();   /* non-constant bounds not yet supported (Why???) */
    }

  /* Now we know the type of the target set, so we switch the constructor
   * type to be the correct type
   */
  TREE_TYPE (constructor) = this_set_type;

  basetype = TREE_TYPE (this_set_type);

  /* Check for ordinal subranges */
  if (TREE_CODE (basetype) == INTEGER_TYPE
      && TREE_TYPE (basetype)
      && TREE_CODE (TREE_TYPE (basetype)) != INTEGER_TYPE)
    basetype = TREE_TYPE (basetype);

  /* Check that the constructor elements are of valid type
   * and within the allowed range (if constant)
   */

  if (set_low && TREE_CODE (set_low) == INTEGER_CST)
    set_low_value = TREE_INT_CST_LOW (set_low);
  else
    set_low_value = - 0x7FFFFFFF;

  if (set_high && TREE_CODE (set_high) == INTEGER_CST)
    set_high_value = TREE_INT_CST_LOW (set_high);
  else
    set_high_value = 0x7FFFFFFF;

  for (elem = elements; elem; elem = TREE_CHAIN (elem))
    {
      tree p = TREE_PURPOSE (elem);
      tree v = TREE_VALUE   (elem);
      enum tree_code pcode = TREE_CODE (TREE_TYPE (p));
      enum tree_code vcode;

      /* @@@ Must be more strict */
      if (v == NULL_TREE)
        abort ();
      if (v == p)
        {
          if (! (pcode == TREE_CODE (basetype)
                 || (pcode == INTEGER_TYPE
                     && TREE_TYPE (TREE_TYPE (p))
                     && TREE_CODE (TREE_TYPE (TREE_TYPE (p)))
                            == TREE_CODE (basetype))))
            error ("type of set constructor element must be the set basetype");
          if (TREE_CODE (p) == INTEGER_CST)
            {
              if (TREE_INT_CST_LOW (p) < set_low_value)
                error ("set constructor element too small for this set");
              if (TREE_INT_CST_LOW (p) > set_high_value)
                error ("set constructor element too large for this set");
            }
        }
      else
        {
          vcode = TREE_CODE (TREE_TYPE (v));
          if (! (pcode == TREE_CODE (basetype)
                 || (pcode == INTEGER_TYPE
                     && TREE_TYPE (TREE_TYPE (p))
                     && TREE_CODE (TREE_TYPE (TREE_TYPE (p)))
                        == TREE_CODE (basetype))))
            error ("type of set constructor range lower bound is not set basetype");
          if (! (vcode == TREE_CODE (basetype)
                 || (vcode == INTEGER_TYPE
                     && TREE_TYPE (TREE_TYPE (v))
                     && TREE_CODE (TREE_TYPE (TREE_TYPE (v)))
                        == TREE_CODE (basetype))))
            error ("type of set constructor range upper bound is not set basetype");
          if (TREE_CODE (p) == INTEGER_CST
              && TREE_INT_CST_LOW (p) < set_low_value)
            error ("set constructor range lower bound too small for this set");
          if (TREE_CODE (v) == INTEGER_CST
              && TREE_INT_CST_LOW (v) > set_high_value)
            error ("set constructor range upper bound too large for this set");
        }
    }

  return constructor;
}

tree
check_set_bounds (type, range)
     tree type, range;
{
  tree lo = TYPE_MIN_VALUE (range);
  tree hi = TYPE_MAX_VALUE (range);

  /* Check if it is a nonlimited integer type, like "set of integer" */
  if (TREE_CODE (type) == INTEGER_TYPE
      && !TREE_TYPE (type)               /* Allows huge subrange sets */
      && TREE_CODE (lo) == INTEGER_CST
      && TREE_CODE (hi) == INTEGER_CST
      && TREE_INT_CST_LOW (integer_set_size) < (TREE_INT_CST_LOW (hi)
                                                - TREE_INT_CST_LOW (lo) + 1))
    {
      warning ("integer set size limited to %d elements from low bound",
               TREE_INT_CST_LOW (integer_set_size));

      warning ("use -fsetlimit:NUMBER to change the limit at compile time");

      range = build_range_type
                (integer_type_node,
                 lo,
                 build_int_2 (TREE_INT_CST_LOW (lo) +
                              TREE_INT_CST_LOW (integer_set_size) - 1, 0));
    }

  return range;
}

/* (This should be in tree.c)
 *
 * Construct, lay out and return the `set of type' with the number of
 * elements specified by the range of values of MEMBERS.
 *
 * MEMBERS is a TREE_LIST node whose TREE_PURPOSE is the type of the
 * set and TREE_VALUE is the range of values it has.
 *
 * The SET_TYPE has the same fields as the ARRAY_TYPE.
 *
 * The TREE_TYPE of the SET_TYPE node is the type of the set members.
 * (If that is a subrange, then TREE_TYPE (TREE_TYPE (type)) is the
 * base type of the set)
 */
tree
build_set_type (members)
     tree members;
{
  tree d = TREE_VALUE (members);
  tree type;

  if (! ORDINAL_TYPE (TREE_CODE (TREE_PURPOSE (members)))
      && TREE_PURPOSE (members) != void_type_node)
    {
      error ("set base type must be an ordinal type");
      return integer_set_type_node;
    }

  type = make_node (SET_TYPE);
  TREE_TYPE (type) = TREE_PURPOSE (members);
  TYPE_DOMAIN (type) = d;

  /* @@ Hash set types here ...
   */

  if (TYPE_SIZE (type) == 0)
    layout_type (type);

  return type;
}

/* Assign SOURCE to TARGET.  Both must be of SET_TYPE.
 */
tree
assign_set (target, source)
     tree target, source;
{
  tree params;
  if (! comptypes (TREE_TYPE (source), TREE_TYPE (target)))
    error ("assignment between incompatible sets");

  if (TREE_CODE (TREE_TYPE (TREE_TYPE (source))) == VOID_TYPE)
    {
      /* empty set */
      params = build_tree_list (NULL_TREE, build_pascal_unary_op (ADDR_EXPR, target, 0));
      params = chainon (params,
                        build_tree_list (NULL_TREE,
                                         TYPE_MIN_VALUE (TYPE_DOMAIN (TREE_TYPE (target)))));
      params = chainon (params,
                        build_tree_list (NULL_TREE,
                                         TYPE_MAX_VALUE (TYPE_DOMAIN (TREE_TYPE (target)))));
      return rts_call (set_clear, void_type_node, NULL_TREE, params);
    }
  else
    {
      params = build_tree_list (NULL_TREE, build_pascal_unary_op (ADDR_EXPR, source, 0));
      params = chainon (params,
                        build_tree_list (NULL_TREE,
                                         TYPE_MIN_VALUE (TYPE_DOMAIN (TREE_TYPE (source)))));
      params = chainon (params,
                        build_tree_list (NULL_TREE,
                                         TYPE_MAX_VALUE (TYPE_DOMAIN (TREE_TYPE (source)))));
      params = chainon (params,
                        build_tree_list (NULL_TREE,
                                         build_pascal_unary_op (ADDR_EXPR, target, 0)));
      params = chainon (params,
                        build_tree_list (NULL_TREE,
                                         TYPE_MIN_VALUE (TYPE_DOMAIN (TREE_TYPE (target)))));
      params = chainon (params,
                        build_tree_list (NULL_TREE,
                                         TYPE_MAX_VALUE (TYPE_DOMAIN (TREE_TYPE (target)))));
      return rts_call (set_copy, void_type_node, NULL_TREE, params);
    }
}



/*
 * Strings.
 */

/* Assign a (char,string-type) to (char,string-type).
 * Each string may be either a fixed or variable length string.
 *
 * @@@ No sophisticated checks are made for capacity violations.
 */
void
assign_string (target, source)
     tree target;
     tree source;
{
  tree length = NULL_TREE;
  tree t_type = base_type (TREE_TYPE (target));
  tree s_type = TREE_TYPE (source);

  switch (TREE_CODE (s_type))
    {
      case CHAR_TYPE:
        /* target must be a string-type since source is a char */
        expand_expr_stmt (build_modify_expr (build_array_ref (PASCAL_STRING_VALUE (target),
                                                              integer_one_node),
                                             NOP_EXPR,
                                             source));
        length = integer_one_node;
        break;

      case RECORD_TYPE:  /* String schema */
      case STRING_CST:
      case ARRAY_TYPE:
        if (is_string_type (source, 1)
            && (is_string_type (target, 1)
                || (TREE_CODE (t_type) == CHAR_TYPE)
                || ( /* @@@ flag_extended_syntax && */
                    TYPE_MAIN_VARIANT (t_type) == cstring_type_node)))
          {
            if (TREE_CODE (t_type) == CHAR_TYPE)
              {
                if ((flag_what_pascal & E_O_PASCAL) == 0)
                  warning ("assignment of string value to char variable");
                expand_expr_stmt (build_modify_expr
                                  (target,
                                   NOP_EXPR,
                                   (build_array_ref (PASCAL_STRING_VALUE (source),
                                   /* @@@@ Generalize for arbitrary lower bounds */
                                                     integer_one_node))));
              }
            else if (TYPE_MAIN_VARIANT (t_type) == cstring_type_node
                     && TREE_CODE (t_type))
              {
                tree addr;
                if (TREE_CODE (s_type) == RECORD_TYPE)  /* String schema */
                  source = append_null_character (source);
                allow_packed_var_parameters++;
                addr = convert (cstring_type_node,
                                build_pascal_unary_op (ADDR_EXPR,
                                                       PASCAL_STRING_VALUE (source),
                                                       0));
                allow_packed_var_parameters--;
                expand_expr_stmt (build_modify_expr (target, NOP_EXPR, addr));
              }
            else
              {
                /* Silently truncate the string if it does not fit in
                 * the target.
                 */
                tree capacity;
                if (is_variable_string_type (t_type))
                  capacity = PASCAL_STRING_CAPACITY (target);
                else if (TREE_CODE (t_type) == ARRAY_TYPE)
                  {
                    tree upper_bound = TYPE_MAX_VALUE (TYPE_DOMAIN (t_type));
                    tree lower_bound = TYPE_MIN_VALUE (TYPE_DOMAIN (t_type));
                    capacity = fold (build_binary_op (PLUS_EXPR,
                                         build_binary_op (MINUS_EXPR,
                                                          upper_bound,
                                                          lower_bound, 0),
                                         integer_one_node, 0));

                  }
                else
                  abort ();

                length = fold (build (MIN_EXPR, integer_type_node,
                                      PASCAL_STRING_LENGTH (source),
                                      capacity));

                /* The target needs to be an lvalue, but the
                 * source might be e.g. an array returned by a
                 * function or whatever.
                 *
                 * The conversions here are only made so that
                 * the string copy can copy the characters of
                 * possibly different MODE strings to each other
                 */
                allow_packed_var_parameters++;
                expand_expr_stmt (build_string_move (
                  build_unary_op (ADDR_EXPR, PASCAL_STRING_VALUE (target), 0),
                  build1 (ADDR_EXPR, string_type_node, PASCAL_STRING_VALUE (source)),
                  length));
                allow_packed_var_parameters--;
              }
            break;
          }

      default:
        error ("only Pascal string type, array of char, and char type");
        error (" are assignment compatible with a string");
#if 0  /* @@@ */
        if (! flag_extended_syntax
            && TYPE_MAIN_VARIANT (t_type) == cstring_type_node
            && is_string_type (source, 1))
          cstring_inform ();
#endif
        return;
    }

  if (is_variable_string_type (t_type))
    expand_expr_stmt (build_modify_expr (PASCAL_STRING_LENGTH (target),
                                         NOP_EXPR, length));
  else if (length)
    {
      tree upper_bound = TYPE_MAX_VALUE (TYPE_DOMAIN (t_type));
      tree lower_bound = TYPE_MIN_VALUE (TYPE_DOMAIN (t_type));
      tree t_length = fold (build_binary_op (PLUS_EXPR,
                                             build_binary_op (MINUS_EXPR,
                                                              upper_bound,
                                                              lower_bound, 0),
                                             integer_one_node, 0));

      if (TREE_CODE (length) != INTEGER_CST
          || TREE_CODE (t_length) != INTEGER_CST
          || ! tree_int_cst_equal (length, t_length))
        {
          /* Blank pad the fixed string target.
           */
          tree pad_target;
          allow_packed_var_parameters++;
          pad_target = build_array_ref (target,
                                        build_binary_op (PLUS_EXPR,
                                                         lower_bound,
                                                         length, 0));
          expand_expr_stmt (build_string_pad (
              build_unary_op (ADDR_EXPR, pad_target, 0),
              build_binary_op (MINUS_EXPR, t_length, length, 0),
              build_int_2 (' ', 0)));
          allow_packed_var_parameters--;
        }
    }
}

/* Append `chr ( 0 )' to the string VAL which must be of variable string
 * (schema) type.  This is always possible since the capacity of the string
 * is calculated to reserve place for the null character.
 */
tree
append_null_character (val)
     tree val;
{
  tree org_val = val, stmts = NULL_TREE, z, l;

  /* Dig out the real string, so stabilize_reference() does
   * not need to worry about NON_LVALUE_EXPRs and such.
   * Track COMPOUND_EXPRs on the way for later use.
   */
  while (TREE_CODE (val) == COMPOUND_EXPR
         || TREE_CODE (val) == NON_LVALUE_EXPR
         || TREE_CODE (val) == NOP_EXPR)
    if (TREE_CODE (val) == COMPOUND_EXPR)
      {
        if (stmts)
          stmts = build (COMPOUND_EXPR, void_type_node,
                         TREE_OPERAND (val, 0), stmts);
        else
          stmts = TREE_OPERAND (val, 0);
        val = TREE_OPERAND (val, 1);
      }
    else
      val = TREE_OPERAND (val, 0);
  if (lvalue_p (val))
    val = build_indirect_ref (stabilize_reference (build_unary_op (ADDR_EXPR, val, 0)), "CString parameter");
  else
    val = stabilize_reference (val);
  TREE_READONLY (val) = 0;  /* Avoid warning. */
  l = build_binary_op (PLUS_EXPR, PASCAL_STRING_LENGTH (val),
                       integer_one_node, 0);
  z = PASCAL_STRING_VALUE (val);
  z = build_array_ref (z, l);
  z = build_modify_expr (z, NOP_EXPR,
                         convert (char_type_node,
                                  integer_zero_node));
  if (stmts)
    z = build (COMPOUND_EXPR, void_type_node, stmts, z);
  val = build (COMPOUND_EXPR, TREE_TYPE (org_val), z, val);
  return val;
}

/* The lexer returns char constants as string constants
 * of length one.  Convert them to a chars here.
 */
tree
string_may_be_char (expr)
    tree expr;
{
  if (expr
      && TREE_CODE (expr) == STRING_CST
      && TREE_STRING_LENGTH (expr) <= 2)  /* including the trailing 0 */
    {
      char code;
      if (TREE_STRING_LENGTH (expr) == 2)
        code = TREE_STRING_POINTER (expr) [0];
      else
        {
          /* Assigning an empty string to a char.
           * Extended Pascal Standard says this is legal. (Autsch!)
           */
          if (!flag_what_pascal || (flag_what_pascal && E_O_PASCAL == 0))
            pedwarn ("assignment of empty string to a char yields a space");
          code = ' ';
        }
      /* Like in gpc-lex.c: If char type is signed, sign-extend the constant. */
      if (TREE_UNSIGNED (char_type_node)
          || ((code >> (BITS_PER_UNIT - 1)) & 1) == 0)
        expr = build_int_2(code & ((1 << BITS_PER_UNIT) - 1), 0);
      else
        expr = build_int_2(code | ((-1) << BITS_PER_UNIT), -1);
      TREE_TYPE (expr) = char_type_node;
    }
  return expr;
}

/* Each variable length string looks like:
 *
 * String = record
 *              Capacity : Integer;
 *              Length   : Integer;
 *              String   : packed array [ 1 .. Capacity + 1 ] of Char;
 *          end;
 *
 * (The "+ 1" is for appending a #0 character when passing this
 * as a `CString' parameter.)
 */
tree
build_pascal_string_schema (capacity)
     tree capacity;
{
#ifdef PG__NEW_STRINGS
  tree fields, string;
  tree placeholder, internal_capacity, internal_capacity_2, string_range;
  int mom = suspend_momentary ();

  string = start_struct (RECORD_TYPE, NULL_TREE);
  placeholder = build (PLACEHOLDER_EXPR, string);
  if (TREE_CODE (capacity) == INTEGER_CST
      && ! integer_zerop (capacity))
    internal_capacity = capacity;
  else
    internal_capacity = build (COMPONENT_REF, unsigned_type_node,
                               placeholder, error_mark_node);


  /* If the string index type is altered,
   * verify integer_ptr_type_node usage.
   */
  string_range = build_range_type (integer_type_node, integer_one_node,
                                   build_pascal_binary_op (PLUS_EXPR,
                                                           internal_capacity,
                                                           integer_one_node));
  resume_momentary (mom);

  fields = chainon (grokfield (input_filename, lineno,
                               get_identifier ("Capacity"),
                               build_tree_list (NULL_TREE,
                                                unsigned_type_node),
                               NULL_TREE),
                    chainon (grokfield (input_filename, lineno,
                                        get_identifier ("length"),
                                        build_tree_list (NULL_TREE,
                                                         unsigned_type_node),
                                        NULL_TREE),
                             grokfield (input_filename, lineno, string_id,
                                        build_tree_list (NULL_TREE,
                                                         grok_packed (build_array_type
                                                                      (char_type_node,
                                                                       string_range))),
                                        NULL_TREE)));

  string = finish_struct (string, fields, NULL_TREE);

  /* Flag this as a variable-string-type
   */
  TYPE_LANG_SPECIFIC (string) = allocate_type_lang_specific ();
  TYPE_LANG_CODE (string) = PASCAL_LANG_STRING;
  TYPE_LANG_BASE (string) = string_schema_proto_type;
  TYPE_LANG_DECLARED_CAPACITY (string) = capacity;

  if (TREE_CODE (internal_capacity) != INTEGER_CST)
    {
      internal_capacity_2 = build_component_ref (placeholder,
                                                 get_identifier ("Capacity"));
      TREE_OPERAND (internal_capacity, 1) = TREE_OPERAND (internal_capacity_2, 1);
    }

  TYPE_SIZE (string) = NULL_TREE;
  TYPE_SIZE_UNIT (string) = NULL_TREE;
  layout_type (string);

  return string;
#else
  tree string, fields, string_range;

  push_obstacks_nochange ();
  end_temporary_allocation ();

  /* If the string index type is altered,
   * verify integer_ptr_type_node usage.
   */
  string_range = build_range_type (integer_type_node,
                                   integer_one_node,
                                   build_pascal_binary_op (PLUS_EXPR,
                                                           capacity,
                                                           integer_one_node));

  fields = chainon (grokfield (input_filename, lineno,
                               get_identifier ("Capacity"),
                               build_tree_list (NULL_TREE,
                                                unsigned_type_node),
                               NULL_TREE),
                    chainon (grokfield (input_filename, lineno,
                                        get_identifier ("length"),
                                        build_tree_list (NULL_TREE,
                                                         unsigned_type_node),
                                        NULL_TREE),
                             grokfield (input_filename, lineno, string_id,
                                        build_tree_list (NULL_TREE,
                                                         grok_packed (build_array_type
                                                                      (char_type_node,
                                                                       string_range))),
                                        NULL_TREE)));

  string = finish_struct (start_struct (RECORD_TYPE, NULL_TREE),
                          fields, NULL_TREE);

  /* Flag this as a variable-string-type
   */
  TYPE_LANG_SPECIFIC (string) = allocate_type_lang_specific ();
  TYPE_LANG_CODE (string) = PASCAL_LANG_STRING;
  TYPE_LANG_BASE (string) = string_schema_proto_type;
  TYPE_LANG_DECLARED_CAPACITY (string) = capacity;

  pop_obstacks ();
  return string;
#endif
}

/*
 * Return 1 if the type of the node STRING is a character array node
 *  or it's a string constant, or it's a string schema.
 * Return 2 if it is a valid conformant array whose low bound should
 *  be checked at runtime.
 *
 * Return 0 if we know it's not a valid string.
 */
int
is_string_type (string, warn)
     tree string;
     int warn;
{
  if (TREE_CODE (string) == STRING_CST)
    return 1;

  return is_of_string_type (TREE_TYPE (string), warn);
}

int
is_of_string_type (type, warn)
     tree type;
     int warn;
{
  if (PASCAL_TYPE_STRING (type))
    return 1;

  if (TREE_CODE (type) != ARRAY_TYPE
      || TYPE_MAIN_VARIANT (TREE_TYPE (type)) != char_type_node)
    return 0;

  if (warn && ! PASCAL_TYPE_PACKED (type) && PEDANTIC (B_D_PASCAL))
    pedwarn ("ISO Pascal requires `packed' for fixed strings");

  /* @@@ We should verify at runtime that the low index of the
   * conformant array parameter is 1
   */
  if (C_TYPE_VARIABLE_SIZE (type))
    return 2;

  if (flag_what_pascal & ISO_PASCAL)
    {
      /* String type low index must be one */
      return tree_int_cst_equal (TYPE_MIN_VALUE (TYPE_DOMAIN (type)),
                                 integer_one_node);
    }
  else if (PEDANTIC (B_D_PASCAL)
           && ! tree_int_cst_equal (TYPE_MIN_VALUE (TYPE_DOMAIN (type)),
                                    integer_one_node))
    pedwarn ("ISO Pascal requires lower bound 1 for fixed strings");

  return 1;
}

int
declared_string_capacity (type)
     tree type;
{
  if (PASCAL_TYPE_STRING (type))
    {
#ifdef PG__NEW_STRINGS
      tree capacity = TYPE_LANG_DECLARED_CAPACITY (type);
#else
      tree capacity = NULL_TREE;
      tree string_field = simple_get_field (string_id, type,
                            "Internal GPC error: no data area in string");
      if (string_field
          && TREE_TYPE (string_field)
          && TYPE_DOMAIN (TREE_TYPE (string_field)))
        capacity = fold (build_pascal_binary_op (MINUS_EXPR,
                             TYPE_MAX_VALUE (TYPE_DOMAIN (TREE_TYPE (string_field))),
                             integer_one_node));
#endif
      if (capacity && TREE_CODE (capacity) == INTEGER_CST)
        return TREE_INT_CST_LOW (capacity);
    }
  abort ();
}

int
is_variable_string_type (type)
     tree type;
{
  return ((TREE_CODE (type) == REFERENCE_TYPE
           && PASCAL_TYPE_STRING (TREE_TYPE (type)))
          || PASCAL_TYPE_STRING (type));
}




/*
 * Schemata.
 */

/* Check whether the expression EXPR (to be used in type definitions)
 * contains a formal schema discriminant.  This is the case if and only
 * if EXPR contains a VAR_DECL with the PASCAL_TREE_DISCRIMINANT flag set.
 *
 * If no, return EXPR.  If yes, return a constant instead and add it
 * to the discriminant's fix-up list in the PASCAL_DECL_FIXUPLIST field
 * of the VAR_DECL.
 */
tree
maybe_schema_discriminant (expr)
     tree expr;
{
  if (TREE_CODE (expr) == VAR_DECL
      && PASCAL_TREE_DISCRIMINANT (expr))
    {
      tree new_expr = build1 (CONVERT_EXPR, TREE_TYPE (expr),
                              integer_zero_node);
      PASCAL_DECL_FIXUPLIST (expr) = chainon (PASCAL_DECL_FIXUPLIST (expr),
                                              build_tree_list (NULL_TREE,
                                                               new_expr));
      PASCAL_TREE_DISCRIMINANT (new_expr) = 1;
      expr = new_expr;
    }
  else
    {
      switch (TREE_CODE_CLASS (TREE_CODE (expr)))
        {
          case '3':
            TREE_OPERAND (expr, 2)
              = maybe_schema_discriminant (TREE_OPERAND (expr, 2));
            /* FALLTHROUGH */

          case '2':
          case '<':
            TREE_OPERAND (expr, 1)
              = maybe_schema_discriminant (TREE_OPERAND (expr, 1));
            /* FALLTHROUGH */

          case '1':
          case 'e':
            TREE_OPERAND (expr, 0)
              = maybe_schema_discriminant (TREE_OPERAND (expr, 0));
            /* FALLTHROUGH */

          default:
            /* do nothing */
            ;
        }
    }
  return expr;
}

/* Return a new schema type with formal discriminants DISCRIMINANTS
 * (a tree list holding VAR_DECL nodes) for the type template TYPE.
 *
 * The schema is a RECORD_TYPE with PASCAL_LANG_SCHEMA in its
 * TYPE_LANG_CODE slot, having the discriminants as fields plus
 * a `schema' field which contains the actual type.
 */
tree
build_schema_type (type, discriminants)
     tree type, discriminants;
{
  int mom = suspend_momentary ();
  tree fields = NULL_TREE, link, name;

  for (link = discriminants; link; link = TREE_CHAIN (link))
    {
      tree decl = TREE_VALUE (link);
      tree name = build_tree_list (NULL_TREE, DECL_NAME (decl));
      fields = chainon (fields, p_grokfields (name, TREE_TYPE (decl)));
    }
  name = build_tree_list (NULL_TREE, schema_id);
  fields = chainon (fields, p_grokfields (name, type));
  resume_momentary (mom);
  type = finish_struct (start_struct (RECORD_TYPE, NULL_TREE),
                        fields, NULL_TREE);

  /* Copy the fix-up list from the VAR_DECL's PASCAL_DECL_FIXUPLIST
   * to that of the FIELD_DECL.
   * Mark the discriminants as such.
   */
  for (link = discriminants, fields = TYPE_FIELDS (type);
       link && fields; link = TREE_CHAIN (link), fields = TREE_CHAIN (fields))
    {
      PASCAL_DECL_FIXUPLIST (fields) = PASCAL_DECL_FIXUPLIST (TREE_VALUE (link));
      PASCAL_TREE_DISCRIMINANT (fields) = 1;
    }
  TYPE_LANG_SPECIFIC (type) = allocate_type_lang_specific ();
  TYPE_LANG_CODE (type) = PASCAL_LANG_SCHEMA;

  /* Release the identifiers of the discriminants.
   * (This is similar to poplevel(), but only affects the discriminants.)
   */
  for (link = discriminants; link; link = TREE_CHAIN (link))
    {
      tree id = DECL_NAME (TREE_VALUE (link));
      IDENTIFIER_LOCAL_VALUE (id) = TREE_PURPOSE (link);
    }

  /* Done.  TYPE is not a valid type, but will continue to serve
   * as a template for discriminated types.
   */
  return type;
}

int
number_of_schema_discriminants (type)
     tree type;
{
  if (PASCAL_TYPE_UNDISCRIMINATED_STRING (type)
      || PASCAL_TYPE_PREDISCRIMINATED_STRING (type))
    return 1;

  if (PASCAL_TYPE_UNDISCRIMINATED_SCHEMA (type)
      || PASCAL_TYPE_PREDISCRIMINATED_SCHEMA (type))
    {
      int count = 0;
      tree field;
      if (TREE_CODE (type) != RECORD_TYPE)
        abort ();
      for (field = TYPE_FIELDS (type); field; field = TREE_CHAIN (field))
        if (PASCAL_TREE_DISCRIMINANT (field))
          count++;
      return count;
    }
  return 0;
}

/* Return 1 if EXPR is a schema and DISCR is a discriminant
 * of that schema, otherwise 0.
 */
int
is_schema_discriminant_access (expr, discr)
     tree expr, discr;
{
  if (TREE_CODE (expr) == COMPONENT_REF
      && PASCAL_TYPE_SCHEMA (TREE_TYPE (TREE_OPERAND (expr, 0))))
    {
      tree schema = TREE_TYPE (TREE_OPERAND (expr, 0)), field;
      for (field = TYPE_FIELDS (schema); field; field = TREE_CHAIN (field))
        if (DECL_NAME (field) == discr)
          return 1;
    }
  return 0;
}

/* Pre-discriminate an undiscriminated schema which is the type
 * of a pointer or a `Var' parameter, using its own contents.
 *
 * This makes the schema a valid type but preserves the fix-up information
 * needed to derive discriminated schemata from this schema.
 *
 * If OVERWRITE is zero, pointer/reference types to the undiscriminated
 * schema are preserved.
 */
void
prediscriminate_schema (decl, overwrite)
     tree decl;
     int overwrite;
{
  if ((TREE_CODE (TREE_TYPE (decl)) == POINTER_TYPE
       || TREE_CODE (TREE_TYPE (decl)) == REFERENCE_TYPE)
      /* This might be NULL_TREE while loading a GPI file. */
      && TREE_TYPE (TREE_TYPE (decl)))
    {
      if (PASCAL_TYPE_UNDISCRIMINATED_STRING (TREE_TYPE (TREE_TYPE (decl))))
        {
#ifdef PG__NEW_STRINGS
          return;
#else
          tree new_type, string_type, val;
          string_type = TREE_TYPE (TREE_TYPE (decl));
          val = build_component_ref (build_indirect_ref (decl,
                                                         "string access"),
                                     get_identifier ("Capacity"));
          size_volatile++;
          new_type = build_pascal_string_schema (val);
          size_volatile--;

          /* Preserve volatility and readonlyness.
           */
          TYPE_READONLY (new_type) = TYPE_READONLY (string_type);
          TYPE_VOLATILE (new_type) = TYPE_VOLATILE (string_type);

          /* Return the result, but don't spoil pointers/references
           * to "the" generic `String' type.
           */
          if (overwrite == 0)
            TREE_TYPE (decl) = build_type_copy (TREE_TYPE (decl));
          TREE_TYPE (TREE_TYPE (decl)) = new_type;
#endif
        }
      else if (PASCAL_TYPE_UNDISCRIMINATED_SCHEMA (TREE_TYPE (TREE_TYPE (decl))))
        {
          tree schema_type = TREE_TYPE (TREE_TYPE (decl)), new_type;
          tree field, new_field, discr = NULL_TREE;
          for (field = TYPE_FIELDS (schema_type);
               field && PASCAL_TREE_DISCRIMINANT (field);
               field = TREE_CHAIN (field))
            {
              discr = chainon (discr,
                        build_tree_list (NULL_TREE,
                          build_component_ref (build_indirect_ref (decl,
                                                 "schema access"),
                                               DECL_NAME (field))));
            }

          /* The size of this type may vary within one function body.
           */
          size_volatile++;
          new_type = build_discriminated_schema_type (TREE_TYPE (TREE_TYPE (decl)),
                                                      discr);
          size_volatile--;

          /* Re-activate the discriminant fields.
           * This makes the schema a valid template again.
           */
          for (field = TYPE_FIELDS (schema_type),
               new_field = TYPE_FIELDS (new_type);
               field && new_field && PASCAL_TREE_DISCRIMINANT (new_field);
               field = TREE_CHAIN (field),
               new_field = TREE_CHAIN (new_field))
            PASCAL_DECL_FIXUPLIST (new_field) = PASCAL_DECL_FIXUPLIST (field);

          /* Return the result, but don't spoil pointers/references
           * to this (undiscriminated) schema type.
           */
          if (overwrite == 0)
            TREE_TYPE (decl) = build_type_copy (TREE_TYPE (decl));
          TREE_TYPE (TREE_TYPE (decl)) = new_type;
        }
    }
}



/*
 * Other Pascal types.
 */

/* Returns the base type of an ordinal subrange, or the type
 * itself if it is not a subrange.
 */
tree
base_type (type)
     tree type;
{
  if (type == error_mark_node)
    return error_mark_node;

  /* Check for ordinal subranges.
   */
  if (ORDINAL_TYPE (TREE_CODE (type))
      && TREE_TYPE (type))
    type = TREE_TYPE (type);
  return TYPE_MAIN_VARIANT (type);
}

/* FILETYPE is the file component type.
 * If INDEX_TYPE == NULL_TREE, this is a normal sequential access file.
 * If INDEX_TYPE != NULL_TREE, then it is a TREE_LIST with:
 *
 *   TREE_PURPOSE: index type name or new index type (unused)
 *   TREE_VALUE:   domain of the index type
 */
tree
build_file_type (filetype, index_type)
     tree filetype;
     tree index_type;
{
    tree File = make_node (FILE_TYPE);

    /* This file is not a TEXT type file */
    TYPE_FILE_TEXT (File)  = NULL_TREE;
    TYPE_DOMAIN (File)     = NULL_TREE;

    /* For a direct access file type just store the indices */
    if (index_type)
      {
        if (TREE_VALUE (index_type) != error_mark_node)
          TYPE_DOMAIN (File) = TREE_VALUE (index_type);
        else
          error ("file treated as sequential access file");
      }
    TREE_TYPE (File) = filetype;

    layout_type (File);
    if (! TYPE_SIZE (File))
      TYPE_SIZE (File) = size_of_file_type;
#ifdef EGCS
    if (! TYPE_SIZE_UNIT (File))
      TYPE_SIZE_UNIT (File) = size_unit_of_file_type;
#endif /* EGCS */

    return File;
}

/* Information in variant field into tree list node:
 *
 * VARSEL:
 *  TREE_LIST(PURPOSE (PURPOSE(selector_id), VALUE(selector_type)),
 *            VALUE   (PURPOSE(file),        VALUE(line)))
 *
 *
 * FIELDLIST:
 *  TREE_LIST(PURPOSE   (case_list),
 *            VALUE     (field_decl_node))
 */
tree
build_record_variant_part (file, line, varsel, fieldlist)
     char *file;
     int line;
     tree varsel, fieldlist;
{
    tree ret, type;
    tree link, fields = NULL_TREE, constants = NULL_TREE;
    enum tree_code code;

    for (link = fieldlist; link; link = TREE_CHAIN (link))
      {
        tree c;
        for (c = TREE_PURPOSE (link); c; c = TREE_CHAIN (c))
          TREE_PURPOSE (c) = TREE_VALUE (link);
        fields = chainon (fields, TREE_VALUE (link));
        if (comptypes (TREE_TYPE (TREE_VALUE (TREE_PURPOSE (link))),
                       TREE_PURPOSE (varsel)) == 0)
          error ("type mismatch in variant selector constant");
        constants = chainon (constants, TREE_PURPOSE (link));
      }

    code = UNION_TYPE;
    for (link = fields; link; link = TREE_CHAIN (link))
      if (TREE_TYPE (link)
          && contains_file_or_schema_p (TREE_TYPE (link)))
        code = RECORD_TYPE;

    type = finish_struct (start_struct (code, NULL_TREE),
                          fields,
                          NULL_TREE);

    ret = grokfield (file, line,
                     NULL_TREE, /* field name */
                     build_tree_list (NULL_TREE, type),
                     NULL_TREE); /* field width */

    ret = build_tree_list (constants,  /* case constants */
                           ret);

    return ret;
}

/* Set the packed flag of the type, and re-calculate its size.
 *
 * `packed' packs all immediate ARRAY_TYPE components of TYPE.
 *
 * packed array [ 5..10, 1..20 ] of Char; is actually two arrays
 * which should both be packed.
 *
 * @@@ Hmm, maybe packed array [ 1..10 ] of array_type_name;
 * @@@ should not pack array_type_name???
 * @@@ If so, grok_packed requires info of the levels to pack
 * @@@ from the parser.  There is no way to find it out here.
 */
tree
grok_packed (type)
     tree type;
{
  type = pascal_type_variant (type, TYPE_QUALIFIER_PACKED, 1);
  if (TREE_CODE (type) == ARRAY_TYPE)
    {
      tree bits, new_size;
      tree domain = TYPE_DOMAIN (type);

      if (TREE_CODE (TREE_TYPE (type)) == ARRAY_TYPE)
        TREE_TYPE (type) = grok_packed (TREE_TYPE (type));

      if (! TYPE_MIN_VALUE (domain) || ! TYPE_MAX_VALUE (domain))
        domain = TREE_TYPE (domain);
      if (! domain || ! TYPE_MIN_VALUE (domain) || ! TYPE_MAX_VALUE (domain))
        abort ();

      /* layout_type() does not know about Pascal's thoughts about
       * packed arrays, so we re-calculate the size here.
       */
      if (ORDINAL_TYPE (TREE_CODE (TREE_TYPE (type))))
        bits = count_bits (TREE_TYPE (type));
      else
        bits = TYPE_SIZE (TREE_TYPE (type));

      /* Nothing to pack.
       */
      if (! bits
          || (TREE_CODE (bits) == INTEGER_CST
              && TREE_INT_CST_LOW (bits) % BITS_PER_UNIT == 0))
        return type;

      if (TREE_UNSIGNED (bits) != TREE_UNSIGNED (type))
        {
          TREE_TYPE (type) = build_type_copy (TREE_TYPE (type));
          TREE_UNSIGNED (TREE_TYPE (type)) = TREE_UNSIGNED (bits);
        }

      new_size = build_pascal_binary_op (MINUS_EXPR,
                     convert (integer_type_node, TYPE_MAX_VALUE (domain)),
                     convert (integer_type_node, TYPE_MIN_VALUE (domain)));
      new_size = build_pascal_binary_op (PLUS_EXPR, new_size, integer_one_node);
      new_size = build_pascal_binary_op (MULT_EXPR, new_size, bits);
      new_size = build_pascal_binary_op (CEIL_DIV_EXPR, new_size,
                     size_int (TYPE_PRECISION (packed_array_unsigned_long_type_node) / 2));
      new_size = build_pascal_binary_op (MULT_EXPR, new_size,
                     size_int (TYPE_PRECISION (packed_array_unsigned_long_type_node) / 2));
      TYPE_SIZE (type) = fold (new_size);
#ifdef EGCS
      TYPE_SIZE_UNIT (type) = fold (build_pascal_binary_op (CEIL_DIV_EXPR,
                                                            TYPE_SIZE (type),
                                                            size_int (BITS_PER_UNIT)));
#endif /* EGCS */
      TYPE_ALIGN (type) = TYPE_PRECISION (packed_array_unsigned_long_type_node) / 2;
    }
  else if (ORDINAL_TYPE (TREE_CODE (type)))
    {
      tree bits = count_bits (type);
      if (bits
          && TREE_INT_CST_LOW (bits)
             != TREE_INT_CST_LOW (TYPE_SIZE (type)))
        {
          TYPE_SIZE (type) = NULL_TREE;
#ifdef EGCS
          TYPE_SIZE_UNIT (type) = NULL_TREE;
#endif /* EGCS */
          TYPE_PRECISION (type) = TREE_INT_CST_LOW (bits);
          layout_type (type);
        }
    }
  return type;
}

/* Return 1 if VAL is a field of a packed array or record,
 * otherwise return 0.
 */
int
is_packed_field (val)
     tree val;
{
  if (TREE_CODE_CLASS (TREE_CODE (val)) == 'r'
      && (PASCAL_TREE_PACKED_ACCESS (val)
          || TREE_CODE (val) == BIT_FIELD_REF
          || (TREE_CODE (val) == COMPONENT_REF
              && DECL_BIT_FIELD (TREE_OPERAND (val, 1)))))
    return 1;
  else
    return 0;
}

/* Return a temporary variable of type TYPE holding the
 * value of the packed field VAL which will be used as
 * a `var' parameter instead of the packed field itself.
 *
 * @@ Currently, this is only used to avoid too verbose error messages.
 */
tree expand_packed_field (val, type)
     tree val, type;
{
  tree temp_val;
  if (pedantic || flag_what_pascal)
    {
      pedwarn ("ISO Pascal does not allow to pass packed fields");
      pedwarn (" as `var' parameters");
    }
  temp_val = make_new_variable ("packed_var_parm", type);
  expand_expr_stmt (build_modify_expr (temp_val, NOP_EXPR, val));
  PASCAL_TREE_PACKED_ACCESS (temp_val) = 1;

  /* Store the original location of the parameter,
   * so it can be stored back after the function
   * has been called.
   */
  DECL_INITIAL (temp_val) = val;
  return temp_val;
}

/* Count how many bits a variable (e.g. a record field) of type TYPE needs.
 * Return the result as an INTEGER_CST node, or NULL_TREE if packing of
 * this type is not possible.
 */
tree
count_bits (type)
     tree type;
{
  int positive = 1;
  tree min_val, max_val, result;
  if (! ORDINAL_TYPE (TREE_CODE (type)))
    return NULL_TREE;
  min_val = TYPE_MIN_VALUE (type);
  max_val = TYPE_MAX_VALUE (type);
  if (min_val == NULL_TREE || max_val == NULL_TREE
      || TREE_CODE (min_val) != INTEGER_CST
      || TREE_CODE (max_val) != INTEGER_CST)
    return NULL_TREE;
  if (! TREE_UNSIGNED (type)
      && ((! TREE_UNSIGNED (min_val) && TREE_INT_CST_HIGH (min_val) < 0)
          || (! TREE_UNSIGNED (max_val) && TREE_INT_CST_HIGH (max_val) < 0)))
    positive = 0;
  if (positive)
    {
      int min_bits = 0, max_bits = 0;
      unsigned HOST_WIDE_INT check = TREE_INT_CST_HIGH (min_val);
      while (check)
        {
          min_bits++;
          check >>= 1;
        }
      if (min_bits)
        min_bits += HOST_BITS_PER_WIDE_INT;
      else
        {
          check = TREE_INT_CST_LOW (min_val);
          while (check)
            {
              min_bits++;
              check >>= 1;
            }
        }
      check = TREE_INT_CST_HIGH (max_val);
      while (check)
        {
          max_bits++;
          check >>= 1;
        }
      if (max_bits)
        max_bits += HOST_BITS_PER_WIDE_INT;
      else
        {
          check = TREE_INT_CST_LOW (max_val);
          while (check)
            {
              max_bits++;
              check >>= 1;
            }
        }
      result = build_int_2 (min_bits > max_bits ? min_bits : max_bits, 0);
    }
  else
    {
      int min_bits = 0, max_bits = 0;
      HOST_WIDE_INT check = TREE_INT_CST_HIGH (min_val);
      unsigned HOST_WIDE_INT ucheck;
      if (check < 0)
        check = ~check;
      while (check)
        {
          min_bits++;
          check >>= 1;
        }
      if (min_bits)
        min_bits += HOST_BITS_PER_WIDE_INT;
      else
        {
          ucheck = TREE_INT_CST_LOW (min_val);
          if ((HOST_WIDE_INT) TREE_INT_CST_HIGH (min_val) < 0)
            ucheck = ~ucheck;
          while (ucheck)
            {
              min_bits++;
              ucheck >>= 1;
            }
        }
      check = TREE_INT_CST_HIGH (max_val);
      if (check < 0)
        check = ~check;
      while (check)
        {
          max_bits++;
          check >>= 1;
        }
      if (max_bits)
        max_bits += HOST_BITS_PER_WIDE_INT;
      else
        {
          ucheck = TREE_INT_CST_LOW (max_val);
          if ((HOST_WIDE_INT) TREE_INT_CST_HIGH (max_val) < 0)
            ucheck = ~ucheck;
          while (ucheck)
            {
              max_bits++;
              ucheck >>= 1;
            }
        }
      result = build_int_2 ((min_bits > max_bits ? min_bits : max_bits) + 1, 0);
    }
  TREE_UNSIGNED (result) = positive;
  return result;
}

tree
p_grokfields (list, type)
     tree list, type;
{
  tree link, ret = NULL_TREE, bits = NULL_TREE;
  if (TREE_CODE (type) == IDENTIFIER_NODE)
    {
      error ("type name expected, identifier `%s' given",
             IDENTIFIER_POINTER (type));

      /* @@ `error_mark_node' yields "type defaults to `int'" warning.
       */
      type = integer_type_node;
    }
  else
    {
      if (flag_pack_struct && ORDINAL_TYPE (TREE_CODE (type)))
        bits = count_bits (type);
      if (bits && TREE_UNSIGNED (bits) != TREE_UNSIGNED (type))
        {
          type = build_type_copy (type);
          TREE_UNSIGNED (type) = TREE_UNSIGNED (bits);
        }
    }
  type = build_tree_list (NULL_TREE, type);
  for (link = list; link; link = TREE_CHAIN (link))
    ret = chainon (ret, grokfield ("", 0, TREE_VALUE (link), type, bits));
  return ret;
}

/*
 * @@@. Sigh. I just noticed that array_type_nelts thinks that
 * the array type is max-min long. However, I assume that arrays are
 * max-min+1 long, so I had to make yet another routine...
 */

/* Return, as an INTEGER_CST node, the number of elements for
 * TYPE (which is an ARRAY_TYPE).
 */
tree
pascal_array_type_nelts (type)
     tree type;
{
  tree index_type = TYPE_DOMAIN (type);
  tree len;

  if (! index_type || index_type == error_mark_node)
    return error_mark_node;

  len = tree_int_cst_equal (TYPE_MIN_VALUE (index_type), integer_zero_node)
          ? TYPE_MAX_VALUE (index_type)
          : fold (build (MINUS_EXPR, integer_type_node,
                         TYPE_MAX_VALUE (index_type),
                         TYPE_MIN_VALUE (index_type)));

  return fold (build (PLUS_EXPR, integer_type_node,
                      len, integer_one_node));
}

tree
object_size (type)
     tree type;
{
  if (TREE_CODE (type) == ARRAY_TYPE)
    return build_binary_op (MULT_EXPR,
                            pascal_array_type_nelts (type),
                            object_size (TREE_TYPE (type)),
                            1);
  else
    return size_in_bytes (type);
}

/* Compare two INTEGER_CST nodes which may both be either
 * signed or unsigned.
 */
int
int_cst_lt_always (val1, val2)
    tree val1, val2;
{
  if ((TREE_UNSIGNED (val1) || TREE_UNSIGNED (TREE_TYPE (val1)))
      && INT_CST_LT_UNSIGNED (TYPE_MAX_VALUE (long_long_integer_type_node), val1))
    {
      if ((TREE_UNSIGNED (val2) || TREE_UNSIGNED (TREE_TYPE (val2)))
          && INT_CST_LT_UNSIGNED (TYPE_MAX_VALUE (long_long_integer_type_node), val2))
        return INT_CST_LT_UNSIGNED (val1, val2);
      else
        return 0;
    }
  else if ((TREE_UNSIGNED (val2) || TREE_UNSIGNED (TREE_TYPE (val2)))
           && INT_CST_LT_UNSIGNED (TYPE_MAX_VALUE (long_long_integer_type_node), val2))
    return 1;
  else
    return INT_CST_LT (val1, val2);
}

/* Return the smallest standard signed integer type that can hold
 * the negative of the unsigned integer type TYPE.
 */
tree
select_signed_integer_type (type)
     tree type;
{
  int precision = TYPE_PRECISION (type);

  if (precision < TYPE_PRECISION (integer_type_node))
    return integer_type_node;
  else if (precision < TYPE_PRECISION (long_integer_type_node))
    return long_integer_type_node;
  else
    return long_long_integer_type_node;
}

/* Return a reasonable common type for the ordinal values
 * VAL1 and VAL2.  WHY is the operation intended for these
 * values and might be NOP_EXPR.
 */
tree
select_integer_type (val1, val2, why)
     tree val1, val2;
     enum tree_code why;
{
  if (TREE_CODE (val1) != INTEGER_CST
      || TREE_CODE (val2) != INTEGER_CST
      || TREE_CODE (TREE_TYPE (val1)) != INTEGER_TYPE
      || TREE_CODE (TREE_TYPE (val2)) != INTEGER_TYPE)
    {
      if ((why == MINUS_EXPR || why == PLUS_EXPR)
           && TREE_UNSIGNED (TREE_TYPE (val1)) != TREE_UNSIGNED (TREE_TYPE (val2)))
        {
          if TREE_UNSIGNED (TREE_TYPE (val1))
            return select_signed_integer_type (TREE_TYPE (val1));
          else
            return select_signed_integer_type (TREE_TYPE (val2));
        }
      else
        return common_type (TREE_TYPE (val1), TREE_TYPE (val2));
    }
  else
    {
      /* Everything constant.  Make it as small as possible, but big
       * enough to hold the result of the intended operation (if known)
       * and not smaller than the largest explicit type.
       */
      tree min_val, max_val;
      if (int_cst_lt_always (val1, val2))
        {
          min_val = val1;
          max_val = val2;
        }
      else
        {
          min_val = val2;
          max_val = val1;
        }
      if (! PASCAL_TREE_FRESH_INT_CST (val1))
        {
          if (int_cst_lt_always (TYPE_MIN_VALUE (TREE_TYPE (val1)), min_val))
            min_val = TYPE_MIN_VALUE (TREE_TYPE (val1));
          if (int_cst_lt_always (max_val, TYPE_MAX_VALUE (TREE_TYPE (val1))))
            max_val = TYPE_MAX_VALUE (TREE_TYPE (val1));
        }
      if (! PASCAL_TREE_FRESH_INT_CST (val2))
        {
          if (int_cst_lt_always (TYPE_MIN_VALUE (TREE_TYPE (val2)), min_val))
            min_val = TYPE_MIN_VALUE (TREE_TYPE (val2));
          if (int_cst_lt_always (max_val, TYPE_MAX_VALUE (TREE_TYPE (val2))))
            max_val = TYPE_MAX_VALUE (TREE_TYPE (val2));
        }
      switch (why)
        {
          case PLUS_EXPR:
          case MINUS_EXPR:
          case MULT_EXPR:
          case CEIL_DIV_EXPR:
          case ROUND_DIV_EXPR:
          case TRUNC_MOD_EXPR:
          case FLOOR_MOD_EXPR:
          /* case EXPON_EXPR: */
          case LSHIFT_EXPR:
          case RSHIFT_EXPR:
          case LROTATE_EXPR:
          case RROTATE_EXPR:
            {
              /* For operations which can make the result take off from
               * zero, take it into account.
               */
              tree op1 = convert (TREE_UNSIGNED (val1) || ! INT_CST_LT (val1, integer_zero_node)
                                  ? long_long_unsigned_type_node
                                  : long_long_integer_type_node, val1);
              tree op2 = convert (TREE_UNSIGNED (val2) || ! INT_CST_LT (val2, integer_zero_node)
                                  ? long_long_unsigned_type_node
                                  : long_long_integer_type_node, val2);
              tree result = fold (build_binary_op (why, op1, op2, 0));
              if (TREE_CODE (result) == INTEGER_CST)
                {
                  if (int_cst_lt_always (result, min_val))
                    min_val = result;
                  if (int_cst_lt_always (max_val, result))
                    max_val = result;
                }
              break;
            }
          default:
            break;
        }

      if (int_cst_lt_always (min_val, integer_zero_node))
        {
          if (int_cst_lt_always (TYPE_MAX_VALUE (long_integer_type_node), max_val)
              || int_cst_lt_always (min_val, TYPE_MIN_VALUE (long_integer_type_node)))
            return long_long_integer_type_node;
          else if (int_cst_lt_always (TYPE_MAX_VALUE (integer_type_node), max_val)
                   || int_cst_lt_always (min_val, TYPE_MIN_VALUE (integer_type_node)))
            return long_integer_type_node;
          else
            return integer_type_node;
        }
      else
        {
          /* If `Integer' is sufficient to hold this value, use that.
           */
          if (int_cst_lt_always (TYPE_MAX_VALUE (long_unsigned_type_node), max_val))
            return long_long_unsigned_type_node;
          else if (int_cst_lt_always (TYPE_MAX_VALUE (unsigned_type_node), max_val))
            return long_unsigned_type_node;
          else if (int_cst_lt_always (TYPE_MAX_VALUE (integer_type_node), max_val))
            return long_unsigned_type_node;
          else
            return integer_type_node;
        }
    }
}

/* Build a subrange type.  Same as `build_range_type()', but
 * derive the TREE_TYPE from the values, and don't become
 * confused from integer constants being `LongestInt|Card'.
 * In particular, prevent schema discriminants from being
 * converted if used together with a constant.
 *
 * @@ If two schema discriminants of different types are
 * used together here, this will still break.  But why
 * should anybody use such things?  ;-)
 */
tree
build_pascal_range_type (lowval, highval)
     tree lowval, highval;
{
  tree type, range_type;
  if (TREE_CODE (lowval) == INTEGER_CST
      && TREE_CODE (highval) == INTEGER_CST)
    {
      if (int_cst_lt_always (highval, lowval))
        error ("invalid subrange type");
      else if (int_cst_lt_always (lowval, integer_zero_node)
               && int_cst_lt_always (TYPE_MAX_VALUE (long_long_integer_type_node), highval))
        error ("range too big");
    }
  type = select_integer_type (lowval, highval, NOP_EXPR);
  range_type = build_range_type (type, lowval, highval);
  TREE_SET_CODE (range_type, TREE_CODE (type));
  if (TREE_CODE (lowval) == INTEGER_CST)
    TREE_UNSIGNED (range_type) = ! int_cst_lt_always (lowval, integer_zero_node);
  else
    TREE_UNSIGNED (range_type) = TREE_UNSIGNED (TREE_TYPE (lowval));
  return range_type;
}

/* Construct a pointer to a possibly unknown type.
 */
tree
build_pascal_pointer_type (type, constp)
     tree type;
     int constp;
{
  char *ptr_name, *ref_name;
  tree ref_type;
  tree name = type;

  if (TREE_CODE (name) == LANG_TYPE)
    {
      /* This type is already tagged.  Fine.
       */
      return build_pointer_type (build_type_variant (type, constp, 0));
    }
  else if (TREE_CODE (name) != IDENTIFIER_NODE)
    {
      if (defining_types)
        {
          tree scan;
          tree decl = TYPE_NAME (name);

          /* If the type has no name, it might be a
           * function pointer or string schema.
           */
          if (! decl)
            return build_pointer_type (build_type_variant (type, constp, 0));

          if (TREE_CODE (decl) == IDENTIFIER_NODE)
            decl = lookup_name (decl);

          if (TREE_CODE (decl) != TYPE_DECL)
            abort ();

          name = DECL_NAME (decl);

          for (scan = current_type_list; scan; scan = TREE_CHAIN (scan))
            {
              /* If there already is a local type declaration
               * in this scope, use that.
               */

              if (TREE_VALUE (scan) == name)
                {
                  tree lv = IDENTIFIER_LOCAL_VALUE (TREE_VALUE (scan));
                  if (! lv)
                    {
                      if (global_bindings_p())
                        lv = IDENTIFIER_GLOBAL_VALUE (TREE_VALUE (scan));

                      if (! lv)
                        lv = error_mark_node;
                    }
                  if (TREE_CODE (lv) != TYPE_DECL)
                    {
                      error ("pointer domain `%s' is not a type",
                             IDENTIFIER_POINTER (name));
                      break;
                    }
                  return build_pointer_type (build_type_variant (TREE_TYPE (lv), constp, 0));
                }
            }
        }
      else
        return build_pointer_type (build_type_variant (type, constp, 0));
    }

  /* The type is an identifier, which is a reference to a
   * type that is unknown.  This is only legal inside a type
   * definition block.
   */
  if (! defining_types)
    {
      error ("pointer domain type undefined");
      return TYPE_POINTER_TO (integer_type_node);
    }

  ptr_name = concat ("ptr_xref_", IDENTIFIER_POINTER (name), NULL_PTR);
  type = xref_tag (POINTER_TYPE, get_identifier (ptr_name));
  TREE_TYPE (type) = make_node (LANG_TYPE);
  TYPE_NAME (TREE_TYPE (type)) = name;  /* ###??? */
  TYPE_POINTER_TO (TREE_TYPE (type)) = type;
  layout_type (type);

  /* Also create a reference type for this target.  It must be patched
   * together with the pointer type.
   */
  ref_name = concat ("ref_xref_", IDENTIFIER_POINTER (name), NULL_PTR);
  ref_type = xref_tag (REFERENCE_TYPE, get_identifier (ref_name));
  TREE_TYPE (ref_type) = TREE_TYPE (type);
  TYPE_REFERENCE_TO (TREE_TYPE (ref_type)) = ref_type;
  layout_type (ref_type);

  free (ptr_name);
  free (ref_name);
  return type;
}

/* The ISO 7185 Standard says that the two ways, abbreviated (when
 * giving several index types) and full (when listing each index type
 * separately), of specifying and accessing arrays are equivalent.
 *
 * So we represent arrays internally in their full form.
 *
 * The ELT_TYPE is the user specified element type.
 *
 * The INDEX_LIST is a list of TREE_LIST nodes of index types,
 * in forward order, and thus the ELT_TYPE is the type for the
 * last element in the INDEX_LIST.
 *
 * Returns the type of the complete array specified by the user.
 */
tree
build_pascal_array_type (elt_type, index_list)
     tree elt_type;
     tree index_list;
{
  tree link;
  for (link = nreverse (index_list); link; link = TREE_CHAIN (link))
    {
      tree type = TREE_TYPE (TREE_VALUE (link));
      if (type && type != error_mark_node)
        {
          tree lo = TYPE_MIN_VALUE (type);
          tree hi = TYPE_MAX_VALUE (type);
          if (TREE_CODE (lo) == INTEGER_CST
              && TREE_CODE (hi) == INTEGER_CST
              && int_cst_lt_always (hi, lo))
            error ("array index has invalid range");
          elt_type = build_array_type (elt_type, TREE_VALUE (link));
        }
    }
  return elt_type;
}

/* Build a reference to an array slice
 *
 * ISO-10206 requires ARR to be a string-type, but GPC allows
 * slice accesses to all arrays.
 *
 * SLICE_INX is a TREE_LIST node whose TREE_VALUE is the lower bound
 * and TREE_PURPOSE is the higher bound of the slice range.
 */
static tree
build_array_slice_ref (arr, slice_indices)
     tree arr;
     tree slice_indices;
{
  tree start;
  tree sub_domain, sub_type, sub_ptr;
  tree component_type = char_type_node;
  int is_string = is_string_type (arr, 1);

  if (PEDANTIC (E_O_PASCAL))
    pedwarn ("substring access is an ISO 10206 Extended Pascal extension");

  /* Don't let `fold()' optimize away needed parts of `arr'.
   */
  arr = build1 (CONVERT_EXPR, TREE_TYPE (arr), arr);

  if (! is_string)
    {
      if (TYPE_MAIN_VARIANT (TREE_TYPE (arr)) == cstring_type_node)
        {
          arr = convert (build_pointer_type (char_array_type_node), arr);
          arr = build_indirect_ref (arr, "subarray access");
        }

      if (TREE_CODE (TREE_TYPE (arr)) != ARRAY_TYPE)
        {
          error ("array slice access requires an array object");
          return error_mark_node;
        }

      if (pedantic || flag_what_pascal)
        pedwarn ("ISO Pascal restricts subarray accesses to string types");

      component_type = TREE_TYPE (TREE_TYPE (arr));
    }

  /* Build an array type that is the same length as the
   * sub-array access is.  In EP mode, the new array always
   * starts from index 1.
   */
  if (flag_what_pascal & E_O_PASCAL)
    {
      tree low_index = integer_one_node;
      tree high_index = build_binary_op (PLUS_EXPR, low_index,
                                         build_binary_op (MINUS_EXPR,
                                                          TREE_PURPOSE (slice_indices),
                                                          TREE_VALUE (slice_indices),
                                                          1), 1);
      sub_domain = build_range_type (integer_type_node, low_index, high_index);
    }
  else
    sub_domain = build_range_type (integer_type_node,
                                   TREE_VALUE (slice_indices),
                                   TREE_PURPOSE (slice_indices));
  sub_type = build_pascal_array_type (component_type,
                                      build_tree_list (NULL_TREE, sub_domain));

  /* If the array is packed or a string, so is the subarray.
   */
  if (is_string || PASCAL_TYPE_PACKED (TREE_TYPE (arr)))
    sub_type = grok_packed (sub_type);

  sub_ptr = build_pointer_type (sub_type);

  /* Form a pointer to the start of the subarray.
   */
  start = build1 (ADDR_EXPR,
                  sub_ptr,
                  build_array_ref (PASCAL_STRING_VALUE (arr),
                                   TREE_VALUE (slice_indices)));

  /* Form an access to the slice and return the access.
   */
  return build1 (INDIRECT_REF, sub_type, start);
}

/*
 * Constructs a reference to a pascal array. INDEX_LIST is a
 * TREE_LIST chain of expressions to index the array with.
 * This INDEX_LIST is passed in forward order.
 */
tree
build_pascal_array_ref (var_access, index_list)
     tree var_access;
     tree index_list;
{
  tree link;

  /* `foo [i]' with `foo' being an array type decl crashed GPC.
   * I don't see any other array type entities which could do that ...
   */
  if (TREE_CODE (var_access) == TYPE_DECL
      || (TREE_CODE (TREE_TYPE (var_access)) != ARRAY_TYPE
          && ! PASCAL_TYPE_STRING (TREE_TYPE (var_access))
          && ! (flag_extended_syntax
                && TYPE_MAIN_VARIANT (TREE_TYPE (var_access))
                   == cstring_type_node)))
    {
      error ("subscripted object is not an array or string");
      if (TYPE_MAIN_VARIANT (TREE_TYPE (var_access)) == cstring_type_node)
        cstring_inform ();
      return error_mark_node;
    }

  for (link = index_list; link; link = TREE_CHAIN (link))
    {
      if (TREE_PURPOSE (link))
        var_access = build_array_slice_ref (var_access, link);
      else
        {
          tree bits = NULL_TREE;
          tree domain;

          TREE_VALUE (link) = string_may_be_char (TREE_VALUE (link));

          if (PASCAL_TYPE_STRING (TREE_TYPE (var_access)))
            var_access = PASCAL_STRING_VALUE (var_access);

          if (TYPE_MAIN_VARIANT (TREE_TYPE (var_access)) == cstring_type_node)
            {
              var_access = convert (build_pointer_type (char_array_type_node),
                                    var_access);
              var_access = build_indirect_ref (var_access, "array reference");
            }

          domain = TYPE_DOMAIN (TREE_TYPE (var_access));

          if (domain)
            {
              tree min_index = TYPE_MIN_VALUE (domain);
              tree max_index = TYPE_MAX_VALUE (domain);

              if (TREE_TYPE (domain))
                domain = TREE_TYPE (domain);
              if (comptypes (TREE_TYPE (TREE_VALUE (link)), domain) == 0)
                error ("type mismatch in array index");

              /* Do some trivial range checking.
               */
              if (min_index
                  && TREE_CODE (min_index) == INTEGER_CST
                  && max_index
                  && TREE_CODE (max_index) == INTEGER_CST
                  && TREE_CODE (TREE_VALUE (link)) == INTEGER_CST)
                {
                  if (int_cst_lt_always (TREE_VALUE (link), min_index)
                      || int_cst_lt_always (max_index, TREE_VALUE (link)))
                    error ("array index out of range");
                }
            }

          if (PASCAL_TYPE_PACKED (TREE_TYPE (var_access)))
            bits = count_bits (TREE_TYPE (TREE_TYPE (var_access)));

          if (bits && TREE_INT_CST_LOW (bits)
                      != TREE_INT_CST_LOW (TYPE_SIZE (TREE_TYPE (TREE_TYPE (var_access)))))
            {
              /* Accessing a field of a packed array possibly not located
               * on a byte boundary.
               */
              tree array = TREE_TYPE (var_access);

              /* Index starting from 0.
               */
              tree index = build_binary_op (MINUS_EXPR, TREE_VALUE (link),
                                            TYPE_MIN_VALUE (TYPE_DOMAIN (array)),
                                            0);

              /* Index measured in bits.
               */
              index = fold (build_binary_op (MULT_EXPR, index, bits, 1));
              STRIP_NOPS (index);

              /* Access the bits.
               */
              if (TREE_CODE (index) == INTEGER_CST)
                {
                  var_access = build (BIT_FIELD_REF, TREE_TYPE (array),
                                      var_access, bits, index);
                  TREE_UNSIGNED (var_access) = TREE_UNSIGNED (TREE_TYPE (array));
                }
              else
                {
                  /* BIT_FIELD_REF can only handle constant offsets.
                   * To work around, create a "read" access out of
                   * shift operations here.
                   */
                  unsigned HOST_WIDE_INT imask;
                  tree long_type = packed_array_unsigned_long_type_node;
                  tree short_type = packed_array_unsigned_short_type_node;
                  tree mask;
                  tree half_bits_per_long_type = size_int (TYPE_PRECISION (long_type) / 2);
                  tree half_bytes_per_long_type = size_int (TYPE_PRECISION (long_type) / (2 * BITS_PER_UNIT));
                  tree short_type_ptr = build_pointer_type (short_type);
                  tree low_var_access, high_var_access, offset;

                  index = default_conversion (index);
                  offset = build_binary_op (FLOOR_MOD_EXPR, index,
                                            half_bits_per_long_type, 0);
#if WORDS_BIG_ENDIAN
                  offset = build_binary_op (MINUS_EXPR,
                                            size_int (TYPE_PRECISION (long_type)
                                                      - TREE_INT_CST_LOW (bits)),
                                            offset, 1);
#endif
                  index = build_binary_op (FLOOR_DIV_EXPR, index,
                                           half_bits_per_long_type, 0);
                  imask = ((unsigned HOST_WIDE_INT) ~0)
                          >> (TYPE_PRECISION (long_type) - TREE_INT_CST_LOW (bits));
                  mask = size_int (imask);
                  TREE_TYPE (mask) = long_type;
                  mask = build_binary_op (LSHIFT_EXPR, mask, offset, 0);
                  var_access = convert_array_to_pointer (var_access);
                  var_access = convert (string_type_node, var_access);
                  var_access = build_binary_op (PLUS_EXPR, var_access,
                                 build_binary_op (MULT_EXPR, index,
                                                  half_bytes_per_long_type, 1), 1);

                  /* Get the long_type value in two halfs to avoid alignment problems.
                   */
#if WORDS_BIG_ENDIAN
                  high_var_access = convert (short_type_ptr, var_access);
                  high_var_access = convert (long_type, build_indirect_ref (high_var_access,
                                                          "packed array access"));
                  var_access = build_binary_op (PLUS_EXPR, var_access,
                                                half_bytes_per_long_type, 1);
                  low_var_access = convert (short_type_ptr, var_access);
                  low_var_access = convert (long_type, build_indirect_ref (low_var_access,
                                                         "packed array access"));
#else
                  low_var_access = convert (short_type_ptr, var_access);
                  low_var_access = convert (long_type, build_indirect_ref (low_var_access,
                                                         "packed array access"));
                  var_access = build_binary_op (PLUS_EXPR, var_access,
                                                half_bytes_per_long_type, 1);
                  high_var_access = convert (short_type_ptr, var_access);
                  high_var_access = convert (long_type, build_indirect_ref (high_var_access,
                                                         "packed array access"));
#endif
                  high_var_access = build_binary_op (LSHIFT_EXPR, high_var_access,
                                                     half_bits_per_long_type, 1);
                  var_access = build_binary_op (BIT_IOR_EXPR, low_var_access, high_var_access, 1);

                  /* Now AND with the mask and shift down.
                   */
                  var_access = build_binary_op (BIT_AND_EXPR, var_access, mask, 1);
                  var_access = build_binary_op (RSHIFT_EXPR, var_access, offset, 0);
                  if (! TREE_UNSIGNED (TREE_TYPE (array)))
                    {
                      tree negative, neg_var_access;
                      imask = ~imask;
                      mask = size_int (imask);
                      neg_var_access = build_binary_op (BIT_IOR_EXPR, var_access, mask, 0);
                      imask = ((unsigned HOST_WIDE_INT) 1) << (TREE_INT_CST_LOW (bits) - 1);
                      mask = size_int (imask);
                      negative = build_binary_op (BIT_AND_EXPR, var_access, mask, 0);
                      var_access = build (COND_EXPR, TREE_TYPE (var_access),
                                          negative, neg_var_access, var_access);
                    }
                  var_access = convert (TREE_TYPE (array), var_access);

                  /* Flag the expression, so build_modify_expr() can identify
                   * this and transform it to something to write to the field.
                   */
                  PASCAL_TREE_PACKED_ACCESS (var_access) = 1;
                }
            }
          else
            {
              /* Not packed.  Just a normal array reference.
               */
              var_access = build_array_ref (var_access, TREE_VALUE (link));
            }
        }
      if (var_access == error_mark_node)
        break;
    }
  prediscriminate_schema (var_access, 0);
  return var_access;
}

static int
guess_precision (minval, maxval)
     tree minval, maxval;
{
    if (int_fits_type_p (minval, signed_char_type_node) &&
        int_fits_type_p (maxval, signed_char_type_node))
        return TYPE_PRECISION (signed_char_type_node);
    else if (int_fits_type_p (minval, short_integer_type_node) &&
             int_fits_type_p (maxval, short_integer_type_node))
        return TYPE_PRECISION (short_integer_type_node);
    else if (int_fits_type_p (minval, integer_type_node) &&
             int_fits_type_p (maxval, integer_type_node))
        return TYPE_PRECISION (integer_type_node);
    else if (int_fits_type_p (minval, long_integer_type_node) &&
             int_fits_type_p (maxval, long_integer_type_node))
        return TYPE_PRECISION (long_integer_type_node);
    else {
        error ("range will not fit in the largest integer type available");
        return TYPE_PRECISION (long_integer_type_node);
    }
}

/* Create and return a type for signed integers of PRECISION bits, if
 * the bounds are constant, otherwise just return a new range type with
 * variable bounds.
 */
tree
make_signed_range (minval, maxval)
     tree minval;
     tree maxval;
{
  register tree type = make_node (INTEGER_TYPE);

  /* Set the upper and lower limits of the range.
   */

  TYPE_MIN_VALUE (type) = minval;
  TYPE_MAX_VALUE (type) = maxval;

  if (TREE_CODE (minval) == INTEGER_CST && TREE_CODE (maxval) == INTEGER_CST)
    {
      /* Set these things only if the bounds are constant */

      TYPE_PRECISION (type) = guess_precision (minval, maxval);

      /* Lay out the type: set its alignment, size, etc.  */

      TYPE_SIZE (type) = NULL_TREE;
#ifdef EGCS
      TYPE_SIZE_UNIT (type) = NULL_TREE;
#endif /* EGCS */
      layout_type (type);
    }
  return type;
}

/* @@@ Range needs to be the correct type!!!
 */
tree
convert_type_to_range (type)
     tree type;
{
  tree min, max;
  tree itype;

  if (! ORDINAL_TYPE (TREE_CODE (type)))
    {
      error ("ordinal type expected");
      return error_mark_node;
    }

  min = TYPE_MIN_VALUE (type);
  max = TYPE_MAX_VALUE (type);

  if (TREE_TYPE (min) != TREE_TYPE (max))
    {
      error ("range limits are not of the same type");
      return error_mark_node;
    }

  itype = copy_node (build_pascal_range_type (min, max));

  if TREE_TYPE (type)
    TREE_TYPE (itype) = TREE_TYPE (type);
  else
    TREE_TYPE (itype) = type;

  return itype;
}
