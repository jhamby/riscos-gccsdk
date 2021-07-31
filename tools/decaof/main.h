/*
 * global variable declarations
 *
 * Copyright (c) 1992 Andy Duplain, andy.duplain@dsl.pipex.com
 * Copyright (c) 2005-2013 GCCSDK Developers
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */

#ifndef MAIN_HEADER_INCLUDED
#define MAIN_HEADER_INCLUDED

#include <stdbool.h>

extern const char **files;
extern int nfiles;
extern bool opt_print_area_contents;
extern bool opt_print_area_dec;
extern bool opt_print_reloc_dir;
extern bool opt_print_debug;
extern bool opt_print_symtab;
extern bool opt_print_strtab;
extern bool opt_print_ident;

#endif
