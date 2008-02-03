/*
 * Copyright (C) 1999-2003 Robin Watts/Justin Fletcher
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "error.h"
#include "mem.h"
#include "str.h"

int stricmp(const char *p, const char *q) {

  char a, b;

  while (1) {
    a = toupper(*p++);
    b = toupper(*q++);
    if (a == b) {
      if (a == 0)
        return 1;
    } else
      return 0;
  }
}

char *strdup_strip(const char *p) {

  char *res;
  int s;

  /* Skip any prefix of whitespace */
  while (isspace(*p))
    p++;
  s = strlen(p)+1;

  /* Strip any trailing whitespace */
  while (s>1 && isspace(p[s-2]))
    s--;

  res = Malloc(s);
  memcpy(res, p, s-1);
  res[s-1]='\0';
  return res;
}

const char *strduptok(const char *p, const char **resPP)
{
  const char *s;
  char *resP;

  /* Skip any prefix of whitespace */
  while (isspace(*p))
    p++;
  s = p;
  while (!isspace(*p) && (*p != ',') && (*p != ')') &&
         (*p != '(') && (*p != 0))
    p++;
  resP = Malloc(p-s+1);
  memcpy(resP, s, p-s);
  resP[p-s] = 0;
  *resPP = resP;
  /* Skip any more whitespace */
  while (isspace(*p))
    p++;
  return p;
}

int strprefix(const char *p, const char *q) {

  char a, b;

  while (1) {
    a = toupper(*p++);
    b = toupper(*q++);
    if (a == b) {
      if (b == 0)
        return 1;
    } else {
      if (b == 0)
        return 1;
      else
        return 0;
    }
  }
}

const char *strfindpair(const char *p, const char **p1PP, const char **p2PP)
{
  const char *s;
  char *p1P, *p2P;

  /* Skip any whitespace */
  while (isspace(*p))
    p++;
  s = p;
  /* Look for slash, comma, bracket or whitespace */
  while (!isspace(*p) &&
         (*p != '/') &&
         (*p != ',') &&
         (*p != '(') &&
         (*p != 0)) {
    p++;
  }
  if (p == 0)
    ErrorFatal("Failed to find handler name!");
  /* Copy the first name */
  p1P = Malloc(p-s+1);
  memcpy(p1P, s, p-s);
  p1P[p-s] = 0;
  *p1PP = p1P;

  /* Skip any more whitespace */
  while (isspace(*p))
    p++;

  /* Did we find a / ? */
  if (*p != '/') {
    *p2PP = NULL;
    /* Skip over any comma */
    if (*p == ',') {
      p++;
      /* Then any whitespace */
      while (isspace(*p))
        p++;
    }
    return p;
  }
  /* So we found a / */
  p++;
  while (isspace(*p))
    p++;
  s = p;
  while (!isspace(*p) &&
         (*p != ',') &&
         (*p != '(') &&
         (*p != 0)) {
    p++;
  }
  if (p == 0)
    ErrorFatal("Failed to find handler name!");
  /* Copy the second name */
  p2P = Malloc(p-s+1);
  memcpy(p2P, s, p-s);
  p2P[p-s] = 0;
  *p2PP = p2P;

  /* Skip any more whitespace */
  while (isspace(*p))
    p++;

  if (*p == ',') {
    p++;
    /* Skip any more whitespace */
    while (isspace(*p))
      p++;
  }
  return p;
}

const char *strcomma(const char *s) {

  while (isspace(*s))
    s++;
  if (*s == ',')
    s++;
  while (isspace(*s))
    s++;
  return s;
}

const char *strskip(const char *s, char c) {

  while (isspace(*s))
    s++;
  if (*s != c)
    ErrorFatal("Failed to find expected '%c'!", c);
  s++;
  while (isspace(*s))
    s++;
  return s;
}

const char *strstring(const char *s, const char **resPP)
{
  const char *p;
  char *resP;
  int escape;

  while (isspace(*s))
    s++;
  if (*s != '\"')
    ErrorFatal("Failed to find expected '\"'!");
  s++;
  p = s;
  escape = 0;
  while ((*s != '\"') || (escape != 0)) {
    if (*s == '\\')
      escape = !escape;
    else
      escape = 0;
    s++;
  }
  resP = Malloc(s-p+1);
  memcpy(resP, p, s-p);
  resP[s-p] = 0;
  *resPP = resP;
  
  s++;
  while (isspace(*s))
    s++;
  return s;
}

const char *strint(const char *s, unsigned int *i) {

  const char *s0;
  int paren;

  s0 = s;
  paren = 0;
  while ((isspace(*s)) || (*s == '(')) {
    if (*s == '(')
      paren++;
    s++;
  }

  if (sscanf(s, "0x%x", i)) {
    s += 2;
    while (((*s >= '0') && (*s <= '9')) ||
           ((*s >= 'A') && (*s <= 'F')) ||
           ((*s >= 'a') && (*s <= 'f')))
      s++;
  } else if (sscanf(s, "&%x", i)) {
    s++;
    while (((*s >= '0') && (*s <= '9')) ||
           ((*s >= 'A') && (*s <= 'F')) ||
           ((*s >= 'a') && (*s <= 'f')))
      s++;
  } else if (sscanf(s, "%d", i)) {
    while ((*s >= '0') && (*s <= '9'))
      s++;
  } else
    ErrorFatal("Failed to read expected number: %s", s0);

  /* Now skip closing parens etc */
  while ((*s) &&
         (isspace(*s) || (*s == ')') || (*s == '+'))) {
    if (*s == ')')
    {
      if (paren==0)
        break;
      paren--;
    }

    if (*s == '+')
    {
      unsigned int add;
      /* Followed by a + symbol so we do some very simple processing to allow
         additive maths, allowing recursion to take us around again for
         another go */
      s = strint(s+1, &add);
      *i = *i + add;
    }
    else
      s++;
  }
  /* JRF: Too many closing parentheses may be a valid situation /iff/ you have
          thing(16) - obviously that's a valid situation to have a closing
          bracket at the end */
  /* if (paren < 0) */
    /* ErrorFatal("Too many closing parentheses!"); */
  if (paren > 0)
    ErrorFatal("Mismatched closing parentheses!");
  return s;
}
