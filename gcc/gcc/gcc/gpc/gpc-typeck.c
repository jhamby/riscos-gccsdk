/* Build expressions with type checking for Pascal compiler.
   Copyright (C) 1987, 88, 1991-2000 Free Software Foundation, Inc.

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
02111-1307, USA. */


/* This file is part of the C (and Pascal) front end.
   It contains routines to build C expressions given their operands,
   including computing the types of the result, C-specific error checks,
   and some optimization.

   There are also routines to build RETURN_STMT nodes and CASE_STMT nodes,
   and to process initializations in declarations (since they work
   like a strange sort of assignment).  */

#include "gcc-version.h"
#include "config.h"
#include "gansidecl.h"
#include "system.h"
#ifdef EGCS
#include "toplev.h"
#endif

#include <stdio.h>

#ifdef GPC
#include "gbe.h"
#include "gpc-tree.h"
#else
/* In GPC, they are included via gbe.h */
#include "tree.h"
#include "flags.h"
#endif
#include "output.h"

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#else
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#endif

#ifdef GPC
#include "gpc-defs.h"
#include "module.h"
#include "rts.h"
#include "types.h"
#include "objects.h"
#include "util.h"
#ifdef __EMX__
/* These are quick fixes to solve a small incompatibility between gpc's
        gpc-typeck.c and EMX's c-typeck.c with bounds checking enabled.
*/
int bounds_disable_field_check = 0;
int pipes_supported = 0;
#endif /* __EMX__ */
#endif /* GPC */

#if defined(GPC) && !defined(EGCS92)
# define gpc_pedwarn_init pedwarn_init
# define gpc_error_init error_init
#endif

/* Nonzero if we've already printed a "missing braces around initializer"
   message within this initializer.  */
static int missing_braces_mentioned;

#ifdef NEED_DECLARATION_INDEX
extern char *index ();
#endif

#ifdef NEED_DECLARATION_RINDEX
extern char *rindex ();
#endif

#ifdef GPC
/* Nonzero if we warn about void type pointer dereferences.
 * @@@ Should be a parameter to build_indirect_ref().
 */
int warn_void_dereference = 0;

/* For type checking during the body of a `case' statement is parsed.
 */
tree current_case_expression = NULL_TREE;

/* Label to exit a `case' statement, so `break' can exit a loop.
 */
tree current_case_exit_label = NULL_TREE;
#endif /* GPC */

static tree qualify_type                PROTO((tree, tree));
static int comp_target_types            PROTO((tree, tree));
#ifdef GPC
static int comp_object_or_schema_pointer_types PROTO((tree, tree));
static int compatible_types_p           PROTO((tree, tree));
static int compatible_assignment_p      PROTO((tree, tree));
static int compatible_relop_p           PROTO((tree, tree));
#endif /* GPC */
static int function_types_compatible_p  PROTO((tree, tree));
static int type_lists_compatible_p      PROTO((tree, tree));
static int self_promoting_type_p        PROTO((tree));
static tree decl_constant_value         PROTO((tree));
static tree lookup_field                PROTO((tree, tree, tree *));
static tree convert_arguments           PROTO((tree, tree, tree, tree));
static tree pointer_int_sum             PROTO((enum tree_code, tree, tree));
static tree pointer_diff                PROTO((tree, tree));
static tree unary_complex_lvalue        PROTO((enum tree_code, tree));
static void pedantic_lvalue_warning     PROTO((enum tree_code));
static tree internal_build_compound_expr PROTO((tree, int));
static tree convert_for_assignment      PROTO((tree, tree, char *, tree,
                                               tree, int));
static void warn_for_assignment         PROTO((char *, char *, tree, int));
#ifdef GPC
static void error_assignment            PROTO((char *, char *, tree, int));
#endif /* GPC */
static tree valid_compound_expr_initializer PROTO((tree, tree));
static void push_string                 PROTO((char *));
static void push_member_name            PROTO((tree));
static void push_array_bounds           PROTO((int));
static int spelling_length              PROTO((void));
static char *print_spelling             PROTO((char *));
static char *get_spelling               PROTO((char *));
static void warning_init                PROTO((char *, char *,
                                               char *));
static tree digest_init                 PROTO((tree, tree, int, int));
static void check_init_type_bitfields   PROTO((tree));
static void output_init_element         PROTO((tree, tree, tree, int));
static void output_pending_init_elements PROTO((int));

/* Do `exp = require_complete_type (exp);' to make sure exp
   does not have an incomplete type.  (That includes void types.)  */

tree
require_complete_type (value)
     tree value;
{
  tree type = TREE_TYPE (value);

  /* First, detect a valid value with a complete type.  */
  if (TYPE_SIZE (type) != 0
      && type != void_type_node)
    return value;

  incomplete_type_error (value, type);
  return error_mark_node;
}

/* Print an error message for invalid use of an incomplete type.
   VALUE is the expression that was used (or 0 if that isn't known)
   and TYPE is the type that was invalid.  */

void
incomplete_type_error (value, type)
     tree value;
     tree type;
{
  char *errmsg;

  /* Avoid duplicate error message.  */
  if (TREE_CODE (type) == ERROR_MARK)
    return;

  if (value != 0 && (TREE_CODE (value) == VAR_DECL
                     || TREE_CODE (value) == PARM_DECL))
    error ("`%s' has an incomplete type",
           IDENTIFIER_POINTER (DECL_NAME (value)));
  else
    {
    retry:
      /* We must print an error message.  Be clever about what it says.  */

      switch (TREE_CODE (type))
        {
        case RECORD_TYPE:
          errmsg = "invalid use of undefined type `struct %s'";
          break;

        case UNION_TYPE:
          errmsg = "invalid use of undefined type `union %s'";
          break;

        case ENUMERAL_TYPE:
          errmsg = "invalid use of undefined type `enum %s'";
          break;

        case VOID_TYPE:
          error ("invalid use of void expression");
          return;

        case ARRAY_TYPE:
          if (TYPE_DOMAIN (type))
            {
              type = TREE_TYPE (type);
              goto retry;
            }
          error ("invalid use of array with unspecified bounds");
          return;

        default:
          abort ();
        }

      if (TREE_CODE (TYPE_NAME (type)) == IDENTIFIER_NODE)
        error (errmsg, IDENTIFIER_POINTER (TYPE_NAME (type)));
      else
        /* If this type has a typedef-name, the TYPE_NAME is a TYPE_DECL.  */
        error ("invalid use of incomplete typedef `%s'",
               IDENTIFIER_POINTER (DECL_NAME (TYPE_NAME (type))));
    }
}

/* Return a variant of TYPE which has all the type qualifiers of LIKE
   as well as those of TYPE.  */

static tree
qualify_type (type, like)
     tree type, like;
{
  int constflag = TYPE_READONLY (type) || TYPE_READONLY (like);
  int volflag = TYPE_VOLATILE (type) || TYPE_VOLATILE (like);
  return build_c_type_variant (type, constflag, volflag);
}

/* Return the common type of two types.
   We assume that comptypes has already been done and returned 1;
   if that isn't so, this may crash.  In particular, we assume that qualifiers
   match.

   This is the type for the result of most arithmetic operations
   if the operands have the given two types.  */

tree
common_type (t1, t2)
     tree t1, t2;
{
  register enum tree_code code1;
  register enum tree_code code2;
  tree attributes;

#ifdef GPC
  /* Extend subranges of ordinal types to their full types.
   * Otherwise, operations leaving the range won't work.
   */
  if (ORDINAL_TYPE (TREE_CODE (t1)) && TREE_TYPE (t1))
    t1 = TREE_TYPE (t1);
  if (ORDINAL_TYPE (TREE_CODE (t2)) && TREE_TYPE (t2))
    t2 = TREE_TYPE (t2);
#endif /* GPC */

  /* Save time if the two types are the same.  */

  if (t1 == t2) return t1;

  /* If one type is nonsense, use the other.  */
  if (t1 == error_mark_node)
    return t2;
  if (t2 == error_mark_node)
    return t1;

  /* Merge the attributes */
  attributes = merge_attributes (TYPE_ATTRIBUTES (t1), TYPE_ATTRIBUTES (t2));

  /* Treat an enum type as the unsigned integer type of the same width.  */

  if (TREE_CODE (t1) == ENUMERAL_TYPE)
    t1 = type_for_size (TYPE_PRECISION (t1), 1);
  if (TREE_CODE (t2) == ENUMERAL_TYPE)
    t2 = type_for_size (TYPE_PRECISION (t2), 1);

  code1 = TREE_CODE (t1);
  code2 = TREE_CODE (t2);

  /* If one type is complex, form the common type of the non-complex
     components, then make that complex.  Use T1 or T2 if it is the
     required type.  */
  if (code1 == COMPLEX_TYPE || code2 == COMPLEX_TYPE)
    {
      tree subtype1 = code1 == COMPLEX_TYPE ? TREE_TYPE (t1) : t1;
      tree subtype2 = code2 == COMPLEX_TYPE ? TREE_TYPE (t2) : t2;
      tree subtype = common_type (subtype1, subtype2);

      if (code1 == COMPLEX_TYPE && TREE_TYPE (t1) == subtype)
        return build_type_attribute_variant (t1, attributes);
      else if (code2 == COMPLEX_TYPE && TREE_TYPE (t2) == subtype)
        return build_type_attribute_variant (t2, attributes);
      else
        return build_type_attribute_variant (build_complex_type (subtype),
                                             attributes);
    }

  switch (code1)
    {
    case INTEGER_TYPE:
    case REAL_TYPE:
#ifdef GPC
      /* If only one is complex, use that for the result type */
      if (code2 == COMPLEX_TYPE)
          return build_type_attribute_variant (t2, attributes);
#endif /* GPC */

      /* If only one is real, use it as the result.  */

      if (code1 == REAL_TYPE && code2 != REAL_TYPE)
        return build_type_attribute_variant (t1, attributes);

      if (code2 == REAL_TYPE && code1 != REAL_TYPE)
        return build_type_attribute_variant (t2, attributes);

      /* Both real or both integers; use the one with greater precision.  */

      if (TYPE_PRECISION (t1) > TYPE_PRECISION (t2))
        return build_type_attribute_variant (t1, attributes);
      else if (TYPE_PRECISION (t2) > TYPE_PRECISION (t1))
        return build_type_attribute_variant (t2, attributes);

      /* Same precision.  Prefer longs to ints even when same size.  */

      if (TYPE_MAIN_VARIANT (t1) == long_unsigned_type_node
          || TYPE_MAIN_VARIANT (t2) == long_unsigned_type_node)
        return build_type_attribute_variant (long_unsigned_type_node,
                                             attributes);

      if (TYPE_MAIN_VARIANT (t1) == long_integer_type_node
          || TYPE_MAIN_VARIANT (t2) == long_integer_type_node)
        {
          /* But preserve unsignedness from the other type,
             since long cannot hold all the values of an unsigned int.  */
          if (TREE_UNSIGNED (t1) || TREE_UNSIGNED (t2))
             t1 = long_unsigned_type_node;
          else
             t1 = long_integer_type_node;
          return build_type_attribute_variant (t1, attributes);
        }

      /* Likewise, prefer long double to double even if same size.  */
      if (TYPE_MAIN_VARIANT (t1) == long_double_type_node
          || TYPE_MAIN_VARIANT (t2) == long_double_type_node)
        return build_type_attribute_variant (long_double_type_node,
                                             attributes);

      /* Otherwise prefer the unsigned one.  */

      if (TREE_UNSIGNED (t1))
        return build_type_attribute_variant (t1, attributes);
      else
        return build_type_attribute_variant (t2, attributes);

#ifdef GPC
    /* Since comptypes checks that code2 is ok, return this. */
    case COMPLEX_TYPE:
      return build_type_attribute_variant (t1, attributes);

    case SET_TYPE:
      return build_type_attribute_variant (t1, attributes);
#endif /* GPC */

    case POINTER_TYPE:
      /* For two pointers, do this recursively on the target type,
         and combine the qualifiers of the two types' targets.  */
      /* This code was turned off; I don't know why.
         But ANSI C specifies doing this with the qualifiers.
         So I turned it on again.  */
      {
        tree target = common_type (TYPE_MAIN_VARIANT (TREE_TYPE (t1)),
                                   TYPE_MAIN_VARIANT (TREE_TYPE (t2)));
        int constp
          = TYPE_READONLY (TREE_TYPE (t1)) || TYPE_READONLY (TREE_TYPE (t2));
        int volatilep
          = TYPE_VOLATILE (TREE_TYPE (t1)) || TYPE_VOLATILE (TREE_TYPE (t2));
        t1 = build_pointer_type (build_c_type_variant (target, constp,
                                 volatilep));
        return build_type_attribute_variant (t1, attributes);
      }
#if 0
      t1 = build_pointer_type (common_type (TREE_TYPE (t1), TREE_TYPE (t2)));
      return build_type_attribute_variant (t1, attributes);
#endif

    case ARRAY_TYPE:
      {
        tree elt = common_type (TREE_TYPE (t1), TREE_TYPE (t2));
        /* Save space: see if the result is identical to one of the args.  */
        if (elt == TREE_TYPE (t1) && TYPE_DOMAIN (t1))
          return build_type_attribute_variant (t1, attributes);
        if (elt == TREE_TYPE (t2) && TYPE_DOMAIN (t2))
          return build_type_attribute_variant (t2, attributes);
        /* Merge the element types, and have a size if either arg has one.  */
        t1 = build_array_type (elt, TYPE_DOMAIN (TYPE_DOMAIN (t1) ? t1 : t2));
        return build_type_attribute_variant (t1, attributes);
      }

    case FUNCTION_TYPE:
      /* Function types: prefer the one that specified arg types.
         If both do, merge the arg types.  Also merge the return types.  */
      {
        tree valtype = common_type (TREE_TYPE (t1), TREE_TYPE (t2));
        tree p1 = TYPE_ARG_TYPES (t1);
        tree p2 = TYPE_ARG_TYPES (t2);
        int len;
        tree newargs, n;
        int i;

        /* Save space: see if the result is identical to one of the args.  */
        if (valtype == TREE_TYPE (t1) && ! TYPE_ARG_TYPES (t2))
          return build_type_attribute_variant (t1, attributes);
        if (valtype == TREE_TYPE (t2) && ! TYPE_ARG_TYPES (t1))
          return build_type_attribute_variant (t2, attributes);

        /* Simple way if one arg fails to specify argument types.  */
        if (TYPE_ARG_TYPES (t1) == 0)
         {
           t1 = build_function_type (valtype, TYPE_ARG_TYPES (t2));
           return build_type_attribute_variant (t1, attributes);
         }
        if (TYPE_ARG_TYPES (t2) == 0)
         {
           t1 = build_function_type (valtype, TYPE_ARG_TYPES (t1));
           return build_type_attribute_variant (t1, attributes);
         }

        /* If both args specify argument types, we must merge the two
           lists, argument by argument.  */

        len = list_length (p1);
        newargs = 0;

        for (i = 0; i < len; i++)
          newargs = tree_cons (NULL_TREE, NULL_TREE, newargs);

        n = newargs;

        for (; p1;
             p1 = TREE_CHAIN (p1), p2 = TREE_CHAIN (p2), n = TREE_CHAIN (n))
          {
#ifdef GPC
            /* In Pascal, both types must have been specified.
             */
            if (TREE_VALUE (p1) == 0 || TREE_VALUE (p2) == 0)
              abort ();

            /* Take the first type and avoid a recursive call to
             * common_type() which does too much.
             * For compatible function types, these must match anyway.
             * @@ re-check it here
             * (@@ Maybe it would be better to let common_type() do less?)
             */
            TREE_VALUE (n) = TREE_VALUE (p1);
            goto parm_done;

#else /* not GPC */
            /* A null type means arg type is not specified.
               Take whatever the other function type has.  */
            if (TREE_VALUE (p1) == 0)
              {
                TREE_VALUE (n) = TREE_VALUE (p2);
                goto parm_done;
              }
            if (TREE_VALUE (p2) == 0)
              {
                TREE_VALUE (n) = TREE_VALUE (p1);
                goto parm_done;
              }
#endif /* not GPC */

            /* Given  wait (union {union wait *u; int *i} *)
               and  wait (union wait *),
               prefer  union wait *  as type of parm.  */
            if (TREE_CODE (TREE_VALUE (p1)) == UNION_TYPE
                && TREE_VALUE (p1) != TREE_VALUE (p2))
              {
                tree memb;
                for (memb = TYPE_FIELDS (TREE_VALUE (p1));
                     memb; memb = TREE_CHAIN (memb))
                  if (comptypes (TREE_TYPE (memb), TREE_VALUE (p2)))
                    {
                      TREE_VALUE (n) = TREE_VALUE (p2);
                      if (pedantic)
                        pedwarn ("function types not truly compatible in ANSI C");
                      goto parm_done;
                    }
              }
            if (TREE_CODE (TREE_VALUE (p2)) == UNION_TYPE
                && TREE_VALUE (p2) != TREE_VALUE (p1))
              {
                tree memb;
                for (memb = TYPE_FIELDS (TREE_VALUE (p2));
                     memb; memb = TREE_CHAIN (memb))
                  if (comptypes (TREE_TYPE (memb), TREE_VALUE (p1)))
                    {
                      TREE_VALUE (n) = TREE_VALUE (p1);
                      if (pedantic)
                        pedwarn ("function types not truly compatible in ANSI C");
                      goto parm_done;
                    }
              }
            TREE_VALUE (n) = common_type (TREE_VALUE (p1), TREE_VALUE (p2));
          parm_done: ;
          }

        t1 = build_function_type (valtype, newargs);
        /* ... falls through ...  */
      }

    default:
      return build_type_attribute_variant (t1, attributes);
    }

}

/* Return 1 if TYPE1 and TYPE2 are compatible types for assignment
   or various other operations.  Return 2 if they are compatible
   but a warning may be needed if you use them together.  */

int
comptypes (type1, type2)
     tree type1, type2;
{
  register tree t1 = type1;
  register tree t2 = type2;
  int attrval, val;

  /* Suppress errors caused by previously reported errors.  */

  if (t1 == t2 || TREE_CODE (t1) == ERROR_MARK || TREE_CODE (t2) == ERROR_MARK)
    return 1;

#ifdef GPC

  /* Enumeral types must match almost exactly.
   */
  if (TREE_CODE (t1) == ENUMERAL_TYPE || TREE_CODE (t2) == ENUMERAL_TYPE)
    return TYPE_VALUES (base_type (t1)) == TYPE_VALUES (base_type (t2)) ? 1 : 0;

#else /* not GPC */

  /* Treat an enum type as the integer type of the same width and
     signedness.  */

  if (TREE_CODE (t1) == ENUMERAL_TYPE)
    t1 = type_for_size (TYPE_PRECISION (t1), TREE_UNSIGNED (t1));
  if (TREE_CODE (t2) == ENUMERAL_TYPE)
    t2 = type_for_size (TYPE_PRECISION (t2), TREE_UNSIGNED (t2));

#endif /* not GPC */

  if (t1 == t2)
    return 1;

  /* Different classes of types can't be compatible.  */

  if (TREE_CODE (t1) != TREE_CODE (t2)) return 0;

#ifdef GPC
  if ((TREE_CODE (t1) == ARRAY_TYPE || TREE_CODE (t1) == RECORD_TYPE)
      && PASCAL_TYPE_PACKED (t1) != PASCAL_TYPE_PACKED (t2))
    return 0;
#endif

  /* Qualifiers must match.  */

  if (TYPE_READONLY (t1) != TYPE_READONLY (t2))
    return 0;
  if (TYPE_VOLATILE (t1) != TYPE_VOLATILE (t2))
    return 0;

  /* Allow for two different type nodes which have essentially the same
     definition.  Note that we already checked for equality of the type
     qualifiers (just above).  */

  if (TYPE_MAIN_VARIANT (t1) == TYPE_MAIN_VARIANT (t2))
    return 1;

#ifdef GPC
  attrval = 0;
#else /* not GPC */
#ifndef COMP_TYPE_ATTRIBUTES
#define COMP_TYPE_ATTRIBUTES(t1,t2)     1
#endif

  /* 1 if no need for warning yet, 2 if warning cause has been seen.  */
  if (! (attrval = COMP_TYPE_ATTRIBUTES (t1, t2)))
     return 0;
#endif /* not GPC */

  /* 1 if no need for warning yet, 2 if warning cause has been seen.  */
  val = 0;

  switch (TREE_CODE (t1))
    {
#ifdef GPC
    case INTEGER_TYPE:  /* All integer types are compatible. */
    case CHAR_TYPE:  /* All char types are compatible. */
    case BOOLEAN_TYPE:  /* All Boolean types are compatible. */
      if (TREE_CODE (t1) == TREE_CODE (t2))
        return 1;
      break;

    /* @@@@ Check SET_TYPE & FILE_TYPE more.
     */
    case SET_TYPE:
      val = (TREE_TYPE (t1) == TREE_TYPE (t2)
              ? 1 : TREE_TYPE (t1) == void_type_node  /* empty set */
                    || TREE_TYPE (t2) == void_type_node
                    || comptypes (TREE_TYPE (t1), TREE_TYPE (t2)));
      break;

    case FILE_TYPE:
      val = (TREE_TYPE (t1) == TREE_TYPE (t2)
              ? 1 : comptypes (TREE_TYPE (t1), TREE_TYPE (t2)));
      break;

    /* Complex type may be mixed with real and integer */
    /* @@@: But does it need some warnings? */
    case COMPLEX_TYPE:
      return TREE_CODE (t2) == COMPLEX_TYPE ||
             TREE_CODE (t2) == INTEGER_TYPE ||
             TREE_CODE (t2) == REAL_TYPE;
#endif /* GPC */

    case POINTER_TYPE:
#ifdef GPC
    case REFERENCE_TYPE:
      if (! TREE_TYPE (t1) || ! (TREE_TYPE (t2)))
        {
          if (we_are_loading_a_gpi_file)
            {
              /* @@@@@@  This happens during a method of an object is
               * being loaded:  The object itself is not yet complete,
               * but the method needs an implicit `Self' parameter
               * which is a reference to the object.
               * A clean solution to this would be to generalize load_tree()
               * and store_tree() to load and store just everything, so we
               * do not need to simulate the parser's behaviour any more.
               * This would both simplify and speed up GPI at the same time.
               */
              return 1;
            }
          else
            abort ();
        }
#endif /* GPC */
      val = (TREE_TYPE (t1) == TREE_TYPE (t2)
              ? 1 : comptypes (TREE_TYPE (t1), TREE_TYPE (t2)));
      break;

    case FUNCTION_TYPE:
      val = function_types_compatible_p (t1, t2);
      break;

    case ARRAY_TYPE:
      {
        tree d1 = TYPE_DOMAIN (t1);
        tree d2 = TYPE_DOMAIN (t2);
        val = 1;

        /* Target types must match incl. qualifiers.  */
        if (TREE_TYPE (t1) != TREE_TYPE (t2)
            && 0 == (val = comptypes (TREE_TYPE (t1), TREE_TYPE (t2))))
          return 0;

        /* Sizes must match unless one is missing or variable.  */
#ifdef GPC
        /* ... or unless one of both is an "open array" parameter.  */
        if (PASCAL_TYPE_OPEN_ARRAY (t1) || PASCAL_TYPE_OPEN_ARRAY (t2))
          break;
#endif /* GPC */

        if (d1 == 0 || d2 == 0 || d1 == d2
            || TREE_CODE (TYPE_MIN_VALUE (d1)) != INTEGER_CST
            || TREE_CODE (TYPE_MIN_VALUE (d2)) != INTEGER_CST
            || TREE_CODE (TYPE_MAX_VALUE (d1)) != INTEGER_CST
            || TREE_CODE (TYPE_MAX_VALUE (d2)) != INTEGER_CST)
          break;

        if (! ((TREE_INT_CST_LOW (TYPE_MIN_VALUE (d1))
                  == TREE_INT_CST_LOW (TYPE_MIN_VALUE (d2)))
                 && (TREE_INT_CST_HIGH (TYPE_MIN_VALUE (d1))
                     == TREE_INT_CST_HIGH (TYPE_MIN_VALUE (d2)))
                 && (TREE_INT_CST_LOW (TYPE_MAX_VALUE (d1))
                     == TREE_INT_CST_LOW (TYPE_MAX_VALUE (d2)))
                 && (TREE_INT_CST_HIGH (TYPE_MAX_VALUE (d1))
                     == TREE_INT_CST_HIGH (TYPE_MAX_VALUE (d2)))))
           val = 0;
        break;
      }

    case RECORD_TYPE:
#ifdef GPC
      /* If one and the same object is loaded from two or more
       * GPI files, it might have two RECORD_TYPE nodes.
       */
      if (PASCAL_TYPE_OBJECT (t1)
          && PASCAL_TYPE_OBJECT (t2)
          && TYPE_LANG_NAME (t1) == TYPE_LANG_NAME (t2))
        val = 1;

      /* The same problem can also occur with ordinary records, strings,
       * and schemata.  Check it the hard way.  :-(
       */
      else if (PASCAL_TYPE_STRING (t1) && PASCAL_TYPE_STRING (t2))
        {
          if (PASCAL_TYPE_UNDISCRIMINATED_STRING (t1)
              || PASCAL_TYPE_UNDISCRIMINATED_STRING (t2))
            val = 1;
          else
            {
              /* Compare the arrays.
               */
              tree field1 = TREE_CHAIN (TREE_CHAIN (TYPE_FIELDS (t1)));
              tree field2 = TREE_CHAIN (TREE_CHAIN (TYPE_FIELDS (t2)));
              val = comptypes (TREE_TYPE (field1), TREE_TYPE (field2));
            }
        }
      else if (! PASCAL_TYPE_OBJECT (t1) && ! PASCAL_TYPE_OBJECT (t2))
        {
          tree field1, field2;
          for (field1 = TYPE_FIELDS (t1), field2 = TYPE_FIELDS (t2);
               field1 && field2;
               field1 = TREE_CHAIN (field1), field2 = TREE_CHAIN (field2))
            {
              if (DECL_NAME (field1) != DECL_NAME (field2)
                  || (TYPE_NAME (TREE_TYPE (field1))
                      && TYPE_NAME (TREE_TYPE (field2))
                      && TYPE_NAME (TREE_TYPE (field1)) != TYPE_NAME (TREE_TYPE (field2)))
#if 0  /* This leads to an infinite loop in the case of a recursive type. )-: */
                  || comptypes (TREE_TYPE (field1), TREE_TYPE (field2)) == 0)
#else  /* This, OTOH, is not strong enough :-( */
                  || TREE_CODE (TREE_TYPE (field1)) != TREE_CODE (TREE_TYPE (field2)))
#endif  /* )-: */
              break;
            }
          if (field1 == NULL_TREE && field2 == NULL_TREE)
            val = 1;
        }
#else /* not GPC */
      if (maybe_objc_comptypes (t1, t2, 0) == 1)
        val = 1;
#endif /* GPC */
      break;

    default:
      break;
    }
  return attrval == 2 && val == 1 ? 2 : val;
}

/* Return 1 if TTL and TTR are pointers to types that are equivalent,
   ignoring their qualifiers.  */

static int
comp_target_types (ttl, ttr)
     tree ttl, ttr;
{
  int val;

  /* Give maybe_objc_comptypes a crack at letting these types through.  */
  if ((val = maybe_objc_comptypes (ttl, ttr, 1)) >= 0)
    return val;

#ifdef GPC
  if (TREE_CODE (TREE_TYPE (ttl)) == INTEGER_TYPE
      && TREE_CODE (TREE_TYPE (ttr)) == INTEGER_TYPE)
    {
      /* While all integer types are assignment compatible, pointers to
       * integers only match if the integers have the same precision and sign.
       */
      if (TYPE_PRECISION (TREE_TYPE (ttl)) != TYPE_PRECISION (TREE_TYPE (ttr))
          || TREE_UNSIGNED (TREE_TYPE (ttl)) != TREE_UNSIGNED (TREE_TYPE (ttr)))
        return 0;
    }
  if (TREE_CODE (TREE_TYPE (ttl)) == POINTER_TYPE
      && TREE_CODE (TREE_TYPE (ttr)) == POINTER_TYPE)
    val = comp_target_types (TREE_TYPE (ttl),
                             TREE_TYPE (ttr));
  else
    val = comptypes (TYPE_MAIN_VARIANT (TREE_TYPE (ttl)),
                     TYPE_MAIN_VARIANT (TREE_TYPE (ttr)));
#else /* not GPC */
  val = comptypes (TYPE_MAIN_VARIANT (TREE_TYPE (ttl)),
                   TYPE_MAIN_VARIANT (TREE_TYPE (ttr)));
#endif /* not GPC */

  if (val == 2 && pedantic)
    pedwarn ("types are not quite compatible");
  return val;
}

#ifdef GPC
/* Return 1 if two pointers to objects are assignment compatible;
 * This is the case if rhs has the same type as lhs or if it
 * has the type of lhs as a parent type.  Otherwise return 0.
 */
static int
comp_object_or_schema_pointer_types (lhs, rhs)
     tree lhs, rhs;
{
  register tree r;
  if (lhs == rhs
      || TREE_CODE (lhs) == ERROR_MARK
      || TREE_CODE (rhs) == ERROR_MARK)
    return 1;
  else if (PASCAL_TYPE_CONFORMANT_ARRAY (lhs)
      && TREE_CODE (rhs) == ARRAY_TYPE)
    {
      if (comptypes (TYPE_DOMAIN (lhs), TYPE_DOMAIN (rhs))
          && comptypes (TREE_TYPE (lhs), TREE_TYPE (rhs)))
        return 1;
      else
        return 0;
    }
  else if ((PASCAL_TYPE_DISCRIMINATED_STRING (lhs)
            && PASCAL_TYPE_DISCRIMINATED_STRING (rhs))
           || (PASCAL_TYPE_DISCRIMINATED_SCHEMA (lhs)
               && PASCAL_TYPE_DISCRIMINATED_SCHEMA (rhs)))
    {
      if (comptypes (lhs, rhs))
        return 1;
    }
  else if ((PASCAL_TYPE_STRING (lhs)
            && PASCAL_TYPE_STRING (rhs))
           || (PASCAL_TYPE_SCHEMA (lhs)
               && PASCAL_TYPE_SCHEMA (rhs)))
    {
      tree rbase;
      if (TYPE_LANG_BASE (rhs))
        rbase = TYPE_MAIN_VARIANT (TYPE_LANG_BASE (rhs));
      else
        rbase = TYPE_MAIN_VARIANT (rhs);
      while (TYPE_LANG_BASE (lhs)
             && TYPE_LANG_BASE (lhs) != lhs)
        {
          if (TYPE_MAIN_VARIANT (TYPE_LANG_BASE (lhs)) == rbase)
            return 1;
          lhs = TYPE_LANG_BASE (lhs);
        }
      if (comptypes (lhs, rbase))
        return 1;
    }
  else if (PASCAL_TYPE_OBJECT (lhs)
           && PASCAL_TYPE_OBJECT (rhs))
    {
      for (r = rhs; r; r = TYPE_LANG_BASE (r))
        {
          if (comptypes (lhs, r))
            return 1;
        }
    }
  return 0;
}

#endif /* GPC */
/* Subroutines of `comptypes'.  */

/* Return 1 if two function types F1 and F2 are compatible.
   If either type specifies no argument types,
   the other must specify a fixed number of self-promoting arg types.
   Otherwise, if one type specifies only the number of arguments,
   the other must specify that number of self-promoting arg types.
   Otherwise, the argument types must match.  */

static int
function_types_compatible_p (f1, f2)
     tree f1, f2;
{
  tree args1, args2;
  /* 1 if no need for warning yet, 2 if warning cause has been seen.  */
  int val = 1;
  int val1;

  if (!(TREE_TYPE (f1) == TREE_TYPE (f2)
        || (val = comptypes (TREE_TYPE (f1), TREE_TYPE (f2)))))
    return 0;

  args1 = TYPE_ARG_TYPES (f1);
  args2 = TYPE_ARG_TYPES (f2);

  /* An unspecified parmlist matches any specified parmlist
     whose argument types don't need default promotions.  */

  if (args1 == 0)
    {
      if (!self_promoting_args_p (args2))
        return 0;
      /* If one of these types comes from a non-prototype fn definition,
         compare that with the other type's arglist.
         If they don't match, ask for a warning (but no error).  */
      if (TYPE_ACTUAL_ARG_TYPES (f1)
          && 1 != type_lists_compatible_p (args2, TYPE_ACTUAL_ARG_TYPES (f1)))
        val = 2;
      return val;
    }
  if (args2 == 0)
    {
      if (!self_promoting_args_p (args1))
        return 0;
      if (TYPE_ACTUAL_ARG_TYPES (f2)
          && 1 != type_lists_compatible_p (args1, TYPE_ACTUAL_ARG_TYPES (f2)))
        val = 2;
      return val;
    }

  /* Both types have argument lists: compare them and propagate results.  */
  val1 = type_lists_compatible_p (args1, args2);
  return val1 != 1 ? val1 : val;
}

/* Check two lists of types for compatibility,
   returning 0 for incompatible, 1 for compatible,
   or 2 for compatible with warning.  */

static int
type_lists_compatible_p (args1, args2)
     tree args1, args2;
{
  /* 1 if no need for warning yet, 2 if warning cause has been seen.  */
  int val = 1;
  int newval = 0;

  while (1)
    {
      if (args1 == 0 && args2 == 0)
        return val;
      /* If one list is shorter than the other,
         they fail to match.  */
      if (args1 == 0 || args2 == 0)
        return 0;
      /* A null pointer instead of a type
         means there is supposed to be an argument
         but nothing is specified about what type it has.
         So match anything that self-promotes.  */
      if (TREE_VALUE (args1) == 0)
        {
          if (! self_promoting_type_p (TREE_VALUE (args2)))
            return 0;
        }
      else if (TREE_VALUE (args2) == 0)
        {
          if (! self_promoting_type_p (TREE_VALUE (args1)))
            return 0;
        }
      else if (! (newval = comptypes (TREE_VALUE (args1), TREE_VALUE (args2))))
        {
          /* Allow  wait (union {union wait *u; int *i} *)
             and  wait (union wait *)  to be compatible.  */
          if (TREE_CODE (TREE_VALUE (args1)) == UNION_TYPE
              && (TYPE_NAME (TREE_VALUE (args1)) == 0
                  || TYPE_TRANSPARENT_UNION (TREE_VALUE (args1)))
              && TREE_CODE (TYPE_SIZE (TREE_VALUE (args1))) == INTEGER_CST
              && tree_int_cst_equal (TYPE_SIZE (TREE_VALUE (args1)),
                                     TYPE_SIZE (TREE_VALUE (args2))))
            {
              tree memb;
              for (memb = TYPE_FIELDS (TREE_VALUE (args1));
                   memb; memb = TREE_CHAIN (memb))
                if (comptypes (TREE_TYPE (memb), TREE_VALUE (args2)))
                  break;
              if (memb == 0)
                return 0;
            }
          else if (TREE_CODE (TREE_VALUE (args2)) == UNION_TYPE
                   && (TYPE_NAME (TREE_VALUE (args2)) == 0
                       || TYPE_TRANSPARENT_UNION (TREE_VALUE (args2)))
                   && TREE_CODE (TYPE_SIZE (TREE_VALUE (args2))) == INTEGER_CST
                   && tree_int_cst_equal (TYPE_SIZE (TREE_VALUE (args2)),
                                          TYPE_SIZE (TREE_VALUE (args1))))
            {
              tree memb;
              for (memb = TYPE_FIELDS (TREE_VALUE (args2));
                   memb; memb = TREE_CHAIN (memb))
                if (comptypes (TREE_TYPE (memb), TREE_VALUE (args1)))
                  break;
              if (memb == 0)
                return 0;
            }
          else
            return 0;
        }

      /* comptypes said ok, but record if it said to warn.  */
      if (newval > val)
        val = newval;

      args1 = TREE_CHAIN (args1);
      args2 = TREE_CHAIN (args2);
    }
}

/* Return 1 if PARMS specifies a fixed number of parameters
   and none of their types is affected by default promotions.  */

int
self_promoting_args_p (parms)
     tree parms;
{
  register tree t;
  for (t = parms; t; t = TREE_CHAIN (t))
    {
      register tree type = TREE_VALUE (t);

      if (TREE_CHAIN (t) == 0 && type != void_type_node)
        return 0;

      if (type == 0)
        return 0;

      if (TYPE_MAIN_VARIANT (type) == float_type_node)
        return 0;

      if (C_PROMOTING_INTEGER_TYPE_P (type))
        return 0;
    }
  return 1;
}

/* Return 1 if TYPE is not affected by default promotions.  */

static int
self_promoting_type_p (type)
     tree type;
{
  if (TYPE_MAIN_VARIANT (type) == float_type_node)
    return 0;

  if (C_PROMOTING_INTEGER_TYPE_P (type))
    return 0;

  return 1;
}

/* Return an unsigned type the same as TYPE in other respects.  */

tree
unsigned_type (type)
     tree type;
{
  tree type1 = TYPE_MAIN_VARIANT (type);
  if (type1 == signed_char_type_node || type1 == char_type_node)
    return unsigned_char_type_node;
  if (type1 == integer_type_node)
    return unsigned_type_node;
  if (type1 == short_integer_type_node)
    return short_unsigned_type_node;
  if (type1 == long_integer_type_node)
    return long_unsigned_type_node;
  if (type1 == long_long_integer_type_node)
    return long_long_unsigned_type_node;
  if (type1 == intDI_type_node)
    return unsigned_intDI_type_node;
  if (type1 == intSI_type_node)
    return unsigned_intSI_type_node;
  if (type1 == intHI_type_node)
    return unsigned_intHI_type_node;
  if (type1 == intQI_type_node)
    return unsigned_intQI_type_node;

  return signed_or_unsigned_type (1, type);
}

/* Return a signed type the same as TYPE in other respects.  */

tree
signed_type (type)
     tree type;
{
  tree type1 = TYPE_MAIN_VARIANT (type);
  if (type1 == unsigned_char_type_node || type1 == char_type_node)
    return signed_char_type_node;
  if (type1 == unsigned_type_node)
    return integer_type_node;
  if (type1 == short_unsigned_type_node)
    return short_integer_type_node;
  if (type1 == long_unsigned_type_node)
    return long_integer_type_node;
  if (type1 == long_long_unsigned_type_node)
    return long_long_integer_type_node;
  if (type1 == unsigned_intDI_type_node)
    return intDI_type_node;
  if (type1 == unsigned_intSI_type_node)
    return intSI_type_node;
  if (type1 == unsigned_intHI_type_node)
    return intHI_type_node;
  if (type1 == unsigned_intQI_type_node)
    return intQI_type_node;

  return signed_or_unsigned_type (0, type);
}

/* Return a type the same as TYPE except unsigned or
   signed according to UNSIGNEDP.  */

tree
signed_or_unsigned_type (unsignedp, type)
     int unsignedp;
     tree type;
{
  if (! INTEGRAL_TYPE_P (type)
      || TREE_UNSIGNED (type) == unsignedp)
    return type;
  if (TYPE_PRECISION (type) == TYPE_PRECISION (signed_char_type_node))
    return unsignedp ? unsigned_char_type_node : signed_char_type_node;
  if (TYPE_PRECISION (type) == TYPE_PRECISION (integer_type_node))
    return unsignedp ? unsigned_type_node : integer_type_node;
  if (TYPE_PRECISION (type) == TYPE_PRECISION (short_integer_type_node))
    return unsignedp ? short_unsigned_type_node : short_integer_type_node;
  if (TYPE_PRECISION (type) == TYPE_PRECISION (long_integer_type_node))
    return unsignedp ? long_unsigned_type_node : long_integer_type_node;
  if (TYPE_PRECISION (type) == TYPE_PRECISION (long_long_integer_type_node))
    return (unsignedp ? long_long_unsigned_type_node
            : long_long_integer_type_node);
  return type;
}

/* Compute the value of the `sizeof' operator.  */

tree
c_sizeof (type)
     tree type;
{
  enum tree_code code = TREE_CODE (type);
  tree t;

  if (code == FUNCTION_TYPE)
    {
      if (pedantic || warn_pointer_arith)
        pedwarn ("sizeof applied to a function type");
      return size_int (1);
    }
  if (code == VOID_TYPE)
    {
      if (pedantic || warn_pointer_arith)
        pedwarn ("sizeof applied to a void type");
      return size_int (1);
    }
  if (code == ERROR_MARK)
    return size_int (1);
  if (TYPE_SIZE (type) == 0)
    {
      error ("sizeof applied to an incomplete type");
      return size_int (0);
    }

  /* Convert in case a char is more than one unit.  */
  t = size_binop (CEIL_DIV_EXPR, TYPE_SIZE (type),
                  size_int (TYPE_PRECISION (char_type_node)));
  /* size_binop does not put the constant in range, so do it now.  */
  if (TREE_CODE (t) == INTEGER_CST && force_fit_type (t, 0))
    TREE_CONSTANT_OVERFLOW (t) = TREE_OVERFLOW (t) = 1;
  return t;
}

#ifdef GPC
/* Compute the value of the `BitSizeOf' pseudo-function.
 */

tree
bit_sizeof (type)
     tree type;
{
  enum tree_code code = TREE_CODE (type);
  tree t;

  if (code == FUNCTION_TYPE)
    {
      if (pedantic || warn_pointer_arith)
        pedwarn ("`BitSizeOf' applied to a function type");
      return size_int (1);
    }
  if (code == VOID_TYPE)
    {
      if (pedantic || warn_pointer_arith)
        pedwarn ("`BitSizeOf' applied to a void type");
      return size_int (1);
    }
  if (code == ERROR_MARK)
    return size_int (1);
  if (TYPE_SIZE (type) == 0)
    {
      error ("`BitSizeOf' applied to an incomplete type");
      return size_int (0);
    }

  /* Get the size in bits.
   */
  t = TYPE_SIZE (type);

  /* size_binop does not put the constant in range, so do it now.  */
  if (TREE_CODE (t) == INTEGER_CST && force_fit_type (t, 0))
    TREE_CONSTANT_OVERFLOW (t) = TREE_OVERFLOW (t) = 1;
  return t;
}
#endif /* GPC */

tree
c_sizeof_nowarn (type)
     tree type;
{
  enum tree_code code = TREE_CODE (type);
  tree t;

  if (code == FUNCTION_TYPE
      || code == VOID_TYPE
      || code == ERROR_MARK)
    return size_int (1);
  if (TYPE_SIZE (type) == 0)
    return size_int (0);

  /* Convert in case a char is more than one unit.  */
  t = size_binop (CEIL_DIV_EXPR, TYPE_SIZE (type),
                  size_int (TYPE_PRECISION (char_type_node)));
  force_fit_type (t, 0);
  return t;
}

/* Compute the size to increment a pointer by.  */

tree
c_size_in_bytes (type)
     tree type;
{
  enum tree_code code = TREE_CODE (type);
  tree t;

  if (code == FUNCTION_TYPE)
    return size_int (1);
  if (code == VOID_TYPE)
    return size_int (1);
  if (code == ERROR_MARK)
    return size_int (1);
  if (TYPE_SIZE (type) == 0)
    {
      error ("arithmetic on pointer to an incomplete type");
      return size_int (1);
    }

  /* Convert in case a char is more than one unit.  */
  t = size_binop (CEIL_DIV_EXPR, TYPE_SIZE (type),
                     size_int (BITS_PER_UNIT));
  force_fit_type (t, 0);
  return t;
}

/* Implement the __alignof keyword: Return the minimum required
   alignment of TYPE, measured in bytes.  */

tree
c_alignof (type)
     tree type;
{
  enum tree_code code = TREE_CODE (type);

  if (code == FUNCTION_TYPE)
    return size_int (FUNCTION_BOUNDARY / BITS_PER_UNIT);

  if (code == VOID_TYPE || code == ERROR_MARK)
    return size_int (1);

  return size_int (TYPE_ALIGN (type) / BITS_PER_UNIT);
}

/* Implement the __alignof keyword: Return the minimum required
   alignment of EXPR, measured in bytes.  For VAR_DECL's and
   FIELD_DECL's return DECL_ALIGN (which can be set from an
   "aligned" __attribute__ specification).  */

tree
c_alignof_expr (expr)
     tree expr;
{
  if (TREE_CODE (expr) == VAR_DECL)
    return size_int (DECL_ALIGN (expr) / BITS_PER_UNIT);

  if (TREE_CODE (expr) == COMPONENT_REF
      && DECL_C_BIT_FIELD (TREE_OPERAND (expr, 1)))
    {
      error ("`__alignof' applied to a bit-field");
      return size_int (1);
    }
  else if (TREE_CODE (expr) == COMPONENT_REF
      && TREE_CODE (TREE_OPERAND (expr, 1)) == FIELD_DECL)
    return size_int (DECL_ALIGN (TREE_OPERAND (expr, 1)) / BITS_PER_UNIT);

  if (TREE_CODE (expr) == INDIRECT_REF)
    {
      tree t = TREE_OPERAND (expr, 0);
      tree best = t;
      int bestalign = TYPE_ALIGN (TREE_TYPE (TREE_TYPE (t)));

      while (TREE_CODE (t) == NOP_EXPR
              && TREE_CODE (TREE_TYPE (TREE_OPERAND (t, 0))) == POINTER_TYPE)
        {
          int thisalign;

          t = TREE_OPERAND (t, 0);
          thisalign = TYPE_ALIGN (TREE_TYPE (TREE_TYPE (t)));
          if (thisalign > bestalign)
            best = t, bestalign = thisalign;
        }
      return c_alignof (TREE_TYPE (TREE_TYPE (best)));
    }
  else
    return c_alignof (TREE_TYPE (expr));
}

/* Return either DECL or its known constant value (if it has one).  */

static tree
decl_constant_value (decl)
     tree decl;
{
  if (/* Don't change a variable array bound or initial value to a constant
         in a place where a variable is invalid.  */
      current_function_decl != 0
      && ! pedantic
      && ! TREE_THIS_VOLATILE (decl)
      && TREE_READONLY (decl) && ! ITERATOR_P (decl)
      && DECL_INITIAL (decl) != 0
      && TREE_CODE (DECL_INITIAL (decl)) != ERROR_MARK
      /* This is invalid if initial value is not constant.
         If it has either a function call, a memory reference,
         or a variable, then re-evaluating it could give different results.  */
      && TREE_CONSTANT (DECL_INITIAL (decl))
      /* Check for cases where this is sub-optimal, even though valid.  */
      && TREE_CODE (DECL_INITIAL (decl)) != CONSTRUCTOR
      && DECL_MODE (decl) != BLKmode)
    return DECL_INITIAL (decl);
  return decl;
}

/* Perform default promotions for C data used in expressions.
   Arrays and functions are converted to pointers;
   enumeral types or short or char, to int.
   In addition, manifest constants symbols are replaced by their values.  */

tree
default_conversion (exp)
     tree exp;
{
  register tree type = TREE_TYPE (exp);
  register enum tree_code code = TREE_CODE (type);

  /* Constants can be used directly unless they're not loadable.  */
  if (TREE_CODE (exp) == CONST_DECL)
    exp = DECL_INITIAL (exp);

  /* Replace a nonvolatile const static variable with its value unless
     it is an array, in which case we must be sure that taking the
     address of the array produces consistent results.  */
  else if (optimize && TREE_CODE (exp) == VAR_DECL && code != ARRAY_TYPE)
    {
      exp = decl_constant_value (exp);
      type = TREE_TYPE (exp);
    }

  /* Strip NON_LVALUE_EXPRs and no-op conversions, since we aren't using as
     an lvalue.  */
  /* Do not use STRIP_NOPS here!  It will remove conversions from pointer
     to integer and cause infinite recursion.  */
  while (TREE_CODE (exp) == NON_LVALUE_EXPR
         || (TREE_CODE (exp) == NOP_EXPR
             && TREE_TYPE (TREE_OPERAND (exp, 0)) == TREE_TYPE (exp)))
    exp = TREE_OPERAND (exp, 0);

  /* Normally convert enums to int,
     but convert wide enums to something wider.  */
#ifdef GPC
  if (ORDINAL_TYPE (code))
#else
  if (code == ENUMERAL_TYPE)
#endif /* GPC */
    {
      type = type_for_size (MAX (TYPE_PRECISION (type),
                                 TYPE_PRECISION (integer_type_node)),
                            ((flag_traditional
                              || (TYPE_PRECISION (type)
                                  >= TYPE_PRECISION (integer_type_node)))
                             && TREE_UNSIGNED (type)));
      return convert (type, exp);
    }

  if (TREE_CODE (exp) == COMPONENT_REF
      && DECL_C_BIT_FIELD (TREE_OPERAND (exp, 1)))
    {
      tree width = DECL_SIZE (TREE_OPERAND (exp, 1));
      HOST_WIDE_INT low = TREE_INT_CST_LOW (width);

      /* If it's thinner than an int, promote it like a
         C_PROMOTING_INTEGER_TYPE_P, otherwise leave it alone.  */

      if (low < TYPE_PRECISION (integer_type_node))
        {
          if (flag_traditional && TREE_UNSIGNED (type))
            return convert (unsigned_type_node, exp);
          else
            return convert (integer_type_node, exp);
        }
    }

  if (C_PROMOTING_INTEGER_TYPE_P (type))
    {
      /* Traditionally, unsignedness is preserved in default promotions.
         Also preserve unsignedness if not really getting any wider.  */
      if (TREE_UNSIGNED (type)
          && (flag_traditional
              || TYPE_PRECISION (type) == TYPE_PRECISION (integer_type_node)))
        return convert (unsigned_type_node, exp);
      return convert (integer_type_node, exp);
    }
  if (flag_traditional && !flag_allow_single_precision
      && TYPE_MAIN_VARIANT (type) == float_type_node)
    return convert (double_type_node, exp);
  if (code == VOID_TYPE)
    {
      error ("void value not ignored as it ought to be");
      return error_mark_node;
    }
#ifdef GPC
  /* @@@ Note: Now that the reference type works in this gcc version,
     maybe I should re-implement var parameter code once more. */

  /* Get rid of var parameter REFERENCE_TYPE */
  if (code == REFERENCE_TYPE)
    return convert (build_pointer_type (TREE_TYPE (type)), exp);

  /* No array and function conversion in GPC.
   */
  if (code == ARRAY_TYPE || code == FUNCTION_TYPE)
    return exp;
#else /* not GPC */
  if (code == FUNCTION_TYPE)
    {
      return build_unary_op (ADDR_EXPR, exp, 0);
    }
#endif /* not GPC */
  if (code == ARRAY_TYPE)
    {
      register tree adr;
      tree restype = TREE_TYPE (type);
      tree ptrtype;
      int constp = 0;
      int volatilep = 0;

      if (TREE_CODE_CLASS (TREE_CODE (exp)) == 'r'
          || TREE_CODE_CLASS (TREE_CODE (exp)) == 'd')
        {
          constp = TREE_READONLY (exp);
          volatilep = TREE_THIS_VOLATILE (exp);
        }

      if (TYPE_READONLY (type) || TYPE_VOLATILE (type)
          || constp || volatilep)
        restype = build_c_type_variant (restype,
                                        TYPE_READONLY (type) || constp,
                                        TYPE_VOLATILE (type) || volatilep);

      if (TREE_CODE (exp) == INDIRECT_REF)
        return convert (TYPE_POINTER_TO (restype),
                        TREE_OPERAND (exp, 0));

      if (TREE_CODE (exp) == COMPOUND_EXPR)
        {
          tree op1 = default_conversion (TREE_OPERAND (exp, 1));
          return build (COMPOUND_EXPR, TREE_TYPE (op1),
                        TREE_OPERAND (exp, 0), op1);
        }

      if (! lvalue_p (exp)
          && ! (TREE_CODE (exp) == CONSTRUCTOR && TREE_STATIC (exp)))
        {
          error ("invalid use of non-lvalue array");
          return error_mark_node;
        }

      ptrtype = build_pointer_type (restype);

      if (TREE_CODE (exp) == VAR_DECL)
        {
          /* ??? This is not really quite correct
             in that the type of the operand of ADDR_EXPR
             is not the target type of the type of the ADDR_EXPR itself.
             Question is, can this lossage be avoided?  */
          adr = build1 (ADDR_EXPR, ptrtype, exp);
          if (mark_addressable (exp) == 0)
            return error_mark_node;
          TREE_CONSTANT (adr) = staticp (exp);
          TREE_SIDE_EFFECTS (adr) = 0;   /* Default would be, same as EXP.  */
          return adr;
        }
      /* This way is better for a COMPONENT_REF since it can
         simplify the offset for a component.  */
      adr = build_unary_op (ADDR_EXPR, exp, 1);
      return convert (ptrtype, adr);
    }
  return exp;
}

/* Look up component name in the structure type definition.

   If this component name is found indirectly within an anonymous union,
   store in *INDIRECT the component which directly contains
   that anonymous union.  Otherwise, set *INDIRECT to 0.  */

static tree
lookup_field (type, component, indirect)
     tree type, component;
     tree *indirect;
{
  tree field;

#ifndef GPC
  /* If TYPE_LANG_SPECIFIC is set, then it is a sorted array of pointers
     to the field elements.  Use a binary search on this array to quickly
     find the element.  Otherwise, do a linear search.  TYPE_LANG_SPECIFIC
     will always be set for structures which have many elements.  */

  if (TYPE_LANG_SPECIFIC (type))
    {
      int bot, top, half;
      tree *field_array = &TYPE_LANG_SPECIFIC (type)->elts[0];

      field = TYPE_FIELDS (type);
      bot = 0;
      top = TYPE_LANG_SPECIFIC (type)->len;
      while (top - bot > 1)
        {
          half = (top - bot + 1) >> 1;
          field = field_array[bot+half];

          if (DECL_NAME (field) == NULL_TREE)
            {
              /* Step through all anon unions in linear fashion.  */
              while (DECL_NAME (field_array[bot]) == NULL_TREE)
                {
                  tree anon = 0, junk;

                  field = field_array[bot++];
                  if (TREE_CODE (TREE_TYPE (field)) == RECORD_TYPE
                      || TREE_CODE (TREE_TYPE (field)) == UNION_TYPE)
                    anon = lookup_field (TREE_TYPE (field), component, &junk);

                  if (anon != NULL_TREE)
                    {
                      *indirect = field;
                      return anon;
                    }
                }

              /* Entire record is only anon unions.  */
              if (bot > top)
                return NULL_TREE;

              /* Restart the binary search, with new lower bound.  */
              continue;
            }

          if (DECL_NAME (field) == component)
            break;
          if (DECL_NAME (field) < component)
            bot += half;
          else
            top = bot + half;
        }

      if (DECL_NAME (field_array[bot]) == component)
        field = field_array[bot];
      else if (DECL_NAME (field) != component)
        field = 0;
    }
  else
#endif /* not GPC */
    {
      for (field = TYPE_FIELDS (type); field; field = TREE_CHAIN (field))
        {
          if (DECL_NAME (field) == NULL_TREE)
            {
              tree junk;
              tree anon = 0;

              if (TREE_CODE (TREE_TYPE (field)) == RECORD_TYPE
                  || TREE_CODE (TREE_TYPE (field)) == UNION_TYPE)
                anon = lookup_field (TREE_TYPE (field), component, &junk);

              if (anon != NULL_TREE)
                {
                  *indirect = field;
                  return anon;
                }
            }

          if (DECL_NAME (field) == component)
            break;
        }
    }

  *indirect = NULL_TREE;
  return field;
}

#ifdef GPC
/* Convert ARRAY_TYPE to POINTER_TYPE.
 * This code is equivalent to the code for C-arrays
 * in default_conversion () above.
 */
tree
convert_array_to_pointer (exp)
     tree exp;
{
  register tree type = TREE_TYPE (exp);
  register enum tree_code code = TREE_CODE (type);

  if (code == ARRAY_TYPE)
    {
      register tree adr;
      tree restype = TREE_TYPE (type);
      tree ptrtype;
      int constp = 0;
      int volatilep = 0;

      if (TREE_CODE_CLASS (TREE_CODE (exp)) == 'r'
          || TREE_CODE_CLASS (TREE_CODE (exp)) == 'd')
        {
          constp = TREE_READONLY (exp);
          volatilep = TREE_THIS_VOLATILE (exp);
        }

      if (TYPE_READONLY (type) || TYPE_VOLATILE (type)
          || constp || volatilep)
        restype = build_c_type_variant (restype,
                                        TYPE_READONLY (type) || constp,
                                        TYPE_VOLATILE (type) || volatilep);

      ptrtype = build_pointer_type (restype);

      if (TREE_CODE (exp) == INDIRECT_REF)
        return convert (ptrtype, TREE_OPERAND (exp, 0));

      if (TREE_CODE (exp) == COMPOUND_EXPR)
        {
          tree op1 = default_conversion (TREE_OPERAND (exp, 1));
          return build (COMPOUND_EXPR, TREE_TYPE (op1),
                        TREE_OPERAND (exp, 0), op1);
        }

      if (!lvalue_p (exp)
          && ! (TREE_CODE (exp) == CONSTRUCTOR && TREE_STATIC (exp)))
        {
          error ("invalid use of non-lvalue array");
          return error_mark_node;
        }

      if (TREE_CODE (exp) == VAR_DECL)
        {
          /* ??? This is not really quite correct
             in that the type of the operand of ADDR_EXPR
             is not the target type of the type of the ADDR_EXPR itself.
             Question is, can this lossage be avoided?  */
          adr = build1 (ADDR_EXPR, ptrtype, exp);
          if (mark_addressable (exp) == 0)
            return error_mark_node;
          TREE_CONSTANT (adr) = staticp (exp);
          TREE_SIDE_EFFECTS (adr) = 0;   /* Default would be, same as EXP.  */
          return adr;
        }
      /* This way is better for a COMPONENT_REF since it can
         simplify the offset for a component.  */
      adr = build_unary_op (ADDR_EXPR, exp, 1);
      return convert (ptrtype, adr);
    }
  /* OOPS: not an array */
  return exp;
}

/* Make an expression to refer to the COMPONENT field of
 * structure or union value DATUM.  COMPONENT is an IDENTIFIER_NODE.
 */
tree
find_field (datum, component)
    tree datum, component;
{
  register tree basename = datum;
  register tree basetype = TREE_TYPE (basename);
  register enum tree_code form = TREE_CODE (basetype);
  register tree field = NULL_TREE;

  /* First, see if there is a field or component with name COMPONENT.
   *
   * @@@@ This code does not check all fields of the current level
   * before propagating to inner levels.  This is not necessary for Pascal
   * since the fields in a variant record appear in the same scope
   * as those outside the variant part.
   */

  if (form == RECORD_TYPE || form == UNION_TYPE)
    {
      for (field = TYPE_FIELDS (basetype); field; field = TREE_CHAIN (field))
        {
          if (DECL_NAME (field) == component)
            return build_tree_list (NULL_TREE, field);
          else if (DECL_NAME (field) == NULL_TREE)
            {
              tree ret;
              ret = find_field (field, component);

              /* We need to include the invisible UNION_TYPE to the list of
               * fields we need to build_component_ref of to reach the field.
               *
               * Otherwise the 'bitpos' offset field will be incorrect
               * and the access is made to wrong memory location.
               */
              if (ret)
                return chainon (ret, build_tree_list (NULL_TREE, field));
            }
        }
    }
  return NULL_TREE; /* No such implicit or explicit field or wrong type */
}

/* Check if selected type codes are compatible in various
 * contexts.
 *
 * String arguments should not come here.
 *
 * @@@ Should perhaps rewrite the type checking functions
 * @@@ instead of patching them like this, sigh.
 */

static int
compatible_types_p (type0, type1)
     tree type0, type1;
{
  enum tree_code code0 = TREE_CODE (type0);
  enum tree_code code1 = TREE_CODE (type1);

  /* For enumerated subrange types: dig out the actual type
   * from the INTEGER_TYPE node returned by build_range_type().
   */
  if (code0 == INTEGER_TYPE && TREE_TYPE (type0))
    code0 = TREE_CODE (TREE_TYPE (type0));
  if (code1 == INTEGER_TYPE && TREE_TYPE (type1))
    code1 = TREE_CODE (TREE_TYPE (type1));

  if (code0 == ENUMERAL_TYPE || code1 == ENUMERAL_TYPE)
    return base_type (TYPE_MAIN_VARIANT (type0))
           == base_type (TYPE_MAIN_VARIANT (type1));
  else
    return code0 == code1;
}

static int
compatible_assignment_p (type0, type1)
    tree type0, type1;
{
  enum tree_code code0 = TREE_CODE (type0);
  enum tree_code code1 = TREE_CODE (type1);
  if (code0 == ENUMERAL_TYPE || code1 == ENUMERAL_TYPE)
    return comptypes (type0, type1);
  else
    return (code0 == code1)
           || (code0 == REAL_TYPE && code1 == INTEGER_TYPE)
           || (code0 == COMPLEX_TYPE
               && (code1 == REAL_TYPE || code1 == INTEGER_TYPE));
}

static int
compatible_relop_p (type0, type1)
     tree type0, type1;
{
  enum tree_code code0 = TREE_CODE (type0);
  enum tree_code code1 = TREE_CODE (type1);
  return compatible_types_p (type0, type1)
         || (code0 == REAL_TYPE    && code1 == INTEGER_TYPE)
         || (code0 == INTEGER_TYPE && code1 == REAL_TYPE)
         || (code0 == COMPLEX_TYPE
             && (code1 == REAL_TYPE || code1 == INTEGER_TYPE))
         || ((code0 == REAL_TYPE || code0 == INTEGER_TYPE)
             && code1 == COMPLEX_TYPE);
}
#endif /* GPC */

/* Make an expression to refer to the COMPONENT field of
   structure or union value DATUM.  COMPONENT is an IDENTIFIER_NODE.  */

tree
build_component_ref (datum, component)
     tree datum, component;
{
  register tree type = TREE_TYPE (datum);
  register enum tree_code code = TREE_CODE (type);
  register tree field = NULL;
  register tree ref;

#ifdef GPC
  if (TREE_CODE (datum) == ERROR_MARK)
    return error_mark_node;

  if (TREE_CODE (datum) == TYPE_DECL)
    {
      if (PASCAL_TYPE_OBJECT (TREE_TYPE (datum)))
        {
          tree field = simple_get_field (component, TREE_TYPE (datum), NULL);
          if (! field || TREE_CODE (field) != FUNCTION_DECL)
            {
              error ("trying to access fields of a type definition");
              return error_mark_node;
            }
        }
      else
        {
          error ("trying to access fields of a type definition");
          return error_mark_node;
        }
    }
  else if (TREE_CODE (datum) == FUNCTION_DECL
           && (TREE_CODE (TREE_TYPE (type)) == RECORD_TYPE
               || TREE_CODE (TREE_TYPE (type)) == UNION_TYPE))
    {
      datum = (probably_call_function (datum));
      type = TREE_TYPE (datum);
      code = TREE_CODE (type);
    }
#endif /* GPC */

  /* If DATUM is a COMPOUND_EXPR or COND_EXPR, move our reference inside it
     unless we are not to support things not strictly ANSI.  */
  switch (TREE_CODE (datum))
    {
    case COMPOUND_EXPR:
      {
        tree value = build_component_ref (TREE_OPERAND (datum, 1), component);
        return build (COMPOUND_EXPR, TREE_TYPE (value),
                      TREE_OPERAND (datum, 0), value);
      }
    case COND_EXPR:
      return build_conditional_expr
        (TREE_OPERAND (datum, 0),
         build_component_ref (TREE_OPERAND (datum, 1), component),
         build_component_ref (TREE_OPERAND (datum, 2), component));

    default:
      break;
    }

  /* See if there is a field or component with name COMPONENT.  */

  if (code == RECORD_TYPE || code == UNION_TYPE)
    {
#ifndef GPC
      tree indirect = 0;
#endif /* not GPC */

      if (TYPE_SIZE (type) == 0)
        {
          incomplete_type_error (NULL_TREE, type);
          return error_mark_node;
        }

#ifdef GPC
      /* For Pascal: implicitely propagate to the inner layers
       * of records and unions whose DECL_NAME is NULL_TREE.
       * find_field() also constructs a TREE_LIST of fields and
       * returns that.
       */
      field = find_field (datum, component);

      if (!field)
        {
          error ("record, schema or object has no field named `%s'",
                 IDENTIFIER_POINTER (component));
          return error_mark_node;
        }

      if (TREE_TYPE (field) == error_mark_node)
        return error_mark_node;

      /* @@@@ jtv@hut.fi : TREE_READONLY??  */

      /* Generate COMPONENT_REFs to access the field */
      ref = datum;
      for (field = nreverse (field); field; field = TREE_CHAIN (field))
        {
          tree ref1;
          ref1 = build (COMPONENT_REF, TREE_TYPE (TREE_VALUE (field)),
                        ref, TREE_VALUE (field));

          if (TREE_READONLY (ref) || TREE_READONLY (TREE_VALUE (field)))
            TREE_READONLY (ref1) = 1;

          if (TREE_THIS_VOLATILE (ref) || TREE_THIS_VOLATILE (TREE_VALUE (field)))
            TREE_THIS_VOLATILE (ref1) = 1;

          ref = ref1;
        }
#else /* not GPC */
      field = lookup_field (type, component, &indirect);

      if (!field)
        {
          error (code == RECORD_TYPE
                 ? "structure has no member named `%s'"
                 : "union has no member named `%s'",
                 IDENTIFIER_POINTER (component));
          return error_mark_node;
        }
      if (TREE_TYPE (field) == error_mark_node)
        return error_mark_node;

      /* If FIELD was found buried within an anonymous union,
         make one COMPONENT_REF to get that anonymous union,
         then fall thru to make a second COMPONENT_REF to get FIELD.  */
      if (indirect != 0)
        {
          ref = build (COMPONENT_REF, TREE_TYPE (indirect), datum, indirect);
          if (TREE_READONLY (datum) || TREE_READONLY (indirect))
            TREE_READONLY (ref) = 1;
          if (TREE_THIS_VOLATILE (datum) || TREE_THIS_VOLATILE (indirect))
            TREE_THIS_VOLATILE (ref) = 1;
          datum = ref;
        }

      ref = build (COMPONENT_REF, TREE_TYPE (field), datum, field);

      if (TREE_READONLY (datum) || TREE_READONLY (field))
        TREE_READONLY (ref) = 1;
      if (TREE_THIS_VOLATILE (datum) || TREE_THIS_VOLATILE (field))
        TREE_THIS_VOLATILE (ref) = 1;
#endif /* not GPC */

      return ref;
    }
  else if (code != ERROR_MARK)
#ifdef GPC
    {
      if (TREE_CODE (datum) == COMPONENT_REF
          && PASCAL_TYPE_SCHEMA (TREE_TYPE (TREE_OPERAND (datum, 0)))
          && DECL_NAME (TREE_OPERAND (datum, 1)) == schema_id)
        {
          if (TREE_CODE (TREE_TYPE (TREE_OPERAND (datum, 1))) == RECORD_TYPE)
            error ("record schema has no field or discriminant named `%s'",
                   IDENTIFIER_POINTER (component));
          else
            error ("schema has no discriminant named `%s'",
                   IDENTIFIER_POINTER (component));
        }
      else
        {
          error ("request for member `%s' in something not",
                 IDENTIFIER_POINTER (component));
          error (" a record, schema or object");
        }
    }
#else /* not GPC */
    error ("request for member `%s' in something not a structure or union",
            IDENTIFIER_POINTER (component));
#endif /* not GPC */

  return error_mark_node;
}

/* Given an expression PTR for a pointer, return an expression
   for the value pointed to.
   ERRORSTRING is the name of the operator to appear in error messages.  */

#ifdef EGCS92
tree
build_indirect_ref (ptr, errorstring)
     tree ptr;
     const char *errorstring;
#else
tree
build_indirect_ref (ptr, errorstring)
     tree ptr;
     char *errorstring;
#endif
{
  register tree pointer = default_conversion (ptr);
  register tree type = TREE_TYPE (pointer);

  if (TREE_CODE (type) == POINTER_TYPE)
    {
      if (TREE_CODE (pointer) == ADDR_EXPR
          && !flag_volatile
          && (TREE_TYPE (TREE_OPERAND (pointer, 0))
              == TREE_TYPE (type)))
        return TREE_OPERAND (pointer, 0);
      else
        {
          tree t = TREE_TYPE (type);
          register tree ref = build1 (INDIRECT_REF,
                                      TYPE_MAIN_VARIANT (t), pointer);

          if (TYPE_SIZE (t) == 0 && TREE_CODE (t) != ARRAY_TYPE)
            {
              error ("dereferencing pointer to incomplete type");
              return error_mark_node;
            }
#ifdef GPC
          /* @@@ This is a kludge to prevent warnings when accessing
           * void type parameters which are void* pointers internally
           * and automatically dereferenced even when only taking the
           * address of the untyped parameter.  It is now arranged
           * that the warning is only enabled with *explicit* ("^")
           * dereferences.  In all other situations where build_indi-
           * rect_ref is used, t can only be of VOID_TYPE if an error
           * already happened.
           */
          if (warn_void_dereference
              && (flag_what_pascal & B_D_PASCAL) == 0
              && TREE_CODE (t) == VOID_TYPE
              && skip_evaluation == 0)
            warning ("dereferencing untyped pointer");
#else /* not GPC */
          if (TREE_CODE (t) == VOID_TYPE && skip_evaluation == 0)
            warning ("dereferencing `void *' pointer");
#endif /* not GPC */

          /* We *must* set TREE_READONLY when dereferencing a pointer to const,
             so that we get the proper error message if the result is used
             to assign to.  Also, &* is supposed to be a no-op.
             And ANSI C seems to specify that the type of the result
             should be the const type.  */
          /* A de-reference of a pointer to const is not a const.  It is valid
             to change it via some other pointer.  */
          TREE_READONLY (ref) = TYPE_READONLY (t);
          TREE_SIDE_EFFECTS (ref)
            = TYPE_VOLATILE (t) || TREE_SIDE_EFFECTS (pointer) || flag_volatile;
          TREE_THIS_VOLATILE (ref) = TYPE_VOLATILE (t);
#ifdef GPC
          prediscriminate_schema (ref, 0);
#endif /* GPC */
          return ref;
        }
    }
  else if (TREE_CODE (pointer) != ERROR_MARK)
    error ("invalid type argument of `%s'", errorstring);
  return error_mark_node;
}

/* This handles expressions of the form "a[i]", which denotes
   an array reference.

   This is logically equivalent in C to *(a+i), but we may do it differently.
   If A is a variable or a member, we generate a primitive ARRAY_REF.
   This avoids forcing the array out of registers, and can work on
   arrays that are not lvalues (for example, members of structures returned
   by functions).  */

tree
build_array_ref (array, index)
     tree array, index;
{
  if (index == 0)
    {
      error ("subscript missing in array reference");
      return error_mark_node;
    }

  if (TREE_TYPE (array) == error_mark_node
      || TREE_TYPE (index) == error_mark_node)
    return error_mark_node;

  if (TREE_CODE (TREE_TYPE (array)) == ARRAY_TYPE
      && TREE_CODE (array) != INDIRECT_REF)
    {
      tree rval, type;

#ifdef GPC
      /* Pascal Chars are always unsigned.
       */
#else /* not GPC */
      /* Subscripting with type char is likely to lose
         on a machine where chars are signed.
         So warn on any machine, but optionally.
         Don't warn for unsigned char since that type is safe.
         Don't warn for signed char because anyone who uses that
         must have done so deliberately.  */
      if (warn_char_subscripts
          && TYPE_MAIN_VARIANT (TREE_TYPE (index)) == char_type_node)
        warning ("array subscript has type `char'");
#endif /* not GPC */

      /* Apply default promotions *after* noticing character types.  */
      index = default_conversion (index);

      /* Require integer *after* promotion, for sake of enums.  */
#ifdef GPC
      if (! INTEGRAL_TYPE_P (TREE_TYPE (index)))
        {
          error ("array subscript is not an ordinal type");
          return error_mark_node;
        }
#else /* not GPC */
      if (TREE_CODE (TREE_TYPE (index)) != INTEGER_TYPE)
        {
          error ("array subscript is not an integer");
          return error_mark_node;
        }
#endif /* not GPC */

      /* An array that is indexed by a non-constant
         cannot be stored in a register; we must be able to do
         address arithmetic on its address.
         Likewise an array of elements of variable size.  */
      if (TREE_CODE (index) != INTEGER_CST
          || (TYPE_SIZE (TREE_TYPE (TREE_TYPE (array))) != 0
              && TREE_CODE (TYPE_SIZE (TREE_TYPE (TREE_TYPE (array)))) != INTEGER_CST))
        {
          if (mark_addressable (array) == 0)
            return error_mark_node;
        }
      /* An array that is indexed by a constant value which is not within
         the array bounds cannot be stored in a register either; because we
         would get a crash in store_bit_field/extract_bit_field when trying
         to access a non-existent part of the register.  */
      if (TREE_CODE (index) == INTEGER_CST
          && TYPE_VALUES (TREE_TYPE (array))
          && ! int_fits_type_p (index, TYPE_VALUES (TREE_TYPE (array))))
        {
          if (mark_addressable (array) == 0)
            return error_mark_node;
        }

      if (pedantic && !lvalue_p (array))
        {
          if (DECL_REGISTER (array))
            pedwarn ("ANSI C forbids subscripting `register' array");
          else
            pedwarn ("ANSI C forbids subscripting non-lvalue array");
        }

      if (pedantic)
        {
          tree foo = array;
          while (TREE_CODE (foo) == COMPONENT_REF)
            foo = TREE_OPERAND (foo, 0);
          if (TREE_CODE (foo) == VAR_DECL && DECL_REGISTER (foo))
            pedwarn ("ANSI C forbids subscripting non-lvalue array");
        }

      type = TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (array)));
      rval = build (ARRAY_REF, type, array, index);
      /* Array ref is const/volatile if the array elements are
         or if the array is.  */
      TREE_READONLY (rval)
        |= (TYPE_READONLY (TREE_TYPE (TREE_TYPE (array)))
            | TREE_READONLY (array));
      TREE_SIDE_EFFECTS (rval)
        |= (TYPE_VOLATILE (TREE_TYPE (TREE_TYPE (array)))
            | TREE_SIDE_EFFECTS (array));
      TREE_THIS_VOLATILE (rval)
        |= (TYPE_VOLATILE (TREE_TYPE (TREE_TYPE (array)))
            /* This was added by rms on 16 Nov 91.
               It fixes  vol struct foo *a;  a->elts[1]
               in an inline function.
               Hope it doesn't break something else.  */
            | TREE_THIS_VOLATILE (array));
      return require_complete_type (fold (rval));
    }

  {
#ifdef GPC
    tree ar = (TYPE_MAIN_VARIANT (TREE_TYPE (array)) == cstring_type_node)
               ? array : convert_array_to_pointer (array);
#else /* not GPC */
    tree ar = default_conversion (array);
#endif /* not GPC */
    tree ind = default_conversion (index);

    /* Do the same warning check as above, but only on the part that's
       syntactically the index and only if it is also semantically
       the index.  */
    if (warn_char_subscripts
        && TREE_CODE (TREE_TYPE (index)) == INTEGER_TYPE
        && TYPE_MAIN_VARIANT (TREE_TYPE (index)) == char_type_node)
      warning ("subscript has type `char'");

    /* Put the integer in IND to simplify error checking.  */
    if (TREE_CODE (TREE_TYPE (ar)) == INTEGER_TYPE)
      {
        tree temp = ar;
        ar = ind;
        ind = temp;
      }

    if (ar == error_mark_node)
      return ar;

#ifdef GPC
    /* @@@ GPC:  should check here that
     * 1) We index an array and
     * 2) that the index is of correct type
     */
    if (TREE_CODE (TREE_TYPE (ar)) != POINTER_TYPE
        || TREE_CODE (TREE_TYPE (TREE_TYPE (ar))) == FUNCTION_TYPE)
      {
        error ("subscripted object is not an array or string");
        return error_mark_node;
      }
#else /* not GPC */
    if (TREE_CODE (TREE_TYPE (ar)) != POINTER_TYPE
        || TREE_CODE (TREE_TYPE (TREE_TYPE (ar))) == FUNCTION_TYPE)
      {
        error ("subscripted value is neither array nor pointer");
        return error_mark_node;
      }
#endif /* GPC */

#ifdef GPC
    /* jtv: Kenner's code does not handle non-zero low bound array
     * indirect_refs, so I leave this code here.  When it does
     * this can be disabled partially.
     */
    if (! ORDINAL_TYPE (TREE_CODE (TREE_TYPE (ind))))
      {
        error ("array subscript is not of ordinal type");
        return error_mark_node;
      }
    if (TREE_CODE (TREE_TYPE (ind)) != INTEGER_TYPE)
      ind = convert (integer_type_node, ind);

    /* Arrays may have non-zero low bound */
    if (TYPE_DOMAIN (TREE_TYPE (array)))
      return build_indirect_ref
          (build_binary_op
           (PLUS_EXPR,
            ar,
            fold (build
                  (MINUS_EXPR, TREE_TYPE (ind),
                   ind,
                   TYPE_MIN_VALUE (TYPE_DOMAIN (TREE_TYPE (array))))),
            0), "array indexing");
    else
      return build_indirect_ref (build_binary_op (PLUS_EXPR, ar, ind, 0),
                                 "array indexing");
#else /* not GPC */
    if (TREE_CODE (TREE_TYPE (ind)) != INTEGER_TYPE)
      {
        error ("array subscript is not an integer");
        return error_mark_node;
      }

    return build_indirect_ref (build_binary_op (PLUS_EXPR, ar, ind, 0),
                               "array indexing");
#endif /* not GPC */
  }
}

/* Build a function call to function FUNCTION with parameters PARAMS.
   PARAMS is a list--a chain of TREE_LIST nodes--in which the
   TREE_VALUE of each node is a parameter-expression.
   FUNCTION's data type may be a function type or a pointer-to-function.  */

tree
build_function_call (function, params)
     tree function, params;
{
  register tree fntype, fundecl = 0;
  register tree coerced_params;
  tree name = NULL_TREE, assembler_name = NULL_TREE;

  /* Strip NON_LVALUE_EXPRs, etc., since we aren't using as an lvalue.  */
  STRIP_TYPE_NOPS (function);

  /* Convert anything with function type to a pointer-to-function.  */
  if (TREE_CODE (function) == FUNCTION_DECL)
    {
      name = DECL_NAME (function);
      assembler_name = DECL_ASSEMBLER_NAME (function);

      /* Differs from default_conversion by not setting TREE_ADDRESSABLE
         (because calling an inline function does not mean the function
         needs to be separately compiled).  */
      fntype = build_type_variant (TREE_TYPE (function),
                                   TREE_READONLY (function),
                                   TREE_THIS_VOLATILE (function));
      fundecl = function;
      function = build1 (ADDR_EXPR, build_pointer_type (fntype), function);
    }
#ifdef GPC
  /* Not converted by default_conversion().
   */
  else if (TREE_CODE (TREE_TYPE (function)) == FUNCTION_TYPE)
    function = build_unary_op (ADDR_EXPR, function, 0);
  else if (TREE_CODE (TREE_TYPE (function)) == POINTER_TYPE)
    error ("missing `^' in indirect function call");
#endif /* GPC */
  else
    function = default_conversion (function);

  fntype = TREE_TYPE (function);

  if (TREE_CODE (fntype) == ERROR_MARK)
    return error_mark_node;

  if (!(TREE_CODE (fntype) == POINTER_TYPE
        && TREE_CODE (TREE_TYPE (fntype)) == FUNCTION_TYPE))
    {
#ifdef GPC
      error ("called object is not a procedure or function");
#else
      error ("called object is not a function");
#endif
      return error_mark_node;
    }

  /* fntype now gets the type of function pointed to.  */
  fntype = TREE_TYPE (fntype);

  /* Convert the parameters to the types declared in the
     function prototype, or apply default promotions.  */

  coerced_params
    = convert_arguments (TYPE_ARG_TYPES (fntype), params, name, fundecl);

#ifndef GPC
  /* Check for errors in format strings.  */

  if (warn_format && (name || assembler_name))
    check_function_format (name, assembler_name, coerced_params);
#endif /* not GPC */

  /* Recognize certain built-in functions so we can make tree-codes
     other than CALL_EXPR.  We do this when it enables fold-const.c
     to do something useful.  */

  if (TREE_CODE (function) == ADDR_EXPR
      && TREE_CODE (TREE_OPERAND (function, 0)) == FUNCTION_DECL
      && DECL_BUILT_IN (TREE_OPERAND (function, 0)))
    switch (DECL_FUNCTION_CODE (TREE_OPERAND (function, 0)))
      {
      case BUILT_IN_ABS:
      case BUILT_IN_LABS:
      case BUILT_IN_FABS:
        if (coerced_params == 0)
          return integer_zero_node;
        return build_unary_op (ABS_EXPR, TREE_VALUE (coerced_params), 0);
      default:
        break;
      }

  {
    register tree result
      = build (CALL_EXPR, TREE_TYPE (fntype),
               function, coerced_params, NULL_TREE);

    TREE_SIDE_EFFECTS (result) = 1;

#ifdef GPC
    if (PASCAL_TYPE_STRING (TREE_TYPE (fntype)))
      {
        /* Handle functions returning string schemata.
         *
         * Since we don't know what will happen to the string to be
         * returned, we must provide a temporary variable and expand
         * the assignment to it now.  Otherwise, subsequent component
         * references (to Capacity, length, and string) will cause the
         * function to be called three times.
         */
        tree temp_string, init;
        temp_string = start_decl (get_unique_identifier ("string_result", 0),
                                  build_tree_list (NULL_TREE,
                                                   TREE_TYPE (fntype)),
                                  0, NULL_TREE, NULL_TREE);
        finish_decl (temp_string, NULL_TREE, NULL_TREE);
        TREE_USED (temp_string) = 1;

        /* Simulate "init_any (temp_string)".
         */
        init = build_modify_expr (PASCAL_STRING_CAPACITY (temp_string), NOP_EXPR,
            build_pascal_binary_op (MINUS_EXPR,
                TYPE_MAX_VALUE (TYPE_DOMAIN (TREE_TYPE (PASCAL_STRING_VALUE (temp_string)))),
                integer_one_node));

        result = build (COMPOUND_EXPR, TREE_TYPE (fntype), init,
                        save_expr (build_modify_expr (temp_string, NOP_EXPR, result)));
        result = build (COMPOUND_EXPR, TREE_TYPE (fntype),
                        result, temp_string);
        TREE_USED (result) = 1;
      }
    {
      /* Handle packed fields passed as `var' parameters.
       */
      tree parm, temp_result = NULL_TREE;
      for (parm = coerced_params; parm; parm = TREE_CHAIN (parm))
        {
          tree val = TREE_VALUE (parm);
          if (TREE_CODE (val) == ADDR_EXPR
              && TREE_CODE (TREE_OPERAND (val, 0)) == VAR_DECL
              && PASCAL_TREE_PACKED_ACCESS (TREE_OPERAND (val, 0)))
            {
              /* Expand the function call before the back-assignment.
               */
              if (temp_result == NULL_TREE
                  && ! PASCAL_TYPE_STRING (TREE_TYPE (fntype)))
                {
                  if (TREE_TYPE (result) == void_type_node)
                    {
                      temp_result = result;
                      expand_expr_stmt (result);

                      /* The return value must be something non-NULL_TREE
                       * that doesn't produce code or error messages when
                       * expanded.
                       */
                      result = error_mark_node;
                    }
                  else
                    {
                      temp_result = make_new_variable ("function_result",
                                                       TREE_TYPE (result));
                      expand_expr_stmt (build_modify_expr (temp_result,
                                                           NOP_EXPR, result));
                      result = temp_result;
                    }
                }
              val = TREE_OPERAND (val, 0);
              expand_expr_stmt (build_modify_expr (DECL_INITIAL (val),
                                                   NOP_EXPR, val));
            }
        }
    }
#endif /* GPC */

    if (TREE_TYPE (result) == void_type_node)
      return result;
    return require_complete_type (result);
  }
}

/* Convert the argument expressions in the list VALUES
   to the types in the list TYPELIST.  The result is a list of converted
   argument expressions.

   If TYPELIST is exhausted, or when an element has NULL as its type,
   perform the default conversions.

   PARMLIST is the chain of parm decls for the function being called.
   It may be 0, if that info is not available.
   It is used only for generating error messages.

   NAME is an IDENTIFIER_NODE or 0.  It is used only for error messages.

   This is also where warnings about wrong number of args are generated.

   Both VALUES and the returned value are chains of TREE_LIST nodes
   with the elements of the list in the TREE_VALUE slots of those nodes.  */

#ifdef GPC
/*
 * Pascal conformant array convention follows:
 *
 *  An even number of array bounds are passed immediately preceding the
 *  conformant array parameter.
 *  The formal parameter decl TYPE nodes have the language bit
 *  PASCAL_TYPE_CONFORMANT set if they are part of a conformant array
 *  parameter.  One conformant array parameter consists of the indices,
 *  followed by one or more array_type parameters (value/var) that have
 *  the same indices and the same type.
 *
 *  It is an error if the PASCAL_TYPE_CONFORMANT is set for other parameters
 *  or if the array parameter with this bit set is not preceded by
 *  another array with this bit set, or an even number of indices with
 *  this bit set.
 */
#endif

static tree
convert_arguments (typelist, values, name, fundecl)
     tree typelist, values, name, fundecl;
{
  register tree typetail, valtail;
  register tree result = NULL;
  int parmnum;

#ifdef GPC
  tree keep_this_val      = NULL_TREE;
  int  conf_array_indices = 0;
#endif

  /* Scan the given expressions and types, producing individual
     converted arguments and pushing them on RESULT in reverse order.  */

#ifdef GPC
  for (valtail = values, typetail = typelist, parmnum = 0;
       valtail;
       valtail = keep_this_val ? valtail : TREE_CHAIN (valtail), parmnum++)
#else
  for (valtail = values, typetail = typelist, parmnum = 0;
       valtail;
       valtail = TREE_CHAIN (valtail), parmnum++)
#endif /* GPC */
    {
      register tree type = typetail ? TREE_VALUE (typetail) : 0;
      register tree val = TREE_VALUE (valtail);

      if (type == void_type_node)
        {
          if (name)
            error ("too many arguments to function `%s'",
                   IDENTIFIER_POINTER (name));
          else
            error ("too many arguments to function");
          break;
        }

#ifndef GPC
      /* Strip NON_LVALUE_EXPRs since we aren't using as an lvalue.  */
      /* Do not use STRIP_NOPS here!  We do not want an enumerator with value 0
         to convert automatically to a pointer.  */
      if (TREE_CODE (val) == NON_LVALUE_EXPR)
        val = TREE_OPERAND (val, 0);
#else /* GPC */
      /* Strip NON_LVALUE_EXPRs unless this is a `var' parameter.
       */
      if (TREE_CODE (val) == NON_LVALUE_EXPR
          && (type == NULL_TREE || TREE_CODE (type) != REFERENCE_TYPE))
        val = TREE_OPERAND (val, 0);
#endif /* GPC */

#ifdef GPC
      /* Passing a function/procedure as a parameter?
       * This is more fun in Pascal, since we have to know if we
       * are passing a function or the return value of the
       * function that has no parameters.  This requires knowledge
       * of the formal parameter types of the called function.
       */
      if (TREE_CODE (val) == FUNCTION_DECL      /* Passed arg is a function. */
          || PASCAL_PROCEDURAL_VARIABLE (val))
        {
          tree fun_type = TREE_TYPE (val);
          tree ttl, ttr;
          while (TREE_CODE (fun_type) == FUNCTION_TYPE
                 || TREE_CODE (fun_type) == REFERENCE_TYPE)
            fun_type = TREE_TYPE (fun_type);
          if (fun_type != void_type_node  /* Procedure.  Pass - don't call. */
              && (! type                  /* Varargs.  Call the function.   */
                  /*
                   * Call the function if its return type is compatible to
                   * the parameter wanted by this function.
                   *
                   * (If the routine has parameters, it will never come here
                   * if it needs evaluation because it has the parenthesized
                   * argument list then.)
                   */
                  || comptypes (type, fun_type) == 1
                  || (TREE_CODE (type) == POINTER_TYPE
                      && TREE_CODE (fun_type) == POINTER_TYPE
                      && (TYPE_MAIN_VARIANT (ttl = TREE_TYPE (type))
                              == void_type_node
                          || TYPE_MAIN_VARIANT (ttr = TREE_TYPE (fun_type))
                                 == void_type_node
                          || (TYPE_MAIN_VARIANT (ttl) == ptr_type_node
                              && TREE_CODE (ttr) == POINTER_TYPE)
                          || comp_target_types (type, fun_type)
                          || comp_object_or_schema_pointer_types (ttl, ttr)))))
            {
              val = build_function_call (val, NULL_TREE);
            }
        }

      /* Set types.
       */
      else if (TREE_CODE (val) == CONSTRUCTOR && TREE_CODE (type) == SET_TYPE)
        {
          /* Convert the set constructor to the corresponding set type */
          val = construct_set (val, type, 2);
        }

      /* Chars.
       */
      else if (type && TREE_CODE (base_type (type)) == CHAR_TYPE)
        {
          val = string_may_be_char (val);
          if (is_string_type (val, 0))
            {
              tree string_val = val;

              /* Formal type `char' must accept string values.
               * Assign the string to a temporary char variable and pass that.
               */
              val = start_decl (get_unique_identifier ("char_parm", 1),
                                build_tree_list (NULL_TREE, type),
                                0, NULL_TREE, NULL_TREE);
              finish_decl (val, NULL_TREE, NULL_TREE);
              assign_string (val, string_val);
            }
        }

      /* Strings.
       */
      else if (type
               && (is_string_type (val, 0)
                   || TREE_CODE (base_type (TREE_TYPE (val))) == CHAR_TYPE)
               && (is_of_string_type (type, 0)
                   || (TREE_CODE (type) == REFERENCE_TYPE
                       && PASCAL_TYPE_STRING (TREE_TYPE (type)))))
        {
          int varparm = 0;
          int val_ref_parm = 0;
          int conforming;
          int is_readonly;
          int is_volatile;

          if (TREE_CODE (type) == REFERENCE_TYPE)
            {
              varparm = 1;
              val_ref_parm = PASCAL_TYPE_VAL_REF_PARM (type);
              is_readonly = TYPE_READONLY (TREE_TYPE (type));
              is_volatile = TYPE_VOLATILE (TREE_TYPE (type));
            }
          else
            {
              is_readonly = TYPE_READONLY (type);
              is_volatile = TYPE_VOLATILE (type);
            }

          /* If the type is "string (XX)" do not convert type,
           * but still convert the CHAR or fixed-string to a new
           * schema variable.
           */
          conforming = ((varparm
                         && (PASCAL_TYPE_UNDISCRIMINATED_STRING (TREE_TYPE (type))
                             || PASCAL_TYPE_PREDISCRIMINATED_STRING (TREE_TYPE (type))))
                        || /* should not happen: always passed by reference */
                           PASCAL_TYPE_UNDISCRIMINATED_STRING (type)
                        || PASCAL_TYPE_PREDISCRIMINATED_STRING (type));

          /* See if the actual parameter is a variable length string
           * and the formal is an undiscriminated string schema -> use
           * type of actual parameter.
           *
           * If actual is not a variable-string, create a new variable string
           * type with the proper length and use the new type.
           */
          if (conforming)
            {
              if (is_variable_string_type (TREE_TYPE (val)))
                type = TREE_TYPE (val);
              else if (! varparm)
                type = build_pascal_string_schema (PASCAL_STRING_LENGTH (val));
              if (is_readonly || is_volatile)
                type = build_type_variant (type, is_readonly, is_volatile);
              if (varparm && TREE_CODE (type) != REFERENCE_TYPE)
                {
                  type = copy_node (build_reference_type (type));
                  PASCAL_TYPE_VAL_REF_PARM (type) = val_ref_parm;
                }
            }

          /* Take care of value parameter type conversions/blank paddings.
           */
          if (! varparm)
            {
              /* Ugh, the actual parameter is an ARRAY_TYPE or
               * a CHAR_TYPE; the formal parameter is a string
               * schema. So, create a new schema and copy VAL
               * there and pass that.
               *
               * If the formal paramater is already discriminated
               * use that type, otherwise create a new type
               * with capacity equal to LENGTH (val).
               */

              /* @@ Creates a new copy of string, but the size is now
               * @@ the same as the formal parameters size, padding the
               * @@ value parameter with spaces if necessary.
               * @@
               * @@ The result is then *again* copied to the stack as a value parameter.
               * @@
               * @@ The first copy can be avoided if the formal and actual
               * @@ parameters are of same size. Now avoided if the types
               * @@ are equal.
               */
              if (TREE_TYPE (val) != type)
                val = new_string_by_model (type, val, 1);
            }
        }

      /* Conformant arrays.
       */
      else if (! conf_array_indices)
        /* Clear keep_this_val when all conf. indices have been passed and the
         * array itself is in turn.
         */
        keep_this_val = NULL_TREE;

      /*
       * For conformant array formal parameters we have first the indices
       * that are missing from the actual parameter list.
       * --> We must insert them now.
       */
      if (type && PASCAL_TYPE_CONFORMANT_INDEX (type))
        {
          if (TREE_CODE (TREE_TYPE (val)) != ARRAY_TYPE
              && TREE_CODE (TREE_TYPE (val)) != VOID_TYPE)
            error ("argument does not match conformant array formal parameter");
          else
            if (keep_this_val == NULL_TREE)
              {
                tree type_scan = typetail;

                while (type_scan)
                  {
                    tree ttype = TREE_VALUE (type_scan);

                    if (TREE_CODE (ttype) == ARRAY_TYPE ||
                        (TREE_CODE (ttype) == REFERENCE_TYPE &&
                         TREE_CODE (TREE_TYPE (ttype)) == ARRAY_TYPE))
                      break;

                    /* @@ Could check that PASCAL_TYPE_CONFORMANT_INDEX is
                     * set for all indices
                     */
                    conf_array_indices++;
                    type_scan = TREE_CHAIN (type_scan);
                  }

                if (!type_scan || (conf_array_indices & 1))
                  abort ();

                keep_this_val = val;
              }
        }

      if (conf_array_indices > 0)
        {
          tree atype = TREE_TYPE (keep_this_val);

          if (TREE_CODE (atype) == VOID_TYPE)
            val = TYPE_MIN_VALUE (type);
          else
            {
              if (TREE_CODE (atype) != ARRAY_TYPE)
                abort ();

              if (conf_array_indices & 1)
                {
                  val = TYPE_MAX_VALUE (TYPE_DOMAIN (atype));
                  keep_this_val = TREE_TYPE (keep_this_val);
                }
              else
                val = TYPE_MIN_VALUE (TYPE_DOMAIN (atype));

              /* Strip NON_LVALUE_EXPRs since we aren't using as an lvalue.  */
              if (TREE_CODE (val) == NON_LVALUE_EXPR)
                val = TREE_OPERAND (val, 0);

              val = convert (TYPE_DOMAIN (atype), val);
            }

          /* Used one index */
          conf_array_indices--;
        }

      /* Open arrays.
       *
       * For open array formal parameters we have first the length of
       * the array which is missing from the actual parameter list.
       * --> We must insert it now.
       */
      if (type && PASCAL_TYPE_OPEN_ARRAY (type))
        {
          if (TREE_CODE (TREE_TYPE (val)) != ARRAY_TYPE
              && TREE_CODE (TREE_TYPE (val)) != VOID_TYPE)
            error ("argument does not match open array formal parameter");
          else if (TREE_CODE (type) == INTEGER_TYPE)
            {
              /* Pass the length of the array.
               */
              tree atype = TREE_TYPE (val);
              keep_this_val = val;
              if (TREE_CODE (atype) == VOID_TYPE)
                val = integer_zero_node;  /* Passing `null' as an open array */
              else
                val = build_pascal_binary_op (MINUS_EXPR,
                          convert (integer_type_node, TYPE_MAX_VALUE (TYPE_DOMAIN (atype))),
                          convert (integer_type_node, TYPE_MIN_VALUE (TYPE_DOMAIN (atype))));
            }
          else if (TREE_CODE (type) == ARRAY_TYPE)
            {
              /* Pass the array itself.
               * No special action required here.
               */
            }
          else
            {
              /* Should not happen.
               */
              abort();
            }
        }
#endif /* GPC */

#ifndef GPC
      if (TREE_CODE (TREE_TYPE (val)) == ARRAY_TYPE
          || TREE_CODE (TREE_TYPE (val)) == FUNCTION_TYPE)
        val = default_conversion (val);
#endif /* not GPC */

#ifdef GPC

      /* If formal reference parameter is a schema, undo a possible
       * implicit schema dereference of VAL.
       */
      if (type
          && TREE_CODE (type) == REFERENCE_TYPE
          && PASCAL_TYPE_SCHEMA (TREE_TYPE (type))
          && TREE_CODE (val) == COMPONENT_REF
          && TREE_CODE (TREE_OPERAND (val, 1)) == FIELD_DECL
          && DECL_NAME (TREE_OPERAND (val, 1)) == schema_id)
        val = TREE_OPERAND (val, 0);

      /* Actual CONST parameters may be constant values, although
       * they are passed by reference.  Create a temporary variable.
       * The same has to be done for other value parameters passed
       * by reference, e.g. schemata without specified discriminants.
       */
      if (type
          && TREE_CODE (type) == REFERENCE_TYPE
          && ((TYPE_READONLY (TREE_TYPE (type))
               && (TREE_CODE (val) == STRING_CST
                   || TREE_CODE (val) == CONSTRUCTOR
                   || ! lvalue_p (val)
                   || (PASCAL_TYPE_STRING (TREE_TYPE (type))
                       && (TREE_CODE (TREE_TYPE (val)) == CHAR_TYPE
                           || (TREE_CODE (TREE_TYPE (val)) == ARRAY_TYPE
                               && TREE_CODE (TREE_TYPE (TREE_TYPE (val))) == CHAR_TYPE)))))
              || PASCAL_TYPE_VAL_REF_PARM (type)))
        {
          if ((TREE_CODE (val) == STRING_CST
               || TREE_CODE (TREE_TYPE (val)) == CHAR_TYPE
               || (TREE_CODE (TREE_TYPE (val)) == ARRAY_TYPE
                   && TREE_CODE (TREE_TYPE (TREE_TYPE (val))) == CHAR_TYPE))
              && PASCAL_TYPE_STRING (TREE_TYPE (type)))
            {
              val = new_string_by_model (NULL_TREE, val, 1);

              /* Avoid "incompatible pointer type" warning ...
               */
              type = build_reference_type (build_type_variant (TREE_TYPE (val),
                         TYPE_READONLY (TREE_TYPE (type)),
                         TYPE_VOLATILE (TREE_TYPE (type))));
            }
          else if (TREE_CODE (val) == CONSTRUCTOR)
            val = construct_set (val, NULL_TREE, 0);
          else if (TREE_CODE (val) == FUNCTION_DECL)
            val = build_function_call (val, NULL_TREE);
#ifdef PG__NEW_STRINGS
          else if (PASCAL_TYPE_STRING (TREE_TYPE (val)))
            {
              /* This is not really used as an lvalue, so we can
               * avoid copying the data to the stack when it is
               * addressable.
               */
              if (lvalue_p (val))
                {
                  /* Just fine.  Do not touch it here and pass
                   * its address below.
                   */
                }
              else if (TREE_CODE (val) == COMPOUND_EXPR
                  && TREE_CODE (TREE_OPERAND (val, 1)) == NON_LVALUE_EXPR)
                {
                  /* Common special case:  A "magic" string is
                   * passed as a `Const' parameter.
                   */
                  TREE_OPERAND (val, 1) = TREE_OPERAND (TREE_OPERAND (val, 1), 0);
                }
              else
                {
                  /* @@@@@@@@ General case:  Make a copy on the stack.
                   */
                }
            }
#endif
          else
            {
              /* Accept structured values like in CONST decls here?
               * @@@ Yes, according to some informal standard clarifications.
               */
              tree temp_val = make_new_variable ("const_parm",
                                                 build_type_variant (TREE_TYPE (val),
                                                     0, TYPE_VOLATILE (TREE_TYPE (val))));
              expand_expr_stmt (build_modify_expr (temp_val, NOP_EXPR, val));

              if (TREE_CODE (val) == STRING_CST
                  && TREE_CODE (TREE_TYPE (type)) == ARRAY_TYPE
                  && TREE_CODE (TREE_TYPE (TREE_TYPE (type))) == CHAR_TYPE
                  && TREE_CODE (TYPE_SIZE (TREE_TYPE (type))) != INTEGER_CST)
                {
                  /* This is a string constant being passed to an
                   * "array of char" parameter of variable size.
                   * Avoid "incompatible pointer type" warning ...
                   */
                  type = build_reference_type (build_type_variant (TREE_TYPE (val),
                             TYPE_READONLY (TREE_TYPE (type)),
                             TYPE_VOLATILE (TREE_TYPE (type))));
                }
              val = temp_val;
            }
        }

      /* VAR parameters don't require a complete type.
       * They may even have explicit VOID type.
       */
      if (!type || TREE_CODE (type) != REFERENCE_TYPE)
        val = require_complete_type (val);
#else /* not GPC */
      val = require_complete_type (val);
#endif /* not GPC */

      if (type != 0)
        {
          /* Formal parm type is specified by a function prototype.  */
          tree parmval;

          if (TYPE_SIZE (type) == 0)
            {
              error ("type of formal parameter %d is incomplete", parmnum + 1);
              parmval = val;
            }
          else
            {
#ifdef GPC
              /* VAR parameters.
               */
              if (TREE_CODE (type) == REFERENCE_TYPE)
                {
                  /* Do about the same type checking like for pointers in
                   * convert_for_assignment(), but produce an error message
                   * instead of a warning.
                   */
                  tree ttl = TREE_TYPE (type);
                  tree ttr = TREE_TYPE (val);
                  type = build_pointer_type (ttl);
                  if (! (TREE_CODE (ttr) == ERROR_MARK
                         || TREE_CODE (ttr) == REFERENCE_TYPE
                         || TYPE_MAIN_VARIANT (ttl) == void_type_node
                         || TYPE_MAIN_VARIANT (ttr) == void_type_node
                         || (TYPE_MAIN_VARIANT (ttl) == ptr_type_node
                             && TREE_CODE (ttr) == POINTER_TYPE)
                         || (TYPE_MAIN_VARIANT (ttr) == ptr_type_node
                             && TREE_CODE (ttl) == POINTER_TYPE)
                         || (TREE_CODE (ttl) == FUNCTION_TYPE
                             && TYPE_MAIN_VARIANT (ttr) == ptr_type_node
                             && integer_zerop (val))
                         || comp_target_types (type, build_pointer_type (ttr))
                         || comp_object_or_schema_pointer_types (ttl, ttr)))
                    {
                      if (fundecl)
                        error ("type mismatch in argument %d of `%s'",
                               parmnum + 1,
                               IDENTIFIER_POINTER (DECL_NAME (fundecl)));
                      else
                        error ("type mismatch in argument %d of indirect function call",
                               parmnum + 1);
                      /*
                       * Avoid further warnings.
                       */
                      type = build_pointer_type (ttr);
                    }

                  /* Passing a function return variable as a VAR parameter can
                   * assign a value to it, so do not warn about uninitialized
                   * function results afterwards.
                   */
                  if (TREE_CODE (val) == VAR_DECL)
                    PASCAL_VALUE_ASSIGNED (val) = 1;

                  /* If the parameter is of REFERENCE_TYPE, we must
                   * convert the actual parameter to a pointer.
                   *
                   * Also, since the REFERENCE_TYPE has now done it's job
                   * get rid of it by converting the REFERENCE_TYPE to
                   * POINTER_TYPE.
                   */
                  if (TREE_CODE (TREE_TYPE (val)) == REFERENCE_TYPE
                      && (comptypes (TREE_TYPE (type),
                                     TREE_TYPE (TREE_TYPE (val))) != 0))
                    {
                      /* The parameter already is a reference type of
                       * the correct type.  Don't do anything serious.
                       */
                      val = convert (type, val);
                    }
                  else if (TREE_CODE (ttl) == FUNCTION_TYPE
                           && TYPE_MAIN_VARIANT (ttr) == ptr_type_node
                           && integer_zerop (val))
                    {
                      /* Allow for `Nil' as an actual parameter
                       * to a formal procedural reference parameter.
                       */
                      val = convert (type, val);
                    }
                  else
                    {
                      /* Pass bit fields through temporary variables.
                       * This violates ISO; thus the error message.
                       */
                      if (is_packed_field (val))
                        {
                          error ("packed fields may not be used as `var' parameters");
                          val = expand_packed_field (val, type);
                        }
                      val = build_unary_op (ADDR_EXPR, val, 0);

                      if (TREE_CODE (TREE_TYPE (type)) == POINTER_TYPE
                          && TREE_CODE (TREE_TYPE (TREE_TYPE (type)))
                             == VOID_TYPE)
                        {
                          /* This is a void pointer `var' parameter.
                           * (See also below.)
                           */
                          if (TREE_CODE (TREE_TYPE (TREE_TYPE (val)))
                              == POINTER_TYPE)
                            type = TREE_TYPE (val);
                        }
                    }
                }
              else if (TREE_CODE (type) == POINTER_TYPE
                       && TREE_CODE (TREE_TYPE (type)) == VOID_TYPE)
                {
                  /* This is a void pointer parameter, which is a BP/GPC
                   * extension to make it easier to pass objects to other
                   * languages.
                   *
                   * Use with care.
                   *
                   * It accepts any pointer value as a parameter thus breaking
                   * type checking completely.
                   */
                  if (TREE_CODE (TREE_TYPE (val)) == POINTER_TYPE)
                    type = TREE_TYPE (val);
                }
              else if (TYPE_MAIN_VARIANT (type) == cstring_type_node)
                {
                  /* This is a CString parameter, which is a GPC extension
                   * to make it easier to pass string types to C routines.
                   *
                   * Use with care.  This is a *real* kludge.
                   *
                   * It accepts any string-type value as a parameter thus breaking
                   * type checking completely.
                   *
                   * If the actual parameter is a STRING-TYPE value parameter
                   * (it can not be a CHAR_TYPE) the address of the string is passed,
                   * not the value!  Also, if it is of string schema type, the address
                   * of the character store is passed, not the address of the
                   * string schema object.
                   */
                  if (is_string_type (val, 1))
                    {
                      if (is_variable_string_type (TREE_TYPE (val)))
                        {
                          val = append_null_character (val);
                          val = build1 (ADDR_EXPR, type, PASCAL_STRING_VALUE (val));
                        }
                      else
                        val = build1 (ADDR_EXPR, type, val);
                    }
                }
#endif /* GPC */

              /* Optionally warn about conversions that
                 differ from the default conversions.  */
              if (warn_conversion)
                {
                  int formal_prec = TYPE_PRECISION (type);

                  if (INTEGRAL_TYPE_P (type)
                      && TREE_CODE (TREE_TYPE (val)) == REAL_TYPE)
                    warn_for_assignment ("%s as integer rather than floating due to prototype", (char *) 0, name, parmnum + 1);
                  else if (TREE_CODE (type) == COMPLEX_TYPE
                           && TREE_CODE (TREE_TYPE (val)) == REAL_TYPE)
                    warn_for_assignment ("%s as complex rather than floating due to prototype", (char *) 0, name, parmnum + 1);
                  else if (TREE_CODE (type) == REAL_TYPE
                           && INTEGRAL_TYPE_P (TREE_TYPE (val)))
                    warn_for_assignment ("%s as floating rather than integer due to prototype", (char *) 0, name, parmnum + 1);
                  else if (TREE_CODE (type) == REAL_TYPE
                           && TREE_CODE (TREE_TYPE (val)) == COMPLEX_TYPE)
                    warn_for_assignment ("%s as floating rather than complex due to prototype", (char *) 0, name, parmnum + 1);
                  /* ??? At some point, messages should be written about
                     conversions between complex types, but that's too messy
                     to do now.  */
                  else if (TREE_CODE (type) == REAL_TYPE
                           && TREE_CODE (TREE_TYPE (val)) == REAL_TYPE)
                    {
                      /* Warn if any argument is passed as `float',
                         since without a prototype it would be `double'.  */
                      if (formal_prec == TYPE_PRECISION (float_type_node))
                        warn_for_assignment ("%s as `float' rather than `double' due to prototype", (char *) 0, name, parmnum + 1);
                    }
                  /* Detect integer changing in width or signedness.  */
                  else if (INTEGRAL_TYPE_P (type)
                           && INTEGRAL_TYPE_P (TREE_TYPE (val)))
                    {
                      tree would_have_been = default_conversion (val);
                      tree type1 = TREE_TYPE (would_have_been);

                      if (TREE_CODE (type) == ENUMERAL_TYPE
                          && type == TREE_TYPE (val))
                        /* No warning if function asks for enum
                           and the actual arg is that enum type.  */
                        ;
                      else if (formal_prec != TYPE_PRECISION (type1))
                        warn_for_assignment ("%s with different width due to prototype", (char *) 0, name, parmnum + 1);
                      else if (TREE_UNSIGNED (type) == TREE_UNSIGNED (type1))
                        ;
                      /* Don't complain if the formal parameter type
                         is an enum, because we can't tell now whether
                         the value was an enum--even the same enum.  */
                      else if (TREE_CODE (type) == ENUMERAL_TYPE)
                        ;
                      else if (TREE_CODE (val) == INTEGER_CST
                               && int_fits_type_p (val, type))
                        /* Change in signedness doesn't matter
                           if a constant value is unaffected.  */
                        ;
                      /* Likewise for a constant in a NOP_EXPR.  */
                      else if (TREE_CODE (val) == NOP_EXPR
                               && TREE_CODE (TREE_OPERAND (val, 0)) == INTEGER_CST
                               && int_fits_type_p (TREE_OPERAND (val, 0), type))
                        ;
#if 0 /* We never get such tree structure here.  */
                      else if (TREE_CODE (TREE_TYPE (val)) == ENUMERAL_TYPE
                               && int_fits_type_p (TYPE_MIN_VALUE (TREE_TYPE (val)), type)
                               && int_fits_type_p (TYPE_MAX_VALUE (TREE_TYPE (val)), type))
                        /* Change in signedness doesn't matter
                           if an enum value is unaffected.  */
                        ;
#endif
                      /* If the value is extended from a narrower
                         unsigned type, it doesn't matter whether we
                         pass it as signed or unsigned; the value
                         certainly is the same either way.  */
                      else if (TYPE_PRECISION (TREE_TYPE (val)) < TYPE_PRECISION (type)
                               && TREE_UNSIGNED (TREE_TYPE (val)))
                        ;
                      else if (TREE_UNSIGNED (type))
                        warn_for_assignment ("%s as unsigned due to prototype", (char *) 0, name, parmnum + 1);
                      else
                        warn_for_assignment ("%s as signed due to prototype", (char *) 0, name, parmnum + 1);
                    }
                }

              parmval = convert_for_assignment (type, val,
                                                (char *) 0, /* arg passing  */
                                                fundecl, name, parmnum + 1);

#ifdef PROMOTE_PROTOTYPES
#ifdef GPC
              if (INTEGRAL_TYPE_P (type)
                  && (TYPE_PRECISION (type) < TYPE_PRECISION (integer_type_node)))
                parmval = default_conversion (parmval);
#else
              if ((TREE_CODE (type) == INTEGER_TYPE
                   || TREE_CODE (type) == ENUMERAL_TYPE)
                  && (TYPE_PRECISION (type) < TYPE_PRECISION (integer_type_node)))
                parmval = default_conversion (parmval);
#endif /* GPC */
#endif
            }
          result = tree_cons (NULL_TREE, parmval, result);
        }
      else if (TREE_CODE (TREE_TYPE (val)) == REAL_TYPE
               && (TYPE_PRECISION (TREE_TYPE (val))
                   < TYPE_PRECISION (double_type_node)))
        /* Convert `float' to `double'.  */
        result = tree_cons (NULL_TREE, convert (double_type_node, val), result);
      else
#ifdef GPC
        /*
         * Varargs.
         *
         * Convert ordinal constants to integers.
         * Convert short integers and fresh integer constants
         * (returned as `LongestCard' by the lexer) to `Integer'.
         */
        {
          val = string_may_be_char (val);
          if (TREE_CODE (val) == INTEGER_CST)
            {
              if (PASCAL_TREE_FRESH_INT_CST (val))
                val = convert (select_integer_type (val, integer_zero_node,
                                                    NOP_EXPR),
                               val);
              if (TYPE_PRECISION (TREE_TYPE (val))
                  < TYPE_PRECISION (integer_type_node))
                result = tree_cons (NULL_TREE,
                                    convert (integer_type_node, val),
                                    result);
              else
                result = tree_cons (NULL_TREE, val, result);
            }
          else if (TREE_CODE (val) == STRING_CST)
            result = tree_cons (NULL_TREE,
                                build_pascal_unary_op (ADDR_EXPR, val, 0),
                                result);
          else
            result = tree_cons (NULL_TREE, default_conversion (val), result);
        }
#else /* not GPC */
        /* Convert `short' and `char' to full-size `int'.  */
        result = tree_cons (NULL_TREE, default_conversion (val), result);
#endif /* not GPC */

      if (typetail)
        typetail = TREE_CHAIN (typetail);
    }

#ifdef GPC
#if 0
  /* Trying to implement optional (initialized) parameters ...
   *
   * This does not work like this.
   * When the function is called, the info about the argument types is gone.
   * In order to handle initialized types in parameter lists we need the
   * initial value in the type node, perhaps in TYPE_LANG_SPECIFIC.  (For a
   * more rigorous type checking, it would be desireable anyway to have the
   * whole TYPE_DECL available.)
   */
  if (typetail != 0 && TREE_VALUE (typetail) != void_type_node)
    {
      /* Too few actual parameters, but the remaining formal parameters
       * might be optional (= initialized).  Check it here.
       */
      tree t, parm = DECL_ARGUMENTS (fundecl);

      /* Now we need the PARM_DECLs, too.
       * Pick up the decl corresponding to TYPETAIL.
       */
      for (t = typelist; t != typetail && parm; t = TREE_CHAIN (t))
        parm = TREE_CHAIN (parm);
      if (!parm)
        error ("Internal GPC problem: parameter declarations not found");
      else
        {
          int opt_parm_count = 0;

          while (typetail
                 && parm
                 && TREE_VALUE (typetail) != void_type_node
                 && DECL_INITIAL (TREE_VALUE (parm)))
            {
              /* These are initialized types for parameters which indicate
               * optional parameters.  Pass the initial values.
               */
              result = tree_cons (NULL_TREE,
                                  DECL_INITIAL (TREE_VALUE (typetail)),
                                  result);
              typetail = TREE_CHAIN (typetail);
              parm = TREE_CHAIN (parm);
            }
          if (!flag_extended_syntax && opt_parm_count)
            if (opt_parm_count == 1)
              pedwarn ("missing parameter; passing initial value");
            else
              pedwarn ("%d missing parameters; passing initial values",
                       opt_parm_count);
        }
    }
#endif /* 0 */
#endif /* GPC */

  if (typetail != 0 && TREE_VALUE (typetail) != void_type_node)
    {
      if (name)
        error ("too few arguments to function `%s'",
               IDENTIFIER_POINTER (name));
      else
        error ("too few arguments to function");
    }

  return nreverse (result);
}

#ifdef GPC
/* For Pascal initialized variables and constants:  Return 0 if the
 * type of the initializer in INIT matches TYPE, nonzero if invalid.
 *
 * In fact the initializer is not only checked, but some conversions
 * are done as well.
 *
 * The TREE_VALUEs in INIT are the initializers, while the
 * TREE_PURPOSEs are array indices or field names.
 *
 * @@@@@@@@@@ Currently, the TREE_PURPOSEs are
 * ignored completely!  Write any nonsense, and it
 * will be accepted by the compiler!
 */
int
check_pascal_initializer (type, init)
     tree type, init;
{
  tree link, field;
  static int informed = 0;

  if (!init || init == error_mark_node)
    return 0;  /* avoid further error messages */

  if (TREE_CODE (init) != TREE_LIST)
    return 1;

  if (PASCAL_TYPE_SCHEMA (type))
    {
      tree schema_field = simple_get_field (schema_id, type,
                              "Internal GPC error: missing `schema' field");
      if (schema_field)
        type = TREE_TYPE (schema_field);
      else
        abort ();
    }

  /* Warn about ignored TREE_PURPOSEs.
   */
  if (TREE_PURPOSE (init)
      && ((flag_what_pascal & B_D_PASCAL) == 0)
      && !informed)
    {
      if (warn_field_name_problem)
        warning ("GPC currently ignores indices and field names in initializers");
      informed = 1;
    }

  if (TREE_CODE (TREE_VALUE (init)) == TREE_LIST)
    {
      /* Structured initializer.
       */
      if (TREE_CODE (type) == ARRAY_TYPE)
        {
          for (link = TREE_VALUE (init); link; link = TREE_CHAIN (link))
            if (check_pascal_initializer (TREE_TYPE (type), link) != 0)
              return 1;
          return 0;
        }
      else if (TREE_CODE (type) == RECORD_TYPE)
        {
          for (link = TREE_VALUE (init), field = TYPE_FIELDS (type);
               link && field;
               link = TREE_CHAIN (link), field = TREE_CHAIN (field))
            if (check_pascal_initializer (TREE_TYPE (field), link) != 0)
              return 1;
          return 0;
        }
      else if (TREE_CHAIN (TREE_VALUE (init)))
        {
          /* Simple type is going to be initialized by structured
           * initializer with more than one component.
           */
          return 1;
        }
      else
        {
          /* Simple type is going to be initialized by structured
           * initializer with one component.  Recurse into.
           */
          return check_pascal_initializer (type, TREE_VALUE (init));
        }
    }
  else
    {
      /* Simple initializer. */

      /* Avoid superfluous error messages.
       */
      if (TREE_CODE (type) == ERROR_MARK)
        TREE_VALUE (init) = error_mark_node;

      /* Call functions without parameters.
       */
      TREE_VALUE (init) = probably_call_function (TREE_VALUE (init));

      /* Char constants.
       */
      if (TREE_CODE (type) == CHAR_TYPE)
        TREE_VALUE (init) = string_may_be_char (TREE_VALUE (init));

      /* Ordinal types.
       * Do some trivial range checking.
       */
      if (ORDINAL_TYPE (TREE_CODE (type))
          && TREE_CODE (TREE_VALUE (init)) == INTEGER_CST)
        {
          tree lo = TYPE_MIN_VALUE (type);
          tree hi = TYPE_MAX_VALUE (type);
          tree val = TREE_VALUE (init);
          if ((lo && int_cst_lt_always (val, lo))
              || (hi && int_cst_lt_always (hi, val)))
            {
              error ("constant out of range");     /* Or only pedwarn? */
              TREE_VALUE (init) = integer_zero_node;
            }
        }

      /* Strings.
       */
      if (PASCAL_TYPE_STRING (type)
          && TREE_CODE (TREE_VALUE (init)) == STRING_CST)
        {
          /* String constants are arrays.  Convert them.
           *
           * Pascal's string length does not include the trailing zero.
           */
          int string_length = TREE_STRING_LENGTH (TREE_VALUE (init)) - 1;
          int string_capacity = declared_string_capacity (type);
          if (TREE_CODE (type) != RECORD_TYPE)
            {
              error ("Internal GPC error: string is no schema");
              TREE_VALUE (init) = error_mark_node;
              return 1;
            }

          /* Do some trivial range checking.
           */
          if (string_length > string_capacity)
            {
              if (flag_truncate_strings)
                pedwarn ("string constant exceeds capacity - truncated");
              else
                error ("string constant exceeds capacity");

              /* Truncate the string.
               */
              string_length = string_capacity;
              TREE_STRING_LENGTH (TREE_VALUE (init)) = string_capacity + 1;
              TREE_STRING_POINTER (TREE_VALUE (init)) [string_length] = 0;
            }

          /* A string schema is a record.  Initialize it as such.
           */
          TREE_VALUE (init) = build_tree_list (NULL_TREE, TREE_VALUE (init));
          TREE_VALUE (init) = chainon (build_tree_list (NULL_TREE,
                                         build_int_2 (string_length, 0)),
                                         TREE_VALUE (init));  /* length */
          TREE_VALUE (init) = chainon (build_tree_list (NULL_TREE,
                                         build_int_2 (string_capacity, 0)),
                                         TREE_VALUE (init));  /* Capacity */
          return check_pascal_initializer (type, init);
        }
      else if (TREE_CODE (type) == ARRAY_TYPE
               && TYPE_MAIN_VARIANT (TREE_TYPE (type)) == char_type_node
               && TREE_CODE (TREE_VALUE (init)) == STRING_CST)
        {
          /* Initializing an array of char with a string constant.
           * Make the length match by either cutting or padding with blanks.
           * @@ Or else we can append a `chr ( 0 )' --> ternary switch.
           */
          tree upper_bound = convert (integer_type_node,
                                      TYPE_MAX_VALUE (TYPE_DOMAIN (type)));
          tree lower_bound = convert (integer_type_node,
                                      TYPE_MIN_VALUE (TYPE_DOMAIN (type)));
          tree capacity = fold (build_binary_op (PLUS_EXPR,
                                    build_binary_op (MINUS_EXPR,
                                                     upper_bound,
                                                     lower_bound, 0),
                                    integer_one_node, 0));
          if (TREE_CODE (capacity) == INTEGER_CST)
            {
              if (TREE_STRING_LENGTH (TREE_VALUE (init))
                  < TREE_INT_CST_LOW (capacity))
                {
                  char *buffer = xmalloc (TREE_INT_CST_LOW (capacity) + 15);
                  char *p;
                  int i, len = TREE_STRING_LENGTH (TREE_VALUE (init)) - 1;

                  bcopy (TREE_STRING_POINTER (TREE_VALUE (init)), buffer, len);
                  p = buffer + len;
                  for (i = len; i < TREE_INT_CST_LOW (capacity); i++)
                    {
                      *p = ' ';
                      p++;
                    }
                  *p = 0;
                  TREE_VALUE (init) = build_string (TREE_INT_CST_LOW (capacity),
                                                    buffer);
                  TREE_TYPE (TREE_VALUE (init)) = type;
                  free (buffer);
                }
              else
                TREE_STRING_LENGTH (TREE_VALUE (init))
                    = TREE_INT_CST_LOW (capacity);
            }
          return 0;
        }
      else if (TYPE_MAIN_VARIANT (type) == cstring_type_node
               && TREE_CODE (TREE_TYPE (TREE_VALUE (init))) == ARRAY_TYPE
               && TREE_CODE (TREE_TYPE (TREE_TYPE (TREE_VALUE (init)))) == CHAR_TYPE)
        {
          /* Initializing a `CString' with a string constant
           * or an array of char.
           * Let the `CString' variable point to it.
           */
          TREE_VALUE (init) = convert (cstring_type_node,
                                       build_pascal_unary_op (ADDR_EXPR,
                                                              TREE_VALUE (init),
                                                              0));
          return 0;
        }

      /* Pointers to strings.
       */
      else if (TREE_CODE (TREE_VALUE (init)) == ADDR_EXPR
               && TREE_CODE (TREE_OPERAND (TREE_VALUE (init), 0)) == STRING_CST)
        {
          tree value = TREE_OPERAND (TREE_VALUE (init), 0);
          if (TREE_CODE (type) == POINTER_TYPE
              && (TREE_CODE (TREE_TYPE (type)) == VOID_TYPE
                  || PASCAL_TYPE_STRING (TREE_TYPE (type))))
            {
              int string_length = TREE_STRING_LENGTH (value) - 1;
              int string_capacity;
              tree string_type;

              if (PASCAL_TYPE_UNDISCRIMINATED_STRING (TREE_TYPE (type))
                  || TREE_CODE (TREE_TYPE (type)) == VOID_TYPE)
                {
                  string_capacity = string_length;
                  string_type = build_pascal_string_schema (build_int_2 (string_capacity, 0));
                }
              else
                {
                  string_type = TREE_TYPE (type);
                  string_capacity = declared_string_capacity (string_type);
                  if (string_length > string_capacity)
                    {
                      string_length = string_capacity;
                      if (flag_truncate_strings)
                        pedwarn ("string constant exceeds capacity - truncated");
                      else
                        error ("string constant exceeds capacity");
                    }
                }

              value = build_tree_list (NULL_TREE, value);
              value = chainon (build_tree_list (NULL_TREE,
                               build_int_2 (string_length, 0)),
                               value);  /* length */
              value = chainon (build_tree_list (NULL_TREE,
                               build_int_2 (string_capacity, 0)),
                               value);  /* Capacity */
              value = build (CONSTRUCTOR, string_type, NULL_TREE, value);

              /* Make this a valid lvalue for taking addresses.
               */
              TREE_CONSTANT (value) = 1;
              TREE_STATIC (value) = 1;

              TREE_VALUE (init) = build_unary_op (ADDR_EXPR, value, 0);
              return 0;
            }
          else if (TYPE_MAIN_VARIANT (type) == cstring_type_node)
            {
              TREE_TYPE (TREE_VALUE (init)) = cstring_type_node;
              return 0;
            }
          else
            return 1;
        }

      /* Procedural variables.
       * (Pointers to procedures do not cause problems (I hope at least;-).)
       */
      else if (TREE_CODE (type) == REFERENCE_TYPE
               && TREE_CODE (TREE_TYPE (type)) == FUNCTION_TYPE
               && TREE_CODE (TREE_VALUE (init)) == CALL_EXPR)
        {
          TREE_VALUE (init) = convert (type, TREE_OPERAND (TREE_VALUE (init), 0));
          TREE_CONSTANT (TREE_VALUE (init))
              = TREE_STATIC (TREE_OPERAND (TREE_OPERAND (TREE_VALUE (init), 0), 0));
          return 0;
        }

      /* Sets.
       */
      else if (TREE_CODE (TREE_VALUE (init)) == CONSTRUCTOR
               && TREE_CODE (TREE_TYPE (TREE_VALUE (init))) == SET_TYPE
               && TREE_CODE (type) == SET_TYPE
               && TREE_TYPE (TREE_TYPE (TREE_VALUE (init))) == void_type_node)
        {
          /* The type of the set constructor was not known to the parser.
           * Specify it now.
           */
          TREE_TYPE (TREE_VALUE (init)) = type;
          return 0;
        }

      /* Conformant arrays.
       */
      else if (PASCAL_TYPE_CONFORMANT_ARRAY (type)
               && comptypes (type, TREE_VALUE (init)) != 0)
        return 0;

      /* Normal initialization.
       */
      else
        {
          /* This is just a special case of an assignment.
           */
          TREE_VALUE (init) = convert_for_assignment (type, TREE_VALUE (init),
                                  "initialization", NULL_TREE, NULL_TREE, 0);
          return (TREE_CODE (TREE_TYPE (TREE_VALUE (init)))
                  != TREE_CODE (type));
        }
    }
  abort ();
}

/* Subroutine of re_layout_type:  Grok a single schema discriminant.
 * This routine does change the contents of EXPR, so copy_node() it
 * *before* calling grok_discriminant() if you still need it.
 */
static tree
grok_discriminant (expr)
     tree expr;
{
  if (TREE_CODE (expr) == CONVERT_EXPR
      && PASCAL_TREE_DISCRIMINANT (expr))
    {
      expr = TREE_OPERAND (expr, 0);
      if (TREE_CODE (expr) == CONVERT_EXPR
          && PASCAL_TREE_DISCRIMINANT (expr))
        return expr;
      else
        return fold (expr);
    }
  else
    {
      switch (TREE_CODE_CLASS (TREE_CODE (expr)))
        {
          case '3':
            TREE_OPERAND (expr, 2)
              = grok_discriminant (copy_node (TREE_OPERAND (expr, 2)));
            /* FALLTHROUGH */

          case '2':
          case '<':
            TREE_OPERAND (expr, 1)
              = grok_discriminant (copy_node (TREE_OPERAND (expr, 1)));
            /* FALLTHROUGH */

          case '1':
          case 'e':
          case 'x':
            TREE_OPERAND (expr, 0)
              = grok_discriminant (copy_node (TREE_OPERAND (expr, 0)));
            /* FALLTHROUGH */

          default:
            /* do nothing */
            ;
        }
      return expr;
    }
}

/* Recursive subroutine of build_discriminated_schema_type:
 * Re-build a type with actual discriminants replacing the formal ones.
 */
static tree
re_layout_type (type)
     tree type;
{
  int was_packed = PASCAL_TYPE_PACKED (type);

  switch (TREE_CODE (type))
    {
      case ERROR_MARK:
        return error_mark_node;

      case BOOLEAN_TYPE:
      case CHAR_TYPE:
      case ENUMERAL_TYPE:
      case INTEGER_TYPE:
        {
          type = copy_node (type);

          TYPE_MIN_VALUE (type) = grok_discriminant (copy_node
                                      (TYPE_MIN_VALUE (type)));
          TYPE_MAX_VALUE (type) = grok_discriminant (copy_node
                                      (TYPE_MAX_VALUE (type)));

          if (TREE_TYPE (type))
            {
              /* Subrange type.
               */
              TREE_TYPE (type) = re_layout_type (TREE_TYPE (type));
            }

          /* Do some trivial range checking.
           */
          if (TREE_CODE (TYPE_MIN_VALUE (type)) == INTEGER_CST
              && TREE_CODE (TYPE_MAX_VALUE (type)) == INTEGER_CST
              && int_cst_lt_always (TYPE_MAX_VALUE (type),
                                    TYPE_MIN_VALUE (type)))
            {
              error ("actual schema discriminant has invalid value");
              return error_mark_node;
            }

          if (TYPE_PRECISION (type) < TYPE_PRECISION (TREE_TYPE (TYPE_MAX_VALUE (type))))
            TYPE_PRECISION (type) = TYPE_PRECISION (TREE_TYPE (TYPE_MAX_VALUE (type)));
          if (TYPE_PRECISION (type) < TYPE_PRECISION (TREE_TYPE (TYPE_MIN_VALUE (type))))
            TYPE_PRECISION (type) = TYPE_PRECISION (TREE_TYPE (TYPE_MIN_VALUE (type)));
          break;
        }
      case ARRAY_TYPE:
        {
          tree domain = re_layout_type (TYPE_DOMAIN (type));
          type = build_array_type (re_layout_type (TREE_TYPE (type)),
                                   domain);
          if (TREE_CODE (TREE_TYPE (type)) == ERROR_MARK
              || TREE_CODE (domain) == ERROR_MARK)
            return error_mark_node;
        }
        break;
      case RECORD_TYPE:
        if (PASCAL_TYPE_STRING (type))
          {
            tree capacity = TYPE_LANG_DECLARED_CAPACITY (type);
            if (capacity)
              type = build_pascal_string_schema (grok_discriminant (copy_node (capacity)));
            break;
          }
        else
          {
            tree field, new_fields = NULL_TREE;
            struct lang_type *lang_specific = TYPE_LANG_SPECIFIC (type);
            int mom = suspend_momentary ();
            for (field = TYPE_FIELDS (type); field; field = TREE_CHAIN (field))
              new_fields = chainon (new_fields,
                             p_grokfields (build_tree_list (NULL_TREE,
                                                            DECL_NAME (field)),
                                           re_layout_type (TREE_TYPE (field))));
            resume_momentary (mom);
            type = finish_struct (start_struct (RECORD_TYPE, NULL_TREE),
                                  new_fields, NULL_TREE);
            TYPE_LANG_SPECIFIC (type) = lang_specific;
            break;
          }
      case FILE_TYPE:
        {
          /* Do not call `layout_type' for it.
           */
          type = copy_node (type);
          return type;
        }
      case POINTER_TYPE:
      case REFERENCE_TYPE:
          /* No need to re-layout a pointer or reference.
           */
          type = copy_node (type);
          return type;
      default:
        type = copy_node (type);
        break;
    }

  TYPE_SIZE (type) = NULL_TREE;
  layout_type (type);

  if (was_packed)
    type = grok_packed (type);

  return type;
}

/* Return a discriminated schema type derived from TYPE (which must
 * be a schema type as returned by build_schema_type) using the
 * actual discriminants held in the tree list DISCRIMINANTS.
 */
tree
build_discriminated_schema_type (type, discriminants)
     tree type, discriminants;
{
  tree type_template = type, field, disc;
  if (TREE_CODE (type) != RECORD_TYPE || ! PASCAL_TYPE_SCHEMA (type_template))
    {
      /* The parser should have filtered out all non-schemata.
       */
      error ("Internal GPC error: discriminated type is not a schema");
      return error_mark_node;
    }
  for (field = TYPE_FIELDS (type_template), disc = discriminants;
       field && disc;
       field = TREE_CHAIN (field), disc = TREE_CHAIN (disc))
    {
      tree val = TREE_VALUE (disc), fix;
      val = convert_for_assignment (TREE_TYPE (field), val,
                                    "schema discrimination",
                                    NULL_TREE, NULL_TREE, 0);
      for (fix = PASCAL_DECL_FIXUPLIST (field); fix; fix = TREE_CHAIN (fix))
        {
          tree target = TREE_VALUE (fix);

          /* @@@@ We do all changes on the type template, lay out a copy,
           * and finally restore the template.  This is a kludge.
           * It would be better to create the copy *without* modifying
           * the template.
           *
           * Remember the old value of this to set it back after
           * re_layout_type() has done its job.
           */
          TREE_PURPOSE (fix) = TREE_OPERAND (target, 0);

          /* This must be a CONVERT_EXPR with the PASCAL_TREE_DISCRIMINANT
           * flag set.
           */
          if (TREE_CODE (target) == CONVERT_EXPR
              && PASCAL_TREE_DISCRIMINANT (target))
            TREE_OPERAND (target, 0) = val;
          else
            error ("Internal GPC error: invalid schema discriminant fix-up list");
        }
    }
  if (! field || DECL_NAME (field) != schema_id || disc)
    error ("number of discriminants does not match schema declaration");

  type = re_layout_type (type);

  /* Restore the type template for future use.
   */
  for (field = TYPE_FIELDS (type_template); field; field = TREE_CHAIN (field))
    {
      tree fix;
      for (fix = PASCAL_DECL_FIXUPLIST (field); fix; fix = TREE_CHAIN (fix))
        {
          tree target = TREE_VALUE (fix);
          TREE_OPERAND (target, 0) = TREE_PURPOSE (fix);
        }
    }

  /* Store the values in the new type instead of the fixup-list.
   * This is needed in init_any() to initialize these fields, and
   * it is used to distinguish discriminated from non-discriminated
   * schemata.  (Is a TREE_LIST <=> non-discriminated.)
   */
  for (field = TYPE_FIELDS (type), disc = discriminants;
       field && disc;
       field = TREE_CHAIN (field), disc = TREE_CHAIN (disc))
    {
      PASCAL_TREE_DISCRIMINANT (field) = 1;
      PASCAL_DECL_FIXUPLIST (field) = TREE_VALUE (disc);
    }

  /* Store TYPE_TEMPLATE in TYPE_LANG_SPECIFIC
   * to allow for pointer compatibility checks.
   * Do not use TYPE_MAIN_VARIANT because the
   * size of schemata varies.
   */
  if (! TYPE_LANG_SPECIFIC (type))
    TYPE_LANG_SPECIFIC (type) = allocate_type_lang_specific ();
  TYPE_LANG_CODE (type) = PASCAL_LANG_SCHEMA;
  TYPE_LANG_BASE (type) = type_template;  /* base type */

  /* Preserve volatility and readonlyness.
   */
  TYPE_READONLY (type) = TYPE_READONLY (type_template);
  TYPE_VOLATILE (type) = TYPE_VOLATILE (type_template);

  return type;
}
#endif /* GPC */

#ifndef GPC

/* This is the entry point used by the parser
   for binary operators in the input.
   In addition to constructing the expression,
   we check for operands that were written with other binary operators
   in a way that is likely to confuse the user.  */

tree
parser_build_binary_op (code, arg1, arg2)
     enum tree_code code;
     tree arg1, arg2;
{
  tree result = build_binary_op (code, arg1, arg2, 1);

  char class;
  char class1 = TREE_CODE_CLASS (TREE_CODE (arg1));
  char class2 = TREE_CODE_CLASS (TREE_CODE (arg2));
  enum tree_code code1 = ERROR_MARK;
  enum tree_code code2 = ERROR_MARK;

  if (class1 == 'e' || class1 == '1'
      || class1 == '2' || class1 == '<')
    code1 = C_EXP_ORIGINAL_CODE (arg1);
  if (class2 == 'e' || class2 == '1'
      || class2 == '2' || class2 == '<')
    code2 = C_EXP_ORIGINAL_CODE (arg2);

  /* Check for cases such as x+y<<z which users are likely
     to misinterpret.  If parens are used, C_EXP_ORIGINAL_CODE
     is cleared to prevent these warnings.  */
  if (warn_parentheses)
    {
      if (code == LSHIFT_EXPR || code == RSHIFT_EXPR)
        {
          if (code1 == PLUS_EXPR || code1 == MINUS_EXPR
              || code2 == PLUS_EXPR || code2 == MINUS_EXPR)
            warning ("suggest parentheses around + or - inside shift");
        }

      if (code == TRUTH_ORIF_EXPR)
        {
          if (code1 == TRUTH_ANDIF_EXPR
              || code2 == TRUTH_ANDIF_EXPR)
            warning ("suggest parentheses around && within ||");
        }

      if (code == BIT_IOR_EXPR)
        {
          if (code1 == BIT_AND_EXPR || code1 == BIT_XOR_EXPR
              || code1 == PLUS_EXPR || code1 == MINUS_EXPR
              || code2 == BIT_AND_EXPR || code2 == BIT_XOR_EXPR
              || code2 == PLUS_EXPR || code2 == MINUS_EXPR)
            warning ("suggest parentheses around arithmetic in operand of |");
          /* Check cases like x|y==z */
          if (TREE_CODE_CLASS (code1) == '<' || TREE_CODE_CLASS (code2) == '<')
            warning ("suggest parentheses around comparison in operand of |");
        }

      if (code == BIT_XOR_EXPR)
        {
          if (code1 == BIT_AND_EXPR
              || code1 == PLUS_EXPR || code1 == MINUS_EXPR
              || code2 == BIT_AND_EXPR
              || code2 == PLUS_EXPR || code2 == MINUS_EXPR)
            warning ("suggest parentheses around arithmetic in operand of ^");
          /* Check cases like x^y==z */
          if (TREE_CODE_CLASS (code1) == '<' || TREE_CODE_CLASS (code2) == '<')
            warning ("suggest parentheses around comparison in operand of ^");
        }

      if (code == BIT_AND_EXPR)
        {
          if (code1 == PLUS_EXPR || code1 == MINUS_EXPR
              || code2 == PLUS_EXPR || code2 == MINUS_EXPR)
            warning ("suggest parentheses around + or - in operand of &");
          /* Check cases like x&y==z */
          if (TREE_CODE_CLASS (code1) == '<' || TREE_CODE_CLASS (code2) == '<')
            warning ("suggest parentheses around comparison in operand of &");
        }
    }

  /* Similarly, check for cases like 1<=i<=10 that are probably errors.  */
  if (TREE_CODE_CLASS (code) == '<' && extra_warnings
      && (TREE_CODE_CLASS (code1) == '<' || TREE_CODE_CLASS (code2) == '<'))
    warning ("comparisons like X<=Y<=Z do not have their mathematical meaning");

  unsigned_conversion_warning (result, arg1);
  unsigned_conversion_warning (result, arg2);
  overflow_warning (result);

  class = TREE_CODE_CLASS (TREE_CODE (result));

  /* Record the code that was specified in the source,
     for the sake of warnings about confusing nesting.  */
  if (class == 'e' || class == '1'
      || class == '2' || class == '<')
    C_SET_EXP_ORIGINAL_CODE (result, code);
  else
    {
      int flag = TREE_CONSTANT (result);
      /* We used to use NOP_EXPR rather than NON_LVALUE_EXPR
         so that convert_for_assignment wouldn't strip it.
         That way, we got warnings for things like p = (1 - 1).
         But it turns out we should not get those warnings.  */
      result = build1 (NON_LVALUE_EXPR, TREE_TYPE (result), result);
      C_SET_EXP_ORIGINAL_CODE (result, code);
      TREE_CONSTANT (result) = flag;
    }

  return result;
}
#endif /* not GPC */

/* Build a binary-operation expression without default conversions.
   CODE is the kind of expression to build.
   This function differs from `build' in several ways:
   the data type of the result is computed and recorded in it,
   warnings are generated if arg data types are invalid,
   special handling for addition and subtraction of pointers is known,
   and some optimization is done (operations on narrow ints
   are done in the narrower type when that gives the same result).
   Constant folding is also done before the result is returned.

   Note that the operands will never have enumeral types, or function
   or array types, because either they will have the default conversions
   performed or they have both just been converted to some other type in which
   the arithmetic is to be done.  */

tree
build_binary_op (code, orig_op0, orig_op1, convert_p)
     enum tree_code code;
     tree orig_op0, orig_op1;
     int convert_p;
{
  tree type0, type1;
  register enum tree_code code0, code1;
  tree op0, op1;

  /* Expression code to give to the expression when it is built.
     Normally this is CODE, which is what the caller asked for,
     but in some special cases we change it.  */
  register enum tree_code resultcode = code;

  /* Data type in which the computation is to be performed.
     In the simplest cases this is the common type of the arguments.  */
  register tree result_type = NULL;

  /* Nonzero means operands have already been type-converted
     in whatever way is necessary.
     Zero means they need to be converted to RESULT_TYPE.  */
  int converted = 0;

  /* Nonzero means create the expression with this type, rather than
     RESULT_TYPE.  */
  tree build_type = 0;

  /* Nonzero means after finally constructing the expression
     convert it to this type.  */
  tree final_type = 0;

  /* Nonzero if this is an operation like MIN or MAX which can
     safely be computed in short if both args are promoted shorts.
     Also implies COMMON.
     -1 indicates a bitwise operation; this makes a difference
     in the exact conditions for when it is safe to do the operation
     in a narrower mode.  */
  int shorten = 0;

  /* Nonzero if this is a comparison operation;
     if both args are promoted shorts, compare the original shorts.
     Also implies COMMON.  */
  int short_compare = 0;

  /* Nonzero if this is a right-shift operation, which can be computed on the
     original short and then promoted if the operand is a promoted short.  */
  int short_shift = 0;

  /* Nonzero means set RESULT_TYPE to the common type of the args.  */
  int common = 0;

  if (convert_p)
    {
      op0 = default_conversion (orig_op0);
      op1 = default_conversion (orig_op1);
    }
  else
    {
      op0 = orig_op0;
      op1 = orig_op1;
    }

  type0 = TREE_TYPE (op0);
  type1 = TREE_TYPE (op1);

  /* The expression codes of the data types of the arguments tell us
     whether the arguments are integers, floating, pointers, etc.  */
#ifdef GPC
  /* Use the base type of the subrange rather than INTEGER_TYPE
   */
  code0 = TREE_CODE (base_type (type0));
  code1 = TREE_CODE (base_type (type1));
#else
  code0 = TREE_CODE (type0);
  code1 = TREE_CODE (type1);
#endif /* GPC */

  /* Strip NON_LVALUE_EXPRs, etc., since we aren't using as an lvalue.  */
  STRIP_TYPE_NOPS (op0);
  STRIP_TYPE_NOPS (op1);

  /* If an error was already reported for one of the arguments,
     avoid reporting another error.  */

  if (code0 == ERROR_MARK || code1 == ERROR_MARK)
    return error_mark_node;

  switch (code)
    {
    case PLUS_EXPR:
      /* Handle the pointer + int case.  */
      if (code0 == POINTER_TYPE && code1 == INTEGER_TYPE)
        return pointer_int_sum (PLUS_EXPR, op0, op1);
      else if (code1 == POINTER_TYPE && code0 == INTEGER_TYPE)
        return pointer_int_sum (PLUS_EXPR, op1, op0);
      else
        common = 1;
      break;

    case MINUS_EXPR:
      /* Subtraction of two similar pointers.
         We must subtract them as integers, then divide by object size.  */
      if (code0 == POINTER_TYPE && code1 == POINTER_TYPE
          && comp_target_types (type0, type1))
        return pointer_diff (op0, op1);
      /* Handle pointer minus int.  Just like pointer plus int.  */
      else if (code0 == POINTER_TYPE && code1 == INTEGER_TYPE)
        return pointer_int_sum (MINUS_EXPR, op0, op1);
      else
        common = 1;
      break;

    case MULT_EXPR:
      common = 1;
      break;

#ifdef GPC

    case RDIV_EXPR:
      if (INT_REAL (code0) && INT_REAL (code1))
        {
          if ((code0 == REAL_TYPE
               && TYPE_PRECISION (type0) > TYPE_PRECISION (double_type_node))
              || (code1 == REAL_TYPE
                  && TYPE_PRECISION (type1) > TYPE_PRECISION (double_type_node)))
            result_type = long_double_type_node;
          else
            result_type = double_type_node;
          /* This is wrong, e.g., if exactly one operand is LongReal (fjf237.pas)
          converted = code0 != INTEGER_TYPE && code1 != INTEGER_TYPE; */
        }
      else if ((code0 == COMPLEX_TYPE &&
                (code1 == COMPLEX_TYPE || INT_REAL (code1))) ||
               (INT_REAL (code0) && code1 == COMPLEX_TYPE))
        {
          result_type = complex_type_node;
          /* This is wrong if one operand is real and the other is complex (maur4.pas)
          converted = 1; */
        }
      break;

    case TRUNC_DIV_EXPR:
    case CEIL_DIV_EXPR:
    case FLOOR_DIV_EXPR:
    case ROUND_DIV_EXPR:
    case EXACT_DIV_EXPR:
      if (code0 == INTEGER_TYPE && code1 == INTEGER_TYPE)
        {
          /* Although it would be tempting to shorten always here, that
             loses on some targets, since the modulo instruction is
             undefined if the quotient can't be represented in the
             computation mode.  We shorten only if unsigned or if
             dividing by something we know != -1.  */
          shorten = (TREE_UNSIGNED (TREE_TYPE (orig_op0))
                     || (TREE_CODE (op1) == INTEGER_CST
                         && (TREE_INT_CST_LOW (op1) != -1
                             || TREE_INT_CST_HIGH (op1) != -1)));
          common = 1;
        }
      break;

#else /* not GPC */

    case TRUNC_DIV_EXPR:
    case CEIL_DIV_EXPR:
    case FLOOR_DIV_EXPR:
    case ROUND_DIV_EXPR:
    case EXACT_DIV_EXPR:
      if ((code0 == INTEGER_TYPE || code0 == REAL_TYPE
           || code0 == COMPLEX_TYPE)
          && (code1 == INTEGER_TYPE || code1 == REAL_TYPE
              || code1 == COMPLEX_TYPE))
        {
          if (!(code0 == INTEGER_TYPE && code1 == INTEGER_TYPE))
            resultcode = RDIV_EXPR;
          else
            {
              /* Although it would be tempting to shorten always here, that
                 loses on some targets, since the modulo instruction is
                 undefined if the quotient can't be represented in the
                 computation mode.  We shorten only if unsigned or if
                 dividing by something we know != -1.  */
              shorten = (TREE_UNSIGNED (TREE_TYPE (orig_op0))
                         || (TREE_CODE (op1) == INTEGER_CST
                             && (TREE_INT_CST_LOW (op1) != -1
                                 || TREE_INT_CST_HIGH (op1) != -1)));
            }
          common = 1;
        }
      break;

#endif /* GPC */

    case BIT_AND_EXPR:
    case BIT_ANDTC_EXPR:
    case BIT_IOR_EXPR:
    case BIT_XOR_EXPR:
      if (code0 == INTEGER_TYPE && code1 == INTEGER_TYPE)
        shorten = -1;
      /* If one operand is a constant, and the other is a short type
         that has been converted to an int,
         really do the work in the short type and then convert the
         result to int.  If we are lucky, the constant will be 0 or 1
         in the short type, making the entire operation go away.  */
      if (TREE_CODE (op0) == INTEGER_CST
          && TREE_CODE (op1) == NOP_EXPR
          && TYPE_PRECISION (type1) > TYPE_PRECISION (TREE_TYPE (TREE_OPERAND (op1, 0)))
          && TREE_UNSIGNED (TREE_TYPE (TREE_OPERAND (op1, 0))))
        {
          final_type = result_type;
          op1 = TREE_OPERAND (op1, 0);
          result_type = TREE_TYPE (op1);
        }
      if (TREE_CODE (op1) == INTEGER_CST
          && TREE_CODE (op0) == NOP_EXPR
          && TYPE_PRECISION (type0) > TYPE_PRECISION (TREE_TYPE (TREE_OPERAND (op0, 0)))
          && TREE_UNSIGNED (TREE_TYPE (TREE_OPERAND (op0, 0))))
        {
          final_type = result_type;
          op0 = TREE_OPERAND (op0, 0);
          result_type = TREE_TYPE (op0);
        }
#ifdef GPC
      if (code0 == SET_TYPE && code1 == SET_TYPE)
        {
          /* @@@@@ Check for set compatibility */
          converted = 1;
          result_type = TREE_TYPE (op0);
        }
#endif /* GPC */
      break;

    case TRUNC_MOD_EXPR:
    case FLOOR_MOD_EXPR:
      if (code0 == INTEGER_TYPE && code1 == INTEGER_TYPE)
        {
          /* Although it would be tempting to shorten always here, that loses
             on some targets, since the modulo instruction is undefined if the
             quotient can't be represented in the computation mode.  We shorten
             only if unsigned or if dividing by something we know != -1.  */
          shorten = (TREE_UNSIGNED (TREE_TYPE (orig_op0))
                     || (TREE_CODE (op1) == INTEGER_CST
                         && (TREE_INT_CST_LOW (op1) != -1
                             || TREE_INT_CST_HIGH (op1) != -1)));
          common = 1;
        }
      break;

    case TRUTH_ANDIF_EXPR:
    case TRUTH_ORIF_EXPR:
    case TRUTH_AND_EXPR:
    case TRUTH_OR_EXPR:
    case TRUTH_XOR_EXPR:
#ifdef GPC
      build_type = boolean_type_node;
      if (IS_BOOLEAN (code0) && IS_BOOLEAN (code1))
#else /* not GPC */
      if ((code0 == INTEGER_TYPE || code0 == POINTER_TYPE
           || code0 == REAL_TYPE || code0 == COMPLEX_TYPE)
          && (code1 == INTEGER_TYPE || code1 == POINTER_TYPE
              || code1 == REAL_TYPE || code1 == COMPLEX_TYPE))
#endif /* not GPC */
        {
          /* Result of these operations is always an int,
             but that does not mean the operands should be
             converted to ints!  */
#ifdef GPC
          result_type = boolean_type_node;
#else /* not GPC */
          result_type = integer_type_node;
#endif /* not GPC */
          op0 = truthvalue_conversion (op0);
          op1 = truthvalue_conversion (op1);
          converted = 1;
        }
      break;

      /* Shift operations: result has same type as first operand;
         always convert second operand to int.
         Also set SHORT_SHIFT if shifting rightward.  */

    case RSHIFT_EXPR:
      if (code0 == INTEGER_TYPE && code1 == INTEGER_TYPE)
        {
          if (TREE_CODE (op1) == INTEGER_CST && skip_evaluation == 0)
            {
              if (tree_int_cst_sgn (op1) < 0)
                warning ("right shift count is negative");
              else
                {
                  if (TREE_INT_CST_LOW (op1) | TREE_INT_CST_HIGH (op1))
                    short_shift = 1;
                  if (TREE_INT_CST_HIGH (op1) != 0
                      || ((unsigned HOST_WIDE_INT) TREE_INT_CST_LOW (op1)
                          >= TYPE_PRECISION (type0)))
                    warning ("right shift count >= width of type");
                }
            }
          /* Use the type of the value to be shifted.
             This is what most traditional C compilers do.  */
          result_type = type0;
          /* Unless traditional, convert the shift-count to an integer,
             regardless of size of value being shifted.  */
          if (! flag_traditional)
            {
              if (TYPE_MAIN_VARIANT (TREE_TYPE (op1)) != integer_type_node)
                op1 = convert (integer_type_node, op1);
              /* Avoid converting op1 to result_type later.  */
              converted = 1;
            }
        }
      break;

    case LSHIFT_EXPR:
      if (code0 == INTEGER_TYPE && code1 == INTEGER_TYPE)
        {
          if (TREE_CODE (op1) == INTEGER_CST && skip_evaluation == 0)
            {
              if (tree_int_cst_sgn (op1) < 0)
                warning ("left shift count is negative");
              else if (TREE_INT_CST_HIGH (op1) != 0
                       || ((unsigned HOST_WIDE_INT) TREE_INT_CST_LOW (op1)
                           >= TYPE_PRECISION (type0)))
                warning ("left shift count >= width of type");
            }
          /* Use the type of the value to be shifted.
             This is what most traditional C compilers do.  */
          result_type = type0;
          /* Unless traditional, convert the shift-count to an integer,
             regardless of size of value being shifted.  */
          if (! flag_traditional)
            {
              if (TYPE_MAIN_VARIANT (TREE_TYPE (op1)) != integer_type_node)
                op1 = convert (integer_type_node, op1);
              /* Avoid converting op1 to result_type later.  */
              converted = 1;
            }
        }
      break;

    case RROTATE_EXPR:
    case LROTATE_EXPR:
      if (code0 == INTEGER_TYPE && code1 == INTEGER_TYPE)
        {
          if (TREE_CODE (op1) == INTEGER_CST && skip_evaluation == 0)
            {
              if (tree_int_cst_sgn (op1) < 0)
                warning ("shift count is negative");
              else if (TREE_INT_CST_HIGH (op1) != 0
                       || ((unsigned HOST_WIDE_INT) TREE_INT_CST_LOW (op1)
                           >= TYPE_PRECISION (type0)))
                warning ("shift count >= width of type");
            }
          /* Use the type of the value to be shifted.
             This is what most traditional C compilers do.  */
          result_type = type0;
          /* Unless traditional, convert the shift-count to an integer,
             regardless of size of value being shifted.  */
          if (! flag_traditional)
            {
              if (TYPE_MAIN_VARIANT (TREE_TYPE (op1)) != integer_type_node)
                op1 = convert (integer_type_node, op1);
              /* Avoid converting op1 to result_type later.  */
              converted = 1;
            }
        }
      break;

    case EQ_EXPR:
    case NE_EXPR:
#ifdef GPC
      /* Result of comparison is always Boolean,
       * but don't convert the args to Boolean.
       */
      build_type = boolean_type_node;
      if (ORDINAL_REAL_OR_COMPLEX_TYPE (code0)
          && compatible_relop_p (type0, type1))
        short_compare = 1;
      else if (code0 == ARRAY_TYPE && code1 == ARRAY_TYPE)
        {
          result_type = boolean_type_node;
          converted = 1;
        }
      else if (code0 == SET_TYPE && code1 == SET_TYPE)
        {
          result_type = boolean_type_node;
          converted = 1;
        }
#else /* not GPC */
      /* Result of comparison is always int,
         but don't convert the args to int!  */
      build_type = integer_type_node;
      if ((code0 == INTEGER_TYPE || code0 == REAL_TYPE
           || code0 == COMPLEX_TYPE)
          && (code1 == INTEGER_TYPE || code1 == REAL_TYPE
              || code1 == COMPLEX_TYPE))
        short_compare = 1;
#endif /* not GPC */
      else if (code0 == POINTER_TYPE && code1 == POINTER_TYPE)
        {
          register tree tt0 = TREE_TYPE (type0);
          register tree tt1 = TREE_TYPE (type1);
          /* Anything compares with void *.  void * compares with anything.
             Otherwise, the targets must be compatible
             and both must be object or both incomplete.  */
          if (comp_target_types (type0, type1))
            result_type = common_type (type0, type1);
          else if (TYPE_MAIN_VARIANT (tt0) == void_type_node)
            {
              /* op0 != orig_op0 detects the case of something
                 whose value is 0 but which isn't a valid null ptr const.  */
              if (pedantic && (!integer_zerop (op0) || op0 != orig_op0)
                  && TREE_CODE (tt1) == FUNCTION_TYPE)
                pedwarn ("ANSI C forbids comparison of untyped pointer with function pointer");
            }
          else if (TYPE_MAIN_VARIANT (tt1) == void_type_node)
            {
              if (pedantic && (!integer_zerop (op1) || op1 != orig_op1)
                  && TREE_CODE (tt0) == FUNCTION_TYPE)
                pedwarn ("ANSI C forbids comparison of untyped pointer with function pointer");
            }
#ifdef GPC
          else if (! comp_object_or_schema_pointer_types (tt0, tt1)
                   && ! comp_object_or_schema_pointer_types (tt1, tt0))
#else
          else
#endif
            pedwarn ("comparison of distinct pointer types lacks a cast");

          if (result_type == NULL_TREE)
            result_type = ptr_type_node;
        }
#ifdef GPC
      else if (code0 == REFERENCE_TYPE
               && TREE_CODE (TREE_TYPE (type0)) == FUNCTION_TYPE
               && code1 == POINTER_TYPE
               && integer_zerop (op1))
        result_type = type0;
#else /* not GPC */
      else if (code0 == POINTER_TYPE && TREE_CODE (op1) == INTEGER_CST
               && integer_zerop (op1))
        result_type = type0;
      else if (code1 == POINTER_TYPE && TREE_CODE (op0) == INTEGER_CST
               && integer_zerop (op0))
        result_type = type1;
      else if (code0 == POINTER_TYPE && code1 == INTEGER_TYPE)
        {
          result_type = type0;
          if (! flag_traditional)
            pedwarn ("comparison between pointer and integer");
        }
      else if (code0 == INTEGER_TYPE && code1 == POINTER_TYPE)
        {
          result_type = type1;
          if (! flag_traditional)
            pedwarn ("comparison between pointer and integer");
        }
#endif /* not GPC */
      break;

    case MAX_EXPR:
    case MIN_EXPR:
#ifdef GPC
      if (ORDINAL_OR_REAL_TYPE (code0)
          && compatible_types_p (type0, type1))
#else /* not GPC */
      if ((code0 == INTEGER_TYPE || code0 == REAL_TYPE)
          && (code1 == INTEGER_TYPE || code1 == REAL_TYPE))
#endif /* not GPC */
        shorten = 1;
      else if (code0 == POINTER_TYPE && code1 == POINTER_TYPE)
        {
          if (comp_target_types (type0, type1))
            {
              result_type = common_type (type0, type1);
#ifdef GPC
              if (TREE_CODE (TREE_TYPE (type0)) == FUNCTION_TYPE)
                warning ("ordered comparisons of pointers to functions");
#else /* not GPC */
              if (pedantic
                  && TREE_CODE (TREE_TYPE (type0)) == FUNCTION_TYPE)
                pedwarn ("ANSI C forbids ordered comparisons of pointers to functions");
#endif /* not GPC */
            }
#ifdef GPC
          else if (! comp_object_or_schema_pointer_types (TREE_TYPE (type0),
                                                          TREE_TYPE (type1))
                   && ! comp_object_or_schema_pointer_types (TREE_TYPE (type1),
                                                             TREE_TYPE (type0)))
#else
          else
#endif
            {
              result_type = ptr_type_node;
              pedwarn ("comparison of distinct pointer types lacks a cast");
            }
        }
      break;

    case LE_EXPR:
    case GE_EXPR:
    case LT_EXPR:
    case GT_EXPR:
#ifdef GPC
      build_type = boolean_type_node;
      result_type = boolean_type_node;
      if (ORDINAL_OR_REAL_TYPE (code0)
          && compatible_relop_p (type0, type1))
#else /* not GPC */
      build_type = integer_type_node;
      if ((code0 == INTEGER_TYPE || code0 == REAL_TYPE)
          && (code1 == INTEGER_TYPE || code1 == REAL_TYPE))
#endif /* not GPC */
        short_compare = 1;
      else if (code0 == POINTER_TYPE && code1 == POINTER_TYPE)
        {
          if (comp_target_types (type0, type1))
            {
              result_type = common_type (type0, type1);
              if ((TYPE_SIZE (TREE_TYPE (type0)) != 0)
                  != (TYPE_SIZE (TREE_TYPE (type1)) != 0))
                pedwarn ("comparison of complete and incomplete pointers");
              else if (pedantic
                       && TREE_CODE (TREE_TYPE (type0)) == FUNCTION_TYPE)
#ifdef GPC
                warning ("ordered comparision of pointers to functions");
#else /* not GPC */
                pedwarn ("ANSI C forbids ordered comparisons of pointers to functions");
#endif /* not GPC */
            }
          else
            {
              result_type = ptr_type_node;
              pedwarn ("comparison of distinct pointer types lacks a cast");
            }
        }
#ifdef GPC
      else if ((code0 == POINTER_TYPE && code1 == INTEGER_TYPE)
               || (code0 == INTEGER_TYPE && code1 == POINTER_TYPE))
        {
          result_type = type1;  /* avoid further error messages */
          error ("comparison between pointer and integer");
        }
#else /* not GPC */
      else if (code0 == POINTER_TYPE && TREE_CODE (op1) == INTEGER_CST
               && integer_zerop (op1))
        {
          result_type = type0;
          if (pedantic || extra_warnings)
            pedwarn ("ordered comparison of pointer with integer zero");
        }
      else if (code1 == POINTER_TYPE && TREE_CODE (op0) == INTEGER_CST
               && integer_zerop (op0))
        {
          result_type = type1;
          if (pedantic)
            pedwarn ("ordered comparison of pointer with integer zero");
        }
      else if (code0 == POINTER_TYPE && code1 == INTEGER_TYPE)
        {
          result_type = type0;
          if (! flag_traditional)
            pedwarn ("comparison between pointer and integer");
        }
      else if (code0 == INTEGER_TYPE && code1 == POINTER_TYPE)
        {
          result_type = type1;
          if (! flag_traditional)
            pedwarn ("comparison between pointer and integer");
        }
#endif /* not GPC */
#ifdef GPC
      else if (code0 == ARRAY_TYPE && code1 == ARRAY_TYPE)
        {
          if (! is_string_type (op0, 1) || ! is_string_type (op1, 1))
            error ("ordered comparison between arrays that are not of string type");
          else
            result_type = boolean_type_node;
          converted = 1;
        }
      else if (code0 == SET_TYPE && code1 == SET_TYPE)
        {
          result_type = boolean_type_node;
          converted = 1;
        }
#endif /* GPC */
      break;

#ifdef GPC
  case IN_EXPR:
      if (code0 == SET_TYPE
          && (TREE_CODE (TREE_TYPE (type0)) == VOID_TYPE
              || code1 == TREE_CODE (TREE_TYPE (type0))
              || code1 == TREE_CODE (base_type (TREE_TYPE (type0)))
              || (code1 == INTEGER_TYPE /* Maybe the set is of subrange type */
                  && TREE_TYPE (type1)
                  && TREE_TYPE (type1) == TREE_TYPE (type0)))) /* subrange of same type? */
        {
          /* Make expr.c(expand_expr:IN_EXPR) happy and avoid aborting with
             bit_index with e.g. CHAR_TYPE index. */

          if (type1 != integer_type_node)
            op1 = convert (integer_type_node, op1);

          result_type = boolean_type_node;
          converted = 1;
        }
      break;

  case CARD_EXPR:
      if (code0 == SET_TYPE)
        {
          converted = 1; /* Don't convert anything */
        }
      break;
#endif /* GPC */
    default:
      break;
    }

#ifdef GPC
  if ((ORDINAL_REAL_OR_COMPLEX_TYPE (code0)
       && ORDINAL_REAL_OR_COMPLEX_TYPE (code1))
      || (code0 == SET_TYPE && code1 == SET_TYPE))
#else /* not GPC */
  if ((code0 == INTEGER_TYPE || code0 == REAL_TYPE || code0 == COMPLEX_TYPE)
      &&
      (code1 == INTEGER_TYPE || code1 == REAL_TYPE || code1 == COMPLEX_TYPE))
#endif /* not GPC */
    {
      int none_complex = (code0 != COMPLEX_TYPE && code1 != COMPLEX_TYPE);

      if (shorten || common || short_compare)
        result_type = common_type (type0, type1);

      /* For certain operations (which identify themselves by shorten != 0)
         if both args were extended from the same smaller type,
         do the arithmetic in that type and then extend.

         shorten !=0 and !=1 indicates a bitwise operation.
         For them, this optimization is safe only if
         both args are zero-extended or both are sign-extended.
         Otherwise, we might change the result.
         Eg, (short)-1 | (unsigned short)-1 is (int)-1
         but calculated in (unsigned short) it would be (unsigned short)-1.  */

      if (shorten && none_complex)
        {
          int unsigned0, unsigned1;
          tree arg0 = get_narrower (op0, &unsigned0);
          tree arg1 = get_narrower (op1, &unsigned1);
          /* UNS is 1 if the operation to be done is an unsigned one.  */
          int uns = TREE_UNSIGNED (result_type);
          tree type;

          final_type = result_type;

          /* Handle the case that OP0 (or OP1) does not *contain* a conversion
             but it *requires* conversion to FINAL_TYPE.  */

          if ((TYPE_PRECISION (TREE_TYPE (op0))
               == TYPE_PRECISION (TREE_TYPE (arg0)))
              && TREE_TYPE (op0) != final_type)
            unsigned0 = TREE_UNSIGNED (TREE_TYPE (op0));
          if ((TYPE_PRECISION (TREE_TYPE (op1))
               == TYPE_PRECISION (TREE_TYPE (arg1)))
              && TREE_TYPE (op1) != final_type)
            unsigned1 = TREE_UNSIGNED (TREE_TYPE (op1));

          /* Now UNSIGNED0 is 1 if ARG0 zero-extends to FINAL_TYPE.  */

          /* For bitwise operations, signedness of nominal type
             does not matter.  Consider only how operands were extended.  */
          if (shorten == -1)
            uns = unsigned0;

          /* Note that in all three cases below we refrain from optimizing
             an unsigned operation on sign-extended args.
             That would not be valid.  */

          /* Both args variable: if both extended in same way
             from same width, do it in that width.
             Do it unsigned if args were zero-extended.  */
          if ((TYPE_PRECISION (TREE_TYPE (arg0))
               < TYPE_PRECISION (result_type))
              && (TYPE_PRECISION (TREE_TYPE (arg1))
                  == TYPE_PRECISION (TREE_TYPE (arg0)))
              && unsigned0 == unsigned1
              && (unsigned0 || !uns))
            result_type
              = signed_or_unsigned_type (unsigned0,
                                         common_type (TREE_TYPE (arg0), TREE_TYPE (arg1)));
          else if (TREE_CODE (arg0) == INTEGER_CST
                   && (unsigned1 || !uns)
                   && (TYPE_PRECISION (TREE_TYPE (arg1))
                       < TYPE_PRECISION (result_type))
                   && (type = signed_or_unsigned_type (unsigned1,
                                                       TREE_TYPE (arg1)),
                       int_fits_type_p (arg0, type)))
            result_type = type;
          else if (TREE_CODE (arg1) == INTEGER_CST
                   && (unsigned0 || !uns)
                   && (TYPE_PRECISION (TREE_TYPE (arg0))
                       < TYPE_PRECISION (result_type))
                   && (type = signed_or_unsigned_type (unsigned0,
                                                       TREE_TYPE (arg0)),
                       int_fits_type_p (arg1, type)))
            result_type = type;
        }

      /* Shifts can be shortened if shifting right.  */

      if (short_shift)
        {
          int unsigned_arg;
          tree arg0 = get_narrower (op0, &unsigned_arg);

          final_type = result_type;

          if (arg0 == op0 && final_type == TREE_TYPE (op0))
            unsigned_arg = TREE_UNSIGNED (TREE_TYPE (op0));

          if (TYPE_PRECISION (TREE_TYPE (arg0)) < TYPE_PRECISION (result_type)
              /* We can shorten only if the shift count is less than the
                 number of bits in the smaller type size.  */
              && TREE_INT_CST_HIGH (op1) == 0
              && TYPE_PRECISION (TREE_TYPE (arg0)) > TREE_INT_CST_LOW (op1)
              /* If arg is sign-extended and then unsigned-shifted,
                 we can simulate this with a signed shift in arg's type
                 only if the extended result is at least twice as wide
                 as the arg.  Otherwise, the shift could use up all the
                 ones made by sign-extension and bring in zeros.
                 We can't optimize that case at all, but in most machines
                 it never happens because available widths are 2**N.  */
              && (!TREE_UNSIGNED (final_type)
                  || unsigned_arg
                  || 2 * TYPE_PRECISION (TREE_TYPE (arg0)) <= TYPE_PRECISION (result_type)))
            {
              /* Do an unsigned shift if the operand was zero-extended.  */
              result_type
                = signed_or_unsigned_type (unsigned_arg,
                                           TREE_TYPE (arg0));
              /* Convert value-to-be-shifted to that type.  */
              if (TREE_TYPE (op0) != result_type)
                op0 = convert (result_type, op0);
              converted = 1;
            }
        }

      /* Comparison operations are shortened too but differently.
         They identify themselves by setting short_compare = 1.  */

      if (short_compare)
        {
          /* Don't write &op0, etc., because that would prevent op0
             from being kept in a register.
             Instead, make copies of the our local variables and
             pass the copies by reference, then copy them back afterward.  */
          tree xop0 = op0, xop1 = op1, xresult_type = result_type;
          enum tree_code xresultcode = resultcode;
          tree val
            = shorten_compare (&xop0, &xop1, &xresult_type, &xresultcode);
          if (val != 0)
#ifdef GPC
            return build_type ? convert (build_type, val) : val;
#else /* not GPC */
            return val;
#endif /* not GPC */
          op0 = xop0, op1 = xop1;
          converted = 1;
          resultcode = xresultcode;

          if ((warn_sign_compare < 0 ? extra_warnings : warn_sign_compare != 0)
              && skip_evaluation == 0)
            {
              int op0_signed = ! TREE_UNSIGNED (TREE_TYPE (orig_op0));
              int op1_signed = ! TREE_UNSIGNED (TREE_TYPE (orig_op1));

              int unsignedp0, unsignedp1;
              tree primop0 = get_narrower (op0, &unsignedp0);
              tree primop1 = get_narrower (op1, &unsignedp1);

              /* Avoid spurious warnings for comparison with enumerators.  */

              xop0 = orig_op0;
              xop1 = orig_op1;
              STRIP_TYPE_NOPS (xop0);
              STRIP_TYPE_NOPS (xop1);

              /* Give warnings for comparisons between signed and unsigned
                 quantities that may fail.  */
              /* Do the checking based on the original operand trees, so that
                 casts will be considered, but default promotions won't be.  */

              /* Do not warn if the comparison is being done in a signed type,
                 since the signed type will only be chosen if it can represent
                 all the values of the unsigned type.  */
              if (! TREE_UNSIGNED (result_type))
                /* OK */;
              /* Do not warn if both operands are unsigned.  */
              else if (op0_signed == op1_signed)
                /* OK */;
              /* Do not warn if the signed quantity is an unsuffixed
                 integer literal (or some static constant expression
                 involving such literals) and it is non-negative.  */
              else if ((op0_signed && TREE_CODE (xop0) == INTEGER_CST
                        && tree_int_cst_sgn (xop0) >= 0)
                       || (op1_signed && TREE_CODE (xop1) == INTEGER_CST
                           && tree_int_cst_sgn (xop1) >= 0))
                /* OK */;
              /* Do not warn if the comparison is an equality operation,
                 the unsigned quantity is an integral constant and it does
                 not use the most significant bit of result_type.  */
              else if ((resultcode == EQ_EXPR || resultcode == NE_EXPR)
                       && ((op0_signed && TREE_CODE (xop1) == INTEGER_CST
                            && int_fits_type_p (xop1, signed_type (result_type)))
                           || (op1_signed && TREE_CODE (xop0) == INTEGER_CST
                               && int_fits_type_p (xop0, signed_type (result_type)))))
                /* OK */;
              else
                warning ("comparison between signed and unsigned");

              /* Warn if two unsigned values are being compared in a size
                 larger than their original size, and one (and only one) is the
                 result of a `~' operator.  This comparison will always fail.

                 Also warn if one operand is a constant, and the constant
                 does not have all bits set that are set in the ~ operand
                 when it is extended.  */

              if ((TREE_CODE (primop0) == BIT_NOT_EXPR)
                  != (TREE_CODE (primop1) == BIT_NOT_EXPR))
                {
                  if (TREE_CODE (primop0) == BIT_NOT_EXPR)
                    primop0 = get_narrower (TREE_OPERAND (primop0, 0),
                                            &unsignedp0);
                  else
                    primop1 = get_narrower (TREE_OPERAND (primop1, 0),
                                            &unsignedp1);

                  if (TREE_CODE (primop0) == INTEGER_CST
                      || TREE_CODE (primop1) == INTEGER_CST)
                    {
                      tree primop;
                      long constant, mask;
                      int unsignedp, bits;

                      if (TREE_CODE (primop0) == INTEGER_CST)
                        {
                          primop = primop1;
                          unsignedp = unsignedp1;
                          constant = TREE_INT_CST_LOW (primop0);
                        }
                      else
                        {
                          primop = primop0;
                          unsignedp = unsignedp0;
                          constant = TREE_INT_CST_LOW (primop1);
                        }

                      bits = TYPE_PRECISION (TREE_TYPE (primop));
                      if (bits < TYPE_PRECISION (result_type)
                          && bits < HOST_BITS_PER_LONG && unsignedp)
                        {
                          mask = (~0L) << bits;
                          if ((mask & constant) != mask)
                            warning ("comparison of promoted ~unsigned with constant");
                        }
                    }
                  else if (unsignedp0 && unsignedp1
                           && (TYPE_PRECISION (TREE_TYPE (primop0))
                               < TYPE_PRECISION (result_type))
                           && (TYPE_PRECISION (TREE_TYPE (primop1))
                               < TYPE_PRECISION (result_type)))
                    warning ("comparison of promoted ~unsigned with unsigned");
                }
            }
        }
    }

  /* At this point, RESULT_TYPE must be nonzero to avoid an error message.
     If CONVERTED is zero, both args will be converted to type RESULT_TYPE.
     Then the expression will be built.
     It will be given type FINAL_TYPE if that is nonzero;
     otherwise, it will be given type RESULT_TYPE.  */

  if (!result_type)
    {
      binary_op_error (code);
      return error_mark_node;
    }

  if (! converted)
    {
      if (TREE_TYPE (op0) != result_type)
        op0 = convert (result_type, op0);
      if (TREE_TYPE (op1) != result_type)
        op1 = convert (result_type, op1);
    }

  if (build_type == NULL_TREE)
    build_type = result_type;

  {
    register tree result = build (resultcode, build_type, op0, op1);
    register tree folded;

    folded = fold (result);
    if (folded == result)
      TREE_CONSTANT (folded) = TREE_CONSTANT (op0) & TREE_CONSTANT (op1);
    if (final_type != 0)
      return convert (final_type, folded);
    return folded;
  }
}

/* Return a tree for the sum or difference (RESULTCODE says which)
   of pointer PTROP and integer INTOP.  */

static tree
pointer_int_sum (resultcode, ptrop, intop)
     enum tree_code resultcode;
     register tree ptrop, intop;
{
  tree size_exp;

  register tree result;
  register tree folded;

  /* The result is a pointer of the same type that is being added.  */

  register tree result_type = TREE_TYPE (ptrop);

  if (TREE_CODE (TREE_TYPE (result_type)) == VOID_TYPE)
    {
      if (pedantic || warn_pointer_arith)
        pedwarn ("untyped pointer used in arithmetic");
      size_exp = integer_one_node;
    }
  else if (TREE_CODE (TREE_TYPE (result_type)) == FUNCTION_TYPE)
    {
      if (pedantic || warn_pointer_arith)
        pedwarn ("pointer to a function used in arithmetic");
      size_exp = integer_one_node;
    }
  else
    size_exp = c_size_in_bytes (TREE_TYPE (result_type));

  /* If what we are about to multiply by the size of the elements
     contains a constant term, apply distributive law
     and multiply that constant term separately.
     This helps produce common subexpressions.  */

  if ((TREE_CODE (intop) == PLUS_EXPR || TREE_CODE (intop) == MINUS_EXPR)
      && ! TREE_CONSTANT (intop)
      && TREE_CONSTANT (TREE_OPERAND (intop, 1))
      && TREE_CONSTANT (size_exp)
      /* If the constant comes from pointer subtraction,
         skip this optimization--it would cause an error.  */
      && TREE_CODE (TREE_TYPE (TREE_OPERAND (intop, 0))) == INTEGER_TYPE
      /* If the constant is unsigned, and smaller than the pointer size,
         then we must skip this optimization.  This is because it could cause
         an overflow error if the constant is negative but INTOP is not.  */
      && (! TREE_UNSIGNED (TREE_TYPE (intop))
          || (TYPE_PRECISION (TREE_TYPE (intop))
              == TYPE_PRECISION (TREE_TYPE (ptrop)))))
    {
      enum tree_code subcode = resultcode;
      tree int_type = TREE_TYPE (intop);
      if (TREE_CODE (intop) == MINUS_EXPR)
        subcode = (subcode == PLUS_EXPR ? MINUS_EXPR : PLUS_EXPR);
      /* Convert both subexpression types to the type of intop,
         because weird cases involving pointer arithmetic
         can result in a sum or difference with different type args.  */
      ptrop = build_binary_op (subcode, ptrop,
                               convert (int_type, TREE_OPERAND (intop, 1)), 1);
      intop = convert (int_type, TREE_OPERAND (intop, 0));
    }

  /* Convert the integer argument to a type the same size as sizetype
     so the multiply won't overflow spuriously.  */

  if (TYPE_PRECISION (TREE_TYPE (intop)) != TYPE_PRECISION (sizetype)
      || TREE_UNSIGNED (TREE_TYPE (intop)) != TREE_UNSIGNED (sizetype))
    intop = convert (type_for_size (TYPE_PRECISION (sizetype),
                                    TREE_UNSIGNED (sizetype)), intop);

  /* Replace the integer argument with a suitable product by the object size.
     Do this multiplication as signed, then convert to the appropriate
     pointer type (actually unsigned integral).  */

  intop = convert (result_type,
                   build_binary_op (MULT_EXPR, intop,
                                    convert (TREE_TYPE (intop), size_exp), 1));

  /* Create the sum or difference.  */

  result = build (resultcode, result_type, ptrop, intop);

  folded = fold (result);
  if (folded == result)
    TREE_CONSTANT (folded) = TREE_CONSTANT (ptrop) & TREE_CONSTANT (intop);
  return folded;
}

/* Return a tree for the difference of pointers OP0 and OP1.
   The resulting tree has type int.  */

static tree
pointer_diff (op0, op1)
     register tree op0, op1;
{
  register tree result, folded;
  tree restype = ptrdiff_type_node;

  tree target_type = TREE_TYPE (TREE_TYPE (op0));

  if (pedantic || warn_pointer_arith)
    {
      if (TREE_CODE (target_type) == VOID_TYPE)
        pedwarn ("untyped pointer used in subtraction");
      if (TREE_CODE (target_type) == FUNCTION_TYPE)
        pedwarn ("pointer to a function used in subtraction");
    }

  /* First do the subtraction as integers;
     then drop through to build the divide operator.
     Do not do default conversions on the minus operator
     in case restype is a short type.  */

  op0 = build_binary_op (MINUS_EXPR, convert (restype, op0),
                         convert (restype, op1), 0);
  /* This generates an error if op1 is pointer to incomplete type.  */
  if (TYPE_SIZE (TREE_TYPE (TREE_TYPE (op1))) == 0)
    error ("arithmetic on pointer to an incomplete type");

  /* This generates an error if op0 is pointer to incomplete type.  */
  op1 = c_size_in_bytes (target_type);

  /* Divide by the size, in easiest possible way.  */

  result = build (EXACT_DIV_EXPR, restype, op0, convert (restype, op1));

  folded = fold (result);
  if (folded == result)
    TREE_CONSTANT (folded) = TREE_CONSTANT (op0) & TREE_CONSTANT (op1);
  return folded;
}

/* Construct and perhaps optimize a tree representation
   for a unary operation.  CODE, a tree_code, specifies the operation
   and XARG is the operand.  NOCONVERT nonzero suppresses
   the default promotions (such as from short to int).  */

tree
build_unary_op (code, xarg, noconvert)
     enum tree_code code;
     tree xarg;
     int noconvert;
{
  /* No default_conversion here.  It causes trouble for ADDR_EXPR.  */
  register tree arg = xarg;
  register tree argtype = 0;
  register enum tree_code typecode = TREE_CODE (TREE_TYPE (arg));
  char *errstring = NULL;
  tree val;

  if (typecode == ERROR_MARK)
    return error_mark_node;
  if (typecode == ENUMERAL_TYPE)
    typecode = INTEGER_TYPE;

  switch (code)
    {
    case CONVERT_EXPR:
      /* This is used for unary plus, because a CONVERT_EXPR
         is enough to prevent anybody from looking inside for
         associativity, but won't generate any code.  */
      if (!(typecode == INTEGER_TYPE || typecode == REAL_TYPE
            || typecode == COMPLEX_TYPE))
        errstring = "wrong type argument to unary plus";
      else if (!noconvert)
        arg = default_conversion (arg);
      break;

    case NEGATE_EXPR:
      if (!(typecode == INTEGER_TYPE || typecode == REAL_TYPE
            || typecode == COMPLEX_TYPE))
        errstring = "wrong type argument to unary minus";
#ifdef GPC
      else
        {
          if (!noconvert)
            arg = default_conversion (arg);

          /* If this is an unsigned integer subrange make sure
           * the negated arg can be represented in the result type.
           */
          if (typecode == INTEGER_TYPE && TREE_TYPE (TREE_TYPE (arg)))
            argtype = type_for_size (TYPE_PRECISION (TREE_TYPE (arg)), 0);
        }
#else
      else if (!noconvert)
        arg = default_conversion (arg);
#endif /* GPC */
      break;

    case BIT_NOT_EXPR:
      if (typecode == COMPLEX_TYPE)
        {
          code = CONJ_EXPR;
          if (!noconvert)
            arg = default_conversion (arg);
        }
      else if (typecode != INTEGER_TYPE)
        errstring = "wrong type argument to bit-complement";
      else if (!noconvert)
        arg = default_conversion (arg);
      break;

    case ABS_EXPR:
      if (!(typecode == INTEGER_TYPE || typecode == REAL_TYPE
            || typecode == COMPLEX_TYPE))
        errstring = "wrong type argument to abs";
#ifdef GPC
      else if (typecode == COMPLEX_TYPE)
        return fold (build1 (ABS_EXPR, TREE_TYPE (TREE_TYPE (arg)), arg));
#endif
      else if (!noconvert)
        arg = default_conversion (arg);
      break;

    case CONJ_EXPR:
      /* Conjugating a real value is a no-op, but allow it anyway.  */
      if (!(typecode == INTEGER_TYPE || typecode == REAL_TYPE
            || typecode == COMPLEX_TYPE))
        errstring = "wrong type argument to conjugation";
      else if (!noconvert)
        arg = default_conversion (arg);
      break;

    case TRUTH_NOT_EXPR:
#ifdef GPC
      /* @@@@ Check this! */
      if (! IS_BOOLEAN (typecode))
#else /* !GPC */
      if (typecode != INTEGER_TYPE
          && typecode != REAL_TYPE && typecode != POINTER_TYPE
          && typecode != COMPLEX_TYPE
          /* These will convert to a pointer.  */
          && typecode != ARRAY_TYPE && typecode != FUNCTION_TYPE)
#endif /* GPC */
        {
#ifdef GPC
          errstring = "wrong type argument to boolean `not'";
#else
          errstring = "wrong type argument to unary exclamation mark";
#endif /* GPC */
          break;
        }
      arg = truthvalue_conversion (arg);
      return invert_truthvalue (arg);

    case NOP_EXPR:
      break;

    case REALPART_EXPR:
      if (TREE_CODE (arg) == COMPLEX_CST)
        return TREE_REALPART (arg);
      else if (TREE_CODE (TREE_TYPE (arg)) == COMPLEX_TYPE)
        return fold (build1 (REALPART_EXPR, TREE_TYPE (TREE_TYPE (arg)), arg));
      else
        return arg;

    case IMAGPART_EXPR:
      if (TREE_CODE (arg) == COMPLEX_CST)
        return TREE_IMAGPART (arg);
      else if (TREE_CODE (TREE_TYPE (arg)) == COMPLEX_TYPE)
        return fold (build1 (IMAGPART_EXPR, TREE_TYPE (TREE_TYPE (arg)), arg));
      else
        return convert (TREE_TYPE (arg), integer_zero_node);

    case PREINCREMENT_EXPR:
    case POSTINCREMENT_EXPR:
    case PREDECREMENT_EXPR:
    case POSTDECREMENT_EXPR:
      /* Handle complex lvalues (when permitted)
         by reduction to simpler cases.  */

      val = unary_complex_lvalue (code, arg);
      if (val != 0)
        return val;

      /* Increment or decrement the real part of the value,
         and don't change the imaginary part.  */
      if (typecode == COMPLEX_TYPE)
        {
          tree real, imag;

          arg = stabilize_reference (arg);
          real = build_unary_op (REALPART_EXPR, arg, 1);
          imag = build_unary_op (IMAGPART_EXPR, arg, 1);
          return build (COMPLEX_EXPR, TREE_TYPE (arg),
                        build_unary_op (code, real, 1), imag);
        }

      /* Report invalid types.  */

#ifdef GPC
      if (! ORDINAL_OR_REAL_TYPE (typecode) && typecode != POINTER_TYPE)
#else /* GPC */
      if (typecode != POINTER_TYPE
          && typecode != INTEGER_TYPE && typecode != REAL_TYPE)
#endif /* GPC */
        {
          if (code == PREINCREMENT_EXPR || code == POSTINCREMENT_EXPR)
            errstring ="wrong type argument to increment";
          else
            errstring ="wrong type argument to decrement";
          break;
        }

      {
        register tree inc;
        tree result_type = TREE_TYPE (arg);

        arg = get_unwidened (arg, 0);
        argtype = TREE_TYPE (arg);

        /* Compute the increment.  */

        if (typecode == POINTER_TYPE)
          {
            /* If pointer target is an undefined struct,
               we just cannot know how to do the arithmetic.  */
            if (TYPE_SIZE (TREE_TYPE (result_type)) == 0)
              error ("%s of pointer to unknown structure",
                       ((code == PREINCREMENT_EXPR
                         || code == POSTINCREMENT_EXPR)
                        ? "increment" : "decrement"));
            else if ((pedantic || warn_pointer_arith)
                     && (TREE_CODE (TREE_TYPE (result_type)) == FUNCTION_TYPE
                         || TREE_CODE (TREE_TYPE (result_type)) == VOID_TYPE))
              pedwarn ("wrong type argument to %s",
                       ((code == PREINCREMENT_EXPR
                         || code == POSTINCREMENT_EXPR)
                        ? "increment" : "decrement"));
            inc = c_size_in_bytes (TREE_TYPE (result_type));
          }
        else
          inc = integer_one_node;

        inc = convert (argtype, inc);

        /* Handle incrementing a cast-expression.  */

        while (1)
          switch (TREE_CODE (arg))
            {
            case NOP_EXPR:
            case CONVERT_EXPR:
            case FLOAT_EXPR:
            case FIX_TRUNC_EXPR:
            case FIX_FLOOR_EXPR:
            case FIX_ROUND_EXPR:
            case FIX_CEIL_EXPR:
#ifndef GPC
              pedantic_lvalue_warning (CONVERT_EXPR);
#endif /* not GPC */
              /* If the real type has the same machine representation
                 as the type it is cast to, we can make better output
                 by adding directly to the inside of the cast.  */
              if ((TREE_CODE (TREE_TYPE (arg))
                   == TREE_CODE (TREE_TYPE (TREE_OPERAND (arg, 0))))
                  && (TYPE_MODE (TREE_TYPE (arg))
                      == TYPE_MODE (TREE_TYPE (TREE_OPERAND (arg, 0)))))
                arg = TREE_OPERAND (arg, 0);
              else
                {
                  tree incremented, modify, value;
                  arg = stabilize_reference (arg);
                  if (code == PREINCREMENT_EXPR || code == PREDECREMENT_EXPR)
                    value = arg;
                  else
                    value = save_expr (arg);
                  incremented = build (((code == PREINCREMENT_EXPR
                                         || code == POSTINCREMENT_EXPR)
                                        ? PLUS_EXPR : MINUS_EXPR),
                                       argtype, value, inc);
                  TREE_SIDE_EFFECTS (incremented) = 1;
                  modify = build_modify_expr (arg, NOP_EXPR, incremented);
                  value = build (COMPOUND_EXPR, TREE_TYPE (arg), modify, value);
                  TREE_USED (value) = 1;
                  return value;
                }
              break;

            default:
              goto give_up;
            }
      give_up:

        /* Complain about anything else that is not a true lvalue.  */
        if (!lvalue_or_else (arg, ((code == PREINCREMENT_EXPR
                                    || code == POSTINCREMENT_EXPR)
                                   ? "increment" : "decrement")))
          return error_mark_node;

        /* Report a read-only lvalue.  */
        if (TREE_READONLY (arg))
          readonly_warning (arg,
                            ((code == PREINCREMENT_EXPR
                              || code == POSTINCREMENT_EXPR)
                             ? "increment" : "decrement"));

        val = build (code, TREE_TYPE (arg), arg, inc);
        TREE_SIDE_EFFECTS (val) = 1;
        val = convert (result_type, val);
        if (TREE_CODE (val) != code)
          TREE_NO_UNUSED_WARNING (val) = 1;
        return val;
      }

    case ADDR_EXPR:
      /* Note that this operation never does default_conversion
         regardless of NOCONVERT.  */

      /* Let &* cancel out to simplify resulting code.  */
      if (TREE_CODE (arg) == INDIRECT_REF)
        {
          /* Don't let this be an lvalue.  */
          if (lvalue_p (TREE_OPERAND (arg, 0)))
            return non_lvalue (TREE_OPERAND (arg, 0));
          return TREE_OPERAND (arg, 0);
        }

      /* For &x[y], return x+y */
      if (TREE_CODE (arg) == ARRAY_REF)
        {
          if (mark_addressable (TREE_OPERAND (arg, 0)) == 0)
            return error_mark_node;
#ifdef GPC
          if (! allow_packed_var_parameters
              && (PEDANTIC (B_D_PASCAL))
              && PASCAL_TYPE_PACKED (TREE_TYPE (TREE_OPERAND (arg, 0))))
            pedwarn ("ISO Pascal forbids this use of packed array components");

          /* Kenner's get_inner_reference() code affected also this */
          {
            /* Pascal arrays are not pointers by default */

            tree ind = default_conversion (TREE_OPERAND (arg, 1));

            if (TREE_CODE (TREE_TYPE (ind)) == INTEGER_TYPE &&
                ORDINAL_TYPE (TREE_CODE (TREE_TYPE (ind))))
              ind = convert (integer_type_node, ind);

            return build_binary_op
                        (PLUS_EXPR,
                         convert_array_to_pointer (TREE_OPERAND (arg, 0)),
                         fold (build (MINUS_EXPR, TREE_TYPE (ind),
                                      ind,
                                      TYPE_MIN_VALUE
                                        (TYPE_DOMAIN
                                           (TREE_TYPE
                                              (TREE_OPERAND (arg, 0)))))),
                         0);
          }
#else /* not GPC */
          return build_binary_op (PLUS_EXPR, TREE_OPERAND (arg, 0),
                                  TREE_OPERAND (arg, 1), 1);
#endif /* not GPC */
        }

      /* Handle complex lvalues (when permitted)
         by reduction to simpler cases.  */
      val = unary_complex_lvalue (code, arg);
      if (val != 0)
        return val;

#if 0 /* Turned off because inconsistent;
         float f; *&(int)f = 3.4 stores in int format
         whereas (int)f = 3.4 stores in float format.  */
      /* Address of a cast is just a cast of the address
         of the operand of the cast.  */
      switch (TREE_CODE (arg))
        {
        case NOP_EXPR:
        case CONVERT_EXPR:
        case FLOAT_EXPR:
        case FIX_TRUNC_EXPR:
        case FIX_FLOOR_EXPR:
        case FIX_ROUND_EXPR:
        case FIX_CEIL_EXPR:
          if (pedantic)
            pedwarn ("ANSI C forbids the address of a cast expression");
          return convert (build_pointer_type (TREE_TYPE (arg)),
                          build_unary_op (ADDR_EXPR, TREE_OPERAND (arg, 0),
                                          0));
        }
#endif

      /* Allow the address of a constructor if all the elements
         are constant.  */
      if (TREE_CODE (arg) == CONSTRUCTOR && TREE_CONSTANT (arg))
        ;
      /* Anything not already handled and not a true memory reference
         is an error.  */
#ifdef GPC
      else if (TREE_CODE (arg) == CONSTRUCTOR)
        {
          /* Create a temporary variable for non-constant
           * constructors.
           */
          tree temp = make_new_variable ("structured_value", TREE_TYPE (arg));
          expand_expr_stmt (build_modify_expr (temp, NOP_EXPR, arg));
          arg = temp;
        }
      else if (TREE_CODE (arg) == NOP_EXPR
               || TREE_CODE (arg) == CONVERT_EXPR)
        {
          /* Address of a cast is just a cast of the address
           * of the operand of the cast.
           */
          return convert (build_pointer_type (TREE_TYPE (arg)),
                          build_unary_op (ADDR_EXPR, TREE_OPERAND (arg, 0),
                                          0));
        }
      else if (TREE_CODE (arg) == CONST_DECL)
        ;
      else if (typecode != FUNCTION_TYPE && ! lvalue_p (arg))
        {
          error ("reference expected, value given");
          return error_mark_node;
        }
#else /* not GPC */
      else if (typecode != FUNCTION_TYPE && !lvalue_or_else (arg, "unary `&'"))
        return error_mark_node;
#endif /* not GPC */

      /* Ordinary case; arg is a COMPONENT_REF or a decl.  */
      argtype = TREE_TYPE (arg);
      /* If the lvalue is const or volatile,
         merge that into the type that the address will point to.  */
      if (TREE_CODE_CLASS (TREE_CODE (arg)) == 'd'
          || TREE_CODE_CLASS (TREE_CODE (arg)) == 'r')
        {
          if (TREE_READONLY (arg) || TREE_THIS_VOLATILE (arg))
            argtype = build_c_type_variant (argtype,
                                            TREE_READONLY (arg),
                                            TREE_THIS_VOLATILE (arg));
        }

      argtype = build_pointer_type (argtype);

      if (mark_addressable (arg) == 0)
        return error_mark_node;

      {
        tree addr;

#ifdef GPC
        if (! allow_packed_var_parameters
            && TREE_CODE (arg) == BIT_FIELD_REF
            && PASCAL_TYPE_PACKED (TREE_TYPE (TREE_OPERAND (arg, 0))))
          error ("invalid use of component of packed array `%s'",
                 IDENTIFIER_POINTER (DECL_NAME (TREE_OPERAND (arg, 0))));
#endif /* GPC */

        if (TREE_CODE (arg) == COMPONENT_REF)
          {
            tree field = TREE_OPERAND (arg, 1);
#ifdef GPC
            if (! allow_packed_var_parameters
                && (pedantic || flag_what_pascal)
                && PASCAL_TYPE_PACKED (TREE_TYPE (TREE_OPERAND (arg, 0))))
              pedwarn ("ISO Pascal forbids this use of packed record fields");
#endif /* GPC */

            addr = build_unary_op (ADDR_EXPR, TREE_OPERAND (arg, 0), 0);

            if (DECL_C_BIT_FIELD (field))
              {
#ifdef GPC
                error ("invalid use of field `%s' of packed record `%s'",
                       IDENTIFIER_POINTER (DECL_NAME (field)),
                       IDENTIFIER_POINTER (DECL_NAME (TREE_OPERAND (arg, 0))));
#else /* not GPC */
                error ("attempt to take address of bit-field structure member `%s'",
                       IDENTIFIER_POINTER (DECL_NAME (field)));
#endif /* not GPC */
                return error_mark_node;
              }

            addr = convert (argtype, addr);

            if (! integer_zerop (DECL_FIELD_BITPOS (field)))
              {
                tree offset
                  = size_binop (EASY_DIV_EXPR, DECL_FIELD_BITPOS (field),
                                size_int (BITS_PER_UNIT));
                int flag = TREE_CONSTANT (addr);
                addr = fold (build (PLUS_EXPR, argtype,
                                    addr, convert (argtype, offset)));
                TREE_CONSTANT (addr) = flag;
              }
          }
        else
          addr = build1 (code, argtype, arg);

        /* Address of a static or external variable or
           file-scope function counts as a constant.  */
        if (staticp (arg)
            && ! (TREE_CODE (arg) == FUNCTION_DECL
                  && DECL_CONTEXT (arg) != 0))
          TREE_CONSTANT (addr) = 1;
        return addr;
      }

    default:
      break;
    }

  if (!errstring)
    {
      if (argtype == 0)
        argtype = TREE_TYPE (arg);
      return fold (build1 (code, argtype, arg));
    }

  error (errstring);
  return error_mark_node;
}

#if 0
/* If CONVERSIONS is a conversion expression or a nested sequence of such,
   convert ARG with the same conversions in the same order
   and return the result.  */

static tree
convert_sequence (conversions, arg)
     tree conversions;
     tree arg;
{
  switch (TREE_CODE (conversions))
    {
    case NOP_EXPR:
    case CONVERT_EXPR:
    case FLOAT_EXPR:
    case FIX_TRUNC_EXPR:
    case FIX_FLOOR_EXPR:
    case FIX_ROUND_EXPR:
    case FIX_CEIL_EXPR:
      return convert (TREE_TYPE (conversions),
                      convert_sequence (TREE_OPERAND (conversions, 0),
                                        arg));

    default:
      return arg;
    }
}
#endif /* 0 */

/* Return nonzero if REF is an lvalue valid for this language.
   Lvalues can be assigned, unless their type has TYPE_READONLY.
   Lvalues can have their address taken, unless they have DECL_REGISTER.  */

int
lvalue_p (ref)
     tree ref;
{
  register enum tree_code code = TREE_CODE (ref);

  switch (code)
    {
    case REALPART_EXPR:
    case IMAGPART_EXPR:
    case COMPONENT_REF:
#ifdef GPC
    case BIT_FIELD_REF:
#endif /* GPC */
      return lvalue_p (TREE_OPERAND (ref, 0));

    case STRING_CST:
      return 1;

#ifdef GPC
    /* Special case: a reference which has been converted to a pointer
     * using `@' is an lvalue.
     */
    case CONVERT_EXPR:
      if (TREE_CODE (TREE_TYPE (ref)) == POINTER_TYPE
          && TREE_CODE (TREE_TYPE (TREE_OPERAND (ref, 0))) == REFERENCE_TYPE)
        return 1;
      else
        return 0;
#endif /* GPC */

    case INDIRECT_REF:
    case ARRAY_REF:
    case VAR_DECL:
    case PARM_DECL:
    case RESULT_DECL:
    case ERROR_MARK:
      return (TREE_CODE (TREE_TYPE (ref)) != FUNCTION_TYPE
              && TREE_CODE (TREE_TYPE (ref)) != METHOD_TYPE);

    case BIND_EXPR:
    case RTL_EXPR:
      return TREE_CODE (TREE_TYPE (ref)) == ARRAY_TYPE;

    default:
      return 0;
    }
}

/* Return nonzero if REF is an lvalue valid for this language;
   otherwise, print an error message and return zero.  */

#ifdef EGCS92
int
lvalue_or_else (ref, string)
     tree ref;
     const char *string;
#else
int
lvalue_or_else (ref, string)
     tree ref;
     char *string;
#endif
{
  int win = lvalue_p (ref);
  if (! win)
    error ("invalid lvalue in %s", string);
  return win;
}

/* Apply unary lvalue-demanding operator CODE to the expression ARG
   for certain kinds of expressions which are not really lvalues
   but which we can accept as lvalues.

   If ARG is not a kind of expression we can handle, return zero.  */

static tree
unary_complex_lvalue (code, arg)
     enum tree_code code;
     tree arg;
{
  /* Handle (a, b) used as an "lvalue".  */
  if (TREE_CODE (arg) == COMPOUND_EXPR)
    {
      tree real_result = build_unary_op (code, TREE_OPERAND (arg, 1), 0);

      /* If this returns a function type, it isn't really being used as
         an lvalue, so don't issue a warning about it.  */
      if (TREE_CODE (TREE_TYPE (arg)) != FUNCTION_TYPE)
        pedantic_lvalue_warning (COMPOUND_EXPR);

      return build (COMPOUND_EXPR, TREE_TYPE (real_result),
                    TREE_OPERAND (arg, 0), real_result);
    }

  /* Handle (a ? b : c) used as an "lvalue".  */
  if (TREE_CODE (arg) == COND_EXPR)
    {
      pedantic_lvalue_warning (COND_EXPR);
      if (TREE_CODE (TREE_TYPE (arg)) != FUNCTION_TYPE)
        pedantic_lvalue_warning (COMPOUND_EXPR);

      return (build_conditional_expr
              (TREE_OPERAND (arg, 0),
               build_unary_op (code, TREE_OPERAND (arg, 1), 0),
               build_unary_op (code, TREE_OPERAND (arg, 2), 0)));
    }

  return 0;
}

/* If pedantic, warn about improper lvalue.   CODE is either COND_EXPR
   COMPOUND_EXPR, or CONVERT_EXPR (for casts).  */

static void
pedantic_lvalue_warning (code)
     enum tree_code code;
{
  if (pedantic)
    pedwarn ("ANSI C forbids use of %s expressions as lvalues",
             code == COND_EXPR ? "conditional"
             : code == COMPOUND_EXPR ? "compound" : "cast");
}

/* Warn about storing in something that is `const'.  */

#ifdef EGCS92
void
readonly_warning (arg, string)
     tree arg;
     const char *string;
#else
void
readonly_warning (arg, string)
     tree arg;
     char *string;
#endif
{
  char buf[80];
  strcpy (buf, string);

  /* Forbid assignments to iterators.  */
  if (TREE_CODE (arg) == VAR_DECL && ITERATOR_P (arg))
    {
      strcat (buf, " of iterator `%s'");
      pedwarn (buf, IDENTIFIER_POINTER (DECL_NAME (arg)));
    }

  if (TREE_CODE (arg) == COMPONENT_REF)
    {
#ifdef GPC
      if (TREE_READONLY (TREE_OPERAND (arg, 0))
          || TYPE_READONLY (TREE_TYPE (TREE_OPERAND (arg, 0))))
#else /* not GPC */
      if (TYPE_READONLY (TREE_TYPE (TREE_OPERAND (arg, 0))))
#endif /* not GPC */
        readonly_warning (TREE_OPERAND (arg, 0), string);
      else
        {
          strcat (buf, " of read-only member `%s'");
          pedwarn (buf, IDENTIFIER_POINTER (DECL_NAME (TREE_OPERAND (arg, 1))));
        }
    }
  else if (TREE_CODE (arg) == VAR_DECL)
    {
      strcat (buf, " of read-only variable `%s'");
      pedwarn (buf, IDENTIFIER_POINTER (DECL_NAME (arg)));
    }
  else
    {
      pedwarn ("%s of read-only location", buf);
    }
}

/* Mark EXP saying that we need to be able to take the
   address of it; it should not be allocated in a register.
   Value is 1 if successful.  */

int
mark_addressable (exp)
     tree exp;
{
  register tree x = exp;
  while (1)
    switch (TREE_CODE (x))
      {
      case COMPONENT_REF:
        if (DECL_C_BIT_FIELD (TREE_OPERAND (x, 1)))
          {
            error ("cannot take address of bitfield `%s'",
                   IDENTIFIER_POINTER (DECL_NAME (TREE_OPERAND (x, 1))));
            return 0;
          }

        /* ... fall through ...  */

      case ADDR_EXPR:
      case ARRAY_REF:
      case REALPART_EXPR:
      case IMAGPART_EXPR:
        x = TREE_OPERAND (x, 0);
        break;

      case CONSTRUCTOR:
        TREE_ADDRESSABLE (x) = 1;
        return 1;

      case VAR_DECL:
      case CONST_DECL:
      case PARM_DECL:
      case RESULT_DECL:
        if (DECL_REGISTER (x) && !TREE_ADDRESSABLE (x)
            && DECL_NONLOCAL (x))
          {
            if (TREE_PUBLIC (x))
              {
                error ("global register variable `%s' used in nested function",
                       IDENTIFIER_POINTER (DECL_NAME (x)));
                return 0;
              }
            pedwarn ("register variable `%s' used in nested function",
                     IDENTIFIER_POINTER (DECL_NAME (x)));
          }
        else if (DECL_REGISTER (x) && !TREE_ADDRESSABLE (x))
          {
            if (TREE_PUBLIC (x))
              {
                error ("address of global register variable `%s' requested",
                       IDENTIFIER_POINTER (DECL_NAME (x)));
                return 0;
              }

            /* If we are making this addressable due to its having
               volatile components, give a different error message.  Also
               handle the case of an unnamed parameter by not trying
               to give the name.  */

            else if (C_TYPE_FIELDS_VOLATILE (TREE_TYPE (x)))
              {
                error ("cannot put object with volatile field into register");
                return 0;
              }

            pedwarn ("address of register variable `%s' requested",
                     IDENTIFIER_POINTER (DECL_NAME (x)));
          }
        put_var_into_stack (x);

        /* drops in */
      case FUNCTION_DECL:
        TREE_ADDRESSABLE (x) = 1;
#if 0  /* poplevel deals with this now.  */
        if (DECL_CONTEXT (x) == 0)
          TREE_ADDRESSABLE (DECL_ASSEMBLER_NAME (x)) = 1;
#endif

      default:
        return 1;
    }
}

/* Build and return a conditional expression IFEXP ? OP1 : OP2.  */

tree
build_conditional_expr (ifexp, op1, op2)
     tree ifexp, op1, op2;
{
  register tree type1;
  register tree type2;
  register enum tree_code code1;
  register enum tree_code code2;
  register tree result_type = NULL;
  tree orig_op1 = op1, orig_op2 = op2;

  ifexp = truthvalue_conversion (default_conversion (ifexp));

#if 0 /* Produces wrong result if within sizeof.  */
  /* Don't promote the operands separately if they promote
     the same way.  Return the unpromoted type and let the combined
     value get promoted if necessary.  */

  if (TREE_TYPE (op1) == TREE_TYPE (op2)
      && TREE_CODE (TREE_TYPE (op1)) != ARRAY_TYPE
      && TREE_CODE (TREE_TYPE (op1)) != ENUMERAL_TYPE
      && TREE_CODE (TREE_TYPE (op1)) != FUNCTION_TYPE)
    {
      if (TREE_CODE (ifexp) == INTEGER_CST)
        return pedantic_non_lvalue (integer_zerop (ifexp) ? op2 : op1);

      return fold (build (COND_EXPR, TREE_TYPE (op1), ifexp, op1, op2));
    }
#endif

  /* Promote both alternatives.  */

  if (TREE_CODE (TREE_TYPE (op1)) != VOID_TYPE)
    op1 = default_conversion (op1);
  if (TREE_CODE (TREE_TYPE (op2)) != VOID_TYPE)
    op2 = default_conversion (op2);

  if (TREE_CODE (ifexp) == ERROR_MARK
      || TREE_CODE (TREE_TYPE (op1)) == ERROR_MARK
      || TREE_CODE (TREE_TYPE (op2)) == ERROR_MARK)
    return error_mark_node;

  type1 = TREE_TYPE (op1);
  code1 = TREE_CODE (type1);
  type2 = TREE_TYPE (op2);
  code2 = TREE_CODE (type2);

  /* Quickly detect the usual case where op1 and op2 have the same type
     after promotion.  */
  if (TYPE_MAIN_VARIANT (type1) == TYPE_MAIN_VARIANT (type2))
    {
      if (type1 == type2)
        result_type = type1;
      else
        result_type = TYPE_MAIN_VARIANT (type1);
    }
  else if ((code1 == INTEGER_TYPE || code1 == REAL_TYPE)
           && (code2 == INTEGER_TYPE || code2 == REAL_TYPE))
    {
      result_type = common_type (type1, type2);
    }
  else if (code1 == VOID_TYPE || code2 == VOID_TYPE)
    {
      if (pedantic && (code1 != VOID_TYPE || code2 != VOID_TYPE))
        pedwarn ("ANSI C forbids conditional expr with only one void side");
      result_type = void_type_node;
    }
  else if (code1 == POINTER_TYPE && code2 == POINTER_TYPE)
    {
      if (comp_target_types (type1, type2))
        result_type = common_type (type1, type2);
      else if (integer_zerop (op1) && TREE_TYPE (type1) == void_type_node
               && TREE_CODE (orig_op1) != NOP_EXPR)
        result_type = qualify_type (type2, type1);
      else if (integer_zerop (op2) && TREE_TYPE (type2) == void_type_node
               && TREE_CODE (orig_op2) != NOP_EXPR)
        result_type = qualify_type (type1, type2);
      else if (TYPE_MAIN_VARIANT (TREE_TYPE (type1)) == void_type_node)
        {
          if (pedantic && TREE_CODE (TREE_TYPE (type2)) == FUNCTION_TYPE)
            pedwarn ("ANSI C forbids conditional expr between untyped pointer and function pointer");
          result_type = qualify_type (type1, type2);
        }
      else if (TYPE_MAIN_VARIANT (TREE_TYPE (type2)) == void_type_node)
        {
          if (pedantic && TREE_CODE (TREE_TYPE (type1)) == FUNCTION_TYPE)
            pedwarn ("ANSI C forbids conditional expr between untyped pointer and function pointer");
          result_type = qualify_type (type2, type1);
        }
#ifdef GPC
      else if (code1 == SET_TYPE && code2 == SET_TYPE)
        {
          /* @@@ Check for set compatibility */
#if 0
          converted = 1;
#endif
        }
      else if (code1 == ARRAY_TYPE && code2 == ARRAY_TYPE
               && comptypes(type1, type2))
        {
#if 0
          converted = 1;
#endif
          if (! is_string_type (op1, 1) || ! is_string_type (op2, 1))
            {
              pedwarn ("compared arrays are not of string type");
              result_type = NULL_TREE;
            }
        }
#endif /* GPC */
      else
        {
          pedwarn ("pointer type mismatch in conditional expression");
          result_type = build_pointer_type (void_type_node);
        }
    }
  else if (code1 == POINTER_TYPE && code2 == INTEGER_TYPE)
    {
      if (! integer_zerop (op2))
        pedwarn ("pointer/integer type mismatch in conditional expression");
      else
        {
          op2 = null_pointer_node;
#if 0  /* The spec seems to say this is permitted.  */
          if (pedantic && TREE_CODE (type1) == FUNCTION_TYPE)
            pedwarn ("ANSI C forbids conditional expr between 0 and function pointer");
#endif
        }
      result_type = type1;
    }
  else if (code2 == POINTER_TYPE && code1 == INTEGER_TYPE)
    {
      if (!integer_zerop (op1))
        pedwarn ("pointer/integer type mismatch in conditional expression");
      else
        {
          op1 = null_pointer_node;
#if 0  /* The spec seems to say this is permitted.  */
          if (pedantic && TREE_CODE (type2) == FUNCTION_TYPE)
            pedwarn ("ANSI C forbids conditional expr between 0 and function pointer");
#endif
        }
      result_type = type2;
    }

  if (!result_type)
    {
      if (flag_cond_mismatch)
        result_type = void_type_node;
      else
        {
          error ("type mismatch in conditional expression");
          return error_mark_node;
        }
    }

  /* Merge const and volatile flags of the incoming types.  */
  result_type
    = build_type_variant (result_type,
                          TREE_READONLY (op1) || TREE_READONLY (op2),
                          TREE_THIS_VOLATILE (op1) || TREE_THIS_VOLATILE (op2));

  if (result_type != TREE_TYPE (op1))
    op1 = convert_and_check (result_type, op1);
  if (result_type != TREE_TYPE (op2))
    op2 = convert_and_check (result_type, op2);

#if 0
  if (code1 == RECORD_TYPE || code1 == UNION_TYPE)
    {
      result_type = TREE_TYPE (op1);
      if (TREE_CONSTANT (ifexp))
        return pedantic_non_lvalue (integer_zerop (ifexp) ? op2 : op1);

      if (TYPE_MODE (result_type) == BLKmode)
        {
          register tree tempvar
            = build_decl (VAR_DECL, NULL_TREE, result_type);
          register tree xop1 = build_modify_expr (tempvar, op1);
          register tree xop2 = build_modify_expr (tempvar, op2);
          register tree result = fold (build (COND_EXPR, result_type,
                                              ifexp, xop1, xop2));

          layout_decl (tempvar, TYPE_ALIGN (result_type));
          /* No way to handle variable-sized objects here.
             I fear that the entire handling of BLKmode conditional exprs
             needs to be redone.  */
          if (TREE_CODE (DECL_SIZE (tempvar)) != INTEGER_CST)
            abort ();
          DECL_RTL (tempvar)
            = assign_stack_local (DECL_MODE (tempvar),
                                  (TREE_INT_CST_LOW (DECL_SIZE (tempvar))
                                   + BITS_PER_UNIT - 1)
                                  / BITS_PER_UNIT,
                                  0);

          TREE_SIDE_EFFECTS (result)
            = TREE_SIDE_EFFECTS (ifexp) | TREE_SIDE_EFFECTS (op1)
              | TREE_SIDE_EFFECTS (op2);
          return build (COMPOUND_EXPR, result_type, result, tempvar);
        }
    }
#endif /* 0 */

  if (TREE_CODE (ifexp) == INTEGER_CST)
    return pedantic_non_lvalue (integer_zerop (ifexp) ? op2 : op1);

  return fold (build (COND_EXPR, result_type, ifexp, op1, op2));
}

/* Given a list of expressions, return a compound expression
   that performs them all and returns the value of the last of them.  */

tree
build_compound_expr (list)
     tree list;
{
  return internal_build_compound_expr (list, TRUE);
}

static tree
internal_build_compound_expr (list, first_p)
     tree list;
     int first_p;
{
  register tree rest;

  if (TREE_CHAIN (list) == 0)
    {
#if 0 /* If something inside inhibited lvalueness, we should not override.  */
      /* Consider (x, y+0), which is not an lvalue since y+0 is not.  */

      /* Strip NON_LVALUE_EXPRs since we aren't using as an lvalue.  */
      if (TREE_CODE (list) == NON_LVALUE_EXPR)
        list = TREE_OPERAND (list, 0);
#endif

      /* Don't let (0, 0) be null pointer constant.  */
      if (!first_p && integer_zerop (TREE_VALUE (list)))
        return non_lvalue (TREE_VALUE (list));
      return TREE_VALUE (list);
    }

  if (TREE_CHAIN (list) != 0 && TREE_CHAIN (TREE_CHAIN (list)) == 0)
    {
      /* Convert arrays to pointers when there really is a comma operator.  */
      if (TREE_CODE (TREE_TYPE (TREE_VALUE (TREE_CHAIN (list)))) == ARRAY_TYPE)
        TREE_VALUE (TREE_CHAIN (list))
          = default_conversion (TREE_VALUE (TREE_CHAIN (list)));
    }

  rest = internal_build_compound_expr (TREE_CHAIN (list), FALSE);

  if (! TREE_SIDE_EFFECTS (TREE_VALUE (list)))
    {
      /* The left-hand operand of a comma expression is like an expression
         statement: with -W or -Wunused, we should warn if it doesn't have
         any side-effects, unless it was explicitly cast to (void).  */
      if ((extra_warnings || warn_unused)
           && ! (TREE_CODE (TREE_VALUE (list)) == CONVERT_EXPR
                && TREE_TYPE (TREE_VALUE (list)) == void_type_node))
        warning ("left-hand operand of comma expression has no effect");

      /* When pedantic, a compound expression can be neither an lvalue
         nor an integer constant expression.  */
      if (! pedantic)
        return rest;
    }

  /* With -Wunused, we should also warn if the left-hand operand does have
     side-effects, but computes a value which is not used.  For example, in
     `foo() + bar(), baz()' the result of the `+' operator is not used,
     so we should issue a warning.  */
  else if (warn_unused)
    warn_if_unused_value (TREE_VALUE (list));

  return build (COMPOUND_EXPR, TREE_TYPE (rest), TREE_VALUE (list), rest);
}

/* Build an expression representing a cast to type TYPE of expression EXPR.  */

tree
build_c_cast (type, expr)
     register tree type;
     tree expr;
{
  register tree value = expr;

  if (type == error_mark_node || expr == error_mark_node)
    return error_mark_node;
  type = TYPE_MAIN_VARIANT (type);

#if 0
  /* Strip NON_LVALUE_EXPRs since we aren't using as an lvalue.  */
  if (TREE_CODE (value) == NON_LVALUE_EXPR)
    value = TREE_OPERAND (value, 0);
#endif

#ifdef GPC
  /* Dereference procedural variables rather than cast them.
   * Use the address operator `@' to cast a reference to a pointer.
   */
  if (PASCAL_PROCEDURAL_VARIABLE (value))
    value = build_indirect_ref (value, "procedure/function reference");
  if (TREE_CODE (TREE_TYPE (value)) == FUNCTION_TYPE)
    value = probably_call_function (value);
#else /* not GPC */
  if (TREE_CODE (type) == ARRAY_TYPE)
    {
      error ("cast specifies array type");
      return error_mark_node;
    }
#endif /* not GPC */

  if (TREE_CODE (type) == FUNCTION_TYPE)
    {
      error ("cast specifies function type");
      return error_mark_node;
    }

#ifndef GPC
  if (type == TREE_TYPE (value))
    {
      if (pedantic)
        {
          if (TREE_CODE (type) == RECORD_TYPE
              || TREE_CODE (type) == UNION_TYPE)
            pedwarn ("ANSI C forbids casting nonscalar to the same type");
        }
    }
  else if (TREE_CODE (type) == UNION_TYPE)
    {
      tree field;
      if (TREE_CODE (TREE_TYPE (value)) == ARRAY_TYPE
          || TREE_CODE (TREE_TYPE (value)) == FUNCTION_TYPE)
        value = default_conversion (value);

      for (field = TYPE_FIELDS (type); field; field = TREE_CHAIN (field))
        if (comptypes (TYPE_MAIN_VARIANT (TREE_TYPE (field)),
                       TYPE_MAIN_VARIANT (TREE_TYPE (value))))
          break;

      if (field)
        {
          char *name;
          tree t;

          if (pedantic)
            pedwarn ("ANSI C forbids casts to union type");
          if (TYPE_NAME (type) != 0)
            {
              if (TREE_CODE (TYPE_NAME (type)) == IDENTIFIER_NODE)
                name = IDENTIFIER_POINTER (TYPE_NAME (type));
              else
                name = IDENTIFIER_POINTER (DECL_NAME (TYPE_NAME (type)));
            }
          else
            name = "";
          t = digest_init (type, build (CONSTRUCTOR, type, NULL_TREE,
                                        build_tree_list (field, value)),
                           0, 0);
          TREE_CONSTANT (t) = TREE_CONSTANT (value);
          return t;
        }
      error ("cast to union type from type not present in union");
      return error_mark_node;
    }
  else
#else /* GPC */
  if (type != TREE_TYPE (value))
#endif /* GPC */
    {
      tree otype, ovalue;

      /* If casting to void, avoid the error that would come
         from default_conversion in the case of a non-lvalue array.  */
      if (type == void_type_node)
        return build1 (CONVERT_EXPR, type, value);

#ifdef GPC
      if (TREE_CODE (TREE_TYPE (value)) == FUNCTION_TYPE)
        {
          error ("cannot cast a function");
          return error_mark_node;
        }
#else /* not GPC */
      /* Convert functions and arrays to pointers,
         but don't convert any other types.  */
      if (TREE_CODE (TREE_TYPE (value)) == FUNCTION_TYPE
          || TREE_CODE (TREE_TYPE (value)) == ARRAY_TYPE)
        value = default_conversion (value);
#endif /* not GPC */
      otype = TREE_TYPE (value);

      /* Optionally warn about potentially worrisome casts.  */

      if (warn_cast_qual
          && TREE_CODE (type) == POINTER_TYPE
          && TREE_CODE (otype) == POINTER_TYPE)
        {
          if (TYPE_VOLATILE (TREE_TYPE (otype))
              && ! TYPE_VOLATILE (TREE_TYPE (type)))
            pedwarn ("cast discards `volatile' from pointer target type");
          if (TYPE_READONLY (TREE_TYPE (otype))
              && ! TYPE_READONLY (TREE_TYPE (type)))
            pedwarn ("cast discards `const' from pointer target type");
        }

      /* Warn about possible alignment problems.  */
#ifdef GPC
      if ((flag_what_pascal & B_D_PASCAL) == 0
#else /* not GPC */
      if (STRICT_ALIGNMENT && warn_cast_align
#endif /* not GPC */
          && TREE_CODE (type) == POINTER_TYPE
          && TREE_CODE (otype) == POINTER_TYPE
          && TREE_CODE (TREE_TYPE (otype)) != VOID_TYPE
          && TREE_CODE (TREE_TYPE (otype)) != FUNCTION_TYPE
          /* Don't warn about opaque types, where the actual alignment
             restriction is unknown.  */
          && !((TREE_CODE (TREE_TYPE (otype)) == UNION_TYPE
                || TREE_CODE (TREE_TYPE (otype)) == RECORD_TYPE)
               && TYPE_MODE (TREE_TYPE (otype)) == VOIDmode)
          && TYPE_ALIGN (TREE_TYPE (type)) > TYPE_ALIGN (TREE_TYPE (otype)))
        warning ("cast increases required alignment of target type");

      if (TREE_CODE (type) == INTEGER_TYPE
          && TREE_CODE (otype) == POINTER_TYPE
          && TYPE_PRECISION (type) != TYPE_PRECISION (otype)
          && !TREE_CONSTANT (value))
        warning ("cast from pointer to integer of different size");

      if (warn_bad_function_cast
          && TREE_CODE (value) == CALL_EXPR
          && TREE_CODE (type) != TREE_CODE (otype))
        warning ("cast does not match function type");

      if (TREE_CODE (type) == POINTER_TYPE
          && TREE_CODE (otype) == INTEGER_TYPE
          && TYPE_PRECISION (type) != TYPE_PRECISION (otype)
#if 0
          /* Don't warn about converting 0 to pointer,
             provided the 0 was explicit--not cast or made by folding.  */
          && !(TREE_CODE (value) == INTEGER_CST && integer_zerop (value))
#endif
          /* Don't warn about converting any constant.  */
          && !TREE_CONSTANT (value))
        warning ("cast to pointer from integer of different size");

      ovalue = value;
#ifdef GPC
      if (((ORDINAL_TYPE (TREE_CODE (otype))
            || TREE_CODE (otype) == POINTER_TYPE
            || TREE_CODE (otype) == REFERENCE_TYPE)
           && (ORDINAL_TYPE (TREE_CODE (type))
               || TREE_CODE (type) == POINTER_TYPE
               || TREE_CODE (type) == REFERENCE_TYPE))
          || ((TREE_CODE (otype) == REAL_TYPE
               || TREE_CODE (otype) == INTEGER_TYPE)
              && TREE_CODE (type) == REAL_TYPE))
        {
          if (TREE_CODE (value) == INTEGER_CST
              && ORDINAL_TYPE (TREE_CODE (otype))
              && ORDINAL_TYPE (TREE_CODE (type)))
            {
              /* Do some trivial range checking,
               * and pedwarn about range overflows in
               * explicit casts between ordinal types.
               */
              if ((TYPE_MIN_VALUE (type)
                   && TREE_CODE (TYPE_MIN_VALUE (type)) == INTEGER_CST
                   && int_cst_lt_always (value, TYPE_MIN_VALUE (type)))
                  || (TYPE_MAX_VALUE (type)
                      && TREE_CODE (TYPE_MAX_VALUE (type)) == INTEGER_CST
                      && int_cst_lt_always (TYPE_MAX_VALUE (type), value)))
                error ("constant out of range");
            }

          if (TREE_CODE (type) != POINTER_TYPE
              && TREE_CODE (TREE_TYPE (value)) != POINTER_TYPE
              && TREE_UNSIGNED (type) != TREE_UNSIGNED (TREE_TYPE (value))
              && TYPE_PRECISION (type) != TYPE_PRECISION (TREE_TYPE (value)))
            {
              /* If the source and the target type differ both in size and
               * in sign, do the conversion in two stages such that the size
               * conversion is done on the unsigned type to avoid the value
               * being 1-padded.
               *
               * Examples:  Integer --> Cardinal --> LongCard
               *            Byte --> Cardinal --> Integer
               */
              tree new_type = build_type_copy (TREE_TYPE (value));
              if (TREE_UNSIGNED (new_type))
                TYPE_PRECISION (new_type) = TYPE_PRECISION (type);
              else
                TREE_UNSIGNED (new_type) = TREE_UNSIGNED (type);
              value = convert (new_type, value);
            }
          value = convert (type, value);
        }
      else
        {
          if (! TREE_CONSTANT (value)
              && TREE_CODE (otype) != VOID_TYPE
              && TREE_CODE (type) != VOID_TYPE
              && TYPE_SIZE (otype) != TYPE_SIZE (type)
              && ! (TREE_CODE (TYPE_SIZE (otype)) == INTEGER_CST
                    && TREE_CODE (TYPE_SIZE (type)) == INTEGER_CST
                    && TREE_INT_CST_LOW (TYPE_SIZE (otype))
                       == TREE_INT_CST_LOW (TYPE_SIZE (type))
                    && TREE_INT_CST_HIGH (TYPE_SIZE (otype))
                       == TREE_INT_CST_HIGH (TYPE_SIZE (type)))
              && ! (PASCAL_TYPE_OBJECT (type)
                    && PASCAL_TYPE_OBJECT (otype)
                    && ! pedantic
                    && ! flag_what_pascal
                    && (comp_object_or_schema_pointer_types (type, otype)
                        || comp_object_or_schema_pointer_types (otype, type))))
            pedwarn ("cast to type of different size");
          value = build_pascal_unary_op (ADDR_EXPR, value, 0);
          type = build_pointer_type (type);
          value = convert (type, value);
          value = build_indirect_ref (value, "type cast");
        }
#else /* not GPC */
      value = convert (type, value);
#endif /* not GPC */

      /* Ignore any integer overflow caused by the cast.  */
      if (TREE_CODE (value) == INTEGER_CST)
        {
          TREE_OVERFLOW (value) = TREE_OVERFLOW (ovalue);
          TREE_CONSTANT_OVERFLOW (value) = TREE_CONSTANT_OVERFLOW (ovalue);
        }
    }

#ifdef GPC
  /* Pedantically, Pascal does not allow casts at all.
   * But a warning has already been given.
   */
#else /* not GPC */
  /* Pedantically, don't let (void *) (FOO *) 0 be a null pointer constant.  */
  if (pedantic && TREE_CODE (value) == INTEGER_CST
      && TREE_CODE (expr) == INTEGER_CST
      && TREE_CODE (TREE_TYPE (expr)) != INTEGER_TYPE)
    value = non_lvalue (value);

  /* If pedantic, don't let a cast be an lvalue.  */
  if (value == expr && pedantic)
    value = non_lvalue (value);
#endif /* not GPC */

  return value;
}

/* Build an assignment expression of lvalue LHS from value RHS.
   MODIFYCODE is the code for a binary operator that we use
   to combine the old value of LHS with RHS to get the new value.
   Or else MODIFYCODE is NOP_EXPR meaning do a simple assignment.  */

tree
build_modify_expr (lhs, modifycode, rhs)
     tree lhs, rhs;
     enum tree_code modifycode;
{
  register tree result;
  tree newrhs;
  tree lhstype = TREE_TYPE (lhs);
  tree olhstype = lhstype;

  /* Types that aren't fully specified cannot be used in assignments.  */
  lhs = require_complete_type (lhs);

  /* Avoid duplicate error messages from operands that had errors.  */
  if (TREE_CODE (lhs) == ERROR_MARK || TREE_CODE (rhs) == ERROR_MARK)
    return error_mark_node;

  /* Strip NON_LVALUE_EXPRs since we aren't using as an lvalue.  */
  /* Do not use STRIP_NOPS here.  We do not want an enumerator
     whose value is 0 to count as a null pointer constant.  */
  if (TREE_CODE (rhs) == NON_LVALUE_EXPR)
    rhs = TREE_OPERAND (rhs, 0);

  newrhs = rhs;

  /* Handle control structure constructs used as "lvalues".  */

#ifdef GPC

#ifndef EGCS
/* This #ifdef is intended to vanish together with all the other
 * "#ifdef EGCS" when GPC will be fully integrated into GCC, so
 * we are _not_ using "#ifndef IS_EXPR_CODE_CLASS" here.
 */
#define IS_EXPR_CODE_CLASS(CLASS) \
  (CLASS == '<' || CLASS == '1' || CLASS == '2' || CLASS == 'e')
#endif /* not EGCS */

  if ((IS_EXPR_CODE_CLASS (TREE_CODE_CLASS (TREE_CODE (lhs)))
       || TREE_CODE (lhs) == VAR_DECL)
      && PASCAL_TREE_PACKED_ACCESS (lhs))
    {
      /* This is a construction to read a component of a packed array.
       * Use it to construct an expression to write to that component.
       *
       * This depends on how build_pascal_array_ref() works.
       */
      tree newlhs, temprhs, access = lhs;
      tree mask, shifted_mask, offset, eraser, brush;
      tree low_lhs, high_lhs, low_assignment, high_assignment;

      /* Extract the mask and the offset from the expression.
       */
      if (TREE_CODE (access) == CONVERT_EXPR)
        access = TREE_OPERAND (access, 0);
      if (TREE_CODE (access) == COND_EXPR)
        access = TREE_OPERAND (access, 2);
      while (TREE_CODE (access) == NOP_EXPR)
        access = TREE_OPERAND (access, 0);
      /*
       * Now ACCESS must be an RSHIFT_EXPR; its arg0 must be a BIT_AND_EXPR.
       */
      if (TREE_CODE (access) != RSHIFT_EXPR
          || TREE_CODE (TREE_OPERAND (access, 0)) != BIT_AND_EXPR)
        abort ();
      offset = TREE_OPERAND (access, 1);
      newlhs = TREE_OPERAND (TREE_OPERAND (access, 0), 0);
      shifted_mask = TREE_OPERAND (TREE_OPERAND (access, 0), 1);

      /* NEWLHS must be a BIT_IOR_EXPR.  Prepare to access it in two halfs.
       */
      if (TREE_CODE (newlhs) != BIT_IOR_EXPR)
        abort ();
      low_lhs = TREE_OPERAND (newlhs, 0);
      while (TREE_CODE (low_lhs) == NOP_EXPR)
        low_lhs = TREE_OPERAND (low_lhs, 0);
      high_lhs = TREE_OPERAND (newlhs, 1);
      if (TREE_CODE (high_lhs) != LSHIFT_EXPR)
        abort ();
      high_lhs = TREE_OPERAND (high_lhs, 0);
      while (TREE_CODE (high_lhs) == NOP_EXPR)
        high_lhs = TREE_OPERAND (high_lhs, 0);

      /* Dig out the non-shifted mask.
       */
      mask = shifted_mask;
      if (TREE_CODE (mask) == NOP_EXPR)
        mask = TREE_OPERAND (mask, 0);
      if (TREE_CODE (mask) != LSHIFT_EXPR
          || TREE_CODE (TREE_OPERAND (mask, 0)) != INTEGER_CST)
        abort ();
      mask = TREE_OPERAND (mask, 0);

      /* NEWRHS will be accessed more than once, so use a temporary variable.
       */
      temprhs = make_new_variable ("assign_packed", TREE_TYPE (newrhs));
      expand_expr_stmt (build_modify_expr (temprhs, NOP_EXPR, newrhs));
      newrhs = temprhs;

      /* Do the modification.
       */
      if (modifycode != NOP_EXPR)
        newrhs = build_binary_op (modifycode, lhs, newrhs, 1);

      /* Now form a new expression to store the lower part
       * of NEWRHS in LOW_LHS.
       *
       * Clear the bits in the target.
       */
      eraser = build_unary_op (BIT_NOT_EXPR, shifted_mask, 0);
      eraser = build_modify_expr (low_lhs, BIT_AND_EXPR, eraser);
      /*
       * Do the assignment.
       */
      brush = build_binary_op (BIT_AND_EXPR, newrhs, mask, 1);
      brush = build_binary_op (LSHIFT_EXPR, brush, offset, 0);
      brush = build_modify_expr (low_lhs, BIT_IOR_EXPR, brush);
      /*
       * Construct a COMPOUND_EXPR holding both.
       */
      low_assignment = build (COMPOUND_EXPR, TREE_TYPE (brush),
                              eraser, brush);

      /* Now do the same for the higher part and HIGH_LHS.
       *
       * Prepare SHIFTED_MASK to access the higher half.
       */
      shifted_mask = build (RSHIFT_EXPR, TREE_TYPE (shifted_mask), shifted_mask,
                            TYPE_SIZE (TREE_TYPE (low_lhs)));
      /*
       * Clear the bits in the target.
       */
      eraser = build_unary_op (BIT_NOT_EXPR, shifted_mask, 0);
      eraser = build_modify_expr (high_lhs, BIT_AND_EXPR, eraser);
      /*
       * Do the assignment.
       */
      brush = build_binary_op (BIT_AND_EXPR, newrhs, mask, 1);
      brush = build_binary_op (LSHIFT_EXPR, brush, offset, 0);
      brush = build_binary_op (RSHIFT_EXPR, brush, TYPE_SIZE (TREE_TYPE (low_lhs)), 0);
      brush = build_modify_expr (high_lhs, BIT_IOR_EXPR, brush);
      /*
       * Construct a COMPOUND_EXPR holding both.
       */
      high_assignment = build (COMPOUND_EXPR, TREE_TYPE (brush),
                               eraser, brush);

      /* Return another COMPOUND_EXPR holding both halfs.
       */
      return build (COMPOUND_EXPR, TREE_TYPE (high_assignment),
                    low_assignment, high_assignment);
    }
  else if (TREE_CODE (lhs) == COMPOUND_EXPR)
    {
      /* COMPOUND_EXPRs are generated by some "magic" functions.
       */
      newrhs = build_modify_expr (TREE_OPERAND (lhs, 1),
                                  modifycode, rhs);
      if (TREE_CODE (newrhs) == ERROR_MARK)
        return error_mark_node;
      return build (COMPOUND_EXPR, lhstype,
                    TREE_OPERAND (lhs, 0), newrhs);
    }
#else /* not GPC */
  switch (TREE_CODE (lhs))
    {
      /* Handle (a, b) used as an "lvalue".  */
    case COMPOUND_EXPR:
      pedantic_lvalue_warning (COMPOUND_EXPR);
      newrhs = build_modify_expr (TREE_OPERAND (lhs, 1),
                                  modifycode, rhs);
      if (TREE_CODE (newrhs) == ERROR_MARK)
        return error_mark_node;
      return build (COMPOUND_EXPR, lhstype,
                    TREE_OPERAND (lhs, 0), newrhs);

      /* Handle (a ? b : c) used as an "lvalue".  */
    case COND_EXPR:
      pedantic_lvalue_warning (COND_EXPR);
      rhs = save_expr (rhs);
      {
        /* Produce (a ? (b = rhs) : (c = rhs))
           except that the RHS goes through a save-expr
           so the code to compute it is only emitted once.  */
        tree cond
          = build_conditional_expr (TREE_OPERAND (lhs, 0),
                                    build_modify_expr (TREE_OPERAND (lhs, 1),
                                                       modifycode, rhs),
                                    build_modify_expr (TREE_OPERAND (lhs, 2),
                                                       modifycode, rhs));
        if (TREE_CODE (cond) == ERROR_MARK)
          return cond;
        /* Make sure the code to compute the rhs comes out
           before the split.  */
        return build (COMPOUND_EXPR, TREE_TYPE (lhs),
                      /* But cast it to void to avoid an "unused" error.  */
                      convert (void_type_node, rhs), cond);
      }
    default:
      break;
    }
#endif /* not GPC */

  /* If a binary op has been requested, combine the old LHS value with the RHS
     producing the value we should actually store into the LHS.  */

  if (modifycode != NOP_EXPR)
    {
      lhs = stabilize_reference (lhs);
      newrhs = build_binary_op (modifycode, lhs, rhs, 1);
    }

  /* Handle a cast used as an "lvalue".
     We have already performed any binary operator using the value as cast.
     Now convert the result to the cast type of the lhs,
     and then true type of the lhs and store it there;
     then convert result back to the cast type to be the value
     of the assignment.  */

  switch (TREE_CODE (lhs))
    {
    case NOP_EXPR:
    case CONVERT_EXPR:
    case FLOAT_EXPR:
    case FIX_TRUNC_EXPR:
    case FIX_FLOOR_EXPR:
    case FIX_ROUND_EXPR:
    case FIX_CEIL_EXPR:
#ifdef GPC
      {
        /* Cast used as an lvalue.
         * Do not convert between types; just copy the data in memory.
         */
        tree inner_lhs = TREE_OPERAND (lhs, 0);

        /* For most types, build_c_cast() already has checked
         * that the sizes of the types match.  For integer
         * types, this must be re-checked here.
         */
        if (TREE_CODE (TYPE_SIZE (TREE_TYPE (inner_lhs))) == INTEGER_CST
            && TREE_CODE (TYPE_SIZE (lhstype)) == INTEGER_CST
            && (TREE_INT_CST_LOW (TYPE_SIZE (TREE_TYPE (inner_lhs)))
                != TREE_INT_CST_LOW (TYPE_SIZE (lhstype))
                || TREE_INT_CST_LOW (TYPE_SIZE (TREE_TYPE (inner_lhs)))
                   != TREE_INT_CST_LOW (TYPE_SIZE (lhstype))))
          {
            pedwarn ("cast to type of different size");

            lhs = build_pascal_unary_op (ADDR_EXPR, inner_lhs, 0);
            lhstype = build_pointer_type (TREE_TYPE (newrhs));
            lhs = convert (lhstype, lhs);
            lhs = build_indirect_ref (lhs, "assignment");
          }
        else
          {
            newrhs = build1 (NOP_EXPR, TREE_TYPE (inner_lhs), newrhs);
            lhs = inner_lhs;
          }

        return (build_modify_expr (lhs, NOP_EXPR, newrhs));
      }
#else /* not GPC */
      if (TREE_CODE (TREE_TYPE (newrhs)) == ARRAY_TYPE
          || TREE_CODE (TREE_TYPE (newrhs)) == FUNCTION_TYPE)
        newrhs = default_conversion (newrhs);
      {
        tree inner_lhs = TREE_OPERAND (lhs, 0);
        tree result;
        result = build_modify_expr (inner_lhs, NOP_EXPR,
                                    convert (TREE_TYPE (inner_lhs),
                                             convert (lhstype, newrhs)));
        if (TREE_CODE (result) == ERROR_MARK)
          return result;
        pedantic_lvalue_warning (CONVERT_EXPR);
        return convert (TREE_TYPE (lhs), result);
      }
#endif /* not GPC */

    default:
      break;
    }

  /* Now we have handled acceptable kinds of LHS that are not truly lvalues.
     Reject anything strange now.  */

  if (!lvalue_or_else (lhs, "assignment"))
    return error_mark_node;

  /* Warn about storing in something that is `const'.  */

  if (TREE_READONLY (lhs) || TYPE_READONLY (lhstype)
      || ((TREE_CODE (lhstype) == RECORD_TYPE
           || TREE_CODE (lhstype) == UNION_TYPE)
          && C_TYPE_FIELDS_READONLY (lhstype)))
    readonly_warning (lhs, "assignment");

  /* If storing into a structure or union member,
     it has probably been given type `int'.
     Compute the type that would go with
     the actual amount of storage the member occupies.  */

  if (TREE_CODE (lhs) == COMPONENT_REF
      && (TREE_CODE (lhstype) == INTEGER_TYPE
#ifdef GPC
          || TREE_CODE (lhstype) == REAL_TYPE))
#else /* not GPC */
          || TREE_CODE (lhstype) == REAL_TYPE
          || TREE_CODE (lhstype) == ENUMERAL_TYPE))
#endif /* not GPC */
    lhstype = TREE_TYPE (get_unwidened (lhs, 0));

  /* If storing in a field that is in actuality a short or narrower than one,
     we must store in the field in its actual type.  */

  if (lhstype != TREE_TYPE (lhs))
    {
      lhs = copy_node (lhs);
      TREE_TYPE (lhs) = lhstype;
    }

  /* Convert new value to destination type.  */

  newrhs = convert_for_assignment (lhstype, newrhs, "assignment",
                                   NULL_TREE, NULL_TREE, 0);
  if (TREE_CODE (newrhs) == ERROR_MARK)
    return error_mark_node;

  result = build (MODIFY_EXPR, lhstype, lhs, newrhs);
  TREE_SIDE_EFFECTS (result) = 1;

  /* If we got the LHS in a different type for storing in,
     convert the result back to the nominal type of LHS
     so that the value we return always has the same type
     as the LHS argument.  */

  if (olhstype == TREE_TYPE (result))
    return result;
  return convert_for_assignment (olhstype, result, "assignment",
                                 NULL_TREE, NULL_TREE, 0);
}

/* Convert value RHS to type TYPE as preparation for an assignment
   to an lvalue of type TYPE.
   The real work of conversion is done by `convert'.
   The purpose of this function is to generate error messages
   for assignments that are not allowed in C.
   ERRTYPE is a string to use in error messages:
   "assignment", "return", etc.  If it is null, this is parameter passing
   for a function call (and different error messages are output).  Otherwise,
   it may be a name stored in the spelling stack and interpreted by
   get_spelling.

   FUNNAME is the name of the function being called,
   as an IDENTIFIER_NODE, or null.
   PARMNUM is the number of the argument, for printing in error messages.  */

static tree
convert_for_assignment (type, rhs, errtype, fundecl, funname, parmnum)
     tree type, rhs;
     char *errtype;
     tree fundecl, funname;
     int parmnum;
{
  register enum tree_code codel = TREE_CODE (type);
  register tree rhstype;
  register enum tree_code coder;

  /* Strip NON_LVALUE_EXPRs since we aren't using as an lvalue.  */
  /* Do not use STRIP_NOPS here.  We do not want an enumerator
     whose value is 0 to count as a null pointer constant.  */
  if (TREE_CODE (rhs) == NON_LVALUE_EXPR)
    rhs = TREE_OPERAND (rhs, 0);

#ifdef GPC
  /* default_conversion() does not convert array and
   * function types in GPC.
   */
  if (TREE_CODE (TREE_TYPE (rhs)) == FUNCTION_TYPE)
    {
      rhs = build_unary_op (ADDR_EXPR, rhs, 0);
      /* Create a reference rather than a pointer.
       */
      rhs = convert (build_reference_type (TREE_TYPE (TREE_TYPE (rhs))), rhs);
    }
#else /* not GPC */
  if (TREE_CODE (TREE_TYPE (rhs)) == ARRAY_TYPE
      || TREE_CODE (TREE_TYPE (rhs)) == FUNCTION_TYPE)
    rhs = default_conversion (rhs);
#endif /* not GPC */
  else if (optimize && TREE_CODE (rhs) == VAR_DECL)
    rhs = decl_constant_value (rhs);

  rhstype = TREE_TYPE (rhs);
  coder = TREE_CODE (rhstype);

  if (coder == ERROR_MARK)
    return error_mark_node;

#ifdef GPC
  /* File assignments are always illegal.
   */
  if (contains_file_p (type) || contains_file_p (rhstype))
    {
      error ("illegal explicit assignment between files");
      return error_mark_node;
    }
#endif /* GPC */

  if (TYPE_MAIN_VARIANT (type) == TYPE_MAIN_VARIANT (rhstype))
#ifdef GPC
    return (convert_and_check (type, rhs));
#else /* not GPC */
    {
      overflow_warning (rhs);
      /* Check for Objective-C protocols.  This will issue a warning if
         there are protocol violations.  No need to use the return value.  */
      maybe_objc_comptypes (type, rhstype, 0);
      return rhs;
    }
#endif /* not GPC */

  if (coder == VOID_TYPE)
    {
      error ("void value not ignored as it ought to be");
      return error_mark_node;
    }
#ifdef GPC
  /* Check if lhs is a subrange
   *
   * @@ Is it possible that non-integer type subranges should
   * have size conversions?
   * If so, we should construct a limited type from the correct type in
   * the first place, not by having to use INTEGER_TYPE node as a range
   * specifier.
   */
#if 1
  codel = TREE_CODE (base_type (type));
  coder = TREE_CODE (base_type (rhstype));
#else
  if (codel == INTEGER_TYPE && TREE_TYPE (type) &&
      TREE_CODE (TREE_TYPE (type)) != INTEGER_TYPE) /* for size conv. */
    {
      /* yes it is; assignment goes to the real subrange type */
      /* not to the INTEGER_TYPE used as range index specifier */
      codel = TREE_CODE (TREE_TYPE (type));
      type  = TREE_TYPE (type);
    }
#endif

  if (ORDINAL_REAL_OR_COMPLEX_TYPE (codel)
      && compatible_assignment_p (type, rhstype))
#else /* not GPC */
  /* Arithmetic types all interconvert, and enum is treated like int.  */
  if ((codel == INTEGER_TYPE || codel == REAL_TYPE || codel == ENUMERAL_TYPE
       || codel == COMPLEX_TYPE)
      && (coder == INTEGER_TYPE || coder == REAL_TYPE || coder == ENUMERAL_TYPE
          || coder == COMPLEX_TYPE))
#endif /* not GPC */
    return convert_and_check (type, rhs);

  /* Conversion to a transparent union from its member types.
     This applies only to function arguments.  */
  else if (codel == UNION_TYPE && TYPE_TRANSPARENT_UNION (type) && ! errtype)
    {
      tree memb_types;
      tree marginal_memb_type = 0;

      for (memb_types = TYPE_FIELDS (type); memb_types;
           memb_types = TREE_CHAIN (memb_types))
        {
          tree memb_type = TREE_TYPE (memb_types);

          if (comptypes (TYPE_MAIN_VARIANT (memb_type),
                         TYPE_MAIN_VARIANT (rhstype)))
            break;

          if (TREE_CODE (memb_type) != POINTER_TYPE)
            continue;

          if (coder == POINTER_TYPE)
            {
              register tree ttl = TREE_TYPE (memb_type);
              register tree ttr = TREE_TYPE (rhstype);

              /* Any non-function converts to a [const][volatile] void *
                 and vice versa; otherwise, targets must be the same.
                 Meanwhile, the lhs target must have all the qualifiers of
                 the rhs.  */
              if (TYPE_MAIN_VARIANT (ttl) == void_type_node
                  || TYPE_MAIN_VARIANT (ttr) == void_type_node
                  || comp_target_types (memb_type, rhstype))
                {
                  /* If this type won't generate any warnings, use it.  */
                  if ((TREE_CODE (ttr) == FUNCTION_TYPE
                       && TREE_CODE (ttl) == FUNCTION_TYPE)
                      ? ((! TYPE_READONLY (ttl) | TYPE_READONLY (ttr))
                         & (! TYPE_VOLATILE (ttl) | TYPE_VOLATILE (ttr)))
                      : ((TYPE_READONLY (ttl) | ! TYPE_READONLY (ttr))
                         & (TYPE_VOLATILE (ttl) | ! TYPE_VOLATILE (ttr))))
                    break;

                  /* Keep looking for a better type, but remember this one.  */
                  if (! marginal_memb_type)
                    marginal_memb_type = memb_type;
                }
            }

          /* Can convert integer zero to any pointer type.  */
          if (integer_zerop (rhs)
              || (TREE_CODE (rhs) == NOP_EXPR
                  && integer_zerop (TREE_OPERAND (rhs, 0))))
            {
              rhs = null_pointer_node;
              break;
            }
        }

      if (memb_types || marginal_memb_type)
        {
          if (! memb_types)
            {
              /* We have only a marginally acceptable member type;
                 it needs a warning.  */
              register tree ttl = TREE_TYPE (marginal_memb_type);
              register tree ttr = TREE_TYPE (rhstype);

              /* Const and volatile mean something different for function
                 types, so the usual warnings are not appropriate.  */
              if (TREE_CODE (ttr) == FUNCTION_TYPE
                  && TREE_CODE (ttl) == FUNCTION_TYPE)
                {
                  /* Because const and volatile on functions are
                     restrictions that say the function will not do
                     certain things, it is okay to use a const or volatile
                     function where an ordinary one is wanted, but not
                     vice-versa.  */
                  if (TYPE_READONLY (ttl) && ! TYPE_READONLY (ttr))
                    warn_for_assignment ("%s makes `const *' function pointer from non-const",
                                         get_spelling (errtype), funname,
                                         parmnum);
                  if (TYPE_VOLATILE (ttl) && ! TYPE_VOLATILE (ttr))
                    warn_for_assignment ("%s makes `volatile *' function pointer from non-volatile",
                                         get_spelling (errtype), funname,
                                         parmnum);
                }
              else
                {
                  if (! TYPE_READONLY (ttl) && TYPE_READONLY (ttr))
                    warn_for_assignment ("%s discards `const' from pointer target type",
                                         get_spelling (errtype), funname,
                                         parmnum);
                  if (! TYPE_VOLATILE (ttl) && TYPE_VOLATILE (ttr))
                    warn_for_assignment ("%s discards `volatile' from pointer target type",
                                         get_spelling (errtype), funname,
                                         parmnum);
                }
            }

          if (pedantic && ! DECL_IN_SYSTEM_HEADER (fundecl))
            pedwarn ("ANSI C prohibits argument conversion to union type");

          return build1 (NOP_EXPR, type, rhs);
        }
    }

  /* Conversions among pointers */
  else if (codel == POINTER_TYPE && coder == POINTER_TYPE)
    {
      register tree ttl = TREE_TYPE (type);
      register tree ttr = TREE_TYPE (rhstype);

#ifdef GPC
      /* Special case:  Assignment of an address of a string constant
       * to a string schema pointer creates a temporary string schema.
       * This is about the same as what check_pascal_initializer() does.
       */
      if ((PASCAL_TYPE_STRING (ttl)
           || TREE_CODE (ttl) == VOID_TYPE)
          && TREE_CODE (rhs) == ADDR_EXPR
          && TREE_CODE (TREE_OPERAND (rhs, 0)) == STRING_CST)
        {
          tree value = TREE_OPERAND (rhs, 0);
          int string_length = TREE_STRING_LENGTH (value) - 1;
          int string_capacity;
          tree string_type;

          if (PASCAL_TYPE_PREDISCRIMINATED_STRING (ttl)
              || TREE_CODE (ttl) == VOID_TYPE)
            {
              string_capacity = string_length;
              string_type = build_pascal_string_schema (build_int_2 (string_capacity, 0));
            }
          else
            {
              string_type = ttl;
              string_capacity = declared_string_capacity (string_type);
              if (string_length > string_capacity)
                {
                  string_length = string_capacity;
                  if (flag_truncate_strings)
                    pedwarn ("string constant exceeds capacity - truncated");
                  else
                    error ("string constant exceeds capacity");
                }
            }

          value = build_tree_list (NULL_TREE, value);
          value = chainon (build_tree_list (NULL_TREE,
                           build_int_2 (string_length, 0)),
                           value);  /* length */
          value = chainon (build_tree_list (NULL_TREE,
                           build_int_2 (string_capacity, 0)),
                           value);  /* Capacity */
          value = build (CONSTRUCTOR, string_type, NULL_TREE, value);

          /* Make this a valid lvalue for taking addresses.
           */
          TREE_CONSTANT (value) = 1;
          TREE_STATIC (value) = 1;

          rhs = build_unary_op (ADDR_EXPR, value, 0);
          ttl = ttr;
        }

#endif /* GPC */

      /* Any non-function converts to a [const][volatile] void *
         and vice versa; otherwise, targets must be the same.
         Meanwhile, the lhs target must have all the qualifiers of the rhs.  */
      if (TYPE_MAIN_VARIANT (ttl) == void_type_node
          || TYPE_MAIN_VARIANT (ttr) == void_type_node
          || comp_target_types (type, rhstype)
#ifdef GPC
          || comp_object_or_schema_pointer_types (ttl, ttr))
#else /* not GPC */
          || (unsigned_type (TYPE_MAIN_VARIANT (ttl))
              == unsigned_type (TYPE_MAIN_VARIANT (ttr))))
#endif /* not GPC */
        {
#ifndef GPC
          if (pedantic
              && ((TYPE_MAIN_VARIANT (ttl) == void_type_node
                   && TREE_CODE (ttr) == FUNCTION_TYPE)
                  ||
                  (TYPE_MAIN_VARIANT (ttr) == void_type_node
                   /* Check TREE_CODE to catch cases like (void *) (char *) 0
                      which are not ANSI null ptr constants.  */
                   && (!integer_zerop (rhs) || TREE_CODE (rhs) == NOP_EXPR)
                   && TREE_CODE (ttl) == FUNCTION_TYPE)))
            warn_for_assignment ("ANSI forbids %s between function pointer and untyped pointer",
                                 get_spelling (errtype), funname, parmnum);
          else
#endif /* not GPC */
          /* Const and volatile mean something different for function types,
             so the usual warnings are not appropriate.  */
          if (TREE_CODE (ttr) != FUNCTION_TYPE
                   && TREE_CODE (ttl) != FUNCTION_TYPE)
            {
              if (! TYPE_READONLY (ttl) && TYPE_READONLY (ttr))
                warn_for_assignment ("%s discards `const' from pointer target type",
                                     get_spelling (errtype), funname, parmnum);
              else if (! TYPE_VOLATILE (ttl) && TYPE_VOLATILE (ttr))
                warn_for_assignment ("%s discards `volatile' from pointer target type",
                                     get_spelling (errtype), funname, parmnum);
#ifdef GPC
              else if (TREE_CODE (ttl) == INTEGER_TYPE
                       && TREE_CODE (ttr) == INTEGER_TYPE)
                {
                  if (TREE_UNSIGNED (ttl) != TREE_UNSIGNED (ttr))
                    {
                      if (TYPE_PRECISION (ttl) != TYPE_PRECISION (ttr))
                        error_assignment ("pointer targets in %s differ in size and signedness",
                                          get_spelling (errtype), funname, parmnum);
                      else
                        error_assignment ("pointer targets in %s differ in signedness",
                                          get_spelling (errtype), funname, parmnum);
                    }
                  else if (TYPE_PRECISION (ttl) != TYPE_PRECISION (ttr))
                    error_assignment ("pointer targets in %s differ in size",
                                      get_spelling (errtype), funname, parmnum);
                }
#else /* not GPC */
              /* If this is not a case of ignoring a mismatch in signedness,
                 no warning.  */
              else if (TYPE_MAIN_VARIANT (ttl) == void_type_node
                       || TYPE_MAIN_VARIANT (ttr) == void_type_node
                       || comp_target_types (type, rhstype))
                ;
              /* If there is a mismatch, do warn.  */
              else if (pedantic)
                warn_for_assignment ("pointer targets in %s differ in signedness",
                                     get_spelling (errtype), funname, parmnum);
#endif /* not GPC */
            }
          else if (TREE_CODE (ttl) == FUNCTION_TYPE
                   && TREE_CODE (ttr) == FUNCTION_TYPE)
            {
              /* Because const and volatile on functions are restrictions
                 that say the function will not do certain things,
                 it is okay to use a const or volatile function
                 where an ordinary one is wanted, but not vice-versa.  */
              if (TYPE_READONLY (ttl) && ! TYPE_READONLY (ttr))
                warn_for_assignment ("%s makes `const *' function pointer from non-const",
                                     get_spelling (errtype), funname, parmnum);
              if (TYPE_VOLATILE (ttl) && ! TYPE_VOLATILE (ttr))
                warn_for_assignment ("%s makes `volatile *' function pointer from non-volatile",
                                     get_spelling (errtype), funname, parmnum);
            }
        }
      else
#ifdef GPC
        error_assignment ("%s from incompatible pointer type",
                          get_spelling (errtype), funname, parmnum);
#else /* not GPC */
        warn_for_assignment ("%s from incompatible pointer type",
                             get_spelling (errtype), funname, parmnum);
#endif /* not GPC */
      return convert (type, rhs);
    }
#ifndef GPC
  else if (codel == POINTER_TYPE && coder == INTEGER_TYPE)
    {
      /* An explicit constant 0 can convert to a pointer,
         or one that results from arithmetic, even including
         a cast to integer type.  */
      if (! (TREE_CODE (rhs) == INTEGER_CST && integer_zerop (rhs))
          &&
          ! (TREE_CODE (rhs) == NOP_EXPR
             && TREE_CODE (TREE_TYPE (rhs)) == INTEGER_TYPE
             && TREE_CODE (TREE_OPERAND (rhs, 0)) == INTEGER_CST
             && integer_zerop (TREE_OPERAND (rhs, 0))))
        {
          warn_for_assignment ("%s makes pointer from integer without a cast",
                               get_spelling (errtype), funname, parmnum);
          return convert (type, rhs);
        }
      return null_pointer_node;
    }
  else if (codel == INTEGER_TYPE && coder == POINTER_TYPE)
    {
      warn_for_assignment ("%s makes integer from pointer without a cast",
                           get_spelling (errtype), funname, parmnum);
      return convert (type, rhs);
    }
#else /* GPC */
  /* Array assignments.
   */
  else if (codel == ARRAY_TYPE && coder == ARRAY_TYPE)
    {
      if (comptypes (type, rhstype) == 0)
        {
          if (errtype)
            error ("type mismatch in array %s",
                   get_spelling (errtype));
          else if (funname)
            error ("passing arg %d of `%s' from incompatible array",
                   parmnum, IDENTIFIER_POINTER (funname));
          else
            error ("passing arg %d from incompatible array",
                   parmnum);
          return error_mark_node;
        }
      /* @@@@ the array size must be checked also!!!!!! */
      return rhs;       /* No conversion */
    }
  /* Object assignments are always illegal.
   */
  else if (PASCAL_TYPE_OBJECT (type) || PASCAL_TYPE_OBJECT (rhstype))
    {
      error ("illegal explicit assignment between objects");
      return error_mark_node;
    }
  /* Record assignments.
   */
  else if (codel == RECORD_TYPE && coder == RECORD_TYPE)
    {
      if (comptypes (type, rhstype) == 0)
        {
          if (errtype)
            error ("type mismatch in record %s",
                   get_spelling (errtype));
          else if (funname)
            error ("passing arg %d of `%s' from incompatible record or schema",
                   parmnum, IDENTIFIER_POINTER (funname));
          else
            error ("passing arg %d from incompatible record or schema",
                   parmnum);
          return error_mark_node;
        }
      else
        return rhs;
    }
  /* Set assignments.
   */
  else if (codel == SET_TYPE && coder == SET_TYPE)
    {
      tree lhs_basetype = base_type (TREE_TYPE (type));
      tree rhs_basetype = base_type (TREE_TYPE (rhstype));
      tree lhs_domain = TYPE_DOMAIN (type);
      tree rhs_domain = TYPE_DOMAIN (rhstype);

      /* Allow empty set operations & ops when set base types match */
      if (TREE_CODE (rhs_basetype) != TREE_CODE (lhs_basetype)
          && TREE_CODE (rhs_basetype) != VOID_TYPE)
        {
          error ("set %s from different type of set", errtype);
          return error_mark_node;
        }

      if (TYPE_MIN_VALUE (lhs_domain) == TYPE_MIN_VALUE (rhs_domain)
          && TYPE_MAX_VALUE (lhs_domain) == TYPE_MAX_VALUE (rhs_domain))
        return rhs;     /* No conversions */
      else
        return convert (type, rhs);
    }
  /* Complex assignments.
   */
  else if (codel == TREE_CODE (TREE_TYPE (complex_type_node)) &&
           coder == COMPLEX_TYPE)
    {
       if (TREE_CODE (rhs) == REALPART_EXPR ||
           TREE_CODE (rhs) == IMAGPART_EXPR)    /*@@@@ Take care of ARG!!! */
           return rhs;
    }
  /* Assignments of procedural variables.
   */
  else if (codel == REFERENCE_TYPE
           && TREE_CODE (TREE_TYPE (type)) == FUNCTION_TYPE)
    {
      if (coder == REFERENCE_TYPE
          && TREE_CODE (TREE_TYPE (rhstype)) == FUNCTION_TYPE)
        return convert (type, rhs);
      if (coder == POINTER_TYPE
          && integer_zerop (rhs))
        return convert (type, rhs);
    }
#endif /* GPC */
  if (!errtype)
    {
      if (funname)
        {
          tree selector = maybe_building_objc_message_expr ();

          if (selector && parmnum > 2)
            error ("incompatible type for argument %d of `%s'",
                   parmnum - 2, IDENTIFIER_POINTER (selector));
          else
            error ("incompatible type for argument %d of `%s'",
                   parmnum, IDENTIFIER_POINTER (funname));
        }
      else
        error ("incompatible type for argument %d of indirect function call",
               parmnum);
    }
  else
    error ("incompatible types in %s", get_spelling (errtype));
  return error_mark_node;
}

/* Print a warning using MSG.
   It gets OPNAME as its one parameter.
   If OPNAME is null, it is replaced by "passing arg ARGNUM of `FUNCTION'".
   FUNCTION and ARGNUM are handled specially if we are building an
   Objective-C selector.  */

static void
warn_for_assignment (msg, opname, function, argnum)
     char *msg;
     char *opname;
     tree function;
     int argnum;
{
  static char argstring[] = "passing arg %d of `%s'";
  static char argnofun[] =  "passing arg %d";

  if (opname == 0)
    {
      tree selector = maybe_building_objc_message_expr ();

      if (selector && argnum > 2)
        {
          function = selector;
          argnum -= 2;
        }
      if (function)
        {
          /* Function name is known; supply it.  */
          opname = (char *) alloca (IDENTIFIER_LENGTH (function)
                                    + sizeof (argstring) + 25 /*%d*/ + 1);
          sprintf (opname, argstring, argnum, IDENTIFIER_POINTER (function));
        }
      else
        {
          /* Function name unknown (call through ptr); just give arg number.  */
          opname = (char *) alloca (sizeof (argnofun) + 25 /*%d*/ + 1);
          sprintf (opname, argnofun, argnum);
        }
    }
  pedwarn (msg, opname);
}

#ifdef GPC
/* Same as warn_for_assignment(), but as an error message.
 */
static void
error_assignment (msg, opname, function, argnum)
     char *msg;
     char *opname;
     tree function;
     int argnum;
{
  static char argstring[] = "passing arg %d of `%s'";
  static char argnofun[] = "passing arg %d";

  if (opname == 0)
    {
      tree selector = maybe_building_objc_message_expr ();

      if (selector && argnum > 2)
        {
          function = selector;
          argnum -= 2;
        }
      if (function)
        {
          /* Function name is known; supply it.  */
          opname = (char *) alloca (IDENTIFIER_LENGTH (function)
                                    + sizeof (argstring) + 25 /*%d*/ + 1);
          sprintf (opname, argstring, argnum, IDENTIFIER_POINTER (function));
        }
      else
        {
          /* Function name unknown (call through ptr); just give arg number.  */
          opname = (char *) alloca (sizeof (argnofun) + 25 /*%d*/ + 1);
          sprintf (opname, argnofun, argnum);
        }
    }
  error (msg, opname);
}
#endif /* GPC */

/* Return nonzero if VALUE is a valid constant-valued expression
   for use in initializing a static variable; one that can be an
   element of a "constant" initializer.

   Return null_pointer_node if the value is absolute;
   if it is relocatable, return the variable that determines the relocation.
   We assume that VALUE has been folded as much as possible;
   therefore, we do not need to check for such things as
   arithmetic-combinations of integers.  */

tree
initializer_constant_valid_p (value, endtype)
     tree value;
     tree endtype;
{
  switch (TREE_CODE (value))
    {
    case CONSTRUCTOR:
      if ((TREE_CODE (TREE_TYPE (value)) == UNION_TYPE
           || TREE_CODE (TREE_TYPE (value)) == RECORD_TYPE)
          && TREE_CONSTANT (value)
          && CONSTRUCTOR_ELTS (value))
        return
          initializer_constant_valid_p (TREE_VALUE (CONSTRUCTOR_ELTS (value)),
                                        endtype);

      return TREE_STATIC (value) ? null_pointer_node : 0;

    case INTEGER_CST:
    case REAL_CST:
    case STRING_CST:
    case COMPLEX_CST:
      return null_pointer_node;

    case ADDR_EXPR:
      return TREE_OPERAND (value, 0);

    case NON_LVALUE_EXPR:
      return initializer_constant_valid_p (TREE_OPERAND (value, 0), endtype);

    case CONVERT_EXPR:
    case NOP_EXPR:
#ifdef GPC
      /* Allow conversions between reference and pointer types.
       */
      if ((TREE_CODE (TREE_TYPE (value)) == POINTER_TYPE
           || TREE_CODE (TREE_TYPE (value)) == REFERENCE_TYPE)
          && (TREE_CODE (TREE_TYPE (TREE_OPERAND (value, 0))) == POINTER_TYPE
              || TREE_CODE (TREE_TYPE (TREE_OPERAND (value, 0))) == REFERENCE_TYPE))
        return initializer_constant_valid_p (TREE_OPERAND (value, 0), endtype);
#else /* not GPC */
      /* Allow conversions between pointer types.  */
      if (TREE_CODE (TREE_TYPE (value)) == POINTER_TYPE
          && TREE_CODE (TREE_TYPE (TREE_OPERAND (value, 0))) == POINTER_TYPE)
        return initializer_constant_valid_p (TREE_OPERAND (value, 0), endtype);
#endif /* not GPC */

      /* Allow conversions between real types.  */
      if (TREE_CODE (TREE_TYPE (value)) == REAL_TYPE
          && TREE_CODE (TREE_TYPE (TREE_OPERAND (value, 0))) == REAL_TYPE)
        return initializer_constant_valid_p (TREE_OPERAND (value, 0), endtype);

      /* Allow length-preserving conversions between integer types.  */
      if (TREE_CODE (TREE_TYPE (value)) == INTEGER_TYPE
          && TREE_CODE (TREE_TYPE (TREE_OPERAND (value, 0))) == INTEGER_TYPE
          && (TYPE_PRECISION (TREE_TYPE (value))
              == TYPE_PRECISION (TREE_TYPE (TREE_OPERAND (value, 0)))))
        return initializer_constant_valid_p (TREE_OPERAND (value, 0), endtype);

      /* Allow conversions between other integer types only if
         explicit value.  */
      if (TREE_CODE (TREE_TYPE (value)) == INTEGER_TYPE
          && TREE_CODE (TREE_TYPE (TREE_OPERAND (value, 0))) == INTEGER_TYPE)
        {
          tree inner = initializer_constant_valid_p (TREE_OPERAND (value, 0),
                                                     endtype);
          if (inner == null_pointer_node)
            return null_pointer_node;
          return 0;
        }

      /* Allow (int) &foo provided int is as wide as a pointer.  */
      if (TREE_CODE (TREE_TYPE (value)) == INTEGER_TYPE
          && TREE_CODE (TREE_TYPE (TREE_OPERAND (value, 0))) == POINTER_TYPE
          && (TYPE_PRECISION (TREE_TYPE (value))
              >= TYPE_PRECISION (TREE_TYPE (TREE_OPERAND (value, 0)))))
        return initializer_constant_valid_p (TREE_OPERAND (value, 0),
                                             endtype);

      /* Likewise conversions from int to pointers.  */
      if (TREE_CODE (TREE_TYPE (value)) == POINTER_TYPE
          && TREE_CODE (TREE_TYPE (TREE_OPERAND (value, 0))) == INTEGER_TYPE
          && (TYPE_PRECISION (TREE_TYPE (value))
              <= TYPE_PRECISION (TREE_TYPE (TREE_OPERAND (value, 0)))))
        return initializer_constant_valid_p (TREE_OPERAND (value, 0),
                                             endtype);

      /* Allow conversions to union types if the value inside is okay.  */
      if (TREE_CODE (TREE_TYPE (value)) == UNION_TYPE)
        return initializer_constant_valid_p (TREE_OPERAND (value, 0),
                                             endtype);
      return 0;

    case PLUS_EXPR:
      if (TREE_CODE (endtype) == INTEGER_TYPE
          && TYPE_PRECISION (endtype) < POINTER_SIZE)
        return 0;
      {
        tree valid0 = initializer_constant_valid_p (TREE_OPERAND (value, 0),
                                                    endtype);
        tree valid1 = initializer_constant_valid_p (TREE_OPERAND (value, 1),
                                                    endtype);
        /* If either term is absolute, use the other terms relocation.  */
        if (valid0 == null_pointer_node)
          return valid1;
        if (valid1 == null_pointer_node)
          return valid0;
        return 0;
      }

    case MINUS_EXPR:
      if (TREE_CODE (endtype) == INTEGER_TYPE
          && TYPE_PRECISION (endtype) < POINTER_SIZE)
        return 0;
      {
        tree valid0 = initializer_constant_valid_p (TREE_OPERAND (value, 0),
                                                    endtype);
        tree valid1 = initializer_constant_valid_p (TREE_OPERAND (value, 1),
                                                    endtype);
        /* Win if second argument is absolute.  */
        if (valid1 == null_pointer_node)
          return valid0;
        /* Win if both arguments have the same relocation.
           Then the value is absolute.  */
        if (valid0 == valid1)
          return null_pointer_node;
        return 0;
      }

    default:
      return 0;
    }
}

/* If VALUE is a compound expr all of whose expressions are constant, then
   return its value.  Otherwise, return error_mark_node.

   This is for handling COMPOUND_EXPRs as initializer elements
   which is allowed with a warning when -pedantic is specified.  */

static tree
valid_compound_expr_initializer (value, endtype)
     tree value;
     tree endtype;
{
  if (TREE_CODE (value) == COMPOUND_EXPR)
    {
      if (valid_compound_expr_initializer (TREE_OPERAND (value, 0), endtype)
          == error_mark_node)
        return error_mark_node;
      return valid_compound_expr_initializer (TREE_OPERAND (value, 1),
                                              endtype);
    }
  else if (! TREE_CONSTANT (value)
           && ! initializer_constant_valid_p (value, endtype))
    return error_mark_node;
  else
    return value;
}

/* Perform appropriate conversions on the initial value of a variable,
   store it in the declaration DECL,
   and print any error messages that are appropriate.
   If the init is invalid, store an ERROR_MARK.  */

void
store_init_value (decl, init)
     tree decl, init;
{
  register tree value, type;

  /* If variable's type was invalidly declared, just ignore it.  */

  type = TREE_TYPE (decl);
  if (TREE_CODE (type) == ERROR_MARK)
    return;

  /* Digest the specified initializer into an expression.  */

  value = digest_init (type, init, TREE_STATIC (decl),
                       TREE_STATIC (decl) || pedantic);

  /* Store the expression if valid; else report error.  */

#if 0
  /* Note that this is the only place we can detect the error
     in a case such as   struct foo bar = (struct foo) { x, y };
     where there is one initial value which is a constructor expression.  */
  if (value == error_mark_node)
    ;
  else if (TREE_STATIC (decl) && ! TREE_CONSTANT (value))
    {
      error ("initializer for static variable is not constant");
      value = error_mark_node;
    }
  else if (TREE_STATIC (decl)
           && initializer_constant_valid_p (value, TREE_TYPE (value)) == 0)
    {
      error ("initializer for static variable uses complicated arithmetic");
      value = error_mark_node;
    }
  else
    {
      if (pedantic && TREE_CODE (value) == CONSTRUCTOR)
        {
          if (! TREE_CONSTANT (value))
            pedwarn ("aggregate initializer is not constant");
          else if (! TREE_STATIC (value))
            pedwarn ("aggregate initializer uses complicated arithmetic");
        }
    }
#endif

  DECL_INITIAL (decl) = value;

  /* ANSI wants warnings about out-of-range constant initializers.  */
  STRIP_TYPE_NOPS (value);
  constant_expression_warning (value);
}

/* Methods for storing and printing names for error messages.  */

/* Implement a spelling stack that allows components of a name to be pushed
   and popped.  Each element on the stack is this structure.  */

struct spelling
{
  int kind;
  union
    {
      int i;
      char *s;
    } u;
};

#define SPELLING_STRING 1
#define SPELLING_MEMBER 2
#define SPELLING_BOUNDS 3

static struct spelling *spelling;       /* Next stack element (unused).  */
static struct spelling *spelling_base;  /* Spelling stack base.  */
static int spelling_size;               /* Size of the spelling stack.  */

/* Macros to save and restore the spelling stack around push_... functions.
   Alternative to SAVE_SPELLING_STACK.  */

#define SPELLING_DEPTH() (spelling - spelling_base)
#define RESTORE_SPELLING_DEPTH(depth) (spelling = spelling_base + depth)

/* Save and restore the spelling stack around arbitrary C code.  */

#define SAVE_SPELLING_DEPTH(code)               \
{                                               \
  int __depth = SPELLING_DEPTH ();              \
  code;                                         \
  RESTORE_SPELLING_DEPTH (__depth);             \
}

/* Push an element on the spelling stack with type KIND and assign VALUE
   to MEMBER.  */

#define PUSH_SPELLING(KIND, VALUE, MEMBER)                              \
{                                                                       \
  int depth = SPELLING_DEPTH ();                                        \
                                                                        \
  if (depth >= spelling_size)                                           \
    {                                                                   \
      spelling_size += 10;                                              \
      if (spelling_base == 0)                                           \
        spelling_base                                                   \
          = (struct spelling *) xmalloc (spelling_size * sizeof (struct spelling));     \
      else                                                              \
        spelling_base                                                   \
          = (struct spelling *) xrealloc (spelling_base,                \
                                          spelling_size * sizeof (struct spelling));    \
      RESTORE_SPELLING_DEPTH (depth);                                   \
    }                                                                   \
                                                                        \
  spelling->kind = (KIND);                                              \
  spelling->MEMBER = (VALUE);                                           \
  spelling++;                                                           \
}

/* Push STRING on the stack.  Printed literally.  */

static void
push_string (string)
     char *string;
{
  PUSH_SPELLING (SPELLING_STRING, string, u.s);
}

/* Push a member name on the stack.  Printed as '.' STRING.  */

static void
push_member_name (decl)
     tree decl;

{
  char *string
    = DECL_NAME (decl) ? IDENTIFIER_POINTER (DECL_NAME (decl)) : "<anonymous>";
  PUSH_SPELLING (SPELLING_MEMBER, string, u.s);
}

/* Push an array bounds on the stack.  Printed as [BOUNDS].  */

static void
push_array_bounds (bounds)
     int bounds;
{
  PUSH_SPELLING (SPELLING_BOUNDS, bounds, u.i);
}

/* Compute the maximum size in bytes of the printed spelling.  */

static int
spelling_length ()
{
  register int size = 0;
  register struct spelling *p;

  for (p = spelling_base; p < spelling; p++)
    {
      if (p->kind == SPELLING_BOUNDS)
        size += 25;
      else
        size += strlen (p->u.s) + 1;
    }

  return size;
}

/* Print the spelling to BUFFER and return it.  */

static char *
print_spelling (buffer)
     register char *buffer;
{
  register char *d = buffer;
  register char *s;
  register struct spelling *p;

  for (p = spelling_base; p < spelling; p++)
    if (p->kind == SPELLING_BOUNDS)
      {
        sprintf (d, "[%d]", p->u.i);
        d += strlen (d);
      }
    else
      {
        if (p->kind == SPELLING_MEMBER)
          *d++ = '.';
        for (s = p->u.s; (*d = *s++); d++)
          ;
      }
  *d++ = '\0';
  return buffer;
}

/* Provide a means to pass component names derived from the spelling stack.  */

char initialization_message;

/* Interpret the spelling of the given ERRTYPE message.  */

static char *
get_spelling (errtype)
     char *errtype;
{
  static char *buffer;
  static int size = -1;

  if (errtype == &initialization_message)
    {
      /* Avoid counting chars */
      static char message[] = "initialization of `%s'";
      register int needed = sizeof (message) + spelling_length () + 1;
      char *temp;

      if (size < 0)
        buffer = (char *) xmalloc (size = needed);
      if (needed > size)
        buffer = (char *) xrealloc (buffer, size = needed);

      temp = (char *) alloca (needed);
      sprintf (buffer, message, print_spelling (temp));
      return buffer;
    }

  return errtype;
}

/* Issue an error message for a bad initializer component.
   FORMAT describes the message.  OFWHAT is the name for the component.
   LOCAL is a format string for formatting the insertion of the name
   into the message.

   If OFWHAT is null, the component name is stored on the spelling stack.
   If the component name is a null string, then LOCAL is omitted entirely.  */

#ifdef EGCS92
void
gpc_error_init (format, local, ofwhat)
     const char *format, *local, *ofwhat;
#else
void
error_init (format, local, ofwhat)
     char *format, *local, *ofwhat;
#endif
{
  char *buffer;

  if (ofwhat == 0)
    ofwhat = print_spelling ((char *) alloca (spelling_length () + 1));
  buffer = (char *) alloca (strlen (local) + strlen (ofwhat) + 2);

  if (*ofwhat)
    sprintf (buffer, local, ofwhat);
  else
    buffer[0] = 0;

  error (format, buffer);
}

/* Issue a pedantic warning for a bad initializer component.
   FORMAT describes the message.  OFWHAT is the name for the component.
   LOCAL is a format string for formatting the insertion of the name
   into the message.

   If OFWHAT is null, the component name is stored on the spelling stack.
   If the component name is a null string, then LOCAL is omitted entirely.  */

#ifdef EGCS92
void
gpc_pedwarn_init (format, local, ofwhat)
     const char *format, *local, *ofwhat;
#else
void
pedwarn_init (format, local, ofwhat)
     char *format, *local, *ofwhat;
#endif
{
  char *buffer;

  if (ofwhat == 0)
    ofwhat = print_spelling ((char *) alloca (spelling_length () + 1));
  buffer = (char *) alloca (strlen (local) + strlen (ofwhat) + 2);

  if (*ofwhat)
    sprintf (buffer, local, ofwhat);
  else
    buffer[0] = 0;

  pedwarn (format, buffer);
}

/* Issue a warning for a bad initializer component.
   FORMAT describes the message.  OFWHAT is the name for the component.
   LOCAL is a format string for formatting the insertion of the name
   into the message.

   If OFWHAT is null, the component name is stored on the spelling stack.
   If the component name is a null string, then LOCAL is omitted entirely.  */

static void
warning_init (format, local, ofwhat)
     char *format, *local, *ofwhat;
{
  char *buffer;

  if (ofwhat == 0)
    ofwhat = print_spelling ((char *) alloca (spelling_length () + 1));
  buffer = (char *) alloca (strlen (local) + strlen (ofwhat) + 2);

  if (*ofwhat)
    sprintf (buffer, local, ofwhat);
  else
    buffer[0] = 0;

  warning (format, buffer);
}

/* Digest the parser output INIT as an initializer for type TYPE.
   Return a C expression of type TYPE to represent the initial value.

   The arguments REQUIRE_CONSTANT and CONSTRUCTOR_CONSTANT request errors
   if non-constant initializers or elements are seen.  CONSTRUCTOR_CONSTANT
   applies only to elements of constructors.  */

static tree
digest_init (type, init, require_constant, constructor_constant)
     tree type, init;
     int require_constant, constructor_constant;
{
  enum tree_code code = TREE_CODE (type);
  tree inside_init = init;

  if (init == error_mark_node)
    return init;

  /* Strip NON_LVALUE_EXPRs since we aren't using as an lvalue.  */
  /* Do not use STRIP_NOPS here.  We do not want an enumerator
     whose value is 0 to count as a null pointer constant.  */
  if (TREE_CODE (init) == NON_LVALUE_EXPR)
    inside_init = TREE_OPERAND (init, 0);

  /* Initialization of an array of chars from a string constant
     optionally enclosed in braces.  */

  if (code == ARRAY_TYPE)
    {
      tree typ1 = TYPE_MAIN_VARIANT (TREE_TYPE (type));
      if ((typ1 == char_type_node
           || typ1 == signed_char_type_node
           || typ1 == unsigned_char_type_node
           || typ1 == unsigned_wchar_type_node
           || typ1 == signed_wchar_type_node)
          && ((inside_init && TREE_CODE (inside_init) == STRING_CST)))
        {
          if (comptypes (TYPE_MAIN_VARIANT (TREE_TYPE (inside_init)),
                         TYPE_MAIN_VARIANT (type)))
            return inside_init;

          if ((TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (inside_init)))
               != char_type_node)
              && TYPE_PRECISION (typ1) == TYPE_PRECISION (char_type_node))
            {
              gpc_error_init ("char-array%s initialized from wide string",
                          " `%s'", NULL);
              return error_mark_node;
            }
          if ((TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (inside_init)))
               == char_type_node)
              && TYPE_PRECISION (typ1) != TYPE_PRECISION (char_type_node))
            {
              gpc_error_init ("int-array%s initialized from non-wide string",
                          " `%s'", NULL);
              return error_mark_node;
            }

          TREE_TYPE (inside_init) = type;
          if (TYPE_DOMAIN (type) != 0
              && TREE_CODE (TYPE_SIZE (type)) == INTEGER_CST)
            {
              register int size = TREE_INT_CST_LOW (TYPE_SIZE (type));
              size = (size + BITS_PER_UNIT - 1) / BITS_PER_UNIT;
              /* Subtract 1 (or sizeof (wchar_t))
                 because it's ok to ignore the terminating null char
                 that is counted in the length of the constant.  */
              if (size < TREE_STRING_LENGTH (inside_init)
                  - (TYPE_PRECISION (typ1) != TYPE_PRECISION (char_type_node)
                     ? TYPE_PRECISION (wchar_type_node) / BITS_PER_UNIT
                     : 1))
                gpc_pedwarn_init (
                  "initializer-string for array of chars%s is too long",
                  " `%s'", NULL);
            }
          return inside_init;
        }
    }

  /* Any type can be initialized
     from an expression of the same type, optionally with braces.  */

  if (inside_init && TREE_TYPE (inside_init) != 0
      && (comptypes (TYPE_MAIN_VARIANT (TREE_TYPE (inside_init)),
                     TYPE_MAIN_VARIANT (type))
          || (code == ARRAY_TYPE
              && comptypes (TREE_TYPE (inside_init), type))
#ifdef GPC
          || (code == SET_TYPE
              && comptypes (TREE_TYPE (inside_init), type))
#endif
          || (code == POINTER_TYPE
              && (TREE_CODE (TREE_TYPE (inside_init)) == ARRAY_TYPE
                  || TREE_CODE (TREE_TYPE (inside_init)) == FUNCTION_TYPE)
              && comptypes (TREE_TYPE (TREE_TYPE (inside_init)),
                            TREE_TYPE (type)))))
    {
      if (code == POINTER_TYPE
          && (TREE_CODE (TREE_TYPE (inside_init)) == ARRAY_TYPE
              || TREE_CODE (TREE_TYPE (inside_init)) == FUNCTION_TYPE))
        inside_init = default_conversion (inside_init);
#ifndef GPC
      else if (code == ARRAY_TYPE && TREE_CODE (inside_init) != STRING_CST
               && TREE_CODE (inside_init) != CONSTRUCTOR)
        {
          gpc_error_init ("array%s initialized from non-constant array expression",
                      " `%s'", NULL);
          return error_mark_node;
        }
#endif /* not GPC */

      if (optimize && TREE_CODE (inside_init) == VAR_DECL)
        inside_init = decl_constant_value (inside_init);

      /* Compound expressions can only occur here if -pedantic or
         -pedantic-errors is specified.  In the later case, we always want
         an error.  In the former case, we simply want a warning.  */
      if (require_constant && pedantic
          && TREE_CODE (inside_init) == COMPOUND_EXPR)
        {
          inside_init
            = valid_compound_expr_initializer (inside_init,
                                               TREE_TYPE (inside_init));
          if (inside_init == error_mark_node)
            gpc_error_init ("initializer element%s is not constant",
                        " for `%s'", NULL);
          else
            gpc_pedwarn_init ("initializer element%s is not constant",
                          " for `%s'", NULL);
          if (flag_pedantic_errors)
            inside_init = error_mark_node;
        }
      else if (require_constant && ! TREE_CONSTANT (inside_init))
        {
          gpc_error_init ("initializer element%s is not constant",
                      " for `%s'", NULL);
          inside_init = error_mark_node;
        }
      else if (require_constant
               && initializer_constant_valid_p (inside_init, TREE_TYPE (inside_init)) == 0)
        {
          gpc_error_init ("initializer element%s is not computable at load time",
                      " for `%s'", NULL);
          inside_init = error_mark_node;
        }

      return inside_init;
    }

  /* Handle scalar types, including conversions.  */

  if (code == INTEGER_TYPE || code == REAL_TYPE || code == POINTER_TYPE
#ifdef GPC
      || code == CHAR_TYPE || code == BOOLEAN_TYPE
#endif
      || code == ENUMERAL_TYPE || code == COMPLEX_TYPE)
    {
      /* Note that convert_for_assignment calls default_conversion
         for arrays and functions.  We must not call it in the
         case where inside_init is a null pointer constant.  */
      inside_init
        = convert_for_assignment (type, init, "initialization",
                                  NULL_TREE, NULL_TREE, 0);

      if (require_constant && ! TREE_CONSTANT (inside_init))
        {
          gpc_error_init ("initializer element%s is not constant",
                      " for `%s'", NULL);
          inside_init = error_mark_node;
        }
      else if (require_constant
               && initializer_constant_valid_p (inside_init, TREE_TYPE (inside_init)) == 0)
        {
          gpc_error_init ("initializer element%s is not computable at load time",
                      " for `%s'", NULL);
          inside_init = error_mark_node;
        }

      return inside_init;
    }

  /* Come here only for records and arrays.  */

  if (TYPE_SIZE (type) && TREE_CODE (TYPE_SIZE (type)) != INTEGER_CST)
    {
      gpc_error_init ("variable-sized object%s may not be initialized",
                  " `%s'", NULL);
      return error_mark_node;
    }

  /* Traditionally, you can write  struct foo x = 0;
     and it initializes the first element of x to 0.  */
  if (flag_traditional)
    {
      tree top = 0, prev = 0, otype = type;
      while (TREE_CODE (type) == RECORD_TYPE
             || TREE_CODE (type) == ARRAY_TYPE
             || TREE_CODE (type) == QUAL_UNION_TYPE
             || TREE_CODE (type) == UNION_TYPE)
        {
          tree temp = build (CONSTRUCTOR, type, NULL_TREE, NULL_TREE);
          if (prev == 0)
            top = temp;
          else
            TREE_OPERAND (prev, 1) = build_tree_list (NULL_TREE, temp);
          prev = temp;
          if (TREE_CODE (type) == ARRAY_TYPE)
            type = TREE_TYPE (type);
          else if (TYPE_FIELDS (type))
            type = TREE_TYPE (TYPE_FIELDS (type));
          else
            {
              gpc_error_init ("invalid initializer%s", " for `%s'", NULL);
              return error_mark_node;
            }
        }

      if (otype != type)
        {
          TREE_OPERAND (prev, 1)
            = build_tree_list (NULL_TREE,
                               digest_init (type, init, require_constant,
                                            constructor_constant));
          return top;
        }
      else
        return error_mark_node;
    }
  gpc_error_init ("invalid initializer%s", " for `%s'", NULL);
  return error_mark_node;
}

/* Handle initializers that use braces.  */

/* Type of object we are accumulating a constructor for.
   This type is always a RECORD_TYPE, UNION_TYPE or ARRAY_TYPE.  */
static tree constructor_type;

/* For a RECORD_TYPE or UNION_TYPE, this is the chain of fields
   left to fill.  */
static tree constructor_fields;

/* For an ARRAY_TYPE, this is the specified index
   at which to store the next element we get.
   This is a special INTEGER_CST node that we modify in place.  */
static tree constructor_index;

/* For an ARRAY_TYPE, this is the end index of the range
   to initialize with the next element, or NULL in the ordinary case
   where the element is used just once.  */
static tree constructor_range_end;

/* For an ARRAY_TYPE, this is the maximum index.  */
static tree constructor_max_index;

/* For a RECORD_TYPE, this is the first field not yet written out.  */
static tree constructor_unfilled_fields;

/* For an ARRAY_TYPE, this is the index of the first element
   not yet written out.
   This is a special INTEGER_CST node that we modify in place.  */
static tree constructor_unfilled_index;

/* In a RECORD_TYPE, the byte index of the next consecutive field.
   This is so we can generate gaps between fields, when appropriate.
   This is a special INTEGER_CST node that we modify in place.  */
static tree constructor_bit_index;

/* If we are saving up the elements rather than allocating them,
   this is the list of elements so far (in reverse order,
   most recent first).  */
static tree constructor_elements;

/* 1 if so far this constructor's elements are all compile-time constants.  */
static int constructor_constant;

/* 1 if so far this constructor's elements are all valid address constants.  */
static int constructor_simple;

/* 1 if this constructor is erroneous so far.  */
static int constructor_erroneous;

/* 1 if have called defer_addressed_constants.  */
static int constructor_subconstants_deferred;

/* List of pending elements at this constructor level.
   These are elements encountered out of order
   which belong at places we haven't reached yet in actually
   writing the output.  */
static tree constructor_pending_elts;

/* The SPELLING_DEPTH of this constructor.  */
static int constructor_depth;

/* 0 if implicitly pushing constructor levels is allowed.  */
int constructor_no_implicit = 0; /* 0 for C; 1 for some other languages.  */

static int require_constant_value;
static int require_constant_elements;

/* 1 if it is ok to output this constructor as we read it.
   0 means must accumulate a CONSTRUCTOR expression.  */
static int constructor_incremental;

/* DECL node for which an initializer is being read.
   0 means we are reading a constructor expression
   such as (struct foo) {...}.  */
static tree constructor_decl;

/* start_init saves the ASMSPEC arg here for really_start_incremental_init.  */
static char *constructor_asmspec;

/* Nonzero if this is an initializer for a top-level decl.  */
static int constructor_top_level;

/* This stack has a level for each implicit or explicit level of
   structuring in the initializer, including the outermost one.  It
   saves the values of most of the variables above.  */

struct constructor_stack
{
  struct constructor_stack *next;
  tree type;
  tree fields;
  tree index;
  tree range_end;
  tree max_index;
  tree unfilled_index;
  tree unfilled_fields;
  tree bit_index;
  tree elements;
  int offset;
  tree pending_elts;
  int depth;
  /* If nonzero, this value should replace the entire
     constructor at this level.  */
  tree replacement_value;
  char constant;
  char simple;
  char implicit;
  char incremental;
  char erroneous;
  char outer;
};

struct constructor_stack *constructor_stack;

/* This stack records separate initializers that are nested.
   Nested initializers can't happen in ANSI C, but GNU C allows them
   in cases like { ... (struct foo) { ... } ... }.  */

struct initializer_stack
{
  struct initializer_stack *next;
  tree decl;
  char *asmspec;
  struct constructor_stack *constructor_stack;
  tree elements;
  struct spelling *spelling;
  struct spelling *spelling_base;
  int spelling_size;
  char top_level;
  char incremental;
  char require_constant_value;
  char require_constant_elements;
  char deferred;
};

struct initializer_stack *initializer_stack;

/* Prepare to parse and output the initializer for variable DECL.  */

void
start_init (decl, asmspec_tree, top_level)
     tree decl;
     tree asmspec_tree;
     int top_level;
{
  char *locus;
  struct initializer_stack *p
    = (struct initializer_stack *) xmalloc (sizeof (struct initializer_stack));
  char *asmspec = 0;

  if (asmspec_tree)
    asmspec = TREE_STRING_POINTER (asmspec_tree);

  p->decl = constructor_decl;
  p->asmspec = constructor_asmspec;
  p->incremental = constructor_incremental;
  p->require_constant_value = require_constant_value;
  p->require_constant_elements = require_constant_elements;
  p->constructor_stack = constructor_stack;
  p->elements = constructor_elements;
  p->spelling = spelling;
  p->spelling_base = spelling_base;
  p->spelling_size = spelling_size;
  p->deferred = constructor_subconstants_deferred;
  p->top_level = constructor_top_level;
  p->next = initializer_stack;
  initializer_stack = p;

  constructor_decl = decl;
  constructor_incremental = top_level;
  constructor_asmspec = asmspec;
  constructor_subconstants_deferred = 0;
  constructor_top_level = top_level;

  if (decl != NULL_TREE)
    {
      require_constant_value = TREE_STATIC (decl);
      require_constant_elements
        = ((TREE_STATIC (decl) || pedantic)
           /* For a scalar, you can always use any value to initialize,
              even within braces.  */
           && (TREE_CODE (TREE_TYPE (decl)) == ARRAY_TYPE
               || TREE_CODE (TREE_TYPE (decl)) == RECORD_TYPE
               || TREE_CODE (TREE_TYPE (decl)) == UNION_TYPE
               || TREE_CODE (TREE_TYPE (decl)) == QUAL_UNION_TYPE));
      locus = IDENTIFIER_POINTER (DECL_NAME (decl));
      constructor_incremental |= TREE_STATIC (decl);
    }
  else
    {
      require_constant_value = 0;
      require_constant_elements = 0;
      locus = "(anonymous)";
    }

  constructor_stack = 0;

  missing_braces_mentioned = 0;

  spelling_base = 0;
  spelling_size = 0;
  RESTORE_SPELLING_DEPTH (0);

  if (locus)
    push_string (locus);
}

void
finish_init ()
{
  struct initializer_stack *p = initializer_stack;

  /* Output subconstants (string constants, usually)
     that were referenced within this initializer and saved up.
     Must do this if and only if we called defer_addressed_constants.  */
  if (constructor_subconstants_deferred)
    output_deferred_addressed_constants ();

  /* Free the whole constructor stack of this initializer.  */
  while (constructor_stack)
    {
      struct constructor_stack *q = constructor_stack;
      constructor_stack = q->next;
      free (q);
    }

  /* Pop back to the data of the outer initializer (if any).  */
  constructor_decl = p->decl;
  constructor_asmspec = p->asmspec;
  constructor_incremental = p->incremental;
  require_constant_value = p->require_constant_value;
  require_constant_elements = p->require_constant_elements;
  constructor_stack = p->constructor_stack;
  constructor_elements = p->elements;
  spelling = p->spelling;
  spelling_base = p->spelling_base;
  spelling_size = p->spelling_size;
  constructor_subconstants_deferred = p->deferred;
  constructor_top_level = p->top_level;
  initializer_stack = p->next;
  free (p);
}

/* Call here when we see the initializer is surrounded by braces.
   This is instead of a call to push_init_level;
   it is matched by a call to pop_init_level.

   TYPE is the type to initialize, for a constructor expression.
   For an initializer for a decl, TYPE is zero.  */

void
really_start_incremental_init (type)
     tree type;
{
  struct constructor_stack *p
    = (struct constructor_stack *) xmalloc (sizeof (struct constructor_stack));

  if (type == 0)
    type = TREE_TYPE (constructor_decl);

  /* Turn off constructor_incremental if type is a struct with bitfields.
     Do this before the first push, so that the corrected value
     is available in finish_init.  */
  check_init_type_bitfields (type);

  p->type = constructor_type;
  p->fields = constructor_fields;
  p->index = constructor_index;
  p->range_end = constructor_range_end;
  p->max_index = constructor_max_index;
  p->unfilled_index = constructor_unfilled_index;
  p->unfilled_fields = constructor_unfilled_fields;
  p->bit_index = constructor_bit_index;
  p->elements = constructor_elements;
  p->constant = constructor_constant;
  p->simple = constructor_simple;
  p->erroneous = constructor_erroneous;
  p->pending_elts = constructor_pending_elts;
  p->depth = constructor_depth;
  p->replacement_value = 0;
  p->implicit = 0;
  p->incremental = constructor_incremental;
  p->outer = 0;
  p->next = 0;
  constructor_stack = p;

  constructor_constant = 1;
  constructor_simple = 1;
  constructor_depth = SPELLING_DEPTH ();
  constructor_elements = 0;
  constructor_pending_elts = 0;
  constructor_type = type;

  if (TREE_CODE (constructor_type) == RECORD_TYPE
      || TREE_CODE (constructor_type) == UNION_TYPE)
    {
      constructor_fields = TYPE_FIELDS (constructor_type);
      /* Skip any nameless bit fields at the beginning.  */
      while (constructor_fields != 0 && DECL_C_BIT_FIELD (constructor_fields)
             && DECL_NAME (constructor_fields) == 0)
        constructor_fields = TREE_CHAIN (constructor_fields);
      constructor_unfilled_fields = constructor_fields;
      constructor_bit_index = copy_node (integer_zero_node);
    }
  else if (TREE_CODE (constructor_type) == ARRAY_TYPE)
    {
      constructor_range_end = 0;
      if (TYPE_DOMAIN (constructor_type))
        {
#ifdef GPC
          constructor_max_index
            = copy_node (TYPE_MAX_VALUE (TYPE_DOMAIN (constructor_type)));
          TREE_TYPE (constructor_max_index) = integer_type_node;
          constructor_index
            = copy_node (TYPE_MIN_VALUE (TYPE_DOMAIN (constructor_type)));
          TREE_TYPE (constructor_index) = integer_type_node;
#else /* not GPC */
          constructor_max_index
            = TYPE_MAX_VALUE (TYPE_DOMAIN (constructor_type));
          constructor_index
            = copy_node (TYPE_MIN_VALUE (TYPE_DOMAIN (constructor_type)));
#endif /* not GPC */
        }
      else
        constructor_index = copy_node (integer_zero_node);
      constructor_unfilled_index = copy_node (constructor_index);
    }
  else
    {
      /* Handle the case of int x = {5}; */
      constructor_fields = constructor_type;
      constructor_unfilled_fields = constructor_type;
    }

  if (constructor_incremental)
    {
      int momentary = suspend_momentary ();
      push_obstacks_nochange ();
      if (TREE_PERMANENT (constructor_decl))
        end_temporary_allocation ();
      make_decl_rtl (constructor_decl, constructor_asmspec,
                     constructor_top_level);
      assemble_variable (constructor_decl, constructor_top_level, 0, 1);
      pop_obstacks ();
      resume_momentary (momentary);
    }

  if (constructor_incremental)
    {
      defer_addressed_constants ();
      constructor_subconstants_deferred = 1;
    }
}

/* Push down into a subobject, for initialization.
   If this is for an explicit set of braces, IMPLICIT is 0.
   If it is because the next element belongs at a lower level,
   IMPLICIT is 1.  */

void
push_init_level (implicit)
     int implicit;
{
  struct constructor_stack *p;

  /* If we've exhausted any levels that didn't have braces,
     pop them now.  */
  while (constructor_stack->implicit)
    {
      if ((TREE_CODE (constructor_type) == RECORD_TYPE
           || TREE_CODE (constructor_type) == UNION_TYPE)
          && constructor_fields == 0)
        process_init_element (pop_init_level (1));
      else if (TREE_CODE (constructor_type) == ARRAY_TYPE
               && tree_int_cst_lt (constructor_max_index, constructor_index))
        process_init_element (pop_init_level (1));
      else
        break;
    }

  /* Structure elements may require alignment.  Do this now if necessary
     for the subaggregate, and if it comes next in sequence.  Don't do
     this for subaggregates that will go on the pending list.  */
  if (constructor_incremental && constructor_type != 0
      && TREE_CODE (constructor_type) == RECORD_TYPE && constructor_fields
      && constructor_fields == constructor_unfilled_fields)
    {
      /* Advance to offset of this element.  */
      if (! tree_int_cst_equal (constructor_bit_index,
                                DECL_FIELD_BITPOS (constructor_fields)))
        {
          int next = (TREE_INT_CST_LOW
                      (DECL_FIELD_BITPOS (constructor_fields))
                      / BITS_PER_UNIT);
          int here = (TREE_INT_CST_LOW (constructor_bit_index)
                      / BITS_PER_UNIT);

          assemble_zeros (next - here);
        }
      /* Indicate that we have now filled the structure up to the current
         field.  */
      constructor_unfilled_fields = constructor_fields;
    }

  p = (struct constructor_stack *) xmalloc (sizeof (struct constructor_stack));
  p->type = constructor_type;
  p->fields = constructor_fields;
  p->index = constructor_index;
  p->range_end = constructor_range_end;
  p->max_index = constructor_max_index;
  p->unfilled_index = constructor_unfilled_index;
  p->unfilled_fields = constructor_unfilled_fields;
  p->bit_index = constructor_bit_index;
  p->elements = constructor_elements;
  p->constant = constructor_constant;
  p->simple = constructor_simple;
  p->erroneous = constructor_erroneous;
  p->pending_elts = constructor_pending_elts;
  p->depth = constructor_depth;
  p->replacement_value = 0;
  p->implicit = implicit;
  p->incremental = constructor_incremental;
  p->outer = 0;
  p->next = constructor_stack;
  constructor_stack = p;

  constructor_constant = 1;
  constructor_simple = 1;
  constructor_depth = SPELLING_DEPTH ();
  constructor_elements = 0;
  constructor_pending_elts = 0;

  /* Don't die if an entire brace-pair level is superfluous
     in the containing level.  */
  if (constructor_type == 0)
    ;
  else if (TREE_CODE (constructor_type) == RECORD_TYPE
           || TREE_CODE (constructor_type) == UNION_TYPE)
    {
      /* Don't die if there are extra init elts at the end.  */
      if (constructor_fields == 0)
        constructor_type = 0;
      else
        {
          constructor_type = TREE_TYPE (constructor_fields);
          push_member_name (constructor_fields);
          constructor_depth++;
          if (constructor_fields != constructor_unfilled_fields)
            constructor_incremental = 0;
        }
    }
  else if (TREE_CODE (constructor_type) == ARRAY_TYPE)
    {
      constructor_type = TREE_TYPE (constructor_type);
      push_array_bounds (TREE_INT_CST_LOW (constructor_index));
      constructor_depth++;
      if (! tree_int_cst_equal (constructor_index, constructor_unfilled_index)
          || constructor_range_end != 0)
        constructor_incremental = 0;
    }

  if (constructor_type == 0)
    {
      gpc_error_init ("extra brace group at end of initializer%s",
                  " for `%s'", NULL);
      constructor_fields = 0;
      constructor_unfilled_fields = 0;
      return;
    }

  /* Turn off constructor_incremental if type is a struct with bitfields.  */
  check_init_type_bitfields (constructor_type);

  if (implicit && warn_missing_braces && !missing_braces_mentioned)
    {
      missing_braces_mentioned = 1;
      warning_init ("missing braces around initializer%s", " for `%s'", NULL);
    }

  if (TREE_CODE (constructor_type) == RECORD_TYPE
           || TREE_CODE (constructor_type) == UNION_TYPE)
    {
      constructor_fields = TYPE_FIELDS (constructor_type);
      /* Skip any nameless bit fields at the beginning.  */
      while (constructor_fields != 0 && DECL_C_BIT_FIELD (constructor_fields)
             && DECL_NAME (constructor_fields) == 0)
        constructor_fields = TREE_CHAIN (constructor_fields);
      constructor_unfilled_fields = constructor_fields;
      constructor_bit_index = copy_node (integer_zero_node);
    }
  else if (TREE_CODE (constructor_type) == ARRAY_TYPE)
    {
      constructor_range_end = 0;
      if (TYPE_DOMAIN (constructor_type))
        {
#ifdef GPC
          constructor_max_index
            = copy_node (TYPE_MAX_VALUE (TYPE_DOMAIN (constructor_type)));
          TREE_TYPE (constructor_max_index) = integer_type_node;
          constructor_index
            = copy_node (TYPE_MIN_VALUE (TYPE_DOMAIN (constructor_type)));
          TREE_TYPE (constructor_index) = integer_type_node;
#else /* not GPC */
          constructor_max_index
            = TYPE_MAX_VALUE (TYPE_DOMAIN (constructor_type));
          constructor_index
            = copy_node (TYPE_MIN_VALUE (TYPE_DOMAIN (constructor_type)));
#endif /* not GPC */
        }
      else
        constructor_index = copy_node (integer_zero_node);
      constructor_unfilled_index = copy_node (constructor_index);
    }
  else
    {
      warning_init ("braces around scalar initializer%s", " for `%s'", NULL);
      constructor_fields = constructor_type;
      constructor_unfilled_fields = constructor_type;
    }
}

/* Don't read a struct incrementally if it has any bitfields,
   because the incremental reading code doesn't know how to
   handle bitfields yet.  */

static void
check_init_type_bitfields (type)
     tree type;
{
  if (TREE_CODE (type) == RECORD_TYPE)
    {
      tree tail;
      for (tail = TYPE_FIELDS (type); tail;
           tail = TREE_CHAIN (tail))
        {
          if (DECL_C_BIT_FIELD (tail)
              /* This catches cases like `int foo : 8;'.  */
              || DECL_MODE (tail) != TYPE_MODE (TREE_TYPE (tail)))
            {
              constructor_incremental = 0;
              break;
            }

          check_init_type_bitfields (TREE_TYPE (tail));
        }
    }

  else if (TREE_CODE (type) == ARRAY_TYPE)
    check_init_type_bitfields (TREE_TYPE (type));
}

/* At the end of an implicit or explicit brace level,
   finish up that level of constructor.
   If we were outputting the elements as they are read, return 0
   from inner levels (process_init_element ignores that),
   but return error_mark_node from the outermost level
   (that's what we want to put in DECL_INITIAL).
   Otherwise, return a CONSTRUCTOR expression.  */

tree
pop_init_level (implicit)
     int implicit;
{
  struct constructor_stack *p;
  int size = 0;
  tree constructor = 0;

  if (implicit == 0)
    {
      /* When we come to an explicit close brace,
         pop any inner levels that didn't have explicit braces.  */
      while (constructor_stack->implicit)
        process_init_element (pop_init_level (1));
    }

  p = constructor_stack;

  if (constructor_type != 0)
    size = int_size_in_bytes (constructor_type);

  /* Now output all pending elements.  */
  output_pending_init_elements (1);

#if 0 /* c-parse.in warns about {}.  */
  /* In ANSI, each brace level must have at least one element.  */
  if (! implicit && pedantic
      && (TREE_CODE (constructor_type) == ARRAY_TYPE
          ? integer_zerop (constructor_unfilled_index)
          : constructor_unfilled_fields == TYPE_FIELDS (constructor_type)))
    gpc_pedwarn_init ("empty braces in initializer%s", " for `%s'", NULL);
#endif

  /* Pad out the end of the structure.  */

  if (p->replacement_value)
    {
      /* If this closes a superfluous brace pair,
         just pass out the element between them.  */
      constructor = p->replacement_value;
      /* If this is the top level thing within the initializer,
         and it's for a variable, then since we already called
         assemble_variable, we must output the value now.  */
      if (p->next == 0 && constructor_decl != 0
          && constructor_incremental)
        {
          constructor = digest_init (constructor_type, constructor,
                                     require_constant_value,
                                     require_constant_elements);

          /* If initializing an array of unknown size,
             determine the size now.  */
          if (TREE_CODE (constructor_type) == ARRAY_TYPE
              && TYPE_DOMAIN (constructor_type) == 0)
            {
              int failure;
              int momentary_p;

              push_obstacks_nochange ();
              if (TREE_PERMANENT (constructor_type))
                end_temporary_allocation ();

              momentary_p = suspend_momentary ();

              /* We shouldn't have an incomplete array type within
                 some other type.  */
              if (constructor_stack->next)
                abort ();

              failure
                = complete_array_type (constructor_type,
                                       constructor, 0);
              if (failure)
                abort ();

              size = int_size_in_bytes (constructor_type);
              resume_momentary (momentary_p);
              pop_obstacks ();
            }

          output_constant (constructor, size);
        }
    }
  else if (constructor_type == 0)
    ;
  else if (TREE_CODE (constructor_type) != RECORD_TYPE
           && TREE_CODE (constructor_type) != UNION_TYPE
           && TREE_CODE (constructor_type) != ARRAY_TYPE
           && ! constructor_incremental)
    {
      /* A nonincremental scalar initializer--just return
         the element, after verifying there is just one.  */
      if (constructor_elements == 0)
        {
          gpc_error_init ("empty scalar initializer%s",
                      " for `%s'", NULL);
          constructor = error_mark_node;
        }
      else if (TREE_CHAIN (constructor_elements) != 0)
        {
          gpc_error_init ("extra elements in scalar initializer%s",
                      " for `%s'", NULL);
          constructor = TREE_VALUE (constructor_elements);
        }
      else
        constructor = TREE_VALUE (constructor_elements);
    }
  else if (! constructor_incremental)
    {
      if (constructor_erroneous)
        constructor = error_mark_node;
      else
        {
          int momentary = suspend_momentary ();

          constructor = build (CONSTRUCTOR, constructor_type, NULL_TREE,
                               nreverse (constructor_elements));
          if (constructor_constant)
            TREE_CONSTANT (constructor) = 1;
          if (constructor_constant && constructor_simple)
            TREE_STATIC (constructor) = 1;

          resume_momentary (momentary);
        }
    }
  else
    {
      tree filled;
      int momentary = suspend_momentary ();

      if (TREE_CODE (constructor_type) == RECORD_TYPE
          || TREE_CODE (constructor_type) == UNION_TYPE)
        {
          /* Find the offset of the end of that field.  */
          filled = size_binop (CEIL_DIV_EXPR,
                               constructor_bit_index,
                               size_int (BITS_PER_UNIT));
        }
      else if (TREE_CODE (constructor_type) == ARRAY_TYPE)
        {
          /* If initializing an array of unknown size,
             determine the size now.  */
          if (TREE_CODE (constructor_type) == ARRAY_TYPE
              && TYPE_DOMAIN (constructor_type) == 0)
            {
              tree maxindex
                = size_binop (MINUS_EXPR,
                              constructor_unfilled_index,
                              integer_one_node);

              push_obstacks_nochange ();
              if (TREE_PERMANENT (constructor_type))
                end_temporary_allocation ();
              maxindex = copy_node (maxindex);
              TYPE_DOMAIN (constructor_type) = build_index_type (maxindex);
              TREE_TYPE (maxindex) = TYPE_DOMAIN (constructor_type);

              /* TYPE_MAX_VALUE is always one less than the number of elements
                 in the array, because we start counting at zero.  Therefore,
                 warn only if the value is less than zero.  */
              if (pedantic
                  && (tree_int_cst_sgn (TYPE_MAX_VALUE (TYPE_DOMAIN (constructor_type)))
                      < 0))
                error_with_decl (constructor_decl,
                                 "zero or negative array size `%s'");
              layout_type (constructor_type);
              size = int_size_in_bytes (constructor_type);
              pop_obstacks ();
            }

          filled = size_binop (MULT_EXPR, constructor_unfilled_index,
                               size_in_bytes (TREE_TYPE (constructor_type)));
        }
      else
        filled = 0;

      if (filled != 0)
        assemble_zeros (size - TREE_INT_CST_LOW (filled));

      resume_momentary (momentary);
    }


  constructor_type = p->type;
  constructor_fields = p->fields;
  constructor_index = p->index;
  constructor_range_end = p->range_end;
  constructor_max_index = p->max_index;
  constructor_unfilled_index = p->unfilled_index;
  constructor_unfilled_fields = p->unfilled_fields;
  constructor_bit_index = p->bit_index;
  constructor_elements = p->elements;
  constructor_constant = p->constant;
  constructor_simple = p->simple;
  constructor_erroneous = p->erroneous;
  constructor_pending_elts = p->pending_elts;
  constructor_depth = p->depth;
  constructor_incremental = p->incremental;
  RESTORE_SPELLING_DEPTH (constructor_depth);

  constructor_stack = p->next;
  free (p);

  if (constructor == 0)
    {
      if (constructor_stack == 0)
        return error_mark_node;
      return NULL_TREE;
    }
  return constructor;
}

/* Within an array initializer, specify the next index to be initialized.
   FIRST is that index.  If LAST is nonzero, then initialize a range
   of indices, running from FIRST through LAST.  */

void
set_init_index (first, last)
     tree first, last;
{
  while ((TREE_CODE (first) == NOP_EXPR
          || TREE_CODE (first) == CONVERT_EXPR
          || TREE_CODE (first) == NON_LVALUE_EXPR)
         && (TYPE_MODE (TREE_TYPE (first))
             == TYPE_MODE (TREE_TYPE (TREE_OPERAND (first, 0)))))
    (first) = TREE_OPERAND (first, 0);
  if (last)
    while ((TREE_CODE (last) == NOP_EXPR
            || TREE_CODE (last) == CONVERT_EXPR
            || TREE_CODE (last) == NON_LVALUE_EXPR)
           && (TYPE_MODE (TREE_TYPE (last))
               == TYPE_MODE (TREE_TYPE (TREE_OPERAND (last, 0)))))
      (last) = TREE_OPERAND (last, 0);

  if (TREE_CODE (first) != INTEGER_CST)
    gpc_error_init ("nonconstant array index in initializer%s", " for `%s'", NULL);
  else if (last != 0 && TREE_CODE (last) != INTEGER_CST)
    gpc_error_init ("nonconstant array index in initializer%s", " for `%s'", NULL);
  else if (! constructor_unfilled_index)
    gpc_error_init ("array index in non-array initializer%s", " for %s'", NULL);
  else if (tree_int_cst_lt (first, constructor_unfilled_index))
    gpc_error_init ("duplicate array index in initializer%s", " for `%s'", NULL);
  else
    {
      TREE_INT_CST_LOW (constructor_index) = TREE_INT_CST_LOW (first);
      TREE_INT_CST_HIGH (constructor_index) = TREE_INT_CST_HIGH (first);

      if (last != 0 && tree_int_cst_lt (last, first))
        gpc_error_init ("empty index range in initializer%s", " for `%s'", NULL);
      else
        {
          if (pedantic)
            pedwarn ("ANSI C forbids specifying element to initialize");
          constructor_range_end = last;
        }
    }
}

/* Within a struct initializer, specify the next field to be initialized.  */

void
set_init_label (fieldname)
     tree fieldname;
{
  tree tail;
  int passed = 0;

  /* Don't die if an entire brace-pair level is superfluous
     in the containing level.  */
  if (constructor_type == 0)
    return;

  for (tail = TYPE_FIELDS (constructor_type); tail;
       tail = TREE_CHAIN (tail))
    {
      if (tail == constructor_unfilled_fields)
        passed = 1;
      if (DECL_NAME (tail) == fieldname)
        break;
    }

  if (tail == 0)
    error ("unknown field `%s' specified in initializer",
           IDENTIFIER_POINTER (fieldname));
  else if (!passed)
    error ("field `%s' already initialized",
           IDENTIFIER_POINTER (fieldname));
  else
    {
      constructor_fields = tail;
      if (pedantic)
        pedwarn ("ANSI C forbids specifying structure member to initialize");
    }
}

/* "Output" the next constructor element.
   At top level, really output it to assembler code now.
   Otherwise, collect it in a list from which we will make a CONSTRUCTOR.
   TYPE is the data type that the containing data type wants here.
   FIELD is the field (a FIELD_DECL) or the index that this element fills.

   PENDING if non-nil means output pending elements that belong
   right after this element.  (PENDING is normally 1;
   it is 0 while outputting pending elements, to avoid recursion.)  */

static void
output_init_element (value, type, field, pending)
     tree value, type, field;
     int pending;
{
  int duplicate = 0;

  if (TREE_CODE (TREE_TYPE (value)) == FUNCTION_TYPE
      || (TREE_CODE (TREE_TYPE (value)) == ARRAY_TYPE
          && !(TREE_CODE (value) == STRING_CST
               && TREE_CODE (type) == ARRAY_TYPE
#ifdef GPC
               && TREE_CODE (TREE_TYPE (type)) == CHAR_TYPE)
#else /* not GPC */
               && TREE_CODE (TREE_TYPE (type)) == INTEGER_TYPE)
#endif /* not GPC */
          && !comptypes (TYPE_MAIN_VARIANT (TREE_TYPE (value)),
                         TYPE_MAIN_VARIANT (type))))
    value = default_conversion (value);

  if (value == error_mark_node)
    constructor_erroneous = 1;
  else if (!TREE_CONSTANT (value))
    constructor_constant = 0;
  else if (initializer_constant_valid_p (value, TREE_TYPE (value)) == 0
           || ((TREE_CODE (constructor_type) == RECORD_TYPE
                || TREE_CODE (constructor_type) == UNION_TYPE)
               && DECL_C_BIT_FIELD (field)
               && TREE_CODE (value) != INTEGER_CST))
    constructor_simple = 0;

  if (require_constant_value && ! TREE_CONSTANT (value))
    {
      gpc_error_init ("initializer element%s is not constant",
                  " for `%s'", NULL);
      value = error_mark_node;
    }
  else if (require_constant_elements
           && initializer_constant_valid_p (value, TREE_TYPE (value)) == 0)
    {
      gpc_error_init ("initializer element%s is not computable at load time",
                  " for `%s'", NULL);
      value = error_mark_node;
    }

  /* If this element duplicates one on constructor_pending_elts,
     print a message and ignore it.  Don't do this when we're
     processing elements taken off constructor_pending_elts,
     because we'd always get spurious errors.  */
  if (pending)
    {
      if (TREE_CODE (constructor_type) == RECORD_TYPE
          || TREE_CODE (constructor_type) == UNION_TYPE)
        {
          if (purpose_member (field, constructor_pending_elts))
            {
              gpc_error_init ("duplicate initializer%s", " for `%s'", NULL);
              duplicate = 1;
            }
        }
      if (TREE_CODE (constructor_type) == ARRAY_TYPE)
        {
          tree tail;
          for (tail = constructor_pending_elts; tail;
               tail = TREE_CHAIN (tail))
            if (TREE_PURPOSE (tail) != 0
                && TREE_CODE (TREE_PURPOSE (tail)) == INTEGER_CST
                && tree_int_cst_equal (TREE_PURPOSE (tail), constructor_index))
              break;

          if (tail != 0)
            {
              gpc_error_init ("duplicate initializer%s", " for `%s'", NULL);
              duplicate = 1;
            }
        }
    }

  /* If this element doesn't come next in sequence,
     put it on constructor_pending_elts.  */
  if (TREE_CODE (constructor_type) == ARRAY_TYPE
      && !tree_int_cst_equal (field, constructor_unfilled_index))
    {
      if (! duplicate)
        /* The copy_node is needed in case field is actually
           constructor_index, which is modified in place.  */
        constructor_pending_elts
          = tree_cons (copy_node (field),
                       digest_init (type, value, require_constant_value,
                                    require_constant_elements),
                       constructor_pending_elts);
    }
  else if (TREE_CODE (constructor_type) == RECORD_TYPE
           && field != constructor_unfilled_fields)
    {
      /* We do this for records but not for unions.  In a union,
         no matter which field is specified, it can be initialized
         right away since it starts at the beginning of the union.  */
      if (!duplicate)
        constructor_pending_elts
          = tree_cons (field,
                       digest_init (type, value, require_constant_value,
                                    require_constant_elements),
                       constructor_pending_elts);
    }
  else
    {
      /* Otherwise, output this element either to
         constructor_elements or to the assembler file.  */

      if (!duplicate)
        {
          if (! constructor_incremental)
            {
              if (field && TREE_CODE (field) == INTEGER_CST)
                field = copy_node (field);
              constructor_elements
                = tree_cons (field, digest_init (type, value,
                                                 require_constant_value,
                                                 require_constant_elements),
                             constructor_elements);
            }
          else
            {
              /* Structure elements may require alignment.
                 Do this, if necessary.  */
              if (TREE_CODE (constructor_type) == RECORD_TYPE)
                {
                  /* Advance to offset of this element.  */
                  if (! tree_int_cst_equal (constructor_bit_index,
                                            DECL_FIELD_BITPOS (field)))
                    {
                      int next = (TREE_INT_CST_LOW (DECL_FIELD_BITPOS (field))
                                  / BITS_PER_UNIT);
                      int here = (TREE_INT_CST_LOW (constructor_bit_index)
                                  / BITS_PER_UNIT);

                      assemble_zeros (next - here);
                    }
                }
              output_constant (digest_init (type, value,
                                            require_constant_value,
                                            require_constant_elements),
                               int_size_in_bytes (type));

              /* For a record or union,
                 keep track of end position of last field.  */
              if (TREE_CODE (constructor_type) == RECORD_TYPE
                  || TREE_CODE (constructor_type) == UNION_TYPE)
                {
                  tree temp = size_binop (PLUS_EXPR, DECL_FIELD_BITPOS (field),
                                          DECL_SIZE (field));
                  TREE_INT_CST_LOW (constructor_bit_index)
                    = TREE_INT_CST_LOW (temp);
                  TREE_INT_CST_HIGH (constructor_bit_index)
                    = TREE_INT_CST_HIGH (temp);
                }
            }
        }

      /* Advance the variable that indicates sequential elements output.  */
      if (TREE_CODE (constructor_type) == ARRAY_TYPE)
        {
          tree tem = size_binop (PLUS_EXPR, constructor_unfilled_index,
                                 integer_one_node);
          TREE_INT_CST_LOW (constructor_unfilled_index)
            = TREE_INT_CST_LOW (tem);
          TREE_INT_CST_HIGH (constructor_unfilled_index)
            = TREE_INT_CST_HIGH (tem);
        }
      else if (TREE_CODE (constructor_type) == RECORD_TYPE)
        constructor_unfilled_fields = TREE_CHAIN (constructor_unfilled_fields);
      else if (TREE_CODE (constructor_type) == UNION_TYPE)
        constructor_unfilled_fields = 0;

      /* Now output any pending elements which have become next.  */
      if (pending)
        output_pending_init_elements (0);
    }
}

/* Output any pending elements which have become next.
   As we output elements, constructor_unfilled_{fields,index}
   advances, which may cause other elements to become next;
   if so, they too are output.

   If ALL is 0, we return when there are
   no more pending elements to output now.

   If ALL is 1, we output space as necessary so that
   we can output all the pending elements.  */

static void
output_pending_init_elements (all)
     int all;
{
  tree tail;
  tree next;

 retry:

  /* Look thru the whole pending list.
     If we find an element that should be output now,
     output it.  Otherwise, set NEXT to the element
     that comes first among those still pending.  */

  next = 0;
  for (tail = constructor_pending_elts; tail;
       tail = TREE_CHAIN (tail))
    {
      if (TREE_CODE (constructor_type) == ARRAY_TYPE)
        {
          if (tree_int_cst_equal (TREE_PURPOSE (tail),
                                  constructor_unfilled_index))
            {
              output_init_element (TREE_VALUE (tail),
                                   TREE_TYPE (constructor_type),
                                   constructor_unfilled_index, 0);
              goto retry;
            }
          else if (tree_int_cst_lt (TREE_PURPOSE (tail),
                                    constructor_unfilled_index))
            ;
          else if (next == 0
                   || tree_int_cst_lt (TREE_PURPOSE (tail), next))
            next = TREE_PURPOSE (tail);
        }
      else if (TREE_CODE (constructor_type) == RECORD_TYPE
               || TREE_CODE (constructor_type) == UNION_TYPE)
        {
          if (TREE_PURPOSE (tail) == constructor_unfilled_fields)
            {
              output_init_element (TREE_VALUE (tail),
                                   TREE_TYPE (constructor_unfilled_fields),
                                   constructor_unfilled_fields,
                                   0);
              goto retry;
            }
          else if (constructor_unfilled_fields == 0
                   || tree_int_cst_lt (DECL_FIELD_BITPOS (TREE_PURPOSE (tail)),
                                       DECL_FIELD_BITPOS (constructor_unfilled_fields)))
            ;
          else if (next == 0
                   || tree_int_cst_lt (DECL_FIELD_BITPOS (TREE_PURPOSE (tail)),
                                       DECL_FIELD_BITPOS (next)))
            next = TREE_PURPOSE (tail);
        }
    }

  /* Ordinarily return, but not if we want to output all
     and there are elements left.  */
  if (! (all && next != 0))
    return;

  /* Generate space up to the position of NEXT.  */
  if (constructor_incremental)
    {
      tree filled;
      tree nextpos_tree = size_int (0);

      if (TREE_CODE (constructor_type) == RECORD_TYPE
          || TREE_CODE (constructor_type) == UNION_TYPE)
        {
          /* Find the last field written out, if any.  */
          for (tail = TYPE_FIELDS (constructor_type); tail;
               tail = TREE_CHAIN (tail))
            if (TREE_CHAIN (tail) == constructor_unfilled_fields)
              break;

          if (tail)
            /* Find the offset of the end of that field.  */
            filled = size_binop (CEIL_DIV_EXPR,
                                 size_binop (PLUS_EXPR,
                                             DECL_FIELD_BITPOS (tail),
                                             DECL_SIZE (tail)),
                                 size_int (BITS_PER_UNIT));
          else
            filled = size_int (0);

          nextpos_tree = size_binop (CEIL_DIV_EXPR,
                                     DECL_FIELD_BITPOS (next),
                                     size_int (BITS_PER_UNIT));

          TREE_INT_CST_HIGH (constructor_bit_index)
            = TREE_INT_CST_HIGH (DECL_FIELD_BITPOS (next));
          TREE_INT_CST_LOW (constructor_bit_index)
            = TREE_INT_CST_LOW (DECL_FIELD_BITPOS (next));
          constructor_unfilled_fields = next;
        }
      else if (TREE_CODE (constructor_type) == ARRAY_TYPE)
        {
          filled = size_binop (MULT_EXPR, constructor_unfilled_index,
                               size_in_bytes (TREE_TYPE (constructor_type)));
          nextpos_tree
            = size_binop (MULT_EXPR, next,
                          size_in_bytes (TREE_TYPE (constructor_type)));
          TREE_INT_CST_LOW (constructor_unfilled_index)
            = TREE_INT_CST_LOW (next);
          TREE_INT_CST_HIGH (constructor_unfilled_index)
            = TREE_INT_CST_HIGH (next);
        }
      else
        filled = 0;

      if (filled)
        {
          int nextpos = TREE_INT_CST_LOW (nextpos_tree);

          assemble_zeros (nextpos - TREE_INT_CST_LOW (filled));
        }
    }
  else
    {
      /* If it's not incremental, just skip over the gap,
         so that after jumping to retry we will output the next
         successive element.  */
      if (TREE_CODE (constructor_type) == RECORD_TYPE
          || TREE_CODE (constructor_type) == UNION_TYPE)
        constructor_unfilled_fields = next;
      else if (TREE_CODE (constructor_type) == ARRAY_TYPE)
        {
          TREE_INT_CST_LOW (constructor_unfilled_index)
            = TREE_INT_CST_LOW (next);
          TREE_INT_CST_HIGH (constructor_unfilled_index)
            = TREE_INT_CST_HIGH (next);
        }
    }

  goto retry;
}

/* Add one non-braced element to the current constructor level.
   This adjusts the current position within the constructor's type.
   This may also start or terminate implicit levels
   to handle a partly-braced initializer.

   Once this has found the correct level for the new element,
   it calls output_init_element.

   Note: if we are incrementally outputting this constructor,
   this function may be called with a null argument
   representing a sub-constructor that was already incrementally output.
   When that happens, we output nothing, but we do the bookkeeping
   to skip past that element of the current constructor.  */

void
process_init_element (value)
     tree value;
{
  tree orig_value = value;
  int string_flag = value != 0 && TREE_CODE (value) == STRING_CST;

  /* Handle superfluous braces around string cst as in
     char x[] = {"foo"}; */
  if (string_flag
      && constructor_type
      && TREE_CODE (constructor_type) == ARRAY_TYPE
#ifdef GPC
      && TREE_CODE (TREE_TYPE (constructor_type)) == CHAR_TYPE
#else /* not GPC */
      && TREE_CODE (TREE_TYPE (constructor_type)) == INTEGER_TYPE
#endif /* GPC */
      && integer_zerop (constructor_unfilled_index))
    {
      constructor_stack->replacement_value = value;
      return;
    }

  if (constructor_stack->replacement_value != 0)
    {
      gpc_error_init ("excess elements in struct initializer%s",
                  " after `%s'", NULL_PTR);
      return;
    }

  /* Ignore elements of a brace group if it is entirely superfluous
     and has already been diagnosed.  */
  if (constructor_type == 0)
    return;

  /* If we've exhausted any levels that didn't have braces,
     pop them now.  */
  while (constructor_stack->implicit)
    {
      if ((TREE_CODE (constructor_type) == RECORD_TYPE
           || TREE_CODE (constructor_type) == UNION_TYPE)
          && constructor_fields == 0)
        process_init_element (pop_init_level (1));
      else if (TREE_CODE (constructor_type) == ARRAY_TYPE
               && (constructor_max_index == 0
                   || tree_int_cst_lt (constructor_max_index,
                                       constructor_index)))
        process_init_element (pop_init_level (1));
      else
        break;
    }

  while (1)
    {
      if (TREE_CODE (constructor_type) == RECORD_TYPE)
        {
          tree fieldtype;
          enum tree_code fieldcode;

          if (constructor_fields == 0)
            {
              gpc_pedwarn_init ("excess elements in struct initializer%s",
                            " after `%s'", NULL_PTR);
              break;
            }

          fieldtype = TREE_TYPE (constructor_fields);
          if (fieldtype != error_mark_node)
            fieldtype = TYPE_MAIN_VARIANT (fieldtype);
          fieldcode = TREE_CODE (fieldtype);

          /* Accept a string constant to initialize a subarray.  */
          if (value != 0
              && fieldcode == ARRAY_TYPE
#ifdef GPC
              && TREE_CODE (TREE_TYPE (fieldtype)) == CHAR_TYPE
#else /* not GPC */
              && TREE_CODE (TREE_TYPE (fieldtype)) == INTEGER_TYPE
#endif /* not GPC */
              && string_flag)
            value = orig_value;
          /* Otherwise, if we have come to a subaggregate,
             and we don't have an element of its type, push into it.  */
          else if (value != 0 && !constructor_no_implicit
                   && value != error_mark_node
                   && TYPE_MAIN_VARIANT (TREE_TYPE (value)) != fieldtype
                   && (fieldcode == RECORD_TYPE || fieldcode == ARRAY_TYPE
                       || fieldcode == UNION_TYPE))
            {
              push_init_level (1);
              continue;
            }

          if (value)
            {
              push_member_name (constructor_fields);
              output_init_element (value, fieldtype, constructor_fields, 1);
              RESTORE_SPELLING_DEPTH (constructor_depth);
            }
          else
            /* Do the bookkeeping for an element that was
               directly output as a constructor.  */
            {
              /* For a record, keep track of end position of last field.  */
              tree temp = size_binop (PLUS_EXPR,
                                      DECL_FIELD_BITPOS (constructor_fields),
                                      DECL_SIZE (constructor_fields));
              TREE_INT_CST_LOW (constructor_bit_index)
                = TREE_INT_CST_LOW (temp);
              TREE_INT_CST_HIGH (constructor_bit_index)
                = TREE_INT_CST_HIGH (temp);

              constructor_unfilled_fields = TREE_CHAIN (constructor_fields);
            }

          constructor_fields = TREE_CHAIN (constructor_fields);
          /* Skip any nameless bit fields at the beginning.  */
          while (constructor_fields != 0
                 && DECL_C_BIT_FIELD (constructor_fields)
                 && DECL_NAME (constructor_fields) == 0)
            constructor_fields = TREE_CHAIN (constructor_fields);
          break;
        }
      if (TREE_CODE (constructor_type) == UNION_TYPE)
        {
          tree fieldtype;
          enum tree_code fieldcode;

          if (constructor_fields == 0)
            {
              gpc_pedwarn_init ("excess elements in union initializer%s",
                            " after `%s'", NULL_PTR);
              break;
            }

          fieldtype = TREE_TYPE (constructor_fields);
          if (fieldtype != error_mark_node)
            fieldtype = TYPE_MAIN_VARIANT (fieldtype);
          fieldcode = TREE_CODE (fieldtype);

          /* Accept a string constant to initialize a subarray.  */
          if (value != 0
              && fieldcode == ARRAY_TYPE
#ifdef GPC
              && TREE_CODE (TREE_TYPE (fieldtype)) == CHAR_TYPE
#else /* not GPC */
              && TREE_CODE (TREE_TYPE (fieldtype)) == INTEGER_TYPE
#endif /* not GPC */
              && string_flag)
            value = orig_value;
          /* Otherwise, if we have come to a subaggregate,
             and we don't have an element of its type, push into it.  */
          else if (value != 0 && !constructor_no_implicit
                   && value != error_mark_node
                   && TYPE_MAIN_VARIANT (TREE_TYPE (value)) != fieldtype
                   && (fieldcode == RECORD_TYPE || fieldcode == ARRAY_TYPE
                       || fieldcode == UNION_TYPE))
            {
              push_init_level (1);
              continue;
            }

          if (value)
            {
              push_member_name (constructor_fields);
              output_init_element (value, fieldtype, constructor_fields, 1);
              RESTORE_SPELLING_DEPTH (constructor_depth);
            }
          else
            /* Do the bookkeeping for an element that was
               directly output as a constructor.  */
            {
              TREE_INT_CST_LOW (constructor_bit_index)
                = TREE_INT_CST_LOW (DECL_SIZE (constructor_fields));
              TREE_INT_CST_HIGH (constructor_bit_index)
                = TREE_INT_CST_HIGH (DECL_SIZE (constructor_fields));

              constructor_unfilled_fields = TREE_CHAIN (constructor_fields);
            }

          constructor_fields = 0;
          break;
        }
      if (TREE_CODE (constructor_type) == ARRAY_TYPE)
        {
          tree elttype = TYPE_MAIN_VARIANT (TREE_TYPE (constructor_type));
          enum tree_code eltcode = TREE_CODE (elttype);

          /* Accept a string constant to initialize a subarray.  */
          if (value != 0
              && eltcode == ARRAY_TYPE
#ifdef GPC
              && TREE_CODE (TREE_TYPE (elttype)) == CHAR_TYPE
#else /* not GPC */
              && TREE_CODE (TREE_TYPE (elttype)) == INTEGER_TYPE
#endif /* not GPC */
              && string_flag)
            value = orig_value;
          /* Otherwise, if we have come to a subaggregate,
             and we don't have an element of its type, push into it.  */
          else if (value != 0 && !constructor_no_implicit
                   && value != error_mark_node
                   && TYPE_MAIN_VARIANT (TREE_TYPE (value)) != elttype
                   && (eltcode == RECORD_TYPE || eltcode == ARRAY_TYPE
                       || eltcode == UNION_TYPE))
            {
              push_init_level (1);
              continue;
            }

          if (constructor_max_index != 0
              && tree_int_cst_lt (constructor_max_index, constructor_index))
            {
              gpc_pedwarn_init ("excess elements in array initializer%s",
                            " after `%s'", NULL_PTR);
              break;
            }

          /* In the case of [LO .. HI] = VALUE, only evaluate VALUE once.  */
          if (constructor_range_end)
            {
              if (constructor_max_index != 0
                  && tree_int_cst_lt (constructor_max_index,
                                      constructor_range_end))
                {
                  gpc_pedwarn_init ("excess elements in array initializer%s",
                                " after `%s'", NULL_PTR);
                  TREE_INT_CST_HIGH (constructor_range_end)
                    = TREE_INT_CST_HIGH (constructor_max_index);
                  TREE_INT_CST_LOW (constructor_range_end)
                    = TREE_INT_CST_LOW (constructor_max_index);
                }

              value = save_expr (value);
            }

          /* Now output the actual element.
             Ordinarily, output once.
             If there is a range, repeat it till we advance past the range.  */
          do
            {
              tree tem;

              if (value)
                {
                  push_array_bounds (TREE_INT_CST_LOW (constructor_index));
                  output_init_element (value, elttype, constructor_index, 1);
                  RESTORE_SPELLING_DEPTH (constructor_depth);
                }

              tem = size_binop (PLUS_EXPR, constructor_index,
                                integer_one_node);
              TREE_INT_CST_LOW (constructor_index) = TREE_INT_CST_LOW (tem);
              TREE_INT_CST_HIGH (constructor_index) = TREE_INT_CST_HIGH (tem);

              if (!value)
                /* If we are doing the bookkeeping for an element that was
                   directly output as a constructor,
                   we must update constructor_unfilled_index.  */
                {
                  TREE_INT_CST_LOW (constructor_unfilled_index)
                    = TREE_INT_CST_LOW (constructor_index);
                  TREE_INT_CST_HIGH (constructor_unfilled_index)
                    = TREE_INT_CST_HIGH (constructor_index);
                }
            }
          while (! (constructor_range_end == 0
                    || tree_int_cst_lt (constructor_range_end,
                                        constructor_index)));

          break;
        }

      /* Handle the sole element allowed in a braced initializer
         for a scalar variable.  */
      if (constructor_fields == 0)
        {
          gpc_pedwarn_init ("excess elements in scalar initializer%s",
                        " after `%s'", NULL_PTR);
          break;
        }

      if (value)
        output_init_element (value, constructor_type, NULL_TREE, 1);
      constructor_fields = 0;
      break;
    }

  /* If the (lexically) previous elments are not now saved,
     we can discard the storage for them.  */
  if (constructor_incremental && constructor_pending_elts == 0 && value != 0
      && constructor_stack == 0)
    clear_momentary ();
}

/* Expand an ASM statement with operands, handling output operands
   that are not variables or INDIRECT_REFS by transforming such
   cases into cases that expand_asm_operands can handle.

   Arguments are same as for expand_asm_operands.  */

void
c_expand_asm_operands (string, outputs, inputs, clobbers, vol, filename, line)
     tree string, outputs, inputs, clobbers;
     int vol;
     char *filename;
     int line;
{
  int noutputs = list_length (outputs);
  register int i;
  /* o[I] is the place that output number I should be written.  */
  register tree *o = (tree *) alloca (noutputs * sizeof (tree));
  register tree tail;

  if (TREE_CODE (string) == ADDR_EXPR)
    string = TREE_OPERAND (string, 0);
  if (TREE_CODE (string) != STRING_CST)
    {
      error ("asm template is not a string constant");
      return;
    }

  /* Record the contents of OUTPUTS before it is modified.  */
  for (i = 0, tail = outputs; tail; tail = TREE_CHAIN (tail), i++)
    o[i] = TREE_VALUE (tail);

  /* Perform default conversions on array and function inputs.  */
  /* Don't do this for other types--
     it would screw up operands expected to be in memory.  */
  for (i = 0, tail = inputs; tail; tail = TREE_CHAIN (tail), i++)
    if (TREE_CODE (TREE_TYPE (TREE_VALUE (tail))) == ARRAY_TYPE
        || TREE_CODE (TREE_TYPE (TREE_VALUE (tail))) == FUNCTION_TYPE)
      TREE_VALUE (tail) = default_conversion (TREE_VALUE (tail));

  /* Generate the ASM_OPERANDS insn;
     store into the TREE_VALUEs of OUTPUTS some trees for
     where the values were actually stored.  */
  expand_asm_operands (string, outputs, inputs, clobbers, vol, filename, line);

  /* Copy all the intermediate outputs into the specified outputs.  */
  for (i = 0, tail = outputs; tail; tail = TREE_CHAIN (tail), i++)
    {
      if (o[i] != TREE_VALUE (tail))
        {
          expand_expr (build_modify_expr (o[i], NOP_EXPR, TREE_VALUE (tail)),
                       0, VOIDmode, 0);
          free_temp_slots ();
        }
      /* Detect modification of read-only values.
         (Otherwise done by build_modify_expr.)  */
      else
        {
          tree type = TREE_TYPE (o[i]);
          if (TREE_READONLY (o[i])
              || TYPE_READONLY (type)
              || ((TREE_CODE (type) == RECORD_TYPE
                   || TREE_CODE (type) == UNION_TYPE)
                  && C_TYPE_FIELDS_READONLY (type)))
            readonly_warning (o[i], "modification by `asm'");
        }
    }

  /* Those MODIFY_EXPRs could do autoincrements.  */
  emit_queue ();
}

/* Expand a C `return' statement.
   RETVAL is the expression for what to return,
   or a null pointer for `return;' with no value.  */

void
c_expand_return (retval)
     tree retval;
{
  tree valtype = TREE_TYPE (TREE_TYPE (current_function_decl));

  if (TREE_THIS_VOLATILE (current_function_decl))
    warning ("function declared `noreturn' has a `return' statement");

  if (!retval)
    {
      current_function_returns_null = 1;
      if (warn_return_type && valtype != 0 && TREE_CODE (valtype) != VOID_TYPE)
        warning ("`return' with no value, in function returning non-void");
      expand_null_return ();
    }
  else if (valtype == 0 || TREE_CODE (valtype) == VOID_TYPE)
    {
      current_function_returns_null = 1;
#ifdef GPC
      error ("`return' with a value in a procedure");
#else /* not GPC */
      if (pedantic || TREE_CODE (TREE_TYPE (retval)) != VOID_TYPE)
        pedwarn ("`return' with a value, in function returning void");
#endif /* not GPC */
      expand_return (retval);
    }
  else
    {
      tree t = convert_for_assignment (valtype, retval, "return",
                                       NULL_TREE, NULL_TREE, 0);
      tree res = DECL_RESULT (current_function_decl);
      tree inner;

      if (t == error_mark_node)
        return;

      inner = t = convert (TREE_TYPE (res), t);

      /* Strip any conversions, additions, and subtractions, and see if
         we are returning the address of a local variable.  Warn if so.  */
      while (1)
        {
          switch (TREE_CODE (inner))
            {
            case NOP_EXPR:   case NON_LVALUE_EXPR:  case CONVERT_EXPR:
            case PLUS_EXPR:
              inner = TREE_OPERAND (inner, 0);
              continue;

            case MINUS_EXPR:
              /* If the second operand of the MINUS_EXPR has a pointer
                 type (or is converted from it), this may be valid, so
                 don't give a warning.  */
              {
                tree op1 = TREE_OPERAND (inner, 1);

                while (! POINTER_TYPE_P (TREE_TYPE (op1))
                       && (TREE_CODE (op1) == NOP_EXPR
                           || TREE_CODE (op1) == NON_LVALUE_EXPR
                           || TREE_CODE (op1) == CONVERT_EXPR))
                  op1 = TREE_OPERAND (op1, 0);

                if (POINTER_TYPE_P (TREE_TYPE (op1)))
                  break;

                inner = TREE_OPERAND (inner, 0);
                continue;
              }

            case ADDR_EXPR:
              inner = TREE_OPERAND (inner, 0);

              while (TREE_CODE_CLASS (TREE_CODE (inner)) == 'r')
                inner = TREE_OPERAND (inner, 0);

              if (TREE_CODE (inner) == VAR_DECL
                  && ! DECL_EXTERNAL (inner)
                  && ! TREE_STATIC (inner)
                  && DECL_CONTEXT (inner) == current_function_decl)
                warning ("function returns address of local variable");
              break;

            default:
              break;
            }

          break;
        }

      t = build (MODIFY_EXPR, TREE_TYPE (res), res, t);
      TREE_SIDE_EFFECTS (t) = 1;
      expand_return (t);
      current_function_returns_value = 1;
    }
}

/* Start a C switch statement, testing expression EXP.
   Return EXP if it is valid, an error node otherwise.  */

tree
c_expand_start_case (exp)
     tree exp;
{
  register enum tree_code code = TREE_CODE (TREE_TYPE (exp));
  tree type = TREE_TYPE (exp);

#ifdef GPC
  if (! INTEGRAL_TYPE_P (type)
      && code != ERROR_MARK)
#else
  if (code != INTEGER_TYPE && code != ENUMERAL_TYPE && code != ERROR_MARK)
#endif /* GPC */
    {
#ifdef GPC
      error ("`case' selector is not of ordinal type");
#else
      error ("switch quantity not an integer");
#endif /* GPC */
      exp = error_mark_node;
    }
  else
    {
      tree index;
      type = TYPE_MAIN_VARIANT (TREE_TYPE (exp));

      if (warn_traditional
          && (type == long_integer_type_node
              || type == long_unsigned_type_node))
        pedwarn ("`long' switch expression not converted to `int' in ANSI C");

      exp = default_conversion (exp);
      type = TREE_TYPE (exp);
      index = get_unwidened (exp, NULL_TREE);
      /* We can't strip a conversion from a signed type to an unsigned,
         because if we did, int_fits_type_p would do the wrong thing
         when checking case values for being in range,
         and it's too hard to do the right thing.  */
      if (TREE_UNSIGNED (TREE_TYPE (exp))
          == TREE_UNSIGNED (TREE_TYPE (index)))
        exp = index;
    }

#ifdef GPC
  expand_start_case (0, exp, type, "`case' statement");
#else
  expand_start_case (1, exp, type, "switch statement");
#endif /* GPC */

  return exp;
}
