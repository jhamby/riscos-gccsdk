/*
 *  input.h
 * Copyright © 1992 Niklas Röjemo
 */

#ifndef _input_h
#define _input_h

#ifndef _global_h
#include "global.h"
#endif

#ifndef _macros_h
#include "macros.h"
#endif

#include <ctype.h>

#if 0				/*'A'==65 */
#define FLIP(x) (isalpha((x)) ? (x)^32 : (x))
#define TOLOWER(x) (isupper((x)) ? (x)+32 : (x))
  /* these are ASCII-like charset dependent */
#else
#define FLIP(x) (isupper((x)) ? tolower((x)) : islower((x)) ? toupper((x)) : ((x)))
#define TOLOWER(x) tolower((x))

#endif

extern BOOL inputExpand;
extern BOOL inputRewind;
extern char *inputName;

extern void inputInit (char *infile);
extern void inputFinish (void);

extern BOOL inputNextLine (void);

extern void skipblanks (void);
extern void skiprest (void);

extern long int inputLineNo;	/* Do not change, only read! */

extern int inputComment (void);	/* True if null or a comment */
extern char inputLook (void);
extern char inputLookLower (void);
extern char inputLookUC (void);
extern char inputLookN (int n);
extern char inputLookNLower (int n);
extern char inputLookNUC (int n);
extern char inputGet (void);
extern char inputGetLower (void);
extern char inputGetUC (void);
extern void inputUnGet (char c);
extern char inputSkipLook (void);
extern char *inputRest (void);
extern char *inputLine (void);
extern void inputSkip (void);
extern void inputSkipN (int n);
extern char *inputSymbol (int *ilen, char del, int include_dot);

extern void inputMark (void);
extern void inputRollback (void);

extern void inputThisInstead (char *);

extern int uc;
#if 0				/* __GNUC__ */
extern char *input_buff;
extern char *input_pos, *input_mark;

extern __inline__ char
inputLook (void)
{
  return *input_pos;
}

extern __inline__ char
inputLookLower (void)
{
  return TOLOWER (*input_pos);
}

extern __inline__ char
inputLookUC (void)
{
  char x = *input_pos;
  return uc ? FLIP (x) : TOLOWER (x);
}

extern __inline__ int
inputComment (void)
{
  int c = *input_pos;
  return c == 0 || c == ';';
}


extern __inline__ char
inputLookN (int n)		/* Unsafe */
{
  return input_pos[n];
}

extern __inline__ char
inputLookNLower (int n)		/* Unsafe */
{
  return TOLOWER (input_pos[n]);
}

extern __inline__ char
inputLookNUC (int n)		/* Unsafe */
{
  char x = input_pos[n];
  return uc ? FLIP (x) : TOLOWER (x);
}

extern __inline__ char
inputGet (void)
{
  if (*input_pos)
    return *input_pos++;
  else
    return *input_pos;
}


extern __inline__ char
inputGetLower (void)
{
  char c = *input_pos ? *input_pos++ : *input_pos;
  return TOLOWER (c);
}

extern __inline__ char
inputGetUC (void)
{
  char x = *input_pos ? *input_pos++ : *input_pos;
  return uc ? FLIP (x) : TOLOWER (x);
}

extern __inline__ void
inputSkip (void)
{
  if (*input_pos)
    input_pos++;
}

extern __inline__ char *
inputLine (void)
{
  return input_buff;
}

extern __inline__ char
inputSkipLook (void)
{
  return *input_pos ? *++input_pos : *input_pos;
}

extern __inline__ void
skiprest (void)
{
  (input_pos = input_mark = input_buff)[0] = 0;
}

extern __inline__ void
inputMark (void)
{
  input_mark = input_pos;
}


extern __inline__ void
inputRollback (void)
{
  input_pos = input_mark;
}

#endif /* __GNUC__ */

#endif
