/* ARM/RISC OS declarations to prevent some undefined symbol warnings
in the link phase for GNU compiler.
Copyright (C) 1996 Free Software Foundation, Inc.
Contributed by Nick Burrett (n.a.burrett@mcs.salford.ac.uk)

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.

As a special exception, if you link this library with other files,
some of which are compiled with GCC, to produce an executable,
this library does not by itself cause the resulting executable
to be covered by the GNU General Public License.
This exception does not however invalidate any other reasons why
the executable file might be covered by the GNU General Public License.  */

/* Version 1.03 (09 May 1996)  */

	AREA	|C$$code|, CODE, READONLY

	EXPORT	|_exit_dummy_decl|
	/* Referenced in libgcc2.c but never declared there for our
	   particular configuration.  */
|_exit_dummy_decl|
	DCD	0


	END
