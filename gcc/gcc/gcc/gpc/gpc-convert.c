/* Language-level data type conversion for GNU C.
   Copyright (C) 1987, 1988, 1991, 1997-2000 Free Software Foundation, Inc.

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
along with GNU Pascal; see the file COPYING. If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA. */


/* This file contains the functions for converting C expressions
   to different data types.  The only entry point is `convert'.
   Every language front end must have a `convert' function
   but what kind of conversions it does will depend on the language.  */

#include "gcc-version.h"
#include "config.h"
#include "gansidecl.h"
#ifdef EGCS
#include "system.h"
#include "toplev.h"
#endif

#ifdef GPC
#include "gbe.h"
#include "gpc-defs.h"
#include "types.h"
#include "util.h"
#else
/* In GPC, they are included via gbe.h */
#include "tree.h"
#include "flags.h"
#endif
#include "convert.h"

/* Change of width--truncation and extension of integers or reals--
   is represented with NOP_EXPR.  Proper functioning of many things
   assumes that no other conversions can be NOP_EXPRs.

   Conversion between integer and pointer is represented with CONVERT_EXPR.
   Converting integer to real uses FLOAT_EXPR
   and real to integer uses FIX_TRUNC_EXPR.

   Here is a list of all the functions that assume that widening and
   narrowing is always done with a NOP_EXPR:
     In convert.c, convert_to_integer.
     In c-typeck.c, build_binary_op (boolean ops), and truthvalue_conversion.
     In expr.c: expand_expr, for operands of a MULT_EXPR.
     In fold-const.c: fold.
     In tree.c: get_narrower and get_unwidened.  */

/* Subroutines of `convert'.  */

#ifdef GPC

extern tree boolean_false_node;
extern tree boolean_true_node;

static tree
convert_to_boolean (type, expr)
     tree type, expr;
{
  register tree intype = TREE_TYPE (expr);
  register enum tree_code form = TREE_CODE (intype);

  if (integer_zerop (expr))
    return boolean_false_node;
  else if (integer_onep (expr))
    return boolean_true_node;
  else if (form == BOOLEAN_TYPE)
    return build1 (NOP_EXPR, type, expr);
  else if (ORDINAL_TYPE (form) || form == POINTER_TYPE)
    return build1 (CONVERT_EXPR, type, expr);

  error ("cannot convert to a boolean type");
  return boolean_false_node;
}

static tree
convert_to_char (type, expr)
     tree type, expr;
{
  register tree intype = TREE_TYPE (expr);
  register enum tree_code form = TREE_CODE (intype);

  if (form == CHAR_TYPE)
    return build1 (NOP_EXPR, type, expr);

  /* @@ If it does not fit? */
  if (form == INTEGER_TYPE)
    return build1 (CONVERT_EXPR, type, expr);

  error ("cannot convert to a char type");

  {
    register tree tem = build_int_2 (0, 0);
    TREE_TYPE (tem) = type;
    return tem;
  }
}

/* @@@@@

   A hack to fix inline function set_type arguments,
   when the sets are subranges of the same base_type.
   I think this should be taken care of earlier.
   Occurs only with high optimization levels.

   FIXME!!!
 */
static tree
convert_sets (type, e)
     tree type, e;
{
  tree intype = TREE_TYPE (e);
  enum tree_code form = TREE_CODE (intype);
  tree e_base;
  tree t_base = base_type (TREE_TYPE (type));

  if (form != SET_TYPE)
    return NULL_TREE;

  e_base = base_type (TREE_TYPE (intype));

  if (TREE_CODE (t_base) != TREE_CODE (e_base))
    return NULL_TREE;

  /* Check whether both types are actually the same type.
   */
  if (TREE_CODE (TYPE_MIN_VALUE (TYPE_DOMAIN (type))) == INTEGER_CST
      && TREE_CODE (TYPE_MAX_VALUE (TYPE_DOMAIN (type))) == INTEGER_CST
      && TREE_CODE (TYPE_MIN_VALUE (TYPE_DOMAIN (intype))) == INTEGER_CST
      && TREE_CODE (TYPE_MAX_VALUE (TYPE_DOMAIN (intype))) == INTEGER_CST
      && TREE_INT_CST_LOW (TYPE_MIN_VALUE (TYPE_DOMAIN (type)))
         == TREE_INT_CST_LOW (TYPE_MIN_VALUE (TYPE_DOMAIN (intype)))
      && TREE_INT_CST_HIGH (TYPE_MIN_VALUE (TYPE_DOMAIN (type)))
         == TREE_INT_CST_HIGH (TYPE_MIN_VALUE (TYPE_DOMAIN (intype)))
      && TREE_INT_CST_LOW (TYPE_MAX_VALUE (TYPE_DOMAIN (type)))
         == TREE_INT_CST_LOW (TYPE_MAX_VALUE (TYPE_DOMAIN (intype)))
      && TREE_INT_CST_HIGH (TYPE_MAX_VALUE (TYPE_DOMAIN (type)))
         == TREE_INT_CST_HIGH (TYPE_MAX_VALUE (TYPE_DOMAIN (intype))))
    return e;

#if 0
  /* This does not convert anything!
   */
  return build1 (CONVERT_EXPR, type, e);
#else
  /* Force setop_copy() to be called by calculating the intersection
   * with a full set of the type to be converted to.
   */
  {
    tree start_decl ();
    tree build_binary_op ();
    tree domain = TYPE_DOMAIN (type);
    tree minval = copy_node (TYPE_MIN_VALUE (domain));
    tree maxval = copy_node (TYPE_MAX_VALUE (domain));
    tree d, c;

    TREE_TYPE (minval) = TREE_TYPE (maxval) = TREE_TYPE (type);
    d = start_decl (get_unique_identifier ("set_conversion", 0),
                    build_tree_list (NULL_TREE, type),
                    1, NULL_TREE, NULL_TREE);
    finish_decl (d, NULL_TREE, NULL_TREE);
    c = construct_set (build_set_constructor (construct_set_member (minval,
                                                                maxval)),
                       d, 0);
    if (c)
      expand_expr_stmt (build_modify_expr (d, NOP_EXPR, c));
    return build_pascal_binary_op (MULT_EXPR, d, e);
  }
#endif
}
#endif /* GPC */

/* Create an expression whose value is that of EXPR,
   converted to type TYPE.  The TREE_TYPE of the value
   is always TYPE.  This function implements all reasonable
   conversions; callers should filter out those that are
   not permitted by the language being compiled.  */

tree
convert (type, expr)
     tree type, expr;
{
  register tree e = expr;
  register enum tree_code code = TREE_CODE (type);

#ifdef GPC
  if (type == NULL_TREE || expr == NULL_TREE)
    abort ();
#endif /* GPC */
  if (type == TREE_TYPE (expr)
      || TREE_CODE (expr) == ERROR_MARK)
    return expr;
  if (TYPE_MAIN_VARIANT (type) == TYPE_MAIN_VARIANT (TREE_TYPE (expr)))
    return fold (build1 (NOP_EXPR, type, expr));
  if (TREE_CODE (TREE_TYPE (expr)) == ERROR_MARK)
    return error_mark_node;
  if (TREE_CODE (TREE_TYPE (expr)) == VOID_TYPE)
    {
      error ("void value not ignored as it ought to be");
      return error_mark_node;
    }
  if (code == VOID_TYPE)
    return build1 (CONVERT_EXPR, type, e);
#if 0
  /* This is incorrect.  A truncation can't be stripped this way.
     Extensions will be stripped by the use of get_unwidened.  */
  if (TREE_CODE (expr) == NOP_EXPR)
    return convert (type, TREE_OPERAND (expr, 0));
#endif
  if (code == INTEGER_TYPE || code == ENUMERAL_TYPE)
#ifdef GPC
    {
      tree result = fold (convert_to_integer (type, e));
      if (TREE_CODE (result) == INTEGER_CST
          && TREE_UNSIGNED (TREE_TYPE (result)))
        TREE_UNSIGNED (result) = 1;
      return result;
    }
#else /* not GPC */
    return fold (convert_to_integer (type, e));
#endif /* not GPC */
  if (code == POINTER_TYPE)
#ifdef GPC
    {
      /* @@@ REFERENCE_TYPE not utilized fully in GPC.
       *     Should be re-implemented. (I implemented var parameters
       *     before GCC had REFERENCE_TYPE)
       */
      if (TREE_CODE (TREE_TYPE (e)) == REFERENCE_TYPE)
        return fold (build1 (CONVERT_EXPR, type, e));

      return fold (convert_to_pointer (type, e));
    }

  /* @@@ This is ugly;, see above; FIXME */
  if (code == SET_TYPE)
    {
      tree converted = convert_sets (type, e);
      if (converted)
        return converted;
    }

#if 1
  /* @@@@@@@@@@ FIXME FIXME
     Note: I am not sure this is the correct place
     to try to fix the problem:
        -O6 in alpha compiling zap.pas
     traps here. But should the reference_type be trapped
     earlier??
   */
  if (code == REFERENCE_TYPE)
    {
      if (TREE_CODE (TREE_TYPE (e)) == POINTER_TYPE)
        return fold (build1 (CONVERT_EXPR, type, e));

      return fold (build1 (CONVERT_EXPR, type,
                           fold (convert_to_pointer
                                 (build_pointer_type (TREE_TYPE (type)),
                                  e))));
    }
#endif /* @@@@ FIXME FIXME */
#else
    return fold (convert_to_pointer (type, e));
#endif /* GPC */
  if (code == REAL_TYPE)
    return fold (convert_to_real (type, e));
  if (code == COMPLEX_TYPE)
    return fold (convert_to_complex (type, e));
#ifdef GPC
  if (code == BOOLEAN_TYPE)
    return fold (convert_to_boolean (type, e));
  if (code == CHAR_TYPE)
    return fold (convert_to_char (type, e));
#endif

  error ("conversion to non-scalar type requested");
  return error_mark_node;
}
