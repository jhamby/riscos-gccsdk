/* Language-specific hook definitions for Pascal front end.
   Copyright (C) 1991, 1995, 1997-2000 Free Software Foundation, Inc.

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


#include "gcc-version.h"
#include "config.h"
#include "gansidecl.h"
#include <stdio.h>
#ifdef GPC
#include "gbe.h"
#include "util.h"
#include "gpc-defs.h"
#else
/* In GPC, they are included via gbe.h */
#include "tree.h"
#include "flags.h"
#include "input.h"
#endif
#ifdef EGCS
#include "toplev.h"
#endif

/* Each of the functions defined here
   is an alternative to a function in objc-actions.c.  */

#ifdef EGCS

int
lang_decode_option (argc, argv)
     int argc;
     char **argv;
{
  return c_decode_option (argc, argv);
}

#else /* not EGCS */

int
lang_decode_option (p)
     char *p;
{
  return c_decode_option (p);
}

#endif /* not EGCS */

void
lang_init_options ()
{
  pascal_init_options ();
}

void
lang_init ()
{
  pascal_init ();
#if !USE_CPPLIB
  /* the beginning of the file is a new line; check for # */
  /* With luck, we discover the real source file's name from that
     and put it in input_filename.  */
  ungetc (check_newline (), finput);
#endif
}

void
lang_finish ()
{
}

char *
lang_identify ()
{
  return "pascal";
}

void
print_lang_statistics ()
{
}

/* Used by c-lex.c, but only for objc.  */

tree
lookup_interface (arg)
     tree arg;
{
  return 0;
}

tree
is_class_name (arg)
    tree arg;
{
  return 0;
}

void
maybe_objc_check_decl (decl)
     tree decl;
{
}

int
maybe_objc_comptypes (lhs, rhs, reflexive)
     tree lhs, rhs;
     int reflexive;
{
  return -1;
}

tree
maybe_objc_method_name (decl)
    tree decl;
{
  return 0;
}

tree
maybe_building_objc_message_expr ()
{
  return 0;
}

int
recognize_objc_keyword ()
{
  return 0;
}

tree
build_objc_string (len, str)
    int len;
    const char *str;
{
  abort ();
  return NULL_TREE;
}

void
GNU_xref_begin ()
{
  fatal ("GPC does not yet support XREF");
}

void
GNU_xref_end ()
{
  fatal ("GPC does not yet support XREF");
}

/* Called at end of parsing, but before end-of-file processing.  */

void
finish_file ()
{
  extern tree static_ctors ATTRIBUTE_UNUSED, static_dtors ATTRIBUTE_UNUSED;
  extern tree build_function_call                 PROTO((tree, tree));
  tree void_list_node ATTRIBUTE_UNUSED =
      build_tree_list (NULL_TREE, void_type_node);
#ifndef ASM_OUTPUT_CONSTRUCTOR
  if (static_ctors)
    {
      tree fnname = get_file_function_name ('I');
      start_function (void_list_node,
                      build_parse_node (CALL_EXPR, fnname, void_list_node,
                                        NULL_TREE),
                      NULL_TREE, NULL_TREE, 0);
      fnname = DECL_ASSEMBLER_NAME (current_function_decl);
      store_parm_decls ();

      for (; static_ctors; static_ctors = TREE_CHAIN (static_ctors))
        expand_expr_stmt (build_function_call (TREE_VALUE (static_ctors),
                                               NULL_TREE));

      finish_function (0);

      assemble_constructor (IDENTIFIER_POINTER (fnname));
    }
#endif
#ifndef ASM_OUTPUT_DESTRUCTOR
  if (static_dtors)
    {
      tree fnname = get_file_function_name ('D');
      start_function (void_list_node,
                      build_parse_node (CALL_EXPR, fnname, void_list_node,
                                        NULL_TREE),
                      NULL_TREE, NULL_TREE, 0);
      fnname = DECL_ASSEMBLER_NAME (current_function_decl);
      store_parm_decls ();

      for (; static_dtors; static_dtors = TREE_CHAIN (static_dtors))
        expand_expr_stmt (build_function_call (TREE_VALUE (static_dtors),
                                               NULL_TREE));

      finish_function (0);

      assemble_destructor (IDENTIFIER_POINTER (fnname));
    }
#endif
}
