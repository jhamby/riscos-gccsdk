/*
** Drlink AOF Linker
**
** Copyright (c) 1993, 1994, 1995, 1996, 1997, 1998  David Daniels
** Copyright (c) 2001, 2002, 2003, 2004, 2005  GCCSDK Developers
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
**
** This file contains some of the 'link edit' functions. The
** actual editing itself is carried out in the 'symbols' module.
** The link edit commands are not the same as Acorn's as those
** are undocumented. In fact, there seems to be no mention of
** them in link's code either!
*/

#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "drlhdr.h"
#include "edithdr.h"
#include "procdefs.h"

/*
** Variables referenced outside this file
*/
editcmd
  *symedit_list,			/* List of symbols to edit */
  *refedit_list,			/* List of references to change */
  *new_entry;				/* Replacement entry point */

const char
  *editname;				/* Name of edit file */
char
  *editptr,				/* Pointer into link edit file buffer */
  *editendptr;				/* Pointer to end of link edit buffer */

int
  symedit_count,			/* Count of rename/hide/reveal commands */
  refedit_count;			/* Count of change commands */

/* Private declarations */

typedef enum {IDENT, LPAREN, RPAREN, COMMA, NOTHING} editsymbol;

static editsymbol editsym;
static char *ident;
static int editline;

#define LINEFEED 10

/*
** 'edit_error' lists the edit error message passed to it
*/
static void edit_error(const char *message) {
  char text[MSGBUFLEN];
  strcpy(text, message);
  strcat(text, " at line %d in edit file '%s'");
  error(text, editline, editname);
}

/*
** 'getsym' returns the next item in the edit file, pointing
** 'editptr' at the char following the token.
*/
static void getsym(void) {
  char *cp;
  char ch;
  cp = editptr;
  editsym = NOTHING;
  while (cp!=editendptr && *cp<=' ') {
    if (*cp==LINEFEED) editline+=1;
    cp++;
  }
  if (cp!=editendptr) {
    ch = *cp;
    cp++;
    switch (ch) {
    case '(':
      editsym = LPAREN;
      break;
    case ')':
      editsym = RPAREN;
      break;
    case ',':
      editsym = COMMA;
      break;
    default:
      editsym = IDENT;
      ident = cp-1;
      ch = *cp;
      while (cp!=editendptr && ch>' ' && ch!='(' && ch!=')' && ch!=',') {
        cp++;
        ch = *cp;
      }
    }
  }
  editptr = cp;
}

/*
** 'find_cmd' tries to identify the current token as a link
** edit command
*/

#define MAXLEN 6

static editcommands find_cmd(void) {
  typedef struct {const char *cmdname; editcommands cmd;} commands;
  commands cmdlist [] = {
/* rename */	   {"rename", EDT_RENAME},
/* change */	   {"change", EDT_CHANGE},
/* hide */	   {"hide", EDT_HIDE},
/* reveal */	   {"reveal", EDT_REVEAL},
/* entry */	   {"entry", EDT_ENTRY}
  };
  unsigned int n;
  char *p;
  char name[10];
  n = 0;
  p = ident;
  while (p!=editptr && n<MAXLEN) {
    name[n] = tolower(*p);
    n++;
    p++;
  }
  name[n] = NULLCHAR;
  n = 0;
  while (n<sizeof(cmdlist)/sizeof(commands) && strcmp(cmdlist[n].cmdname, name)!=0) n++;
  if (n<sizeof(cmdlist)/sizeof(commands)) {	/* Command found */
    return cmdlist[n].cmd;
  }
  else {
    edit_error("Error: Invalid edit command found");
    return EDT_BAD;
  }
}

/*
** 'insert_null' is called to add a null after a symbol or
** filename
*/
static void insert_null(char *cp) {
  while (*cp>' ' && *cp!='(' && *cp!=')' && *cp!=',') cp++;
  *cp = NULLCHAR;
}

/*
** 'insert_edit' is called to insert a new edit command into
** the list of edits passed to it. The lists are ordered by
** the hash value of the file name so that all edits for
** one file are grouped together
*/
static void insert_edit(editcmd **editlist, editcmd *newedit) {
  int hashval;
  editcmd *ep, *lastep;
  ep = *editlist;
  lastep = NIL;
  hashval = newedit->edtfnhash;
  while (ep!=NIL && hashval>ep->edtfnhash) {
    lastep = ep;
    ep = ep->edtnext;
  }
  if (lastep==NIL) {	/* List is empty or new first entry */
    newedit->edtnext = *editlist;
    *editlist = newedit;
  }
  else {
    newedit->edtnext = lastep->edtnext;
    lastep->edtnext = newedit;
  }
}

/*
** 'parse_command' parses a link edit command, returing 'TRUE'
** if it is okay otherwise returning 'FALSE'. Edit commands are
** all of the form:
**   <command> <file>(<old_symbol> {,<new_symbol>})
** although only the 'rename' and 'change' commands have
** '<new_symbol>'.
** The edit commands are added to two lists, one for symbol
** edits and one for symbol reference edits. All the changes
** for one AOF file are grouped together by ordering the
** lists by hash value of filename
*/
static bool parse_command(void) {
  editcommands thiscmd;
  char *oldsymbol, *newsymbol, *file;
  editcmd *p;
  thiscmd = find_cmd();
  if (thiscmd==EDT_BAD) return FALSE;
  getsym();
  if (editsym!=IDENT) {
    edit_error("Error: File name expected");
    return FALSE;
  }
  file = ident;
  getsym();
  if (editsym!=LPAREN) {
    edit_error("Error: '(' expected after file name");
    return FALSE;
  }
  getsym();
  if (editsym!=IDENT) {
    edit_error("Error: Symbol name expected");
    return FALSE;
  }
  oldsymbol = ident;
  newsymbol = NIL;
  getsym();
  if (thiscmd==EDT_CHANGE || thiscmd==EDT_RENAME) {
    if (editsym!=COMMA) {
      edit_error("Error: ',' expected after old symbol name");
      return FALSE;
    }
    getsym();
    if (editsym!=IDENT) {
      edit_error("Error: Symbol name expected");
      return FALSE;
    }
    newsymbol = ident;
    getsym();
    insert_null(newsymbol);
  }
  if (editsym!=RPAREN) {
    edit_error("Error: ')' expected after symbol name");
    return FALSE;
  }
  insert_null(file);
  insert_null(oldsymbol);
  if (thiscmd==EDT_ENTRY && new_entry!=NIL) {
    edit_error("Error: More than one 'entry' command found");
    return FALSE;
  }
  if ((p = allocmem(sizeof(editcmd)))==NIL) {
    error("Fatal: Out of memory in 'parse_command'");
  }
  p->edtoper = thiscmd;
  p->edtdone = FALSE;
  p->edtfile = file;
  p->edtfnhash = hash(file);
  p->edtold = oldsymbol;
  p->edtsyhash = hash(oldsymbol);
  p->edtnew = newsymbol;
  switch (thiscmd) {
  case EDT_RENAME:
  case EDT_HIDE:
  case EDT_REVEAL:
    insert_edit(&symedit_list, p);
    symedit_count+=1;
    break;
  case EDT_CHANGE:
    insert_edit(&refedit_list, p);
    refedit_count+=1;
    break;
  case EDT_ENTRY:
    new_entry = p;
  default:
    break;
  }
  return TRUE;
}

/*
** 'scan_editfile' is the main link edit file scan routine
*/
bool scan_editfile(void) {
  bool ok;
  ok = TRUE;
  editline = 1;
  getsym();
  do {
    if (editsym!=IDENT) {
      edit_error("Error: Command not found where expected");
      ok = FALSE;
    }
    else {
      ok = parse_command();
      if (ok) getsym();
    }
  } while (ok && editsym!=NOTHING);
  return ok;
}
/*
** 'list_badedits' lists edits that were not carried out
*/
static void list_badedits(editcmd *p) {
  const char *opnames [] = {"", "rename", "change", "hide", "reveal", "entry"};

  while (p!=NIL) {
    if (!p->edtdone) error("    %s '%s' in '%s'", opnames[p->edtoper], p->edtold, p->edtfile);
    p = p->edtnext;
  }
}


/*
** 'verify_edits' is called to ensure that all the edits
** in the edit file(s) have been carried out
*/
bool verify_edits(void) {
  if (symedit_count==0 && refedit_count==0) return TRUE;
  error("Warning: The following link edits were not carried out:");
  if (symedit_count!=0) list_badedits(symedit_list);
  if (refedit_count!=0) list_badedits(refedit_list);
  return FALSE;
}

void init_edit(void) {
  symedit_list = refedit_list = NIL;
  symedit_count = refedit_count = 0;
  new_entry = NIL;
}
