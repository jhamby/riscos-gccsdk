/* Object oriented support routines for GNU Pascal
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
 * Author: Peter Gerwinski <peter@gerwinski.de>
 */

#include <stdio.h>

#include "gcc-version.h"
#include "config.h"
#include "gansidecl.h"
#ifdef EGCS
#include "system.h"
#endif

#include "gbe.h"
#include "gpc-defs.h"
#include "module.h"
#include "types.h"
#include "util.h"
#include "rts/types.h"
#include "objects.h"

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif



/*
 * Objects.
 */

/* Get a FIELD_DECL node of structured type OBJ.
 * This is only applied for structures with no variant part,
 * so it is much simpler than find_field().
 */
tree
simple_get_field (name, obj, errmsg)
     register tree name;
     tree obj;
     char *errmsg;
{
  register tree field = TYPE_FIELDS (obj);
  while (field && DECL_NAME (field) != name)
    field = TREE_CHAIN (field);
  if ((! field) && errmsg)
    error (errmsg, IDENTIFIER_POINTER (name));
  return field;
}

/* Build a method call out of a COMPONENT_REF.
 */
tree
call_method (cref, args)
     tree cref;
     tree args;
{
  tree obj = TREE_OPERAND (cref, 0);
  tree fun = TREE_OPERAND (cref, 1);

  if (! PASCAL_TYPE_OBJECT (TREE_TYPE (obj)))
    {
      error ("calling method of something not an object");
      return error_mark_node;
    }
  /* If this is a method, call it.
   */
  if (TREE_CODE (fun) == FUNCTION_DECL
      && TREE_CODE (TREE_TYPE (fun)) == FUNCTION_TYPE)
    {
      int is_virtual = PASCAL_VIRTUAL_METHOD (fun);
      if (TREE_CODE (obj) == TYPE_DECL)
        {
          if (defining_methods)
            {
              /* This is an explicit call to a parent's method.
               * In this case, no constructor code is generated.
               */
              fun = simple_get_field (DECL_NAME (fun), TREE_TYPE (obj),
                                      "method not found");
              if (! fun)
                return error_mark_node;
              obj = build_indirect_ref (lookup_name (self_name_node),
                                        "method call");
            }
          else
            {
              /* Somebody is looking for the address of this method.
               * Give him the FUNCTION_DECL.
               */
              return fun;
            }
        }
      else if (is_virtual
               || PASCAL_CONSTRUCTOR_METHOD (fun))
        {
          tree vmt_field = simple_get_field (get_identifier ("vmt"),
                             TREE_TYPE (obj),
                             "Internal GPC error: no implicit VMT field");
          if (PASCAL_CONSTRUCTOR_METHOD (fun))
            {
              /* Generate constructor code in the caller.
               */
              char *vmt_name;
              tree vmt;
              tree obj_type_name = TYPE_LANG_NAME (TREE_TYPE (obj));

              /* Get the VMT (an external variable).
               */
              if (! obj_type_name)
                abort ();
              else if (TREE_CODE (obj_type_name) == TYPE_DECL)
                obj_type_name = DECL_NAME (obj_type_name);
              vmt_name = (char *) xmalloc (strlen (IDENTIFIER_POINTER (obj_type_name)) + 5);
              sprintf (vmt_name, "vmt_%s", IDENTIFIER_POINTER (obj_type_name));
              vmt = lookup_name (get_identifier (vmt_name));
              free (vmt_name);

              /* Assign the address of VMT to the object's VMT field.
               */
              if (vmt && vmt_field)
                {
                  tree vmt_addr = build_pascal_unary_op (ADDR_EXPR, vmt, 0);
                  vmt_field = build (COMPONENT_REF, TREE_TYPE (vmt_field),
                                     obj, vmt_field);
                  expand_expr_stmt (build_modify_expr (vmt_field,
                                                       NOP_EXPR, vmt_addr));
                }
              else
                abort ();
            }
          if (is_virtual)
            {
              tree vmt_field = simple_get_field (get_identifier ("vmt"),
                               TREE_TYPE (obj),
                               "Internal GPC error: no implicit VMT field");
              tree method = NULL_TREE;
              /*
               * @@@ Here a runtime check can be added: does the VMT field
               * of the object TREE_OPERAND (cref, 0) really point to a VMT?
               */
              if (vmt_field)
                method = simple_get_field (DECL_NAME (fun),
                           TREE_TYPE (TREE_TYPE (vmt_field)),
                           "Internal GPC error: virtual method `%s' not in VMT");
              if (method)
                {
                  tree type_save = TREE_TYPE (fun);
                  fun = build (COMPONENT_REF, TREE_TYPE (vmt_field), obj, vmt_field);
                  fun = build_indirect_ref (fun, "virtual method call");
                  fun = build (COMPONENT_REF, TREE_TYPE (method), fun, method);

                  /* In the VMT, only generic pointers are stored to avoid
                   * confusion in GPI files.  Repair it here.
                   */
                  TREE_TYPE (fun) = build_pointer_type (type_save);

                  fun = build_indirect_ref (fun, "virtual method call");
                }
              else
                fun = error_mark_node;
            }
        }

      if (! is_virtual)
        {
          /* Not a virtual method.  Use the function definition node rather
           * than the forward declaration inside the object if possible.
           * Otherwise, inline optimizations of methods don't work.
           */
          tree realfun = lookup_name (DECL_ASSEMBLER_NAME (fun));
          if (realfun)
            fun = realfun;
        }

      /* Check if OBJ is an lvalue and do the call */
      if (lvalue_or_else (obj, "method call"))
        {
          if (args)
            fun = build_function_call (fun,
                    chainon (build_tree_list (NULL_TREE, obj), args));
          else
            fun = build_function_call (fun, build_tree_list (NULL_TREE, obj));
        }
    }
  else
    error ("invalid method call");
  return fun;
}

/* Construct the external name "Myobj_Mymethod" for a method.
 * This won't crash with a user-declared function "Myobj_mymethod"
 * but will be accessable from a C program using a Pascal module.
 */
tree
get_method_name (object_name, method_name)
     tree object_name, method_name;
{
  if (object_name && method_name)
    {
      char *object_id = IDENTIFIER_POINTER (object_name);
      char *method_id = IDENTIFIER_POINTER (method_name);
      /* allocate enough space to avoid alignment problems with i386 */
      char *copy = oballoc (IDENTIFIER_LENGTH (object_name)
                            + IDENTIFIER_LENGTH (method_name) + 5);
      sprintf (copy, "%s_%s", object_id, method_id);
      return get_identifier (copy);
    }
  else if (method_name)
    return method_name;
  else
    abort ();
}

/* Push an implicit "Self" reference parameter for method definitions.
 */
void
push_self_parameter (object_name, xref)
     tree object_name;
     int xref;
{
  tree type = lookup_name (object_name);

  /* Enable use of a forward referenced type.  Try hard not to
   * get confused by something already existing with the same
   * name, but recycle an already existing forward reference.
   */
  if (type
      && TREE_CODE (type) == TYPE_DECL
      && (PASCAL_TYPE_OBJECT (TREE_TYPE (type))
          || TREE_CODE (TREE_TYPE (type)) == LANG_TYPE))
    type = TREE_TYPE (type);
  else
    {
      char *ptr_name = concat ("ptr_xref_", IDENTIFIER_POINTER (object_name), NULL_PTR);
      char *ref_name = concat ("ref_xref_", IDENTIFIER_POINTER (object_name), NULL_PTR);
      tree ptr_type = xref_tag (POINTER_TYPE, get_identifier (ptr_name));
      tree ref_type = xref_tag (REFERENCE_TYPE, get_identifier (ref_name));
      if (TREE_TYPE (ptr_type))
        type = TREE_TYPE (ptr_type);
      else if (TREE_TYPE (ref_type))
        type = TREE_TYPE (ref_type);
      else
        {
          type = make_node (LANG_TYPE);
          TYPE_NAME (type) = object_name;
          TYPE_POINTER_TO (type) = ptr_type;
          TYPE_REFERENCE_TO (type) = ref_type;
        }
      TREE_TYPE (ptr_type) = type;
      TREE_TYPE (ref_type) = type;
      layout_type (ptr_type);
      layout_type (ref_type);
      free (ptr_name);
      free (ref_name);
      if (! xref)
        error ("object type name expected, identifier `%s' given",
               IDENTIFIER_POINTER (object_name));
    }
  handle_formal_param_list (build_tree_list (NULL_TREE, self_name_node),
                            type, NULL_TREE, 1, 0);
}

/* Make sure that the object type being declared exists as a
 * cross reference.  This is needed for implicit "Self" parameters.
 */
void
check_object_pointer ()
{
  tree p, ptr = NULL_TREE;

  /* In most practical cases the user program will have
   * explicitely specified a pointer type pointing to the
   * object being declared.  Recycle that type, if possible.
   */
  for (p = current_type_list; p; p = TREE_CHAIN (p))
    {
      tree t = TREE_TYPE (lookup_name (TREE_VALUE (p)));
      if (TREE_CODE (t) == POINTER_TYPE
          && TYPE_NAME (TREE_TYPE (t)) == current_type_name)
        {
          ptr = p;
          break;
        }
    }

  /* No user-defined pointer type found.
   * Define it here as "ptr_to_Myobj".
   */
  if (!ptr && current_type_name)
    {
      tree d, ptr_name_node;
      char *ptr_name = concat ("ptr_to_",
                               IDENTIFIER_POINTER (current_type_name),
                               NULL_PTR);
      ptr = build_pascal_pointer_type (current_type_name, 0);
      ptr_name_node = get_identifier (ptr_name);
      d = start_decl (ptr_name_node, tree_cons (NULL_TREE, ptr,
                        build_tree_list (NULL_TREE, type_id)),
                      0, NULL_TREE, NULL_TREE);
      finish_decl (d, NULL_TREE, NULL_TREE);
      current_type_list = chainon (current_type_list,
                                   build_tree_list (NULL_TREE, ptr_name_node ));
      if (this_is_an_interface_module)
        handle_autoexport (ptr_name_node);
      free (ptr_name);
    }
}

/* Make a copy of the object type's name to TYPE_LANG_SPECIFIC.
 * The name is needed when a method is called and may vanish
 * in expressions where a TYPE is replaced by its TYPE_MAIN_VARIANT.
 */
void
store_object_name (obj)
     tree obj;
{
  TYPE_LANG_SPECIFIC (obj) = allocate_type_lang_specific ();
  TYPE_LANG_CODE (obj) = PASCAL_LANG_OBJECT;
  TYPE_LANG_NAME (obj) = current_type_name;
  TYPE_LANG_BASE (obj) = NULL_TREE;  /* base type */
}

/* Perform inheritance between two already complete object structures.
 */
void
inherit (dest, parent)
     tree dest, parent;
{
  tree dest_fields = TYPE_FIELDS (dest);
  tree parent_fields = copy_list (TYPE_FIELDS (parent));
  register tree df, pf, df0, pf0;

  /* Make `parent_fields' distinguishable from `TYPE_FIELDS (parent)'.
   */
  for (pf = parent_fields; pf; pf = TREE_CHAIN (pf))
    if (DECL_LANG_SPECIFIC (pf))
      {
        DECL_LANG_INTERFACE (pf) = NULL_TREE;
        DECL_LANG_UID (pf) = 0;
      }

  /* Handle overwriting of methods.
   */
  for (pf0 = NULL_TREE, pf = parent_fields;
       pf; pf0 = pf, pf = TREE_CHAIN (pf))
    {
      for (df0 = NULL_TREE, df = dest_fields;
           df; df0 = df, df = TREE_CHAIN (df))
        if (DECL_NAME (df) == DECL_NAME (pf))
          break;
      if (df)
        {
          if (TREE_CODE (df) == FUNCTION_DECL)
            {
              if (TREE_CODE (pf) == FUNCTION_DECL)
                {
                  if (PASCAL_VIRTUAL_METHOD (pf)
                      && ! PASCAL_VIRTUAL_METHOD (df))
                    {
                      /* Overridden virtual methods must be virtual. */
                      warning ("method `%s' is virtual",
                               IDENTIFIER_POINTER (DECL_NAME (df)));
                      PASCAL_VIRTUAL_METHOD (df) = 1;
                    }
                  /* Replace the parent's method with the child's one
                   */
                  if (df0)
                    TREE_CHAIN (df0) = TREE_CHAIN (df);
                  else
                    dest_fields = TREE_CHAIN (df);
                  if (pf0)
                    TREE_CHAIN (pf0) = df;
                  else
                    parent_fields = df;
                  TREE_CHAIN (df) = TREE_CHAIN (pf);
                  pf = df;
                }
              else
                error ("method `%s' conflicts with data field of parent object",
                       IDENTIFIER_POINTER (DECL_NAME (df)));
            }
          else if (TREE_CODE (pf) == FUNCTION_DECL)
            error ("data field `%s' conflicts with method of parent object",
                   IDENTIFIER_POINTER (DECL_NAME (df)));
          else
            error ("cannot overwrite data field `%s' of parent object",
                   IDENTIFIER_POINTER (DECL_NAME (df)));
        }
    }
  TYPE_FIELDS (dest) = chainon (parent_fields, dest_fields);
  TYPE_SIZE (dest) = NULL_TREE;
#ifdef EGCS
  TYPE_SIZE_UNIT (dest) = NULL_TREE;
#endif /* EGCS */
  TYPE_LANG_BASE (dest) = parent;
  layout_type (dest);
}

/* Add an implicit VMT field to an already complete object DEST.
 */
void
add_vmt_field (dest)
     tree dest;
{
  tree vmt_pointer_type = build_pointer_type (void_type_node);
  tree vmt_field;
  vmt_field = build_decl (FIELD_DECL, get_identifier ("vmt"),
                          vmt_pointer_type);
  TYPE_FIELDS (dest) = chainon (vmt_field, TYPE_FIELDS (dest));
  TYPE_SIZE (dest) = NULL_TREE;
#ifdef EGCS
  TYPE_SIZE_UNIT (dest) = NULL_TREE;
#endif /* EGCS */
  layout_type (dest);
}

/* Check whether the formal parameters of the method conflict
 * with fields of the object.
 * This can be done faster when the fields of the object are sorted.
 */
void
check_method_parameters (dest)
     tree dest;
{
  tree field;
  for (field = TYPE_FIELDS (dest); field; field = TREE_CHAIN (field))
    if (TREE_CODE (field) == FUNCTION_DECL
        && DECL_CONTEXT (field) == dest)  /* do not check inherited methods */
      {
        tree parm;
        for (parm = DECL_LANG_PARMS (field); parm; parm = TREE_CHAIN (parm))
          {
            tree field2;
            for (field2 = TYPE_FIELDS (dest); field2;
                 field2 = TREE_CHAIN (field2))
              {
                if (DECL_NAME (parm) == DECL_NAME (field2))
                  {
                    if (TREE_CODE (field2) == FUNCTION_DECL)
                      error ("formal parameter `%s' of method `%s' conflicts with object method",
                             IDENTIFIER_POINTER (DECL_NAME (parm)),
                             IDENTIFIER_POINTER (DECL_NAME (field)));
                    else
                      error ("formal parameter `%s' of method `%s' conflicts with object field",
                             IDENTIFIER_POINTER (DECL_NAME (parm)),
                             IDENTIFIER_POINTER (DECL_NAME (field)));
                  }
              }
          }
      }
}

/* Construct a virtual method table (VMT) for object DEST.
 */
void
construct_vmt (dest)
     tree dest;
{
  tree object_name_node = TYPE_LANG_NAME (dest);
  char *object_name = IDENTIFIER_POINTER (object_name_node);
  /* allocate enough space to avoid alignment problems with i386 */
  char *vmt_name = oballoc (IDENTIFIER_LENGTH (object_name_node) + 9);
  tree vmt_entry, vmt_type, field;

  /* Create a unique name for the VMT
   */
  sprintf (vmt_name, "vmt_%s", object_name);

  /* Allocate all required fields.
   * First field: size of the object (integer)
   */
  vmt_entry = p_grokfields (build_tree_list (NULL_TREE,
                              get_identifier ("object_size")),
                            sizetype);

  /* Second field: negated size of the object
   * (integer; for future runtime checks)
   */
  vmt_entry = chainon (vmt_entry,
                p_grokfields (build_tree_list (NULL_TREE,
                                get_identifier ("neg_object_size")),
                              sizetype));

  /* Create a record type for the VMT.
   * The fields will contain pointers to all virtual methods.
   */
  for (field = TYPE_FIELDS (dest); field; field = TREE_CHAIN (field))
    {
      if (TREE_CODE (field) == FUNCTION_DECL
          && PASCAL_VIRTUAL_METHOD (field))
        {
          /* The real type of this pointer is not needed for type
           * checking because it already is in the field of the object.
           * Repeating it here would only cause confusion in GPI files
           * if the method has a prediscriminated parameter.
           */
          vmt_type = ptr_type_node;
          vmt_entry = chainon (vmt_entry,
                        p_grokfields (build_tree_list (NULL_TREE,
                                        DECL_NAME (field)),
                                      vmt_type));
        }
    }

  vmt_type = start_struct (RECORD_TYPE, NULL_TREE);
  vmt_type = finish_struct (vmt_type, vmt_entry, NULL_TREE);

  /* Build an initializer for the VMT record
   */
  vmt_entry = build_tree_list (NULL_TREE, size_in_bytes (dest));
  field = build_pascal_unary_op (NEGATE_EXPR, size_in_bytes (dest), 1);
  /* Avoid range checking to barf. */
  field = fold (build1 (NOP_EXPR, sizetype, field));
  vmt_entry = chainon (vmt_entry, build_tree_list (NULL_TREE, field));

  for (field = TYPE_FIELDS (dest); field; field = TREE_CHAIN (field))
    {
      if (TREE_CODE (field) == FUNCTION_DECL
          && PASCAL_VIRTUAL_METHOD (field))
        {
          tree method_name, method, dest2 = dest;
          do
            {
              object_name_node = TYPE_LANG_NAME (dest2);
              method_name = get_method_name (object_name_node,
                                             DECL_NAME (field));
              method = lookup_name (method_name);
              dest2 = TYPE_LANG_BASE (dest2);
            }
          while (! method && dest2);
          if (method)
            method = build_unary_op (ADDR_EXPR, method, 0);
          else
            abort ();
          vmt_entry = chainon (vmt_entry, build_tree_list (NULL_TREE, method));
        }
    }

  /* Now create a global var declaration.
   */
  declare_vars (build_tree_list (NULL_TREE, get_identifier (vmt_name)),
                vmt_type, build_tree_list (NULL_TREE, vmt_entry),
                NULL_PTR, this_is_an_interface_module ? 0 : VQ_STATIC, 
                this_is_an_interface_module, NULL_TREE);

  /* Suppress "unused variable" warning.
   */
  TREE_USED (lookup_name (get_identifier (vmt_name))) = 1;

  /* Attach VMT_TYPE to the implicit VMT field of the object
   * (At the moment it still has the inherited type or ^void type.)
   */
  field = simple_get_field (get_identifier ("vmt"), dest,
                            "Internal GPC error: no implicit VMT field");
  if (field && TREE_CODE (TREE_TYPE (field)) == POINTER_TYPE)
    TREE_TYPE (field) = build_pointer_type (vmt_type);
}

/* Mark the last field in the list DEST as a virtual method
 * and return the list.  If the last field is a constructor
 * or a data field, complain.
 */
tree
mark_virtual_method (dest)
     tree dest;
{
  register tree field = dest;
  while (TREE_CHAIN (field))
    field = TREE_CHAIN (field);
  if (TREE_CODE (field) == FUNCTION_DECL)
    {
      if (PASCAL_CONSTRUCTOR_METHOD (field) == 0)
        PASCAL_VIRTUAL_METHOD (field) = 1;
      else
        error ("constructors must not be virtual");
    }
  else
    error ("cannot make data fields virtual");
  return dest;
}
