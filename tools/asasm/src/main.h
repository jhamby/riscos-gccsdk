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
#define APCS_OPT_ROPI		(1<<6) /* APCS PIC attribute.  */
#define APCS_OPT_SOFTFP		(1<<7) /* APCS soft fp attribute.  */
#define APCS_OPT_INTERWORK	(1<<8) /* APCS interwork attribute.  */
#define APCS_OPT_VFPENDIAN	(1<<9) /* VFP endian instead of FPA endian.  */

typedef enum
{
  eAPCS_None, /* R0-R15, r0-r15, SP, sp, LR, lr, PC and pc are predefined.  */
  eAPCS_v3, /* R0-R15, r0-r15, SP, sp, LR, lr, PC, pc, a1-a4, v1-v5 and
    ip are predefined.  Depending on /reentrant, /swstackcheck and /fp sb, sl,
    fp and v6-v8 can be predefined as well.  */
  eAPCS_Empty /* Like eAPCS_v3 and SB, SL, FP and IP are predefined as well.  */
} APCS_Version_e;

extern APCS_Version_e gOptionVersionAPCS;
extern uint32_t gOptionAPCS; /* OR'd APCS_OPT_* bits.  */

typedef enum
{
  eNone, /* No registers predefined.  */
  eCallStd, /* Standard set of registers predefined, depending on APCS option as well.  */
  eAll /* All registers predefined.  */
} RegNames_e;

extern RegNames_e gOptionRegNames;

/* Command line options:  */
extern bool option_abs;
extern int option_aof;
extern int option_fussy;
extern bool option_no_code_gen;
extern bool option_nowarn;
extern int option_pedantic;
extern int option_throwback;
extern bool option_uppercase;
extern int option_verbose;
extern bool option_debug;

extern const char *SourceFileName;

extern jmp_buf asmContinue;
extern bool asmContinueValid;
extern jmp_buf asmAbort;
extern bool asmAbortValid;

#endif
