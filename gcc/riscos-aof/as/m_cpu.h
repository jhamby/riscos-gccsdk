/*
 * m_cpu.h
 * Copyright © 1992 Niklas Röjemo
 */

#ifndef m_cpu_header_included
#define m_cpu_header_included

#define M_MNEM 0x01e00000

#define M_ADC 0x00a00000
#define M_ADD 0x00800000
#define M_AND 0x00000000
#define M_BIC 0x01c00000
#define M_EOR 0x00200000
#define M_MOV 0x01a00000
#define M_MVN 0x01e00000
#define M_ORR 0x01800000
#define M_RSB 0x00600000
#define M_RSC 0x00e00000
#define M_SBC 0x00c00000
#define M_SUB 0x00400000
#define M_CMN 0x01600000
#define M_CMP 0x01400000
#define M_TEQ 0x01200000
#define M_TST 0x01000000
#define M_MLA 0x00200090
#define M_MUL 0x00000090

#define M_UMULL 0x00800090
#define M_UMLAL 0x00A00090
#define M_SMULL 0x00C00090
#define M_SMLAL 0x00E00090

#endif
