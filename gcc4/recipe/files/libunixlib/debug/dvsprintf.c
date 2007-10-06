/* Linux kernel vsprintf modified for UnixLib debugging.
   Copyright (c) 2005-2007 UnixLib Devlopers.

   This must be compiled without stack checking.  */

/*
 *  linux/lib/vsprintf.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

/* vsprintf.c -- Lars Wirzenius & Linus Torvalds. */
/*
 * Wirzenius wrote this portably, Torvalds fucked it up :-)
 */

/* 
 * Fri Jul 13 2001 Crutcher Dunnavant <crutcher+kernel@datastacks.com>
 * - changed to provide snprintf and vsnprintf functions
 */

#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#define DEBUG
#include <sys/debug.h>
#include <unixlib/unix.h>
#include <unixlib/os.h>

static __inline int local_isxdigit (const char x)
{
  return ((x >= '0' && x <= '9')
	  || (x >= 'A' && x <= 'F')
	  || (x >= 'a' && x <= 'f')) ? 1 : 0;
}

static __inline int local_isdigit (const char x)
{
  return (x >= '0' && x <= '9') ? 1 : 0;
}

static __inline char local_toupper (const char x)
{
  return (x >= 'a') ? x - 26 + 7 : x;
}

static __inline int local_islower (const char x)
{
  return (x >= 'a' && x <= 'z') ? 1 : 0;
}

static __inline size_t local_strnlen (const char *s, size_t maxlen)
{
  size_t r;
  for (r = 0; s[r] && r < maxlen; ++r)
    /* */;
  return r;
}

/**
 * simple_strtoul - convert a string to an unsigned long
 * @cp: The start of the string
 * @endp: A pointer to the end of the parsed string will be placed here
 * @base: The number base to use
 */
static unsigned long simple_strtoul(const char *cp,
				    char **endp,unsigned int base)
{
  unsigned long result = 0,value;

  if (!base) {
    base = 10;
    if (*cp == '0') {
      base = 8;
      cp++;
      if ((*cp == 'x') && local_isxdigit(cp[1])) {
	cp++;
	base = 16;
      }
    }
  }
  while (local_isxdigit(*cp) &&
	 (value = local_isdigit(*cp) ? *cp-'0' : local_toupper(*cp)-'A'+10) < base) {
    result = result*base + value;
    cp++;
  }
  if (endp)
    *endp = (char *)cp;
  return result;
}

/**
 * simple_strtol - convert a string to a signed long
 * @cp: The start of the string
 * @endp: A pointer to the end of the parsed string will be placed here
 * @base: The number base to use
 */
static long simple_strtol(const char *cp,char **endp,unsigned int base)
{
  if (*cp == '-')
    return -simple_strtoul(cp+1,endp,base);
  return simple_strtoul(cp,endp,base);
}

/**
 * simple_strtoull - convert a string to an unsigned long long
 * @cp: The start of the string
 * @endp: A pointer to the end of the parsed string will be placed here
 * @base: The number base to use
 */
static unsigned long long simple_strtoull(const char *cp,char **endp,unsigned int base)
{
  unsigned long long result = 0,value;

  if (!base) {
    base = 10;
    if (*cp == '0') {
      base = 8;
      cp++;
      if ((*cp == 'x') && local_isxdigit(cp[1])) {
	cp++;
	base = 16;
      }
    }
  }
  while (local_isxdigit(*cp) && (value = local_isdigit(*cp) ? *cp-'0' : (local_islower(*cp)
							     ? local_toupper(*cp) : *cp)-'A'+10) < base) {
    result = result*base + value;
    cp++;
  }
  if (endp)
    *endp = (char *)cp;
  return result;
}

/**
 * simple_strtoll - convert a string to a signed long long
 * @cp: The start of the string
 * @endp: A pointer to the end of the parsed string will be placed here
 * @base: The number base to use
 */
static long long simple_strtoll(const char *cp,char **endp,unsigned int base)
{
  if(*cp=='-')
    return -simple_strtoull(cp+1,endp,base);
  return simple_strtoull(cp,endp,base);
}

static int skip_atoi(const char **s)
{
  int i=0;

  while (local_isdigit(**s))
    i = i*10 + *((*s)++) - '0';
  return i;
}

#define ZEROPAD	1		/* pad with zero */
#define SIGN	2		/* unsigned/signed long */
#define PLUS	4		/* show plus */
#define SPACE	8		/* space if plus */
#define LEFT	16		/* left justified */
#define SPECIAL	32		/* 0x */
#define LARGE	64		/* use 'ABCDEF' instead of 'abcdef' */

static char * number(char * buf, char * end, long long num,
		     int base, int size, int precision, int type)
{
  char c,sign,tmp[66];
  const char *digits;
  static const char small_digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
  static const char large_digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  int i;

  digits = (type & LARGE) ? large_digits : small_digits;
  if (type & LEFT)
    type &= ~ZEROPAD;
  if (base < 2 || base > 36)
    return 0;
  c = (type & ZEROPAD) ? '0' : ' ';
  sign = 0;
  if (type & SIGN) {
    if (num < 0) {
      sign = '-';
      num = -num;
      size--;
    } else if (type & PLUS) {
      sign = '+';
      size--;
    } else if (type & SPACE) {
      sign = ' ';
      size--;
    }
  }
  if (type & SPECIAL) {
    if (base == 16)
      size -= 2;
    else if (base == 8)
      size--;
  }
  i = 0;
  if (num == 0)
    tmp[i++]='0';
  else
    {
      while (num != 0)
	{
	  int res = num % base;
	  num /= base;
	  tmp[i++] = digits[res];
	}
    }
  if (i > precision)
    precision = i;
  size -= precision;
  if (!(type&(ZEROPAD+LEFT))) {
    while(size-->0) {
      if (buf <= end)
	*buf = ' ';
      ++buf;
    }
  }
  if (sign) {
    if (buf <= end)
      *buf = sign;
    ++buf;
  }
  if (type & SPECIAL) {
    if (base==8) {
      if (buf <= end)
	*buf = '0';
      ++buf;
    } else if (base==16) {
      if (buf <= end)
	*buf = '0';
      ++buf;
      if (buf <= end)
	*buf = digits[33];
      ++buf;
    }
  }
  if (!(type & LEFT)) {
    while (size-- > 0) {
      if (buf <= end)
	*buf = c;
      ++buf;
    }
  }
  while (i < precision--) {
    if (buf <= end)
      *buf = '0';
    ++buf;
  }
  while (i-- > 0) {
    if (buf <= end)
      *buf = tmp[i];
    ++buf;
  }
  while (size-- > 0) {
    if (buf <= end)
      *buf = ' ';
    ++buf;
  }
  return buf;
}

/**
 * vsnprintf - Format a string and place it in a buffer
 * @buf: The buffer to place the result into
 * @size: The size of the buffer, including the trailing null space
 * @fmt: The format string to use
 * @args: Arguments for the format string
 *
 * Call this function if you are already dealing with a va_list.
 * You probably want snprintf instead.
 */
static int _vsnprintf(char *buf, size_t size, const char *fmt, va_list args)
{
  int len;
  unsigned long long num;
  int i, base;
  char c;
  const char *s;

  int flags;		/* flags to number() */

  int field_width;	/* width of output field */
  int precision;		/* min. # of digits for integers; max
				   number of chars for from string */
  int qualifier;		/* 'h', 'l', or 'L' for integer fields */
				/* 'z' support added 23/7/1999 S.H.    */
				/* 'z' changed to 'Z' --davidm 1/25/99 */

  char *str = buf;
  char *end = buf + size - 1;

  if (end < buf - 1) {
    end = ((void *) -1);
    size = end - buf + 1;
  }

  for (; *fmt ; ++fmt) {
    if (*fmt != '%') {
      if (str <= end)
	*str = *fmt;

      /* Because we are using RISC OS SWI calls to output the resultant
	 string, all newlines must be CR-LF.  */
      if (*fmt == '\n')
	*++str = '\r';

      ++str;
      continue;
    }

    /* process flags */
    flags = 0;
  repeat:
    ++fmt;		/* this also skips first '%' */
    switch (*fmt) {
    case '-': flags |= LEFT; goto repeat;
    case '+': flags |= PLUS; goto repeat;
    case ' ': flags |= SPACE; goto repeat;
    case '#': flags |= SPECIAL; goto repeat;
    case '0': flags |= ZEROPAD; goto repeat;
    }

    /* get field width */
    field_width = -1;
    if (local_isdigit(*fmt))
      field_width = skip_atoi(&fmt);
    else if (*fmt == '*') {
      ++fmt;
      /* it's the next argument */
      field_width = va_arg(args, int);
      if (field_width < 0) {
	field_width = -field_width;
	flags |= LEFT;
      }
    }

    /* get the precision */
    precision = -1;
    if (*fmt == '.') {
      ++fmt;	
      if (local_isdigit(*fmt))
	precision = skip_atoi(&fmt);
      else if (*fmt == '*') {
	++fmt;
	/* it's the next argument */
	precision = va_arg(args, int);
      }
      if (precision < 0)
	precision = 0;
    }

    /* get the conversion qualifier */
    qualifier = -1;
    if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L' ||
	*fmt =='Z' || *fmt == 'z') {
      qualifier = *fmt;
      ++fmt;
      if (qualifier == 'l' && *fmt == 'l') {
	qualifier = 'L';
	++fmt;
      }
    }

    /* default base */
    base = 10;

    switch (*fmt) {
    case 'c':
      if (!(flags & LEFT)) {
	while (--field_width > 0) {
	  if (str <= end)
	    *str = ' ';
	  ++str;
	}
      }
      c = (unsigned char) va_arg(args, int);
      if (str <= end)
	*str = c;
      ++str;
      while (--field_width > 0) {
	if (str <= end)
	  *str = ' ';
	++str;
      }
      continue;

    case 's':
      s = va_arg(args, char *);
      if ((unsigned long)s < 0x1000)
	s = "<NULL>";

      len = local_strnlen(s, precision);

      if (!(flags & LEFT)) {
	while (len < field_width--) {
	  if (str <= end)
	    *str = ' ';
	  ++str;
	}
      }
      for (i = 0; i < len; ++i) {
	if (str <= end)
	  *str = *s;
	++str; ++s;
      }
      while (len < field_width--) {
	if (str <= end)
	  *str = ' ';
	++str;
      }
      continue;

    case 'p':
      if (field_width == -1) {
	field_width = 2*sizeof(void *);
	flags |= ZEROPAD;
      }
      str = number(str, end,
		   (unsigned long) va_arg(args, void *),
		   16, field_width, precision, flags);
      continue;


    case 'n':
      /* FIXME:
       * What does C99 say about the overflow case here? */
      if (qualifier == 'l') {
	long * ip = va_arg(args, long *);
	*ip = (str - buf);
      } else if (qualifier == 'Z' || qualifier == 'z') {
	size_t * ip = va_arg(args, size_t *);
	*ip = (str - buf);
      } else {
	int * ip = va_arg(args, int *);
	*ip = (str - buf);
      }
      continue;

    case '%':
      if (str <= end)
	*str = '%';
      ++str;
      continue;

      /* integer number formats - set up the flags and "break" */
    case 'o':
      base = 8;
      break;

    case 'X':
      flags |= LARGE;
    case 'x':
      base = 16;
      break;

    case 'd':
    case 'i':
      flags |= SIGN;
    case 'u':
      break;

    default:
      if (str <= end)
	*str = '%';
      ++str;
      if (*fmt) {
	if (str <= end)
	  *str = *fmt;
	++str;
      } else {
	--fmt;
      }
      continue;
    }
    if (qualifier == 'L')
      num = va_arg(args, long long);
    else if (qualifier == 'l') {
      num = va_arg(args, unsigned long);
      if (flags & SIGN)
	num = (signed long) num;
    } else if (qualifier == 'Z' || qualifier == 'z') {
      num = va_arg(args, size_t);
    } else if (qualifier == 'h') {
      num = (unsigned short) va_arg(args, int);
      if (flags & SIGN)
	num = (signed short) num;
    } else {
      num = va_arg(args, unsigned int);
      if (flags & SIGN)
	num = (signed int) num;
    }
    str = number(str, end, num, base,
		 field_width, precision, flags);
  }
  if (str <= end)
    *str = '\0';
  else if (size > 0)
    /* don't write out a null byte if the buf size is zero */
    *end = '\0';
  /* the trailing null byte doesn't count towards the total
   * ++str;
   */
  return str-buf;
}

/* Must be a global rather than local to prevent any stack extension.  */
static char buf[512];

void debug_printf (const char *fmt, ...)
{
  va_list args;
  va_start (args, fmt);

  PTHREAD_UNSAFE
  buf[0] = '-';
  buf[1] = '-';
  buf[2] = ' ';
  _vsnprintf (buf + 3, sizeof (buf) - 3, fmt, args);
  va_end (args);

  __os_print (buf);
}
