/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2002-2013 GCCSDK Developers
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

#include <stdbool.h>

#include "global.h"

#define CP_NUMBER(n) ((n)<< 8) /* CP# */
#define CP_DCODE(n)  ((n)<<20) /* CP Opc */
#define CP_RTRAN(n)  ((n)<<21) /* ??? */
#define CPDST_OP(n)  ((n)<<12) /* CRd */
#define CPRHS_OP(n)  ((n)<< 0) /* CRm */
#define CPLHS_OP(n)  ((n)<<16) /* CRn */
#define CP_INFO(n)   ((n)<< 5) /* CP */

#define CP_GET_NUMBER(ir) (((ir)>>8) & 0xF) /**< Get coprocessor number.  */
#define CP_GET_LHS(ir) (((ir)>>16) & 0xF)

void HelpCop_Addr (ARMWord ir, bool literal, bool stack, bool isThumb);

#endif
