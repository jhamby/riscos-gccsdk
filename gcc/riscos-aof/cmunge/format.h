/*
 * Copyright (C) 1999-2002 Robin Watts/Justin Fletcher
 */

/* Formatted text printing for CMunge.
 *
 * These routines merge the code used for writing formatted lines in the
 * C header files and that used for the blank CMHG header file generation
 * resulting in a single implementation of the formatting code
 */

#ifndef FORMAT_H
#define FORMAT_H

#include <stdio.h>

/* The length of lines that we re-format */
#define LINELEN (76)
/* #define LINELEN (96) */

/*************************************************** Gerph *********
 Function:     format_wrap
 Description:  Format a line in a wrapped manner
 Parameters:   file-> the file to write to
               lead1-> the string to use to lead the first line
               lead2-> the string to use to lead subsequent lines
               text-> the string to write (wrapping)
               trailer-> the string to write at the end of the last
                         line
 Returns:      none
 Note:         \n will be interpreted as a forced newline
               spaces will be condensed into a single space
               \t will indent to the next 2 character tab point until
                  the next newline.
               \b anywhere in a line will be treated as a break point
                  at which new lines will be aligned (invisible marker)
               words will be wrapped if they would run over LINELEN
 ******************************************************************/
void format_wrap(FILE *file,
                 const char *lead1,
                 const char *lead2,
                 const char *text,
                 const char *trailer);

#endif
