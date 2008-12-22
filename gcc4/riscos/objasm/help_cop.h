/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas R�emo
 * Copyright (c) 2002-2006 GCCSDK Developers
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
 * help_cop.h
 */

#ifndef help_cop_header_included
#define help_cop_header_included

#define CP_NUMBER(n) ((n)<< 8)
#define CP_DCODE(n)  ((n)<<20)
#define CP_RTRAN(n)  ((n)<<21)
#define CPDST_OP(n)  ((n)<<12)
#define CPRHS_OP(n)  ((n)<< 0)
#define CPLHS_OP(n)  ((n)<<16)
#define CP_INFO(n)   ((n)<< 5)

#define FROM_CP_BIT  (1<<20)

int help_copInt (int max, const char *msg);

WORD help_copAddr (WORD ir, BOOL stack);

#endif
