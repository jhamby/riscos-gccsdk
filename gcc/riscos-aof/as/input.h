/*
 *  input.h
 * Copyright © 1992 Niklas Röjemo
 */

#ifndef input_header_included
#define input_header_included

#include <ctype.h>
#include <stdio.h>

#include "global.h"
#include "macros.h"

#define FLIP(x) (isupper((x)) ? tolower((x)) : islower((x)) ? toupper((x)) : ((x)))
#define TOLOWER(x) tolower((x))

extern FILE *asmfile;
extern long int inputLineNo;	/* Do not change, only read! */
extern BOOL inputExpand;
extern BOOL inputRewind;
extern const char *inputName;

void inputInit (const char *infile);
void inputFinish (void);

/* Debug only:
 */
const char *inputGiveRestLine(void);

BOOL inputNextLine (void);

void skipblanks (void);
void skiprest (void);

int inputComment (void);	/* True if null or a comment */
char inputLook (void);
char inputLookLower (void);
char inputLookUC (void);
char inputLookN (int n);
char inputLookNLower (int n);
char inputLookNUC (int n);
char inputGet (void);
char inputGetLower (void);
char inputGetUC (void);
void inputUnGet (char c);
void inputPutBack (char c);
char inputSkipLook (void);
char *inputRest (void);
char *inputLine (void);
void inputSkip (void);
void inputSkipN (int n);
char *inputSymbol (int *ilen, char del);

void inputMark (void);
void inputRollback (void);

void inputThisInstead (const char *);

#endif
