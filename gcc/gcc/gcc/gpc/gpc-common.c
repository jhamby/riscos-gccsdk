/* Subroutines shared by all languages that are variants of C.
   Copyright (C) 1992-2000 Free Software Foundation, Inc.

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

#include "gcc-version.h"
#include "config.h"
#include "gansidecl.h"
#include "system.h"

#include <stdio.h>
#ifdef GPC
#include "gbe.h"
#include "gpc-tree.h"
#else
/* In GPC, they are included via gbe.h */
#include "tree.h"
#include "flags.h"
#endif
#include "c-lex.h"
#include "obstack.h"
#include <ctype.h>
#ifdef EGCS
#include "toplev.h"
#endif
#include "output.h"

#ifdef GPC
#include "types.h"
#include "gpc-defs.h"
#include "parse.h"
#include "rts.h"
#include "util.h"
#endif /* GPC */

#ifndef WCHAR_TYPE_SIZE
#ifdef INT_TYPE_SIZE
#define WCHAR_TYPE_SIZE INT_TYPE_SIZE
#else
#define WCHAR_TYPE_SIZE BITS_PER_WORD
#endif
#endif

extern struct obstack permanent_obstack;

/* Nonzero means the expression being parsed will never be evaluated.
   This is a count, since unevaluated expressions can nest.  */
int skip_evaluation;

enum attrs {A_PACKED, A_NOCOMMON, A_COMMON, A_NORETURN, A_CONST, A_T_UNION,
            A_CONSTRUCTOR, A_DESTRUCTOR, A_MODE, A_SECTION, A_ALIGNED,
            A_UNUSED, A_FORMAT, A_FORMAT_ARG, A_WEAK, A_ALIAS};

static void declare_hidden_char_array   PROTO((char *, char *));
static void add_attribute               PROTO((enum attrs, char *,
                                               int, int, int));
static void init_attributes             PROTO((void));
static void record_international_format PROTO((tree, tree, int));

/* Keep a stack of if statements.  The value recorded is the number of
   compound statements seen up to the if keyword.  */
static int *if_stack;

/* Amount of space in the if statement stack.  */
static int if_stack_space = 0;

/* Stack pointer.  */
static int if_stack_pointer = 0;

void
c_expand_start_cond (cond, exitflag, compstmt_count)
     tree cond;
     int exitflag;
     int compstmt_count;
{
  /* Make sure there is enough space on the stack.  */
  if (if_stack_space == 0)
    {
      if_stack_space = 10;
      if_stack = (int *)xmalloc (10 * sizeof (int));
    }
  else if (if_stack_space == if_stack_pointer)
    {
      if_stack_space += 10;
      if_stack = (int *)xrealloc (if_stack, if_stack_space * sizeof (int));
    }

  /* Record this if statement.  */
  if_stack[if_stack_pointer++] = compstmt_count;

  expand_start_cond (cond, exitflag);
}

void
c_expand_end_cond ()
{
  if_stack_pointer--;
  expand_end_cond ();
}

void
c_expand_start_else ()
{
  if (warn_parentheses
      && if_stack_pointer > 1
      && if_stack[if_stack_pointer - 1] == if_stack[if_stack_pointer - 2])
    warning ("suggest explicit braces to avoid ambiguous `else'");

  /* This if statement can no longer cause a dangling else.  */
  if_stack[if_stack_pointer - 1]--;

  expand_start_else ();
}

/* Make bindings for __FUNCTION__ and __PRETTY_FUNCTION__.  */

void
declare_function_name ()
{
  char *name, *printable_name;

  if (current_function_decl == NULL)
    {
      name = "";
      printable_name = "top level";
    }
  else
    {
      /* Allow functions to be nameless (such as artificial ones).  */
      if (DECL_NAME (current_function_decl))
        name = IDENTIFIER_POINTER (DECL_NAME (current_function_decl));
      else
        name = "";
      printable_name = (*decl_printable_name) (current_function_decl, 2);
    }

  declare_hidden_char_array ("__FUNCTION__", name);
  declare_hidden_char_array ("__PRETTY_FUNCTION__", printable_name);
}

static void
declare_hidden_char_array (name, value)
     char *name, *value;
{
  tree decl, type, init;
  int vlen;

  /* If the default size of char arrays isn't big enough for the name,
     or if we want to give warnings for large objects, make a bigger one.  */
  vlen = strlen (value) + 1;
  type = char_array_type_node;
  if (TREE_INT_CST_LOW (TYPE_MAX_VALUE (TREE_TYPE (type))) < vlen
      || warn_larger_than)
    type = build_array_type (char_type_node,
                             build_index_type (build_int_2 (vlen, 0)));
  push_obstacks_nochange ();
  decl = build_decl (VAR_DECL, get_identifier (name), type);
  TREE_STATIC (decl) = 1;
  TREE_READONLY (decl) = 1;
  TREE_ASM_WRITTEN (decl) = 1;
  DECL_SOURCE_LINE (decl) = 0;
  DECL_ARTIFICIAL (decl) = 1;
  DECL_IN_SYSTEM_HEADER (decl) = 1;
  DECL_IGNORED_P (decl) = 1;
  init = build_string (vlen, value);
  TREE_TYPE (init) = type;
  DECL_INITIAL (decl) = init;
  finish_decl (pushdecl (decl), init, NULL_TREE);
}

/* Given a chain of STRING_CST nodes,
   concatenate them into one STRING_CST
   and give it a suitable array-of-chars data type.  */

tree
combine_strings (strings)
     tree strings;
{
  register tree value, t;
  register int length = 1;
  int wide_length = 0;
  int wide_flag = 0;
  int wchar_bytes = TYPE_PRECISION (wchar_type_node) / BITS_PER_UNIT;
  int nchars;
#ifdef GPC
  tree indices;
#endif /* GPC */

  if (TREE_CHAIN (strings))
    {
      /* More than one in the chain, so concatenate.  */
      register char *p, *q;

#ifdef GPC
      if (PEDANTIC (B_D_PASCAL))
        pedwarn ("this kind of string catenation is a GNU Pascal extension");
#endif /* GPC */
      /* Don't include the \0 at the end of each substring,
         except for the last one.
         Count wide strings and ordinary strings separately.  */
      for (t = strings; t; t = TREE_CHAIN (t))
        {
          if (TREE_TYPE (t) == wchar_array_type_node)
            {
              wide_length += (TREE_STRING_LENGTH (t) - wchar_bytes);
              wide_flag = 1;
            }
          else
            length += (TREE_STRING_LENGTH (t) - 1);
        }

      /* If anything is wide, the non-wides will be converted,
         which makes them take more space.  */
      if (wide_flag)
        length = length * wchar_bytes + wide_length;

      p = savealloc (length);

      /* Copy the individual strings into the new combined string.
         If the combined string is wide, convert the chars to ints
         for any individual strings that are not wide.  */

      q = p;
      for (t = strings; t; t = TREE_CHAIN (t))
        {
          int len = (TREE_STRING_LENGTH (t)
                     - ((TREE_TYPE (t) == wchar_array_type_node)
                        ? wchar_bytes : 1));
          if ((TREE_TYPE (t) == wchar_array_type_node) == wide_flag)
            {
              bcopy (TREE_STRING_POINTER (t), q, len);
              q += len;
            }
          else
            {
              int i;
              for (i = 0; i < len; i++)
                {
                  if (WCHAR_TYPE_SIZE == HOST_BITS_PER_SHORT)
                    ((short *) q)[i] = TREE_STRING_POINTER (t)[i];
                  else
                    ((int *) q)[i] = TREE_STRING_POINTER (t)[i];
                }
              q += len * wchar_bytes;
            }
        }
      if (wide_flag)
        {
          int i;
          for (i = 0; i < wchar_bytes; i++)
            *q++ = 0;
        }
      else
        *q = 0;

      value = make_node (STRING_CST);
      TREE_STRING_POINTER (value) = p;
      TREE_STRING_LENGTH (value) = length;
      TREE_CONSTANT (value) = 1;
    }
  else
    {
      value = strings;
      length = TREE_STRING_LENGTH (value);
      if (TREE_TYPE (value) == wchar_array_type_node)
        wide_flag = 1;
    }

  /* Compute the number of elements, for the array type.  */
  nchars = wide_flag ? length / wchar_bytes : length;

#ifdef GPC
  /* In Pascal, strings start at index 1.
   *
   * Also, take out the trailing NULL from the upper boundary
   * (except for the empty string --> kludge)-:.
   *
   * Now: TREE_STRING_LENGTH includes the NULL domain indices don't.
   * Why? To output C-compatible strings and to avoid changing
   *      the files that output assembler code.
   * How: The only place where the trailing null is meaningful
   *      is when we ASSIGN the string constant somewhere.
   *
   *      This is handled in expand_assignment() in expr.c
   */
  if (nchars <= 1)
    indices = build_index_2_type (build_int_2 (1, 0), build_int_2 (1, 0));
  else
    indices = build_index_2_type (build_int_2 (1, 0),
                  build_int_2 (nchars - (wide_flag ? wchar_bytes : 1), 0));
#endif /* GPC */

  /* Create the array type for the string constant.
     -Wwrite-strings says make the string constant an array of const char
     so that copying it to a non-const pointer will get a warning.  */
  if (warn_write_strings
      && (! flag_traditional  && ! flag_writable_strings))
    {
      tree elements
        = build_type_variant (wide_flag ? wchar_type_node : char_type_node,
                              1, 0);
#ifdef GPC
      TREE_TYPE (value)
        = build_array_type (elements, indices);
#else /* not GPC */
      TREE_TYPE (value)
        = build_array_type (elements,
                            build_index_type (build_int_2 (nchars - 1, 0)));
#endif /* not GPC */
    }
  else
#ifdef GPC
    TREE_TYPE (value)
      = build_array_type (wide_flag ? wchar_type_node : char_type_node,
                          indices);
  /* In addition, the string (array) constant is `packed' */
  PASCAL_TYPE_PACKED (TREE_TYPE (value)) = 1;
#else /* not GPC */
    TREE_TYPE (value)
      = build_array_type (wide_flag ? wchar_type_node : char_type_node,
                          build_index_type (build_int_2 (nchars - 1, 0)));
#endif /* not GPC */
  TREE_CONSTANT (value) = 1;
  TREE_STATIC (value) = 1;
  return value;
}

#ifdef GPC
/* Same as above, but insert "\n\t" between the strings.
 */
tree asm_combine_strings (strings)
     tree strings;
{
  tree scan = strings;
  while (TREE_CHAIN (scan))
    {
      tree newline_tab = build_string (3, "\n\t");
                                    /* Does the trailing NUL count here??? */
      TREE_CHAIN (newline_tab) = TREE_CHAIN (scan);
      TREE_CHAIN (scan) = newline_tab;
      scan = TREE_CHAIN (newline_tab);
    }
  return combine_strings (strings);
}
#endif /* GPC */

/* To speed up processing of attributes, we maintain an array of
   IDENTIFIER_NODES and the corresponding attribute types.  */

/* Array to hold attribute information.  */

static struct {enum attrs id; tree name; int min, max, decl_req;} attrtab[50];

static int attrtab_idx = 0;

/* Add an entry to the attribute table above.  */

static void
add_attribute (id, string, min_len, max_len, decl_req)
     enum attrs id;
     char *string;
     int min_len, max_len;
     int decl_req;
{
  char buf[100];

  attrtab[attrtab_idx].id = id;
  attrtab[attrtab_idx].name = get_identifier (string);
  attrtab[attrtab_idx].min = min_len;
  attrtab[attrtab_idx].max = max_len;
  attrtab[attrtab_idx++].decl_req = decl_req;

  sprintf (buf, "__%s__", string);

  attrtab[attrtab_idx].id = id;
  attrtab[attrtab_idx].name = get_identifier (buf);
  attrtab[attrtab_idx].min = min_len;
  attrtab[attrtab_idx].max = max_len;
  attrtab[attrtab_idx++].decl_req = decl_req;
}

/* Initialize attribute table.  */

static void
init_attributes ()
{
  add_attribute (A_PACKED, "packed", 0, 0, 0);
  add_attribute (A_NOCOMMON, "nocommon", 0, 0, 1);
  add_attribute (A_COMMON, "common", 0, 0, 1);
  add_attribute (A_NORETURN, "noreturn", 0, 0, 1);
  add_attribute (A_NORETURN, "volatile", 0, 0, 1);
  add_attribute (A_UNUSED, "unused", 0, 0, 0);
  add_attribute (A_CONST, "const", 0, 0, 1);
  add_attribute (A_T_UNION, "transparent_union", 0, 0, 0);
  add_attribute (A_CONSTRUCTOR, "constructor", 0, 0, 1);
  add_attribute (A_DESTRUCTOR, "destructor", 0, 0, 1);
  add_attribute (A_MODE, "mode", 1, 1, 1);
  add_attribute (A_SECTION, "section", 1, 1, 1);
  add_attribute (A_ALIGNED, "aligned", 0, 1, 0);
  add_attribute (A_FORMAT, "format", 3, 3, 1);
  add_attribute (A_FORMAT_ARG, "format_arg", 1, 1, 1);
  add_attribute (A_WEAK, "weak", 0, 0, 1);
  add_attribute (A_ALIAS, "alias", 1, 1, 1);
}

/* Process the attributes listed in ATTRIBUTES and PREFIX_ATTRIBUTES
   and install them in NODE, which is either a DECL (including a TYPE_DECL)
   or a TYPE.  PREFIX_ATTRIBUTES can appear after the declaration specifiers
   and declaration modifiers but before the declaration proper.  */

void
decl_attributes (node, attributes, prefix_attributes)
     tree node, attributes, prefix_attributes;
{
  tree decl = 0, type = 0;
  int is_type = 0;
  tree a;

  if (attrtab_idx == 0)
    init_attributes ();

  if (TREE_CODE_CLASS (TREE_CODE (node)) == 'd')
    {
      decl = node;
      type = TREE_TYPE (decl);
      is_type = TREE_CODE (node) == TYPE_DECL;
    }
  else if (TREE_CODE_CLASS (TREE_CODE (node)) == 't')
    type = node, is_type = 1;

  attributes = chainon (prefix_attributes, attributes);

  for (a = attributes; a; a = TREE_CHAIN (a))
    {
      tree name = TREE_PURPOSE (a);
      tree args = TREE_VALUE (a);
      int i;
      enum attrs id;

      for (i = 0; i < attrtab_idx; i++)
        if (attrtab[i].name == name)
          break;

      if (i == attrtab_idx)
        {
          if (! valid_machine_attribute (name, args, decl, type))
            warning ("`%s' attribute directive ignored",
                     IDENTIFIER_POINTER (name));
          else if (decl != 0)
            type = TREE_TYPE (decl);
          continue;
        }
      else if (attrtab[i].decl_req && decl == 0)
        {
          warning ("`%s' attribute does not apply to types",
                   IDENTIFIER_POINTER (name));
          continue;
        }
      else if (list_length (args) < attrtab[i].min
               || list_length (args) > attrtab[i].max)
        {
          error ("wrong number of arguments specified for `%s' attribute",
                 IDENTIFIER_POINTER (name));
          continue;
        }

      id = attrtab[i].id;
      switch (id)
        {
        case A_PACKED:
          if (is_type)
            TYPE_PACKED (type) = 1;
          else if (TREE_CODE (decl) == FIELD_DECL)
            DECL_PACKED (decl) = 1;
          /* We can't set DECL_PACKED for a VAR_DECL, because the bit is
             used for DECL_REGISTER.  It wouldn't mean anything anyway.  */
          else
            warning ("`%s' attribute ignored", IDENTIFIER_POINTER (name));
          break;

        case A_NOCOMMON:
          if (TREE_CODE (decl) == VAR_DECL)
            DECL_COMMON (decl) = 0;
          else
            warning ("`%s' attribute ignored", IDENTIFIER_POINTER (name));
          break;

        case A_COMMON:
          if (TREE_CODE (decl) == VAR_DECL)
            DECL_COMMON (decl) = 1;
          else
            warning ("`%s' attribute ignored", IDENTIFIER_POINTER (name));
          break;

        case A_NORETURN:
          if (TREE_CODE (decl) == FUNCTION_DECL)
            TREE_THIS_VOLATILE (decl) = 1;
          else if (TREE_CODE (type) == POINTER_TYPE
                   && TREE_CODE (TREE_TYPE (type)) == FUNCTION_TYPE)
            TREE_TYPE (decl) = type
              = build_pointer_type
                (build_type_variant (TREE_TYPE (type),
                                     TREE_READONLY (TREE_TYPE (type)), 1));
          else
            warning ("`%s' attribute ignored", IDENTIFIER_POINTER (name));
          break;

        case A_UNUSED:
          if (is_type)
            TREE_USED (type) = 1;
          else if (TREE_CODE (decl) == PARM_DECL
                   || TREE_CODE (decl) == VAR_DECL
                   || TREE_CODE (decl) == FUNCTION_DECL)
            TREE_USED (decl) = 1;
          else
            warning ("`%s' attribute ignored", IDENTIFIER_POINTER (name));
          break;

        case A_CONST:
          if (TREE_CODE (decl) == FUNCTION_DECL)
            TREE_READONLY (decl) = 1;
          else if (TREE_CODE (type) == POINTER_TYPE
                   && TREE_CODE (TREE_TYPE (type)) == FUNCTION_TYPE)
            TREE_TYPE (decl) = type
              = build_pointer_type
                (build_type_variant (TREE_TYPE (type), 1,
                                     TREE_THIS_VOLATILE (TREE_TYPE (type))));
          else
            warning ( "`%s' attribute ignored", IDENTIFIER_POINTER (name));
          break;

        case A_T_UNION:
          if (is_type
              && TREE_CODE (type) == UNION_TYPE
              && (decl == 0
                  || (TYPE_FIELDS (type) != 0
                      && TYPE_MODE (type) == DECL_MODE (TYPE_FIELDS (type)))))
            TYPE_TRANSPARENT_UNION (type) = 1;
          else if (decl != 0 && TREE_CODE (decl) == PARM_DECL
                   && TREE_CODE (type) == UNION_TYPE
                   && TYPE_MODE (type) == DECL_MODE (TYPE_FIELDS (type)))
            DECL_TRANSPARENT_UNION (decl) = 1;
          else
            warning ("`%s' attribute ignored", IDENTIFIER_POINTER (name));
          break;

        case A_CONSTRUCTOR:
          if (TREE_CODE (decl) == FUNCTION_DECL
              && TREE_CODE (type) == FUNCTION_TYPE
              && decl_function_context (decl) == 0)
            {
              DECL_STATIC_CONSTRUCTOR (decl) = 1;
              TREE_USED (decl) = 1;
            }
          else
            warning ("`%s' attribute ignored", IDENTIFIER_POINTER (name));
          break;

        case A_DESTRUCTOR:
          if (TREE_CODE (decl) == FUNCTION_DECL
              && TREE_CODE (type) == FUNCTION_TYPE
              && decl_function_context (decl) == 0)
            {
              DECL_STATIC_DESTRUCTOR (decl) = 1;
              TREE_USED (decl) = 1;
            }
          else
            warning ("`%s' attribute ignored", IDENTIFIER_POINTER (name));
          break;

        case A_MODE:
          if (TREE_CODE (TREE_VALUE (args)) != IDENTIFIER_NODE)
            warning ("`%s' attribute ignored", IDENTIFIER_POINTER (name));
          else
            {
              int j;
              char *p = IDENTIFIER_POINTER (TREE_VALUE (args));
              int len = strlen (p);
              enum machine_mode mode = VOIDmode;
              tree typefm;

              if (len > 4 && p[0] == '_' && p[1] == '_'
                  && p[len - 1] == '_' && p[len - 2] == '_')
                {
                  char *newp = (char *) alloca (len - 1);

                  strcpy (newp, &p[2]);
                  newp[len - 4] = '\0';
                  p = newp;
                }

              /* Give this decl a type with the specified mode.
                 First check for the special modes.  */
              if (! strcmp (p, "byte"))
                mode = byte_mode;
              else if (!strcmp (p, "word"))
                mode = word_mode;
              else if (! strcmp (p, "pointer"))
                mode = ptr_mode;
              else
                for (j = 0; j < NUM_MACHINE_MODES; j++)
                  if (!strcmp (p, GET_MODE_NAME (j)))
                    mode = (enum machine_mode) j;

              if (mode == VOIDmode)
                error ("unknown machine mode `%s'", p);
              else if (0 == (typefm = type_for_mode (mode,
                                                     TREE_UNSIGNED (type))))
                error ("no data type for mode `%s'", p);
              else
                {
                  TREE_TYPE (decl) = type = typefm;
                  DECL_SIZE (decl) = 0;
                  layout_decl (decl, 0);
                }
            }
          break;

        case A_SECTION:
#ifdef ASM_OUTPUT_SECTION_NAME
          if ((TREE_CODE (decl) == FUNCTION_DECL
               || TREE_CODE (decl) == VAR_DECL)
              && TREE_CODE (TREE_VALUE (args)) == STRING_CST)
            {
              if (TREE_CODE (decl) == VAR_DECL
                  && current_function_decl != NULL_TREE
                  && ! TREE_STATIC (decl))
                error_with_decl (decl,
                  "section attribute cannot be specified for local variables");
              /* The decl may have already been given a section attribute from
                 a previous declaration.  Ensure they match.  */
              else if (DECL_SECTION_NAME (decl) != NULL_TREE
                       && strcmp (TREE_STRING_POINTER (DECL_SECTION_NAME (decl)),
                                  TREE_STRING_POINTER (TREE_VALUE (args))) != 0)
                error_with_decl (node,
                                 "section of `%s' conflicts with previous declaration");
              else
                DECL_SECTION_NAME (decl) = TREE_VALUE (args);
            }
          else
            error_with_decl (node,
                           "section attribute not allowed for `%s'");
#else
          error_with_decl (node,
                  "section attributes are not supported for this target");
#endif
          break;

        case A_ALIGNED:
          {
            tree align_expr
              = (args ? TREE_VALUE (args)
                 : size_int (BIGGEST_ALIGNMENT / BITS_PER_UNIT));
            int align;

            /* Strip any NOPs of any kind.  */
            while (TREE_CODE (align_expr) == NOP_EXPR
                   || TREE_CODE (align_expr) == CONVERT_EXPR
                   || TREE_CODE (align_expr) == NON_LVALUE_EXPR)
              align_expr = TREE_OPERAND (align_expr, 0);

            if (TREE_CODE (align_expr) != INTEGER_CST)
              {
                error ("requested alignment is not a constant");
                continue;
              }

            align = TREE_INT_CST_LOW (align_expr) * BITS_PER_UNIT;

            if (exact_log2 (align) == -1)
              error ("requested alignment is not a power of 2");
            else if (is_type)
              TYPE_ALIGN (type) = align;
            else if (TREE_CODE (decl) != VAR_DECL
                     && TREE_CODE (decl) != FIELD_DECL)
              error_with_decl (decl,
                               "alignment may not be specified for `%s'");
            else
              DECL_ALIGN (decl) = align;
          }
          break;

        case A_FORMAT:
          {
            tree format_type = TREE_VALUE (args);
            tree format_num_expr = TREE_VALUE (TREE_CHAIN (args));
            tree first_arg_num_expr
              = TREE_VALUE (TREE_CHAIN (TREE_CHAIN (args)));
            int format_num;
            int first_arg_num;
            int is_scan;
            tree argument;
            int arg_num;

            if (TREE_CODE (decl) != FUNCTION_DECL)
              {
                error_with_decl (decl,
                         "argument format specified for non-function `%s'");
                continue;
              }

            if (TREE_CODE (format_type) == IDENTIFIER_NODE
                && (!strcmp (IDENTIFIER_POINTER (format_type), "printf")
                    || !strcmp (IDENTIFIER_POINTER (format_type),
                                "__printf__")))
              is_scan = 0;
            else if (TREE_CODE (format_type) == IDENTIFIER_NODE
                     && (!strcmp (IDENTIFIER_POINTER (format_type), "scanf")
                         || !strcmp (IDENTIFIER_POINTER (format_type),
                                     "__scanf__")))
              is_scan = 1;
            else if (TREE_CODE (format_type) == IDENTIFIER_NODE)
              {
                error ("`%s' is an unrecognized format function type",
                       IDENTIFIER_POINTER (format_type));
                continue;
              }
            else
              {
                error ("unrecognized format specifier");
                continue;
              }

            /* Strip any conversions from the string index and first arg number
               and verify they are constants.  */
            while (TREE_CODE (format_num_expr) == NOP_EXPR
                   || TREE_CODE (format_num_expr) == CONVERT_EXPR
                   || TREE_CODE (format_num_expr) == NON_LVALUE_EXPR)
              format_num_expr = TREE_OPERAND (format_num_expr, 0);

            while (TREE_CODE (first_arg_num_expr) == NOP_EXPR
                   || TREE_CODE (first_arg_num_expr) == CONVERT_EXPR
                   || TREE_CODE (first_arg_num_expr) == NON_LVALUE_EXPR)
              first_arg_num_expr = TREE_OPERAND (first_arg_num_expr, 0);

            if (TREE_CODE (format_num_expr) != INTEGER_CST
                || TREE_CODE (first_arg_num_expr) != INTEGER_CST)
              {
                error ("format string has non-constant operand number");
                continue;
              }

            format_num = TREE_INT_CST_LOW (format_num_expr);
            first_arg_num = TREE_INT_CST_LOW (first_arg_num_expr);
            if (first_arg_num != 0 && first_arg_num <= format_num)
              {
                error ("format string arg follows the args to be formatted");
                continue;
              }

            /* If a parameter list is specified, verify that the format_num
               argument is actually a string, in case the format attribute
               is in error.  */
            argument = TYPE_ARG_TYPES (type);
            if (argument)
              {
                for (arg_num = 1; ; ++arg_num)
                  {
                    if (argument == 0 || arg_num == format_num)
                      break;
                    argument = TREE_CHAIN (argument);
                  }
                if (! argument
                    || TREE_CODE (TREE_VALUE (argument)) != POINTER_TYPE
                  || (TYPE_MAIN_VARIANT (TREE_TYPE (TREE_VALUE (argument)))
                      != char_type_node))
                  {
                    error ("format string arg not a string type");
                    continue;
                  }
                if (first_arg_num != 0)
                  {
                    /* Verify that first_arg_num points to the last arg,
                       the ...  */
                    while (argument)
                      arg_num++, argument = TREE_CHAIN (argument);
                  if (arg_num != first_arg_num)
                    {
                      error ("args to be formatted is not ...");
                      continue;
                    }
                  }
              }

            record_function_format (DECL_NAME (decl),
                                    DECL_ASSEMBLER_NAME (decl),
                                    is_scan, format_num, first_arg_num);
            break;
          }

        case A_FORMAT_ARG:
          {
            tree format_num_expr = TREE_VALUE (args);
            int format_num, arg_num;
            tree argument;

            if (TREE_CODE (decl) != FUNCTION_DECL)
              {
                error_with_decl (decl,
                         "argument format specified for non-function `%s'");
                continue;
              }

            /* Strip any conversions from the first arg number and verify it
               is a constant.  */
            while (TREE_CODE (format_num_expr) == NOP_EXPR
                   || TREE_CODE (format_num_expr) == CONVERT_EXPR
                   || TREE_CODE (format_num_expr) == NON_LVALUE_EXPR)
              format_num_expr = TREE_OPERAND (format_num_expr, 0);

            if (TREE_CODE (format_num_expr) != INTEGER_CST)
              {
                error ("format string has non-constant operand number");
                continue;
              }

            format_num = TREE_INT_CST_LOW (format_num_expr);

            /* If a parameter list is specified, verify that the format_num
               argument is actually a string, in case the format attribute
               is in error.  */
            argument = TYPE_ARG_TYPES (type);
            if (argument)
              {
                for (arg_num = 1; ; ++arg_num)
                  {
                    if (argument == 0 || arg_num == format_num)
                      break;
                    argument = TREE_CHAIN (argument);
                  }
                if (! argument
                    || TREE_CODE (TREE_VALUE (argument)) != POINTER_TYPE
                  || (TYPE_MAIN_VARIANT (TREE_TYPE (TREE_VALUE (argument)))
                      != char_type_node))
                  {
                    error ("format string arg not a string type");
                    continue;
                  }
              }

            if (TREE_CODE (TREE_TYPE (TREE_TYPE (decl))) != POINTER_TYPE
                || (TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (TREE_TYPE (decl))))
                    != char_type_node))
              {
                error ("function does not return string type");
                continue;
              }

            record_international_format (DECL_NAME (decl),
                                         DECL_ASSEMBLER_NAME (decl),
                                         format_num);
            break;
          }

        case A_WEAK:
          declare_weak (decl);
          break;

        case A_ALIAS:
          if ((TREE_CODE (decl) == FUNCTION_DECL && DECL_INITIAL (decl))
              || (TREE_CODE (decl) != FUNCTION_DECL && ! DECL_EXTERNAL (decl)))
            error_with_decl (decl,
                             "`%s' defined both normally and as an alias");
          else if (decl_function_context (decl) == 0)
            {
              tree id = get_identifier (TREE_STRING_POINTER
                                        (TREE_VALUE (args)));
              if (TREE_CODE (decl) == FUNCTION_DECL)
                DECL_INITIAL (decl) = error_mark_node;
              else
                DECL_EXTERNAL (decl) = 0;
              assemble_alias (decl, id);
            }
          else
            warning ("`%s' attribute ignored", IDENTIFIER_POINTER (name));
          break;
        }
    }
}

/* Split SPECS_ATTRS, a list of declspecs and prefix attributes, into two
   lists.  SPECS_ATTRS may also be just a typespec (eg: RECORD_TYPE).

   The head of the declspec list is stored in DECLSPECS.
   The head of the attribute list is stored in PREFIX_ATTRIBUTES.

   Note that attributes in SPECS_ATTRS are stored in the TREE_PURPOSE of
   the list elements.  We drop the containing TREE_LIST nodes and link the
   resulting attributes together the way decl_attributes expects them.  */

void
split_specs_attrs (specs_attrs, declspecs, prefix_attributes)
     tree specs_attrs;
     tree *declspecs, *prefix_attributes;
{
  tree t, s, a, next, specs, attrs;

  /* This can happen in c++ (eg: decl: typespec initdecls ';').  */
  if (specs_attrs != NULL_TREE
      && TREE_CODE (specs_attrs) != TREE_LIST)
    {
      *declspecs = specs_attrs;
      *prefix_attributes = NULL_TREE;
      return;
    }

  /* Remember to keep the lists in the same order, element-wise.  */

  specs = s = NULL_TREE;
  attrs = a = NULL_TREE;
  for (t = specs_attrs; t; t = next)
    {
      next = TREE_CHAIN (t);
      /* Declspecs have a non-NULL TREE_VALUE.  */
      if (TREE_VALUE (t) != NULL_TREE)
        {
          if (specs == NULL_TREE)
            specs = s = t;
          else
            {
              TREE_CHAIN (s) = t;
              s = t;
            }
        }
      else
        {
          if (attrs == NULL_TREE)
            attrs = a = TREE_PURPOSE (t);
          else
            {
              TREE_CHAIN (a) = TREE_PURPOSE (t);
              a = TREE_PURPOSE (t);
            }
          /* More attrs can be linked here, move A to the end.  */
          while (TREE_CHAIN (a) != NULL_TREE)
            a = TREE_CHAIN (a);
        }
    }

  /* Terminate the lists.  */
  if (s != NULL_TREE)
    TREE_CHAIN (s) = NULL_TREE;
  if (a != NULL_TREE)
    TREE_CHAIN (a) = NULL_TREE;

  /* All done.  */
  *declspecs = specs;
  *prefix_attributes = attrs;
}

/* Check a printf/fprintf/sprintf/scanf/fscanf/sscanf format against
   a parameter list.  */

#define T_I     &integer_type_node
#define T_L     &long_integer_type_node
#define T_LL    &long_long_integer_type_node
#define T_S     &short_integer_type_node
#define T_UI    &unsigned_type_node
#define T_UL    &long_unsigned_type_node
#define T_ULL   &long_long_unsigned_type_node
#define T_US    &short_unsigned_type_node
#define T_F     &float_type_node
#define T_D     &double_type_node
#define T_LD    &long_double_type_node
#define T_C     &char_type_node
#define T_V     &void_type_node
#define T_W     &wchar_type_node
#define T_ST    &sizetype

typedef struct {
  char *format_chars;
  int pointer_count;
  /* Type of argument if no length modifier is used.  */
  tree *nolen;
  /* Type of argument if length modifier for shortening is used.
     If NULL, then this modifier is not allowed.  */
  tree *hlen;
  /* Type of argument if length modifier `l' is used.
     If NULL, then this modifier is not allowed.  */
  tree *llen;
  /* Type of argument if length modifier `q' or `ll' is used.
     If NULL, then this modifier is not allowed.  */
  tree *qlen;
  /* Type of argument if length modifier `L' is used.
     If NULL, then this modifier is not allowed.  */
  tree *bigllen;
  /* List of other modifier characters allowed with these options.  */
  char *flag_chars;
} format_char_info;

static format_char_info print_char_table[] = {
  { "di",       0,      T_I,    T_I,    T_L,    T_LL,   T_LL,   "-wp0 +"        },
  { "oxX",      0,      T_UI,   T_UI,   T_UL,   T_ULL,  T_ULL,  "-wp0#"         },
  { "u",        0,      T_UI,   T_UI,   T_UL,   T_ULL,  T_ULL,  "-wp0"          },
/* Two GNU extensions.  */
  { "Z",        0,      T_ST,   NULL,   NULL,   NULL,   NULL,   "-wp0"          },
  { "m",        0,      T_V,    NULL,   NULL,   NULL,   NULL,   "-wp"           },
  { "feEgGaA",  0,      T_D,    NULL,   NULL,   NULL,   T_LD,   "-wp0 +#"       },
  { "c",        0,      T_I,    NULL,   T_W,    NULL,   NULL,   "-w"            },
  { "C",        0,      T_W,    NULL,   NULL,   NULL,   NULL,   "-w"            },
  { "s",        1,      T_C,    NULL,   T_W,    NULL,   NULL,   "-wp"           },
  { "S",        1,      T_W,    NULL,   NULL,   NULL,   NULL,   "-wp"           },
  { "p",        1,      T_V,    NULL,   NULL,   NULL,   NULL,   "-w"            },
  { "n",        1,      T_I,    T_S,    T_L,    T_LL,   NULL,   ""              },
  { NULL }
};

static format_char_info scan_char_table[] = {
  { "di",       1,      T_I,    T_S,    T_L,    T_LL,   T_LL,   "*"     },
  { "ouxX",     1,      T_UI,   T_US,   T_UL,   T_ULL,  T_ULL,  "*"     },
  { "efgEGaA",  1,      T_F,    NULL,   T_D,    NULL,   T_LD,   "*"     },
  { "sc",       1,      T_C,    NULL,   T_W,    NULL,   NULL,   "*a"    },
  { "[",        1,      T_C,    NULL,   NULL,   NULL,   NULL,   "*a"    },
  { "C",        1,      T_W,    NULL,   NULL,   NULL,   NULL,   "*"     },
  { "S",        1,      T_W,    NULL,   NULL,   NULL,   NULL,   "*"     },
  { "p",        2,      T_V,    NULL,   NULL,   NULL,   NULL,   "*"     },
  { "n",        1,      T_I,    T_S,    T_L,    T_LL,   NULL,   ""      },
  { NULL }
};

typedef struct function_format_info
{
  struct function_format_info *next;  /* next structure on the list */
  tree name;                    /* identifier such as "printf" */
  tree assembler_name;          /* optional mangled identifier (for C++) */
  int is_scan;                  /* TRUE if *scanf */
  int format_num;               /* number of format argument */
  int first_arg_num;            /* number of first arg (zero for varargs) */
} function_format_info;

static function_format_info *function_format_list = NULL;

typedef struct international_format_info
{
  struct international_format_info *next;  /* next structure on the list */
  tree name;                    /* identifier such as "gettext" */
  tree assembler_name;          /* optional mangled identifier (for C++) */
  int format_num;               /* number of format argument */
} international_format_info;

static international_format_info *international_format_list = NULL;

static void check_format_info           PROTO((function_format_info *, tree));

/* Initialize the table of functions to perform format checking on.
   The ANSI functions are always checked (whether <stdio.h> is
   included or not), since it is common to call printf without
   including <stdio.h>.  There shouldn't be a problem with this,
   since ANSI reserves these function names whether you include the
   header file or not.  In any case, the checking is harmless.

   Also initialize the name of function that modify the format string for
   internationalization purposes.  */

void
init_function_format_info ()
{
  record_function_format (get_identifier ("printf"), NULL_TREE, 0, 1, 2);
  record_function_format (get_identifier ("fprintf"), NULL_TREE, 0, 2, 3);
  record_function_format (get_identifier ("sprintf"), NULL_TREE, 0, 2, 3);
  record_function_format (get_identifier ("scanf"), NULL_TREE, 1, 1, 2);
  record_function_format (get_identifier ("fscanf"), NULL_TREE, 1, 2, 3);
  record_function_format (get_identifier ("sscanf"), NULL_TREE, 1, 2, 3);
  record_function_format (get_identifier ("vprintf"), NULL_TREE, 0, 1, 0);
  record_function_format (get_identifier ("vfprintf"), NULL_TREE, 0, 2, 0);
  record_function_format (get_identifier ("vsprintf"), NULL_TREE, 0, 2, 0);

  record_international_format (get_identifier ("gettext"), NULL_TREE, 1);
  record_international_format (get_identifier ("dgettext"), NULL_TREE, 2);
  record_international_format (get_identifier ("dcgettext"), NULL_TREE, 2);
}

/* Record information for argument format checking.  FUNCTION_IDENT is
   the identifier node for the name of the function to check (its decl
   need not exist yet).  IS_SCAN is true for scanf-type format checking;
   false indicates printf-style format checking.  FORMAT_NUM is the number
   of the argument which is the format control string (starting from 1).
   FIRST_ARG_NUM is the number of the first actual argument to check
   against the format string, or zero if no checking is not be done
   (e.g. for varargs such as vfprintf).  */

void
record_function_format (name, assembler_name, is_scan,
                        format_num, first_arg_num)
      tree name;
      tree assembler_name;
      int is_scan;
      int format_num;
      int first_arg_num;
{
  function_format_info *info;

  /* Re-use existing structure if it's there.  */

  for (info = function_format_list; info; info = info->next)
    {
      if (info->name == name && info->assembler_name == assembler_name)
        break;
    }
  if (! info)
    {
      info = (function_format_info *) xmalloc (sizeof (function_format_info));
      info->next = function_format_list;
      function_format_list = info;

      info->name = name;
      info->assembler_name = assembler_name;
    }

  info->is_scan = is_scan;
  info->format_num = format_num;
  info->first_arg_num = first_arg_num;
}

/* Record information for the names of function that modify the format
   argument to format functions.  FUNCTION_IDENT is the identifier node for
   the name of the function (its decl need not exist yet) and FORMAT_NUM is
   the number of the argument which is the format control string (starting
   from 1).  */

static void
record_international_format (name, assembler_name, format_num)
      tree name;
      tree assembler_name;
      int format_num;
{
  international_format_info *info;

  /* Re-use existing structure if it's there.  */

  for (info = international_format_list; info; info = info->next)
    {
      if (info->name == name && info->assembler_name == assembler_name)
        break;
    }

  if (! info)
    {
      info
        = (international_format_info *)
          xmalloc (sizeof (international_format_info));
      info->next = international_format_list;
      international_format_list = info;

      info->name = name;
      info->assembler_name = assembler_name;
    }

  info->format_num = format_num;
}

static char     tfaff[] = "too few arguments for format";

/* Check the argument list of a call to printf, scanf, etc.
   NAME is the function identifier.
   ASSEMBLER_NAME is the function's assembler identifier.
   (Either NAME or ASSEMBLER_NAME, but not both, may be NULL_TREE.)
   PARAMS is the list of argument values.  */

void
check_function_format (name, assembler_name, params)
     tree name;
     tree assembler_name;
     tree params;
{
  function_format_info *info;

  /* See if this function is a format function.  */
  for (info = function_format_list; info; info = info->next)
    {
      if (info->assembler_name
          ? (info->assembler_name == assembler_name)
          : (info->name == name))
        {
          /* Yup; check it.  */
          check_format_info (info, params);
          break;
        }
    }
}

/* Check the argument list of a call to printf, scanf, etc.
   INFO points to the function_format_info structure.
   PARAMS is the list of argument values.  */

static void
check_format_info (info, params)
     function_format_info *info;
     tree params;
{
  int i;
  int arg_num;
  int suppressed, wide, precise;
  int length_char;
  int format_char;
  int format_length;
  tree format_tree;
  tree cur_param;
  tree cur_type;
  tree wanted_type;
  tree first_fillin_param;
  char *format_chars;
  format_char_info *fci;
  static char message[132];
  char flag_chars[8];
  int has_operand_number = 0;

  /* Skip to format argument.  If the argument isn't available, there's
     no work for us to do; prototype checking will catch the problem.  */
  for (arg_num = 1; ; ++arg_num)
    {
      if (params == 0)
        return;
      if (arg_num == info->format_num)
        break;
      params = TREE_CHAIN (params);
    }
  format_tree = TREE_VALUE (params);
  params = TREE_CHAIN (params);
  if (format_tree == 0)
    return;

  /* We can only check the format if it's a string constant.  */
  while (TREE_CODE (format_tree) == NOP_EXPR)
    format_tree = TREE_OPERAND (format_tree, 0); /* strip coercion */

  if (TREE_CODE (format_tree) == CALL_EXPR
      && TREE_CODE (TREE_OPERAND (format_tree, 0)) == ADDR_EXPR
      && (TREE_CODE (TREE_OPERAND (TREE_OPERAND (format_tree, 0), 0))
          == FUNCTION_DECL))
    {
      tree function = TREE_OPERAND (TREE_OPERAND (format_tree, 0), 0);

      /* See if this is a call to a known internationalization function
         that modifies the format arg.  */
      international_format_info *info;

      for (info = international_format_list; info; info = info->next)
        if (info->assembler_name
            ? (info->assembler_name == DECL_ASSEMBLER_NAME (function))
            : (info->name == DECL_NAME (function)))
          {
            tree inner_args;
            int i;

            for (inner_args = TREE_OPERAND (format_tree, 1), i = 1;
                 inner_args != 0;
                 inner_args = TREE_CHAIN (inner_args), i++)
              if (i == info->format_num)
                {
                  format_tree = TREE_VALUE (inner_args);

                  while (TREE_CODE (format_tree) == NOP_EXPR)
                    format_tree = TREE_OPERAND (format_tree, 0);
                }
          }
    }

  if (integer_zerop (format_tree))
    {
      warning ("null format string");
      return;
    }
  if (TREE_CODE (format_tree) != ADDR_EXPR)
    return;
  format_tree = TREE_OPERAND (format_tree, 0);
  if (TREE_CODE (format_tree) != STRING_CST)
    return;
  format_chars = TREE_STRING_POINTER (format_tree);
  format_length = TREE_STRING_LENGTH (format_tree);
  if (format_length <= 1)
    warning ("zero-length format string");
  if (format_chars[--format_length] != 0)
    {
      warning ("unterminated format string");
      return;
    }
  /* Skip to first argument to check.  */
  while (arg_num + 1 < info->first_arg_num)
    {
      if (params == 0)
        return;
      params = TREE_CHAIN (params);
      ++arg_num;
    }

  first_fillin_param = params;
  while (1)
    {
      int aflag;
      if (*format_chars == 0)
        {
          if (format_chars - TREE_STRING_POINTER (format_tree) != format_length)
            warning ("embedded `\\0' in format");
          if (info->first_arg_num != 0 && params != 0 && ! has_operand_number)
            warning ("too many arguments for format");
          return;
        }
      if (*format_chars++ != '%')
        continue;
      if (*format_chars == 0)
        {
          warning ("spurious trailing `%%' in format");
          continue;
        }
      if (*format_chars == '%')
        {
          ++format_chars;
          continue;
        }
      flag_chars[0] = 0;
      suppressed = wide = precise = FALSE;
      if (info->is_scan)
        {
          suppressed = *format_chars == '*';
          if (suppressed)
            ++format_chars;
          while (ISDIGIT (*format_chars))
            ++format_chars;
        }
      else
        {
          /* See if we have a number followed by a dollar sign.  If we do,
             it is an operand number, so set PARAMS to that operand.  */
          if (*format_chars >= '0' && *format_chars <= '9')
            {
              char *p = format_chars;

              while (*p >= '0' && *p++ <= '9')
                ;

              if (*p == '$')
                {
                  int opnum = atoi (format_chars);

                  params = first_fillin_param;
                  format_chars = p + 1;
                  has_operand_number = 1;

                  for (i = 1; i < opnum && params != 0; i++)
                    params = TREE_CHAIN (params);

                  if (opnum == 0 || params == 0)
                    {
                      warning ("operand number out of range in format");
                      return;
                    }
                }
            }

          while (*format_chars != 0 && index (" +#0-", *format_chars) != 0)
            {
              if (index (flag_chars, *format_chars) != 0)
                {
                  sprintf (message, "repeated `%c' flag in format",
                           *format_chars);
                  warning (message);
                }
              else
                {
                  i = strlen (flag_chars);
                  flag_chars[i++] = *format_chars++;
                  flag_chars[i] = 0;
                }
            }
          /* "If the space and + flags both appear,
             the space flag will be ignored."  */
          if (index (flag_chars, ' ') != 0
              && index (flag_chars, '+') != 0)
            warning ("use of both ` ' and `+' flags in format");
          /* "If the 0 and - flags both appear,
             the 0 flag will be ignored."  */
          if (index (flag_chars, '0') != 0
              && index (flag_chars, '-') != 0)
            warning ("use of both `0' and `-' flags in format");
          if (*format_chars == '*')
            {
              wide = TRUE;
              /* "...a field width...may be indicated by an asterisk.
                 In this case, an int argument supplies the field width..."  */
              ++format_chars;
              if (params == 0)
                {
                  warning (tfaff);
                  return;
                }
              if (info->first_arg_num != 0)
                {
                  cur_param = TREE_VALUE (params);
                  params = TREE_CHAIN (params);
                  ++arg_num;
                  /* size_t is generally not valid here.
                     It will work on most machines, because size_t and int
                     have the same mode.  But might as well warn anyway,
                     since it will fail on other machines.  */
                  if ((TYPE_MAIN_VARIANT (TREE_TYPE (cur_param))
                       != integer_type_node)
                      &&
                      (TYPE_MAIN_VARIANT (TREE_TYPE (cur_param))
                       != unsigned_type_node))
                    {
                      sprintf (message,
                               "field width is not type int (arg %d)",
                               arg_num);
                      warning (message);
                    }
                }
            }
          else
            {
              while (ISDIGIT (*format_chars))
                {
                  wide = TRUE;
                  ++format_chars;
                }
            }
          if (*format_chars == '.')
            {
              precise = TRUE;
              ++format_chars;
              if (*format_chars != '*' && !ISDIGIT (*format_chars))
                warning ("`.' not followed by `*' or digit in format");
              /* "...a...precision...may be indicated by an asterisk.
                 In this case, an int argument supplies the...precision."  */
              if (*format_chars == '*')
                {
                  if (info->first_arg_num != 0)
                    {
                      ++format_chars;
                      if (params == 0)
                        {
                          warning (tfaff);
                          return;
                        }
                      cur_param = TREE_VALUE (params);
                      params = TREE_CHAIN (params);
                      ++arg_num;
                      if (TYPE_MAIN_VARIANT (TREE_TYPE (cur_param))
                          != integer_type_node)
                        {
                          sprintf (message,
                                   "field width is not type int (arg %d)",
                                   arg_num);
                          warning (message);
                        }
                    }
                }
              else
                {
                  while (ISDIGIT (*format_chars))
                    ++format_chars;
                }
            }
        }
      if (*format_chars == 'h' || *format_chars == 'l')
        length_char = *format_chars++;
      else if (*format_chars == 'q' || *format_chars == 'L')
        {
          length_char = *format_chars++;
          if (pedantic)
            pedwarn ("ANSI C does not support the `%c' length modifier",
                     length_char);
        }
      else
        length_char = 0;
      if (length_char == 'l' && *format_chars == 'l')
        {
          length_char = 'q', format_chars++;
          if (pedantic)
            pedwarn ("ANSI C does not support the `ll' length modifier");
        }
      aflag = 0;
      if (*format_chars == 'a' && info->is_scan)
        {
          if (format_chars[1] == 's' || format_chars[1] == 'S'
              || format_chars[1] == '[')
            {
              /* `a' is used as a flag.  */
              aflag = 1;
              format_chars++;
            }
        }
      if (suppressed && length_char != 0)
        {
          sprintf (message,
                   "use of `*' and `%c' together in format",
                   length_char);
          warning (message);
        }
      format_char = *format_chars;
      if (format_char == 0 || format_char == '%')
        {
          warning ("conversion lacks type at end of format");
          continue;
        }
      format_chars++;
      fci = info->is_scan ? scan_char_table : print_char_table;
      while (fci->format_chars != 0
             && index (fci->format_chars, format_char) == 0)
          ++fci;
      if (fci->format_chars == 0)
        {
          if (format_char >= 040 && format_char < 0177)
            sprintf (message,
                     "unknown conversion type character `%c' in format",
                     format_char);
          else
            sprintf (message,
                     "unknown conversion type character 0x%x in format",
                     format_char);
          warning (message);
          continue;
        }
      if (wide && index (fci->flag_chars, 'w') == 0)
        {
          sprintf (message, "width used with `%c' format",
                   format_char);
          warning (message);
        }
      if (precise && index (fci->flag_chars, 'p') == 0)
        {
          sprintf (message, "precision used with `%c' format",
                   format_char);
          warning (message);
        }
      if (aflag && index (fci->flag_chars, 'a') == 0)
        {
          sprintf (message, "`a' flag used with `%c' format",
                   format_char);
          warning (message);
          /* To simplify the following code.  */
          aflag = 0;
        }
      if (info->is_scan && format_char == '[')
        {
          /* Skip over scan set, in case it happens to have '%' in it.  */
          if (*format_chars == '^')
            ++format_chars;
          /* Find closing bracket; if one is hit immediately, then
             it's part of the scan set rather than a terminator.  */
          if (*format_chars == ']')
            ++format_chars;
          while (*format_chars && *format_chars != ']')
            ++format_chars;
          if (*format_chars != ']')
              /* The end of the format string was reached.  */
              warning ("no closing `]' for `%%[' format");
        }
      if (suppressed)
        {
          if (index (fci->flag_chars, '*') == 0)
            {
              sprintf (message,
                       "suppression of `%c' conversion in format",
                       format_char);
              warning (message);
            }
          continue;
        }
      for (i = 0; flag_chars[i] != 0; ++i)
        {
          if (index (fci->flag_chars, flag_chars[i]) == 0)
            {
              sprintf (message, "flag `%c' used with type `%c'",
                       flag_chars[i], format_char);
              warning (message);
            }
        }
      if (precise && index (flag_chars, '0') != 0
          && (format_char == 'd' || format_char == 'i'
              || format_char == 'o' || format_char == 'u'
              || format_char == 'x' || format_char == 'x'))
        {
          sprintf (message,
                   "`0' flag ignored with precision specifier and `%c' format",
                   format_char);
          warning (message);
        }
      switch (length_char)
        {
        default: wanted_type = fci->nolen ? *(fci->nolen) : 0; break;
        case 'h': wanted_type = fci->hlen ? *(fci->hlen) : 0; break;
        case 'l': wanted_type = fci->llen ? *(fci->llen) : 0; break;
        case 'q': wanted_type = fci->qlen ? *(fci->qlen) : 0; break;
        case 'L': wanted_type = fci->bigllen ? *(fci->bigllen) : 0; break;
        }
      if (wanted_type == 0)
        {
          sprintf (message,
                   "use of `%c' length character with `%c' type character",
                   length_char, format_char);
          warning (message);
        }

      /*
       ** XXX -- should kvetch about stuff such as
       **       {
       **               const int       i;
       **
       **               scanf ("%d", &i);
       **       }
       */

      /* Finally. . .check type of argument against desired type!  */
      if (info->first_arg_num == 0)
        continue;
      if (fci->pointer_count == 0 && wanted_type == void_type_node)
        /* This specifier takes no argument.  */
        continue;
      if (params == 0)
        {
          warning (tfaff);
          return;
        }
      cur_param = TREE_VALUE (params);
      params = TREE_CHAIN (params);
      ++arg_num;
      cur_type = TREE_TYPE (cur_param);

      STRIP_NOPS (cur_param);

      /* Check the types of any additional pointer arguments
         that precede the "real" argument.  */
      for (i = 0; i < fci->pointer_count + aflag; ++i)
        {
          if (TREE_CODE (cur_type) == POINTER_TYPE)
            {
              cur_type = TREE_TYPE (cur_type);

              if (TREE_CODE (cur_param) == ADDR_EXPR)
                cur_param = TREE_OPERAND (cur_param, 0);
              else
                cur_param = 0;

              continue;
            }
          if (TREE_CODE (cur_type) != ERROR_MARK)
            {
              sprintf (message,
                       "format argument is not a %s (arg %d)",
                       ((fci->pointer_count + aflag == 1)
                        ? "pointer" : "pointer to a pointer"),
                       arg_num);
              warning (message);
            }
          break;
        }

      /* See if this is an attempt to write into a const type with
         scanf.  */
      if (info->is_scan && i == fci->pointer_count + aflag
          && wanted_type != 0
          && TREE_CODE (cur_type) != ERROR_MARK
          && (TYPE_READONLY (cur_type)
              || (cur_param != 0
                  && (TREE_CODE_CLASS (TREE_CODE (cur_param)) == 'c'
                      || (TREE_CODE_CLASS (TREE_CODE (cur_param)) == 'd'
                          && TREE_READONLY (cur_param))))))
        {
          sprintf (message, "writing into constant object (arg %d)", arg_num);
          warning (message);
        }

      /* Check the type of the "real" argument, if there's a type we want.  */
      if (i == fci->pointer_count + aflag && wanted_type != 0
          && TREE_CODE (cur_type) != ERROR_MARK
          && wanted_type != TYPE_MAIN_VARIANT (cur_type)
          /* If we want `void *', allow any pointer type.
             (Anything else would already have got a warning.)  */
          && ! (wanted_type == void_type_node
                && fci->pointer_count > 0)
          /* Don't warn about differences merely in signedness.  */
          && !(TREE_CODE (wanted_type) == INTEGER_TYPE
               && TREE_CODE (TYPE_MAIN_VARIANT (cur_type)) == INTEGER_TYPE
               && (TREE_UNSIGNED (wanted_type)
                   ? wanted_type == (cur_type = unsigned_type (cur_type))
                   : wanted_type == (cur_type = signed_type (cur_type))))
          /* Likewise, "signed char", "unsigned char" and "char" are
             equivalent but the above test won't consider them equivalent.  */
          && ! (wanted_type == char_type_node
                && (TYPE_MAIN_VARIANT (cur_type) == signed_char_type_node
                    || TYPE_MAIN_VARIANT (cur_type) == unsigned_char_type_node)))
        {
          register char *this;
          register char *that;

          this = IDENTIFIER_POINTER (DECL_NAME (TYPE_NAME (wanted_type)));
          that = 0;
          if (TREE_CODE (cur_type) != ERROR_MARK
              && TYPE_NAME (cur_type) != 0
              && TREE_CODE (cur_type) != INTEGER_TYPE
              && !(TREE_CODE (cur_type) == POINTER_TYPE
                   && TREE_CODE (TREE_TYPE (cur_type)) == INTEGER_TYPE))
            {
              if (TREE_CODE (TYPE_NAME (cur_type)) == TYPE_DECL
                  && DECL_NAME (TYPE_NAME (cur_type)) != 0)
                that = IDENTIFIER_POINTER (DECL_NAME (TYPE_NAME (cur_type)));
              else
                that = IDENTIFIER_POINTER (TYPE_NAME (cur_type));
            }

          /* A nameless type can't possibly match what the format wants.
             So there will be a warning for it.
             Make up a string to describe vaguely what it is.  */
          if (that == 0)
            {
              if (TREE_CODE (cur_type) == POINTER_TYPE)
                that = "pointer";
              else
                that = "different type";
            }

          /* Make the warning better in case of mismatch of int vs long.  */
          if (TREE_CODE (cur_type) == INTEGER_TYPE
              && TREE_CODE (wanted_type) == INTEGER_TYPE
              && TYPE_PRECISION (cur_type) == TYPE_PRECISION (wanted_type)
              && TYPE_NAME (cur_type) != 0
              && TREE_CODE (TYPE_NAME (cur_type)) == TYPE_DECL)
            that = IDENTIFIER_POINTER (DECL_NAME (TYPE_NAME (cur_type)));

          if (strcmp (this, that) != 0)
            {
              sprintf (message, "%s format, %s arg (arg %d)",
                        this, that, arg_num);
              warning (message);
            }
        }
    }
}

/* Print a warning if a constant expression had overflow in folding.
   Invoke this function on every expression that the language
   requires to be a constant expression.
   Note the ANSI C standard says it is erroneous for a
   constant expression to overflow.  */

void
constant_expression_warning (value)
     tree value;
{
  if ((TREE_CODE (value) == INTEGER_CST || TREE_CODE (value) == REAL_CST
       || TREE_CODE (value) == COMPLEX_CST)
      && TREE_CONSTANT_OVERFLOW (value) && pedantic)
    pedwarn ("overflow in constant expression");
}

/* Print a warning if an expression had overflow in folding.
   Invoke this function on every expression that
   (1) appears in the source code, and
   (2) might be a constant expression that overflowed, and
   (3) is not already checked by convert_and_check;
   however, do not invoke this function on operands of explicit casts.  */

void
overflow_warning (value)
     tree value;
{
  if ((TREE_CODE (value) == INTEGER_CST
       || (TREE_CODE (value) == COMPLEX_CST
           && TREE_CODE (TREE_REALPART (value)) == INTEGER_CST))
      && TREE_OVERFLOW (value))
    {
      TREE_OVERFLOW (value) = 0;
      if (skip_evaluation == 0)
        warning ("integer overflow in expression");
    }
  else if ((TREE_CODE (value) == REAL_CST
            || (TREE_CODE (value) == COMPLEX_CST
                && TREE_CODE (TREE_REALPART (value)) == REAL_CST))
           && TREE_OVERFLOW (value))
    {
      TREE_OVERFLOW (value) = 0;
      if (skip_evaluation == 0)
        warning ("floating point overflow in expression");
    }
}

/* Print a warning if a large constant is truncated to unsigned,
   or if -Wconversion is used and a constant < 0 is converted to unsigned.
   Invoke this function on every expression that might be implicitly
   converted to an unsigned type.  */

void
unsigned_conversion_warning (result, operand)
     tree result, operand;
{
  if (TREE_CODE (operand) == INTEGER_CST
      && TREE_CODE (TREE_TYPE (result)) == INTEGER_TYPE
      && TREE_UNSIGNED (TREE_TYPE (result))
      && skip_evaluation == 0
      && !int_fits_type_p (operand, TREE_TYPE (result)))
    {
      if (!int_fits_type_p (operand, signed_type (TREE_TYPE (result))))
        /* This detects cases like converting -129 or 256 to unsigned char.  */
        warning ("large integer implicitly truncated to unsigned type");
      else if (warn_conversion)
        warning ("negative integer implicitly converted to unsigned type");
    }
}

/* Convert EXPR to TYPE, warning about conversion problems with constants.
   Invoke this function on every expression that is converted implicitly,
   i.e. because of language rules and not because of an explicit cast.  */

tree
convert_and_check (type, expr)
     tree type, expr;
{
  tree t = convert (type, expr);
  if (TREE_CODE (t) == INTEGER_CST)
    {
#ifdef GPC
      /* Do some trivial range checks.
       */
      if (TREE_CODE (expr) == INTEGER_CST
          && ((TYPE_MIN_VALUE (type)
               && TREE_CODE (TYPE_MIN_VALUE (type)) == INTEGER_CST
               && int_cst_lt_always (expr, TYPE_MIN_VALUE (type)))
              || (TYPE_MAX_VALUE (type)
                  && TREE_CODE (TYPE_MAX_VALUE (type)) == INTEGER_CST
                  && int_cst_lt_always (TYPE_MAX_VALUE (type), expr))))
        {
          error ("constant out of range");
          t = integer_zero_node;
        }
#endif /* GPC */
      if (TREE_OVERFLOW (t))
        {
          TREE_OVERFLOW (t) = 0;

          /* Do not diagnose overflow in a constant expression merely
             because a conversion overflowed.  */
          TREE_CONSTANT_OVERFLOW (t) = TREE_CONSTANT_OVERFLOW (expr);

          /* No warning for converting 0x80000000 to int.  */
          if (!(TREE_UNSIGNED (type) < TREE_UNSIGNED (TREE_TYPE (expr))
                && TREE_CODE (TREE_TYPE (expr)) == INTEGER_TYPE
                && TYPE_PRECISION (type) == TYPE_PRECISION (TREE_TYPE (expr))))
            /* If EXPR fits in the unsigned version of TYPE,
               don't warn unless pedantic.  */
            if ((pedantic
                 || TREE_UNSIGNED (type)
                 || ! int_fits_type_p (expr, unsigned_type (type)))
                && skip_evaluation == 0)
                warning ("overflow in implicit constant conversion");
        }
      else
        unsigned_conversion_warning (t, expr);
    }
  return t;
}

void
c_expand_expr_stmt (expr)
     tree expr;
{
  /* Do default conversion if safe and possibly important,
     in case within ({...}).  */
  if ((TREE_CODE (TREE_TYPE (expr)) == ARRAY_TYPE && lvalue_p (expr))
      || TREE_CODE (TREE_TYPE (expr)) == FUNCTION_TYPE)
    expr = default_conversion (expr);

  if (TREE_TYPE (expr) != error_mark_node
      && TYPE_SIZE (TREE_TYPE (expr)) == 0
      && TREE_CODE (TREE_TYPE (expr)) != ARRAY_TYPE)
    error ("expression statement has incomplete type");

  expand_expr_stmt (expr);
}

/* Validate the expression after `case' and apply default promotions.  */

tree
check_case_value (value)
     tree value;
{
  if (value == NULL_TREE)
    return value;

  /* Strip NON_LVALUE_EXPRs since we aren't using as an lvalue.  */
  STRIP_TYPE_NOPS (value);

  if (TREE_CODE (value) != INTEGER_CST
      && value != error_mark_node)
    {
      error ("case label does not reduce to an integer constant");
      value = error_mark_node;
    }
  else
    /* Promote char or short to int.  */
    value = default_conversion (value);

  constant_expression_warning (value);

  return value;
}

/* Return an integer type with BITS bits of precision,
   that is unsigned if UNSIGNEDP is nonzero, otherwise signed.  */

tree
type_for_size (bits, unsignedp)
     unsigned bits;
     int unsignedp;
{
  if (bits == TYPE_PRECISION (integer_type_node))
    return unsignedp ? unsigned_type_node : integer_type_node;

  if (bits == TYPE_PRECISION (signed_char_type_node))
    return unsignedp ? unsigned_char_type_node : signed_char_type_node;

  if (bits == TYPE_PRECISION (short_integer_type_node))
    return unsignedp ? short_unsigned_type_node : short_integer_type_node;

  if (bits == TYPE_PRECISION (long_integer_type_node))
    return unsignedp ? long_unsigned_type_node : long_integer_type_node;

  if (bits == TYPE_PRECISION (long_long_integer_type_node))
    return (unsignedp ? long_long_unsigned_type_node
            : long_long_integer_type_node);

  if (bits <= TYPE_PRECISION (intQI_type_node))
    return unsignedp ? unsigned_intQI_type_node : intQI_type_node;

  if (bits <= TYPE_PRECISION (intHI_type_node))
    return unsignedp ? unsigned_intHI_type_node : intHI_type_node;

  if (bits <= TYPE_PRECISION (intSI_type_node))
    return unsignedp ? unsigned_intSI_type_node : intSI_type_node;

  if (bits <= TYPE_PRECISION (intDI_type_node))
    return unsignedp ? unsigned_intDI_type_node : intDI_type_node;

  return 0;
}

/* Return a data type that has machine mode MODE.
   If the mode is an integer,
   then UNSIGNEDP selects between signed and unsigned types.  */

tree
type_for_mode (mode, unsignedp)
     enum machine_mode mode;
     int unsignedp;
{
  if (mode == TYPE_MODE (integer_type_node))
    return unsignedp ? unsigned_type_node : integer_type_node;

  if (mode == TYPE_MODE (signed_char_type_node))
    return unsignedp ? unsigned_char_type_node : signed_char_type_node;

  if (mode == TYPE_MODE (short_integer_type_node))
    return unsignedp ? short_unsigned_type_node : short_integer_type_node;

  if (mode == TYPE_MODE (long_integer_type_node))
    return unsignedp ? long_unsigned_type_node : long_integer_type_node;

  if (mode == TYPE_MODE (long_long_integer_type_node))
    return unsignedp ? long_long_unsigned_type_node : long_long_integer_type_node;

  if (mode == TYPE_MODE (intQI_type_node))
    return unsignedp ? unsigned_intQI_type_node : intQI_type_node;

  if (mode == TYPE_MODE (intHI_type_node))
    return unsignedp ? unsigned_intHI_type_node : intHI_type_node;

  if (mode == TYPE_MODE (intSI_type_node))
    return unsignedp ? unsigned_intSI_type_node : intSI_type_node;

  if (mode == TYPE_MODE (intDI_type_node))
    return unsignedp ? unsigned_intDI_type_node : intDI_type_node;

  if (mode == TYPE_MODE (float_type_node))
    return float_type_node;

  if (mode == TYPE_MODE (double_type_node))
    return double_type_node;

  if (mode == TYPE_MODE (long_double_type_node))
    return long_double_type_node;

  if (mode == TYPE_MODE (build_pointer_type (char_type_node)))
    return build_pointer_type (char_type_node);

  if (mode == TYPE_MODE (build_pointer_type (integer_type_node)))
    return build_pointer_type (integer_type_node);

  return 0;
}

/* Return the minimum number of bits needed to represent VALUE in a
   signed or unsigned type, UNSIGNEDP says which.  */

int
min_precision (value, unsignedp)
     tree value;
     int unsignedp;
{
  int log;

  /* If the value is negative, compute its negative minus 1.  The latter
     adjustment is because the absolute value of the largest negative value
     is one larger than the largest positive value.  This is equivalent to
     a bit-wise negation, so use that operation instead.  */

  if (tree_int_cst_sgn (value) < 0)
    value = fold (build1 (BIT_NOT_EXPR, TREE_TYPE (value), value));

  /* Return the number of bits needed, taking into account the fact
     that we need one more bit for a signed than unsigned type.  */

  if (integer_zerop (value))
    log = 0;
  else if (TREE_INT_CST_HIGH (value) != 0)
    log = HOST_BITS_PER_WIDE_INT + floor_log2 (TREE_INT_CST_HIGH (value));
  else
    log = floor_log2 (TREE_INT_CST_LOW (value));

  return log + 1 + ! unsignedp;
}

/* Print an error message for invalid operands to arith operation CODE.
   NOP_EXPR is used as a special case (see truthvalue_conversion).  */

void
binary_op_error (code)
     enum tree_code code;
{
  register char *opname;

  switch (code)
    {
    case NOP_EXPR:
      error ("invalid truth-value expression");
      return;

    case PLUS_EXPR:
      opname = "+"; break;
    case MINUS_EXPR:
      opname = "-"; break;
    case MULT_EXPR:
      opname = "*"; break;
#ifndef GPC
    case MAX_EXPR:
      opname = "max"; break;
    case MIN_EXPR:
      opname = "min"; break;
    case EQ_EXPR:
      opname = "=="; break;
    case NE_EXPR:
      opname = "!="; break;
#endif /* not GPC */
    case LE_EXPR:
      opname = "<="; break;
    case GE_EXPR:
      opname = ">="; break;
    case LT_EXPR:
      opname = "<"; break;
    case GT_EXPR:
      opname = ">"; break;
#ifndef GPC
    case LSHIFT_EXPR:
      opname = "<<"; break;
    case RSHIFT_EXPR:
      opname = ">>"; break;
    case TRUNC_MOD_EXPR:
    case FLOOR_MOD_EXPR:
      opname = "%"; break;
    case TRUNC_DIV_EXPR:
    case FLOOR_DIV_EXPR:
      opname = "/"; break;
    case BIT_AND_EXPR:
      opname = "&"; break;
    case BIT_IOR_EXPR:
      opname = "|"; break;
    case TRUTH_ANDIF_EXPR:
      opname = "&&"; break;
    case TRUTH_ORIF_EXPR:
      opname = "||"; break;
    case BIT_XOR_EXPR:
      opname = "^"; break;
    case LROTATE_EXPR:
    case RROTATE_EXPR:
      opname = "rotate"; break;
#else /* GPC */
    /* Use these instead for Pascal.
     */
    case EQ_EXPR:
      opname = "="; break;
    case NE_EXPR:
      opname = "<>"; break;
    case TRUNC_MOD_EXPR:
    case FLOOR_MOD_EXPR:
      opname = "`mod'"; break;
    case TRUNC_DIV_EXPR:
    case FLOOR_DIV_EXPR:
      opname = "`div'"; break;
    case BIT_AND_EXPR:
      opname = "set intersection"; break;
    case BIT_IOR_EXPR:
      opname = "set union"; break;
    case TRUTH_ANDIF_EXPR:
      opname = "`and_then'"; break;
    case TRUTH_ORIF_EXPR:
      opname = "`or_else'"; break;
    case BIT_XOR_EXPR:
      opname = "set symmetric difference"; break;
    case BIT_ANDTC_EXPR:
      opname = "set difference"; break;
    case IN_EXPR:
      opname = "`in'"; break;
    case TRUTH_AND_EXPR:
      opname = "`and'"; break;
    case TRUTH_OR_EXPR:
      opname = "`or'"; break;
    case RDIV_EXPR:
      opname = "/"; break;
    case LSHIFT_EXPR:
      opname = "`shl'"; break;
    case RSHIFT_EXPR:
      opname = "`shr'"; break;
#endif /* GPC */
    default:
      opname = "unknown"; break;
    }
  error ("invalid operands to binary %s", opname);
}

#ifdef GPC

/* Subroutine of get_operator_identifier():
 *
 * Return an IDENTIFIER_NODE representing the name of
 * the type TYPE, if one can be figured out anyhow,
 * otherwise return NULL_TREE.
 */
static tree
get_type_name (type)
     tree type;
{
  if (! type)
    return NULL_TREE;

  if (TYPE_NAME (type))
    type = TYPE_NAME (type);
  if (TREE_CODE (type) == IDENTIFIER_NODE)
    return type;
  else if (TREE_CODE (type) == TYPE_DECL && DECL_NAME (type))
    return DECL_NAME (type);
  else
    {
      /* Let's do the same for TYPE_MAIN_VARIANT (type).
       */
      type = TYPE_MAIN_VARIANT (type);
      if (TYPE_NAME (type))
        type = TYPE_NAME (type);
      else if (TREE_CODE (type) == REFERENCE_TYPE
               && TYPE_NAME (TREE_TYPE (type)))
        type = TYPE_NAME (TREE_TYPE (type));

      if (TREE_CODE (type) == IDENTIFIER_NODE)
        return type;
      else if (TREE_CODE (type) == TYPE_DECL && DECL_NAME (type))
        return DECL_NAME (type);
      else
        {
          /* See if we get a unique name when scanning
           * all variants of this type.
           */
          tree name = NULL_TREE;
          type = TYPE_NEXT_VARIANT (type);
          while (type)
            {
              tree t = type;
              if (TYPE_NAME (t))
                t = TYPE_NAME (t);
              else if (TREE_CODE (type) == REFERENCE_TYPE
                       && TYPE_NAME (TREE_TYPE (type)))
                type = TYPE_NAME (TREE_TYPE (type));

              if (TREE_CODE (t) == IDENTIFIER_NODE)
                {
                  if (name && name != t)
                    return NULL_TREE;
                  else
                    name = t;
                }
              else if (TREE_CODE (t) == TYPE_DECL && DECL_NAME (t))
                {
                  if (name && name != DECL_NAME (t))
                    return NULL_TREE;
                  else
                    name = DECL_NAME (t);
                }
              type = TYPE_NEXT_VARIANT (type);
            }
          return name;
        }
    }
}

/* Return an identifier_node for the assembler-name
 * of a user-defined operator.  Both args may be
 * expression, type or type_decl nodes.
 */
tree
get_operator_identifier (op_name, arg1, arg2)
     char *op_name;
     tree arg1, arg2;
{
  if (arg1 == error_mark_node || arg2 == error_mark_node)
    return NULL_TREE;

  if (TREE_CODE_CLASS (TREE_CODE (arg1)) != 't')
    {
      if (TREE_CODE (arg1) == TREE_LIST)
        arg1 = TREE_VALUE (arg1);
      else
        arg1 = TREE_TYPE (arg1);
    }

  if (TREE_CODE_CLASS (TREE_CODE (arg2)) != 't')
    {
      if (TREE_CODE (arg2) == TREE_LIST)
        arg2 = TREE_VALUE (arg2);
      else
        arg2 = TREE_TYPE (arg2);
    }

  arg1 = get_type_name (arg1);
  arg2 = get_type_name (arg2);

  if (! arg1 || ! arg2)
    return NULL_TREE;
  else
    {
      char *temp = concat (op_name, "_", IDENTIFIER_POINTER (arg1), "_",
                           IDENTIFIER_POINTER (arg2), NULL_PTR);
      tree result = get_identifier (temp);
      free (temp);
      return result;
    }
}

/* Convert the operations for sets to their corresponding bitwise
 * boolean operators.  Other that sets, do some checks and call
 * build_binary_op with the supplied args (no conversion).
 *
 * @@@ Here is a great chance to optimize constant sets (later).
 *
 * If the arguments are constructors, this is a good place
 * to convert them to sets since we know how big the other
 * set is we are operating with.
 */
tree
build_pascal_binary_op (code, exp1, exp2)
     enum tree_code code;
     tree exp1;
     tree exp2;
{
  enum tree_code t1 = TREE_CODE (exp1);
  enum tree_code t2 = TREE_CODE (exp2);
  char *op_name;
  tree result;

  /* Check for String constants being Chars.
   */
  if (t1 == STRING_CST && TREE_STRING_LENGTH (exp1) == 2)
    {
      exp1 = string_may_be_char (exp1);
      t1 = TREE_CODE (exp1);
    }
  if (t2 == STRING_CST && TREE_STRING_LENGTH (exp2) == 2)
    {
      exp2 = string_may_be_char (exp2);
      t2 = TREE_CODE (exp2);
    }

  /* INTEGER_CSTs are of type `LongestCard', usually.
   * Convert them to something smaller here.
   *
   * @@@@ Overloaded operators must be re-implemented!
   * We must keep a list of operators, try to apply them using
   * usual parameter conversions, trigger an error if it is
   * ambiguous, use the default operator if there is no
   * overloaded one.  The same mechanism will work for
   * overloaded functions.
   */
  if (TREE_CODE (TREE_TYPE (exp1)) == INTEGER_TYPE
      && TREE_CODE (TREE_TYPE (exp2)) == INTEGER_TYPE
      && code != TRUNC_MOD_EXPR)
    {
      tree common_ordinal_type = select_integer_type (exp1, exp2, code);
      exp1 = convert (common_ordinal_type, exp1);
      exp2 = convert (common_ordinal_type, exp2);
    }

  /* Look if this is a user-defined operator
   * which must be converted to a function call
   */
  switch (code)
    {
      case PLUS_EXPR:
        op_name = "plus";
        break;
      case MINUS_EXPR:
        op_name = "minus";
        break;
      case MULT_EXPR:
        op_name = "mult";
        break;
      case RDIV_EXPR:
        op_name = "rdiv";
        break;
      case TRUNC_DIV_EXPR:
        op_name = "div";
        break;
      case TRUNC_MOD_EXPR:
        op_name = "mod";
        break;
      case EXPON_EXPR:
        op_name = "pow";
        break;
      case IN_EXPR:
        op_name = "in";
        break;
      case LT_EXPR:
        op_name = "lt";
        break;
      case EQ_EXPR:
        op_name = "eq";
        break;
      case GT_EXPR:
        op_name = "gt";
        break;
      case NE_EXPR:
        op_name = "neq";
        break;
      case GE_EXPR:
        op_name = "gte";
        break;
      case LE_EXPR:
        op_name = "lte";
        break;
      case TRUTH_OR_EXPR:
      case TRUTH_ORIF_EXPR:
      case BIT_IOR_EXPR:
        op_name = "or";
        break;
      case TRUTH_AND_EXPR:
      case TRUTH_ANDIF_EXPR:
      case BIT_AND_EXPR:
        op_name = "and";
        break;
      case TRUTH_XOR_EXPR:
      case BIT_XOR_EXPR:
        op_name = "xor";
        break;
      case LSHIFT_EXPR:
        op_name = "shl";
        break;
      case RSHIFT_EXPR:
        op_name = "shr";
        break;
      default:
        op_name = NULL;
    }
  if (op_name)
    {
      tree func, e1 = exp1, e2 = exp2;

      /* For "fresh" constants use the most basic type,
       * e.g. `Real', not `LongReal'.
       * @@@@ This used to convert *all* real-type expressions to
       * `double', but I don't remember why.  I have changed this,
       * hoping that this does not break anything.  PG
       */
      if (TREE_CODE (e1) == INTEGER_CST
          && PASCAL_TREE_FRESH_INT_CST (e1))
        e1 = integer_type_node;
      else if (TREE_CODE (e1) == REAL_CST)
        e1 = double_type_node;
      if (TREE_CODE (e2) == INTEGER_CST
          && PASCAL_TREE_FRESH_INT_CST (e2))
        e2 = integer_type_node;
      else if (TREE_CODE (e2) == REAL_CST)
        e2 = double_type_node;

      func = get_operator_identifier (op_name, e1, e2);
      if (func)
        func = lookup_name (func);
      if (func && TREE_CODE (func) == FUNCTION_DECL)
        {
          tree arg = build_tree_list (NULL_TREE, exp1);
          arg = chainon (arg, build_tree_list (NULL_TREE, exp2));
          return build_function_call (func, arg);
        }
      else if (TREE_CODE (exp1) == INTEGER_CST
               || TREE_CODE (exp1) == REAL_CST
               || TREE_CODE (exp2) == INTEGER_CST
               || TREE_CODE (exp2) == REAL_CST)
        {
          /* Retry with the "longest" versions of the types.
           */
          e1 = exp1;
          e2 = exp2;
          if (TREE_CODE (e1) == INTEGER_CST
              && PASCAL_TREE_FRESH_INT_CST (e1))
            e1 = long_long_integer_type_node;
          else if (TREE_CODE (e1) == REAL_CST)
            e1 = long_double_type_node;
          if (TREE_CODE (e2) == INTEGER_CST
              && PASCAL_TREE_FRESH_INT_CST (e2))
            e2 = long_long_integer_type_node;
          else if (TREE_CODE (e2) == REAL_CST)
            e2 = long_double_type_node;

          func = get_operator_identifier (op_name, e1, e2);
          if (func)
            func = lookup_name (func);
          if (func && TREE_CODE (func) == FUNCTION_DECL)
            {
              tree arg = build_tree_list (NULL_TREE, exp1);
              arg = chainon (arg, build_tree_list (NULL_TREE, exp2));
              return build_function_call (func, arg);
            }
        }
    }

  /* Call functions.
   */
  if (TREE_CODE (exp1) == FUNCTION_DECL)
    exp1 = probably_call_function (exp1);
  if (TREE_CODE (exp2) == FUNCTION_DECL)
    exp2 = probably_call_function (exp2);

  /* Convert set constructors to sets.
   */
  if (t1 == CONSTRUCTOR)
    exp1 = construct_set (exp1, NULL_TREE, 1);
  t1 = TREE_CODE (TREE_TYPE (exp1));
  if (t2 == CONSTRUCTOR)
    exp2 = construct_set (exp2, NULL_TREE, 1);
  t2 = TREE_CODE (TREE_TYPE (exp2));

  if (t1 == ERROR_MARK || t2 == ERROR_MARK)
    return error_mark_node;

  if (PASCAL_TYPE_RESTRICTED (TREE_TYPE (exp1))
      || PASCAL_TYPE_RESTRICTED (TREE_TYPE (exp2)))
    error ("invalid operation with a restricted type object");

  if (code == EXPON_EXPR)
    return build_rts_call ((t2 == INTEGER_TYPE) ? r_POW : r_EXPON,
                            chainon (build_tree_list (NULL_TREE, exp1),
                                     build_tree_list (NULL_TREE, exp2)));

  /* Handle IN_EXPR, reverse the arguments for expr.c.
   */
  if (code == IN_EXPR)
    {
      if (TREE_CODE (exp2) == CONSTRUCTOR)
        {
          /* Optimize `foo in [ a, b..c ]' to become
           * `( foo = a ) or ( ( foo >= b ) and ( foo <= c ) )'.
           */
          tree elem, exp, result = NULL_TREE;
          exp = make_new_variable ("in_expr", TREE_TYPE (exp1));
          expand_expr_stmt (build_modify_expr (exp, NOP_EXPR, exp1));
          for (elem = CONSTRUCTOR_ELTS (exp2); elem; elem = TREE_CHAIN (elem))
            {
              tree min_c = TREE_PURPOSE (elem);
              tree max_c = TREE_VALUE (elem);
              tree condition;

              STRIP_NOPS (min_c);
              STRIP_NOPS (max_c);

              /* Should not happen - handled in `build_set_constructor()'.
               */
              if (! max_c)
                {
                  warning ("Internal GPC problem: old-style set constructor");
                  max_c = min_c;
                }

              if (comptypes (TREE_TYPE (exp1), TREE_TYPE (min_c)) == 0
                  || comptypes (TREE_TYPE (exp1), TREE_TYPE (max_c)) == 0)
                binary_op_error (code);

              if (max_c == min_c)
                {
                  /* This is a single element, not a range.
                   */
                  condition = build_pascal_binary_op (EQ_EXPR,
                                                      exp, min_c);
                }
              else
                {
                  /* This is a range.
                   *
                   * Avoid warning in the case
                   * "<unsigned value> in [ 0..<whatever> ]".
                   */
                  if (integer_zerop (min_c)
                      && TREE_UNSIGNED (TREE_TYPE (exp)))
                    condition = build_pascal_binary_op (LE_EXPR, exp, max_c);
                  else
                    {
                      condition = build_pascal_binary_op (GE_EXPR,
                                                          exp, min_c);
                      condition = build_pascal_binary_op (TRUTH_ANDIF_EXPR,
                                      condition,
                                      build_pascal_binary_op (LE_EXPR,
                                                              exp, max_c));
                    }
                }
              if (result)
                result = build_pascal_binary_op (TRUTH_ORIF_EXPR,
                                                 result, condition);
              else
                result = condition;
            }
          if (result)
            return result;
          else
            {
              /* `foo in [ ]' (empty set) is always `False'.
               */
              return boolean_false_node;
            }
        }
      else
        {
          /* No optimization possible.
           * Create an RTS call.
           */
          tree params = build_tree_list (NULL_TREE, build_pascal_unary_op (ADDR_EXPR, exp2, 0));
          params = chainon (params,
                            build_tree_list (NULL_TREE,
                                             TYPE_MIN_VALUE (TYPE_DOMAIN (TREE_TYPE (exp2)))));
          params = chainon (params,
                            build_tree_list (NULL_TREE,
                                             TYPE_MAX_VALUE (TYPE_DOMAIN (TREE_TYPE (exp2)))));
          params = chainon (params,
                            build_tree_list (NULL_TREE,
                                             convert (integer_type_node, exp1)));
          return rts_call (set_in, boolean_type_node, NULL_TREE, params);
        }
    }

  /* All string and char types are compatible in Extended Pascal.
   */
  if ((is_string_type (exp1, 1) || t1 == CHAR_TYPE)
      && (is_string_type (exp2, 1) || t2 == CHAR_TYPE)
      && (comptypes (TREE_TYPE (exp1), TREE_TYPE (exp2)) != 1
          || code == PLUS_EXPR
          || is_variable_string_type (TREE_TYPE (exp1))
          || is_variable_string_type (TREE_TYPE (exp2))))
    {
      int rts_code = 0;
      switch (code)
        {
          case EQ_EXPR: rts_code = '='; break;
          case NE_EXPR: rts_code = LEX_NE; break;
          case LT_EXPR: rts_code = '<'; break;
          case LE_EXPR: rts_code = LEX_LE; break;
          case GT_EXPR: rts_code = '>'; break;
          case GE_EXPR: rts_code = LEX_GE; break;
          default: /* do nothing */;
        }
      if (rts_code)
        {
          if (t1 != t2 && PEDANTIC (E_O_U_B_D_PASCAL))
            {
              pedwarn ("ISO 7185 Pascal does not allow comparison of");
              pedwarn (" different string and char types");
            }
          return build_rts_call (rts_code,
                                 chainon (build_tree_list (NULL_TREE, exp1),
                                          build_tree_list (NULL_TREE, exp2)));
        }

      /* All string catenations are handled here.
       */
      if (code == PLUS_EXPR)
        {
          if ((TREE_CODE (exp1) == STRING_CST
               || TREE_CODE (exp1) == INTEGER_CST)
              && (TREE_CODE (exp2) == STRING_CST
                  || TREE_CODE (exp2) == INTEGER_CST))
            {
              /* Concatenation of string constants.
               * Treat it in a special way to preserve constantness.
               *
               * But first transform char constants (back)-: to
               * string constants.
               */
              if (TREE_CODE (exp1) == INTEGER_CST)
                {
                  char buffer[2];
                  buffer[0] = TREE_INT_CST_LOW (exp1);
                  buffer[1] = 0;
                  exp1 = build_string (2, buffer);
                }
              if (TREE_CODE (exp2) == INTEGER_CST)
                {
                  char buffer[2];
                  buffer[0] = TREE_INT_CST_LOW (exp2);
                  buffer[1] = 0;
                  exp2 = build_string (2, buffer);
                }
              TREE_CHAIN (exp1) = exp2;
              return combine_strings (exp1);
            }
          else
            {
              /* Length of the combined strings */
              tree len1 = PASCAL_STRING_LENGTH (exp1);
              tree len2 = PASCAL_STRING_LENGTH (exp2);
              tree length = build_binary_op (PLUS_EXPR, len1, len2, 0);

              /* Create a new string object */
              tree nstr = alloca_string (length);
              tree sval = PASCAL_STRING_VALUE (nstr);
              tree str_addr = build_unary_op (ADDR_EXPR, sval, 0);

              /* Assign the first string to the new object */
              if (t1 == CHAR_TYPE)
                expand_expr_stmt (
                  build_modify_expr (
                    build_array_ref (sval, integer_one_node),
                    NOP_EXPR,
                    exp1));
              else
                expand_expr_stmt (
                  build_string_move (
                    str_addr, build1 (ADDR_EXPR,
                                      string_type_node,
                                      PASCAL_STRING_VALUE (exp1)),
                    len1));

              /* Catenate the second string to the first */
              if (t2 == CHAR_TYPE)
                expand_expr_stmt (
                  build_modify_expr (
                    build_array_ref (sval,
                                     build_binary_op (PLUS_EXPR, len1,
                                                      integer_one_node, 0)),
                                     NOP_EXPR, exp2));
              else
                expand_expr_stmt (
                  build_string_move (
                    build (PLUS_EXPR, string_type_node, str_addr, len1, 0),
                    build1 (ADDR_EXPR, string_type_node,
                            PASCAL_STRING_VALUE (exp2)), len2));

              /* Save the combined length of strings */
              expand_expr_stmt
                (build_modify_expr (PASCAL_STRING_LENGTH (nstr),
                                    NOP_EXPR, length));
              return build1 (NON_LVALUE_EXPR, TREE_TYPE (nstr), nstr);
            }
        }
    }

  if ((t1 == SET_TYPE || t1 == CONSTRUCTOR)
      && (t2 == SET_TYPE || t2 == CONSTRUCTOR))
    {
      int rts_id;
      int negate = 0, empty1 = 0, empty2 = 0;
      tree params, result_type, temp1, temp2;
      if (! comptypes (TREE_TYPE (exp1), TREE_TYPE (exp2)))
        error ("operation on incompatible sets");
      if (TREE_CODE (TREE_TYPE (TREE_TYPE (exp1))) == VOID_TYPE)
        empty1 = 1;
      if (TREE_CODE (TREE_TYPE (TREE_TYPE (exp2))) == VOID_TYPE)
        empty2 = 1;
      switch (code)
        {
          case PLUS_EXPR:
            if (empty1)
              return exp2;
            if (empty2)
              return exp1;
            rts_id = set_union;
            result_type = TREE_TYPE (exp1);
            break;
          case MINUS_EXPR:
            if (empty1 || empty2)
              return exp1;
            rts_id = set_diff;
            result_type = TREE_TYPE (exp1);
            break;
          case MULT_EXPR:
            if (empty1)
              return exp1;
            if (empty2)
              return exp2;
            rts_id = set_intersection;
            result_type = TREE_TYPE (exp1);
            break;
          case BIT_XOR_EXPR:
            if (PEDANTIC (E_O_PASCAL))
              pedwarn ("symmetric set difference is an ISO 10206 Extended Pascal extension");
            if (empty1)
              return exp2;
            if (empty2)
              return exp1;
            rts_id = set_symdiff;
            result_type = TREE_TYPE (exp1);
            break;
          case EQ_EXPR:
            if (empty1 && empty2)
              return boolean_true_node;
            rts_id = set_equal;
            result_type = boolean_type_node;
            break;
          case NE_EXPR:
            if (empty1 && empty2)
              return boolean_false_node;
            rts_id = set_equal;
            result_type = boolean_type_node;
            negate = 1;
            break;
          case LT_EXPR:
            if (empty2)
              return boolean_false_node;
            rts_id = set_less;
            result_type = boolean_type_node;
            break;
          case GT_EXPR:
            if (empty1)
              return boolean_false_node;
            rts_id = set_less;
            result_type = boolean_type_node;
            temp1 = exp1;
            exp1 = exp2;
            exp2 = temp1;
            break;
          case LE_EXPR:
            if (empty1)
              return boolean_true_node;
            rts_id = set_le;
            result_type = boolean_type_node;
            break;
          case GE_EXPR:
            if (empty2)
              return boolean_true_node;
            rts_id = set_le;
            result_type = boolean_type_node;
            temp1 = exp1;
            exp1 = exp2;
            exp2 = temp1;
            break;
          default:
            /* No other set type operations are defined.
             */
            binary_op_error (code);
            return error_mark_node;
        }
      if (! mark_addressable (exp1))
        {
          temp1 = make_new_variable ("set_operand", TREE_TYPE (exp1));
          exp1 = build (COMPOUND_EXPR, TREE_TYPE (exp1),
                        assign_set (temp1, exp1),
                        temp1);
        }
      if (! mark_addressable (exp2))
        {
          temp2 = make_new_variable ("set_operand", TREE_TYPE (exp2));
          exp2 = build (COMPOUND_EXPR, TREE_TYPE (exp2),
                        assign_set (temp2, exp2),
                        temp2);
        }
      params = build_tree_list (NULL_TREE, empty1 ? null_pointer_node
                                                  : build_pascal_unary_op (ADDR_EXPR, exp1, 0));
      params = chainon (params,
                        build_tree_list (NULL_TREE,
                                         TYPE_MIN_VALUE (TYPE_DOMAIN (TREE_TYPE (exp1)))));
      params = chainon (params,
                        build_tree_list (NULL_TREE,
                                         TYPE_MAX_VALUE (TYPE_DOMAIN (TREE_TYPE (exp1)))));
      params = chainon (params,
                        build_tree_list (NULL_TREE,
                                         empty2 ? null_pointer_node
                                                : build_pascal_unary_op (ADDR_EXPR, exp2, 0)));
      params = chainon (params,
                        build_tree_list (NULL_TREE,
                                         TYPE_MIN_VALUE (TYPE_DOMAIN (TREE_TYPE (exp2)))));
      params = chainon (params,
                        build_tree_list (NULL_TREE,
                                         TYPE_MAX_VALUE (TYPE_DOMAIN (TREE_TYPE (exp2)))));
      if (TREE_CODE (result_type) == SET_TYPE)
        {
          result = make_new_variable ("set_result", result_type);
          params = chainon (params,
                            build_tree_list (NULL_TREE,
                                             build_pascal_unary_op (ADDR_EXPR, result, 0)));
          params = chainon (params,
                            build_tree_list (NULL_TREE,
                                             TYPE_MIN_VALUE (TYPE_DOMAIN (TREE_TYPE (result)))));
          params = chainon (params,
                            build_tree_list (NULL_TREE,
                                             TYPE_MAX_VALUE (TYPE_DOMAIN (TREE_TYPE (result)))));
          result = build (COMPOUND_EXPR, result_type,
                          rts_call (rts_id, void_type_node, NULL_TREE, params),
                          result);
          return result;
        }
      else
        {
          result = rts_call (rts_id, result_type, NULL_TREE, params);
          if (negate)
            result = build_pascal_unary_op (TRUTH_NOT_EXPR, result, 0);
          return result;
        }
    }

  if (! flag_extended_syntax
      && (t1 == POINTER_TYPE || t2 == POINTER_TYPE)
      && code != EQ_EXPR
      && code != NE_EXPR)
    {
      error ("only `=' and `<>' operators are allowed for pointers");
      ptrarith_inform ();
    }

  /* Check for division by zero.
   */
  switch (code)
    {
      case RDIV_EXPR:
      case TRUNC_DIV_EXPR:
        {
          if (integer_zerop (exp2))
            error ("division by zero");
          break;
        }
      case TRUNC_MOD_EXPR:
        {
          if (integer_zerop (exp2))
            error ("zero modulus");
          if (! TREE_UNSIGNED (TREE_TYPE (exp2)))
            exp2 = build_unary_op (ABS_EXPR, exp2, 0);
          if (! TREE_UNSIGNED (TREE_TYPE (exp1)))
            {
              tree condition;
              exp1 = save_expr (exp1);
              exp2 = save_expr (exp2);
              condition = build_pascal_binary_op (LT_EXPR, exp1, integer_zero_node);
              result = build (COND_EXPR, TREE_TYPE (exp1), condition,
                         build_unary_op (NEGATE_EXPR,
                           convert (TREE_TYPE (exp1),
                             build_binary_op (TRUNC_MOD_EXPR,
                               build_unary_op (NEGATE_EXPR, exp1, 0), exp2, 0)), 0),
                         convert (TREE_TYPE (exp1),
                           build_binary_op (TRUNC_MOD_EXPR, exp1, exp2, 0)));
              result = build (COMPOUND_EXPR, TREE_TYPE (result), exp2, result);
              return result;
            }
          break;
        }
      default: /* do nothing */;
    }

  result = build_binary_op (code, exp1, exp2, 0);
  switch (TREE_CODE (result))
    {
      case INTEGER_CST:
      case REAL_CST:
      case COMPLEX_CST:
        {
          if (TREE_OVERFLOW (result))
            error ("arithmetical overflow");
          break;
        }
      default: /* do nothing */;
    }
  return result;
}

tree
build_pascal_unary_op (code, xarg, noconvert)
     enum tree_code code;
     tree xarg;
     int noconvert;
{
  if (PASCAL_TYPE_RESTRICTED (TREE_TYPE (xarg)))
    error ("illegal unary operation with restricted type");

  if (TREE_CODE (xarg) == FUNCTION_DECL)
    xarg = probably_call_function (xarg);

  if (code == NEGATE_EXPR
      && TREE_CODE (TREE_TYPE (xarg)) == INTEGER_TYPE
      && TREE_UNSIGNED (TREE_TYPE (xarg)))
    {
      if (TREE_CODE (xarg) == INTEGER_CST)
        {
          /* Convert unsigned constants to signed ones here
           * and do some trivial range checking.
           */
          if (INT_CST_LT_UNSIGNED (
                  build_int_2 (0, (HOST_WIDE_INT) 1
                                  << (HOST_BITS_PER_WIDE_INT - 1)),
                  xarg))
            {
              error ("value does not fit in longest integer type");
              return integer_zero_node;
            }
          else
            {
              int fresh = PASCAL_TREE_FRESH_INT_CST (xarg);

              xarg = copy_node (xarg);
              TREE_TYPE (xarg) = long_long_integer_type_node;
              TREE_UNSIGNED (xarg) = 0;
              xarg = build_unary_op (code, xarg, 0);

              /* - 0x8000000000000000 is no overflow, although
               * 0x8000000000000000 does not fit in a signed integer type.
               */
              TREE_OVERFLOW (xarg) = 0;

              /* Note whether the type of this integer constant was guessed
               * by the lexical analzyer.
               */
              PASCAL_TREE_FRESH_INT_CST (xarg) = fresh;

              return xarg;
            }
        }
      else if (! noconvert)
        xarg = convert (select_signed_integer_type (TREE_TYPE (xarg)), xarg);
    }
  return build_unary_op (code, xarg, noconvert);
}

/* Convert a PXSC operator expression to a function call.
 * The functions are not (yet) built-in but must be supported
 * (overloaded) by the user program.
 */
tree build_pxsc_operator (op_name, arg1, arg2)
     char *op_name;
     tree arg1, arg2;
{
  /* pxsc operator expression */
  /* is converted to a function call */
  tree op_func, arg;
  if (PEDANTIC (PASCAL_SC))
    pedwarn ("overloaded and rounding operators are Pascal-SC extensions");
  op_func = lookup_name (get_operator_identifier (op_name, arg1, arg2));
  if (op_func)
    {
      arg = build_tree_list (NULL_TREE, arg1);
      arg = chainon (arg, build_tree_list (NULL_TREE, arg2));
      return build_function_call (op_func, arg);
    }
  else
    {
      error ("PXSC operators are not built-in but must be overloaded");
      return error_mark_node;
    }
}

/* Dereference a pointer.
 * This may result in a function call.
 */
tree
build_pascal_pointer_reference (pointer)
     tree pointer;
{
  tree result, fun_type;

  if (PASCAL_TYPE_RESTRICTED (TREE_TYPE (pointer)))
    error ("referencing a restricted type object is not allowed");

  if (TREE_CODE (TREE_TYPE (pointer)) == FILE_TYPE)
    result = build_buffer_ref (pointer, 2);
  else if (MAYBE_CALL_FUNCTION (pointer)
           && (fun_type = (TREE_CODE (pointer) == FUNCTION_DECL
                           ? TREE_TYPE (pointer)
                           : TREE_TYPE (TREE_TYPE (pointer))),
               TYPE_ARG_TYPES (fun_type)
               && TREE_VALUE (TYPE_ARG_TYPES (fun_type)) == void_type_node))
    {
      int function_calls = allow_function_calls ();
      result = maybe_call_function (pointer, empty_arglist);
      resume_function_calls (function_calls);
      if (TREE_CODE (pointer) == FUNCTION_DECL)  /* direct function call */
        result = build_pascal_pointer_reference (result);
    }
  else
    {
      warn_void_dereference++;
      result = build_indirect_ref (pointer, "`^'");
      warn_void_dereference--;
    }

  return result;
}

/* Return an expression for the address of FACTOR.
 * In most cases, this is an ADDR_EXPR, but it may also be a cast of a
 * reference to a pointer.
 */
tree
build_pascal_address_expression (factor)
     tree factor;
{
  /* Undo implicit schema dereferences.
   */
  if (TREE_CODE (factor) == COMPONENT_REF
      && TREE_CODE (TREE_OPERAND (factor, 1)) == FIELD_DECL
      && (DECL_NAME (TREE_OPERAND (factor, 1)) == schema_id
                        || DECL_NAME (TREE_OPERAND (factor, 1)) == string_id))
    factor = TREE_OPERAND (factor, 0);

  /* If `foo' is a procedure reference, `@foo' is a
   * type cast to a procedure pointer.
   */
  if (PASCAL_PROCEDURAL_VARIABLE (factor))
    {
      tree ftype = TREE_TYPE (TREE_TYPE (factor));
      return convert (build_pointer_type (ftype), factor);
    }
  else
    {
      /* Don't call build_pascal_unary_op() which would
       * call the function.
       */
      tree result = build_unary_op (ADDR_EXPR, factor, 0);

      /* Mark constant addresses as such (for initialization).
       */
      if (TREE_CODE (factor) == VAR_DECL
          || TREE_CODE (factor) == FUNCTION_DECL)
        TREE_CONSTANT (result) = 1;
      return result;
    }
}

#endif /* GPC */

/* Subroutine of build_binary_op, used for comparison operations.
   See if the operands have both been converted from subword integer types
   and, if so, perhaps change them both back to their original type.
   This function is also responsible for converting the two operands
   to the proper common type for comparison.

   The arguments of this function are all pointers to local variables
   of build_binary_op: OP0_PTR is &OP0, OP1_PTR is &OP1,
   RESTYPE_PTR is &RESULT_TYPE and RESCODE_PTR is &RESULTCODE.

   If this function returns nonzero, it means that the comparison has
   a constant value.  What this function returns is an expression for
   that value.  */

tree
shorten_compare (op0_ptr, op1_ptr, restype_ptr, rescode_ptr)
     tree *op0_ptr, *op1_ptr;
     tree *restype_ptr;
     enum tree_code *rescode_ptr;
{
  register tree type;
  tree op0 = *op0_ptr;
  tree op1 = *op1_ptr;
  int unsignedp0, unsignedp1;
  int real1, real2;
  tree primop0, primop1;
  enum tree_code code = *rescode_ptr;

  /* Throw away any conversions to wider types
     already present in the operands.  */

  primop0 = get_narrower (op0, &unsignedp0);
  primop1 = get_narrower (op1, &unsignedp1);

  /* Handle the case that OP0 does not *contain* a conversion
     but it *requires* conversion to FINAL_TYPE.  */

  if (op0 == primop0 && TREE_TYPE (op0) != *restype_ptr)
    unsignedp0 = TREE_UNSIGNED (TREE_TYPE (op0));
  if (op1 == primop1 && TREE_TYPE (op1) != *restype_ptr)
    unsignedp1 = TREE_UNSIGNED (TREE_TYPE (op1));

  /* If one of the operands must be floated, we cannot optimize.  */
  real1 = TREE_CODE (TREE_TYPE (primop0)) == REAL_TYPE;
  real2 = TREE_CODE (TREE_TYPE (primop1)) == REAL_TYPE;

  /* If first arg is constant, swap the args (changing operation
     so value is preserved), for canonicalization.  Don't do this if
     the second arg is 0.  */

  if (TREE_CONSTANT (primop0)
      && ! integer_zerop (primop1) && ! real_zerop (primop1))
    {
      register tree tem = primop0;
      register int temi = unsignedp0;
      primop0 = primop1;
      primop1 = tem;
      tem = op0;
      op0 = op1;
      op1 = tem;
      *op0_ptr = op0;
      *op1_ptr = op1;
      unsignedp0 = unsignedp1;
      unsignedp1 = temi;
      temi = real1;
      real1 = real2;
      real2 = temi;

      switch (code)
        {
        case LT_EXPR:
          code = GT_EXPR;
          break;
        case GT_EXPR:
          code = LT_EXPR;
          break;
        case LE_EXPR:
          code = GE_EXPR;
          break;
        case GE_EXPR:
          code = LE_EXPR;
          break;
        default:
          break;
        }
      *rescode_ptr = code;
    }

  /* If comparing an integer against a constant more bits wide,
     maybe we can deduce a value of 1 or 0 independent of the data.
     Or else truncate the constant now
     rather than extend the variable at run time.

     This is only interesting if the constant is the wider arg.
     Also, it is not safe if the constant is unsigned and the
     variable arg is signed, since in this case the variable
     would be sign-extended and then regarded as unsigned.
     Our technique fails in this case because the lowest/highest
     possible unsigned results don't follow naturally from the
     lowest/highest possible values of the variable operand.
     For just EQ_EXPR and NE_EXPR there is another technique that
     could be used: see if the constant can be faithfully represented
     in the other operand's type, by truncating it and reextending it
     and see if that preserves the constant's value.  */

  if (!real1 && !real2
      && TREE_CODE (primop1) == INTEGER_CST
      && TYPE_PRECISION (TREE_TYPE (primop0)) < TYPE_PRECISION (*restype_ptr))
    {
      int min_gt, max_gt, min_lt, max_lt;
      tree maxval, minval;
      /* 1 if comparison is nominally unsigned.  */
      int unsignedp = TREE_UNSIGNED (*restype_ptr);
      tree val;

      type = signed_or_unsigned_type (unsignedp0, TREE_TYPE (primop0));

      maxval = TYPE_MAX_VALUE (type);
      minval = TYPE_MIN_VALUE (type);

      if (unsignedp && !unsignedp0)
        *restype_ptr = signed_type (*restype_ptr);

      if (TREE_TYPE (primop1) != *restype_ptr)
        primop1 = convert (*restype_ptr, primop1);
      if (type != *restype_ptr)
        {
          minval = convert (*restype_ptr, minval);
          maxval = convert (*restype_ptr, maxval);
        }

      if (unsignedp && unsignedp0)
        {
          min_gt = INT_CST_LT_UNSIGNED (primop1, minval);
          max_gt = INT_CST_LT_UNSIGNED (primop1, maxval);
          min_lt = INT_CST_LT_UNSIGNED (minval, primop1);
          max_lt = INT_CST_LT_UNSIGNED (maxval, primop1);
        }
      else
        {
          min_gt = INT_CST_LT (primop1, minval);
          max_gt = INT_CST_LT (primop1, maxval);
          min_lt = INT_CST_LT (minval, primop1);
          max_lt = INT_CST_LT (maxval, primop1);
        }

      val = 0;
      /* This used to be a switch, but Genix compiler can't handle that.  */
      if (code == NE_EXPR)
        {
          if (max_lt || min_gt)
            val = boolean_true_node;
        }
      else if (code == EQ_EXPR)
        {
          if (max_lt || min_gt)
            val = boolean_false_node;
        }
      else if (code == LT_EXPR)
        {
          if (max_lt)
            val = boolean_true_node;
          if (!min_lt)
            val = boolean_false_node;
        }
      else if (code == GT_EXPR)
        {
          if (min_gt)
            val = boolean_true_node;
          if (!max_gt)
            val = boolean_false_node;
        }
      else if (code == LE_EXPR)
        {
          if (!max_gt)
            val = boolean_true_node;
          if (min_gt)
            val = boolean_false_node;
        }
      else if (code == GE_EXPR)
        {
          if (!min_lt)
            val = boolean_true_node;
          if (max_lt)
            val = boolean_false_node;
        }

      /* If primop0 was sign-extended and unsigned comparison specd,
         we did a signed comparison above using the signed type bounds.
         But the comparison we output must be unsigned.

         Also, for inequalities, VAL is no good; but if the signed
         comparison had *any* fixed result, it follows that the
         unsigned comparison just tests the sign in reverse
         (positive values are LE, negative ones GE).
         So we can generate an unsigned comparison
         against an extreme value of the signed type.  */

      if (unsignedp && !unsignedp0)
        {
          if (val != 0)
            switch (code)
              {
              case LT_EXPR:
              case GE_EXPR:
                primop1 = TYPE_MIN_VALUE (type);
                val = 0;
                break;

              case LE_EXPR:
              case GT_EXPR:
                primop1 = TYPE_MAX_VALUE (type);
                val = 0;
                break;

              default:
                break;
              }
          type = unsigned_type (type);
        }

#ifdef GPC
      /* Don't complain of the limited range comparisons when checking
       * `for' loop bounds.
       */
      if (! PASCAL_LOOP_CHECK (op0)
          && !max_gt && !unsignedp0 && TREE_CODE (primop0) != INTEGER_CST)
        {
          if (val == boolean_false_node)
            warning ("comparison yields always `False' due to limited range of data type");
          if (val == boolean_true_node)
            warning ("comparison yields always `True' due to limited range of data type");
        }

      if (! PASCAL_LOOP_CHECK (op0)
          && !min_lt && unsignedp0 && TREE_CODE (primop0) != INTEGER_CST)
        {
          if (val == boolean_false_node)
            warning ("comparison is always `False' due to limited range of data type");
          if (val == boolean_true_node)
            warning ("comparison is always `True' due to limited range of data type");
        }
#else /* not GPC */
      if (!max_gt && !unsignedp0 && TREE_CODE (primop0) != INTEGER_CST)
          /* This is the case of (char)x >?< 0x80, which people used to use
             expecting old C compilers to change the 0x80 into -0x80.  */
          if (val == boolean_false_node)
            warning ("comparison is always 0 due to limited range of data type");
          if (val == boolean_true_node)
            warning ("comparison is always 1 due to limited range of data type");
        }

      if (!min_lt && unsignedp0 && TREE_CODE (primop0) != INTEGER_CST)
        {
          /* This is the case of (unsigned char)x >?< -1 or < 0.  */
          if (val == boolean_false_node)
            warning ("comparison is always 0 due to limited range of data type");
          if (val == boolean_true_node)
            warning ("comparison is always 1 due to limited range of data type");
        }
#endif /* not GPC */

      if (val != 0)
        {
          /* Don't forget to evaluate PRIMOP0 if it has side effects.  */
          if (TREE_SIDE_EFFECTS (primop0))
            return build (COMPOUND_EXPR, TREE_TYPE (val), primop0, val);
          return val;
        }

      /* Value is not predetermined, but do the comparison
         in the type of the operand that is not constant.
         TYPE is already properly set.  */
    }
  else if (real1 && real2
           && (TYPE_PRECISION (TREE_TYPE (primop0))
               == TYPE_PRECISION (TREE_TYPE (primop1))))
    type = TREE_TYPE (primop0);

  /* If args' natural types are both narrower than nominal type
     and both extend in the same manner, compare them
     in the type of the wider arg.
     Otherwise must actually extend both to the nominal
     common type lest different ways of extending
     alter the result.
     (eg, (short)-1 == (unsigned short)-1  should be 0.)  */

  else if (unsignedp0 == unsignedp1 && real1 == real2
           && TYPE_PRECISION (TREE_TYPE (primop0)) < TYPE_PRECISION (*restype_ptr)
           && TYPE_PRECISION (TREE_TYPE (primop1)) < TYPE_PRECISION (*restype_ptr))
    {
      type = common_type (TREE_TYPE (primop0), TREE_TYPE (primop1));
      type = signed_or_unsigned_type (unsignedp0
                                      || TREE_UNSIGNED (*restype_ptr),
                                      type);
      /* Make sure shorter operand is extended the right way
         to match the longer operand.  */
      primop0 = convert (signed_or_unsigned_type (unsignedp0, TREE_TYPE (primop0)),
                         primop0);
      primop1 = convert (signed_or_unsigned_type (unsignedp1, TREE_TYPE (primop1)),
                         primop1);
    }
  else
    {
      /* Here we must do the comparison on the nominal type
         using the args exactly as we received them.  */
      type = *restype_ptr;
      primop0 = op0;
      primop1 = op1;

      if (!real1 && !real2 && integer_zerop (primop1)
          && TREE_UNSIGNED (*restype_ptr))
        {
          tree value = 0;
          switch (code)
            {
            case GE_EXPR:
              /* All unsigned values are >= 0, so we warn if extra warnings
                 are requested.  However, if OP0 is a constant that is
                 >= 0, the signedness of the comparison isn't an issue,
                 so suppress the warning.  */
#ifdef GPC
              if (! PASCAL_LOOP_CHECK (op0)
                  && TREE_CODE (primop0) != INTEGER_CST)
                warning ("comparison `unsigned value >= 0' yields always `True'");
#else /* not GPC */
              if (extra_warnings
                  && ! (TREE_CODE (primop0) == INTEGER_CST
                        && ! TREE_OVERFLOW (convert (signed_type (type),
                                                     primop0))))
                warning ("unsigned value >= 0 is always 1");
#endif /* not GPC */
              value = boolean_true_node;
              break;

#ifdef GPC
            case LT_EXPR:
              if (! PASCAL_LOOP_CHECK (op0)
                  && TREE_CODE (primop0) != INTEGER_CST)
                warning ("comparison `unsigned value < 0' yields always `False'");
#else /* not GPC */
              if (extra_warnings
                  && ! (TREE_CODE (primop0) == INTEGER_CST
                        && ! TREE_OVERFLOW (convert (signed_type (type),
                                                     primop0))))
                warning ("unsigned value < 0 is always 0");
#endif /* not GPC */
              value = boolean_false_node;
              break;

            default:
              break;
            }

          if (value != 0)
            {
              /* Don't forget to evaluate PRIMOP0 if it has side effects.  */
              if (TREE_SIDE_EFFECTS (primop0))
                return build (COMPOUND_EXPR, TREE_TYPE (value),
                              primop0, value);
              return value;
            }
        }
    }

  *op0_ptr = convert (type, primop0);
  *op1_ptr = convert (type, primop1);

  *restype_ptr = boolean_type_node;

  return 0;
}

/* Prepare expr to be an argument of a TRUTH_NOT_EXPR,
   or validate its data type for an `if' or `while' statement or ?..: exp.

   This preparation consists of taking the ordinary
   representation of an expression expr and producing a valid tree
   boolean expression describing whether expr is nonzero.  We could
   simply always do build_binary_op (NE_EXPR, expr, boolean_false_node, 1),
   but we optimize comparisons, &&, ||, and !.

   The resulting type should always be `boolean_type_node'.  */

tree
#ifdef GPC
truthvalue_conversion1 (expr)
#else
truthvalue_conversion (expr)
#endif /* GPC */
     tree expr;
{
  if (TREE_CODE (expr) == ERROR_MARK)
    return expr;

#if 0 /* This appears to be wrong for C++.  */
  /* These really should return error_mark_node after 2.4 is stable.
     But not all callers handle ERROR_MARK properly.  */
  switch (TREE_CODE (TREE_TYPE (expr)))
    {
    case RECORD_TYPE:
      error ("struct type value used where scalar is required");
      return boolean_false_node;

    case UNION_TYPE:
      error ("union type value used where scalar is required");
      return boolean_false_node;

    case ARRAY_TYPE:
      error ("array type value used where scalar is required");
      return boolean_false_node;

    default:
      break;
    }
#endif /* 0 */

  switch (TREE_CODE (expr))
    {
      /* It is simpler and generates better code to have only TRUTH_*_EXPR
         or comparison expressions as truth values at this level.  */
#if 0
    case COMPONENT_REF:
      /* A one-bit unsigned bit-field is already acceptable.  */
      if (1 == TREE_INT_CST_LOW (DECL_SIZE (TREE_OPERAND (expr, 1)))
          && TREE_UNSIGNED (TREE_OPERAND (expr, 1)))
        return expr;
      break;
#endif

    case EQ_EXPR:
      /* It is simpler and generates better code to have only TRUTH_*_EXPR
         or comparison expressions as truth values at this level.  */
#if 0
      if (integer_zerop (TREE_OPERAND (expr, 1)))
        return build_unary_op (TRUTH_NOT_EXPR, TREE_OPERAND (expr, 0), 0);
#endif
    case NE_EXPR: case LE_EXPR: case GE_EXPR: case LT_EXPR: case GT_EXPR:
    case TRUTH_ANDIF_EXPR:
    case TRUTH_ORIF_EXPR:
    case TRUTH_AND_EXPR:
    case TRUTH_OR_EXPR:
    case TRUTH_XOR_EXPR:
    case TRUTH_NOT_EXPR:
      TREE_TYPE (expr) = boolean_type_node;
      return expr;

    case ERROR_MARK:
      return expr;

    case INTEGER_CST:
      return integer_zerop (expr) ? boolean_false_node : boolean_true_node;

    case REAL_CST:
      return real_zerop (expr) ? boolean_false_node : boolean_true_node;

    case ADDR_EXPR:
      /* If we are taking the address of a external decl, it might be zero
         if it is weak, so we cannot optimize.  */
      if (TREE_CODE_CLASS (TREE_CODE (TREE_OPERAND (expr, 0))) == 'd'
          && DECL_EXTERNAL (TREE_OPERAND (expr, 0)))
        break;

      if (TREE_SIDE_EFFECTS (TREE_OPERAND (expr, 0)))
        return build (COMPOUND_EXPR, boolean_type_node,
                      TREE_OPERAND (expr, 0), boolean_true_node);
      else
        return boolean_true_node;

    case COMPLEX_EXPR:
      return build_binary_op ((TREE_SIDE_EFFECTS (TREE_OPERAND (expr, 1))
                               ? TRUTH_OR_EXPR : TRUTH_ORIF_EXPR),
                              truthvalue_conversion (TREE_OPERAND (expr, 0)),
                              truthvalue_conversion (TREE_OPERAND (expr, 1)),
                              0);

    case NEGATE_EXPR:
    case ABS_EXPR:
    case FLOAT_EXPR:
    case FFS_EXPR:
      /* These don't change whether an object is non-zero or zero.  */
      return truthvalue_conversion (TREE_OPERAND (expr, 0));

    case LROTATE_EXPR:
    case RROTATE_EXPR:
      /* These don't change whether an object is zero or non-zero, but
         we can't ignore them if their second arg has side-effects.  */
      if (TREE_SIDE_EFFECTS (TREE_OPERAND (expr, 1)))
        return build (COMPOUND_EXPR, boolean_type_node, TREE_OPERAND (expr, 1),
                      truthvalue_conversion (TREE_OPERAND (expr, 0)));
      else
        return truthvalue_conversion (TREE_OPERAND (expr, 0));

    case COND_EXPR:
      /* Distribute the conversion into the arms of a COND_EXPR.  */
      return fold (build (COND_EXPR, boolean_type_node, TREE_OPERAND (expr, 0),
                          truthvalue_conversion (TREE_OPERAND (expr, 1)),
                          truthvalue_conversion (TREE_OPERAND (expr, 2))));

    case CONVERT_EXPR:
      /* Don't cancel the effect of a CONVERT_EXPR from a REFERENCE_TYPE,
         since that affects how `default_conversion' will behave.  */
      if (TREE_CODE (TREE_TYPE (expr)) == REFERENCE_TYPE
          || TREE_CODE (TREE_TYPE (TREE_OPERAND (expr, 0))) == REFERENCE_TYPE)
        break;
      /* fall through...  */
    case NOP_EXPR:
      /* If this is widening the argument, we can ignore it.  */
      if (TYPE_PRECISION (TREE_TYPE (expr))
          >= TYPE_PRECISION (TREE_TYPE (TREE_OPERAND (expr, 0))))
        return truthvalue_conversion (TREE_OPERAND (expr, 0));
      break;

    case MINUS_EXPR:
      /* With IEEE arithmetic, x - x may not equal 0, so we can't optimize
         this case.  */
      if (TARGET_FLOAT_FORMAT == IEEE_FLOAT_FORMAT
          && TREE_CODE (TREE_TYPE (expr)) == REAL_TYPE)
        break;
      /* fall through...  */
    case BIT_XOR_EXPR:
      /* This and MINUS_EXPR can be changed into a comparison of the
         two objects.  */
      if (TREE_TYPE (TREE_OPERAND (expr, 0))
          == TREE_TYPE (TREE_OPERAND (expr, 1)))
        return build_binary_op (NE_EXPR, TREE_OPERAND (expr, 0),
                                TREE_OPERAND (expr, 1), 1);
      return build_binary_op (NE_EXPR, TREE_OPERAND (expr, 0),
                              fold (build1 (NOP_EXPR,
                                            TREE_TYPE (TREE_OPERAND (expr, 0)),
                                            TREE_OPERAND (expr, 1))), 1);

    case BIT_AND_EXPR:
      if (integer_onep (TREE_OPERAND (expr, 1))
          && TREE_TYPE (expr) != boolean_type_node)
        /* Using convert here would cause infinite recursion.  */
        return build1 (NOP_EXPR, boolean_type_node, expr);
      break;

    case MODIFY_EXPR:
      if (warn_parentheses && C_EXP_ORIGINAL_CODE (expr) == MODIFY_EXPR)
        warning ("suggest parentheses around assignment used as truth value");
      break;

    default:
      break;
    }

  if (TREE_CODE (TREE_TYPE (expr)) == COMPLEX_TYPE)
    return (build_binary_op
            ((TREE_SIDE_EFFECTS (expr)
              ? TRUTH_OR_EXPR : TRUTH_ORIF_EXPR),
             truthvalue_conversion (build_unary_op (REALPART_EXPR, expr, 0)),
             truthvalue_conversion (build_unary_op (IMAGPART_EXPR, expr, 0)),
             0));

  return build_binary_op (NE_EXPR, expr, integer_zero_node, 1);
}
#ifdef GPC
/* In pascal, we need to return BOOLEAN_TYPE for all logical
   operations. Instead of changing the real truthvalue_conversion
   do this stub routine that converts the integer_type_node
   to boolean_type_node.
 */
tree
truthvalue_conversion(exp)
tree exp;
{
  tree result = truthvalue_conversion1 (exp);
  result = convert (boolean_type_node, result);

  return result;
}
#endif /* GPC */

/* Read the rest of a #-directive from input stream FINPUT.
   In normal use, the directive name and the white space after it
   have already been read, so they won't be included in the result.
   We allow for the fact that the directive line may contain
   a newline embedded within a character or string literal which forms
   a part of the directive.

   The value is a string in a reusable buffer.  It remains valid
   only until the next time this function is called.

   The terminating character ('\n' or EOF) is left in FINPUT for the
   caller to re-read.  */

char *
get_directive_line (finput)
     register FILE *finput;
{
  static char *directive_buffer = NULL;
  static unsigned buffer_length = 0;
  register char *p;
  register char *buffer_limit;
  register int looking_for = 0;
  register int char_escaped = 0;

  if (buffer_length == 0)
    {
      directive_buffer = (char *)xmalloc (128);
      buffer_length = 128;
    }

  buffer_limit = &directive_buffer[buffer_length];

  for (p = directive_buffer; ; )
    {
      int c;

      /* Make buffer bigger if it is full.  */
      if (p >= buffer_limit)
        {
          register unsigned bytes_used = (p - directive_buffer);

          buffer_length *= 2;
          directive_buffer
            = (char *)xrealloc (directive_buffer, buffer_length);
          p = &directive_buffer[bytes_used];
          buffer_limit = &directive_buffer[buffer_length];
        }

      c = getc (finput);

      /* Discard initial whitespace.  */
      if ((c == ' ' || c == '\t') && p == directive_buffer)
        continue;

      /* Detect the end of the directive.  */
      if (looking_for == 0
          && (c == '\n' || c == EOF))
        {
          ungetc (c, finput);
          c = '\0';
        }

      *p++ = c;

      if (c == 0)
        return directive_buffer;

      /* Handle string and character constant syntax.  */
      if (looking_for)
        {
          if (looking_for == c && !char_escaped)
            looking_for = 0;    /* Found terminator... stop looking.  */
        }
      else
        if (c == '\'' || c == '"')
          looking_for = c;      /* Don't stop buffering until we see another
                                   another one of these (or an EOF).  */

      /* Handle backslash.  */
      char_escaped = (c == '\\' && ! char_escaped);
    }
}

/* Make a variant type in the proper way for C/C++, propagating qualifiers
   down to the element type of an array.  */

#ifdef EGCS92
/* Make a variant type in the proper way for C/C++, propagating qualifiers
   down to the element type of an array.  */

tree
c_build_qualified_type (type, type_quals)
     tree type;
     int type_quals;
{
  /* A restrict-qualified pointer type must be a pointer to object or
     incomplete type.  Note that the use of POINTER_TYPE_P also allows
     REFERENCE_TYPEs, which is appropriate for C++.  Unfortunately,
     the C++ front-end also use POINTER_TYPE for pointer-to-member
     values, so even though it should be illegal to use `restrict'
     with such an entity we don't flag that here.  Thus, special case
     code for that case is required in the C++ front-end.  */
  if ((type_quals & TYPE_QUAL_RESTRICT)
      && (!POINTER_TYPE_P (type)
          || !C_TYPE_OBJECT_OR_INCOMPLETE_P (TREE_TYPE (type))))
    {
      error ("invalid use of `restrict'");
      type_quals &= ~TYPE_QUAL_RESTRICT;
    }

  if (TREE_CODE (type) == ARRAY_TYPE)
    return build_array_type (c_build_qualified_type (TREE_TYPE (type),
                                                     type_quals),
                             TYPE_DOMAIN (type));
  return build_qualified_type (type, type_quals);
}
#else
tree
c_build_type_variant (type, constp, volatilep)
     tree type;
     int constp, volatilep;
{
  if (TREE_CODE (type) == ARRAY_TYPE)
    return build_array_type (c_build_type_variant (TREE_TYPE (type),
                                                   constp, volatilep),
                             TYPE_DOMAIN (type));
  return build_type_variant (type, constp, volatilep);
}
#endif

#ifdef GPC
/* change_main_variant() was removed from tree.c because
   it was "unused". Do we still need it ?*/

/* Give TYPE a new main variant: NEW_MAIN.
   This is the right thing to do only when something else
   about TYPE is modified in place.  */

void
change_main_variant (type, new_main)
     tree type, new_main;
{
  tree t;
  tree omain = TYPE_MAIN_VARIANT (type);

  /* Remove TYPE from the TYPE_NEXT_VARIANT chain of its main variant.  */
  if (TYPE_NEXT_VARIANT (omain) == type)
    TYPE_NEXT_VARIANT (omain) = TYPE_NEXT_VARIANT (type);
  else
    for (t = TYPE_NEXT_VARIANT (omain); t && TYPE_NEXT_VARIANT (t);
         t = TYPE_NEXT_VARIANT (t))
      if (TYPE_NEXT_VARIANT (t) == type)
        {
          TYPE_NEXT_VARIANT (t) = TYPE_NEXT_VARIANT (type);
          break;
        }

  TYPE_MAIN_VARIANT (type) = new_main;
  TYPE_NEXT_VARIANT (type) = TYPE_NEXT_VARIANT (new_main);
  TYPE_NEXT_VARIANT (new_main) = type;
}


/* Build a Pascal variant of the TYPE.
 *
 * QUALIFIER is a bit mask of the following type qualifiers:
 *   `qualified' is not implemented yet.
 *
 *   `restricted', `protected', `bindable', `packed' are ISO-10206 standard
 *   qualifiers for types. (`protected' is not handled here.)
 *
 *   CONFORMANT is an internal flag for conformant array parameters
 *   and their indices (@@@ Will be re-implemented)
 *
 * COPY is nonzero if the type needs to be copied instead if modified.
 * A copy is still not made, if the VARIANT info would not modify the type.
 */
tree
pascal_type_variant (type, qualifier, copy)
     tree type;
     int  qualifier;
     int copy;
{
  int need_copy = 0;
                                                                /* 0 & 1 reserved */
#if 0
  /* handled by c_build_type_variant */
  int protected   = !!(qualifier & TYPE_QUALIFIER_PROTECTED);   /* TREE_READONLY */

  /* handled differently; flag reserved */
  int string      = !!(qualifier & TYPE_QUALIFIER_STRING);      /* 4 */
#endif

  int restricted  = !!(qualifier & TYPE_QUALIFIER_RESTRICTED);  /* 2 */
  int bindable    = !!(qualifier & TYPE_QUALIFIER_BINDABLE);    /* 3 */
                                                                /* 4 strings */
  int conformant  = !!(qualifier & TYPE_QUALIFIER_CONFORMANT);  /* 5 */
  int packed      = !!(qualifier & TYPE_QUALIFIER_PACKED);      /* 6 */
  int qualified   = !!(qualifier & TYPE_QUALIFIER_QUALIFIED);   /* ???? OUT OF BITS */

  need_copy = (restricted    && !PASCAL_TYPE_RESTRICTED (type))
              || (bindable   && !PASCAL_TYPE_BINDABLE (type))
              || (conformant && !PASCAL_TYPE_CONFORMANT_INDEX (type))
#if 0
              || (qualified  && !PASCAL_TYPE_QUALIFIED (type))
#endif
              || (packed     && !PASCAL_TYPE_PACKED (type));

  /* @@ Aux info??? */

  if (copy && need_copy)
    {
      type = build_type_copy (type);
      /* This is a new type, so remove it from the variants of the old type */
      change_main_variant (type, type);

      /* Remove the circularity  */
      TYPE_NEXT_VARIANT (type) = NULL_TREE;
    }

  if (bindable)
    PASCAL_TYPE_BINDABLE (type) = 1;

  if (conformant)
    PASCAL_TYPE_CONFORMANT_INDEX (type) = 1;

  if (restricted)
    PASCAL_TYPE_RESTRICTED (type) = 1;

  if (qualified)
    warning ("`qualified' is not yet implemented - ignored");

  if (packed)
    {
      tree tscan = type;
      /* Packed packs the arrays given in the same index list
       * @@@@@@ This packs all subsequent arrays: this does not
       * @@@@@@ know which of the arrays are defined in the same index list.
       * @@@@@@ Parser should flag them somehow...
       */
      do
        {
          PASCAL_TYPE_PACKED (tscan) = 1;
          if (TREE_CODE (tscan) == ARRAY_TYPE)
            tscan = TREE_TYPE (tscan);
        }
      while (TREE_CODE (tscan) == ARRAY_TYPE);
    }

  if (qualifier & ~(TYPE_QUALIFIER_RESTRICTED   | TYPE_QUALIFIER_BINDABLE
                    | TYPE_QUALIFIER_CONFORMANT | TYPE_QUALIFIER_PACKED
                    | TYPE_QUALIFIER_QUALIFIED))
    abort ();

  /* Not allowed */
  if (PASCAL_TYPE_BINDABLE (type) && PASCAL_TYPE_RESTRICTED (type))
    error ("restricted types must not be bindable");

  return type;
}


tree
pascal_type_extension (type, extension)
     tree type;
     int extension;
{
  int t_byte     = !!(extension & TYPE_QUALIFIER_BYTE);
  int t_short    = !!(extension & TYPE_QUALIFIER_SHORT);
  int t_long     = !!(extension & TYPE_QUALIFIER_LONG);
  int t_longlong = !!(extension & TYPE_QUALIFIER_LONGLONG);
  int t_unsigned = !!(extension & TYPE_QUALIFIER_UNSIGNED);

  if (extension & ~(TYPE_SELECT))
    abort ();

  if (t_byte + t_short + t_long + t_longlong > 1)
    {
      error ("illegal use of multiple size qualifiers");
      return type;
    }

  /* `__byte__ integer' */
  if (t_byte)
    {
      if (type == integer_type_node)
        type = intQI_type_node;
      else
        error ("illegal type used with `__byte__' type qualifier");
    }

  /* `__short__ integer' and `__short__ real' */
  if (t_short)
    {
      if (type == integer_type_node)
        type = short_integer_type_node;
      else if (type == double_type_node)
        type = float_type_node;
      else
        error ("illegal type used with `__short__' type qualifier");
    }

  /* `__long__ integer' and `__long__ real' */
  if (t_long)
    {
      if (type == integer_type_node)
        type = long_integer_type_node;
      else if (type == double_type_node)
        type = long_double_type_node;
      else
        error ("illegal type used with `__long__' type qualifier");
    }

  /* `__longlong__ integer' */
  if (t_longlong)
    {
      if (type == integer_type_node)
        type = long_long_integer_type_node;
      else
        error ("illegal type used with `__longlong__' type qualifier");
    }

  /* `__unsigned__ INTEGER',
   *   where integer may be qualified with __short__, __long__ or __longlong__
   */
  if (t_unsigned)
    {
      int fail = 1;
      if (TREE_CODE (type) == INTEGER_TYPE
          && TREE_TYPE (type) == NULL_TREE)
        {
          tree t = type_for_mode (TYPE_MODE (type), 1);

          if (t)
            {
              type = t;
              fail = 0;
            }
        }
      if (fail)
        error ("illegal type used with `__unsigned__' type qualifier");
    }

  return type;
}
#endif /* GPC */
