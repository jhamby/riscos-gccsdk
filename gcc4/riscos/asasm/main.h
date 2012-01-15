/*
 * AS an assembler for ARM
 * Copyright (c) 2002-2012 GCCSDK Developers
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
 * main.h
 */

#ifndef main_header_included
#define main_header_included

#include <setjmp.h>
#include <stdbool.h>
#include <stdint.h>

#define APCS_OPT_REENTRANT	(1<<0) /* APCS is reentrant.  */
#define APCS_OPT_32BIT		(1<<1) /* APCS is 32 bit (not 26 bit).  */
#define APCS_OPT_SWSTACKCHECK	(1<<2) /* APCS does software stack checking.  */
#define APCS_OPT_FPREGARGS	(1<<3) /* APCS uses floating point registers.  */
#define APCS_OPT_FRAMEPTR	(1<<4) /* APCS uses frame pointer.  */
#define APCS_OPT_FPE3		(1<<5) /* APCS uses FP emulator 3 (instead of 2).  */

extern bool gIsAPCS;
extern uint8_t gOptionAPCS; /* OR'd APCS_OPT_* bits.  */

/* asasm options:  */
extern int option_verbose;
extern int option_pedantic;
extern int option_fussy;
extern int option_throwback;
extern int option_local;
extern int option_apcs_softfloat;
extern int option_aof;
extern bool option_abs;

extern const char *SourceFileName;

extern jmp_buf asmContinue;
extern bool asmContinueValid;
extern jmp_buf asmAbort;
extern bool asmAbortValid;

#endif
