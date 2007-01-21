/*
 * Copyright (C) 2002 Robin Watts/Justin Fletcher
 */

/* Comment writing routines
 *
 * These are purely helper functions that ensure that comments appear in a
 * standard style when output by CMunge
 */

#ifndef COMMENTS_H
#define COMMENTS_H

#include <stdio.h>

/*************************************************** Gerph *********
 Function:     c_comment
 Description:  Helper function to make writing out comments simpler
 Parameters:   file-> the file to write to
               text-> the text to write to the file in a comment
 Returns:      none
 ******************************************************************/
void c_comment(FILE *file,char *text);

/*************************************************** Gerph *********
 Function:     asm_comment
 Description:  Helper function to make writing out comments simpler
 Parameters:   file-> the file to write to
               text-> the text to write to the file in a comment
 Returns:      none
 ******************************************************************/
void asm_comment(FILE *file,char *text);

#endif
