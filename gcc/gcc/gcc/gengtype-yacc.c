
/*  A Bison parser, made from gengtype-yacc.y
    by GNU Bison version 1.28  */

#define YYBISON 1  /* Identify Bison output.  */

#define	ENT_TYPEDEF_STRUCT	257
#define	ENT_STRUCT	258
#define	ENT_EXTERNSTATIC	259
#define	ENT_YACCUNION	260
#define	GTY_TOKEN	261
#define	UNION	262
#define	STRUCT	263
#define	ENUM	264
#define	ALIAS	265
#define	PARAM_IS	266
#define	NUM	267
#define	PERCENTPERCENT	268
#define	SCALAR	269
#define	ID	270
#define	STRING	271
#define	ARRAY	272
#define	PERCENT_ID	273
#define	CHAR	274

#line 22 "gengtype-yacc.y"

#include "hconfig.h"
#include "system.h"
#include "gengtype.h"
#define YYERROR_VERBOSE

#line 29 "gengtype-yacc.y"
typedef union {
  type_p t;
  pair_p p;
  options_p o;
  const char *s;
} YYSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		111
#define	YYFLAG		-32768
#define	YYNTBASE	32

#define YYTRANSLATE(x) ((unsigned)(x) <= 274 ? yytranslate[x] : 52)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,    30,
    31,    28,     2,    29,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,    27,    23,    25,
    24,    26,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    21,     2,    22,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    20
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     1,     4,     7,    10,    11,    20,    21,    29,    35,
    42,    50,    52,    54,    56,    63,    64,    68,    75,    76,
    79,    82,    83,    90,    97,   105,   106,   109,   111,   113,
   116,   122,   125,   131,   134,   137,   143,   144,   150,   154,
   157,   158,   160,   167,   169,   171,   176,   181,   183,   187,
   188
};

static const short yyrhs[] = {    -1,
    33,    32,     0,    36,    32,     0,    39,    32,     0,     0,
     3,    47,    21,    42,    22,    16,    34,    23,     0,     0,
     4,    47,    21,    42,    22,    35,    23,     0,     5,    47,
    37,    16,    38,     0,     5,    47,    37,    16,    18,    38,
     0,     5,    47,    37,    16,    18,    18,    38,     0,    44,
     0,    23,     0,    24,     0,     6,    47,    42,    22,    40,
    14,     0,     0,    40,    19,    41,     0,    40,    19,    25,
    16,    26,    41,     0,     0,    41,    16,     0,    41,    20,
     0,     0,    44,    46,    16,    43,    23,    42,     0,    44,
    46,    16,    18,    23,    42,     0,    44,    46,    16,    18,
    18,    23,    42,     0,     0,    27,    13,     0,    15,     0,
    16,     0,    44,    28,     0,     9,    16,    21,    42,    22,
     0,     9,    16,     0,     8,    16,    21,    42,    22,     0,
     8,    16,     0,    10,    16,     0,    10,    16,    21,    45,
    22,     0,     0,    16,    24,    13,    29,    45,     0,    16,
    29,    45,     0,    16,    45,     0,     0,    47,     0,     7,
    30,    30,    51,    31,    31,     0,    11,     0,    12,     0,
    48,    30,    44,    31,     0,    16,    30,    17,    31,     0,
    49,     0,    50,    29,    49,     0,     0,    50,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    62,    63,    64,    65,    68,    76,    77,    84,    87,    92,
    97,   105,   112,   113,   116,   123,   125,   138,   156,   158,
   169,   183,   184,   194,   204,   216,   217,   220,   222,   224,
   226,   231,   233,   238,   240,   242,   246,   247,   249,   251,
   255,   256,   259,   263,   265,   269,   276,   285,   290,   297,
   298
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","ENT_TYPEDEF_STRUCT",
"ENT_STRUCT","ENT_EXTERNSTATIC","ENT_YACCUNION","\"GTY\"","\"union\"","\"struct\"",
"\"enum\"","\"ptr_alias\"","\"param_is\"","NUM","\"%%\"","SCALAR","ID","STRING",
"ARRAY","PERCENT_ID","CHAR","'{'","'}'","';'","'='","'<'","'>'","':'","'*'",
"','","'('","')'","start","typedef_struct","@1","@2","externstatic","lasttype",
"semiequal","yacc_union","yacc_typematch","yacc_ids","struct_fields","bitfieldopt",
"type","enum_items","optionsopt","options","type_option","option","optionseq",
"optionseqopt", NULL
};
#endif

static const short yyr1[] = {     0,
    32,    32,    32,    32,    34,    33,    35,    33,    36,    36,
    36,    37,    38,    38,    39,    40,    40,    40,    41,    41,
    41,    42,    42,    42,    42,    43,    43,    44,    44,    44,
    44,    44,    44,    44,    44,    44,    45,    45,    45,    45,
    46,    46,    47,    48,    48,    49,    49,    50,    50,    51,
    51
};

static const short yyr2[] = {     0,
     0,     2,     2,     2,     0,     8,     0,     7,     5,     6,
     7,     1,     1,     1,     6,     0,     3,     6,     0,     2,
     2,     0,     6,     6,     7,     0,     2,     1,     1,     2,
     5,     2,     5,     2,     2,     5,     0,     5,     3,     2,
     0,     1,     6,     1,     1,     4,     4,     1,     3,     0,
     1
};

static const short yydefact[] = {     1,
     0,     0,     0,     0,     1,     1,     1,     0,     0,     0,
     0,    22,     2,     3,     4,     0,    22,    22,     0,     0,
     0,    28,    29,     0,    12,     0,    41,    50,     0,     0,
    34,    32,    35,     0,    30,    16,     0,    42,    44,    45,
     0,     0,    48,    51,     0,     0,     7,    22,    22,    37,
     0,    13,    14,     9,     0,    26,     0,     0,     0,     0,
     5,     0,     0,     0,    37,     0,     0,    10,    15,    19,
     0,     0,     0,     0,     0,    49,    43,     0,     8,    33,
    31,     0,    37,    40,    36,    11,     0,    17,     0,    22,
    27,    22,    47,    46,     6,     0,    39,     0,    20,    21,
    22,    24,    23,    37,    19,    25,    38,    18,     0,     0,
     0
};

static const short yydefgoto[] = {    13,
     5,    78,    62,     6,    24,    54,     7,    55,    88,    26,
    73,    27,    66,    37,     9,    42,    43,    44,    45
};

static const short yypact[] = {    56,
     7,     7,     7,     7,    56,    56,    56,     8,     4,     9,
    25,    25,-32768,-32768,-32768,    18,    25,    25,    34,    39,
    40,-32768,-32768,    41,    35,     1,     0,    42,    43,    44,
    46,    47,    48,    21,-32768,-32768,    54,-32768,-32768,-32768,
    49,    50,-32768,    45,    33,    55,-32768,    25,    25,    60,
    28,-32768,-32768,-32768,    -3,    -8,    61,    25,    42,    51,
-32768,    58,    63,    64,    -7,    65,    13,-32768,-32768,    52,
    -5,    59,    66,    57,   -16,-32768,-32768,    67,-32768,-32768,
-32768,    70,    60,-32768,-32768,-32768,    75,    27,    69,    25,
-32768,    25,-32768,-32768,-32768,    68,-32768,    72,-32768,-32768,
    25,-32768,-32768,    60,-32768,-32768,-32768,    27,    93,    94,
-32768
};

static const short yypgoto[] = {    20,
-32768,-32768,-32768,-32768,-32768,   -43,-32768,-32768,   -10,   -17,
-32768,    -9,   -62,-32768,     2,-32768,    37,-32768,-32768
};


#define	YYLAST		98


static const short yytable[] = {    29,
    30,    25,    84,    10,    11,    12,     8,    68,    65,    71,
    69,    35,    89,     8,    94,    70,    82,    90,    72,   109,
    97,    83,    36,    86,    17,    14,    15,    35,    38,    18,
    63,    64,    19,    20,    21,    52,    53,    16,    51,    22,
    23,   107,    99,    52,    53,    67,   100,    28,    75,    31,
    52,    53,    39,    40,    32,    33,    34,    41,     1,     2,
     3,     4,    35,    60,    46,    47,    48,    49,    50,    56,
    61,    91,   102,    59,   103,    65,    87,    74,    57,    58,
    79,    77,    96,   106,    80,    81,    85,    93,    92,    95,
    98,   101,   110,   111,   108,    76,   104,   105
};

static const short yycheck[] = {    17,
    18,    11,    65,     2,     3,     4,     7,    51,    16,    18,
    14,    28,    18,     7,    31,    19,    24,    23,    27,     0,
    83,    29,    22,    67,    21,     6,     7,    28,    27,    21,
    48,    49,     8,     9,    10,    23,    24,    30,    18,    15,
    16,   104,    16,    23,    24,    18,    20,    30,    58,    16,
    23,    24,    11,    12,    16,    16,    16,    16,     3,     4,
     5,     6,    28,    31,    22,    22,    21,    21,    21,    16,
    16,    13,    90,    29,    92,    16,    25,    17,    30,    30,
    23,    31,    13,   101,    22,    22,    22,    31,    23,    23,
    16,    23,     0,     0,   105,    59,    29,    26
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/lib/bison.simple"
/* This file comes from bison-1.28.  */

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

#ifndef YYSTACK_USE_ALLOCA
#ifdef alloca
#define YYSTACK_USE_ALLOCA
#else /* alloca not defined */
#ifdef __GNUC__
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || (defined (__sun) && defined (__i386))
#define YYSTACK_USE_ALLOCA
#include <alloca.h>
#else /* not sparc */
/* We think this test detects Watcom and Microsoft C.  */
/* This used to test MSDOS, but that is a bad idea
   since that symbol is in the user namespace.  */
#if (defined (_MSDOS) || defined (_MSDOS_)) && !defined (__TURBOC__)
#if 0 /* No need for malloc.h, which pollutes the namespace;
	 instead, just don't use alloca.  */
#include <malloc.h>
#endif
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
/* I don't know what this was needed for, but it pollutes the namespace.
   So I turned it off.   rms, 2 May 1997.  */
/* #include <malloc.h>  */
 #pragma alloca
#define YYSTACK_USE_ALLOCA
#else /* not MSDOS, or __TURBOC__, or _AIX */
#if 0
#ifdef __hpux /* haible@ilog.fr says this works for HPUX 9.05 and up,
		 and on HPUX 10.  Eventually we can turn this on.  */
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#endif /* __hpux */
#endif
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc */
#endif /* not GNU C */
#endif /* alloca not defined */
#endif /* YYSTACK_USE_ALLOCA not defined */

#ifdef YYSTACK_USE_ALLOCA
#define YYSTACK_ALLOC alloca
#else
#define YYSTACK_ALLOC malloc
#endif

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Define __yy_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     unsigned int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, unsigned int count)
{
  register char *t = to;
  register char *f = from;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 217 "/usr/lib/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
#ifdef YYPARSE_PARAM
int yyparse (void *);
#else
int yyparse (void);
#endif
#endif

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;
  int yyfree_stacks = 0;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  if (yyfree_stacks)
	    {
	      free (yyss);
	      free (yyvs);
#ifdef YYLSP_NEEDED
	      free (yyls);
#endif
	    }
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
#ifndef YYSTACK_USE_ALLOCA
      yyfree_stacks = 1;
#endif
      yyss = (short *) YYSTACK_ALLOC (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1,
		   size * (unsigned int) sizeof (*yyssp));
      yyvs = (YYSTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1,
		   size * (unsigned int) sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1,
		   size * (unsigned int) sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 5:
#line 69 "gengtype-yacc.y"
{
		     new_structure (yyvsp[-5].t->u.s.tag, UNION_P (yyvsp[-5].t), &lexer_line,
				    yyvsp[-2].p, yyvsp[-4].o);
		     do_typedef (yyvsp[0].s, yyvsp[-5].t, &lexer_line);
		     lexer_toplevel_done = 1;
		   ;
    break;}
case 6:
#line 76 "gengtype-yacc.y"
{;
    break;}
case 7:
#line 78 "gengtype-yacc.y"
{
		     new_structure (yyvsp[-4].t->u.s.tag, UNION_P (yyvsp[-4].t), &lexer_line,
				    yyvsp[-1].p, yyvsp[-3].o);
		     lexer_toplevel_done = 1;
		   ;
    break;}
case 8:
#line 84 "gengtype-yacc.y"
{;
    break;}
case 9:
#line 88 "gengtype-yacc.y"
{
	           note_variable (yyvsp[-1].s, adjust_field_type (yyvsp[-2].t, yyvsp[-3].o), yyvsp[-3].o,
				  &lexer_line);
	         ;
    break;}
case 10:
#line 93 "gengtype-yacc.y"
{
	           note_variable (yyvsp[-2].s, create_array (yyvsp[-3].t, yyvsp[-1].s),
	      		    yyvsp[-4].o, &lexer_line);
	         ;
    break;}
case 11:
#line 98 "gengtype-yacc.y"
{
	           note_variable (yyvsp[-3].s, create_array (create_array (yyvsp[-4].t, yyvsp[-1].s),
	      				      yyvsp[-2].s),
	      		    yyvsp[-5].o, &lexer_line);
	         ;
    break;}
case 12:
#line 106 "gengtype-yacc.y"
{
	      lexer_toplevel_done = 1;
	      yyval.t = yyvsp[0].t;
	    ;
    break;}
case 15:
#line 118 "gengtype-yacc.y"
{
	        note_yacc_type (yyvsp[-4].o, yyvsp[-3].p, yyvsp[-1].p, &lexer_line);
	      ;
    break;}
case 16:
#line 124 "gengtype-yacc.y"
{ yyval.p = NULL; ;
    break;}
case 17:
#line 126 "gengtype-yacc.y"
{
		     pair_p p;
		     for (p = yyvsp[0].p; p->next != NULL; p = p->next)
		       {
		         p->name = NULL;
			 p->type = NULL;
		       }
		     p->name = NULL;
		     p->type = NULL;
		     p->next = yyvsp[-2].p;
		     yyval.p = yyvsp[0].p;
		   ;
    break;}
case 18:
#line 139 "gengtype-yacc.y"
{
		     pair_p p;
		     type_p newtype = NULL;
		     if (strcmp (yyvsp[-4].s, "type") == 0)
		       newtype = (type_p) 1;
		     for (p = yyvsp[0].p; p->next != NULL; p = p->next)
		       {
		         p->name = yyvsp[-2].s;
		         p->type = newtype;
		       }
		     p->name = yyvsp[-2].s;
		     p->next = yyvsp[-5].p;
		     p->type = newtype;
		     yyval.p = yyvsp[0].p;
		   ;
    break;}
case 19:
#line 157 "gengtype-yacc.y"
{ yyval.p = NULL; ;
    break;}
case 20:
#line 159 "gengtype-yacc.y"
{
	  pair_p p = xcalloc (1, sizeof (*p));
	  p->next = yyvsp[-1].p;
	  p->line = lexer_line;
	  p->opt = xmalloc (sizeof (*(p->opt)));
	  p->opt->name = "tag";
	  p->opt->next = NULL;
	  p->opt->info = (char *)yyvsp[0].s;
	  yyval.p = p;
	;
    break;}
case 21:
#line 170 "gengtype-yacc.y"
{
	  pair_p p = xcalloc (1, sizeof (*p));
	  p->next = yyvsp[-1].p;
	  p->line = lexer_line;
	  p->opt = xmalloc (sizeof (*(p->opt)));
	  p->opt->name = "tag";
	  p->opt->next = NULL;
	  p->opt->info = xmalloc (3 + strlen (yyvsp[0].s));
	  sprintf (p->opt->info, "'%s'", yyvsp[0].s);
	  yyval.p = p;
	;
    break;}
case 22:
#line 183 "gengtype-yacc.y"
{ yyval.p = NULL; ;
    break;}
case 23:
#line 185 "gengtype-yacc.y"
{
	            pair_p p = xmalloc (sizeof (*p));
		    p->type = adjust_field_type (yyvsp[-5].t, yyvsp[-4].o);
		    p->opt = yyvsp[-4].o;
		    p->name = yyvsp[-3].s;
		    p->next = yyvsp[0].p;
		    p->line = lexer_line;
		    yyval.p = p;
		  ;
    break;}
case 24:
#line 195 "gengtype-yacc.y"
{
	            pair_p p = xmalloc (sizeof (*p));
		    p->type = adjust_field_type (create_array (yyvsp[-5].t, yyvsp[-2].s), yyvsp[-4].o);
		    p->opt = yyvsp[-4].o;
		    p->name = yyvsp[-3].s;
		    p->next = yyvsp[0].p;
		    p->line = lexer_line;
		    yyval.p = p;
		  ;
    break;}
case 25:
#line 205 "gengtype-yacc.y"
{
	            pair_p p = xmalloc (sizeof (*p));
		    p->type = create_array (create_array (yyvsp[-6].t, yyvsp[-2].s), yyvsp[-3].s);
		    p->opt = yyvsp[-5].o;
		    p->name = yyvsp[-4].s;
		    p->next = yyvsp[0].p;
		    p->line = lexer_line;
		    yyval.p = p;
		  ;
    break;}
case 28:
#line 221 "gengtype-yacc.y"
{ yyval.t = yyvsp[0].t; ;
    break;}
case 29:
#line 223 "gengtype-yacc.y"
{ yyval.t = resolve_typedef (yyvsp[0].s, &lexer_line); ;
    break;}
case 30:
#line 225 "gengtype-yacc.y"
{ yyval.t = create_pointer (yyvsp[-1].t); ;
    break;}
case 31:
#line 227 "gengtype-yacc.y"
{
	   new_structure (yyvsp[-3].s, 0, &lexer_line, yyvsp[-1].p, NULL);
           yyval.t = find_structure (yyvsp[-3].s, 0);
	 ;
    break;}
case 32:
#line 232 "gengtype-yacc.y"
{ yyval.t = find_structure (yyvsp[0].s, 0); ;
    break;}
case 33:
#line 234 "gengtype-yacc.y"
{
	   new_structure (yyvsp[-3].s, 1, &lexer_line, yyvsp[-1].p, NULL);
           yyval.t = find_structure (yyvsp[-3].s, 1);
	 ;
    break;}
case 34:
#line 239 "gengtype-yacc.y"
{ yyval.t = find_structure (yyvsp[0].s, 1); ;
    break;}
case 35:
#line 241 "gengtype-yacc.y"
{ yyval.t = create_scalar_type (yyvsp[0].s, strlen (yyvsp[0].s)); ;
    break;}
case 36:
#line 243 "gengtype-yacc.y"
{ yyval.t = create_scalar_type (yyvsp[-3].s, strlen (yyvsp[-3].s)); ;
    break;}
case 38:
#line 248 "gengtype-yacc.y"
{ ;
    break;}
case 39:
#line 250 "gengtype-yacc.y"
{ ;
    break;}
case 40:
#line 252 "gengtype-yacc.y"
{ ;
    break;}
case 41:
#line 255 "gengtype-yacc.y"
{ yyval.o = NULL; ;
    break;}
case 42:
#line 256 "gengtype-yacc.y"
{ yyval.o = yyvsp[0].o; ;
    break;}
case 43:
#line 260 "gengtype-yacc.y"
{ yyval.o = yyvsp[-2].o; ;
    break;}
case 44:
#line 264 "gengtype-yacc.y"
{ yyval.s = "ptr_alias"; ;
    break;}
case 45:
#line 266 "gengtype-yacc.y"
{ yyval.s = "param_is"; ;
    break;}
case 46:
#line 270 "gengtype-yacc.y"
{
	     options_p o = xmalloc (sizeof (*o));
	     o->name = yyvsp[-3].s;
	     o->info = yyvsp[-1].t;
	     yyval.o = o;
	   ;
    break;}
case 47:
#line 277 "gengtype-yacc.y"
{
	     options_p o = xmalloc (sizeof (*o));
	     o->name = yyvsp[-3].s;
	     o->info = (void *)yyvsp[-1].s;
	     yyval.o = o;
	   ;
    break;}
case 48:
#line 286 "gengtype-yacc.y"
{
	        yyvsp[0].o->next = NULL;
		yyval.o = yyvsp[0].o;
	      ;
    break;}
case 49:
#line 291 "gengtype-yacc.y"
{
	        yyvsp[0].o->next = yyvsp[-2].o;
		yyval.o = yyvsp[0].o;
	      ;
    break;}
case 50:
#line 297 "gengtype-yacc.y"
{ yyval.o = NULL; ;
    break;}
case 51:
#line 298 "gengtype-yacc.y"
{ yyval.o = yyvsp[0].o; ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 543 "/usr/lib/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;

 yyacceptlab:
  /* YYACCEPT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 0;

 yyabortlab:
  /* YYABORT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 1;
}
#line 300 "gengtype-yacc.y"

