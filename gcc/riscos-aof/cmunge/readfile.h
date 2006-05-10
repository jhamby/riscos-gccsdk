/*
 * Copyright (C) 1999 Robin Watts/Justin Fletcher
 */

#ifndef READFILE_H

#define READFILE_H

#include <stdio.h> /* for FILE * */

/* We have a field declaration in the C file that we need to use elsewhere
   for blank generation */
typedef enum
{
  ft_none,     /* Does nothing - only valid for functions */
  ft_flag_on,  /* A flag, or if no flag defaults to 'on' */
  ft_flag_off, /* A flag, or if no flag defaults to 'off' */
  ft_alias,    /* An alias for another entry, name in 'value' */
  ft_int,      /* Any integer, non-zero (0=unassigned) */
  ft_uint,     /* Any integer, unsigned (-1=unassigned) */
  ft_int64,    /* 64 aligned integer */
  ft_xint64,   /* 64 aligned integer without x set */
  ft_string,   /* A string */
  ft_quoted,   /* A string in quotes */
  ft_label,    /* A label (one word string) */
  ft_check     /* Special case for functions; should check the value is 0 */
} fieldtype;

#define IN_CMHG   (1)
#define IN_CMUNGE (2)
#define IN_ERRORS (4)
#define IN_BOTH   (IN_CMHG | IN_CMUNGE)

/* The main definition handler uses a structure to describe each of the
   keyword fields we support */
typedef void (*field_f)(const char *trailer,FILE *file);
typedef void (*fieldblank_f)(FILE *file);
typedef struct {
  char *name;        /* The name of this field */
  struct {
    field_f func;    /* Function to call to process this, or NULL if none */
    fieldtype type;  /* What type of entry this is, if no function */
    void *value;     /* Where to store the value of this field */
  } values;

  struct {
    char *value;     /* The default vaue to give it, or NULL to comment it */
    char *comment;   /* The comment to preceed it */
    fieldblank_f func; /* Function to call for additional blank output */
  } blank;
  int hgtype;        /* Which header generator it is in */
} field_t;

/* The command definition handler uses a structure to describe each of the
   keyword fields it supports. Because commands are the most complex and
   structured of the keywords, they have a similar structure to that of the
   main definitions. */
typedef const char * (*cmdfield_f)(const char *trailer,
                                   struct help_list_s *helpl);
typedef struct {
  char *name;        /* The name of this field */
  struct {
    cmdfield_f func; /* Function to call to process this, or NULL if none */
    fieldtype type;  /* What type of entry this is, if no function */
    void *value;     /* Where to store the value of this field */
  } values;

  struct {
    char *value;     /* The default vaue to give it, or NULL to comment it */
    char *comment;   /* The comment to preceed it */
  } blank;
  int hgtype;        /* Which header generator it is in */
} cmdfield_t;

extern field_t fields[];       /* This declares all the fields we know */
extern cmdfield_t cmdfields[]; /* This declares all the fields we know */

void ReadFile(void);

#endif
