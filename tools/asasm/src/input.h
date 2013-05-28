/*
 * AsAsm an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 2001-2013 GCCSDK Developers
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

#ifndef input_header_included
#define input_header_included

#include <stdbool.h>
#include <stddef.h>

#include "phase.h"

void Input_PrepareForPhase (Phase_e phase);

bool Input_AddPredefine (const char *preDef);

bool Input_Match (char c, bool spacesToo);
bool Input_MatchKeyword (const char *keyword);
bool Input_MatchKeywordLower (const char *keyword);
bool Input_MatchString (const char *str);
bool Input_MatchStringLower (const char *str);

bool Input_IsEndOfKeywordN (size_t n);
static inline bool
Input_IsEndOfKeyword (void)
{
  return Input_IsEndOfKeywordN (0);
}


typedef enum
{
  eNoVarSubst,
  eVarSubstNoWarning,
  eVarSubst
} Level_e;

bool Input_NextLine (Level_e level);

void Input_SkipWS (void);

bool Input_IsEolOrCommentStart (void);
char Input_Look (void);
char Input_LookLower (void);
char Input_LookN (int n);
char Input_LookNLower (int n);
char Input_GetC (void);
char Input_GetCLower (void);
void Input_UnGetC (char c);
const char *Input_Rest (void);
#if DEBUG
const char *Input_Line (void);
#endif
void Input_Skip (void);
void Input_SkipN (size_t n);

char *Input_GetString (size_t *len);

const char *Input_Symbol (size_t *ilen);
const char *Input_Symbol2 (size_t *ilen, char del);

const char *Input_GetMark (void);
void Input_RollBackToMark (const char *mark);

void Input_ShowLine (void);
size_t Input_GetColumn (void);

void Input_ThisInstead (const char *);

#endif
