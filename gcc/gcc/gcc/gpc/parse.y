/* A Bison parser for the programming language Pascal.
   Copyright (C) 1989, 1995, 1998-2000 Free Software Foundation, Inc.

This file is part of GNU Pascal.

GNU Pascal is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 1, or (at your option)
any later version.

GNU Pascal is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Pascal; see the file COPYING. If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA. */

/* Authors: Jukka Virtanen <jtv@hut.fi>
 *          Helsinki University of Technology
 *          Computing Centre
 *          Finland
 *
 *          Peter Gerwinski <peter@gerwinski.de>
 *          Essen, Germany
 *
 * Bison parser for ISO 7185 Pascal originally written on
 * 3 Feb 1987 by Jukka Virtanen <jtv@hut.fi>
 *
 * The parser also parses (most of) the ISO/IEC 10206 Extended Pascal language.
 * 7 Dec 1993 by Jukka Virtanen <jtv@hut.fi>
 *
 * The parser also parses (most of) the Borland Pascal extensions.
 * 7 Oct 1995 by Peter Gerwinski <peter@gerwinski.de>
 *
 * The dangling else will not cause a shift/reduce conflict - it's
 * solved by precedence rules.
 *
 * Acknowledgements:
 *
 *   Bill Cox <bill@cygnus.com> added the error recovery rules.
 *
 * Expect some shift/reduce conflicts, due to error reporting.
 * The error detection/recovery strategy (the placement of
 * `error' and `yyerrok' tokens) used here is that suggested in
 * "An Introduction to Compiler Construction with Unix", by Axel
 * Schreiner and H. George Friedman, chapter four.  Published by
 * Prentice-Hall in 1985.
 *
 * jtv: I disabled the error ("improperly handled ...") calls because
 *      they are too verbose and I think an error report is already given
 *      for all cases.  If not -> bug.
 *
 *
 * SHIFT/REDUCE CONFLICTS
 *
 * BP style typed constants and variables initialized with `=' would
 * normally cause a shift/reduce conflict. They crash with upper-bound
 * expressions of subranges. For an example, write
 *
 *     Const Foo: Boolean = False;
 * as
 *     Const Foo: False .. 1 = 1 = 2 = 7;
 *
 * This cannot be parsed (even BP can't do it). But consider the following:
 *
 *     Const Foo: False .. True = 2 = 7;
 * or
 *     Const Foo: False .. (1 = 1) = 2 = 7;
 *
 * This is resolved with a hack in the lexical analyzer: In this context,
 * the `=' immediately before the initializer is not lexed as `=' but as
 * the special token LEX_CONST_EQUAL.
 *
 * The 13 shift/reduce conflicts come from error recovery rules.
 * - PG
 */
%expect 13

%{
#include "gcc-version.h"
#include "config.h"
#include "gansidecl.h"
#ifdef EGCS
#include "system.h"
#endif

#include <stdio.h>
#include <ctype.h>
#include <errno.h>

#include "gbe.h"
#include "gpc-defs.h"
#include "module.h"
#include "objects.h"
#include "rts.h"
#include "types.h"
#include "util.h"

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifndef errno
extern int errno;
#endif

/* Cause the `yydebug' variable to be defined.  */
#define YYDEBUG 1

void yyerror PROTO ((char *));
void set_yydebug PROTO ((int));

/* Like YYERROR but do call yyerror */
#define YYERROR1 { yyerror ("syntax error"); YYERROR; }

void position_after_white_space ();

/* Since parsers are distinct for each language, put the language string
 * definition here.
 */
char *language_string = "GNU Pascal";

tree main_program_name = NULL_TREE;

tree unresolved_pointer_types = NULL_TREE;
%}

%start pascal_program

/* The union representing a semantic stack entry - this will become
 * the yyunion.inc file.
 */
%union {
    char *filename;
    long  itype;
    int   lineno;
    enum  tree_code code;
    tree  ttype;
}


%token LEX_ID

/*
 * Reserved words.
 */

/* Reserved words in Standard Pascal */
%token LEX_ARRAY LEX_BEGIN LEX_CASE LEX_CONST LEX_DO LEX_DOWNTO LEX_END
%token LEX_FILE LEX_FOR LEX_FUNCTION LEX_GOTO LEX_LABEL LEX_NIL LEX_OF
%token LEX_PACKED LEX_PROCEDURE LEX_PROGRAM LEX_RECORD LEX_REPEAT LEX_SET
%token LEX_THEN LEX_TO LEX_TYPE LEX_UNTIL LEX_VAR LEX_WHILE LEX_WITH

/* Additional reserved words for Extended Pascal */
%token LEX_AND_THEN LEX_BINDABLE LEX_EXPORT LEX_IMPORT LEX_MODULE
%token LEX_ONLY LEX_OR_ELSE LEX_OTHERWISE LEX_RESTRICTED
%token LEX_PROTECTED LEX_QUALIFIED LEX_VALUE

/* Additional reserved words for Object Pascal */
%token LEX_ABSTRACT LEX_CLASS LEX_CONSTRUCTOR LEX_DESTRUCTOR
%token LEX_INHERITED LEX_PROPERTY LEX_VIEW

/* Additional reserved word for UCSD Pascal */
%token LEX_EXTERNAL LEX_FORWARD LEX_SEGMENT

/* Additional reserved words for Borland Pascal */
%token LEX_EXPORTS BP_INDEX LEX_INTERRUPT LEX_LIBRARY LEX_OBJECT
%token LEX_PRIVATE LEX_PUBLIC LEX_PUBLISHED LEX_RESIDENT LEX_UNIT LEX_USES

/* Additional reserved words for Pascal-SC (PXSC) */
%token LEX_OPERATOR

/* Additional reserved words for GNU Pascal */
%token LEX_INLINE

%token LEX_ALL      /* Extended Pascal "export foo = all" extension */

%token LEX_RENAME   /* Extended Pascal token `=>' for export renaming */
%token LEX_SYMDIFF  /* Extended Pascal: Set symmetric difference (XOR) */

%token LEX_IMPLEMENTATION /* Extended Pascal module implementation directive */
                          /* or Borland Pascal unit implementation keyword */
%token LEX_INTERFACE      /* Extended Pascal module interface directive */
                          /* or Borland Pascal unit interface keyword */

%token LEX_FORWARD   /* Required directive Forward - reserved word in some dialects */
%token LEX_ATTRIBUTE /* function attributes (e.g. stack conventions) */
%token D_C           /* directive C */
%token D_C_LANGUAGE  /* directive C_LANGUAGE (same as D_C) */
%token D_EXTERN      /* directive Extern */
%token LEX_EXTERNAL  /* directive External - reserved word in some dialects */
%token LEX_ASMNAME   /* directive `asmname' (GNU extension) */
%token D_OVERRIDE    /* Object pascal directive */
%token D_REGISTER    /* directive Register (extension) */
%token LEX_STATIC    /* directive Static (extension) */
%token LEX_VOLATILE  /* directive Volatile (extension) */
%token LEX_ABSOLUTE  /* Borland Pascal `absolute' directive */
%token LEX_VIRTUAL   /* Borland Pascal `virtual' directive */
%token LEX_SEGMENT   /* UCSD Pascal `segment' reserved word */

/* The following ones are not tokens used in the parser.
 * However they are used in the same context as some tokens,
 * so assign unique numbers to them.
 */
%token pp_SIN pp_COS pp_EXP pp_LN pp_SQRT pp_ARCTAN rr_POW rr_EXPON
%token r_WRITE r_READ r_INITFDR r_LAZYTRYGET r_LAZYGET r_LAZYUNGET r_POW r_EXPON
%token z_ABS z_ARCTAN z_COS z_EXP z_LN z_SIN z_SQRT z_POW z_EXPON
%token bp_VAL_byteint_check
%token bp_VAL_shortint_check
%token bp_VAL_integer_check
%token bp_VAL_medint_check
%token bp_VAL_longint_check
%token bp_VAL_bytecard_check
%token bp_VAL_shortcard_check
%token bp_VAL_cardinal_check
%token bp_VAL_medcard_check
%token bp_VAL_longcard_check
%token bp_VAL_byteint_nocheck
%token bp_VAL_shortint_nocheck
%token bp_VAL_integer_nocheck
%token bp_VAL_medint_nocheck
%token bp_VAL_longint_nocheck
%token bp_VAL_bytecard_nocheck
%token bp_VAL_shortcard_nocheck
%token bp_VAL_cardinal_nocheck
%token bp_VAL_medcard_nocheck
%token bp_VAL_longcard_nocheck
%token bp_VAL_shortreal
%token bp_VAL_real
%token bp_VAL_longreal
%token set_card, set_equal, set_le, set_less, set_in, set_clear
%token set_include, set_exclude, set_include_range, set_copy
%token set_intersection, set_union, set_diff, set_symdiff
%token bp_UPCASE bp_LOCASE bp_RANDREAL
%token p_GOT p_DONEFDR gpc_IOCHECK gpc_RUNTIME_ERROR

/*
 * Redefinable identifiers.
 */

/* Redefinable identifiers in Standard Pascal */
%token p_INPUT p_OUTPUT p_REWRITE p_RESET p_PUT p_GET p_WRITE p_READ
%token p_WRITELN p_READLN p_PAGE p_NEW p_DISPOSE
%token p_ABS p_SQR p_SIN p_COS p_EXP p_LN p_SQRT p_ARCTAN
%token p_TRUNC p_ROUND p_PACK p_UNPACK p_ORD p_CHR p_SUCC p_PRED
%token p_ODD p_EOF p_EOLN
%token p_MAXINT p_TRUE p_FALSE

/* Additional redefinable identifiers for Extended Pascal */
%token p_EXTEND p_UPDATE p_SEEKWRITE p_SEEKREAD p_SEEKUPDATE
%token p_READSTR p_WRITESTR p_BIND p_UNBIND p_HALT p_GETTIMESTAMP
%token p_ARG p_RE p_IM p_CARD p_CMPLX p_POLAR
%token p_EMPTY p_POSITION p_LASTPOSITION
%token p_LENGTH p_SUBSTR p_TRIM
%token p_EQ p_LT p_GT p_NE p_LE p_GE
%token p_BINDING
%token p_DATE p_TIME
%token p_MAXCHAR p_MAXREAL p_MINREAL p_EPSREAL
%token p_INDEX

/* Additional redefinable identifiers for UCSD Pascal */
%token ucsd_FILLCHAR ucsd_MOVELEFT ucsd_MOVERIGHT
%token ucsd_BLOCKREAD ucsd_BLOCKWRITE
%token ucsd_CONCAT ucsd_STR ucsd_INSERT ucsd_DELETE ucsd_POS ucsd_COPY
%token ucsd_SEEK ucsd_INT ucsd_IORESULT
%token p_MARK p_RELEASE

/* Additional redefinable identifiers for Borland Pascal */
%token bp_ASSIGN bp_APPEND bp_FILEPOS bp_FILESIZE bp_TRUNCATE
%token bp_FLUSH bp_ERASE bp_RENAME bp_CHDIR bp_MKDIR bp_RMDIR bp_MOVE
%token bp_ADDR bp_ASSIGNED bp_GETMEM bp_FREEMEM bp_PARAMCOUNT bp_PARAMSTR
%token bp_INC bp_DEC gpc_UPCASE bp_NEAR bp_FAR bp_VAL bp_FRAC
%token bp_RUNERROR bp_FILEMODE bp_INOUTRES bp_PI bp_HIGH bp_LOW
%token bp_INCLUDE bp_EXCLUDE bp_RANDOM bp_RANDOMIZE bp_NAME
%token BREAK CONTINUE

/* Additional redefinable identifiers for Borland Delphi */
%token delphi_NULL

/* Additional redefinable identifiers for GNU Pascal */
%token gpc_SETLENGTH gpc_SETTYPE gpc_MIN gpc_MAX gpc_STDERR gpc_LOCASE
%token gpc_SETFILETIME gpc_INOUTRESSTR gpc_NEWCSTRING gpc_CSTRINGCOPYSTRING
%token gpc_CSTRING2STRING gpc_STRING2CSTRING
%token LEX_ASM

/* Additional redefinable identifiers for Object Pascal */
%token op_COPY op_NULL op_ROOT op_SELF op_TEXTWRITABLE

/* redefinable keyword extensions */
%token RETURN_ RESULT EXIT FAIL DEFAULT OTHERS p_CLOSE
%token SIZEOF BITSIZEOF ALIGNOF TYPEOF gpc_RETURNADDRESS gpc_FRAMEADDRESS
%token LEX_LABEL_ADDR CONJUGATE p_DEFINESIZE

/* GPC internal tokens */
%token LEX_INTCONST LEX_STRCONST CHAR_LITERAL LEX_REALCONST LEX_CONST_EQUAL

/* LEX_ELLIPSIS, "...", used for functions with variable arglists.  */
%token LEX_RANGE LEX_ELLIPSIS

/* For unimplemented stuff, never returned by the lex analyzer */
%token UNIMPLEMENTED

/* Precedence rules */

/* The precedence declarations for LEX_IF and LEX_ELSE are just to avoid
   the dangling else shift-reduce conflict */
%nonassoc LEX_IF
%nonassoc LEX_ELSE

%right        LEX_ASSIGN
%nonassoc     '<' '=' '>' LEX_IN LEX_NE LEX_GE LEX_LE
%left         '-' '+' LEX_OR LEX_CEIL_PLUS LEX_CEIL_MINUS LEX_FLOOR_PLUS LEX_FLOOR_MINUS
%left         '/' '*' LEX_DIV LEX_MOD LEX_AND LEX_SHL LEX_SHR LEX_XOR LEX_CEIL_MULT LEX_CEIL_DIV LEX_FLOOR_MULT LEX_FLOOR_DIV
%left <ttype> LEX_POW LEX_POWER LEX_IS
%left         LEX_NOT

/* These tokens are to avoid a shift-reduce conflict coming from BP
   style structured initializers enclosed in `(' ... `)' which crash
   with expressions in parentheses. The precendence should be higher
   than that of all the operators. Though it currently doesn't
   matter if it's lower, when at some later time a shift/reduce
   conflict arises, it would then be resolved with parentheses
   having a lower precedence than operators, and we don't want that,
   do we? (-; Well, at least not until the next dialect by Borland
   appears...) */
%nonassoc PREC_INITIALIZER
%nonassoc ')'

/* types for the nonterminals. */
%type <ttype>           LEX_ABSTRACT
%type <ttype>           LEX_ASMNAME
%type <ttype>           LEX_ATTRIBUTE
%type <ttype>           ALIGNOF
%type <ttype>           LEX_AND_THEN
%type <ttype>           LEX_ASM
%type <ttype>           BREAK
%type <ttype>           LEX_BINDABLE
%type <ttype>           ucsd_BLOCKREAD
%type <ttype>           ucsd_BLOCKWRITE
%type <ttype>           CHAR_LITERAL
%type <ttype>           D_C
%type <ttype>           D_C_LANGUAGE
%type <ttype>           LEX_CLASS
%type <ttype>           ucsd_CONCAT
%type <ttype>           CONJUGATE
%type <ttype>           CONTINUE
%type <ttype>           LEX_CONSTRUCTOR
%type <ttype>           DEFAULT
%type <ttype>           LEX_DESTRUCTOR
%type <ttype>           EXIT
%type <ttype>           LEX_EXPORT
%type <ttype>           D_EXTERN
%type <ttype>           LEX_EXTERNAL
%type <ttype>           FAIL
%type <ttype>           bp_FAR
%type <ttype>           ucsd_FILLCHAR
%type <ttype>           LEX_FORWARD
%type <ttype>           LEX_ID
%type <ttype>           LEX_IMPLEMENTATION
%type <ttype>           LEX_INTERFACE
%type <ttype>           LEX_INTERRUPT
%type <ttype>           bp_MOVE
%type <ttype>           ucsd_MOVELEFT
%type <ttype>           ucsd_MOVERIGHT
%type <ttype>           D_OVERRIDE
%type <ttype>           D_REGISTER
%type <ttype>           LEX_RESIDENT
%type <ttype>           LEX_SEGMENT
%type <ttype>           LEX_STATIC
%type <ttype>           LEX_IMPORT
%type <ttype>           LEX_INHERITED
%type <ttype>           LEX_INLINE
%type <ttype>           LEX_LIBRARY
%type <ttype>           LEX_MODULE
%type <ttype>           bp_NEAR
%type <ttype>           LEX_NIL
%type <ttype>           LEX_OBJECT
%type <ttype>           LEX_ONLY
%type <ttype>           LEX_OR_ELSE
%type <ttype>           OTHERS
%type <ttype>           LEX_OTHERWISE
%type <ttype>           LEX_RESTRICTED
%type <ttype>           LEX_OPERATOR
%type <ttype>           LEX_PROPERTY
%type <ttype>           LEX_PROTECTED
%type <ttype>           LEX_QUALIFIED
%type <ttype>           RETURN_
%type <ttype>           RESULT
%type <ttype>           gpc_SETLENGTH
%type <ttype>           gpc_SETTYPE
%type <ttype>           SIZEOF
%type <ttype>           BITSIZEOF
%type <ttype>           LEX_STRCONST
%type <ttype>           TYPEOF
%type <ttype>           gpc_RETURNADDRESS
%type <ttype>           gpc_FRAMEADDRESS
%type <ttype>           LEX_UNIT
%type <ttype>           LEX_INTCONST
%type <ttype>           LEX_REALCONST
%type <ttype>           LEX_USES
%type <ttype>           LEX_VALUE
%type <ttype>           LEX_VIEW
%type <ttype>           absolute_or_value_specification
%type <ttype>           actual_parameter
%type <ttype>           actual_parameter_list
%type <code>            adding_operator
%type <ttype>           address_operator
%type <ttype>           and_then
%type <ttype>           any_term
%type <ttype>           any_word
%type <ttype>           array_index_list
%type <ttype>           array_type
%type <ttype>           asm_clobbers
%type <ttype>           asm_operand
%type <ttype>           asm_operands
%type <itype>           asm_qualifier
%type <ttype>           attrib
%type <ttype>           attribute_list
%type <ttype>           boolean_expression
%type <ttype>           case_constant_list
%type <ttype>           case_default
%type <ttype>           conformant_array_schema
%type <ttype>           constant
%type <ttype>           constant_literal
%type <ttype>           direct_access_index_type
%type <ttype>           directive
%type <ttype>           directive_list
%type <ttype>           directive_or_identifier
%type <ttype>           discriminant_expression_list
%type <ttype>           discriminant_specification
%type <ttype>           discriminant_specification_list
%type <itype>           enable_caret
%type <ttype>           enum_list
%type <ttype>           enumerated_type
%type <ttype>           enumerator
%type <ttype>           export_list_or_all
%type <ttype>           export_list
%type <ttype>           export_list_item
%type <ttype>           expression
%type <ttype>           factor
%type <ttype>           signed_factor
%type <ttype>           file_type
%type <ttype>           fixed_part
%type <code>            for_direction
%type <ttype>           function_heading
%type <ttype>           function_identifier
%type <ttype>           functiontype
%type <ttype>           gpc_STDERR
%type <ttype>           id_list
%type <ttype>           identifier
%type <ttype>           import_clause
%type <ttype>           import_clause_list
%type <ttype>           index_expression_list
%type <ttype>           index_type_specification
%type <ttype>           index_type_specification_list
%type <ttype>           initializer_expression
%type <ttype>           iso_initializer_expression
%type <ttype>           borland_initializer_expression
%type <ttype>           iso_initializer_list
%type <ttype>           borland_initializer_list
%type <ttype>           structured_iso_initializer
%type <ttype>           structured_borland_initializer
/**** unimplemented ISO initializers ****\
%type <ttype>           array_value
%type <ttype>           array_value_element
%type <ttype>           array_value_element_list
%type <ttype>           array_value_initializer
%type <ttype>           optional_array_value_completer
%type <ttype>           component_value
%type <ttype>           field_identifier
%type <ttype>           field_identifier_list
%type <ttype>           field_list_value
%type <ttype>           field_value
%type <ttype>           field_value_list
%type <ttype>           fixed_part_value
%type <ttype>           optional_tag_field_identifier
%type <ttype>           optional_variant_part_value
%type <ttype>           record_value
%type <ttype>           structured_value_constructor
%type <ttype>           structured_value
%type <ttype>           variant_part_value
\**** end of ISO initializers ****/
%type <ttype>           label
%type <ttype>           member_designator
%type <ttype>           method_identifier
%type <code>            multiplying_operator
%type <ttype>           new_identifier
%type <ttype>           new_identifier_1
%type <ttype>           new_ordinal_type
%type <ttype>           new_pointer_type
%type <ttype>           new_procedural_type
%type <ttype>           new_structured_type
%type <ttype>           nonnull_asm_operands
%type <ttype>           number
%type <ttype>           object_field_list
%type <ttype>           object_fixed_part
%type <ttype>           object_method_heading
%type <ttype>           object_section
%type <ttype>           object_type
%type <ttype>           one_case_constant
%type <ttype>           open_array
%type <itype>           optional_const
%type <ttype>           operator_identifier
%type <ttype>           optional_access_qualifier
%type <itype>           optional_type_qualifiers
%type <ttype>           optional_directive_list
%type <ttype>           optional_module_parameters
%type <ttype>           optional_import_qualifier
%type <ttype>           optional_inline
%type <ttype>           optional_par_actual_parameter
%type <ttype>           optional_par_actual_parameter_list
%type <ttype>           optional_par_formal_parameter_list
%type <ttype>           optional_par_id_list
%type <ttype>           optional_par_write_parameter_list
%type <ttype>           optional_program_heading
%type <itype>           optional_protected
%type <ttype>           optional_rename
%type <ttype>           optional_retval_def
%type <ttype>           optional_actual_schema_discriminants
%type <ttype>           optional_formal_schema_discriminants
%type <ttype>           optional_unit_filename
%type <ttype>           op_COPY
%type <ttype>           op_NULL
%type <ttype>           op_ROOT
%type <ttype>           op_SELF
%type <ttype>           op_TEXTWRITABLE
%type <ttype>           ordinal_index_type
%type <ttype>           or_else
%type <ttype>           p_ABS
%type <ttype>           bp_ADDR
%type <ttype>           bp_APPEND
%type <ttype>           p_ARCTAN
%type <ttype>           p_ARG
%type <ttype>           bp_ASSIGN
%type <ttype>           bp_ASSIGNED
%type <ttype>           p_BIND
%type <ttype>           p_BINDING
%type <ttype>           p_CARD
%type <ttype>           bp_CHDIR
%type <ttype>           p_CHR
%type <ttype>           p_CLOSE
%type <ttype>           p_CMPLX
%type <ttype>           p_COS
%type <ttype>           ucsd_COPY
%type <ttype>           gpc_CSTRING2STRING
%type <ttype>           p_DATE
%type <ttype>           bp_DEC
%type <ttype>           p_DEFINESIZE
%type <ttype>           ucsd_DELETE
%type <ttype>           p_DISPOSE
%type <ttype>           p_EMPTY
%type <ttype>           p_EOF
%type <ttype>           p_EOLN
%type <ttype>           p_EPSREAL
%type <ttype>           p_EQ
%type <ttype>           bp_ERASE
%type <ttype>           bp_EXCLUDE
%type <ttype>           p_EXP
%type <ttype>           p_EXTEND
%type <ttype>           p_FALSE
%type <ttype>           bp_FILEMODE
%type <ttype>           bp_FILEPOS
%type <ttype>           bp_FILESIZE
%type <ttype>           bp_FLUSH
%type <ttype>           bp_FRAC
%type <ttype>           bp_FREEMEM
%type <ttype>           p_GE
%type <ttype>           p_GET
%type <ttype>           bp_GETMEM
%type <ttype>           p_GETTIMESTAMP
%type <ttype>           p_GT
%type <ttype>           p_HALT
%type <ttype>           bp_HIGH
%type <ttype>           p_IM
%type <ttype>           bp_INC
%type <ttype>           bp_INCLUDE
%type <ttype>           p_INDEX
%type <ttype>           bp_INOUTRES
%type <ttype>           gpc_INOUTRESSTR
%type <ttype>           p_INPUT
%type <ttype>           ucsd_INSERT
%type <ttype>           ucsd_INT
%type <ttype>           ucsd_IORESULT
%type <ttype>           p_LASTPOSITION
%type <ttype>           p_LE
%type <ttype>           p_LENGTH
%type <ttype>           p_LN
%type <ttype>           gpc_LOCASE
%type <ttype>           bp_LOW
%type <ttype>           p_LT
%type <ttype>           p_MARK
%type <ttype>           gpc_MAX
%type <ttype>           p_MAXCHAR
%type <ttype>           p_MAXINT
%type <ttype>           p_MAXREAL
%type <ttype>           gpc_MIN
%type <ttype>           p_MINREAL
%type <ttype>           bp_MKDIR
%type <ttype>           p_NE
%type <ttype>           p_NEW
%type <ttype>           gpc_NEWCSTRING
%type <ttype>           delphi_NULL
%type <ttype>           p_ODD
%type <ttype>           p_ORD
%type <ttype>           p_OUTPUT
%type <ttype>           p_PACK
%type <ttype>           p_PAGE
%type <ttype>           bp_PARAMCOUNT
%type <ttype>           bp_PARAMSTR
%type <ttype>           bp_PI
%type <ttype>           p_POLAR
%type <ttype>           ucsd_POS
%type <ttype>           p_POSITION
%type <ttype>           p_PRED
%type <ttype>           p_PUT
%type <ttype>           bp_RANDOM
%type <ttype>           bp_RANDOMIZE
%type <ttype>           bp_NAME
%type <ttype>           p_RE
%type <ttype>           p_READ
%type <ttype>           p_READLN
%type <ttype>           p_READSTR
%type <ttype>           p_RELEASE
%type <ttype>           bp_RENAME
%type <ttype>           p_RESET
%type <ttype>           p_REWRITE
%type <ttype>           bp_RMDIR
%type <ttype>           p_ROUND
%type <ttype>           bp_RUNERROR
%type <ttype>           ucsd_SEEK
%type <ttype>           p_SEEKREAD
%type <ttype>           p_SEEKUPDATE
%type <ttype>           p_SEEKWRITE
%type <ttype>           gpc_SETFILETIME
%type <ttype>           p_SIN
%type <ttype>           p_SQR
%type <ttype>           p_SQRT
%type <ttype>           ucsd_STR
%type <ttype>           gpc_CSTRINGCOPYSTRING
%type <ttype>           gpc_STRING2CSTRING
%type <ttype>           p_SUBSTR
%type <ttype>           p_SUCC
%type <ttype>           p_TIME
%type <ttype>           p_TRIM
%type <ttype>           p_TRUE
%type <ttype>           p_TRUNC
%type <ttype>           bp_TRUNCATE
%type <ttype>           p_UNBIND
%type <ttype>           p_UNPACK
%type <ttype>           gpc_UPCASE
%type <ttype>           p_UPDATE
%type <ttype>           bp_VAL
%type <ttype>           p_WRITE
%type <ttype>           p_WRITELN
%type <ttype>           p_WRITESTR
%type <ttype>           packed_conformant_array_schema
%type <ttype>           par_id_list
%type <ttype>           parameter_form
%type <ttype>           parmlist1
%type <ttype>           parmlist2
%type <ttype>           pointer_domain_type
%type <ttype>           predefined_literal
%type <ttype>           primary
%type <ttype>           procedural_type_formal_parameter
%type <ttype>           procedural_type_formal_parameter_list
%type <ttype>           optional_procedural_type_formal_parameter_list
%type <ttype>           program_heading
%type <filename>        pxsc_adding_operator
%type <filename>        pxsc_multiplying_operator
%type <ttype>           record_field_list
%type <ttype>           record_section
%type <ttype>           record_type
%type <code>            relational_operator
%type <ttype>           rest_of_export_item
%type <ttype>           rest_of_statement
%type <ttype>           rest_of_variant
%type <itype>           rts_fun_onepar
%type <itype>           rts_fun_optpar
%type <itype>           rts_fun_parlist
%type <itype>           rts_fun_twopar
%type <itype>           rts_fun_zeropar
%type <itype>           rts_proc_onepar
%type <itype>           rts_proc_parlist
%type <itype>           rts_proc_zeropar
%type <filename>        save_filename
%type <lineno>          save_lineno
%type <ttype>           set_constructor
%type <ttype>           set_constructor_element_list
%type <ttype>           set_type
%type <itype>           setspecs
%type <code>            sign
%type <ttype>           simple_expression
%type <ttype>           standard_functions
%type <ttype>           static_expression
%type <itype>           optional_storage_qualifier_list
%type <itype>           storage_qualifier_list
%type <itype>           storage_qualifier
%type <ttype>           optional_variable_directive_list
%type <ttype>           variable_directive_list
%type <ttype>           variable_directive
%type <ttype>           string
%type <ttype>           string_constant
%type <itype>           structured_variable
%type <itype>           structured_variable_list
%type <ttype>           subrange_type
%type <ttype>           term
%type <ttype>           type_denoter
%type <ttype>           type_denoter_1
%type <ttype>           type_inquiry
%type <ttype>           typename
%type <ttype>           typename_or_conformant_array_schema
%type <ttype>           unpacked_conformant_array_schema
%type <ttype>           unpacked_structured_type
%type <ttype>           unsigned_number
%type <ttype>           variable_access_or_typename
%type <ttype>           variable_or_function_access
%type <ttype>           variable_or_function_access_maybe_assignment
%type <ttype>           variable_or_function_access_no_id
%type <ttype>           variant
%type <ttype>           variant_list
%type <ttype>           variant_part
%type <ttype>           variant_selector
%type <ttype>           variant_type
%type <ttype>           write_actual_parameter
%type <ttype>           write_actual_parameter_list

%{

/* Number of statements (loosely speaking) seen so far.  */
static int stmt_count;

static char *if_stmt_file;
static int if_stmt_line;

tree check_set_bounds ();
tree current_type_list = NULL_TREE;

/* This is TRUE if we are in a `type' declaration part
 * It is written by the parser,
 * checked in build_pascal_pointer_type().
 */
int defining_types = FALSE;

/* Nonzero if the current module is an interface module */
int this_is_an_interface_module;

#define DUMMY_FILENAME "<implicit code>"
%}

%%

/*
 * Pascal parser starts here
 */

pascal_program:
    /* empty */
    {
      warning ("empty input file");
      finish_file ();
    }
  | program_component_list
    { finish_file (); }
  ;

/* Allow multiple modules in input file.
 * Only one LEX_PROGRAM is allowed per file.
 */
program_component_list:
    program_component
  | program_component_list program_component
  ;

program_component:
    main_program_declaration dot_or_error
  | module_declaration dot_or_error
  ;

dot_or_error:
    '.'
      {
        finish_program_component ();

        /* In BP mode, bump out.
         */
        if (flag_what_pascal & B_D_PASCAL)
          YYACCEPT;
      }
  | error
      {
        warning ("missing `.' at the end of program");
        finish_program_component ();

        /* In BP mode, bump out.
         */
        if (flag_what_pascal & B_D_PASCAL)
          YYACCEPT;
      }
  ;

main_program_declaration:
    optional_program_heading
      {
        tree parms;

        if (main_program_name)
          {
            error ("only one program declaration allowed");
            YYERROR1;
          }

        main_program_name = get_main_program_name (TREE_VALUE ($1));

        if (PEDANTIC (U_B_D_PASCAL)
            && TREE_PURPOSE ($1) == NULL_TREE)
          warning ("no program parameters specified");

        /* The main program is one of the modules */
        initialize_module (TREE_VALUE ($1), 1);
        current_module->parms = TREE_PURPOSE ($1);
        this_is_an_interface_module = 0;

        /* handle parameter list here */
        pushlevel (0);
        clear_parm_order ();
        declare_parm_level (1);
        parms = get_parm_info (1);
        poplevel (0, 0, 0);

        main_program_context = build_nt (CALL_EXPR, main_program_name,
                                         parms, NULL_TREE);

        pushlevel (0);

        current_module->parms = TREE_PURPOSE ($1);
        associate_external_objects (current_module->parms);
      }
    import_or_any_declaration_part
      {
        $<ttype>$ = getdecls ();
        push_label_level ();

        current_function_return_variable = NULL_TREE;

        associate_external_objects (current_module->parms);

        /* Now we know if program parameters are undefined.
         */
        check_external_objects (TREE_PURPOSE ($1));

        /* emit_line_note (input_filename, lineno); @@ fjf401. */

        if (! start_function (build_tree_list (NULL_TREE, void_type_node),
                              main_program_context,
                              NULL_TREE, NULL_TREE,
                              0))
          YYERROR1;

        store_parm_decls ();

        pushlevel_expand ();
        do_setjmp ();
      }
    statement_part
      {
        tree context, name, parm;
        #if 0
        int save_lineno;
        #endif
        char *save_filename;

        yyerrok;
        poplevel_expand ();

        /* Finish the statement part above.
         */
        pop_label_level ();
        poplevel (0, 0, 0);
        finish_function (0);
        declared_labels = NULL;

        #if 0
        /* Since the main program is created automatically, output a dummy
         * line number debugging information for it (if enabled), so it
         * does not appear to be after the main program in the source
         * file. This relies on the fact that the backend treats line 0
         * specially, i.e. does not output subsequent stubs within the
         * function which would irritate gdb when setting a breakpoint on
         * `main'.
         */
        save_lineno = lineno;
        lineno = 0;
        #endif
        save_filename = input_filename;
        input_filename = DUMMY_FILENAME;

        /* Initialize the variables of the main program
         * in a constructor.
         */
        name = get_identifier ("init_pascal_main_program");
        parm = no_parameters ();
        context = build_nt (CALL_EXPR, name, parm, NULL_TREE);
        if (! start_function (build_tree_list (NULL_TREE, void_type_node),
                              context, NULL_TREE, NULL_TREE, 0))
          YYERROR1;
        store_parm_decls ();
        pushlevel_expand ();
        start_constructor ();
        un_initialize_block ($<ttype>4, 0);
        finish_constructor ();
        poplevel_expand ();
        finish_function (0);

        /* Finalize the variables - including those imported
         * from Modules and Units - in a destructor.
         */
        name = get_identifier ("fini_pascal_main_program");
        parm = no_parameters ();
        context = build_nt (CALL_EXPR, name, parm, NULL_TREE);
        if (! start_function (build_tree_list (NULL_TREE, void_type_node),
                              context, NULL_TREE, NULL_TREE, 0))
          YYERROR1;
        store_parm_decls ();
        pushlevel_expand ();
        un_initialize_block ($<ttype>4, 1);
        poplevel_expand ();
        finish_function (0);

        /* Generate main() that calls the user main program.
         */
        output_real_main_program (main_program_name);

        #if 0
        lineno = save_lineno;
        #endif
        input_filename = save_filename;
      }
  ;

optional_program_heading:
    /* empty */
      {
        /* Borland Pascal would not even warn,
         * but I think we should.
         */
        if ((flag_what_pascal & B_D_PASCAL) == 0)
          warning ("missing program header");
        $$ = build_tree_list (NULL_TREE, get_identifier ("noname"));
        store_executable_name ();
      }
  | program_heading semi
      { store_executable_name (); }
  ;

program_heading:
    LEX_PROGRAM
      {
        disable_keyword ("Module");
        disable_keyword ("Unit");
        disable_keyword ("Library");
      }
    new_identifier optional_par_id_list
      { $$ = build_tree_list ($4, $3); }
  | LEX_PROGRAM
      {
        disable_keyword ("Module");
        disable_keyword ("Unit");
        disable_keyword ("Library");
      }
    error
      {
        error ("missing program name");
        yyerrok;
      }
    optional_par_id_list
      { $$ = build_tree_list ($5, get_identifier ("noname")); }
  ;

optional_par_id_list:
    /* empty */
      { $$ = NULL_TREE; }
  | par_id_list
  ;

par_id_list:
    '(' id_list r_paren_or_error
      { $$ = $2; }
  ;

id_list:
    new_identifier
      { $$ = build_tree_list (NULL_TREE, $1); }
  | id_list ',' new_identifier
      { $$ = chainon ($1, build_tree_list (NULL_TREE, $3)); }
  | id_list error new_identifier
      {
        error ("comma missing after identifier `%s'",
               IDENTIFIER_POINTER (TREE_VALUE($1)));
        $$ = chainon ($1, build_tree_list (NULL_TREE, $3));
        yyerrok;
      }
  | id_list ',' error
      {
        error ("extra comma following id_list");
        $$ = $1;
      }
  | id_list error
      {
        /* error ("improperly terminated id_list"); */
        $$ = $1;
      }
  ;

typename:
    LEX_ID
      {
        /* This happens when `value initializer' is appended to a
         * variable declaration:  The parser finds that out only
         * after reading the `value' and backtracks, but then
         * lastiddecl is gone already.  No big deal, recheck it now.
         */
        if (! lastiddecl)
          lastiddecl = check_if_predefined_type ($1);

        if (lastiddecl && TREE_CODE (lastiddecl) == TYPE_DECL)
          $$ = TREE_TYPE (lastiddecl);
        else if (defining_types && defining_methods)
          {
            /* Return the IDENTIFIER node as a forward reference.
             */
            $$ = $1;
          }
        else
          {
            error ("type name expected, identifier `%s' given",
                   IDENTIFIER_POINTER ($1));
            $$ = error_mark_node;
          }
      }
  ;

identifier:
    LEX_ID
      { $$ = check_identifier (lastiddecl, $1); }
  ;

new_identifier:
    new_identifier_1
      {
        $$ = $1;
        warn_about_keyword_redeclaration (IDENTIFIER_POINTER ($1));
      }
  ;

/* These standard identifiers are redefinable although some of them
 * are even reserved words.  See also gpc-util.c: KItable [].
 */
new_identifier_1:
    LEX_ID
/* Standard Pascal constants */
  | p_MAXINT
  | p_FALSE
  | p_TRUE
/* Standard Pascal I/O */
  | p_INPUT
  | p_OUTPUT
  | p_REWRITE
  | p_RESET
  | p_PUT
  | p_GET
  | p_WRITE
  | p_READ
  | p_WRITELN
  | p_READLN
  | p_PAGE
  | p_EOF
  | p_EOLN
/* Standard Pascal heap handling */
  | p_NEW
  | p_DISPOSE
/* Standard Pascal arithmetic */
  | p_ABS
  | p_SQR
  | p_SIN
  | p_COS
  | p_EXP
  | p_LN
  | p_SQRT
  | p_ARCTAN
  | p_TRUNC
  | p_ROUND
/* Standard Pascal transfer functions */
  | p_PACK
  | p_UNPACK
/* Standard Pascal ordinal functions */
  | p_ORD
  | p_CHR
  | p_SUCC
  | p_PRED
  | p_ODD
/* Extended Pascal constants */
  | p_MAXCHAR
  | p_MAXREAL
  | p_MINREAL
  | p_EPSREAL
/* Extended Pascal I/O */
  | p_EXTEND
  | p_UPDATE
  | p_SEEKWRITE
  | p_SEEKREAD
  | p_SEEKUPDATE
  | p_EMPTY
  | p_POSITION
  | p_LASTPOSITION
/* Extended Pascal external binding */
  | p_BIND
  | p_UNBIND
  | p_BINDING
/* Extended Pascal time routines */
  | p_GETTIMESTAMP
  | p_DATE
  | p_TIME
/* Extended Pascal String routines */
  | p_READSTR
  | p_WRITESTR
  | p_LENGTH
  | p_INDEX
  | p_SUBSTR
  | p_TRIM
  | p_EQ
  | p_LT
  | p_GT
  | p_NE
  | p_LE
  | p_GE
/* Extended Pascal Complex arithmetic */
  | p_RE
  | p_IM
  | p_CMPLX
  | p_ARG
  | p_POLAR
/* Extended Pascal miscellaneous routines */
  | p_HALT
  | p_CARD
/* Other Object Pascal extensions */
  | D_OVERRIDE      /* directive */
  | op_COPY         /* function */
  | op_NULL         /* constant */
  | op_ROOT         /* root class type */
  | op_TEXTWRITABLE /* predefined property class type */
  | op_SELF         /* implicit protected parameter */
/* UCSD Pascal extensions */
  | ucsd_FILLCHAR
  | ucsd_MOVELEFT
  | ucsd_MOVERIGHT
  | ucsd_BLOCKREAD
  | ucsd_BLOCKWRITE
  | ucsd_CONCAT
  | ucsd_STR
  | ucsd_INSERT
  | ucsd_DELETE
  | ucsd_POS
  | ucsd_COPY
  | ucsd_SEEK
  | ucsd_INT
  | ucsd_IORESULT
/* Borland Pascal extensions */
  | bp_MOVE
  | bp_ASSIGN
  | bp_APPEND
  | bp_FILEPOS
  | bp_FILESIZE
  | bp_TRUNCATE
  | bp_FLUSH
  | bp_ERASE
  | bp_RENAME
  | bp_CHDIR
  | bp_MKDIR
  | bp_RMDIR
  | bp_ADDR
  | bp_ASSIGNED
  | bp_GETMEM
  | bp_FREEMEM
  | bp_PARAMCOUNT
  | bp_PARAMSTR
  | bp_INC
  | bp_DEC
  | gpc_UPCASE
  | bp_NEAR
  | bp_FAR
  | bp_VAL
  | bp_FRAC
  | bp_RUNERROR
  | bp_FILEMODE
  | bp_INOUTRES
  | bp_PI
  | bp_HIGH
  | bp_LOW
  | bp_INCLUDE
  | bp_EXCLUDE
  | bp_RANDOM
  | bp_RANDOMIZE
  | bp_NAME
/* Borland Delphi extensions */
  | delphi_NULL
/* GPC directives */
  | D_C
  | D_C_LANGUAGE
  | D_EXTERN
  | D_REGISTER
/* Other extensions */
  | gpc_STDERR
  | ALIGNOF
  | gpc_SETTYPE
  | gpc_SETLENGTH
  | BREAK
  | CONTINUE
  | RETURN_
  | RESULT
  | EXIT
  | FAIL
  | SIZEOF
  | BITSIZEOF
  | TYPEOF
  | gpc_RETURNADDRESS
  | gpc_FRAMEADDRESS
  | gpc_MAX
  | gpc_MIN
  | gpc_LOCASE
  | gpc_SETFILETIME
  | gpc_INOUTRESSTR
  | CONJUGATE
  | p_MARK
  | p_RELEASE
/*
  | DEFAULT
  | OTHERS
*/
  | p_CLOSE
  | p_DEFINESIZE
  | gpc_NEWCSTRING
  | gpc_CSTRINGCOPYSTRING
  | gpc_CSTRING2STRING
  | gpc_STRING2CSTRING
  | LEX_STATIC
      { $$ = get_identifier ("Static"); }
  | LEX_VOLATILE
      { $$ = get_identifier ("Volatile"); }
  ;


import_or_any_declaration_part:
    LEX_IMPORT import_specification_list semi
      {
        do_extra_import ();
        if (PEDANTIC (E_O_PASCAL))
          pedwarn ("`import' is an ISO 10206 Extended Pascal extension");
      }
    any_declaration_part
  | LEX_USES uses_list semi
      {
        do_extra_import ();
        if (PEDANTIC (U_B_D_PASCAL))
          pedwarn ("`uses' is an UCSD Pascal extension");
      }
    any_declaration_part
  |
      { do_extra_import (); }
    any_declaration_part
  ;

any_declaration_part:
    /* empty */
  | any_decl_list
  ;

any_decl_list:
    any_decl_1
  | any_decl_list any_decl_1
  ;

any_decl_1:
    any_decl
  ;

any_decl:
    simple_decl
  | function_declaration
  ;

simple_decl:
    label_declaration_part
  | constant_definition_part
  | type_definition_part
      { defining_types = FALSE; }
  | variable_declaration_part
  ;


/* Label declaration part */

label_declaration_part:
    LEX_LABEL label_list semi
  | LEX_LABEL semi
      { error ("missing label declaration"); }
  ;

label_list:
    label
      { declare_label ($1, pascal_global_bindings_p ()); }
  | label_list ',' label
      {
        declare_label ($3, pascal_global_bindings_p ());
        yyerrok;
      }
  | error
      { error ("non-label in label_list"); }
  | label_list error label
      {
        error ("missing comma");
        declare_label ($3, pascal_global_bindings_p ());
        yyerrok;
      }
  | label_list ',' error
      { error ("extra comma"); }
  | label_list error
      { /* error ("improperly terminated label_list"); */ }
  ;

/*
 * Labels are returned as identifier nodes for compatibility with gcc
 */
label:
    LEX_INTCONST
      {
        char *ptr;
        if ((pedantic || flag_what_pascal) &&
            ((TREE_INT_CST_HIGH ($1) != 0) ||
             (TREE_INT_CST_LOW ($1) > 9999)))
          pedwarn ("ISO Pascal does not allow label values greater than 9999");
        for (ptr = token_buffer; *ptr == '0'; ptr++);
        if (*ptr == '\0')
          --ptr;
        $$ = get_identifier (ptr);
      }
  | new_identifier
      {
        if (PEDANTIC (B_D_PASCAL))
          pedwarn ("ISO Pascal does not allow non-numeric labels");
        $$ = $1;
      }
  ;


/* constant definition part */

constant_definition_part:
    LEX_CONST constant_definition_list
  | LEX_CONST semi
      { error ("missing constant definition"); }
  | LEX_CONST error semi
  ;

constant_definition_list:
    constant_definition
  | constant_definition_list constant_definition
  | error
  ;

constant_definition:
    new_identifier '=' static_expression semi
      {
        tree decl = build_decl (CONST_DECL, $1, TREE_TYPE ($3));
        DECL_INITIAL (decl) = $3;
        pushdecl (decl);
        if (this_is_an_interface_module)
          handle_autoexport ($1);
        rest_of_decl_compilation (decl, NULL_PTR,
                                  pascal_global_bindings_p (), 0);
      }
/* This would cause a shift/reduce conflict with '=' instead of LEX_CONST_EQUAL. */
  | new_identifier enable_caret
      {
        if (! flag_what_pascal || (flag_what_pascal & B_D_PASCAL))
          lex_const_equal = 0;
        enable_keyword ("Absolute");
        enable_keyword ("Value");
        enable_keyword ("External");
        enable_keyword ("Asmname");
        enable_keyword ("Static");
        enable_keyword ("Volatile");
        enable_keyword ("Attribute");
      }
    ':' optional_storage_qualifier_list type_denoter
      { lex_caret = $2; }
    LEX_CONST_EQUAL
      {
        lex_const_equal = -1;
        $<itype>$ = suspend_function_calls ();
      }
    initializer_expression semi
    optional_variable_directive_list
      {
        /* Borland Pascal allows such a "typed constant"
         * to be used as an initialized variable and does not
         * even warn if a value is written to it (because there
         * are no other initialized variables).
         *
         * To be compatible, we don't flag typed constants with
         * TYPE_READONLY but with a new flag PASCAL_TYPE_TYPEDCONST.
         * Like this, assignments will work, but we can warn
         * (not only if pedantic) and keep a clean conscience.
         *
         * When doing it like this, typed constants will always get
         * an address, which is often the purpose of a typed const
         * declaration.
         */
        unsigned qualifiers = $5;
        tree type = TREE_VALUE ($6);
        tree init = $10;
        tree directives = $12;
        tree attributes = NULL_TREE;
        char *asmname = NULL_PTR;
        tree d;

        for (d = directives; d; d = TREE_CHAIN (d))
          {
            if (TREE_VALUE (d) == extern_id)
              qualifiers |= VQ_EXTERNAL;
            else if (TREE_VALUE (d) == asmname_id)
              {
                if (TREE_PURPOSE (d)
                    && TREE_CODE (TREE_PURPOSE (d)) == STRING_CST)
                  asmname = TREE_STRING_POINTER (TREE_PURPOSE (d));
                else
                  abort ();
              }
            else if (TREE_VALUE (d) == attribute_id)
              attributes = TREE_VALUE (d);
          }

        if (type != error_mark_node)
          type = build_type_copy (type);

        resume_function_calls ($<itype>9);

        if (qualifiers && PEDANTIC (B_D_PASCAL))
          warning ("ISO Pascal does not allow type qualifiers in constants");

        if (! pascal_global_bindings_p ())
          qualifiers |= VQ_STATIC;

        if (!init)
          error ("constant must have specified value");
        PASCAL_TYPE_TYPEDCONST (type) = 1;
        declare_vars (build_tree_list (NULL_TREE, $1),
                      type, init, asmname, qualifiers,
                      this_is_an_interface_module, NULL_TREE);
        if (PEDANTIC (B_D_PASCAL))
          pedwarn ("typed constants are a Borland Pascal extension");
        if (this_is_an_interface_module)
          handle_autoexport ($1);
        disable_keyword ("Absolute");
        disable_keyword ("Value");
        disable_keyword ("External");
        disable_keyword ("Asmname");
        disable_keyword ("Static");
        disable_keyword ("Volatile");
        disable_keyword ("Attribute");
      }
  ;

enable_caret:
    /* empty */
      {
        $$ = lex_caret;
        lex_caret = 1;
      }
  ;

constant:
    identifier
  | sign identifier
      { $$ = build_pascal_unary_op ($1, $2, 0); }
  | number
  | constant_literal
  ;

number:
    sign unsigned_number
      { $$ = build_pascal_unary_op ($1, $2, 0); }
  | unsigned_number
  ;

unsigned_number:
    LEX_INTCONST
  | LEX_REALCONST
  | p_MAXINT
      { $$ = copy_node (integer_maxint_node); }
  | p_MAXREAL
      {
        if (PEDANTIC (E_O_PASCAL))
          pedwarn ("`MaxReal' is an ISO 10206 Extended Pascal extension");
        $$ = copy_node (real_max_node);
      }
  | p_MINREAL
      {
        if (PEDANTIC (E_O_PASCAL))
          pedwarn ("`MinReal' is an ISO 10206 Extended Pascal extension");
        $$ = copy_node (real_min_node);
      }
  | p_EPSREAL
      {
        if (PEDANTIC (E_O_PASCAL))
          pedwarn ("`EpsReal' is an ISO 10206 Extended Pascal extension");
        $$ = copy_node (real_eps_node);
      }
  | p_MAXCHAR
      {
        if (PEDANTIC (E_O_PASCAL))
          pedwarn ("`MaxChar' is an ISO 10206 Extended Pascal extension");
        $$ = copy_node (char_max_node);
      }
  ;

sign:
    '+'
      { $$ = CONVERT_EXPR; }
  | '-'
      { $$ = NEGATE_EXPR; }
  ;

constant_literal:
    string
      { $$ = combine_strings ($1); }
  | predefined_literal
  ;

predefined_literal:
    LEX_NIL
      { $$ = copy_node (null_pointer_node); }
  | delphi_NULL
      {
        if (PEDANTIC (BORLAND_DELPHI))
          pedwarn ("`Null' is a Delphi extension");
        $$ = build_indirect_ref (null_pointer_node, "NULL variable");
      }
  | p_FALSE
      { $$ = copy_node (boolean_false_node); }
  | p_TRUE
      { $$ = copy_node (boolean_true_node); }
  ;

string:
    string_constant
  | string string_constant
      { $$ = chainon ($1, $2); }
  ;

string_constant:
    LEX_STRCONST
  | CHAR_LITERAL
      {
        /* convert to a STRING_CST
         */
        char temp_str [2];
        temp_str [0] = TREE_INT_CST_LOW ($$);
        temp_str [1] = 0;
        $$ = build_string (2, temp_str);
        TREE_TYPE ($$) = char_array_type_node;
      }
  ;

/* type definition part */

type_definition_part:
    LEX_TYPE
      {
        enable_keyword ("Object");
        enable_keyword ("Class");
        /* To be implemented */
        enable_keyword ("Public");
        enable_keyword ("Published");
        enable_keyword ("Private");
        enable_keyword ("Protected");
        defining_types = TRUE;
        current_type_list = NULL_TREE;
      }
    type_definition_list semi
      {
        handle_forward_pointers (current_type_list);
        disable_keyword ("Object");
        disable_keyword ("Class");
        disable_keyword ("Public");
        disable_keyword ("Published");
        disable_keyword ("Private");
        disable_keyword ("Protected");
      }
  | LEX_TYPE semi
      { error ("missing type definition"); }
  ;

type_definition_list:
    type_definition
  | type_definition_list semi type_definition
      { yyerrok; }
  | error
  | type_definition_list error type_definition
      {
        error ("missing semicolon");
        yyerrok;
      }
  | type_definition_list semi error
      { error ("extra semicolon"); }
  | type_definition_list error
      { /* error ("improperly terminated type_definition_list"); */ }
  ;

/* $1 = IDENTIFIER_NODE
 * $2 = Schema discriminants
 * $5 = ..._TYPE node
 * $6 = initial value
 */
type_definition:
    new_identifier optional_formal_schema_discriminants enable_caret
      {
        current_type_name = $1;

        /* If this is a schema type, do not calculate the size yet.
         */
        $<itype>$ = immediate_size_expand;
        if ($2)
          immediate_size_expand = 0;
        size_volatile++;
      }
    '='
      {
        if (! flag_what_pascal || (flag_what_pascal & B_D_PASCAL))
          lex_const_equal = 0;
        enable_keyword ("Value");
      }
    type_denoter
      { lex_caret = $3; }
    absolute_or_value_specification
      {
        tree d, init = $9;
        tree type = TREE_VALUE ($7);

        disable_keyword ("Value");

        if ($2)
          {
            if (PASCAL_TYPE_OBJECT (type))
              error ("objects must not be schemata");
            else
              type = build_schema_type (type, $2);
          }
        else if ((pedantic || flag_what_pascal)
                 && PASCAL_TYPE_UNDISCRIMINATED_SCHEMA (type))
          {
            pedwarn ("ISO 10206 Extended Pascal does not allow undiscriminated");
            pedwarn (" schemata as the rhs of a type definition");
          }

        immediate_size_expand = $<itype>4;

        d = start_decl ($1, tree_cons (NULL_TREE, type,
                              build_tree_list (NULL_TREE, type_id)),
                        0, NULL_TREE, NULL_TREE);

        finish_decl (d, NULL_TREE, NULL_TREE);

        /* If value_specification is given, attach that to the
         * TYPE_DECL node just created.
         */
        DECL_INITIAL (d) = init;

        /* Pascal allows this:
         *
         *    type foo = ^foo;
         *
         * which creates a pointer to a new type.
         *
         * Avoid leaving this to the tags list as LANG_TYPE.
         *
         * @@ This code avoids some problems, but it is not correct
         *
         */
        if (TREE_CODE (type) == POINTER_TYPE
            && TREE_CODE (TREE_TYPE (type)) == LANG_TYPE
            && TYPE_NAME (TREE_TYPE (type)) == $1)
          {
            if (!resolve_forward_pointer
                      (d, build_pointer_type (integer_type_node)))
              abort ();
          }

        /* Chain all TYPE_DECL names in this type list
         * for resolving forward pointers later.
         */
        current_type_list
          = chainon (current_type_list,
                     build_tree_list (NULL_TREE, $1));
        if (this_is_an_interface_module)
          handle_autoexport ($1);
        lex_const_equal = -1;
        size_volatile--;
      }
  ;

optional_formal_schema_discriminants:
    /* empty */
      { $$ = NULL_TREE; }
  | '(' discriminant_specification_list ')'
      {
        tree discriminant;
        for (discriminant = $2; discriminant;
             discriminant = TREE_CHAIN (discriminant))
          {
            tree field = TREE_VALUE (discriminant);
            tree id = DECL_NAME (field);

            /* Make the formal discriminants denote "variables".
             * Store the previous meanings of these identifiers
             * in the TREE_PURPOSE fields of the id_list.
             */
            TREE_PURPOSE (discriminant) = IDENTIFIER_LOCAL_VALUE (id);
            IDENTIFIER_LOCAL_VALUE (id) = field;
          }
        $$ = $2;
      }
  | '(' error ')'
      {
        error ("invalid schema discriminants");
        $$ = NULL_TREE;
      }
  ;

discriminant_specification_list:
    discriminant_specification
      { $$ = $1; }
  | discriminant_specification_list semi discriminant_specification
      { $$ = chainon ($1, $3); }
  | discriminant_specification_list error discriminant_specification
      {
        error ("missing semicolon");
        $$ = chainon ($1, $3);
        yyerrok;
      }
  | discriminant_specification_list semi error
      {
        error ("extra semicolon");
        $$ = $1;
      }
  | discriminant_specification_list error
      { $$ = $1; }
  ;

discriminant_specification:
    setspecs id_list ':' typename
      {
        tree id;
        if (! ORDINAL_TYPE (TREE_CODE ($4)))
          {
            error ("schema discriminant type must be ordinal");
            $4 = integer_type_node;
          }
        for (id = $2; id; id = TREE_CHAIN (id))
          {
            /* Build VAR_DECL nodes to represent the formal
             * discriminants.
             */
            TREE_VALUE (id) = build_decl (VAR_DECL,
                                          TREE_VALUE (id), $4);
            PASCAL_TREE_DISCRIMINANT (TREE_VALUE (id)) = 1;
          }
        $$ = $2;
        resume_momentary ($1);
      }
  ;

/* This must return a TREE_LIST:
 *  purpose : initial value
 *  value   : .._TYPE node
 */
type_denoter:
    optional_type_qualifiers typename optional_actual_schema_discriminants
      {
        tree type = $2;

        if ($3)
          {
            /* Discriminants specified.  If TYPE is a schema type,
             * attach them, otherwise complain.
             */

            if (PASCAL_TYPE_UNDISCRIMINATED_STRING (type))
              {
                /* Strings are a special case.
                 */

                if (PEDANTIC (E_O_U_B_D_PASCAL))
                  pedwarn ("ISO 7185 Standard Pascal does not have string type");
                if ($1)
                  warning ("type qualifiers ignored in string type");
                /*
                 * UCSD/Borland Pascal:  For `String ( 42 )'
                 * a warning already has been given below in
                 * optional_actual_schema_discriminants below.
                 */
                if (TREE_CHAIN ($3))
                  error ("too many discriminants for string schema");

                type = build_pascal_string_schema (TREE_VALUE ($3));
              }
            else if (type == integer_type_node
                     || type == unsigned_type_node
                     || type == boolean_type_node)
              {
                /* Another special case: `Integer ( 16 )'
                 * (GPC extension: number of bits)
                 */
                if (pedantic || flag_what_pascal)
                  {
                    if (type == boolean_type_node)
                      pedwarn ("GPC-specific use of Boolean type");
                    else
                      pedwarn ("GPC-specific use of integer type");
                  }
                if (TREE_CODE (TREE_VALUE ($3)) == INTEGER_CST)
                  {
                    int bits = TREE_INT_CST_LOW (TREE_VALUE ($3));
                    if (TREE_INT_CST_HIGH (TREE_VALUE ($3)) != 0
                        || bits == 0)
                      error ("invalid integer bit specification");
                    else if (bits > TREE_INT_CST_LOW (TYPE_SIZE (long_long_integer_type_node)))
                      error ("integer type too large (maximum is %d bits)",
                             TREE_INT_CST_LOW (TYPE_SIZE (long_long_integer_type_node)));

                    if (type == boolean_type_node)
                      type = create_boolean_type_node (bits);
                    /* Avoid recursion when storing this in a GPI file.
                     * :-(This is a kludge.  Maybe it's better to create
                     * a subrange here.)
                     */
                    else if (TYPE_MAIN_VARIANT (type) == integer_type_node)
                      {
                        type = make_signed_type (bits);
                        TREE_TYPE (TYPE_MIN_VALUE (type)) = long_long_integer_type_node;
                        TREE_TYPE (TYPE_MAX_VALUE (type)) = long_long_integer_type_node;
                      }
                    else
                      {
                        type = make_unsigned_type (bits);
                        TREE_TYPE (TYPE_MIN_VALUE (type)) = long_long_unsigned_type_node;
                        TREE_TYPE (TYPE_MAX_VALUE (type)) = long_long_unsigned_type_node;
                      }
                  }
                else
                  error ("integer bit specification must be constant");
              }
            else if (PASCAL_TYPE_SCHEMA (type))
              {
                if (PEDANTIC (E_O_PASCAL))
                  pedwarn ("schema types are an ISO 10206 Extended Pascal extension");
                if ($1)
                  warning ("type qualifiers ignored in discriminated schema");
                if ($3 && PASCAL_TYPE_DISCRIMINATED_SCHEMA (type))
                  error ("schema is already discriminated");
                type = build_discriminated_schema_type (type, $3);
              }
            else
              error ("discriminated type is not a schema or string");
            $$ = build_tree_list (NULL_TREE, type);
          }
        else if (PASCAL_TYPE_STRING (type)
                 && declared_string_capacity (type) == 0)
          {
            if ((flag_what_pascal & B_D_PASCAL) == 0)
              {
                if ((flag_what_pascal & E_O_PASCAL) != 0)
                  pedwarn ( "missing string capacity - assuming 255");
                else
                  warning ( "missing string capacity - assuming 255");
              }
            if ($1)
              warning ("type qualifiers ignored in string type");
            $3 = build_int_2 (255, 0);
            type = build_pascal_string_schema ($3);
            $$ = build_tree_list (NULL_TREE, type);
          }
        else
          {
            int variant = $1 & TYPE_VARIANT;
            int ext     = $1 & TYPE_SELECT;
            if (variant)
              type = pascal_type_variant (type, variant, 1);

            if (lastiddecl)
              {
                if (ext)
                  type = pascal_type_extension (type, ext);

                $$ = build_tree_list (DECL_INITIAL (lastiddecl), type);
              }
            else /* error() has been called already; avoid crashing */
              $$ = build_tree_list (NULL_TREE, type);
          }
      }
  | optional_type_qualifiers type_denoter_1
      {
        tree type = $2;

        if ($1 & ~(TYPE_QUALIFIER_BINDABLE))
          error ("only `bindable' type qualifier allowed with a new type");

        if ($1 & TYPE_QUALIFIER_BINDABLE)
          type = pascal_type_variant (type, TYPE_QUALIFIER_BINDABLE, 1);

        $$ = build_tree_list (NULL_TREE, type);
      }
  ;

optional_type_qualifiers:
    /* empty */
      { $$ = 0; }
  | LEX_BINDABLE
      { $$ = TYPE_QUALIFIER_BINDABLE; }
  | LEX_BINDABLE LEX_RESTRICTED
      { $$ = TYPE_QUALIFIER_BINDABLE | TYPE_QUALIFIER_RESTRICTED; }
  | LEX_RESTRICTED
      { $$ = TYPE_QUALIFIER_RESTRICTED; }
  ;

type_denoter_1:
    new_ordinal_type
  | new_pointer_type
  | new_procedural_type
  | new_structured_type
  | type_inquiry
  ;

new_ordinal_type:
    enumerated_type
  | subrange_type
  ;

/* ENUMERATED */

enumerated_type:
    '(' setspecs
      {
        if (current_enum_type)  /* @@@ does it need a stack? */
          abort ();
        current_enum_type = $$ = start_enum (NULL_TREE);
      }
    enum_list ')'
      {
        $$ = finish_enum ($<ttype>3, nreverse ($4), NULL_TREE);
        current_enum_type = NULL_TREE;
        resume_momentary ($2);
      }
  | '(' error ')'
      { $$ = error_mark_node; }  /* @@@ ??? */
  ;

/* For speed reasons, we chain the enumerators in reverse order.
 * they are later put in forward order.
 */

enum_list:
    enumerator
  | enum_list ',' enumerator
      {
        $$ = chainon ($3, $1);
        yyerrok;
      }
/* (causes a shift/reduce conflict) */
  | error
      { error ("non-enumerator in enumerator list"); }
  | enum_list error enumerator
      {
        $$ = chainon ($3, $1);
        error ("missing comma");
        yyerrok;
      }
  | enum_list ',' error
      {
        $$ = $1;
        error ("extra comma");
      }
  | enum_list error
      {
        $$ = $1;
        /* error ("improperly terminated enum_list"); */
      }
  ;

/* ord(first_enumerator) == 0 */
/* second arg to build enumerator is the ord number exp of the enumerator */
enumerator:
    new_identifier
      { $$ = build_enumerator ($1, NULL_TREE); }
  ;

/* SUBRANGE */

/* Here is a problem:  In Extended Pascal, subrange bounds are allowed
 * to be arbitrary expressions.  If just done the obvious way we get
 * 49 shift/reduce conflicts (one for each new_identifier).
 *
 * So we currently allow expressions only as the upper bound
 * of the range, the lower bound must be a constant until
 * we figure out a way to handle this.  "(a) .. j;" can be mixed up
 * with enumerated_type definition.
 */
subrange_type:
    constant LEX_RANGE expression
      {
        $1 = maybe_schema_discriminant (string_may_be_char ($1));
        $3 = maybe_schema_discriminant (string_may_be_char ($3));
        if (PEDANTIC (E_O_PASCAL)
            && (! TREE_CONSTANT ($3) || ! TREE_CONSTANT ($1)))
          pedwarn ("non-constant range bounds are an ISO 10206 Extended Pascal extension");
        if (TREE_TYPE ($1) == TREE_TYPE ($3)
            || (TREE_CODE (TREE_TYPE ($1)) == INTEGER_TYPE
                && TREE_CODE (TREE_TYPE ($3)) == INTEGER_TYPE))
          $$ = build_pascal_range_type ($1, $3);
        else
          {
            error ("subrange bounds are not of the same type");
            $$ = error_mark_node;
          }
      }
  | LEX_PACKED constant LEX_RANGE expression
      {
        if (pedantic || flag_what_pascal)
          pedwarn ("packed subrange types are a GNU Pascal extension");
        $2 = maybe_schema_discriminant (string_may_be_char ($2));
        $4 = maybe_schema_discriminant (string_may_be_char ($4));
        if (PEDANTIC (E_O_PASCAL)
            && (! TREE_CONSTANT ($4) || ! TREE_CONSTANT ($2)))
          pedwarn ("non-constant range bounds are an ISO 10206 Extended Pascal extension");
        if (TREE_TYPE ($2) == TREE_TYPE ($4)
            || (TREE_CODE (TREE_TYPE ($2)) == INTEGER_TYPE
                && TREE_CODE (TREE_TYPE ($4)) == INTEGER_TYPE))
          $$ = grok_packed (build_pascal_range_type ($2, $4));
        else
          {
            error ("subrange bounds are not of the same type");
            $$ = error_mark_node;
          }
      }
  ;

/* POINTER
 *
 * If this is a pointer to unknown type, it is pushed to
 * the tags list by build_pascal_pointer_type()
 */
new_pointer_type:
    pointer_char optional_const
      { defining_pointers = 1; }
    pointer_domain_type
      {
        $$ = build_pascal_pointer_type ($4, $2);
        defining_pointers = 0;
      }
  ;

pointer_char:
    '^'
  | '@'
  ;

optional_const:
    /* empty */
      { $$ = 0; }
  | LEX_CONST
      { $$ = 1; }
  ;

/* new_identifier allows forward referencing.
 * This is either LEX_ID or predefined word
 */
pointer_domain_type:
    new_identifier
      {
        if (lastiddecl != 0 && TREE_CODE (lastiddecl) == TYPE_DECL)
          $$ = TREE_TYPE (lastiddecl);

        /* Do this later when fixing up forward references:
         *
         *   else if (TREE_TYPE ($1) == string_schema_proto_type)
         *     $$ = string_schema_proto_type;
         */

        else /* a forward declared pointer to an unknown type */
          $$ = $1; /* return an IDENTIFIER_NODE */
      }
  | new_procedural_type
      {
        if (pedantic || flag_what_pascal)
          pedwarn ("pointers to procedures are a GNU Pascal extension");
        if (TREE_CODE ($1) != REFERENCE_TYPE || ! TREE_TYPE ($1))
          abort ();
        $$ = TREE_TYPE ($1);
      }
  | LEX_FILE
      {
        if (PEDANTIC (U_B_D_PASCAL))
          pedwarn ("untyped files are an UCSD Pascal extension");
        $$ = build_file_type (void_type_node,
                              NULL_TREE);
      }
  ;

/* PROCEDURAL TYPES */

new_procedural_type:
    LEX_PROCEDURE optional_procedural_type_formal_parameter_list
      {
        $$ = build_function_type (void_type_node, $2);
        $$ = build_reference_type ($$);
        if (PEDANTIC (B_D_PASCAL))
          pedwarn ("procedural variables and types are a Borland Pascal extension");
      }
  | LEX_FUNCTION optional_procedural_type_formal_parameter_list ':' typename
      {
        tree funtype = $4;
        if (PASCAL_TYPE_UNDISCRIMINATED_STRING (funtype))
          {
            if ((flag_what_pascal & B_D_PASCAL) == 0)
              {
                if ((flag_what_pascal & E_O_PASCAL) != 0)
                  pedwarn ( "unknown string capacity - assuming 255");
                else
                  warning ( "unknown string capacity - assuming 255");
              }
            funtype = build_pascal_string_schema (build_int_2 (255, 0));
          }
        $$ = build_function_type (funtype, $2);
        $$ = build_reference_type ($$);
        if (PEDANTIC (B_D_PASCAL))
          pedwarn ("procedural variables and types are a Borland Pascal extension");
      }
  ;

optional_procedural_type_formal_parameter_list:
    /* empty */
      { $$ = build_tree_list (NULL_TREE, void_type_node); }
  | '(' LEX_ELLIPSIS ')'
      { $$ = NULL_TREE; }
  | '(' procedural_type_formal_parameter_list semi LEX_ELLIPSIS ')'
      { $$ = $2; }
  | '(' procedural_type_formal_parameter_list ')'
      { $$ = chainon ($2, build_tree_list (NULL_TREE, void_type_node)); }
  | '(' error ')'
      {
        error ("invalid parameter list in procedural type");
        $$ = error_mark_node;
      }
  ;

procedural_type_formal_parameter_list:
    procedural_type_formal_parameter
      { $$ = $1; }
  | procedural_type_formal_parameter_list semi procedural_type_formal_parameter
      { $$ = chainon ($1, $3); }
  ;

procedural_type_formal_parameter:
    id_list
      {
        /* A list of type names.
         * (For backward compatibility with gpc-970420.)
         */
        tree id;
        tree types = NULL_TREE;
        for (id = $1; id; id = TREE_CHAIN (id))
          {
            tree type = lookup_name (TREE_VALUE (id));
            if (! type || TREE_CODE (type) != TYPE_DECL)
              type = check_if_predefined_type (TREE_VALUE (id));
            if (type && TREE_CODE (type) == TYPE_DECL)
              types = chainon (types,
                               build_tree_list (NULL_TREE,
                                                TREE_TYPE (type)));
          }
        $$ = types;
      }
  | id_list ':' typename
      {
        tree id;
        tree type = $3;
        tree types = NULL_TREE;
        if (type == string_schema_proto_type)
          {
            type = build_type_copy (build_reference_type (type));
            PASCAL_TYPE_VAL_REF_PARM (type) = 1;
          }
        for (id = $1; id; id = TREE_CHAIN (id))
          types = chainon (types, build_tree_list (NULL_TREE, type));
        $$ = types;
      }
  | LEX_CONST id_list ':' typename
      {
        tree id;
        tree type = $4;
        int size;
        tree types = NULL_TREE;
        type = build_type_variant (type, 1, TYPE_VOLATILE (type));
        if (PASCAL_TYPE_OBJECT (type)
            || (size = int_size_in_bytes (type)) == -1
            || size > int_size_in_bytes (ptr_type_node))
          type = build_reference_type (type);
        for (id = $2; id; id = TREE_CHAIN (id))
          types = chainon (types, build_tree_list (NULL_TREE, type));
        $$ = types;
      }
  | LEX_VAR id_list ':' typename
      {
        tree id;
        tree type = $4;
        tree types = NULL_TREE;
        type = build_reference_type (type);
        for (id = $2; id; id = TREE_CHAIN (id))
          types = chainon (types, build_tree_list (NULL_TREE, type));
        $$ = types;
      }
  | LEX_CONST id_list
      {
        tree id;
        tree type = void_type_node;
        tree types = NULL_TREE;
        type = build_type_variant (type, 1, TYPE_VOLATILE (type));
        type = build_reference_type (type);
        for (id = $2; id; id = TREE_CHAIN (id))
          types = chainon (types, build_tree_list (NULL_TREE, type));
        $$ = types;
      }
  | LEX_VAR id_list
      {
        tree id;
        tree type = void_type_node;
        tree types = NULL_TREE;
        type = build_reference_type (type);
        for (id = $2; id; id = TREE_CHAIN (id))
          types = chainon (types, build_tree_list (NULL_TREE, type));
        $$ = types;
      }
/* (causes a shift/reduce conflict) */
  | error
      {
        error ("invalid parameter in procedural type");
        $$ = NULL_TREE;
      }
  ;

/* STRUCTURED TYPES */

new_structured_type:
    LEX_PACKED
      {
        /* Temporarily flip the "flag_pack_struct" flag,
         * usually connected to the `-fpack-struct' switch,
         * to pack this structured type and to inform the
         * routines in gpc-util.c to do their part.
         */
        $<itype>$ = flag_pack_struct;
        flag_pack_struct = 1;
      }
    unpacked_structured_type
      {
        flag_pack_struct = $<itype>2;
        $$ = grok_packed ($3);
      }
  | unpacked_structured_type
  ;

unpacked_structured_type:
    array_type
  | file_type
  | set_type
  | record_type
  |
      {
        if (! top_level_p (current_module->main_program))
          error ("`object' type definition only allowed at top level");
        enable_keyword ("Constructor");
        enable_keyword ("Destructor");
        defining_methods = 1;
        check_object_pointer ();
      }
    object_type
      {
        defining_methods = 0;
        $$ = $2;
      }
  ;

optional_actual_schema_discriminants:
    /* empty */
      { $$ = NULL_TREE; }
  | '(' discriminant_expression_list ')'
      {
        $$ = $2;
        if (PEDANTIC (E_O_PASCAL))
          pedwarn ("UCSD and Borland Pascal want string capacity in brackets");
      }
  | '[' expression ']'
      {
        if (PEDANTIC (U_B_D_PASCAL))
          pedwarn ("ISO Pascal wants string capacity in parentheses");
        $$ = build_tree_list (NULL_TREE,
                              maybe_schema_discriminant (string_may_be_char ($2)));
      }
  ;

/* Array */

array_type:
    LEX_ARRAY '[' array_index_list ']' enable_caret LEX_OF type_denoter
      {
        lex_caret = $5;
        $$ = build_pascal_array_type (TREE_VALUE ($7), $3);
      }
  ;

array_index_list:
    ordinal_index_type
      { $$ = $1; }
  | array_index_list ',' ordinal_index_type
      {
        $$ = chainon ($1, $3);
        yyerrok;
      }
  | error
      {
        error ("non-expression in array_index_list");
        $$ = build_tree_list (error_mark_node, error_mark_node);
      }
  | array_index_list error ordinal_index_type
      {
        error ("missing comma");
        $$ = chainon ($1, $3);
        yyerrok;
      }
  | array_index_list ',' error
      {
        error ("extra comma");
        $$ = $1;
      }
  | array_index_list error
      {
        /* error ("improperly terminated array_index_list"); */
        $$ = $1;
      }
  ;


/*
 * TREE_LIST whose TREE_PURPOSE is the type and TREE_VALUE
 * is an integer range.
 *
 * Only for arrays and sets.
 */
ordinal_index_type:
    new_ordinal_type
      { $$ = build_tree_list ($1, convert_type_to_range ($1)); }
  | typename
      { $$ = build_tree_list ($1, convert_type_to_range ($1)); }
  ;

/* FILE */
file_type:
    LEX_FILE
      {
        if (PEDANTIC (U_B_D_PASCAL))
          pedwarn ("untyped files are an UCSD Pascal extension");
        $$ = build_file_type (void_type_node,
                              NULL_TREE);
      }
  |
    LEX_FILE direct_access_index_type LEX_OF type_denoter
      { $$ = build_file_type (TREE_VALUE ($4), $2); }
  ;

direct_access_index_type:
    /* empty */
      { $$ = NULL_TREE; }
  | '[' ordinal_index_type ']'
      { $$ = $2; }
  ;


/* sets */
set_type:
    LEX_SET LEX_OF ordinal_index_type
      {
        /* tree range = TREE_VALUE ($3); */
        /* Avoid huge sets, like 'set of -maxint..maxint' */
        TREE_VALUE ($3) = check_set_bounds (TREE_PURPOSE ($3),
                                            TREE_VALUE ($3));
        $$ = build_set_type ($3);
      }
  | LEX_SET LEX_OF error
      {
        $$ = build_set_type
              (build_tree_list
                  (void_type_node,
                   build_index_2_type (size_zero_node,
                                       build_int_2 (-1, 0))));
      }
  ;

/* records */
record_type:
/* This is not early enough. :-(
      {
        unsigned keyword_flags = 0;
        if (keyword_enabled ("Absolute"))
          keyword_flags |= 0x01;
        disable_keyword ("Absolute");
        if (keyword_enabled ("Value"))
          keyword_flags |= 0x02;
        disable_keyword ("Value");
        if (keyword_enabled ("External"))
          keyword_flags |= 0x04;
        disable_keyword ("External");
        if (keyword_enabled ("Asmname"))
          keyword_flags |= 0x08;
        disable_keyword ("Asmname");
        if (keyword_enabled ("Static"))
          keyword_flags |= 0x10;
        disable_keyword ("Static");
        if (keyword_enabled ("Volatile"))
          keyword_flags |= 0x20;
        disable_keyword ("Volatile");
*
        if (keyword_enabled ("Register"))
          keyword_flags |= 0x40;
        disable_keyword ("Register");
*
        $<itype>$ = keyword_flags;
      }
*/
    LEX_RECORD record_field_list LEX_END
      {
/*
        unsigned keyword_flags = $<itype>1;
        if (keyword_flags & 0x01)
          enable_keyword ("Absolute");
        if (keyword_flags & 0x02)
          enable_keyword ("Value");
        if (keyword_flags & 0x04)
          enable_keyword ("External");
        if (keyword_flags & 0x08)
          enable_keyword ("Asmname");
        if (keyword_flags & 0x10)
          enable_keyword ("Static");
        if (keyword_flags & 0x20)
          enable_keyword ("Volatile");
*
        if (keyword_flags & 0x40)
          enable_keyword ("Register");
*/
        $$ = $2;
        yyerrok;
      }
  | LEX_RECORD error LEX_END /* Return empty record */
      {
        $$ = finish_struct (start_struct (RECORD_TYPE, NULL_TREE),
                            NULL_TREE, NULL_TREE);
      }
  ;

/* returns RECORD_TYPE */
record_field_list:
    /* empty */
      {
        $$ = finish_struct (start_struct (RECORD_TYPE, NULL_TREE),
                            NULL_TREE, NULL_TREE);
      }
  | fixed_part optional_semicolon
      {
        $$ = finish_struct (start_struct (RECORD_TYPE, NULL_TREE),
                            $1, NULL_TREE);
      }
  | fixed_part semi variant_part
      {
        /* Chain the tag field or NULL_TREE */
        tree fields = chainon ($1, TREE_VALUE (TREE_PURPOSE ($3)));
        check_variant_record (fields, TREE_TYPE (TREE_VALUE (TREE_VALUE ($3))));
        /* Chain the variant part */
        fields = chainon ($1, TREE_VALUE (TREE_VALUE ($3)));
        $$ = finish_struct (start_struct (RECORD_TYPE, NULL_TREE),
                            fields, NULL_TREE);
        store_variant_tag_info ($$, $3);
      }
  | variant_part
      {
        /* Store the tag field or NULL_TREE */
        tree fields = TREE_VALUE (TREE_PURPOSE ($1));
        check_variant_record (fields, TREE_TYPE (TREE_VALUE (TREE_VALUE ($1))));
        /* Chain the variant part */
        fields = chainon (fields, TREE_VALUE (TREE_VALUE ($1)));
        $$ = finish_struct (start_struct (RECORD_TYPE, NULL_TREE),
                            fields, NULL_TREE);
        store_variant_tag_info ($$, $1);
      }
  ;

/* record fixed part */
/* chain on FIELD_DECL */
fixed_part:
    record_section
  | fixed_part semi record_section
      {
        $$ = chainon ($1, $3);
        yyerrok;
      }
  | fixed_part error record_section
      {
        error ("missing semicolon");
        $$ = chainon ($1, $3);
        yyerrok;
      }
  | fixed_part semi error
      {
        error ("extra semicolon");
        $$ = $1;
      }
  | fixed_part error
      {
        /* error ("improperly terminated record fixed part"); */
        $$ = $1;
      }
  ;

/* chain of FIELD_DECL */
/* @@@ check *_momentary use here? */
record_section:
    setspecs id_list enable_caret ':' type_denoter
      {
        if (TREE_PURPOSE ($5))
          warning ("initial value ignored in field declaration");
        $$ = p_grokfields ($2, TREE_VALUE ($5));
        resume_momentary ($1);
        lex_caret = $3;
      }
  ;

/* record variants */

/* returns a TREE_LIST node whose
 * PURPOSE : TREE_LIST of variant_selector
 * VALUE   : unnamed FIELD_DECL node of type UNION_TYPE
 */
variant_part:
    LEX_CASE variant_selector LEX_OF variant_list rest_of_variant
      {
        tree vlist = $4;
        if ($5)
          vlist = chainon (vlist, $5);
        $$ = build_tree_list ($2,
                build_record_variant_part (input_filename, lineno,
                                           $2, vlist));
      }
  ;

rest_of_variant:
    optional_semicolon
      { $$ = NULL_TREE; }
  | case_default '(' record_field_list ')' optional_semicolon
      {
        $$ = build_tree_list (NULL_TREE,
                              grokfield (input_filename, lineno,
                                         NULL_TREE,   /* field name */
                                         build_tree_list (NULL_TREE, $3),
                                         NULL_TREE)); /* field width */
      }
  ;

/* returns a TREE_LIST node whose
 * PURPOSE is the type of the selector
 * and
 * VALUE is a FIELD_DECL node if the variant selector actually exists,
 *       NULL_TREE otherwise.
 */
variant_selector:
    new_identifier ':' variant_type
    /* with init value? */
      {
        $$ = build_tree_list ($3,
                grokfield (input_filename,
                           lineno,
                           $1,
                           build_tree_list (NULL_TREE, $3),
                           NULL_TREE));
        yyerrok;
      }
/* @@@ causes reduce/reduce conflicts with Borland initializers.
 * These will vanish when array indices and field names are implemented.
 *
  | new_identifier error variant_type
      {
        error ("missing semicolon");
        $$ = build_tree_list ($3,
                grokfield (input_filename,
                           lineno,
                           $1,
                           build_tree_list (NULL_TREE, $3),
                           NULL_TREE));
        yyerrok;
      }
*/
  | variant_type
    /* with init value? */
      { $$ = build_tree_list ($1, NULL_TREE); }
  ;

variant_type:
    typename
  | new_ordinal_type
      {
        if (PEDANTIC (U_B_D_PASCAL))
          pedwarn ("ISO Pascal wants type identifier as variant tag type");
        $$ = $1;
      }
  ;

/* returns a chain of TREE_LIST nodes whose
 * PURPOSE field is the case list
 * and
 * VALUE is the FIELD_DECL node of RECORD_TYPE
 */
variant_list:
    variant
  | variant_list semi variant
      {
        $$ = chainon ($1, $3);
        yyerrok;
      }
  | variant_list error variant
      {
        error ("missing semicolon");
        $$ = chainon ($1, $3);
        yyerrok;
      }
  | error
      { $$ = NULL_TREE; }
  | variant_list error
      {
        /* error ("improperly terminated record variant list"); */
        $$ = $1;
      }
  ;

variant:
    case_constant_list ':' '(' record_field_list ')'
      {
        $$ = build_tree_list ($1,
               grokfield (input_filename, lineno,
                          NULL_TREE,   /* field name */
                          build_tree_list (NULL_TREE, $4),
                          NULL_TREE)); /* field width */
      }
  ;

case_constant_list:
    one_case_constant
  | case_constant_list ',' one_case_constant
      {
        $$ = chainon ($1, $3);
        yyerrok;
      }
  | case_constant_list ',' error
      {
        error ("extra comma");
        $$ = $1;
      }
  | case_constant_list error one_case_constant
      {
        error ("missing comma");
        $$ = chainon ($1, $3);
        yyerrok;
      }
  | case_constant_list error
      {
        /* error ("improperly terminated case constant list"); */
        $$ = $1;
      }
  ;

one_case_constant:
    static_expression
      { $$ = build_tree_list (NULL_TREE, string_may_be_char ($1)); }
  | static_expression LEX_RANGE static_expression
      {
        $$ = build_tree_list (string_may_be_char ($3),
                              string_may_be_char ($1));
      }
/* (caused 2 shift/reduce conflicts and 6 reduce/reduce conflicts)
  | error
      { $$ = NULL_TREE; }
*/
/* (causes a shift/reduce conflict) */
  | static_expression error static_expression
      {
        error ("missing `..'");
        $$ = build_tree_list ($3, $1);
        yyerrok;
      }
  | static_expression LEX_RANGE error
      {
        error ("extra `..'");
        $$ = build_tree_list (NULL_TREE, $1);
      }
  ;

/* in Extended Pascal */
type_inquiry:
    LEX_TYPE LEX_OF expression
      {
        $$ = TREE_TYPE ($3);
        if (PEDANTIC (E_O_PASCAL))
          pedwarn ("`type of' is an ISO 10206 Extended Pascal extension");
      }
  ;


/* OBJECT */
object_type:
    LEX_OBJECT object_field_list LEX_END
      {
        $$ = $2;
        store_object_name ($$);
        add_vmt_field ($$);
        check_method_parameters ($$);
        construct_vmt ($$);
        yyerrok;
      }
  | LEX_OBJECT '(' new_identifier ')' object_field_list LEX_END
      {
        $$ = $5;
        store_object_name ($$);
        if ($3)
          {
            tree parent_type = lookup_name ($3);
            if (parent_type && TREE_CODE (parent_type) == TYPE_DECL)
              {
                parent_type = TREE_TYPE (parent_type);
                if (TREE_CODE (parent_type) == RECORD_TYPE)
                  inherit ($$, parent_type);  /* including vmt field */
                else
                  {
                    error ("parent type `%s' is no object type",
                           IDENTIFIER_POINTER ($3));
                    add_vmt_field ($$);
                  }
              }
            else
              {
                error ("parent object type expected, identifier `%s' given",
                       IDENTIFIER_POINTER ($3));
                add_vmt_field ($$);
              }
          }
        else
          {
            /* Can this ever happen? */
            error ("parent object type expected");
            add_vmt_field ($$);
          }
        check_method_parameters ($$);
        construct_vmt ($$);
        yyerrok;
      }
  | LEX_OBJECT '(' error ')' object_field_list LEX_END
      {
        $$ = $5;
        error ("invalid parent object specification");
        store_object_name ($$);
        add_vmt_field ($$);
        check_method_parameters ($$);
        construct_vmt ($$);
        yyerrok;
      }
  | LEX_OBJECT error LEX_END
      {
        $$ = finish_struct (start_struct (RECORD_TYPE, NULL_TREE),
                            NULL_TREE, NULL_TREE);
        store_object_name ($$);
      }
  ;

object_field_list:
    /* empty */
      {
        $$ = finish_struct (start_struct (RECORD_TYPE, NULL_TREE),
                            NULL_TREE, NULL_TREE);
        TYPE_ALIGN ($$) = BIGGEST_ALIGNMENT;
      }
  | object_fixed_part optional_semicolon
      {
        $$ = finish_struct (start_struct (RECORD_TYPE, NULL_TREE),
                            $1, NULL_TREE);
        TYPE_ALIGN ($$) = BIGGEST_ALIGNMENT;
      }
  ;

/* Extension to Borland standard:  Data fields and methods may be mixed. */
object_fixed_part:
    optional_object_directive object_section
      { $$ = $2; }
  | object_fixed_part semi optional_object_directive object_section
      {
        if ($4 == virtual_mark_node)
          $$ = mark_virtual_method ($1);
        else
          $$ = chainon ($1, $4);
        yyerrok;
      }
  | object_fixed_part error object_section
      {
        error ("missing semicolon");
        if ($3 == virtual_mark_node)
          $$ = mark_virtual_method ($1);
        else
          $$ = chainon ($1, $3);
        yyerrok;
      }
  | object_fixed_part semi optional_object_directive error
      {
        error ("extra semicolon");
        $$ = $1;
      }
  | object_fixed_part error
  ;

optional_object_directive:
    /* empty */
  | LEX_PRIVATE
      { object_directive_warning (); }
  | LEX_PUBLIC
      { object_directive_warning (); }
  | LEX_PUBLISHED
      { object_directive_warning (); }
  | LEX_PROTECTED
      { object_directive_warning (); }
  ;

object_section:
    setspecs id_list enable_caret ':' type_denoter
      {
        if (TREE_PURPOSE ($5))
          warning ("initial value ignored in field declaration");
        $$ = p_grokfields ($2, TREE_VALUE ($5));
        resume_momentary ($1);
        disable_keyword ("Virtual");
        lex_caret = $3;
      }
  | setspecs object_method_heading
      {
        tree ftype, arg, argtypes, fwd_heading, method, method_name;
        int is_structor =
          PASCAL_STRUCTOR_METHOD (TREE_PURPOSE ($2));

        /* Generate a FORWARD declaration `Myobj_Mymethod'
         */
        fwd_heading = copy_node (TREE_PURPOSE ($2));
        method_name = get_method_name (current_type_name,
                                       TREE_OPERAND (fwd_heading, 0));
        TREE_OPERAND (fwd_heading, 0) = method_name;
        method = grok_directive (TREE_VALUE ($2),
                   fwd_heading,
                   build_tree_list (NULL_TREE,
                     top_level_p (0)
                     && (current_module->main_program
                         || this_is_an_interface_module) ? d_extern
                                                         : d_static),
                     0);  /* never nested */

        argtypes = NULL_TREE;
        for (arg = TREE_OPERAND (TREE_PURPOSE ($2), 1);
             arg;
             arg = TREE_CHAIN (arg))
          if (TREE_PURPOSE (arg))
            argtypes = chainon (argtypes,
                                TREE_TYPE (TREE_PURPOSE (arg)));

        ftype = TREE_TYPE (method);
        $$ = p_grokfields (build_tree_list (NULL_TREE,
                             TREE_OPERAND (TREE_PURPOSE ($2), 0)),
                           ftype);

        /* Mark both the field decl and the function decl.
         */
        PASCAL_STRUCTOR_METHOD ($$) = is_structor;
        PASCAL_STRUCTOR_METHOD (method) = is_structor;

        resume_momentary ($1);
        if (this_is_an_interface_module)
          handle_autoexport (method_name);
        enable_keyword ("Virtual");
      }
  | LEX_VIRTUAL
      {
        $$ = virtual_mark_node;
        disable_keyword ("Virtual");
      }
  ;

object_method_heading:
    optional_inline LEX_PROCEDURE new_identifier optional_par_formal_parameter_list
      {
        $$ = tree_cons (build_nt (CALL_EXPR, $3, $4, NULL_TREE),
                        tree_cons (NULL_TREE, void_type_node, $1),
                        NULL_TREE);
      }
  | optional_inline LEX_CONSTRUCTOR new_identifier optional_par_formal_parameter_list
      {
        tree retval =
            get_identifier
              (concat ("retval_", IDENTIFIER_POINTER ($3), NULL_PTR));
        $$ = tree_cons (build_nt (CALL_EXPR, $3, $4, NULL_TREE),
                        tree_cons (NULL_TREE, boolean_type_node, $1),
                        retval);
        PASCAL_STRUCTOR_METHOD (TREE_PURPOSE ($$)) = 1;
      }
  | optional_inline LEX_DESTRUCTOR new_identifier optional_par_formal_parameter_list
      {
        $$ = tree_cons (build_nt (CALL_EXPR, $3, $4, NULL_TREE),
                        tree_cons (NULL_TREE, void_type_node, $1),
                        NULL_TREE);
        PASCAL_STRUCTOR_METHOD (TREE_PURPOSE ($$)) = 1;
      }
  | optional_inline LEX_FUNCTION new_identifier optional_par_formal_parameter_list
    optional_retval_def functiontype
      {
        tree retval = $5;
        tree funtype = $6;

        if (funtype == NULL_TREE)
          {
            /* Only allowed when function has already been declared
             * with a directive.  But not here.  Here it is just an
             * error.
             */
             error ("type of method function `%s' undefined",
                    IDENTIFIER_POINTER ($3));
          }
        else if (PASCAL_TYPE_UNDISCRIMINATED_STRING (funtype))
          {
            if ((flag_what_pascal & B_D_PASCAL) == 0)
              {
                if ((flag_what_pascal & E_O_PASCAL) != 0)
                  pedwarn ( "unknown string capacity - assuming 255");
                else
                  warning ( "unknown string capacity - assuming 255");
              }
            funtype = build_pascal_string_schema (build_int_2 (255, 0));
          }
        else if (PASCAL_TYPE_UNDISCRIMINATED_SCHEMA (funtype))
          error ("return type of function `%s' is incomplete",
                 IDENTIFIER_POINTER ($3));

        /* If user did not specify a name, we create one.
         */
        if (retval == NULL_TREE)
          retval =
            get_identifier
              (concat ("retval_", IDENTIFIER_POINTER ($3), NULL_PTR));

        $$ = tree_cons (build_nt (CALL_EXPR, $3, $4, NULL_TREE),
                        tree_cons (NULL_TREE, funtype, $1),
                        retval);
      }
  ;


/* variable declaration part */

variable_declaration_part:
    LEX_VAR variable_declaration_list
  ;

variable_declaration_list:
    variable_declaration
  | variable_declaration_list variable_declaration
      { yyerrok; }
  | error
  ;

variable_declaration:
    id_list
      {
        enable_keyword ("Absolute");
        enable_keyword ("Value");
        enable_keyword ("External");
        enable_keyword ("Asmname");
        enable_keyword ("Static");
        enable_keyword ("Volatile");
        enable_keyword ("Attribute");
        if (! flag_what_pascal || (flag_what_pascal & B_D_PASCAL))
          lex_const_equal = 0;
      }
    enable_caret ':' optional_storage_qualifier_list type_denoter
      {
        tree type = TREE_VALUE ($6);
        lex_caret = $3;
        if ((TREE_CODE (type) == POINTER_TYPE
             || TREE_CODE (type) == REFERENCE_TYPE)
            && TREE_CODE (TREE_TYPE (type)) == FUNCTION_TYPE)
          $<itype>$ = suspend_function_calls ();
        else
          $<itype>$ = -1;
      }
    absolute_or_value_specification semi
    optional_variable_directive_list
      {
        tree name_list = $1;
        unsigned qualifiers = $5;
        tree type_denoter = $6;
        tree init = $8;
        tree directives = $10;
        tree attributes = NULL_TREE;
        char *asmname = NULL_PTR;
        tree d;

        if ($<itype>7 != -1)
          resume_function_calls ($<itype>7);

        for (d = directives; d; d = TREE_CHAIN (d))
          {
            if (TREE_VALUE (d) == extern_id)
              qualifiers |= VQ_EXTERNAL;
            else if (TREE_VALUE (d) == asmname_id)
              {
                if (TREE_PURPOSE (d)
                    && TREE_CODE (TREE_PURPOSE (d)) == STRING_CST)
                  asmname = TREE_STRING_POINTER (TREE_PURPOSE (d));
                else
                  abort ();
              }
            else if (TREE_VALUE (d) == attribute_id)
              attributes = TREE_VALUE (d);
          }

        if (init && PASCAL_ABSOLUTE_CLAUSE (init))
          {
            /* No initial value but an absolute clause.
             */
            tree type = TREE_VALUE (type_denoter);
            tree var_name, var;
            for (var_name = name_list; var_name;
                 var_name = TREE_CHAIN (var_name))
              {
                var = build_decl (VAR_DECL, TREE_VALUE (var_name), type);
                type = build_pointer_type (type);
                DECL_INITIAL (var) = build_c_cast (type, init);
                PASCAL_ABSOLUTE_CLAUSE (DECL_INITIAL (var)) = 1;
                if (this_is_an_interface_module
                    && ! we_are_loading_a_gpi_file)
                  handle_autoexport (TREE_VALUE (var_name));
                pushdecl (var);
              }
          }
        else
          {
            /* If variable initialization not given,
             * try type initialization.
             */
            if (! init)
              init = TREE_PURPOSE (type_denoter);
            declare_vars (name_list, TREE_VALUE (type_denoter), init, asmname, qualifiers,
                          this_is_an_interface_module, NULL_TREE);
          }
        disable_keyword ("Absolute");
        disable_keyword ("Value");
        disable_keyword ("External");
        disable_keyword ("Asmname");
        disable_keyword ("Static");
        disable_keyword ("Volatile");
        disable_keyword ("Attribute");
        lex_const_equal = -1;
      }
  ;

optional_storage_qualifier_list:
    /* empty */
      { $$ = 0; }
  | storage_qualifier_list
      {
        if (pedantic || flag_what_pascal)
          pedwarn ("storage class qualifiers are a GNU Pascal extension");
        $$ = $1;
      }
  ;

storage_qualifier_list:
    storage_qualifier
  | storage_qualifier_list storage_qualifier
      { $$ = $1 | $2; }
  ;

storage_qualifier:
    LEX_STATIC
      { $$ = VQ_STATIC; }
  | LEX_VOLATILE
      { $$ = VQ_VOLATILE; }
  | D_REGISTER
      { $$ = VQ_REGISTER; }
  ;

optional_variable_directive_list:
    /* empty */
      { $$ = NULL_TREE; }
  | variable_directive_list
      {
        if (pedantic || flag_what_pascal)
          pedwarn ("variable directives are a GNU Pascal extension");
        $$ = nreverse ($1);
      }
  ;

variable_directive_list:
    variable_directive
      { $$ = $1; }
  | variable_directive variable_directive
      { $$ = chainon ($2, $1); }
  ;

variable_directive:
    LEX_EXTERNAL semi
      { $$ = build_tree_list (NULL_TREE, extern_id); }
  | LEX_ASMNAME string_constant semi
      { $$ = build_tree_list ($2, asmname_id); }
  | LEX_ATTRIBUTE
      { disable_keyword ("Const"); }
    '(' attribute_list ')'
      {
        $$ = build_tree_list ($4, attribute_id);
        enable_keyword ("Const");
      }
    semi 
  ;

absolute_or_value_specification:
    /* empty */
      { $$ = NULL_TREE; }
  | LEX_VALUE
      { $<itype>$ = suspend_function_calls (); }
    initializer_expression
      {
        if (PEDANTIC (E_O_PASCAL))
          pedwarn ("type/var initialization is an ISO 10206 Extended Pascal extension");
        $$ = $3;
        resume_function_calls ($<itype>2);
      }
  | LEX_ASSIGN
      { $<itype>$ = suspend_function_calls (); }
    initializer_expression
      {
        /* VAX Pascal initializer */
        if (PEDANTIC (VAX_PASCAL))
          pedwarn ("initialization with `:=' is a VAX Pascal extension");
        $$ = $3;
        resume_function_calls ($<itype>2);
      }
/* This would cause a shift/reduce conflict with '=' instead of LEX_CONST_EQUAL.
 * See also constant_definition.
 */
  | LEX_CONST_EQUAL
      { $<itype>$ = suspend_function_calls (); }
    initializer_expression
      {
        if (PEDANTIC (BORLAND_DELPHI))
          pedwarn ("initialization with `=' is a Delphi extension");
        $$ = $3;
        resume_function_calls ($<itype>2);
      }
  | LEX_ABSOLUTE
      { $<itype>$ = suspend_function_calls (); }
    expression
      {
        tree address = fold ($3);

        resume_function_calls ($<itype>2);

        if (PEDANTIC (B_D_PASCAL))
          pedwarn ("`absolute' clauses are a Borland Pascal extension");

        if (TREE_CODE (address) == INTEGER_CST)
          {
            if (! flag_extended_syntax)
              warning ("variables at absolute adresses may cause problems");
            if (TREE_CODE (TREE_TYPE (address)) != INTEGER_TYPE)
              error ("type mismatch in absolute address specification");
            $$ = copy_node (address);
          }
        else
          {
            tree x;
            TREE_USED (address) = 1;

            address = build_pascal_unary_op (ADDR_EXPR, address, 0);
            if (! TREE_CONSTANT (address)
                && ! /* Special case:  Variable parameters are okay here. */
                     (TREE_CODE (x = address) == NON_LVALUE_EXPR
                      && TREE_CODE (x = TREE_OPERAND (x, 0)) == CONVERT_EXPR
                      && TREE_CODE (x = TREE_OPERAND (x, 0)) == PARM_DECL
                      && TREE_CODE (x = TREE_TYPE (x)) == REFERENCE_TYPE)
                && ! flag_extended_syntax)
              warning ("`absolute' variable has non-constant address");
            $$ = address;
          }
        PASCAL_ABSOLUTE_CLAUSE ($$) = 1;
      }
  | LEX_VALUE error
      { $$ = NULL_TREE; }
  | LEX_ASSIGN error
      { $$ = NULL_TREE; }
/* This would cause a shift/reduce conflict with '=' instead of LEX_CONST_EQUAL.
 * See also constant_definition.
 */
  | LEX_CONST_EQUAL error
      { $$ = NULL_TREE; }
  | LEX_ABSOLUTE error
      { $$ = NULL_TREE; }
  ;

initializer_expression:
    static_expression
      { $$ = build_tree_list (NULL_TREE, $1); }
  | structured_iso_initializer
/* (causes a shift/reduce conflict) */
  | structured_borland_initializer
  ;

structured_iso_initializer:
    '[' iso_initializer_list ']'
      {
        if (PEDANTIC (E_O_PASCAL))
          pedwarn ("ISO 7185 Pascal does not support initialization");
        $$ = build_tree_list (NULL_TREE, nreverse ($2));
      }
  | '[' error ']'
      {
        if (PEDANTIC (E_O_PASCAL))
          pedwarn ("ISO 7185 Pascal does not support initialization");
        error ("invalid structured initializer");
        $$ = error_mark_node;
      }
  ;

iso_initializer_list:
    iso_initializer_expression
  | iso_initializer_list semi iso_initializer_expression
      {
        TREE_CHAIN ($3) = $1;
        $$ = $3;
      }
  | iso_initializer_list error iso_initializer_expression
      {
        error ("missing separator");
        TREE_CHAIN ($3) = $1;
        $$ = $3;
      }
  ;

iso_initializer_expression:
    new_identifier ':' initializer_expression
      {
        if (! TREE_CHAIN ($3) && ! TREE_PURPOSE ($3))
          $3 = TREE_VALUE ($3);
        $$ = build_tree_list ($1, $3);
      }
  | LEX_INTCONST ':' initializer_expression
      {
        if (! TREE_CHAIN ($3) && ! TREE_PURPOSE ($3))
          $3 = TREE_VALUE ($3);
        $$ = build_tree_list ($1, $3);
      }
  | sign LEX_INTCONST ':' initializer_expression
      {
        if (! TREE_CHAIN ($4) && ! TREE_PURPOSE ($4))
          $4 = TREE_VALUE ($4);
        $$ = build_tree_list ($2, $4);
      }
  ;

structured_borland_initializer:
    '(' ')'
    { $$ = NULL_TREE; }
  | '(' borland_initializer_list ')'
    {
      if (PEDANTIC (B_D_PASCAL))
        pedwarn ("Borland-specific initialization");
      $$ = build_tree_list (NULL_TREE, nreverse ($2));
    }
  | '(' error ')'
    {
      if (PEDANTIC (B_D_PASCAL))
        pedwarn ("Borland-specific initialization");
      error ("invalid structured initializer");
      $$ = error_mark_node;
    }
  ;

borland_initializer_list:
    borland_initializer_expression
  | borland_initializer_list initializer_separator borland_initializer_expression
      {
        TREE_CHAIN ($3) = $1;
        $$ = $3;
      }
  | borland_initializer_list error borland_initializer_expression
      {
        error ("missing separator");
        TREE_CHAIN ($3) = $1;
        $$ = $3;
      }
  ;

initializer_separator:
    semi
  | ','
      {
        if (PEDANTIC (B_D_PASCAL))
          pedwarn ("ISO Pascal wants `;' between initializer components");
      }
  ;

borland_initializer_expression:
    initializer_expression
  | iso_initializer_expression
  ;

/**** @@@@@ UNIMPLEMENTED ISO Initializers ****\

iso_initializer_expression:
    static_expression
  | structured_value_constructor
  ;

structured_value_constructor:
    typename structured_value
    { }
  ;

  @@@@ Must check that the typename in the structured_value_constructor
   is of correct type, i.e: array, record or set type name

structured_value:
    array_value
  | record_value
  | set_constructor
  ;

  tree_list (array_value_element_list, array_value_completer)

array_value:
    '[' array_value_initializer ']'
    { $$ = $2; }
  ;

array_value_initializer:
    array_value_element_list
    optional_array_value_completer
    optional_semicolon
    { $$ = build_tree_list ($1, $2); }
  ;

array_value_element_list:
    array_value_element
  | array_value_element_list ';' array_value_element
  ;

array_value_element:
    case_constant_list ':' component_value
    { }
  ;

optional_array_value_completer:
    { $$ = NULL_TREE; }
  | ';' LEX_OTHERWISE component_value
    { $$ = $3; }
  ;

component_value:
    static_expression
  | array_value
  | record_value
  ;

record_value:
    '[' field_list_value optional_semicolon ']'
    { $$ = $2; }
  ;

field_list_value:
    { $$ = NULL_TREE; }
  | fixed_part_value optional_variant_part_value
    { $$ = build_tree_list ($1, $2); }
  | variant_part_value
    { $$ = build_tree_list (NULL_TREE, $1); }
  ;

fixed_part_value:
    field_value_list
  ;

field_value_list:
    field_value
  | field_value_list semi field_value
  ;

field_value:
    field_identifier_list ':' component_value
  ;

optional_variant_part_value:
    { $$ = NULL_TREE; }
  | variant_part_value
  ;

variant_part_value:
    LEX_CASE optional_tag_field_identifier static_expression
    LEX_OF field_list_value
    { }
  ;

optional_tag_field_identifier:
    { $$ = NULL_TREE; }
  | field_identifier
  ;

field_identifier_list:
    field_identifier
  | field_identifier_list ',' field_identifier
  ;

field_identifier:
    identifier
  ;
\**** @@@@ The above is not implemented ****/


/* procedure = function returning void_type_node */
/* function */

enable_directives:
    /* empty */
      {
        enable_keyword ("External");
        enable_keyword ("Asmname");
        enable_keyword ("Attribute");
      }
  ;

function_declaration:
    function_heading enable_directives semi directive_list semi
      {
        tree heading = $1;
        tree dir = $4;
        if (is_known_directive (dir))
          grok_directive (TREE_VALUE (heading), TREE_PURPOSE (heading), dir,
                          ! pascal_global_bindings_p ());
        else
          error ("declaration part or directive expected, `%s' given",
                 IDENTIFIER_POINTER (TREE_VALUE (dir)));

        current_type_name = NULL_TREE;
        if (defining_methods)
          defining_methods--;
        disable_keyword ("Inherited");
        disable_keyword ("External");
        disable_keyword ("Asmname");
        disable_keyword ("Attribute");
      }
  | function_heading enable_directives semi optional_borland_directive
      {
        tree qual = TREE_VALUE ($1);
        tree heading = TREE_PURPOSE ($1);

        disable_keyword ("External");
        disable_keyword ("Asmname");
        disable_keyword ("Attribute");

        associate_external_objects (current_module->parms);

        if (defining_methods == 1
            && ! top_level_p (current_module->main_program))
          error ("method definition only allowed at top level");

        push_label_level ();

        /* set flag if we are defining a nested function */
        $<itype>3 = ! pascal_global_bindings_p ();

        if ($<itype>3)
          push_c_function_context ();

        if (! current_module->main_program
            && ! this_is_an_interface_module /* @@@ Can't be. */
            && TREE_CODE (TREE_OPERAND (heading, 0)) == IDENTIFIER_NODE
            && ! name_exported_p (TREE_OPERAND (heading, 0)))
          qual = maybe_make_static (qual);

        if (! ($<ttype>$ = start_pascal_function (qual,
                                                  heading,
                                                  $<itype>3)))
          {
            if ($<itype>3)
              pop_c_function_context ();
            YYERROR1;
          }

        /* Mark constructor and destructor methods as such.
         */
        if (PASCAL_STRUCTOR_METHOD (TREE_PURPOSE ($1)))
          {
            tree function_decl = lookup_name (TREE_VALUE (heading));
            if (function_decl)
              PASCAL_STRUCTOR_METHOD (function_decl) = 1;
          }

        store_parm_decls ();
      }
    pushlevel1
      {
        tree rtype;

        if (defining_methods == 1)
          {
            /* Implicitly do "with Self do".
             */
            tree self = lookup_name (self_name_node);
            self = build_indirect_ref (self, "variable parameter reference");
            pascal_shadow_record_fields (self, 0);
          }

        /* Declare the function RESULT_DECL.
         */
        rtype = $<ttype>5;
        if (rtype != void_type_node)
          {
            tree d = start_decl (TREE_CHAIN ($1),
                                 chainon (NULL_TREE,
                                          build_tree_list (NULL_TREE, rtype)),
                                 0, NULL_TREE, NULL_TREE);
            finish_decl (d, NULL_TREE, NULL_TREE);
          }
      }
    any_declaration_part
      {
        un_initialize_block (getdecls (), 0);
        if (TREE_CHAIN ($1))
          {
            current_function_return_variable = lookup_name (TREE_CHAIN ($1));
            if (current_function_return_variable)
              {
                if (PASCAL_STRUCTOR_METHOD (TREE_PURPOSE ($1)))
                  {
                    expand_expr_stmt (build_modify_expr (current_function_return_variable,
                                                         NOP_EXPR, boolean_true_node));
                    PASCAL_VALUE_ASSIGNED (current_function_return_variable) = 1;
                  }
              }
            else
              warning ("Internal GPC problem: return variable not found");
          }
        else
          current_function_return_variable = NULL_TREE;
        disable_keyword ("Inline");
      }
    statement_part semi
      {
        tree result = DECL_RESULT (current_function_decl);

        enable_keyword ("Inline");

        un_initialize_block (getdecls (), 1);
        if (TREE_TYPE (result) != void_type_node)
          {
            tree d;
            /* We do it like this, since if the code
             * directly assigns to DECL_RESULT it will lose
             * on recursive functions.
             *
             * TREE_CHAIN ($1) contains the unique name for
             * the return value parameter of this function.
             */
            if (! TREE_CHAIN ($1)
                || !(d = lookup_name (TREE_CHAIN ($1))))
              abort ();

            if (! PASCAL_VALUE_ASSIGNED (d))
              warning ("return value of function not assigned");

            c_expand_return (d);
          }
        if (defining_methods == 1)
          poplevel (0, 0, 0);
      }
    poplevel1
      {
        finish_function ($<itype>3);
        if ($<itype>3)
          pop_c_function_context ();

        pop_label_level ();

        if (defining_methods)
          defining_methods--;
        if (! defining_methods)
          {
            current_type_name = NULL_TREE;
            disable_keyword ("Inherited");
          }
        defining_operators = 0;
      }
  ;

/* Parameters of forward-declared procedures/functions and method
 * definition are checked whether they match with the previous declaration
 * in start_pascal_function().  Parameters are taken from the first
 * definition if omitted here.
 */
function_heading:
    optional_inline
    LEX_PROCEDURE
    function_identifier
    optional_par_formal_parameter_list
    {
      $$ = tree_cons (build_nt (CALL_EXPR, $3, $4, NULL_TREE),
                      tree_cons (NULL_TREE, void_type_node, $1),
                      NULL_TREE);
    }
  | optional_inline
    LEX_CONSTRUCTOR
    method_identifier
    optional_par_formal_parameter_list
    {
      tree retval =
          get_identifier
            (concat ("retval_", IDENTIFIER_POINTER ($3), NULL_PTR));
      $$ = tree_cons (build_nt (CALL_EXPR, $3, $4, NULL_TREE),
                      tree_cons (NULL_TREE, boolean_type_node, $1),
                      retval);
      PASCAL_STRUCTOR_METHOD (TREE_PURPOSE ($$)) = 1;
    }
  | optional_inline
    LEX_DESTRUCTOR
    method_identifier
    optional_par_formal_parameter_list
    {
      $$ = tree_cons (build_nt (CALL_EXPR, $3, $4, NULL_TREE),
                      tree_cons (NULL_TREE, void_type_node, $1),
                      NULL_TREE);
      PASCAL_STRUCTOR_METHOD (TREE_PURPOSE ($$)) = 1;
    }
  | optional_inline
    LEX_FUNCTION
    function_identifier
    optional_par_formal_parameter_list
    optional_retval_def
    functiontype
    {
      tree retval = $5;
      tree funtype = $6;

      if (funtype == NULL_TREE)
        {
          /* Only allowed when function has already been declared with
           * a directive. This will be altered to the real return type.
           */

          tree decl = lookup_name ($3);
          funtype = void_type_node;

          if (! decl || TREE_CODE (decl) != FUNCTION_DECL)
            {
              error ("type of function `%s' undefined",
                     IDENTIFIER_POINTER ($3));
              funtype = error_mark_node;
            }
        }
      else if (PASCAL_TYPE_UNDISCRIMINATED_STRING (funtype))
        {
          if ((flag_what_pascal & B_D_PASCAL) == 0)
            {
              if ((flag_what_pascal & E_O_PASCAL) != 0)
                pedwarn ("unknown string capacity - assuming 255");
              else
                warning ("unknown string capacity - assuming 255");
            }
          funtype = build_pascal_string_schema (build_int_2 (255, 0));
        }
      else if (contains_file_p (funtype))
        error ("functions must not return files");
      else if (PASCAL_TYPE_UNDISCRIMINATED_SCHEMA (funtype))
        error ("return type of function `%s' is incomplete",
               IDENTIFIER_POINTER ($3));

      /* If user did not specify a name, we create one.
       */
      if (retval == NULL_TREE)
        retval =
          get_identifier
            (concat ("retval_", IDENTIFIER_POINTER ($3), NULL_PTR));

      $$ = tree_cons (build_nt (CALL_EXPR, $3, $4, NULL_TREE),
                      tree_cons (NULL_TREE, funtype, $1),
                      retval);
    }
  | optional_inline
    LEX_OPERATOR
    { defining_operators = 1; }
    operator_identifier
    optional_par_formal_parameter_list
    optional_retval_def
    functiontype
    {
      tree retval = $6;
      tree funtype = $7;

      current_type_name = NULL_TREE;

      if (PEDANTIC (PASCAL_SC))
        pedwarn ("operator definitions are a Pascal-SC extension");

      if (funtype == NULL_TREE)
        {
          error ("return type of operator undefined");
          funtype = error_mark_node;
        }

      /* Make the parser accept this identifier as "known";
       * it will be replaced with the "real" identifier
       * (something like "plus_Integer_Complex") afterwards.
       */
      IDENTIFIER_LOCAL_VALUE ($4) = error_mark_node;

      if (! $5
          || ! TREE_CHAIN ($5)
          || ! TREE_CHAIN (TREE_CHAIN ($5))
          || ! TREE_CHAIN (TREE_CHAIN (TREE_CHAIN ($5)))
          || TREE_CHAIN (TREE_CHAIN (TREE_CHAIN (TREE_CHAIN ($5)))))
        error ("operator must have two arguments");
      else
        $4 = get_operator_identifier (IDENTIFIER_POINTER ($4),
                                      TREE_CHAIN ($5),
                                      TREE_CHAIN (TREE_CHAIN ($5)));

      if (retval == NULL_TREE)
        {
          char *retval_name = concat ("retval_", IDENTIFIER_POINTER ($4), NULL_PTR);
          pedwarn ("PXSC requires operator result variable specification");
          retval = get_identifier (retval_name);
          free (retval_name);
        }

      $$ = tree_cons (build_nt (CALL_EXPR, $4, $5, NULL_TREE),
                      tree_cons (NULL_TREE, funtype, $1),
                      retval);
    }
  ;

optional_inline:
    /* empty */
    { $$ = NULL_TREE; }
  |
    LEX_INLINE
    { $$ = build_tree_list (NULL_TREE, inline_id); }
  ;

function_identifier:
    new_identifier
    {
      if (defining_methods)  /* Nested function inside method */
        defining_methods++;
      else
        current_type_name = NULL_TREE;
      $$ = $1;
    }
  | method_identifier
  ;

method_identifier:
    /* The first component should be identifier, but this causes a number
       of s/r and r/r conflict due to the function_identifier rules.
       Using new_identifier doesn't hurt, except that we have to check
       elsewhere that the object type (or class) really exists. It might
       even have the advantage that the parser won't try to accept
       qualified identifiers here (which are not allowed in the header
       of an object method implementation). */
    new_identifier '.' new_identifier
    {
      current_type_name = $1;
      defining_methods++;
      enable_keyword ("Inherited");
      $$ = get_method_name ($1, $3);
    }
  ;

operator_identifier:
    new_identifier
    { $$ = $1; }
  | '+'
    { $$ = get_identifier ("plus"); }
  | '-'
    { $$ = get_identifier ("minus"); }
  | '*'
    { $$ = get_identifier ("mult"); }
  | '/'
    { $$ = get_identifier ("rdiv"); }
  | LEX_DIV
    { $$ = get_identifier ("div"); }
  | LEX_MOD
    { $$ = get_identifier ("mod"); }
  | LEX_IN
    { $$ = get_identifier ("in"); }
  | '<'
    { $$ = get_identifier ("lt"); }
  | '='
    { $$ = get_identifier ("eq"); }
  | '>'
    { $$ = get_identifier ("gt"); }
  | LEX_NE
    { $$ = get_identifier ("neq"); }
  | LEX_GE
    { $$ = get_identifier ("gte"); }
  | LEX_LE
    { $$ = get_identifier ("lte"); }
  | LEX_OR
    { $$ = get_identifier ("or"); }
  | LEX_AND
    { $$ = get_identifier ("and"); }
  | LEX_CEIL_PLUS
    { $$ = get_identifier ("ceilplus"); }
  | LEX_CEIL_MINUS
    { $$ = get_identifier ("ceilminus"); }
  | LEX_CEIL_MULT
    { $$ = get_identifier ("ceilmult"); }
  | LEX_CEIL_DIV
    { $$ = get_identifier ("ceilrdiv"); }
  | LEX_FLOOR_PLUS
    { $$ = get_identifier ("floorplus"); }
  | LEX_FLOOR_MINUS
    { $$ = get_identifier ("floorminus"); }
  | LEX_FLOOR_MULT
    { $$ = get_identifier ("floormult"); }
  | LEX_FLOOR_DIV
    { $$ = get_identifier ("floorrdiv"); }
  /*
   * The remaining operators such as "pow" or "shl" belong to
   * new_identifer.  To include them here would cause reduce/reduce
   * conflicts but is unnecessary for the same reason.
   */
  ;

directive_or_identifier:  /* For simple error recovery (?) */
    directive
  | LEX_ID
    { $$ = build_tree_list (NULL_TREE, $1); }
  ;

directive_list:
    directive_or_identifier
  | directive_list semi directive
    { $$ = chainon ($1, $3); }

directive:
    LEX_FORWARD
      { $$ = build_tree_list (NULL_TREE, $1); }
  | LEX_ASMNAME string_constant
      { $$ = build_tree_list ($2, $1); }
  | LEX_ATTRIBUTE
      { disable_keyword ("Const"); }
    '(' attribute_list ')'
      {
        $$ = build_tree_list ($4, $1);
        enable_keyword ("Const");
      }
  | D_C
      { $$ = build_tree_list (NULL_TREE, $1); }
  | D_C_LANGUAGE
      { $$ = build_tree_list (NULL_TREE, $1); }
  | LEX_EXTERNAL
      { $$ = build_tree_list (NULL_TREE, $1); }
  | D_EXTERN
      { $$ = build_tree_list (NULL_TREE, $1); }
  | D_REGISTER
      { $$ = build_tree_list (NULL_TREE, $1); }
  | LEX_STATIC
      { $$ = build_tree_list (NULL_TREE, $1); }
  ;

attribute_list:
    attrib
      { $$ = $1; }
  | attribute_list ',' attrib
      { $$ = chainon ($1, $3); }
  ;

attrib:
    /* empty */
      { $$ = NULL_TREE; }
  | any_word
      { $$ = build_tree_list ($1, NULL_TREE); }
/*
 * This was in c-parse.y, but caused a shift/reduce conflict. - PG
 *
  | any_word '(' LEX_ID ')'
      { $$ = build_tree_list ($1, build_tree_list (NULL_TREE, $3)); }
 */
  | any_word '(' LEX_ID ',' expression ')'
      { $$ = build_tree_list ($1, tree_cons (NULL_TREE, $3, $5)); }
  | any_word '(' expression ')'
      { $$ = build_tree_list ($1, build_tree_list (NULL_TREE, $3)); }
  ;

any_word:
    new_identifier
      { $$ = de_capitalize ($1); }
  ;

optional_borland_directive:
    /* empty */
  | bp_NEAR semi
      {
        if (PEDANTIC (B_D_PASCAL))
          pedwarn ("`near' directive is specific to Borland Pascal");
        else if (warn_near_far)
          warning ("`near' directive ignored in flat memory model");
      }
  | bp_FAR semi
      {
        if (PEDANTIC (B_D_PASCAL))
          pedwarn ("`far' directive is specific to Borland Pascal");
        else if (warn_near_far)
          warning ("`far' directive ignored in flat memory model");
      }
  ;

optional_retval_def:
    /* empty */
      { $$ = NULL_TREE; }
  | new_identifier
      {
        if ((pedantic || flag_what_pascal) && ! defining_operators)
          {
            pedwarn ("ISO 10206 Pascal requires `=' in front of");
            pedwarn (" function result variable specification");
          }
        $$ = $1;
      }
  | '=' new_identifier
      {
        if ((pedantic || flag_what_pascal) && defining_operators)
          {
            pedwarn ("Pascal-SC does not allow `=' before operator");
            pedwarn (" result variable specification");
          }
        if (PEDANTIC (E_O_PASCAL) && ! defining_operators)
          {
            pedwarn ("function result variable specifications are");
            pedwarn (" an ISO 10206 Extended Pascal extension");
          }
        $$ = $2;
      }
  ;

functiontype:
    /* empty */
      { $$ = NULL_TREE; }
  | ':' typename
      {
        /* If with an initial value: Should it init the function? */
        $$ = $2;
      }
/* (causes 3 shift/reduce conflicts) */
  | error
      { $$ = void_type_node; }
  ;


/* parameter specification section */

optional_par_formal_parameter_list:
      {
        pushlevel (0);
        clear_parm_order ();
        declare_parm_level (1);
        $<itype>$ = keyword_enabled ("Protected");
        enable_keyword ("Protected");
      }
    parmlist1
      {
        $$ = $2;
        /* parmlist_tags_warning (); */
        poplevel (0, 0, 0);
        if (! $<itype>1)
          disable_keyword ("Protected");
      }
  ;

parmlist1:
    /* empty */
      {
        if (defining_methods == 1)
          push_self_parameter (current_type_name, defining_types);
        $$ = get_parm_info (1);
      }
  | '('
      {
        if (defining_methods == 1)
          push_self_parameter (current_type_name, defining_types);
      }
    parmlist2
      { $$ = $3 }
  ;

/* splitted to avoid 131 reduce/reduce conflicts
 * which came with the action after the '('
 */

parmlist2:
    formal_parameter_list ')'
      { $$ = get_parm_info (1); }
  | formal_parameter_list semi LEX_ELLIPSIS ')'
      {
        if (pedantic || flag_what_pascal)
          pedwarn ("ellipsis parameters are a GNU Pascal extension");
        $$ = get_parm_info (0);
      }
  | LEX_ELLIPSIS ')'
      {
        if (pedantic || flag_what_pascal)
          pedwarn ("ellipsis parameters are a GNU Pascal extension");
        $$ = get_parm_info (0);
      }
/* (causes a shift/reduce conflict) */
  | error ')'
      { $$ = tree_cons (NULL_TREE, NULL_TREE, NULL_TREE); }
  ;

formal_parameter_list:
    formal_parameter
  | formal_parameter_list semi formal_parameter
      { yyerrok; }
/* (causes a shift/reduce conflict) */
  | error
/* (causes 4 shift/reduce conflicts) */
  | formal_parameter_list error formal_parameter
      {
       error ("missing semicolon");
       yyerrok;
      }
  | formal_parameter_list semi error
  ;

formal_parameter:
    optional_protected id_list ':' parameter_form
      { handle_formal_param_list ($2, $4, NULL_TREE, 0, $1); }
  | LEX_CONST id_list
      {
        if (pedantic || flag_what_pascal)
          pedwarn ("untyped `const' parameters are a Borland Pascal extension");
        handle_formal_param_list ($2, void_type_node, NULL_TREE, 1, 1);
      }
  | LEX_CONST id_list ':' parameter_form
      {
        int size;
        if (PEDANTIC (B_D_PASCAL))
          pedwarn ("`const' parameters are a Borland Pascal extension");
        if (TREE_CODE ($4) == IDENTIFIER_NODE  /* forward reference */
            || TREE_CODE ($4) == VOID_TYPE
            || PASCAL_TYPE_OBJECT ($4)
            || PASCAL_TYPE_UNDISCRIMINATED_SCHEMA ($4)
            || (size = int_size_in_bytes ($4)) == -1
            || size > int_size_in_bytes (ptr_type_node))
          handle_formal_param_list ($2, $4, NULL_TREE, 1, 1);
        else
          handle_formal_param_list ($2, $4, NULL_TREE, 0, 1);
      }
  | optional_protected LEX_VAR id_list
      {
        if (PEDANTIC (U_B_D_PASCAL))
          pedwarn ("untyped `var' parameters are an UCSD Pascal extension");
        handle_formal_param_list ($3, void_type_node, NULL_TREE, 1, $1);
      }
  | optional_protected LEX_VAR id_list ':' parameter_form
      { handle_formal_param_list ($3, $5, NULL_TREE, 1, $1); }
  | function_heading
      {
        handle_formal_param_list (TREE_PURPOSE ($1), TREE_VALUE ($1),
                                  NULL_TREE, 0, 0);
      }
  | optional_protected id_list ':' conformant_array_schema
      { handle_formal_conf_array_param_list ($2, $4, 0, $1); }
  | LEX_CONST id_list ':' conformant_array_schema
      {
        if (pedantic || flag_what_pascal)
          pedwarn ("`const' parameters are a Borland Pascal extension");
        handle_formal_conf_array_param_list ($2, $4, 1, 1);
      }
  | optional_protected LEX_VAR id_list ':' conformant_array_schema
      { handle_formal_conf_array_param_list ($3, $5, 1, $1); }
  ;

optional_protected:
    /* Empty */
      { $$ = 0; }
  | LEX_PROTECTED
      { $$ = 1; }
  ;

parameter_form:
    typename
      {
        if (PASCAL_TYPE_STRING ($1)
            && declared_string_capacity ($1) == 0)
          $$ = string_schema_proto_type;
        else
          $$ = $1;  /* No string, or string with specified length. */
      }
  | LEX_FILE
      {
        /* Kind of special case of a typename.
         */
        if (PEDANTIC (U_B_D_PASCAL))
          pedwarn ("untyped files are an UCSD Pascal extension");
        $$ = build_file_type (void_type_node, NULL_TREE);
      }
  | type_inquiry
  | open_array
  ;

conformant_array_schema:
    packed_conformant_array_schema
  | unpacked_conformant_array_schema
  ;

typename_or_conformant_array_schema:
    typename
      {
        if (PASCAL_TYPE_STRING ($1)
            && declared_string_capacity ($1) == 0)
          $$ = string_schema_proto_type;
        else
          $$ = $1;  /* No string, or string with specified length. */
      }
  | packed_conformant_array_schema
  | unpacked_conformant_array_schema
  ;

/* Packed conformant array formal parameter */
packed_conformant_array_schema:
    LEX_PACKED LEX_ARRAY '[' index_type_specification ']'
    LEX_OF typename_or_conformant_array_schema
      {
        $$ = build_tree_list (error_mark_node,
                              build_tree_list ($4, $7));
      }
  ;

/* Unpacked conformant array formal parameter */
unpacked_conformant_array_schema:
    LEX_ARRAY '[' index_type_specification_list ']'
    LEX_OF typename_or_conformant_array_schema
      {
        $$ = build_tree_list (NULL_TREE,
                              build_tree_list ($3, $6));
      }
  ;

index_type_specification:
    new_identifier LEX_RANGE new_identifier ':' typename
      { $$ = build_tree_list (build_tree_list ($1, $3), $5); }
  ;

index_type_specification_list:
    index_type_specification
  | index_type_specification_list semi index_type_specification
      {
        $$ = chainon ($1, $3);
        yyerrok;
      }
/* | error
*/
  | index_type_specification_list error index_type_specification
      {
        error ("missing semicolon");
        yyerrok;
      }
  | index_type_specification_list semi error
  ;

/* Open array formal parameter */
open_array:
    LEX_ARRAY LEX_OF type_denoter
      {
        /*
         * Index range 0..1 is inserted, and the array
         * is flagged.
         */
        tree index = build_range_type (integer_type_node,
          integer_zero_node, integer_one_node);
        index = build_tree_list (index,
                                 convert_type_to_range (index));
        $$ = build_pascal_array_type (TREE_VALUE ($3), index);
        PASCAL_TYPE_OPEN_ARRAY ($$) = 1;
      }
  ;

/* statement part */

statement_part:
    compound_statement
      { stmt_count++; }
  ;

compound_statement:
    LEX_BEGIN pushlevel statement_sequence poplevel LEX_END
      { yyerrok; }
  ;

statement_sequence:
    lineno_statement
  | statement_sequence semi lineno_statement
      { yyerrok; }
/* (causes a shift/reduce conflict) */
  | statement_sequence error lineno_statement
      { /* error ("missing semicolon");  @@ Too verbose, and often misleading. */ }
/* (causes a shift/reduce conflict - see above) */
  | statement_sequence semi error
      {
        /* error ("extra semicolon");  @@ Perhaps too innovative :-) */
        yyerrok;
      }
  ;

save_filename:
      { $$ = input_filename; }
  ;

save_lineno:
      { $$ = lineno; }
  ;

lineno_statement:
      {
        if (debug_statement)
          {
            tree filename = build_string (strlen (input_filename) + 1, input_filename);
            TREE_TYPE (filename) = char_array_type_node;
            expand_expr_stmt (build_function_call (lookup_name (debug_statement),
              chainon (build_tree_list (NULL_TREE, filename),
                       build_tree_list (NULL_TREE, build_int_2 (lineno, 0)))));
          }
      }
    save_filename save_lineno statement
  ;

statement:
    label ':'
      {
        tree label = define_label (input_filename, lineno, $1);
        stmt_count++;
        emit_nop ();
        if (label)
          expand_label (label);
        position_after_white_space ();
      }
    save_filename save_lineno unlabelled_statement
  | unlabelled_statement
  | declaring_statement
/* (causes a shift/reduce conflict) */
  | error
  ;

/* GPC extension */
declaring_statement:
    LEX_VAR id_list
      {
        enable_keyword ("Absolute");
        enable_keyword ("Value");
        if (! flag_what_pascal || (flag_what_pascal & B_D_PASCAL))
          lex_const_equal = 0;
      }
    enable_caret ':' optional_storage_qualifier_list type_denoter
      { lex_caret = $4; }
    absolute_or_value_specification
      {
        tree init = $9;
        if (pedantic || flag_what_pascal)
          pedwarn ("variable declarations in statement part are a GNU Pascal extension");

        if (init && PASCAL_ABSOLUTE_CLAUSE (init))
          {
            /* no initial value but an absolute clause. */
            tree type = TREE_VALUE ($7);
            tree var_name, var;
            for (var_name = $2; var_name;
                 var_name = TREE_CHAIN (var_name))
              {
                var = build_decl (VAR_DECL, TREE_VALUE (var_name), type);
                type = build_pointer_type (type);
                DECL_INITIAL (var) = build_c_cast (type, init);
                PASCAL_ABSOLUTE_CLAUSE (DECL_INITIAL (var)) = 1;
                if (this_is_an_interface_module
                    && ! we_are_loading_a_gpi_file)
                  handle_autoexport (TREE_VALUE (var_name));
                pushdecl (var);
              }
          }
        else
          {
            tree var_name;
            /* If variable initialization not given, try type initialization */
            if (!init)
              init = TREE_PURPOSE ($7);
            declare_vars ($2, TREE_VALUE ($7), init, NULL_PTR, $6, 0, NULL_TREE);
            for (var_name = $2; var_name;
                 var_name = TREE_CHAIN (var_name))
              init_any (lookup_name (TREE_VALUE (var_name)), 0);
          }
        disable_keyword ("Absolute");
        disable_keyword ("Value");
        lex_const_equal = -1;
      }
  ;

unlabelled_statement:
    structured_statement
  | simple_statement
  ;

/* STRUCTURED STATEMENT */
structured_statement:
    compound_statement
  | with_statement
  | conditional_statement
  | repetitive_statement
  ;

/* `with' STATEMENT */
with_statement:
    LEX_WITH structured_variable_list LEX_DO lineno_statement
      {
        int i;
        for (i = $2; i; i--)
          poplevel (0, 0, 0);
      }
  ;

structured_variable_list:
    structured_variable
  | structured_variable_list ',' structured_variable
      {
        $$ += $3;
        yyerrok;
      }
  | error
      { $$ = 0; }
  | structured_variable_list error structured_variable
      {
        error ("missing comma");
        $$ += $3;
        yyerrok;
      }
  | structured_variable_list semi error
      { error ("extra comma"); }
  ;

structured_variable:
    variable_or_function_access
      {
        tree element = $1;

        /* Implicitly de-dereference schemata.
         */
        if (TREE_CODE (element) == COMPONENT_REF
            && PASCAL_TYPE_SCHEMA (TREE_TYPE (TREE_OPERAND (element, 0)))
            && DECL_NAME (TREE_OPERAND (element, 1)) == schema_id)
          element = TREE_OPERAND (element, 0);

        $$ = pascal_shadow_record_fields (element, 0);
      }
  ;

/* CONDITIONAL STATEMENT */
conditional_statement:
    if_statement
  | case_statement
  ;

/* `if then else' (shift/reduce conflict avoided with %nonassoc rules) */

simple_if:
    LEX_IF boolean_expression THEN_or_error
      {
        emit_line_note (input_filename, lineno);
        stmt_count++;
        expand_start_cond ($2, 0);
        $<itype>1 = stmt_count;
        if_stmt_file = $<filename>-1;
        if_stmt_line = $<lineno>0;
        position_after_white_space ();
      }
    lineno_statement
  ;

THEN_or_error:
    LEX_THEN
  | error
      { error ("missing `then'"); }
  ;

if_statement:
    simple_if LEX_ELSE
      {
        expand_start_else ();
        $<itype>1 = stmt_count;
        position_after_white_space ();
      }
    lineno_statement
      {
        expand_end_cond ();
        if (extra_warnings && stmt_count == $<itype>1)
          warning ("empty body in an `else' statement");
      }
  | simple_if %prec LEX_IF
      {
        expand_end_cond ();
        if (extra_warnings && stmt_count == $<itype>1)
          warning_with_file_and_line (if_stmt_file, if_stmt_line,
                                      "empty body in an `if' statement");
      }
  ;

/* `case' (see also VARIANT RECORDS) */
case_statement:
    LEX_CASE expression OF_or_error
      {
        stmt_count++;
        emit_line_note ($<filename>-1, $<lineno>0);

        /* Preserve (stack) the values of these global variables.
         */
        $<ttype>$ = build_tree_list (current_case_exit_label,
                                     current_case_expression);

        /* Declare a label for exiting this `case' statement.
         */
        push_label_level ();
        current_case_exit_label = get_unique_identifier ("case", 1);
        declare_label (current_case_exit_label,
                       pascal_global_bindings_p ());

        /* For type checking.
         * c_expand_start_case() will convert the type to
         * `Integer', so we copy_node() it.
         */
        $2 = string_may_be_char ($2);
        current_case_expression = copy_node ($2);
        c_expand_start_case ($2);

        /* Don't let the tree nodes for $2 be discarded by
         * clear_momentary during the parsing of the next stmt.
         */
        position_after_white_space ();
        $<itype>3 = keyword_enabled ("Otherwise");
        enable_keyword ("Otherwise");
      }
    case_element_list optional_semicolon_or_else_branch LEX_END
      {
        tree label;

        expand_end_case ($2);

        /* Define the label for exiting this `case' statement.
         */
        label = define_label (input_filename, lineno,
                              current_case_exit_label);
        stmt_count++;
        emit_nop ();
        if (label)
          expand_label (label);
        pop_label_level ();

        current_case_expression = TREE_VALUE ($<ttype>4);
        current_case_exit_label = TREE_PURPOSE ($<ttype>4);

        if (! $<itype>3)
          disable_keyword ("Otherwise");
        yyerrok;
      }
  ;

optional_semicolon_or_else_branch:
    /* empty */
  | semi
      {
        /* Create an implicit `otherwise' to avoid warnings
         * of unhandled cases.  In ISO Pascal, we trigger a
         * run-time error for them here.
         */
        tree duplicate;
        register tree label
          = build_decl (LABEL_DECL, NULL_TREE, NULL_TREE);
        int success = pushcase (NULL_TREE, 0, label, &duplicate);
        if (success != 0)
          abort ();
        if ((flag_what_pascal & S_E_O_PASCAL) == 0)
          {
            tree label = lookup_label (current_case_exit_label);
            TREE_USED (label) = 1;
            expand_goto (label);
          }
        else
          expand_expr_stmt (rts_call (gpc_RUNTIME_ERROR, void_type_node,
                                      ptype_int,
                                      build_tree_list (NULL_TREE,
                                                       build_int_2 (351, 0))));
      }
  | case_default
      {
        tree duplicate;
        register tree label
          = build_decl (LABEL_DECL, NULL_TREE, NULL_TREE);
        int success = pushcase (NULL_TREE, 0, label, &duplicate);
        stmt_count++;
        if (success == 1)
          error ("`else' or `otherwise' not within an `if' or `case' statement");
        else if (success == 2)
          {
            error ("multiple default labels in one case statement");
            error_with_decl (duplicate, "this is the first entry for that value");
          }
        position_after_white_space ();
      }
    pushlevel statement_sequence poplevel
      {
        /* Do not use pascal_expand_goto() since we
         * are not jumping to the main program from
         * outside.
         */
        tree label = lookup_label (current_case_exit_label);
        TREE_USED (label) = 1;
        expand_goto (label);
      }
  ;

OF_or_error:
    LEX_OF
  | error
      { error ("missing `of'"); }
  ;

case_element_list:
    case_element
  | case_element_list semi case_element
      { yyerrok; }
  | error
      { error ("case element expected"); }
  | case_element_list
    error case_element
      {
        error ("missing semicolon");
        yyerrok;
      }
  | case_element_list
    semi error
      { error ("extra semicolon"); }
  ;

/* now this looks more like a C switch statement */
case_element:
    case_constant_list ':'
      {
        register tree link;
        register tree value1, value2;
        register tree label;
        tree duplicate;

        for (link = $1; link; link = TREE_CHAIN (link))
          {
            value1 = TREE_VALUE (link);
            value2 = TREE_PURPOSE (link);
            label = build_decl (LABEL_DECL, NULL_TREE, NULL_TREE);

            /* @@@ use check_case_value ()??? */
            if (TREE_CODE (value1) != INTEGER_CST
                && value1 != error_mark_node)
              {
                if (value2)
                  {
                    error ("lower value of `case' range does not reduce to");
                    error (" a constant of ordinal type");
                  }
                else
                  error ("`case' label does not reduce to a constant of ordinal type");
                continue;
              }

            if (value2
                && TREE_CODE (value2) != INTEGER_CST
                && value2 != error_mark_node)
              {
                error ("upper value of `case' range does not reduce to");
                error (" a constant of ordinal type");
                continue;
              }

            if (value1 != error_mark_node && value2 != error_mark_node)
              {
                int success;

                if (comptypes (TREE_TYPE (current_case_expression),
                               TREE_TYPE (value1)) == 0)
                  {
                    if (value2)
                      error ("type mismatch in lower value of `case' range");
                    else
                      error ("type mismatch in `case' element");
                  }

                if (value2)
                  {
                    if (comptypes (TREE_TYPE (current_case_expression),
                                   TREE_TYPE (value2)) == 0)
                      error ("type mismatch in upper value of `case' range");

                    if (PEDANTIC (ANY_PASCAL & ~STANDARD_PASCAL))
                        pedwarn ("ISO 7185 Pascal does not allow `case' ranges");

                    success = pushcase_range (value1, value2, convert_and_check, label,
                                              &duplicate);
                  }
                else
                  success = pushcase (value1, convert_and_check, label, &duplicate);

                if (success == 1)
                  error ("`case' label not within `case' statement");
                else if (success == 2)
                  {
                    error ("duplicate `case' label value in `case' statement");
                    error_with_decl (duplicate, "this is the first entry for that value");
                  }
                else if (success == 3)
                  warning ("`case' label out of range");
                else if (success == 4)
                  warning ("empty `case' label range");
              }
          }
        position_after_white_space ();
      }
    pushlevel lineno_statement poplevel
      {
        /* Do not use pascal_expand_goto() since we
         * are not jumping to the main program from
         * outside.
         */
        tree label = lookup_label (current_case_exit_label);
        TREE_USED (label) = 1;
        expand_goto (label);
      }
  ;

/* None of these is in ISO-7185.
 */
case_default:
    LEX_ELSE
      {
        if (PEDANTIC (B_D_PASCAL))
          pedwarn ("`else' in `case' statements is a Borland Pascal extension");
      }
  | semi LEX_ELSE
      {
        if (PEDANTIC (B_D_PASCAL))
          pedwarn ("`else' in `case' statements is a Borland Pascal extension");
      }
  | LEX_OTHERWISE
      {
        if (PEDANTIC (E_O_PASCAL))
          pedwarn ("`otherwise' is an ISO 10206 Extended Pascal extension");
      }
  | semi LEX_OTHERWISE
      {
        if (PEDANTIC (E_O_PASCAL))
          pedwarn ("`otherwise' is an ISO 10206 Extended Pascal extension");
      }
  ;

/* REPETITIVE STATEMENT */

repetitive_statement:
    repeat_statement
  | while_statement
  | for_statement
  ;

/* `repeat' */
repeat_statement:
    LEX_REPEAT
      {
        emit_nop ();
        stmt_count++;
        emit_line_note ($<filename>-1, $<lineno>0);
        expand_start_loop_continue_elsewhere (1);
      }
    pushlevel statement_sequence poplevel LEX_UNTIL
      { expand_loop_continue_here (); }
    boolean_expression
      {
        emit_line_note (input_filename, lineno);
        expand_exit_loop_if_false (0,
              build_pascal_unary_op (TRUTH_NOT_EXPR, $8, 0));
        expand_end_loop ();
        /* clear_momentary (); */
      }
  ;

/* `while' */
while_statement:
    LEX_WHILE
      {
        emit_nop ();
        stmt_count++;
        emit_line_note ($<filename>-1, $<lineno>0);
        expand_start_loop (1);
      }
    boolean_expression
      {
        emit_line_note (input_filename, lineno);
        expand_exit_loop_if_false (0, $3);
      }
    DO_or_error pushlevel lineno_statement poplevel
      { expand_end_loop (); }
  ;

DO_or_error:
    LEX_DO
  | error
      { error ("missing `do'"); }
  ;

/* `for' */

/* We did not manage to do this without a temporary variable.
 *
 * Currently this is done so that we allocate a temporary VAR_DECL
 * for the loop upper bound; and store the expression there.
 *
 * ARGV!!! Both the lower and upper bounds must be evaluated
 * before the for loop control variable is assigned.
 * if not, the following does not work:
 *    for i:= (i+1) to (i+10) do
 *
 * So allocate another temporary variable for the lower bound...
 */
for_statement:
    LEX_FOR variable_or_function_access LEX_ASSIGN expression
    for_direction expression
      {
        tree for_l_bound, for_u_bound;
        int mark_low = 0;
        int mark_high = 0;

        check_for_control_variable ($2);
        emit_nop ();
        stmt_count++;
        emit_line_note ($<filename>-1, $<lineno>0);
        if (TREE_CONSTANT ($4))
          for_l_bound = string_may_be_char ($4);
        else
          {
            mark_low++;
            for_l_bound = start_decl
              (get_unique_identifier ("for_lower", 0),
               build_tree_list
                 (NULL_TREE,
                  TREE_TYPE ($2)),
               0, NULL_TREE, NULL_TREE);
            finish_decl (for_l_bound, NULL_TREE, NULL_TREE);
            PASCAL_LOOP_CHECK (for_l_bound) = 1;

            /* Assign the lower bound to temp */
            /* @@@ c_expand_expr_stmt() ??? */
            expand_expr_stmt (build_modify_expr (for_l_bound,
                                                 NOP_EXPR, $4));
          }

        if (TREE_CONSTANT ($6))
          for_u_bound = $6;
        else
          {
            mark_high++;
            for_u_bound = start_decl
              (get_unique_identifier ("for_upper", 0),
               build_tree_list
                 (NULL_TREE,
                  TREE_TYPE ($2)),
               0, NULL_TREE, NULL_TREE);
            finish_decl (for_u_bound, NULL_TREE, NULL_TREE);
            PASCAL_LOOP_CHECK (for_u_bound) = 1;

            emit_line_note ($<filename>-1, $<lineno>0);
            /* and the upper one */
            expand_expr_stmt (build_modify_expr (for_u_bound,
                                                 NOP_EXPR, $6));
          }
        $<ttype>$ = for_u_bound;

        /* necessary to prevent infinite loops when
         * incrementing/decrementing would cause
         * wrap-around at maxint/-maxint
         */
        expand_start_cond (build_pascal_binary_op ($5, for_l_bound,
                                                   for_u_bound), 0);
        if (mark_low)
          PASCAL_LOOP_CHECK (for_l_bound) = 0;
        if (mark_high)
          PASCAL_LOOP_CHECK (for_u_bound) = 0;

        emit_line_note ($<filename>-1, $<lineno>0);
        expand_expr_stmt (build_modify_expr ($2, NOP_EXPR, for_l_bound));
        expand_start_loop_continue_elsewhere (1);
      }
    DO_or_error
      { position_after_white_space (); }
    pushlevel lineno_statement poplevel
      {
        tree var_as_integer;
        emit_line_note ($<filename>-1, $<lineno>0);
        expand_loop_continue_here ();
        expand_exit_loop_if_false (0,
           fold (build_pascal_binary_op
                 ($5 == LE_EXPR ? LT_EXPR : GT_EXPR,
                  $2, $<ttype>7)));
        var_as_integer = convert (type_for_size (TYPE_PRECISION (TREE_TYPE ($2)),
                                                 TREE_UNSIGNED (TREE_TYPE ($2))), $2);
        expand_expr_stmt (build_modify_expr (var_as_integer,
                                             $5 == GE_EXPR ? MINUS_EXPR : PLUS_EXPR,
                                             integer_one_node));
        expand_end_loop ();
        expand_end_cond ();
      }
  | LEX_FOR variable_or_function_access
    LEX_IN simple_expression
      {
        /* Set member iteration.
         *
         * For now, translate it to something like
         *
         *    for myvar:= <lowest> to <highest possible value> do
         *      if myvar in myset then
         *        <do something>;
         */

        tree set_domain, set_var, for_l_bound, for_u_bound;
        int empty_set = 0;

        if (PEDANTIC (E_O_PASCAL))
          pedwarn ("set member iteration is an ISO 10206 Extended Pascal extension");
        check_for_control_variable ($2);
        emit_nop ();
        stmt_count++;
        emit_line_note ($<filename>-1, $<lineno>0);

        /* Make a set out of a set constructor
         */
        if (TREE_CODE ($4) == CONSTRUCTOR)
          $4 = construct_set ($4, NULL_TREE, 0);

        if (TREE_CODE (TREE_TYPE ($4)) != SET_TYPE)
          {
            error ("operand for `in' is not a set");
            empty_set = 1;
          }
        /* If the type of the set is unknown, derive it from
         * the type of the control variable.
         */
        else if (TREE_TYPE (TREE_TYPE ($4)) == NULL_TREE)
          TREE_TYPE (TREE_TYPE ($4)) = TREE_TYPE ($2);
        else if (TREE_TYPE (TREE_TYPE ($4)) == void_type_node)
          {
            /* We have "for foo in []".
             * Be as silent as possible to give the optimizer
             * a chance to remove the whole loop.
             */
            empty_set = 1;
          }
        $<itype>3 = empty_set;

        emit_line_note ($<filename>-1, $<lineno>0);
        if (empty_set)
          expand_start_cond (boolean_false_node, 0);
        else
          {
            if (TYPE_DOMAIN (TREE_TYPE ($4)))
              set_domain = TYPE_DOMAIN (TREE_TYPE ($4));
            else
              set_domain = TREE_TYPE ($4);
            for_l_bound = convert (TREE_TYPE (TREE_TYPE ($4)), TYPE_MIN_VALUE (set_domain));
            for_u_bound = convert (TREE_TYPE (TREE_TYPE ($4)), TYPE_MAX_VALUE (set_domain));
            $<ttype>$ = for_u_bound;
            set_var = make_new_variable ("for_set", TREE_TYPE ($4));
            expand_expr_stmt (build_modify_expr (set_var,
                                                 NOP_EXPR, $4));
            expand_expr_stmt (build_modify_expr ($2, NOP_EXPR,
                                                 for_l_bound));
            expand_start_loop_continue_elsewhere (1);
            expand_start_cond (build_pascal_binary_op (IN_EXPR, $2,
                                                       set_var), 0);
          }
      }
    DO_or_error
      { position_after_white_space (); }
    pushlevel lineno_statement poplevel
      {
        emit_line_note ($<filename>-1, $<lineno>0);
        expand_end_cond ();
        if (! $<itype>3)
          {
            tree var_as_integer;
            expand_loop_continue_here ();
            expand_exit_loop_if_false (0,
                fold (build_pascal_binary_op (LT_EXPR, $2, $<ttype>5)));
            var_as_integer = convert (type_for_size (TYPE_PRECISION (TREE_TYPE ($2)),
                                                     TREE_UNSIGNED (TREE_TYPE ($2))), $2);
            expand_expr_stmt (build_modify_expr (var_as_integer, PLUS_EXPR,
                                                 integer_one_node));
            expand_end_loop ();
          }
      }
  ;

for_direction:
    LEX_TO
      { $$ = LE_EXPR; }
  | LEX_DOWNTO
      { $$ = GE_EXPR; }
  | error
      { error ("missing `to' or `downto'"); }
  ;

/* SIMPLE STATEMENT */
simple_statement:
    empty_statement
  | goto_statement
      { stmt_count++; }
  | assignment_or_call_statement
      { stmt_count++; }
  | standard_function_statement
      { stmt_count++; }
  | statement_extensions
      { stmt_count++; }
  ;

empty_statement:
    /* empty */
  ;

goto_statement:
    LEX_GOTO label
      {
        emit_line_note ($<filename>-1, $<lineno>0);
        pascal_expand_goto ($2);
      }
  | LEX_GOTO '*' expression
      {
        emit_line_note ($<filename>-1, $<lineno>0);
        if (pedantic || flag_what_pascal)
          pedwarn ("indirect jumps are a GNU Pascal extension");
        expand_computed_goto (convert (ptr_type_node, $3));
      }
  ;

/* function calls */

optional_par_actual_parameter_list:
    /* empty */
      { $$ = NULL_TREE; }
  | '('
      { $<itype>1 = suspend_function_calls (); }
    actual_parameter_list r_paren_or_error
      {
        $$ = $3;
        resume_function_calls ($<itype>1);
        yyerrok;
      }
  ;

r_paren_or_error:
    ')'
  | error
      { error ("missing `)'"); }
  ;

actual_parameter_list:
    actual_parameter
      { $$ = build_tree_list (NULL_TREE, $1); }
  | actual_parameter_list ',' actual_parameter
      {
        $$ = chainon ($1, build_tree_list (NULL_TREE, $3));
        yyerrok;
      }
  | error
      { $$ = NULL_TREE; /* @@ verify this */ }
  | actual_parameter_list ',' error
      { error ("extra comma"); }
  ;

actual_parameter:
    expression
  ;

/* ASSIGNMENT and procedure calls.
 *
 * These were separated before I implemented procedure and function
 * types for variables and components of structured types.
 */

/* Should also implement threatening of optional_retval_def in assignment */
assignment_or_call_statement:
    variable_or_function_access_maybe_assignment
      {
        if (PASCAL_PROCEDURAL_VARIABLE ($1))
          $<itype>$ = suspend_function_calls ();
        else
          $<itype>$ = allow_function_calls ();
        emit_line_note (input_filename, lineno);
      }
    rest_of_statement
      {
        tree target = $1;
        tree source = $3;  /* In assignment != NULL_TREE */

        if (source == NULL_TREE
            || TREE_CODE (source) == ERROR_MARK)
          {
            /* No source:  This is no assignment.
             */
            resume_function_calls ($<itype>2);
            if (target == NULL_TREE
                || TREE_CODE (target) == ERROR_MARK
                || (source && TREE_CODE (source) == ERROR_MARK))
              {
                /* This is a procedure call, so the target
                 * is NULL_TREE.
                 *
                 * Just do nothing.
                 * `variable_or_function_access_maybe_assignment'
                 * already did call the procedure.
                 *
                 * Or, this is an error which already has been
                 * reported.  Just do nothing either.
                 */
              }
            else
              {
                int function_called = TREE_CODE (target) == CALL_EXPR;
                if (MAYBE_CALL_FUNCTION (target))
                  {
                    target = maybe_call_function (target, NULL_TREE);
                    function_called = 1;
                  }
                else if (CALL_METHOD (target))
                  {
                    target = call_method (target, NULL_TREE);
                    function_called = 1;
                  }

                if (target != error_mark_node)
                  {
                    if (! function_called)
                      pedwarn ("expression used as a statement - value is ignored");
                    else if (TREE_TYPE (target) != void_type_node
                             && ! (TREE_CODE (target) == CALL_EXPR
                                   && TREE_CODE (TREE_OPERAND (target, 0)) == ADDR_EXPR
                                   && PASCAL_CONSTRUCTOR_METHOD (TREE_OPERAND (TREE_OPERAND (target, 0), 0)))
                             && ! flag_ignore_function_results)
                      pedwarn ("function call as a statement - value is ignored");

                    expand_expr_stmt (target);
                  }
              }
          }
        else
          {
            /* We have a source:  This is an assignment.
             */
            int allow_restricted_target = FALSE;
            int allow_restricted_source = FALSE;

            if (CALL_METHOD (target))
              target = lookup_name (get_method_name (
                  TYPE_LANG_NAME (TREE_TYPE (TREE_OPERAND (target, 0))),
                  DECL_NAME (TREE_OPERAND (target, 1))));

            if (TREE_CODE (target) == FUNCTION_DECL)
              {
                if (TREE_TYPE (TREE_TYPE (target)) == void_type_node)
                  {
                    error ("you can't assign to a procedure");
                    target = error_mark_node;
                  }
                else
                  {
                    if (target == current_function_decl)
                      current_function_returns_value = 1;
                    else if (! maybe_find_function_data (target))
                      {
                        error ("function `%s' value assigned outside its block",
                               IDENTIFIER_POINTER (DECL_NAME (target)));
                        target = error_mark_node;
                      }
                    if (target != error_mark_node)
                      {
                        char *retval_name = concat ("retval_",
                                  IDENTIFIER_POINTER (DECL_NAME (target)), NULL_PTR);
                        target = lookup_name (get_identifier (retval_name));
                        free (retval_name);
                        if (! target)
                          {
                            error ("invalid assignment of function value");
                            target = error_mark_node;
                          }

                        /* Allow assignment of a non-restricted type
                         * to a restricted function return value
                         */
                        allow_restricted_target = TRUE;
                      }
                  }
              }

            if (target != error_mark_node)
              {
                /* To mark that we have assigned to this
                 * variable.  Currently only used to flag
                 * function value assignments, but it cannot
                 * be inside the FUNCTION_DECL conditional
                 * above, since GPC allows you to define a
                 * name for the return value of the
                 * function, and that name is not (at least
                 * currently) recorded in the function
                 * declaration. GPC assigns a VAR_DECL node
                 * for the name.
                 *
                 * @@ For COMPONENT_REFs, we should rather
                 * flag the fields and look at the end of
                 * the function whether all fields were
                 * assigned.
                 */
                tree t = target;
                while (TREE_CODE (t) == COMPONENT_REF)
                  t = TREE_OPERAND (t, 0);
                if (TREE_CODE (t) == VAR_DECL)
                  PASCAL_VALUE_ASSIGNED (t) = 1;

                /* Warn about Borland Pascal "typed const" misuse.
                 */
                typed_const_warning (target);

                /* Restricted types.
                 *
                 * @@@@ Maybe this needs further checking
                 */
                if (TREE_CODE (source) == CALL_EXPR
                    && PASCAL_TYPE_RESTRICTED (TREE_TYPE (source)))
                  {
                    allow_restricted_source = TRUE;
                    allow_restricted_target = TRUE;

                    if (!PASCAL_TYPE_RESTRICTED (TREE_TYPE (target)))
                      {
                        error ("a restricted return value may only be assigned");
                        error (" to a restricted type object");
                      }
                  }

                if ((!allow_restricted_target
                     && PASCAL_TYPE_RESTRICTED (TREE_TYPE (target)))
                    || (!allow_restricted_source
                        && PASCAL_TYPE_RESTRICTED (TREE_TYPE (source))))
                  error ("assigning a restricted type object is not allowed");

                /* Sets.
                 *
                 * @@ Test new constructor code without this.
                 *
                 * Construct a set directly to the set variable.
                 */
                if (TREE_CODE (TREE_TYPE (target)) == SET_TYPE
                    && TREE_CODE (source) == CONSTRUCTOR)
                  source = construct_set (source, target, 0);

                if (source)
                  {
                    /* handle char, vstring and fstring mixing */
                    if (TREE_CODE (base_type (TREE_TYPE (target)))
                        == CHAR_TYPE)
                      source = string_may_be_char (source);
                    if (is_string_type (target, 0)
                        || is_string_type (source, 0))
                      assign_string (target, source);
                    else if (TREE_CODE (TREE_TYPE (target)) == SET_TYPE
                             && TREE_CODE (TREE_TYPE (target)) == SET_TYPE)
                      expand_expr_stmt (assign_set (target, source));
                    else
                      expand_expr_stmt (build_modify_expr (target, NOP_EXPR, source));
                  }
              }
            resume_function_calls ($<itype>2);
          } /* "if (! source) {} else {}" ends here */
      }
  ;

variable_or_function_access_maybe_assignment:
    identifier
      {
        tree id = $1;

        if (IS_ABSOLUTE_VAR (id))
          $$ = build_indirect_ref (DECL_INITIAL (id),
                                   "absolute variable access");
        else if (TREE_CODE (TREE_TYPE (id)) == REFERENCE_TYPE
                 && ! PASCAL_PROCEDURAL_VARIABLE (id))
          $$ = build_indirect_ref (id, "variable parameter reference");
        else
          $$ = id;

        /* Implicitly dereference schemata.
         */
        if (TREE_CODE (TREE_TYPE ($$)) == RECORD_TYPE
            && PASCAL_TYPE_SCHEMA (TREE_TYPE ($$)))
          $$ = build_component_ref ($$, schema_id);
      }
  | address_operator
      { $<itype>$ = suspend_function_calls (); }
    variable_or_function_access
      {
        if (PEDANTIC (B_D_PASCAL))
          pedwarn ("the address operator is a Borland Pascal extension");

        /* Special case where `@foo' can be an lvalue:
         * If `foo' is a procedure reference, `@foo' is a
         * type cast to a procedure pointer.
         */
        if (PASCAL_PROCEDURAL_VARIABLE ($3))
          {
            tree ftype;
            if ((flag_what_pascal & B_D_PASCAL) == 0)
              warning ("using address expression as lvalue");
            ftype = TREE_TYPE (TREE_TYPE ($3));
            $$ = convert (build_pointer_type (ftype), $3);
          }
        else
          {
            warning ("using address expression as lvalue");

            /* Don't call build_pascal_unary_op() which would
             * call the function.
             */
            $$ = build_unary_op (ADDR_EXPR, $3, 0);
          }
        resume_function_calls ($<itype>2);
      }
  | variable_or_function_access_no_id
  ;

rest_of_statement:
    /* Empty */
    { $$ = NULL_TREE; }
  | assign_operator expression
    { $$ = $2; }
  ;

assign_operator:
    LEX_ASSIGN
  | '='
      { error ("using `=' instead of `:=' in assignment"); }
  ;

standard_function_statement:
    p_HALT optional_par_actual_parameter
      {
        if (PEDANTIC (E_O_B_D_PASCAL))
          pedwarn ("`Halt' is an ISO 10206 Extended Pascal extension");
        if (PEDANTIC (B_D_PASCAL) && $2 != NULL_TREE)
          pedwarn ("parameters to `Halt' are an UCSD Pascal extension");
        build_rts_call (p_HALT, $2);
      }
  | rts_proc_zeropar
      { build_rts_call ($1, NULL_TREE); }
  | rts_proc_onepar '(' actual_parameter r_paren_or_error
      { build_rts_call ($1, build_tree_list(NULL_TREE, $3)); }
  | rts_proc_parlist '(' actual_parameter_list r_paren_or_error
      { build_rts_call ($1, $3); }
  | p_WRITE optional_par_write_parameter_list
      { build_rts_call (p_WRITE, $2); }
  | p_WRITELN optional_par_write_parameter_list
      { build_rts_call (p_WRITELN, $2); }
  | p_READ optional_par_actual_parameter_list
      { build_rts_call (p_READ, $2); }
  | p_READLN optional_par_actual_parameter_list
      { build_rts_call (p_READLN, $2); }
  | p_PAGE optional_par_actual_parameter_list
      { build_rts_call (p_PAGE, $2); }
  | ucsd_STR '(' write_actual_parameter_list r_paren_or_error
      { build_rts_call (ucsd_STR, $3); }
  /* Extended pascal write to string / read from string */
  | p_WRITESTR '(' write_actual_parameter_list r_paren_or_error
      { build_rts_call (p_WRITESTR, $3); }
  | p_READSTR '(' actual_parameter_list r_paren_or_error
      { build_rts_call (p_READSTR, $3); }
  | p_DISPOSE '(' actual_parameter r_paren_or_error
      { build_rts_call (p_DISPOSE, build_tree_list (NULL_TREE, $3)); }
  | p_DISPOSE '(' actual_parameter ','
      {
        if (TREE_CODE (TREE_TYPE ($3)) == POINTER_TYPE
            && PASCAL_TYPE_OBJECT (TREE_TYPE (TREE_TYPE ($3))))
          {
            tree temp = make_new_variable ("dispose_object", TREE_TYPE ($3));
            expand_expr_stmt (build_modify_expr (temp, NOP_EXPR, $3));
            $3 = temp;
            pascal_shadow_record_fields (build_indirect_ref ($3, "`Dispose'"), 1);
            /* $<ttype>$ = specialize_method_calls (TREE_TYPE ($3), 2); !!! */
          }
      }
    actual_parameter_list r_paren_or_error
      {
        if (TREE_CODE (TREE_TYPE ($3)) == POINTER_TYPE
            && PASCAL_TYPE_OBJECT (TREE_TYPE (TREE_TYPE ($3))))
          {
            /* resume_method_calls ($<ttype>5); !!! */
            expand_expr_stmt (TREE_VALUE ($6));
            poplevel (0, 0, 0);
            if (TREE_CHAIN ($6))
              warning ("extra arguments ignored in `Dispose'");
            build_rts_call (p_DISPOSE, build_tree_list (NULL_TREE, $3));
          }
        else
          build_rts_call (p_DISPOSE,
            chainon (build_tree_list (NULL_TREE, $3), $6));
      }
  ;

optional_par_write_parameter_list:
    /* empty */
      { $$ = NULL_TREE; }
  | '(' write_actual_parameter_list r_paren_or_error
      {
        $$ = $2;
        yyerrok;
      }
  ;

write_actual_parameter_list:
    write_actual_parameter
  | write_actual_parameter_list ',' write_actual_parameter
      {
        $$ = chainon ($1, $3);
        yyerrok;
      }
  | error
      {
        error ("missing parameter to `Write'");
        $$ = NULL_TREE;
      }
  | write_actual_parameter_list error write_actual_parameter
      {
        error ("missing comma");
        $$ = chainon ($1, $3);
        yyerrok;
      }
  | write_actual_parameter_list ',' error
      { error ("extra comma"); }
  ;


/* How to represent the `:' expressions?
 * Yes, as a list in TREE_PURPOSE of each actual parameter.
 */
write_actual_parameter:
    actual_parameter
      { $$ = build_tree_list (NULL_TREE, $1); }
  | actual_parameter ':' expression
      { $$ = build_tree_list (build_tree_list (NULL_TREE, $3), $1); }
  | actual_parameter ':' expression ':' expression
      { $$ = build_tree_list (build_tree_list ($5, $3), $1); }
  ;

/* run time system calls with no parameter */
rts_proc_zeropar:
    bp_RANDOMIZE
      { $$ = bp_RANDOMIZE; }
  | bp_RUNERROR
      { $$ = bp_RUNERROR; }
  ;

/* run time system calls with one parameter */
rts_proc_onepar:
    p_PUT
      { $$ = p_PUT; }
  | p_GET
      { $$ = p_GET; }
  | p_MARK
      { $$ = p_MARK; }
  | p_RELEASE
      { $$ = p_RELEASE; }
  | p_CLOSE
      { $$ = p_CLOSE; }
  | p_UPDATE
      { $$ = p_UPDATE; }
  | p_GETTIMESTAMP
      { $$ = p_GETTIMESTAMP; }
  | p_UNBIND
      { $$ = p_UNBIND; }
  | bp_FLUSH
      { $$ = bp_FLUSH; }
  | bp_ERASE
      { $$ = bp_ERASE; }
  | bp_CHDIR
      { $$ = bp_CHDIR; }
  | bp_MKDIR
      { $$ = bp_MKDIR; }
  | bp_RMDIR
      { $$ = bp_RMDIR; }
  | bp_RUNERROR
      { $$ = bp_RUNERROR; }
  ;

rts_proc_parlist:
    p_REWRITE     /* Up to three args */
      { $$ = p_REWRITE; }
  | p_RESET       /* Up to three args */
      { $$ = p_RESET; }
  | p_EXTEND      /* Up to three args */
      { $$ = p_EXTEND; }
  | bp_APPEND     /* Up to three args */
      { $$ = bp_APPEND; }
  | p_PACK        /* Three args */
      { $$ = p_PACK; }
  | p_UNPACK      /* Three args */
      { $$ = p_UNPACK; }
  | p_BIND        /* Two args */
      { $$ = p_BIND; }
  | ucsd_SEEK     /* Two args */
      { $$ = ucsd_SEEK; }
  | p_SEEKREAD
      { $$ = p_SEEKREAD; }
  | p_SEEKWRITE
      { $$ = p_SEEKWRITE; }
  | p_SEEKUPDATE
      { $$ = p_SEEKUPDATE; }
/* UCSD extensions */
  | ucsd_FILLCHAR /* Three args */
      { $$ = ucsd_FILLCHAR; }
  | ucsd_MOVELEFT /* Three args */
      { $$ = ucsd_MOVELEFT; }
  | ucsd_MOVERIGHT /* Three args */
      { $$ = ucsd_MOVERIGHT; }
  | ucsd_BLOCKREAD /* Three or four args */
      { $$ = ucsd_BLOCKREAD; }
  | ucsd_BLOCKWRITE /* Three or four args */
      { $$ = ucsd_BLOCKWRITE; }
  | ucsd_INSERT   /* Four args */
      { $$ = ucsd_INSERT; }
  | ucsd_DELETE   /* Three args */
      { $$ = ucsd_DELETE; }
/* Borland extensions */
  | bp_RENAME     /* Two args */
      { $$ = bp_RENAME; }
  | bp_MOVE       /* Three args */
      { $$ = bp_MOVE; }
  | bp_FREEMEM     /* Two args */
      { $$ = bp_FREEMEM; }
  | bp_INC         /* One or two args */
      { $$ = bp_INC; }
  | bp_DEC         /* One or two args */
      { $$ = bp_DEC; }
  | bp_VAL         /* Three args */
      { $$ = bp_VAL; }
  | bp_INCLUDE     /* Two args */
      { $$ = bp_INCLUDE; }
  | bp_EXCLUDE     /* Two args */
      { $$ = bp_EXCLUDE; }
/* GPC extension: Define random access file size */
  | p_DEFINESIZE  /* Two args */
      { $$ = p_DEFINESIZE; }
/* GPC extensions: `and', `or', `not', and `xor' as "procedures";
 * explicit assignment of object type; ...
 */
  | LEX_AND           /* Two args */
      { $$ = LEX_AND; }
  | LEX_OR            /* Two args */
      { $$ = LEX_OR; }
  | LEX_NOT           /* One arg */
      { $$ = LEX_NOT; }
  | LEX_XOR        /* Two args */
      { $$ = LEX_XOR; }
  | gpc_SETTYPE
      { $$ = gpc_SETTYPE; }
  | gpc_SETLENGTH
      { $$ = gpc_SETLENGTH; }
/* file procedures */
  | bp_ASSIGN     /* Two args */
      { $$ = bp_ASSIGN; }
  | bp_TRUNCATE   /* Two args */
      { $$ = bp_TRUNCATE; }
  | gpc_SETFILETIME       /* Two args */
      { $$ = gpc_SETFILETIME; }
  ;

/* statement extensions to ISO Pascal */
statement_extensions:
    return_statement
  | continue_statement
  | break_statement
  |
      { enable_keyword ("Volatile"); }
    asm_statement
      { disable_keyword ("Volatile"); }
  ;

return_statement:
    RETURN_
      {
        emit_line_note ($<filename>-1, $<lineno>0);
        if (pedantic || flag_what_pascal)
          pedwarn ("return statements are a GNU Pascal extension");
        c_expand_return (NULL_TREE);
      }
  | RETURN_ expression
      {
        emit_line_note ($<filename>-1, $<lineno>0);
        if (pedantic || flag_what_pascal)
          pedwarn ("return statements are a GNU Pascal extension");
        current_function_returns_value = 1;
        if (current_function_return_variable
            && (is_string_type (current_function_return_variable, 0)
                || is_string_type ($2, 0))
            && TREE_CODE (TREE_TYPE (current_function_return_variable))
               != CHAR_TYPE)
          {
            assign_string (current_function_return_variable, $2);
            PASCAL_VALUE_ASSIGNED (current_function_return_variable) = 1;
            c_expand_return (current_function_return_variable);
          }
        else
          {
            $2 = string_may_be_char ($2);
            if (current_function_return_variable)
              PASCAL_VALUE_ASSIGNED (current_function_return_variable) = 1;
            c_expand_return ($2);
          }
      }
  | EXIT
      {
        emit_line_note ($<filename>-1, $<lineno>0);
        if (PEDANTIC (U_B_D_PASCAL))
          pedwarn ("`exit' is an UCSD Pascal extension");
        c_expand_return (current_function_return_variable);
      }
  | FAIL
      {
        /* Check whether we are inside a constructor.
         */
        if (! PASCAL_CONSTRUCTOR_METHOD (current_function_decl))
          error ("`fail' called from outside a constructor");
        else
          {
            /* @@@@ `Fail' fails when called from a nested
             * subroutine of the constructor.
             */
            emit_line_note ($<filename>-1, $<lineno>0);
            if (PEDANTIC (B_D_PASCAL))
              pedwarn ("`fail' is a Borland Pascal extension");
            current_function_returns_value = 1;
            if (current_function_return_variable)
              PASCAL_VALUE_ASSIGNED (current_function_return_variable) = 1;
            c_expand_return (boolean_false_node);
          }
      }
  ;

break_statement:
    BREAK
      {
        emit_line_note ($<filename>-1, $<lineno>0);
        if (PEDANTIC (B_D_PASCAL))
          pedwarn ("`break' is a Borland Pascal extension");
        if (! expand_exit_something ())
          error ("`break' statement not within a loop or `case' statement");
      }
  ;

continue_statement:
    CONTINUE
      {
        emit_line_note ($<filename>-1, $<lineno>0);
        if (PEDANTIC (B_D_PASCAL))
          pedwarn ("`continue' is a Borland Pascal extension");
        if (! expand_continue_loop (0))       /* @@@ NP */
          error ("`continue' statement not within a loop");
      }
  ;

asm_statement:
    LEX_ASM asm_qualifier '(' string ')'
    {
      /* emit_line_note ($<filename>-1, $<lineno>0); */
      emit_line_note (input_filename, lineno);
      if (pedantic || flag_what_pascal)
        {
          if (flag_what_pascal & B_D_PASCAL)
            warning ("GPC and Borland Pascal have different `asm' syntax");
          else
            pedwarn ("`asm' is a Borland Pascal extension");
        }
      if (TREE_CHAIN ($4))
        $4 = asm_combine_strings ($4);
      expand_asm ($4);
    }
  /* This is the case with just output operands.  */
  | LEX_ASM asm_qualifier '(' string ':' asm_operands ')'
    {
      tree out_ops = $6;
      while (out_ops)
        {
          PASCAL_VALUE_ASSIGNED (TREE_VALUE (out_ops)) = 1;
          out_ops = TREE_CHAIN (out_ops);
        }
      /* emit_line_note ($<filename>-1, $<lineno>0); */
      emit_line_note (input_filename, lineno);
      if (pedantic || flag_what_pascal)
        {
          if (flag_what_pascal & B_D_PASCAL)
            warning ("GPC and Borland Pascal have different `asm' syntax");
          else
            pedwarn ("`asm' is a Borland Pascal extension");
        }
      if (TREE_CHAIN ($4))
        $4 = asm_combine_strings ($4);
      c_expand_asm_operands ($4, $6, NULL_TREE, NULL_TREE,
                             $2, input_filename, lineno);
    }
  /* This is the case with input operands as well.  */
  | LEX_ASM asm_qualifier '(' string ':' asm_operands ':' asm_operands ')'
    {
      tree out_ops = $6;
      while (out_ops)
        {
          PASCAL_VALUE_ASSIGNED (TREE_VALUE (out_ops)) = 1;
          out_ops = TREE_CHAIN (out_ops);
        }
      /* emit_line_note ($<filename>-1, $<lineno>0); */
      emit_line_note (input_filename, lineno);
      if (pedantic || flag_what_pascal)
        {
          if (flag_what_pascal & B_D_PASCAL)
            warning ("GPC and Borland Pascal have different `asm' syntax");
          else
            pedwarn ("`asm' is a Borland Pascal extension");
        }
      if (TREE_CHAIN ($4))
        $4 = asm_combine_strings ($4);
      c_expand_asm_operands ($4, $6, $8, NULL_TREE,
                             $2, input_filename, lineno);
    }
  /* This is the case with clobbered registers as well.  */
  | LEX_ASM asm_qualifier '(' string ':' asm_operands ':' asm_operands ':' asm_clobbers ')'
    {
      tree out_ops = $6;
      while (out_ops)
        {
          PASCAL_VALUE_ASSIGNED (TREE_VALUE (out_ops)) = 1;
          out_ops = TREE_CHAIN (out_ops);
        }
      /* emit_line_note ($<filename>-1, $<lineno>0); */
      emit_line_note (input_filename, lineno);
      if (pedantic || flag_what_pascal)
        {
          if (flag_what_pascal & B_D_PASCAL)
            warning ("GPC and Borland Pascal have different `asm' syntax");
          else
            pedwarn ("`asm' is a Borland Pascal extension");
        }
      if (TREE_CHAIN ($4))
        $4 = asm_combine_strings ($4);
      c_expand_asm_operands ($4, $6, $8, $10,
                             $2, input_filename, lineno);
    }
  ;

asm_qualifier:
    /* empty */
    { $$ = 0; }
  | LEX_VOLATILE
    { $$ = 1; }
  ;

/* These are the operands other than the first string and colon
   in  asm ("addextend %2,%1": "=dm" (x): "0" (y), "g" (*x))  */
asm_operands:
    /* empty */
    { $$ = NULL_TREE; }
  | nonnull_asm_operands
  ;

nonnull_asm_operands:
    asm_operand
  | nonnull_asm_operands ',' asm_operand
    { $$ = chainon ($1, $3); }
  ;

asm_operand:
    string_constant '(' expression ')'
    {
      if (TREE_CODE ($3) == INTEGER_CST)
        $3 = convert (integer_type_node, $3);
      $$ = build_tree_list ($1, $3);
    }
  ;

asm_clobbers:
    string
    { $$ = tree_cons (NULL_TREE, $1, NULL_TREE); }
  | asm_clobbers ',' string
    { $$ = tree_cons (NULL_TREE, $3, $1); }
  ;


/* For SizeOf, AlitnOf, TypeOf, New and GetMem */
variable_access_or_typename:
    variable_or_function_access_no_id
  | LEX_ID
    {
      if (lastiddecl && TREE_CODE (lastiddecl) == TYPE_DECL)
        $$ = lastiddecl;
      else
        {
          tree id = check_identifier (lastiddecl, $1);
          if (TREE_CODE (TREE_TYPE (id)) == REFERENCE_TYPE
              && TREE_CODE (TREE_TYPE (TREE_TYPE (id))) != FUNCTION_TYPE)
            $$ = build_indirect_ref (id, "variable parameter reference");
          else
            $$ = id;
        }
    }
  | LEX_FILE
    {
      /* Kind of special case of a typename.
       */
      tree d = get_identifier ("file");
      tree type = build_file_type (void_type_node, NULL_TREE);
      if (PEDANTIC (U_B_D_PASCAL))
        pedwarn ("untyped files are an UCSD Pascal extension");
      d = start_decl (d, tree_cons (NULL_TREE, type,
                           build_tree_list (NULL_TREE, type_id)),
                      0, NULL_TREE, NULL_TREE);
      finish_decl (d, NULL_TREE, NULL_TREE);
      $$ = d;
    }
  ;

discriminant_expression_list:
    expression
    {
      /* This expression might be a discriminant of another schema.
       */
      $$ = build_tree_list (NULL_TREE,
                            maybe_schema_discriminant (string_may_be_char ($1)));
    }
  | discriminant_expression_list ',' expression
    {
      $$ = chainon ($1, build_tree_list (NULL_TREE,
               maybe_schema_discriminant (string_may_be_char ($3))));
      yyerrok;
    }
  | error
    {
      error ("missing expression");
      $$ = NULL_TREE;
    }
  | discriminant_expression_list error expression
    {
      error ("missing comma");
      $$ = chainon ($1, build_tree_list (NULL_TREE,
               maybe_schema_discriminant (string_may_be_char ($3))));
      yyerrok;
    }
  | discriminant_expression_list ',' error
    { error ("extra comma"); }
  ;

index_expression_list:
    expression
    { $$ = build_tree_list (NULL_TREE, string_may_be_char ($1)); }
  | expression LEX_RANGE expression               /* SUBSTRING ACCESS */
    {
      if (PEDANTIC (E_O_PASCAL))
        pedwarn ("substring access is an ISO 10206 Extended Pascal extension");
      $$ = build_tree_list (string_may_be_char ($3),
                            string_may_be_char ($1));
    }
  | index_expression_list ',' expression
    {
      $$ = chainon ($1, build_tree_list (NULL_TREE,
                                         string_may_be_char ($3)));
      yyerrok;
    }
  | error
    {
      error ("missing expression");
      $$ = NULL_TREE;
    }
  | index_expression_list error expression
    {
      error ("missing comma");
      $$ = chainon ($1, build_tree_list (NULL_TREE,
                                         string_may_be_char ($3)));
      yyerrok;
    }
  | index_expression_list ',' error
    { error ("extra comma"); }
  ;


/* expressions */

/* We should check that this really is compile time constant */
static_expression:
    expression %prec PREC_INITIALIZER
  ;

/* The expression result must be type boolean.
 */
boolean_expression:
    expression
    {
      if (TREE_CODE (TREE_TYPE ($1)) != BOOLEAN_TYPE
          && TREE_CODE (TREE_TYPE ($1)) != ERROR_MARK)
        {
          error ("condition must be of Boolean type");
          $$ = error_mark_node;
        }
      else
        $$ = $1;
    }
  ;

expression:
    simple_expression relational_operator simple_expression
    { $$ = fold (build_pascal_binary_op ($2, $1, $3)); }
  | simple_expression LEX_IN simple_expression
    { $$ = fold (build_pascal_binary_op (IN_EXPR, $1, $3)); }
  | simple_expression
    { $$ = fold ($1); }
  ;

simple_expression:
    any_term
  | simple_expression adding_operator any_term
    { $$ = build_pascal_binary_op ($2, $1, $3); }
  | simple_expression pxsc_adding_operator any_term
    { $$ = build_pxsc_operator ($2, $1, $3); }
  | simple_expression LEX_OR any_term
    {
      enum tree_code code;
      if (TREE_CODE (TREE_TYPE ($1)) == INTEGER_TYPE)
        {
          code = BIT_IOR_EXPR;
          if (PEDANTIC (B_D_PASCAL))
            pedwarn ("bitwise `or' is a Borland Pascal extension");
        }
      else if (flag_short_circuit)
        code = TRUTH_ORIF_EXPR;
      else
        code = TRUTH_OR_EXPR;
      $$ = build_pascal_binary_op (code, $1, $3);
    }
  | simple_expression or_else any_term
    {
      if (PEDANTIC (E_O_PASCAL))
          pedwarn ("`or_else' is an ISO 10206 Extended Pascal extension");
      $$ = build_pascal_binary_op (TRUTH_ORIF_EXPR, $1, $3);
    }
  | simple_expression LEX_XOR any_term
    {
      enum tree_code code;
      if (PEDANTIC (B_D_PASCAL))
        pedwarn ("`xor' is a Borland Pascal extension");
      if (TREE_CODE (TREE_TYPE ($1)) == INTEGER_TYPE)
        code = BIT_XOR_EXPR;
      else
        code = TRUTH_XOR_EXPR;
      $$ = build_pascal_binary_op (code, $1, $3);
    }
  ;

or_else:
    LEX_OR LEX_ELSE
    {
      $$ = NULL_TREE;  /* Any tree value is fine */
      if (pedantic || flag_what_pascal)
        pedwarn ("`or else' is a GNU Pascal extension; ISO 10206 wants `or_else'");
    }
  | LEX_OR_ELSE
  ;

and_then:
    LEX_AND LEX_THEN
    {
      $$ = NULL_TREE;  /* Any tree value is fine */
      if (pedantic || flag_what_pascal)
        pedwarn ("`and then' is a GNU Pascal extension; ISO 10206 wants `and_then'");
    }
  | LEX_AND_THEN
  ;


any_term:
    term
  ;


term:
    primary
  | term multiplying_operator primary
    { $$ = build_pascal_binary_op ($2, $1, $3); }
  | term pxsc_multiplying_operator primary
    { $$ = build_pxsc_operator ($2, $1, $3); }
  | term LEX_SYMDIFF primary
    { $$ = build_pascal_binary_op (BIT_XOR_EXPR, $1, $3); }
  | term LEX_AND primary
    {
      enum tree_code code;
      if (TREE_CODE (TREE_TYPE ($1)) == INTEGER_TYPE)
        {
          code = BIT_AND_EXPR;
          if (PEDANTIC (B_D_PASCAL))
            pedwarn ("bitwise `and' is a Borland Pascal extension");
        }
      else if (flag_short_circuit)
        code = TRUTH_ANDIF_EXPR;
      else
        code = TRUTH_AND_EXPR;
      $$ = build_pascal_binary_op (code, $1, $3);
    }
  | term and_then primary
    { if (PEDANTIC (E_O_PASCAL))
        pedwarn ("`and_then' is an ISO 10206 Extended Pascal extension");
      $$ = build_pascal_binary_op (TRUTH_ANDIF_EXPR, $1, $3); }
  ;


/* The 10206 does not allow a SIGN before the exponent.  Only unsigned factors.
 * GPC used to allow these, but now it's like the standard says.
 */
primary:
    sign primary
    { $$ = build_pascal_unary_op ($1, $2, 0); }
  | factor
  | factor LEX_ID factor
    {
      /* A user-defined operator expression
       * is converted to a function call.
       */
      tree name = get_operator_identifier (IDENTIFIER_POINTER ($2), $1, $3);
      tree fun = name ? lookup_name (name) : NULL_TREE;
      tree arg = build_tree_list (NULL_TREE, $1);
      arg = chainon (arg, build_tree_list (NULL_TREE, $3));
      if (! fun)
        {
          if (lookup_name ($2) == error_mark_node)
            error ("invalid use of operator `%s'",
                   IDENTIFIER_POINTER ($2));
          else
            error ("syntax error before `%s'",
                   IDENTIFIER_POINTER ($2));
        }
      else
        $$ = build_function_call (fun, arg);
    }
  | factor LEX_POW factor
    {
      if (TREE_CODE (TREE_TYPE ($3)) != INTEGER_TYPE)
        {
          error ("`pow' exponent is not of integer type");
          $$ = error_mark_node;
        }
      else
        $$ = build_pascal_binary_op (EXPON_EXPR, $1, $3);
    }
  | factor LEX_POWER factor
    {
      tree exp = $3;
      if (TREE_CODE (TREE_TYPE (exp)) == INTEGER_TYPE)
        exp = convert (TREE_TYPE (real_zero_node),
                       exp);
      if (TREE_CODE (TREE_TYPE (exp)) != REAL_TYPE)
        {
          error ("`**' exponent is not of real or integer type");
          $$ = error_mark_node;
        }
      else
        $$ = build_pascal_binary_op (EXPON_EXPR, $1, exp);
    }
  | factor LEX_IS factor
    { error ("Object Pascal operator `is' not supported");
      $$ = error_mark_node;
    }
  ;

signed_factor:
    factor
  | sign signed_factor
    { $$ = build_pascal_unary_op ($1, $2, 0); }
  ;

factor:
    variable_or_function_access
    {
      if (TREE_CODE ($1) == TYPE_DECL)
        {
            error ("variable access expected - type name given");
            $$ = error_mark_node;
        }
      else if (MAYBE_CALL_FUNCTION ($1)
               && TREE_CODE (function_return_type ($1)) != VOID_TYPE)
        $$ = maybe_call_function ($1, NULL_TREE);
      else if (CALL_METHOD ($1))
        $$ = call_method ($1, NULL_TREE);
      else
        $$ = $1;
    }
  | constant_literal
  | unsigned_number
  | set_constructor
  | LEX_NOT signed_factor
    {
      /* @@@ Check later that the operand of `not' is of
       * Boolean type.
       */
      enum tree_code code;
      if (TREE_CODE (TREE_TYPE ($2)) == INTEGER_TYPE)
        {
          code = BIT_NOT_EXPR;
          if (PEDANTIC (B_D_PASCAL))
            pedwarn ("bitwise `not' is a Borland Pascal extension");
          $$ = build_pascal_unary_op (code, $2, 1);
        }
      else
        {
          code = TRUTH_NOT_EXPR;
          $$ = build_pascal_unary_op (code, $2, 0);
        }
    }
  | address_operator
      { $<itype>$ = suspend_function_calls (); }
    factor
      {
        if (PEDANTIC (B_D_PASCAL))
          pedwarn ("the address operator is a Borland Pascal extension");
        $$ = build_pascal_address_expression ($3);
        resume_function_calls ($<itype>2);
      }
/* More GNU Pascal extensions */
  /* Refer to the address of a label as a pointer.  */
  /* For computed `goto *' statement */
  | LEX_LABEL_ADDR label
      {
        tree label = lookup_label ($2);
        TREE_USED (label) = 1;
        $$ = build1 (ADDR_EXPR, ptr_type_node, label);
        TREE_CONSTANT ($$) = 1;
      }
  | CONJUGATE '(' expression ')'
      { $$ = build_pascal_unary_op (CONJ_EXPR, $3, 1); }
  | SIZEOF '(' variable_access_or_typename ')'
      {
        if (PEDANTIC (U_B_D_PASCAL))
          pedwarn ("`SizeOf' is an UCSD Pascal extension");
        if (TREE_CONSTANT ($3))
          warning ("`SizeOf' applied to a constant");

        /* Undo implicit schema dereferences.
         */
        if (TREE_CODE ($3) == COMPONENT_REF
            && TREE_CODE (TREE_OPERAND ($3, 1)) == FIELD_DECL
            && (DECL_NAME (TREE_OPERAND ($3, 1)) == schema_id
                || DECL_NAME (TREE_OPERAND ($3, 1)) == string_id))
          $3 = TREE_OPERAND ($3, 0);

        if (TREE_CODE ($3) == COMPONENT_REF
            && DECL_BIT_FIELD (TREE_OPERAND ($3, 1)))
          error ("`SizeOf' applied to a packed record field");

        /* Now $3 is either a variable access or a type declaration.
         * In both cases, TREE_TYPE ($3) carries the actual type.
         */
        if (PASCAL_TYPE_OBJECT (TREE_TYPE ($3)))
          {
            /* Read the size of the object at run time from the VMT.
             */
            tree vmt;
            if (TREE_CODE ($3) == TYPE_DECL)
              {
                char *name = concat ("vmt_", IDENTIFIER_POINTER (
                                   TYPE_LANG_NAME (TREE_TYPE ($3))), NULL_PTR);
                vmt = lookup_name (get_identifier (name));
                if (! vmt)
                  abort ();
                free (name);
              }
            else
              {
                vmt = build_component_ref ($3,
                                           get_identifier ("vmt"));
                vmt = build_indirect_ref (vmt, "`SizeOf'");
              }
            $$ = build_component_ref (vmt,
                                      get_identifier ("object_size"));
          }
        else
          {
            tree type = TREE_TYPE ($3);
            if (TREE_CODE ($3) == TYPE_DECL
                && PASCAL_TYPE_UNDISCRIMINATED_SCHEMA (type))
              {
                error ("`SizeOf' applied to an undiscriminated schema");
                $$ = integer_one_node;
              }
            else if (TREE_CODE ($3) == TYPE_DECL
                     && PASCAL_TYPE_UNDISCRIMINATED_STRING (type))
              {
                error ("`SizeOf' applied to an undiscriminated string");
                $$ = integer_one_node;
              }
            else
              $$ = c_sizeof (type);
          }
      }
  | BITSIZEOF '(' variable_access_or_typename ')'
      {
        if (PEDANTIC (U_B_D_PASCAL))
          pedwarn ("`BitSizeOf' is a GNU Pascal extension");
        if (TREE_CONSTANT ($3))
          warning ("`BitSizeOf' applied to a constant");

        /* Undo implicit schema dereferences.
         */
        if (TREE_CODE ($3) == COMPONENT_REF
            && TREE_CODE (TREE_OPERAND ($3, 1)) == FIELD_DECL
            && (DECL_NAME (TREE_OPERAND ($3, 1)) == schema_id
                || DECL_NAME (TREE_OPERAND ($3, 1)) == string_id))
          $3 = TREE_OPERAND ($3, 0);

        if (TREE_CODE ($3) == COMPONENT_REF
            && DECL_BIT_FIELD (TREE_OPERAND ($3, 1)))
          $$ = build_int_2 (DECL_FIELD_SIZE (TREE_OPERAND ($3, 1)), 0);
        else if (PASCAL_TYPE_OBJECT (TREE_TYPE ($3)))
          {
            /* Read the size of the object at run time from the VMT.
             */
            tree vmt = build_component_ref ($3,
                                            get_identifier ("vmt"));
            $$ = size_binop (MULT_EXPR,
                             build_component_ref (build_indirect_ref (vmt,
                                                                      "`BitSizeOf'"),
                                                  get_identifier ("object_size")),
                             build_int_2 (BITS_PER_UNIT, 0));
          }
        else
          {
            tree type = TREE_TYPE ($3);
            if (TREE_CODE ($3) == TYPE_DECL
                && PASCAL_TYPE_UNDISCRIMINATED_SCHEMA (type))
              {
                error ("`BitSizeOf' applied to an undiscriminated schema");
                $$ = integer_one_node;
              }
            else if (TREE_CODE ($3) == TYPE_DECL
                     && PASCAL_TYPE_UNDISCRIMINATED_STRING (type))
              {
                error ("`BitSizeOf' applied to an undiscriminated string");
                $$ = integer_one_node;
              }
            else
              $$ = bit_sizeof (type);
          }
      }
  | ALIGNOF '(' variable_access_or_typename ')'
      {
        if (pedantic || flag_what_pascal)
          pedwarn ("`AlignOf' is a GNU Pascal extension");
        if (TREE_CONSTANT ($3))
          warning ("`AlignOf' applied to a constant");

        /* Undo implicit schema dereferences.
         */
        if (TREE_CODE ($3) == COMPONENT_REF
            && TREE_CODE (TREE_OPERAND ($3, 1)) == FIELD_DECL
            && (DECL_NAME (TREE_OPERAND ($3, 1)) == schema_id
                || DECL_NAME (TREE_OPERAND ($3, 1)) == string_id))
          $3 = TREE_OPERAND ($3, 0);

        if (TREE_CODE ($3) == COMPONENT_REF
            && DECL_BIT_FIELD (TREE_OPERAND ($3, 1)))
          error ("`AlignOf' applied to a bit-field");

        if (TREE_CODE ($3) == INDIRECT_REF)
          {
            tree t = TREE_OPERAND ($3, 0);
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
            $$ = c_alignof (TREE_TYPE (TREE_TYPE (best)));
          }
        else
          $$ = c_alignof (TREE_TYPE ($3));
      }
  | bp_HIGH '(' variable_access_or_typename ')'
      {
        if (PEDANTIC (B_D_PASCAL))
          pedwarn ("`High' is a Borland Pascal extension");

        /* Implicitly dereference schemata.
         */
        if (TREE_CODE (TREE_TYPE ($3)) == RECORD_TYPE
            && PASCAL_TYPE_SCHEMA (TREE_TYPE ($3)))
          $3 = build_component_ref ($3, schema_id);

        if (TREE_CODE (TREE_TYPE ($3)) == RECORD_TYPE
            && PASCAL_TYPE_STRING (TREE_TYPE ($3)))
          $$ = PASCAL_STRING_CAPACITY ($3);
        else
          {
            tree type = TREE_TYPE ($3);
            if (TREE_CODE (type) == ARRAY_TYPE
                || TREE_CODE (type) == SET_TYPE)
              type = TYPE_DOMAIN (type);
            if (ORDINAL_TYPE (TREE_CODE (type)))
              {
                if TREE_TYPE (type)  /* subrange */
                  $$ = convert (TREE_TYPE (type), TYPE_MAX_VALUE (type));
                else
                  $$ = convert (type, TYPE_MAX_VALUE (type));
              }
            else
              {
                error ("invalid argument to `High'");
                $$ = error_mark_node;
              }
          }
      }
  | bp_LOW '(' variable_access_or_typename ')'
      {
        if (PEDANTIC (B_D_PASCAL))
          pedwarn ("`Low' is a Borland Pascal extension");

        /* Implicitly dereference schemata.
         */
        if (TREE_CODE (TREE_TYPE ($3)) == RECORD_TYPE
            && PASCAL_TYPE_SCHEMA (TREE_TYPE ($3)))
          $3 = build_component_ref ($3, schema_id);

        if (TREE_CODE (TREE_TYPE ($3)) == RECORD_TYPE
            && PASCAL_TYPE_STRING (TREE_TYPE ($3)))
          $$ = integer_one_node;
        else
          {
            tree type = TREE_TYPE ($3);
            if (TREE_CODE (type) == ARRAY_TYPE
                || TREE_CODE (type) == SET_TYPE)
              type = TYPE_DOMAIN (type);
            if (ORDINAL_TYPE (TREE_CODE (type)))
              {
                if TREE_TYPE (type)  /* subrange */
                  $$ = convert (TREE_TYPE (type), TYPE_MIN_VALUE (type));
                else
                  $$ = convert (type, TYPE_MIN_VALUE (type));
              }
            else
              {
                error ("invalid argument to `Low'");
                $$ = error_mark_node;
              }
          }
      }
  ;

address_operator:
    '&'
      {
        if (pedantic || flag_what_pascal)
          pedwarn ("the address operator `&' is a GNU Pascal extension");
      }
  | '@'
      {
        if (PEDANTIC (B_D_PASCAL))
          pedwarn ("the address operator is a Borland Pascal extension");
      }
  ;

variable_or_function_access:
    identifier
      {
        if (IS_ABSOLUTE_VAR ($1))
          $$ = build_indirect_ref (DECL_INITIAL ($1),
                                   "absolute variable access");
        else if (TREE_CODE (TREE_TYPE ($1)) == REFERENCE_TYPE
                 && TREE_CODE (TREE_TYPE (TREE_TYPE ($1))) != FUNCTION_TYPE)
          $$ = build_indirect_ref ($1, "variable parameter reference");
        else
          $$ = $1;

        /* Implicitly dereference schemata.
         */
        if (TREE_CODE (TREE_TYPE ($$)) == RECORD_TYPE
            && PASCAL_TYPE_SCHEMA (TREE_TYPE ($$)))
          $$ = build_component_ref ($$, schema_id);
      }
  | variable_or_function_access_no_id
      {
        $$ = $1;

        /* Implicitly dereference schemata.
         */
        if (TREE_CODE (TREE_TYPE ($$)) == RECORD_TYPE
            && PASCAL_TYPE_SCHEMA (TREE_TYPE ($$)))
          $$ = build_component_ref ($$, schema_id);
      }
  | standard_functions
  ;

variable_or_function_access_no_id:
    p_OUTPUT
      { $$ = get_standard_output (); }
  | p_INPUT
      { $$ = get_standard_input (); }
  | gpc_STDERR
      {
        if (pedantic || flag_what_pascal)
          pedwarn ("`StdErr' is a GNU Pascal extension");
        $$ = get_standard_error ();
      }
  | RESULT
      {
        tree target;
        if (PEDANTIC (BORLAND_DELPHI))
          pedwarn ("`Result' is a Delphi extension");
        target = lookup_name (get_identifier
                   (concat ("retval_",
                            IDENTIFIER_POINTER (DECL_NAME (current_function_decl)),
                            NULL_PTR)));
        if (! target)
          {
            error ("invalid assignment of function value");
            target = error_mark_node;
          }
        $$ = target;
      }
  | bp_FILEMODE
      { $$ = get_builtin_variable ("_p_filemode", integer_type_node); }
  | bp_INOUTRES
      { $$ = get_builtin_variable ("_p_inoutres", integer_type_node); }
  | gpc_INOUTRESSTR
      { $$ = get_builtin_variable ("_p_inoutres_str", cstring_type_node); }
  | bp_PI
      {
        $$ = build_real (long_double_type_node,
                         ereal_atof ("3.14159265358979323846264338327950288419716",
                                     TYPE_MODE (long_double_type_node)));
      }
  | variable_or_function_access '.' new_identifier
      {
        if (PASCAL_TYPE_RESTRICTED (TREE_TYPE ($1)))
          error ("accessing fields of a restricted type object is not allowed");
        if (is_schema_discriminant_access ($1, $3))
          $1 = TREE_OPERAND ($1, 0);
        $$ = build_component_ref ($1, $3);
        prediscriminate_schema ($$, 0);
      }
  | LEX_INHERITED new_identifier
      {
        tree target = IDENTIFIER_LOCAL_VALUE ($2);
        if (target && TREE_CODE (target) == COMPONENT_REF)
          {
            tree basetype = TREE_TYPE (TREE_OPERAND (target, 0));
            if (TYPE_LANG_BASE (basetype))
              basetype = TYPE_LANG_BASE (basetype);
            else
              error ("there is no parent type to inherit from");

            /* Get a TYPE_DECL instead of a type.
             */
            basetype = lookup_name (TYPE_LANG_NAME (basetype));
            $$ = build (COMPONENT_REF, TREE_TYPE (target),
                        basetype, TREE_OPERAND (target, 1));
          }
        else
          {
            error ("method not found");
            $$ = error_mark_node;
          }
      }
  | '(' expression ')'
      { $$ = $2; }
  | variable_or_function_access pointer_char
      { $$ = build_pascal_pointer_reference ($1); }
  | variable_or_function_access '[' index_expression_list ']'
      {
        if (PASCAL_TYPE_RESTRICTED (TREE_TYPE ($1)))
          error ("accessing an component of a restricted type object is not allowed");
        $$ = build_pascal_array_ref ($1, $3);
      }
  | variable_or_function_access '('
      {
        $<itype>2 = suspend_function_calls ();
        $$ = lastiddecl;
      }
    actual_parameter_list r_paren_or_error
      {
        resume_function_calls ($<itype>2);
        if ($<ttype>3 && TREE_CODE ($<ttype>3) == TYPE_DECL)
          {
            if (PEDANTIC (B_D_PASCAL))
               pedwarn ("type casts are a Borland Pascal extension");
            if (list_length ($4) != 1)
              {
                error ("type cast expects one expression argument");
                $$ = error_mark_node;
              }
            else
              {
                tree type =
                  groktypename (
                      build_tree_list (build_tree_list (NULL_TREE,
                                          TREE_TYPE ($<ttype>3)),
                                       DECL_NAME ($<ttype>3)));
                $$ = build_c_cast (type, TREE_VALUE ($4));
              }
          }
        else if (CALL_METHOD ($1))
          $$ = call_method ($1, $4);
        else
          $$ = build_function_call ($1, $4);
      }
  | LEX_FILE '(' expression ')'
      {
        /* Special case of a type cast.
         */
        tree type = build_file_type (void_type_node, NULL_TREE);
        if (PEDANTIC (B_D_PASCAL))
          {
            pedwarn ("type casts are a Borland Pascal extension");
            pedwarn ("untyped files are a UCSD Pascal extension");
          }
        $$ = build_c_cast (type, $3);
      }
  | p_NEW '(' variable_access_or_typename ')'
      {
        if (TREE_CODE (TREE_TYPE ($3)) == POINTER_TYPE
            && PASCAL_TYPE_OBJECT (TREE_TYPE (TREE_TYPE ($3)))
            && TREE_CODE ($3) == TYPE_DECL)
          {
            tree temp = start_decl (get_unique_identifier ("self", 1),
                                    build_tree_list (NULL_TREE, TREE_TYPE ($3)),
                                    0, NULL_TREE, NULL_TREE);
            finish_decl (temp, NULL_TREE, NULL_TREE);
            build_rts_call (p_NEW, build_tree_list (NULL_TREE, temp));
            $$ = temp;
          }
        else
          {
            tree temp = build_rts_call (p_NEW,
                            build_tree_list (NULL_TREE, $3));
            if (TREE_CODE ($3) == TYPE_DECL)
              $$ = temp;
            else
              $$ = error_mark_node;
          }
      }
  | p_NEW '(' variable_access_or_typename ','
      {
        $$ = NULL_TREE;
        if (TREE_CODE (TREE_TYPE ($3)) == POINTER_TYPE
            && PASCAL_TYPE_OBJECT (TREE_TYPE (TREE_TYPE ($3))))
          {
            tree self;
            if (TREE_CODE ($3) == TYPE_DECL)
              {
                tree temp = make_new_variable ("self", TREE_TYPE ($3));
                build_rts_call (p_NEW, build_tree_list (NULL_TREE, temp));
                self = build_indirect_ref (temp, "`New'");
                $$ = temp;
              }
            else
              {
                build_rts_call (p_NEW, build_tree_list (NULL_TREE, $3));
                self = build_indirect_ref ($3, "`New'");
                $$ = $3;
              }
            pascal_shadow_record_fields (self, 1);
          }
      }
    actual_parameter_list ')'
      {
        if (TREE_CODE (TREE_TYPE ($3)) == POINTER_TYPE
            && PASCAL_TYPE_OBJECT (TREE_TYPE (TREE_TYPE ($3))))
          {
            tree new_object;
            tree constructor_result = make_new_variable ("new_constructor_result",
                                                         boolean_type_node);

            if (TREE_CODE ($3) == TYPE_DECL)
              {
                new_object = make_new_variable ("new_object_ptr",
                                                TREE_TYPE ($<ttype>5));
                expand_expr_stmt (build_modify_expr (new_object,
                                                     NOP_EXPR, $<ttype>5));
              }
            else
              new_object = $3;

            /* Call the object's constructor as a "function"
             * and save the result on a temporary variable.
             */
            expand_expr_stmt (build_modify_expr (constructor_result,
                                                 NOP_EXPR, TREE_VALUE ($6)));

            /* Dispose the object and return `nil' if the
             * constructor returned `False'.
             */
            constructor_result = build_pascal_binary_op (EQ_EXPR,
                                                         constructor_result,
                                                         boolean_false_node);
            expand_start_cond (constructor_result, 0);
            build_rts_call (p_DISPOSE, build_tree_list (NULL_TREE, new_object));
            expand_expr_stmt (build_modify_expr (new_object,
                                                 NOP_EXPR, null_pointer_node));
            expand_end_cond ();
            poplevel (0, 0, 0);

            if (TREE_CODE ($3) == TYPE_DECL)
              $$ = new_object;
            else
              $$ = error_mark_node;
          }
        else
          {
            tree temp = build_rts_call (p_NEW,
                   chainon (build_tree_list (NULL_TREE, $3), $6));
            if (TREE_CODE ($3) == TYPE_DECL)
              $$ = temp;
            else
              $$ = error_mark_node;
          }
      }
  | bp_ADDR
      { $<itype>$ = suspend_function_calls (); }
    '(' factor ')'
      {
        if (PEDANTIC (B_D_PASCAL))
          pedwarn ("`Addr' is a Borland Pascal extension");
        $$ = build_pascal_address_expression ($4);
        resume_function_calls ($<itype>2);
      }
  | bp_ASSIGNED
      { $<itype>$ = suspend_function_calls (); }
    '(' factor ')'
      {
        $$ = build_rts_call (bp_ASSIGNED,
                             build_tree_list (NULL_TREE, $4));
        resume_function_calls ($<itype>2);
      }
  | bp_GETMEM '(' actual_parameter ')'
      {
        $$ = build_rts_call (bp_GETMEM,
                             build_tree_list (NULL_TREE, $3));
      }
  | bp_GETMEM '(' actual_parameter ',' actual_parameter ')'
      {
        build_rts_call (bp_GETMEM,
               chainon (build_tree_list (NULL_TREE, $3),
                        build_tree_list (NULL_TREE, $5)));
        $$ = NULL_TREE;
      }
  | TYPEOF '(' variable_access_or_typename ')'
      {
        if (PEDANTIC (B_D_PASCAL))
          pedwarn ("`TypeOf' is a Borland Pascal extension");

        if (PASCAL_TYPE_OBJECT (TREE_TYPE ($3)))
          {
            tree result;

            if (TREE_CODE ($3) == TYPE_DECL)
              {
                /* Return a pointer to the VMT of this type.
                 */
                tree vmt = lookup_name (get_identifier (concat ("vmt_",
                               IDENTIFIER_POINTER (DECL_NAME ($3)), NULL_PTR)));
                result = build_pascal_unary_op (ADDR_EXPR, vmt, 0);
              }
            else
              {
                /* Return the implicit VMT field.
                 */
                result = build_component_ref ($3, get_identifier ("vmt"));
              }
            $$ = build1 (NON_LVALUE_EXPR, ptr_type_node, result);
          }
        else
          {
            error ("`TypeOf' applied to something not an object");
            $$ = error_mark_node;
          }
      }
  | gpc_RETURNADDRESS '(' expression ')'
      {
        if (pedantic || flag_what_pascal)
          pedwarn ("`ReturnAddress' is a GPC extension");
        $$ = build_function_call (lookup_name (get_identifier ("__builtin_return_address")),
                                  build_tree_list (NULL_TREE, $3));
      }
  | gpc_FRAMEADDRESS '(' expression ')'
      {
        if (pedantic || flag_what_pascal)
          pedwarn ("`FrameAddress' is a GPC extension");
        $$ = build_function_call (lookup_name (get_identifier ("__builtin_frame_address")),
                                  build_tree_list (NULL_TREE, $3));
      }
  ;

/* SET CONSTRUCTOR,
 * save some work with a small kludge.
 */
set_constructor:
    '[' set_constructor_element_list ']'
      { $$ = build_set_constructor ($2); }
  ;

set_constructor_element_list:
    /* empty */
      { $$ = NULL_TREE; }
  | member_designator
  | set_constructor_element_list ',' member_designator
      {
        if ($3 != NULL_TREE)
          $$ = chainon ($1, $3);
        yyerrok;
      }
  | set_constructor_element_list error member_designator
      {
        error ("missing comma");
        if ($3 != NULL_TREE)
          $$ = chainon ($1, $3);
        yyerrok;
      }
  | set_constructor_element_list ',' error
      { error ("extra comma"); }
  ;

/* Returns a TREE_LIST node with TREE_PURPOSE as the range low bound,
 * and TREE_VALUE as the range upper bound.
 */
member_designator:
    expression
      { $$ = construct_set_member ($1, NULL_TREE); }
  | expression LEX_RANGE expression
      { $$ = construct_set_member ($1, $3);}
  ;

standard_functions:
    rts_fun_zeropar
      { $$ = build_rts_call ($1, NULL_TREE); }
  | rts_fun_onepar '(' actual_parameter ')'
      { $$ = build_rts_call ($1, build_tree_list (NULL_TREE, $3)); }
  | rts_fun_twopar '(' actual_parameter ',' actual_parameter ')'
      {
        $$ = build_rts_call ($1,
                    chainon (build_tree_list (NULL_TREE, $3),
                             build_tree_list (NULL_TREE, $5)));
      }
  | rts_fun_optpar optional_par_actual_parameter
      { $$ = build_rts_call ($1, $2); }
  | rts_fun_parlist '(' actual_parameter_list ')'
      { $$ = build_rts_call ($1, $3); }
  ;


optional_par_actual_parameter:
    /* empty */
      { $$ = NULL_TREE; }
  |  '(' actual_parameter ')'
      { $$ = build_tree_list (NULL_TREE, $2); }
  ;

rts_fun_zeropar:
    ucsd_IORESULT
      { $$ = ucsd_IORESULT; }
  | bp_PARAMCOUNT
      { $$ = bp_PARAMCOUNT; }
  ;

rts_fun_optpar:
    p_EOF
      { $$ = p_EOF; }
  | p_EOLN
      { $$ = p_EOLN; }
  | bp_RANDOM
      { $$ = bp_RANDOM; }
  ;

rts_fun_onepar:
/* arithmetic functions */
    p_ABS
      { $$ = p_ABS; }
  | p_SQR
      { $$ = p_SQR; }
  | p_SIN
      { $$ = p_SIN; }
  | p_COS
      { $$ = p_COS; }
  | p_EXP
      { $$ = p_EXP; }
  | p_LN
      { $$ = p_LN; }
  | p_SQRT
      { $$ = p_SQRT; }
  | p_ARCTAN
      { $$ = p_ARCTAN; }
  | p_ARG
      { $$ = p_ARG; }
  | p_RE
      { $$ = p_RE; }
  | p_IM
      { $$ = p_IM; }
  | ucsd_INT
      { $$ = ucsd_INT; }
  | bp_FRAC
      { $$ = bp_FRAC; }
/* transfer functions */
  | p_TRUNC
      { $$ = p_TRUNC; }
  | p_ROUND
      { $$ = p_ROUND; }
  | p_CARD
      { $$ = p_CARD; }
  | gpc_UPCASE
      { $$ = gpc_UPCASE; }
  | gpc_LOCASE
      { $$ = gpc_LOCASE; }
/* ordinal functions */
  | p_ORD
      { $$ = p_ORD; }
  | p_CHR
      { $$ = p_CHR; }
/* boolean functions */
  | p_ODD
      { $$ = p_ODD; }
  | p_EMPTY
      { $$ = p_EMPTY; }
/* Direct access position functions */
  | p_POSITION
      { $$ = p_POSITION; }
  | p_LASTPOSITION
      { $$ = p_LASTPOSITION; }
  | bp_FILEPOS
      { $$ = bp_FILEPOS; }
  | bp_FILESIZE
      { $$ = bp_FILESIZE; }
/* String functions */
  | p_LENGTH
      { $$ = p_LENGTH; }
  | p_TRIM
      { $$ = p_TRIM; }
  | gpc_NEWCSTRING
      { $$ = gpc_NEWCSTRING; }
  | gpc_CSTRING2STRING
      { $$ = gpc_CSTRING2STRING; }
  | gpc_STRING2CSTRING
      { $$ = gpc_STRING2CSTRING; }
/* Binding function */
  | p_BINDING
      { $$ = p_BINDING; }
/* time functions */
  | p_DATE
      { $$ = p_DATE; }
  | p_TIME
      { $$ = p_TIME; }
/* command-line parameters */
  | bp_PARAMSTR
      { $$ = bp_PARAMSTR; }
  ;

rts_fun_twopar:
    gpc_MAX
      { $$ = gpc_MAX; }
  | gpc_MIN
      { $$ = gpc_MIN; }
/* Complex functions */
  | p_CMPLX
      { $$ = p_CMPLX; }
  | p_POLAR
      { $$ = p_POLAR; }
/* String functions */
  | p_INDEX
      { $$ = p_INDEX; }
  | p_EQ
      { $$ = p_EQ; }
  | p_LT
      { $$ = p_LT; }
  | p_GT
      { $$ = p_GT; }
  | p_NE
      { $$ = p_NE; }
  | p_LE
      { $$ = p_LE; }
  | p_GE
      { $$ = p_GE; }
  | ucsd_POS
      { $$ = ucsd_POS; }
  | gpc_CSTRINGCOPYSTRING
      { $$ = gpc_CSTRINGCOPYSTRING; }
  ;

rts_fun_parlist:
    p_SUCC        /* One or two args */
      { $$ = p_SUCC; }
  | p_PRED        /* One or two args */
      { $$ = p_PRED; }
  | p_SUBSTR      /* Two or three args */
      { $$ = p_SUBSTR; }
  | ucsd_CONCAT   /* One or more args */
      { $$ = ucsd_CONCAT; }
  | ucsd_COPY     /* Three args */
      { $$ = ucsd_COPY; }
  ;

/* operator definitions */

relational_operator:
    LEX_NE
      { $$ = NE_EXPR; }
  | LEX_LE
      { $$ = LE_EXPR; }
  | LEX_GE
      { $$ = GE_EXPR; }
  | '='
      { $$ = EQ_EXPR; }
  | '<'
      { $$ = LT_EXPR; }
  | '>'
      { $$ = GT_EXPR; }
  ;

multiplying_operator:
    LEX_DIV
      { $$ = TRUNC_DIV_EXPR; }
  | LEX_MOD
      { $$ = TRUNC_MOD_EXPR; }
  | '/'
      { $$ = RDIV_EXPR; }
  | '*'
      { $$ = MULT_EXPR; }
  | LEX_SHL
      {
        $$ = LSHIFT_EXPR;
        if (PEDANTIC (B_D_PASCAL))
          pedwarn ("`shl' is a Borland Pascal extension");
      }
  | LEX_SHR
      {
        $$ = RSHIFT_EXPR;
        if (PEDANTIC (B_D_PASCAL))
          pedwarn ("`shr' is a Borland Pascal extension");
      }
  ;

pxsc_multiplying_operator:
    LEX_CEIL_MULT
      { $$ = "ceilmult"; }
  | LEX_CEIL_DIV
      { $$ = "ceilrdiv"; }
  | LEX_FLOOR_MULT
      { $$ = "floormult"; }
  | LEX_FLOOR_DIV
      { $$ = "floorrdiv"; }
  ;

adding_operator:
    '-'
      { $$ = MINUS_EXPR; }
  | '+'
      { $$ = PLUS_EXPR; }
  ;

pxsc_adding_operator:
    LEX_CEIL_PLUS
      { $$ = "ceilplus"; }
  | LEX_CEIL_MINUS
      { $$ = "ceilminus"; }
  | LEX_FLOOR_PLUS
      { $$ = "floorplus"; }
  | LEX_FLOOR_MINUS
      { $$ = "floorminus"; }
  ;

semi:
    ';'
      { /* No yyerrok here. */ }
  ;

optional_semicolon:
    /* empty */
      { yyerrok; }
  | ';'
      { yyerrok; }
  ;

/*
optional_colon:
     * empty *
  | ':'
  ;
*/

/* These are support states, not pascal syntax */

pushlevel:
    /* empty */
      {
        emit_line_note (input_filename, lineno);
        pushlevel (0);
        clear_last_expr ();
        expand_start_bindings (0);
      }
  ;

/* Do not push_momentary() */
pushlevel1:
    /* empty */
      {
        emit_line_note (input_filename, lineno);
        pushlevel (0);
        clear_last_expr ();
        expand_start_bindings (0);
      }
  ;

/* @@@ Note that this does not return the value of poplevel().
 * GCC uses that value to handle braced-group expressions, which
 * are not implemented in GPC (yet :-))
 */
poplevel:
  poplevel1
  ;

poplevel1:
    /* empty */
      {
        tree decls = getdecls ();
        emit_line_note (input_filename, lineno);
        if (decls != NULL_TREE)
          {
            expand_end_bindings (decls, 1, 0);
            poplevel (1, 1, 0);
          }
        else
          {
            expand_end_bindings (decls, kept_level_p (), 0);
            poplevel (kept_level_p (), 0, 0);
          }
      }
  ;

setspecs:
    /* empty */
      { $$ = suspend_momentary (); }
  ;

/*
 * The EXTENDED PASCAL Module support below is currently under construction.
 */

module_declaration:
    LEX_MODULE
      {
        enable_keyword ("Interface");
        enable_keyword ("Implementation");
      }
    new_identifier
      {
        initialize_module ($3, 0);
        if (PEDANTIC (E_O_PASCAL))
          pedwarn ("modules are an ISO 10206 Extended Pascal extension");
        pushlevel (0);
      }
    rest_of_module
      {
        poplevel (0, 0, 0);
        extend_gpi_files ();
        finalize_module (current_module);
        current_module = NULL_MODULE;
      }
  | LEX_UNIT
      {
        enable_keyword ("Interface");
        enable_keyword ("Implementation");
      }
    new_identifier semi
      {
        initialize_module ($3, 0);
        this_is_an_interface_module = 0;
        if (PEDANTIC (B_D_PASCAL))
          pedwarn ("units are an UCSD Pascal extension");
        pushlevel (0);
      }
    rest_of_unit
      {
        poplevel (0, 0, 0);
        extend_gpi_files ();
        finalize_module (current_module);
        current_module = NULL_MODULE;
      }
  ;

rest_of_unit:
    LEX_INTERFACE
      {
        this_is_an_interface_module = 1;
        export_interface (current_module->name, export_all);
      }
    unit_interface LEX_IMPLEMENTATION
      {
        module_repair_exported_ranges ();
        create_gpi_files ();
        if (flag_interface_only)
          exit_compilation ();
        this_is_an_interface_module = 0;
        /* handle_pending_decls (); */
      }
    unit_implementation optional_unit_constructor LEX_END
  ;

unit_interface:
      { do_extra_import (); }
    import_part                           /* This may be empty */
    any_module_decl_part                  /*  as well as this */
  ;

unit_implementation:
    import_part                           /* This may be empty */
    any_declaration_part
  ;

rest_of_module:
    optional_module_parameters module_interface semi
      {
        module_repair_exported_ranges ();
        create_gpm_file ();
        create_gpi_files ();
        this_is_an_interface_module = 0;
        /* handle_pending_decls (); */
      }
    module_block
  | LEX_INTERFACE optional_module_parameters module_interface
      {
        if (current_module->interface)
          error ("Module `%s' already has an interface part",
                 IDENTIFIER_POINTER (current_module->name));
        else
          current_module->interface = 1;
        module_repair_exported_ranges ();
        create_gpm_file ();
        create_gpi_files ();
        this_is_an_interface_module = 0;
        /* handle_pending_decls (); */
      }
  | LEX_IMPLEMENTATION semi
      {
        if (current_module->implementation)
          error ("Module `%s' already has an implementation part",
                 IDENTIFIER_POINTER (current_module->name));
        else
          current_module->implementation = 1;

        if (load_gpm_file () != 0
            && !current_module->interface)
          pedwarn ("module `%s' has no interface module",
                   IDENTIFIER_POINTER (current_module->name));

        this_is_an_interface_module = 0;
      }
    module_block
  | optional_module_parameters
    import_or_any_declaration_part
      {
        if (pedantic || flag_what_pascal)
          pedwarn ("GPC specific module declaration");
      }
    optional_init_and_final_part
    LEX_END
  ;

optional_module_parameters:
    optional_par_id_list semi
    {
      current_module->parms = $1;
      associate_external_objects (current_module->parms);
    }
  ;

module_interface:
    LEX_EXPORT
    {
      this_is_an_interface_module = 1;
      do_extra_import ();
    }
    export_part_list semi         /* LEX_EXPORT part is never empty */
    import_part                   /* This may be empty */
    any_module_decl_part          /* as well as this */
    LEX_END
  ;

any_module_decl_part:
    /* empty */
  | any_module_decl_list
  ;

any_module_decl_list:
    any_module_decl_1
  | any_module_decl_list any_module_decl_1
  ;

any_module_decl_1:
    any_module_decl
  ;

any_module_decl:
    simple_decl
  | function_interface_decl
  ;

/*
 * If there is a user specified directive, use that. If not:
 *
 * Exported function interface declares a function as it would
 * have directive FORWARD attached.  It does not matter if it
 * is not declared in the same file (hhen treated as `external'
 * directive).
 *
 * If the function has not been exported, treat it as static.
 */
function_interface_decl:
    function_heading
      {
        enable_keyword ("External");
        enable_keyword ("Asmname");
        enable_keyword ("Attribute");
      }
    semi optional_directive_list
      {
        tree heading = $1;
        tree dir = $4;

        handle_autoexport (TREE_OPERAND (TREE_PURPOSE (heading), 0));
        if (dir == NULL_TREE)
          {
            if (name_exported_p (TREE_OPERAND (TREE_PURPOSE (heading), 0)))
              dir = build_tree_list (NULL_TREE, d_forward);
            else
              dir = build_tree_list (NULL_TREE, d_static);
          }
/*
        if (this_is_an_interface_module
            && dir == d_static
            && ! we_are_loading_a_gpi_file)
          {
            current_module->pending_decls =
              chainon (current_module->pending_decls,
                       build_tree_list (void_type_node,
                                        build_tree_list (dir,
                                                         heading)));
          }
        else
*/
          grok_directive (TREE_VALUE (heading), TREE_PURPOSE (heading), dir, 0);
        disable_keyword ("External");
        disable_keyword ("Asmname");
        disable_keyword ("Attribute");
      }
  ;

optional_directive_list:
    /* empty */
      { $$ = NULL_TREE; }
  | directive_list semi
      {
        if (is_known_directive ($1))
          $$ = $1;
        else
          $$ = NULL_TREE;
      }
  ;

/* The module block may be empty until LEX_END */
module_block:
    import_or_any_declaration_part
    optional_init_and_final_part
    LEX_END
  ;

module_constructor:
    LEX_TO LEX_BEGIN LEX_DO
    {
      tree context;

      char *name_buffer = concat ("init_",
               IDENTIFIER_POINTER (current_module->name), NULL_PTR);
      tree name = $<ttype>$ = get_identifier (name_buffer);
      tree parm = no_parameters ();

      free (name_buffer);

      $<ttype>2 = getdecls ();

      if (PEDANTIC (E_O_PASCAL))
        pedwarn ("`to begin do' is an ISO 10206 Extended Pascal extension");
      context = build_nt (CALL_EXPR, name, parm, NULL_TREE);

      if (! start_function (build_tree_list (NULL_TREE, void_type_node),
                            context,
                            NULL_TREE, NULL_TREE,
                            0))
        YYERROR1;

      store_parm_decls ();
    }
    pushlevel1
    {
      start_constructor ();
      un_initialize_block ($<ttype>2, 0);
    }
    lineno_statement semi
    {
      un_initialize_block (getdecls (), 1);
      finish_constructor ();
    }
    poplevel1
    {
      mark_addressable (lookup_name ($<ttype>5));
      finish_function (0);
    }
  ;

implicit_module_constructor:
    /* empty */
      {
        tree context;

        char *name_buffer = concat ("init_",
                 IDENTIFIER_POINTER (current_module->name), NULL_PTR);
        tree name = get_identifier (name_buffer);
        tree decls = getdecls ();
        tree parm = no_parameters ();

        char *save_filename = input_filename;
        input_filename = DUMMY_FILENAME;

        free (name_buffer);

        context = build_nt (CALL_EXPR, name, parm, NULL_TREE);

        if (! start_function (build_tree_list (NULL_TREE, void_type_node),
                              context,
                              NULL_TREE, NULL_TREE,
                              0))
          YYERROR1;

        store_parm_decls ();
      
        emit_line_note (input_filename, lineno);
        pushlevel (0);
        clear_last_expr ();
        expand_start_bindings (0);

        start_constructor ();
        un_initialize_block (decls, 0);
        finish_constructor ();

        decls = getdecls ();
        emit_line_note (input_filename, lineno);
        if (decls != NULL_TREE)
          {
            expand_end_bindings (decls, 1, 0);
            poplevel (1, 1, 0);
          }
        else
          {
            expand_end_bindings (decls, kept_level_p (), 0);
            poplevel (kept_level_p (), 0, 0);
          }

        mark_addressable (lookup_name (name));
        finish_function (0);
        input_filename = save_filename;
      }
  ;

implicit_module_destructor:
    /* empty */
      {
        tree context;

        char *name_buffer = concat ("fini_",
                 IDENTIFIER_POINTER (current_module->name), NULL_PTR);
        tree name = get_identifier (name_buffer);
        tree parm = no_parameters ();

        char *save_filename = input_filename;
        input_filename = DUMMY_FILENAME;

        free (name_buffer);

        context = build_nt (CALL_EXPR, name, parm, NULL_TREE);

        if (! start_function (chainon (build_tree_list (NULL_TREE, static_id),
                                       build_tree_list (NULL_TREE, void_type_node)),
                              context,
                              NULL_TREE, NULL_TREE,
                              0))
          YYERROR1;

        store_parm_decls ();
        mark_addressable (lookup_name (name));
        finish_function (0);
        input_filename = save_filename;
      }
  ;

optional_unit_constructor:
    implicit_module_constructor implicit_module_destructor
  | LEX_BEGIN
      {
        tree context;

        char *name_buffer = concat ("init_",
                 IDENTIFIER_POINTER (current_module->name), NULL_PTR);
        tree name = $<ttype>$ = get_identifier (name_buffer);
        tree parm = no_parameters ();

        free (name_buffer);

        $<ttype>1 = getdecls ();

        context = build_nt (CALL_EXPR, name, parm, NULL_TREE);

        if (! start_function (build_tree_list (NULL_TREE, void_type_node),
                              context,
                              NULL_TREE, NULL_TREE,
                              0))
          YYERROR1;

        store_parm_decls ();
      }
    pushlevel1
      {
        start_constructor ();
        un_initialize_block ($<ttype>1, 0);
      }
    statement_sequence
      {
        un_initialize_block (getdecls (), 1);
        finish_constructor ();
      }
    poplevel1
      {
        mark_addressable (lookup_name ($<ttype>2));
        finish_function (0);
      }
    implicit_module_destructor
  /* GPC extension: also allow ISO-style constructors and
   * destructors in units.
   */
  | module_constructor module_destructor
  | module_constructor implicit_module_destructor
  | module_destructor implicit_module_constructor
  ;

module_destructor:
    LEX_TO LEX_END LEX_DO
    {
      tree context;

      char *name_buffer = concat ("fini_",
               IDENTIFIER_POINTER (current_module->name), NULL_PTR);
      tree name = $<ttype>$ = get_identifier (name_buffer);
      tree parm = no_parameters ();

      free (name_buffer);

      context = build_nt (CALL_EXPR, name, parm, NULL_TREE);

      if (PEDANTIC (E_O_PASCAL))
        pedwarn ("`to end do' is an ISO 10206 Extended Pascal extension");
      if (! start_function (build_tree_list (NULL_TREE, void_type_node),
                            context, NULL_TREE, NULL_TREE, 0))
        YYERROR1;

      store_parm_decls ();
    }
    pushlevel1
    lineno_statement semi
    { un_initialize_block (getdecls (), 1); }
    poplevel1
    {
      mark_addressable (lookup_name ($<ttype>4));
      finish_function (0);
    }
  ;

/* Allow both, either or none of these. `to begin do' comes always first. */
optional_init_and_final_part:
    implicit_module_constructor implicit_module_destructor
  | module_constructor module_destructor
  | module_constructor implicit_module_destructor
  | module_destructor implicit_module_constructor
  ;

export_part_list:
    export_part
  | export_part_list semi export_part
    { yyerrok; }
  | error
    { error ("module specifications need an export part"); }
  | export_part_list error export_part
    {
      warning ("missing semicolon");
      yyerrok;
    }
  | export_part_list semi error
    { error ("extra semicolon"); }
  ;

export_part:
    new_identifier
    {
      enable_keyword ("Protected");
      enable_keyword ("All");
    }
    optional_equal_sign export_list_or_all
    {
      export_interface ($1, $4);
      disable_keyword ("Protected");
      disable_keyword ("All");
    }
  ;

optional_equal_sign:
    /* Empty */
    { warning ("missing `=' after export interface identifier"); }
  | '='
    { }
  ;

export_list_or_all:
    '(' export_list ')'
    { $$ = $2; }
  | LEX_ALL
    { $$ = export_all; }
  ;

export_list:
    export_list_item
  | export_list ',' export_list_item
    { $$ = chainon ($1, $3); }
  | error
    { $$ = NULL_TREE; }
  | export_list error export_list_item
    { $$ = NULL_TREE; }
  | export_list ',' error
    { $$ = NULL_TREE; }
  ;

export_list_item:
    new_identifier rest_of_export_item
    { if ($2)
        {
          if (TREE_CODE ($2) == TREE_LIST)
            $$ = module_export_range ($1, TREE_VALUE ($2));
          else
            $$ = module_export_clause ($1, $2, 0);
        }
      else
        $$ = module_export_clause ($1, NULL_TREE, 0);
    }
  | LEX_PROTECTED new_identifier optional_rename
    { $$ = module_export_clause ($2, $3, 1); }
  ;

/* Identifiers must be CONSTANT NAMES */
rest_of_export_item:
    optional_rename
  | LEX_RANGE new_identifier
    { $$ = build_tree_list (NULL_TREE, $2); }
  ;

optional_rename:
    /* empty */
    { $$ = NULL_TREE; }
  | LEX_RENAME new_identifier
    { $$ = $2; }
  ;

import_part:
    /* empty */
  | LEX_IMPORT import_specification_list semi
    { }
  | LEX_USES uses_list semi
    { }
  ;

/* Removed: | import_specification_list semi error
 */
import_specification_list:
    import_specification
  | import_specification_list semi import_specification
  | import_specification_list error import_specification
    {
      warning ("missing semicolon");
      yyerrok;
    }
  ;

uses_list:
    import_specification
  | uses_list ',' import_specification
  | uses_list error import_specification
    {
      warning ("missing comma");
      yyerrok;
    }
  ;

import_specification:
    new_identifier optional_access_qualifier optional_import_qualifier
    optional_unit_filename
    {
      /* Recovering from an error if id is NULL_TREE.
       * If $1 is NULL_TREE, lastiddecl is one of the
       * predefined identifiers, but it does not matter
       * anymore.
       */
      if ($1)
        import_interface ($1, $3, $2 != NULL_TREE, $4);
    }
  ;

optional_access_qualifier:
    /* Empty */
    { $$ = NULL_TREE; }
  | LEX_QUALIFIED
  ;

optional_import_qualifier:
    /* Empty */
    { $$ = NULL_TREE; }
  | '(' import_clause_list ')'
    { $$ = build_tree_list (NULL_TREE, $2); }
  | LEX_ONLY '(' import_clause_list ')'
    { $$ = build_tree_list ($3, $3); }
  ;

optional_unit_filename:
    /* Empty */
    { $$ = NULL_TREE; }
  | LEX_IN string_constant
    {
      if (PEDANTIC (BORLAND_DELPHI))
        pedwarn ("file name specification with `in' is a Delphi extension");
      $$ = $2;
    }
  ;

import_clause_list:
    import_clause
  | import_clause_list ',' import_clause
    { $$ = chainon ($1, $3); }
  | error
    { $$ = NULL_TREE; }
  | import_clause_list error import_clause
    { warning ("missing comma");
      $$ = chainon ($1, $3);
    }
  | import_clause_list ',' error
    { $$ = NULL_TREE; }
  ;

import_clause:
    new_identifier optional_rename
    { $$ = build_tree_list ($1, $2); }
  ;

%%
/* Lexical analyzer moved to gpc-lex.c */

/* Sets the value of the 'yydebug' variable to VALUE.
   This is a function so we don't have to have YYDEBUG defined
   in order to build the compiler.  */
void
set_yydebug (value)
     int value;
{
#if YYDEBUG != 0
  yydebug = value;
#else
  warning ("YYDEBUG not defined.");
#endif
}

/*
Local variables:
mode:c
tab-width: 8
version-control: t
End:
*/
