/* Interface routines for the GNU Pascal runtime system.
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
 *          Frank Heckenbach <frank@pascal.gnu.de>
 */

#include "gcc-version.h"
#include "config.h"
#include "gansidecl.h"
#ifdef EGCS
#include "system.h"
#endif

#include <stdio.h>

#include "gbe.h"
#include "gpc-defs.h"
#include "module.h"
#include "parse.h"
#include "types.h"
#include "util.h"
#include "rts/types.h"
#include "rts.h"



/* Internal flag:  Nonzero means to accept components of packed
 * structured variables as variable parameters.
 */
int allow_packed_var_parameters = 0;

/* no params to function */
tree ptype_void;

/* single param rts calls */
tree ptype_int;
tree ptype_longint;
tree ptype_char;
tree ptype_bool;
tree ptype_double;
tree ptype_long_double;
tree ptype_complex;
tree ptype_cstring;
tree ptype_string_schema;
tree ptype_const_string_schema;

/* any number of any parms */
tree ptype_vararg;

/* inline functions are not here */
struct rts_symbol rts[] = {
    { r_WRITE,      "_p_write",      NULL_RTX, NULL_TREE, 1},
    { r_READ,       "_p_read",       NULL_RTX, NULL_TREE, 1},
    { r_INITFDR,    "_p_initfdr",    NULL_RTX, NULL_TREE, 0},
    { r_LAZYTRYGET, "_p_lazytryget", NULL_RTX, NULL_TREE, 1},
    { r_LAZYGET,    "_p_lazyget",    NULL_RTX, NULL_TREE, 1},
    { r_LAZYUNGET,  "_p_lazyunget",  NULL_RTX, NULL_TREE, 1},

    { p_ARCTAN, "_p_arctan",    NULL_RTX, NULL_TREE, 0},
    { p_COS,    "_p_cos",       NULL_RTX, NULL_TREE, 0},
    { p_EXP,    "_p_exp",       NULL_RTX, NULL_TREE, 0},
    { p_LN,     "_p_ln",        NULL_RTX, NULL_TREE, 0},
    { p_SIN,    "_p_sin",       NULL_RTX, NULL_TREE, 0},
    { p_SQRT,   "_p_sqrt",      NULL_RTX, NULL_TREE, 0},

    { pp_ARCTAN, "_p_l_arctan",  NULL_RTX, NULL_TREE, 0},
    { pp_SIN,   "_p_l_sin",     NULL_RTX, NULL_TREE, 0},
    { pp_COS,   "_p_l_cos",     NULL_RTX, NULL_TREE, 0},
    { pp_EXP,   "_p_l_exp",     NULL_RTX, NULL_TREE, 0},
    { pp_LN,    "_p_l_ln",      NULL_RTX, NULL_TREE, 0},
    { pp_SQRT,  "_p_l_sqrt",    NULL_RTX, NULL_TREE, 0},

    { ucsd_INT, "_p_int",       NULL_RTX, NULL_TREE, 0},
    { bp_FRAC,  "_p_frac",      NULL_RTX, NULL_TREE, 0},

    { p_DISPOSE,"_p_dispose",   NULL_RTX, NULL_TREE, 0},
    { p_EOF,    "_p_eof",       NULL_RTX, NULL_TREE, 1},
    { p_EOLN,   "_p_eoln",      NULL_RTX, NULL_TREE, 1},
    { bp_FREEMEM,"_p_dispose",  NULL_RTX, NULL_TREE, 0},
    { p_GET,    "_p_get",       NULL_RTX, NULL_TREE, 1},
    { p_GOT,    "_p_got",       NULL_RTX, NULL_TREE, 1},
    { bp_GETMEM,"_p_new",       NULL_RTX, NULL_TREE, 0},
    { p_NEW,    "_p_new",       NULL_RTX, NULL_TREE, 0},
    { p_PAGE,   "_p_page",      NULL_RTX, NULL_TREE, 1},
    { p_PUT,    "_p_put",       NULL_RTX, NULL_TREE, 1},
    { p_RESET,  "_p_reset",     NULL_RTX, NULL_TREE, 1},
    { p_REWRITE,"_p_rewrite",   NULL_RTX, NULL_TREE, 1},
    { p_MARK,   "_p_mark",      NULL_RTX, NULL_TREE, 0},
    { p_RELEASE,"_p_release",   NULL_RTX, NULL_TREE, 0},
    { p_CLOSE,  "_p_close",     NULL_RTX, NULL_TREE, 1},
    { p_DONEFDR,"_p_donefdr",   NULL_RTX, NULL_TREE, 1},
    { r_POW,    "_p_pow",       NULL_RTX, NULL_TREE, 0},
    { r_EXPON,  "_p_expon",     NULL_RTX, NULL_TREE, 0},
    { rr_POW,   "_p_l_pow",     NULL_RTX, NULL_TREE, 0},
    { rr_EXPON, "_p_l_expon",    NULL_RTX, NULL_TREE, 0},

    /* Extended Pascal libcalls. */

    /* Functions with COMPLEX_TYPE parameters */
    { z_ABS,    "_p_z_abs",     NULL_RTX, NULL_TREE, 0},
    { z_ARCTAN, "_p_z_arctan",  NULL_RTX, NULL_TREE, 0},
    { z_COS,    "_p_z_cos",     NULL_RTX, NULL_TREE, 0},
    { z_EXP,    "_p_z_exp",     NULL_RTX, NULL_TREE, 0},
    { z_LN,     "_p_z_ln",      NULL_RTX, NULL_TREE, 0},
    { z_SIN,    "_p_z_sin",     NULL_RTX, NULL_TREE, 0},
    { z_SQRT,   "_p_z_sqrt",    NULL_RTX, NULL_TREE, 0},
    { z_POW,    "_p_z_pow",     NULL_RTX, NULL_TREE, 0},
    { z_EXPON,  "_p_z_expon",   NULL_RTX, NULL_TREE, 0},
    { p_POLAR,  "_p_polar",     NULL_RTX, NULL_TREE, 0},
    { p_ARG,    "_p_arg",       NULL_RTX, NULL_TREE, 0},

    { p_GETTIMESTAMP, "_p_gettimestamp", NULL_RTX, NULL_TREE, 0},
    { p_DATE,   "_p_date",      NULL_RTX, NULL_TREE, 0},
    { p_TIME,   "_p_time",      NULL_RTX, NULL_TREE, 0},
    { p_HALT,   "_p_halt",      NULL_RTX, NULL_TREE, 0},

    { p_EMPTY,  "_p_empty",     NULL_RTX, NULL_TREE, 1},
    { p_EXTEND, "_p_extend",    NULL_RTX, NULL_TREE, 1},
    { p_UPDATE, "_p_update",    NULL_RTX, NULL_TREE, 1},
    { p_POSITION,     "_p_position",    NULL_RTX, NULL_TREE, 1},
    { p_LASTPOSITION, "_p_lastposition",NULL_RTX, NULL_TREE, 1},
    { ucsd_SEEK,      "_p_seekall",     NULL_RTX, NULL_TREE, 1},
    { p_SEEKWRITE,    "_p_seekwrite",   NULL_RTX, NULL_TREE, 1},
    { p_SEEKREAD,     "_p_seekread",    NULL_RTX, NULL_TREE, 1},
    { p_SEEKUPDATE,   "_p_seekupdate",  NULL_RTX, NULL_TREE, 1},

    /* String functions. */
    { p_LENGTH, "_p_string", NULL_RTX, NULL_TREE, 0},
    { p_INDEX,  "_p_string", NULL_RTX, NULL_TREE, 0},  /* reserved word */
    { p_SUBSTR, "_p_string", NULL_RTX, NULL_TREE, 0},
    { p_TRIM,   "_p_string", NULL_RTX, NULL_TREE, 0},
    /* Lexicographic string comparisons. */
    { p_EQ,     "_p_string", NULL_RTX, NULL_TREE, 0},
    { p_LT,     "_p_string", NULL_RTX, NULL_TREE, 0},
    { p_GT,     "_p_string", NULL_RTX, NULL_TREE, 0},
    { p_NE,     "_p_string", NULL_RTX, NULL_TREE, 0},
    { p_LE,     "_p_string", NULL_RTX, NULL_TREE, 0},
    { p_GE,     "_p_string", NULL_RTX, NULL_TREE, 0},
    /* String comparisons with space padding. */
    { '=',      "_p_string", NULL_RTX, NULL_TREE, 0},
    { LEX_NE,   "_p_string", NULL_RTX, NULL_TREE, 0},
    { '<',      "_p_string", NULL_RTX, NULL_TREE, 0},
    { LEX_LE,   "_p_string", NULL_RTX, NULL_TREE, 0},
    { '>',      "_p_string", NULL_RTX, NULL_TREE, 0},
    { LEX_GE,   "_p_string", NULL_RTX, NULL_TREE, 0},

    /* Read from string, write to string. */
    { p_READSTR, "_p_readstr", NULL_RTX, NULL_TREE, 1},
    { p_WRITESTR,"_p_writestr",NULL_RTX, NULL_TREE, 1},
    { ucsd_STR,  "_p_writestr",NULL_RTX, NULL_TREE, 1},

    /* BP's `Val' are *many* RTS functions. */
    { bp_VAL_byteint_check,   "_p_val_byteint_check",  NULL_RTX, NULL_TREE, 0},
    { bp_VAL_shortint_check,  "_p_val_shortint_check", NULL_RTX, NULL_TREE, 0},
    { bp_VAL_integer_check,   "_p_val_integer_check",  NULL_RTX, NULL_TREE, 0},
    { bp_VAL_medint_check,    "_p_val_medint_check",   NULL_RTX, NULL_TREE, 0},
    { bp_VAL_longint_check,   "_p_val_longint_check",  NULL_RTX, NULL_TREE, 0},
    { bp_VAL_bytecard_check,  "_p_val_bytecard_check",  NULL_RTX, NULL_TREE, 0},
    { bp_VAL_shortcard_check, "_p_val_shortcard_check", NULL_RTX, NULL_TREE, 0},
    { bp_VAL_cardinal_check,  "_p_val_cardinal_check",  NULL_RTX, NULL_TREE, 0},
    { bp_VAL_medcard_check,   "_p_val_medcard_check",   NULL_RTX, NULL_TREE, 0},
    { bp_VAL_longcard_check,  "_p_val_longcard_check",  NULL_RTX, NULL_TREE, 0},
    { bp_VAL_byteint_nocheck,   "_p_val_byteint_nocheck",  NULL_RTX, NULL_TREE, 0},
    { bp_VAL_shortint_nocheck,  "_p_val_shortint_nocheck", NULL_RTX, NULL_TREE, 0},
    { bp_VAL_integer_nocheck,   "_p_val_integer_nocheck",  NULL_RTX, NULL_TREE, 0},
    { bp_VAL_medint_nocheck,    "_p_val_medint_nocheck",   NULL_RTX, NULL_TREE, 0},
    { bp_VAL_longint_nocheck,   "_p_val_longint_nocheck",  NULL_RTX, NULL_TREE, 0},
    { bp_VAL_bytecard_nocheck,  "_p_val_bytecard_nocheck",  NULL_RTX, NULL_TREE, 0},
    { bp_VAL_shortcard_nocheck, "_p_val_shortcard_nocheck", NULL_RTX, NULL_TREE, 0},
    { bp_VAL_cardinal_nocheck,  "_p_val_cardinal_nocheck",  NULL_RTX, NULL_TREE, 0},
    { bp_VAL_medcard_nocheck,   "_p_val_medcard_nocheck",   NULL_RTX, NULL_TREE, 0},
    { bp_VAL_longcard_nocheck,  "_p_val_longcard_nocheck",  NULL_RTX, NULL_TREE, 0},
    { bp_VAL_shortreal, "_p_val_shortreal", NULL_RTX, NULL_TREE, 0},
    { bp_VAL_real,      "_p_val_real",      NULL_RTX, NULL_TREE, 0},
    { bp_VAL_longreal,  "_p_val_longreal",  NULL_RTX, NULL_TREE, 0},

    /* Set operations */
    { set_card,          "_p_set_card",          NULL_RTX, NULL_TREE, 0},
    { set_equal,         "_p_set_equal",         NULL_RTX, NULL_TREE, 0},
    { set_le,            "_p_set_le",            NULL_RTX, NULL_TREE, 0},
    { set_less,          "_p_set_less",          NULL_RTX, NULL_TREE, 0},
    { set_in,            "_p_set_in",            NULL_RTX, NULL_TREE, 0},
    { set_clear,         "_p_set_clear",         NULL_RTX, NULL_TREE, 0},
    { set_include,       "_p_set_include",       NULL_RTX, NULL_TREE, 0},
    { set_exclude,       "_p_set_execlude",      NULL_RTX, NULL_TREE, 0},
    { set_include_range, "_p_set_include_range", NULL_RTX, NULL_TREE, 0},
    { set_copy,          "_p_set_copy",          NULL_RTX, NULL_TREE, 0},
    { set_intersection,  "_p_set_intersection",  NULL_RTX, NULL_TREE, 0},
    { set_union,         "_p_set_union",         NULL_RTX, NULL_TREE, 0},
    { set_diff,          "_p_set_diff",          NULL_RTX, NULL_TREE, 0},
    { set_symdiff,       "_p_set_symdiff",       NULL_RTX, NULL_TREE, 0},

    /* Binding routines */
    { p_BIND,    "_p_bind",    NULL_RTX, NULL_TREE, 1},
    { p_BINDING, "_p_binding", NULL_RTX, NULL_TREE, 0},
    { p_UNBIND,  "_p_unbind",  NULL_RTX, NULL_TREE, 1},

    /* UCSD/BP extensions */
    { ucsd_FILLCHAR,    "memset",       NULL_RTX, NULL_TREE, 0},
    { bp_MOVE,          "_p_move",      NULL_RTX, NULL_TREE, 0},
    { ucsd_MOVELEFT,    "_p_moveleft",  NULL_RTX, NULL_TREE, 0},
    { ucsd_MOVERIGHT,   "_p_moveright", NULL_RTX, NULL_TREE, 0},
    { ucsd_BLOCKREAD,   "_p_blockread", NULL_RTX, NULL_TREE, 1},
    { ucsd_BLOCKWRITE,  "_p_blockwrite",NULL_RTX, NULL_TREE, 1},
    { bp_ASSIGN,        "_p_assign",    NULL_RTX, NULL_TREE, 1},
    { bp_APPEND,        "_p_extend",    NULL_RTX, NULL_TREE, 1},  /* Just a synonym */
    { bp_FILEPOS,       "_p_position",  NULL_RTX, NULL_TREE, 1},  /* Just a synonym */
    { bp_FILESIZE,      "_p_getsize",   NULL_RTX, NULL_TREE, 1},  /* Just a synonym */
    { bp_TRUNCATE,      "_p_truncate",  NULL_RTX, NULL_TREE, 1},
    { bp_FLUSH,         "_p_flush",     NULL_RTX, NULL_TREE, 1},
    { bp_ERASE,         "_p_erase",     NULL_RTX, NULL_TREE, 1},
    { bp_RENAME,        "_p_rename",    NULL_RTX, NULL_TREE, 1},
    { bp_CHDIR,         "_p_chdir",     NULL_RTX, NULL_TREE, 1},
    { bp_MKDIR,         "_p_mkdir",     NULL_RTX, NULL_TREE, 1},
    { bp_RMDIR,         "_p_rmdir",     NULL_RTX, NULL_TREE, 1},
    { bp_RUNERROR,      "_p_runerror",  NULL_RTX, NULL_TREE, 0},  /* Just a synonym */
    { ucsd_INSERT,      "_p_insert",    NULL_RTX, NULL_TREE, 0},
    { ucsd_DELETE,      "_p_delete",    NULL_RTX, NULL_TREE, 0},
    { ucsd_POS,         "_p_string",    NULL_RTX, NULL_TREE, 0},
    { ucsd_COPY,        "_p_string",    NULL_RTX, NULL_TREE, 0},
    { ucsd_IORESULT,    "_p_ioresult",  NULL_RTX, NULL_TREE, 0},
    { gpc_UPCASE,       "_p_gpc_upcase",   NULL_RTX, NULL_TREE, 0},
    { bp_UPCASE,        "_p_bp_upcase",    NULL_RTX, NULL_TREE, 0},
    { bp_INCLUDE,       "_p_set_include",  NULL_RTX, NULL_TREE, 0},
    { bp_EXCLUDE,       "_p_set_exclude",  NULL_RTX, NULL_TREE, 0},
    { bp_RANDOM,        "_p_randint",   NULL_RTX, NULL_TREE, 0},
    { bp_RANDREAL,      "_p_randreal",  NULL_RTX, NULL_TREE, 0},
    { bp_RANDOMIZE,     "_p_randomize", NULL_RTX, NULL_TREE, 0},

    /* GPC extensions.  Pax used to have `DefineSize'. */
    { p_DEFINESIZE,     "_p_definesize", NULL_RTX, NULL_TREE, 1},
    { gpc_LOCASE,       "_p_gpc_locase", NULL_RTX, NULL_TREE, 0},
    { bp_LOCASE,        "_p_bp_locase",  NULL_RTX, NULL_TREE, 0},
    { gpc_SETFILETIME,  "_p_set_file_time",NULL_RTX, NULL_TREE, 1},
    { gpc_NEWCSTRING,   "_p_newcstring", NULL_RTX, NULL_TREE, 0},
    { gpc_CSTRINGCOPYSTRING, "_p_cstringcopystring",     NULL_RTX, NULL_TREE, 0},
    { gpc_CSTRING2STRING, "_p_copycstring", NULL_RTX, NULL_TREE, 0},

    /* Internal functions */
    { gpc_IOCHECK, "_p_check_inoutres", NULL_RTX, NULL_TREE, 0},
    { gpc_RUNTIME_ERROR, "_p_error", NULL_RTX, NULL_TREE, 0},

    { 0,         NULL,         NULL_RTX, NULL_TREE, 0}
};



/* Declare functions with prototypes. */

static void rts_read PROTO ((int, tree));
static void rts_write PROTO ((int, tree));
static void rts_val PROTO ((tree));
static tree rts_string PROTO ((int, tree));
static tree pascal_unpack_and_pack PROTO ((int, tree, tree, tree));



/*
 * RTS calls.
 */

/* Some rts function formal parameters are constructed
 * by this routine.
 */
tree
do_ptype (type, var, last)
     tree type;
     int  var;  /* 0: use type; 1: build_reference; 2: build_pointer */
     int  last;
{
    tree temp;

    if (var == 1)
        type = build_reference_type (type);
    else if (var == 2)
        type = build_pointer_type (type);

    temp = build_tree_list (NULL_TREE, type);

    /* True if this is the last parameter to the function */
    if (last)
        temp = chainon (temp, ptype_void);

    return temp;
}

/* Declares some useful rts call types
 */
void
declare_rts_types ()
{
  ptype_void = build_tree_list (NULL_TREE, void_type_node);
  ptype_vararg = NULL_TREE;

  ptype_int = do_ptype (integer_type_node, 0, 1);
  ptype_longint = do_ptype (long_long_integer_type_node, 0, 1);
  ptype_char = do_ptype (char_type_node, 0, 1);

  ptype_bool = do_ptype (boolean_type_node, 0, 1);
  ptype_double = do_ptype (double_type_node, 0, 1);
  ptype_long_double = do_ptype (long_double_type_node, 0, 1);
  ptype_complex = do_ptype (complex_type_node, 0, 1);

  /* @@@ `cstring_type_node' is not yet initialized at this moment.
   */
  ptype_cstring = do_ptype (string_type_node, 0, 1);

  ptype_string_schema = do_ptype (string_schema_proto_type, 1, 1);
  ptype_const_string_schema = do_ptype (const_string_schema_proto_type, 1, 1);
}

/* RTS_ID : What routine we are calling
 * VALUE_TYPE : return value from calling NAME
 * FORMAL_PARAMS : List of formal parameters; NULL_TREE means varargs.
 * PARAMS is a list--a chain of TREE_LIST nodes--in which the
 *        TREE_VALUE of each node is a parameter-expression.
 */
tree
rts_call (rts_id, value_type, formal_params, params)
     int rts_id;
     tree value_type;
     tree formal_params;
     tree params;
{
  register int rts_routine_found;
  int index;
  tree fun, result;

  for (index = 0; (rts_routine_found = rts[index].val) != rts_id; index++)
    assert (rts_routine_found);

  fun = build_decl (FUNCTION_DECL,
                    get_identifier(rts[index].name),
                    build_function_type (value_type, formal_params));

  DECL_EXTERNAL (fun) = 1;
  TREE_PUBLIC (fun) = 1;

  DECL_SOURCE_FILE (fun) = "Pascal Run Time System";
  DECL_SOURCE_LINE (fun) = 1;

  /* Add `attribute (noreturn)' if appropriate.
   */
  switch (rts_id)
    {
      case p_HALT:
      case bp_RUNERROR:
      case gpc_RUNTIME_ERROR:
        {
          TREE_THIS_VOLATILE (fun) = 1;
          break;
        }
      default:
        /* nothing */;
    }

  rest_of_decl_compilation (fun, NULL_PTR, 0, 0);

  /* @@@ Can't reuse the fun later because the argument types
   * may vary in each call (i.e. the _p_new takes any pointer)
   *
   * Should check if could use `void *' et. al. for these cases ...
   *
   * fh: Apart from the few varargs routines, this must certainly be
   * possible, since all the RTS routines are declared as regular C
   * or Pascal routines. The declarations are readily available in
   * the rts directory.
   */
  if (! rts[index].fun && ! flag_syntax_only)
    {
      if (! rts[index].symref)
        abort ();

      /* If this machine requires an external definition for library
       * functions, write one out.
       *
       * HP-UX .import FOO directives are done here.
       * I don't think this needs to be called but once per routine.
       */
      assemble_external_libcall (rts[index].symref);
      rts[index].fun = fun;
    }

  allow_packed_var_parameters++;
  result = build_function_call (fun, params);
  allow_packed_var_parameters--;
  if (flag_io_checking && rts[index].io_critical)
    {
      tree condition;
      tree inoutres_name = get_identifier ("_p_inoutres");
      tree inoutres_var = lookup_name (inoutres_name);

      if (TREE_TYPE (result) == void_type_node
          || TREE_TYPE (result) == error_mark_node)
        {
          expand_expr_stmt (result);
          result = error_mark_node;  /* No error; just do nothing.
                                      * This already has been expanded.
                                      */
        }
      else
        {
          tree temp_var = make_new_variable ("io_check", TREE_TYPE (result));
          expand_expr_stmt (build_modify_expr (temp_var, NOP_EXPR, result));
          result = temp_var;
        }

      if (inoutres_var == NULL_TREE)
        {
          declare_vars (build_tree_list (NULL_TREE, inoutres_name),
                        integer_type_node, NULL_TREE, NULL_PTR, VQ_EXTERNAL,
                        0, NULL_TREE);
          inoutres_var = lookup_name (inoutres_name);
        }
      condition = build_pascal_binary_op (NE_EXPR, inoutres_var,
                                          integer_zero_node);
      expand_start_cond (condition, 0);
      expand_expr_stmt (rts_call (gpc_IOCHECK, void_type_node,
                        NULL_TREE, NULL_TREE));
      expand_end_cond ();
    }
  return result;
}

/* Read from files and strings.
 */
static void
rts_read (rts_id, params)
     int rts_id;
     tree params;
{
    tree parm;
    int length;
    tree arglist = NULL_TREE;
    tree string_curlen = NULL_TREE;
    tree string_length = NULL_TREE;

    if (rts_id == p_READSTR)
      {
        tree string, string_length;

        if (! params
            || ! (is_string_type (TREE_VALUE (params), 1)
                  || ((flag_extended_syntax || (flag_what_pascal & B_D_PASCAL))
                      && TYPE_MAIN_VARIANT (TREE_TYPE (TREE_VALUE (params)))
                         == cstring_type_node)))
          {
            error ("first argument to `ReadStr' must be the string to read from");
            if (! flag_extended_syntax
                && TYPE_MAIN_VARIANT (TREE_TYPE (TREE_VALUE (params)))
                   == cstring_type_node)
              cstring_inform ();
            return;
          }

        /* Find out the number of args we are reading.
         * ??? This does not work if it is done
         * after `params = TREE_CHAIN (params)'.  :-(
         */
        length = list_length (params) - 1;

        string = TREE_VALUE (params);
        params = TREE_CHAIN (params);

        if (TYPE_MAIN_VARIANT (TREE_TYPE (string)) == cstring_type_node)
          string_length = build_int_2 (-1, -1);
        else
          {
            string_length = PASCAL_STRING_LENGTH (string);
            string = build1 (ADDR_EXPR, string_type_node,
                             PASCAL_STRING_VALUE (string));
          }

        /* First three args:
         *  string pointer, current length, number of args
         *
         * Note that the string does not need to be an lvalue.
         */
        arglist = tree_cons (NULL_TREE, string,
                             tree_cons (NULL_TREE, string_length,
                                        build_tree_list (NULL_TREE,
                                                         size_int (length))));
      }
    else
      {
        tree file;
        int is_text;

        if (params
            && TREE_CODE (TREE_TYPE (TREE_VALUE (params))) == FILE_TYPE)
          {
            file = TREE_VALUE (params);
            params = TREE_CHAIN (params);
          }
        else
          file = get_standard_input ();

        if (rts_id == p_READ && ! params)
          {
            if ((flag_what_pascal & B_D_PASCAL) == 0)
              pedwarn ("`read' without variables to read - ignored");
            return;
          }

        is_text = TYPE_FILE_TEXT (TREE_TYPE (file)) != NULL_TREE;

        if (rts_id == p_READLN)
          if (! is_text)
            {
              error ("`readln' is allowed only when reading from files of type `text'");
              return;
            }

        if (rts_id == p_READ)
          {
            if (params == NULL_TREE)
              {
                error ("too few parameters to predefined procedure `read'");
                return;
              }

            /* Non TEXT file reads */
            if (! is_text)
              {
                tree the_file = build_tree_list (NULL_TREE, file);
                tree buffer = build_buffer_ref (file, 0);
                for (parm = params; parm; parm = TREE_CHAIN (parm))
                  {
                    /* Move the contents of the file buffer to the read param.
                     */
                    expand_expr_stmt (build_modify_expr (TREE_VALUE (parm),
                                                         NOP_EXPR, buffer));

                    /* Do a get from the file.
                     */

                    /* After reading the last parameter, do only a _p_got()
                       so that the buffer behaves lazy */
                    build_rts_call (TREE_CHAIN (parm) ? p_GET : p_GOT, the_file);
                  }
                return;
              }
          }
        /* find out the number of args we are reading */
        length = list_length (params);
        length += rts_id == p_READLN;   /* add P_LINE */

        allow_packed_var_parameters++;
        arglist = tree_cons (NULL_TREE, build_unary_op (ADDR_EXPR, file, 0),
                             build_tree_list (NULL_TREE, size_int (length)));
        allow_packed_var_parameters--;
      }

    for (parm = params; parm; parm = TREE_CHAIN (parm))
      {
        tree p = TREE_VALUE (parm);
        enum tree_code code = TREE_CODE (TREE_TYPE (p));
        int what;

        length--;
        switch (code)
          {
            case INTEGER_TYPE:
              {
                /* Handle all integer types.
                 */
                if (! TREE_UNSIGNED (TREE_TYPE (p)))
                  {
                    int prec = TYPE_PRECISION (TREE_TYPE (p));
                    if (prec > TYPE_PRECISION (long_integer_type_node))
                      what = P_S_LONGLONG;
                    else if (prec > TYPE_PRECISION (integer_type_node))
                      what = P_S_LONG;
                    else if (prec > TYPE_PRECISION (short_integer_type_node))
                      what = P_S_INT;
                    else if (prec > TYPE_PRECISION (signed_char_type_node))
                      what = P_S_SHORT;
                    else
                      what = P_S_BYTE;
                  }
                else
                  {
                    int prec = TYPE_PRECISION (TREE_TYPE (p));
                    if (prec > TYPE_PRECISION (long_unsigned_type_node))
                      what = P_U_LONGLONG;
                    else if (prec > TYPE_PRECISION (unsigned_type_node))
                      what = P_U_SHORT;
                    else if (prec > TYPE_PRECISION (short_unsigned_type_node))
                      what = P_U_INT;
                    else if (prec > TYPE_PRECISION (unsigned_char_type_node))
                      what = P_U_SHORT;
                    else
                      what = P_U_BYTE;
                  }
                /*@@@@@ INT_READ_CHECK_MASK was removed (19981223). Now, the
                        `check' parameter to _p_readi() has to be used instead
                if (flag_input_range_checking)
                  what |= INT_READ_CHECK_MASK; */
                if ((flag_what_pascal & B_D_PASCAL) != 0)
                  what |= INT_READ_HEX_MASK;
                else if (pedantic == 0
                         && flag_what_pascal == 0)
                  what |= INT_READ_BASE_SPEC_MASK | INT_READ_HEX_MASK;
                break;
              }
            case CHAR_TYPE :
              what = P_CHAR;
              break;
            case REAL_TYPE :
              /* Handle all real types. */
              {
                int prec = TYPE_PRECISION (TREE_TYPE (p));
                if (prec == TYPE_PRECISION (double_type_node))
                  what = P_REAL;
                else if (prec == TYPE_PRECISION (float_type_node))
                  what = P_SHORT_REAL;
                else if (prec == TYPE_PRECISION (long_double_type_node))
                  what = P_LONG_REAL;
                else
                  {
                    error ("unknown real type to read");
                    return;
                  }
                if ((flag_what_pascal & STANDARD_PASCAL) != 0
                    && (flag_what_pascal & ~STANDARD_PASCAL) == 0)
                  what |= REAL_READ_ISO7185_ONLY_MASK;
                break;
              }
            break;

            case RECORD_TYPE: /* String schema. */
            case ARRAY_TYPE:  /* Fixed length string. */
              switch (is_string_type (p, 1))
                {
                  case 0:
                    error ("only packed arrays of char may be read from `text' files");
                    continue;
                  case 1:
                    break;
                  case 2:
                    /* @@@ Should generate a runtime check for conformant arrays
                     * (low index has to be 1 for arrays to be valid string-type).
                     *
                     * For now, just read them.
                     */
                    break;
                  default:
                    abort ();
                }

              if (PEDANTIC (E_O_B_D_PASCAL))
                pedwarn ("ISO 7185 Standard Pascal does not allow reading strings from textfiles");

              if (is_variable_string_type (TREE_TYPE (p)))
                {
                  what = P_STRING;

                  /* The run time system expects another argument
                   * before max length: a pointer to int where
                   * it stores the current length of the string.
                   *
                   * This needs to be an lvalue.
                   */
                  string_curlen = build_unary_op (ADDR_EXPR, PASCAL_STRING_LENGTH (p), 0);

                  /* String max length.
                   */
                  string_length = PASCAL_STRING_CAPACITY (p);
                }
              else
                {
                  what = P_FIXED_STRING;

                  /* String max length.
                   */
                  string_length = pascal_array_type_nelts (TREE_TYPE (PASCAL_STRING_VALUE (p)));
                }

              /* The char store.
               */
              p = PASCAL_STRING_VALUE (p);

              break;

            default:
              error ("parameter of `read'/`readln' from `text' file is of wrong type");
              /* FALLTHROUGH */

            case ERROR_MARK:
              return;
          }

        /* Inform the run time system of the next arg type.
         */
        arglist = chainon (arglist,
                           build_tree_list (NULL_TREE, size_int (what)));

        /* Do not allow to read read-only locations.
         */
        typed_const_warning (p);
        if (TREE_CONSTANT (p))
          error ("read-only location passed to `read'/`readln'");

        /* Pass the address of the variable we want to read.
         * According to ISO, packed fields are okay here :-/
         * (but nowhere else when it comes to `var' parameters)-|.
         */
        allow_packed_var_parameters++;
        if (is_packed_field (p))
          p = expand_packed_field (p, TREE_TYPE (p));
        arglist = chainon (arglist,
                           build_tree_list (NULL_TREE,
                                            build_unary_op (ADDR_EXPR, p, 0)));
        allow_packed_var_parameters--;
        if (what == P_STRING)
          arglist = chainon (arglist,
                             build_tree_list (NULL_TREE, string_curlen));

        if (what == P_STRING || what == P_FIXED_STRING)
          arglist = chainon (arglist,
                             build_tree_list (NULL_TREE, string_length));

        /* Mark that we have assigned a value to this variable.
         */
        while (TREE_CODE (p) == COMPONENT_REF)
          p = TREE_OPERAND (p, 0);
        if (TREE_CODE (p) == VAR_DECL)
          PASCAL_VALUE_ASSIGNED (p) = 1;
      }

    if (rts_id == p_READLN)
      {
        /* Inform the run time system that we should do a readln */
        arglist = chainon (arglist, build_tree_list (NULL_TREE,
                                                     size_int (P_LINE)));
        length--;
      }

    if (length)
      abort ();

    expand_expr_stmt (rts_call ((rts_id == p_READSTR) ? p_READSTR : r_READ,
                                void_type_node, NULL_TREE, arglist));
}

/* Write to files and strings.
 */
static void
rts_write (rts_id, params)
     int rts_id;
     tree params;
{
    tree parm;
    tree arglist;
    int length;

    if (rts_id == p_WRITESTR || rts_id == ucsd_STR)
      {
        tree string, string_pointer;
        int string_type = 0;

        if (rts_id == p_WRITESTR)
          {
            if (PEDANTIC (E_O_PASCAL))
              pedwarn ("`WriteStr' is an ISO 10206 Extended Pascal extension");
            if (! params || ! is_string_type (TREE_VALUE (params), 1))
              {
                error ("first argument to `WriteStr' must be the string to write to");
                return;
              }

            /* Mark that we are assigning a value to the string variable.
             */
            string = TREE_VALUE (params);
            while (TREE_CODE (string) == COMPONENT_REF)
              string = TREE_OPERAND (string, 0);
            if (TREE_CODE (string) == VAR_DECL)
              PASCAL_VALUE_ASSIGNED (string) = 1;

            string = TREE_VALUE (params);
            params = TREE_CHAIN (params);

            /* Don't allow writes to a constant string.
             */
            if (really_constant_p (string) || TREE_READONLY (string))
              readonly_warning (string, "`WriteStr': modification");
          }
        else
          {
            if (PEDANTIC (U_B_D_PASCAL))
              pedwarn ("`Str' is an UCSD Pascal extension");

            if (params)
              {
                tree p = params, q = NULL_TREE;
                while (TREE_CHAIN (p))
                  {
                    q = p;
                    p = TREE_CHAIN (p);
                  }
                string = p;
                if (q)
                  TREE_CHAIN (q) = NULL_TREE;
                else
                  {
                    error ("too few arguments to `Str'");
                    return;
                  }
              }
            else
              string = NULL_TREE;

            if (! string || ! is_string_type (TREE_VALUE (string), 1))
              {
                error ("last argument to `Str' must be the string to write to");
                return;
              }

            string = TREE_VALUE (string);

            /* Don't allow writes to a constant string.
             */
            if (really_constant_p (string) || TREE_READONLY (string))
              readonly_warning (string, "`Str': modification");
          }

        /* Find out the number of args we are writing.
         */
        length = list_length (params);
        if (rts_id == ucsd_STR && length > 1 && (pedantic || flag_what_pascal))
          pedwarn ("multiple values in `Str' are a GNU Pascal extension");

        /* First four or five args: type of string, pointer to string,
         * current length, maximum length, number of values.
         * For fixed strings, no current length is passed.
         * The string needs to be an lvalue.
         */
        if (PASCAL_TYPE_STRING (TREE_TYPE (string)))
          {
            /* String schema.
             */
            string_type = P_STRING;
          }
        else if (TYPE_MAIN_VARIANT (base_type (TREE_TYPE (string)))
            == cstring_type_node)
          {
            /* CString.
             */
            string_type = P_CSTRING;
          }
        else if (TREE_CODE (TREE_TYPE (string)) == ARRAY_TYPE
                 && TREE_CODE (TREE_TYPE (TREE_TYPE (string))) == CHAR_TYPE)
          {
            /* If this array is not packed, a warning about ISO
             * violation already has been given in the call to
             * is_string_type() above.
             */
            string_type = P_FIXED_STRING;
          }
        else
          {
            /* It has already been checked that STRING is a string.
             */
            abort ();
          }

        /* Tell the run time system what to do if the data is too long
         * for this string: truncate or abort.
         */
        if (flag_truncate_strings)
          string_type |= TRUNCATE_STRING_MASK;

        allow_packed_var_parameters++;
        string_pointer = build_unary_op (ADDR_EXPR,
                                         PASCAL_STRING_VALUE (string), 0);
        allow_packed_var_parameters--;
        arglist = build_tree_list (NULL_TREE, build_int_2 (string_type, 0));
        arglist = chainon (arglist,
                           build_tree_list (NULL_TREE, string_pointer));
        if ((string_type & OUTPUT_TYPE_MASK) == P_STRING)
          {
            tree curlen = build_unary_op (ADDR_EXPR,
                                          PASCAL_STRING_LENGTH (string), 0);
            arglist = chainon (arglist, build_tree_list (NULL_TREE, curlen));
            arglist = chainon (arglist,
                               build_tree_list (NULL_TREE,
                                 PASCAL_STRING_CAPACITY (string)));
          }
        else
          arglist = chainon (arglist,
                             build_tree_list (NULL_TREE,
                               pascal_array_type_nelts (TREE_TYPE (
                                   PASCAL_STRING_VALUE (string)))));
        arglist = chainon (arglist,
                           build_tree_list (NULL_TREE,
                                            build_int_2 (length, 0)));
      }
    else
      {
        tree file;
        int is_text;

        if (params
            && TREE_CODE (TREE_TYPE (TREE_VALUE (params))) == FILE_TYPE)
          {
            file = TREE_VALUE (params);
            params = TREE_CHAIN (params);
          }
        else
          file = get_standard_output ();

        is_text = TYPE_FILE_TEXT (TREE_TYPE (file)) != NULL_TREE;

        if (rts_id == p_WRITELN)
          if (! is_text)
            {
              error ("`writeln' is allowed only when writing to files of type `text'");
              return;
            }

        if (rts_id == p_WRITE)
          {
            if (! params)
              {
                if ((flag_what_pascal & B_D_PASCAL) == 0)
                  pedwarn ("`write' without values to write - ignored");
                return;
              }

            /* Non TEXT file writes.
             */
            if (! is_text)
              {
                tree the_file = build_tree_list (NULL_TREE, file);
                tree buffer = build_buffer_ref (file, 1);
                for (parm = params; parm; parm = TREE_CHAIN (parm))
                  {
                    /* Check for strings being chars.
                     */
                    TREE_VALUE (parm) = string_may_be_char (TREE_VALUE (parm));

                    /* Check whether the types match.
                     */
                    if (comptypes (TREE_TYPE (buffer),
                                   TREE_TYPE (TREE_VALUE (parm))) == 0)
                      error ("incompatible types in `write'");
                    else
                      {
                        /* Move the contents of the write parm to the file buffer.
                         */
                        expand_assignment (buffer, TREE_VALUE (parm), 0, 0);

                        /* Do a put to the file.
                         */
                        build_rts_call (p_PUT, the_file);
                      }
                  }
                return;
              }
          }
        /* find out the number of args we are writing */
        length = list_length (params);
        length += rts_id == p_WRITELN;  /* add P_LINE */

        allow_packed_var_parameters++;
        arglist = tree_cons (NULL_TREE, build_unary_op (ADDR_EXPR, file, 0),
                             build_tree_list (NULL_TREE, size_int (length)));
        allow_packed_var_parameters--;
      }

    for (parm = params; parm; parm = TREE_CHAIN (parm))
      {
        tree field1;
        tree field2;
        int what;
        tree p = TREE_VALUE (parm);
        enum tree_code code;
        if (TREE_CODE (p) == STRING_CST && TREE_STRING_LENGTH (p) > 1)
          p = string_may_be_char (p);
        code = TREE_CODE (base_type (TREE_TYPE (p)));

        length--;

        if (TREE_PURPOSE (parm))
          {
            field1 = TREE_VALUE (TREE_PURPOSE (parm));
            field2 = TREE_PURPOSE (TREE_PURPOSE (parm));
          }
        else
          {
            field1 = NULL_TREE;
            field2 = NULL_TREE;
          }

        if (field1)
          field1 = convert (integer_type_node, field1);

        if (field2)
          field2 = convert (integer_type_node, field2);

        if (rts_id == ucsd_STR
            && code != INTEGER_TYPE
            && code != REAL_TYPE
            && (pedantic || flag_what_pascal))
          pedwarn ("non-numeric values in `Str' are a GNU Pascal extension");

        switch (code)
          {
            case INTEGER_TYPE:
              if (TREE_CODE (p) == INTEGER_CST)
                {
                  if (int_fits_type_p (p, integer_type_node))
                    p = convert (integer_type_node, p);
                  else if (int_fits_type_p (p, unsigned_type_node))
                    p = convert (unsigned_type_node, p);
                }

              /* Handle all integer types.
               */
              if (! TREE_UNSIGNED (TREE_TYPE (p)))
                {
                  if (TYPE_PRECISION (TREE_TYPE (p)) >
                      TYPE_PRECISION (integer_type_node))
                    {
                      what = P_S_LONGLONG;
                      p = convert (long_long_integer_type_node, p);
                    }
                  else
                    {
                      what = P_S_INT;
                      p = convert (integer_type_node, p);
                    }
                }
              else
                {
                  if (TYPE_PRECISION (TREE_TYPE (p)) >
                      TYPE_PRECISION (unsigned_type_node))
                    {
                      what = P_U_LONGLONG;
                      p = convert (long_long_unsigned_type_node, p);
                    }
                  else
                    {
                      what = P_U_INT;
                      p = convert (unsigned_type_node, p);
                    }
                }

              if (field1)
                  what |= FIX_WIDTH_MASK;
              else if ((what & OUTPUT_TYPE_MASK) == P_S_LONGLONG
                       || (what & OUTPUT_TYPE_MASK) == P_U_LONGLONG)
                {
                  if (long_int_out_width)
                    {
                      field1 = build_int_2 (long_int_out_width, 0);
                      what |= FIX_WIDTH_MASK;
                    }
                }
              else
                {
                  if (int_out_width)
                    {
                      field1 = build_int_2 (int_out_width, 0);
                      what |= FIX_WIDTH_MASK;
                    }
                }
              break;

            case BOOLEAN_TYPE:
              if (field1)
                what = P_BOOL | FIX_WIDTH_MASK;
              else if (bool_out_width)
                {
                  field1 = build_int_2 (bool_out_width, 0);
                  what = P_BOOL | FIX_WIDTH_MASK;
                }
              else
                what = P_BOOL;
              /* For va_arg: pass everything smaller than `int' as `int'.
               */
              p = convert (integer_type_node, p);
              break;

            case CHAR_TYPE:
              if (field1)
                what = P_CHAR | FIX_WIDTH_MASK;
              else
                what = P_CHAR;
              /* For va_arg: pass everything smaller than `int' as `int'.
               */
              p = convert (integer_type_node, p);
              break;

            case REAL_TYPE :
            {
              /* Handle also LongReals.
               */
              int long_real_flag = TYPE_PRECISION (TREE_TYPE (p)) >
                                   TYPE_PRECISION (double_type_node);
              what = P_LONG_REAL;
              p = convert (long_double_type_node, p);

              if (! flag_real_blank)
                what |= REAL_NOBLANK_MASK;

              if (flag_capital_exponent)
                what |= REAL_CAPITAL_EXP_MASK;

              if (field1)
                {
                  what |= FIX_WIDTH_MASK;
                  if (field2)
                    what |= FIX2_REAL_MASK;
                }
              else if (long_real_flag)
                {
                  if (long_real_out_width)
                    {
                      field1 = build_int_2 (long_real_out_width, 0);
                      what |= FIX_WIDTH_MASK;
                    }
                }
              else
                {
                  if (real_out_width)
                    {
                      field1 = build_int_2 (real_out_width, 0);
                      what |= FIX_WIDTH_MASK;
                    }
                }
              break;
            }

            case RECORD_TYPE:
            case ARRAY_TYPE:
              switch (is_string_type (p, 1))
                {
                  case 0:
                    error ("only packed arrays of char with low index 1 may be");
                    error (" written to text files");
                    continue;
                  case 1:
                    break;
                  case 2:
                    /* @@@ Should generate a runtime check for conformant arrays.
                     * (Low index has to be 1 for arrays to be valid string-type).
                     * For now, just write them out.
                     * Note that the field1 below will be incorrect if confarrays
                     * don't start from 1.
                     */
                    break;
                  default:
                    abort ();
                }

              what = (field1)
                 ? (flag_clip_strings ? (P_ANY_STRING | FIX_WIDTH_MASK | CLIP_STRING_MASK)
                                      : (P_ANY_STRING | FIX_WIDTH_MASK))
                 : P_ANY_STRING;

              if (field2)
                error ("second field width allowed only when writing `real' type");

              field2 = field1;
              field1 = convert (integer_type_node, PASCAL_STRING_LENGTH (p));
              p = PASCAL_STRING_VALUE (p);

              /* pass the address of the string */
              p = build1 (ADDR_EXPR, string_type_node, p);
              break;

            case POINTER_TYPE:
              if (TYPE_MAIN_VARIANT (base_type (TREE_TYPE (p))) == cstring_type_node)
                {
                  if (flag_extended_syntax || (flag_what_pascal & B_D_PASCAL))
                    {
                      what = field1
                           ? (flag_clip_strings ? (P_ANY_STRING | FIX_WIDTH_MASK | CLIP_STRING_MASK)
                                                : (P_ANY_STRING | FIX_WIDTH_MASK))
                           : P_ANY_STRING;

                      if (field2)
                        error ("second field width allowed only when writing `real' type");
                      field2 = field1;
                      field1 = build_int_2 (-1, -1);
                      break;
                    }
                  else
                    {
                      error ("parameter to `write'/`writeln' to `text' file is of wrong type");
                      cstring_inform ();
                      return;
                    }
                }

            default:
              error ("parameter to `write'/`writeln' to `text' file is of wrong type");
              /* FALLTHROUGH */

            case ERROR_MARK:
              return;
          }
        if (field2
            && (what & OUTPUT_TYPE_MASK) != P_LONG_REAL
            && (what & OUTPUT_TYPE_MASK) != P_ANY_STRING)
          warning ("second field width allowed only when writing `real' type");

        /* Inform the run time system of the next arg type.
         */
        arglist = chainon (arglist,
                           build_tree_list (NULL_TREE, size_int (what)));

        /* Pass the variable we want to write.
         */
        arglist = chainon (arglist, build_tree_list (NULL_TREE, p));

        if (field1)
            arglist = chainon (arglist, build_tree_list (NULL_TREE, field1));

        if (field2)
            arglist = chainon (arglist, build_tree_list (NULL_TREE, field2));
      }

    if (rts_id == p_WRITELN)
      {
        /* Inform the run time system that we should do a writeln.
         */
        arglist = chainon (arglist, build_tree_list (NULL_TREE,
                                                     size_int (P_LINE)));
        length--;
      }

    if (length)
        abort ();

    expand_expr_stmt (rts_call ((rts_id == p_WRITESTR)
                                ? p_WRITESTR : ((rts_id == ucsd_STR)
                                                ? ucsd_STR : r_WRITE),
                                void_type_node, NULL_TREE, arglist));
}

/* Implement BP's procedure `Val'.
 */
static void
rts_val (params)
     tree params;
{
  tree par, fpar, string, result_var, code;
  int rts_id;

  if (PEDANTIC (B_D_PASCAL))
    pedwarn ("`Val' is a Borland Pascal extension");

  /* Check the parameter list and transform it to that one needed by the
   * RTS function.
   *
   * @@ In the future, minimal and maximal value will be added for
   * range checking.
   */

  par = params;
  if (! par
      || ! (is_string_type (TREE_VALUE (par), 1)
            || TREE_CODE (TREE_TYPE (TREE_VALUE (par))) == CHAR_TYPE))
    {
      error ("string type required for first parameter of `Val'");
      return;
    }

  /* First parameter: pointer to the string data.
   */
  string = TREE_VALUE (par);
  if (TREE_CODE (TREE_TYPE (string)) == CHAR_TYPE)
    string = new_string_by_model (NULL_TREE, string, 1);
  TREE_VALUE (par) = PASCAL_STRING_VALUE (string);

  /* Second parameter: index of last char in string.
   */
  TREE_CHAIN (par) = chainon (build_tree_list (NULL_TREE,
                                               PASCAL_STRING_LENGTH (string)),
                              TREE_CHAIN (par));
  par = TREE_CHAIN (par);

  /* Third parameter: Flags.
   */
  TREE_CHAIN (par) = chainon (build_tree_list (NULL_TREE, integer_zero_node),
                              TREE_CHAIN (par));
  par = TREE_CHAIN (TREE_CHAIN (par));

  /* Fourth parameter: The result variable.
   * (For the user this is the second parameter.)
   */
  if (! par || (TREE_CODE (TREE_TYPE (TREE_VALUE (par))) != INTEGER_TYPE
                && TREE_CODE (TREE_TYPE (TREE_VALUE (par))) != REAL_TYPE))
    {
      error ("integer or real type required for second parameter of `Val'");
      return;
    }
  result_var = TREE_VALUE (par);

  /* The third user parameter is the code we assign the return value of the
   * RTS function to.  Take it out of business here.
   */
  if (! TREE_CHAIN (par)
      || TREE_CODE (TREE_TYPE (TREE_VALUE (TREE_CHAIN (par)))) != INTEGER_TYPE)
    {
      error ("integer type required for third parameter of `Val'");
      return;
    }
  code = TREE_VALUE (TREE_CHAIN (par));
  TREE_CHAIN (par) = NULL_TREE;

  /* Select the RTS function for this type.
   */
  if (TREE_CODE (TREE_TYPE (result_var)) == INTEGER_TYPE)
    {
      if (TYPE_PRECISION (TREE_TYPE (result_var))
          > TYPE_PRECISION (long_integer_type_node))
        rts_id = bp_VAL_longint_nocheck;
      else if (TYPE_PRECISION (TREE_TYPE (result_var))
               > TYPE_PRECISION (integer_type_node))
        rts_id = bp_VAL_medint_nocheck;
      else if (TYPE_PRECISION (TREE_TYPE (result_var))
               > TYPE_PRECISION (short_integer_type_node))
        rts_id = bp_VAL_integer_nocheck;
      else if (TYPE_PRECISION (TREE_TYPE (result_var))
               > TYPE_PRECISION (signed_char_type_node))
        rts_id = bp_VAL_shortint_nocheck;
      else
        rts_id = bp_VAL_byteint_nocheck;

      if (TREE_UNSIGNED (TREE_TYPE (result_var)))
        rts_id += bp_VAL_bytecard_nocheck - bp_VAL_byteint_nocheck;
    }
  else
    {
      /* Real type.
       */
      if (TYPE_PRECISION (TREE_TYPE (result_var))
          > TYPE_PRECISION (double_type_node))
        rts_id = bp_VAL_longreal;
      else if (TYPE_PRECISION (TREE_TYPE (result_var))
               > TYPE_PRECISION (float_type_node))
        rts_id = bp_VAL_real;
      else
        rts_id = bp_VAL_shortreal;
    }

  fpar = chainon (chainon (do_ptype (cstring_type_node, 0, 0),
                           do_ptype (integer_type_node, 0, 0)),
                  chainon (do_ptype (integer_type_node, 0, 0),
                           do_ptype (TREE_TYPE (result_var), 1, 1)));

  expand_expr_stmt (build_modify_expr (code, NOP_EXPR,
                        rts_call (rts_id, integer_type_node, fpar, params)));
}

/* Implement many string-related operations and functions.
 */
static tree
rts_string (rts_id, params)
     int rts_id;
     tree params;
{
    int opcode = 0;
    tree arglist = NULL_TREE;

    int parm_types = 0; /* Both are strings (default) */

    /* params != NULL, verified by the caller */
    tree parm1 = TREE_VALUE (params);
    tree parm2 = NULL_TREE;

    tree arg1 = NULL_TREE;
    tree len1 = NULL_TREE;
    tree arg2 = NULL_TREE;
    tree len2 = NULL_TREE;
    tree sub_from = NULL_TREE;
    tree sub_length = NULL_TREE;
    tree return_type;
    tree funcall;

    switch (rts_id)
      {
        case ucsd_POS:
          {
            /* Same as p_INDEX, but swap the first and second parameter.
             * (TREE_CHAIN (params) != NULL_TREE, verified by the caller.)
             */
            TREE_VALUE (params) = TREE_VALUE (TREE_CHAIN (params));
            TREE_VALUE (TREE_CHAIN (params)) = parm1;
            parm1 = TREE_VALUE (params);
            if (PEDANTIC (U_B_D_PASCAL))
              pedwarn ("`Pos' is an UCSD Pascal extension (ISO 10206 has `Index')");
            break;
          }

        case ucsd_COPY:
          {
            if (PEDANTIC (U_B_D_PASCAL))
              pedwarn ("`Copy' is an UCSD Pascal extension (ISO 10206 has `SubStr')");
            break;
          }

        case p_EQ:
        case p_LT:
        case p_LE:
        case p_GE:
        case p_GT:
        case p_NE:
        case p_TRIM:
        case p_INDEX:  /* reserved word in Borland Pascal */
        case p_SUBSTR:
          {
            if (PEDANTIC (E_O_PASCAL))
              pedwarn ("this string function is an ISO 10206 Extended Pascal extension");
            break;
          }

        default:
          /* do nothing */
          ;
      }

    /* @@ Hmmph, identify the routines with incorrect parameters some day.
     */
    if (TREE_CODE (TREE_TYPE (parm1)) == CHAR_TYPE)
      {
        parm_types |= P_STR_FIRST_IS_CHAR;      /* First arg is CHAR_TYPE */
        len1 = integer_one_node;
        arg1 = parm1;
      }
    else if (is_string_type (parm1, 1))
      len1 = PASCAL_STRING_LENGTH (parm1);
    else
      {
        if (TREE_CODE (TREE_TYPE (parm1)) != ERROR_MARK)
          error ("first parameter to string routines must be of char or string type");
        return error_mark_node;
      }

    if (!arg1)
      arg1 = build1 (ADDR_EXPR, string_type_node, PASCAL_STRING_VALUE (parm1));

    if (rts_id != p_TRIM)
      {
        parm2 = TREE_VALUE (TREE_CHAIN (params));

        if (rts_id == p_SUBSTR)
          {
            /* `SubStr' may have 3rd parameter.  Check later.
             */
            if (TREE_CODE (TREE_TYPE (parm2)) != INTEGER_TYPE)
              {
                if (TREE_CODE (TREE_TYPE (parm2)) != ERROR_MARK)
                  error ("second parameter to `SubStr' must be of integer type");
                return error_mark_node;
              }
            sub_from = parm2;
          }
        else if (rts_id == ucsd_COPY)
          {
            /* `Copy' may have 3rd parameter.  Check later.
             */
            if (TREE_CODE (TREE_TYPE (parm2)) != INTEGER_TYPE)
              {
                if (TREE_CODE (TREE_TYPE (parm2)) != ERROR_MARK)
                  error ("second parameter to `Copy' must be of integer type");
                return error_mark_node;
              }
            sub_from = parm2;
          }
        else
          {
            if (TREE_CODE (TREE_TYPE (parm2)) == CHAR_TYPE)
              {
                parm_types |= P_STR_SECOND_IS_CHAR;     /* Second arg is CHAR_TYPE */
                len2 = integer_one_node;
                arg2 = parm2;
              }
            else if (is_string_type (parm2, 1))
              len2 = PASCAL_STRING_LENGTH (parm2);
            else
              {
                if (TREE_CODE (TREE_TYPE (parm2)) != ERROR_MARK)
                  error ("second parameter to this string routine must be of char or string type");
                return error_mark_node;
              }

            parm2 = PASCAL_STRING_VALUE (parm2);

            if (!arg2)
              arg2 = build1 (ADDR_EXPR, string_type_node, parm2);
          }
      }

    return_type = boolean_type_node;
    switch (rts_id) {
      /* Lexicographic string comparison.
       * Two args, returns Boolean.
       */
    case p_EQ: opcode = R_EQ; break;
    case p_LT: opcode = R_LT; break;
    case p_LE: opcode = R_LE; break;
    case p_GE: opcode = R_GE; break;
    case p_GT: opcode = R_GT; break;
    case p_NE: opcode = R_NE; break;

      /* If flag_exact_compare_strings is zero, comparisons padded with spaces;
       * otherwise as above.  Two args, returns Boolean.
       */
    case '=': if (flag_exact_compare_strings)
                opcode = R_EQ;
              else
                opcode = R_eq;
              break;
    case '<': if (flag_exact_compare_strings)
                opcode = R_LT;
              else
                opcode = R_lt;
              break;
    case '>': if (flag_exact_compare_strings)
                opcode = R_GT;
              else
                opcode = R_gt;
              break;
    case LEX_LE:
              if (flag_exact_compare_strings)
                opcode = R_LE;
              else
                opcode = R_le;
              break;
    case LEX_GE:
              if (flag_exact_compare_strings)
                opcode = R_GE;
              else
                opcode = R_ge;
              break;
    case LEX_NE:
              if (flag_exact_compare_strings)
                opcode = R_NE;
              else
                opcode = R_ne;
              break;

      /* pos (s2, s1), index (s1, s2) : first position where s1 contains s2,
       * 0 if none.  Two args; reserved word.
       */
    case ucsd_POS:
    case p_INDEX:
      opcode = R_INDEX;
      return_type = integer_type_node;
      if (TREE_CHAIN (TREE_CHAIN (params)))
        {
          if (rts_id == ucsd_POS)
            error ("too many arguments to `Pos'");
          else
            error ("too many arguments to `Index'");
        }
      break;

      /* substr(s,i) or substr(s,i,j) or copy (s,i) or copy (s,i,j)
       * (if j is missing, it is calculated as: length(s)-i+1)
       *
       * Return a substring of s from index i to index j.
       */
    case ucsd_COPY:  /* two or three args */
    case p_SUBSTR:   /* two or three args */
      if (rts_id == p_SUBSTR)
        opcode = R_SUBSTR;
      else
        opcode = R_COPY;
      if (TREE_CHAIN (TREE_CHAIN (params)))
        {
          sub_length = TREE_VALUE (TREE_CHAIN (TREE_CHAIN (params)));
          if (TREE_CODE (TREE_TYPE (sub_length)) != INTEGER_TYPE)
            {
              if (TREE_CODE (TREE_TYPE (sub_length)) != ERROR_MARK)
                error ("substring start position must be of integer type");
              return error_mark_node;
            }
          if (TREE_CHAIN (TREE_CHAIN (TREE_CHAIN (params))))
            {
              if (rts_id == p_SUBSTR)
                error ("too many arguments to `SubStr'");
              else
                error ("too many arguments to `Copy'");
            }
        }
      else
        {
          if (rts_id == ucsd_COPY && PEDANTIC (~U_B_D_PASCAL))
            pedwarn ("`Copy' with only two arguments is a GNU Pascal extension");
          sub_length = build_binary_op (PLUS_EXPR, integer_one_node,
                           build_binary_op (MINUS_EXPR, len1, sub_from, 1), 1);
        }
      /* FALLTHROUGH */

    case p_TRIM:
      if (rts_id == p_TRIM)
        {
          opcode = R_TRIM;
          if (TREE_CHAIN (params))
            error ("too many arguments to `Trim'");
        }
      return_type = void_type_node;
      /* Allocate a new string, pass that and the length field
       * as a pointer to run time system.
       *
       * <*string,*len> to arg2, len2.
       */
      parm2 = new_string_by_model (NULL_TREE, parm1, 0);
      arg2 = build_unary_op (ADDR_EXPR, PASCAL_STRING_VALUE (parm2), 0);
      len2 = build_unary_op (ADDR_EXPR, PASCAL_STRING_LENGTH (parm2), 0);
      break;

    default:
      abort ();
    }

    arglist = tree_cons (NULL_TREE, size_int (opcode),
               tree_cons (NULL_TREE, size_int (parm_types),
                tree_cons (NULL_TREE, arg1, NULL_TREE)));

    /* Length of first string; nothing if char */
    if ((parm_types & 1) == 0)
      arglist = chainon (arglist, build_tree_list (NULL_TREE, len1));

    /* Second <*string,length> or just <char>,
     * or <*string, *length> for p_TRIM and p_SUBSTR
     */
    arglist = chainon (arglist, build_tree_list (NULL_TREE, arg2));
    if ((parm_types & 2) == 0)
      arglist = chainon (arglist, build_tree_list (NULL_TREE, len2));

    if (rts_id == p_SUBSTR || rts_id == ucsd_COPY)
      {
        /* Substring start */
        arglist = chainon (arglist, build_tree_list (NULL_TREE, sub_from));
        /* Substring length */
        arglist = chainon (arglist, build_tree_list (NULL_TREE, sub_length));
      }

    funcall = rts_call (rts_id, return_type, NULL_TREE, arglist);

    if (return_type != void_type_node)
      return funcall;

    if (rts_id != p_TRIM && rts_id != p_SUBSTR && rts_id != ucsd_COPY)
      abort ();

    return build1 (NON_LVALUE_EXPR, TREE_TYPE (parm2),
                   build (COMPOUND_EXPR, TREE_TYPE (parm2),
                          funcall, parm2));
}

/* Implement Pascal pack and unpack transfer procedures.
 */
static tree
pascal_unpack_and_pack (unpack, unpacked, packed, ustart)
     int  unpack;                       /* Nonzero if UNPACK */
     tree unpacked;
     tree packed;
     tree ustart;
{
  tree utype;
  tree ptype;
  tree len;
  tree pmin, pmax, umin, umax;
  tree check;                   /* Check of arguments */
  tree bits;

  rtx x,y;
  rtx target;
  rtx source;
  rtx length;

  if (unpacked == error_mark_node || packed == error_mark_node)
    return error_mark_node;

  utype = TREE_TYPE (unpacked);
  ptype = TREE_TYPE (packed);
  pmin = TYPE_MIN_VALUE (TYPE_DOMAIN (ptype));
  pmax = TYPE_MAX_VALUE (TYPE_DOMAIN (ptype));
  umin = TYPE_MIN_VALUE (TYPE_DOMAIN (utype));
  umax = TYPE_MAX_VALUE (TYPE_DOMAIN (utype));

  x = expand_expr
    (build_indirect_ref
     (build_unary_op (ADDR_EXPR, build_array_ref (unpacked, ustart), 0),
      "unpacked"),
     0, BLKmode, 0);

  y = expand_expr
    (build_indirect_ref
     (build_unary_op (ADDR_EXPR, packed, 0),
      "packed"),
     0, BLKmode, 0);

  if (unpack)
    {
      target = x;
      source = y;
    }
  else
    {
      target = y;
      source = x;
    }

  /* Length we copy is the length of the packed array */
  len = fold (build (PLUS_EXPR, integer_type_node,
                     fold (build (MINUS_EXPR, integer_type_node, pmax, pmin)),
                     integer_one_node));

  /* Sanity check */
  /* Check that (ustart >= umin) and (ustart..umax >= pmin..pmax) */
  check =
    fold (build (TRUTH_AND_EXPR, boolean_type_node,
                 fold (build (GE_EXPR, boolean_type_node,
                              ustart, umin)),
                 fold (build (GE_EXPR, boolean_type_node,
                              fold (build (PLUS_EXPR, integer_type_node,
                                           fold (build (MINUS_EXPR,
                                                        integer_type_node,
                                                        umax, ustart)),
                                           integer_one_node)),
                              len))));

  if (! TREE_CONSTANT (check))
    /* @@@ Runtime check here variable size array */;
  else if (integer_zerop (check))
    {
      error ("invalid arguments to `%s'", unpack ? "Unpack" : "Pack");
      return error_mark_node;
    }

  bits = count_bits (TREE_TYPE (ptype));
  if (bits && TREE_INT_CST_LOW (bits)
              != TREE_INT_CST_LOW (TYPE_SIZE (TREE_TYPE (ptype))))
    {
      /* Construct a loop like ISO wants (abbreviated):
       *
       *   j:= low ( packed );
       *   k:= ustart;
       *   repeat
       *     if unpack then
       *       unpacked [ k ]:= packed [ j ]
       *     else
       *       packed [ j ]:= unpacked [ k ];
       *     if j < high ( packed ) then
       *       begin
       *         inc ( j );
       *         inc ( k );
       *       end (* if *);
       *   until j >= high ( packed );
       */
      tree j = make_new_variable ("pack", TREE_TYPE (TYPE_DOMAIN (ptype)));
      tree k = make_new_variable ("pack", TREE_TYPE (TYPE_DOMAIN (utype)));
      tree condition, packed_j, unpacked_k;
      tree j_as_integer = convert (type_for_size (TYPE_PRECISION (TREE_TYPE (j)),
                                                  TREE_UNSIGNED (TREE_TYPE (j))), j);
      tree k_as_integer = convert (type_for_size (TYPE_PRECISION (TREE_TYPE (k)),
                                                  TREE_UNSIGNED (TREE_TYPE (k))), k);

      expand_expr_stmt (build_modify_expr (j, NOP_EXPR,
                            TYPE_MIN_VALUE (TYPE_DOMAIN (ptype))));
      expand_expr_stmt (build_modify_expr (k, NOP_EXPR,
                            convert (TREE_TYPE (k), ustart)));
      expand_start_loop (1);
      unpacked_k = build_pascal_array_ref (unpacked,
                                           build_tree_list (NULL_TREE, k));
      packed_j = build_pascal_array_ref (packed,
                                         build_tree_list (NULL_TREE, j));
      if (unpack)
        expand_expr_stmt (build_modify_expr (unpacked_k, NOP_EXPR, packed_j));
      else
        expand_expr_stmt (build_modify_expr (packed_j, NOP_EXPR, unpacked_k));
      condition = build_binary_op (LT_EXPR, j,
                                   TYPE_MAX_VALUE (TYPE_DOMAIN (ptype)), 0);
      expand_exit_loop_if_false (0, condition);
      expand_expr_stmt (build_modify_expr (j_as_integer,
                                           PLUS_EXPR, integer_one_node));
      expand_expr_stmt (build_modify_expr (k_as_integer,
                                           PLUS_EXPR, integer_one_node));
      expand_end_loop ();
    }
  else
    {
      /* Not really packed; elements have same size.
       * Get size in bytes.
       */
      tree elem_size = size_in_bytes (TREE_TYPE (utype));

      length = expand_expr
                   (fold (build (MULT_EXPR, integer_type_node, elem_size, len)),
                    0, SImode, 0);

      if (GET_CODE (source) == MEM && GET_CODE (target) == MEM)
        {
          extern rtx change_address ();

          if (GET_MODE (target) != BLKmode)
            target = change_address (target, BLKmode, 0);

          if (GET_MODE (source) != BLKmode)
            source = change_address (source, BLKmode, 0);

          emit_block_move
            (target, source, length, TYPE_ALIGN (ptype) / BITS_PER_UNIT);
        }
      else
        abort ();
    }

  return NULL_TREE;
}

/* This routine constructs Pascal run time system calls
 * with correct arguments.
 *
 * RTS_ID is the %token number of the run time system routine to call.
 * APAR is a TREE_LIST chain of arguments; args are in the TREE_VALUE field.
 * If there is something in the TREE_PURPOSE field, it is a TREE_LIST
 * node of write | writeln output field length expressions, the first
 * expression is in TREE_VALUE and the second one is in TREE_PURPOSE
 * i.e. actual_parameter : expression : expression.
 *
 */
tree
build_rts_call (rts_id, apar)
     int rts_id;
     register tree apar;   /* actual parameters of the routine */
{
  tree fpar = NULL_TREE;       /* formal parameters, default to varargs */
  tree rval = void_type_node;  /* return value; void_type_node if !function */
  tree actual_return_value = NULL_TREE; /* Value to return for a procedure call if any */

  tree un_init_this = NULL_TREE; /* If nonzero, try to un-initalize the beast */

  tree post_conversion = NULL_TREE; /* for integer `a pow b': should be inlined */
  tree val  = NULL_TREE;
  tree val2 = NULL_TREE;
  enum tree_code code  = MINUS_EXPR; /* Something that is impossible */
  enum tree_code code2 = MINUS_EXPR; /* Something that is impossible */
  tree type = NULL_TREE, type2 = NULL_TREE;
  int length;                  /* length of the actual parameter list */
  int schema_ids   = 0;
  int rts_inline   = 0;        /* set to 1 if this is compiled inline */
  int wins         = 1;        /* Will be zeroed if formal params are not
                                  constants */
  tree temp;

  char *errstr = (char *)NULL;
  int ptrarith_inform_flag = 0;
  tree retval   = NULL_TREE;

  length = list_length (apar);

#define INLINE_RTS_LENGTH(nam, len) \
 { if (length != len) {                                         \
    error ("incorrect number of parameters to `%s'", nam);      \
    return error_mark_node; }                                   \
   else rts_inline = 1; }

  if (apar)
    {
      val  = TREE_VALUE (apar);
      switch (rts_id)
        {
          case p_ORD: case p_PRED: case p_SUCC:
          case gpc_MIN: case gpc_MAX:
            val = string_may_be_char (val);
            break;
        }
      type = TREE_TYPE (val);
      if (type)
        {
          code = TREE_CODE (type);
          if (code == FUNCTION_TYPE)
            {
              /* This is a function without parameters.
               * Call it.
               */
              val = probably_call_function (val);
              type = TREE_TYPE (val);
              code = TREE_CODE (type);
            }
        }
      if (TREE_CHAIN (apar))
        {
          val2 = TREE_VALUE (TREE_CHAIN (apar));
          switch (rts_id)
            {
              case gpc_MIN: case gpc_MAX: case p_PACK:
              case bp_INCLUDE: case bp_EXCLUDE:
                val2 = string_may_be_char (val2);
                break;
            }
          type2 = TREE_TYPE (val2);
          if (type2)
            {
              code2 = TREE_CODE (type2);
              if (code2 == FUNCTION_TYPE)
                {
                  /* This is a function without parameters.
                   * Call it.
                   */
                  val2 = probably_call_function (val2);
                  type2 = TREE_TYPE (val2);
                  code2 = TREE_CODE (type2);
                }
            }
        }
      /* @@@ wins is not used */
      for (temp = apar; temp && wins; temp = TREE_CHAIN (temp))
          wins = TREE_CONSTANT (TREE_VALUE (temp));
    }

  switch (rts_id) {

  case p_CARD:
    {
      INLINE_RTS_LENGTH ("card", 1);
      if (code == SET_TYPE)
        {
          if (TREE_CODE (val) != CONSTRUCTOR
              && TREE_CODE (TREE_TYPE (type)) == VOID_TYPE)
            retval = integer_zero_node;
          else
            {
              tree params;
              params = build_tree_list (NULL_TREE, build_pascal_unary_op (ADDR_EXPR, val, 0));
              params = chainon (params,
                                build_tree_list (NULL_TREE,
                                                 TYPE_MIN_VALUE (TYPE_DOMAIN (TREE_TYPE (val)))));
              params = chainon (params,
                                build_tree_list (NULL_TREE,
                                                 TYPE_MAX_VALUE (TYPE_DOMAIN (TREE_TYPE (val)))));
              retval = rts_call (set_card, integer_type_node, NULL_TREE, params);
            }
        }
      else
        errstr = "set type required for `Card'";
      break;
    }

  /* rts_inline functions */
  case p_ABS:
    {
      if (length != 1)
        errstr = "incorrect number of parameters to `abs'";
      if (INT_REAL (code))
        {
          rts_inline = 1;
          retval = build_unary_op (ABS_EXPR, val, 0);
        }
      else if (code == COMPLEX_TYPE)
        {
          rts_id = z_ABS;
          rval = double_type_node;
          fpar = ptype_complex;
        }
      else
        errstr = "argument to `Abs' must be of integer, real or complex type";
      break;
    }

  case p_SQR:
    {
      INLINE_RTS_LENGTH ("sqr", 1);
      if (INT_REAL (code) || code == COMPLEX_TYPE)
        retval = build_pascal_binary_op (MULT_EXPR, val, val);
      else
        errstr = "argument to `Sqr' must be of integer, real or complex type";
      break;
    }

  case p_TRUNC:
    {
      /* Check the return value; maybe wrong TRUNC_EXPR used.
       */
      INLINE_RTS_LENGTH ("trunc", 1);
      if (code == REAL_TYPE)
        retval = convert (long_long_integer_type_node, val);
      else
        errstr = "argument to `Trunc' must be of real type";
      break;
    }

  case p_ROUND:
    {
        /* ISO Pascal: round := trunc ( x >= 0.0 ? x + 0.5 : x - 0.5 );
         */
      INLINE_RTS_LENGTH ("round", 1);
      if (code != REAL_TYPE)
        errstr = "argument to `Round' must be of real type";
      else
        {
          val = save_expr (val);
          val = build (COND_EXPR, TREE_TYPE (val),
                       build_pascal_binary_op (GE_EXPR, val, real_zero_node),
                       build_pascal_binary_op (PLUS_EXPR, val, real_half_node),
                       build_pascal_binary_op (MINUS_EXPR, val, real_half_node));
          retval = convert (long_long_integer_type_node, val);
        }
      break;
    }

  case p_SUCC:
    {
      /* rts_inline */
      if (length != 1 && length != 2)
        {
          error ("incorrect number of parameters to `Succ'");
          return error_mark_node;
        }
      if (length == 2 && PEDANTIC (E_O_PASCAL))
        pedwarn ("second argument to `Succ' is an ISO 10206 Extended Pascal extension");

      rts_inline = 1;

      if (ORDINAL_TYPE (code)
          || (flag_extended_syntax && code == POINTER_TYPE))
        {
          tree increment = integer_one_node;
          if (length == 2)
            {
              if (TREE_CODE (type2) != INTEGER_TYPE)
                errstr = "second arg to `Succ' must be of integer type";
              else
                increment = val2;
            }
#if 0
          if (code == POINTER_TYPE && TREE_TYPE (type) != void_type_node)
            increment = build_pascal_binary_op (MULT_EXPR,
                                                c_sizeof (TREE_TYPE (type)),
                                                increment);
#endif
          if (! errstr)
            retval = convert (type,
                              build_pascal_binary_op (PLUS_EXPR, val, increment));
        }
      else if (code == POINTER_TYPE && ! flag_extended_syntax)
        {
          errstr = "argument to `Succ' must be of ordinal type";
          ptrarith_inform_flag++;
        }
      else if (flag_extended_syntax)
        errstr = "argument to `Succ' must be of ordinal or pointer type";
      else
        errstr = "argument to `Succ' must be of ordinal type";
      break;
    }

  case p_PRED:
    {
      /* rts_inline */
      if (length != 1 && length != 2)
        {
          error ("incorrect number of parameters to `Pred'");
          return error_mark_node;
        }
      if (length == 2 && PEDANTIC (E_O_PASCAL))
        pedwarn ("second argument to `Pred' is an ISO 10206 Extended Pascal extension");

      rts_inline = 1;

      if (ORDINAL_TYPE (code)
          || (flag_extended_syntax && code == POINTER_TYPE))
        {
          tree increment = integer_one_node;
          if (length == 2)
            {
              if (TREE_CODE (type2) != INTEGER_TYPE)
                errstr = "second arg to `Pred' must be of integer type";
              else
                increment = val2;
            }
#if 0
          if (code == POINTER_TYPE && TREE_TYPE (type) != void_type_node)
            increment = build_pascal_binary_op (MULT_EXPR,
                                                c_sizeof (TREE_TYPE (type)),
                                                increment);
#endif
          if (! errstr)
            retval = convert (type,
                              build_pascal_binary_op (MINUS_EXPR, val, increment));
        }
      else if (code == POINTER_TYPE && ! flag_extended_syntax)
        {
          errstr = "argument to `Pred' must be of ordinal type";
          ptrarith_inform_flag++;
        }
      else if (flag_extended_syntax)
        errstr = "argument to `Pred' must be of ordinal or pointer type";
      else
        errstr = "argument to `Pred' must be of ordinal type";
      break;
    }

  case bp_INC:
    {
      /* rts_inline */
      if (PEDANTIC (B_D_PASCAL))
        pedwarn ("`Inc' is a Borland Pascal extension");
      if (length != 1 && length != 2)
        {
          error ("incorrect number of parameters to `Inc'");
          return error_mark_node;
        }
      rts_inline = 1;
      retval = NULL_TREE;
      typed_const_warning (val);
      if (ORDINAL_TYPE (code)
          || (flag_extended_syntax && code == POINTER_TYPE))
        {
          tree increment = integer_one_node;
          if (length == 2)
            {
              if (TREE_CODE (type2) != INTEGER_TYPE)
                errstr = "second arg to `Inc' must be of integer type";
              else
                increment = val2;
            }
          if (! errstr)
            {
              if (code == POINTER_TYPE || code == INTEGER_TYPE)
                expand_expr_stmt (build_modify_expr (val, PLUS_EXPR, increment));
              else
                {
                  tree val_as_integer
                    = convert (type_for_size (TYPE_PRECISION (TREE_TYPE (val)),
                                              TREE_UNSIGNED (TREE_TYPE (val))), val);
                  expand_expr_stmt (build_modify_expr (val_as_integer,
                                                       PLUS_EXPR, increment));
                }
            }
        }
      else if (code == POINTER_TYPE && ! flag_extended_syntax)
        {
          errstr = "argument to `Inc' must be of ordinal type";
          ptrarith_inform_flag++;
        }
      else if (flag_extended_syntax)
        errstr = "argument to `Inc' must be of ordinal or pointer type";
      else
        errstr = "argument to `Inc' must be of ordinal type";
      break;
    }

  case bp_DEC:
    {
      /* rts_inline */
      if (PEDANTIC (B_D_PASCAL))
        pedwarn ("`Dec' is a Borland Pascal extension");
      if (length != 1 && length != 2)
        {
          error ("incorrect number of parameters to `Dec'");
          return error_mark_node;
        }
      rts_inline = 1;
      retval = NULL_TREE;
      typed_const_warning (val);
      if (ORDINAL_TYPE (code)
          || (flag_extended_syntax && code == POINTER_TYPE))
        {
          tree increment = integer_one_node;
          if (length == 2)
            {
              if (TREE_CODE (type2) != INTEGER_TYPE)
                errstr = "second arg to `Dec' must be of integer type";
              else
                increment = val2;
            }
          if (! errstr)
            {
              if (code == POINTER_TYPE || code == INTEGER_TYPE)
                expand_expr_stmt (build_modify_expr (val, MINUS_EXPR, increment));
              else
                {
                  tree val_as_integer
                    = convert (type_for_size (TYPE_PRECISION (TREE_TYPE (val)),
                                              TREE_UNSIGNED (TREE_TYPE (val))), val);
                  expand_expr_stmt (build_modify_expr (val_as_integer,
                                                       MINUS_EXPR, increment));
                }
            }
        }
      else if (code == POINTER_TYPE && ! flag_extended_syntax)
        {
          errstr = "argument to `Dec' must be of ordinal type";
          ptrarith_inform_flag++;
        }
      else if (flag_extended_syntax)
        errstr = "argument to `Dec' must be of ordinal or pointer type";
      else
        errstr = "argument to `Dec' must be of ordinal type";
      break;
    }

  case ucsd_FILLCHAR:
    {
      tree val3 = NULL_TREE;
      enum tree_code code3;

      if (PEDANTIC (U_B_D_PASCAL))
        pedwarn ("`FillChar' is an UCSD Pascal extension");
      if (length != 3)
        errstr = "incorrect number of parameters to `FillChar'";
      else
        {
          val3 = string_may_be_char (TREE_VALUE (TREE_CHAIN (TREE_CHAIN (apar))));
          code3 = TREE_CODE (TREE_TYPE (val3));
          if (code2 != INTEGER_TYPE)
            errstr = "second parameter to `FillChar' must be of integer type";
          else if (code3 != CHAR_TYPE && code3 != INTEGER_TYPE)
            errstr = "third parameter to `FillChar' must be of char or integer type";
          else if (code3 != CHAR_TYPE && PEDANTIC (B_D_PASCAL))
            {
              pedwarn ("integer values for third parameter to `FillChar' are");
              pedwarn (" a Borland Pascal extension");
            }
        }
      if (! errstr)
        {
          TREE_VALUE (TREE_CHAIN (TREE_CHAIN (apar))) = val2;
          TREE_VALUE (TREE_CHAIN (apar)) =
              convert_and_check (unsigned_char_type_node, val3);
          TREE_VALUE (apar) = build_unary_op (ADDR_EXPR, val, 0);
        }
      break;
    }

  case bp_MOVE:
    {
      tree val3 = TREE_VALUE (TREE_CHAIN (TREE_CHAIN (apar)));
      enum tree_code code3 = TREE_CODE (TREE_TYPE (val3));

      if (rts_id == bp_MOVE && PEDANTIC (B_D_PASCAL))
        pedwarn ("`Move' is a Borland Pascal extension");
      if (length != 3)
        errstr = "incorrect number of parameters to `Move'";
      else if (code3 != INTEGER_TYPE)
        errstr = "third parameter to `Move' must be of integer type";
      if (! errstr)
        {
          TREE_VALUE (apar) = build_unary_op (ADDR_EXPR, val, 0);
          TREE_VALUE (TREE_CHAIN (apar)) = build_unary_op (ADDR_EXPR, val2, 0);
        }
      break;
    }

  case ucsd_MOVELEFT:
  case ucsd_MOVERIGHT:
    {
      tree val3;
      enum tree_code code3;

      if (PEDANTIC (UCSD_PASCAL))
        pedwarn ("`MoveLeft' and `MoveRight' are UCSD Pascal extensions");
      if (length != 3)
        errstr = "incorrect number of parameters to `MoveLeft' or `MoveRight'";
      else
        {
          val3 = TREE_VALUE (TREE_CHAIN (TREE_CHAIN (apar)));
          code3 = TREE_CODE (TREE_TYPE (val3));
          if (code3 != INTEGER_TYPE)
            errstr = "third parameter to `MoveLeft'/`MoveRight' must be of integer type";
        }
      if (! errstr)
        {
          TREE_VALUE (apar) = build_unary_op (ADDR_EXPR, val, 0);
          TREE_VALUE (TREE_CHAIN (apar)) = build_unary_op (ADDR_EXPR, val2, 0);
        }
      break;
    }

  case ucsd_BLOCKREAD:
  case ucsd_BLOCKWRITE:
    {
      tree val3 = NULL_TREE, val4 = NULL_TREE;
      enum tree_code code3, code4;

      if (PEDANTIC (U_B_D_PASCAL))
        pedwarn ("`BlockRead' and `BlockWrite' are UCSD Pascal extensions");
      if (length != 3 && length != 4)
        errstr = "incorrect number of parameters to `BlockRead' or `BlockWrite'";
      else
        {
          val3 = TREE_VALUE (TREE_CHAIN (TREE_CHAIN (apar)));
          code3 = TREE_CODE (TREE_TYPE (val3));
          if (code != FILE_TYPE || TREE_CODE (TREE_TYPE (type)) != VOID_TYPE)
            errstr = "first parameter to `BlockRead'/`BlockWrite' must be an untyped file";
          if (code3 != INTEGER_TYPE)
            errstr = "third parameter to `BlockRead'/`BlockWrite' must be of integer type";
          if (length == 4)
            {
              val4 = TREE_VALUE (TREE_CHAIN (TREE_CHAIN (TREE_CHAIN (apar))));
              code4 = TREE_CODE (TREE_TYPE (val4));
              if (TYPE_MAIN_VARIANT (TREE_TYPE (val4)) != integer_type_node
                  && TYPE_MAIN_VARIANT (TREE_TYPE (val4)) != unsigned_type_node)
                errstr = "fourth parameter to `BlockRead'/`BlockWrite' must be of integer type";
            }
          else
            val4 = NULL_TREE;
        }
      if (! errstr)
        {
          TREE_VALUE (apar) = build_unary_op (ADDR_EXPR, val, 0);
          TREE_VALUE (TREE_CHAIN (apar)) = build_unary_op (ADDR_EXPR, val2, 0);
          if (val4)
            TREE_VALUE (TREE_CHAIN (TREE_CHAIN (TREE_CHAIN (apar)))) =
                build_unary_op (ADDR_EXPR, val4, 0);
          else
            apar = chainon (apar, build_tree_list (NULL_TREE,
                                                   null_pointer_node));
        }
      break;
      break;
    }

  case ucsd_CONCAT:
    {
      tree arg;
      if (length < 2)
        {
          error ("`Concat' must get at least two args");
          /* Avoid further error messages */
          retval = build_string (1, "");
          TREE_TYPE (retval) = char_array_type_node;
          return new_string_by_model (NULL_TREE, retval, 1);
        }
      for (arg = apar; arg; arg = TREE_CHAIN (arg))
        if (TREE_CODE (TREE_TYPE (TREE_VALUE (arg))) != CHAR_TYPE
            && ! is_string_type (TREE_VALUE (arg), 1))
          {
            error ("arguments to `Concat' must be of string type");
            /* Avoid further error messages */
            retval = build_string (1, "");
            TREE_TYPE (retval) = char_array_type_node;
            return new_string_by_model (NULL_TREE, retval, 1);
          }
      rts_inline = 1;
      retval = val;
      for (arg = TREE_CHAIN (apar); arg; arg = TREE_CHAIN (arg))
        retval = build_pascal_binary_op (PLUS_EXPR, retval, TREE_VALUE (arg));
      break;
    }

  case LEX_AND:
    {
      /* rts_inline */
      if (pedantic || flag_what_pascal)
        pedwarn ("procedure-like use of `And' is a GNU Pascal extension");
      if (length != 2)
        {
          error ("incorrect number of parameters to `And'");
          return error_mark_node;
        }
      rts_inline = 1;
      retval = NULL_TREE;
      typed_const_warning (val);
      if (TREE_CODE (type) == INTEGER_TYPE
          && TREE_CODE (type2) == INTEGER_TYPE)
        {
          if (! errstr)
            expand_expr_stmt (build_modify_expr (val, BIT_AND_EXPR, val2));
        }
      else
        errstr = "arguments to `And' must be of integer type";
      break;
    }

  case LEX_OR:
    {
      /* rts_inline */
      if (pedantic || flag_what_pascal)
        pedwarn ("procedure-like use of `Or' is a GNU Pascal extension");
      if (length != 2)
        {
          error ("incorrect number of parameters to `Or'");
          return error_mark_node;
        }
      rts_inline = 1;
      retval = NULL_TREE;
      typed_const_warning (val);
      if (TREE_CODE (type) == INTEGER_TYPE
          && TREE_CODE (type2) == INTEGER_TYPE)
        {
          if (! errstr)
            expand_expr_stmt (build_modify_expr (val, BIT_IOR_EXPR, val2));
        }
      else
        errstr = "arguments to `Or' must be of integer type";
      break;
    }

  case LEX_NOT:
    {
      /* rts_inline */
      if (pedantic || flag_what_pascal)
        pedwarn ("procedure-like use of `Not' is a GNU Pascal extension");
      if (length != 1)
        {
          error ("incorrect number of parameters to `Not'");
          return error_mark_node;
        }
      rts_inline = 1;
      retval = NULL_TREE;
      typed_const_warning (val);
      if (TREE_CODE (type) == INTEGER_TYPE)
        {
          if (! errstr)
            expand_expr_stmt (build_modify_expr (val, NOP_EXPR,
              build_pascal_unary_op (BIT_NOT_EXPR, val, 0)));
        }
      else
        errstr = "argument to `Not' must be of integer type";
      break;
    }

  case LEX_XOR:
    {
      /* rts_inline */
      if (pedantic || flag_what_pascal)
        pedwarn ("procedure-like use of `Xor' is a GNU Pascal extension");
      if (length != 2)
        {
          error ("incorrect number of parameters to `Xor'");
          return error_mark_node;
        }
      rts_inline = 1;
      retval = NULL_TREE;
      typed_const_warning (val);
      if (TREE_CODE (type) == INTEGER_TYPE
          && TREE_CODE (type2) == INTEGER_TYPE)
        {
          if (! errstr)
            expand_expr_stmt (build_modify_expr (val, BIT_XOR_EXPR, val2));
        }
      else
        errstr = "arguments to `Xor' must be of integer type";
      break;
    }

  case p_ORD:
    {
      /* rts_inline */
      INLINE_RTS_LENGTH ("ord", 1);
      if (ORDINAL_TYPE (code))
        retval = convert (integer_type_node, val);
      else
        errstr = "argument to `Ord' must be of ordinal type";
      break;
    }

  case p_CHR:
    {
      /* rts_inline */
      INLINE_RTS_LENGTH ("chr", 1);
      if (code == INTEGER_TYPE)
        retval = convert (char_type_node, val);
      else
        errstr = "argument to `Chr' must be of integer type";
      break;
    }

  case gpc_UPCASE:
    {
      if (PEDANTIC (B_D_PASCAL))
        pedwarn ("`UpCase' is a Borland Pascal extension");
      if (flag_what_pascal & B_D_PASCAL)
        rts_id = bp_UPCASE;
      if (length != 1)
        errstr = "incorrect number of parameters to `UpCase'";
      else if (code != CHAR_TYPE)
        errstr = "argument to `UpCase' must be of char type";
      rval = char_type_node;
      break;
    }

  case gpc_LOCASE:
    {
      if (pedantic || flag_what_pascal)
        pedwarn ("`LoCase' is a GNU Pascal extension");
      if (flag_what_pascal & B_D_PASCAL)
        rts_id = bp_LOCASE;
      if (length != 1)
        errstr = "incorrect number of parameters to `LoCase'";
      else if (code != CHAR_TYPE)
        errstr = "argument to `LoCase' must be of char type";
      rval = char_type_node;
      break;
    }

  case gpc_SETFILETIME:
    {
      /* This code assumes that `UnixTimeType' is `LongInt'.
       */
      tree type3 = apar;
      if (type3) type3 = TREE_CHAIN (type3);
      if (type3) type3 = TREE_CHAIN (type3);
      if (type3) type3 = TREE_VALUE (type3);
      if (type3) type3 = TREE_TYPE (type3);
      if (pedantic || flag_what_pascal)
        pedwarn ("`SetFileTime' is a GNU Pascal extension");
      if (length != 3)
        errstr = "incorrect number of parameters to `SetFileTime'";
      else if (code != FILE_TYPE)
        errstr = "file type required for first parameter of `SetFileTime'";
      else if (code2 != INTEGER_TYPE)
        errstr = "`UnixTimeType' required for second parameter of `SetFileTime'";
      else if (!type3 || TREE_CODE (type3) != INTEGER_TYPE)
        errstr = "`UnixTimeType' required for third parameter of `SetFileTime'";
      else
        fpar = chainon (chainon (do_ptype (type, 1, 0),
                 do_ptype (long_long_integer_type_node, 0, 0)), ptype_longint);
      break;
    }

  case bp_INCLUDE:
    {
      if (length != 2)
        errstr = "incorrect number of parameters to `Include'";
      else if (code != SET_TYPE)
        errstr = "set type required for first parameter of `Include'";
      else if (comptypes (TREE_TYPE (type), type2) == 0)
        errstr = "incompatible type for second parameter of `Include'";
      else
        {
          tree params;
          params = build_tree_list (NULL_TREE, build_pascal_unary_op (ADDR_EXPR, val, 0));
          params = chainon (params,
                            build_tree_list (NULL_TREE,
                                             TYPE_MIN_VALUE (TYPE_DOMAIN (TREE_TYPE (val)))));
          params = chainon (params,
                            build_tree_list (NULL_TREE,
                                             TYPE_MAX_VALUE (TYPE_DOMAIN (TREE_TYPE (val)))));
          params = chainon (params, build_tree_list (NULL_TREE, val2));
          apar = params;
          fpar = do_ptype (type, 2, 0);
          fpar = chainon (fpar, do_ptype (TREE_TYPE (type), 0, 0));
          fpar = chainon (fpar, do_ptype (TREE_TYPE (type), 0, 0));
          fpar = chainon (fpar, do_ptype (TREE_TYPE (type), 0, 1));
        }
      break;
    }

  case bp_EXCLUDE:
    {
      if (length != 2)
        errstr = "incorrect number of parameters to `Exclude'";
      else if (code != SET_TYPE)
        errstr = "set type required for first parameter of `Exclude'";
      else if (comptypes (TREE_TYPE (type), type2) == 0)
        errstr = "incompatible type for second parameter of `Exclude'";
      else
        {
          tree params;
          params = build_tree_list (NULL_TREE, build_pascal_unary_op (ADDR_EXPR, val, 0));
          params = chainon (params,
                            build_tree_list (NULL_TREE,
                                             TYPE_MIN_VALUE (TYPE_DOMAIN (TREE_TYPE (val)))));
          params = chainon (params,
                            build_tree_list (NULL_TREE,
                                             TYPE_MAX_VALUE (TYPE_DOMAIN (TREE_TYPE (val)))));
          params = chainon (params, build_tree_list (NULL_TREE, val2));
          apar = params;
          fpar = do_ptype (type, 2, 0);
          fpar = chainon (fpar, do_ptype (TREE_TYPE (type), 0, 0));
          fpar = chainon (fpar, do_ptype (TREE_TYPE (type), 0, 0));
          fpar = chainon (fpar, do_ptype (TREE_TYPE (type), 0, 1));
        }
      break;
    }

  case bp_RANDOM:
    {
      if (PEDANTIC (U_B_D_PASCAL))
        pedwarn ("`Random' is an UCSD Pascal extension");
      if (length != 0 && length != 1)
        errstr = "incorrect number of parameters to `Random'";
      else if (length == 1 && code != INTEGER_TYPE)
        errstr = "argument to `Random' must be of integer type";
      else if (length == 0)
        {
          rts_id = bp_RANDREAL;
          rval = long_double_type_node;
        }
      else
        {
          fpar = do_ptype (long_long_unsigned_type_node, 0, 1);
          rval = long_long_unsigned_type_node;
        }
      break;
    }

  case bp_RANDOMIZE:
    {
      if (PEDANTIC (U_B_D_PASCAL))
        pedwarn ("`Randomize' is an UCSD Pascal extension");
      if (length != 0)
        errstr = "incorrect number of parameters to `Randomize'";
      break;
    }

  case p_ODD:
    {
      /* rts_inline */
      INLINE_RTS_LENGTH ("odd", 1);
      if (code == INTEGER_TYPE)
        retval = convert (boolean_type_node,
                          build_binary_op (BIT_AND_EXPR, val, integer_one_node, 1));
      else
        errstr = "argument to `Odd' must be of integer type";
      break;
    }

  case gpc_SETTYPE:
    {
      /* rts_inline */
      INLINE_RTS_LENGTH ("SetType", 2);
      rts_inline = 1;
      if (! flag_extended_syntax)
        warning ("please specify `{$X+}' when using `SetType'");
      if (PASCAL_TYPE_OBJECT (type))
        {
          tree vmt = build_component_ref (val, get_identifier ("vmt"));
          if (code2 == POINTER_TYPE)
            expand_expr_stmt (build_modify_expr (vmt, NOP_EXPR, val2));
          else
            errstr = "second argument to `SetType' must be of pointer type";
        }
      else if (code2 == POINTER_TYPE)
        errstr = "first argument to `SetType' must be of object type";
      else
        {
          error ("first argument to `SetType' must be of object type");
          errstr = "second argument to `SetType' must be of pointer type";
        }
      break;
    }

  case gpc_SETLENGTH:
    {
      /* rts_inline */
      INLINE_RTS_LENGTH ("SetLength", 2);
      rts_inline = 1;
      if (! flag_extended_syntax)
        warning ("please specify `{$X+}' when using `SetLength'");
      if (PASCAL_TYPE_STRING (type))
        {
          tree length_field = build_component_ref (val, get_identifier ("length"));
          if (code2 == INTEGER_TYPE)
            expand_expr_stmt (build_modify_expr (length_field, NOP_EXPR, val2));
          else
            errstr = "second argument to `SetLength' must be of integer type";
        }
      else if (code2 == INTEGER_TYPE)
        errstr = "first argument to `SetLength' must be a string schema";
      else
        {
          error ("first argument to `SetLength' must be a string schema");
          errstr = "second argument to `SetLength' must be of pointer type";
        }
      break;
    }

  case p_LENGTH:
    {
      /* rts_inline */
      INLINE_RTS_LENGTH ("length", 1);
      if (code == CHAR_TYPE || is_string_type (val, 1))
        retval = build1 (NON_LVALUE_EXPR, integer_type_node,
                         convert (integer_type_node, PASCAL_STRING_LENGTH (val)));
      else
        errstr = "argument to `Length' must be of string or char type";
      break;
    }

  case gpc_NEWCSTRING:
    {
      if (pedantic || flag_what_pascal)
        pedwarn ("`NewCString' is a GNU Pascal extension");
      if (length != 1)
        errstr = "incorrect number of parameters to `NewCString'";
      else if (! is_string_type (val, 1))
        errstr = "string type required for parameter of `NewCString'";
      else
        fpar = ptype_const_string_schema;
      rval = cstring_type_node;
      break;
    }

  case gpc_CSTRINGCOPYSTRING:
    {
      if (pedantic || flag_what_pascal)
        pedwarn ("built-in `CStringCopyString' is a GNU Pascal extension");
      if (length != 2)
        errstr = "incorrect number of parameters to `CStringCopyString'";
      else
        fpar = chainon (do_ptype (cstring_type_node, 0, 0),
                        ptype_const_string_schema);
      rval = cstring_type_node;
      break;
    }

  case gpc_CSTRING2STRING:
    {
      /* rts_inline */
      tree new_val, capacity, nil_condition, stmt;
      tree empty_string = build_string (1, "");

      TREE_TYPE (empty_string) = char_array_type_node;

      if (pedantic || flag_what_pascal)
        pedwarn ("`CString2String' is a GNU Pascal extension");
      INLINE_RTS_LENGTH ("CString2String", 1);
      if (TREE_CODE (type) == ARRAY_TYPE
          && TREE_CODE (TREE_TYPE (type)) == CHAR_TYPE)
        {
          TREE_VALUE (apar) = val = build1 (ADDR_EXPR, cstring_type_node, val);
          type = cstring_type_node;
        }
      if (TREE_CODE (type) == POINTER_TYPE && integer_zerop (val))
        {
          /* Explicit `Nil'.
           */
          return new_string_by_model (NULL_TREE, empty_string, 1);
        }
      if (TYPE_MAIN_VARIANT (type) != cstring_type_node)
        {
          retval = error_mark_node;
          errstr = "parameter to `CString2String' must be of `CString' (`PChar') type";
          break;
        }

      /* Var
       *   new_val: CString;
       *   nil_condition: Boolean;
       *   capacity: Integer;
       */
      new_val = make_new_variable ("cstring2string_new_val", cstring_type_node);
      TREE_USED (new_val) = 1;
      nil_condition = make_new_variable ("cstring2string_nil_condition", boolean_type_node);
      TREE_USED (nil_condition) = 1;
      capacity = make_new_variable ("cstring2string_capacity", integer_type_node);
      TREE_USED (capacity) = 1;

      /* new_val := val;
       */
      stmt = build_modify_expr (new_val, NOP_EXPR, val);

      /* nil_condition := new_val = Nil;
       */
      stmt = build (COMPOUND_EXPR, void_type_node, stmt,
               build_modify_expr (nil_condition, NOP_EXPR,
                 build_pascal_binary_op (EQ_EXPR, null_pointer_node, new_val)));

      /* capacity := nil_condition ? 0 : __builtin_strlen (new_val);
       */
      stmt = build (COMPOUND_EXPR, void_type_node, stmt,
               build_modify_expr (capacity, NOP_EXPR,
                 build (COND_EXPR, integer_type_node, nil_condition, integer_zero_node,
                        build_function_call (lookup_name (get_identifier ("__builtin_strlen")),
                                             build_tree_list (NULL_TREE, new_val)))));
      capacity = build (COMPOUND_EXPR, integer_type_node, stmt, capacity);

      /* alloca() the return value.
       */
      retval = alloca_string (capacity);

      /* _p_copycstring (new_val, retval);
       */
      fpar = chainon (do_ptype (cstring_type_node, 0, 0), ptype_string_schema);
      apar = chainon (apar, build_tree_list (NULL_TREE, retval));
      TREE_VALUE (apar) = new_val;
      stmt = rts_call (rts_id, void_type_node, fpar, apar);

      /* return (retval^);
       */
      retval = build (COMPOUND_EXPR, TREE_TYPE (retval),
                      build (SAVE_EXPR, void_type_node, stmt,
                             current_function_decl, NULL_RTX),
                      retval);
      retval = build1 (NON_LVALUE_EXPR, TREE_TYPE (retval), retval);
      break;
    }

  case gpc_STRING2CSTRING:
    {
      /* rts_inline */
      tree capacity = make_new_variable ("string2cstring_capacity",
                                         integer_type_node);
      if (pedantic || flag_what_pascal)
        pedwarn ("`String2CString' is a GNU Pascal extension");
      INLINE_RTS_LENGTH ("String2CString", 1);
      if (! is_string_type (val, 1))
        {
          retval = error_mark_node;
          errstr = "parameter to `String2CString' must be of string type";
          break;
        }
      expand_expr_stmt (build_modify_expr (capacity, NOP_EXPR,
                                           PASCAL_STRING_LENGTH (val)));
      rval = build_array_type (char_type_node,
                               build_range_type (integer_type_node,
                                                 integer_zero_node, capacity));
      retval = make_new_variable ("string2cstring_result", rval);
      fpar = chainon (do_ptype (cstring_type_node, 0, 0), ptype_const_string_schema);
      apar = chainon (build_tree_list (NULL_TREE, retval), apar);
      expand_expr_stmt (rts_call (gpc_CSTRINGCOPYSTRING, void_type_node, fpar, apar));
      retval = build1 (ADDR_EXPR, cstring_type_node, retval);
      break;
    }

  case p_CMPLX:
    {
      /* rts_inline */
      INLINE_RTS_LENGTH ("cmplx", 2);
      if (! INT_REAL (code) || ! INT_REAL (TREE_CODE (TREE_TYPE (val2))))
        errstr = "wrong type arguments to `Cmplx'";
      else
        {
          tree complex = TREE_TYPE (complex_type_node);

          if (type != complex)
            val = convert (complex, val);

          if (TREE_TYPE (val2) != complex)
            val2 = convert (complex, val2);

          retval = build (COMPLEX_EXPR, complex_type_node, val, val2);
        }
      break;
    }

  case p_IM:
    {
      INLINE_RTS_LENGTH ("im", 1);
      if (code == COMPLEX_TYPE)
          retval = build_unary_op (IMAGPART_EXPR, val, 1);
      else
          errstr = "argument to `Im' must be of complex type";
      break;
    }

  case p_RE:
    {
      INLINE_RTS_LENGTH ("re", 1);
      if (code == COMPLEX_TYPE)
          retval = build_unary_op (REALPART_EXPR, val, 1);
      else
          errstr = "argument to `Re' must be of complex type";
      break;
    }

  case gpc_MAX:
  case gpc_MIN:
    {
      INLINE_RTS_LENGTH (rts_id == gpc_MAX ? "max" : "min", 2);
      if (code == REAL_TYPE || code2 == REAL_TYPE)
        {
          if (code == INTEGER_TYPE && code2 == REAL_TYPE)
            {
              val = convert (type2, val);
              type = type2;
            }
          else if (code == REAL_TYPE && code2 == INTEGER_TYPE)
            val2 = convert (type, val2);
        }
      else if (! ORDINAL_TYPE (code) || ! ORDINAL_TYPE (code2))
        {
          errstr = "arguments to `Max' or `Min' must be of ordinal or real type";
          break;
        }
      else if (code != code2)
        {
          errstr = "both arguments to `Max' or `Min' must have the same type";
          break;
        }
      retval = convert (type,
                 build_pascal_binary_op (rts_id == gpc_MAX ? MAX_EXPR : MIN_EXPR,
                                         val, val2));
      break;
    }

  case p_PACK:
    {
      tree val3, type3, unpacked_domain;

      emit_line_note (input_filename, lineno);
      INLINE_RTS_LENGTH ("pack", 3);

      val3 = TREE_VALUE (TREE_CHAIN (TREE_CHAIN (apar)));
      type3 = TREE_TYPE (val3);
      unpacked_domain = TYPE_DOMAIN (type);

      if (TREE_CODE (type3) != ARRAY_TYPE || !PASCAL_TYPE_PACKED (type3))
        error ("third argument to `Pack' must be a packed array");
      else if (TREE_CODE (type) != ARRAY_TYPE || PASCAL_TYPE_PACKED (type))
        error ("first argument to `Pack' must be an unpacked array");
      else if (TREE_CODE (type2) != TREE_CODE (unpacked_domain)
               && (TREE_CODE (unpacked_domain) != INTEGER_TYPE
                   || TREE_CODE (type2) != TREE_CODE (TREE_TYPE (unpacked_domain))))
        error ("second argument to `Pack' must be of unpacked array index type");
      else if (TREE_CODE (TREE_TYPE (type)) != TREE_CODE (TREE_TYPE (type3)))
        error ("source and destination arrays in `Pack' are not of same type");
      else
        return pascal_unpack_and_pack (0, val, val3, val2);
      return error_mark_node;
    }

  case p_UNPACK:
    {
      tree val3, type3, unpacked_domain;

      emit_line_note (input_filename, lineno);
      INLINE_RTS_LENGTH ("unpack", 3);

      val3 = string_may_be_char (TREE_VALUE (TREE_CHAIN (TREE_CHAIN (apar))));
      type3 = TREE_TYPE (val3);
      unpacked_domain = TYPE_DOMAIN (type2);

      /* I just wonder why on venus they had to shuffle these */
      if (TREE_CODE (type2) != ARRAY_TYPE || PASCAL_TYPE_PACKED (type2))
        error ("second argument to `Unpack' must be an unpacked array");
      else if (TREE_CODE (type) != ARRAY_TYPE || !PASCAL_TYPE_PACKED (type))
        error ("first argument to `Unpack' must be a packed array");
      else if (TREE_CODE (type3) != TREE_CODE (unpacked_domain)
               && (TREE_CODE (unpacked_domain) != INTEGER_TYPE
                   || TREE_CODE (type3) != TREE_CODE(TREE_TYPE (unpacked_domain))))
        error ("third argument to `Unpack' must be of unpacked array index type");
      else if (TREE_CODE (TREE_TYPE (type)) != TREE_CODE (TREE_TYPE (type2)))
        error ("source and destination arrays in `Unpack' are not of same type");
      else
        return pascal_unpack_and_pack (1, val2, val, val3);
      return error_mark_node;
    }

  case bp_ASSIGNED:
    {
      if (PEDANTIC (B_D_PASCAL))
        pedwarn ("`Assigned' is a Borland Pascal extension");
      INLINE_RTS_LENGTH ("assigned", 1);
      if (code != POINTER_TYPE && code != REFERENCE_TYPE)
        errstr = "pointer or procedural type required for `Assigned'";
      else
        retval = build (NE_EXPR, boolean_type_node, integer_zero_node, val);
      break;
    }

  /* calls to run time system */

  /* statements */
  case p_NEW:
  case bp_GETMEM:
    {
      /*
       * There are a lot of allowed call styles for `New':
       *
       *   New ( Ptr );                      (* Standard Pascal *)
       *   New ( RecordPtr, TagFields );     (* SP pointer to variant record *)
       *   New ( SchemaPtr, Discriminants ); (* EP pointer to schema *)
       *   New ( Ptr, ConstructorCall );     (* BP poitner to object *)
       *
       *   Ptr:= New ( PtrType );            (* BP ordinary pointer *)
       *
       *   Ptr:= New ( PtrType,              (* BP pointer to object *)
       *               ConstructorCall );
       *
       *   GetMem ( PtrVar, Size );          (* BP *)
       *
       * Just to maximize confusion, we allow `New' for schemata and `GetMem'
       * to be called as "functions" as well.
       *
       *   Ptr:= GetMem ( Size );            (* GNU *)
       *
       *   Ptr:= New ( PtrType,              (* GNU pointer to schema *)
       *               Discriminants );
       *
       * Internally, we call `New' and `GetMem' as functions.  If called as a
       * procedure, we do the assignment inline.
       */

      tree result, save_type = NULL_TREE, tags = NULL_TREE;

      if (rts_id == bp_GETMEM)
        {
          if (PEDANTIC (B_D_PASCAL))
            pedwarn ("`GetMem' is a Borland Pascal extension");
          if (length > 2)       /* @@ Parser prevents this */
            errstr = "too many arguments to `GetMem'";
          else if (length == 2)
            {
              if (code != POINTER_TYPE)
                errstr = "pointer type required for `GetMem'";
              else if (TREE_CODE (type2) != INTEGER_TYPE)
                errstr = "size argument to `GetMem' must be of Integer type";
            }
          else if (TREE_CODE (type) != INTEGER_TYPE)
            errstr = "size argument to `GetMem' must be of Integer type";
        }
      else if (code != POINTER_TYPE)
        errstr = "pointer type required for `New'";

      if (errstr)
        break;

      if (rts_id == p_NEW
          && (PASCAL_TYPE_UNDISCRIMINATED_STRING (TREE_TYPE (type))
              || PASCAL_TYPE_PREDISCRIMINATED_STRING (TREE_TYPE (type))
              || PASCAL_TYPE_UNDISCRIMINATED_SCHEMA (TREE_TYPE (type))
              || PASCAL_TYPE_PREDISCRIMINATED_SCHEMA (TREE_TYPE (type))))
        {
          schema_ids = number_of_schema_discriminants (TREE_TYPE (type));

          /* If this is a schema type, NEW does two things:
           *  - it selects the type of the object
           *  - and allocates the required space for the object.
           *
           * So we must create a new type for the object with the
           * schema discriminants filled with the values.
           */
          if (schema_ids)
            {
              if (length < schema_ids + 1)
                {
                  error ("`New' applied to this schema requires %d %s",
                         schema_ids, schema_ids > 1 ? "discriminant values"
                                                    : "discriminant value");
                  return error_mark_node;
                }

              if (PASCAL_TYPE_STRING (TREE_TYPE (type)))
                type = build_pointer_type (build_pascal_string_schema (val2));
              else if (PASCAL_TYPE_SCHEMA (TREE_TYPE (type)))
                {
                  tree schema_type = TREE_TYPE (type);
                  while (TYPE_LANG_BASE (schema_type)
                         && TYPE_LANG_BASE (schema_type) != schema_type)
                    schema_type = TYPE_LANG_BASE (schema_type);
                  if (TREE_CODE (schema_type) == TYPE_DECL)
                    schema_type = TREE_TYPE (schema_type);
                  type = build_pointer_type (
                           build_discriminated_schema_type (schema_type,
                                                            TREE_CHAIN (apar)));
                }
              else
                warning ("Internal GPC problem: malformed string or schema");

              /* Force the type of the variable to be a pointer to
               * the discriminated schema type instead of a pointer
               * to the schema type.  This will be undone after the
               * newly allocated object will be initialized.
               */
              save_type = TREE_TYPE (val);
              TREE_TYPE (val) = type;
            }
        }

      /* Since `New' is a function internally, call it and do the
       * assignment of the return value inline.  But later.  For now take
       * the first argument out of game (the pointer where the result is
       * assigned).  The only parameter to the run time system is always
       * the size of the object being created (of type `SizeType').
       */
      fpar = do_ptype (sizetype, 0, 1);

      if (rts_id == bp_GETMEM && length == 1)
        type = ptr_type_node;
      else
        apar = TREE_CHAIN (apar);
      if (rts_id == p_NEW)
        {
          if (length != 1)
            {
              /*
               * Object constructor calls are handled by the parser
               * so the remaining values are for schema discriminants
               * and tag fields of variant records.
               * Check if their number matches the schema/record.
               */
              int i;
              tags = apar;
              for (i = 0; i < schema_ids && tags; i++)
                tags = TREE_CHAIN (tags);
              length -= schema_ids;
              if (length != 1 && PEDANTIC (~U_B_D_PASCAL))
                {
                  pedwarn ("UCSD and Borland Pascal do not support");
                  pedwarn (" tag fields in `New'");
                }
            }
          apar = build_tree_list (NULL_TREE,
                                  object_size (TREE_TYPE (type)));
        }

      /* Call the library function.
       */
      retval = rts_call (rts_id, type, fpar, apar);
      rts_inline = 1;

      if (TREE_CODE (val) == TYPE_DECL
          || (rts_id == bp_GETMEM && length == 1))
        {
          /* Function-style call.
           */
          /* We use a temporary variable here because we want to avoid
           * this function to be called more than once if it returns a
           * string or schema.
           */
          result = make_new_variable ("new", TREE_TYPE (retval));
          expand_expr_stmt (build_modify_expr (result, NOP_EXPR, retval));
          retval = result;
        }
      else
        {
          /* Procedure-style call.
           *
           * Do the assignment to the first parameter here.
           */
          expand_expr_stmt (build_modify_expr (val, NOP_EXPR, retval));

          /* Mark the pointer as being assigned.
           */
          if (TREE_CODE (val) == VAR_DECL)
            PASCAL_VALUE_ASSIGNED (val) = 1;

          retval = void_type_node;
          result = val;
        }

      /* Un-initialize the object we get from new.
       */
      if (rts_id != bp_GETMEM)
        init_any (build_indirect_ref (result, "New"), 0);

      /* If there are tag fields, assign them.
       */
      if (tags)
        assign_tags (build_indirect_ref (result, "New"), tags);

      /* If this was a schema type, restore the undiscriminated schema type
       * after init_any has done its job to avoid type conflicts when this
       * pointer is assigned to some lvalue.
       */
      if (save_type)
        {
          TREE_TYPE (result) = save_type;

          /* VAL might be a type decl, thus we must repair it, too.
           */
          TREE_TYPE (val) = save_type;
        }

      break;
    }

  case p_DISPOSE:
    {
      if (code == ERROR_MARK)  /* error message already has been given */
        return error_mark_node;
      if (code != POINTER_TYPE)
        {
          errstr = "pointer type required for `Dispose'";
          break;
        }
      if (integer_zerop (val)
          && (flag_what_pascal & B_D_PASCAL) == 0)
        warning ("disposing `nil' has no effect");

      if (length > 1)
        {
          static int informed = 0;
          if ((pedantic || flag_what_pascal) && ! informed)
            {
              warning ("tag fields ignored in `Dispose'");
              informed++;
            }
          apar = copy_node (apar);
        }
      break;
    }

  case bp_FREEMEM:
    {
      if (code == ERROR_MARK)  /* error message already has been given */
        return error_mark_node;
      if (PEDANTIC (B_D_PASCAL))
        pedwarn ("`FreeMem' is a Borland Pascal extension");
      if (code != POINTER_TYPE)
        {
          errstr = "pointer type required for `FreeMem'";
          break;
        }
      if (length > 2)
        {
          errstr = "too many arguments to `FreeMem'";
          break;
        }
      if (length == 2)
        {
          if (TREE_CODE (type2) != INTEGER_TYPE)
            {
              errstr = "second argument to `FreeMem' must be of integer type";
              break;
            }
          /*
           * @@@ Perhaps we should do a run-time check with second parameter?
           */
         }
      break;
    }

  case p_POSITION:
  case p_LASTPOSITION:
  case bp_FILESIZE:
  case bp_FILEPOS:
    {
      rval = long_long_integer_type_node;
      if (code != FILE_TYPE)
          errstr = "file type required";
      else
        {
          fpar = do_ptype (type, 1, 1);
          if (TYPE_DOMAIN (type))
            {
              retval = build_binary_op (PLUS_EXPR,
                                        rts_call (rts_id, rval, fpar, apar),
                                        convert (rval,
                                                 TYPE_MIN_VALUE (TYPE_DOMAIN (type))),
                                        0);

              rts_inline = 1; /* Fake it is inline */
            }
          else if (rts_id == p_POSITION)
            {
              if (PEDANTIC (E_O_PASCAL))
                pedwarn ("`Position' is an ISO 10206 Extended Pascal extension");
            }
          else if (rts_id == p_LASTPOSITION)
            {
              if (PEDANTIC (E_O_PASCAL))
                pedwarn ("`LastPosition' is an ISO 10206 Extended Pascal extension");
            }
          else if (rts_id == bp_FILESIZE)
            {
              if (PEDANTIC (B_D_PASCAL))
                pedwarn ("`FileSize' is a Borland Pascal extension (ISO 10206 has `LastPosition')");
            }
          else if (rts_id == bp_FILEPOS)
            {
              if (PEDANTIC (B_D_PASCAL))
                pedwarn ("`FilePos' is a Borland Pascal extension (ISO 10206 has `Position')");
            }
          else if (PEDANTIC (B_D_PASCAL))
            pedwarn ("ISO 10206 Extended Pascal does not allow direct access to normal files");
        }
      break;
    }

  case p_PAGE:
    {
      if (length == 0)
        {
          apar = build_tree_list (NULL_TREE, get_standard_output ());
          type = TREE_TYPE (TREE_VALUE (apar));
          code = TREE_CODE (type);
        }
    }
    /* FALLTHROUGH */

  case p_CLOSE:
  case p_DONEFDR:
  case p_UPDATE:
  case p_PUT:
  case p_GET:
  case p_GOT:
  case bp_TRUNCATE:
  case bp_FLUSH:
  case bp_ERASE:
    {
      switch (rts_id)
        {
          case bp_TRUNCATE:
            {
              if (PEDANTIC (B_D_PASCAL))
                pedwarn ("`Truncate' is a Borland Pascal extension");
              break;
            }
          case bp_FLUSH:
            {
              if (PEDANTIC (B_D_PASCAL))
                pedwarn ("`Flush' is a Borland Pascal extension");
              break;
            }
          case bp_ERASE:
            {
              if (PEDANTIC (B_D_PASCAL))
                pedwarn ("`Erase' is a Borland Pascal extension");
              break;
            }
        }
      if (code != FILE_TYPE)
        errstr = "file type required";
      else
        fpar = do_ptype (type, 1, 1);
      break;
    }

  case bp_RENAME:
    {
      if (PEDANTIC (B_D_PASCAL))
        pedwarn ("`Rename' is a Borland Pascal extension");
      if (length != 2)
        errstr = "invalid number of parameters to `Rename'";
      else if (code != FILE_TYPE)
        errstr = "file type required for first parameter to `Rename'";
      else if (! is_string_type (val2, 1))
        errstr = "string type required for second parameter of `Rename'";
      else
        fpar = chainon (do_ptype (type, 1, 0), ptype_cstring);
      break;
    }

  case bp_RUNERROR:
    {
      if (length == 0)
        apar = build_tree_list (NULL_TREE, build_int_2 (-1, -1));
      else if (code != INTEGER_TYPE)
        errstr = "integer type required for `RunError'";
      fpar = do_ptype (integer_type_node, 0, 1);
      break;
    }

  case p_HALT:
    {
      if (length == 0)
        apar = build_tree_list (NULL_TREE, integer_zero_node);
      else if (code != INTEGER_TYPE)
        {
          errstr = "`Halt' requires integer type parameter";
          break;
        }
      fpar = do_ptype (TREE_TYPE (TREE_VALUE (apar)), 0, 1);
      break;
    }

  case p_BINDING:
    {
      tree stype = gpc_type_BINDINGTYPE;

      if (PEDANTIC (E_O_PASCAL))
        pedwarn ("`Binding' is an ISO 10206 Extended Pascal extension");

      /* Check for FILE_TYPE here too; it is not enough if p_BIND does it.
       */
      if (TREE_CODE (type) != FILE_TYPE)
        errstr = "GPC supports `Binding' only for file type objects";

      /* Pass a reference to a temporary variable; run time system
       * fills it it instead of returning a record type.
       */
      actual_return_value = make_new_variable ("binding", stype);
      fpar = chainon (do_ptype (type, 1, 0), do_ptype (stype,1, 1));
      apar = chainon (apar, build_tree_list (NULL_TREE, actual_return_value));
      break;
    }

  case p_UNBIND:
    {
      if (PEDANTIC (E_O_PASCAL))
        pedwarn ("`Unbind' is an ISO 10206 Extended Pascal extension");

      /* Check for FILE_TYPE here as well; it is not enough if p_BIND does it.
       */
      if (TREE_CODE (type) != FILE_TYPE)
        errstr = "GPC supports `Unbind' only for file type objects";

      fpar = do_ptype (type, 1, 1);
      break;
    }

  case p_BIND:
    {
      if (PEDANTIC (E_O_PASCAL))
        pedwarn ("`Bind' is an ISO 10206 Extended Pascal extension");

      if (TREE_CODE (type) != FILE_TYPE)
        errstr = "GPC supports `Bind' only for file type objects";
      else if (type2 != gpc_type_BINDINGTYPE)
        errstr = "second argument to `Bind' must be of type `BindingType'";
#if 0
      /* Why not??? */
      else if (! PASCAL_EXTERNAL_OBJECT (apar))
        errstr = "GPC supports `Bind' only for external files";
#endif
      /* @@@@@@ I'm too tired to do this now:
       * For variable parameters the bindability is determined by
       * the type of the actual parameter, at least when it is a file
       * type.
       * Now I allow binding of all var file parameters, not only
       * those that are declared bindable.
       * @@@@@ FIXME
       */
      else if (((flag_what_pascal & E_O_PASCAL) != 0
                 && (flag_what_pascal & ! B_D_PASCAL) == 0)
               && (! PASCAL_TYPE_BINDABLE (type)
                   && !((TREE_CODE (val) == INDIRECT_REF)
                        && ((TREE_CODE (TREE_OPERAND (val, 0)) == PARM_DECL)
                            || ((TREE_CODE (TREE_OPERAND (val, 0)) == CONVERT_EXPR)
                                && (TREE_CODE (TREE_OPERAND (TREE_OPERAND (val, 0), 0))
                                    == PARM_DECL))))))
        errstr = "type has not been declared `bindable'";
      else
        fpar = chainon (do_ptype (type, 1,0), do_ptype (type2,1,1));
      break;
    }

  case p_DATE:
  case p_TIME:
    {
      if (PEDANTIC (E_O_PASCAL))
        pedwarn ("`Date' and `Time' are ISO 10206 Extended Pascal extensions");

      if (type != gpc_type_TIMESTAMP)
        errstr = "TimeStamp record type parameter required";
      else
        {
          /* A REFERENCE_TYPE to val and an additional parameter
           * that is the location where run time system stores
           * DATE/TIME conversion results.
           */

          tree stype = (rts_id == p_DATE) ? gpc_type_DATE : gpc_type_TIME;

          actual_return_value = make_new_variable ("times", stype);

          fpar = chainon (do_ptype (type, 1, 0),
                          do_ptype (stype, 1, 1));

          apar = chainon (apar, build_tree_list (NULL_TREE, actual_return_value));

          rval = void_type_node;
        }
      break;
    }

  case p_GETTIMESTAMP:
    {
      if (PEDANTIC (E_O_PASCAL))
        pedwarn ("`GetTimeStamp' is an ISO 10206 Extended Pascal extension");
      if (type != gpc_type_TIMESTAMP)
        errstr = "TimeStamp record type parameter required";
      else
        fpar = do_ptype (type, 1, 1);
      break;
    }

  case ucsd_SEEK:
  case p_SEEKUPDATE:
  case p_SEEKREAD:
  case p_SEEKWRITE:
    {
      if (rts_id == ucsd_SEEK && PEDANTIC (U_B_D_PASCAL))
        pedwarn ("`Seek' is an UCSD Pascal extension");
      if (rts_id != ucsd_SEEK && PEDANTIC (E_O_PASCAL))
        pedwarn ("this seek operation is an ISO 10206 Extended Pascal extension");
      if (code != FILE_TYPE)
        errstr = "file type required for seek operations";
      else
        {
          fpar = chainon (do_ptype (type, 1, 0), ptype_longint);

          if (val2 && ORDINAL_TYPE (code2))
            {
              if (TYPE_DOMAIN (type))
                {
                  if (comptypes (TREE_TYPE (TYPE_DOMAIN (type)), type2) != 0)
                    {
                      val2 = build_binary_op (MINUS_EXPR,
                                              val2,
                                              TYPE_MIN_VALUE (TYPE_DOMAIN (type)),
                                              0);

                      TREE_VALUE (TREE_CHAIN (apar)) = val2;
                    }
                  else
                    errstr = "index type does not match direct access file range type";
                }
              else if (pedantic || flag_what_pascal)
                pedwarn ("ISO 10206 Extended Pascal does not allow direct access to normal files");
            }
          else
            errstr = "second argument to seek routines must be of ordinal type";
        }
      break;
    }

  /* functions */
  case p_EOF:
    {
      rval = boolean_type_node;

      if (length == 0)
        {
          tree std_input = get_standard_input ();
          if (std_input)
            apar = build_tree_list (NULL_TREE, std_input);
        }
      else if (code != FILE_TYPE)
        {
          errstr = "EOF may only be applied to files";
          break;
        }
      if (apar)
          fpar = do_ptype (TREE_TYPE (TREE_VALUE (apar)), 1, 1);
      break;
    }

  case p_EOLN:
    {
      rval = boolean_type_node;

      if (length == 0)
        {
          tree std_input = get_standard_input ();
          if (std_input)
            apar = build_tree_list (NULL_TREE, std_input);
        }
      else if (code != FILE_TYPE || TYPE_FILE_TEXT (TREE_TYPE (val)) == NULL_TREE)
        {
          errstr = "`EOLn' may only be applied to `Text' files";
          break;
        }
      if (apar)
          fpar = do_ptype (TREE_TYPE (TREE_VALUE (apar)), 1, 1);
      break;
    }

  case p_POLAR:
    {
      if (code == REAL_TYPE && code2 == REAL_TYPE)
        {
          rval = complex_type_node;
          fpar = chainon (do_ptype (double_type_node, 0, 0),
                          do_ptype (double_type_node, 0, 1));
        }
      else
        errstr = "the two arguments to `Polar' must be of real type";
      break;
    }

  case p_SQRT:
    {
      if (code == COMPLEX_TYPE)
        {
          rval = complex_type_node;
          fpar = ptype_complex;
          rts_id = z_SQRT;
        }
      else
        {
          rval = double_type_node;
          if (code == INTEGER_TYPE || code == REAL_TYPE)
            fpar = ptype_double;
          else
            errstr = "argument to `Sqrt' must be of integer or real type";
          if (code == REAL_TYPE
              && TYPE_PRECISION (type) > TYPE_PRECISION (double_type_node))
            {
              rval = long_double_type_node;
              fpar = ptype_long_double;
              rts_id = pp_SQRT;
            }
        }
      break;
    }

  case p_SIN:
  case p_COS:
  case p_EXP:
  case p_LN:
  case p_ARCTAN:
    {
      if (code == COMPLEX_TYPE)
        {
          rval = complex_type_node;
          fpar = ptype_complex;
          switch (rts_id)
            {
              case p_SIN:    rts_id = z_SIN;    break;
              case p_COS:    rts_id = z_COS;    break;
              case p_EXP:    rts_id = z_EXP;    break;
              case p_LN:     rts_id = z_LN;     break;
              case p_ARCTAN: rts_id = z_ARCTAN; break;
              default:       abort ();
            }
        }
      else if (code == REAL_TYPE
               && TYPE_PRECISION (type) > TYPE_PRECISION (double_type_node))
        {
          switch (rts_id)
            {
              case p_SIN:    rts_id = pp_SIN;    break;
              case p_COS:    rts_id = pp_COS;    break;
              case p_EXP:    rts_id = pp_EXP;    break;
              case p_LN:     rts_id = pp_LN;     break;
              case p_ARCTAN: rts_id = pp_ARCTAN; break;
              default:       abort ();
            }
          rval = long_double_type_node;
          fpar = ptype_long_double;
        }
      else
        {
          rval = double_type_node;
          fpar = ptype_double;
        }
      break;
    }

  case ucsd_INT:
  case bp_FRAC:
    {
      rval = long_double_type_node;
      fpar = ptype_long_double;
      break;
    }

  case p_ARG:
    {
      if (code == COMPLEX_TYPE)
        {
          fpar = ptype_complex;
          rval = double_type_node;
        }
      else
        errstr = "`Arg' requires one complex type argument";
      break;
    }

  case r_EXPON:
    {
      /* Exponent type is checked in the parser.
       */
      rval = double_type_node;
      if (code == COMPLEX_TYPE)
        {
          rts_id = z_EXPON;
          rval = complex_type_node;
        }
      else if (code != REAL_TYPE && code != INTEGER_TYPE)
        errstr = "left operand of `**' must be of integer, real or complex type";
      else if (TYPE_PRECISION (type) > TYPE_PRECISION (double_type_node)
               || TYPE_PRECISION (type2) > TYPE_PRECISION (double_type_node))
        {
          rts_id = rr_EXPON;
          rval = long_double_type_node;
        }

      if (! errstr)
        fpar = chainon (do_ptype (rval, 0, 0), do_ptype (rval, 0, 1));
      break;
    }

  case r_POW:
    {
      /* Exponent type is checked in the parser.
       */

      rval = type; /* Type is same as left operand type */
      if (code == COMPLEX_TYPE)
        rts_id = z_POW;
      else if (code == INTEGER_TYPE)
        {
          /* @@ Calculate in double type, then convert back to int.
           * @@ Should be inlined.
           */
          post_conversion = integer_type_node;
          rval = double_type_node;
          val  = convert (double_type_node, val);
        }
      else if (code != REAL_TYPE)
        errstr = "left operand of `pow' must be of integer, real or complex type";
      else if (TYPE_PRECISION (type) > TYPE_PRECISION (double_type_node))
        rts_id = rr_POW;

      if (! errstr)
        fpar = chainon (do_ptype (rval, 0, 0),
                        do_ptype (type2, 0, 1));
      break;
    }

  case p_EXTEND:
  case bp_APPEND:
    {
      if (rts_id == p_EXTEND && PEDANTIC (E_O_PASCAL))
        pedwarn ("`Extend' is an ISO 10206 Extended Pascal extension");
      else if (rts_id == bp_APPEND && PEDANTIC (B_D_PASCAL))
        pedwarn ("`Append' is a Borland Pascal extension");
    }
    /* FALLTHROUGH */

  case p_RESET:
  case p_REWRITE:
    {
      if (length > 3)
        errstr = "too many parameters";
      else if (code != FILE_TYPE)
        errstr = "file type required";
      else
        {
          tree name_length = integer_zero_node;
          tree file_name = null_pointer_node;
          tree buffer_size = NULL_TREE;

          if ((rts_id == p_EXTEND || rts_id == bp_APPEND)
              && PEDANTIC (~B_D_PASCAL)
              && ! TYPE_FILE_TEXT (type))
             pedwarn ("`Append' for non-text files is a GNU Pascal extension");

          if (val2)
            {
              tree val3 = NULL_TREE;
              if (is_string_type (val2, 1))
                file_name = val2;
              else if (code2 == INTEGER_TYPE)
                buffer_size = val2;
              else
                errstr = "type mismatch in optional parameter";

              if (TREE_CHAIN (TREE_CHAIN (apar)))
                val3 = TREE_VALUE (TREE_CHAIN (TREE_CHAIN (apar)));
              if (val3)
                {
                  if (TREE_CODE (TREE_TYPE (val3)) == INTEGER_TYPE)
                    buffer_size = val3;
                  else
                    errstr = "file buffer size parameter must be of integer type";
                }
            }

          if (file_name != null_pointer_node)
            {
              name_length = PASCAL_STRING_LENGTH (file_name);

              if (is_variable_string_type (TREE_TYPE (file_name)))
                {
                  /* Use the string, not the schema type.
                   * Converted to a reference by fpar.
                   */
                  append_null_character (file_name);
                  file_name = PASCAL_STRING_VALUE (file_name);
                }
            }

          if (! buffer_size
              && TREE_CODE (TREE_TYPE (type)) == VOID_TYPE)
            {
              if (flag_what_pascal & B_D_PASCAL)
                {
                  warning ("unspecified buffer size for untyped file defaults to 128");
                  buffer_size = build_int_2 (128, 0);
                }
              else
                errstr = "missing buffer size parameter for untyped file";
            }

          if (buffer_size)
            {
              tree file_ptr;

              if (PEDANTIC (U_B_D_PASCAL))
                pedwarn ("file buffer size parameters are an UCSD Pascal extension");
              if (TREE_CODE (TREE_TYPE (type)) != VOID_TYPE)
                error ("file buffer size parameter only allowed for untyped files");

              /* Assign this integer value to the internal file size
               * variable in the FDR which comes after the buffer
               * pointer and the status bits (an integer).
               */
              file_ptr = build_unary_op (ADDR_EXPR, val, 0);
              file_ptr = build1 (NOP_EXPR,
                                 build_pointer_type (integer_type_node),
                                 file_ptr);
              file_ptr = build (PLUS_EXPR, TREE_TYPE (file_ptr),
                                file_ptr, c_size_in_bytes (integer_type_node));
              file_ptr = build (PLUS_EXPR, TREE_TYPE (file_ptr),
                                file_ptr, c_size_in_bytes (ptr_type_node));
              file_ptr = build_indirect_ref (file_ptr, "file opening");
              expand_expr_stmt (build_modify_expr (file_ptr, NOP_EXPR, buffer_size));
            }

          apar = chainon (build_tree_list (NULL_TREE, val),
                          chainon (build_tree_list (NULL_TREE, file_name),
                                   build_tree_list (NULL_TREE, name_length)));
          fpar = chainon (do_ptype (type, 1, 0),
                          chainon (do_ptype (cstring_type_node, 0, 0),
                                   ptype_int));
        }
      break;
    }

  case ucsd_STR:
  case p_WRITESTR:
  case p_WRITE:
  case p_WRITELN:
    {
      emit_line_note (input_filename, lineno);
      rts_write (rts_id, apar);
      return NULL_TREE;
    }

  case p_READSTR:
    {
      if (PEDANTIC (E_O_PASCAL))
        pedwarn ("`ReadStr' is an ISO 10206 Extended Pascal extension");
    }
    /* FALLTHROUGH */

  case p_READ:
  case p_READLN:
    {
      emit_line_note (input_filename, lineno);
      rts_read (rts_id, apar);
      return NULL_TREE;
    }

  case bp_VAL:
    {
      emit_line_note (input_filename, lineno);
      rts_val (apar);
      return NULL_TREE;
    }

  /* UCSD string functions.
   */
  case ucsd_INSERT:
    {
      if (PEDANTIC (U_B_D_PASCAL))
        pedwarn ("`Insert' is an UCSD Pascal extension");
      if (length != 3)
        errstr = "incorrect number of parameters to `Insert'";
      else if (! is_string_type (val, 1) && TREE_CODE (type) != CHAR_TYPE)
        errstr = "string type required for first parameter of `Insert'";
      else if (! is_string_type (val2, 1))
        errstr = "string type required for second parameter of `Insert'";
      else
        {
          tree val3;
          if (TREE_CHAIN (TREE_CHAIN (apar))
              && (val3 = TREE_VALUE (TREE_CHAIN (TREE_CHAIN (apar)))) != NULL_TREE
              && TREE_CODE (TREE_TYPE (val3)) == INTEGER_TYPE)
            {
              /* Add an implicit fourth parameter that tells whether the
               * string shall be truncated if it becomes too long.
               *
               * @@ Currently, we always pass `True'.
               */
              apar = chainon (apar, build_tree_list (NULL_TREE, boolean_true_node));
              fpar = chainon (chainon (do_ptype (const_string_schema_proto_type, 1, 0),
                                       do_ptype (string_schema_proto_type, 1, 0)),
                              chainon (do_ptype (integer_type_node, 0, 0),
                                       do_ptype (boolean_type_node, 0, 1)));
            }
          else
            errstr = "integer type required for third parameter of `Insert'";
        }
      break;
    }

  case ucsd_DELETE:
    {
      if (PEDANTIC (U_B_D_PASCAL))
        pedwarn ("`Delete' is an UCSD Pascal extension");
      if (length != 3)
        errstr = "incorrect number of parameters to `Delete'";
      else if (! is_string_type (val, 1))
        errstr = "string type required for first parameter of `Delete'";
      else if (code2 != INTEGER_TYPE)
        errstr = "integer type required for second parameter of `Delete'";
      else
        {
          tree val3;
          if (TREE_CHAIN (TREE_CHAIN (apar))
              && (val3 = TREE_VALUE (TREE_CHAIN (TREE_CHAIN (apar)))) != NULL_TREE
              && TREE_CODE (TREE_TYPE (val3)) == INTEGER_TYPE)
            {
              fpar = chainon (do_ptype (string_schema_proto_type, 1, 0),
                              chainon (do_ptype (integer_type_node, 0, 0),
                                       do_ptype (integer_type_node, 0, 1)));
            }
          else
            errstr = "integer type required for third parameter of `Delete'";
        }
      break;
    }

  /* Other extensions.
   */
  case p_MARK:
    {
      if (PEDANTIC (U_B_D_PASCAL))
        pedwarn ("`Mark' is an UCSD Pascal extension");
      if (length != 1)
        errstr = "incorrect number of parameters to `Mark'";
      else if (code != POINTER_TYPE)
        errstr = "pointer type required for `Mark'";
      else
        fpar = do_ptype (type, 1, 1);
      break;
    }

  case p_RELEASE:
    {
      if (PEDANTIC (U_B_D_PASCAL))
        pedwarn ("`Release' is an UCSD Pascal extension");
      if (length != 1)
        errstr = "incorrect number of parameters to `Release'";
      else if (code != POINTER_TYPE)
        errstr = "pointer type required for `Release'";
      else
        fpar = do_ptype (type, 0, 1);
      break;
    }

  case p_DEFINESIZE:
    {
      if (pedantic || flag_what_pascal)
        pedwarn ("`DefineSize' is a GNU Pascal extension");
      if (length != 2)
        errstr = "incorrect number of parameters to `DefineSize'";
      else if (code != FILE_TYPE)
        errstr = "file type required for first parameter to `DefineSize'";
      else if (code2 != INTEGER_TYPE)
        errstr = "integer type required for second parameter to `DefineSize'";
      else
        fpar = chainon (do_ptype (type, 1, 0), ptype_longint);
      break;
    }

  case bp_PARAMCOUNT:
    {
      tree argc_name = get_identifier ("_p_argc");
      INLINE_RTS_LENGTH ("ParamCount", 0);
      if (PEDANTIC (B_D_PASCAL))
        pedwarn ("`ParamCount' is a Borland Pascal extension");
      declare_vars (build_tree_list (NULL_TREE, argc_name),
                    integer_type_node, NULL_TREE, NULL_PTR,
                    VQ_EXTERNAL, 0, NULL_TREE);
      retval = build_pascal_binary_op (MINUS_EXPR,
                                       lookup_name (argc_name),
                                       integer_one_node);
      break;
    }

  case bp_PARAMSTR:
    {
      tree empty_string;
      tree condition;
      tree stmt;
      tree capacity, the_paramstr, move_and_capacity;
      tree argc_name = get_identifier ("_p_argc");
      tree argv_name = get_identifier ("_p_argv");

      INLINE_RTS_LENGTH ("ParamStr", 1);

      if (PEDANTIC (B_D_PASCAL))
        pedwarn ("`ParamStr' is a Borland Pascal extension");

      empty_string = build_string (1, "");
      TREE_TYPE (empty_string) = char_array_type_node;

      if (code != INTEGER_TYPE)
        {
          error ("argument to `ParamStr' must be of integer type");
          /* Avoid further error messages */
          return new_string_by_model (NULL_TREE, empty_string, 1);
        }

      /* condition:= ( val < 0 ) or ( val >= _p_argc );
       * if condition then
       *   capacity:= 0
       * else
       *   capacity:= strlen ( _p_argv [ val ]^ );
       * Var retval: String ( capacity );
       * if condition then
       *   retval:= ''
       * else
       *   begin
       *     retval.length:= capacity;
       *     move ( _p_argv [ val ]^, retval.string, capacity );
       *   end (* else *);
       */
      declare_vars (build_tree_list (NULL_TREE, argc_name),
                    integer_type_node, NULL_TREE, NULL_PTR,
                    VQ_EXTERNAL, 0, NULL_TREE);
      TREE_USED (lookup_name (argc_name)) = 1;
      declare_vars (build_tree_list (NULL_TREE, argv_name),
                    build_pointer_type (cstring_type_node), NULL_TREE,
                    NULL_PTR, VQ_EXTERNAL, 0, NULL_TREE);
      TREE_USED (lookup_name (argv_name)) = 1;
      condition = make_new_variable ("paramstr_condition", boolean_type_node);
      TREE_USED (condition) = 1;
      if (TREE_UNSIGNED (TREE_TYPE (val)))
        {
          /* Save one comparison when VAL is unsigned.
           */
          stmt = build_modify_expr (condition, NOP_EXPR,
              build_pascal_binary_op (GE_EXPR, val, lookup_name (argc_name)));
        }
      else
        {
          tree condition1 = build_pascal_binary_op (LT_EXPR, val,
                                                    integer_zero_node);
          tree condition2 = build_pascal_binary_op (GE_EXPR, val,
                                                    lookup_name (argc_name));
          stmt = build_modify_expr (condition, NOP_EXPR,
              build_pascal_binary_op (TRUTH_ORIF_EXPR, condition1, condition2));
        }
      the_paramstr = build_array_ref (lookup_name (argv_name), val);
      capacity = make_new_variable ("paramstr_capacity", integer_type_node);
      TREE_USED (capacity) = 1;
      stmt = build (COMPOUND_EXPR, void_type_node, stmt,
          build_modify_expr (capacity, NOP_EXPR,
              build (COND_EXPR, integer_type_node, condition, integer_zero_node,
                 build_function_call (lookup_name (get_identifier ("__builtin_strlen")),
                                      build_tree_list (NULL_TREE, the_paramstr)))));
      capacity = build (COMPOUND_EXPR, integer_type_node, stmt, capacity);
      retval = alloca_string (capacity);
      TREE_USED (retval) = 1;

      move_and_capacity = build_string_move (build_unary_op (ADDR_EXPR, PASCAL_STRING_VALUE (retval), 0),
                                             the_paramstr, capacity),
      move_and_capacity = build (COMPOUND_EXPR, integer_type_node,
                                 move_and_capacity, capacity);
      stmt = build_modify_expr (PASCAL_STRING_LENGTH (retval), NOP_EXPR,
                                build (COND_EXPR, integer_type_node, condition,
                                       integer_zero_node, move_and_capacity));
      TREE_USED (stmt) = 1;
      retval = build (COMPOUND_EXPR, TREE_TYPE (retval), stmt, retval);
      retval = build1 (NON_LVALUE_EXPR, TREE_TYPE (retval), retval);
      break;
    }

  case bp_ASSIGN:
    {
      if (PEDANTIC (B_D_PASCAL))
        pedwarn ("`Assign' is a Borland Pascal extension");
      if (length != 2)
        errstr = "incorrect number of parameters to `Assign'";
      else if (code != FILE_TYPE)
        errstr = "file type required for first parameter of `Assign'";
      else if (! is_string_type (val2, 1))
        errstr = "string type required for second parameter of `Assign'";
      else
        fpar = chainon (do_ptype (type, 1, 0), ptype_const_string_schema);
      break;
    }

  case bp_CHDIR:
    {
      if (PEDANTIC (B_D_PASCAL))
        pedwarn ("`ChDir' is a Borland Pascal extension");
      if (length != 1)
        errstr = "incorrect number of parameters to `ChDir'";
      else if (! is_string_type (val, 1))
        errstr = "string type required for parameter of `ChDir'";
      else
        fpar = ptype_cstring;
      break;
    }

  case bp_MKDIR:
    {
      if (PEDANTIC (B_D_PASCAL))
        pedwarn ("`MkDir' is a Borland Pascal extension");
      if (length != 1)
        errstr = "incorrect number of parameters to `MkDir'";
      else if (! is_string_type (val, 1))
        errstr = "string type required for parameter of `MkDir'";
      else
        fpar = ptype_cstring;
      break;
    }

  case bp_RMDIR:
    {
      if (PEDANTIC (B_D_PASCAL))
        pedwarn ("`RmDir' is a Borland Pascal extension");
      if (length != 1)
        errstr = "incorrect number of parameters to `RmDir'";
      else if (! is_string_type (val, 1))
        errstr = "string type required for parameter of `RmDir'";
      else
        fpar = ptype_cstring;
      break;
    }

  case ucsd_IORESULT:
    {
      if (PEDANTIC (U_B_D_PASCAL))
        pedwarn ("`IOResult' is an UCSD Pascal extension");
      if (length != 0)
        errstr = "incorrect number of parameters to `IOResult'";
      rval = integer_type_node;
      break;
    }

  case p_EMPTY:
    {
      if (PEDANTIC (E_O_PASCAL))
        pedwarn ("`Empty' is an ISO 10206 Extended Pascal extension");
      if (length != 1)
        errstr = "incorrect number of parameters to `Empty'";
      else if (code != FILE_TYPE)
        errstr = "file type required for `Empty'";
      else
        fpar = do_ptype (type, 1, 1);
      rval = boolean_type_node;
      break;
    }

  /* UCSD string functions */
  case ucsd_POS:
  case ucsd_COPY:
  /* Extended Pascal string functions */
  case p_EQ:
  case p_LT:
  case p_LE:
  case p_GE:
  case p_GT:
  case p_NE:
  case p_TRIM:
  case p_INDEX:  /* reserved word in Borland Pascal */
  case p_SUBSTR:
  /* String comparisons */
  case '=':
  case LEX_NE:
  case '<':
  case LEX_LE:
  case '>':
  case LEX_GE:
    {
      if (val && (val2 || rts_id == p_TRIM))
        {
          emit_line_note (input_filename, lineno);
          return rts_string (rts_id, apar);
        }
      else
        errstr = "incorrect number of parameters to a string routine";
      break;
    }

  default:
    {
      int rts_routine_found = 0;
      assert (rts_routine_found);
    }

  }  /* The big `switch' statement ends here. */

  if (errstr)
    {
      error (errstr);
      if (ptrarith_inform_flag)
        ptrarith_inform ();
      return error_mark_node;
    }

  /* Output an RTL line note.
   */
  emit_line_note (input_filename, lineno);

  /* Construct a call to the run time system if not compiled inline.
   */

  if (! rts_inline)
    {
      retval = rts_call (rts_id, rval, fpar, apar);

      /* If this is a statement, built rtl from it,
       * otherwise let the caller do whatever it likes to do with it.
       */
      if (rval == void_type_node)
        {
          expand_expr_stmt (retval);

          /* If we need to return something, like a string that the
           * RTS wrote into in p_DATE/p_TIME
           */
          if (actual_return_value)
            retval = actual_return_value;
        }
      else
        if (post_conversion)
          retval = convert (post_conversion, retval);

      /* Un-initialize at run-time the node in UN_INIT_THIS.
       */
      if (un_init_this)
        init_any (un_init_this, 0);
    }
  return retval;
}



/*
 * File I/O.
 */

/*
 * The first word of the FILE_TYPE BLKmode variable is
 * a pointer to the file buffer we want to access.
 *
 * So we need to generate:  *(*(&file))
 *
 * MODE can have one of the following values:
 * * 0 for reading (Read on typed files)
 * * 1 for writing (Write on typed files)
 * * 2 for reading and/or writing (dereferencing file buffers)
 *
 * Because of lazy I/O, each buffer access needs to check
 * that the buffer is valid. If not, we need to do a get before
 * accessing the buffer.
 *
 * Lazy I/O helps to overcome the biggest problem of Pascal:
 * It's I/O system.
 *
 * When we do a reset or read something from a file, the old method
 * needs to read new contents to the buffer before the data is
 * actually needeed.  This is annoying if you do interactive programs,
 * the output to terminal asking for input comes after you have
 * already given the input to the program, or then you have to code
 * things differently for terminals and files, which is also annoying.
 *
 * The old method GPC uses is the same implementation dependent feature
 * that the Pax compiler uses. The run time system checks each file when
 * it's reset, and if it's a terminal, it sets EOLN implicitely on and places
 * a space in the file buffer. This is valid according to the standard, since
 * it explicitely states that handling of INPUT and OUTPUT is implementation
 * dependent.
 *
 * Lazy I/O means that we must do a PUT as early as we can, and GET as
 * late as we can.  The first condition is satisfied either by not
 * buffering output at all, or then flushing output to terminals
 * before each get; the second condition is fulfilled when we check
 * that the buffer is valid each time we generate buffer references.
 */

tree
build_buffer_ref (file, mode)
     tree file; int mode;
{
  register tree ft = TREE_TYPE (file);  /* type of variable */

  if (TREE_CODE (ft) == FILE_TYPE)
    {
      tree t = TREE_TYPE (ft);          /* type of file component */
      register tree ref;

      tree file_addr = build1 (ADDR_EXPR, ft, file);

      /* For reading: Fill file buffer if it is undefined
       * @@@@ To use lazy I/O implement code that does a "get" if the file buffer is undefined
       * @@@@ Easy way out: call run time routine _p_lazyget(File);
       * For writing: Empty the buffer if it is defined
       */
        expand_expr_stmt (rts_call ((mode == 0) ? r_LAZYGET :
                                    ((mode == 1) ? r_LAZYUNGET
                                     : r_LAZYTRYGET),
                                    void_type_node,
                                    do_ptype (TREE_TYPE (file), 1, 0),
                                    build_tree_list (NULL_TREE, file)));

      ref = build1 (INDIRECT_REF, TYPE_MAIN_VARIANT (t),               /* type we point to */
                    build1 (INDIRECT_REF, build_pointer_type (t),      /* Through this pointer */
                            file_addr));                               /* Address of file */
      TREE_READONLY (ref) = TREE_READONLY (t);
      TYPE_VOLATILE (ref) = TYPE_VOLATILE (t) || TYPE_VOLATILE (file);
      TREE_THIS_VOLATILE (ref) = TYPE_VOLATILE (t);
      init_any (ref, 0);
      return ref;
    }
  else if (file != error_mark_node && ft != error_mark_node)
    error ("file buffer referencing requires a file type argument");
  return error_mark_node;
}

/*
 * Return standard input/output node of current module.
 * If not found, return global_in/output_file_node and warn
 * about ISO violation.  Never return NULL.
 */
tree
get_standard_input ()
{
  if (!current_module->input_file_node)
    {
      current_module->input_file_node = global_input_file_node;
      if (PEDANTIC (U_B_D_PASCAL))
        {
          if (current_module->main_program)
            pedwarn ("file `Input' was not mentioned in program header");
          else
            pedwarn ("`StandardInput' not imported by module");
        }
    }
  return current_module->input_file_node;
}

tree
get_standard_output ()
{
  if (!current_module->output_file_node)
    {
      current_module->output_file_node = global_output_file_node;
      if (PEDANTIC (U_B_D_PASCAL))
        {
          if (current_module->main_program)
            pedwarn ("file `Output' was not mentioned in program header");
          else
            pedwarn ("`StandardOutput' not imported by module");
        }
    }
  return current_module->output_file_node;
}

tree
get_standard_error ()
{
  if (!current_module->error_file_node)
    {
      current_module->error_file_node = global_error_file_node;
      if (PEDANTIC (U_B_D_PASCAL))
        {
          if (current_module->main_program)
            pedwarn ("file `StdErr' was not mentioned in program header");
          else
            pedwarn ("`StandardError' not imported by module");
        }
    }
  return current_module->error_file_node;
}

tree
get_builtin_variable (name, type)
     char *name;
     tree type;
{
  tree id = get_identifier (name);
  tree var = lookup_name (id);
  if (var == NULL_TREE)
    {
      declare_vars (build_tree_list (NULL_TREE, id), type, NULL_TREE,
                    NULL_PTR, VQ_EXTERNAL, 0, NULL_TREE);
      var = lookup_name (id);
    }
  return var;
}
