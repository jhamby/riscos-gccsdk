/* Process declarations and variables for Pascal compiler.
   Copyright (C) 1988, 1992-1997, 1998, 2000 Free Software Foundation, Inc.

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


/* Process declarations and symbol lookup for C front end.
   Also constructs types; the standard scalar types at initialization,
   and structure, union, array and enum types when they are declared.  */

/* ??? not all decl nodes are given the most useful possible
   line numbers.  For example, the CONST_DECLs for enum values.  */

#include "gcc-version.h"
#include "config.h"
#include "gansidecl.h"
#include "system.h"

#include <stdio.h>

#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif

#ifdef GPC
#include "gbe.h"
#else
/* In GPC, they are included via gbe.h */
#include "tree.h"
#include "flags.h"
#endif
#include "output.h"
#include "gpc-tree.h"
#include "c-lex.h"

#ifdef GPC
#include "except.h"
#include "function.h"
#include "float.h"
#include "gpc-defs.h"
#include "obstack.h"
#include "module.h"
#include "objects.h"
#include "types.h"
#include "util.h"
#include "gpc-options.h"
#ifdef EGCS
#include "toplev.h"
#endif
#include <ctype.h>

extern rtx tail_recursion_reentry;  /* in function.c */

/* for language specific parts of the tree_node */
extern struct obstack permanent_obstack;

void pop_c_function_context ();
void push_c_function_context ();

/* Nonzero means that the size of a type may vary
 * within one function context.
 */
int size_volatile = 0;

tree forward_pointer_list = NULL_TREE;

tree current_type_name = NULL_TREE;
int defining_methods = 0;
int defining_pointers = 0;
int defining_operators = 0;

#endif /* GPC */

/* In grokdeclarator, distinguish syntactic contexts of declarators.  */
enum decl_context
{ NORMAL,                       /* Ordinary declaration */
  FUNCDEF,                      /* Function definition */
  PARM,                         /* Declaration of parm before function body */
  FIELD,                        /* Declaration inside struct or union */
  BITFIELD,                     /* Likewise but with specified width */
  TYPENAME};                    /* Typename (inside cast or sizeof)  */

#ifndef CHAR_TYPE_SIZE
#define CHAR_TYPE_SIZE BITS_PER_UNIT
#endif

#ifndef SHORT_TYPE_SIZE
#define SHORT_TYPE_SIZE (BITS_PER_UNIT * MIN ((UNITS_PER_WORD + 1) / 2, 2))
#endif

#ifndef INT_TYPE_SIZE
#define INT_TYPE_SIZE BITS_PER_WORD
#endif

#ifndef LONG_TYPE_SIZE
#define LONG_TYPE_SIZE BITS_PER_WORD
#endif

#ifndef LONG_LONG_TYPE_SIZE
#define LONG_LONG_TYPE_SIZE (BITS_PER_WORD * 2)
#endif

#ifndef WCHAR_UNSIGNED
#define WCHAR_UNSIGNED 0
#endif

#ifndef FLOAT_TYPE_SIZE
#define FLOAT_TYPE_SIZE BITS_PER_WORD
#endif

#ifndef DOUBLE_TYPE_SIZE
#define DOUBLE_TYPE_SIZE (BITS_PER_WORD * 2)
#endif

#ifndef LONG_DOUBLE_TYPE_SIZE
#define LONG_DOUBLE_TYPE_SIZE (BITS_PER_WORD * 2)
#endif

/* We let tm.h override the types used here, to handle trivial differences
   such as the choice of unsigned int or long unsigned int for size_t.
   When machines start needing nontrivial differences in the size type,
   it would be best to do something here to figure out automatically
   from other information what type to use.  */

#ifndef SIZE_TYPE
#define SIZE_TYPE "long unsigned int"
#endif

#ifndef PTRDIFF_TYPE
#define PTRDIFF_TYPE "long int"
#endif

#ifndef WCHAR_TYPE
#define WCHAR_TYPE "int"
#endif

/* a node which has tree code ERROR_MARK, and whose type is itself.
   All erroneous expressions are replaced with this node.  All functions
   that accept nodes as arguments should avoid generating error messages
   if this node is one of the arguments, since it is undesirable to get
   multiple error messages from one error in the input.  */

tree error_mark_node;

/* INTEGER_TYPE and REAL_TYPE nodes for the standard data types */

tree short_integer_type_node;
tree integer_type_node;
tree long_integer_type_node;
tree long_long_integer_type_node;

tree short_unsigned_type_node;
tree unsigned_type_node;
tree long_unsigned_type_node;
tree long_long_unsigned_type_node;

tree boolean_type_node;
tree byte_boolean_type_node;
tree short_boolean_type_node;
tree word_boolean_type_node;
tree long_boolean_type_node;
tree long_long_boolean_type_node;
tree boolean_false_node;
tree boolean_true_node;

tree ptrdiff_type_node;

tree unsigned_char_type_node;
tree signed_char_type_node;
tree char_type_node;
tree wchar_type_node;
tree signed_wchar_type_node;
tree unsigned_wchar_type_node;

tree float_type_node;
tree double_type_node;
tree long_double_type_node;

tree complex_integer_type_node;
tree complex_float_type_node;
tree complex_double_type_node;
tree complex_long_double_type_node;

tree intQI_type_node;
tree intHI_type_node;
tree intSI_type_node;
tree intDI_type_node;

tree unsigned_intQI_type_node;
tree unsigned_intHI_type_node;
tree unsigned_intSI_type_node;
tree unsigned_intDI_type_node;

/* a VOID_TYPE node.  */

tree void_type_node;

/* Nodes for types `void *' and `const void *'.  */

tree ptr_type_node, const_ptr_type_node;

/* Nodes for types `char *' and `const char *'.  */

tree string_type_node, const_string_type_node;

/* Type `char[SOMENUMBER]'.
   Used when an array of char is needed and the size is irrelevant.  */

tree char_array_type_node;

/* Type `int[SOMENUMBER]' or something like it.
   Used when an array of int needed and the size is irrelevant.  */

tree int_array_type_node;

/* Type `wchar_t[SOMENUMBER]' or something like it.
   Used when a wide string literal is created.  */

tree wchar_array_type_node;

/* type `int ()' -- used for implicit declaration of functions.  */

tree default_function_type;

/* function types `double (double)' and `double (double, double)', etc.  */

tree double_ftype_double, double_ftype_double_double;
tree int_ftype_int, long_ftype_long;
tree float_ftype_float;
tree ldouble_ftype_ldouble;

/* Function type `void (void *, void *, int)' and similar ones */

tree void_ftype_ptr_ptr_int, int_ftype_ptr_ptr_int, void_ftype_ptr_int_int;

/* Function type `char *(char *, char *)' and similar ones */
tree string_ftype_ptr_ptr, int_ftype_string_string;

/* Function type `int (const void *, const void *, size_t)' */
tree int_ftype_cptr_cptr_sizet;

/* Two expressions that are constants with value zero.
   The first is of type `int', the second of type `void *'.  */

tree integer_zero_node;
tree null_pointer_node;

/* A node for the integer constant 1.  */

tree integer_one_node;

/* Nonzero if we have seen an invalid cross reference
   to a struct, union, or enum, but not yet printed the message.  */

tree pending_invalid_xref;
/* File and line to appear in the eventual error message.  */
char *pending_invalid_xref_file;
int pending_invalid_xref_line;

/* While defining an enum type, this is 1 plus the last enumerator
   constant value.  Note that will do not have to save this or `enum_overflow'
   around nested function definition since such a definition could only
   occur in an enum value expression and we don't use these variables in
   that case.  */

static tree enum_next_value;

/* Nonzero means that there was overflow computing enum_next_value.  */

static int enum_overflow;

#ifdef GPC

/* Even more integer types */

tree ptrsize_unsigned_type_node;
tree ptrsize_integer_type_node;
tree packed_array_unsigned_long_type_node;
tree packed_array_unsigned_short_type_node;

/* node for marking virtual methods while parsing */

tree virtual_mark_node;

/* name for implicit `Self' formal parameter */

tree self_name_node;

#if 0 /* unused */
/*
 * Chain of TREE_LIST nodes whose TREE_VALUE is the with_element
 * of one currently active WITH_STMT
 */
tree with_element_chain;
#endif /* 0 */

/*
 * The context of the Pascal main program before it really exists.
 */
tree main_program_context = NULL_TREE;

/* TREE_LIST of labels declared in the current function `label'
   statement in Pascal.

   This is a chain of TREE_LIST nodes;
    TREE_PURPOSE is the LABEL_DECL node
    TREE_VALUE   is the name of the label (IDENTIFIER_NODE)
 */
tree declared_labels;

/* Used in build_enumerator() and gpc-parse.y */
tree current_enum_type;


/* Known directives */
tree d_forward = NULL_TREE;
tree d_external = NULL_TREE;
tree d_extern = NULL_TREE;
tree d_c_lang = NULL_TREE;
tree d_c_lang1 = NULL_TREE;
tree d_asmname = NULL_TREE;
tree d_attribute = NULL_TREE;
tree d_o_override = NULL_TREE;
tree d_static = NULL_TREE;
#endif /* GPC */

/* Parsing a function declarator leaves a list of parameter names
   or a chain or parameter decls here.  */

static tree last_function_parms;

/* Parsing a function declarator leaves here a chain of structure
   and enum types declared in the parmlist.  */

static tree last_function_parm_tags;

/* After parsing the declarator that starts a function definition,
   `start_function' puts here the list of parameter names or chain of decls.
   `store_parm_decls' finds it here.  */

static tree current_function_parms;

/* Similar, for last_function_parm_tags.  */
static tree current_function_parm_tags;

/* Similar, for the file and line that the prototype came from if this is
   an old-style definition.  */
static char *current_function_prototype_file;
static int current_function_prototype_line;

/* A list (chain of TREE_LIST nodes) of all LABEL_DECLs in the function
   that have names.  Here so we can clear out their names' definitions
   at the end of the function.  */

static tree named_labels;

/* A list of LABEL_DECLs from outer contexts that are currently shadowed.  */

static tree shadowed_labels;

/* Nonzero when store_parm_decls is called indicates a varargs function.
   Value not meaningful after store_parm_decls.  */

static int c_function_varargs;

/* The FUNCTION_DECL for the function currently being compiled,
   or 0 if between functions.  */
tree current_function_decl;

/* Set to 0 at beginning of a function definition, set to 1 if
   a return statement that specifies a return value is seen.  */

int current_function_returns_value;

/* Set to 0 at beginning of a function definition, set to 1 if
   a return statement with no argument is seen.  */

int current_function_returns_null;

#ifdef GPC

/* VAR_DECL node for the function's return value variable,
 * or NULL_TREE in a Procedure.
 */
tree current_function_return_variable;

#endif /* GPC */

/* Set to nonzero by `grokdeclarator' for a function
   whose return type is defaulted, if warnings for this are desired.  */

static int warn_about_return_type;

/* Nonzero when starting a function declared `extern inline'.  */

static int current_extern_inline;

/* For each binding contour we allocate a binding_level structure
 * which records the names defined in that contour.
 * Contours include:
 *  0) the global one
 *  1) one for each function definition,
 *     where internal declarations of the parameters appear.
 *  2) one for each compound statement,
 *     to record its declarations.
 *
 * The current meaning of a name can be found by searching the levels from
 * the current one out to the global one.
 */

/* Note that the information in the `names' component of the global contour
   is duplicated in the IDENTIFIER_GLOBAL_VALUEs of all identifiers.  */

struct binding_level
  {
    /* A chain of _DECL nodes for all variables, constants, functions,
       and typedef types.  These are in the reverse of the order supplied.
     */
    tree names;

    /* A list of structure, union and enum definitions,
     * for looking up tag names.
     * It is a chain of TREE_LIST nodes, each of whose TREE_PURPOSE is a name,
     * or NULL_TREE; and whose TREE_VALUE is a RECORD_TYPE, UNION_TYPE,
     * or ENUMERAL_TYPE node.
     */
    tree tags;

    /* For each level, a list of shadowed outer-level local definitions
       to be restored when this level is popped.
       Each link is a TREE_LIST whose TREE_PURPOSE is an identifier and
       whose TREE_VALUE is its old definition (a kind of ..._DECL node).  */
    tree shadowed;

    /* For each level (except not the global one),
       a chain of BLOCK nodes for all the levels
       that were entered and exited one level down.  */
    tree blocks;

    /* The BLOCK node for this level, if one has been preallocated.
       If 0, the BLOCK is allocated (if needed) when the level is popped.  */
    tree this_block;

    /* The binding level which this one is contained in (inherits from).  */
    struct binding_level *level_chain;

    /* Nonzero for the level that holds the parameters of a function.  */
    char parm_flag;

    /* Nonzero if this level "doesn't exist" for tags.  */
    char tag_transparent;

    /* Nonzero if sublevels of this level "don't exist" for tags.
       This is set in the parm level of a function definition
       while reading the function body, so that the outermost block
       of the function body will be tag-transparent.  */
    char subblocks_tag_transparent;

    /* Nonzero means make a BLOCK for this level regardless of all else.  */
    char keep;

    /* Nonzero means make a BLOCK if this level has any subblocks.  */
    char keep_if_subblocks;

    /* Number of decls in `names' that have incomplete
       structure or union types.  */
    int n_incomplete;

    /* A list of decls giving the (reversed) specified order of parms,
       not including any forward-decls in the parmlist.
       This is so we can put the parms in proper order for assign_parms.  */
    tree parm_order;

#ifdef GPC
    /* Chain of the labels declared with the `label' statement
       at this level */
    /* push_binding_level & pop_binding_level use this */
    tree declared_labels;
#endif /* GPC */
  };

#define NULL_BINDING_LEVEL (struct binding_level *) NULL

/* The binding level currently in effect.  */

static struct binding_level *current_binding_level;

/* A chain of binding_level structures awaiting reuse.  */

static struct binding_level *free_binding_level;

/* The outermost binding level, for names of file scope.
   This is created when the compiler is started and exists
   through the entire run.  */

static struct binding_level *global_binding_level;

/* Binding level structures are initialized by copying this one.  */

static struct binding_level clear_binding_level
  = {NULL, NULL, NULL, NULL, NULL, NULL_BINDING_LEVEL, 0, 0, 0, 0, 0, 0,
#ifdef GPC
     NULL, NULL};
#else
     NULL};
#endif /* GPC */

/* Nonzero means unconditionally make a BLOCK for the next level pushed.  */

static int keep_next_level_flag;

/* Nonzero means make a BLOCK for the next level pushed
   if it has subblocks.  */

static int keep_next_if_subblocks;

/* The chain of outer levels of label scopes.
   This uses the same data structure used for binding levels,
   but it works differently: each link in the chain records
   saved values of named_labels and shadowed_labels for
   a label binding level outside the current one.  */

static struct binding_level *label_level_chain;

/* Functions called automatically at the beginning and end of execution.  */

tree static_ctors, static_dtors;

/* Forward declarations.  */

static struct binding_level * make_binding_level        PROTO((void));
static void clear_limbo_values          PROTO((tree));
static int duplicate_decls              PROTO((tree, tree, int));
static char *redeclaration_error_message PROTO((tree, tree));
static void storedecls                  PROTO((tree));
static void storetags                   PROTO((tree));
static tree lookup_tag                  PROTO((enum tree_code, tree,
                                               struct binding_level *, int));
static tree lookup_tag_reverse          PROTO((tree));
static tree grokdeclarator              PROTO((tree, tree, enum decl_context,
                                               int));
static tree grokparms                   PROTO((tree, int));
#ifndef GPC
static int field_decl_cmp               PROTO((const GENERIC_PTR, const GENERIC_PTR));
#endif
static void layout_array_type           PROTO((tree));

/* C-specific option variables.  */

/* Nonzero means allow type mismatches in conditional expressions;
   just make their values `void'.   */

int flag_cond_mismatch;

/* Nonzero means give `double' the same size as `float'.  */

int flag_short_double;

/* Nonzero means don't recognize the keyword `asm'.  */

int flag_no_asm;

/* Nonzero means don't recognize any builtin functions.  */

#ifdef GPC
int flag_no_builtin = 1;
#else
int flag_no_builtin;
#endif

/* Nonzero means don't recognize the non-ANSI builtin functions.
   -ansi sets this.  */

int flag_no_nonansi_builtin;

/* Nonzero means do some things the same way PCC does.  */

int flag_traditional;

/* Nonzero means that we have builtin functions, and main is an int */

int flag_hosted = 1;

/* Nonzero means to allow single precision math even if we're generally
   being traditional.  */
int flag_allow_single_precision = 0;

/* Nonzero means to treat bitfields as signed unless they say `unsigned'.  */

int flag_signed_bitfields = 1;
int explicit_flag_signed_bitfields = 0;

/* Nonzero means handle `#ident' directives.  0 means ignore them.  */

#ifdef EGCS92
extern int flag_no_ident;
#else
int flag_no_ident = 0;
#endif

/* Nonzero means warn about use of implicit int. */

int warn_implicit_int;

/* Nonzero means message about use of implicit function declarations;
 1 means warning; 2 means error. */

int mesg_implicit_function_declaration;

/* Nonzero means give string constants the type `const char *'
   to get extra warnings from them.  These warnings will be too numerous
   to be useful, except in thoroughly ANSIfied programs.  */

int warn_write_strings;

/* Nonzero means warn about pointer casts that can drop a type qualifier
   from the pointer target type.  */

int warn_cast_qual;

/* Nonzero means warn when casting a function call to a type that does
   not match the return type (e.g. (float)sqrt() or (anything*)malloc()
   when there is no previous declaration of sqrt or malloc.  */

int warn_bad_function_cast;

/* Warn about traditional constructs whose meanings changed in ANSI C.  */

int warn_traditional;

/* Nonzero means warn about sizeof(function) or addition/subtraction
   of function pointers.  */

int warn_pointer_arith;

/* Nonzero means warn for non-prototype function decls
   or non-prototyped defs without previous prototype.  */

int warn_strict_prototypes;

/* Nonzero means warn for any global function def
   without separate previous prototype decl.  */

int warn_missing_prototypes;

/* Nonzero means warn for any global function def
   without separate previous decl.  */

/* The `bounds' stuff below is only needed to compile the EMX version.
 * But it could become interesting when implementing bounds checking
 * into GNU Pascal in the future. -- PG
 */

/* Bounds checking is turned off temporarily inside static initializers by
 * c-typeck.c:start_init using the following flag.      (RWMJ)
 */

int bounds_in_static_decl;

/* Pop level if it was pushed in bounds_build_args ().  (RWMJ)
 */

int bounds_poplevel = 0;

int warn_missing_declarations;

/* Nonzero means warn about multiple (redundant) decls for the same single
   variable or function.  */

int warn_redundant_decls = 0;

/* Nonzero means warn about extern declarations of objects not at
   file-scope level and about *all* declarations of functions (whether
   extern or static) not at file-scope level.  Note that we exclude
   implicit function declarations.  To get warnings about those, use
   -Wimplicit.  */

int warn_nested_externs = 0;

/* Warn about *printf or *scanf format/argument anomalies.  */

int warn_format;

/* Warn about a subscript that has type char.  */

int warn_char_subscripts = 0;

/* Warn if a type conversion is done that might have confusing results.  */

int warn_conversion;

/* Warn if adding () is suggested.  */

int warn_parentheses;

/* Warn if initializer is not completely bracketed.  */

int warn_missing_braces;

/* Warn if main is suspicious.  */

int warn_main;

#ifdef EGCS

/* Warn about #pragma directives that are not recognised.  */

int warn_unknown_pragmas = 0; /* Tri state variable.  */

#endif

/* Warn about comparison of signed and unsigned values.
   If -1, neither -Wsign-compare nor -Wno-sign-compare has been specified.  */

int warn_sign_compare = -1;

/* Nonzero means `$' can be in an identifier.  */

#ifndef DOLLARS_IN_IDENTIFIERS
#ifdef GPC
#define DOLLARS_IN_IDENTIFIERS 0
#else
#define DOLLARS_IN_IDENTIFIERS 1
#endif
#endif
int dollars_in_ident = DOLLARS_IN_IDENTIFIERS;

#ifdef GPC

/* Pascal-specific flags */

/* Nonzero means allow mixed comments;
 * then (* } and { *) are okay, according to ISO standard.
 */
int flag_mixed_comments = 0;

/* Nonzero means allow nested comments;
 * then (* (* *) *) and { { } } are okay (optional GPC extension).
 */
int flag_nested_comments = 0;

/* Nonzero means allow Delphi comments.
 */
int flag_delphi_comments = 1;

/* Nonzero allows GPC to ignore function results without warning.
 */
int flag_ignore_function_results = 0;

/* Nonzero means allow GPC to read Borland-style
 * `#' and `^' character constants
 */
int flag_borland_char_constants = 1;

/* Nonzero means to allow string clipping: "write ( 'foobar' : 3 )" --> "foo".
 */
int flag_clip_strings = 1;

/* Nonzero means to allow string truncation: "writestr ( s3, 'foobar' )"
 * with `s3' having a capacity of 3 --> "foo".  Zero means to produce a
 * runtime error.
 */
int flag_truncate_strings = 1;

/* Nonzero means to do exact (lexicographic) string comparisons with
 * operators like `<'.  Zero means to pad with spaces as ISO requires.
 */
int flag_exact_compare_strings = 1;

/* Nonzero means to produce a blank in front of positive reals.
 * (Required by ISO.)
 */
int flag_real_blank = 1;

/* Nonzero means to write exponents with a capital 'E'.
 * Zero means to write a lowercase 'e'.
 */
int flag_capital_exponent = 0;

/* Nonzero means to derive the external file name from
 * the name of the file variable.
 */
int flag_transparent_file_names = 0;

/* Nonzero means the result of the address is a typed pointer.
 */
int flag_typed_address = 1;

/* Generate LINE symbols to the assembler output.
 * for GPC traceback feature.  Not implemented.
 */
int flag_debug_lines = 0;

/* What kind of Pascal are we compiling?
 */
unsigned long flag_what_pascal = 0;

/* Nonzero means don't warn about "extended syntax"
 * such as a function called as a procedure.
 */
int flag_extended_syntax = 0;

/* Use short circuit operators with standard AND and OR.
 * This is necessary when compiling some "portable" programs
 * like TeX ...
 */
int flag_short_circuit = 1;

/* Nonzero means to check for IOresult after each I/O operation.
 */
int flag_io_checking = 1;

/* Nonzero means to do range checking on `readln' and such.
 */
int flag_input_range_checking = 0;

/* If you really wish to use signed chars ...
 */
int force_signed_chars = 0;

#endif /* GPC */

static char *do_option PROTO((char *));

static char *
do_option (p)
     char *p;
{
  if (*p == ':' || *p == '=')
    p++;
  if (*p == '"' || *p == '\'')
    {
      char e = *p, *q;
      p++;
      q = p;
      while (*q && *q != e)
        q++;
      *q = 0;
    }
  return p;
}

/* Decode the string P as a language-specific option for C.
   Return the number of strings consumed.  */

int
#ifdef EGCS
c_decode_option (argc, argv)
     int argc;
     char **argv;
#else /* not EGCS */
c_decode_option (p)
     char *p;
#endif /* not EGCS */
{
#ifdef GPC
  int temp;
#endif /* GPC */
#ifdef EGCS
  char *p = argv[0];
#endif /* EGCS */
  const struct lang_option_map *map = lang_option_map;
  while (*map->src)
    {
      char **src = map->src;
      while (*src && strcmp (p, *src) != 0)
        src++;
      if (*src)
        {
          char **dest = map->dest;
          while (*dest)
#ifdef EGCS
            c_decode_option (1, dest++);
#else
            c_decode_option (*dest++);
#endif
        }
      map++;
    }
  if (!strcmp (p, "-ftraditional") || !strcmp (p, "-traditional"))
    {
      flag_traditional = 1;
      flag_writable_strings = 1;
    }
  else if (!strcmp (p, "-fallow-single-precision"))
    flag_allow_single_precision = 1;
  else if (!strcmp (p, "-fhosted") || !strcmp (p, "-fno-freestanding"))
    {
      flag_hosted = 1;
      flag_no_builtin = 0;
    }
  else if (!strcmp (p, "-ffreestanding") || !strcmp (p, "-fno-hosted"))
    {
      flag_hosted = 0;
      flag_no_builtin = 1;
      /* warn_main will be 2 if set by -Wall, 1 if set by -Wmain */
      if (warn_main == 2)
        warn_main = 0;
    }
  else if (!strcmp (p, "-fnotraditional") || !strcmp (p, "-fno-traditional"))
    {
      flag_traditional = 0;
      flag_writable_strings = 0;
    }
  else if (!strcmp (p, "-fdollars-in-identifiers"))
    dollars_in_ident = 1;
  else if (!strcmp (p, "-fno-dollars-in-identifiers"))
    dollars_in_ident = 0;
#ifndef GPC
  else if (!strcmp (p, "-fsigned-char"))
    flag_signed_char = 1;
  else if (!strcmp (p, "-funsigned-char"))
    flag_signed_char = 0;
  else if (!strcmp (p, "-fno-signed-char"))
    flag_signed_char = 0;
  else if (!strcmp (p, "-fno-unsigned-char"))
    flag_signed_char = 1;
#endif /* not GPC */
  else if (!strcmp (p, "-fsigned-bitfields")
           || !strcmp (p, "-fno-unsigned-bitfields"))
    {
      flag_signed_bitfields = 1;
      explicit_flag_signed_bitfields = 1;
    }
  else if (!strcmp (p, "-funsigned-bitfields")
           || !strcmp (p, "-fno-signed-bitfields"))
    {
      flag_signed_bitfields = 0;
      explicit_flag_signed_bitfields = 1;
    }
  else if (!strcmp (p, "-fshort-enums"))
    flag_short_enums = 1;
  else if (!strcmp (p, "-fno-short-enums"))
    flag_short_enums = 0;
  else if (!strcmp (p, "-fcond-mismatch"))
    flag_cond_mismatch = 1;
  else if (!strcmp (p, "-fno-cond-mismatch"))
    flag_cond_mismatch = 0;
  else if (!strcmp (p, "-fshort-double"))
    flag_short_double = 1;
  else if (!strcmp (p, "-fno-short-double"))
    flag_short_double = 0;
  else if (!strcmp (p, "-fasm"))
    flag_no_asm = 0;
  else if (!strcmp (p, "-fno-asm"))
    flag_no_asm = 1;
  else if (!strcmp (p, "-fbuiltin"))
    flag_no_builtin = 0;
  else if (!strcmp (p, "-fno-builtin"))
    flag_no_builtin = 1;
  else if (!strcmp (p, "-fno-ident"))
    flag_no_ident = 1;
  else if (!strcmp (p, "-fident"))
    flag_no_ident = 0;
  else if (!strcmp (p, "-ansi"))
    flag_no_asm = 1, flag_no_nonansi_builtin = 1;
  else if (!strcmp (p, "-Werror-implicit-function-declaration"))
    mesg_implicit_function_declaration = 2;
  else if (!strcmp (p, "-Wimplicit-function-declaration"))
    mesg_implicit_function_declaration = 1;
  else if (!strcmp (p, "-Wno-implicit-function-declaration"))
    mesg_implicit_function_declaration = 0;
  else if (!strcmp (p, "-Wimplicit-int"))
    warn_implicit_int = 1;
  else if (!strcmp (p, "-Wno-implicit-int"))
    warn_implicit_int = 0;
  else if (!strcmp (p, "-Wimplicit"))
    {
      warn_implicit_int = 1;
      if (mesg_implicit_function_declaration != 2)
        mesg_implicit_function_declaration = 1;
    }
  else if (!strcmp (p, "-Wno-implicit"))
    warn_implicit_int = 0, mesg_implicit_function_declaration = 0;
  else if (!strcmp (p, "-Wwrite-strings"))
    warn_write_strings = 1;
  else if (!strcmp (p, "-Wno-write-strings"))
    warn_write_strings = 0;
  else if (!strcmp (p, "-Wcast-qual"))
    warn_cast_qual = 1;
  else if (!strcmp (p, "-Wno-cast-qual"))
    warn_cast_qual = 0;
  else if (!strcmp (p, "-Wbad-function-cast"))
    warn_bad_function_cast = 1;
  else if (!strcmp (p, "-Wno-bad-function-cast"))
    warn_bad_function_cast = 0;
  else if (!strcmp (p, "-Wpointer-arith"))
    warn_pointer_arith = 1;
  else if (!strcmp (p, "-Wno-pointer-arith"))
    warn_pointer_arith = 0;
  else if (!strcmp (p, "-Wstrict-prototypes"))
    warn_strict_prototypes = 1;
  else if (!strcmp (p, "-Wno-strict-prototypes"))
    warn_strict_prototypes = 0;
  else if (!strcmp (p, "-Wmissing-prototypes"))
    warn_missing_prototypes = 1;
  else if (!strcmp (p, "-Wno-missing-prototypes"))
    warn_missing_prototypes = 0;
  else if (!strcmp (p, "-Wmissing-declarations"))
    warn_missing_declarations = 1;
  else if (!strcmp (p, "-Wno-missing-declarations"))
    warn_missing_declarations = 0;
  else if (!strcmp (p, "-Wredundant-decls"))
    warn_redundant_decls = 1;
  else if (!strcmp (p, "-Wno-redundant-decls"))
    warn_redundant_decls = 0;
  else if (!strcmp (p, "-Wnested-externs"))
    warn_nested_externs = 1;
  else if (!strcmp (p, "-Wno-nested-externs"))
    warn_nested_externs = 0;
  else if (!strcmp (p, "-Wtraditional"))
    warn_traditional = 1;
  else if (!strcmp (p, "-Wno-traditional"))
    warn_traditional = 0;
  else if (!strcmp (p, "-Wformat"))
    warn_format = 1;
  else if (!strcmp (p, "-Wno-format"))
    warn_format = 0;
  else if (!strcmp (p, "-Wchar-subscripts"))
    warn_char_subscripts = 1;
  else if (!strcmp (p, "-Wno-char-subscripts"))
    warn_char_subscripts = 0;
  else if (!strcmp (p, "-Wconversion"))
    warn_conversion = 1;
  else if (!strcmp (p, "-Wno-conversion"))
    warn_conversion = 0;
  else if (!strcmp (p, "-Wparentheses"))
    warn_parentheses = 1;
  else if (!strcmp (p, "-Wno-parentheses"))
    warn_parentheses = 0;
  else if (!strcmp (p, "-Wreturn-type"))
    warn_return_type = 1;
  else if (!strcmp (p, "-Wno-return-type"))
    warn_return_type = 0;
  else if (!strcmp (p, "-Wcomment"))
    ; /* cpp handles this one.  */
  else if (!strcmp (p, "-Wno-comment"))
    ; /* cpp handles this one.  */
  else if (!strcmp (p, "-Wcomments"))
    ; /* cpp handles this one.  */
  else if (!strcmp (p, "-Wno-comments"))
    ; /* cpp handles this one.  */
  else if (!strcmp (p, "-Wtrigraphs"))
    ; /* cpp handles this one.  */
  else if (!strcmp (p, "-Wno-trigraphs"))
    ; /* cpp handles this one.  */
  else if (!strcmp (p, "-Wundef"))
    ; /* cpp handles this one.  */
  else if (!strcmp (p, "-Wno-undef"))
    ; /* cpp handles this one.  */
  else if (!strcmp (p, "-Wimport"))
    ; /* cpp handles this one.  */
  else if (!strcmp (p, "-Wno-import"))
    ; /* cpp handles this one.  */
  else if (!strcmp (p, "-Wmissing-braces"))
    warn_missing_braces = 1;
  else if (!strcmp (p, "-Wno-missing-braces"))
    warn_missing_braces = 0;
  else if (!strcmp (p, "-Wmain"))
    warn_main = 1;
  else if (!strcmp (p, "-Wno-main"))
    warn_main = 0;
  else if (!strcmp (p, "-Wsign-compare"))
    warn_sign_compare = 1;
  else if (!strcmp (p, "-Wno-sign-compare"))
    warn_sign_compare = 0;
  else if (!strcmp (p, "-Wall"))
    {
      /* We save the value of warn_uninitialized, since if they put
         -Wuninitialized on the command line, we need to generate a
         warning about not using it without also specifying -O.  */
      if (warn_uninitialized != 1)
        warn_uninitialized = 2;
      warn_unused = 1;
      warn_switch = 1;
      /* We set this to 2 here, but 1 in -Wmain, so -ffreestanding can turn
         it off only if it's not explicit.  */
      warn_main = 2;
    }
#ifdef GPC
  else if (!strcmp (p, "-fstandard-pascal-level-0"))
    {
      flag_what_pascal |= STANDARD_PASCAL_LEVEL_0;
      if (lineno)  /* Not at startup */
        {
          init_language_specification ();
          declare_known_ids ();
        }
    }
  else if (!strcmp (p, "-fstandard-pascal"))
    {
      flag_what_pascal |= STANDARD_PASCAL_LEVEL_1;
      if (lineno)  /* Not at startup */
        {
          init_language_specification ();
          declare_known_ids ();
        }
    }
  else if (!strcmp (p, "-fextended-pascal"))
    {
      flag_what_pascal |= EXTENDED_PASCAL;
      if (lineno)  /* Not at startup */
        {
          init_language_specification ();
          declare_known_ids ();
        }
    }
  else if (!strcmp (p, "-fobject-pascal"))
    {
      flag_what_pascal |= OBJECT_PASCAL;
      if (lineno)  /* Not at startup */
        {
          init_language_specification ();
          declare_known_ids ();
        }
    }
  else if (!strcmp (p, "-fborland-pascal"))
    {
      flag_what_pascal |= BORLAND_PASCAL;
      if (lineno)  /* Not at startup */
        {
          init_language_specification ();
          declare_known_ids ();
        }
    }
  else if (!strcmp (p, "-fdelphi"))
    {
      flag_what_pascal |= BORLAND_DELPHI;
      if (lineno)  /* Not at startup */
        {
          init_language_specification ();
          declare_known_ids ();
        }
    }
  else if (!strcmp (p, "-fpascal-sc"))
    {
      flag_what_pascal |= PASCAL_SC;
      if (lineno)  /* Not at startup */
        {
          init_language_specification ();
          declare_known_ids ();
        }
    }
  else if (!strcmp (p, "-fgnu-pascal"))
    {
      flag_what_pascal = 0;
      int_out_width = INT_OUT_WIDTH;
      real_out_width = REAL_OUT_WIDTH;
      bool_out_width = BOOL_OUT_WIDTH;
      long_int_out_width = LONG_INT_OUT_WIDTH;
      long_real_out_width = LONG_REAL_OUT_WIDTH;
      if (lineno)  /* Not at startup */
        {
          init_language_specification ();
          declare_known_ids ();
        }
    }
  else if (!strcmp (p, "-flines"))
    flag_debug_lines = 1;
  else if (!strncmp (p, "-fdebug-tree", temp = strlen ("-fdebug-tree")))
    {
      p = do_option (p + temp);
      if (*p)
        {
          tree t;
          if (strncmp (p, "0x", 2) == 0)
            t = (tree) strtol (p, NULL_PTR, 16);
          else
            t = get_identifier (p);
          if (t)
            debug_tree (t);
          else
            fprintf (stderr, "NULL_TREE\n");
        }
    }
  else if (!strcmp (p, "-fdebug-gpi"))
    flag_debug_gpi = 1;
  else if (!strcmp (p, "-fdebug-source"))
    flag_debug_source = 1;
  else if (!strcmp (p, "-fprogress-messages"))
    flag_progress_messages = 1;
  else if (!strcmp (p, "-fprogress-bar"))
    flag_progress_bar = 1;
  else if (!strncmp (p, "-fautolink", temp = strlen ("-fautolink")))
    {
      p += temp;
      if (*p)
        {
          warning ("ignoring AutoLink options `%s';", p + 1);
          if (automake_gpc_options)
            warning (" (using `%s')", automake_gpc_options);
        }
      if (flag_automake < 1)
        flag_automake = 1;
    }
  else if (!strcmp (p, "-fno-autolink"))
    flag_automake = 0;
  else if (!strncmp (p, "-fautomake", temp = strlen ("-fautomake")))
    {
      p += temp;
      if (*p)
        {
          warning ("ignoring AutoMake options `%s';", p + 1);
          if (automake_gpc_options)
            warning (" (using `%s')", automake_gpc_options);
        }
      if (flag_automake < 2)
        flag_automake = 2;
    }
  else if (!strcmp (p, "-fno-automake"))
    flag_automake = 0;
  else if (!strncmp (p, "-fautobuild", temp = strlen ("-fautobuild")))
    {
      p += temp;
      if (*p)
        {
          warning ("ignoring AutoBuild options `%s';", p + 1);
          if (automake_gpc_options)
            warning (" (using `%s')", automake_gpc_options);
        }
      flag_automake = 3;
    }
  else if (!strcmp (p, "-fno-autobuild"))
    flag_automake = 0;
  else if (!strncmp (p, "-famtmpfile", temp = strlen ("-famtmpfile")))
    {
      /* This information is passed by the
       * gcc/gpc program, not by the user.
       */
      p += temp;
      if (*p == ':' || *p == '=')
        p++;
      if (*p)
        automake_temp_filename = p;
      if (automake_temp_filename)
        {
          /* Extract the `gpc' command line options from the automake
           * temp file and use them to initialize automake_gpc_options.
           */
          FILE *automake_temp_file = fopen (automake_temp_filename, "rt");
          if (!automake_temp_file)
            fatal ("cannot open automake temp file `%s'", automake_temp_filename);
          while (! feof (automake_temp_file))
            {
              char s[10240];
              if (fgets (s, 10240, automake_temp_file))
                {
                  if (strncmp (s, "#gpc: ", 6) == 0)
                    {
                      int l = strlen (s) - 1;
                      if (s[l] == '\n')
                        s[l] = 0;
                      if (automake_gpc)
                        free (automake_gpc);
                      automake_gpc = save_string (s + 6);
                    }
                  else if (strncmp (s, "#cmdline: ", 10) == 0)
                    {
                      int l = strlen (s) - 1;
                      if (s[l] == '\n')
                        s[l] = 0;
                      add_automake_gpc_options (s + 10);
                    }
                }
            }
          fclose (automake_temp_file);
        }
    }
  else if (!strcmp (p, "-fextended-syntax"))
    {
      flag_extended_syntax = flag_ignore_function_results = 1;
      if (pedantic)
        warning ("\"extended syntax\" annullates parts of \"pedantic\"");
    }
  else if (!strcmp (p, "-fno-extended-syntax"))
    flag_extended_syntax = flag_ignore_function_results = 0;
  else if (!strcmp (p, "-fshort-circuit"))
    flag_short_circuit = 1;
  else if (!strcmp (p, "-fno-short-circuit"))
    flag_short_circuit = 0;
  else if (!strcmp (p, "-fmixed-comments"))
    flag_mixed_comments = 1;
  else if (!strcmp (p, "-fno-mixed-comments"))
    flag_mixed_comments = 0;
  else if (!strcmp (p, "-fnested-comments"))
    flag_nested_comments = 1;
  else if (!strcmp (p, "-fno-nested-comments"))
    flag_nested_comments = 0;
  else if (!strcmp (p, "-fdelphi-comments"))
    flag_delphi_comments = 1;
  else if (!strcmp (p, "-fno-delphi-comments"))
    flag_delphi_comments = 0;
  else if (!strcmp (p, "-fborland-char-constants"))
    flag_borland_char_constants = 1;
  else if (!strcmp (p, "-fno-borland-char-constants"))
    flag_borland_char_constants = 0;
  else if (!strcmp (p, "-fignore-function-results"))
    flag_ignore_function_results = 1;
  else if (!strcmp (p, "-fno-ignore-function-results"))
    flag_ignore_function_results = 0;
  else if (!strcmp (p, "-ftruncate-strings"))
    flag_truncate_strings = 1;
  else if (!strcmp (p, "-fno-truncate-strings"))
    flag_truncate_strings = 0;
  else if (!strcmp (p, "-fexact-compare-strings"))
    flag_exact_compare_strings = 1;
  else if (!strcmp (p, "-fno-exact-compare-strings"))
    flag_exact_compare_strings = 0;
  else if (!strcmp (p, "-fio-checking"))
    flag_io_checking = 1;
  else if (!strcmp (p, "-fno-io-checking"))
    flag_io_checking = 0;
  else if (!strcmp (p, "-fwrite-clip-strings"))
    flag_clip_strings = 1;
  else if (!strcmp (p, "-fno-write-clip-strings"))
    flag_clip_strings = 0;
  else if (!strcmp (p, "-fwrite-real-blank"))
    flag_real_blank = 1;
  else if (!strcmp (p, "-fno-write-real-blank"))
    flag_real_blank = 0;
  else if (!strcmp (p, "-fwrite-capital-exponent"))
    flag_capital_exponent = 1;
  else if (!strcmp (p, "-fno-write-capital-exponent"))
    flag_capital_exponent = 0;
  else if (!strcmp (p, "-ftransparent-file-names"))
    flag_transparent_file_names = 1;
  else if (!strcmp (p, "-fno-transparent-file-names"))
    flag_transparent_file_names = 0;
  else if (!strncmp (p, "-ffield-widths", temp = strlen ("-ffield-widths")))
    {
      p += temp;
      if (*p == ':' || *p == '=')
        p++;
      if (*p == 0)
        {
          int_out_width = strlen ("-2147483647");
          real_out_width = strlen ("-3.141592653589793e+000");
          bool_out_width = strlen ("-False");
          long_int_out_width = strlen ("-18446744073709551615");
          long_real_out_width = strlen ("-3.14159265358979323846e+0000");
        }
      else
        {
          /* Parse numeric values given for int,real,bool_out_width.
           */
          char *q = p;
          int finish;
          while (*p && *p != ',')
            p++;
          finish = (*p != ',');
          *p = 0;
          int_out_width = atoi (q);
          if (finish)
            {
              real_out_width = strlen ("-3.141592653589793e+000");
              bool_out_width = strlen ("-False");
              long_int_out_width = strlen ("-18446744073709551615");
              long_real_out_width = strlen ("-3.14159265358979323846e+0000");
            }
          else
            {
              p++;
              q = p;
              while (*p && *p != ',')
                p++;
              finish = (*p != ',');
              *p = 0;
              real_out_width = atoi (q);
              if (finish)
                {
                  bool_out_width = strlen ("-False");
                  long_int_out_width = strlen ("-18446744073709551615");
                  long_real_out_width = strlen ("-3.14159265358979323846e+0000");
                }
              else
                {
                  p++;
                  q = p;
                  while (*p && *p != ',')
                    p++;
                  finish = (*p != ',');
                  *p = 0;
                  bool_out_width = atoi (q);
                  if (finish)
                    {
                      long_int_out_width = strlen ("-18446744073709551615");
                      long_real_out_width = strlen ("-3.14159265358979323846e+0000");
                    }
                  else
                    {
                      p++;
                      q = p;
                      while (*p && *p != ',')
                        p++;
                      finish = (*p != ',');
                      *p = 0;
                      long_int_out_width = atoi (q);
                      if (finish)
                        long_real_out_width = strlen ("-3.14159265358979323846e+0000");
                      else
                        {
                          p++;
                          q = p;
                          while (*p && *p != ',')
                            p++;
                          *p = 0;
                          long_real_out_width = atoi (q);
                        }
                    }
                }
            }
        }
    }
  else if (!strcmp (p, "-fno-field-widths"))
    {
      int_out_width = 0;
      real_out_width = 0;
      bool_out_width = 0;
      long_int_out_width = 0;
      long_real_out_width = 0;
    }
  else if (!strcmp (p, "-fpedantic"))
    {
      pedantic = 1;
      if (flag_extended_syntax)
        warning ("\"extended syntax\" annullates parts of \"pedantic\"");
    }
  else if (!strcmp (p, "-fno-pedantic"))
    pedantic = 0;
  else if (!strcmp (p, "-fstack-checking"))
    flag_stack_check = 1;
  else if (!strcmp (p, "-fno-stack-checking"))
    flag_stack_check = 0;
  else if (!strcmp (p, "-ftyped-address"))
    flag_typed_address = 1;
  else if (!strcmp (p, "-fno-typed-address"))
    flag_typed_address = 0;
  else if (!strncmp (p, "-fsetlimit", temp = strlen ("-fsetlimit")))
    {
      p += temp;
      if (*p == ':' || *p == '=')
        p++;
      temp = atoi (p);
      if (temp > 0)
        requested_set_size = temp;
      else
        warning ("%s specified illegal set limit. Using default %d", p, DEFAULT_SET_SIZE);
    }
  else if (!strncmp (p, "-fgpc-main", temp = strlen ("-fgpc-main")))
    {
      p = do_option (p + temp);
      gpc_main = p;
      if (*p == 0)
        error ("empty `gpc-main' name");
      else
        {
          while (*p)
            {
              if (((*p >= 'A') && (*p <= 'Z'))
                  || ((*p >= 'a') && (*p <= 'z'))
                  || ((*p >= '0') && (*p <= '9'))
                  || (*p == '_')
                  || (*p == '$')
                  || (*p == '.'))
                p++;
              else
                {
                  error ("illegal character in `gpc-main' name");
                  break;
                }
            }
        }
    }
  else if (!strcmp (p, "-finterface-only"))
    {
      flag_interface_only = 1;
    }
  else if (!strcmp (p, "-fimplementation-only"))
    {
      flag_implementation_only = 1;
      flag_automake = 0;
    }
  else if (!strncmp (p, "-fexecutable-file-name", temp = strlen ("-fexecutable-file-name")))
    {
      p += temp;
      if (*p == ':' || *p == '=')
        p++;

      /* Having an empty string here indicates that we want to derive the
       * file name of the executable from the name of the file carrying the
       * `Program' headline.
       */
      executable_file_name = p;

      /* If the `Program' headline is gone already, store it now.
       */
      if (main_program_name)
        store_executable_name ();
    }
  else if (!strncmp (p, "-funit-path", temp = strlen ("-funit-path")))
    {
      p = do_option (p + temp);
      if (*p)
        {
          if (unit_path)
            {
              static char path_separator[2] = {PATH_SEPARATOR, 0};
              char *new_unit_path = concat (p, path_separator,
                                            unit_path, NULL_PTR);
              free (unit_path);
              unit_path = new_unit_path;
            }
          else
            unit_path = save_string (p);
        }
    }
  else if (!strcmp (p, "-fno-unit-path"))
    {
      if (unit_path)
        free (unit_path);
      unit_path = NULL_PTR;
    }
  else if (!strncmp (p, "-fobject-path", temp = strlen ("-fobject-path")))
    {
      p = do_option (p + temp);
      if (*p)
        {
          if (object_path)
            {
              static char path_separator[2] = {PATH_SEPARATOR, 0};
              char *new_object_path = concat (p, path_separator,
                                              object_path, NULL_PTR);
              free (object_path);
              object_path = new_object_path;
            }
          else
            object_path = save_string (p);
        }
    }
  else if (!strcmp (p, "-fno-object-path"))
    {
      if (object_path)
        free (object_path);
      object_path = NULL_PTR;
    }
  else if (!strncmp (p, "-fexecutable-path", temp = strlen ("-fexecutable-path")))
    {
      p = do_option (p + temp);
      if (executable_path)
        free (executable_path);
      if (*p)
        {
          int l = strlen (p) - 1;
          if (p[l] == DIR_SEPARATOR || p[l] == '/')
            executable_path = save_string (p);
          else
            {
              char dir_separator[2] = {DIR_SEPARATOR, 0};
              executable_path = concat (p, dir_separator, NULL_PTR);
            }
        }
      else
        executable_path = NULL_PTR;
    }
  else if (!strcmp (p, "-fno-executable-path"))
    {
      if (executable_path)
        free (executable_path);
      executable_path = NULL_PTR;
    }
  else if (!strncmp (p, "-funit-destination-path", temp = strlen ("-funit-destination-path")))
    {
      p = do_option (p + temp);
      if (unit_destination_path)
        free (unit_destination_path);
      if (*p)
        {
          int l = strlen (p) - 1;
          if (p[l] == DIR_SEPARATOR || p[l] == '/')
            unit_destination_path = save_string (p);
          else
            {
              char dir_separator[2] = {DIR_SEPARATOR, 0};
              unit_destination_path = concat (p, dir_separator, NULL_PTR);
            }
        }
      else
        unit_destination_path = NULL_PTR;
    }
  else if (!strcmp (p, "-fno-unit-destination-path"))
    {
      if (unit_destination_path)
        free (unit_destination_path);
      unit_destination_path = NULL_PTR;
    }
  else if (!strncmp (p, "-fobject-destination-path", temp = strlen ("-fobject-destination-path")))
    {
      p = do_option (p + temp);
      if (object_destination_path)
        free (object_destination_path);
      if (*p)
        {
          int l = strlen (p) - 1;
          if (p[l] == DIR_SEPARATOR || p[l] == '/')
            object_destination_path = save_string (p);
          else
            {
              char dir_separator[2] = {DIR_SEPARATOR, 0};
              object_destination_path = concat (p, dir_separator, NULL_PTR);
            }
        }
      else
        object_destination_path = NULL_PTR;
    }
  else if (!strcmp (p, "-fno-object-destination-path"))
    {
      if (object_destination_path)
        free (object_destination_path);
      object_destination_path = NULL_PTR;
    }
  else if (!strcmp (p, "-fno-default-paths"))
    {
      flag_no_default_paths = 1;
    }
  else if (!strncmp (p, "-fgpi-destination-path", temp = strlen ("-fgpi-destination-path")))
    {
      p = do_option (p + temp);
      if (gpi_destination_path)
        free (gpi_destination_path);
      if (*p)
        {
          int l = strlen (p) - 1;
          if (p[l] == DIR_SEPARATOR || p[l] == '/')
            gpi_destination_path = save_string (p);
          else
            {
              char dir_separator[2] = {DIR_SEPARATOR, 0};
              gpi_destination_path = concat (p, dir_separator, NULL_PTR);
            }
        }
      else
        gpi_destination_path = NULL_PTR;
    }
  else if (!strncmp (p, "-fuses", temp = strlen ("-fuses")))
    {
      p += temp;
      if (*p == ':' || *p == '=')
        p++;
      extra_imports = p;
    }
  else if (!strcmp (p, "-Wwarnings"))
    inhibit_warnings = 0;
  else if (!strcmp (p, "-Wno-warnings"))
    inhibit_warnings = 1;
  else if (!strcmp (p, "-Wfield-name-problem"))
    warn_field_name_problem = 1;
  else if (!strcmp (p, "-Wno-field-name-problem"))
    warn_field_name_problem = 0;
  else if (!strcmp (p, "-Wtyped-const"))
    warn_typed_const = 1;
  else if (!strcmp (p, "-Wno-typed-const"))
    warn_typed_const = 0;
  else if (!strcmp (p, "-Wnear-far"))
    warn_near_far = 1;
  else if (!strcmp (p, "-Wno-near-far"))
    warn_near_far = 0;

#endif /* GPC */
  else
    return 0;

  return 1;
}

/* This initialization is run once per compilation just before
 * command-line options are processed (only with EGCS -- also
 * GCC-2.8.1 with patch to toplev.c). */
void
pascal_init_options ()
{
  char **option = default_options;
  while (*option)
    {
      if (!
#ifdef EGCS
        c_decode_option (1, option)
#else
        c_decode_option (*option)
#endif
        )
        warning ("internal problem: unknown default option `%s'", *option);
      option++;
    }
}

/* Hooks for print_node.  */

void
print_lang_decl (file, node, indent)
     FILE *file;
     tree node;
     int indent;
{
#ifdef GPC
  struct lang_decl *p = DECL_LANG_SPECIFIC (node);
  if (p)
    {
      print_node (file, "pascal_interface", p->interface, indent + 4);
      fprintf (file, " pascal_uid %d", p->uid);
      print_node (file, "pascal_directive", p->directive, indent + 4);
      print_node (file, "pascal_parms", p->parms, indent + 4);
    }
#endif /* GPC */
}

void
print_lang_type (file, node, indent)
     FILE *file;
     tree node;
     int indent;
{
#ifdef GPC
  if TYPE_LANG_SPECIFIC (node)
    {
      indent_to (file, indent + 4);
      print_node (file, "pascal_interface", TYPE_LANG_INTERFACE (node), indent + 4);
      fprintf (file, " pascal_uid %d", TYPE_LANG_UID (node));
      switch (TYPE_LANG_CODE (node))
        {
          case PASCAL_LANG_NOTHING_SPECIAL:
            break;
          case PASCAL_LANG_VARIANT_RECORD:
            fputs ("pascal_variant_record", file);
            break;
          case PASCAL_LANG_OBJECT:
            fputs ("pascal_object", file);
            break;
          case PASCAL_LANG_STRING:
            fputs ("pascal_string", file);
            break;
          case PASCAL_LANG_SCHEMA:
            fputs ("pascal_schema", file);
            break;
          default:
            fputs ("!!unknown Pascal TYPE_LANG_CODE!!", file);
        }
      print_node (file, TYPE_LANG_CODE (node) == PASCAL_LANG_OBJECT ? "pascal_name"
                                                                    : "pascal_info",
                        TYPE_LANG_INFO (node), indent + 4);
      print_node (file, "pascal_base", TYPE_LANG_BASE (node), indent + 4);
    }
#endif /* GPC */
}

void
print_lang_identifier (file, node, indent)
     FILE *file;
     tree node;
     int indent;
{
  print_node (file, "global", IDENTIFIER_GLOBAL_VALUE (node), indent + 4);
  print_node (file, "local", IDENTIFIER_LOCAL_VALUE (node), indent + 4);
  print_node (file, "label", IDENTIFIER_LABEL_VALUE (node), indent + 4);
  print_node (file, "implicit", IDENTIFIER_IMPLICIT_DECL (node), indent + 4);
  print_node (file, "error locus", IDENTIFIER_ERROR_LOCUS (node), indent + 4);
  print_node (file, "limbo value", IDENTIFIER_LIMBO_VALUE (node), indent + 4);
}

/* Hook called at end of compilation to assume 1 elt
   for a top-level array decl that wasn't complete before.  */

void
finish_incomplete_decl (decl)
     tree decl;
{
  if (TREE_CODE (decl) == VAR_DECL)
    {
      tree type = TREE_TYPE (decl);
      if (type != error_mark_node
          && TREE_CODE (type) == ARRAY_TYPE
          && TYPE_DOMAIN (type) == 0)
        {
          if (! DECL_EXTERNAL (decl))
            warning_with_decl (decl, "array `%s' assumed to have one element");

          complete_array_type (type, NULL_TREE, 1);

          layout_decl (decl, 0);
        }
    }
}

/* Create a new `struct binding_level'.  */

static
struct binding_level *
make_binding_level ()
{
  /* NOSTRICT */
  return (struct binding_level *) xmalloc (sizeof (struct binding_level));
}

/* Nonzero if we are currently in the global binding level.  */

int
global_bindings_p ()
{
#ifdef GPC
  if (size_volatile)
    return -1;
#endif /* GPC */
  return current_binding_level == global_binding_level;
}

#ifdef GPC
int
pascal_global_bindings_p ()
{
  if (current_module)
    return top_level_p (current_module->main_program);
  else
    return current_binding_level == global_binding_level;
}

/* To find out if program/module parameters should
 * be associated with objects defined at this level.
 *
 * They should be associated only to top level things.
 */
int
top_level_p (main_program)
     int main_program;
{
  if (current_binding_level == global_binding_level
      || current_binding_level->level_chain == global_binding_level)
    return TRUE;
  else
    return FALSE;
}
#endif /* GPC */

void
keep_next_level ()
{
  keep_next_level_flag = 1;
}

/* Nonzero if the current level needs to have a BLOCK made.  */

int
kept_level_p ()
{
  return ((current_binding_level->keep_if_subblocks
           && current_binding_level->blocks != 0)
          || current_binding_level->keep
          || current_binding_level->names != 0
          || (current_binding_level->tags != 0
              && !current_binding_level->tag_transparent));
}

/* Identify this binding level as a level of parameters.
   DEFINITION_FLAG is 1 for a definition, 0 for a declaration.
   But it turns out there is no way to pass the right value for
   DEFINITION_FLAG, so we ignore it.  */

void
declare_parm_level (definition_flag)
     int definition_flag;
{
  current_binding_level->parm_flag = 1;
}

/* Nonzero if currently making parm declarations.  */

int
in_parm_level_p ()
{
  return current_binding_level->parm_flag;
}

/* Enter a new binding level.
   If TAG_TRANSPARENT is nonzero, do so only for the name space of variables,
   not for that of tags.  */

void
pushlevel (tag_transparent)
     int tag_transparent;
{
  register struct binding_level *newlevel = NULL_BINDING_LEVEL;

  /* If this is the top level of a function,
     just make sure that NAMED_LABELS is 0.  */

  if (current_binding_level == global_binding_level)
    {
      named_labels = 0;
    }

  /* Reuse or create a struct for this binding level.  */

  if (free_binding_level)
    {
      newlevel = free_binding_level;
      free_binding_level = free_binding_level->level_chain;
    }
  else
    {
      newlevel = make_binding_level ();
    }

  /* Add this level to the front of the chain (stack) of levels that
     are active.  */

  *newlevel = clear_binding_level;
  newlevel->tag_transparent
    = (tag_transparent
       || (current_binding_level
           ? current_binding_level->subblocks_tag_transparent
           : 0));
  newlevel->level_chain = current_binding_level;
  current_binding_level = newlevel;
  newlevel->keep = keep_next_level_flag;
  keep_next_level_flag = 0;
  newlevel->keep_if_subblocks = keep_next_if_subblocks;
  keep_next_if_subblocks = 0;
}

/* Clear the limbo values of all identifiers defined in BLOCK or a subblock. */

static void
clear_limbo_values (block)
     tree block;
{
  tree tem;

  for (tem = BLOCK_VARS (block); tem; tem = TREE_CHAIN (tem))
    if (DECL_NAME (tem) != 0)
      IDENTIFIER_LIMBO_VALUE (DECL_NAME (tem)) = 0;

  for (tem = BLOCK_SUBBLOCKS (block); tem; tem = TREE_CHAIN (tem))
    clear_limbo_values (tem);
}

/* Exit a binding level.
   Pop the level off, and restore the state of the identifier-decl mappings
   that were in effect when this level was entered.

   If KEEP is nonzero, this level had explicit declarations, so
   and create a "block" (a BLOCK node) for the level
   to record its declarations and subblocks for symbol table output.

   If FUNCTIONBODY is nonzero, this level is the body of a function,
   so create a block as if KEEP were set and also clear out all
   label names.

   If REVERSE is nonzero, reverse the order of decls before putting
   them into the BLOCK.  */

tree
poplevel (keep, reverse, functionbody)
     int keep;
     int reverse;
     int functionbody;
{
  register tree link;
  /* The chain of decls was accumulated in reverse order.
     Put it into forward order, just for cleanliness.  */
  tree decls;
  tree tags = current_binding_level->tags;
  tree subblocks = current_binding_level->blocks;
  tree block = 0;
  tree decl;
  int block_previously_created;

  keep |= current_binding_level->keep;

  /* This warning is turned off because it causes warnings for
     declarations like `extern struct foo *x'.  */
#if 0
  /* Warn about incomplete structure types in this level.  */
  for (link = tags; link; link = TREE_CHAIN (link))
    if (TYPE_SIZE (TREE_VALUE (link)) == 0)
      {
        tree type = TREE_VALUE (link);
        char *errmsg;
        switch (TREE_CODE (type))
          {
          case RECORD_TYPE:
            errmsg = "`struct %s' incomplete in scope ending here";
            break;
          case UNION_TYPE:
            errmsg = "`union %s' incomplete in scope ending here";
            break;
          case ENUMERAL_TYPE:
            errmsg = "`enum %s' incomplete in scope ending here";
            break;
          }
        if (TREE_CODE (TYPE_NAME (type)) == IDENTIFIER_NODE)
          error (errmsg, IDENTIFIER_POINTER (TYPE_NAME (type)));
        else
          /* If this type has a typedef-name, the TYPE_NAME is a TYPE_DECL.  */
          error (errmsg, IDENTIFIER_POINTER (DECL_NAME (TYPE_NAME (type))));
      }
#endif /* 0 */

  /* Get the decls in the order they were written.
     Usually current_binding_level->names is in reverse order.
     But parameter decls were previously put in forward order.  */

  if (reverse)
    current_binding_level->names
      = decls = nreverse (current_binding_level->names);
  else
    decls = current_binding_level->names;

  /* Output any nested inline functions within this block
     if they weren't already output.  */

  for (decl = decls; decl; decl = TREE_CHAIN (decl))
    if (TREE_CODE (decl) == FUNCTION_DECL
        && ! TREE_ASM_WRITTEN (decl)
        && DECL_INITIAL (decl) != 0
        && TREE_ADDRESSABLE (decl))
      {
        /* If this decl was copied from a file-scope decl
           on account of a block-scope extern decl,
           propagate TREE_ADDRESSABLE to the file-scope decl.

           DECL_ABSTRACT_ORIGIN can be set to itself if warn_return_type is
           true, since then the decl goes through save_for_inline_copying.  */
        if (DECL_ABSTRACT_ORIGIN (decl) != 0
            && DECL_ABSTRACT_ORIGIN (decl) != decl)
          TREE_ADDRESSABLE (DECL_ABSTRACT_ORIGIN (decl)) = 1;
        else if (DECL_SAVED_INSNS (decl) != 0)
          {
            push_function_context ();
            output_inline_function (decl);
            pop_function_context ();
          }
      }

  /* If there were any declarations or structure tags in that level,
     or if this level is a function body,
     create a BLOCK to record them for the life of this function.  */

  block = 0;
  block_previously_created = (current_binding_level->this_block != 0);
  if (block_previously_created)
    block = current_binding_level->this_block;
  else if (keep || functionbody
           || (current_binding_level->keep_if_subblocks && subblocks != 0))
    block = make_node (BLOCK);
  if (block != 0)
    {
      BLOCK_VARS (block) = decls;
      BLOCK_TYPE_TAGS (block) = tags;
      BLOCK_SUBBLOCKS (block) = subblocks;
      remember_end_note (block);
    }

  /* In each subblock, record that this is its superior.  */

  for (link = subblocks; link; link = TREE_CHAIN (link))
    BLOCK_SUPERCONTEXT (link) = block;

  /* Clear out the meanings of the local variables of this level.  */

  for (link = decls; link; link = TREE_CHAIN (link))
    {
      if (DECL_NAME (link) != 0)
        {
          /* If the ident. was used or addressed via a local extern decl,
             don't forget that fact.  */
          if (DECL_EXTERNAL (link))
            {
              if (TREE_USED (link))
                TREE_USED (DECL_NAME (link)) = 1;
              if (TREE_ADDRESSABLE (link))
                TREE_ADDRESSABLE (DECL_ASSEMBLER_NAME (link)) = 1;
            }
          IDENTIFIER_LOCAL_VALUE (DECL_NAME (link)) = 0;
        }
    }

  /* Restore all name-meanings of the outer levels
     that were shadowed by this level.  */

  for (link = current_binding_level->shadowed; link; link = TREE_CHAIN (link))
    IDENTIFIER_LOCAL_VALUE (TREE_PURPOSE (link)) = TREE_VALUE (link);

  /* If the level being exited is the top level of a function,
     check over all the labels, and clear out the current
     (function local) meanings of their names.  */

  if (functionbody)
    {
      clear_limbo_values (block);

      /* If this is the top level block of a function,
         the vars are the function's parameters.
         Don't leave them in the BLOCK because they are
         found in the FUNCTION_DECL instead.  */

      BLOCK_VARS (block) = 0;

      /* Clear out the definitions of all label names,
         since their scopes end here,
         and add them to BLOCK_VARS.  */

      for (link = named_labels; link; link = TREE_CHAIN (link))
        {
          register tree label = TREE_VALUE (link);

          if (DECL_INITIAL (label) == 0)
            {
              error_with_decl (label, "label `%s' used but not defined");
              /* Avoid crashing later.  */
              define_label (input_filename, lineno,
                            DECL_NAME (label));
            }
          else if (warn_unused && !TREE_USED (label))
            warning_with_decl (label, "label `%s' defined but not used");
          IDENTIFIER_LABEL_VALUE (DECL_NAME (label)) = 0;

          /* Put the labels into the "variables" of the
             top-level block, so debugger can see them.  */
          TREE_CHAIN (label) = BLOCK_VARS (block);
          BLOCK_VARS (block) = label;
        }
#ifdef GPC
      /* For Pascal, notify if the `label' declaration part declared a label
       * that was not defined in its scope.
       */
      for (link = declared_labels; link; link = TREE_CHAIN (link))
        {
          tree lab;
          for (lab = named_labels; lab; lab = TREE_CHAIN (lab))
            if (DECL_NAME (TREE_VALUE (lab)) == TREE_VALUE (link))
              break;
          if (! lab)
            pedwarn ("declared label `%s' not defined in scope ending here",
                     IDENTIFIER_POINTER (TREE_VALUE (link)));
        }
#endif /* GPC */
    }

  /* Pop the current level, and free the structure for reuse.  */

  {
    register struct binding_level *level = current_binding_level;
    current_binding_level = current_binding_level->level_chain;

    level->level_chain = free_binding_level;
    free_binding_level = level;
  }

  /* Dispose of the block that we just made inside some higher level.  */
  if (functionbody)
    DECL_INITIAL (current_function_decl) = block;
  else if (block)
    {
      if (!block_previously_created)
        current_binding_level->blocks
          = chainon (current_binding_level->blocks, block);
    }
  /* If we did not make a block for the level just exited,
     any blocks made for inner levels
     (since they cannot be recorded as subblocks in that level)
     must be carried forward so they will later become subblocks
     of something else.  */
  else if (subblocks)
    current_binding_level->blocks
      = chainon (current_binding_level->blocks, subblocks);

  /* Set the TYPE_CONTEXTs for all of the tagged types belonging to this
     binding contour so that they point to the appropriate construct, i.e.
     either to the current FUNCTION_DECL node, or else to the BLOCK node
     we just constructed.

     Note that for tagged types whose scope is just the formal parameter
     list for some function type specification, we can't properly set
     their TYPE_CONTEXTs here, because we don't have a pointer to the
     appropriate FUNCTION_TYPE node readily available to us.  For those
     cases, the TYPE_CONTEXTs of the relevant tagged type nodes get set
     in `grokdeclarator' as soon as we have created the FUNCTION_TYPE
     node which will represent the "scope" for these "parameter list local"
     tagged types.
  */

  if (functionbody)
    for (link = tags; link; link = TREE_CHAIN (link))
      TYPE_CONTEXT (TREE_VALUE (link)) = current_function_decl;
  else if (block)
    for (link = tags; link; link = TREE_CHAIN (link))
      TYPE_CONTEXT (TREE_VALUE (link)) = block;

  if (block)
    TREE_USED (block) = 1;
  return block;
}

/* Delete the node BLOCK from the current binding level.
   This is used for the block inside a stmt expr ({...})
   so that the block can be reinserted where appropriate.  */

void
delete_block (block)
     tree block;
{
  tree t;
  if (current_binding_level->blocks == block)
    current_binding_level->blocks = TREE_CHAIN (block);
  for (t = current_binding_level->blocks; t;)
    {
      if (TREE_CHAIN (t) == block)
        TREE_CHAIN (t) = TREE_CHAIN (block);
      else
        t = TREE_CHAIN (t);
    }
  TREE_CHAIN (block) = NULL;
  /* Clear TREE_USED which is always set by poplevel.
     The flag is set again if insert_block is called.  */
  TREE_USED (block) = 0;
}

/* Insert BLOCK at the end of the list of subblocks of the
   current binding level.  This is used when a BIND_EXPR is expanded,
   to handle the BLOCK node inside the BIND_EXPR.  */

void
insert_block (block)
     tree block;
{
  TREE_USED (block) = 1;
  current_binding_level->blocks
    = chainon (current_binding_level->blocks, block);
}

/* Set the BLOCK node for the innermost scope
   (the one we are currently in).  */

void
set_block (block)
     register tree block;
{
  current_binding_level->this_block = block;
}

void
push_label_level ()
{
  register struct binding_level *newlevel;

  /* Reuse or create a struct for this binding level.  */

  if (free_binding_level)
    {
      newlevel = free_binding_level;
      free_binding_level = free_binding_level->level_chain;
    }
  else
    {
      newlevel = make_binding_level ();
    }

  /* Add this level to the front of the chain (stack) of label levels.  */

  newlevel->level_chain = label_level_chain;
  label_level_chain = newlevel;

  newlevel->names = named_labels;
  newlevel->shadowed = shadowed_labels;
  named_labels = 0;
  shadowed_labels = 0;
#ifdef GPC
  newlevel->declared_labels = declared_labels;
  declared_labels = NULL;
#endif /* GPC */
}

void
pop_label_level ()
{
  register struct binding_level *level = label_level_chain;
  tree link, prev;

  /* Clear out the definitions of the declared labels in this level.
     Leave in the list any ordinary, non-declared labels.  */
  for (link = named_labels, prev = 0; link;)
    {
      if (C_DECLARED_LABEL_FLAG (TREE_VALUE (link)))
        {
          if (DECL_SOURCE_LINE (TREE_VALUE (link)) == 0)
            {
              error_with_decl (TREE_VALUE (link),
                               "label `%s' used but not defined");
              /* Avoid crashing later.  */
              define_label (input_filename, lineno,
                            DECL_NAME (TREE_VALUE (link)));
            }
          else if (warn_unused && !TREE_USED (TREE_VALUE (link)))
            warning_with_decl (TREE_VALUE (link),
                               "label `%s' defined but not used");
          IDENTIFIER_LABEL_VALUE (DECL_NAME (TREE_VALUE (link))) = 0;

          /* Delete this element from the list.  */
          link = TREE_CHAIN (link);
          if (prev)
            TREE_CHAIN (prev) = link;
          else
            named_labels = link;
        }
      else
        {
          prev = link;
          link = TREE_CHAIN (link);
        }
    }

  /* Bring back all the labels that were shadowed.  */
  for (link = shadowed_labels; link; link = TREE_CHAIN (link))
    if (DECL_NAME (TREE_VALUE (link)) != 0)
      IDENTIFIER_LABEL_VALUE (DECL_NAME (TREE_VALUE (link)))
        = TREE_VALUE (link);

  named_labels = chainon (named_labels, level->names);
  shadowed_labels = level->shadowed;

#ifdef GPC
  declared_labels = level->declared_labels;
#endif /* GPC */
  /* Pop the current level, and free the structure for reuse.  */
  label_level_chain = label_level_chain->level_chain;
  level->level_chain = free_binding_level;
  free_binding_level = level;
}

/* Push a definition or a declaration of struct, union or enum tag "name".
   "type" should be the type node.
   We assume that the tag "name" is not already defined.

   Note that the definition may really be just a forward reference.
   In that case, the TYPE_SIZE will be zero.  */

void
pushtag (name, type)
     tree name, type;
{
  register struct binding_level *b;

  /* Find the proper binding level for this type tag.  */

  for (b = current_binding_level; b->tag_transparent; b = b->level_chain)
    continue;

  if (name)
    {
      /* Record the identifier as the type's name if it has none.  */

      if (TYPE_NAME (type) == 0)
        TYPE_NAME (type) = name;
    }

  if (b == global_binding_level)
    b->tags = perm_tree_cons (name, type, b->tags);
  else
    b->tags = saveable_tree_cons (name, type, b->tags);

  /* Create a fake NULL-named TYPE_DECL node whose TREE_TYPE will be the
     tagged type we just added to the current binding level.  This fake
     NULL-named TYPE_DECL node helps dwarfout.c to know when it needs
     to output a representation of a tagged type, and it also gives
     us a convenient place to record the "scope start" address for the
     tagged type.  */

  TYPE_STUB_DECL (type) = pushdecl (build_decl (TYPE_DECL, NULL_TREE, type));

  /* An approximation for now, so we can tell this is a function-scope tag.
     This will be updated in poplevel.  */
  TYPE_CONTEXT (type) = DECL_CONTEXT (TYPE_STUB_DECL (type));
}

/* Handle when a new declaration NEWDECL
   has the same name as an old one OLDDECL
   in the same binding contour.
   Prints an error message if appropriate.

   If safely possible, alter OLDDECL to look like NEWDECL, and return 1.
   Otherwise, return 0.

   When DIFFERENT_BINDING_LEVEL is true, NEWDECL is an external declaration,
   and OLDDECL is in an outer binding level and should thus not be changed.  */

static int
duplicate_decls (newdecl, olddecl, different_binding_level)
     register tree newdecl, olddecl;
     int different_binding_level;
{
  int types_match = comptypes (TREE_TYPE (newdecl), TREE_TYPE (olddecl));
  int new_is_definition = (TREE_CODE (newdecl) == FUNCTION_DECL
                           && DECL_INITIAL (newdecl) != 0);
  tree oldtype = TREE_TYPE (olddecl);
  tree newtype = TREE_TYPE (newdecl);
  char *errmsg = 0;

#ifdef GPC
  /* When using Units, everything is allowed here.
   * @@ With EP Modules, this is the place to take care of
   * duplicate definitions and such.
   */
  if (! we_are_loading_a_gpi_file)
    {
#endif /* GPC */

  if (TREE_CODE_CLASS (TREE_CODE (olddecl)) == 'd')
    DECL_MACHINE_ATTRIBUTES (newdecl) = DECL_MACHINE_ATTRIBUTES (olddecl);

  if (TREE_CODE (newtype) == ERROR_MARK
      || TREE_CODE (oldtype) == ERROR_MARK)
    types_match = 0;

  /* New decl is completely inconsistent with the old one =>
     tell caller to replace the old one.
     This is always an error except in the case of shadowing a builtin.  */
  if (TREE_CODE (olddecl) != TREE_CODE (newdecl))
    {
      if (TREE_CODE (olddecl) == FUNCTION_DECL
          && (DECL_BUILT_IN (olddecl)
              || DECL_BUILT_IN_NONANSI (olddecl)))
        {
          /* If you declare a built-in or predefined function name as static,
             the old definition is overridden,
             but optionally warn this was a bad choice of name.  */
          if (!TREE_PUBLIC (newdecl))
            {
              if (!warn_shadow)
                ;
              else if (DECL_BUILT_IN (olddecl))
                warning_with_decl (newdecl, "shadowing built-in function `%s'");
              else
                warning_with_decl (newdecl, "shadowing library function `%s'");
            }
          /* Likewise, if the built-in is not ansi, then programs can
             override it even globally without an error.  */
          else if (! DECL_BUILT_IN (olddecl))
            warning_with_decl (newdecl,
                               "library function `%s' declared as non-function");

          else if (DECL_BUILT_IN_NONANSI (olddecl))
            warning_with_decl (newdecl,
                               "built-in function `%s' declared as non-function");
          else
            warning_with_decl (newdecl,
                             "built-in function `%s' declared as non-function");
        }
      else
        {
          error_with_decl (newdecl, "`%s' redeclared as different kind of symbol");
          error_with_decl (olddecl, "previous declaration of `%s'");
        }

      return 0;
    }

  /* For real parm decl following a forward decl,
     return 1 so old decl will be reused.  */
  if (types_match && TREE_CODE (newdecl) == PARM_DECL
      && TREE_ASM_WRITTEN (olddecl) && ! TREE_ASM_WRITTEN (newdecl))
    return 1;

  /* The new declaration is the same kind of object as the old one.
     The declarations may partially match.  Print warnings if they don't
     match enough.  Ultimately, copy most of the information from the new
     decl to the old one, and keep using the old one.  */

  if (flag_traditional && TREE_CODE (newdecl) == FUNCTION_DECL
      && IDENTIFIER_IMPLICIT_DECL (DECL_NAME (newdecl)) == olddecl
      && DECL_INITIAL (olddecl) == 0)
    /* If -traditional, avoid error for redeclaring fcn
       after implicit decl.  */
    ;
  else if (TREE_CODE (olddecl) == FUNCTION_DECL
           && DECL_BUILT_IN (olddecl))
    {
      /* A function declaration for a built-in function.  */
      if (!TREE_PUBLIC (newdecl))
        {
          /* If you declare a built-in function name as static, the
             built-in definition is overridden,
             but optionally warn this was a bad choice of name.  */
          if (warn_shadow)
            warning_with_decl (newdecl, "shadowing built-in function `%s'");
          /* Discard the old built-in function.  */
          return 0;
        }
      else if (!types_match)
        {
          /* Accept the return type of the new declaration if same modes.  */
          tree oldreturntype = TREE_TYPE (oldtype);
          tree newreturntype = TREE_TYPE (newtype);

          /* Make sure we put the new type in the same obstack as the old ones.
             If the old types are not both in the same obstack, use the
             permanent one.  */
          if (TYPE_OBSTACK (oldtype) == TYPE_OBSTACK (newtype))
            push_obstacks (TYPE_OBSTACK (oldtype), TYPE_OBSTACK (oldtype));
          else
            {
              push_obstacks_nochange ();
              end_temporary_allocation ();
            }

          if (TYPE_MODE (oldreturntype) == TYPE_MODE (newreturntype))
            {
              /* Function types may be shared, so we can't just modify
                 the return type of olddecl's function type.  */
              tree trytype
                = build_function_type (newreturntype,
                                       TYPE_ARG_TYPES (oldtype));

              types_match = comptypes (newtype, trytype);
              if (types_match)
                oldtype = trytype;
            }
          /* Accept harmless mismatch in first argument type also.
             This is for ffs.  */
          if (TYPE_ARG_TYPES (TREE_TYPE (newdecl)) != 0
              && TYPE_ARG_TYPES (oldtype) != 0
              && TREE_VALUE (TYPE_ARG_TYPES (newtype)) != 0
              && TREE_VALUE (TYPE_ARG_TYPES (oldtype)) != 0
              && (TYPE_MODE (TREE_VALUE (TYPE_ARG_TYPES (newtype)))
                  == TYPE_MODE (TREE_VALUE (TYPE_ARG_TYPES (oldtype)))))
            {
              /* Function types may be shared, so we can't just modify
                 the return type of olddecl's function type.  */
              tree trytype
                = build_function_type (TREE_TYPE (oldtype),
                                       tree_cons (NULL_TREE,
                                                  TREE_VALUE (TYPE_ARG_TYPES (newtype)),
                                                  TREE_CHAIN (TYPE_ARG_TYPES (oldtype))));

              types_match = comptypes (newtype, trytype);
              if (types_match)
                oldtype = trytype;
            }
          if (! different_binding_level)
            TREE_TYPE (olddecl) = oldtype;

          pop_obstacks ();
        }
      if (!types_match)
        {
          /* If types don't match for a built-in, throw away the built-in.  */
          warning_with_decl (newdecl, "conflicting types for built-in function `%s'");
          return 0;
        }
    }
  else if (TREE_CODE (olddecl) == FUNCTION_DECL
           && DECL_SOURCE_LINE (olddecl) == 0)
    {
      /* A function declaration for a predeclared function
         that isn't actually built in.  */
      if (!TREE_PUBLIC (newdecl))
        {
          /* If you declare it as static, the
             default definition is overridden.  */
          return 0;
        }
      else if (!types_match)
        {
          /* If the types don't match, preserve volatility indication.
             Later on, we will discard everything else about the
             default declaration.  */
          TREE_THIS_VOLATILE (newdecl) |= TREE_THIS_VOLATILE (olddecl);
        }
    }
  /* Permit char *foo () to match void *foo (...) if not pedantic,
     if one of them came from a system header file.  */
  else if (!types_match
           && TREE_CODE (olddecl) == FUNCTION_DECL
           && TREE_CODE (newdecl) == FUNCTION_DECL
           && TREE_CODE (TREE_TYPE (oldtype)) == POINTER_TYPE
           && TREE_CODE (TREE_TYPE (newtype)) == POINTER_TYPE
           && (DECL_IN_SYSTEM_HEADER (olddecl)
               || DECL_IN_SYSTEM_HEADER (newdecl))
           && ((TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (newtype))) == void_type_node
                && TYPE_ARG_TYPES (oldtype) == 0
                && self_promoting_args_p (TYPE_ARG_TYPES (newtype))
                && TREE_TYPE (TREE_TYPE (oldtype)) == char_type_node)
               ||
               (TREE_TYPE (TREE_TYPE (newtype)) == char_type_node
                && TYPE_ARG_TYPES (newtype) == 0
                && self_promoting_args_p (TYPE_ARG_TYPES (oldtype))
                && TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (oldtype))) == void_type_node)))
    {
      if (pedantic)
        pedwarn_with_decl (newdecl, "conflicting types for `%s'");
      /* Make sure we keep void * as ret type, not char *.  */
      if (TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (oldtype))) == void_type_node)
        TREE_TYPE (newdecl) = newtype = oldtype;

      /* Set DECL_IN_SYSTEM_HEADER, so that if we see another declaration
         we will come back here again.  */
      DECL_IN_SYSTEM_HEADER (newdecl) = 1;
    }
  else if (!types_match
           /* Permit char *foo (int, ...); followed by char *foo ();
              if not pedantic.  */
           && ! (TREE_CODE (olddecl) == FUNCTION_DECL
                 && ! pedantic
                 /* Return types must still match.  */
                 && comptypes (TREE_TYPE (oldtype),
                               TREE_TYPE (newtype))
                 && TYPE_ARG_TYPES (newtype) == 0))
    {
      error_with_decl (newdecl, "conflicting types for `%s'");
      /* Check for function type mismatch
         involving an empty arglist vs a nonempty one.  */
      if (TREE_CODE (olddecl) == FUNCTION_DECL
          && comptypes (TREE_TYPE (oldtype),
                        TREE_TYPE (newtype))
          && ((TYPE_ARG_TYPES (oldtype) == 0
               && DECL_INITIAL (olddecl) == 0)
              ||
              (TYPE_ARG_TYPES (newtype) == 0
               && DECL_INITIAL (newdecl) == 0)))
        {
          /* Classify the problem further.  */
          register tree t = TYPE_ARG_TYPES (oldtype);
          if (t == 0)
            t = TYPE_ARG_TYPES (newtype);
          for (; t; t = TREE_CHAIN (t))
            {
              register tree type = TREE_VALUE (t);

              if (TREE_CHAIN (t) == 0
                  && TYPE_MAIN_VARIANT (type) != void_type_node)
                {
                  error ("A parameter list with an ellipsis can't match");
                  error (" an empty parameter name list declaration.");
                  break;
                }

              if (TYPE_MAIN_VARIANT (type) == float_type_node
                  || C_PROMOTING_INTEGER_TYPE_P (type))
                {
                  error ("An argument type that has a default promotion");
                  error (" can't match an empty parameter name list declaration.");
                  break;
                }
            }
        }
      error_with_decl (olddecl, "previous declaration of `%s'");
    }
  else
    {
      errmsg = redeclaration_error_message (newdecl, olddecl);
      if (errmsg)
        {
          error_with_decl (newdecl, errmsg);
          error_with_decl (olddecl,
                           ((DECL_INITIAL (olddecl)
                             && current_binding_level == global_binding_level)
                            ? "`%s' previously defined here"
                            : "`%s' previously declared here"));
        }
      else if (TREE_CODE (newdecl) == TYPE_DECL
               && (DECL_IN_SYSTEM_HEADER (olddecl)
                   || DECL_IN_SYSTEM_HEADER (newdecl)))
        {
          warning_with_decl (newdecl, "redefinition of `%s'");
          warning_with_decl
            (olddecl,
             ((DECL_INITIAL (olddecl)
               && current_binding_level == global_binding_level)
              ? "`%s' previously defined here"
              : "`%s' previously declared here"));
        }
      else if (TREE_CODE (olddecl) == FUNCTION_DECL
               && DECL_INITIAL (olddecl) != 0
               && TYPE_ARG_TYPES (oldtype) == 0
               && TYPE_ARG_TYPES (newtype) != 0
               && TYPE_ACTUAL_ARG_TYPES (oldtype) != 0)
        {
          register tree type, parm;
          register int nargs;
          /* Prototype decl follows defn w/o prototype.  */

          for (parm = TYPE_ACTUAL_ARG_TYPES (oldtype),
               type = TYPE_ARG_TYPES (newtype),
               nargs = 1;
               (TYPE_MAIN_VARIANT (TREE_VALUE (parm)) != void_type_node
                || TYPE_MAIN_VARIANT (TREE_VALUE (type)) != void_type_node);
               parm = TREE_CHAIN (parm), type = TREE_CHAIN (type), nargs++)
            {
              if (TYPE_MAIN_VARIANT (TREE_VALUE (parm)) == void_type_node
                  || TYPE_MAIN_VARIANT (TREE_VALUE (type)) == void_type_node)
                {
                  errmsg = "prototype for `%s' follows and number of arguments";
                  break;
                }
              /* Type for passing arg must be consistent
                 with that declared for the arg.  */
              if (! comptypes (TREE_VALUE (parm), TREE_VALUE (type))
                  /* If -traditional, allow `unsigned int' instead of `int'
                     in the prototype.  */
                  && (! (flag_traditional
                         && TYPE_MAIN_VARIANT (TREE_VALUE (parm)) == integer_type_node
                         && TYPE_MAIN_VARIANT (TREE_VALUE (type)) == unsigned_type_node)))
                {
                  errmsg = "prototype for `%s' follows and argument %d";
                  break;
                }
            }
          if (errmsg)
            {
              error_with_decl (newdecl, errmsg, nargs);
              error_with_decl (olddecl,
                               "doesn't match non-prototype definition here");
            }
          else
            {
              warning_with_decl (newdecl, "prototype for `%s' follows");
              warning_with_decl (olddecl, "non-prototype definition here");
            }
        }
      /* Warn about mismatches in various flags.  */
      else
        {
          /* Warn if function is now inline
             but was previously declared not inline and has been called.  */
          if (TREE_CODE (olddecl) == FUNCTION_DECL
              && ! DECL_INLINE (olddecl) && DECL_INLINE (newdecl)
              && TREE_USED (olddecl))
            warning_with_decl (newdecl,
                               "`%s' declared inline after being called");
          if (TREE_CODE (olddecl) == FUNCTION_DECL
              && ! DECL_INLINE (olddecl) && DECL_INLINE (newdecl)
              && DECL_INITIAL (olddecl) != 0)
            warning_with_decl (newdecl,
                               "`%s' declared inline after its definition");

          /* If pedantic, warn when static declaration follows a non-static
             declaration.  Otherwise, do so only for functions.  */
          if ((pedantic || TREE_CODE (olddecl) == FUNCTION_DECL)
              && TREE_PUBLIC (olddecl)
              && !TREE_PUBLIC (newdecl))
            warning_with_decl (newdecl, "static declaration for `%s' follows non-static");

          /* Warn when const declaration follows a non-const
             declaration, but not for functions.  */
          if (TREE_CODE (olddecl) != FUNCTION_DECL
              && !TREE_READONLY (olddecl)
              && TREE_READONLY (newdecl))
            warning_with_decl (newdecl, "const declaration for `%s' follows non-const");
          /* These bits are logically part of the type, for variables.
             But not for functions
             (where qualifiers are not valid ANSI anyway).  */
          else if (pedantic && TREE_CODE (olddecl) != FUNCTION_DECL
              && (TREE_READONLY (newdecl) != TREE_READONLY (olddecl)
                  || TREE_THIS_VOLATILE (newdecl) != TREE_THIS_VOLATILE (olddecl)))
            pedwarn_with_decl (newdecl, "type qualifiers for `%s' conflict with previous decl");
        }
    }

  /* Optionally warn about more than one declaration for the same name.  */
  if (errmsg == 0 && warn_redundant_decls && DECL_SOURCE_LINE (olddecl) != 0
      /* Don't warn about a function declaration
         followed by a definition.  */
      && !(TREE_CODE (newdecl) == FUNCTION_DECL && DECL_INITIAL (newdecl) != 0
           && DECL_INITIAL (olddecl) == 0)
      /* Don't warn about extern decl followed by (tentative) definition.  */
      && !(DECL_EXTERNAL (olddecl) && ! DECL_EXTERNAL (newdecl)))
    {
      warning_with_decl (newdecl, "redundant redeclaration of `%s' in same scope");
      warning_with_decl (olddecl, "previous declaration of `%s'");
    }

  /* Copy all the DECL_... slots specified in the new decl
     except for any that we copy here from the old type.

     Past this point, we don't change OLDTYPE and NEWTYPE
     even if we change the types of NEWDECL and OLDDECL.  */

  if (types_match)
    {
      /* When copying info to olddecl, we store into write_olddecl
         instead.  This allows us to avoid modifying olddecl when
         different_binding_level is true.  */
      tree write_olddecl = different_binding_level ? newdecl : olddecl;

      /* Make sure we put the new type in the same obstack as the old ones.
         If the old types are not both in the same obstack, use the permanent
         one.  */
      if (TYPE_OBSTACK (oldtype) == TYPE_OBSTACK (newtype))
        push_obstacks (TYPE_OBSTACK (oldtype), TYPE_OBSTACK (oldtype));
      else
        {
          push_obstacks_nochange ();
          end_temporary_allocation ();
        }

      /* Merge the data types specified in the two decls.  */
      if (TREE_CODE (newdecl) != FUNCTION_DECL || !DECL_BUILT_IN (olddecl))
        {
          if (different_binding_level)
            TREE_TYPE (newdecl)
              = build_type_attribute_variant
                (newtype,
                 merge_attributes (TYPE_ATTRIBUTES (newtype),
                                   TYPE_ATTRIBUTES (oldtype)));
          else
            TREE_TYPE (newdecl)
              = TREE_TYPE (olddecl)
                = common_type (newtype, oldtype);
        }

      /* Lay the type out, unless already done.  */
      if (oldtype != TREE_TYPE (newdecl))
        {
          if (TREE_TYPE (newdecl) != error_mark_node)
            layout_type (TREE_TYPE (newdecl));
          if (TREE_CODE (newdecl) != FUNCTION_DECL
              && TREE_CODE (newdecl) != TYPE_DECL
              && TREE_CODE (newdecl) != CONST_DECL)
            layout_decl (newdecl, 0);
        }
      else
        {
          /* Since the type is OLDDECL's, make OLDDECL's size go with.  */
          DECL_SIZE (newdecl) = DECL_SIZE (olddecl);
          if (TREE_CODE (olddecl) != FUNCTION_DECL)
            if (DECL_ALIGN (olddecl) > DECL_ALIGN (newdecl))
              DECL_ALIGN (newdecl) = DECL_ALIGN (olddecl);
        }

      /* Keep the old rtl since we can safely use it.  */
      DECL_RTL (newdecl) = DECL_RTL (olddecl);

      /* Merge the type qualifiers.  */
      if (DECL_BUILT_IN_NONANSI (olddecl) && TREE_THIS_VOLATILE (olddecl)
          && !TREE_THIS_VOLATILE (newdecl))
        TREE_THIS_VOLATILE (write_olddecl) = 0;
      if (TREE_READONLY (newdecl))
        TREE_READONLY (write_olddecl) = 1;
      if (TREE_THIS_VOLATILE (newdecl))
        {
          TREE_THIS_VOLATILE (write_olddecl) = 1;
          if (TREE_CODE (newdecl) == VAR_DECL)
            make_var_volatile (newdecl);
        }

      /* Keep source location of definition rather than declaration.  */
      /* When called with different_binding_level set, keep the old
         information so that meaningful diagnostics can be given.  */
      if (DECL_INITIAL (newdecl) == 0 && DECL_INITIAL (olddecl) != 0
          && ! different_binding_level)
        {
          DECL_SOURCE_LINE (newdecl) = DECL_SOURCE_LINE (olddecl);
          DECL_SOURCE_FILE (newdecl) = DECL_SOURCE_FILE (olddecl);
        }

      /* Merge the unused-warning information.  */
      if (DECL_IN_SYSTEM_HEADER (olddecl))
        DECL_IN_SYSTEM_HEADER (newdecl) = 1;
      else if (DECL_IN_SYSTEM_HEADER (newdecl))
        DECL_IN_SYSTEM_HEADER (write_olddecl) = 1;

      /* Merge the initialization information.  */
      /* When called with different_binding_level set, don't copy over
         DECL_INITIAL, so that we don't accidentally change function
         declarations into function definitions.  */
      if (DECL_INITIAL (newdecl) == 0 && ! different_binding_level)
        DECL_INITIAL (newdecl) = DECL_INITIAL (olddecl);

      /* Merge the section attribute.
         We want to issue an error if the sections conflict but that must be
         done later in decl_attributes since we are called before attributes
         are assigned.  */
      if (DECL_SECTION_NAME (newdecl) == NULL_TREE)
        DECL_SECTION_NAME (newdecl) = DECL_SECTION_NAME (olddecl);

      if (TREE_CODE (newdecl) == FUNCTION_DECL)
        {
          DECL_STATIC_CONSTRUCTOR(newdecl) |= DECL_STATIC_CONSTRUCTOR(olddecl);
          DECL_STATIC_DESTRUCTOR (newdecl) |= DECL_STATIC_DESTRUCTOR (olddecl);
        }

      pop_obstacks ();
    }
  /* If cannot merge, then use the new type and qualifiers,
     and don't preserve the old rtl.  */
  else if (! different_binding_level)
    {
      TREE_TYPE (olddecl) = TREE_TYPE (newdecl);
      TREE_READONLY (olddecl) = TREE_READONLY (newdecl);
      TREE_THIS_VOLATILE (olddecl) = TREE_THIS_VOLATILE (newdecl);
      TREE_SIDE_EFFECTS (olddecl) = TREE_SIDE_EFFECTS (newdecl);
    }

#ifdef GPC
    }
  /* "if (! we_are_loading_a_gpi_file)" ends here.
   */
#endif /* GPC */

  /* Merge the storage class information.  */
  DECL_WEAK (newdecl) |= DECL_WEAK (olddecl);
  /* For functions, static overrides non-static.  */
  if (TREE_CODE (newdecl) == FUNCTION_DECL)
    {
      TREE_PUBLIC (newdecl) &= TREE_PUBLIC (olddecl);
      /* This is since we don't automatically
         copy the attributes of NEWDECL into OLDDECL.  */
      /* No need to worry about different_binding_level here because
         then TREE_PUBLIC (newdecl) was true.  */
      TREE_PUBLIC (olddecl) = TREE_PUBLIC (newdecl);
      /* If this clears `static', clear it in the identifier too.  */
      if (! TREE_PUBLIC (olddecl))
        TREE_PUBLIC (DECL_NAME (olddecl)) = 0;
    }
  if (DECL_EXTERNAL (newdecl))
    {
      TREE_STATIC (newdecl) = TREE_STATIC (olddecl);
      DECL_EXTERNAL (newdecl) = DECL_EXTERNAL (olddecl);
      /* An extern decl does not override previous storage class.  */
      TREE_PUBLIC (newdecl) = TREE_PUBLIC (olddecl);
      if (! DECL_EXTERNAL (newdecl))
        DECL_CONTEXT (newdecl) = DECL_CONTEXT (olddecl);
    }
  else
    {
      TREE_STATIC (olddecl) = TREE_STATIC (newdecl);
      TREE_PUBLIC (olddecl) = TREE_PUBLIC (newdecl);
    }

  /* If either decl says `inline', this fn is inline,
     unless its definition was passed already.  */
  if (DECL_INLINE (newdecl) && DECL_INITIAL (olddecl) == 0)
    DECL_INLINE (olddecl) = 1;
  DECL_INLINE (newdecl) = DECL_INLINE (olddecl);

  if (TREE_CODE (newdecl) == FUNCTION_DECL)
    {
      if (DECL_BUILT_IN (olddecl))
        {
          /* Get rid of any built-in function if new arg types don't match it
             or if we have a function definition.  */
          if (! types_match || new_is_definition)
            {
              if (! different_binding_level)
                {
                  TREE_TYPE (olddecl) = TREE_TYPE (newdecl);
                  DECL_BUILT_IN (olddecl) = 0;
                }
            }
          else
            {
              /* If redeclaring a builtin function, and not a definition,
                 it stays built in.  */
              DECL_BUILT_IN (newdecl) = 1;
              DECL_FUNCTION_CODE (newdecl) = DECL_FUNCTION_CODE (olddecl);
            }
        }
      /* Also preserve various other info from the definition.  */
      else if (! new_is_definition)
        DECL_FRAME_SIZE (newdecl) = DECL_FRAME_SIZE (olddecl);
      if (! new_is_definition)
        {
          DECL_RESULT (newdecl) = DECL_RESULT (olddecl);
          /* When called with different_binding_level set, don't copy over
             DECL_INITIAL, so that we don't accidentally change function
             declarations into function definitions.  */
          if (! different_binding_level)
            DECL_INITIAL (newdecl) = DECL_INITIAL (olddecl);
          DECL_SAVED_INSNS (newdecl) = DECL_SAVED_INSNS (olddecl);
          DECL_ARGUMENTS (newdecl) = DECL_ARGUMENTS (olddecl);
          if (DECL_INLINE (newdecl))
            DECL_ABSTRACT_ORIGIN (newdecl) = olddecl;
        }
    }
  if (different_binding_level)
    {
      /* Don't output a duplicate symbol for this declaration.  */
      TREE_ASM_WRITTEN (newdecl) = 1;
      return 0;
    }

  /* Copy most of the decl-specific fields of NEWDECL into OLDDECL.
     But preserve OLDdECL's DECL_UID.  */
  {
    register unsigned olddecl_uid = DECL_UID (olddecl);

    bcopy ((char *) newdecl + sizeof (struct tree_common),
           (char *) olddecl + sizeof (struct tree_common),
           sizeof (struct tree_decl) - sizeof (struct tree_common));
    DECL_UID (olddecl) = olddecl_uid;
  }

  return 1;
}

/* Record a decl-node X as belonging to the current lexical scope.
   Check for errors (such as an incompatible declaration for the same
   name already seen in the same scope).

   Returns either X or an old decl for the same name.
   If an old decl is returned, it may have been smashed
   to agree with what X says.  */

tree
pushdecl (x)
     tree x;
{
  register tree t;
  register tree name = DECL_NAME (x);
  register struct binding_level *b = current_binding_level;

  DECL_CONTEXT (x) = current_function_decl;
  /* A local extern declaration for a function doesn't constitute nesting.
     A local auto declaration does, since it's a forward decl
     for a nested function coming later.  */
  if (TREE_CODE (x) == FUNCTION_DECL && DECL_INITIAL (x) == 0
      && DECL_EXTERNAL (x))
    DECL_CONTEXT (x) = 0;

  if (warn_nested_externs && DECL_EXTERNAL (x) && b != global_binding_level
      && x != IDENTIFIER_IMPLICIT_DECL (name)
      /* Don't print error messages for __FUNCTION__ and __PRETTY_FUNCTION__ */
      && !DECL_IN_SYSTEM_HEADER (x))
    warning ("nested extern declaration of `%s'", IDENTIFIER_POINTER (name));

  if (name)
    {
      char *file;
      int line;
      int different_binding_level = 0;

      t = lookup_name_current_level (name);
      /* Don't type check externs here when -traditional.  This is so that
         code with conflicting declarations inside blocks will get warnings
         not errors.  X11 for instance depends on this.  */
      if (! t && DECL_EXTERNAL (x) && TREE_PUBLIC (x) && ! flag_traditional)
        {
          t = IDENTIFIER_GLOBAL_VALUE (name);
          /* Type decls at global scope don't conflict with externs declared
             inside lexical blocks.  */
          if (t && TREE_CODE (t) == TYPE_DECL)
            t = 0;
          different_binding_level = 1;
        }
      if (t != 0 && t == error_mark_node)
        /* error_mark_node is 0 for a while during initialization!  */
        {
          t = 0;
          error_with_decl (x, "`%s' used prior to declaration");
        }

      if (t != 0)
        {
          file = DECL_SOURCE_FILE (t);
          line = DECL_SOURCE_LINE (t);
        }

      /* If this decl is `static' and an implicit decl was seen previously,
         warn.  But don't complain if -traditional,
         since traditional compilers don't complain.  */
      if (! flag_traditional && TREE_PUBLIC (name)
          /* Don't test for DECL_EXTERNAL, because grokdeclarator
             sets this for all functions.  */
          && ! TREE_PUBLIC (x)
          && (TREE_CODE (x) == FUNCTION_DECL || b == global_binding_level)
          /* We used to warn also for explicit extern followed by static,
             but sometimes you need to do it that way.  */
          && IDENTIFIER_IMPLICIT_DECL (name) != 0)
        {
          pedwarn ("`%s' was declared implicitly `extern' and later `static'",
                   IDENTIFIER_POINTER (name));
          pedwarn_with_file_and_line
            (DECL_SOURCE_FILE (IDENTIFIER_IMPLICIT_DECL (name)),
             DECL_SOURCE_LINE (IDENTIFIER_IMPLICIT_DECL (name)),
             "previous declaration of `%s'",
             IDENTIFIER_POINTER (name));
        }

#ifdef GPC
      if (t != 0 && ! PASCAL_REDEFINABLE_DECL (t) && duplicate_decls (x, t, different_binding_level))
#else
      if (t != 0 && duplicate_decls (x, t, different_binding_level))
#endif /* GPC */
        {
          if (TREE_CODE (t) == PARM_DECL)
            {
              /* Don't allow more than one "real" duplicate
                 of a forward parm decl.  */
              TREE_ASM_WRITTEN (t) = TREE_ASM_WRITTEN (x);
              return t;
            }
          return t;
        }

      /* If we are processing a typedef statement, generate a whole new
         ..._TYPE node (which will be just an variant of the existing
         ..._TYPE node with identical properties) and then install the
         TYPE_DECL node generated to represent the typedef name as the
         TYPE_NAME of this brand new (duplicate) ..._TYPE node.

         The whole point here is to end up with a situation where each
         and every ..._TYPE node the compiler creates will be uniquely
         associated with AT MOST one node representing a typedef name.
         This way, even though the compiler substitutes corresponding
         ..._TYPE nodes for TYPE_DECL (i.e. "typedef name") nodes very
         early on, later parts of the compiler can always do the reverse
         translation and get back the corresponding typedef name.  For
         example, given:

                typedef struct S MY_TYPE;
                MY_TYPE object;

         Later parts of the compiler might only know that `object' was of
         type `struct S' if if were not for code just below.  With this
         code however, later parts of the compiler see something like:

                struct S' == struct S
                typedef struct S' MY_TYPE;
                struct S' object;

         And they can then deduce (from the node for type struct S') that
         the original object declaration was:

                MY_TYPE object;

         Being able to do this is important for proper support of protoize,
         and also for generating precise symbolic debugging information
         which takes full account of the programmer's (typedef) vocabulary.

         Obviously, we don't want to generate a duplicate ..._TYPE node if
         the TYPE_DECL node that we are now processing really represents a
         standard built-in type.

         Since all standard types are effectively declared at line zero
         in the source file, we can easily check to see if we are working
         on a standard type by checking the current value of lineno.  */

      if (TREE_CODE (x) == TYPE_DECL)
        {
          if (DECL_SOURCE_LINE (x) == 0)
            {
              if (TYPE_NAME (TREE_TYPE (x)) == 0)
                TYPE_NAME (TREE_TYPE (x)) = x;
            }
          else if (TREE_TYPE (x) != error_mark_node)
            {
              tree tt = TREE_TYPE (x);
              DECL_ORIGINAL_TYPE (x) = tt;
              tt = build_type_copy (tt);
              TYPE_NAME (tt) = x;
              TREE_TYPE (x) = tt;
            }
        }

      /* Multiple external decls of the same identifier ought to match.
         Check against both global declarations (when traditional) and out of
         scope (limbo) block level declarations.

         We get warnings about inline functions where they are defined.
         Avoid duplicate warnings where they are used.  */
      if (TREE_PUBLIC (x) && ! DECL_INLINE (x))
        {
          tree decl;

          if (flag_traditional && IDENTIFIER_GLOBAL_VALUE (name) != 0
              && (DECL_EXTERNAL (IDENTIFIER_GLOBAL_VALUE (name))
                  || TREE_PUBLIC (IDENTIFIER_GLOBAL_VALUE (name))))
            decl = IDENTIFIER_GLOBAL_VALUE (name);
          else if (IDENTIFIER_LIMBO_VALUE (name) != 0)
            /* Decls in limbo are always extern, so no need to check that.  */
            decl = IDENTIFIER_LIMBO_VALUE (name);
          else
            decl = 0;

          if (decl && ! comptypes (TREE_TYPE (x), TREE_TYPE (decl))
              /* If old decl is built-in, we already warned if we should.  */
              && !DECL_BUILT_IN (decl))
            {
              pedwarn_with_decl (x,
                                 "type mismatch with previous external decl");
              pedwarn_with_decl (decl, "previous external decl of `%s'");
            }
        }

      /* If a function has had an implicit declaration, and then is defined,
         make sure they are compatible.  */

      if (IDENTIFIER_IMPLICIT_DECL (name) != 0
          && IDENTIFIER_GLOBAL_VALUE (name) == 0
          && TREE_CODE (x) == FUNCTION_DECL
          && ! comptypes (TREE_TYPE (x),
                          TREE_TYPE (IDENTIFIER_IMPLICIT_DECL (name))))
        {
          warning_with_decl (x, "type mismatch with previous implicit declaration");
          warning_with_decl (IDENTIFIER_IMPLICIT_DECL (name),
                             "previous implicit declaration of `%s'");
        }

      /* In PCC-compatibility mode, extern decls of vars with no current decl
         take effect at top level no matter where they are.  */
      if (flag_traditional && DECL_EXTERNAL (x)
          && lookup_name (name) == 0)
        {
          tree type = TREE_TYPE (x);

          /* But don't do this if the type contains temporary nodes.  */
          while (type)
            {
              if (type == error_mark_node)
                break;
              if (! TREE_PERMANENT (type))
                {
                  warning_with_decl (x, "type of external `%s' is not global");
                  /* By exiting the loop early, we leave TYPE nonzero,
                     and thus prevent globalization of the decl.  */
                  break;
                }
              else if (TREE_CODE (type) == FUNCTION_TYPE
                       && TYPE_ARG_TYPES (type) != 0)
                /* The types might not be truly local,
                   but the list of arg types certainly is temporary.
                   Since prototypes are nontraditional,
                   ok not to do the traditional thing.  */
                break;
              type = TREE_TYPE (type);
            }

          if (type == 0)
            b = global_binding_level;
        }

      /* This name is new in its binding level.
         Install the new declaration and return it.  */
      if (b == global_binding_level)
        {
          /* Install a global value.  */

          /* If the first global decl has external linkage,
             warn if we later see static one.  */
          if (IDENTIFIER_GLOBAL_VALUE (name) == 0 && TREE_PUBLIC (x))
            TREE_PUBLIC (name) = 1;

          IDENTIFIER_GLOBAL_VALUE (name) = x;

          /* We no longer care about any previous block level declarations.  */
          IDENTIFIER_LIMBO_VALUE (name) = 0;

          /* Don't forget if the function was used via an implicit decl.  */
          if (IDENTIFIER_IMPLICIT_DECL (name)
              && TREE_USED (IDENTIFIER_IMPLICIT_DECL (name)))
            TREE_USED (x) = 1, TREE_USED (name) = 1;

          /* Don't forget if its address was taken in that way.  */
          if (IDENTIFIER_IMPLICIT_DECL (name)
              && TREE_ADDRESSABLE (IDENTIFIER_IMPLICIT_DECL (name)))
            TREE_ADDRESSABLE (x) = 1;

#ifndef GPC
          /* Warn about mismatches against previous implicit decl.  */
          if (IDENTIFIER_IMPLICIT_DECL (name) != 0
              /* If this real decl matches the implicit, don't complain.  */
              && ! (TREE_CODE (x) == FUNCTION_DECL
                    && (TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (x)))
                        == integer_type_node)))
            pedwarn ("`%s' was previously implicitly declared to return `int'",
                     IDENTIFIER_POINTER (name));
#endif /* not GPC */

          /* If this decl is `static' and an `extern' was seen previously,
             that is erroneous.  */
          if (TREE_PUBLIC (name)
              && ! TREE_PUBLIC (x) && ! DECL_EXTERNAL (x))
            {
              /* Okay to redeclare an ANSI built-in as static.  */
              if (t != 0 && DECL_BUILT_IN (t))
                ;
              /* Okay to declare a non-ANSI built-in as anything.  */
              else if (t != 0 && DECL_BUILT_IN_NONANSI (t))
                ;
              /* Okay to have global type decl after an earlier extern
                 declaration inside a lexical block.  */
              else if (TREE_CODE (x) == TYPE_DECL)
                ;
              else if (IDENTIFIER_IMPLICIT_DECL (name))
                pedwarn ("`%s' was declared implicitly `extern' and later `static'",
                         IDENTIFIER_POINTER (name));
              else
                pedwarn ("`%s' was declared `extern' and later `static'",
                         IDENTIFIER_POINTER (name));
            }
        }
      else
        {
          /* Here to install a non-global value.  */
          tree oldlocal = IDENTIFIER_LOCAL_VALUE (name);
          tree oldglobal = IDENTIFIER_GLOBAL_VALUE (name);
          IDENTIFIER_LOCAL_VALUE (name) = x;

          /* If this is an extern function declaration, see if we
             have a global definition or declaration for the function.  */
          if (oldlocal == 0
              && DECL_EXTERNAL (x) && !DECL_INLINE (x)
              && oldglobal != 0
              && TREE_CODE (x) == FUNCTION_DECL
              && TREE_CODE (oldglobal) == FUNCTION_DECL)
            {
              /* We have one.  Their types must agree.  */
              if (! comptypes (TREE_TYPE (x),
                               TREE_TYPE (IDENTIFIER_GLOBAL_VALUE (name))))
                pedwarn_with_decl (x, "extern declaration of `%s' doesn't match global one");
              else
                {
                  /* Inner extern decl is inline if global one is.
                     Copy enough to really inline it.  */
                  if (DECL_INLINE (oldglobal))
                    {
                      DECL_INLINE (x) = DECL_INLINE (oldglobal);
                      DECL_INITIAL (x) = (current_function_decl == oldglobal
                                          ? 0 : DECL_INITIAL (oldglobal));
                      DECL_SAVED_INSNS (x) = DECL_SAVED_INSNS (oldglobal);
                      DECL_FRAME_SIZE (x) = DECL_FRAME_SIZE (oldglobal);
                      DECL_ARGUMENTS (x) = DECL_ARGUMENTS (oldglobal);
                      DECL_RESULT (x) = DECL_RESULT (oldglobal);
                      TREE_ASM_WRITTEN (x) = TREE_ASM_WRITTEN (oldglobal);
                      DECL_ABSTRACT_ORIGIN (x) = oldglobal;
                    }
                  /* Inner extern decl is built-in if global one is.  */
                  if (DECL_BUILT_IN (oldglobal))
                    {
                      DECL_BUILT_IN (x) = DECL_BUILT_IN (oldglobal);
                      DECL_FUNCTION_CODE (x) = DECL_FUNCTION_CODE (oldglobal);
                    }
                  /* Keep the arg types from a file-scope fcn defn.  */
                  if (TYPE_ARG_TYPES (TREE_TYPE (oldglobal)) != 0
                      && DECL_INITIAL (oldglobal)
                      && TYPE_ARG_TYPES (TREE_TYPE (x)) == 0)
                    TREE_TYPE (x) = TREE_TYPE (oldglobal);
                }
            }

#if 0 /* This case is probably sometimes the right thing to do.  */
          /* If we have a local external declaration,
             then any file-scope declaration should not
             have been static.  */
          if (oldlocal == 0 && oldglobal != 0
              && !TREE_PUBLIC (oldglobal)
              && DECL_EXTERNAL (x) && TREE_PUBLIC (x))
            warning ("`%s' locally external but globally static",
                     IDENTIFIER_POINTER (name));
#endif

          /* If we have a local external declaration,
             and no file-scope declaration has yet been seen,
             then if we later have a file-scope decl it must not be static.  */
          if (oldlocal == 0
              && DECL_EXTERNAL (x)
              && TREE_PUBLIC (x))
            {
              if (oldglobal == 0)
                TREE_PUBLIC (name) = 1;

#ifndef GPC
              /* Save this decl, so that we can do type checking against
                 other decls after it falls out of scope.

                 Only save it once.  This prevents temporary decls created in
                 expand_inline_function from being used here, since this
                 will have been set when the inline function was parsed.
                 It also helps give slightly better warnings.  */
              if (IDENTIFIER_LIMBO_VALUE (name) == 0)
                IDENTIFIER_LIMBO_VALUE (name) = x;
#endif /* not GPC */
            }

          /* Warn if shadowing an argument at the top level of the body.  */
          if (oldlocal != 0 && !DECL_EXTERNAL (x)
              /* This warning doesn't apply to the parms of a nested fcn.  */
              && ! current_binding_level->parm_flag
              /* Check that this is one level down from the parms.  */
              && current_binding_level->level_chain->parm_flag
              /* Check that the decl being shadowed
                 comes from the parm level, one level up.  */
              && chain_member (oldlocal, current_binding_level->level_chain->names))
            {
              if (TREE_CODE (oldlocal) == PARM_DECL)
                pedwarn ("declaration of `%s' shadows a parameter",
                         IDENTIFIER_POINTER (name));
              else
                pedwarn ("declaration of `%s' shadows a symbol from the parameter list",
                         IDENTIFIER_POINTER (name));
            }

          /* Maybe warn if shadowing something else.  */
          else if (warn_shadow && !DECL_EXTERNAL (x)
                   /* No shadow warnings for internally generated vars.  */
                   && DECL_SOURCE_LINE (x) != 0
                   /* No shadow warnings for vars made for inlining.  */
                   && ! DECL_FROM_INLINE (x))
            {
              char *warnstring = 0;

              if (TREE_CODE (x) == PARM_DECL
                  && current_binding_level->level_chain->parm_flag)
                /* Don't warn about the parm names in function declarator
                   within a function declarator.
                   It would be nice to avoid warning in any function
                   declarator in a declaration, as opposed to a definition,
                   but there is no way to tell it's not a definition.  */
                ;
              else if (oldlocal != 0 && TREE_CODE (oldlocal) == PARM_DECL)
                warnstring = "declaration of `%s' shadows a parameter";
              else if (oldlocal != 0)
                warnstring = "declaration of `%s' shadows previous local";
              else if (IDENTIFIER_GLOBAL_VALUE (name) != 0
                       && IDENTIFIER_GLOBAL_VALUE (name) != error_mark_node)
                warnstring = "declaration of `%s' shadows global declaration";

              if (warnstring)
                warning (warnstring, IDENTIFIER_POINTER (name));
            }

          /* If storing a local value, there may already be one (inherited).
             If so, record it for restoration when this binding level ends.  */
          if (oldlocal != 0)
            b->shadowed = tree_cons (name, oldlocal, b->shadowed);
        }

      /* Keep count of variables in this level with incomplete type.  */
      if (TYPE_SIZE (TREE_TYPE (x)) == 0)
        ++b->n_incomplete;
    }

  /* Put decls on list in reverse order.
     We will reverse them later if necessary.  */
  TREE_CHAIN (x) = b->names;
  b->names = x;

  return x;
}

/* Like pushdecl, only it places X in GLOBAL_BINDING_LEVEL, if appropriate.  */

tree
pushdecl_top_level (x)
     tree x;
{
  register tree t;
  register struct binding_level *b = current_binding_level;

  current_binding_level = global_binding_level;
  t = pushdecl (x);
  current_binding_level = b;
  return t;
}

/* Generate an implicit declaration for identifier FUNCTIONID
   as a function of type int ().  Print a warning if appropriate.  */

tree
implicitly_declare (functionid)
     tree functionid;
{
  register tree decl;
  int traditional_warning = 0;
  /* Only one "implicit declaration" warning per identifier.  */
  int implicit_warning;

  /* Save the decl permanently so we can warn if definition follows.  */
  push_obstacks_nochange ();
  end_temporary_allocation ();

  /* We used to reuse an old implicit decl here,
     but this loses with inline functions because it can clobber
     the saved decl chains.  */
/*  if (IDENTIFIER_IMPLICIT_DECL (functionid) != 0)
    decl = IDENTIFIER_IMPLICIT_DECL (functionid);
  else  */
    decl = build_decl (FUNCTION_DECL, functionid, default_function_type);

  /* Warn of implicit decl following explicit local extern decl.
     This is probably a program designed for traditional C.  */
  if (TREE_PUBLIC (functionid) && IDENTIFIER_GLOBAL_VALUE (functionid) == 0)
    traditional_warning = 1;

  /* Warn once of an implicit declaration.  */
  implicit_warning = (IDENTIFIER_IMPLICIT_DECL (functionid) == 0);

  DECL_EXTERNAL (decl) = 1;
  TREE_PUBLIC (decl) = 1;

  /* Record that we have an implicit decl and this is it.  */
  IDENTIFIER_IMPLICIT_DECL (functionid) = decl;

  /* ANSI standard says implicit declarations are in the innermost block.
     So we record the decl in the standard fashion.
     If flag_traditional is set, pushdecl does it top-level.  */
  pushdecl (decl);

  /* This is a no-op in c-lang.c or something real in objc-actions.c.  */
  maybe_objc_check_decl (decl);

  rest_of_decl_compilation (decl, NULL_PTR, 0, 0);

  if (mesg_implicit_function_declaration && implicit_warning)
    {
      if (mesg_implicit_function_declaration == 2)
        error ("implicit declaration of function `%s'",
                 IDENTIFIER_POINTER (functionid));
      else
        warning ("implicit declaration of function `%s'",
                 IDENTIFIER_POINTER (functionid));
    }
  else if (warn_traditional && traditional_warning)
    warning ("function `%s' was previously declared within a block",
             IDENTIFIER_POINTER (functionid));

  /* Write a record describing this implicit function declaration to the
     prototypes file (if requested).  */

  gen_aux_info_record (decl, 0, 1, 0);

  pop_obstacks ();

  return decl;
}

/* Return zero if the declaration NEWDECL is valid
   when the declaration OLDDECL (assumed to be for the same name)
   has already been seen.
   Otherwise return an error message format string with a %s
   where the identifier should go.  */

static char *
redeclaration_error_message (newdecl, olddecl)
     tree newdecl, olddecl;
{
  if (TREE_CODE (newdecl) == TYPE_DECL)
    {
      if (flag_traditional && TREE_TYPE (newdecl) == TREE_TYPE (olddecl))
        return 0;
      /* pushdecl creates distinct types for TYPE_DECLs by calling
         build_type_copy, so the above comparison generally fails.  We do
         another test against the TYPE_MAIN_VARIANT of the olddecl, which
         is equivalent to what this code used to do before the build_type_copy
         call.  The variant type distinction should not matter for traditional
         code, because it doesn't have type qualifiers.  */
      if (flag_traditional
          && TYPE_MAIN_VARIANT (TREE_TYPE (olddecl)) == TREE_TYPE (newdecl))
        return 0;
      if (DECL_IN_SYSTEM_HEADER (olddecl) || DECL_IN_SYSTEM_HEADER (newdecl))
        return 0;
      return "redefinition of `%s'";
    }
  else if (TREE_CODE (newdecl) == FUNCTION_DECL)
    {
      /* Declarations of functions can insist on internal linkage
         but they can't be inconsistent with internal linkage,
         so there can be no error on that account.
         However defining the same name twice is no good.  */
      if (DECL_INITIAL (olddecl) != 0 && DECL_INITIAL (newdecl) != 0
          /* However, defining once as extern inline and a second
             time in another way is ok.  */
          && !(DECL_INLINE (olddecl) && DECL_EXTERNAL (olddecl)
               && !(DECL_INLINE (newdecl) && DECL_EXTERNAL (newdecl))))
        return "redefinition of `%s'";
      return 0;
    }
#ifdef GPC
  else if (pascal_global_bindings_p ())
#else /* not GPC */
  else if (current_binding_level == global_binding_level)
#endif
    {
      /* Objects declared at top level:  */
      /* If at least one is a reference, it's ok.  */
      if (DECL_EXTERNAL (newdecl) || DECL_EXTERNAL (olddecl))
        return 0;
      /* Reject two definitions.  */
      if (DECL_INITIAL (olddecl) != 0 && DECL_INITIAL (newdecl) != 0)
        return "redefinition of `%s'";
      /* Now we have two tentative defs, or one tentative and one real def.  */
      /* Insist that the linkage match.  */
      if (TREE_PUBLIC (olddecl) != TREE_PUBLIC (newdecl))
        return "conflicting declarations of `%s'";
      return 0;
    }
  else if (current_binding_level->parm_flag
           && TREE_ASM_WRITTEN (olddecl) && !TREE_ASM_WRITTEN (newdecl))
    return 0;
  else
    {
      /* Newdecl has block scope.  If olddecl has block scope also, then
         reject two definitions, and reject a definition together with an
         external reference.  Otherwise, it is OK, because newdecl must
         be an extern reference to olddecl.  */
      if (!(DECL_EXTERNAL (newdecl) && DECL_EXTERNAL (olddecl))
          && DECL_CONTEXT (newdecl) == DECL_CONTEXT (olddecl))
        return "redeclaration of `%s'";
      return 0;
    }
}

#ifdef GPC
/* Declare a label named ID.  TOP_LEVEL_LABELS is nonzero when
 * this label is declared in the main program and can be `goto'ed
 * from outside.
 *
 * This function is used only to keep track of labels declared
 * in a label statement.
 */
void
declare_label (id, top_level_labels)
     tree id;
     int top_level_labels;
{
  tree link;
  tree label;

  for (link = declared_labels; link; link = TREE_CHAIN (link))
    if (TREE_VALUE (link) == id)
      {
        warning ("label `%s' already declared at current level",
                 IDENTIFIER_POINTER (id));
        return;
      }

  /* Prepend the declared label list with
   * TREE_VALUE set to the name of the label.
   * TREE_PURPOSE is not used.
   */
  declared_labels = tree_cons (NULL_TREE, id, declared_labels);

  /* Mark the label as a possible target of a non-local goto.
   */
  if (top_level_labels)
    {
      char *jmpbuf_name;
      tree jmpbuf_id, jmpbuf_type;

      label = shadow_label (id);
      C_DECLARED_LABEL_FLAG (label) = 1;
      main_program_nonlocal_labels = tree_cons (NULL_TREE, label,
                                                main_program_nonlocal_labels);
      LABEL_PRESERVE_P (label_rtx (label)) = 1;

      if (main_program_first_label_num < 0)
        main_program_first_label_num = get_first_label_num ();

      /* Create a jmp_buf variable `jmpbuf_Id' for this label.
       * `jmp_buf' is in fact `unsigned[5]' (see ../expr.c).
       */
      jmpbuf_name = concat ("jmpbuf_", IDENTIFIER_POINTER (id), NULL_PTR);
      jmpbuf_id = get_identifier (jmpbuf_name);
      jmpbuf_type = build_array_type (unsigned_type_node,
                                      build_index_type (build_int_2 (5, 0)));
      declare_vars (build_tree_list (NULL_TREE, jmpbuf_id), jmpbuf_type,
                    NULL_TREE, NULL_PTR, 0, 0, NULL_TREE);
      free (jmpbuf_name);
    }
  else
    {
      label = shadow_label (id);
      C_DECLARED_LABEL_FLAG (label) = 1;
      declare_nonlocal_label (label);
    }
}
#endif /* GPC */

/* Get the LABEL_DECL corresponding to identifier ID as a label.
   Create one if none exists so far for the current function.
   This function is called for both label definitions and label references.  */

tree
lookup_label (id)
     tree id;
{
  register tree decl = IDENTIFIER_LABEL_VALUE (id);

#ifndef GPC
  if (current_function_decl == 0)
    {
      error ("label %s referenced outside of any function",
             IDENTIFIER_POINTER (id));
      return 0;
    }
#endif

#ifdef GPC
  /* If this is a label located in the Pascal main program with a nonlocal
   * `goto' to it, get its specifications from `main_program_nonlocal_labels'.
   */
  {
    tree link;
    int local = 0;
    for (link = declared_labels; link && ! local; link = TREE_CHAIN (link))
      if (TREE_VALUE (link) == id)
        local = 1;

    if (! local)
      for (link = main_program_nonlocal_labels; link;
           link = TREE_CHAIN (link))
        {
          if (DECL_NAME (TREE_VALUE (link)) == id)
            {
              tree scan;

              end_temporary_allocation ();

              decl = TREE_VALUE (link);
              label_rtx (decl);

              /* Look whether we already handled this label.
               */
              for (scan = named_labels; scan; scan = TREE_CHAIN (scan))
                if (TREE_VALUE (scan) == decl)
                  {
                    resume_temporary_allocation ();
                    return decl;
                  }

              /* Say where one reference is to the label,
               * for the sake of the error if it is not defined.
               */
              DECL_SOURCE_LINE (decl) = lineno;
              DECL_SOURCE_FILE (decl) = input_filename;

              /* Mark this label as declared in the main program.
               */
              DECL_INITIAL (decl) = main_program_context;

              named_labels = tree_cons (NULL_TREE, decl, named_labels);
              main_program_named_labels
                  = tree_cons (NULL_TREE, decl, main_program_named_labels);

              resume_temporary_allocation ();
              return decl;
           }
        }
  }
#endif /* GPC */

  /* Use a label already defined or ref'd with this name.  */
  if (decl != 0)
    {
      /* But not if it is inherited and wasn't declared to be inheritable.  */
      if (DECL_CONTEXT (decl) != current_function_decl
          && ! C_DECLARED_LABEL_FLAG (decl))
        return shadow_label (id);
      return decl;
    }

  decl = build_decl (LABEL_DECL, id, void_type_node);

  /* Make sure every label has an rtx.  */
  label_rtx (decl);

  /* A label not explicitly declared must be local to where it's ref'd.  */
  DECL_CONTEXT (decl) = current_function_decl;

#ifdef GPC
  /* Check whether this label appeared in a `Label' declaration section,
   * and put it into the main_program_context if it belongs there.
   */
  {
    tree link, found = NULL_TREE;
    struct binding_level *level;
    for (link = declared_labels;
         link && ! found;
         link = TREE_CHAIN (link))
      {
        if (TREE_VALUE (link) == id)
          found = id;
      }
    for (level = label_level_chain;
         level && ! found;
         level = level->level_chain)
      {
        for (link = level->declared_labels;
             link && ! found;
             link = TREE_CHAIN (link))
          {
            if (TREE_VALUE (link) == id)
              {
                found = id;
                if (! (current_function_decl
                       && DECL_NAME (current_function_decl)
                          == main_program_name))
                  {
                    /* Mark this label as declared in the main program.
                     */
                    DECL_INITIAL (decl) = main_program_context;
                  }
              }
          }
      }
    if (! found)
      error ("undeclared label `%s'", IDENTIFIER_POINTER (id));
  }
#endif /* GPC */

  DECL_MODE (decl) = VOIDmode;

  /* Say where one reference is to the label,
     for the sake of the error if it is not defined.  */
  DECL_SOURCE_LINE (decl) = lineno;
  DECL_SOURCE_FILE (decl) = input_filename;

  IDENTIFIER_LABEL_VALUE (id) = decl;

  named_labels = tree_cons (NULL_TREE, decl, named_labels);

  return decl;
}

/* Make a label named NAME in the current function,
   shadowing silently any that may be inherited from containing functions
   or containing scopes.

   Note that valid use, if the label being shadowed
   comes from another scope in the same function,
   requires calling declare_nonlocal_label right away.  */

tree
shadow_label (name)
     tree name;
{
  register tree decl = IDENTIFIER_LABEL_VALUE (name);

  if (decl != 0)
    {
      register tree dup;

      /* Check to make sure that the label hasn't already been declared
         at this label scope */
      for (dup = named_labels; dup; dup = TREE_CHAIN (dup))
        if (TREE_VALUE (dup) == decl)
          {
            error ("duplicate label declaration `%s'",
                   IDENTIFIER_POINTER (name));
            error_with_decl (TREE_VALUE (dup),
                             "this is a previous declaration");
            /* Just use the previous declaration.  */
            return lookup_label (name);
          }

      shadowed_labels = tree_cons (NULL_TREE, decl, shadowed_labels);
      IDENTIFIER_LABEL_VALUE (name) = decl = 0;
    }

  return lookup_label (name);
}

/* Define a label, specifying the location in the source file.
   Return the LABEL_DECL node for the label, if the definition is valid.
   Otherwise return 0.  */

tree
define_label (filename, line, name)
     char *filename;
     int line;
     tree name;
{
  tree decl = lookup_label (name);

  /* If label with this name is known from an outer context, shadow it.  */
#ifdef GPC
  if (decl != 0
      && DECL_INITIAL (decl) != main_program_context
      && DECL_CONTEXT (decl) != current_function_decl)
#else /* not GPC */
  if (decl != 0 && DECL_CONTEXT (decl) != current_function_decl)
#endif /* not GPC */
    {
      shadowed_labels = tree_cons (NULL_TREE, decl, shadowed_labels);
      IDENTIFIER_LABEL_VALUE (name) = 0;
      decl = lookup_label (name);
    }

#ifdef GPC
  if (DECL_INITIAL (decl) == main_program_context)
    DECL_INITIAL (decl) = NULL_TREE;
#endif

  if (DECL_INITIAL (decl) != 0)
    {
      error ("duplicate label `%s'", IDENTIFIER_POINTER (name));
      return 0;
    }
  else
    {
      /* Mark label as having been defined.  */
      DECL_INITIAL (decl) = error_mark_node;
      /* Say where in the source.  */
      DECL_SOURCE_FILE (decl) = filename;
      DECL_SOURCE_LINE (decl) = line;
      return decl;
    }
}

/* Return the list of declarations of the current level.
   Note that this list is in reverse order unless/until
   you nreverse it; and when you do nreverse it, you must
   store the result back using `storedecls' or you will lose.  */

tree
getdecls ()
{
  return current_binding_level->names;
}

/* Return the list of type-tags (for structs, etc) of the current level.  */

tree
gettags ()
{
  return current_binding_level->tags;
}

/* Store the list of declarations of the current level.
   This is done for the parameter declarations of a function being defined,
   after they are modified in the light of any missing parameters.  */

static void
storedecls (decls)
     tree decls;
{
  current_binding_level->names = decls;
}

/* Similarly, store the list of tags of the current level.  */

static void
storetags (tags)
     tree tags;
{
  current_binding_level->tags = tags;
}

/* Given NAME, an IDENTIFIER_NODE,
   return the structure (or union or enum) definition for that name.
   Searches binding levels from BINDING_LEVEL up to the global level.
   If THISLEVEL_ONLY is nonzero, searches only the specified context
   (but skips any tag-transparent contexts to find one that is
   meaningful for tags).
   CODE says which kind of type the caller wants;
   it is RECORD_TYPE or UNION_TYPE or ENUMERAL_TYPE.
   If the wrong kind of type is found, an error is reported.  */

static tree
lookup_tag (code, name, binding_level, thislevel_only)
     enum tree_code code;
     struct binding_level *binding_level;
     tree name;
     int thislevel_only;
{
  register struct binding_level *level;

  for (level = binding_level; level; level = level->level_chain)
    {
      register tree tail;
      for (tail = level->tags; tail; tail = TREE_CHAIN (tail))
        {
          if (TREE_PURPOSE (tail) == name)
            {
#ifdef GPC
              /* The forward pointer will point to CODE type */
              if (code != LANG_TYPE
                  && TREE_CODE (TREE_VALUE (tail)) == LANG_TYPE)
                TREE_SET_CODE (TREE_VALUE (tail), code);

              /* We found a resolved forward pointer, but are not
               * looking for it.
               */
              if (code == LANG_TYPE
                  && TREE_CODE (TREE_VALUE (tail)) != LANG_TYPE)
                return NULL_TREE;
#endif
              if (TREE_CODE (TREE_VALUE (tail)) != code)
                {
                  /* Definition isn't the kind we were looking for.  */
                  pending_invalid_xref = name;
                  pending_invalid_xref_file = input_filename;
                  pending_invalid_xref_line = lineno;
                }
              return TREE_VALUE (tail);
            }
        }
      if (thislevel_only && ! level->tag_transparent)
        return NULL_TREE;
    }
  return NULL_TREE;
}

/* Print an error message now
   for a recent invalid struct, union or enum cross reference.
   We don't print them immediately because they are not invalid
   when used in the `struct foo;' construct for shadowing.  */

void
pending_xref_error ()
{
  if (pending_invalid_xref != 0)
    error_with_file_and_line (pending_invalid_xref_file,
                              pending_invalid_xref_line,
                              "`%s' defined as wrong kind of tag",
                              IDENTIFIER_POINTER (pending_invalid_xref));
  pending_invalid_xref = 0;
}

/* Given a type, find the tag that was defined for it and return the tag name.
   Otherwise return 0.  */

static tree
lookup_tag_reverse (type)
     tree type;
{
  register struct binding_level *level;

  for (level = current_binding_level; level; level = level->level_chain)
    {
      register tree tail;
      for (tail = level->tags; tail; tail = TREE_CHAIN (tail))
        {
          if (TREE_VALUE (tail) == type)
            return TREE_PURPOSE (tail);
        }
    }
  return NULL_TREE;
}

/* Look up NAME in the current binding level and its superiors
   in the namespace of variables, functions and typedefs.
   Return a ..._DECL node of some kind representing its definition,
   or return 0 if it is undefined.  */

tree
lookup_name (name)
     tree name;
{
  register tree val;
  if (current_binding_level != global_binding_level
      && IDENTIFIER_LOCAL_VALUE (name))
    val = IDENTIFIER_LOCAL_VALUE (name);
  else
    val = IDENTIFIER_GLOBAL_VALUE (name);
  return val;
}

/* Similar to `lookup_name' but look only at current binding level.  */

tree
lookup_name_current_level (name)
     tree name;
{
  register tree t;

  if (current_binding_level == global_binding_level)
    return IDENTIFIER_GLOBAL_VALUE (name);

  if (IDENTIFIER_LOCAL_VALUE (name) == 0)
    return 0;

  for (t = current_binding_level->names; t; t = TREE_CHAIN (t))
    if (DECL_NAME (t) == name)
      break;

  return t;
}

#ifdef GPC
/* Return a new Boolean type node with precision PRECISION.
 */
tree
create_boolean_type_node (precision)
     unsigned precision;
{
  tree foo_boolean_type_node = make_node (BOOLEAN_TYPE);
  TYPE_PRECISION (foo_boolean_type_node) = precision;
  TYPE_SIZE (foo_boolean_type_node) = 0;
#ifdef EGCS
  TYPE_SIZE_UNIT (foo_boolean_type_node) = 0;
#endif /* EGCS */
  fixup_unsigned_type (foo_boolean_type_node);
  TREE_UNSIGNED (foo_boolean_type_node) = 1;
  return foo_boolean_type_node;
}
#endif /* GPC */

/* Create the predefined scalar types of C,
   and some nodes representing standard constants (0, 1, (void *) 0).
   Initialize the global binding level.
   Make definitions for built-in primitive functions.  */

void
init_decl_processing ()
{
  register tree endlink;
  /* Either char* or void*.  */
  tree traditional_ptr_type_node;
  /* Data types of memcpy and strlen.  */
  tree memcpy_ftype, memset_ftype, strlen_ftype;
  tree void_ftype_any, ptr_ftype_void, ptr_ftype_ptr;
  int wchar_type_size;
  tree temp;
  tree array_domain_type;

#ifdef GPC
#if 0 /* unused */
  with_element_chain = NULL_TREE;
#endif /* 0 */
  declared_labels = NULL_TREE;

  /* Override DEFAULT_SIGNED_CHAR here, Pascal likes unsigned chars.
   * Signed chars used only if -fsigned-char is given on command line
   */
  flag_signed_char = force_signed_chars;

  if (flag_signed_char && pedantic)
    pedwarn ("ISO Pascal wants to use unsigned characters");
#endif /* GPC */

  current_function_decl = NULL;
  named_labels = NULL;
  current_binding_level = NULL_BINDING_LEVEL;
  free_binding_level = NULL_BINDING_LEVEL;
  pushlevel (0);        /* make the binding_level structure for global names */
  global_binding_level = current_binding_level;

#ifdef GPC
  /* Unique tokens to tell grokdeclarator what we are defining.
   */
  const_id     = get_identifier ("<const>");
  type_id      = get_identifier ("<type>");
  inline_id    = get_identifier ("<inline>");
  volatile_id  = get_identifier ("<volatile>");
  varparm_id   = get_identifier ("<varparm>");
  auto_id      = get_identifier ("<auto>");
  extern_id    = get_identifier ("<extern>");
  asmname_id   = get_identifier ("<asmname>");
  register_id  = get_identifier ("<register>");
  static_id    = get_identifier ("<static>");
  attribute_id = get_identifier ("<attribute>");

  /* Initialize field identifiers for strings and schemata.
   */
  string_id    = get_identifier ("schema$");
  schema_id    = get_identifier ("schema$");

  /* Initialize known directives.
   */
  d_forward    = get_identifier ("Forward");
  d_external   = get_identifier ("External");
  d_extern     = get_identifier ("Extern");
  d_c_lang     = get_identifier ("C");
  d_c_lang1    = get_identifier ("C_language");
  d_asmname    = get_identifier ("Asmname");
  d_attribute  = get_identifier ("Attribute");
  d_o_override = get_identifier ("Override");
  d_static     = get_identifier ("Static");

  /* Initialize export "all" mark */
  export_all   = get_identifier ("<all>");

#define GET_IDENTIFIER(str) get_identifier_with_blank(str)

/* Pascal required identifiers are not reserved words, so
 * handle them differently
 */
#define PUSHDECL

#else /* not GPC */
#define GET_IDENTIFIER(str) get_identifier(str)
#define PUSHDECL pushdecl
#endif /* not GPC */

  /* Define `int' and `char' first so that dbx will output them first.  */

  integer_type_node = make_signed_type (INT_TYPE_SIZE);
#ifdef GPC
  /* GPC has no ridpointers.
     This is here for wchar_type_nodes and other stuff that
     want to get IDENTIFIER_GLOBAL_VALUE
   */
  pushdecl (build_decl (TYPE_DECL, GET_IDENTIFIER("int"),
                      integer_type_node));
#else
  pushdecl (build_decl (TYPE_DECL, ridpointers[(int) RID_INT],
                        integer_type_node));
#endif /* GPC */

  /* Define `char', which is like either `signed char' or `unsigned char'
     but not the same as either.  */

#ifdef GPC
  /* Pascal CHAR type */
  char_type_node = make_node (CHAR_TYPE);
  TYPE_PRECISION (char_type_node) = CHAR_TYPE_SIZE;
  TYPE_SIZE (char_type_node) = 0;
#ifdef EGCS
  TYPE_SIZE_UNIT (char_type_node) = 0;
#endif /* EGCS */
  if (flag_signed_char)
    {
      fixup_signed_type (char_type_node);
      TREE_UNSIGNED (char_type_node) = 0;
    }
  else
    {
      fixup_unsigned_type (char_type_node);
      TREE_UNSIGNED (char_type_node) = 1;
    }

  /* GPC:
     Some get_identifier() calls changed to GET_IDENTIFIER() macros.

     Some pushdecl call changed to PUSHDECL macros
     */
#else /* not GPC */
  char_type_node
    = (flag_signed_char
       ? make_signed_type (CHAR_TYPE_SIZE)
       : make_unsigned_type (CHAR_TYPE_SIZE));
#endif /* not GPC */
  pushdecl (build_decl (TYPE_DECL, GET_IDENTIFIER ("char"),
                        char_type_node));

  long_integer_type_node = make_signed_type (LONG_TYPE_SIZE);
  pushdecl (build_decl (TYPE_DECL, GET_IDENTIFIER ("long int"),
                        long_integer_type_node));

  unsigned_type_node = make_unsigned_type (INT_TYPE_SIZE);
  pushdecl (build_decl (TYPE_DECL, GET_IDENTIFIER ("unsigned int"),
                        unsigned_type_node));

  long_unsigned_type_node = make_unsigned_type (LONG_TYPE_SIZE);
  pushdecl (build_decl (TYPE_DECL, GET_IDENTIFIER ("long unsigned int"),
                        long_unsigned_type_node));

  long_long_integer_type_node = make_signed_type (LONG_LONG_TYPE_SIZE);
  pushdecl (build_decl (TYPE_DECL, GET_IDENTIFIER ("long long int"),
                        long_long_integer_type_node));

  long_long_unsigned_type_node = make_unsigned_type (LONG_LONG_TYPE_SIZE);
  pushdecl (build_decl (TYPE_DECL, GET_IDENTIFIER ("long long unsigned int"),
                        long_long_unsigned_type_node));

  short_integer_type_node = make_signed_type (SHORT_TYPE_SIZE);
  pushdecl (build_decl (TYPE_DECL, GET_IDENTIFIER ("short int"),
                        short_integer_type_node));

  short_unsigned_type_node = make_unsigned_type (SHORT_TYPE_SIZE);
  pushdecl (build_decl (TYPE_DECL, GET_IDENTIFIER ("short unsigned int"),
                        short_unsigned_type_node));

  /* `unsigned long' is the standard type for sizeof.
     Traditionally, use a signed type.
     Note that stddef.h uses `unsigned long',
     and this must agree, even of long and int are the same size.  */
#ifdef EGCS
  set_sizetype
    (TREE_TYPE (IDENTIFIER_GLOBAL_VALUE (GET_IDENTIFIER (SIZE_TYPE))));
  if (flag_traditional && TREE_UNSIGNED (sizetype))
    set_sizetype (signed_type (sizetype));
#else /* not EGCS */
  sizetype = TREE_TYPE (IDENTIFIER_GLOBAL_VALUE (GET_IDENTIFIER (SIZE_TYPE)));
  if (flag_traditional && TREE_UNSIGNED (sizetype))
    sizetype = signed_type (sizetype);
#endif /* not EGCS */

  ptrdiff_type_node
    = TREE_TYPE (IDENTIFIER_GLOBAL_VALUE (GET_IDENTIFIER (PTRDIFF_TYPE)));

  TREE_TYPE (TYPE_SIZE (integer_type_node)) = sizetype;
  TREE_TYPE (TYPE_SIZE (char_type_node)) = sizetype;
  TREE_TYPE (TYPE_SIZE (unsigned_type_node)) = sizetype;
  TREE_TYPE (TYPE_SIZE (long_unsigned_type_node)) = sizetype;
  TREE_TYPE (TYPE_SIZE (long_integer_type_node)) = sizetype;
  TREE_TYPE (TYPE_SIZE (long_long_integer_type_node)) = sizetype;
  TREE_TYPE (TYPE_SIZE (long_long_unsigned_type_node)) = sizetype;
  TREE_TYPE (TYPE_SIZE (short_integer_type_node)) = sizetype;
  TREE_TYPE (TYPE_SIZE (short_unsigned_type_node)) = sizetype;

  error_mark_node = make_node (ERROR_MARK);
  TREE_TYPE (error_mark_node) = error_mark_node;

  /* Define both `signed char' and `unsigned char'.  */
  signed_char_type_node = make_signed_type (CHAR_TYPE_SIZE);
  pushdecl (build_decl (TYPE_DECL, GET_IDENTIFIER ("signed char"),
                        signed_char_type_node));

  unsigned_char_type_node = make_unsigned_type (CHAR_TYPE_SIZE);
  pushdecl (build_decl (TYPE_DECL, GET_IDENTIFIER ("unsigned char"),
                        unsigned_char_type_node));

  intQI_type_node = make_signed_type (GET_MODE_BITSIZE (QImode));
  pushdecl (build_decl (TYPE_DECL, NULL_TREE, intQI_type_node));

  intHI_type_node = make_signed_type (GET_MODE_BITSIZE (HImode));
  pushdecl (build_decl (TYPE_DECL, NULL_TREE, intHI_type_node));

  intSI_type_node = make_signed_type (GET_MODE_BITSIZE (SImode));
  pushdecl (build_decl (TYPE_DECL, NULL_TREE, intSI_type_node));

  intDI_type_node = make_signed_type (GET_MODE_BITSIZE (DImode));
  pushdecl (build_decl (TYPE_DECL, NULL_TREE, intDI_type_node));

  unsigned_intQI_type_node = make_unsigned_type (GET_MODE_BITSIZE (QImode));
  pushdecl (build_decl (TYPE_DECL, NULL_TREE, unsigned_intQI_type_node));

  unsigned_intHI_type_node = make_unsigned_type (GET_MODE_BITSIZE (HImode));
  pushdecl (build_decl (TYPE_DECL, NULL_TREE, unsigned_intHI_type_node));

  unsigned_intSI_type_node = make_unsigned_type (GET_MODE_BITSIZE (SImode));
  pushdecl (build_decl (TYPE_DECL, NULL_TREE, unsigned_intSI_type_node));

  unsigned_intDI_type_node = make_unsigned_type (GET_MODE_BITSIZE (DImode));
  pushdecl (build_decl (TYPE_DECL, NULL_TREE, unsigned_intDI_type_node));

  float_type_node = make_node (REAL_TYPE);
  TYPE_PRECISION (float_type_node) = FLOAT_TYPE_SIZE;
  PUSHDECL (build_decl (TYPE_DECL, ridpointers[(int) RID_FLOAT],
                        float_type_node));
  layout_type (float_type_node);

  double_type_node = make_node (REAL_TYPE);
  if (flag_short_double)
    TYPE_PRECISION (double_type_node) = FLOAT_TYPE_SIZE;
  else
    TYPE_PRECISION (double_type_node) = DOUBLE_TYPE_SIZE;
  PUSHDECL (build_decl (TYPE_DECL, ridpointers[(int) RID_DOUBLE],
                        double_type_node));
  layout_type (double_type_node);

  long_double_type_node = make_node (REAL_TYPE);
  TYPE_PRECISION (long_double_type_node) = LONG_DOUBLE_TYPE_SIZE;
  pushdecl (build_decl (TYPE_DECL, GET_IDENTIFIER ("long double"),
                        long_double_type_node));
  layout_type (long_double_type_node);

  complex_integer_type_node = make_node (COMPLEX_TYPE);
  pushdecl (build_decl (TYPE_DECL, GET_IDENTIFIER ("complex int"),
                        complex_integer_type_node));
  TREE_TYPE (complex_integer_type_node) = integer_type_node;
  layout_type (complex_integer_type_node);

  complex_float_type_node = make_node (COMPLEX_TYPE);
  pushdecl (build_decl (TYPE_DECL, GET_IDENTIFIER ("complex float"),
                        complex_float_type_node));
  TREE_TYPE (complex_float_type_node) = float_type_node;
  layout_type (complex_float_type_node);

  complex_double_type_node = make_node (COMPLEX_TYPE);
  pushdecl (build_decl (TYPE_DECL, GET_IDENTIFIER ("complex double"),
                        complex_double_type_node));
  TREE_TYPE (complex_double_type_node) = double_type_node;
  layout_type (complex_double_type_node);

  complex_long_double_type_node = make_node (COMPLEX_TYPE);
  pushdecl (build_decl (TYPE_DECL, GET_IDENTIFIER ("complex long double"),
                        complex_long_double_type_node));
  TREE_TYPE (complex_long_double_type_node) = long_double_type_node;
  layout_type (complex_long_double_type_node);

  wchar_type_node
    = TREE_TYPE (IDENTIFIER_GLOBAL_VALUE (GET_IDENTIFIER (WCHAR_TYPE)));
  wchar_type_size = TYPE_PRECISION (wchar_type_node);
  signed_wchar_type_node = signed_type (wchar_type_node);
  unsigned_wchar_type_node = unsigned_type (wchar_type_node);

  integer_zero_node = build_int_2 (0, 0);
  TREE_TYPE (integer_zero_node) = integer_type_node;
  integer_one_node = build_int_2 (1, 0);
  TREE_TYPE (integer_one_node) = integer_type_node;

  boolean_type_node = integer_type_node;
  boolean_true_node = integer_one_node;
  boolean_false_node = integer_zero_node;

  size_zero_node = build_int_2 (0, 0);
  TREE_TYPE (size_zero_node) = sizetype;
  size_one_node = build_int_2 (1, 0);
  TREE_TYPE (size_one_node) = sizetype;

  void_type_node = make_node (VOID_TYPE);
  PUSHDECL (build_decl (TYPE_DECL,
                        ridpointers[(int) RID_VOID], void_type_node));
  layout_type (void_type_node); /* Uses integer_zero_node */
  /* We are not going to have real types in C with less than byte alignment,
     so we might as well not have any types that claim to have it.  */
  TYPE_ALIGN (void_type_node) = BITS_PER_UNIT;

#ifdef GPC
  /* Initialize module gpc-util.c
   *
   * Call this after integer_type_node & integer_zero_node is set up,
   * but before any new GPC types are initialized
   */
  init_util ();

  /* Pascal types.
   */

  /* Boolean types.
   */
  boolean_type_node = create_boolean_type_node (1);
  byte_boolean_type_node = create_boolean_type_node (TYPE_PRECISION (unsigned_char_type_node));
  short_boolean_type_node = create_boolean_type_node (TYPE_PRECISION (short_unsigned_type_node));
  word_boolean_type_node = create_boolean_type_node (TYPE_PRECISION (unsigned_type_node));
  long_boolean_type_node = create_boolean_type_node (TYPE_PRECISION (long_unsigned_type_node));
  long_long_boolean_type_node = create_boolean_type_node (TYPE_PRECISION (long_long_unsigned_type_node));

  /* Make sure that TYPE_POINTER_TO (boolean_type_node) is filled.
     This is for SET_TYPE to work */
  (void) build_pointer_type (boolean_type_node);

  /* TEXT is a FILE_TYPE node if it's TYPE_FILE_TEXT is not NULL_TREE
     and the file buffer size is one character. */

  text_type_node = make_node (FILE_TYPE);
  TYPE_FILE_TEXT (text_type_node) = integer_one_node;
  /* By default the file is not external */
  TREE_TYPE (text_type_node) = char_type_node;
  TYPE_SIZE (text_type_node) = 0;
#ifdef EGCS
  TYPE_SIZE_UNIT (text_type_node) = 0;
#endif /* EGCS */
  layout_type (text_type_node);
  TYPE_SIZE (text_type_node) = size_of_file_type;
#ifdef EGCS
  TYPE_SIZE_UNIT (text_type_node) = size_unit_of_file_type;
#endif /* EGCS */

  /* Extended Pascal node for COMPLEX type */
  /* @@@@ Fixme: Used to be different, now use the node initialized above */
  complex_type_node = complex_double_type_node;

  /* values for some Pascal nodes */
  boolean_false_node = TYPE_MIN_VALUE (boolean_type_node);
  TREE_TYPE (boolean_false_node) = boolean_type_node;
  boolean_true_node = TYPE_MAX_VALUE (boolean_type_node);
  TREE_TYPE (boolean_true_node) = boolean_type_node;

  integer_maxint_node = copy_node (TYPE_MAX_VALUE (integer_type_node));
  TREE_TYPE (integer_maxint_node) = integer_type_node;

#if 0 /* @@@ in module struct */
  input_file_node  = NULL_TREE; /* redefined later */
  output_file_node = NULL_TREE; /* redefined later */
#endif

  real_zero_node = build_real (double_type_node, GPC_GET_REAL (0.0));
  real_half_node = build_real (double_type_node, GPC_GET_REAL (0.5));

  /* Extended Pascal nodes */
  /* Check float.h for DBL_MAX & DBL_MIN & DBL_EPSILON */

  real_max_node = build_real (double_type_node, GPC_GET_REAL (DBL_MAX));
  TREE_TYPE (real_max_node) = double_type_node;

  real_min_node = build_real (double_type_node, GPC_GET_REAL (DBL_MIN));
  TREE_TYPE (real_min_node) = double_type_node;

  real_eps_node = build_real (double_type_node, GPC_GET_REAL (DBL_EPSILON));
  TREE_TYPE (real_eps_node) = double_type_node;

  char_max_node = copy_node (TYPE_MAX_VALUE (char_type_node));
  TREE_TYPE (char_max_node) = char_type_node;

  /* Create a zero node for the complex types used in Extended Pascal */
  complex_zero_node = build_complex (TREE_TYPE (real_zero_node),
                                     real_zero_node, real_zero_node);

  /* Create a node to mark virtual methods in an object while parsing */
  virtual_mark_node = make_node (LANG_TYPE);

  /* Create an identifier node for implicit `Self' formal parameter */
  self_name_node = get_identifier ("Self");
#endif /* GPC */

  null_pointer_node = build_int_2 (0, 0);
  TREE_TYPE (null_pointer_node) = build_pointer_type (void_type_node);
  layout_type (TREE_TYPE (null_pointer_node));

  string_type_node = build_pointer_type (char_type_node);
  const_string_type_node
    = build_pointer_type (build_type_variant (char_type_node, 1, 0));

  /* Make a type to be the domain of a few array types
     whose domains don't really matter.
     200 is small enough that it always fits in size_t
     and large enough that it can hold most function names for the
     initializations of __FUNCTION__ and __PRETTY_FUNCTION__.  */
  array_domain_type = build_index_type (build_int_2 (200, 0));

  /* make a type for arrays of characters.
     With luck nothing will ever really depend on the length of this
     array type.  */
  char_array_type_node
    = build_array_type (char_type_node, array_domain_type);
  /* Likewise for arrays of ints.  */
  int_array_type_node
    = build_array_type (integer_type_node, array_domain_type);
  /* This is for wide string constants.  */
  wchar_array_type_node
    = build_array_type (wchar_type_node, array_domain_type);

  default_function_type
    = build_function_type (integer_type_node, NULL_TREE);

  ptr_type_node = build_pointer_type (void_type_node);
  const_ptr_type_node
    = build_pointer_type (build_type_variant (void_type_node, 1, 0));

#ifdef GPC
  ptrsize_unsigned_type_node = make_unsigned_type (TREE_INT_CST_LOW (TYPE_SIZE (ptr_type_node)));
  pushdecl (build_decl (TYPE_DECL, NULL_TREE, ptrsize_unsigned_type_node));

  ptrsize_integer_type_node = make_signed_type (TREE_INT_CST_LOW (TYPE_SIZE (ptr_type_node)));
  pushdecl (build_decl (TYPE_DECL, NULL_TREE, ptrsize_integer_type_node));

  packed_array_unsigned_long_type_node = make_unsigned_type (HOST_BITS_PER_WIDE_INT);
  packed_array_unsigned_short_type_node = make_unsigned_type (HOST_BITS_PER_WIDE_INT / 2);
#endif /* GPC */

  endlink = tree_cons (NULL_TREE, void_type_node, NULL_TREE);

  void_ftype_any
    = build_function_type (void_type_node, NULL_TREE);

  float_ftype_float
    = build_function_type (float_type_node,
                           tree_cons (NULL_TREE, float_type_node, endlink));

  double_ftype_double
    = build_function_type (double_type_node,
                           tree_cons (NULL_TREE, double_type_node, endlink));

  ldouble_ftype_ldouble
    = build_function_type (long_double_type_node,
                           tree_cons (NULL_TREE, long_double_type_node,
                                      endlink));

  double_ftype_double_double
    = build_function_type (double_type_node,
                           tree_cons (NULL_TREE, double_type_node,
                                      tree_cons (NULL_TREE,
                                                 double_type_node, endlink)));

  int_ftype_int
    = build_function_type (integer_type_node,
                           tree_cons (NULL_TREE, integer_type_node, endlink));

  long_ftype_long
    = build_function_type (long_integer_type_node,
                           tree_cons (NULL_TREE,
                                      long_integer_type_node, endlink));

  void_ftype_ptr_ptr_int
    = build_function_type (void_type_node,
                           tree_cons (NULL_TREE, ptr_type_node,
                                      tree_cons (NULL_TREE, ptr_type_node,
                                                 tree_cons (NULL_TREE,
                                                            integer_type_node,
                                                            endlink))));

  int_ftype_cptr_cptr_sizet
    = build_function_type (integer_type_node,
                           tree_cons (NULL_TREE, const_ptr_type_node,
                                      tree_cons (NULL_TREE, const_ptr_type_node,
                                                 tree_cons (NULL_TREE,
                                                            sizetype,
                                                            endlink))));

  void_ftype_ptr_int_int
    = build_function_type (void_type_node,
                           tree_cons (NULL_TREE, ptr_type_node,
                                      tree_cons (NULL_TREE, integer_type_node,
                                                 tree_cons (NULL_TREE,
                                                            integer_type_node,
                                                            endlink))));

  string_ftype_ptr_ptr          /* strcpy prototype */
    = build_function_type (string_type_node,
                           tree_cons (NULL_TREE, string_type_node,
                                      tree_cons (NULL_TREE,
                                                 const_string_type_node,
                                                 endlink)));

  int_ftype_string_string       /* strcmp prototype */
    = build_function_type (integer_type_node,
                           tree_cons (NULL_TREE, const_string_type_node,
                                      tree_cons (NULL_TREE,
                                                 const_string_type_node,
                                                 endlink)));

  strlen_ftype          /* strlen prototype */
    = build_function_type (flag_traditional ? integer_type_node : sizetype,
                           tree_cons (NULL_TREE, const_string_type_node,
                                      endlink));

  traditional_ptr_type_node
    = (flag_traditional ? string_type_node : ptr_type_node);

  memcpy_ftype  /* memcpy prototype */
    = build_function_type (traditional_ptr_type_node,
                           tree_cons (NULL_TREE, ptr_type_node,
                                      tree_cons (NULL_TREE, const_ptr_type_node,
                                                 tree_cons (NULL_TREE,
                                                            sizetype,
                                                            endlink))));

  memset_ftype  /* memset prototype */
    = build_function_type (traditional_ptr_type_node,
                           tree_cons (NULL_TREE, ptr_type_node,
                                      tree_cons (NULL_TREE, integer_type_node,
                                                 tree_cons (NULL_TREE,
                                                            sizetype,
                                                            endlink))));

  ptr_ftype_void = build_function_type (ptr_type_node, endlink);
  ptr_ftype_ptr
    = build_function_type (ptr_type_node,
                           tree_cons (NULL_TREE, ptr_type_node, endlink));

  builtin_function ("__builtin_constant_p", default_function_type,
                    BUILT_IN_CONSTANT_P, NULL_PTR);

  builtin_function ("__builtin_return_address",
                    build_function_type (ptr_type_node,
                                         tree_cons (NULL_TREE,
                                                    unsigned_type_node,
                                                    endlink)),
                    BUILT_IN_RETURN_ADDRESS, NULL_PTR);

  builtin_function ("__builtin_frame_address",
                    build_function_type (ptr_type_node,
                                         tree_cons (NULL_TREE,
                                                    unsigned_type_node,
                                                    endlink)),
                    BUILT_IN_FRAME_ADDRESS, NULL_PTR);

  builtin_function ("__builtin_aggregate_incoming_address",
                    build_function_type (ptr_type_node, NULL_TREE),
                    BUILT_IN_AGGREGATE_INCOMING_ADDRESS, NULL_PTR);

  /* Hooks for the DWARF 2 __throw routine.  */
  builtin_function ("__builtin_unwind_init",
                    build_function_type (void_type_node, endlink),
                    BUILT_IN_UNWIND_INIT, NULL_PTR);
  builtin_function ("__builtin_dwarf_fp_regnum",
                    build_function_type (unsigned_type_node, endlink),
                    BUILT_IN_DWARF_FP_REGNUM, NULL_PTR);
  builtin_function ("__builtin_dwarf_reg_size", int_ftype_int,
                    BUILT_IN_DWARF_REG_SIZE, NULL_PTR);
  builtin_function ("__builtin_frob_return_addr", ptr_ftype_ptr,
                    BUILT_IN_FROB_RETURN_ADDR, NULL_PTR);
  builtin_function ("__builtin_extract_return_addr", ptr_ftype_ptr,
                    BUILT_IN_EXTRACT_RETURN_ADDR, NULL_PTR);

  builtin_function ("__builtin_alloca",
                    build_function_type (ptr_type_node,
                                         tree_cons (NULL_TREE,
                                                    sizetype,
                                                    endlink)),
                    BUILT_IN_ALLOCA, "alloca");
  builtin_function ("__builtin_ffs", int_ftype_int, BUILT_IN_FFS, NULL_PTR);
  /* Define alloca, ffs as builtins.
     Declare _exit just to mark it as volatile.  */
  if (! flag_no_builtin && !flag_no_nonansi_builtin)
    {
      temp = builtin_function ("alloca",
                               build_function_type (ptr_type_node,
                                                    tree_cons (NULL_TREE,
                                                               sizetype,
                                                               endlink)),
                               BUILT_IN_ALLOCA, NULL_PTR);
      /* Suppress error if redefined as a non-function.  */
      DECL_BUILT_IN_NONANSI (temp) = 1;
      temp = builtin_function ("ffs", int_ftype_int, BUILT_IN_FFS, NULL_PTR);
      /* Suppress error if redefined as a non-function.  */
      DECL_BUILT_IN_NONANSI (temp) = 1;
      temp = builtin_function ("_exit", void_ftype_any, NOT_BUILT_IN,
                               NULL_PTR);
      TREE_THIS_VOLATILE (temp) = 1;
      TREE_SIDE_EFFECTS (temp) = 1;
      /* Suppress error if redefined as a non-function.  */
      DECL_BUILT_IN_NONANSI (temp) = 1;
    }

  builtin_function ("__builtin_abs", int_ftype_int, BUILT_IN_ABS, NULL_PTR);
  builtin_function ("__builtin_fabsf", float_ftype_float, BUILT_IN_FABS,
                    NULL_PTR);
  builtin_function ("__builtin_fabs", double_ftype_double, BUILT_IN_FABS,
                    NULL_PTR);
  builtin_function ("__builtin_fabsl", ldouble_ftype_ldouble, BUILT_IN_FABS,
                    NULL_PTR);
  builtin_function ("__builtin_labs", long_ftype_long, BUILT_IN_LABS,
                    NULL_PTR);
  builtin_function ("__builtin_saveregs",
                    build_function_type (ptr_type_node, NULL_TREE),
                    BUILT_IN_SAVEREGS, NULL_PTR);
/* EXPAND_BUILTIN_VARARGS is obsolete.  */
#if 0
  builtin_function ("__builtin_varargs",
                    build_function_type (ptr_type_node,
                                         tree_cons (NULL_TREE,
                                                    integer_type_node,
                                                    endlink)),
                    BUILT_IN_VARARGS, NULL_PTR);
#endif
  builtin_function ("__builtin_classify_type", default_function_type,
                    BUILT_IN_CLASSIFY_TYPE, NULL_PTR);
  builtin_function ("__builtin_next_arg",
                    build_function_type (ptr_type_node, NULL_TREE),
                    BUILT_IN_NEXT_ARG, NULL_PTR);
  builtin_function ("__builtin_args_info",
                    build_function_type (integer_type_node,
                                         tree_cons (NULL_TREE,
                                                    integer_type_node,
                                                    endlink)),
                    BUILT_IN_ARGS_INFO, NULL_PTR);

  /* Untyped call and return.  */
  builtin_function ("__builtin_apply_args",
                    build_function_type (ptr_type_node, NULL_TREE),
                    BUILT_IN_APPLY_ARGS, NULL_PTR);

  temp = tree_cons (NULL_TREE,
                    build_pointer_type (build_function_type (void_type_node,
                                                             NULL_TREE)),
                    tree_cons (NULL_TREE,
                               ptr_type_node,
                               tree_cons (NULL_TREE,
                                          sizetype,
                                          endlink)));
  builtin_function ("__builtin_apply",
                    build_function_type (ptr_type_node, temp),
                    BUILT_IN_APPLY, NULL_PTR);
  builtin_function ("__builtin_return",
                    build_function_type (void_type_node,
                                         tree_cons (NULL_TREE,
                                                    ptr_type_node,
                                                    endlink)),
                    BUILT_IN_RETURN, NULL_PTR);

  /* Currently under experimentation.  */
  builtin_function ("__builtin_memcpy", memcpy_ftype,
                    BUILT_IN_MEMCPY, "memcpy");
  builtin_function ("__builtin_memcmp", int_ftype_cptr_cptr_sizet,
                    BUILT_IN_MEMCMP, "memcmp");
  builtin_function ("__builtin_memset", memset_ftype,
                    BUILT_IN_MEMSET, "memset");
  builtin_function ("__builtin_strcmp", int_ftype_string_string,
                    BUILT_IN_STRCMP, "strcmp");
  builtin_function ("__builtin_strcpy", string_ftype_ptr_ptr,
                    BUILT_IN_STRCPY, "strcpy");
  builtin_function ("__builtin_strlen", strlen_ftype,
                    BUILT_IN_STRLEN, "strlen");
  builtin_function ("__builtin_sqrtf", float_ftype_float,
                    BUILT_IN_FSQRT, "sqrtf");
  builtin_function ("__builtin_fsqrt", double_ftype_double,
                    BUILT_IN_FSQRT, "sqrt");
  builtin_function ("__builtin_sqrtl", ldouble_ftype_ldouble,
                    BUILT_IN_FSQRT, "sqrtl");
  builtin_function ("__builtin_sinf", float_ftype_float,
                    BUILT_IN_SIN, "sinf");
  builtin_function ("__builtin_sin", double_ftype_double,
                    BUILT_IN_SIN, "sin");
  builtin_function ("__builtin_sinl", ldouble_ftype_ldouble,
                    BUILT_IN_SIN, "sinl");
  builtin_function ("__builtin_cosf", float_ftype_float,
                    BUILT_IN_COS, "cosf");
  builtin_function ("__builtin_cos", double_ftype_double,
                    BUILT_IN_COS, "cos");
  builtin_function ("__builtin_cosl", ldouble_ftype_ldouble,
                    BUILT_IN_COS, "cosl");
  builtin_function ("__builtin_setjmp",
                    build_function_type (integer_type_node,
                                         tree_cons (NULL_TREE,
                                                    ptr_type_node, endlink)),
                    BUILT_IN_SETJMP, NULL_PTR);
  builtin_function ("__builtin_longjmp",
                    build_function_type
                    (void_type_node,
                     tree_cons (NULL, ptr_type_node,
                                tree_cons (NULL_TREE,
                                           integer_type_node,
                                           endlink))),
                    BUILT_IN_LONGJMP, NULL_PTR);

  /* In an ANSI C program, it is okay to supply built-in meanings
     for these functions, since applications cannot validly use them
     with any other meaning.
     However, honor the -fno-builtin option.  */
  if (!flag_no_builtin)
    {
      builtin_function ("abs", int_ftype_int, BUILT_IN_ABS, NULL_PTR);
      builtin_function ("fabsf", float_ftype_float, BUILT_IN_FABS, NULL_PTR);
      builtin_function ("fabs", double_ftype_double, BUILT_IN_FABS, NULL_PTR);
      builtin_function ("fabsl", ldouble_ftype_ldouble, BUILT_IN_FABS,
                        NULL_PTR);
      builtin_function ("labs", long_ftype_long, BUILT_IN_LABS, NULL_PTR);
      builtin_function ("memcpy", memcpy_ftype, BUILT_IN_MEMCPY, NULL_PTR);
      builtin_function ("memcmp", int_ftype_cptr_cptr_sizet, BUILT_IN_MEMCMP,
                        NULL_PTR);
      builtin_function ("memset", memset_ftype, BUILT_IN_MEMSET, NULL_PTR);
      builtin_function ("strcmp", int_ftype_string_string, BUILT_IN_STRCMP,
                        NULL_PTR);
      builtin_function ("strcpy", string_ftype_ptr_ptr, BUILT_IN_STRCPY,
                        NULL_PTR);
      builtin_function ("strlen", strlen_ftype, BUILT_IN_STRLEN, NULL_PTR);
      builtin_function ("sqrtf", float_ftype_float, BUILT_IN_FSQRT, NULL_PTR);
      builtin_function ("sqrt", double_ftype_double, BUILT_IN_FSQRT, NULL_PTR);
      builtin_function ("sqrtl", ldouble_ftype_ldouble, BUILT_IN_FSQRT,
                        NULL_PTR);
      builtin_function ("sinf", float_ftype_float, BUILT_IN_SIN, NULL_PTR);
      builtin_function ("sin", double_ftype_double, BUILT_IN_SIN, NULL_PTR);
      builtin_function ("sinl", ldouble_ftype_ldouble, BUILT_IN_SIN, NULL_PTR);
      builtin_function ("cosf", float_ftype_float, BUILT_IN_COS, NULL_PTR);
      builtin_function ("cos", double_ftype_double, BUILT_IN_COS, NULL_PTR);
      builtin_function ("cosl", ldouble_ftype_ldouble, BUILT_IN_COS, NULL_PTR);

      /* Declare these functions volatile
         to avoid spurious "control drops through" warnings.  */
      /* Don't specify the argument types, to avoid errors
         from certain code which isn't valid in ANSI but which exists.  */
      temp = builtin_function ("abort", void_ftype_any, NOT_BUILT_IN,
                               NULL_PTR);
      TREE_THIS_VOLATILE (temp) = 1;
      TREE_SIDE_EFFECTS (temp) = 1;
      temp = builtin_function ("exit", void_ftype_any, NOT_BUILT_IN, NULL_PTR);
      TREE_THIS_VOLATILE (temp) = 1;
      TREE_SIDE_EFFECTS (temp) = 1;
    }

#if 0
  /* Support for these has not been written in either expand_builtin
     or build_function_call.  */
  builtin_function ("__builtin_div", default_ftype, BUILT_IN_DIV, NULL_PTR);
  builtin_function ("__builtin_ldiv", default_ftype, BUILT_IN_LDIV, NULL_PTR);
  builtin_function ("__builtin_ffloor", double_ftype_double, BUILT_IN_FFLOOR,
                    NULL_PTR);
  builtin_function ("__builtin_fceil", double_ftype_double, BUILT_IN_FCEIL,
                    NULL_PTR);
  builtin_function ("__builtin_fmod", double_ftype_double_double,
                    BUILT_IN_FMOD, NULL_PTR);
  builtin_function ("__builtin_frem", double_ftype_double_double,
                    BUILT_IN_FREM, NULL_PTR);
  builtin_function ("__builtin_memset", ptr_ftype_ptr_int_int,
                    BUILT_IN_MEMSET, NULL_PTR);
  builtin_function ("__builtin_getexp", double_ftype_double, BUILT_IN_GETEXP,
                    NULL_PTR);
  builtin_function ("__builtin_getman", double_ftype_double, BUILT_IN_GETMAN,
                    NULL_PTR);
#endif

  pedantic_lvalues = pedantic;

  /* Create the global bindings for __FUNCTION__ and __PRETTY_FUNCTION__.  */
  declare_function_name ();

  start_identifier_warnings ();

  /* Prepare to check format strings against argument lists.  */
  init_function_format_info ();

  init_iterators ();

  incomplete_decl_finalize_hook = finish_incomplete_decl;
}

/* Return a definition for a builtin function named NAME and whose data type
   is TYPE.  TYPE should be a function type with argument types.
   FUNCTION_CODE tells later passes how to compile calls to this function.
   See tree.h for its possible values.

   If LIBRARY_NAME is nonzero, use that for DECL_ASSEMBLER_NAME,
   the name to be called if we can't opencode the function.  */

#ifdef EGCS92
tree
builtin_function (name, type, function_code, library_name)
     const char *name;
     tree type;
     enum built_in_function function_code;
     const char *library_name;
#else
tree
builtin_function (name, type, function_code, library_name)
     char *name;
     tree type;
     enum built_in_function function_code;
     char *library_name;
#endif
{
  tree decl = build_decl (FUNCTION_DECL, get_identifier (name), type);
  DECL_EXTERNAL (decl) = 1;
  TREE_PUBLIC (decl) = 1;
  /* If -traditional, permit redefining a builtin function any way you like.
     (Though really, if the program redefines these functions,
     it probably won't work right unless compiled with -fno-builtin.)  */
  if (flag_traditional && name[0] != '_')
    DECL_BUILT_IN_NONANSI (decl) = 1;
  if (library_name)
    DECL_ASSEMBLER_NAME (decl) = get_identifier (library_name);
  make_decl_rtl (decl, NULL_PTR, 1);
  pushdecl (decl);
  if (function_code != NOT_BUILT_IN)
    {
      DECL_BUILT_IN (decl) = 1;
      DECL_FUNCTION_CODE (decl) = function_code;
    }
  /* Warn if a function in the namespace for users
     is used without an occasion to consider it declared.  */
  if (name[0] != '_' || name[1] != '_')
    C_DECL_ANTICIPATED (decl) = 1;

  return decl;
}

/* Called when a declaration is seen that contains no names to declare.
   If its type is a reference to a structure, union or enum inherited
   from a containing scope, shadow that tag name for the current scope
   with a forward reference.
   If its type defines a new named structure or union
   or defines an enum, it is valid but we need not do anything here.
   Otherwise, it is an error.  */

void
shadow_tag (declspecs)
     tree declspecs;
{
  shadow_tag_warned (declspecs, 0);
}

void
shadow_tag_warned (declspecs, warned)
     tree declspecs;
     int warned;
     /* 1 => we have done a pedwarn.  2 => we have done a warning, but
        no pedwarn.  */
{
  int found_tag = 0;
  register tree link;

  pending_invalid_xref = 0;

  for (link = declspecs; link; link = TREE_CHAIN (link))
    {
      register tree value = TREE_VALUE (link);
      register enum tree_code code = TREE_CODE (value);

      if (code == RECORD_TYPE || code == UNION_TYPE || code == ENUMERAL_TYPE)
        /* Used to test also that TYPE_SIZE (value) != 0.
           That caused warning for `struct foo;' at top level in the file.  */
        {
          register tree name = lookup_tag_reverse (value);
          register tree t;

          found_tag++;

          if (name == 0)
            {
              if (warned != 1 && code != ENUMERAL_TYPE)
                /* Empty unnamed enum OK */
                {
                  pedwarn ("unnamed struct/union that defines no instances");
                  warned = 1;
                }
            }
          else
            {
              t = lookup_tag (code, name, current_binding_level, 1);

              if (t == 0)
                {
                  t = make_node (code);
                  pushtag (name, t);
                }
            }
        }
      else
        {
          if (!warned && ! in_system_header)
            {
              warning ("useless keyword or type name in empty declaration");
              warned = 2;
            }
        }
    }

  if (found_tag > 1)
    error ("two types specified in one empty declaration");

  if (warned != 1)
    {
      if (found_tag == 0)
        pedwarn ("empty declaration");
    }
}

/* Decode a "typename", such as "int **", returning a ..._TYPE node.  */

tree
groktypename (typename)
     tree typename;
{
  if (TREE_CODE (typename) != TREE_LIST)
    return typename;
  return grokdeclarator (TREE_VALUE (typename),
                         TREE_PURPOSE (typename),
                         TYPENAME, 0);
}

/* Return a PARM_DECL node for a given pair of specs and declarator.  */

tree
groktypename_in_parm_context (typename)
     tree typename;
{
  if (TREE_CODE (typename) != TREE_LIST)
    return typename;
  return grokdeclarator (TREE_VALUE (typename),
                         TREE_PURPOSE (typename),
                         PARM, 0);
}

/* Decode a declarator in an ordinary declaration or data definition.
   This is called as soon as the type information and variable name
   have been parsed, before parsing the initializer if any.
   Here we create the ..._DECL node, fill in its type,
   and put it on the list of decls for the current context.
   The ..._DECL node is returned as the value.

   Exception: for arrays where the length is not specified,
   the type is left null, to be filled in by `finish_decl'.

   Function definitions do not come here; they go to start_function
   instead.  However, external and forward declarations of functions
   do go through here.  Structure field declarations are done by
   grokfield and not through here.  */

/* Set this to zero to debug not using the temporary obstack
   to parse initializers.  */
int debug_temp_inits = 1;

tree
start_decl (declarator, declspecs, initialized, attributes, prefix_attributes)
     tree declarator, declspecs;
     int initialized;
     tree attributes, prefix_attributes;
{
  register tree decl = grokdeclarator (declarator, declspecs,
                                       NORMAL, initialized);
  register tree tem;
  int init_written = initialized;

  /* The corresponding pop_obstacks is in finish_decl.  */
  push_obstacks_nochange ();

  if (warn_main && !strcmp (IDENTIFIER_POINTER (declarator), "main"))
    warning_with_decl (decl, "`%s' is usually a function");

  if (initialized)
    /* Is it valid for this decl to have an initializer at all?
       If not, set INITIALIZED to zero, which will indirectly
       tell `finish_decl' to ignore the initializer once it is parsed.  */
    switch (TREE_CODE (decl))
      {
      case TYPE_DECL:
        /* typedef foo = bar  means give foo the same type as bar.
           We haven't parsed bar yet, so `finish_decl' will fix that up.
           Any other case of an initialization in a TYPE_DECL is an error.  */
        if (pedantic || list_length (declspecs) > 1)
          {
            error ("typedef `%s' is initialized",
                   IDENTIFIER_POINTER (DECL_NAME (decl)));
            initialized = 0;
          }
        break;

      case FUNCTION_DECL:
        error ("function `%s' is initialized like a variable",
               IDENTIFIER_POINTER (DECL_NAME (decl)));
        initialized = 0;
        break;

      case PARM_DECL:
        /* DECL_INITIAL in a PARM_DECL is really DECL_ARG_TYPE.  */
        error ("parameter `%s' is initialized",
               IDENTIFIER_POINTER (DECL_NAME (decl)));
        initialized = 0;
        break;

      default:
        /* Don't allow initializations for incomplete types
           except for arrays which might be completed by the initialization.  */
        if (TYPE_SIZE (TREE_TYPE (decl)) != 0)
          {
            /* A complete type is ok if size is fixed.  */

            if (TREE_CODE (TYPE_SIZE (TREE_TYPE (decl))) != INTEGER_CST
                || C_DECL_VARIABLE_SIZE (decl))
              {
                error ("variable-sized object may not be initialized");
                initialized = 0;
              }
          }
        else if (TREE_CODE (TREE_TYPE (decl)) != ARRAY_TYPE)
          {
            error ("variable `%s' has initializer but incomplete type",
                   IDENTIFIER_POINTER (DECL_NAME (decl)));
            initialized = 0;
          }
        else if (TYPE_SIZE (TREE_TYPE (TREE_TYPE (decl))) == 0)
          {
            error ("elements of array `%s' have incomplete type",
                   IDENTIFIER_POINTER (DECL_NAME (decl)));
            initialized = 0;
          }
      }

  if (initialized)
    {
#if 0  /* Seems redundant with grokdeclarator.  */
      if (current_binding_level != global_binding_level
          && DECL_EXTERNAL (decl)
          && TREE_CODE (decl) != FUNCTION_DECL)
        warning ("declaration of `%s' has `extern' and is initialized",
                 IDENTIFIER_POINTER (DECL_NAME (decl)));
#endif
      DECL_EXTERNAL (decl) = 0;
      if (current_binding_level == global_binding_level)
        TREE_STATIC (decl) = 1;

      /* Tell `pushdecl' this is an initialized decl
         even though we don't yet have the initializer expression.
         Also tell `finish_decl' it may store the real initializer.  */
      DECL_INITIAL (decl) = error_mark_node;
    }

  /* If this is a function declaration, write a record describing it to the
     prototypes file (if requested).  */

  if (TREE_CODE (decl) == FUNCTION_DECL)
    gen_aux_info_record (decl, 0, 0, TYPE_ARG_TYPES (TREE_TYPE (decl)) != 0);

  /* ANSI specifies that a tentative definition which is not merged with
     a non-tentative definition behaves exactly like a definition with an
     initializer equal to zero.  (Section 3.7.2)
     -fno-common gives strict ANSI behavior.  Usually you don't want it.
     This matters only for variables with external linkage.  */
  if (! flag_no_common || ! TREE_PUBLIC (decl))
    DECL_COMMON (decl) = 1;

  /* Set attributes here so if duplicate decl, will have proper attributes.  */
  decl_attributes (decl, attributes, prefix_attributes);

  /* Add this decl to the current binding level.
     TEM may equal DECL or it may be a previous decl of the same name.  */
  tem = pushdecl (decl);

  /* For a local variable, define the RTL now.  */
#ifdef GPC
  if (! pascal_global_bindings_p ()
#else /* not GPC */
  if (current_binding_level != global_binding_level
#endif /* not GPC */
      /* But not if this is a duplicate decl
         and we preserved the rtl from the previous one
         (which may or may not happen).  */
      && DECL_RTL (tem) == 0)
    {
      if (TYPE_SIZE (TREE_TYPE (tem)) != 0)
        expand_decl (tem);
      else if (TREE_CODE (TREE_TYPE (tem)) == ARRAY_TYPE
               && DECL_INITIAL (tem) != 0)
        expand_decl (tem);
    }

#ifdef GPC
  if (init_written && TREE_CODE (decl) != CONST_DECL)
#else
  if (init_written)
#endif /* GPC */
    {
      /* When parsing and digesting the initializer,
         use temporary storage.  Do this even if we will ignore the value.  */
      if (current_binding_level == global_binding_level && debug_temp_inits)
        temporary_allocation ();
    }

  return tem;
}

/* Finish processing of a declaration;
   install its initial value.
   If the length of an array type is not known before,
   it must be determined now, from the initial value, or it is an error.  */

void
finish_decl (decl, init, asmspec_tree)
     tree decl, init;
     tree asmspec_tree;
{
  register tree type = TREE_TYPE (decl);
  int was_incomplete = (DECL_SIZE (decl) == 0);
  int temporary = allocation_temporary_p ();
  char *asmspec = 0;

  /* If a name was specified, get the string.   */
  if (asmspec_tree)
    asmspec = TREE_STRING_POINTER (asmspec_tree);

  /* If `start_decl' didn't like having an initialization, ignore it now.  */

  if (init != 0 && DECL_INITIAL (decl) == 0)
    init = 0;
  /* Don't crash if parm is initialized.  */
  if (TREE_CODE (decl) == PARM_DECL)
    init = 0;

  if (ITERATOR_P (decl))
    {
      if (init == 0)
        error_with_decl (decl, "iterator has no initial value");
      else
        init = save_expr (init);
    }

  if (init)
    {
      if (TREE_CODE (decl) != TYPE_DECL)
        store_init_value (decl, init);
      else
        {
          /* typedef foo = bar; store the type of bar as the type of foo.  */
          TREE_TYPE (decl) = TREE_TYPE (init);
          DECL_INITIAL (decl) = init = 0;
        }
    }

  /* Pop back to the obstack that is current for this binding level.
     This is because MAXINDEX, rtl, etc. to be made below
     must go in the permanent obstack.  But don't discard the
     temporary data yet.  */
  pop_obstacks ();
#if 0 /* pop_obstacks was near the end; this is what was here.  */
  if (current_binding_level == global_binding_level && temporary)
    end_temporary_allocation ();
#endif

  /* Deduce size of array from initialization, if not already known */

  if (TREE_CODE (type) == ARRAY_TYPE
      && TYPE_DOMAIN (type) == 0
      && TREE_CODE (decl) != TYPE_DECL)
    {
      int do_default
        = (TREE_STATIC (decl)
           /* Even if pedantic, an external linkage array
              may have incomplete type at first.  */
           ? pedantic && !TREE_PUBLIC (decl)
           : !DECL_EXTERNAL (decl));
      int failure
        = complete_array_type (type, DECL_INITIAL (decl), do_default);

      /* Get the completed type made by complete_array_type.  */
      type = TREE_TYPE (decl);

      if (failure == 1)
        error_with_decl (decl, "initializer fails to determine size of `%s'");

      if (failure == 2)
        {
          if (do_default)
            error_with_decl (decl, "array size missing in `%s'");
          /* If a `static' var's size isn't known,
             make it extern as well as static, so it does not get
             allocated.
             If it is not `static', then do not mark extern;
             finish_incomplete_decl will give it a default size
             and it will get allocated.  */
          else if (!pedantic && TREE_STATIC (decl) && ! TREE_PUBLIC (decl))
            DECL_EXTERNAL (decl) = 1;
        }

      /* TYPE_MAX_VALUE is always one less than the number of elements
         in the array, because we start counting at zero.  Therefore,
         warn only if the value is less than zero.  */
      if (pedantic && TYPE_DOMAIN (type) != 0
          && tree_int_cst_sgn (TYPE_MAX_VALUE (TYPE_DOMAIN (type))) < 0)
        error_with_decl (decl, "zero or negative size array `%s'");

      layout_decl (decl, 0);
    }

  if (TREE_CODE (decl) == VAR_DECL)
    {
      if (DECL_SIZE (decl) == 0
          && TYPE_SIZE (TREE_TYPE (decl)) != 0)
        layout_decl (decl, 0);

      if (DECL_SIZE (decl) == 0
          && (TREE_STATIC (decl)
              ?
                /* A static variable with an incomplete type
                   is an error if it is initialized.
                   Also if it is not file scope.
                   Otherwise, let it through, but if it is not `extern'
                   then it may cause an error message later.  */
              /* A duplicate_decls call could have changed an extern
                 declaration into a file scope one.  This can be detected
                 by TREE_ASM_WRITTEN being set.  */
                (DECL_INITIAL (decl) != 0
                 || (DECL_CONTEXT (decl) != 0 && ! TREE_ASM_WRITTEN (decl)))
              :
                /* An automatic variable with an incomplete type
                   is an error.  */
                !DECL_EXTERNAL (decl)))
        {
          error_with_decl (decl, "storage size of `%s' isn't known");
          TREE_TYPE (decl) = error_mark_node;
        }

      if ((DECL_EXTERNAL (decl) || TREE_STATIC (decl))
          && DECL_SIZE (decl) != 0)
        {
          if (TREE_CODE (DECL_SIZE (decl)) == INTEGER_CST)
            constant_expression_warning (DECL_SIZE (decl));
          else
            error_with_decl (decl, "storage size of `%s' isn't constant");
        }

      if (TREE_USED  (type))
        TREE_USED (decl) = 1;
    }

  /* If this is a function and an assembler name is specified, it isn't
     builtin any more.  Also reset DECL_RTL so we can give it its new
     name.  */
  if (TREE_CODE (decl) == FUNCTION_DECL && asmspec)
      {
        DECL_BUILT_IN (decl) = 0;
        DECL_RTL (decl) = 0;
      }

  /* Output the assembler code and/or RTL code for variables and functions,
     unless the type is an undefined structure or union.
     If not, it will get done when the type is completed.  */

  if (TREE_CODE (decl) == VAR_DECL || TREE_CODE (decl) == FUNCTION_DECL)
    {
      if ((flag_traditional || TREE_PERMANENT (decl))
          && allocation_temporary_p ())
        {
          push_obstacks_nochange ();
          end_temporary_allocation ();
          /* This is a no-op in c-lang.c or something real in objc-actions.c.  */
          maybe_objc_check_decl (decl);
          rest_of_decl_compilation (decl, asmspec,
                                    (DECL_CONTEXT (decl) == 0
                                     || TREE_ASM_WRITTEN (decl)),
#ifdef GPC
                                    1);
#else /* not GPC */
                                    0);
#endif /* not GPC */
          pop_obstacks ();
        }
      else
        {
          /* This is a no-op in c-lang.c or something real in objc-actions.c.  */
          maybe_objc_check_decl (decl);
          rest_of_decl_compilation (decl, asmspec, DECL_CONTEXT (decl) == 0,
#ifdef GPC
                                    1);
#else /* not GPC */
                                    0);
#endif /* not GPC */
        }
      if (DECL_CONTEXT (decl) != 0)
        {
          /* Recompute the RTL of a local array now
             if it used to be an incomplete type.  */
          if (was_incomplete
              && ! TREE_STATIC (decl) && ! DECL_EXTERNAL (decl))
            {
              /* If we used it already as memory, it must stay in memory.  */
              TREE_ADDRESSABLE (decl) = TREE_USED (decl);
              /* If it's still incomplete now, no init will save it.  */
              if (DECL_SIZE (decl) == 0)
                DECL_INITIAL (decl) = 0;
              expand_decl (decl);
            }
          /* Compute and store the initial value.  */
          if (TREE_CODE (decl) != FUNCTION_DECL)
            expand_decl_init (decl);
        }
    }

  if (TREE_CODE (decl) == TYPE_DECL)
    {
      /* This is a no-op in c-lang.c or something real in objc-actions.c.  */
      maybe_objc_check_decl (decl);
      rest_of_decl_compilation (decl, NULL_PTR, DECL_CONTEXT (decl) == 0,
                                0);
    }

  /* ??? After 2.3, test (init != 0) instead of TREE_CODE.  */
  /* This test used to include TREE_PERMANENT, however, we have the same
     problem with initializers at the function level.  Such initializers get
     saved until the end of the function on the momentary_obstack.  */
  if (!(TREE_CODE (decl) == FUNCTION_DECL && DECL_INLINE (decl))
      && temporary
      /* DECL_INITIAL is not defined in PARM_DECLs, since it shares
         space with DECL_ARG_TYPE.  */
      && TREE_CODE (decl) != PARM_DECL)
    {
      /* We need to remember that this array HAD an initialization,
         but discard the actual temporary nodes,
         since we can't have a permanent node keep pointing to them.  */
      /* We make an exception for inline functions, since it's
         normal for a local extern redeclaration of an inline function
         to have a copy of the top-level decl's DECL_INLINE.  */
      if (DECL_INITIAL (decl) != 0 && DECL_INITIAL (decl) != error_mark_node)
        {
          /* If this is a const variable, then preserve the
             initializer instead of discarding it so that we can optimize
             references to it.  */
          /* This test used to include TREE_STATIC, but this won't be set
             for function level initializers.  */
          if (TREE_READONLY (decl) || ITERATOR_P (decl))
            {
              preserve_initializer ();
              /* Hack?  Set the permanent bit for something that is permanent,
                 but not on the permanent obstack, so as to convince
                 output_constant_def to make its rtl on the permanent
                 obstack.  */
              TREE_PERMANENT (DECL_INITIAL (decl)) = 1;

              /* The initializer and DECL must have the same (or equivalent
                 types), but if the initializer is a STRING_CST, its type
                 might not be on the right obstack, so copy the type
                 of DECL.  */
              TREE_TYPE (DECL_INITIAL (decl)) = type;
            }
          else
            DECL_INITIAL (decl) = error_mark_node;
        }
    }

  /* If requested, warn about definitions of large data objects.  */

  if (warn_larger_than
      && (TREE_CODE (decl) == VAR_DECL || TREE_CODE (decl) == PARM_DECL)
      && !DECL_EXTERNAL (decl))
    {
      register tree decl_size = DECL_SIZE (decl);

      if (decl_size && TREE_CODE (decl_size) == INTEGER_CST)
        {
           unsigned units = TREE_INT_CST_LOW(decl_size) / BITS_PER_UNIT;

          if (units > larger_than_size)
            warning_with_decl (decl, "size of `%s' is %u bytes", units);
        }
    }

#if 0
  /* Resume permanent allocation, if not within a function.  */
  /* The corresponding push_obstacks_nochange is in start_decl,
     and in push_parm_decl and in grokfield.  */
  pop_obstacks ();
#endif

  /* If we have gone back from temporary to permanent allocation,
     actually free the temporary space that we no longer need.  */
  if (temporary && !allocation_temporary_p ())
    permanent_allocation (0);

  /* At the end of a declaration, throw away any variable type sizes
     of types defined inside that declaration.  There is no use
     computing them in the following function definition.  */
  if (current_binding_level == global_binding_level)
    get_pending_sizes ();
}

/* If DECL has a cleanup, build and return that cleanup here.
   This is a callback called by expand_expr.  */

tree
maybe_build_cleanup (decl)
     tree decl;
{
  /* There are no cleanups in C.  */
  return NULL_TREE;
}

/* Given a parsed parameter declaration,
   decode it into a PARM_DECL and push that on the current binding level.
   Also, for the sake of forward parm decls,
   record the given order of parms in `parm_order'.  */

void
push_parm_decl (parm)
     tree parm;
{
  tree decl;
  int old_immediate_size_expand = immediate_size_expand;
  /* Don't try computing parm sizes now -- wait till fn is called.  */
  immediate_size_expand = 0;

  /* The corresponding pop_obstacks is in finish_decl.  */
  push_obstacks_nochange ();

  decl = grokdeclarator (TREE_VALUE (TREE_PURPOSE (parm)),
                         TREE_PURPOSE (TREE_PURPOSE (parm)), PARM, 0);
  decl_attributes (decl, TREE_VALUE (TREE_VALUE (parm)),
                   TREE_PURPOSE (TREE_VALUE (parm)));

#if 0
  if (DECL_NAME (decl))
    {
      tree olddecl;
      olddecl = lookup_name (DECL_NAME (decl));
      if (pedantic && olddecl != 0 && TREE_CODE (olddecl) == TYPE_DECL)
        pedwarn_with_decl (decl, "ANSI C forbids parameter `%s' shadowing typedef");
    }
#endif

  decl = pushdecl (decl);

  immediate_size_expand = old_immediate_size_expand;

  current_binding_level->parm_order
    = tree_cons (NULL_TREE, decl, current_binding_level->parm_order);

  /* Add this decl to the current binding level.  */
  finish_decl (decl, NULL_TREE, NULL_TREE);
}

/* Clear the given order of parms in `parm_order'.
   Used at start of parm list,
   and also at semicolon terminating forward decls.  */

void
clear_parm_order ()
{
  current_binding_level->parm_order = NULL_TREE;
}

/* Make TYPE a complete type based on INITIAL_VALUE.
   Return 0 if successful, 1 if INITIAL_VALUE can't be deciphered,
   2 if there was no information (in which case assume 1 if DO_DEFAULT).  */

int
complete_array_type (type, initial_value, do_default)
     tree type;
     tree initial_value;
     int do_default;
{
  register tree maxindex = NULL_TREE;
  int value = 0;

  if (initial_value)
    {
      /* Note MAXINDEX  is really the maximum index,
         one less than the size.  */
      if (TREE_CODE (initial_value) == STRING_CST)
        {
          int eltsize
            = int_size_in_bytes (TREE_TYPE (TREE_TYPE (initial_value)));
          maxindex = build_int_2 ((TREE_STRING_LENGTH (initial_value)
                                   / eltsize) - 1, 0);
        }
      else if (TREE_CODE (initial_value) == CONSTRUCTOR)
        {
          tree elts = CONSTRUCTOR_ELTS (initial_value);
          maxindex = size_binop (MINUS_EXPR, integer_zero_node, size_one_node);
          for (; elts; elts = TREE_CHAIN (elts))
            {
              if (TREE_PURPOSE (elts))
                maxindex = TREE_PURPOSE (elts);
              else
                maxindex = size_binop (PLUS_EXPR, maxindex, size_one_node);
            }
          maxindex = copy_node (maxindex);
        }
      else
        {
          /* Make an error message unless that happened already.  */
          if (initial_value != error_mark_node)
            value = 1;

          /* Prevent further error messages.  */
          maxindex = build_int_2 (0, 0);
        }
    }

  if (!maxindex)
    {
      if (do_default)
        maxindex = build_int_2 (0, 0);
      value = 2;
    }

  if (maxindex)
    {
      TYPE_DOMAIN (type) = build_index_type (maxindex);
      if (!TREE_TYPE (maxindex))
        TREE_TYPE (maxindex) = TYPE_DOMAIN (type);
    }

  /* Lay out the type now that we can get the real answer.  */

  layout_type (type);

  return value;
}

/* Given declspecs and a declarator,
   determine the name and type of the object declared
   and construct a ..._DECL node for it.
   (In one case we can return a ..._TYPE node instead.
    For invalid input we sometimes return 0.)

   DECLSPECS is a chain of tree_list nodes whose value fields
    are the storage classes and type specifiers.

   DECL_CONTEXT says which syntactic context this declaration is in:
     NORMAL for most contexts.  Make a VAR_DECL or FUNCTION_DECL or TYPE_DECL.
     FUNCDEF for a function definition.  Like NORMAL but a few different
      error messages in each case.  Return value may be zero meaning
      this definition is too screwy to try to parse.
     PARM for a parameter declaration (either within a function prototype
      or before a function body).  Make a PARM_DECL, or return void_type_node.
     TYPENAME if for a typename (in a cast or sizeof).
      Don't make a DECL node; just return the ..._TYPE node.
     FIELD for a struct or union field; make a FIELD_DECL.
     BITFIELD for a field with specified width.
   INITIALIZED is 1 if the decl has an initializer.

   In the TYPENAME case, DECLARATOR is really an absolute declarator.
   It may also be so in the PARM case, for a prototype where the
   argument type is specified but not the name.

   This function is where the complicated C meanings of `static'
   and `extern' are interpreted.  */

static tree
grokdeclarator (declarator, declspecs, decl_context, initialized)
     tree declspecs;
     tree declarator;
     enum decl_context decl_context;
     int initialized;
{
  int specbits = 0;
  tree spec;
  tree type = NULL_TREE;
  int longlong = 0;
  int constp;
  int volatilep;
  int inlinep;
  int explicit_int = 0;
  int explicit_char = 0;
  int defaulted_int = 0;
  tree typedef_decl = 0;
  char *name;
  tree typedef_type = 0;
  int funcdef_flag = 0;
  enum tree_code innermost_code = ERROR_MARK;
  int bitfield = 0;
  int size_varies = 0;
  tree decl_machine_attr = NULL_TREE;
#ifdef GPC
  int varparmp = 0;
#endif /* GPC */

  if (decl_context == BITFIELD)
    bitfield = 1, decl_context = FIELD;

  if (decl_context == FUNCDEF)
    funcdef_flag = 1, decl_context = NORMAL;

  push_obstacks_nochange ();

  if (flag_traditional && allocation_temporary_p ())
    end_temporary_allocation ();

  /* Look inside a declarator for the name being declared
     and get it as a string, for an error message.  */
  {
    register tree decl = declarator;
    name = 0;

    while (decl)
      switch (TREE_CODE (decl))
        {
        case ARRAY_REF:
        case INDIRECT_REF:
        case CALL_EXPR:
          innermost_code = TREE_CODE (decl);
          decl = TREE_OPERAND (decl, 0);
          break;

        case IDENTIFIER_NODE:
          name = IDENTIFIER_POINTER (decl);
          decl = 0;
          break;

        default:
          abort ();
        }
    if (name == 0)
      name = "type name";
  }

  /* A function definition's declarator must have the form of
     a function declarator.  */

  if (funcdef_flag && innermost_code != CALL_EXPR)
    return 0;

  /* Anything declared one level down from the top level
     must be one of the parameters of a function
     (because the body is at least two levels down).  */

  /* If this looks like a function definition, make it one,
     even if it occurs where parms are expected.
     Then store_parm_decls will reject it and not use it as a parm.  */
  if (decl_context == NORMAL && !funcdef_flag
      && current_binding_level->parm_flag)
    decl_context = PARM;

  /* Look through the decl specs and record which ones appear.
     Some typespecs are defined as built-in typenames.
     Others, the ones that are modifiers of other types,
     are represented by bits in SPECBITS: set the bits for
     the modifiers that appear.  Storage class keywords are also in SPECBITS.

     If there is a typedef name or a type, store the type in TYPE.
     This includes builtin typedefs such as `int'.

     Set EXPLICIT_INT or EXPLICIT_CHAR if the type is `int' or `char'
     and did not come from a user typedef.

     Set LONGLONG if `long' is mentioned twice.  */

  for (spec = declspecs; spec; spec = TREE_CHAIN (spec))
    {
#ifndef GPC
      register int i;
#endif
      register tree id = TREE_VALUE (spec);

#ifdef GPC
      /* This is a quick way to conform with rest of
       * grokdeclarator:  The only time id is either const_id
       * or type_id is when we are defining a CONST or TYPE.
       */
      if (id == type_id) {
          specbits |= 1 << (int) RID_TYPEDEF;
          goto found;
      } else if (id == const_id) {
          specbits |= 1 << (int) RID_CONST;
          goto found;
      } else if (id == inline_id) {
          specbits |= 1 << (int) RID_INLINE;
          goto found;
      } else if (id == auto_id) {
          /* Forward declaring a nested routine */
          specbits |= 1 << (int) RID_AUTO;
          goto found;
      } else if (id == varparm_id) {
          varparmp = 1; /* @@@ Is this used? */
          goto found;
      } else if (id == extern_id) {
          specbits |= 1 << (int) RID_EXTERN;
          goto found;
      } else if (id == static_id) {
          specbits |= 1 << (int) RID_STATIC;
          goto found;
      } else if (id == volatile_id) {
          specbits |= 1 << (int) RID_VOLATILE;
          goto found;
      } else if (id == register_id) {
          specbits |= 1 << (int) RID_REGISTER;
          goto found;
      }
#else
      if (id == ridpointers[(int) RID_INT])
        explicit_int = 1;
      if (id == ridpointers[(int) RID_CHAR])
        explicit_char = 1;

      if (TREE_CODE (id) == IDENTIFIER_NODE)
        for (i = (int) RID_FIRST_MODIFIER; i < (int) RID_MAX; i++)
          {
            if (ridpointers[i] == id)
              {
                if (i == (int) RID_LONG && specbits & (1<<i))
                  {
                    if (longlong)
                      error ("`long long long' is too long for GCC");
                    else
                      {
                        if (pedantic && ! in_system_header)
                          pedwarn ("ANSI C does not support `long long'");
                        longlong = 1;
                      }
                  }
                else if (specbits & (1 << i))
                  pedwarn ("duplicate `%s'", IDENTIFIER_POINTER (id));
                specbits |= 1 << i;
                goto found;
              }
          }
#endif /* GPC */
      if (type)
        error ("two or more data types in declaration of `%s'", name);
      /* Actual typedefs come to us as TYPE_DECL nodes.  */
      else if (TREE_CODE (id) == TYPE_DECL)
        {
          type = TREE_TYPE (id);
          decl_machine_attr = DECL_MACHINE_ATTRIBUTES (id);
          typedef_decl = id;
        }
      /* Built-in types come as identifiers.  */
      else if (TREE_CODE (id) == IDENTIFIER_NODE)
        {
          register tree t = lookup_name (id);
          if (TREE_TYPE (t) == error_mark_node)
            ;
          else if (!t || TREE_CODE (t) != TYPE_DECL)
            error ("`%s' fails to be a typedef or built in type",
                   IDENTIFIER_POINTER (id));
          else
            {
              type = TREE_TYPE (t);
              typedef_decl = t;
            }
        }
      else if (TREE_CODE (id) != ERROR_MARK)
        type = id;

    found: {}
    }

  typedef_type = type;
  if (type)
    size_varies = C_TYPE_VARIABLE_SIZE (type);

  /* No type at all: default to `int', and set DEFAULTED_INT
     because it was not a user-defined typedef.  */

  if (type == 0)
    {
      if (! (specbits & ((1 << (int) RID_LONG) | (1 << (int) RID_SHORT)
                         | (1 << (int) RID_SIGNED)
                         | (1 << (int) RID_UNSIGNED))))
        {
          /* C9x will probably require a diagnostic here.
             For now, issue a warning if -Wreturn-type and this is a function,
             or if -Wimplicit; prefer the former warning since it is more
             explicit.  */
          if ((warn_implicit_int || warn_return_type) && funcdef_flag)
            warn_about_return_type = 1;
          else if (warn_implicit_int)
            warning ("type defaults to `int' in declaration of `%s'", name);
        }

      defaulted_int = 1;
      type = integer_type_node;
    }

  /* Now process the modifiers that were specified
     and check for invalid combinations.  */

  /* Long double is a special combination.  */

  if ((specbits & 1 << (int) RID_LONG) && ! longlong
      && TYPE_MAIN_VARIANT (type) == double_type_node)
    {
      specbits &= ~ (1 << (int) RID_LONG);
      type = long_double_type_node;
    }

  /* Check all other uses of type modifiers.  */

  if (specbits & ((1 << (int) RID_LONG) | (1 << (int) RID_SHORT)
                  | (1 << (int) RID_UNSIGNED) | (1 << (int) RID_SIGNED)))
    {
      int ok = 0;

      if ((specbits & 1 << (int) RID_LONG)
          && (specbits & 1 << (int) RID_SHORT))
        error ("both long and short specified for `%s'", name);
      else if (((specbits & 1 << (int) RID_LONG)
                || (specbits & 1 << (int) RID_SHORT))
               && explicit_char)
        error ("long or short specified with char for `%s'", name);
      else if (((specbits & 1 << (int) RID_LONG)
                || (specbits & 1 << (int) RID_SHORT))
               && TREE_CODE (type) == REAL_TYPE)
        {
          static int already = 0;

          error ("long or short specified with floating type for `%s'", name);
          if (! already && ! pedantic)
            {
              error (" the only valid combination is `long double'");
              already = 1;
            }
        }
      else if ((specbits & 1 << (int) RID_SIGNED)
               && (specbits & 1 << (int) RID_UNSIGNED))
        error ("both signed and unsigned specified for `%s'", name);
      else if (TREE_CODE (type) != INTEGER_TYPE)
        error ("long, short, signed or unsigned invalid for `%s'", name);
      else
        {
          ok = 1;
          if (!explicit_int && !defaulted_int && !explicit_char && pedantic)
            {
              pedwarn ("long, short, signed or unsigned used invalidly for `%s'",
                       name);
              if (flag_pedantic_errors)
                ok = 0;
            }
        }

      /* Discard the type modifiers if they are invalid.  */
      if (! ok)
        {
          specbits &= ~((1 << (int) RID_LONG) | (1 << (int) RID_SHORT)
                        | (1 << (int) RID_UNSIGNED) | (1 << (int) RID_SIGNED));
          longlong = 0;
        }
    }

  if ((specbits & (1 << (int) RID_COMPLEX))
      && TREE_CODE (type) != INTEGER_TYPE && TREE_CODE (type) != REAL_TYPE)
    {
      error ("complex invalid for `%s'", name);
      specbits &= ~ (1 << (int) RID_COMPLEX);
    }

#ifndef GPC
  /* Decide whether an integer type is signed or not.
     Optionally treat bitfields as signed by default.  */
  if (specbits & 1 << (int) RID_UNSIGNED
      /* Traditionally, all bitfields are unsigned.  */
      || (bitfield && flag_traditional
          && (! explicit_flag_signed_bitfields || !flag_signed_bitfields))
      || (bitfield && ! flag_signed_bitfields
          && (explicit_int || defaulted_int || explicit_char
              /* A typedef for plain `int' without `signed'
                 can be controlled just like plain `int'.  */
              || ! (typedef_decl != 0
                    && C_TYPEDEF_EXPLICITLY_SIGNED (typedef_decl)))
          && TREE_CODE (type) != ENUMERAL_TYPE
          && !(specbits & 1 << (int) RID_SIGNED)))
    {
      if (longlong)
        type = long_long_unsigned_type_node;
      else if (specbits & 1 << (int) RID_LONG)
        type = long_unsigned_type_node;
      else if (specbits & 1 << (int) RID_SHORT)
        type = short_unsigned_type_node;
      else if (type == char_type_node)
        type = unsigned_char_type_node;
      else if (typedef_decl)
        type = unsigned_type (type);
      else
        type = unsigned_type_node;
    }
  else if ((specbits & 1 << (int) RID_SIGNED)
           && type == char_type_node)
    type = signed_char_type_node;
  else if (longlong)
    type = long_long_integer_type_node;
  else if (specbits & 1 << (int) RID_LONG)
    type = long_integer_type_node;
  else if (specbits & 1 << (int) RID_SHORT)
    type = short_integer_type_node;
#endif /* not GPC */

  if (specbits & 1 << (int) RID_COMPLEX)
    {
      /* If we just have "complex", it is equivalent to
         "complex double", but if any modifiers at all are specified it is
         the complex form of TYPE.  E.g, "complex short" is
         "complex short int".  */

      if (defaulted_int && ! longlong
          && ! (specbits & ((1 << (int) RID_LONG) | (1 << (int) RID_SHORT)
                            | (1 << (int) RID_SIGNED)
                            | (1 << (int) RID_UNSIGNED))))
        type = complex_double_type_node;
      else if (type == integer_type_node)
        type = complex_integer_type_node;
      else if (type == float_type_node)
        type = complex_float_type_node;
      else if (type == double_type_node)
        type = complex_double_type_node;
      else if (type == long_double_type_node)
        type = complex_long_double_type_node;
      else
        type = build_complex_type (type);
    }

  /* Set CONSTP if this declaration is `const', whether by
     explicit specification or via a typedef.
     Likewise for VOLATILEP.  */

  constp = !! (specbits & 1 << (int) RID_CONST) + TYPE_READONLY (type);
  volatilep = !! (specbits & 1 << (int) RID_VOLATILE) + TYPE_VOLATILE (type);
  inlinep = !! (specbits & (1 << (int) RID_INLINE));
  if (constp > 1)
    pedwarn ("duplicate `const'");
  if (volatilep > 1)
    pedwarn ("duplicate `volatile'");
#ifndef GPC  /* What purpose does this serve in C?? */
  if (! flag_gen_aux_info && (TYPE_READONLY (type) || TYPE_VOLATILE (type)))
    type = TYPE_MAIN_VARIANT (type);
#endif /* not GPC */

  /* Warn if two storage classes are given. Default to `auto'.  */

  {
    int nclasses = 0;

    if (specbits & 1 << (int) RID_AUTO) nclasses++;
    if (specbits & 1 << (int) RID_STATIC) nclasses++;
    if (specbits & 1 << (int) RID_EXTERN) nclasses++;
    if (specbits & 1 << (int) RID_REGISTER) nclasses++;
    if (specbits & 1 << (int) RID_TYPEDEF) nclasses++;
    if (specbits & 1 << (int) RID_ITERATOR) nclasses++;

    /* Warn about storage classes that are invalid for certain
       kinds of declarations (parameters, typenames, etc.).  */

    if (nclasses > 1)
      error ("multiple storage classes in declaration of `%s'", name);
    else if (funcdef_flag
             && (specbits
                 & ((1 << (int) RID_REGISTER)
                    | (1 << (int) RID_AUTO)
                    | (1 << (int) RID_TYPEDEF))))
      {
        if (specbits & 1 << (int) RID_AUTO
            && (pedantic || current_binding_level == global_binding_level))
          pedwarn ("function definition declared `auto'");
        if (specbits & 1 << (int) RID_REGISTER)
          error ("function definition declared `register'");
        if (specbits & 1 << (int) RID_TYPEDEF)
          error ("function definition declared `typedef'");
        specbits &= ~ ((1 << (int) RID_TYPEDEF) | (1 << (int) RID_REGISTER)
                       | (1 << (int) RID_AUTO));
      }
    else if (decl_context != NORMAL && nclasses > 0)
      {
        if (decl_context == PARM && specbits & 1 << (int) RID_REGISTER)
          ;
        else
          {
            error ((decl_context == FIELD
                    ? "storage class specified for structure field `%s'"
                    : (decl_context == PARM
                       ? "storage class specified for parameter `%s'"
                       : "storage class specified for typename")),
                   name);
            specbits &= ~ ((1 << (int) RID_TYPEDEF) | (1 << (int) RID_REGISTER)
                           | (1 << (int) RID_AUTO) | (1 << (int) RID_STATIC)
                           | (1 << (int) RID_EXTERN));
          }
      }
    else if (specbits & 1 << (int) RID_EXTERN && initialized && ! funcdef_flag)
      {
        /* `extern' with initialization is invalid if not at top level.  */
        if (current_binding_level == global_binding_level)
          warning ("`%s' initialized and declared `extern'", name);
        else
          error ("`%s' has both `extern' and initializer", name);
      }
    else if (specbits & 1 << (int) RID_EXTERN && funcdef_flag
#ifdef GPC
             && ! pascal_global_bindings_p ())
#else /* not GPC */
             && current_binding_level != global_binding_level)
#endif /* not GPC */
      error ("nested function `%s' declared `extern'", name);
    else if (current_binding_level == global_binding_level
             && specbits & (1 << (int) RID_AUTO))
      error ("top-level declaration of `%s' specifies `auto'", name);
    else if ((specbits & 1 << (int) RID_ITERATOR)
             && TREE_CODE (declarator) != IDENTIFIER_NODE)
      {
        error ("iterator `%s' has derived type", name);
        type = error_mark_node;
      }
    else if ((specbits & 1 << (int) RID_ITERATOR)
             && TREE_CODE (type) != INTEGER_TYPE)
      {
        error ("iterator `%s' has noninteger type", name);
        type = error_mark_node;
      }
  }

  /* Now figure out the structure of the declarator proper.
     Descend through it, creating more complex types, until we reach
     the declared identifier (or NULL_TREE, in an absolute declarator).  */

  while (declarator && TREE_CODE (declarator) != IDENTIFIER_NODE)
    {
      if (type == error_mark_node)
        {
          declarator = TREE_OPERAND (declarator, 0);
          continue;
        }

      /* Each level of DECLARATOR is either an ARRAY_REF (for ...[..]),
         an INDIRECT_REF (for *...),
         a CALL_EXPR (for ...(...)),
         an identifier (for the name being declared)
         or a null pointer (for the place in an absolute declarator
         where the name was omitted).
         For the last two cases, we have just exited the loop.

         At this point, TYPE is the type of elements of an array,
         or for a function to return, or for a pointer to point to.
         After this sequence of ifs, TYPE is the type of the
         array or function or pointer, and DECLARATOR has had its
         outermost layer removed.  */

      if (TREE_CODE (declarator) == ARRAY_REF)
        {
          register tree itype = NULL_TREE;
          register tree size = TREE_OPERAND (declarator, 1);
          /* An uninitialized decl with `extern' is a reference.  */
          int extern_ref = !initialized && (specbits & (1 << (int) RID_EXTERN));
          /* The index is a signed object `sizetype' bits wide.  */
          tree index_type = signed_type (sizetype);

          declarator = TREE_OPERAND (declarator, 0);

          /* Check for some types that there cannot be arrays of.  */

          if (TYPE_MAIN_VARIANT (type) == void_type_node)
            {
              error ("declaration of `%s' as array of voids", name);
              type = error_mark_node;
            }

          if (TREE_CODE (type) == FUNCTION_TYPE)
            {
              error ("declaration of `%s' as array of functions", name);
              type = error_mark_node;
            }

          if (size == error_mark_node)
            type = error_mark_node;

          if (type == error_mark_node)
            continue;

          /* If this is a block level extern, it must live past the end
             of the function so that we can check it against other extern
             declarations (IDENTIFIER_LIMBO_VALUE).  */
          if (extern_ref && allocation_temporary_p ())
            end_temporary_allocation ();

          /* If size was specified, set ITYPE to a range-type for that size.
             Otherwise, ITYPE remains null.  finish_decl may figure it out
             from an initial value.  */

          if (size)
            {
              /* Strip NON_LVALUE_EXPRs since we aren't using as an lvalue.  */
              STRIP_TYPE_NOPS (size);

              if (TREE_CODE (TREE_TYPE (size)) != INTEGER_TYPE
                  && TREE_CODE (TREE_TYPE (size)) != ENUMERAL_TYPE)
                {
                  error ("size of array `%s' has non-integer type", name);
                  size = integer_one_node;
                }

              if (pedantic && integer_zerop (size))
                pedwarn ("ANSI C forbids zero-size array `%s'", name);

              if (TREE_CODE (size) == INTEGER_CST)
                {
                  constant_expression_warning (size);
                  if (tree_int_cst_sgn (size) < 0)
                    {
                      error ("size of array `%s' is negative", name);
                      size = integer_one_node;
                    }
                }
              else
                {
                  /* Make sure the array size remains visibly nonconstant
                     even if it is (eg) a const variable with known value.  */
                  size_varies = 1;

                  if (pedantic)
                    {
                      if (TREE_CONSTANT (size))
                        pedwarn ("ANSI C forbids array `%s' whose size can't be evaluated", name);
                      else
                        pedwarn ("ANSI C forbids variable-size array `%s'", name);
                    }
                }

              /* Convert size to index_type, so that if it is a variable
                 the computations will be done in the proper mode.  */
              itype = fold (build (MINUS_EXPR, index_type,
                                   convert (index_type, size),
                                   convert (index_type, size_one_node)));

              /* If that overflowed, the array is too big.
                 ??? While a size of INT_MAX+1 technically shouldn't cause
                 an overflow (because we subtract 1), the overflow is recorded
                 during the conversion to index_type, before the subtraction.
                 Handling this case seems like an unnecessary complication.  */
              if (TREE_OVERFLOW (itype))
                {
                  error ("size of array `%s' is too large", name);
                  type = error_mark_node;
                  continue;
                }

              if (size_varies)
                itype = variable_size (itype);
              itype = build_index_type (itype);
            }

#if 0 /* This had bad results for pointers to arrays, as in
         union incomplete (*foo)[4];  */
          /* Complain about arrays of incomplete types, except in typedefs.  */

          if (TYPE_SIZE (type) == 0
              /* Avoid multiple warnings for nested array types.  */
              && TREE_CODE (type) != ARRAY_TYPE
              && !(specbits & (1 << (int) RID_TYPEDEF))
              && !C_TYPE_BEING_DEFINED (type))
            warning ("array type has incomplete element type");
#endif

#if 0  /* We shouldn't have a function type here at all!
          Functions aren't allowed as array elements.  */
          if (pedantic && TREE_CODE (type) == FUNCTION_TYPE
              && (constp || volatilep))
            pedwarn ("ANSI C forbids const or volatile function types");
#endif

          /* Build the array type itself, then merge any constancy or
             volatility into the target type.  We must do it in this order
             to ensure that the TYPE_MAIN_VARIANT field of the array type
             is set correctly.  */

          type = build_array_type (type, itype);
          if (constp || volatilep)
            type = build_c_type_variant (type, constp, volatilep);

#if 0   /* don't clear these; leave them set so that the array type
           or the variable is itself const or volatile.  */
          constp = 0;
          volatilep = 0;
#endif

          if (size_varies)
            C_TYPE_VARIABLE_SIZE (type) = 1;
        }
      else if (TREE_CODE (declarator) == CALL_EXPR)
        {
          int extern_ref = (!(specbits & (1 << (int) RID_AUTO))
                            || current_binding_level == global_binding_level);
          tree arg_types;

          /* Declaring a function type.
             Make sure we have a valid type for the function to return.  */
          if (type == error_mark_node)
            continue;

          size_varies = 0;

          /* Warn about some types functions can't return.  */

          if (TREE_CODE (type) == FUNCTION_TYPE)
            {
              error ("`%s' declared as function returning a function", name);
              type = integer_type_node;
            }
#ifndef GPC  /* GPC functions can return arrays */
          if (TREE_CODE (type) == ARRAY_TYPE)
            {
              error ("`%s' declared as function returning an array", name);
              type = integer_type_node;
            }
#endif /* not GPC */

#ifndef TRADITIONAL_RETURN_FLOAT
          /* Traditionally, declaring return type float means double.  */

          if (flag_traditional && TYPE_MAIN_VARIANT (type) == float_type_node)
            type = double_type_node;
#endif /* TRADITIONAL_RETURN_FLOAT */

          /* If this is a block level extern, it must live past the end
             of the function so that we can check it against other extern
             declarations (IDENTIFIER_LIMBO_VALUE).  */
          if (extern_ref && allocation_temporary_p ())
            end_temporary_allocation ();

          /* Construct the function type and go to the next
             inner layer of declarator.  */

          arg_types = grokparms (TREE_OPERAND (declarator, 1),
                                 funcdef_flag
                                 /* Say it's a definition
                                    only for the CALL_EXPR
                                    closest to the identifier.  */
                                 && TREE_CODE (TREE_OPERAND (declarator, 0)) == IDENTIFIER_NODE);
#if 0 /* This seems to be false.  We turn off temporary allocation
         above in this function if -traditional.
         And this code caused inconsistent results with prototypes:
         callers would ignore them, and pass arguments wrong.  */

          /* Omit the arg types if -traditional, since the arg types
             and the list links might not be permanent.  */
          type = build_function_type (type,
                                      flag_traditional
                                      ? NULL_TREE : arg_types);
#endif
          /* ANSI seems to say that `const int foo ();'
             does not make the function foo const.  */
          if (constp || volatilep)
            type = build_c_type_variant (type, constp, volatilep);
          constp = 0;
          volatilep = 0;

          type = build_function_type (type, arg_types);
          declarator = TREE_OPERAND (declarator, 0);

          /* Set the TYPE_CONTEXTs for each tagged type which is local to
             the formal parameter list of this FUNCTION_TYPE to point to
             the FUNCTION_TYPE node itself.  */

          {
            register tree link;

#ifdef GPC
            /* Is this a bug in GCC???
             * start_function() is called and calls grokdeclarator() which in
             * turn calls grokparm() which sets last_function_parm_tags.
             * After that we are again in grokdeclarator() - at this place -
             * where we need current_function_parm_tags which is only set
             * afterwards in start_function().
             */
            for (link = last_function_parm_tags;
#else /* not GPC */
            for (link = current_function_parm_tags;
#endif /* not GPC */
                 link;
                 link = TREE_CHAIN (link))
              TYPE_CONTEXT (TREE_VALUE (link)) = type;
          }
        }
      else if (TREE_CODE (declarator) == INDIRECT_REF)
        {
          /* Merge any constancy or volatility into the target type
             for the pointer.  */

          if (pedantic && TREE_CODE (type) == FUNCTION_TYPE
              && (constp || volatilep))
            pedwarn ("ANSI C forbids const or volatile function types");
          if (constp || volatilep)
            type = build_c_type_variant (type, constp, volatilep);
          constp = 0;
          volatilep = 0;
          size_varies = 0;

          type = build_pointer_type (type);

          /* Process a list of type modifier keywords
             (such as const or volatile) that were given inside the `*'.  */

          if (TREE_TYPE (declarator))
            {
              register tree typemodlist;
              int erred = 0;
              for (typemodlist = TREE_TYPE (declarator); typemodlist;
                   typemodlist = TREE_CHAIN (typemodlist))
                {
#ifndef GPC
                  if (TREE_VALUE (typemodlist) == ridpointers[(int) RID_CONST])
                    constp++;
                  else if (TREE_VALUE (typemodlist) == ridpointers[(int) RID_VOLATILE])
                    volatilep++;
                  else if (!erred)
#endif /* GPC */
                    {
                      erred = 1;
                      error ("invalid type modifier within pointer declarator");
                    }
                }
              if (constp > 1)
                pedwarn ("duplicate `const'");
              if (volatilep > 1)
                pedwarn ("duplicate `volatile'");
            }

          declarator = TREE_OPERAND (declarator, 0);
        }
      else
        abort ();

    }

  /* Now TYPE has the actual type.  */

  /* Did array size calculations overflow?  */

  if (TREE_CODE (type) == ARRAY_TYPE
      && TYPE_SIZE (type)
#ifdef GPC
      && ! size_volatile  /* ignore overflow in undiscriminated schema types */
#endif /* GPC */
      && TREE_OVERFLOW (TYPE_SIZE (type)))
    error ("size of array `%s' is too large", name);

  /* If this is declaring a typedef name, return a TYPE_DECL.  */

  if (specbits & (1 << (int) RID_TYPEDEF))
    {
      tree decl;
      /* Note that the grammar rejects storage classes
         in typenames, fields or parameters */
      if (pedantic && TREE_CODE (type) == FUNCTION_TYPE
          && (constp || volatilep))
        pedwarn ("ANSI C forbids const or volatile function types");
      if (constp || volatilep)
        type = build_c_type_variant (type, constp, volatilep);
      decl = build_decl (TYPE_DECL, declarator, type);
      if ((specbits & (1 << (int) RID_SIGNED))
          || (typedef_decl && C_TYPEDEF_EXPLICITLY_SIGNED (typedef_decl)))
        C_TYPEDEF_EXPLICITLY_SIGNED (decl) = 1;
      pop_obstacks ();
      return decl;
    }

  /* Detect the case of an array type of unspecified size
     which came, as such, direct from a typedef name.
     We must copy the type, so that each identifier gets
     a distinct type, so that each identifier's size can be
     controlled separately by its own initializer.  */

  if (type != 0 && typedef_type != 0
      && TYPE_MAIN_VARIANT (type) == TYPE_MAIN_VARIANT (typedef_type)
      && TREE_CODE (type) == ARRAY_TYPE && TYPE_DOMAIN (type) == 0)
    {
      type = build_array_type (TREE_TYPE (type), 0);
      if (size_varies)
        C_TYPE_VARIABLE_SIZE (type) = 1;
    }

  /* If this is a type name (such as, in a cast or sizeof),
     compute the type and return it now.  */

  if (decl_context == TYPENAME)
    {
      /* Note that the grammar rejects storage classes
         in typenames, fields or parameters */
      if (pedantic && TREE_CODE (type) == FUNCTION_TYPE
          && (constp || volatilep))
        pedwarn ("ANSI C forbids const or volatile function types");
      if (constp || volatilep)
        type = build_c_type_variant (type, constp, volatilep);
      pop_obstacks ();
      return type;
    }

  /* Aside from typedefs and type names (handle above),
     `void' at top level (not within pointer)
     is allowed only in public variables.
     We don't complain about parms either, but that is because
     a better error message can be made later.  */

  if (TYPE_MAIN_VARIANT (type) == void_type_node && decl_context != PARM
      && ! ((decl_context != FIELD && TREE_CODE (type) != FUNCTION_TYPE)
            && ((specbits & (1 << (int) RID_EXTERN))
                || (current_binding_level == global_binding_level
                    && !(specbits
                         & ((1 << (int) RID_STATIC) | (1 << (int) RID_REGISTER)))))))
    {
      error ("variable or field `%s' declared void", name);
      type = integer_type_node;
    }

  /* Now create the decl, which may be a VAR_DECL, a PARM_DECL
     or a FUNCTION_DECL, depending on DECL_CONTEXT and TYPE.  */

  {
    register tree decl;

    if (decl_context == PARM)
      {
        tree type_as_written = type;
        tree main_type;

        /* A parameter declared as an array of T is really a pointer to T.
           One declared as a function is really a pointer to a function.  */

        if (TREE_CODE (type) == ARRAY_TYPE)
          {
#ifndef GPC
            /* Transfer const-ness of array into that of type pointed to.  */
            type = TREE_TYPE (type);
            if (constp || volatilep)
              type = build_c_type_variant (type, constp, volatilep);
            type = build_pointer_type (type);
            volatilep = constp = 0;
            size_varies = 0;
#endif /* not GPC */
          }
        else if (TREE_CODE (type) == FUNCTION_TYPE)
          {
#ifndef GPC
            if (pedantic && (constp || volatilep))
              pedwarn ("ANSI C forbids const or volatile function types");
#endif /* not GPC */
            if (constp || volatilep)
              type = build_c_type_variant (type, constp, volatilep);
#ifdef GPC
            type = build_reference_type (type);
#else /* not GPC */
            type = build_pointer_type (type);
#endif /* not GPC */
            volatilep = constp = 0;
          }

        decl = build_decl (PARM_DECL, declarator, type);
        if (size_varies)
          C_DECL_VARIABLE_SIZE (decl) = 1;

        /* Compute the type actually passed in the parmlist,
           for the case where there is no prototype.
           (For example, shorts and chars are passed as ints.)
           When there is a prototype, this is overridden later.  */

        DECL_ARG_TYPE (decl) = type;
        main_type = (type == error_mark_node
                     ? error_mark_node
                     : TYPE_MAIN_VARIANT (type));
        if (main_type == float_type_node)
          DECL_ARG_TYPE (decl) = double_type_node;
        /* Don't use TYPE_PRECISION to decide whether to promote,
           because we should convert short if it's the same size as int,
           but we should not convert long if it's the same size as int.  */
        else if (TREE_CODE (main_type) != ERROR_MARK
                 && C_PROMOTING_INTEGER_TYPE_P (main_type))
          {
            if (TYPE_PRECISION (type) == TYPE_PRECISION (integer_type_node)
                && TREE_UNSIGNED (type))
              DECL_ARG_TYPE (decl) = unsigned_type_node;
            else
              DECL_ARG_TYPE (decl) = integer_type_node;
          }

        DECL_ARG_TYPE_AS_WRITTEN (decl) = type_as_written;
      }
    else if (decl_context == FIELD)
      {
#ifdef GPC
        /* Structure field.  A FUNCTION_TYPE indicates a method.
         */
        if (TREE_CODE (type) == FUNCTION_TYPE)
          {
            /* Just don't complain. */
          }
#else /* not GPC */
        /* Structure field.  It may not be a function.  */

        if (TREE_CODE (type) == FUNCTION_TYPE)
          {
            error ("field `%s' declared as a function", name);
            type = build_pointer_type (type);
          }
#endif /* not GPC */
        else if (TREE_CODE (type) != ERROR_MARK && TYPE_SIZE (type) == 0)
          {
            error ("field `%s' has incomplete type", name);
            type = error_mark_node;
          }
        /* Move type qualifiers down to element of an array.  */
        if (TREE_CODE (type) == ARRAY_TYPE && (constp || volatilep))
          {
            type = build_array_type (build_c_type_variant (TREE_TYPE (type),
                                                           constp, volatilep),
                                     TYPE_DOMAIN (type));
#if 0 /* Leave the field const or volatile as well.  */
            constp = volatilep = 0;
#endif
          }
#ifdef GPC
        if (TREE_CODE (type) == FUNCTION_TYPE)
          {
            /* A method is a FUNCTION_DECL to an already forward-declared
             * function, not a FIELD_DECL.
             */
            decl = copy_node (lookup_name (get_method_name (current_type_name,
                                                            declarator)));
            DECL_NAME (decl) = declarator;

            /* Set this to something other than an INTEGER_CST node
             * to suppress a crash when writing out debugging info.
             * @@@@@ This must be rewritten in a clean way using the
             * @@@@@ object backend of GCC, like OBJC and C++ do ...
             * ... no, they also do hacks like I do here, so what???
             */
            DECL_FIELD_BITPOS (decl) = error_mark_node;
          }
        else
          decl = build_decl (FIELD_DECL, declarator, type);
#else
        decl = build_decl (FIELD_DECL, declarator, type);
#endif /* GPC */
        if (size_varies)
          C_DECL_VARIABLE_SIZE (decl) = 1;
      }
    else if (TREE_CODE (type) == FUNCTION_TYPE)
      {
        /* Every function declaration is "external"
           except for those which are inside a function body
           in which `auto' is used.
           That is a case not specified by ANSI C,
           and we use it for forward declarations for nested functions.  */
        int extern_ref = (!(specbits & (1 << (int) RID_AUTO))
                          || current_binding_level == global_binding_level);

        if (specbits & (1 << (int) RID_AUTO)
            && (pedantic || current_binding_level == global_binding_level))
          pedwarn ("invalid storage class for function `%s'", name);
        if (specbits & (1 << (int) RID_REGISTER))
          error ("invalid storage class for function `%s'", name);
        /* Function declaration not at top level.
           Storage classes other than `extern' are not allowed
           and `extern' makes no difference.  */
        if (current_binding_level != global_binding_level
            && (specbits & ((1 << (int) RID_STATIC) | (1 << (int) RID_INLINE)))
            && pedantic)
          pedwarn ("invalid storage class for function `%s'", name);

        /* If this is a block level extern, it must live past the end
           of the function so that we can check it against other
           extern declarations (IDENTIFIER_LIMBO_VALUE).  */
        if (extern_ref && allocation_temporary_p ())
          end_temporary_allocation ();

        decl = build_decl (FUNCTION_DECL, declarator, type);
        decl = build_decl_attribute_variant (decl, decl_machine_attr);

        if (pedantic && (constp || volatilep)
            && ! DECL_IN_SYSTEM_HEADER (decl))
          pedwarn ("ANSI C forbids const or volatile functions");

        if (pedantic
            && TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (decl))) == void_type_node
            && (TYPE_READONLY (TREE_TYPE (TREE_TYPE (decl)))
                || TYPE_VOLATILE (TREE_TYPE (TREE_TYPE (decl))))
            && ! DECL_IN_SYSTEM_HEADER (decl))
          pedwarn ("ANSI C forbids const or volatile void function return type");

        if (volatilep
            && TREE_TYPE (TREE_TYPE (decl)) != void_type_node)
          warning ("`noreturn' function returns non-void value");

        if (extern_ref)
          DECL_EXTERNAL (decl) = 1;
        /* Record absence of global scope for `static' or `auto'.  */
        TREE_PUBLIC (decl)
          = !(specbits & ((1 << (int) RID_STATIC) | (1 << (int) RID_AUTO)));

        /* Record presence of `inline', if it is reasonable.  */
        if (inlinep)
          {
            tree last = tree_last (TYPE_ARG_TYPES (type));

            if (! strcmp (IDENTIFIER_POINTER (declarator), "main"))
              warning ("cannot inline function `main'");
            else if (last && (TYPE_MAIN_VARIANT (TREE_VALUE (last))
                              != void_type_node))
              warning ("inline declaration ignored for function with `...'");
            else
              /* Assume that otherwise the function can be inlined.  */
              DECL_INLINE (decl) = 1;

            if (specbits & (1 << (int) RID_EXTERN))
              current_extern_inline = 1;
          }
      }
    else
      {
        /* It's a variable.  */
        /* An uninitialized decl with `extern' is a reference.  */
        int extern_ref = !initialized && (specbits & (1 << (int) RID_EXTERN));

#ifndef GPC /* Causes return value assignment of array functions to fail */
        /* Move type qualifiers down to element of an array.  */
        if (TREE_CODE (type) == ARRAY_TYPE && (constp || volatilep))
          {
            type = build_array_type (build_c_type_variant (TREE_TYPE (type),
                                                           constp, volatilep),
                                     TYPE_DOMAIN (type));
#if 0 /* Leave the variable const or volatile as well.  */
            constp = volatilep = 0;
#endif
          }
#endif /* not GPC */

        /* If this is a block level extern, it must live past the end
           of the function so that we can check it against other
           extern declarations (IDENTIFIER_LIMBO_VALUE).  */
        if (extern_ref && allocation_temporary_p ())
          end_temporary_allocation ();

        decl = build_decl (VAR_DECL, declarator, type);
        if (size_varies)
          C_DECL_VARIABLE_SIZE (decl) = 1;

        if (inlinep)
          pedwarn_with_decl (decl, "variable `%s' declared `inline'");

        DECL_EXTERNAL (decl) = extern_ref;
        /* At top level, the presence of a `static' or `register' storage
           class specifier, or the absence of all storage class specifiers
           makes this declaration a definition (perhaps tentative).  Also,
           the absence of both `static' and `register' makes it public.  */
#ifdef GPC
        if (pascal_global_bindings_p ())
#else /* not GPC */
        if (current_binding_level == global_binding_level)
#endif /* not GPC */
          {
            TREE_PUBLIC (decl)
              = !(specbits
                  & ((1 << (int) RID_STATIC) | (1 << (int) RID_REGISTER)));
            TREE_STATIC (decl) = ! DECL_EXTERNAL (decl);
          }
        /* Not at top level, only `static' makes a static definition.  */
        else
          {
            TREE_STATIC (decl) = (specbits & (1 << (int) RID_STATIC)) != 0;
            TREE_PUBLIC (decl) = DECL_EXTERNAL (decl);
          }

        if (specbits & 1 << (int) RID_ITERATOR)
          ITERATOR_P (decl) = 1;
      }

    /* Record `register' declaration for warnings on &
       and in case doing stupid register allocation.  */

    if (specbits & (1 << (int) RID_REGISTER))
      DECL_REGISTER (decl) = 1;

    /* Record constancy and volatility.  */

    if (constp)
      TREE_READONLY (decl) = 1;
    if (volatilep)
      {
        TREE_SIDE_EFFECTS (decl) = 1;
        TREE_THIS_VOLATILE (decl) = 1;
      }
    /* If a type has volatile components, it should be stored in memory.
       Otherwise, the fact that those components are volatile
       will be ignored, and would even crash the compiler.  */
    if (C_TYPE_FIELDS_VOLATILE (TREE_TYPE (decl)))
      mark_addressable (decl);

    pop_obstacks ();

    return decl;
  }
}

/* Decode the parameter-list info for a function type or function definition.
   The argument is the value returned by `get_parm_info' (or made in parse.y
   if there is an identifier list instead of a parameter decl list).
   These two functions are separate because when a function returns
   or receives functions then each is called multiple times but the order
   of calls is different.  The last call to `grokparms' is always the one
   that contains the formal parameter names of a function definition.

   Store in `last_function_parms' a chain of the decls of parms.
   Also store in `last_function_parm_tags' a chain of the struct, union,
   and enum tags declared among the parms.

   Return a list of arg types to use in the FUNCTION_TYPE for this function.

   FUNCDEF_FLAG is nonzero for a function definition, 0 for
   a mere declaration.  A nonempty identifier-list gets an error message
   when FUNCDEF_FLAG is zero.  */

static tree
grokparms (parms_info, funcdef_flag)
     tree parms_info;
     int funcdef_flag;
{
  tree first_parm = TREE_CHAIN (parms_info);

  last_function_parms = TREE_PURPOSE (parms_info);
  last_function_parm_tags = TREE_VALUE (parms_info);

  if (warn_strict_prototypes && first_parm == 0 && !funcdef_flag
      && !in_system_header)
    warning ("function declaration isn't a prototype");

  if (first_parm != 0
      && TREE_CODE (TREE_VALUE (first_parm)) == IDENTIFIER_NODE)
    {
      if (! funcdef_flag)
        pedwarn ("parameter names (without types) in function declaration");

      last_function_parms = first_parm;
      return 0;
    }
  else
    {
      tree parm;
      tree typelt;
      /* We no longer test FUNCDEF_FLAG.
         If the arg types are incomplete in a declaration,
         they must include undefined tags.
         These tags can never be defined in the scope of the declaration,
         so the types can never be completed,
         and no call can be compiled successfully.  */
#if 0
      /* In a fcn definition, arg types must be complete.  */
      if (funcdef_flag)
#endif
        for (parm = last_function_parms, typelt = first_parm;
             parm;
             parm = TREE_CHAIN (parm))
          /* Skip over any enumeration constants declared here.  */
          if (TREE_CODE (parm) == PARM_DECL)
            {
              /* Barf if the parameter itself has an incomplete type.  */
              tree type = TREE_VALUE (typelt);
              if (TYPE_SIZE (type) == 0)
                {
                  if (funcdef_flag && DECL_NAME (parm) != 0)
                    error ("parameter `%s' has incomplete type",
                           IDENTIFIER_POINTER (DECL_NAME (parm)));
#ifdef GPC
                  else if (! (TREE_CODE (type) == LANG_TYPE
                              && TYPE_NAME (type)
                              && TYPE_NAME (type) == current_type_name))
#else
                  else
#endif
                    warning ("parameter has incomplete type");
                  if (funcdef_flag)
                    {
                      TREE_VALUE (typelt) = error_mark_node;
                      TREE_TYPE (parm) = error_mark_node;
                    }
                }
#if 0  /* This has been replaced by parm_tags_warning
          which uses a more accurate criterion for what to warn about.  */
              else
                {
                  /* Now warn if is a pointer to an incomplete type.  */
                  while (TREE_CODE (type) == POINTER_TYPE
                         || TREE_CODE (type) == REFERENCE_TYPE)
                    type = TREE_TYPE (type);
                  type = TYPE_MAIN_VARIANT (type);
                  if (TYPE_SIZE (type) == 0)
                    {
                      if (DECL_NAME (parm) != 0)
                        warning ("parameter `%s' points to incomplete type",
                                 IDENTIFIER_POINTER (DECL_NAME (parm)));
                      else
                        warning ("parameter points to incomplete type");
                    }
                }
#endif
              typelt = TREE_CHAIN (typelt);
            }

      /* Allocate the list of types the way we allocate a type.  */
      if (first_parm && ! TREE_PERMANENT (first_parm))
        {
          /* Construct a copy of the list of types
             on the saveable obstack.  */
          tree result = NULL;
          for (typelt = first_parm; typelt; typelt = TREE_CHAIN (typelt))
            result = saveable_tree_cons (NULL_TREE, TREE_VALUE (typelt),
                                         result);
          return nreverse (result);
        }
      else
        /* The list we have is permanent already.  */
        return first_parm;
    }
}


/* Return a tree_list node with info on a parameter list just parsed.
   The TREE_PURPOSE is a chain of decls of those parms.
   The TREE_VALUE is a list of structure, union and enum tags defined.
   The TREE_CHAIN is a list of argument types to go in the FUNCTION_TYPE.
   This tree_list node is later fed to `grokparms'.

   VOID_AT_END nonzero means append `void' to the end of the type-list.
   Zero means the parmlist ended with an ellipsis so don't append `void'.  */

tree
get_parm_info (void_at_end)
     int void_at_end;
{
  register tree decl, t;
  register tree types = 0;
  int erred = 0;
  tree tags = gettags ();
  tree parms = getdecls ();
  tree new_parms = 0;
  tree order = current_binding_level->parm_order;

  /* Just `void' (and no ellipsis) is special.  There are really no parms.  */
  if (void_at_end && parms != 0
      && TREE_CHAIN (parms) == 0
      && TYPE_MAIN_VARIANT (TREE_TYPE (parms)) == void_type_node
      && DECL_NAME (parms) == 0)
    {
      parms = NULL_TREE;
      storedecls (NULL_TREE);
      return saveable_tree_cons (NULL_TREE, NULL_TREE,
                                 saveable_tree_cons (NULL_TREE, void_type_node, NULL_TREE));
    }

  /* Extract enumerator values and other non-parms declared with the parms.
     Likewise any forward parm decls that didn't have real parm decls.  */
  for (decl = parms; decl; )
    {
      tree next = TREE_CHAIN (decl);

      if (TREE_CODE (decl) != PARM_DECL)
        {
          TREE_CHAIN (decl) = new_parms;
          new_parms = decl;
        }
      else if (TREE_ASM_WRITTEN (decl))
        {
          error_with_decl (decl, "parameter `%s' has just a forward declaration");
          TREE_CHAIN (decl) = new_parms;
          new_parms = decl;
        }
      decl = next;
    }

  /* Put the parm decls back in the order they were in in the parm list.  */
  for (t = order; t; t = TREE_CHAIN (t))
    {
      if (TREE_CHAIN (t))
        TREE_CHAIN (TREE_VALUE (t)) = TREE_VALUE (TREE_CHAIN (t));
      else
        TREE_CHAIN (TREE_VALUE (t)) = 0;
    }

  new_parms = chainon (order ? nreverse (TREE_VALUE (order)) : 0,
                       new_parms);

  /* Store the parmlist in the binding level since the old one
     is no longer a valid list.  (We have changed the chain pointers.)  */
  storedecls (new_parms);

  for (decl = new_parms; decl; decl = TREE_CHAIN (decl))
    /* There may also be declarations for enumerators if an enumeration
       type is declared among the parms.  Ignore them here.  */
    if (TREE_CODE (decl) == PARM_DECL)
      {
        /* Since there is a prototype,
           args are passed in their declared types.  */
        tree type = TREE_TYPE (decl);
        DECL_ARG_TYPE (decl) = type;
#ifdef PROMOTE_PROTOTYPES
#ifdef GPC
        if (INTEGRAL_TYPE_P (type)
            && TYPE_PRECISION (type) < TYPE_PRECISION (integer_type_node))
          DECL_ARG_TYPE (decl) = integer_type_node;
#else
        if ((TREE_CODE (type) == INTEGER_TYPE
             || TREE_CODE (type) == ENUMERAL_TYPE)
            && TYPE_PRECISION (type) < TYPE_PRECISION (integer_type_node))
          DECL_ARG_TYPE (decl) = integer_type_node;
#endif /* GPC */
#endif

        types = saveable_tree_cons (NULL_TREE, TREE_TYPE (decl), types);
        if (TYPE_MAIN_VARIANT (TREE_VALUE (types)) == void_type_node && ! erred
            && DECL_NAME (decl) == 0)
          {
            error ("`void' in parameter list must be the entire list");
            erred = 1;
          }
      }

  if (void_at_end)
    return saveable_tree_cons (new_parms, tags,
                               nreverse (saveable_tree_cons (NULL_TREE, void_type_node, types)));

  return saveable_tree_cons (new_parms, tags, nreverse (types));
}

/* At end of parameter list, warn about any struct, union or enum tags
   defined within.  Do so because these types cannot ever become complete.  */

void
parmlist_tags_warning ()
{
  tree elt;
#ifndef GPC
  static int already;
#endif

  for (elt = current_binding_level->tags; elt; elt = TREE_CHAIN (elt))
    {
      enum tree_code code = TREE_CODE (TREE_VALUE (elt));
      /* An anonymous union parm type is meaningful as a GNU extension.
         So don't warn for that.  */
      if (code == UNION_TYPE && TREE_PURPOSE (elt) == 0 && !pedantic)
        continue;
#ifdef GPC
      if (TREE_PURPOSE (elt))
        error ("unresolved forward reference `%s'",
               IDENTIFIER_POINTER (TREE_PURPOSE (elt)));
#else /* not GPC */
      if (TREE_PURPOSE (elt) != 0)
        warning ("`%s %s' declared inside parameter list",
                 (code == RECORD_TYPE ? "struct"
                  : code == UNION_TYPE ? "union"
                  : "enum"),
                 IDENTIFIER_POINTER (TREE_PURPOSE (elt)));
      else
        warning ("anonymous %s declared inside parameter list",
                 (code == RECORD_TYPE ? "struct"
                  : code == UNION_TYPE ? "union"
                  : "enum"));

      if (! already)
        {
          warning (" its scope is only this definition or declaration,");
          warning (" which is probably not what you want.");
          already = 1;
        }
#endif /* GPC */
    }
}

/* Get the struct, enum or union (CODE says which) with tag NAME.
   Define the tag as a forward-reference if it is not defined.  */

tree
xref_tag (code, name)
     enum tree_code code;
     tree name;
{
  int temporary = allocation_temporary_p ();

  /* If a cross reference is requested, look up the type
     already defined for this tag and return it.  */

  register tree ref = lookup_tag (code, name, current_binding_level, 0);
  /* Even if this is the wrong type of tag, return what we found.
     There will be an error message anyway, from pending_xref_error.
     If we create an empty xref just for an invalid use of the type,
     the main result is to create lots of superfluous error messages.  */
  if (ref)
    return ref;

  push_obstacks_nochange ();

  if (current_binding_level == global_binding_level && temporary)
    end_temporary_allocation ();

  /* If no such tag is yet defined, create a forward-reference node
     and record it as the "definition".
     When a real declaration of this type is found,
     the forward-reference will be altered into a real type.  */

  ref = make_node (code);
  if (code == ENUMERAL_TYPE)
    {
      /* (In ANSI, Enums can be referred to only if already defined.)  */
      if (pedantic)
        pedwarn ("ANSI C forbids forward references to `enum' types");
      /* Give the type a default layout like unsigned int
         to avoid crashing if it does not get defined.  */
      TYPE_MODE (ref) = TYPE_MODE (unsigned_type_node);
      TYPE_ALIGN (ref) = TYPE_ALIGN (unsigned_type_node);
      TREE_UNSIGNED (ref) = 1;
      TYPE_PRECISION (ref) = TYPE_PRECISION (unsigned_type_node);
      TYPE_MIN_VALUE (ref) = TYPE_MIN_VALUE (unsigned_type_node);
      TYPE_MAX_VALUE (ref) = TYPE_MAX_VALUE (unsigned_type_node);
    }

  pushtag (name, ref);

  pop_obstacks ();

  return ref;
}

/* Make sure that the tag NAME is defined *in the current binding level*
   at least as a forward reference.
   CODE says which kind of tag NAME ought to be.

   We also do a push_obstacks_nochange
   whose matching pop is in finish_struct.  */

tree
start_struct (code, name)
     enum tree_code code;
     tree name;
{
  /* If there is already a tag defined at this binding level
     (as a forward reference), just return it.  */

  register tree ref = 0;

  push_obstacks_nochange ();
  if (current_binding_level == global_binding_level)
    end_temporary_allocation ();

  if (name != 0)
    ref = lookup_tag (code, name, current_binding_level, 1);
  if (ref && TREE_CODE (ref) == code)
    {
      C_TYPE_BEING_DEFINED (ref) = 1;
      TYPE_PACKED (ref) = flag_pack_struct;
      if (TYPE_FIELDS (ref))
        error ((code == UNION_TYPE ? "redefinition of `union %s'"
                : "redefinition of `struct %s'"),
               IDENTIFIER_POINTER (name));

      return ref;
    }

  /* Otherwise create a forward-reference just so the tag is in scope.  */

  ref = make_node (code);
  pushtag (name, ref);
  C_TYPE_BEING_DEFINED (ref) = 1;
  TYPE_PACKED (ref) = flag_pack_struct;
  return ref;
}

/* Process the specs, declarator (NULL if omitted) and width (NULL if omitted)
   of a structure component, returning a FIELD_DECL node.
   WIDTH is non-NULL for bit fields only, and is an INTEGER_CST node.

   This is done during the parsing of the struct declaration.
   The FIELD_DECL nodes are chained together and the lot of them
   are ultimately passed to `build_struct' to make the RECORD_TYPE node.  */

#ifdef EGCS92
tree
grokfield (filename, line, declarator, declspecs, width)
     const char *filename;
     int line;
     tree declarator, declspecs, width;
#else
tree
grokfield (filename, line, declarator, declspecs, width)
     char *filename;
     int line;
     tree declarator, declspecs, width;
#endif
{
  tree value;

  /* The corresponding pop_obstacks is in finish_decl.  */
  push_obstacks_nochange ();

  value = grokdeclarator (declarator, declspecs, width ? BITFIELD : FIELD, 0);

  finish_decl (value, NULL_TREE, NULL_TREE);
  DECL_INITIAL (value) = width;

  maybe_objc_check_decl (value);
  return value;
}
#ifdef GPC
/*
 * For Pascal variant records:
 *
 *   INFO is a TREE_LIST node whose
 *   TREE_VALUE is the FIELD_DECL node (in records field list)
 *    that is defined as a variant tag
 *   TREE_PURPOSE is the type of the tag field. If TREE_VALUE
 *    is NULL_TREE, the tag field is specified only as a type;
 *   if TYPE_TAG_FIELD is NULL_TREE the record has no variant part.
 *
 *  @@@ This info is stored, but currently not used.
 */

/* Allocate a structure for TYPE_LANG_SPECIFIC.
 */
struct lang_type *
allocate_type_lang_specific ()
{
  char *space;
  /* Allocate additional space for object extensions.
   * @@@ Should be rewritten to use BINFO -- that is cleaner
   * @@@ (really??) and allows for debugging info!
   */
  if (allocation_temporary_p ())
    space = savealloc (sizeof (struct lang_type) + sizeof (tree));
  else
    space = oballoc (sizeof (struct lang_type) + sizeof (tree));
  bzero (space, sizeof (struct lang_type) + sizeof (tree));

  return (struct lang_type *) space;
}

/* Allocate a structure for DECL_LANG_SPECIFIC.
 */
struct lang_decl *
allocate_decl_lang_specific ()
{
  char *space;
  if (allocation_temporary_p ())
    space = savealloc (sizeof (struct lang_decl) + sizeof (tree));
  else
    space = oballoc (sizeof (struct lang_decl) + sizeof (tree));
  bzero (space, sizeof (struct lang_decl) + sizeof (tree));

  return (struct lang_decl *) space;
}

void
store_variant_tag_info (record, info)
     tree record;
     tree info;
{
  if (TREE_CODE (record) != RECORD_TYPE
      && TREE_CODE (record) != UNION_TYPE)
    abort ();

  if (TYPE_LANG_SPECIFIC (record))
    {
      if (TYPE_LANG_INFO (record))
        abort ();
    }
  else
    TYPE_LANG_SPECIFIC (record) = allocate_type_lang_specific ();

  TYPE_LANG_CODE (record) = PASCAL_LANG_VARIANT_RECORD;
  TYPE_LANG_INFO (record) = info;
}

tree
get_variant_tag_info (record)
     tree record;
{
  if ((TREE_CODE (record) != RECORD_TYPE
       && TREE_CODE (record) != UNION_TYPE)
      || (TYPE_LANG_SPECIFIC (record)
          && TYPE_LANG_CODE (record) != PASCAL_LANG_VARIANT_RECORD))
    abort ();

  if (TYPE_LANG_SPECIFIC (record))
    return TYPE_LANG_INFO (record);

  return NULL_TREE;
}
#endif /* GPC */

#ifndef GPC
/* Function to help qsort sort FIELD_DECLs by name order.  */

static int
field_decl_cmp (xp, yp)
     const GENERIC_PTR xp;
     const GENERIC_PTR yp;
{
  tree *x = (tree *)xp, *y = (tree *)yp;

  if (DECL_NAME (*x) == DECL_NAME (*y))
    return 0;
  if (DECL_NAME (*x) == NULL)
    return -1;
  if (DECL_NAME (*y) == NULL)
    return 1;
  if (DECL_NAME (*x) < DECL_NAME (*y))
    return -1;
  return 1;
}
#endif /* not GPC */

/* Fill in the fields of a RECORD_TYPE or UNION_TYPE node, T.
   FIELDLIST is a chain of FIELD_DECL nodes for the fields.
   ATTRIBUTES are attributes to be applied to the structure.

   We also do a pop_obstacks to match the push in start_struct.  */

tree
finish_struct (t, fieldlist, attributes)
     tree t;
     tree fieldlist;
     tree attributes;
{
  register tree x;
  int old_momentary;
  int toplevel = global_binding_level == current_binding_level;

  /* If this type was previously laid out as a forward reference,
     make sure we lay it out again.  */

  TYPE_SIZE (t) = 0;
#ifdef GPC
#ifdef EGCS
  TYPE_SIZE_UNIT (t) = 0;
#endif /* EGCS */
#endif /* GPC */

  decl_attributes (t, attributes, NULL_TREE);

#ifdef GPC

  /* Record types cannot be explicitly defined in parameter lists.
   * With schemata, we sometimes define them implicitly.  In any
   * case, this warning makes no sense.
   */

#else /* not GPC */

  /* Nameless union parm types are useful as GCC extension.  */
  if (! (TREE_CODE (t) == UNION_TYPE && TYPE_NAME (t) == 0) && !pedantic)
    /* Otherwise, warn about any struct or union def. in parmlist.  */
    if (in_parm_level_p ())
      {
        if (pedantic)
          pedwarn ((TREE_CODE (t) == UNION_TYPE ? "union defined inside parms"
                    : "structure defined inside parms"));
        else if (! flag_traditional)
          warning ((TREE_CODE (t) == UNION_TYPE ? "union defined inside parms"
                    : "structure defined inside parms"));
      }

#endif /* not GPC */

  old_momentary = suspend_momentary ();

  if (pedantic)
    {
      for (x = fieldlist; x; x = TREE_CHAIN (x))
        if (DECL_NAME (x) != 0)
          break;

      if (x == 0)
        pedwarn ("%s has no %smembers",
                 (TREE_CODE (t) == UNION_TYPE ? "union" : "structure"),
                 (fieldlist ? "named " : ""));
    }

  /* Install struct as DECL_CONTEXT of each field decl.
     Also process specified field sizes.
     Set DECL_FIELD_SIZE to the specified size, or 0 if none specified.
     The specified size is found in the DECL_INITIAL.
     Store 0 there, except for ": 0" fields (so we can find them
     and delete them, below).  */

  for (x = fieldlist; x; x = TREE_CHAIN (x))
    {
      DECL_CONTEXT (x) = t;
      DECL_PACKED (x) |= TYPE_PACKED (t);
      DECL_FIELD_SIZE (x) = 0;

      /* If any field is const, the structure type is pseudo-const.  */
      if (TREE_READONLY (x))
        C_TYPE_FIELDS_READONLY (t) = 1;
      else
        {
          /* A field that is pseudo-const makes the structure likewise.  */
          tree t1 = TREE_TYPE (x);
          while (TREE_CODE (t1) == ARRAY_TYPE)
            t1 = TREE_TYPE (t1);
          if ((TREE_CODE (t1) == RECORD_TYPE || TREE_CODE (t1) == UNION_TYPE)
              && C_TYPE_FIELDS_READONLY (t1))
            C_TYPE_FIELDS_READONLY (t) = 1;
        }

      /* Any field that is volatile means variables of this type must be
         treated in some ways as volatile.  */
      if (TREE_THIS_VOLATILE (x))
        C_TYPE_FIELDS_VOLATILE (t) = 1;

      /* Any field of nominal variable size implies structure is too.  */
      if (C_DECL_VARIABLE_SIZE (x))
        C_TYPE_VARIABLE_SIZE (t) = 1;

      /* Detect invalid nested redefinition.  */
      if (TREE_TYPE (x) == t)
        error ("nested redefinition of `%s'",
               IDENTIFIER_POINTER (TYPE_NAME (t)));

      /* Detect invalid bit-field size.  */
      if (DECL_INITIAL (x))
        STRIP_NOPS (DECL_INITIAL (x));
      if (DECL_INITIAL (x))
        {
          if (TREE_CODE (DECL_INITIAL (x)) == INTEGER_CST)
            constant_expression_warning (DECL_INITIAL (x));
          else
            {
              error_with_decl (x, "bit-field `%s' width not an integer constant");
              DECL_INITIAL (x) = NULL;
            }
        }

      /* Detect invalid bit-field type.  */
#ifdef GPC
      if (DECL_INITIAL (x)
          && ! ORDINAL_TYPE (TREE_CODE (TREE_TYPE (x))))
        {
          error_with_decl (x, "Internal GPC error: packed field `%s' has invalid type");
          DECL_INITIAL (x) = NULL;
        }
#else /* not GPC */
      if (DECL_INITIAL (x)
          && TREE_CODE (TREE_TYPE (x)) != INTEGER_TYPE
          && TREE_CODE (TREE_TYPE (x)) != ENUMERAL_TYPE)
        {
          error_with_decl (x, "bit-field `%s' has invalid type");
          DECL_INITIAL (x) = NULL;
        }
      if (DECL_INITIAL (x) && pedantic
          && TYPE_MAIN_VARIANT (TREE_TYPE (x)) != integer_type_node
          && TYPE_MAIN_VARIANT (TREE_TYPE (x)) != unsigned_type_node
          /* Accept an enum that's equivalent to int or unsigned int.  */
          && !(TREE_CODE (TREE_TYPE (x)) == ENUMERAL_TYPE
               && (TYPE_PRECISION (TREE_TYPE (x))
                   == TYPE_PRECISION (integer_type_node))))
        pedwarn_with_decl (x, "bit-field `%s' type invalid in ANSI C");
#endif /* not GPC */

      /* Detect and ignore out of range field width.  */
      if (DECL_INITIAL (x))
        {
          unsigned HOST_WIDE_INT width = TREE_INT_CST_LOW (DECL_INITIAL (x));

          if (tree_int_cst_sgn (DECL_INITIAL (x)) < 0)
            {
              DECL_INITIAL (x) = NULL;
              error_with_decl (x, "negative width in bit-field `%s'");
            }
          else if (TREE_INT_CST_HIGH (DECL_INITIAL (x)) != 0
                   || width > TYPE_PRECISION (TREE_TYPE (x)))
            {
              DECL_INITIAL (x) = NULL;
              pedwarn_with_decl (x, "width of `%s' exceeds its type");
            }
          else if (width == 0 && DECL_NAME (x) != 0)
            {
              error_with_decl (x, "zero width for bit-field `%s'");
              DECL_INITIAL (x) = NULL;
            }
        }

      /* Process valid field width.  */
      if (DECL_INITIAL (x))
        {
          register int width = TREE_INT_CST_LOW (DECL_INITIAL (x));

          if (TREE_CODE (TREE_TYPE (x)) == ENUMERAL_TYPE
              && (width < min_precision (TYPE_MIN_VALUE (TREE_TYPE (x)),
                                         TREE_UNSIGNED (TREE_TYPE (x)))
                  || width < min_precision (TYPE_MAX_VALUE (TREE_TYPE (x)),
                                            TREE_UNSIGNED (TREE_TYPE (x)))))
            warning_with_decl (x, "`%s' is narrower than values of its type");

          DECL_FIELD_SIZE (x) = width;
          DECL_BIT_FIELD (x) = DECL_C_BIT_FIELD (x) = 1;
          DECL_INITIAL (x) = NULL;

          if (width == 0)
            {
              /* field size 0 => force desired amount of alignment.  */
#ifdef EMPTY_FIELD_BOUNDARY
              DECL_ALIGN (x) = MAX (DECL_ALIGN (x), EMPTY_FIELD_BOUNDARY);
#endif
#ifdef PCC_BITFIELD_TYPE_MATTERS
              if (PCC_BITFIELD_TYPE_MATTERS)
                DECL_ALIGN (x) = MAX (DECL_ALIGN (x),
                                      TYPE_ALIGN (TREE_TYPE (x)));
#endif
            }
        }
      else if (TREE_TYPE (x) != error_mark_node)
        {
          int min_align = (DECL_PACKED (x) ? BITS_PER_UNIT
                           : TYPE_ALIGN (TREE_TYPE (x)));
          /* Non-bit-fields are aligned for their type, except packed
             fields which require only BITS_PER_UNIT alignment.  */
          DECL_ALIGN (x) = MAX (DECL_ALIGN (x), min_align);
        }
    }

  /* Now DECL_INITIAL is null on all members.  */

  /* Delete all duplicate fields from the fieldlist */
  for (x = fieldlist; x && TREE_CHAIN (x);)
    /* Anonymous fields aren't duplicates.  */
    if (DECL_NAME (TREE_CHAIN (x)) == 0)
      x = TREE_CHAIN (x);
    else
      {
        register tree y = fieldlist;

        while (1)
          {
            if (DECL_NAME (y) == DECL_NAME (TREE_CHAIN (x)))
              break;
            if (y == x)
              break;
            y = TREE_CHAIN (y);
          }
        if (DECL_NAME (y) == DECL_NAME (TREE_CHAIN (x)))
          {
#ifdef GPC
            error_with_decl (TREE_CHAIN (x), "duplicate field `%s'");
#else
            error_with_decl (TREE_CHAIN (x), "duplicate member `%s'");
#endif
            TREE_CHAIN (x) = TREE_CHAIN (TREE_CHAIN (x));
          }
        else x = TREE_CHAIN (x);
      }
#ifdef GPC
  /* Check for name conflicts between schema discriminants and
   * the fields of a contained record.
   */
  if (x
      && DECL_NAME (x) == schema_id
      && TREE_CODE (TREE_TYPE (x)) == RECORD_TYPE)
    {
      for (x = TYPE_FIELDS (TREE_TYPE (x)); x; x = TREE_CHAIN (x))
        {
          register tree y;
          for (y = fieldlist; y; y = TREE_CHAIN (y))
            if (DECL_NAME (x) == DECL_NAME (y) && DECL_NAME (x) != schema_id)
              {
                error_with_decl (x, "field `%s' conflicts with schema discriminant");
                /*
                 * Don't let invalid schema types get born.
                 */
                return error_mark_node;
              }
        }
    }
#endif /* GPC */

  /* Now we have the nearly final fieldlist.  Record it,
     then lay out the structure or union (including the fields).  */

  TYPE_FIELDS (t) = fieldlist;

  layout_type (t);

  /* Delete all zero-width bit-fields from the front of the fieldlist */
  while (fieldlist
         && DECL_INITIAL (fieldlist))
    fieldlist = TREE_CHAIN (fieldlist);
  /* Delete all such members from the rest of the fieldlist */
  for (x = fieldlist; x;)
    {
      if (TREE_CHAIN (x) && DECL_INITIAL (TREE_CHAIN (x)))
        TREE_CHAIN (x) = TREE_CHAIN (TREE_CHAIN (x));
      else x = TREE_CHAIN (x);
    }

  /*  Now we have the truly final field list.
      Store it in this type and in the variants.  */

  TYPE_FIELDS (t) = fieldlist;

#ifdef GPC
  /* Do not sort the fields of the record in GPC.
   * The TYPE_LANG_SPECIFIC is used to store the variant
   * record tag info and for other purposes.
   *
   * If gpc-tree.h struct lang_type() has one more tree type node added
   * I could consider this again (if the sorting does not break the `with'
   * statement implementation)
   *
   */
#else
  /* If there are lots of fields, sort so we can look through them fast.
     We arbitrarily consider 16 or more elts to be "a lot".  */
  {
    int len = 0;

    for (x = fieldlist; x; x = TREE_CHAIN (x))
      {
        if (len > 15)
          break;
        len += 1;
      }
    if (len > 15)
      {
        tree *field_array;
        char *space;

        len += list_length (x);
        /* Use the same allocation policy here that make_node uses, to
           ensure that this lives as long as the rest of the struct decl.
           All decls in an inline function need to be saved.  */
        if (allocation_temporary_p ())
          space = savealloc (sizeof (struct lang_type) + len * sizeof (tree));
        else
          space = oballoc (sizeof (struct lang_type) + len * sizeof (tree));

        TYPE_LANG_SPECIFIC (t) = (struct lang_type *) space;
        TYPE_LANG_SPECIFIC (t)->len = len;

        field_array = &TYPE_LANG_SPECIFIC (t)->elts[0];
        len = 0;
        for (x = fieldlist; x; x = TREE_CHAIN (x))
          field_array[len++] = x;

        qsort (field_array, len, sizeof (tree), field_decl_cmp);
      }
  }
#endif /* GPC */

  for (x = TYPE_MAIN_VARIANT (t); x; x = TYPE_NEXT_VARIANT (x))
    {
      TYPE_FIELDS (x) = TYPE_FIELDS (t);
      TYPE_LANG_SPECIFIC (x) = TYPE_LANG_SPECIFIC (t);
      TYPE_ALIGN (x) = TYPE_ALIGN (t);
    }

  /* If this was supposed to be a transparent union, but we can't
     make it one, warn and turn off the flag.  */
  if (TREE_CODE (t) == UNION_TYPE
      && TYPE_TRANSPARENT_UNION (t)
      && TYPE_MODE (t) != DECL_MODE (TYPE_FIELDS (t)))
    {
      TYPE_TRANSPARENT_UNION (t) = 0;
      warning ("union cannot be made transparent");
    }

  /* If this structure or union completes the type of any previous
     variable declaration, lay it out and output its rtl.  */

  if (current_binding_level->n_incomplete != 0)
    {
      tree decl;
      for (decl = current_binding_level->names; decl; decl = TREE_CHAIN (decl))
        {
          if (TREE_TYPE (decl) == t
              && TREE_CODE (decl) != TYPE_DECL)
            {
              layout_decl (decl, 0);
              /* This is a no-op in c-lang.c or something real in objc-actions.c.  */
              maybe_objc_check_decl (decl);
              rest_of_decl_compilation (decl, NULL_PTR, toplevel, 0);
              if (! toplevel)
                expand_decl (decl);
              --current_binding_level->n_incomplete;
            }
          else if (TYPE_SIZE (TREE_TYPE (decl)) == 0
                   && TREE_CODE (TREE_TYPE (decl)) == ARRAY_TYPE)
            {
              tree element = TREE_TYPE (decl);
              while (TREE_CODE (element) == ARRAY_TYPE)
                element = TREE_TYPE (element);
              if (element == t)
                layout_array_type (TREE_TYPE (decl));
            }
        }
    }

  resume_momentary (old_momentary);

  /* Finish debugging output for this type.  */
  rest_of_type_compilation (t, toplevel);

  /* The matching push is in start_struct.  */
  pop_obstacks ();

  return t;
}

#ifdef GPC
/*
 * Checks a record containing a variant part for duplicate identifiers.
 *
 * FIELDS are the non-variant fields of the record type, including the
 * optional tag field, but excluding the variant part union.
 *
 * VARIANT is a UNION_TYPE which contains the different RECORD_TYPEs
 * representing the variants.
 *
 * The type has not yet been laid out.
 */
void
check_variant_record (fields, variant)
     tree fields, variant;
{
  /* Each record in VARIANT must be checked against each other one
   * plus the containing record.  It needn't be checked against itself
   * because finish_struct() already did so.
   */
  tree rec;
  for (rec = TYPE_FIELDS (variant); rec; rec = TREE_CHAIN (rec))
    {
      tree field;
      for (field = TYPE_FIELDS (TREE_TYPE (rec)); field; field = TREE_CHAIN (field))
        if (DECL_NAME (field))
          {
            tree other_rec, other_field;
            for (other_field = fields; other_field;
                 other_field = TREE_CHAIN (other_field))
              if (DECL_NAME (other_field) == DECL_NAME (field))
                error_with_decl (field, "duplicate field `%s'");
            for (other_rec = TYPE_FIELDS (variant);
                 other_rec && other_rec != rec;
                 other_rec = TREE_CHAIN (other_rec))
              {
                tree other_field;
                for (other_field = TYPE_FIELDS (TREE_TYPE (other_rec)); other_field;
                     other_field = TREE_CHAIN (other_field))
                  if (DECL_NAME (other_field) == DECL_NAME (field))
                    error_with_decl (field, "duplicate field `%s'");
              }
          }
    }
}
#endif /* GPC */

/* Lay out the type T, and its element type, and so on.  */

static void
layout_array_type (t)
     tree t;
{
  if (TREE_CODE (TREE_TYPE (t)) == ARRAY_TYPE)
    layout_array_type (TREE_TYPE (t));
  layout_type (t);
}

/* Begin compiling the definition of an enumeration type.
   NAME is its name (or null if anonymous).
   Returns the type object, as yet incomplete.
   Also records info about it so that build_enumerator
   may be used to declare the individual values as they are read.  */

tree
start_enum (name)
     tree name;
{
  register tree enumtype = 0;

  /* If this is the real definition for a previous forward reference,
     fill in the contents in the same object that used to be the
     forward reference.  */

  if (name != 0)
    enumtype = lookup_tag (ENUMERAL_TYPE, name, current_binding_level, 1);

  /* The corresponding pop_obstacks is in finish_enum.  */
  push_obstacks_nochange ();
  /* If these symbols and types are global, make them permanent.  */
  if (current_binding_level == global_binding_level)
    end_temporary_allocation ();

  if (enumtype == 0 || TREE_CODE (enumtype) != ENUMERAL_TYPE)
    {
      enumtype = make_node (ENUMERAL_TYPE);
      pushtag (name, enumtype);
    }

  C_TYPE_BEING_DEFINED (enumtype) = 1;

  if (TYPE_VALUES (enumtype) != 0)
    {
      /* This enum is a named one that has been declared already.  */
      error ("redeclaration of `enum %s'", IDENTIFIER_POINTER (name));

      /* Completely replace its old definition.
         The old enumerators remain defined, however.  */
      TYPE_VALUES (enumtype) = 0;
    }

  enum_next_value = integer_zero_node;
  enum_overflow = 0;

  if (flag_short_enums)
    TYPE_PACKED (enumtype) = 1;

  return enumtype;
}

/* After processing and defining all the values of an enumeration type,
   install their decls in the enumeration type and finish it off.
   ENUMTYPE is the type object, VALUES a list of decl-value pairs,
   and ATTRIBUTES are the specified attributes.
   Returns ENUMTYPE.  */

tree
finish_enum (enumtype, values, attributes)
     tree enumtype;
     tree values;
     tree attributes;
{
  register tree pair, tem;
  tree minnode = 0, maxnode = 0;
  int lowprec, highprec, precision;
  int toplevel = global_binding_level == current_binding_level;

  if (in_parm_level_p ())
    warning ("enum defined inside parms");

  decl_attributes (enumtype, attributes, NULL_TREE);

  /* Calculate the maximum value of any enumerator in this type.  */

  if (values == error_mark_node)
    minnode = maxnode = integer_zero_node;
  else
    for (pair = values; pair; pair = TREE_CHAIN (pair))
      {
        tree value = TREE_VALUE (pair);
        if (pair == values)
          minnode = maxnode = TREE_VALUE (pair);
        else
          {
            if (tree_int_cst_lt (maxnode, value))
              maxnode = value;
            if (tree_int_cst_lt (value, minnode))
              minnode = value;
          }
      }

  TYPE_MIN_VALUE (enumtype) = minnode;
  TYPE_MAX_VALUE (enumtype) = maxnode;

  /* An enum can have some negative values; then it is signed.  */
  TREE_UNSIGNED (enumtype) = tree_int_cst_sgn (minnode) >= 0;

  /* Determine the precision this type needs.  */

  lowprec = min_precision (minnode, TREE_UNSIGNED (enumtype));
  highprec = min_precision (maxnode, TREE_UNSIGNED (enumtype));
  precision = MAX (lowprec, highprec);

  if (TYPE_PACKED (enumtype) || precision > TYPE_PRECISION (integer_type_node))
    {
      tree narrowest = type_for_size (precision, 1);
      if (narrowest == 0)
        {
          warning ("enumeration values exceed range of largest integer");
          narrowest = long_long_integer_type_node;
        }

      TYPE_PRECISION (enumtype) = TYPE_PRECISION (narrowest);
    }
  else
    TYPE_PRECISION (enumtype) = TYPE_PRECISION (integer_type_node);

  TYPE_SIZE (enumtype) = 0;
#ifdef GPC
#ifdef EGCS
  TYPE_SIZE_UNIT (enumtype) = 0;
#endif /* GPC */
#endif /* EGCS */
  layout_type (enumtype);

  if (values != error_mark_node)
    {
      /* Change the type of the enumerators to be the enum type.
         Formerly this was done only for enums that fit in an int,
         but the comment said it was done only for enums wider than int.
         It seems necessary to do this for wide enums,
         and best not to change what's done for ordinary narrower ones.  */
      for (pair = values; pair; pair = TREE_CHAIN (pair))
        {
          TREE_TYPE (TREE_PURPOSE (pair)) = enumtype;
          DECL_SIZE (TREE_PURPOSE (pair)) = TYPE_SIZE (enumtype);
          if (TREE_CODE (TREE_PURPOSE (pair)) != FUNCTION_DECL)
            DECL_ALIGN (TREE_PURPOSE (pair)) = TYPE_ALIGN (enumtype);
        }

      /* Replace the decl nodes in VALUES with their names.  */
      for (pair = values; pair; pair = TREE_CHAIN (pair))
        TREE_PURPOSE (pair) = DECL_NAME (TREE_PURPOSE (pair));

      TYPE_VALUES (enumtype) = values;
    }

  /* Fix up all variant types of this enum type.  */
  for (tem = TYPE_MAIN_VARIANT (enumtype); tem; tem = TYPE_NEXT_VARIANT (tem))
    {
      TYPE_VALUES (tem) = TYPE_VALUES (enumtype);
      TYPE_MIN_VALUE (tem) = TYPE_MIN_VALUE (enumtype);
      TYPE_MAX_VALUE (tem) = TYPE_MAX_VALUE (enumtype);
      TYPE_SIZE (tem) = TYPE_SIZE (enumtype);
#ifdef GPC
#ifdef EGCS
      TYPE_SIZE_UNIT (tem) = TYPE_SIZE_UNIT (enumtype);
#endif /* EGCS */
#endif /* GPC */
      TYPE_MODE (tem) = TYPE_MODE (enumtype);
      TYPE_PRECISION (tem) = TYPE_PRECISION (enumtype);
      TYPE_ALIGN (tem) = TYPE_ALIGN (enumtype);
      TREE_UNSIGNED (tem) = TREE_UNSIGNED (enumtype);
    }

  /* Finish debugging output for this type.  */
  rest_of_type_compilation (enumtype, toplevel);

  /* This matches a push in start_enum.  */
  pop_obstacks ();

  return enumtype;
}

/* Build and install a CONST_DECL for one value of the
   current enumeration type (one that was begun with start_enum).
   Return a tree-list containing the CONST_DECL and its value.
   Assignment of sequential values by default is handled here.  */

tree
build_enumerator (name, value)
     tree name, value;
{
  register tree decl, type;

  /* Validate and default VALUE.  */

  /* Remove no-op casts from the value.  */
  if (value)
    STRIP_TYPE_NOPS (value);

  if (value != 0)
    {
      if (TREE_CODE (value) == INTEGER_CST)
        {
          value = default_conversion (value);
          constant_expression_warning (value);
        }
      else
        {
          error ("enumerator value for `%s' not integer constant",
                 IDENTIFIER_POINTER (name));
          value = 0;
        }
    }

  /* Default based on previous value.  */
  /* It should no longer be possible to have NON_LVALUE_EXPR
     in the default.  */
  if (value == 0)
    {
      value = enum_next_value;
      if (enum_overflow)
        error ("overflow in enumeration values");
    }

  if (pedantic && ! int_fits_type_p (value, integer_type_node))
    {
      pedwarn ("ANSI C restricts enumerator values to range of `int'");
      value = integer_zero_node;
    }

  /* Set basis for default for next value.  */
  enum_next_value = build_binary_op (PLUS_EXPR, value, integer_one_node, 0);
  enum_overflow = tree_int_cst_lt (enum_next_value, value);

  /* Now create a declaration for the enum value name.  */

  type = TREE_TYPE (value);
  type = type_for_size (MAX (TYPE_PRECISION (type),
                             TYPE_PRECISION (integer_type_node)),
                        ((flag_traditional
                          || TYPE_PRECISION (type) >= TYPE_PRECISION (integer_type_node))
                         && TREE_UNSIGNED (type)));

  decl = build_decl (CONST_DECL, name, type);
#ifdef GPC
  /* @@@? DO NOT change the type of value */
  DECL_INITIAL (decl) = copy_node (value);
  TREE_TYPE (DECL_INITIAL (decl)) = current_enum_type;
#else
  DECL_INITIAL (decl) = value;
  TREE_TYPE (value) = type;
#endif /* GPC */
  pushdecl (decl);

  return saveable_tree_cons (decl, value, NULL_TREE);
}
#ifdef GPC
/*
 * Pascal directives.
 * The required predefined word `forward' is a directive that
 * defines a function or procedure so it can be called before
 * the routine implementation is seen. The funny part is that
 * if routines are defined with directives, the parameter lists
 * and function return values are allowed to appear only in the
 * definition with the directive, not where the routine is implemented.
 *
 *  Example:
 *              function foobar(var zap: fuzz): fuzz; forward;
 *   and then later
 *
 *              function foobar;
 *              begin
 *                foobar = zap;
 *              end;
 *
 * This is also the reason why I don't like the idea of implementing
 * `inline' as a directive (as it perhaps should be). The solution may be that
 * I allow the param lists and return types to appear when the routine
 * implementation is parsed and only check that they match with the
 * definition with the directive.
 *
 * @@@@ The behaviour of the Borland compiler is the following:
 * It allows the param list and return type to appear again, checks
 * if it matches *exactly* (including formal parameter names) and
 * complains if a forward routine is not resolved within the same module.
 * The same holds for interface declarations of routines and method
 * declarations of objects.
 *
 * @@@@ This is not quite correct, since now all directives are
 * handled equally, which means that you may give forward and not
 * define the routine in this program, or extern and define it anyway.
 * (Well, extern *is* implementation dependent, but maybe this is
 * misleading)
 *
 * I store the value of the directive to GPC specific part
 * in the TREE node (see lang.h).
 */
int
is_known_directive (directive)
     tree directive;
{
  int result = 1;
  tree dir;
  for (dir = directive; dir; dir = TREE_CHAIN (dir))
    {
      if (TREE_VALUE (dir) != d_forward
          && TREE_VALUE (dir) != d_asmname
          && TREE_VALUE (dir) != d_attribute
          && TREE_VALUE (dir) != d_external
          && TREE_VALUE (dir) != d_extern
          && TREE_VALUE (dir) != d_c_lang1
          && TREE_VALUE (dir) != d_c_lang
          && TREE_VALUE (dir) != d_static)
        {
          error ("unknown directive");
          result = 0;
        }
    }

  return result;
}

tree
de_capitalize (name)
     tree name;
{
  char *id = IDENTIFIER_POINTER (name);

  if (*id >= 'A' && *id <= 'Z')
    {
      char *copy = oballoc (IDENTIFIER_LENGTH (name));

      strcpy (copy, id);
      *copy = tolower (*copy);
      /* "toupper" was wrong -- I meant "tolower" :-) -- PG */
      name = get_identifier (copy);
    }

  return name;
}

tree
grok_directive (type, heading, directive, nested)
     tree type, heading, directive;
     int nested;
{
    tree d, dir, name_dir = NULL_TREE;
    tree attributes = NULL_TREE;
    int is_external = this_is_an_interface_module;

    for (dir = directive; dir; dir = TREE_CHAIN (dir))
      {
        if (TREE_VALUE (dir) == d_external
            || TREE_VALUE (dir) == d_asmname
            || TREE_VALUE (dir) == d_c_lang)
          is_external = 1;
        else if (TREE_VALUE (dir) == d_extern)
          {
            TREE_VALUE (dir) = d_external;
            is_external = 1;
          }
        else if (TREE_VALUE (dir) == d_c_lang1)
          {
            TREE_VALUE (dir) = d_c_lang;
            is_external = 1;
          }
        else if (TREE_VALUE (dir) == d_forward)
          {
            if (nested)
              /* Use `auto' function with nested forwards.
               */
              type = tree_cons (NULL_TREE, auto_id, type);
            else if (! name_exported_p (TREE_OPERAND (heading, 0)))
              type = tree_cons (NULL_TREE, static_id, type);
            else
              {
                type = tree_cons (NULL_TREE, extern_id, type);
                is_external = 1;
              }
          }
        else if (TREE_VALUE (dir) == d_static)
          type = tree_cons (NULL_TREE, static_id, type);
        if (TREE_VALUE (dir) == d_attribute)
          attributes = TREE_PURPOSE (dir);
        if (TREE_VALUE (dir) == d_c_lang || TREE_VALUE (dir) == d_asmname)
          name_dir = dir;
      }

    d = start_decl (heading, type, 0, attributes, NULL_TREE);

    /* With the C directive, the assembler name is derived from the
     * Pascal name by setting everything lowercase.
     */
    if (name_dir && TREE_VALUE (name_dir) == d_c_lang)
      DECL_ASSEMBLER_NAME (d) = de_capitalize (TREE_OPERAND (heading, 0));

    /* AsmName directive: direct specification of assembler name.
     * Casing is preserved.
     */
    if (name_dir && TREE_VALUE (name_dir) == d_asmname)
      {
        char *p;
        switch (TREE_CODE (TREE_PURPOSE (name_dir)))
          {
            case IDENTIFIER_NODE:
              DECL_ASSEMBLER_NAME (d) = TREE_PURPOSE (name_dir);
              break;
            case STRING_CST:
              DECL_ASSEMBLER_NAME (d)
                  = get_identifier (TREE_STRING_POINTER (TREE_PURPOSE (name_dir)));
              break;
            default:
              abort ();
          }
        p = IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (d));
        while (*p && *p != ' ' && *p != '\t')
          p++;
        if (*p)
          error ("assembler name for `%s' contains whitespace",
                 IDENTIFIER_POINTER (DECL_NAME (d)));
      }

    finish_decl (d, NULL_TREE, NULL_TREE);

    /* Avoid the optimizer to remove this if it is external (in C sense).
     */
    if (is_external)
      mark_addressable (d);

    /* Save the arguments that were placed in the
     * function_maybepermanent_obstack.
     */
    (void) preserve_data ();

    if (d && TREE_CODE (d) == FUNCTION_DECL)
      {
        struct lang_decl *temp;
        if (DECL_LANG_SPECIFIC (d))
          abort ();
        temp = allocate_decl_lang_specific ();
        temp->directive = directive;

        /* @@ should also store last_function_parm_tags ?!
         */
        temp->parms = last_function_parms;
        DECL_LANG_SPECIFIC (d) = temp;
      }

  return d;
}

/* Check whether the tree list DIR_LIST of directives contains THIS_DIR
 * (subroutine of start_function () and start_pascal_function ()).
 */
static int
contains_directive (dir_list, this_dir)
     tree dir_list, this_dir;
{
  tree d;
  for (d = dir_list; d; d = TREE_CHAIN (d))
    if (TREE_VALUE (d) == this_dir)
      return 1;
  return 0;
}

/*
 * Routine checks if there has been a definition with a directive
 * for the routine we start declaring now.  If so, check the
 * parameter lists and return type to match those of the
 * earlier definition.
 *
 * After the (possible) modification, give the stuff to the
 * start_function below.
 */
tree
start_pascal_function (type, heading, nested)
     tree type;
     tree heading;
     int nested;
{
  tree old = lookup_name (TREE_OPERAND (heading, 0));
  tree declarator = type;

  if (defining_methods == 1 && ! old)
    error ("no such method in object");

  if (old && TREE_CODE (old) == FUNCTION_DECL)
    {
      struct lang_decl *temp = DECL_LANG_SPECIFIC (old);
      if (temp && temp->directive)
        {
          tree args = TREE_OPERAND (heading, 1);
#if 0
          /* This must not warn about declarations in the
           * inner level -> disabled.  (@@@ Should check if
           * the name is found at the same binding level
           * as this definition is going to.)
           */
          if (! contains_directive (temp->directive, d_forward))
            warning ("wrong directive specified - `forward' assumed");
#endif
          if (name_exported_p (TREE_OPERAND (heading, 0))
              || contains_directive (temp->directive, d_external)
              || contains_directive (temp->directive, d_asmname))
            {
              /* Replace "static" by "external", or add "external".
               */
              tree d;
              int was_static = 0;
              for (d = declarator; d; d = TREE_CHAIN (d))
                {
                  if (TREE_VALUE (d) == static_id)
                    {
                      was_static = 1;
                      TREE_VALUE (d) = extern_id;
                    }
                }
              if (! was_static)
                declarator = chainon (declarator,
                                      build_tree_list (NULL_TREE, extern_id));
            }

          if (PEDANTIC (U_B_D_PASCAL))
            {
              if (TREE_PURPOSE (args))
                pedwarn ("parameters in forward declared routines are a Borland Pascal extension");

              if (TREE_VALUE (type) != void_type_node)
                pedwarn ("function types in forward declared routines are a Borland Pascal extension");
             }

          /* If parameters have been specified in second definition of
           * function, check them.
           *
           * Methods *only* having the implicit self parameter
           * count as parameterless here.
           */
          if (TREE_CHAIN (args)
              && TREE_VALUE (TREE_CHAIN (args)) != void_type_node
              && (! defining_methods
                  || (TREE_CHAIN (TREE_CHAIN (args))
                      && TREE_VALUE (TREE_CHAIN (TREE_CHAIN (args)))
                         != void_type_node)))
            {
              tree arg, old_arg;
              arg = TREE_PURPOSE (args);
              old_arg = temp->parms;
              while (arg && old_arg)
                {
                  if (TREE_CODE (arg) != TREE_CODE (old_arg)
                      || (TREE_CODE (arg) == PARM_DECL
                          && comptypes (TREE_TYPE (arg), TREE_TYPE (old_arg)) != 1))
                    break;
                  do
                    arg = TREE_CHAIN (arg);
                  while (arg && TREE_CODE (arg) != PARM_DECL);
                  do
                    old_arg = TREE_CHAIN (old_arg);
                  while (old_arg && TREE_CODE (old_arg) != PARM_DECL);
                }
              if (arg
                  || old_arg
                  || (TREE_VALUE (type)
                      && comptypes (TREE_VALUE (type), TREE_TYPE (TREE_TYPE (old))) != 1))
                {
                  if (TREE_VALUE (type) == void_type_node)
                    error ("procedure definition does not match previous declaration");
                  else
                    error ("function definition does not match previous declaration");
                }
            }

          /* Change the parameters of the function to look like
           * the parameters of the forward definition.
           */
          TREE_PURPOSE (args) = temp->parms;

          /* Don't forget the list of argument types */
          TREE_CHAIN (args) = TYPE_ARG_TYPES (TREE_TYPE (old));

          /* And finally the return value type */
          TREE_VALUE (type) = TREE_TYPE (TREE_TYPE (old));
        }
    }

  if (! start_function (declarator, heading, NULL_TREE, NULL_TREE, nested))
    return NULL_TREE;

  /* This is the actual type of the new routine.
   */
  return TREE_VALUE (type);
}

tree
declare_main_program_context (declspecs, declarator)
     tree declspecs, declarator;
{
  return grokdeclarator (declarator, declspecs, FUNCDEF, 1);
}

#endif /* GPC */

/* Create the FUNCTION_DECL for a function definition.
   DECLSPECS, DECLARATOR, PREFIX_ATTRIBUTES and ATTRIBUTES are the parts of
   the declaration; they describe the function's name and the type it returns,
   but twisted together in a fashion that parallels the syntax of C.

   This function creates a binding context for the function body
   as well as setting up the FUNCTION_DECL in current_function_decl.

   Returns 1 on success.  If the DECLARATOR is not suitable for a function
   (it defines a datum instead), we return 0, which tells
   yyparse to report a parse error.

   NESTED is nonzero for a function nested within another function.  */

int
start_function (declspecs, declarator, prefix_attributes, attributes, nested)
     tree declarator, declspecs, prefix_attributes, attributes;
     int nested;
{
  tree decl1, old_decl;
  tree restype;
  int old_immediate_size_expand = immediate_size_expand;

  current_function_returns_value = 0;  /* Assume, until we see it does.  */
  current_function_returns_null = 0;
  warn_about_return_type = 0;
  current_extern_inline = 0;
  c_function_varargs = 0;
  named_labels = 0;
  shadowed_labels = 0;

  /* Don't expand any sizes in the return type of the function.  */
  immediate_size_expand = 0;

#ifdef GPC
  if (declspecs)
    decl1 = grokdeclarator (declarator, declspecs, FUNCDEF, 1);
  else
    {
      last_function_parms     = NULL_TREE; /* No parameters to main program */
      last_function_parm_tags = NULL_TREE;
      decl1 = declarator;
    }
#else
  decl1 = grokdeclarator (declarator, declspecs, FUNCDEF, 1);
#endif /* GPC */

  /* If the declarator is not suitable for a function definition,
     cause a syntax error.  */
  if (decl1 == 0)
    {
      immediate_size_expand = old_immediate_size_expand;
      return 0;
    }

  decl_attributes (decl1, prefix_attributes, attributes);

  announce_function (decl1);

  if (TYPE_SIZE (TREE_TYPE (TREE_TYPE (decl1))) == 0)
    {
      error ("return-type is an incomplete type");
      /* Make it return void instead.  */
      TREE_TYPE (decl1)
        = build_function_type (void_type_node,
                               TYPE_ARG_TYPES (TREE_TYPE (decl1)));
    }

  if (warn_about_return_type)
    warning ("return-type defaults to `int'");

  /* Save the parm names or decls from this function's declarator
     where store_parm_decls will find them.  */
  current_function_parms = last_function_parms;
  current_function_parm_tags = last_function_parm_tags;

  /* Make the init_value nonzero so pushdecl knows this is not tentative.
     error_mark_node is replaced below (in poplevel) with the BLOCK.  */
  DECL_INITIAL (decl1) = error_mark_node;

  /* If this definition isn't a prototype and we had a prototype declaration
     before, copy the arg type info from that prototype.
     But not if what we had before was a builtin function.  */
  old_decl = lookup_name_current_level (DECL_NAME (decl1));
#ifdef GPC
  /* If we have a function declaration which is forward declared,
     and this routine is a void_type function and it has no parameters
     then alter the new declaration to look like the old forward declared model */
  if (old_decl != 0)
    { struct lang_decl *spec = DECL_LANG_SPECIFIC (old_decl);
        if (spec
            && contains_directive (spec->directive, d_forward)
            && TREE_TYPE (TREE_TYPE (decl1)) == void_type_node
            && (TYPE_ARG_TYPES (TREE_TYPE (decl1)) == 0 ||
                TREE_VALUE (TYPE_ARG_TYPES (TREE_TYPE (decl1))) ==
                void_type_node))
          {
            TREE_TYPE (decl1) = TREE_TYPE (old_decl);
            current_function_prototype_file = DECL_SOURCE_FILE (old_decl);
            current_function_prototype_line = DECL_SOURCE_LINE (old_decl);
          }
    }
#else /* not GPC */
  if (old_decl != 0 && TREE_CODE (TREE_TYPE (old_decl)) == FUNCTION_TYPE
      && !DECL_BUILT_IN (old_decl)
      && (TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (decl1)))
          == TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (old_decl))))
      && TYPE_ARG_TYPES (TREE_TYPE (decl1)) == 0)
    {
      TREE_TYPE (decl1) = TREE_TYPE (old_decl);
      current_function_prototype_file = DECL_SOURCE_FILE (old_decl);
      current_function_prototype_line = DECL_SOURCE_LINE (old_decl);
    }

  /* If there is no explicit declaration, look for any out-of-scope implicit
     declarations.  */
  if (old_decl == 0)
    old_decl = IDENTIFIER_IMPLICIT_DECL (DECL_NAME (decl1));

  /* Optionally warn of old-fashioned def with no previous prototype.  */
  if (warn_strict_prototypes
      && TYPE_ARG_TYPES (TREE_TYPE (decl1)) == 0
      && !(old_decl != 0 && TYPE_ARG_TYPES (TREE_TYPE (old_decl)) != 0))
    warning ("function declaration isn't a prototype");
  /* Optionally warn of any global def with no previous prototype.  */
  else if (warn_missing_prototypes
           && TREE_PUBLIC (decl1)
           && !(old_decl != 0 && TYPE_ARG_TYPES (TREE_TYPE (old_decl)) != 0)
           && strcmp ("main", IDENTIFIER_POINTER (DECL_NAME (decl1))))
    warning_with_decl (decl1, "no previous prototype for `%s'");
  /* Optionally warn of any def with no previous prototype
     if the function has already been used.  */
  else if (warn_missing_prototypes
           && old_decl != 0 && TREE_USED (old_decl)
           && TYPE_ARG_TYPES (TREE_TYPE (old_decl)) == 0)
    warning_with_decl (decl1,
                      "`%s' was used with no prototype before its definition");
  /* Optionally warn of any global def with no previous declaration.  */
  else if (warn_missing_declarations
           && TREE_PUBLIC (decl1)
           && old_decl == 0
           && strcmp ("main", IDENTIFIER_POINTER (DECL_NAME (decl1))))
    warning_with_decl (decl1, "no previous declaration for `%s'");
  /* Optionally warn of any def with no previous declaration
     if the function has already been used.  */
  else if (warn_missing_declarations
           && old_decl != 0 && TREE_USED (old_decl)
           && old_decl == IDENTIFIER_IMPLICIT_DECL (DECL_NAME (decl1)))
    warning_with_decl (decl1,
                    "`%s' was used with no declaration before its definition");
#endif /* not GPC */

  /* This is a definition, not a reference.
     So normally clear DECL_EXTERNAL.
     However, `extern inline' acts like a declaration
     except for defining how to inline.  So set DECL_EXTERNAL in that case.  */
  DECL_EXTERNAL (decl1) = current_extern_inline;

  /* This function exists in static storage.
     (This does not mean `static' in the C sense!)  */
  TREE_STATIC (decl1) = 1;

  /* A nested function is not global.  */
  if (current_function_decl != 0)
    TREE_PUBLIC (decl1) = 0;

  /* Warn for unlikely, improbable, or stupid declarations of `main'. */
  if (warn_main
      && strcmp ("main", IDENTIFIER_POINTER (DECL_NAME (decl1))) == 0)
    {
      tree args;
      int argct = 0;

      if (TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (decl1)))
           != integer_type_node)
        pedwarn_with_decl (decl1, "return type of `%s' is not `int'");

      for (args = TYPE_ARG_TYPES (TREE_TYPE (decl1)); args;
           args = TREE_CHAIN (args))
        {
          tree type = args ? TREE_VALUE (args) : 0;

          if (type == void_type_node)
            break;

          ++argct;
          switch (argct)
            {
            case 1:
              if (TYPE_MAIN_VARIANT (type) != integer_type_node)
                pedwarn_with_decl (decl1,
                                   "first argument of `%s' should be `int'");
              break;

            case 2:
              if (TREE_CODE (type) != POINTER_TYPE
                  || TREE_CODE (TREE_TYPE (type)) != POINTER_TYPE
                  || (TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (type)))
                      != char_type_node))
                pedwarn_with_decl (decl1,
                               "second argument of `%s' should be `char **'");
              break;

            case 3:
              if (TREE_CODE (type) != POINTER_TYPE
                  || TREE_CODE (TREE_TYPE (type)) != POINTER_TYPE
                  || (TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (type)))
                      != char_type_node))
                pedwarn_with_decl (decl1,
                   "third argument of `%s' should probably be `char **'");
              break;
            }
        }

      /* It is intentional that this message does not mention the third
         argument, which is warned for only pedantically, because it's
         blessed by mention in an appendix of the standard. */
      if (argct > 0 && (argct < 2 || argct > 3))
        pedwarn_with_decl (decl1, "`%s' takes only zero or two arguments");

      /*
         GPC uses the third argument, but only when necessary, so don't
         warn the user who's not responsible for it.
      if (argct == 3 && pedantic)
        pedwarn_with_decl (decl1, "third argument of `%s' is deprecated");
      */

      if (! TREE_PUBLIC (decl1))
        pedwarn_with_decl (decl1, "`%s' is normally a non-static function");
    }

  /* Record the decl so that the function name is defined.
     If we already have a decl for this name, and it is a FUNCTION_DECL,
     use the old decl.  */

  current_function_decl = pushdecl (decl1);

  pushlevel (0);
  declare_parm_level (1);
  current_binding_level->subblocks_tag_transparent = 1;

  make_function_rtl (current_function_decl);

  restype = TREE_TYPE (TREE_TYPE (current_function_decl));
  /* Promote the value to int before returning it.  */
  if (C_PROMOTING_INTEGER_TYPE_P (restype))
    {
      /* It retains unsignedness if traditional
         or if not really getting wider.  */
      if (TREE_UNSIGNED (restype)
          && (flag_traditional
              || (TYPE_PRECISION (restype)
                  == TYPE_PRECISION (integer_type_node))))
        restype = unsigned_type_node;
      else
        restype = integer_type_node;
    }
  DECL_RESULT (current_function_decl)
    = build_decl (RESULT_DECL, NULL_TREE, restype);

  if (!nested)
    /* Allocate further tree nodes temporarily during compilation
       of this function only.  */
    temporary_allocation ();

  /* If this fcn was already referenced via a block-scope `extern' decl
     (or an implicit decl), propagate certain information about the usage.  */
  if (TREE_ADDRESSABLE (DECL_ASSEMBLER_NAME (current_function_decl)))
    TREE_ADDRESSABLE (current_function_decl) = 1;

  immediate_size_expand = old_immediate_size_expand;

  return 1;
}

/* Record that this function is going to be a varargs function.
   This is called before store_parm_decls, which is too early
   to call mark_varargs directly.  */

void
c_mark_varargs ()
{
  c_function_varargs = 1;
}

/* Store the parameter declarations into the current function declaration.
   This is called after parsing the parameter declarations, before
   digesting the body of the function.

   For an old-style definition, modify the function's type
   to specify at least the number of arguments.  */

void
store_parm_decls ()
{
  register tree fndecl = current_function_decl;
  register tree parm;

  /* This is either a chain of PARM_DECLs (if a prototype was used)
     or a list of IDENTIFIER_NODEs (for an old-fashioned C definition).  */
  tree specparms = current_function_parms;

  /* This is a list of types declared among parms in a prototype.  */
  tree parmtags = current_function_parm_tags;

  /* This is a chain of PARM_DECLs from old-style parm declarations.  */
  register tree parmdecls = getdecls ();

  /* This is a chain of any other decls that came in among the parm
     declarations.  If a parm is declared with  enum {foo, bar} x;
     then CONST_DECLs for foo and bar are put here.  */
  tree nonparms = 0;

  /* Nonzero if this definition is written with a prototype.  */
  int prototype = 0;

  if (specparms != 0 && TREE_CODE (specparms) != TREE_LIST)
    {
      /* This case is when the function was defined with an ANSI prototype.
         The parms already have decls, so we need not do anything here
         except record them as in effect
         and complain if any redundant old-style parm decls were written.  */

      register tree next;
      tree others = 0;

      prototype = 1;

      if (parmdecls != 0)
        {
          tree decl, link;

          error_with_decl (fndecl,
                           "parm types given both in parmlist and separately");
          /* Get rid of the erroneous decls; don't keep them on
             the list of parms, since they might not be PARM_DECLs.  */
          for (decl = current_binding_level->names;
               decl; decl = TREE_CHAIN (decl))
            if (DECL_NAME (decl))
              IDENTIFIER_LOCAL_VALUE (DECL_NAME (decl)) = 0;
          for (link = current_binding_level->shadowed;
               link; link = TREE_CHAIN (link))
            IDENTIFIER_LOCAL_VALUE (TREE_PURPOSE (link)) = TREE_VALUE (link);
          current_binding_level->names = 0;
          current_binding_level->shadowed = 0;
        }

      specparms = nreverse (specparms);
      for (parm = specparms; parm; parm = next)
        {
          next = TREE_CHAIN (parm);
          if (TREE_CODE (parm) == PARM_DECL)
            {
              if (DECL_NAME (parm) == 0)
                error_with_decl (parm, "parameter name omitted");
              else if (TYPE_MAIN_VARIANT (TREE_TYPE (parm)) == void_type_node)
                {
                  error_with_decl (parm, "parameter `%s' declared void");
                  /* Change the type to error_mark_node so this parameter
                     will be ignored by assign_parms.  */
                  TREE_TYPE (parm) = error_mark_node;
                }
              pushdecl (parm);
            }
          else
            {
              /* If we find an enum constant or a type tag,
                 put it aside for the moment.  */
              TREE_CHAIN (parm) = 0;
              others = chainon (others, parm);
            }
        }

      /* Get the decls in their original chain order
         and record in the function.  */
      DECL_ARGUMENTS (fndecl) = getdecls ();

#if 0
      /* If this function takes a variable number of arguments,
         add a phony parameter to the end of the parm list,
         to represent the position of the first unnamed argument.  */
      if (TREE_VALUE (tree_last (TYPE_ARG_TYPES (TREE_TYPE (fndecl))))
          != void_type_node)
        {
          tree dummy = build_decl (PARM_DECL, NULL_TREE, void_type_node);
          /* Let's hope the address of the unnamed parm
             won't depend on its type.  */
          TREE_TYPE (dummy) = integer_type_node;
          DECL_ARG_TYPE (dummy) = integer_type_node;
          DECL_ARGUMENTS (fndecl)
            = chainon (DECL_ARGUMENTS (fndecl), dummy);
        }
#endif

      /* Now pushdecl the enum constants.  */
      for (parm = others; parm; parm = next)
        {
          next = TREE_CHAIN (parm);
          if (DECL_NAME (parm) == 0)
            ;
          else if (TYPE_MAIN_VARIANT (TREE_TYPE (parm)) == void_type_node)
            ;
          else if (TREE_CODE (parm) != PARM_DECL)
            pushdecl (parm);
        }

      storetags (chainon (parmtags, gettags ()));
    }
  else
    {
      /* SPECPARMS is an identifier list--a chain of TREE_LIST nodes
         each with a parm name as the TREE_VALUE.

         PARMDECLS is a chain of declarations for parameters.
         Warning! It can also contain CONST_DECLs which are not parameters
         but are names of enumerators of any enum types
         declared among the parameters.

         First match each formal parameter name with its declaration.
         Associate decls with the names and store the decls
         into the TREE_PURPOSE slots.  */

      for (parm = parmdecls; parm; parm = TREE_CHAIN (parm))
        DECL_RESULT (parm) = 0;

      for (parm = specparms; parm; parm = TREE_CHAIN (parm))
        {
          register tree tail, found = NULL;

          if (TREE_VALUE (parm) == 0)
            {
              error_with_decl (fndecl, "parameter name missing from parameter list");
              TREE_PURPOSE (parm) = 0;
              continue;
            }

          /* See if any of the parmdecls specifies this parm by name.
             Ignore any enumerator decls.  */
          for (tail = parmdecls; tail; tail = TREE_CHAIN (tail))
            if (DECL_NAME (tail) == TREE_VALUE (parm)
                && TREE_CODE (tail) == PARM_DECL)
              {
                found = tail;
                break;
              }

          /* If declaration already marked, we have a duplicate name.
             Complain, and don't use this decl twice.   */
          if (found && DECL_RESULT (found) != 0)
            {
              error_with_decl (found, "multiple parameters named `%s'");
              found = 0;
            }

          /* If the declaration says "void", complain and ignore it.  */
          if (found && TYPE_MAIN_VARIANT (TREE_TYPE (found)) == void_type_node)
            {
              error_with_decl (found, "parameter `%s' declared void");
              TREE_TYPE (found) = integer_type_node;
              DECL_ARG_TYPE (found) = integer_type_node;
              layout_decl (found, 0);
            }

          /* Traditionally, a parm declared float is actually a double.  */
          if (found && flag_traditional
              && TYPE_MAIN_VARIANT (TREE_TYPE (found)) == float_type_node)
            {
              TREE_TYPE (found) = double_type_node;
              DECL_ARG_TYPE (found) = double_type_node;
              layout_decl (found, 0);
            }

          /* If no declaration found, default to int.  */
          if (!found)
            {
              found = build_decl (PARM_DECL, TREE_VALUE (parm),
                                  integer_type_node);
              DECL_ARG_TYPE (found) = TREE_TYPE (found);
              DECL_SOURCE_LINE (found) = DECL_SOURCE_LINE (fndecl);
              DECL_SOURCE_FILE (found) = DECL_SOURCE_FILE (fndecl);
              if (extra_warnings)
                warning_with_decl (found, "type of `%s' defaults to `int'");
              pushdecl (found);
            }

          TREE_PURPOSE (parm) = found;

          /* Mark this decl as "already found" -- see test, above.
             It is safe to use DECL_RESULT for this
             since it is not used in PARM_DECLs or CONST_DECLs.  */
          DECL_RESULT (found) = error_mark_node;
        }

      /* Put anything which is on the parmdecls chain and which is
         not a PARM_DECL onto the list NONPARMS.  (The types of
         non-parm things which might appear on the list include
         enumerators and NULL-named TYPE_DECL nodes.) Complain about
         any actual PARM_DECLs not matched with any names.  */

      nonparms = 0;
      for (parm = parmdecls; parm; )
        {
          tree next = TREE_CHAIN (parm);
          TREE_CHAIN (parm) = 0;

          if (TREE_CODE (parm) != PARM_DECL)
            nonparms = chainon (nonparms, parm);
          else
            {
              /* Complain about args with incomplete types.  */
              if (TYPE_SIZE (TREE_TYPE (parm)) == 0)
                {
                  error_with_decl (parm, "parameter `%s' has incomplete type");
                  TREE_TYPE (parm) = error_mark_node;
                }

              if (DECL_RESULT (parm) == 0)
                {
                  error_with_decl (parm,
                                   "declaration for parameter `%s' but no such parameter");
                  /* Pretend the parameter was not missing.
                     This gets us to a standard state and minimizes
                     further error messages.  */
                  specparms
                    = chainon (specparms,
                               tree_cons (parm, NULL_TREE, NULL_TREE));
                }
            }

          parm = next;
        }

      /* Chain the declarations together in the order of the list of names.  */
      /* Store that chain in the function decl, replacing the list of names.  */
      parm = specparms;
      DECL_ARGUMENTS (fndecl) = 0;
      {
        register tree last;
        for (last = 0; parm; parm = TREE_CHAIN (parm))
          if (TREE_PURPOSE (parm))
            {
              if (last == 0)
                DECL_ARGUMENTS (fndecl) = TREE_PURPOSE (parm);
              else
                TREE_CHAIN (last) = TREE_PURPOSE (parm);
              last = TREE_PURPOSE (parm);
              TREE_CHAIN (last) = 0;
            }
      }

      /* If there was a previous prototype,
         set the DECL_ARG_TYPE of each argument according to
         the type previously specified, and report any mismatches.  */

      if (TYPE_ARG_TYPES (TREE_TYPE (fndecl)))
        {
          register tree type;
          for (parm = DECL_ARGUMENTS (fndecl),
               type = TYPE_ARG_TYPES (TREE_TYPE (fndecl));
               parm || (type && (TYPE_MAIN_VARIANT (TREE_VALUE (type))
                                 != void_type_node));
               parm = TREE_CHAIN (parm), type = TREE_CHAIN (type))
            {
              if (parm == 0 || type == 0
                  || TYPE_MAIN_VARIANT (TREE_VALUE (type)) == void_type_node)
                {
                  error ("number of arguments doesn't match prototype");
                  error_with_file_and_line (current_function_prototype_file,
                                            current_function_prototype_line,
                                            "prototype declaration");
                  break;
                }
              /* Type for passing arg must be consistent
                 with that declared for the arg.  */
              if (! comptypes (DECL_ARG_TYPE (parm), TREE_VALUE (type)))
                {
                  if (TYPE_MAIN_VARIANT (TREE_TYPE (parm))
                      == TYPE_MAIN_VARIANT (TREE_VALUE (type)))
                    {
                      /* Adjust argument to match prototype.  E.g. a previous
                         `int foo(float);' prototype causes
                         `int foo(x) float x; {...}' to be treated like
                         `int foo(float x) {...}'.  This is particularly
                         useful for argument types like uid_t.  */
                      DECL_ARG_TYPE (parm) = TREE_TYPE (parm);
#ifdef PROMOTE_PROTOTYPES
#ifdef GPC
                      if (ORDINAL_TYPE (TREE_CODE (TREE_TYPE (parm)))
                          && TYPE_PRECISION (TREE_TYPE (parm))
                          < TYPE_PRECISION (integer_type_node))
                        DECL_ARG_TYPE (parm) = integer_type_node;
#else /* not GPC */
                      if ((TREE_CODE (TREE_TYPE (parm)) == INTEGER_TYPE
                           || TREE_CODE (TREE_TYPE (parm)) == ENUMERAL_TYPE)
                          && TYPE_PRECISION (TREE_TYPE (parm))
                          < TYPE_PRECISION (integer_type_node))
                        DECL_ARG_TYPE (parm) = integer_type_node;
#endif /* not GPC */
#endif
                      if (pedantic)
                        {
                          pedwarn ("promoted argument `%s' doesn't match prototype",
                                   IDENTIFIER_POINTER (DECL_NAME (parm)));
                          warning_with_file_and_line
                            (current_function_prototype_file,
                             current_function_prototype_line,
                             "prototype declaration");
                        }
                    }
                  /* If -traditional, allow `int' argument to match
                     `unsigned' prototype.  */
                  else if (! (flag_traditional
                              && TYPE_MAIN_VARIANT (TREE_TYPE (parm)) == integer_type_node
                              && TYPE_MAIN_VARIANT (TREE_VALUE (type)) == unsigned_type_node))
                    {
                      error ("argument `%s' doesn't match prototype",
                             IDENTIFIER_POINTER (DECL_NAME (parm)));
                      error_with_file_and_line (current_function_prototype_file,
                                                current_function_prototype_line,
                                                "prototype declaration");
                    }
                }
            }
          TYPE_ACTUAL_ARG_TYPES (TREE_TYPE (fndecl)) = 0;
        }

      /* Otherwise, create a prototype that would match.  */

      else
        {
          tree actual = 0, last = 0, type;

          for (parm = DECL_ARGUMENTS (fndecl); parm; parm = TREE_CHAIN (parm))
            {
              type = perm_tree_cons (NULL_TREE, DECL_ARG_TYPE (parm),
                                     NULL_TREE);
              if (last)
                TREE_CHAIN (last) = type;
              else
                actual = type;
              last = type;
            }
          type = perm_tree_cons (NULL_TREE, void_type_node, NULL_TREE);
          if (last)
            TREE_CHAIN (last) = type;
          else
            actual = type;

          /* We are going to assign a new value for the TYPE_ACTUAL_ARG_TYPES
             of the type of this function, but we need to avoid having this
             affect the types of other similarly-typed functions, so we must
             first force the generation of an identical (but separate) type
             node for the relevant function type.  The new node we create
             will be a variant of the main variant of the original function
             type.  */

          TREE_TYPE (fndecl) = build_type_copy (TREE_TYPE (fndecl));

          TYPE_ACTUAL_ARG_TYPES (TREE_TYPE (fndecl)) = actual;
        }

      /* Now store the final chain of decls for the arguments
         as the decl-chain of the current lexical scope.
         Put the enumerators in as well, at the front so that
         DECL_ARGUMENTS is not modified.  */

      storedecls (chainon (nonparms, DECL_ARGUMENTS (fndecl)));
    }

  /* Make sure the binding level for the top of the function body
     gets a BLOCK if there are any in the function.
     Otherwise, the dbx output is wrong.  */

  keep_next_if_subblocks = 1;

  /* ??? This might be an improvement,
     but needs to be thought about some more.  */
#if 0
  keep_next_level_flag = 1;
#endif

  /* Write a record describing this function definition to the prototypes
     file (if requested).  */

  gen_aux_info_record (fndecl, 1, 0, prototype);

  /* Initialize the RTL code for the function.  */

  init_function_start (fndecl, input_filename, lineno);

  /* If this is a varargs function, inform function.c.  */

  if (c_function_varargs)
    mark_varargs ();

  /* Declare __FUNCTION__ and __PRETTY_FUNCTION__ for this function.  */

  declare_function_name ();

  /* Set up parameters and prepare for return, for the function.  */

  expand_function_start (fndecl, 0);

#ifdef GPC
  /* If this is the Pascal main program, install the stuff
   * responsible for handling of non-local `goto's.
   * This is similar to a subset of what `pop_function_context()' does.
   */
  if (DECL_NAME (fndecl)
      && DECL_NAME (fndecl) == main_program_name)
    {
      if (main_program_first_label_num >= 0)
        set_new_first_and_last_label_num (main_program_first_label_num,
                                          max_label_num ());
      current_function_contains_functions = 1;
      current_function_has_nonlocal_label |= main_program_has_nonlocal_label;
      current_function_needs_context |= main_program_needs_context;
      named_labels = main_program_named_labels;
#if 0
      nonlocal_labels = main_program_nonlocal_labels;
#endif
    }
#endif /* GPC */

  /* If this function is `main', emit a call to `__main'
     to run global initializers, etc.  */
  if (DECL_NAME (fndecl)
      && strcmp (IDENTIFIER_POINTER (DECL_NAME (fndecl)), "main") == 0
      && DECL_CONTEXT (fndecl) == NULL_TREE)
    expand_main_function ();
}

/* SPECPARMS is an identifier list--a chain of TREE_LIST nodes
   each with a parm name as the TREE_VALUE.  A null pointer as TREE_VALUE
   stands for an ellipsis in the identifier list.

   PARMLIST is the data returned by get_parm_info for the
   parmlist that follows the semicolon.

   We return a value of the same sort that get_parm_info returns,
   except that it describes the combination of identifiers and parmlist.  */

tree
combine_parm_decls (specparms, parmlist, void_at_end)
     tree specparms, parmlist;
     int void_at_end;
{
  register tree fndecl = current_function_decl;
  register tree parm;

  tree parmdecls = TREE_PURPOSE (parmlist);

  /* This is a chain of any other decls that came in among the parm
     declarations.  They were separated already by get_parm_info,
     so we just need to keep them separate.  */
  tree nonparms = TREE_VALUE (parmlist);

  tree types = 0;

  for (parm = parmdecls; parm; parm = TREE_CHAIN (parm))
    DECL_RESULT (parm) = 0;

  for (parm = specparms; parm; parm = TREE_CHAIN (parm))
    {
      register tree tail, found = NULL;

      /* See if any of the parmdecls specifies this parm by name.  */
      for (tail = parmdecls; tail; tail = TREE_CHAIN (tail))
        if (DECL_NAME (tail) == TREE_VALUE (parm))
          {
            found = tail;
            break;
          }

      /* If declaration already marked, we have a duplicate name.
         Complain, and don't use this decl twice.   */
      if (found && DECL_RESULT (found) != 0)
        {
          error_with_decl (found, "multiple parameters named `%s'");
          found = 0;
        }

      /* If the declaration says "void", complain and ignore it.  */
      if (found && TYPE_MAIN_VARIANT (TREE_TYPE (found)) == void_type_node)
        {
          error_with_decl (found, "parameter `%s' declared void");
          TREE_TYPE (found) = integer_type_node;
          DECL_ARG_TYPE (found) = integer_type_node;
          layout_decl (found, 0);
        }

      /* Traditionally, a parm declared float is actually a double.  */
      if (found && flag_traditional
          && TYPE_MAIN_VARIANT (TREE_TYPE (found)) == float_type_node)
        {
          TREE_TYPE (found) = double_type_node;
          DECL_ARG_TYPE (found) = double_type_node;
          layout_decl (found, 0);
        }

      /* If no declaration found, default to int.  */
      if (!found)
        {
          found = build_decl (PARM_DECL, TREE_VALUE (parm),
                              integer_type_node);
          DECL_ARG_TYPE (found) = TREE_TYPE (found);
          DECL_SOURCE_LINE (found) = DECL_SOURCE_LINE (fndecl);
          DECL_SOURCE_FILE (found) = DECL_SOURCE_FILE (fndecl);
          error_with_decl (found, "type of parameter `%s' is not declared");
          pushdecl (found);
        }

      TREE_PURPOSE (parm) = found;

      /* Mark this decl as "already found" -- see test, above.
         It is safe to use DECL_RESULT for this
         since it is not used in PARM_DECLs or CONST_DECLs.  */
      DECL_RESULT (found) = error_mark_node;
    }

  /* Complain about any actual PARM_DECLs not matched with any names.  */

  for (parm = parmdecls; parm; )
    {
      tree next = TREE_CHAIN (parm);
      TREE_CHAIN (parm) = 0;

      /* Complain about args with incomplete types.  */
      if (TYPE_SIZE (TREE_TYPE (parm)) == 0)
        {
          error_with_decl (parm, "parameter `%s' has incomplete type");
          TREE_TYPE (parm) = error_mark_node;
        }

      if (DECL_RESULT (parm) == 0)
        {
          error_with_decl (parm,
                           "declaration for parameter `%s' but no such parameter");
          /* Pretend the parameter was not missing.
             This gets us to a standard state and minimizes
             further error messages.  */
          specparms
            = chainon (specparms,
                       tree_cons (parm, NULL_TREE, NULL_TREE));
        }

      parm = next;
    }

  /* Chain the declarations together in the order of the list of names.
     At the same time, build up a list of their types, in reverse order.  */

  parm = specparms;
  parmdecls = 0;
  {
    register tree last;
    for (last = 0; parm; parm = TREE_CHAIN (parm))
      if (TREE_PURPOSE (parm))
        {
          if (last == 0)
            parmdecls = TREE_PURPOSE (parm);
          else
            TREE_CHAIN (last) = TREE_PURPOSE (parm);
          last = TREE_PURPOSE (parm);
          TREE_CHAIN (last) = 0;

          types = saveable_tree_cons (NULL_TREE, TREE_TYPE (parm), types);
        }
  }

  if (void_at_end)
    return saveable_tree_cons (parmdecls, nonparms,
                               nreverse (saveable_tree_cons (NULL_TREE,
                                                             void_type_node,
                                                             types)));

  return saveable_tree_cons (parmdecls, nonparms, nreverse (types));
}

/* Finish up a function declaration and compile that function
   all the way to assembler language output.  The free the storage
   for the function definition.

   This is called after parsing the body of the function definition.

   NESTED is nonzero if the function being finished is nested in another.  */

void
finish_function (nested)
     int nested;
{
  register tree fndecl = current_function_decl;

/*  TREE_READONLY (fndecl) = 1;
    This caused &foo to be of type ptr-to-const-function
    which then got a warning when stored in a ptr-to-function variable.  */

  poplevel (1, 0, 1);
  BLOCK_SUPERCONTEXT (DECL_INITIAL (fndecl)) = fndecl;

  /* Must mark the RESULT_DECL as being in this function.  */

  DECL_CONTEXT (DECL_RESULT (fndecl)) = fndecl;

  /* Obey `register' declarations if `setjmp' is called in this fn.  */
  if (flag_traditional && current_function_calls_setjmp)
    {
      setjmp_protect (DECL_INITIAL (fndecl));
      setjmp_protect_args ();
    }

  if (! strcmp (IDENTIFIER_POINTER (DECL_NAME (fndecl)), "main"))
    {
      if (TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (fndecl)))
          != integer_type_node)
        {
          /* You would expect the sense of this test to be the other way
             around, but if warn_main is set, we will already have warned,
             so this would be a duplicate.  This is the warning you get
             in some environments even if you *don't* ask for it, because
             these are environments where it may be more of a problem than
             usual.  */
          if (! warn_main)
            pedwarn_with_decl (fndecl, "return type of `%s' is not `int'");
        }
      else
        {
#ifdef DEFAULT_MAIN_RETURN
          /* Make it so that `main' always returns success by default.  */
          DEFAULT_MAIN_RETURN;
#endif
        }
    }

  /* Generate rtl for function exit.  */
  expand_function_end (input_filename, lineno, 0);

  /* So we can tell if jump_optimize sets it to 1.  */
  can_reach_end = 0;

  /* Run the optimizers and output the assembler code for this function.  */
  rest_of_compilation (fndecl);

  current_function_returns_null |= can_reach_end;

  if (TREE_THIS_VOLATILE (fndecl) && current_function_returns_null)
    warning ("`noreturn' function does return");
#ifndef GPC
  else if (warn_return_type && can_reach_end
           && TYPE_MAIN_VARIANT (TREE_TYPE (TREE_TYPE (fndecl))) != void_type_node)
    /* If this function returns non-void and control can drop through,
       complain.  */
    warning ("control reaches end of non-void function");
#endif /* GPC */
  /* With just -W, complain only if function returns both with
     and without a value.  */
  else if (extra_warnings
           && current_function_returns_value && current_function_returns_null)
    warning ("this function may return with or without a value");

  /* If requested, warn about function definitions where the function will
     return a value (usually of some struct or union type) which itself will
     take up a lot of stack space.  */

  if (warn_larger_than && !DECL_EXTERNAL (fndecl) && TREE_TYPE (fndecl))
    {
      register tree ret_type = TREE_TYPE (TREE_TYPE (fndecl));

      if (ret_type)
        {
          register tree ret_type_size = TYPE_SIZE (ret_type);

          if (TREE_CODE (ret_type_size) == INTEGER_CST)
            {
              unsigned units
                = TREE_INT_CST_LOW (ret_type_size) / BITS_PER_UNIT;

              if (units > larger_than_size)
                warning_with_decl (fndecl,
                                   "size of return value of `%s' is %u bytes",
                                   units);
            }
        }
    }

  /* Free all the tree nodes making up this function.  */
  /* Switch back to allocating nodes permanently
     until we start another function.  */
  if (! nested)
    permanent_allocation (1);

  if (DECL_SAVED_INSNS (fndecl) == 0 && ! nested)
    {
      /* Stop pointing to the local nodes about to be freed.  */
      /* But DECL_INITIAL must remain nonzero so we know this
         was an actual function definition.  */
      /* For a nested function, this is done in pop_c_function_context.  */
      /* If rest_of_compilation set this to 0, leave it 0.  */
      if (DECL_INITIAL (fndecl) != 0)
        DECL_INITIAL (fndecl) = error_mark_node;
      DECL_ARGUMENTS (fndecl) = 0;
    }

  if (DECL_STATIC_CONSTRUCTOR (fndecl))
    {
#ifndef ASM_OUTPUT_CONSTRUCTOR
      if (! flag_gnu_linker)
        static_ctors = perm_tree_cons (NULL_TREE, fndecl, static_ctors);
      else
#endif
      if (! flag_syntax_only)
        assemble_constructor (IDENTIFIER_POINTER (DECL_NAME (fndecl)));
    }
  if (DECL_STATIC_DESTRUCTOR (fndecl))
    {
#ifndef ASM_OUTPUT_DESTRUCTOR
      if (! flag_gnu_linker)
        static_dtors = perm_tree_cons (NULL_TREE, fndecl, static_dtors);
      else
#endif
      if (! flag_syntax_only)
        assemble_destructor (IDENTIFIER_POINTER (DECL_NAME (fndecl)));
    }

  if (! nested)
    {
      /* Let the error reporting routines know that we're outside a
         function.  For a nested function, this value is used in
         pop_c_function_context and then reset via pop_function_context.  */
      current_function_decl = NULL;
    }
}

#ifdef GPC
void
report_unresolved_forward_pointers (thislevel_only)
     int thislevel_only;
{
  tree tail;
  struct binding_level *level;

  for (level = current_binding_level; level; level = level->level_chain)
    {
      for (tail = level->tags; tail; tail = TREE_CHAIN (tail))
        {
          if (TREE_CODE (TREE_VALUE (tail)) == POINTER_TYPE &&
              TREE_CODE (TREE_TYPE (TREE_VALUE (tail))) == LANG_TYPE)
            {
              tree id = TYPE_NAME (TREE_TYPE (TREE_VALUE (tail)));
              error ("referenced type `%s' undefined",
                     IDENTIFIER_POINTER (id));
            }
        }
      if (thislevel_only && ! level->tag_transparent)
        return;
    }
  return;
}

int
resolve_forward_pointer (decl, to_type)
     tree decl;
     tree to_type;
{
  tree ref = NULL_TREE;
  int ret = FALSE;
  tree new_pointer_to;

  if (decl)
    {
      tree name = DECL_NAME (decl);
      ref = lookup_tag (POINTER_TYPE, name,
                        current_binding_level, 1);
    }

  if (ref)
    {
      /* If a tag is found, but it is already patched
       * don't do anything.
       */
      if (TREE_CODE (TREE_TYPE (ref)) != LANG_TYPE)
        return FALSE;

      /* If we find a LANG_TYPE item in the tag list it is
       * a base type of some pointer that has already been
       * defined.
       */

      if (! TYPE_POINTER_TO (ref))
        abort ();

      /* The LANG_TYPE type REF is the base type of the pointer.
       */
      if (TREE_TYPE (TYPE_POINTER_TO (ref)) != ref)
        abort ();

      ret = TRUE;
    }
  else
    {
      tree tdecl;

      /* If the type of the declaration is a type that
       * can contain pointers, we must check the components
       */
      if (TREE_CODE (to_type) == RECORD_TYPE
          || TREE_CODE (to_type) == UNION_TYPE)
        {
          /* Scan the record fields */
          tree field;
          for (field = TYPE_FIELDS (to_type);
               field;
               field = TREE_CHAIN (field))
            {
              ret |= resolve_forward_pointer (NULL_TREE,
                                              TREE_TYPE (field));
            }
          return ret;
        }
      else if (TREE_CODE (to_type) == FILE_TYPE
               || TREE_CODE (to_type) == ARRAY_TYPE
               || TREE_CODE (to_type) == FUNCTION_TYPE)
        {
          return resolve_forward_pointer (NULL_TREE, TREE_TYPE (to_type));
        }

      /* If no such tag is found, it might be a pointer
       * to one of the predefined types
       */
      if (TREE_CODE (to_type) != POINTER_TYPE
          || TREE_CODE (TREE_TYPE (to_type)) != LANG_TYPE)
        return FALSE;

      /* @@@ What happens to TYPE_POINTER_TO in this? */

      /* It is a forward pointer to a type, but
       * in this case the domain type has not been declared
       * in the user program.
       */

      ref = TREE_TYPE (to_type);

      tdecl = check_if_predefined_type (TYPE_NAME (ref));

      if (! tdecl)
        {
          error ("pointer domain type `%s' undefined",
                 IDENTIFIER_POINTER (TYPE_NAME (ref)));
          /* Avoid multiple error messages */
          to_type = integer_type_node;
        }
      else if (TREE_CODE (tdecl) != TYPE_DECL)
        {
          error ("pointer domain `%s' is not a type",
                 IDENTIFIER_POINTER (TYPE_NAME (ref)));
          /* Avoid multiple error messages */
          to_type = integer_type_node;
        }
      else
        {
          to_type = TREE_TYPE (tdecl);
          ret = TRUE;
        }
    }

  if (TYPE_POINTER_TO (ref) == NULL_TREE
      || TREE_CODE (TYPE_POINTER_TO (ref)) != POINTER_TYPE)
    abort ();
  new_pointer_to = copy_node (TYPE_POINTER_TO (ref));
  TREE_TYPE (TYPE_POINTER_TO (ref)) = to_type;
  TYPE_POINTER_TO (ref) = new_pointer_to;
  return ret;
}

/* Scan the list of names of the current type_definition_part
 * and resolve forward pointers placed in the current tags list.
 */
void
handle_forward_pointers (list)
     tree list;
{
  tree scan;
  struct binding_level *level;

#if 0
  /* Pass 1:  For each type name, look whether it is referenced
   * from somewhere and resolve it.
   */
  for (scan = list; scan; scan = TREE_CHAIN (scan))
    {
      tree decl, ref;
      char *ptr_name, *ref_name;

      decl = lookup_name (TREE_VALUE (scan));
      if (TREE_CODE (decl) != TYPE_DECL || DECL_NAME (decl) == NULL_TREE)
        abort ();
      ptr_name = concat ("ptr_xref_", IDENTIFIER_POINTER (DECL_NAME (decl)), NULL_PTR);
      ref = lookup_tag (POINTER_TYPE, get_identifier (ptr_name),
                        current_binding_level, 1);
      if (ref)
        {
          tree type = TREE_TYPE (decl);
          if (TREE_CODE (ref) != POINTER_TYPE)
            abort ();
          TREE_TYPE (ref) = type;
          TYPE_POINTER_TO (type) = ref;
        }
      ref_name = concat ("ref_xref_", IDENTIFIER_POINTER (DECL_NAME (decl)), NULL_PTR);
      ref = lookup_tag (REFERENCE_TYPE, get_identifier (ref_name),
                        current_binding_level, 1);
      if (ref)
        {
          tree type = TREE_TYPE (decl);
          if (TREE_CODE (ref) != REFERENCE_TYPE)
            abort ();
          TREE_TYPE (ref) = type;
          TYPE_REFERENCE_TO (type) = ref;
        }
      free (ptr_name);
      free (ref_name);
    }
#endif

  /* Pass 2:  For each type name, look whether it is still an
   * unresolved reference and try to resolve it using existing
   * types.  If that fails, report an error.
   */
  for (level = current_binding_level; level; level = level->level_chain)
    {
      tree tail;
      for (tail = level->tags; tail; tail = TREE_CHAIN (tail))
        {
          if (TREE_CODE (TREE_VALUE (tail)) == POINTER_TYPE
              || TREE_CODE (TREE_VALUE (tail)) == REFERENCE_TYPE)
            {
              tree type = TREE_VALUE (tail);
              tree target_type = TREE_TYPE (type);
              tree id = TYPE_POINTER_TO (target_type)
                        ? TYPE_NAME (TYPE_POINTER_TO (target_type))
                        : NULL_TREE;
              if (id)
                {
                  if (TREE_CODE (id) == TYPE_DECL)
                    id = DECL_NAME (id);
                  if (TREE_CODE (id) != IDENTIFIER_NODE)
                    abort ();
                }
              if (id && strncmp (IDENTIFIER_POINTER (id), "ptr_xref_", 8) == 0)
                {
                  tree patched_type;
                  if (TREE_CODE (target_type) == LANG_TYPE)
                    {
                      tree patched_id = get_identifier (IDENTIFIER_POINTER (id) + 9);
                      patched_type = lookup_name (patched_id);
                    }
                  else
                    patched_type = target_type;
                  if (TREE_CODE (patched_type) == TYPE_DECL)
                    patched_type = TREE_TYPE (patched_type);
                  if (patched_type == NULL_TREE)
                    {
                      error ("referenced type `%s' undefined",
                             IDENTIFIER_POINTER (id));
                      patched_type = error_mark_node;
                    }

                  /* Patch all variants.
                   */
                  target_type = TYPE_MAIN_VARIANT (target_type);
                  while (target_type)
                    {
                      int constp = TYPE_READONLY (target_type);
                      int volatilep = TYPE_VOLATILE (target_type);
                      tree pointer_type = TYPE_POINTER_TO (target_type);
                      tree reference_type = TYPE_REFERENCE_TO (target_type);
                      tree patched_target_type = build_type_variant (patched_type,
                                                                     constp, volatilep);
                      if (pointer_type)
                        TREE_TYPE (pointer_type) = patched_target_type;
                      if (reference_type)
                        TREE_TYPE (reference_type) = patched_target_type;
                      target_type = TYPE_NEXT_VARIANT (target_type);
                    }
                }
            }
        }
      /* if (! level->tag_transparent)
        break; */
    }

  /* Pass 3:  For each pointer type definition, propagate the
   * resolved reference from the tag to the actual type.
   */
  for (scan = list; scan; scan = TREE_CHAIN (scan))
    {
      tree decl, type;

      decl = lookup_name (TREE_VALUE (scan));
      type = TREE_TYPE (decl);

      if (TREE_CODE (type) == POINTER_TYPE
          && TREE_CODE (TREE_TYPE (type)) == LANG_TYPE)
        {
          tree patched_pointer_type = TYPE_POINTER_TO (TREE_TYPE (type));
          TREE_TYPE (type) = TREE_TYPE (patched_pointer_type);

#if 0
          tree target_type = TREE_TYPE (type);
          TYPE_NAME (target_type) = TYPE_NAME (type);

          /* Do this for all pointers and references to all
           * variants of the target type.
           */
          while (target_type)
            {
              int constp = TYPE_READONLY (target_type);
              int volatilep = TYPE_VOLATILE (target_type);
              tree pointer_type = TYPE_POINTER_TO (target_type);
              tree reference_type = TYPE_REFERENCE_TO (target_type);
              tree patched_target_type = build_type_variant (TREE_TYPE (patched_type),
                                                             constp, volatilep);
              if (pointer_type)
                TREE_TYPE (pointer_type) = patched_target_type;
              if (reference_type)
                TREE_TYPE (reference_type) = patched_target_type;
              target_type = TYPE_NEXT_VARIANT (target_type);
            }
#endif
        }
    }
}

/* To implement `with' statement:
 *
 *  1) Call pascal_shadow_record_fields() for each record_type
 *     element in the `with' element list.  Each call creates a
 *     new binding level.
 *
 *  2) Construct a component_ref for EACH field in the record,
 *     and store it to the IDENTIFIER_LOCAL_VALUE after adding
 *     the old value to the shadow list.
 *
 *  3) Let lookup_name do the rest.
 *
 *  4) Pop all of the binding levels after the `with' statement ends
 *     (restoring old local values).  You have to keep track of the number
 *     of times you called it.
 */

/*
 * Declare the fields so that lookup_name() will find them as
 * component refs for Pascal `with'.
 *
 * Proceeds to the inner layers of Pascal variant records
 * and to the contents of schemata.
 *
 * Internal routine of pascal_shadow_record_fields ()
 */
static void
handle_one_level (parent, fields, structors_only)
     tree parent;
     tree fields;
     int structors_only;
{
  tree field;
  tree name;

  switch (TREE_CODE (TREE_TYPE (parent)))
    {
      case RECORD_TYPE:
      case UNION_TYPE:
        for (field = fields;
             field;
             field = TREE_CHAIN (field))
          {
            name = DECL_NAME (field);
            if (name == NULL_TREE)
              {
                /* Proceed through variant part.
                 */
                handle_one_level (parent, TYPE_FIELDS (TREE_TYPE (field)),
                                  structors_only);
              }
            else if (name == schema_id)
              {
                /* Proceed through contents of schema.
                 */
                tree schema = build_component_ref (parent, name);
                if (TREE_CODE (TREE_TYPE (schema)) == RECORD_TYPE)
                  handle_one_level (schema, TYPE_FIELDS (TREE_TYPE (field)),
                                    structors_only);
              }
            else if (structors_only == 0
                     || (TREE_CODE (field) == FUNCTION_DECL
                         && (PASCAL_STRUCTOR_METHOD (field))))
              {
                tree old  = IDENTIFIER_LOCAL_VALUE (name);
                current_binding_level->shadowed =
                    tree_cons (name,
                               old,
                               current_binding_level->shadowed);

                IDENTIFIER_LOCAL_VALUE (name) =
                    build_component_ref (parent, name);
                prediscriminate_schema (IDENTIFIER_LOCAL_VALUE (name), 0);
              }
          }
        break;
      default:
        warning ("Internal GPC error: handle_one_level is broken");
    }
}

/*
 * For each FIELD_DECL node in a RECORD_TYPE, we have to declare
 * a name so that lookup_name will find a COMPONENT_REF node
 * when the name is referenced.  This happens in Pascal `with' statement.
 *
 * If the `with' element is not a VAR_DECL node, create a temporary
 * variable and store the address of the `with' element there.
 * Then create an INDIRECT_REF node through this temporary variable
 * to make sure that the `with' element expression is calculated only
 * once, as the standard requires.
 *
 * If STRUCTORS_ONLY is nonzero, only constructors and destructors of
 * objects are shadowed, but other methods and data fields are not.
 * This is needed for `New' and `Dispose'.
 *
 * Return the number of `pushlevel()'s performed.
 */
int
pascal_shadow_record_fields (element, structors_only)
     tree element;
     int structors_only;
{
  tree parent;
  if (TREE_CODE (element) == FUNCTION_DECL
           && (TREE_CODE (TREE_TYPE (TREE_TYPE (element))) == RECORD_TYPE
               || TREE_CODE (TREE_TYPE (TREE_TYPE (element))) == UNION_TYPE))
    {
      tree temp_var = make_new_variable ("with_element",
                                         TREE_TYPE (TREE_TYPE (element)));
      expand_expr_stmt (build_modify_expr (temp_var, NOP_EXPR,
                                           probably_call_function (element)));
      element = temp_var;
    }
  switch (TREE_CODE (TREE_TYPE (element)))
    {
      case ERROR_MARK:
        return 0;
      case UNION_TYPE:
      case RECORD_TYPE:
        pushlevel (0);
        if (TREE_CODE (element) == VAR_DECL)
          parent = lookup_name (DECL_NAME (element));
        else  /* some kind of an expression, should we check it??? */
          {
            tree rec_addr;
            tree elem_type = TREE_TYPE (element);
            if (TREE_READONLY (element))
              elem_type = build_type_variant (elem_type, 1,
                                              TYPE_VOLATILE (elem_type));
            rec_addr = start_decl (get_identifier ("<with_element>"),
                         build_tree_list (NULL_TREE,
                           build_pointer_type (elem_type)),
                         0, NULL_TREE, NULL_TREE);
            finish_decl (rec_addr, NULL_TREE, NULL_TREE);
            expand_expr_stmt (build_modify_expr (rec_addr, NOP_EXPR,
                                build_unary_op (ADDR_EXPR, element, 0)));
            parent = build_indirect_ref (rec_addr, "`with'");
          }
        handle_one_level (parent, TYPE_FIELDS (TREE_TYPE (parent)),
                          structors_only);
        return 1;
      default:
        error ("`with' element must be of record, schema, or object type");
        return 0;
    }
}
#endif /* GPC */

/* Save and restore the variables in this file and elsewhere
   that keep track of the progress of compilation of the current function.
   Used for nested functions.  */

struct c_function
{
  struct c_function *next;
  tree named_labels;
  tree shadowed_labels;
  int returns_value;
  int returns_null;
  int warn_about_return_type;
  int extern_inline;
  struct binding_level *binding_level;
#ifdef GPC
  tree function_declared_labels;
#endif /* GPC */
};

struct c_function *c_function_chain;

/* Save and reinitialize the variables
   used during compilation of a C function.  */

void
push_c_function_context ()
{
  struct c_function *p
    = (struct c_function *) xmalloc (sizeof (struct c_function));

#ifndef GPC
  if (pedantic)
    pedwarn ("ANSI C forbids nested functions");
#endif /* GPC */

  push_function_context ();

  p->next = c_function_chain;
  c_function_chain = p;

  p->named_labels = named_labels;
  p->shadowed_labels = shadowed_labels;
  p->returns_value = current_function_returns_value;
  p->returns_null = current_function_returns_null;
  p->warn_about_return_type = warn_about_return_type;
  p->extern_inline = current_extern_inline;
  p->binding_level = current_binding_level;
#ifdef GPC
  p->function_declared_labels = declared_labels;
#endif /* GPC */
}

/* Restore the variables used during compilation of a C function.  */

void
pop_c_function_context ()
{
  struct c_function *p = c_function_chain;
  tree link;

  /* Bring back all the labels that were shadowed.  */
  for (link = shadowed_labels; link; link = TREE_CHAIN (link))
    if (DECL_NAME (TREE_VALUE (link)) != 0)
      IDENTIFIER_LABEL_VALUE (DECL_NAME (TREE_VALUE (link)))
        = TREE_VALUE (link);

  if (DECL_SAVED_INSNS (current_function_decl) == 0)
    {
      /* Stop pointing to the local nodes about to be freed.  */
      /* But DECL_INITIAL must remain nonzero so we know this
         was an actual function definition.  */
      DECL_INITIAL (current_function_decl) = error_mark_node;
      DECL_ARGUMENTS (current_function_decl) = 0;
    }

  pop_function_context ();

  c_function_chain = p->next;

  named_labels = p->named_labels;
  shadowed_labels = p->shadowed_labels;
  current_function_returns_value = p->returns_value;
  current_function_returns_null = p->returns_null;
  warn_about_return_type = p->warn_about_return_type;
  current_extern_inline = p->extern_inline;
  current_binding_level = p->binding_level;
#ifdef GPC
  declared_labels = p->function_declared_labels;
#endif /* GPC */

  free (p);
}

/* integrate_decl_tree calls this function, but since we don't use the
   DECL_LANG_SPECIFIC field, this is a no-op.  */

void
copy_lang_decl (node)
     tree node;
{
}
