/* gpc-defs.h -- global definitions for GNU Pascal
   Copyright (C) 1997-2000 Free Software Foundation, Inc.

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

#ifndef _GPC_DEFS_H_
#define _GPC_DEFS_H_

/* GPC compile time configuration switches */

#define DEBUG_HASH              /* Debug hash in GPI */

/* #undef DEBUG */              /* Used in execute() in module.c */

/* Include files everyone gets. */

/* Don't use the system's <assert.h>, see the comments in ../dwarfout.c */
#define assert(expression) \
  ((void) ((expression) ? 0 : \
   (fprintf (stderr, "%s:%u: failed assertion `" #expression "'\n", __FILE__, __LINE__), abort (), 0)))

#define build_c_type_variant c_build_type_variant

#ifdef EGCS92
/* copied from egcs c-decl.c */
/* Add qualifiers to a type, in the fashion for C.  */
extern tree c_build_qualified_type              PROTO((tree, int));
#define c_build_type_variant(TYPE, CONST_P, VOLATILE_P)           \
  c_build_qualified_type (TYPE,                                   \
                          ((CONST_P) ? TYPE_QUAL_CONST : 0) |     \
                          ((VOLATILE_P) ? TYPE_QUAL_VOLATILE : 0))
#endif /* EGCS92 */

#ifndef EGCS

#include <stdio.h>

#if 0
#ifdef __GNUC__
# define alloca __builtin_alloca
#else
# if HAVE_ALLOCA_H
#  include <alloca.h>
# else
#  ifdef _AIX
#   pragma alloca
#  else
#   ifdef _WIN32
#    include <malloc.h>
#   else
#    ifndef alloca      /* predefined by HP cc +0libcalls */
char *alloca ();
#    endif
#   endif
#  endif
# endif
#endif /* ! defined (__GNUC__) */
#endif

#ifdef HAVE_STRING_H
# include <string.h>
#else
# ifdef HAVE_STRINGS_H
#  include <strings.h>
# endif
#endif

#ifndef bcopy
# ifdef HAVE_BCOPY
#  ifdef NEED_DECLARATION_BCOPY
extern void bcopy ();
#  endif
# else /* ! HAVE_BCOPY */
#  define bcopy(src,dst,len) memcpy ((dst),(src),(len))
# endif
#endif

#ifndef bcmp
# ifdef HAVE_BCMP
#  ifdef NEED_DECLARATION_BCMP
extern int bcmp ();
#  endif
# else /* ! HAVE_BCMP */
#  define bcmp(left,right,len) memcmp ((left),(right),(len))
# endif
#endif

#ifndef bzero
# ifdef HAVE_BZERO
#  ifdef NEED_DECLARATION_BZERO
extern void bzero ();
#  endif
# else /* ! HAVE_BZERO */
#  define bzero(dst,len) memset ((dst),0,(len))
# endif
#endif

#ifndef index
# ifdef HAVE_INDEX
#  ifdef NEED_DECLARATION_INDEX
extern char *index ();
#  endif
# else /* ! HAVE_INDEX */
#  define index strchr
# endif
#endif

#ifndef rindex
# ifdef HAVE_RINDEX
#  ifdef NEED_DECLARATION_RINDEX
extern char *rindex ();
#  endif
# else /* ! HAVE_RINDEX */
#  define rindex strrchr
# endif
#endif

/* Macro that inserts it's argument in double quotes */
#ifndef STRINGIFY
# if defined(HAVE_CPP_STRINGIFY) || (defined(__GNUC__) && defined(__STDC__))
#  define STRINGIFY(STRING) #STRING
# else
#  define STRINGIFY(STRING) "STRING"
# endif
#endif /* ! STRINGIFY */

#endif /* ! EGCS */

#ifndef PATH_SEPARATOR
#define PATH_SEPARATOR ':'
#endif

#ifndef DIR_SEPARATOR
#define DIR_SEPARATOR '/'
#endif

#include "lang.h"

/* If real is not double must take special care to read in floating point
 * const in gpc-decl.c(init_decl_processing)
 */
#ifdef REAL_IS_NOT_DOUBLE
# define GPC_GET_REAL(real) REAL_VALUE_ATOF (STRINGIFY (real), DFmode)
#else
# define GPC_GET_REAL(X) X
#endif

/* Default field widths for Integers, Reals and Booleans
 */
#ifndef INT_OUT_WIDTH
# define INT_OUT_WIDTH          0
#endif

#ifndef REAL_OUT_WIDTH
# define REAL_OUT_WIDTH         0
#endif

#ifndef BOOL_OUT_WIDTH
# define BOOL_OUT_WIDTH         0
#endif

#ifndef LONG_INT_OUT_WIDTH
# define LONG_INT_OUT_WIDTH     0
#endif

#ifndef LONG_REAL_OUT_WIDTH
# define LONG_REAL_OUT_WIDTH    0
#endif

/*
 * Define the type of Pascal we are compiling.
 * Used for reserved word recognition, warning messages, etc.
 * Let's hope that there will not be more than 32 important dialects ...
 */
#define STANDARD_PASCAL_LEVEL_0   0x1   /* ISO 7185 Standard Pascal, level 0 */
#define STANDARD_PASCAL_LEVEL_1   0x2   /* ISO 7185 Standard Pascal, level 1 */
#define EXTENDED_PASCAL           0x4   /* ISO 10206 Extended Pascal */
#define OBJECT_PASCAL             0x8   /* ANSI draft Object Pascal */
#define UCSD_PASCAL              0x10   /* UCSD Pascal */
#define BORLAND_PASCAL           0x20   /* Borland Pascal 7.0 */
#define BORLAND_DELPHI           0x40   /* Borland Delphi */
#define PASCAL_SC                0x80   /* Pascal-SC (PXSC) */
#define VAX_PASCAL              0x100   /* VAX Pascal */
/* to be continued ... */
#define GNU_PASCAL         0x40000000   /* GNU Pascal (default) */
#define DIALECT_MASK_MAX   0x40000000
#define ANY_PASCAL         0xFFFFFFFF

/* Abbreviations for combinations of these flags
 */
#define STANDARD_PASCAL (STANDARD_PASCAL_LEVEL_0 | STANDARD_PASCAL_LEVEL_1)
#define E_O_PASCAL (EXTENDED_PASCAL | OBJECT_PASCAL)
#define S_E_O_PASCAL (STANDARD_PASCAL | E_O_PASCAL)
#define ISO_PASCAL (STANDARD_PASCAL | E_O_PASCAL)
#define B_D_PASCAL (BORLAND_PASCAL | BORLAND_DELPHI)
#define U_B_D_PASCAL (UCSD_PASCAL | B_D_PASCAL)
#define E_O_B_D_PASCAL (E_O_PASCAL | B_D_PASCAL)
#define E_O_U_B_D_PASCAL (E_O_PASCAL | U_B_D_PASCAL)
#define O_G_PASCAL (OBJECT_PASCAL | GNU_PASCAL)
#define E_O_G_PASCAL (EXTENDED_PASCAL | O_G_PASCAL)
#define S_E_O_G_PASCAL (STANDARD_PASCAL | E_O_G_PASCAL)
#define S1_E_O_G_PASCAL (STANDARD_PASCAL_LEVEL_1 | E_O_G_PASCAL)
#define D_G_PASCAL (BORLAND_DELPHI | GNU_PASCAL)
#define E_O_D_G_PASCAL (E_O_G_PASCAL | D_G_PASCAL)
#define B_D_G_PASCAL (BORLAND_PASCAL | D_G_PASCAL)
#define U_B_D_G_PASCAL ( UCSD_PASCAL | B_D_G_PASCAL)
#define O_B_D_G_PASCAL (OBJECT_PASCAL | B_D_G_PASCAL)
#define E_O_B_D_G_PASCAL (EXTENDED_PASCAL | O_B_D_G_PASCAL)
#define E_O_U_B_D_G_PASCAL (UCSD_PASCAL | E_O_B_D_G_PASCAL)
#define O_D_G_PASCAL (OBJECT_PASCAL | D_G_PASCAL)
#define SC_G_PASCAL (PASCAL_SC | GNU_PASCAL)

/* Check whether we must warn.  The argument specifies the Pascal
 * dialect(s) in which the construct in question is legal; in all
 * the other cases, a warning is triggered.  Always warn if we are
 * --pedantic.
 */
#define PEDANTIC(flags) (pedantic || \
                         (flag_what_pascal && (flag_what_pascal & flags) == 0))

/*
 * Some Pascal set constructors do not allow us to derive the
 * set size from anywhere. In such cases, the maximum allowed
 * members in the set is defined here. (Otherwise, there is no limit)
 *
 * By default the size of these sets is DEFAULT_SET_SIZE bits
 *
 * Users may change this with -fsetlimit:XX switch at compile time.
 *
 */

/* This is the size in bits */
#define DEFAULT_SET_SIZE        (8 * BITS_PER_WORD)

#define ORDINAL_TYPE(code) \
 ((code) == INTEGER_TYPE || (code) == CHAR_TYPE || \
  (code) == BOOLEAN_TYPE || (code) == ENUMERAL_TYPE)

#define ORDINAL_OR_REAL_TYPE(c) (ORDINAL_TYPE(c) || (c) == REAL_TYPE)

#define ORDINAL_REAL_OR_COMPLEX_TYPE(c) (ORDINAL_OR_REAL_TYPE(c) || (c) == COMPLEX_TYPE)

#define INT_REAL(c) ((c) == INTEGER_TYPE || (c) == REAL_TYPE)

#define INT_REAL_BOOL(c) ((c) == INTEGER_TYPE || (c) == REAL_TYPE || (c) == BOOLEAN_TYPE)

#define IS_ENUMERAL(c) ((c) == BOOLEAN_TYPE || (c) == ENUMERAL_TYPE)

#define IS_BOOLEAN(c) ((c) == BOOLEAN_TYPE)

/* @@ Is this really used anymore? */
#define LOGICAL_RESULT_TYPE boolean_type_node

/* If c is SET_TYPE or a set CONSTRUCTOR */
#define SET_OR_CONST(c) ((c) == SET_TYPE || (c) == CONSTRUCTOR)

/* This is a TEXT file type node
 */
#define TYPE_FILE_TEXT          TYPE_MIN_VALUE

/* Misuse DECL_INITIAL in VAR_DECL and FIELD_DECL nodes to hold
 * a fix-up list for schema discriminants.
 *
 * @@ Maybe this should be moved to DECL_LANG_SPECIFIC?
 */
#define PASCAL_DECL_FIXUPLIST(decl) DECL_INITIAL (decl)

/* This flag is set if the type is `packed'
 */
#define PASCAL_TYPE_PACKED(type)        TYPE_PACKED (type)

/* TYPE_LANG_FLAG_0 and 1 are used by C.
 * @@@ Check whether we really use those C constructs.
 */

/* `restricted' type qualified.
 */
#define PASCAL_TYPE_RESTRICTED(type)    TYPE_LANG_FLAG_2 (type)

/* Bindable types get this flag.
 */
#define PASCAL_TYPE_BINDABLE(type)      TYPE_LANG_FLAG_3 (type)

/* Flag conformant array schema index types.
 * Only used in ORDINAL *_TYPE nodes (indices of conformant arrays).
 */
#define PASCAL_TYPE_CONFORMANT_INDEX(type) TYPE_LANG_FLAG_5 (type)

/* Borland Pascal `typed const' type qualifier ("almost readonly").
 */
#define PASCAL_TYPE_TYPEDCONST(type)    TYPE_LANG_FLAG_6 (type)

/* Use some fields in DECL_LANG_SPECIFIC.
 */
#define DECL_LANG_INTERFACE(decl) (DECL_LANG_SPECIFIC ((decl))->interface)
#define DECL_LANG_UID(decl) (DECL_LANG_SPECIFIC ((decl))->uid)
#define DECL_LANG_DIRECTIVE(decl) (DECL_LANG_SPECIFIC ((decl))->directive)
#define DECL_LANG_PARMS(decl) (DECL_LANG_SPECIFIC ((decl))->parms)

/* Use some fields in TYPE_LANG_SPECIFIC.
 */
#define TYPE_LANG_INTERFACE(type) (TYPE_LANG_SPECIFIC ((type))->interface)
#define TYPE_LANG_UID(type) (TYPE_LANG_SPECIFIC ((type))->uid)

/* Use TYPE_LANG_SPECIFIC (type)->code to identify variant records,
 * objects, strings, and schemata.
 */
#define TYPE_LANG_CODE(type) (TYPE_LANG_SPECIFIC (type)->code)

#define PASCAL_LANG_NOTHING_SPECIAL     0
#define PASCAL_LANG_VARIANT_RECORD      1
#define PASCAL_LANG_OBJECT              2
#define PASCAL_LANG_STRING              3
#define PASCAL_LANG_SCHEMA              4

/* Use TYPE_LANG_SPECIFIC (type)->info to store the name of an object.
 */
#define TYPE_LANG_NAME(type) (TYPE_LANG_SPECIFIC (type)->info)

/* Also use TYPE_LANG_SPECIFIC (type)->info to store variant tag info.
 */
#define TYPE_LANG_INFO(type) (TYPE_LANG_SPECIFIC (type)->info)

/* Also use TYPE_LANG_SPECIFIC (type)->info to store the
 * declared string capacity.
 */
#define TYPE_LANG_DECLARED_CAPACITY(type) (TYPE_LANG_SPECIFIC (type)->info)

/* Use TYPE_LANG_SPECIFIC (type)->base to store base type info
 * about objects and schemata.
 */
#define TYPE_LANG_BASE(type) (TYPE_LANG_SPECIFIC (type)->base)

/* Determine if a given TYPE node represents a conformant array type.
 */
#define PASCAL_TYPE_CONFORMANT_ARRAY(type)           \
    (TREE_CODE (type) == ARRAY_TYPE                  \
     && TREE_TYPE (TYPE_DOMAIN (type))               \
     && PASCAL_TYPE_CONFORMANT_INDEX (TREE_TYPE (TYPE_DOMAIN (type))))

/* Determine if a given TYPE node represents a variant record type.
 */
#define PASCAL_TYPE_VARIANT_RECORD(type)             \
    (TREE_CODE (type) == RECORD_TYPE                 \
     && TYPE_LANG_SPECIFIC (type)                    \
     && TYPE_LANG_CODE (type) == PASCAL_LANG_VARIANT_RECORD)

/* Determine if a given TYPE node represents an object type.
 */
#define PASCAL_TYPE_OBJECT(type)                     \
    (TREE_CODE (type) == RECORD_TYPE                 \
     && TYPE_LANG_SPECIFIC (type)                    \
     && TYPE_LANG_CODE (type) == PASCAL_LANG_OBJECT)

/* Determine if a given TYPE node represents a string type.
 */
#define PASCAL_TYPE_STRING(type)                     \
    (TREE_CODE (type) == RECORD_TYPE                 \
     && TYPE_LANG_SPECIFIC (type)                    \
     && TYPE_LANG_CODE (type) == PASCAL_LANG_STRING)

/* Determine if a given TYPE node represents a schema type.
 */
#define PASCAL_TYPE_SCHEMA(type)                     \
    (TREE_CODE (type) == RECORD_TYPE                 \
     && TYPE_LANG_SPECIFIC (type)                    \
     && TYPE_LANG_CODE (type) == PASCAL_LANG_SCHEMA)

/* Determine whether some TYPE is an undiscriminated string.
 */
#define PASCAL_TYPE_UNDISCRIMINATED_STRING(type)                    \
    (TYPE_MAIN_VARIANT (type) == string_schema_proto_type           \
     || TYPE_MAIN_VARIANT (type) == const_string_schema_proto_type)

/* Determine whether some TYPE is a pre-discriminated string.
 */
#ifdef PG__NEW_STRINGS
#define PASCAL_TYPE_PREDISCRIMINATED_STRING(type)                   \
    (PASCAL_TYPE_UNDISCRIMINATED_STRING (type))
#else
#define PASCAL_TYPE_PREDISCRIMINATED_STRING(type)                   \
    (PASCAL_TYPE_STRING (type)                                      \
     && (! TYPE_SIZE (type)                                         \
         || (TREE_CODE (TYPE_SIZE (type)) != INTEGER_CST            \
             && TREE_CODE (TYPE_SIZE (type)) != SAVE_EXPR)))
#endif

/* Determine whether some TYPE is a discriminated string.
 */
#define PASCAL_TYPE_DISCRIMINATED_STRING(type)                      \
    (PASCAL_TYPE_STRING (type)                                      \
     && ! PASCAL_TYPE_UNDISCRIMINATED_STRING (type)                 \
     && ! PASCAL_TYPE_PREDISCRIMINATED_STRING (type))               \

/* Return the pascal string value of DECL.
 */
#define PASCAL_STRING_VALUE(decl) (PASCAL_TYPE_STRING (TREE_TYPE (decl)) ? \
                                   build_component_ref (decl, string_id) : \
                                   decl)

/* Each variable length string has a "current length" field,
 * CHAR_TYPE length is always 1,
 * fixed-length-string length is the domain max value.
 */
#define PASCAL_STRING_LENGTH(decl)                                        \
         (PASCAL_TYPE_STRING (TREE_TYPE(decl))                  ?         \
          build_component_ref (decl, get_identifier ("length")) :         \
          ((TREE_CODE (TREE_TYPE(decl)) == CHAR_TYPE)   ?                 \
           integer_one_node                             :                 \
           ((TREE_CODE (decl) == STRING_CST)                    ?         \
            build_int_2 (TREE_STRING_LENGTH (decl) - 1, 0)      :         \
            fold (build_binary_op (PLUS_EXPR,                             \
                    build_binary_op (MINUS_EXPR,                          \
                      TYPE_MAX_VALUE (TYPE_DOMAIN (TREE_TYPE (decl))),    \
                     TYPE_MIN_VALUE (TYPE_DOMAIN (TREE_TYPE (decl))), 0), \
               integer_one_node, 0)))))

/* Each variable length string has a "Capacity" field.
 */
#define PASCAL_STRING_CAPACITY(decl) build_component_ref (decl, get_identifier ("Capacity"))

/* Determine whether some TYPE is an undiscriminated schema.
 */
#define PASCAL_TYPE_UNDISCRIMINATED_SCHEMA(type)                                 \
    (PASCAL_TYPE_SCHEMA (type)                                                   \
     && TYPE_FIELDS (type)                                                       \
     && (PASCAL_DECL_FIXUPLIST (TYPE_FIELDS (type)) == NULL_TREE                 \
         || (TREE_CODE (PASCAL_DECL_FIXUPLIST (TYPE_FIELDS (type))) == TREE_LIST \
             && (TYPE_LANG_BASE (type) == NULL_TREE                              \
                 || TYPE_LANG_BASE (type) == type))))

/* Determine whether some TYPE is a prediscriminated schema.
 */
#define PASCAL_TYPE_PREDISCRIMINATED_SCHEMA(type)            \
    (PASCAL_TYPE_SCHEMA (type)                               \
     && TYPE_FIELDS (type)                                   \
     && PASCAL_DECL_FIXUPLIST (TYPE_FIELDS (type))           \
     && TREE_CODE (PASCAL_DECL_FIXUPLIST (TYPE_FIELDS (type))) == TREE_LIST \
     && TYPE_LANG_BASE (type) != NULL_TREE                   \
     && TYPE_LANG_BASE (type) != type)

/* Determine whether some TYPE is a discriminated schema.
 */
#define PASCAL_TYPE_DISCRIMINATED_SCHEMA(type)               \
    (PASCAL_TYPE_SCHEMA (type)                               \
     && TYPE_FIELDS (type)                                   \
     && PASCAL_DECL_FIXUPLIST (TYPE_FIELDS (type))           \
     && TREE_CODE (PASCAL_DECL_FIXUPLIST (TYPE_FIELDS (type))) != TREE_LIST)

/* TREE_LANG_FLAG_0 through 2 are used by C.
 * @@@ Check whether we really use those C constructs.
 */

/* Nonzero if the PARM_DECL is a value parameter passed by reference.
 * This is necessary for Strings and other Schema types when passed as
 * value args without specified capacity.
 * Only used in *_TYPE nodes.
 */
#define PASCAL_TYPE_VAL_REF_PARM(type)  TREE_LANG_FLAG_3 (type)

/* Flag for Schema discriminants.
 * Used in VAR_DECL, FIELD_DECL, and CONVERT_EXPR nodes.
 */
#define PASCAL_TREE_DISCRIMINANT(decl)  TREE_LANG_FLAG_3 (decl)

/* Borland Pascal "open array" parameter.
 * (I would prefer to use a TYPE_LANG_FLAG here, but they are all busy.)
 * Only used in ARRAY_TYPE and INTEGER_TYPE nodes.
 */
#define PASCAL_TYPE_OPEN_ARRAY(type)    TREE_LANG_FLAG_4 (type)

/* In a method field: Nonzero indicates that the method
 * is a constructor or destructor.
 * Used in FIELD_DECL, FUNCTION_DECL, and CALL_EXPR nodes.
 */
#define PASCAL_STRUCTOR_METHOD(decl)    TREE_LANG_FLAG_4 (decl)

/* Identify constructor and destructor methods as such.
 * This only checks for function declarations, not for
 * field declarations and call expressions.
 */
#define PASCAL_CONSTRUCTOR_METHOD(decl)                 \
    (PASCAL_STRUCTOR_METHOD (decl)                      \
     && TREE_CODE (decl) == FUNCTION_DECL               \
     && TREE_TYPE (decl)                                \
     && TREE_TYPE (TREE_TYPE (decl)) == boolean_type_node)
#define PASCAL_DESTRUCTOR_METHOD(decl)                  \
    (PASCAL_STRUCTOR_METHOD ((decl))                    \
     && TREE_CODE (decl) == FUNCTION_DECL               \
     && TREE_TYPE (decl)                                \
     && TREE_TYPE (TREE_TYPE (decl)) == void_type_node)

/* Flag for packed array access constructs.
 * Only used in *_EXPR and VAR_DECL nodes.
 */
#define PASCAL_TREE_PACKED_ACCESS(expr) TREE_LANG_FLAG_5 (expr)

/* Flag for fresh integer constants.
 * Only used in INT_CST nodes.
 */
#define PASCAL_TREE_FRESH_INT_CST(expr) TREE_LANG_FLAG_5 (expr)

/* In a method field (FUNCTION_DECL record field): Nonzero indicates that
 * the method is virtual.
 */
#define PASCAL_VIRTUAL_METHOD(decl)     TREE_LANG_FLAG_5 (decl)

/* This flag distinguishes a variable initial value from an
 * `absolute' clause (in Borland sense).  Nonzero -> absolute clause.
 */
#define PASCAL_ABSOLUTE_CLAUSE(init)    TREE_LANG_FLAG_6 (init)

/* This object is `external' in Pascal sense; it has been
 * mentioned in the program heading. In Extended Pascal it does
 * not have to be a file.
 *
 * Use one bit of the language dependend common bits in the
 * tree structure.
 */
#define PASCAL_EXTERNAL_OBJECT(object)  TREE_LANG_FLAG_6 (object)

/* DECL_LANG_FLAG_0 through 4 are used by C.
 * @@@ Check whether we really use those C constructs.
 */

/* Nonzero if the PARM_DECL is a standard pascal procedure parameter.
 *
 * If it is a non-standard function pointer parameter, this is not set,
 * in which case the procedure is not automatically called when
 * referenced.
 */
#define PASCAL_PROCEDURE_PARAMETER(decl) DECL_LANG_FLAG_5 (decl)

/* When a VAR_DECL node is directly assigned to we mark it.
 * (Currently only used to find out if a value is assigned to
 * a function via it's name or a user defined return value name)
 */
#define PASCAL_VALUE_ASSIGNED(decl)     DECL_LANG_FLAG_6 (decl)

/* Set this if NODE refers to a known identifier in Pascal.
 * It's used in gpc-lex.c yylex() to return correct tokens to the parser
 * when a Pascal predefined word is seen. Also it allows redefinition of
 * the known identifiers in pushdecl().
 */
#define PASCAL_REDEFINABLE_DECL(decl)   DECL_LANG_FLAG_7 (decl)

/* Set this when compiler is checking if the for loop should be
 * executed at all (just to prevent the warnings in limited
 * range comparisons). Reset the bit after comparison.
 */
#define PASCAL_LOOP_CHECK(decl) PASCAL_VALUE_ASSIGNED(decl)

/* Pascal type variants.  32 different types fit in one integer
 */
#define TYPE_QUALIFIER_PACKED        1
#define TYPE_QUALIFIER_PROTECTED     2
#define TYPE_QUALIFIER_QUALIFIED     4
#define TYPE_QUALIFIER_BINDABLE      8
#define TYPE_QUALIFIER_RESTRICTED   16
#define TYPE_QUALIFIER_CONFORMANT   32
#define TYPE_QUALIFIER_SHORT        64
#define TYPE_QUALIFIER_LONG        128
#define TYPE_QUALIFIER_LONGLONG    256
#define TYPE_QUALIFIER_UNSIGNED    512
#define TYPE_QUALIFIER_BYTE       1024

#define TYPE_VARIANT (TYPE_QUALIFIER_BINDABLE           \
                      | TYPE_QUALIFIER_RESTRICTED)

#define TYPE_SELECT (TYPE_QUALIFIER_BYTE                \
                     | TYPE_QUALIFIER_SHORT             \
                     | TYPE_QUALIFIER_LONG              \
                     | TYPE_QUALIFIER_LONGLONG          \
                     | TYPE_QUALIFIER_UNSIGNED)

/* Determine if something may be a function call.
 */
#define MAYBE_CALL_FUNCTION(target) (TREE_CODE (target) == FUNCTION_DECL      \
    || (TREE_TYPE (target)                                                    \
        && (TREE_CODE (TREE_TYPE (target)) == REFERENCE_TYPE                  \
            || TREE_CODE (TREE_TYPE (target)) == POINTER_TYPE)                \
        && TREE_CODE (TREE_TYPE (TREE_TYPE (target))) == FUNCTION_TYPE))

/* Determine if something is a method call.
 */
#define CALL_METHOD(target) (TREE_CODE (target) == COMPONENT_REF             \
                         && TREE_CODE (TREE_TYPE (target)) == FUNCTION_TYPE)

/* Determine if something is a procedural variable.
 */
#define PASCAL_PROCEDURAL_VARIABLE(target)                               \
    (target                                                              \
     && TREE_TYPE (target)                                               \
     && TREE_CODE (TREE_TYPE (target)) == REFERENCE_TYPE                 \
     && TREE_CODE (TREE_TYPE (TREE_TYPE (target))) == FUNCTION_TYPE)

/* In a VAR_DECL node: check if it represents an ABSOLUTE
 * declared variable (in Borland sense)
 */
#define IS_ABSOLUTE_VAR(decl) (TREE_CODE (decl) == VAR_DECL                  \
                           && DECL_INITIAL (decl)                            \
                           && PASCAL_ABSOLUTE_CLAUSE (DECL_INITIAL (decl)))

/* ------------------------------------------------------------------ */

/* These should be in parse.h ??? */

extern int this_is_an_interface_module;

/* A list of types already defined in this type_declaration_part */
extern tree current_type_list;

/* This is TRUE if we are in 'TYPE' declaration part.
 * It is written by the parser,
 * checked in build_pascal_pointer_type().
 */
extern int defining_types;

extern tree main_program_name;



/* ------------------------------------------------------------------ */

/* These should be in gpc-common.h */

extern tree asm_combine_strings PROTO ((tree));
extern tree pascal_type_variant PROTO ((tree, int, int));
extern tree pascal_type_extension PROTO ((tree, int));
extern tree build_pascal_unary_op PROTO ((enum tree_code, tree, int));
extern tree build_pascal_pointer_reference PROTO ((tree));
extern tree build_pascal_address_expression PROTO ((tree));
extern void record_function_format PROTO ((tree, tree, int, int, int));

/*
 * construct an identifier_node for the assembler-name
 * of a user-defined operator
 */
extern tree get_operator_identifier PROTO ((char*, tree, tree));

/* convert set operations to their bitwise form;
 * handle user-defined operators;
 * for others just call build_binary_op_nodefault.
 */
extern tree build_pascal_binary_op PROTO ((enum tree_code, tree, tree));

/* convert PXSC operators to function calls
 */
extern tree build_pxsc_operator PROTO ((char*, tree, tree));

/* Make a variant type in the proper way for C/C++
 */
extern tree build_c_type_variant PROTO ((tree, int, int));

/* ------------------------------------------------------------------ */

/* These should be in gpc-decl.h */

extern tree current_function_return_variable;

#if 0 /* unused */
extern int current_function_assigned_value;
#endif
extern int warn_write_strings;

extern tree ptr_type_node;

extern tree boolean_false_node;
extern tree boolean_true_node;
extern tree complex_zero_node;
extern tree real_zero_node;
extern tree real_half_node;
extern tree null_pointer_node;
extern tree empty_set_node;

extern tree ptrsize_unsigned_type_node;
extern tree ptrsize_integer_type_node;
extern tree packed_array_unsigned_long_type_node;
extern tree packed_array_unsigned_short_type_node;

/* node for marking virtual methods while parsing */
extern tree virtual_mark_node;

/* identifier node for implicit `Self' formal parameter */
extern tree self_name_node;

/* Extended Pascal nodes */
extern tree real_max_node;
extern tree real_min_node;
extern tree real_eps_node;
extern tree char_max_node;

/* Other type nodes */
extern tree boolean_type_node;
extern tree byte_boolean_type_node;
extern tree short_boolean_type_node;
extern tree word_boolean_type_node;
extern tree long_boolean_type_node;
extern tree long_long_boolean_type_node;

/* directives */
extern tree d_forward;
extern tree d_external;
extern tree d_extern;
extern tree d_c_lang;
extern tree d_c_lang1;
extern tree d_asmname;
extern tree d_o_override;
extern tree d_static;

/* To allow build_enumerator() to set up the correct type for
 * enumerated type elements (In C they are all ints).
 * Think about [ enumtype ] set constructor.
 */
extern tree current_enum_type;

/* A place to look up the name of a currently declared type
 * (needed to define assembler-names for object methods).
 */
extern tree current_type_name;

/* Labels declared at the current level.
 */
extern tree declared_labels;

/* Must a function get an implicit "Self" parameter or not?
 */
extern int defining_methods;

/* For giving the correct --pedantic warning only ...
 */
extern int defining_pointers;
extern int defining_operators;

/* Build a FUNCTION_DECL node to define the DECL_CONTEXT of the
 * main program before start_function() is called to make non-local
 * gotos to main program work.
 */
extern tree declare_main_program_context PROTO((tree,tree));

extern struct lang_type *allocate_type_lang_specific PROTO ((void));
extern struct lang_decl *allocate_decl_lang_specific PROTO ((void));

extern int pascal_global_bindings_p PROTO ((void));
extern int top_level_p PROTO ((int));

extern tree grok_directive PROTO ((tree, tree, tree, int));

/* Nonzero means to do range checking on `readln' and such.
 */
extern int flag_input_range_checking;

/* Nonzero means to check for IOresult after each I/O operation.
 */
extern int flag_io_checking;

/* Nonzero means always use short circuit boolean `and' and `or'.
 */
extern int flag_short_circuit;

/* Level of the language we recognize.
 */
extern unsigned long flag_what_pascal;

/* Nonzero means don't warn about "extended syntax".
 */
extern int flag_extended_syntax;

/* Nonzero means print GPI debugging information to stderr.
 */
extern int flag_debug_gpi;

/* Nonzero means to dump source to stderr.
 */
extern int flag_debug_source;

/* If non-null, it gives the identifier of a procedure to be called
   automatically before each statement.
 */
extern tree debug_statement;

/* Nonzero means to output progress report messages.
 */
extern int flag_progress_messages;

/* Nonzero means to output progress bar information.
 */
extern int flag_progress_bar;

/* Nonzero means allow mixed comments;
 * then (* } and { *) are okay, according to ISO standard.
 */
extern int flag_mixed_comments;

/* Nonzero means allow nested comments;
 * then (* (* *) *) and { { } } are okay (optional GPC extension).
 */
extern int flag_nested_comments;

/* Nonzero means allow Delphi comments.
 */
extern int flag_delphi_comments;

/* Nonzero allows GPC to ignore function results without warning.
 */
extern int flag_ignore_function_results;

/* Nonzero means allow GPC to read Borland-style
 * `#' and `^' character constants.
 */
extern int flag_borland_char_constants;

/* Nonzero means to allow string clipping: "write ( 'foobar' : 3 )" --> "foo".
 */
extern int flag_clip_strings;

/* Option -ffield-widths:NN,NN,NN changes default field widths
 * `write' operations.
 */
extern int int_out_width;
extern int real_out_width;
extern int bool_out_width;
extern int long_int_out_width;
extern int long_real_out_width;

/* Nonzero means to allow string truncation: "writestr ( s3, 'foobar' )"
 * with `s3' having a capacity of 3 --> "foo".  Zero means to produce a
 * runtime error.
 */
extern int flag_truncate_strings;

/* Nonzero means to do exact (lexicographic) string comparisons with
 * operators like `<'.  Zero means to pad with spaces as ISO requires.
 */
extern int flag_exact_compare_strings;

/* Nonzero means to produce a blank in front of positive reals.
 * (Required by ISO.)
 */
extern int flag_real_blank;

/* Nonzero means to write exponents with a capital 'E'.
 * Zero means to write a lowercase 'e'.
 */
extern int flag_capital_exponent;

/* Nonzero means to derive the external file name from
 * the name of the file variable.
 */
extern int flag_transparent_file_names;

/* Nonzero means the result of the address is a typed pointer.
 */
extern int flag_typed_address;

extern tree main_program_context;

extern tree create_boolean_type_node PROTO ((unsigned));

extern tree get_variant_tag_info PROTO ((tree));
extern void store_variant_tag_info PROTO ((tree, tree));
extern int is_known_directive PROTO ((tree));

/* Handles directives, calls start_function and returns the function
 * return type or 0.
 */
extern tree start_pascal_function PROTO ((tree, tree, int));

extern tree de_capitalize PROTO ((tree));

extern tree integer_maxint_node;

extern void declare_label PROTO ((tree, int));
extern void handle_forward_pointers PROTO ((tree));
extern int pascal_shadow_record_fields PROTO ((tree, int));
extern void check_variant_record PROTO ((tree, tree));

#if 0 /* unused */
/* a chain of TREE_LIST nodes that contain the with_elements
 * of the currently active `with' statements
 */
extern tree with_element_chain;
#endif /* 0 */

/* ------------------------------------------------------------------ */

/* Pointer to token buffer. Actual allocated length is maxtoken + 2. */
extern char *token_buffer;

/* The declaration found for the last LEX_ID token read in. */
extern tree lastiddecl;

/*
  Determines what the lexer returns for `=':
  < 0 means: `='
  = 0 means: LEX_CONST_EQUAL
  > 0 means: `=', but after that many closing parentheses/brackets, return LEX_CONST_EQUAL
*/
extern int lex_const_equal;

/* Nonzero means: don't recognize BP style `^a' character constants.
 */
extern int lex_caret;

/* Warn about redeclaration of keywords. */
extern char *which_language PROTO ((int));
extern void warning_list_languages PROTO ((unsigned long, int));
extern void warn_about_keyword_redeclaration PROTO ((char*));

/* Enable/disable keywords in certain contexts.
 * With this strategy we can avoid shift/reduce
 * and/or reduce/reduce conflicts while keeping
 * all keywords redefinable.
 */
extern void enable_keyword PROTO ((char*));
extern void disable_keyword PROTO ((char*));
extern int keyword_enabled PROTO ((char*));

extern int resolve_forward_pointer PROTO ((tree, tree));
extern tree lookup_forward_pointer PROTO ((tree, int));

extern tree check_if_predefined_type PROTO ((tree));

extern void init_language_specification PROTO ((void));

extern int check_newline PROTO ((void));

extern int yylex PROTO ((void));

extern void reinit_parse_for_function PROTO ((void));

/* ------------------------------------------------------------------ */

/* These should be in gpc-typeck.h */

/* Nonzero if we warn about void type pointer dereferences.
 */
extern int warn_void_dereference;

/* For type checking during the body of a `case' statement is parsed.
 */
extern tree current_case_expression;

/* Label to exit a `case' statement, so `break' can exit a loop.
 */
extern tree current_case_exit_label;

extern tree bit_sizeof PROTO ((tree));
extern tree build_discriminated_schema_type PROTO ((tree, tree));
extern tree build_modify_expr PROTO ((tree, enum tree_code, tree));

extern tree convert_array_to_pointer PROTO ((tree));

/* Check whether an initializer matches a type */
extern int check_pascal_initializer PROTO ((tree, tree));

/* Locates from structure/union DATUM the field NAME, and consturucts
 * an expression to refer to that field
 */
extern tree find_field PROTO ((tree, tree));

/* ------------------------------------------------------------------ */

/* These should be in gpc-lex.h */

struct gpc_option
{
  int source;
  char *name;
};

extern struct gpc_option gpc_options[];

#endif /* _GPC_DEFS_H_ */
