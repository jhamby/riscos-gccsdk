/* Misc routines & definitions for the GPC
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
along with GNU Pascal; see the file COPYING. If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA. */

/*
 * Authors: Jukka Virtanen <jtv@hut.fi>
 *          Peter Gerwinski <peter@gerwinski.de>
 */

#include "gcc-version.h"
#include "config.h"
#include "gansidecl.h"
#ifdef EGCS
#include "system.h"
#include "toplev.h"
#endif

#ifdef va_start
#  undef va_start
#endif
#ifdef __STDC__
#  include <stdarg.h>
#else
#  include <varargs.h>
#endif
#include <stdio.h>

#include "gbe.h"
#ifndef EGCS
#  include "bytecode.h"
#endif
#include "gpc-defs.h"
#include "module.h"
#include "parse.h"
#include "rts.h"
#include "except.h"
#include "function.h"
#include "types.h"
#include "rts/types.h"
#include "util.h"

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

/* This is not used. It's required to compile gpc-decl.c */
tree ridpointers[1];

void init_any PROTO ((tree, int));

tree pascal_array_type_nelts PROTO((tree));

/* Name of the main program, usually "main".
 */
char *gpc_main = "main";

/* Nonzero if we only want to compile the interface part of a Unit/Module.
 */
int flag_interface_only = 0;

/* Nonzero if we only want to compile the implementation part of a Unit/Module.
 */
int flag_implementation_only = 0;

/* Nonzero means to warn about ignored field names in initializers.
 */
int warn_field_name_problem = 1;

/* Nonzero if warnings about the misuse of typed constants as
 * initialized variables shall be issued.
 */
int warn_typed_const = 1;

/* Nonzero if warnings about usage of the useless `near' and
 * `far' directives shall be issued.
 */
int warn_near_far = 1;

/* Nonzero if the main program has a nonlocal label.
 */
int main_program_has_nonlocal_label = 0;

/* Nonzero if the main program needs a stack frame.
 */
int main_program_needs_context = 0;

/* The main program's non-local goto handler.
 */
int main_program_first_label_num = -1;
tree main_program_named_labels = NULL_TREE;
tree main_program_nonlocal_labels = NULL_TREE;

/* Name of executable file to be produced.
 */
char *executable_file_name = NULL;

/* Path where executable files should be produced.
 */
char *executable_path = NULL;

/* Path where unit `.o' and `.gpi' files should be produced during AutoMake.
 */
char *unit_destination_path = NULL;

/* Path where `.o' files from $L directives should be produced during AutoMake.
 */
char *object_destination_path = NULL;

/* Shall we initialize the object and unit path
 * with <prefix>/lib/gcc-lib/<platform>/<version>[/units]?
 * Nonzero --> do not initialize.
 */
int flag_no_default_paths = 0;

/* Path where `.gpi' files should be produced during this compilation.
 * (For `.o' files, `-o /foo/bar.o' can be used.)  This is needed for
 * internal use during AutoMake.
 */
char *gpi_destination_path = NULL;

/* Name(s) of extra modules/units to be imported automatically.
 */
char *extra_imports = NULL;

/* Option -fsetlimit:NNNNN changes the upper bound of "unbound"
 * integer type sets (default upper bound = DEFAULT_SET_SIZE).
 *
 * While reading options we can't make tree nodes.
 */
int requested_set_size = 0;
tree integer_set_size   = NULL_TREE;

/* Option -ffield-widths:NN,NN,NN changes default field widths
 * `write' operations.
 */
int int_out_width = INT_OUT_WIDTH;
int real_out_width = REAL_OUT_WIDTH;
int bool_out_width = BOOL_OUT_WIDTH;
int long_int_out_width = LONG_INT_OUT_WIDTH;
int long_real_out_width = LONG_REAL_OUT_WIDTH;

/* Default set type for unbounded integer sets, size is
 * integer_set_size
 */
tree integer_set_type_node;

/* a pointer to integer type */
tree integer_ptr_type_node;

/* Size of the file control block in bits.
 * This is referenced by stor-layout.c.
 */
tree size_of_file_type;

#ifdef EGCS
/* Size of the file control block in units.
 * This is referenced by types.c.
 */
tree size_unit_of_file_type;
#endif /* EGCS */

/* TEXT and FILE type nodes */
/* TEXT is a predefined file type in ISO Pascal */
tree text_type_node;

/* COMPLEX type node */
tree complex_type_node;

/* Pascal nodes */
tree boolean_false_node;
tree boolean_true_node;
tree integer_maxint_node;
tree identifier_output;
tree identifier_input;
tree identifier_stderr;
tree global_input_file_node;
tree global_output_file_node;
tree global_error_file_node;

tree real_zero_node;
tree real_half_node;

/* index type for character arrays (strings) */
tree char_array_index_type_node;

tree string_schema_proto_type = NULL_TREE;
tree const_string_schema_proto_type = NULL_TREE;

/* GPC extension: string parameter to a C routine */
tree cstring_type_node;

tree empty_arglist;

/* Extended Pascal nodes */
tree complex_zero_node;
tree real_max_node;
tree real_min_node;
tree real_eps_node;
tree char_max_node;

/* Required implementation dependent canonical-string-types
 * for TIME and DATE return values,
 * TIMESTAMP and BINDINGTYPE records.
 */
tree gpc_type_TIME;
tree gpc_type_DATE;
tree gpc_type_TIMESTAMP;
tree gpc_type_BINDINGTYPE;

/* Identifiers to access the contents of strings and schemata.
 */
tree string_id;
tree schema_id;

/* These *_id's are used to tell
 * grokdeclarator what and how we are currently defining things.
 */
tree const_id;
tree type_id;
tree inline_id;
tree varparm_id;
tree volatile_id;
tree register_id;
tree auto_id;
tree extern_id;
tree asmname_id;
tree static_id;
tree attribute_id;

/* If we have module contructors */
tree collect_constructors_now = NULL_TREE;

/* Prototypes for local utility functions */
static int external_var_p PROTO ((tree));
static void init_simple PROTO ((tree, int));
static void init_record PROTO ((tree, int, tree));
static void process_init_list PROTO ((tree));
static void declare_known_id PROTO ((char *, tree, int));



/*
 * Identifiers.
 */

/* Generate unique identifier_nodes of the form "foo_42" if
 * VALID is nonzero, otherwise "__foo_42__".
 * Note: The first parameter must not be longer than 200 chars!
 */
tree
get_unique_identifier (lead, valid)
     char *lead;
     int  valid;
{
  char idbuf [256]; /* No buffer overflow problem here because lead is a
                       compiler-supplied constant and therefore of limited
                       length */
  static int idcount = 0;

  if (valid)
    sprintf (idbuf, "%s_%d", lead, idcount++);
  else
    sprintf (idbuf, "__%s_%d__", lead, idcount++);
  return get_identifier (idbuf);
}

/* Get an "identifier" that starts with a BLANK character.
 */
tree
get_identifier_with_blank (str)
     char *str;
{
  tree result;
  int len = strlen (str);
  char *newstr = (char *) xmalloc (len + 2);

  *newstr = ' ';
  strcpy (newstr + 1, str);
  *(newstr + len + 1) = '\0';
  result = get_identifier (newstr);
  free (newstr);
  return result;
}

/* 1 if explained undeclared var errors.
 */
int undeclared_variable_notice = 0;

/* Called from gpc-parse.y.
 */
tree
check_identifier (decl, id)
    tree decl;
    tree id;
{
    /* First:  Special case for `with'-shadowed names.
     * Environment:  We have a with statement inside which we find a
     * name.  That name may be shadowed by a component reference to the
     * fields of the record variable, i.e. ID is an identifier
     * whose IDENTIFIER_LOCAL_VALUE is a COMPONENT_REF
     * and DECL is also that (found by lookup_name before).
     *
     * I admit this is an ugly way. Maybe someone cleans up this.
     */

  /* @@@ Should we mark the record variable TREE_USED? */
    if (decl && TREE_CODE (decl) == COMPONENT_REF &&
        id   && IDENTIFIER_LOCAL_VALUE (id) &&
        TREE_CODE (IDENTIFIER_LOCAL_VALUE (id)) == COMPONENT_REF)
      return decl;

    if (! decl || decl == error_mark_node)
      {
        if (IDENTIFIER_GLOBAL_VALUE (id) != error_mark_node
            || IDENTIFIER_ERROR_LOCUS (id) != current_function_decl)
          {
            error ("undeclared identifier `%s' (first use in this routine)",
                   IDENTIFIER_POINTER (id));
            if (! undeclared_variable_notice)
              {
                error (" (Each undeclared identifier is reported only once");
                error (" for each routine it appears in.)");
                undeclared_variable_notice = 1;
              }
            /* Prevent repeated error messages */
            IDENTIFIER_GLOBAL_VALUE (id) = error_mark_node;
            IDENTIFIER_ERROR_LOCUS (id) = current_function_decl;
          }
        return error_mark_node;
      }
    else if (! TREE_USED (decl))
      {
        if (DECL_EXTERNAL (decl))
          assemble_external (decl);
        TREE_USED (decl) = 1;
      }

    /* Set TREE_NONLOCAL if var is inherited in nested fcn.  */
    if (DECL_CONTEXT (decl) != 0
        && DECL_CONTEXT (decl) != current_function_decl)
      {
        DECL_NONLOCAL (decl) = 1;
        mark_addressable (decl);
      }

    /* @@@@ Check if the copy_node is really necessary here! */
    if (TREE_CODE (decl) == CONST_DECL)
      {
        if (DECL_INITIAL (decl) && DECL_INITIAL (decl) != error_mark_node)
          decl = copy_node (DECL_INITIAL (decl));   /* User defined constant */
        else
          decl = error_mark_node;
      }

    return decl;
}



/*
 * Variables.
 */

/* Create a new variable of type TYPE with a name constructed from
 * template+unique number.  Return the new variable.
 * Note: The first parameter must not be longer than 200 chars!
 */
tree
make_new_variable (name_template, type)
     char *name_template;
     tree type;
{
  tree var = start_decl (get_unique_identifier (name_template, 1),
                         build_tree_list (NULL_TREE, type),
                         0, NULL_TREE, NULL_TREE);
  finish_decl (var, NULL_TREE, NULL_TREE);
  init_any (var, 0);

  /* In case the size of this variable can only be determined at run
   * time, labels in this level will need fixups even if we thought they
   * wouldn't need any.
   */
  if (! TREE_CONSTANT (TYPE_SIZE (TREE_TYPE (var))))
    /* @@@@@@@ What to do? */;

  return var;
}

/* Let the user program alloca() a new string of capacity
 * CAPACITY, an integer expression, and return an expression for
 * the string that can be evaluated more than once, even if
 * CAPACITY is not constant.
 *
 * This is used for string catenation and "magic" routines that
 * return string values whose capacity is not known at compile-time.
 */
tree
alloca_string (capacity)
     tree capacity;
{
  tree retval, retval_type, retval_size, stmt;
  int int_type_size;

  /* Type
   *   retval_type = String ( capacity );
   */
  capacity = stabilize_reference (capacity);
  size_volatile++;
  retval_type = build_pascal_string_schema (capacity);
  size_volatile--;

  /* Var
   *   retval: ^retval_type;
   */
  retval = make_new_variable ("alloca_string_retval",
                              build_pointer_type (retval_type));
  TREE_USED (retval) = 1;

  /* retval := __builtin_alloca (capacity + 1 + 2 * sizeof (cardinal));
   */
  int_type_size = TREE_INT_CST_LOW (TYPE_SIZE (unsigned_type_node));
  retval_size = build_pascal_binary_op (PLUS_EXPR, capacity,
                  build_int_2 (1 + 2 * int_type_size / BITS_PER_UNIT, 0));
  stmt = build_modify_expr (retval, NOP_EXPR,
           build_function_call (lookup_name (get_identifier ("__builtin_alloca")),
                                build_tree_list (NULL_TREE, retval_size)));

  /* retval^.Capacity := capacity;
   */
  retval = build_indirect_ref (retval, "temporary string allocation");
  stmt = build (COMPOUND_EXPR, void_type_node, stmt,
                build_modify_expr (build_component_ref (retval, get_identifier ("Capacity")),
                                   NOP_EXPR, capacity));

  /* return (retval^);
   */
  retval = build (COMPOUND_EXPR, retval_type,
                  stabilize_reference (stmt), retval);
  return retval;
}

/* Create a new VAR_DECL of variable string type model.
 * Copy the value of DATA to the new VAR_DECL.
 *
 * If MODEL is NULL_TREE, the string capacity is the same as in
 * the char or string type node in DATA.  If DATA is a string schema,
 * the capacity will be the current length of the string in DATA.
 *
 * If DATA is NULL_TREE or COPY is zero, don't copy anything, just
 * create a new string variable.
 *
 * If MODEL and DATA are both NULL_TREE, abort().
 *
 * Return the new string.
 */
tree
new_string_by_model (model, data, copy)
     tree model;
     tree data;
     int  copy;
{
  tree type;
  tree length;
  tree new_string;

  if (! model || PASCAL_TYPE_UNDISCRIMINATED_STRING (model))
    {
      if (! data)
        abort ();
      length = PASCAL_STRING_LENGTH (data);
      type = build_pascal_string_schema (length);
    }
  else
    type = model;

  /* Create a new variable.  Don't let it be constant since
   * we are going to assign DATA to it.
   */
  new_string = make_new_variable ("tmp_string",
                                  build_type_variant (type, 0,
                                                      TYPE_VOLATILE (type)));

  /* Initialize the capacity schema discriminant field to length of string.
   */
  if (PASCAL_TYPE_STRING (type))
    init_any (new_string, 0);

  /* Assign the DATA value to the new string schema.
   */
  if (data && copy)
    assign_string (new_string, data);

  return new_string;
}

#ifndef NEW_DECLARE_VARS

/* Pull any attributes out of `qualifiers'.  If `qualifiers' was pure
 * attributes, qualifiers will be set to `NULL_TREE'.  If there are no
 * attributes in `qualifiers', or `qualifiers' is a `NULL_TREE',
 * `attributes' will be set to `NULL_TREE'.
 */
static void
extract_attributes (qualifiers, attributes)
     tree *qualifiers, *attributes;
{
  tree node = *qualifiers;

  *qualifiers = NULL_TREE;
  *attributes = NULL_TREE;
  while (node)
    {
      tree tmp = TREE_CHAIN (node);
      TREE_CHAIN (node) = NULL_TREE;
      if (TREE_VALUE (node) == attribute_id)
        {
          *attributes = chainon (*attributes, TREE_PURPOSE (node));
        }
      else
        {
          *qualifiers = chainon (*qualifiers, node);
        }
      node = tmp;
    }
}

#endif /* NEW_DECLARE_VARS */

/* Subroutine of declare_vars():
 * Determine whether NAME is declared to be an external variable.
 */
static int
external_var_p (name)
     tree name;
{
  tree meaning = lookup_name (name);
  return meaning
         && TREE_CODE (meaning) == VAR_DECL
         && DECL_EXTERNAL (meaning);
}

/* Declare variables.
 *
 * NAME_LIST is a TREE_LIST of IDENTIFIER nodes holding the names of
 * the variables.
 *
 * TYPE is the type of the variables.
 *
 * ABSOLUTE_OR_INITIAL_VALUE holds an initial value for the
 * variables.  It may be flagged to hold an `absolute' address,
 * but this does not matter here.
 *
 * ASMNAME holds the external (linker) name for this variable.
 * NULL_PTR means to use default names.
 *
 * The bits in QUALIFIERS indicate whether this variable is
 * static, volatile, register, a typed constant, and/or external.
 *
 * When INTERFACE is nonzero, this declaration is part of an
 * interface section of a unit or module.  From an interface
 * module we only output external references to variables.
 *
 * ATTRIBUTES holds a TREE_LIST of machine attributes.
 */
void
declare_vars (name_list, type, absolute_or_initial_value,
              asmname, qualifiers, interface, attributes)
     tree name_list, type, absolute_or_initial_value;
     char *asmname;
     unsigned qualifiers;
     int interface;
     tree attributes;
{

#ifdef NEW_DECLARE_VARS

  tree item;
  tree absolute_value = NULL_TREE;
  tree initial_value = NULL_TREE;

  if (absolute_or_initial_value)
    {
      if (PASCAL_ABSOLUTE_CLAUSE (absolute_or_initial_value))
        absolute_value = absolute_or_initial_value;
      else
        initial_value = absolute_or_initial_value;
    }
  if (asmname && name_list && TREE_CHAIN (name_list))
    error ("declaring multiple variables with one asmname");

  /* Check type and initial value.
   */
  if (TREE_CODE (type) == ERROR_MARK)
    return;
  if (PASCAL_TYPE_UNDISCRIMINATED_SCHEMA (type))
    error ("missing schema discriminants");
  if (initial_value
      && check_pascal_initializer (type, initial_value) != 0)
    {
      error ("initial value is of wrong type");
      absolute_or_initial_value = initial_value = NULL_TREE;
    }

  for (item = name_list; item; item = TREE_CHAIN (item))
    {
      tree name = TREE_VALUE (item);
      tree var;

      if (name == NULL_TREE
          || TREE_CODE (name) != IDENTIFIER_NODE)
        abort ();

      if (interface && ! we_are_loading_a_gpi_file)
        handle_autoexport (name);
      if (top_level_p (0)
          && ! (name_exported_p (name)
                || external_var_p (name)
                || we_are_loading_a_gpi_file))
        {
          /* Don't make unexported variables visible.
           */
          if (interface)
            continue;
          if ((qualifiers & (VQ_STATIC | VQ_EXTERNAL)) == 0)
            qualifiers |= VQ_STATIC;
        }

      /* Build the VAR_DECL node.
       */
      var = build_decl (VAR_DECL, name, type);
      /* DECL_INITIAL (var) = TREE_VALUE (absolute_or_initial_value); */
      DECL_CONTEXT (var) = current_function_decl;

      /* Check and apply qualifiers.
       */
      if (qualifiers & VQ_EXTERNAL)
        {
          if (qualifiers & VQ_STATIC)
            {
              error ("variable `%s' declared both `static' and `external'",
                     IDENTIFIER_POINTER (name));
              qualifiers &= ~((unsigned) VQ_STATIC);
            }
          if (absolute_value)
            {
              error ("variable `%s' declared both `external' and `absolute'",
                     IDENTIFIER_POINTER (name));
              absolute_or_initial_value = absolute_value = NULL_TREE;
            }
          if (initial_value)
            {
              error ("initialized variable `%s' declared `external'",
                     IDENTIFIER_POINTER (name));
              absolute_or_initial_value = initial_value = NULL_TREE;
            }
          DECL_EXTERNAL (var) = 1;
        }
      if (qualifiers & VQ_STATIC)
        TREE_STATIC (var) = 1;
      if (qualifiers & VQ_VOLATILE)
        TREE_THIS_VOLATILE (var) = 1;
      if (qualifiers & VQ_REGISTER)
        DECL_REGISTER (var) = 1;
      if (qualifiers & VQ_TYPEDCONST)
        TREE_READONLY (var) = 1;
      if ((qualifiers & VQ_EXTERNAL)
          || (top_level_p (0)
              && (qualifiers & (VQ_STATIC | VQ_REGISTER)) == 0))
        TREE_PUBLIC (var) = 1;

      /* Check and apply assembler name.
       */
      if (asmname)
        {
          char *p = asmname;
          while (*p && *p != ' ' && *p != '\t')
            p++;
          if (*p)
            error ("assembler name `%s' for `%s' contains whitespace",
                   asmname, IDENTIFIER_POINTER (name));
          DECL_ASSEMBLER_NAME (var) = get_identifier (asmname);
        }

      /* Push the decl into the current binding level.
       */
      pushdecl (var);

      /* Generate code.
       */
      push_obstacks_nochange ();
      end_temporary_allocation ();
      rest_of_decl_compilation (var, asmname, pascal_global_bindings_p(), 0);
      pop_obstacks ();
      expand_decl (var);

      /* VAR might be a pointer to an undiscriminated schema.
       * At this point, we can pre-discriminate it using its own contents ...
       */
      prediscriminate_schema (var, 0);
    }

#else /* not NEW_DECLARE_VARS */

  /* Wrapper to the old routine
   *
   *   declare_vars (var_list, type, initial_value, qualifiers, interface)
   *
   * to be used until the new one is stable.
   */
  
  tree var_list = name_list;
  tree initial_value = absolute_or_initial_value;
  tree tqualifiers;

  tree v;
  tree d;
  tree tt_normal = build_tree_list (NULL_TREE, type);
  tree tt_static = NULL_TREE;

  tqualifiers = attributes;
  attributes = NULL_TREE;
  if (qualifiers & VQ_EXTERNAL)
    tqualifiers = chainon (tqualifiers,
                           build_tree_list (NULL_TREE, extern_id));
  if (qualifiers & VQ_STATIC)
    tqualifiers = chainon (tqualifiers,
                           build_tree_list (NULL_TREE, static_id));
  if (qualifiers & VQ_VOLATILE)
    tqualifiers = chainon (tqualifiers,
                           build_tree_list (NULL_TREE, volatile_id));
  if (qualifiers & VQ_REGISTER)
    tqualifiers = chainon (tqualifiers,
                           build_tree_list (NULL_TREE, register_id));
  if (qualifiers & VQ_TYPEDCONST)
    tqualifiers = chainon (tqualifiers,
                           build_tree_list (NULL_TREE, const_id));

  if (PASCAL_TYPE_UNDISCRIMINATED_SCHEMA (type))
    error ("missing schema discriminants");

  if (initial_value &&
      check_pascal_initializer (type, initial_value) != 0)
    {
      error ("initial value is of wrong type");
      initial_value = NULL_TREE;
    }

  if (tqualifiers)
    tt_normal = chainon (tt_normal, copy_node (tqualifiers));

  extract_attributes (&tqualifiers, &attributes);

#if 0
  if (interface)
    {
      /* The variables get declared in the implementation part,
       * not in the interface.
       */

      tt_extern = build_tree_list (NULL_TREE, type);
      if (tqualifiers)
        tt_extern = chainon (tt_extern, copy_node (tqualifiers));

      tt_extern = chainon (tt_extern,
                           build_tree_list (NULL_TREE, extern_id));
    }
#endif

  for (v = var_list; v; v = TREE_CHAIN (v))
    {
      tree name = TREE_VALUE (v);
#if 0
      tree tt = interface ? tt_extern : tt_normal;
      tree init = interface ? NULL_TREE : initial_value;
#else
      tree tt = tt_normal;
      tree init = initial_value;
#endif
      tree constructor;

      if (interface && ! we_are_loading_a_gpi_file)
        handle_autoexport (name);

      if (top_level_p (0)
          && ! (name_exported_p (name)
                || external_var_p (name)
                || we_are_loading_a_gpi_file))
        {
          /* Don't make unexported variables visible.
           */

#if 0
          if (interface)
            continue;
#endif

          if (!asmname)
            {
              if (! tt_static)
                {
                  tt_static = build_tree_list (NULL_TREE, type);
                  if (tqualifiers)
                    tt_static = chainon (tt_static, copy_node (tqualifiers));

                  tt_static = maybe_make_static (tt_static);
                }
              tt = tt_static;
            }
        }

      d = start_decl (name, tt, init != NULL_TREE, attributes, NULL_TREE);

      /* In case we have an `asmname' qualifier, use the specified name.
       */
      if (asmname)
        {
          char *p;
          DECL_ASSEMBLER_NAME (d) = get_identifier (asmname);
          p = asmname;
          while (*p && *p != ' ' && *p != '\t')
            p++;
          if (*p)
            error ("assembler name for `%s' contains whitespace",
                   IDENTIFIER_POINTER (DECL_NAME (d)));
        }

      /* If there is an initializer, transform it to a constructor for
       * the variable being declared.
       */
      if (init)
        constructor = build_pascal_initializer (d, init);
      else
        constructor = NULL_TREE;

      finish_decl (d, constructor, NULL_TREE);

      /* D might be a pointer to an undiscriminated schema.
       * At this point, we can pre-discriminate it using its own contents ...
       */
      prediscriminate_schema (d, 0);
    }

#if 0
  /* Store the unexported variable declarations of the interface module
   * to the current_module so that the implementation module can
   * declare them as local variables.
   *
   * The list contains also pending function decl nodes, but for them
   * the first TREE_PURPOSE field is void_type_node
   *
   * TREE_LIST = pending decl in the list
   *   TREE_PURPOSE : NULL_TREE
   *   TREE_VALUE   : TREE_LIST
   *                    TREE_PURPOSE  : type
   *                    TREE_VALUE    : chain of pending var names
   *                      TREE_CHAIN  : TREE_LIST
   *                                        TREE_PURPOSE : initial_value
   *                                        TREE_VALUE   : qualifiers
   *   TREE_CHAIN   : next pending decl in the list
   *
   */
  if (interface && ! we_are_loading_a_gpi_file)
    {
      tree this_one = build_tree_list (NULL_TREE,
                                       tree_cons (type,
                                                  name_list,
                                                  build_tree_list (absolute_or_initial_value,
                                                                   tqualifiers)));
      current_module->pending_decls = chainon (current_module->pending_decls,
                                               this_one);
    }
#endif
#endif /* not NEW_DECLARE_VARS */
}

/* Construct a tree expression that copies LENGTH units of
 * storage from SOURCE to DEST.
 */
tree
build_string_move (dest, source, length)
     tree dest, source, length;
{
  tree args;
  args = build_tree_list (NULL_TREE, dest);
  args = chainon (args, build_tree_list (NULL_TREE, source));
  args = chainon (args, build_tree_list (NULL_TREE, length));
  return build_function_call (lookup_name (get_identifier ("__builtin_memcpy")),
                              args);
}

/* Construct a tree expression that pads LENGTH units of
 * storage DEST with the byte PATTERN.
 */
tree
build_string_pad (dest, length, pattern)
     tree dest, length, pattern;
{
  tree args;
  args = build_tree_list (NULL_TREE, dest);
  args = chainon (args, build_tree_list (NULL_TREE, pattern));
  args = chainon (args, build_tree_list (NULL_TREE, length));
  return build_function_call (lookup_name (get_identifier ("__builtin_memset")),
                              args);
}



/*
 * Parameters.
 */

/* Each conformant array argument is passed as follows:
 *
 * The array and it's conformant bounds are in the following
 * argument order:
 *
 *    1   2      n-1  n  n+1     n+2     n+3
 *   LO1 HI1 ... LOn HIn CARRAY0 CARRAY1 CARRAY2
 *
 * Where the indices always appear paired:
 * LO1 HI1  - Top level indices of CARRAYs
 * ...
 * LOn HIn  - lowest level indices of CARRAYs
 *
 * CARRAYx  - the n/2-dimension arrays.
 *
 * All of the indice's types are flagged with
 * PASCAL_TYPE_CONFORMANT_INDEX.
 *
 * The first thing after indices is the conformant
 * array type with variable sized bounds.
 *
 * If an ARRAY_TYPE is not preceded by a list of indices,
 * its indices are the same as the immediately preceding
 * conformant array's. (If some of this is not true, abort())
 */
void
handle_formal_conf_array_param_list (idlist, type, varparm, protected)
     tree idlist;
     tree type;
     int  varparm;
     int  protected;
{
  tree link;
  tree index_list = TREE_PURPOSE (TREE_VALUE (type));
  tree carray = TREE_VALUE (TREE_VALUE (type));
  tree parm;
  int old_value;

  /* Warn about everything ...
   */
  if (PEDANTIC (S1_E_O_G_PASCAL))
    pedwarn ("level 0 of ISO 7185 Pascal has no conformant arrays");

  /* Mark whether the array shall be packed.
   */
  if (TREE_PURPOSE (type))
    index_list = chainon (build_tree_list (NULL_TREE, NULL_TREE),
                          index_list);

  /* Convert "array [ 1..2 ] of packed array [ 3..4 ] of Foo" to
   * packed "array [ 1..2, 3..4 ] of Foo".
   */
  while (TREE_CODE (carray) == TREE_LIST)
    {
      /* Insert a marker if the array is packed at this index.
       */
      if (TREE_PURPOSE (carray))
        index_list = chainon (index_list, build_tree_list (NULL_TREE, NULL_TREE));

      index_list = chainon (index_list, TREE_PURPOSE (TREE_VALUE (carray)));
      carray = TREE_VALUE (TREE_VALUE (carray));
    }

  /* Build array with variable indices. The type is NOT laid
   * out. These are handled specially when actual parameters
   * are passed to this kind of formal in convert_arguments.
   *
   * The TREE_LIST node of the indices part of the conformant
   * array schema is flagged; it tells grokdeclarator to
   * take special care of it.
   */
  for (link = index_list; link; link = TREE_CHAIN (link))
    if (TREE_VALUE (link))
      {
        /* Build a new type node so that we can safely flag it.
         */
        tree ctype = pascal_type_variant (TREE_VALUE (link),
                                          TYPE_QUALIFIER_CONFORMANT, 1);

        tree lo = TREE_PURPOSE (TREE_PURPOSE (link));
        tree hi = TREE_VALUE (TREE_PURPOSE (link));
        tree index_type = build_tree_list (NULL_TREE, ctype);

        /* Push lower bound */
        parm = build_tree_list (build_tree_list (index_type, lo),
                                build_tree_list (NULL_TREE, NULL_TREE));
        push_parm_decl (parm);

        /* Push upper bound */
        parm = build_tree_list (build_tree_list (index_type, hi),
                                build_tree_list (NULL_TREE, NULL_TREE));
        push_parm_decl (parm);
      }

  /* Prevent conformant array size calculation until
   * we enter the function body.
   */
  old_value = immediate_size_expand;
  immediate_size_expand = 0;
  size_volatile++;

  for (link = nreverse (index_list); link; link = TREE_CHAIN (link))
    if (TREE_VALUE (link))
      {
        tree lo = TREE_PURPOSE (TREE_PURPOSE (link));
        tree hi = TREE_VALUE (TREE_PURPOSE (link));
        tree lodecl = lookup_name (lo);
        tree hidecl = lookup_name (hi);
        tree itype;

        if (! lodecl
            || ! hidecl
            || TREE_CODE (lodecl) != PARM_DECL
            || TREE_CODE (hidecl) != PARM_DECL)
          abort ();

        itype = build_range_type (TREE_TYPE (lodecl), lodecl, hidecl);

        carray = build_array_type (carray, itype);
        C_TYPE_VARIABLE_SIZE (carray) = 1;
      }
    else  /* "packed" marker */
      carray = grok_packed (carray);

  size_volatile--;
  immediate_size_expand = old_value;

  /* If this is a protected parameter, make it read-only
   */
  if (protected)
    type = build_type_variant (carray, 1, 0);

  /* Always pass this by reference because of its variable size
   */
  carray = build_reference_type (carray);
  C_TYPE_VARIABLE_SIZE (carray) = 1;

  if (! varparm)
    PASCAL_TYPE_VAL_REF_PARM (carray) = 1;

  /* Add the conformant array parameters.
   */
  for (link = idlist; link; link = TREE_CHAIN(link))
    {
      parm = build_tree_list (build_tree_list ((build_tree_list (NULL_TREE, carray)),
                                               TREE_VALUE (link)),
                              build_tree_list (NULL_TREE, NULL_TREE));
      push_parm_decl (parm);
    }
}

void
handle_formal_param_list (idlist, type, declspecs, varparm, protected)
     tree idlist;
     tree type;
     tree declspecs;
     int varparm;
     int protected;
{
  tree link;
  tree parm;

  if (TREE_CODE (type) == IDENTIFIER_NODE)
    {
      if (type == current_type_name)
        {
          tree name = type;
          char *ptr_name, *ref_name;
          tree ptr_type, ref_type;
          type = NULL_TREE;
          
          ptr_name = concat ("ptr_xref_", IDENTIFIER_POINTER (name), NULL_PTR);
          ptr_type = xref_tag (POINTER_TYPE, get_identifier (ptr_name));
          if (TREE_TYPE (ptr_type))
            type = TREE_TYPE (ptr_type);
          else
            {
              if (! type)
                {
                  type = make_node (LANG_TYPE);
                  TYPE_NAME (type) = name;
                }
              TREE_TYPE (ptr_type) = type;
              TYPE_POINTER_TO (type) = ptr_type;
              layout_type (ptr_type);
            }

          /* Also create a reference type for this target.  It must be patched
           * together with the pointer type.
           */
          ref_name = concat ("ref_xref_", IDENTIFIER_POINTER (name), NULL_PTR);
          ref_type = xref_tag (REFERENCE_TYPE, get_identifier (ref_name));
          if (TREE_TYPE (ref_type))
            type = TREE_TYPE (ref_type);
          else
            {
              if (! type)
                {
                  type = make_node (LANG_TYPE);
                  TYPE_NAME (type) = name;
                }
              TREE_TYPE (ref_type) = type;
              TYPE_REFERENCE_TO (type) = ref_type;
              layout_type (ref_type);
            }

          free (ptr_name);
          free (ref_name);
        }
      else
        {
          error ("type name expected, identifier `%s' given",
                 IDENTIFIER_POINTER (type));
          type = error_mark_node;
        }
    }

  if (TREE_CODE (idlist) == CALL_EXPR)
    {
      tree p;
      /* idlist == CALL_EXPR
       * type == TREE_LIST (NULL_TREE, type_node)
       */
      parm = build_tree_list (build_tree_list (type, idlist),
                              build_tree_list (NULL_TREE, NULL_TREE));

      push_parm_decl (parm);

      p = lookup_name (TREE_OPERAND (idlist, 0));

      if (!p || TREE_CODE (p) != PARM_DECL)
        abort ();

      /* This is a standard routine parameter.
       * Auto-call it on reference.
       */
      PASCAL_PROCEDURE_PARAMETER (p) = 1;
    }
  else
    {
      tree index_type = NULL_TREE, element_type = NULL_TREE;

      /* For open arrays, pass an implicit length (index) parameter.
       */
      if (PASCAL_TYPE_OPEN_ARRAY (type))
        {
          index_type = copy_node (integer_type_node);
          PASCAL_TYPE_OPEN_ARRAY (index_type) = 1;
          index_type = build_tree_list (NULL_TREE, index_type);
          element_type = TREE_TYPE (type);
        }

      /* If this is a protected parameter, make it read-only.
       */
      if (TREE_CODE (type) != ERROR_MARK
          && protected)
        {
          tree new_type = build_type_variant (type, 1, TYPE_VOLATILE (type));
          if (new_type != type
              && TYPE_LANG_SPECIFIC (new_type)
              && TYPE_LANG_SPECIFIC (new_type) == TYPE_LANG_SPECIFIC (type))
            {
              TYPE_LANG_SPECIFIC (new_type) = allocate_type_lang_specific ();
              memcpy (TYPE_LANG_SPECIFIC (new_type), TYPE_LANG_SPECIFIC (type),
                      sizeof (struct lang_type));
              TYPE_LANG_INTERFACE (new_type) = NULL_TREE;
              TYPE_LANG_UID (new_type) = 0;
            }
          type = new_type;
        }

      if (TREE_CODE (type) != ERROR_MARK
          && (varparm
              || PASCAL_TYPE_UNDISCRIMINATED_STRING (type)
              || PASCAL_TYPE_UNDISCRIMINATED_SCHEMA (type)
              || PASCAL_TYPE_OPEN_ARRAY (type)))
        {
          type = build_reference_type (type);
          if (! varparm)
            {
              type = copy_node (type);
              PASCAL_TYPE_VAL_REF_PARM (type) = 1;
            }
        }
      else if (TREE_CODE (type) == LANG_TYPE  /* an object which is forward referenced from a method's parameter list */
               || PASCAL_TYPE_OBJECT (type))
	{
	  error ("objects must not be value parameters");

	  /* This type will be replaced later when fixing forward types.
	   * `error_mark_node' causes "type defaults to `int'" in
	   * push_parm_decl() below.  (Why???)
           */
	  if (TREE_CODE (type) == LANG_TYPE)
	    type = integer_type_node;
	}

      type = chainon (declspecs, build_tree_list (NULL_TREE, type));

      for (link = idlist; link; link = TREE_CHAIN (link))
        {
          if (index_type && element_type)
            {
              /* Open array.  Implicitly pass the length (index).
               */
              int old_value;
              tree uname = get_unique_identifier ("open_array_length", 0);
              parm = build_tree_list (build_tree_list (index_type, uname),
                                      build_tree_list (NULL_TREE, NULL_TREE));
              push_parm_decl (parm);

              old_value = immediate_size_expand;
              immediate_size_expand = 0;
              size_volatile++;

              /* Re-build the array type using the length above as the
               * upper bound.
               */
              type = build_array_type (element_type,
                  build_range_type (integer_type_node, integer_zero_node,
                                    lookup_name (uname)));
              if (protected)
                type = build_type_variant (type, 1, TYPE_VOLATILE (type));
              type = build_reference_type (type);
              if (! varparm)
                {
                  type = copy_node (type);
                  PASCAL_TYPE_VAL_REF_PARM (type) = 1;
                }
              type = chainon (declspecs, build_tree_list (NULL_TREE, type));

              size_volatile--;
              immediate_size_expand = old_value;
            }
          parm = build_tree_list (build_tree_list (type, TREE_VALUE (link)),
                                  build_tree_list (NULL_TREE, NULL_TREE));
          push_parm_decl (parm);

          parm = lookup_name (TREE_VALUE (link));
          prediscriminate_schema (parm, 0);
        }
    }
}



/*
 * Initializers.
 */

/* For `New':  Assign values to tag fields of a variant record.
 */
void
assign_tags (record, tags)
     tree record, tags;
{
  if (record
      && TREE_TYPE (record)
      && PASCAL_TYPE_VARIANT_RECORD (TREE_TYPE (record))
      && tags)
    {
      tree rtype = TREE_TYPE (record);
      tree tag_info = get_variant_tag_info (rtype);
      if (tag_info
          && TREE_CODE (tag_info) == TREE_LIST
          && TREE_PURPOSE (tag_info))
        {
          tree tag_constant, variant, value, tag_field;
          do
            {
              tag_field = TREE_VALUE (TREE_PURPOSE (tag_info));
              value = string_may_be_char (TREE_VALUE (tags));
              if (TREE_CODE (value) != INTEGER_CST)
                pedwarn ("tag value must be a constant of ordinal type");
              if (tag_field)
                {
                  tag_field = build_component_ref (record, DECL_NAME (tag_field));
                  expand_expr_stmt (build_modify_expr (tag_field, NOP_EXPR, value));
                }
              tags = TREE_CHAIN (tags);
              if (tags)
                {
                  tag_constant = TREE_PURPOSE (TREE_VALUE (tag_info));
                  variant = NULL_TREE;
                  while (tag_constant
                         && (/* Should not happen; just to prevent crashes. */
                             TREE_CODE (TREE_VALUE (tag_constant)) != INTEGER_CST
                             || TREE_INT_CST_LOW (TREE_VALUE (tag_constant))
                                    != TREE_INT_CST_LOW (value)
                             || TREE_INT_CST_HIGH (TREE_VALUE (tag_constant))
                                    != TREE_INT_CST_HIGH (value)))
                    tag_constant = TREE_CHAIN (tag_constant);
                  if (tag_constant
                      && TREE_CODE (TREE_VALUE (tag_constant)) == INTEGER_CST)
                    {
                      variant = TREE_PURPOSE (tag_constant);
                      if (variant
                          && TREE_CODE (TREE_TYPE (variant)) == RECORD_TYPE
                          && PASCAL_TYPE_VARIANT_RECORD (TREE_TYPE (variant)))
                        tag_info = get_variant_tag_info (TREE_TYPE (variant));
                      else
                        pedwarn ("too many variant tags");
                    }
                }
            }
          while (tags);
        }
      else
        pedwarn ("ignoring variant tags for non-variant record");
    }
}

/* Internal routine to un_initialize_block.
 *
 * This one does the initialization of the object
 * (or a component of an object) specified in decl.
 * This type is a BASIC type, i.e. not a structured type
 * that has been exploded up by init_any ()
 */
static void
init_simple (decl, the_end)
     tree decl;
     int the_end;
{
    tree type = TREE_TYPE (decl);
    enum tree_code code = TREE_CODE (type);

    /* Clean up only local files */
    if (the_end && code != FILE_TYPE)
        return; /* Forget them! */

    switch (code) {
    case RECORD_TYPE:
    case ARRAY_TYPE:
        abort ();
    case FILE_TYPE :
        {
            char *temp;
            tree fname;
            int  byte_size = 0;
            tree file_size;
            tree file_kind;
            tree file_type = TREE_TYPE (type);

            if (the_end)
              {
                /* Close the files on exit */
                build_rts_call (p_DONEFDR,
                                build_tree_list (NULL_TREE, decl));
                break;
              }

            /* Always pass the internal name to the run time system,
             * not only for external files.
             */
            if (TREE_CODE (decl) == VAR_DECL)
              temp = IDENTIFIER_POINTER (DECL_NAME (decl));
            else if (TREE_CODE (decl) == INDIRECT_REF)
              temp = "<Allocated from heap>";
            else
              temp = "<Name unknown>";

            fname = build_string (strlen (temp)+1, temp);
            TREE_TYPE (fname) = char_array_type_node;
            fname = build_tree_list (NULL_TREE, fname);

            if (pedantic && PASCAL_TYPE_PACKED (type))
              warning ("`packed' has no effect on external file format");

            /* Make a TREE_LIST node of the size of the file buffer */
            /* the size is in bits, if the file is packed, else bytes */
            /* (the run time system does not pack files yet, so it converts */
            /*  sizes back to bytes) */

#if 0
            /* @@@ This feature does not work...
             *
             * Check if it is a subrange
             * @@ Only constant bounds accepted.
             *
             * Tells the run time system that the file buffer
             * actually fits in one byte, even if the file
             * element size is larger (this is used for
             * integer subranges, e.g: 0..255
             */
            if (TREE_CODE (file_type) == INTEGER_TYPE
                && TREE_TYPE (file_type)
                && TREE_CODE (TREE_TYPE (file_type)) == INTEGER_TYPE)
              {
                tree lo = TYPE_MIN_VALUE (file_type);
                tree hi = TYPE_MAX_VALUE (file_type);

                if (TREE_CODE (lo) == INTEGER_CST
                    && TREE_CODE (hi) == INTEGER_CST
                    && TREE_INT_CST_LOW (lo) >= 0)
                  {
                    int h = TREE_INT_CST_LOW (hi);

                    if (h <= (1 << BITS_PER_UNIT))
                      byte_size = 1;
                  }
              }
#endif /* 0 */

            if (TREE_CODE (file_type) == VOID_TYPE)
              {
                file_size =
                  build_tree_list (NULL_TREE, integer_one_node);
              }
            else
              {
                file_size =
                  build_tree_list
                    (NULL_TREE,
                     (! PASCAL_TYPE_PACKED (type) ?
                      size_in_bytes (file_type) :
                      fold (build (MULT_EXPR, sizetype,
                                   size_in_bytes (file_type),
                                   size_int (BITS_PER_UNIT)))));
              }

            file_kind =
                build_tree_list
                  (NULL_TREE,
                   size_int (  !!TYPE_FILE_TEXT (type)       << fkind_TEXT
                             | PASCAL_TYPE_PACKED (type)     << fkind_PACKED
                             | PASCAL_EXTERNAL_OBJECT (decl) << fkind_EXTERN
                             | !!TYPE_DOMAIN (type)          << fkind_DIRECT
                             | byte_size                     << fkind_BYTE
                             | 1 /*@@*/                      << fkind_BINDABLE
                             | !!flag_transparent_file_names << fkind_FILENAME));

            /* Call and construct parameters to the run time system
             * routine that initializes a file buffer to a known state
             */
            expand_expr_stmt
              (rts_call (r_INITFDR,
                         void_type_node,
                         chainon (do_ptype (type, 1, 0),
                          chainon (do_ptype (cstring_type_node, 0, 0),
                           chainon (do_ptype (integer_type_node, 0, 0),
                                    do_ptype (integer_type_node, 0, 1)))),
                         tree_cons (NULL_TREE, decl,
                                    chainon (fname,
                                             chainon (file_size, file_kind)))));
            break;
        }
        default :
            /* nothing yet!!! */ ;
    }
}

/* Subroutine of init_any */
static void
init_record (thing, the_end, base)
     tree thing;
     int the_end;
     tree base;
{
  tree field;

  for (field = TYPE_FIELDS (TREE_TYPE (thing));
       field;
       field = TREE_CHAIN (field))
    {
      if (DECL_NAME (field))
        init_any (build_component_ref (base, DECL_NAME (field)), the_end);
      else
        {
          tree inner_type = TREE_TYPE (field);
          if (TREE_CODE (inner_type) == RECORD_TYPE
              || TREE_CODE (inner_type) == UNION_TYPE)
            init_record (field, the_end, base);
          else
            warning ("strange unnamed field in record");
        }
    }
}

/* Return nonzero if TYPE contains a FILE_TYPE.
 */
int
contains_file_p (type)
     tree type;
{
  if (! type)
    return 0;
  switch (TREE_CODE (type))
    {
      case FILE_TYPE:
        return 1;
      case ARRAY_TYPE:
        return contains_file_p (TREE_TYPE (type));
      case RECORD_TYPE:
      case UNION_TYPE:
        {
          tree field;

          for (field = TYPE_FIELDS (type); field; field = TREE_CHAIN (field))
            {
              tree inner_type = TREE_TYPE (field);

              if (!DECL_NAME (field) &&
                  (TREE_CODE (inner_type) != RECORD_TYPE
                   && TREE_CODE (inner_type) != UNION_TYPE))
                abort ();

              if (contains_file_p (inner_type))
                return 1;
            }
          break;
        }
      default:
        break;
    }
  return 0;
}

/* Return nonzero if TYPE contains a FILE_TYPE or a schema
 * that needs to be initialized.
 */
int
contains_file_or_schema_p (type)
     tree type;
{
  enum tree_code code = TREE_CODE (type);
  switch (code)
    {
      case FILE_TYPE:
        return 1;
      case ARRAY_TYPE:
        return contains_file_or_schema_p (TREE_TYPE (type));
      case RECORD_TYPE:
      case UNION_TYPE:
        {
          tree field;

          if (PASCAL_TYPE_STRING (type) || PASCAL_TYPE_SCHEMA (type))
            return 1;

          for (field = TYPE_FIELDS (type); field; field = TREE_CHAIN (field))
            {
              tree inner_type = TREE_TYPE (field);

              if (!DECL_NAME (field) &&
                  (TREE_CODE (inner_type) != RECORD_TYPE
                   && TREE_CODE (inner_type) != UNION_TYPE))
                abort ();

              if (contains_file_or_schema_p (inner_type))
                return 1;
            }
          break;
        }
      default:
        break;
    }
  return 0;
}

/* Subroutine of un_initialize_block, but also called separately.
 *
 * Recursively propagate through a structured type
 * un_initializing all simple types with init_simple().
 */
void
init_any (thing, the_end)
     tree thing;
     int the_end;
{
  tree type = TREE_TYPE (thing);
  enum tree_code code = TREE_CODE (type);

  /* Only FILE_TYPE and schema variables are initialized here.
   * This check is good for arrays that don't contain those
   * because otherwise we would create an empty loop.
   */
  if (! contains_file_or_schema_p (type))
    return;

  switch (code)
    {
      case FILE_TYPE:
        init_simple (thing, the_end);
        break;

      case ARRAY_TYPE:
        {
          tree domain = TYPE_DOMAIN (type);
          if (domain && domain != error_mark_node)
            {
              tree index = make_new_variable ("init_index", TREE_TYPE (domain));
              tree condition;
              tree index_as_integer;

              /* Choose or create an integer type of the same
               * size and sign as the index type.
               */
              index_as_integer = type_for_size (TYPE_PRECISION (TREE_TYPE (index)),
                                                TREE_UNSIGNED (TREE_TYPE (index)));
              if (! index_as_integer)
                abort ();

              index_as_integer = convert (index_as_integer, index);

              expand_expr_stmt (build_modify_expr (index_as_integer, NOP_EXPR,
                                                   TYPE_MIN_VALUE (domain)));
              expand_start_loop_continue_elsewhere (1);
              init_any (build_pascal_array_ref (thing,
                                                build_tree_list (NULL_TREE,
                                                                 index)),
                        the_end);
              expand_loop_continue_here ();
              condition = build_pascal_binary_op (LT_EXPR, index,
                                                  TYPE_MAX_VALUE (domain));
              expand_exit_loop_if_false (0, condition);
              expand_expr_stmt (build_modify_expr (index_as_integer, PLUS_EXPR,
                                                   integer_one_node));
              expand_end_loop ();
            }
          break;
        }

      case RECORD_TYPE:
        if (PASCAL_TYPE_DISCRIMINATED_STRING (type))
          {
            /* String schema discriminant identifier initializing.
             * The capacity is one less than the actual space in the
             * array to allow for a terminating chr ( 0 ).
             */
            if (! the_end)
              expand_expr_stmt
                (build_modify_expr
                 (PASCAL_STRING_CAPACITY (thing),
                  NOP_EXPR,
                  build_pascal_binary_op (MINUS_EXPR,
                      TYPE_MAX_VALUE (TYPE_DOMAIN (TREE_TYPE (PASCAL_STRING_VALUE (thing)))),
                      integer_one_node)));
            break;
          }
        else if (PASCAL_TYPE_DISCRIMINATED_SCHEMA (type))
          {
            tree field;
            if (! the_end)
              for (field = TYPE_FIELDS (type);
                   field && PASCAL_TREE_DISCRIMINANT (field);
                   field = TREE_CHAIN (field))
                expand_expr_stmt
                  (build_modify_expr
                   (build_component_ref (thing, DECL_NAME (field)),
                    NOP_EXPR,
                    PASCAL_DECL_FIXUPLIST (field)));
            /* FALLTHROUGH */
          }
        /* FALLTHROUGH */

      case UNION_TYPE:  /* In Pascal only in variant records */
        init_record (thing, the_end, thing);
        break;
      default:
        /* NOTHING */ ;
    }
}

/*
 * This code will (some day) un-initialize all variables
 * declared in the current block level.
 *
 * It does not do it yet, however, except for FILE_TYPE
 * nodes that won't work if not un-initialized.
 *
 * THE_END is nonzero if this is called to clean up after
 * the block's statements have been executed.
 *
 *  This also initializes files in
 *   1) components of structured types and
 *   2) parts of dynamic objects.
 *
 * Never initialize `absolute' variables.
 */
void
un_initialize_block (names, the_end)
     tree names;
     int the_end;
{
  tree decl;
  for (decl = names; decl; decl = TREE_CHAIN (decl))
    if (TREE_CODE (decl) == VAR_DECL
        && ! DECL_EXTERNAL (decl)
        && ! (DECL_INITIAL (decl)
              && PASCAL_ABSOLUTE_CLAUSE (DECL_INITIAL (decl))))
      init_any (decl, the_end);
}

/*
 * Recursive subroutine of build_pascal_initializer():
 * Process one level (array or record) of a structured initializer.
 */
static void
process_init_list (init)
     tree init;
{
  while (init)
    {
      if (TREE_CODE (TREE_VALUE (init)) == TREE_LIST)
        {
          push_init_level (0);
          process_init_list (TREE_VALUE (init));
          process_init_element (pop_init_level (0));
        }
      else
        process_init_element (TREE_VALUE (init));
      init = TREE_CHAIN (init);
    }
}

/*
 * Build the actual constructor for the variable declaration DECL out of
 * the tree list INIT.  It is assumed that check_pascal_initializer()
 * already has been called for INIT and returned 0; otherwise this may crash.
 */
tree
build_pascal_initializer (decl, init)
     tree decl, init;
{
  if (!init || init == error_mark_node)
    return NULL_TREE;  /* avoid further error messages */

  if (TREE_CODE (init) != TREE_LIST)
    {
      error ("Internal GPC error: invalid initializer");
      return NULL_TREE;
    }

  start_init (decl, NULL_TREE, global_bindings_p ());

  if (TREE_CODE (TREE_VALUE (init)) == TREE_LIST)
    {
      /* Structured initializer.
       * Do (almost) what c-parse.y does.
       */
      really_start_incremental_init (NULL_TREE);
      /*
       * The call to clear_momentary is in process_init_element.
       * (Whatever that means.;-)
       */
      push_momentary ();
      if (PASCAL_TYPE_SCHEMA (TREE_TYPE (decl)))
        {
          set_init_label (schema_id);
          push_init_level (0);
          process_init_list (TREE_VALUE (init));
          process_init_element (pop_init_level (0));
        }
      else
        process_init_list (TREE_VALUE (init));
      init = pop_init_level (0);
      if (init == error_mark_node)
        /*
         * In c-parse.y:  && ! (yychar == STRING || yychar == CONSTANT)
         * Let's hope that it was not too important ...
         */
        pop_momentary ();
      else
        pop_momentary_nofree ();
    }
  else if (PASCAL_TYPE_SCHEMA (TREE_TYPE (decl)))
    {
      really_start_incremental_init (NULL_TREE);
      push_momentary ();
      set_init_label (schema_id);
      process_init_element (TREE_VALUE (init));
      init = pop_init_level (0);
      if (init == error_mark_node)
        pop_momentary ();
      else
        pop_momentary_nofree ();
    }
  else
    init = TREE_VALUE (init);

  finish_init ();
  return init;
}


/*
 * Procedure and Function calls.
 */

/* Enables/disables procedure/function evaluation when
 * the routines have no parameters.
 * Reason:
 *   1) When parsing an LEX_ID it may be either a routine
 *      name or a variable name.
 *   2) When passing such an item to a routine, it may have to be
 *      evaluated or not depending of the corresponding FORMAL
 *      parameter type; if this is a procedural parameter, the
 *      routine should be passed, if not, the routine should be
 *      evaluated, and the result passed.
 */
static int evaluate_function_calls = 1;

/* Disables the evaluation of procedure/function calls with no parameters
 * and returns the old value of the enable/disable flag.
 *
 * They need to be stacked, because routine calls can be nested :-)
 */
int
suspend_function_calls ()
{
  int old = evaluate_function_calls;
  evaluate_function_calls = 0;
  return old;
}

/* Enables the evaluation of procedure/function calls with no parameters
 * and returns the old value of the enable/disable flag.
 *
 * This is necessary when parsing constructs like `&foo^.bar' where `foo'
 * is a function which returns a record containing a field `bar'.  In this
 * case, `foo' must be CALLED although it follows an address operator.
 */
int
allow_function_calls ()
{
  int old = evaluate_function_calls;
  evaluate_function_calls = 1;
  return old;
}

/* Resumes routine calls for routines with no parameters
 * if STATE is nonzero.
 *
 * STATE should be the value previously returned by
 * suspend_function_calls() or allow_function_calls().
 */
void
resume_function_calls (state)
     int state;
{
  evaluate_function_calls = state;
}

/* Return the return type of the function FUN which may be a
 * function decl, a reference or a pointer to a function.
 */
tree
function_return_type (fun)
     tree fun;
{
  tree type = TREE_TYPE (fun);
  if (TREE_CODE (type) == POINTER_TYPE
      || TREE_CODE (type) == REFERENCE_TYPE)
    type = TREE_TYPE (type);
  if (TREE_CODE (type) != FUNCTION_TYPE)
    abort ();
  return TREE_TYPE (type);
}

/* Maybe call the function, or pass it as a routine parameter, or
 * assign it - or its address - to some lvalue.
 *
 * The problem is that the corresponding argument type is not known
 * when the factor is parsed.  Neither is it known if this is part
 * of an expression ...
 */
tree
maybe_call_function (fun, args)
     tree fun;
     tree args;
{
  tree temp = fun;

  /* 1) This is a procedure statement without parameters.
   * 2) This is an assignment from a function with no parameters
   *    to some variable, or passing the RESULT of such function
   *    to another routine.  @@@@ It may also have args, for
   *    function pointer calls. @@@@
   * 3) This procedure or function is being passed to a procedural
   *    parameter list.
   * 4) This is the assignment of the ADDRESS of a function to
   *    some lvalue.
   * 5) This is the assignment of the function itself to a procedural
   *    variable.
   * 6) This is a procedure call without parameters through a
   *    procedural variable.
   *
   * The problem is this:
   *   in 1, 2, and 6 above the procedure/function should be CALLED;
   *   in 3, 4, and 5 it should NOT be called.
   */

  if (evaluate_function_calls)
    if (TREE_CODE (fun) == FUNCTION_DECL
        || args
        || (TREE_CODE (fun) == PARM_DECL
            && PASCAL_PROCEDURE_PARAMETER (fun))
        || PASCAL_PROCEDURAL_VARIABLE (fun))
      {
        if (args == empty_arglist)
          args = NULL_TREE;
        if (PASCAL_PROCEDURAL_VARIABLE (fun)
            || TREE_CODE (TREE_TYPE (fun)) == POINTER_TYPE)
          fun = build_indirect_ref (fun, "indirect procedure call");
        temp = build_function_call (fun, args);
      }

  return temp;
}

/* A problem with the function calls again...
 * If a forward declared/external function is used in an expression that is
 * part of some function arguments it will not be called by the routine
 * maybe_call_function().
 *
 * The function probably_call_function() is used when we know that the function
 * is not a function parameter but rather should be evaluated.
 */
tree
probably_call_function (fun)
     tree fun;
{
  tree t = TREE_TYPE (fun);

  /* If this is a function without parameters, call it */
  if (TREE_CODE (t) == FUNCTION_TYPE
      && TREE_CODE (TREE_TYPE (t)) != VOID_TYPE
      && TYPE_ARG_TYPES (t)
      && TREE_CODE (TREE_VALUE (TYPE_ARG_TYPES (t))) == VOID_TYPE)
    {
      fun = build_function_call (fun, NULL_TREE);
    }

  return fun;
}

#ifdef EGCS95
struct function *
maybe_find_function_data (decl)
     tree decl;
{
  struct function *p;
  for (p = outer_function_chain; p; p = p->next)
    if (p->decl == decl)
      return p;
  return (struct function *)NULL;
}
#endif



/*
 * Main program.
 */

/* Call `setjmp' for each global label.
 *
 *     for (all labels in the main program)
 *       if (setjmp (global_jmp_buf))
 *         goto local_label;
 */
void
do_setjmp (void)
{
  tree global_label;
  for (global_label = main_program_nonlocal_labels;
       global_label; global_label = TREE_CHAIN (global_label))
    {
      char *jmpbuf_name;
      tree id, jmpbuf_id, jmpbuf_var, condition, setjmp_fun, setjmp_param_list;

      id = DECL_NAME (TREE_VALUE (global_label));
      jmpbuf_name = concat ("jmpbuf_", IDENTIFIER_POINTER (id), NULL_PTR);
      jmpbuf_id = get_identifier (jmpbuf_name);
      jmpbuf_var = lookup_name (jmpbuf_id);

      /* Avoid calling `setjmp' for labels that are only used in
       * the body of the main program.
       */
      if (jmpbuf_var && TREE_USED (jmpbuf_var))
        {
          setjmp_fun = lookup_name (get_identifier ("__builtin_setjmp"));
          setjmp_param_list = build_tree_list (NULL_TREE,
                                               build_unary_op (ADDR_EXPR,
                                                               jmpbuf_var, 0));
          condition = build_function_call (setjmp_fun, setjmp_param_list);
          expand_start_cond (condition, 0);
          expand_goto (TREE_VALUE (global_label));
          expand_end_cond ();
        }
    }
}

/* Construct the following function:
 *
 *   extern void _p_initialize ();
 *
 *   int
 *   main_program_name (int argc, char **argv, char **envp)
 *   {
 *     _p_initialize (argc, argv, envp);
 *     program_Name ();
 *   }
 *
 * This method also makes gcc2 call any special main-kludges,
 * like __main ().
 *
 * main_program_name is usually "main" but can be redefined
 * with the --gpc-main=foo option.  This allows for working
 * with libraries that export their own `main()' function.
 */
void
output_real_main_program (name)
     tree name;
{
  tree parameters, declspecs, declarator;
  tree fun, param_list;
  tree string_ptr_type_node = build_pointer_type (string_type_node);
  tree argc = get_identifier ("argc");
  tree argv = get_identifier ("argv");
  tree envp = get_identifier ("envp");
  tree init_id = get_identifier ("_p_initialize");

  /* This is basically what implicitly_declare() does
   * but does not give warnings...
   */
  push_obstacks_nochange ();
  end_temporary_allocation ();

  fun = build_decl (FUNCTION_DECL, init_id, default_function_type);

  DECL_EXTERNAL (fun) = 1;
  TREE_PUBLIC (fun) = 1;

  /* Record that we have an implicit decl and this is it.  */
  IDENTIFIER_IMPLICIT_DECL (init_id) = fun;

  pushdecl (fun);

  rest_of_decl_compilation (fun, NULL_PTR, 0, 0);

  pop_obstacks ();

  assemble_external (fun);

  /* Construct implicit main program that is invoked as
   *   main_program_name (int argc, char **argv, char **envp)
   */
  pushlevel (0);
  clear_parm_order ();
  declare_parm_level (1);

  push_parm_decl (build_tree_list
                  (build_tree_list (build_tree_list (NULL_TREE,
                                                     integer_type_node),
                                    argc),
                   build_tree_list (NULL_TREE, NULL_TREE)));

  push_parm_decl (build_tree_list
                  (build_tree_list (build_tree_list (NULL_TREE,
                                                     string_ptr_type_node),
                                    argv),
                   build_tree_list (NULL_TREE, NULL_TREE)));

  push_parm_decl (build_tree_list
                  (build_tree_list (build_tree_list (NULL_TREE,
                                                     string_ptr_type_node),
                                    envp),
                   build_tree_list (NULL_TREE, NULL_TREE)));

  parameters = get_parm_info (1);
  poplevel (0, 0, 0);

  declspecs = tree_cons (NULL_TREE, integer_type_node, NULL_TREE);

  declarator = build_nt (CALL_EXPR, get_identifier (gpc_main), parameters,
                         NULL_TREE);

  if (! start_function (declspecs, declarator, NULL_TREE, NULL_TREE, 0))
    abort ();

  store_parm_decls ();

  pushlevel (0);
  expand_start_bindings (0);

  param_list = tree_cons (NULL_TREE,
                          lookup_name (argc),
                          tree_cons (NULL_TREE,
                                     lookup_name (argv),
                                     tree_cons (NULL_TREE,
                                                lookup_name (envp),
                                                NULL_TREE)));

  /* Call _p_initialize (argc, argv, envp).
   */
  expand_expr_stmt (build_function_call (fun, param_list));

  /* Call the main program's constructor which will call the
   * other constructors.  No arguments.
   */
  emit_library_call (gen_rtx (SYMBOL_REF, Pmode, "init_pascal_main_program"),
                     0, VOIDmode, 0);

  /* Call the Pascal main program.  No arguments.
   */
  emit_library_call (gen_rtx (SYMBOL_REF, Pmode, IDENTIFIER_POINTER (name)),
                     0, VOIDmode, 0);

  /* Call the run time system finalization routine.  No arguments.
   */
  emit_library_call (gen_rtx (SYMBOL_REF, Pmode, "_p_finalize"),
                     0, VOIDmode, 0);

  expand_end_bindings (NULL_TREE, 0, 0);
  poplevel (0, 0, 0);

  c_expand_return (integer_zero_node);

  finish_function (0);
}

/* ISO Standard requires that the program_name is in a separate name
 * space, so we do not use "Name" but the special name "pascal_main_program".
 * All other Pascal identifiers start with a capital letter and have
 * lowercase otherwise, so clashes cannot occur (except with AsmName).
 *
 * Return the new NAME.
 */
tree
get_main_program_name (name)
     tree name;
{
  return get_identifier ("pascal_main_program");
}

/* Expand a non-local `goto' to a label in the main program.
 * This is essentially the same as `expand_goto' in `../stmt.c'
 * (and should belong there) but specialized for this situation.
 */
void
expand_goto_main_program (label)
     tree label;
{
  /* Reference a jmp_buf variable `jmpbuf_ID' for this label
   * which has been declared with the label.
   */
  char *jmpbuf_name = concat ("jmpbuf_", IDENTIFIER_POINTER (DECL_NAME (label)),
                              NULL_PTR);
  tree jmpbuf_id = get_identifier (jmpbuf_name);
  tree jmpbuf_var = lookup_name (jmpbuf_id);
  tree longjmp_fun, longjmp_param_list;

  main_program_has_nonlocal_label = 1;
  main_program_needs_context = 0;
  current_function_has_nonlocal_goto = 1;

  /* Call `longjmp'.
   */
  longjmp_fun = lookup_name (get_identifier ("__builtin_longjmp"));
  longjmp_param_list = build_tree_list (NULL_TREE,
                                        build_unary_op (ADDR_EXPR,
                                                        jmpbuf_var, 0));
  longjmp_param_list = chainon (longjmp_param_list,
                                build_tree_list (NULL_TREE,
                                                 integer_one_node));
  expand_expr_stmt (build_function_call (longjmp_fun, longjmp_param_list));
}

/* Expand a Pascal `goto' statement.
 */
void
pascal_expand_goto (label_name)
     tree label_name;
{
  tree decl = lookup_label (label_name);
  if (decl)
    {
      TREE_USED (decl) = 1;
      if (current_module->main_program
          && DECL_INITIAL (decl) == main_program_context
          && DECL_NAME (current_function_decl) != main_program_name)
        expand_goto_main_program (decl);
      else
        expand_goto (decl);
    }
}

/* Do this at the end of each program_component (program, unit,
 * or module).
 */
void
finish_program_component ()
{
  enable_keyword ("Module");
  enable_keyword ("Unit");
  enable_keyword ("Library");
  disable_keyword ("Interface");
  disable_keyword ("Implementation");
}

/* Exit compilation as successfully as reasonable.
 */
void exit_compilation ()
{
  if (errorcount)
    exit (FATAL_EXIT_CODE);
  if (sorrycount)
    exit (FATAL_EXIT_CODE);
  exit (SUCCESS_EXIT_CODE);
}



/*
 * GPC startup.
 */

/* Initialize misc. flags used in here.
 * size_of_file_type is also used in gpc-decl.c (when making TEXT file)
 */
void
init_util ()
{
  int i;

  size_of_file_type = size_int (FDR_Size * BITS_PER_UNIT);
#ifdef EGCS
  size_unit_of_file_type = size_int (FDR_Size);
#endif

  /* set the default integer set size, if not set with -fsetlimit:NNNNN option
   */
  integer_set_size = requested_set_size > 0
                        ? size_int (requested_set_size)
                        : size_int (DEFAULT_SET_SIZE);

  /* On integer sets of unknown size (i.e. constructors with integers in them)
   * we use the following default set
   */
  integer_set_type_node =
    build_set_type
         (build_tree_list
               (integer_type_node,
                INT_RANGE_TYPE (integer_zero_node,
                                TREE_INT_CST_LOW (integer_set_size) - 1)));

  /* Create symbol refs for Pascal run time routines */
  for (i = 0; rts[i].name; i++)
    rts[i].symref = gen_rtx (SYMBOL_REF, Pmode, rts[i].name);
}

/* This initialization is run once per compilation just when types
 * are declared.
 *
 * init_util() is too early, so call this from lang_init ().
 * @@@ rs-6000 causes core dumps with -g option when finish_struct() is called
 * there, asm_out_file is uninitialized in routine varasm.c(text_section)
 */
void
pascal_init ()
{
  tree temp;

  init_language_specification ();

#ifdef EGCS95
  /* Set alignment.
   */
#ifndef LONG_TYPE_SIZE
#define LONG_TYPE_SIZE BITS_PER_WORD
#endif
  set_word_size = LONG_TYPE_SIZE;
  set_alignment = LONG_TYPE_SIZE;
  set_words_big_endian = 0;
#endif /* EGCS95 */

  /* A special node that forces a call through a function pointer that
   * has no arguments.
   */
  empty_arglist = build_tree_list (NULL_TREE, NULL_TREE);

  /* A unique prototype string schema.
   */
  size_volatile++;
  string_schema_proto_type = build_pascal_string_schema (integer_zero_node);
  TYPE_LANG_BASE (string_schema_proto_type) = string_schema_proto_type;
  size_volatile--;

  /* A read-only variant of this.
   */
  const_string_schema_proto_type = build_type_variant (string_schema_proto_type,
                                                       1, 0);

  /* Pointer to integer.
   */
  integer_ptr_type_node = build_pointer_type (integer_type_node);

  /* A canonical-string-type that DATE always returns */
  gpc_type_DATE = grok_packed
                   (build_pascal_array_type
                    (char_type_node,
                     build_tree_list (NULL_TREE,
                                      INT_RANGE_TYPE (integer_one_node,
                                                      GPC_DATE_LENGTH))));

  /* A canonical-string-type that TIME always returns */
  gpc_type_TIME = grok_packed
                    (build_pascal_array_type
                     (char_type_node,
                      build_tree_list (NULL_TREE,
                                       INT_RANGE_TYPE (integer_one_node,
                                                       GPC_TIME_LENGTH))));


  /* Fields of the required RECORD_TYPE TimeStamp */
  temp  =
    chainon
      (DO_FIELD ("Datevalid", boolean_type_node),
       chainon
       (DO_FIELD ("Timevalid", boolean_type_node),
        chainon
        (DO_FIELD ("Year", integer_type_node),
         chainon
         (DO_FIELD ("Month", INT_RANGE_TYPE (integer_one_node, 12)),
          chainon
          (DO_FIELD ("Day",   INT_RANGE_TYPE (integer_one_node, 31)),
           chainon
           (DO_FIELD ("Dayofweek", INT_RANGE_TYPE (integer_zero_node, 6)),
            chainon
            (DO_FIELD ("Hour",   INT_RANGE_TYPE (integer_zero_node, 23)),
             chainon
             (DO_FIELD ("Minute", INT_RANGE_TYPE (integer_zero_node, 59)),
              chainon
              (DO_FIELD ("Second", INT_RANGE_TYPE (integer_zero_node, 61)),
               DO_FIELD ("Microsecond", INT_RANGE_TYPE (integer_zero_node, 999999)))))))))));

  /* Create the packed RECORD_TYPE, we have the fields */
  temp = grok_packed (finish_struct (start_struct (RECORD_TYPE,
                                                   NULL_TREE),
                                     temp, NULL_TREE));

  /* Handled like other predefined types in gpc-lex.c */
  gpc_type_TIMESTAMP = temp;

  temp = build_pascal_string_schema (build_int_2 (BINDING_NAME_LENGTH, 0));

  /* Fields of the required RECORD_TYPE BindingType */
  temp = chainon (DO_FIELD ("Bound", boolean_type_node),
         chainon (DO_FIELD ("Force", boolean_type_node),
         chainon (DO_FIELD ("Extensions_valid", boolean_type_node),
         chainon (DO_FIELD ("Readable", boolean_type_node),
         chainon (DO_FIELD ("Writable", boolean_type_node),
         chainon (DO_FIELD ("Executable", boolean_type_node),
         chainon (DO_FIELD ("Existing", boolean_type_node),
         chainon (DO_FIELD ("Directory", boolean_type_node),
         chainon (DO_FIELD ("Special", boolean_type_node),
         chainon (DO_FIELD ("Symlink", boolean_type_node),
         chainon (DO_FIELD ("Accesstime", long_long_integer_type_node),
         chainon (DO_FIELD ("Modificationtime", long_long_integer_type_node),
         chainon (DO_FIELD ("Changetime", long_long_integer_type_node),
         chainon (DO_FIELD ("User", integer_type_node),
         chainon (DO_FIELD ("Group", integer_type_node),
         chainon (DO_FIELD ("Mode", integer_type_node),
         chainon (DO_FIELD ("Device", integer_type_node),
         chainon (DO_FIELD ("Inode", integer_type_node),
         chainon (DO_FIELD ("Textbinary", boolean_type_node),
         chainon (DO_FIELD ("Handle", integer_type_node),
                  DO_FIELD ("Name", temp)))))))))))))))))))));

  /* Create the packed RECORD_TYPE, we have the fields */
  temp = grok_packed (finish_struct (start_struct (RECORD_TYPE,
                                                   NULL_TREE),
                                     temp, NULL_TREE));

  gpc_type_BINDINGTYPE = temp;

  declare_rts_types ();

  identifier_output = get_identifier ("Output");
  identifier_input  = get_identifier ("Input");
  identifier_stderr = get_identifier ("Stderr");

  standard_interface_input  = get_identifier ("Standardinput");
  standard_interface_output = get_identifier ("Standardoutput");
  standard_interface_error  = get_identifier ("Standarderror");

  {
    /* Obtain the input and output files initialized in
     * the run time system.
     */
    tree external_text = chainon (build_tree_list (NULL_TREE, text_type_node),
                                  build_tree_list (NULL_TREE, extern_id));

    tree iname = get_identifier ("_p_stdin");
    tree oname = get_identifier ("_p_stdout");
    tree ename = get_identifier ("_p_stderr");
    tree var;

    var = start_decl (iname, external_text, 0, NULL_TREE, NULL_TREE);
    finish_decl (var, NULL_TREE, NULL_TREE);
    PASCAL_EXTERNAL_OBJECT (var) = 1;
    global_input_file_node = var;

    var = start_decl (oname, external_text, 0, NULL_TREE, NULL_TREE);
    finish_decl (var, NULL_TREE, NULL_TREE);
    PASCAL_EXTERNAL_OBJECT (var) = 1;
    global_output_file_node = var;

    var = start_decl (ename, external_text, 0, NULL_TREE, NULL_TREE);
    finish_decl (var, NULL_TREE, NULL_TREE);
    PASCAL_EXTERNAL_OBJECT (var) = 1;
    global_error_file_node = var;
  }

  declare_known_ids ();

  if (flag_no_default_paths == 0)
    {
      /* Initialize the unit path from the directory we are living in.
       */
      char *p = save_string (save_argv[0]);
      char *q = p + strlen (p) - 1;
      while (q >= p && *q != DIR_SEPARATOR)
        q--;
      if (*q == DIR_SEPARATOR)
        {
          char *new_unit_path, *new_object_path;
          static char path_separator[2] = {PATH_SEPARATOR, 0};
          q[1] = 0;
          if (unit_path)
            {
              new_unit_path = concat (unit_path, path_separator,
                                      p, "units", NULL_PTR);
              free (unit_path);
              unit_path = new_unit_path;
            }
          else
            unit_path = concat (p, "units", NULL_PTR);
          if (object_path)
            {
              new_object_path = concat (object_path, path_separator,
                                        p, "units", NULL_PTR);
              free (object_path);
              object_path = new_object_path;
            }
          else
            object_path = concat (p, "units", NULL_PTR);
          *q = 0;
          new_unit_path = concat (unit_path, path_separator, p, NULL_PTR);
          free (unit_path);
          unit_path = new_unit_path;
          new_object_path = concat (object_path, path_separator, p, NULL_PTR);
          free (object_path);
          object_path = new_object_path;
        }
      free (p);
    }
}

/* Moved here from gpc-lex.c */

/* ISO Pascal does not deny redefinition of predefined type-names
 * or routines (e.g. you may define `type Integer = Char;')
 * so the RIDPOINTERS array is not useful in here.
 */
static void
declare_known_id (name, type, value)
     char *name;
     tree type;
     int value;
{
  tree id = get_identifier (name);
  tree decl;

  /* Preserve previous user declarations.
   */
  if (! lookup_name (id))
    {
      /* It really does not matter which DECL node it is, since
       * it has the attribute flag PASCAL_REDEFINABLE_DECL(NODE) set.
       * It's not used as a VAR_DECL node anyhow.
       */
      if (type)
        {
          decl = build_decl (TYPE_DECL, id, type);
          TYPE_NAME (TREE_TYPE (decl)) = decl;
        }
      else
        {
          /* This prevents core dumps ...
           */
          decl = build_decl (VAR_DECL, id, integer_type_node);
        }

      PASCAL_REDEFINABLE_DECL (decl) = 1;

      if (value)
        DECL_INITIAL (decl) = build_int_2 (value, 0);

      pushdecl (decl);

      if (type)
        {
          /* Ensure the type to be identical to the
           * global one that is used in the RTS.
           * @@ This is a small kludge.
           */
          TREE_TYPE (decl) = type;
        }

      /* Avoid warnings if the standard decls are unused.
       */
      TREE_USED (decl) = 1;
    }
}

/* Pre-declared identifiers; i.e. not type names.
 * See also gpc-parse.y: new_identifier_1.
 */
static struct known_id
{
  char *name;
  int value;

  /* Nonzero if we already did declare this one.
   */
  int declared;

  /* Flags as in `flag_what_pascal' indicating in
   * which dialects this identifier is valid.
   */
  unsigned long iclass;
}
  KItable [] =
{
    /* Standard Pascal constants */
    {"Maxint",  p_MAXINT,       0, ANY_PASCAL},
    {"False",   p_FALSE,        0, ANY_PASCAL},
    {"True",    p_TRUE,         0, ANY_PASCAL},

    /* Standard Pascal I/O */
    {"Input",   p_INPUT,        0, ANY_PASCAL},
    {"Output",  p_OUTPUT,       0, ANY_PASCAL},
    {"Rewrite", p_REWRITE,      0, ANY_PASCAL},
    {"Reset",   p_RESET,        0, ANY_PASCAL},
    {"Put",     p_PUT,          0, ANY_PASCAL},
    {"Get",     p_GET,          0, ANY_PASCAL},
    {"Write",   p_WRITE,        0, ANY_PASCAL},
    {"Read",    p_READ,         0, ANY_PASCAL},
    {"Writeln", p_WRITELN,      0, ANY_PASCAL},
    {"Readln",  p_READLN,       0, ANY_PASCAL},
    {"Page",    p_PAGE,         0, S_E_O_G_PASCAL},  /* @@ This is Standard? */
    {"Eof",     p_EOF,          0, ANY_PASCAL},
    {"Eoln",    p_EOLN,         0, ANY_PASCAL},

    /* Standard Pascal heap handling */
    {"New",     p_NEW,          0, ANY_PASCAL},
    {"Dispose", p_DISPOSE,      0, ANY_PASCAL},

    /* Standard Pascal arithmetic */
    {"Abs",     p_ABS,          0, ANY_PASCAL},
    {"Sqr",     p_SQR,          0, ANY_PASCAL},
    {"Sin",     p_SIN,          0, ANY_PASCAL},
    {"Cos",     p_COS,          0, ANY_PASCAL},
    {"Exp",     p_EXP,          0, ANY_PASCAL},
    {"Ln",      p_LN,           0, ANY_PASCAL},
    {"Sqrt",    p_SQRT,         0, ANY_PASCAL},
    {"Arctan",  p_ARCTAN,       0, ANY_PASCAL},
    {"Trunc",   p_TRUNC,        0, ANY_PASCAL},
    {"Round",   p_ROUND,        0, ANY_PASCAL},

    /* Standard Pascal transfer functions */
    {"Pack",    p_PACK,         0, S_E_O_G_PASCAL},
    {"Unpack",  p_UNPACK,       0, S_E_O_G_PASCAL},

    /* Standard Pascal ordinal functions */
    {"Ord",     p_ORD,          0, ANY_PASCAL},
    {"Chr",     p_CHR,          0, ANY_PASCAL},
    {"Succ",    p_SUCC,         0, ANY_PASCAL},
    {"Pred",    p_PRED,         0, ANY_PASCAL},
    {"Odd",     p_ODD,          0, ANY_PASCAL},

    /* The Standard Pascal `Forward' and `External'
     * directives are reserved words.
     */

    /* Extended Pascal constants */
    {"Maxchar",  p_MAXCHAR,     0, E_O_G_PASCAL},
    {"Maxreal",  p_MAXREAL,     0, E_O_G_PASCAL},
    {"Minreal",  p_MINREAL,     0, E_O_G_PASCAL},
    {"Epsreal",  p_EPSREAL,     0, E_O_G_PASCAL},

    /* Extended Pascal I/O */
    {"Extend",          p_EXTEND,               0, E_O_G_PASCAL},
    {"Update",          p_UPDATE,               0, E_O_G_PASCAL},
    {"Seekwrite",       p_SEEKWRITE,            0, E_O_G_PASCAL},
    {"Seekread",        p_SEEKREAD,             0, E_O_G_PASCAL},
    {"Seekupdate",      p_SEEKUPDATE,           0, E_O_G_PASCAL},
    {"Empty",           p_EMPTY,                0, E_O_G_PASCAL},
    {"Position",        p_POSITION,             0, E_O_G_PASCAL},
    {"Lastposition",    p_LASTPOSITION,         0, E_O_G_PASCAL},

    /* Extended Pascal external binding */
    {"Bind",            p_BIND,                 0, E_O_G_PASCAL},
    {"Unbind",          p_UNBIND,               0, E_O_G_PASCAL},
    {"Binding",         p_BINDING,              0, E_O_G_PASCAL},

    /* Extended Pascal time routines */
    {"Gettimestamp",    p_GETTIMESTAMP,         0, E_O_G_PASCAL},
    {"Date",            p_DATE,                 0, E_O_G_PASCAL},
    {"Time",            p_TIME,                 0, E_O_G_PASCAL},

    /* Extended Pascal String routines */
    {"Readstr",  p_READSTR,     0, E_O_G_PASCAL},
    {"Writestr", p_WRITESTR,    0, E_O_G_PASCAL},
    {"Length",   p_LENGTH,      0, E_O_U_B_D_G_PASCAL},
    {"Index",    p_INDEX,       0, E_O_G_PASCAL},
    /* `Index' is also a reserved word in Borland Pascal. */
    {"Substr",   p_SUBSTR,      0, E_O_G_PASCAL},
    {"Trim",     p_TRIM,        0, E_O_G_PASCAL},
    {"Eq",       p_EQ,          0, E_O_G_PASCAL},
    {"Lt",       p_LT,          0, E_O_G_PASCAL},
    {"Gt",       p_GT,          0, E_O_G_PASCAL},
    {"Ne",       p_NE,          0, E_O_G_PASCAL},
    {"Le",       p_LE,          0, E_O_G_PASCAL},
    {"Ge",       p_GE,          0, E_O_G_PASCAL},

    /* Extended Pascal Complex arithmetic */
    {"Re",       p_RE,          0, E_O_G_PASCAL},
    {"Im",       p_IM,          0, E_O_G_PASCAL},
    {"Cmplx",    p_CMPLX,       0, E_O_G_PASCAL},
    {"Arg",      p_ARG,         0, E_O_G_PASCAL},
    {"Polar",    p_POLAR,       0, E_O_G_PASCAL},

    /* Extended Pascal miscellaneous routines */
    {"Halt",     p_HALT,        0, E_O_U_B_D_G_PASCAL},
    {"Card",     p_CARD,        0, E_O_G_PASCAL},

    /* Object Pascal extensions */
    {"Override",        D_OVERRIDE,     0, O_G_PASCAL},
/*
    {"Copy",            op_COPY,        0, O_G_PASCAL},
    {"Null",            op_NULL,        0, O_G_PASCAL},
*/
    {"Root",            op_ROOT,        0, O_G_PASCAL},
    {"Textwritable",    op_TEXTWRITABLE,0, O_G_PASCAL},
    {"Self",            op_SELF,        0, O_B_D_G_PASCAL},

    /* UCSD Pascal extensions */
    {"Fillchar",        ucsd_FILLCHAR,  0, U_B_D_G_PASCAL},
    {"Moveleft",        ucsd_MOVELEFT,  0, UCSD_PASCAL},
    {"Moveright",       ucsd_MOVERIGHT, 0, UCSD_PASCAL},
    {"Blockread",       ucsd_BLOCKREAD, 0, U_B_D_G_PASCAL},
    {"Blockwrite",      ucsd_BLOCKWRITE,0, U_B_D_G_PASCAL},
    {"Concat",          ucsd_CONCAT,    0, U_B_D_G_PASCAL},
    {"Str",             ucsd_STR,       0, U_B_D_G_PASCAL},
    {"Insert",          ucsd_INSERT,    0, U_B_D_G_PASCAL},
    {"Delete",          ucsd_DELETE,    0, U_B_D_G_PASCAL},
    {"Pos",             ucsd_POS,       0, U_B_D_G_PASCAL},
    {"Copy",            ucsd_COPY,      0, U_B_D_G_PASCAL},
    {"Seek",            ucsd_SEEK,      0, U_B_D_G_PASCAL},
    {"Int",             ucsd_INT,       0, U_B_D_G_PASCAL},
    {"Ioresult",        ucsd_IORESULT,  0, U_B_D_G_PASCAL},

    /* Borland Pascal extensions */
    {"Assign",          bp_ASSIGN,      0, B_D_G_PASCAL},
    {"Append",          bp_APPEND,      0, B_D_G_PASCAL},
    {"Filepos",         bp_FILEPOS,     0, B_D_G_PASCAL},
    {"Filesize",        bp_FILESIZE,    0, B_D_G_PASCAL},
    {"Truncate",        bp_TRUNCATE,    0, B_D_G_PASCAL},
    {"Flush",           bp_FLUSH,       0, B_D_G_PASCAL},
    {"Erase",           bp_ERASE,       0, B_D_G_PASCAL},
    {"Rename",          bp_RENAME,      0, B_D_G_PASCAL},
    {"Chdir",           bp_CHDIR,       0, B_D_G_PASCAL},
    {"Mkdir",           bp_MKDIR,       0, B_D_G_PASCAL},
    {"Rmdir",           bp_RMDIR,       0, B_D_G_PASCAL},
    {"Assigned",        bp_ASSIGNED,    0, B_D_G_PASCAL},
    {"Addr",            bp_ADDR,        0, B_D_G_PASCAL},
    {"Getmem",          bp_GETMEM,      0, B_D_G_PASCAL},
    {"Freemem",         bp_FREEMEM,     0, B_D_G_PASCAL},
    {"Move",            bp_MOVE,        0, B_D_G_PASCAL},
    {"Paramcount",      bp_PARAMCOUNT,  0, B_D_G_PASCAL},
    {"Paramstr",        bp_PARAMSTR,    0, B_D_G_PASCAL},
    {"Inc",             bp_INC,         0, B_D_G_PASCAL},
    {"Dec",             bp_DEC,         0, B_D_G_PASCAL},
    {"Upcase",          gpc_UPCASE,     0, B_D_G_PASCAL},
    {"Near",            bp_NEAR,        0, B_D_G_PASCAL},
    {"Far",             bp_FAR,         0, B_D_G_PASCAL},
    {"Val",             bp_VAL,         0, B_D_G_PASCAL},
    {"Frac",            bp_FRAC,        0, B_D_G_PASCAL},
    {"Runerror",        bp_RUNERROR,    0, B_D_G_PASCAL},
    {"Filemode",        bp_FILEMODE,    0, B_D_G_PASCAL},
    {"Inoutres",        bp_INOUTRES,    0, B_D_G_PASCAL},
    {"Pi",              bp_PI,          0, B_D_G_PASCAL},
    {"High",            bp_HIGH,        0, B_D_G_PASCAL},
    {"Low",             bp_LOW,         0, B_D_G_PASCAL},
    {"Include",         bp_INCLUDE,     0, B_D_G_PASCAL},
    {"Exclude",         bp_EXCLUDE,     0, B_D_G_PASCAL},
    {"Random",          bp_RANDOM,      0, B_D_G_PASCAL},
    {"Randomize",       bp_RANDOMIZE,   0, B_D_G_PASCAL},
    {"Name",            bp_NAME,        0, B_D_G_PASCAL},

    /* Borland Delphi extensions */
    {"Null",            delphi_NULL,    0, D_G_PASCAL},

    /* GPC directives */
    {"C",               D_C,            0, GNU_PASCAL},
    {"C_language",      D_C_LANGUAGE,   0, GNU_PASCAL},
    {"Extern",          D_EXTERN,       0, GNU_PASCAL},
    {"Register",        D_REGISTER,     0, GNU_PASCAL},
    {"Static",          LEX_STATIC,     0, GNU_PASCAL},

    /* ASM is a reserved word now.  However it can be redefined.  ;-) */

    /* Other extensions */
    {"Stderr",          gpc_STDERR,             0, GNU_PASCAL},
    {"Alignof",         ALIGNOF,                0, GNU_PASCAL},
    {"Setlength",       gpc_SETLENGTH,          0, GNU_PASCAL},
    {"Settype",         gpc_SETTYPE,            0, GNU_PASCAL},
    {"Break",           BREAK,                  0, B_D_G_PASCAL},
    {"Continue",        CONTINUE,               0, B_D_G_PASCAL},
    {"Return",          RETURN_,                0, GNU_PASCAL},
    {"Result",          RESULT,                 0, D_G_PASCAL},
    {"Exit",            EXIT,                   0, U_B_D_G_PASCAL},
    {"Fail",            FAIL,                   0, B_D_G_PASCAL},
    {"Sizeof",          SIZEOF,                 0, U_B_D_G_PASCAL},
    {"Bitsizeof",       BITSIZEOF,              0, GNU_PASCAL},
    {"Typeof",          TYPEOF,                 0, B_D_G_PASCAL},
    {"Returnaddress",   gpc_RETURNADDRESS,      0, GNU_PASCAL},
    {"Frameaddress",    gpc_FRAMEADDRESS,       0, GNU_PASCAL},
    {"Max",             gpc_MAX,                0, GNU_PASCAL},
    {"Min",             gpc_MIN,                0, GNU_PASCAL},
    {"Locase",          gpc_LOCASE,             0, GNU_PASCAL},
    {"Setfiletime",     gpc_SETFILETIME,        0, GNU_PASCAL},
    {"Inoutresstr",     gpc_INOUTRESSTR,        0, GNU_PASCAL},
    {"Newcstring",      gpc_NEWCSTRING,         0, GNU_PASCAL},
    {"Cstringcopystring",gpc_CSTRINGCOPYSTRING, 0, GNU_PASCAL},
    {"Cstring2string",  gpc_CSTRING2STRING,     0, GNU_PASCAL},
    {"String2cstring",  gpc_STRING2CSTRING,     0, GNU_PASCAL},

    {"Conjugate",       CONJUGATE,              0, E_O_PASCAL}, /* Complex conjugate */

    {"Mark",            p_MARK,                 0, U_B_D_G_PASCAL}, /* mark the heap state */
    {"Release",         p_RELEASE,              0, U_B_D_G_PASCAL}, /* release heap to last mark */
/*
    {"Default",         DEFAULT,                0, GNU_PASCAL},  * Case defaults *
    {"Others",          OTHERS,                 0, GNU_PASCAL},
*/
    {"Close",           p_CLOSE,                0, U_B_D_G_PASCAL},     /* Close a file */
    {"Definesize",      p_DEFINESIZE,           0, E_O_G_PASCAL},       /* random access file size definition */

    {NULL, 0}
};

/* called from initialize_world() after type nodes have been initialized */
void
declare_known_ids ()
{
  struct known_id *kptr;

  /* C-style strings, or BP-style "pChar" strings.
   */
  cstring_type_node = string_type_node;

  if (! flag_what_pascal
      || (flag_what_pascal & E_O_PASCAL))
    {
      static int declared = 0;
      if (! declared)
        {
          /* Extended Pascal: Complex, TimeStamp, and BindingType.
           */
          declare_known_id ("Complex",     complex_type_node,    0);
          declare_known_id ("Timestamp",   gpc_type_TIMESTAMP,   0);
          declare_known_id ("Bindingtype", gpc_type_BINDINGTYPE, 0);
          declared++;
        }
    }

  if (! flag_what_pascal
      || (flag_what_pascal & E_O_U_B_D_PASCAL))
    {
      static int declared = 0;
      if (! declared)
        {
          declare_known_id ("String", string_schema_proto_type, 0);
          declared++;
        }
    }

  if (! flag_what_pascal
      || (flag_what_pascal & B_D_PASCAL))
    {
      static int declared = 0;
      if (! declared)
        {
          /* Borland Pascal pre-declared types.
           */
          declare_known_id ("Pointer",  ptr_type_node,                  0);
          declare_known_id ("Word",     unsigned_type_node,             0);
          declare_known_id ("Shortint", short_integer_type_node,        0);
          declare_known_id ("Byte",     unsigned_char_type_node,        0);
          declare_known_id ("Longint",  long_long_integer_type_node,    0);
          declare_known_id ("Single",   float_type_node,                0);
          declare_known_id ("Double",   double_type_node,               0);
          declare_known_id ("Extended", long_double_type_node,          0);
          declare_known_id ("Comp",     long_long_integer_type_node,    0);
          declare_known_id ("Pchar",    cstring_type_node,              0);
          declare_known_id ("Bytebool", byte_boolean_type_node,         0);
          declare_known_id ("Wordbool", word_boolean_type_node,         0);
          declare_known_id ("Longbool", long_long_boolean_type_node,    0);
          declared++;
        }
    }

  if (! flag_what_pascal
      || (flag_what_pascal & BORLAND_DELPHI))
    {
      static int declared = 0;
      if (! declared)
        {
          /* Delphi pre-declared types */
          declare_known_id ("Smallint", short_integer_type_node,        0);
          declare_known_id ("Ansichar", char_type_node,                 0);
          declare_known_id ("Pansichar",cstring_type_node,              0);
          declared++;
        }
    }

  /* Note:  If the meaning of the `Longest*' types changes from
   * `Long*' to something longer (`long long long' in C;-), don't
   * forget to check the RTS.  In particular, `rts/random.c' depends
   * on `LongestCard' being `long long unsigned int' (see `rts/rts.h').
   */
  if (! flag_what_pascal)
    {
      static int declared = 0;
      if (! declared)
        {
          /* Pointer-compatible integer types.
           */
          declare_known_id ("Ptrword",  ptrsize_unsigned_type_node,     0);
          declare_known_id ("Ptrint",   ptrsize_integer_type_node,      0);
          declare_known_id ("Ptrcard",  ptrsize_unsigned_type_node,     0);
          declare_known_id ("Sizetype", sizetype,                       0);
          declare_known_id ("Ptrdifftype", ptrdiff_type_node,           0);

          /* "Natural" GPC Extensions to the Borland Pascal types.
           */
          declare_known_id ("Shortword",short_unsigned_type_node,       0);
          declare_known_id ("Medword",  long_unsigned_type_node,        0);
          declare_known_id ("Longword", long_long_unsigned_type_node,   0);
          declare_known_id ("Longestword", long_long_unsigned_type_node, 0);
          declare_known_id ("Bytecard", unsigned_char_type_node,        0);
          declare_known_id ("Shortcard",short_unsigned_type_node,       0);
          declare_known_id ("Medcard",  long_unsigned_type_node,        0);
          declare_known_id ("Longcard", long_long_unsigned_type_node,   0);
          declare_known_id ("Longestcard", long_long_unsigned_type_node, 0);
          declare_known_id ("Byteint",  signed_char_type_node,          0);
          declare_known_id ("Medint",   long_integer_type_node,         0);
          declare_known_id ("Longestint",long_long_integer_type_node,   0);
          declare_known_id ("Shortreal",float_type_node,                0);
          declare_known_id ("Medreal",  double_type_node,               0);
          declare_known_id ("Longreal", long_double_type_node,          0);
          declare_known_id ("Longestreal", long_double_type_node,       0);
          declare_known_id ("Shortbool",short_boolean_type_node,        0);
          declare_known_id ("Medbool",  long_boolean_type_node,         0);
          declare_known_id ("Longestbool",long_long_boolean_type_node,  0);

          /* GPC Extensions: void type (two spellings) */
          declare_known_id ("Void",     void_type_node,0);

          /* GPC: Extension: Parameter is a C language string type
           * Causes a string-type parameter to be passed as a pointer even
           * if it is a value parameter; also a string-schema value parameter
           * passes the address of the character array, not the address of the
           * schema object.
           *
           * This is a kludge.
           */
          declare_known_id ("Cstring", cstring_type_node, 0);
          declared++;
        }
    }

    /* Pre-declared type names.
     *
     * These are declared last, so error messages prefer "Integer"
     * to "Ptrdifftype".
     */
    if (! flag_what_pascal || (flag_what_pascal & BORLAND_DELPHI))
      {
        static int declared = 0;
        if (! declared)
          {
            declare_known_id ("Cardinal", unsigned_type_node, 0);
            declared++;
          }
      }
    {
      static int declared = 0;
      if (! declared)
        {
          declare_known_id ("Integer", integer_type_node, 0);
          declare_known_id ("Real",    double_type_node,  0);
          declare_known_id ("Boolean", boolean_type_node, 0);
          declare_known_id ("Char",    char_type_node,    0);
          declare_known_id ("Text",    text_type_node,    0);
          declared++;
        }
    }

  /* pre-declared identifiers */
  for (kptr = KItable; kptr->name; kptr++)
    if (! flag_what_pascal
        || (flag_what_pascal & kptr->iclass))
      {
        if (! kptr->declared)
          {
            declare_known_id (kptr->name, NULL_TREE, kptr->value);
            kptr->declared++;
          }
      }
}

/* Generate special code at the beginning of a module/unit constructor.
 */
void
start_constructor ()
{
  tree name, run_condition, imported_interface;
  char *dtor_name;

  /* Declare a static variable which indicates whether this
   * constructor has already been run.
   */
  name = get_unique_identifier ("ctor_run_condition", 1);
  declare_vars (build_tree_list (NULL_TREE, name), boolean_type_node,
                build_tree_list (NULL_TREE, boolean_true_node),
                NULL_PTR, VQ_STATIC, 0, NULL_TREE);
  run_condition = lookup_name (name);

  /* if run_condition then
   *   begin
   *     run_condition := false;
   *     <run constructors of modules used from this one>;
   *     p_atexit (@fini_Foo);
   *     { to be continued in finish_constructor() }
   */
  expand_start_cond (run_condition, 0);
  expand_expr_stmt (build_modify_expr (run_condition, NOP_EXPR,
                                       boolean_false_node));

  for (imported_interface = current_module->imports;
       imported_interface;
       imported_interface = TREE_CHAIN (imported_interface))
    {
      tree interface_id, module_id;
      char *ctor_name;
      interface_id = TREE_VALUE (imported_interface);
      if (! interface_id || TREE_CODE (interface_id) != IDENTIFIER_NODE)
        abort ();
      module_id = itab_get_module_name (interface_id);
      /* This may be NULL_TREE when declarations are used which come
       * from interfaces that have not been explicitly imported.
       */
      if (module_id)
        {
          if (TREE_CODE (module_id) != IDENTIFIER_NODE)
            abort ();
          ctor_name = concat ("init_", IDENTIFIER_POINTER (module_id), NULL_PTR);
          emit_library_call (gen_rtx (SYMBOL_REF, Pmode, ctor_name),
                             0, VOIDmode, 0);
          /* We do _not_ free() ctor_name.  It is needed later for
           * assembler output.
           */
        }
    }

  if (current_module->main_program)
    dtor_name = "fini_pascal_main_program";
  else
    dtor_name = concat ("fini_", IDENTIFIER_POINTER (current_module->name), NULL_PTR);
  emit_library_call (gen_rtx (SYMBOL_REF, Pmode, "_p_atexit"),
                     0, VOIDmode, 1,
                     gen_rtx (SYMBOL_REF, Pmode, dtor_name), Pmode);
}

/* Generate special code at the end of a module/unit constructor.
 */
void
finish_constructor ()
{
  /* Close the open `if' from init_constructor().
   */
  expand_end_cond ();
}



/*
 * Miscellaneous.
 */

void
pushlevel_expand ()
{
  emit_line_note (input_filename, lineno);
  pushlevel (0);
  clear_last_expr ();
  expand_start_bindings (0);
}

tree
poplevel_expand ()
{
  tree decls = getdecls ();
  emit_line_note (input_filename, lineno);
  if (decls != NULL_TREE)
    {
      expand_end_bindings (decls, 1, 0);
      return poplevel (1, 1, 0);
    }
  else
    {
      expand_end_bindings (decls, kept_level_p (), 0);
      return poplevel (kept_level_p (), 0, 0);
    }
}

/* Check a `for' loop control variable for ISO compliance.
 */
void
check_for_control_variable (var)
     tree var;
{
  if (TREE_CODE (var) != VAR_DECL && PEDANTIC (B_D_PASCAL))
    pedwarn ("ISO Pascal requires an entire `for' variable");

  /* ISO Pascal wants VAR to be in the closest possible scope.
   * Borland Pascal also allows global variables.
   */
  if (TREE_CODE (var) == VAR_DECL
      && (DECL_CONTEXT (var) != current_function_decl
          && ! (DECL_CONTEXT (var) == NULL_TREE
                && ((flag_what_pascal & B_D_PASCAL)
                    || (main_program_name
                        && current_function_decl
                           == IDENTIFIER_LOCAL_VALUE (main_program_name)))))
      && (pedantic || flag_what_pascal))
    {
      pedwarn ("ISO Pascal requires `for' variable to be declared");
      pedwarn (" in the closest possible scope");
    }
  if (! ORDINAL_TYPE (TREE_CODE (TREE_TYPE (var)))
      && ! (flag_extended_syntax
            && TREE_CODE (TREE_TYPE (var)) == POINTER_TYPE))
    {
      if (TREE_CODE (TREE_TYPE (var)) == POINTER_TYPE && ! flag_extended_syntax)
        {
          error ("`for' variable must be of ordinal type");
          ptrarith_inform ();

          /* Avoid further error messages.
           */
          TREE_TYPE (var) = error_mark_node;
        }
      else if (flag_extended_syntax)
        error ("`for' variable must be of ordinal or pointer type");
      else
        error ("`for' variable must be of ordinal type");
    }
}

/* Warn about Borland Pascal "typed const" misuse.
 */
void
typed_const_warning (target)
    tree target;
{
  static int informed = 0;
  if (warn_typed_const)
    {
      if (PASCAL_TYPE_TYPEDCONST (TREE_TYPE (target)))
        {
          warning ("typed const misused as initialized variable");
          if (! informed)
            {
              warning (" (Better use ISO 10206 Extended Pascal initialized");
              warning (" types and variables: `var foo: integer value 7'.)");
              informed++;
            }
          /* It is cruel enough to get this warning once for each typed const.
           */
          PASCAL_TYPE_TYPEDCONST (TREE_TYPE (target)) = 0;
        }
      else if (TREE_CODE (target) == COMPONENT_REF
               || TREE_CODE (target) == ARRAY_REF)
        typed_const_warning (TREE_OPERAND (target, 0));
    }
}

/* Warn about a feature currently missing in GPC.
 */
void
object_directive_warning ()
{
  static int informed = 0;
  if (! informed)
    {
      warning ("GPC currently ignores the `private', `public', `published',");
      warning (" and `protected' directives");
      informed = 1;
    }
}

tree
no_parameters ()
{
  tree parms;

  pushlevel (0);
  clear_parm_order ();
  declare_parm_level (1);
  parms = get_parm_info (1);
  poplevel (0, 0, 0);

  return parms;
}

#ifndef EGCS92
char *
concat VPROTO((char *first, ...))
{
  register int length;
  register char *newstr;
  register char *end;
  register char *arg;
  va_list args;
#ifndef __STDC__
  char *first;
#endif

  /* First compute the size of the result and get sufficient memory.  */

  VA_START (args, first);
#ifndef __STDC__
  first = va_arg (args, char *);
#endif

  arg = first;
  length = 0;

  while (arg != 0)
    {
      length += strlen (arg);
      arg = va_arg (args, char *);
    }

  newstr = (char *) xmalloc (length + 1);
  va_end (args);

  /* Now copy the individual pieces to the result string.  */

  VA_START (args, first);
#ifndef __STDC__
  first = va_arg (args, char *);
#endif

  end = newstr;
  arg = first;
  while (arg != 0)
    {
      while (*arg)
        *end++ = *arg++;
      arg = va_arg (args, char *);
    }
  *end = '\000';
  va_end (args);

  return (newstr);
}
#endif

/* Return a newly allocated copy of the string s
 */
char *
save_string (s)
     char *s;
{
  char *result = xmalloc (strlen (s) + 1);
  strcpy (result, s);
  return result;
}

#ifdef EGCS

/* EGCS hooks.
 */

char *
init_parse (filename)
     char *filename;
{
#if !USE_CPPLIB
  /* Open input file.  */
  if (filename == 0 || !strcmp (filename, "-"))
    {
      finput = stdin;
      filename = "stdin";
    }
  else
    finput = fopen (filename, "r");
  if (finput == 0)
    pfatal_with_name (filename);

#ifdef IO_BUFFER_SIZE
  setvbuf (finput, (char *) xmalloc (IO_BUFFER_SIZE), _IOFBF, IO_BUFFER_SIZE);
#endif
#endif /* !USE_CPPLIB */

  init_lex ();

#if USE_CPPLIB
  yy_cur = "\n";
  yy_lim = yy_cur+1;

  cpp_reader_init (&parse_in);
  parse_in.data = &parse_options;
  cpp_options_init (&parse_options);
  cpp_handle_options (&parse_in, 0, NULL); /* FIXME */
  parse_in.show_column = 1;
  if (! cpp_start_read (&parse_in, filename))
    abort ();
#endif

  return filename;
}

void
finish_parse ()
{
#if USE_CPPLIB
  cpp_finish (&parse_in);
#else
  fclose (finput);
#endif
}

void
lang_print_xnode (f, node, indent)
     FILE *f;
     tree node;
     int indent;
{
  /* empty */
}

#endif
