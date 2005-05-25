/*
** Drlink AOF linker
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
** Linker edit facilities definitions
*/

typedef enum
{				/* Types of edit commands */
  EDT_BAD, EDT_RENAME, EDT_CHANGE, EDT_HIDE, EDT_REVEAL, EDT_ENTRY
} editcommands;

typedef struct editcmd
{				/* Defines one link edit command */
  editcommands edtoper;		/* Which edit operation to perform */
  bool edtdone;			/* TRUE if operation has been carried out */
  char *edtfile;		/* File to edit */
  int edtfnhash;		/* Hashed version of file name */
  char *edtold;			/* 'from' symbol name */
  int edtsyhash;		/* Hashed version of symbol name */
  char *edtnew;			/* 'to' name */
  struct editcmd *edtnext;	/* Next command in list */
} editcmd;

extern editcmd * symedit_list,	/* List of symbols to edit */
 *refedit_list,			/* List of references to change */
 *new_entry;			/* Replacement entry point */

extern const char *editname;	/* Name of edit file */
extern char *editptr,		/* Pointer into link edit file buffer */
 *editendptr;			/* Pointer to end of link edit buffer */

extern int symedit_count,	/* Count of rename/hide/reveal commands */
  refedit_count;		/* Count of change commands */
