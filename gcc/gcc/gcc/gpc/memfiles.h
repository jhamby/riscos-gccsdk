/* Memory-buffered files
   Copyright (C) 1999, 2000 Free Software Foundation, Inc.

This file is part of GNU Pascal.

GNU Pascal is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 1, or (at your option)
any later version.

GNU Pascal is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Pascal; see the file COPYING. If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA. */

/*
 * Author:  Peter Gerwinski <peter@gerwinski.de>
 */


/* Currently, this only implements read-only access through the
 * fread() function family. Someone else can generalize this to
 * support read-write access and/or fprintf()-styile I/O.
 */


/* Allow multiple inclusion to work. */

#ifndef _MEMFILES_H_
#define _MEMFILES_H_


/* Include files needed by this one. */


/* Structure definitions. */

typedef struct memfile
{
  size_t size;
  char *buffer;
  long curpos;
} MEMFILE;


/* Simple definitions and enumerations. */


/* Typedefs. */


/* Include files needed by this one. */


/* Structure definitions. */


/* Global objects accessed by users of this module. */


/* Declare functions with prototypes. */

extern MEMFILE *mopen PROTO ((const char*, const char*));
extern int mclose PROTO ((MEMFILE *));
extern size_t mread PROTO ((void *, size_t, size_t, MEMFILE *));
extern long mtell PROTO ((MEMFILE *));
extern int mseek PROTO ((MEMFILE *, long, int));


/* Define macros. */


/* End of #include file. */

#endif /* _MEMFILES_H_ */
