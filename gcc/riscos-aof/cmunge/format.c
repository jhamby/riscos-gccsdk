/*
 * Copyright (C) 2002 Robin Watts/Justin Fletcher
 */

/* Formatted text printing for CMunge.
 *
 * These routines merge the code used for writing formatted lines in the
 * C header files and that used for the blank CMHG header file generation
 * resulting in a single implementation of the formatting code
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "format.h"

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
                  at which new lines will be aligned (invisible marker).
                  All subsequent lines with a \b in them will be lined
                  up such that they match.
               words will be wrapped if they would run over LINELEN
 ******************************************************************/
void format_wrap(FILE *file,
                 const char *lead1,
                 const char *lead2,
                 const char *text,
                 const char *trailer)
{
  int needspace=0;
  int indent=0;
  int lastbreak=0;
  int pos;
  int start=strlen(lead1);
  fprintf(file, "%s", lead1);
  pos=start;
  while (*text)
  {
    int wordlen=strcspn(text,"\b\t\n ");
    if (wordlen!=0)
    {
      if (needspace && pos+needspace+wordlen > LINELEN)
      {
        int count;
        fprintf(file,"\n%s",lead2);
        pos=start+indent;
        for (count=indent; count>0; count--)
          fputc(' ',file);
        needspace=0;
      }
      if (needspace)
      {
        fputc(' ',file);
        pos++;
      }
      fwrite(text,1,wordlen,file);
      text+=wordlen;
      pos+=wordlen;
      needspace=1;
    }

    /* Put any trailing characters and perform operations necessary after
       this break character */
    switch (*text)
    {
      case '\t':
        text++;
        do {
          fputc(' ',file);
          pos++;
        } while (((pos-start)%2) != 0);
        indent=pos-start;
        needspace=0;
        break;

      case '\b':
        text++;
        /* Put a single space separator */
        fputc(' ',file);
        pos++;

        if (lastbreak==0)
        {
          const char *search;

          lastbreak=pos-start;

          /* Work out what the furthest along a break is */
          for (search = strchr(text, '\n'); search;
               search = strchr(search, '\n'))
          {
            int searchpos=0;
            int foundpos=0;
            for (++search; *search && *search!='\n'; ++search)
            {
              if (*search=='\t')
                searchpos+=2-(pos%2);
              else
              {
                searchpos++;
                if (*search=='\b')
                  foundpos=searchpos;
              }
            }
            if (foundpos==0)
              break;
            if (foundpos+1>lastbreak)
              lastbreak=foundpos+1;
          }
        }
        indent=lastbreak;

        /* Now skip up to it */
        {
          int skip;
          for (skip=indent+start - pos; skip>0; skip--)
            fputc(' ',file);
          pos=indent+start;
        }

        needspace=0;
        break;

      case ' ':
        text++;
        break;

      case '\n':
        text++;
        fprintf(file, "\n%s", lead2);
        pos=start;
        needspace=0;
        indent=0;
        break;

      case '\0':
        /* End of the line - we are done with this formatted string, and
           this nul will cause the while loop to terminate. */
        break;
    }
  }
  fprintf(file, "%s\n", trailer);
}
