/*
 * AS an assembler for ARM
 * Copyright © 1992 Niklas Röjemo
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
 * aoffile.h
 * Types that exist in an AOF file.
 */

#ifndef main_header_included
#define main_header_included

#include <setjmp.h>

/* AS options :
 */
extern int verbose;
extern int pedantic;
extern int fussy;
extern int throwback;
extern int dde;
extern int autocast;
extern int align;
extern int gcc_backend;
extern int gas_backend;
extern int local;
extern int objasm;
extern int uc;
extern int apcs_32bit;
extern int apcs_fpv3;
extern int apcs_softfloat;
extern int rma_module;
extern int elf;

extern const char *ProgName;

extern jmp_buf asmContinue;
extern BOOL asmContinueValid;
extern jmp_buf asmAbort;
extern BOOL asmAbortValid;

#endif
