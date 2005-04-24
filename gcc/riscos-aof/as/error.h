/*
 * Copyright © 1992 Niklas Röjemo
 * Copyright © 2005 GCCSDK Developers
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * error.h
 */

#ifndef error_header_included
#define error_header_included

#include "global.h"

typedef enum
{
  ErrorInfo,
  ErrorWarning,
  ErrorError,
  ErrorSerious,
  ErrorAbort
}
ErrorTag;

extern const char *const InsertCommaAfter;

void errorInit (const char *filename);
void errorFinish (void);
void errorLine (long int lineno, const char *file,
		ErrorTag e, BOOL c, const char *format, ...);
void error (ErrorTag e, BOOL c, const char *format, ...);
void errorOutOfMem (const char *fn);

int returnExitStatus (void);

#endif
