/*
** Drlink AOF linker
**
** Copyright � David Daniels 1993, 1994, 1995, 1996, 1997, 1998.
** All rights reserved.
**
** Linker edit facilities definitions
*/

typedef enum {	/* Types of edit commands */
    EDT_BAD, EDT_RENAME, EDT_CHANGE, EDT_HIDE, EDT_REVEAL, EDT_ENTRY} editcommands;

typedef struct editcmd {		/* Defines one link edit command */
  editcommands edtoper;			/* Which edit operation to perform */
  bool edtdone;				/* TRUE if operation has been carried out */
  char *edtfile;			/* File to edit */
  int edtfnhash;			/* Hashed version of file name */
  char *edtold;				/* 'from' symbol name */
  int edtsyhash;			/* Hashed version of symbol name */
  char *edtnew;				/* 'to' name */
  struct editcmd *edtnext;		/* Next command in list */
} editcmd;

extern editcmd
  *symedit_list,			/* List of symbols to edit */
  *refedit_list,			/* List of references to change */
  *new_entry;				/* Replacement entry point */

extern const char
  *editname;				/* Name of edit file */
extern char
  *editptr,				/* Pointer into link edit file buffer */
  *editendptr;				/* Pointer to end of link edit buffer */

extern int
  symedit_count,			/* Count of rename/hide/reveal commands */
  refedit_count;			/* Count of change commands */
