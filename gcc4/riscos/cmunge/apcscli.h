/*******************************************************************
 * File:     apcscli
 * Purpose:  APCS CLI option parsing
 * Author:   Justin Fletcher
 * Date:     29 Dec 2003
 * Note:     This header (and the companion source) is able to be built
 *           and used independantly from the rest of CMunge. It is used
 *           within other tools and as such these fails should not depend
 *           on other features of the CMunge source if possible.
 ******************************************************************/

#ifndef APCSCLI_H
#define APCSCLI_H

/* Declares the default form of APCS which will be generated */
#define DEFAULT_APCS       (APCS_SWSTACKCHECK | APCS_FPREGARGS)

/* APCS variant flags */
#define APCS_REENTRANT     (1<<0) /* Reentrant APCS */
#define APCS_32BIT         (1<<1) /* 32bit APCS (not 26bit) */
#define APCS_FPE3          (1<<2) /* FPE3 (not FPE2) */
#define APCS_SWSTACKCHECK  (1<<3) /* Software stack checking */
#define APCS_FPREGARGS     (1<<4) /* Floating point registers */
#define APCS_FP            (1<<5) /* Frame pointer */

/* Structure representing the APCS options which can be used in opt.apcs */
typedef struct apcsoptions_s {
  char *name;        /* Argument name */
  int minabbrev;     /* Minimum number of characters for abbreviation */
  unsigned long bic; /* Bits to clear */
  unsigned long orr; /* Bits to set */
  char *desc;        /* Description of the flag */
} apcsoptions_t;
extern apcsoptions_t apcsoptions[];

/*************************************************** Gerph *********
 Function:     apcscli_parse
 Description:  Parse CLI options from an argument string
 Parameters:   flags = the current APCS flags
               arg-> the string to parse
               errfunc-> the function to call when an error occurs (vararg)
 Returns:      new flags
 ******************************************************************/
unsigned long apcscli_parse(unsigned long flags, const char *arg,
                            void (*errfunc)(const char *format, ...));

/*************************************************** Gerph *********
 Function:     apcscli_help
 Description:  Display a help message, with an optional indication
               of the current state
 Parameters:   flags = the flags to display
               showstate = whether the state in 'flags' should be shown
 Returns:      none
 ******************************************************************/
void apcscli_help(unsigned long flags, int showstate);

/*************************************************** Gerph *********
 Function:     apcscli_buildstring
 Description:  Build a command line string from flags
 Parameters:   flags = the flags to build a string from
               ignoreflags = flags which shouldn't be printed
               abbreviate = 1 to only use the minimum string length
               buffer-> the buffer to fill in
               bufferlen = the length of the buffer required
 Returns:      buffer length written, or -ve length if not enough space
 ******************************************************************/
int apcscli_buildstring(unsigned long flags, unsigned long ignoreflags,
                        int abbreviate,
                        char *buffer, int bufferlen);

#endif

