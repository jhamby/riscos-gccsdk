/*
** Drlink AOF Linker
**
** Copyright © David Daniels 1993, 1994, 1995, 1996, 1997, 1998.
** All rights reserved.
**
** This module contains the C++ name mangling decoding routines.
** It decodes names as per Stroustrup with a little bit of hacking
** as Acorn's C++ compiler does not mangle names in quite the
** same way. As it stands the code does not work properly and so
** it is not part of the linker. It also seems to be a lot of
** code for little effect
*/

/* #define MANGLED */

#ifdef MANGLED

/*
** Note that only a little bit of the code is here. I have left
** out most of it
*/


#include <string.h>
#include <ctype.h>

#define MAXLENGTH 1024		/* Maximum expanded name length */

typedef enum {	/* Tokens */
  ALLDONE, NOTMANGLED, IDENT, OPER, BASICTYPE, MODIFIER, POINTER,
  REFERENCE, ARRAY, FUNCTION, POINTMEM, NEXTARG, REFTO, QUALIFIED,
  USCORE, DSCORE, LPAREN, RPAREN, COMMA
} component;

typedef enum {	/* What name appears to be */
  UNKNOWN, FUNKTION, OPERATOR, CONSTRUCTOR, DESTRUCTOR, VARIABLE
} symboltype;

typedef enum {ATSTART, ATEND} position;

/*
** The 'element' structure is used to build the de-mangled name. Each
** element in the mangled name generates one or more entries in the
** list from which the name will eventually be reconstituted. The list
** itself is linked in two ways: each element is linked to form the
** name and there are also links that join the list corresponding to
** each function argument.
*/
typedef struct element {
  component eltype;		/* What we've got */
  unsigned char elstrlen;	/* length if character string */
  bool elqual;			/* TRUE if name should be followed by '::' */
  union {
    unsigned int elbound;	/* Size of array */
    char *elstring;		/* Pointer to text if character string */
    struct element *elfunc;	/* Pointer to signature if nested function */
  } elnext;
  struct element *elflink;
} element,

typedef struct argument{
  element *argstart;		/* Point to start of element list for argument */
  struct argument *nextarg;	/* Next argument in list */
} argument;

/*
** The 'signature' structure contains the information relevant to a
** function signature. As functions arguments can themselves be
** function signatures, a stack of signatures that are currently
** being parsed can be built
*/
typedef struct signature {
  argument *arglist;		/* Start of name's demangled element list */
  argument *arglast;		/* End of name's demangled element list */
  element *thisarglist;		/* Start of list for the current argument */
  element *thisarglast;		/* End of list for current argument */
  element *modlist;		/* Things that make up a modifier */
  element *modlast;		/* End of modifier's list */
  element *returntype;		/* Function's return type */
  int pointcount;		/* Count of pointers found */
  int argsofar;			/* Arguments so far in this signature */
  struct signature *sigblink;
} signature;

static char
  *cp,				/* Pointer into mangled name */
  *id;				/* Pointer to last identifier, basic type name, etc */

static element
  *thisname,			/* Pointer to name of function or variable */
  *thisclass;			/* pointer to name of class of function or variable */

static int
  argcount,			/* Count of arguments <x> in Nxy */
  argref,			/* Argument <y> referenced in Nxy and Ty */
  arraybound,			/* Number of elements <x> in array in A<x>_ */
  qualcount,			/* Count of class in qualified class name Q<x>_ */
  idlen;			/* Length of last identifier */

static component class;		/* Class of last component of name */

static symboltype nametype;	/* Type of mangled entity */

static signature *sigstack;	/* Stack of function signatures */

static char fullname[MAXLENGTH];	/* Expanded function name */

struct {unsigned char namelen; symboltype what; char *encoded_name, *proper_name;} operators [] = {
	/* Encoded -> proper operator names */
  {2, OPERATOR, "aa", "&&"},	{3, OPERATOR, "aad", "&="},	{3, OPERATOR, "adv", "/="},
  {2, OPERATOR, "ad", "&"},	{3, OPERATOR, "aer", "^="},	{3, OPERATOR, "als", "<<="},
  {3, OPERATOR, "amd", "%="},	{3, OPERATOR, "ami", "-="},	{3, OPERATOR, "amu", "*="},
  {3, OPERATOR, "aor", "|="},	{3, OPERATOR, "apl", "+="},	{3, OPERATOR, "ars", ">>="},
  {2, OPERATOR, "as", "="},	{2, OPERATOR, "cl", "{}"},	{2, OPERATOR, "cm", ","},
  {2, OPERATOR, "co", "~"},	{2, OPERATOR, "dl", "delete"},	{2, OPERATOR, "dv", "/"},
  {2, OPERATOR, "eq", "=="},	{2, OPERATOR, "er", "^"},	{2, OPERATOR, "ge", ">="},
  {2, OPERATOR, "gt", ">"},	{2, OPERATOR, "le", "<="},	{2, OPERATOR, "ls", "<<"},
  {2, OPERATOR, "lt", "<"},	{2, OPERATOR, "md", "%"},	{2, OPERATOR, "mi", "-"},
  {2, OPERATOR, "ml", "*"},	{2, OPERATOR, "mm", "--"},	{2, OPERATOR, "ne", "!="},
  {2, OPERATOR, "nt", "!"},	{2, OPERATOR, "nw", "new"},	{2, OPERATOR, "oo", "||"},
  {2, OPERATOR, "or", "|"},	{2, OPERATOR, "pl", "+"},	{2, OPERATOR, "pp", "++"},
  {2, OPERATOR, "rf", "->"},	{2, OPERATOR, "rs", ">>"},	{2, OPERATOR, "rm", "->*"},
  {2, OPERATOR, "vc", "[]"},	{2, CONSTRUCTOR, "ct", ""},	{2, DESTRUCTOR, "dt", ""}
};

void get_signature(void);	/* Forward reference */


/*
** 'decode' is the routine called to make sense of the text passed to
** it as a C++ name. It returns a pointer to the expanded name of the
** function or variable or the original string if the name turns out
** not to be mangled after all.
*/
char *decode(char *name) {
  char *opname, *original;
  bool found;
  nametype = UNKNOWN;
  cp = original = name;
  nextelement = 0;
  nextargument = 0;
  nextsignature = 0;
  sigstack = NIL;
  argcount = 0;
  id = NIL;
  idlen = 0;
  thisclass = NIL;
  thisname = NIL;
/* '__' at start = possible operator, constructor or destructor */
  if (strncmp(name, "__", 2)==0) find_operator();
  if (nametype==UNKNOWN) get_ident();
  thisname = new_element(IDENT);
  get_next();
  if (class!=DSCORE) return name;	/* Not mangled. Not even slightly creased */
  get_next();
  if (nametype==CONSTRUCTOR || nametype==DESTRUCTOR) {
    if (class!=IDENT) return name;
    thisclass = thisname = new_element(IDENT);
    get_next();
  }
  else if (class==IDENT) {
    thisclass = new_element(IDENT);
    get_next();
  }
  switch (class) {
  case ALLDONE:
    nametype = VARIABLE;
    break;
  case FUNCTION:
    get_next();
    get_signature();
    break;
  default:
    nametype = UNKNOWN
  }
  if (nametype==UNKNOWN) return name;
  expand_name();
  return &fullname[0];
}

char *decode_name(char *name) {
  if (opt_cpp) {
    return decode(name)
  }
  else {
    return name;
  }
}

#else

/*
** 'decode_name' returns a pointer to a de-mangled version of the
** name passed to it.
*/
char *decode_name(char *name) {
  return name;
}

#endif
