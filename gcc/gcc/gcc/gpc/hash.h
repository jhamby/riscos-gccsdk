/* C code produced by gperf version 2.7 */
/* Command-line: gperf -p -j1 -G -o -t -N is_reserved_word -k1,2,$  */ 
/*
GPC keywords for the various dialects.

Copyright (C) 1989, 1993, 1997-2000 Free Software Foundation, Inc.

Authors: Jukka Virtanen <jtv@hut.fi>
         Peter Gerwinski <peter@gerwinski.de>
         Frank Heckenbach <frank@pascal.gnu.de>

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
along with GNU Pascal; see the file COPYING. If not, write to the
Free Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

/*
 * For GNU Pascal compiler (GPC) by Jukka Virtanen, jtv@hut.fi
 *
 * run this through the Doug Schmidt's gperf program
 * with command
 * gperf  -g -o -j1 -t -p -N is_reserved_word
 *
 * Thanks, Doug.
 */

/* ISO 7185 Standard Pascal keywords are recognized as keywords.
 *
 * To treat ISO 10206 Extended Pascal keywords as keywords,
 * you must give -fextended-pascal when compiling.
 *
 * To treat Object Pascal keywords as keywords, give
 * -fobject-pascal.
 *
 * And so on.
 *
 * Otherwise they are recognized, but may be redfined.
 * This violates Extended Pascal standard, but works anyhow,
 * if your program does not redefine them. And it also makes
 * ISO standard pascal program compile without modifications 
 * with GPC.
 *
 * Keywords are dynamically enabled and disabled such that a
 * correct ISO Pascal program will not notice foreign keywords
 * at all. If you change this, extensions won't work.
 */

struct resword {
  char *name;
  int token;
  int informed;
  int disabled;
  unsigned long iclass;
};

#define TOTAL_KEYWORDS 84
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 14
#define MIN_HASH_VALUE 3
#define MAX_HASH_VALUE 136
/* maximum key range = 134, duplicates = 0 */

#ifdef __GNUC__
__inline
#endif
static unsigned int
hash (str, len)
     register const char *str;
     register unsigned int len;
{
  static unsigned char asso_values[] =
    {
      137, 137, 137, 137, 137, 137, 137, 137, 137, 137,
      137, 137, 137, 137, 137, 137, 137, 137, 137, 137,
      137, 137, 137, 137, 137, 137, 137, 137, 137, 137,
      137, 137, 137, 137, 137, 137, 137, 137, 137, 137,
      137, 137, 137, 137, 137, 137, 137, 137, 137, 137,
      137, 137, 137, 137, 137, 137, 137, 137, 137, 137,
      137, 137, 137, 137, 137,  24,  41,  37,  24,  59,
       54,  19, 137,   2, 137, 137,  33,  13,  49,   3,
        0,  10,  23,  63,  79,  68,  48,  17,   3, 137,
      137, 137, 137, 137, 137, 137, 137,   0,  13,  20,
        9,   0,   6, 137,   0,   0, 137, 137,  24,  70,
        0,   0,   1, 137,   0,  12,   2,  11,   0,   0,
       17,  53, 137, 137, 137, 137, 137, 137, 137, 137,
      137, 137, 137, 137, 137, 137, 137, 137, 137, 137,
      137, 137, 137, 137, 137, 137, 137, 137, 137, 137,
      137, 137, 137, 137, 137, 137, 137, 137, 137, 137,
      137, 137, 137, 137, 137, 137, 137, 137, 137, 137,
      137, 137, 137, 137, 137, 137, 137, 137, 137, 137,
      137, 137, 137, 137, 137, 137, 137, 137, 137, 137,
      137, 137, 137, 137, 137, 137, 137, 137, 137, 137,
      137, 137, 137, 137, 137, 137, 137, 137, 137, 137,
      137, 137, 137, 137, 137, 137, 137, 137, 137, 137,
      137, 137, 137, 137, 137, 137, 137, 137, 137, 137,
      137, 137, 137, 137, 137, 137, 137, 137, 137, 137,
      137, 137, 137, 137, 137, 137, 137, 137, 137, 137,
      137, 137, 137, 137, 137, 137
    };
  return len + asso_values[(unsigned char)str[1]] + asso_values[(unsigned char)str[0]] + asso_values[(unsigned char)str[len - 1]];
}

static struct resword wordlist[] =
  {
    {""}, {""}, {""},
    {"Pow",            LEX_POW,            0, 0, E_O_G_PASCAL        /* exponentiation operator with integer exponent */},
    {"In",             LEX_IN,             0, 0, ANY_PASCAL          /* set membership test or part of a <for> loop when iterating through sets */},
    {"Or",             LEX_OR,             0, 0, ANY_PASCAL          /* Boolean or bitwise `or' operator or part of the <or else> operator */},
    {"Xor",            LEX_XOR,            0, 0, B_D_G_PASCAL        /* Boolean or bitwise `exclusive or' operator */},
    {"Private",        LEX_PRIVATE,        0, 1, B_D_G_PASCAL        /* private object fields */},
    {"Inline",         LEX_INLINE,         0, 0, GNU_PASCAL          /* declaration of inline routines */},
    {"Procedure",      LEX_PROCEDURE,      0, 0, ANY_PASCAL          /* procedure declaration */},
    {"Or_else",        LEX_OR_ELSE,        0, 0, E_O_G_PASCAL        /* short-circuit Boolean <or> operator */},
    {"Interface",      LEX_INTERFACE,      0, 1, E_O_U_B_D_G_PASCAL  /* module or unit interface part */},
    {"Operator",       LEX_OPERATOR,       0, 0, SC_G_PASCAL         /* operator declaration */},
    {"Interrupt",      LEX_INTERRUPT,      0, 0, B_D_PASCAL          /* interrupt handler declaration (not yet implemented) */},
    {"Otherwise",      LEX_OTHERWISE,      0, 1, E_O_G_PASCAL        /* default case label */},
    {"Packed",         LEX_PACKED,         0, 0, ANY_PASCAL          /* declaration of packed record or array types, also packed ordinal subranges */},
    {"If",             LEX_IF,             0, 0, ANY_PASCAL          /* conditional statement */},
    {"Of",             LEX_OF,             0, 0, ANY_PASCAL          /* part of an array, set or typed file type declaration, a <case> statement, a variant record type or a <type of> type inquiry */},
    {"Protected",      LEX_PROTECTED,      0, 1, E_O_D_G_PASCAL      /* read-only formal parameters and protected object fields */},
    {"Module",         LEX_MODULE,         0, 0, E_O_G_PASCAL        /* EP style or PXSC style (only partially implemented) modules */},
    {"Inherited",      LEX_INHERITED,      0, 1, O_B_D_G_PASCAL      /* reference to methods of ancestor object types */},
    {"With",           LEX_WITH,           0, 0, ANY_PASCAL          /* automatic record or object field access */},
    {"While",          LEX_WHILE,          0, 0, ANY_PASCAL          /* loop statement */},
    {"Goto",           LEX_GOTO,           0, 0, ANY_PASCAL          /* statement to jump to a <label> */},
    {"Object",         LEX_OBJECT,         0, 1, B_D_G_PASCAL        /* BP style object declaration */},
    {"Mod",            LEX_MOD,            0, 0, ANY_PASCAL          /* integer remainder operator */},
    {"Do",             LEX_DO,             0, 0, ANY_PASCAL          /* part of a <while> or <for> loop, a <with> statement, or a <to begin do> or <to end do> module constructor or destructor */},
    {"Div",            LEX_DIV,            0, 0, ANY_PASCAL          /* integer or real division operator */},
    {"Is",             LEX_IS,             0, 0, O_D_G_PASCAL        /* object type membership test (not yet implemented) */},
    {"Published",      LEX_PUBLISHED,      0, 1, B_D_G_PASCAL        /* published object fields */},
    {"Downto",         LEX_DOWNTO,         0, 0, ANY_PASCAL          /* part of a <for> loop when counting downwards */},
    {"Repeat",         LEX_REPEAT,         0, 0, ANY_PASCAL          /* loop statement */},
    {"And_then",       LEX_AND_THEN,       0, 0, E_O_G_PASCAL        /* short-circuit Boolean <and> operator */},
    {"Resident",       LEX_RESIDENT,       0, 0, B_D_PASCAL          /* library export specification (not yet implemented) */},
    {"Destructor",     LEX_DESTRUCTOR,     0, 1, O_B_D_G_PASCAL      /* object destructor */},
    {"Attribute",      LEX_ATTRIBUTE,      0, 1, GNU_PASCAL          /* attributes of routines and variables */},
    {"And",            LEX_AND,            0, 0, ANY_PASCAL          /* Boolean or bitwise `and' operator or part of the <and then> operator */},
    {"Public",         LEX_PUBLIC,         0, 1, B_D_G_PASCAL        /* public object fields */},
    {"Record",         LEX_RECORD,         0, 0, ANY_PASCAL          /* structured type declaration */},
    {"Qualified",      LEX_QUALIFIED,      0, 0, E_O_G_PASCAL        /* import specification */},
    {""},
    {"Case",           LEX_CASE,           0, 0, ANY_PASCAL          /* multi-branch conditional statement or variant record type */},
    {"Restricted",     LEX_RESTRICTED,     0, 0, E_O_G_PASCAL        /* type specification */},
    {"Asmname",        LEX_ASMNAME,        0, 1, GNU_PASCAL          /* linker name of routines and variables */},
    {"Const",          LEX_CONST,          0, 0, ANY_PASCAL          /* constant declaration or constant parameter declaration */},
    {"Absolute",       LEX_ABSOLUTE,       0, 1, B_D_G_PASCAL        /* overloaded variable declaration */},
    {"Begin",          LEX_BEGIN,          0, 0, ANY_PASCAL          /* begin of a code block, part of a <to begin do> module constructor */},
    {"Abstract",       LEX_ABSTRACT,       0, 1, O_G_PASCAL          /* abstract object type declaration (not yet implemented) */},
    {"Constructor",    LEX_CONSTRUCTOR,    0, 1, O_B_D_G_PASCAL      /* object constructor */},
    {"Bindable",       LEX_BINDABLE,       0, 0, E_O_G_PASCAL        /* external binding of files */},
    {""},
    {"Var",            LEX_VAR,            0, 0, ANY_PASCAL          /* variable declaration or reference parameter declaration */},
    {"View",           LEX_VIEW,           0, 0, O_G_PASCAL          /* object class view (not yet implemented) */},
    {"Value",          LEX_VALUE,          0, 1, E_O_G_PASCAL        /* variable initializer */},
    {"Not",            LEX_NOT,            0, 0, ANY_PASCAL          /* Boolean or bitwise `not' operator */},
    {""},
    {"Volatile",       LEX_VOLATILE,       0, 1, GNU_PASCAL          /* volatile variable declaration */},
    {"For",            LEX_FOR,            0, 0, ANY_PASCAL          /* loop statement where the number of loops is known in advance */},
    {"File",           LEX_FILE,           0, 0, ANY_PASCAL          /* non-text file type declaration */},
    {""},
    {"Only",           LEX_ONLY,           0, 0, E_O_G_PASCAL        /* import specification */},
    {"Property",       LEX_PROPERTY,       0, 0, O_D_G_PASCAL        /* variable properties (not yet implemented) */},
    {"Label",          LEX_LABEL,          0, 0, ANY_PASCAL          /* label declaration for a <goto> statement */},
    {""}, {""}, {""},
    {"Shr",            LEX_SHR,            0, 0, B_D_G_PASCAL        /* bitwise right shift operator */},
    {""},
    {"Set",            LEX_SET,            0, 0, ANY_PASCAL          /* set type declaration */},
    {""},
    {"Forward",        LEX_FORWARD,        0, 0, U_B_D_G_PASCAL      /* declaration of a routine whose definition follows below */},
    {"End",            LEX_END,            0, 0, ANY_PASCAL          /* end of a code block, end of a <case> statement, end of a record or object declaration, part of a <to end do> module destructor */},
    {"Segment",        LEX_SEGMENT,        0, 1, UCSD_PASCAL         /* segment specification (not yet implemented) */},
    {"Function",       LEX_FUNCTION,       0, 0, ANY_PASCAL          /* function declaration */},
    {"Unit",           LEX_UNIT,           0, 0, U_B_D_G_PASCAL      /* UCSD and BP style unit declaration */},
    {"All",            LEX_ALL,            0, 1, GNU_PASCAL          /* <export> extension (`export foo = all') */},
    {"Nil",            LEX_NIL,            0, 0, ANY_PASCAL          /* reserved value for unassigned pointers */},
    {"Program",        LEX_PROGRAM,        0, 0, ANY_PASCAL          /* start of a Pascal program */},
    {"Class",          LEX_CLASS,          0, 1, O_D_G_PASCAL        /* OOE/Delphi style object classes (not yet implemented) */},
    {"Virtual",        LEX_VIRTUAL,        0, 1, O_B_D_G_PASCAL      /* virtual object method declaration */},
    {"Import",         LEX_IMPORT,         0, 0, E_O_G_PASCAL        /* module interface import */},
    {"To",             LEX_TO,             0, 0, ANY_PASCAL          /* part of a <for> loop when counting upwards or a <to begin do> or <to end do> module constructor or destructor */},
    {"Array",          LEX_ARRAY,          0, 0, ANY_PASCAL          /* array type declaration */},
    {"Then",           LEX_THEN,           0, 0, ANY_PASCAL          /* part of an <if> statement or part of the <and then> operator */},
    {"Export",         LEX_EXPORT,         0, 0, E_O_B_D_G_PASCAL    /* module interface export */},
    {""},
    {"Implementation", LEX_IMPLEMENTATION, 0, 1, E_O_U_B_D_G_PASCAL  /* module or unit implementation part */},
    {"Else",           LEX_ELSE,           0, 0, ANY_PASCAL          /* alternative part of an <if> statement, default case label or part of the <or else> operator */},
    {""}, {""},
    {"Shl",            LEX_SHL,            0, 0, B_D_G_PASCAL        /* bitwise left shift operator */},
    {"Static",         LEX_STATIC,         0, 1, GNU_PASCAL          /* static variable declaration */},
    {""},
    {"Library",        LEX_LIBRARY,        0, 1, B_D_G_PASCAL        /* library declaration (not yet implemented) */},
    {""},
    {"Exports",        LEX_EXPORTS,        0, 0, B_D_G_PASCAL        /* library export (not yet implemented) */},
    {"Uses",           LEX_USES,           0, 0, U_B_D_G_PASCAL      /* unit import */},
    {"Until",          LEX_UNTIL,          0, 0, ANY_PASCAL          /* end of a <repeat> statement */},
    {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
    {""},
    {"External",       LEX_EXTERNAL,       0, 1, E_O_U_B_D_G_PASCAL  /* declaration of external objects */},
    {"Asm",            LEX_ASM,            0, 0, B_D_G_PASCAL        /* GNU style inline assembler code */},
    {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
    {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
    {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
    {"Type",           LEX_TYPE,           0, 0, ANY_PASCAL          /* type declaration or part of a <type of> type inquiry */}
  };

#ifdef __GNUC__
__inline
#endif
struct resword *
is_reserved_word (str, len)
     register const char *str;
     register unsigned int len;
{
  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}
