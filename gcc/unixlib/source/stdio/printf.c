/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/printf.c,v $
 * $Date: 2003/04/13 16:21:02 $
 * $Revision: 1.7 $
 * $State: Exp $
 * $Author: alex $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: printf.c,v 1.7 2003/04/13 16:21:02 alex Exp $";
#endif

/*-
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#if defined(LIBC_SCCS) && !defined(lint)
#if 0
static char sccsid[] = "@(#)vfprintf.c	8.1 (Berkeley) 6/4/93";
#endif
static const char rcsid[] =
		"$Id: printf.c,v 1.7 2003/04/13 16:21:02 alex Exp $";
#endif /* LIBC_SCCS and not lint */

/*
 * Actual printf innards.
 *
 * This code is large and complicated...
 */

#include <sys/types.h>

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdarg.h>
#include <wchar.h>
#include <pthread.h>


/* Define FLOATING_POINT to get floating point. */
#define	FLOATING_POINT

static void __find_arguments (const char *, va_list, void ***);
static int __grow_type_table (int, unsigned char **, int *);
static int vfprintfsub(FILE *fp, size_t max, const char *fmt0, va_list ap);


static size_t out_char(int c, FILE *fp, size_t limit)
{
    /* write a char to the output stream iff enough space */
    if (limit > 0)
    {
        limit--;
        fputc(c, fp);
    }

    return limit;
}


static size_t out_write(const void *ptr, size_t size, size_t nobj, FILE *fp, size_t limit)
{
    /* write a set of items iff enough space */
    if (limit >= size * nobj)
    {
        fwrite(ptr, size, nobj, fp);
        return limit - (size * nobj);
    }

    /* write out as many as we can and stop all further output */
    fwrite(ptr, size, (limit / size), fp);
    return 0;
}



/*
 * Macros for converting digits to letters and vice versa
 */
#define	to_digit(c)	((c) - '0')
#define is_digit(c)	((unsigned)to_digit(c) <= 9)
#define	to_char(n)	((n) + '0')

/*
 * Convert an unsigned long to ASCII for printf purposes, returning
 * a pointer to the first character of the string representation.
 * Octal numbers can be forced to have a leading zero; hex numbers
 * use the given digits.
 */
static char *
__ultoa(u_long val, char *endp, int base, int octzero, char *xdigs)
{
	register char *cp = endp;
	register long sval;

	/*
	 * Handle the three cases separately, in the hope of getting
	 * better/faster code.
	 */
	switch (base) {
	case 10:
		if (val < 10) {	/* many numbers are 1 digit */
			*--cp = to_char(val);
			return (cp);
		}
		/*
		 * On many machines, unsigned arithmetic is harder than
		 * signed arithmetic, so we do at most one unsigned mod and
		 * divide; this is sufficient to reduce the range of
		 * the incoming value to where signed arithmetic works.
		 */
		if (val > LONG_MAX) {
			*--cp = to_char(val % 10);
			sval = val / 10;
		} else
			sval = val;
		do {
			*--cp = to_char(sval % 10);
			sval /= 10;
		} while (sval != 0);
		break;

	case 8:
		do {
			*--cp = to_char(val & 7);
			val >>= 3;
		} while (val);
		if (octzero && *cp != '0')
			*--cp = '0';
		break;

	case 16:
		do {
			*--cp = xdigs[val & 15];
			val >>= 4;
		} while (val);
		break;

	default:			/* oops */
		abort();
	}
	return (cp);
}

#ifdef __GNUC__
/* Identical to __ultoa, but for quads. */
static char *
__uqtoa(u_quad_t val, char *endp, int base, int octzero, char *xdigs)
{
	register char *cp = endp;
	register quad_t sval;

	/* quick test for small values; __ultoa is typically much faster */
	/* (perhaps instead we should run until small, then call __ultoa?) */
	if (val <= ULONG_MAX)
		return (__ultoa((u_long)val, endp, base, octzero, xdigs));
	switch (base) {
	case 10:
		if (val < 10) {
			*--cp = to_char(val % 10);
			return (cp);
		}
		if (val > QUAD_MAX) {
			*--cp = to_char(val % 10);
			sval = val / 10;
		} else
			sval = val;
		do {
			*--cp = to_char(sval % 10);
			sval /= 10;
		} while (sval != 0);
		break;

	case 8:
		do {
			*--cp = to_char(val & 7);
			val >>= 3;
		} while (val);
		if (octzero && *cp != '0')
			*--cp = '0';
		break;

	case 16:
		do {
			*--cp = xdigs[val & 15];
			val >>= 4;
		} while (val);
		break;

	default:
		abort();
	}
	return (cp);
}
#endif

#ifdef FLOATING_POINT
#include <math.h>
#include <float.h>

#define MAXFRACT DBL_DIG
#define MAXEXP DBL_MAX_10_EXP
#define	BUF		(MAXEXP+MAXFRACT+1)	/* + decimal point */
#define	DEFPREC		6

static char *cvt (double, int, int, char *, int *, int, int *);
static int exponent (char *, int, int);

#else /* no FLOATING_POINT */

#define	BUF		68

#endif /* FLOATING_POINT */

#define STATIC_ARG_TBL_SIZE 8		/* Size of static argument table. */

/*
 * Flags used during conversion.
 */
#define	ALT		0x001		/* alternate form */
#define	HEXPREFIX	0x002		/* add 0x or 0X prefix */
#define	LADJUST		0x004		/* left adjustment */
#define	LONGDBL		0x008		/* long double; unimplemented */
#define	LONGINT		0x010		/* long integer */
#define	QUADINT		0x020		/* quad integer */
#define	SHORTINT	0x040		/* short integer */
#define	ZEROPAD		0x080		/* zero (as opposed to blank) pad */
#define FPT		0x100		/* Floating point number */
#define WIDESTRING	0x200		/* Wide string */
#define WCHAR		0x400		/* Wide character */
#define STOP            0x800           /* reached buffer limit inside a multibyte character */

/*
 * Choose PADSIZE to trade efficiency vs. size.  If larger printf
 * fields occur frequently, increase PADSIZE and make the initialisers
 * below longer.
 */
#define PADSIZE 16
static char const blanks[PADSIZE] =
{' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
static char const zeroes[PADSIZE] =
{'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};

static size_t
pad_file (FILE * fp, char pad, int count, size_t limit)
{
  char padbuf[PADSIZE];
  const char *padptr;
  int i;
/*   size_t w; */

  if (pad == ' ')
    padptr = blanks;
  else if (pad == '0')
    padptr = zeroes;
  else
    {
      for (i = PADSIZE; --i >= 0; ) padbuf[i] = pad;
      padptr = padbuf;
    }
  for (i = count; i >= PADSIZE; i -= PADSIZE)
    limit = out_write(padptr, sizeof (char), PADSIZE, fp, limit);

  if (i > 0)
    limit = out_write(padptr, sizeof (char), i, fp, limit);

  return limit;
}

static int
vfprintfsub(FILE *fp, size_t max, const char *fmt0, va_list ap)
{
	register char *fmt;	/* format string */
	register int ch;	/* character from fmt */
	register int n, n2;	/* handy integer (short term usage) */
	register char *cp;	/* handy char pointer (short term usage) */
	register int flags;	/* flags as above */
	int ret;		/* return value accumulator */
	int width;		/* width from format (%8d), or 0 */
	int prec;		/* precision from format (%.3d), or -1 */
	char sign;		/* sign prefix (' ', '+', '-', or \0) */
#ifdef FLOATING_POINT
	char softsign;		/* temporary negative sign for floats */
	double _double;		/* double precision arguments %[eEfgG] */
	int expt;		/* integer value of exponent */
	int expsize;		/* character count for expstr */
	int ndig;		/* actual number of digits returned by cvt */
	char expstr[7];		/* buffer for exponent string */
#endif
	u_long	ulval;		/* integer arguments %[diouxX] */
	u_quad_t uqval;		/* %q integers */
	wchar_t *wcharval;	/* wide string argument */
	wint_t wintval;		/* wide character argument */
	int base;		/* base for [diouxX] conversion */
	int dprec;		/* a copy of prec if [diouxX], 0 otherwise */
	int realsz;		/* field size expanded by dprec, sign, etc */
	int size;		/* size of converted field or string */
	char *xdigs;		/* digits for [xX] conversion */
	char buf[BUF];		/* space for %c, %[diouxX], %[eEfgG] */
	void **argtable;	/* args, built due to positional arg */
	void *statargtable [STATIC_ARG_TBL_SIZE];
	int nextarg;		/* 1-based argument index */
	va_list orgap;		/* original argument pointer */


	/*
	 * Get the argument indexed by nextarg.	  If the argument table is
	 * built, use it to get the argument.  If its not, get the next
	 * argument (and arguments must be gotten sequentially).
	 */
#define GETARG(type) \
	((argtable != NULL) ? *((type*)(argtable[nextarg++])) : \
	    (nextarg++, va_arg(ap, type)))

	/*
	 * To extend shorts properly, we need both signed and unsigned
	 * argument extraction methods.
	 */
#define	SARG() \
	(flags&LONGINT ? GETARG(long) : \
	    flags&SHORTINT ? (long)(short)GETARG(int) : \
	    (long)GETARG(int))
#define	UARG() \
	(flags&LONGINT ? GETARG(u_long) : \
	    flags&SHORTINT ? (u_long)(u_short)GETARG(int) : \
	    (u_long)GETARG(u_int))

	/*
	 * Get * arguments, including the form *nn$.  Preserve the nextarg
	 * that the argument can be gotten once the type is determined.
	 */
#define GETASTER(val) \
	n2 = 0; \
	cp = fmt; \
	while (is_digit(*cp)) { \
		n2 = 10 * n2 + to_digit(*cp); \
		cp++; \
	} \
	if (*cp == '$') { \
		int hold = nextarg; \
		if (argtable == NULL) { \
			argtable = statargtable; \
			__find_arguments (fmt0, orgap, &argtable); \
		} \
		nextarg = n2; \
		val = GETARG (int); \
		nextarg = hold; \
		fmt = ++cp; \
	} else { \
		val = GETARG (int); \
	}


	PTHREAD_UNSAFE

	/* sorry, fprintf(read_only_file, "") returns EOF, not 0 */
	if (!fp->__mode.__write) {
		return (EOF);
	}

	fmt = (char *)fmt0;
	argtable = NULL;
	nextarg = 1;
	orgap = ap;
	ret = 0;

	/*
	 * Scan the format for conversions (`%' character).
	 */
	for (;;) {
		for (cp = fmt; (ch = *fmt) != '\0' && ch != '%'; fmt++)
			/* void */;
		if ((n = fmt - cp) != 0) {
			max = out_write(cp, sizeof (char), n, fp, max);
			ret += n;
		}
		if (ch == '\0')
			goto done;
		fmt++;		/* skip over '%' */

		flags = 0;
		dprec = 0;
		width = 0;
		prec = -1;
		sign = '\0';

rflag:		ch = *fmt++;
reswitch:	switch (ch) {
		case ' ':
			/*
			 * ``If the space and + flags both appear, the space
			 * flag will be ignored.''
			 *	-- ANSI X3J11
			 */
			if (!sign)
				sign = ' ';
			goto rflag;
		case '#':
			flags |= ALT;
			goto rflag;
		case '*':
			/*
			 * ``A negative field width argument is taken as a
			 * - flag followed by a positive field width.''
			 *	-- ANSI X3J11
			 * They don't exclude field widths read from args.
			 */
			GETASTER (width);
			if (width >= 0)
				goto rflag;
			width = -width;
			/* FALLTHROUGH */
		case '-':
			flags |= LADJUST;
			goto rflag;
		case '+':
			sign = '+';
			goto rflag;
		case '.':
			if ((ch = *fmt++) == '*') {
				GETASTER (n);
				prec = n < 0 ? -1 : n;
				goto rflag;
			}
			n = 0;
			while (is_digit(ch)) {
				n = 10 * n + to_digit(ch);
				ch = *fmt++;
			}
			prec = n < 0 ? -1 : n;
			goto reswitch;
		case '0':
			/*
			 * ``Note that 0 is taken as a flag, not as the
			 * beginning of a field width.''
			 *	-- ANSI X3J11
			 */
			flags |= ZEROPAD;
			goto rflag;
		case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			n = 0;
			do {
				n = 10 * n + to_digit(ch);
				ch = *fmt++;
			} while (is_digit(ch));
			if (ch == '$') {
				nextarg = n;
				if (argtable == NULL) {
					argtable = statargtable;
					__find_arguments (fmt0, orgap,
						&argtable);
				}
				goto rflag;
			}
			width = n;
			goto reswitch;
#ifdef FLOATING_POINT
		case 'L':
			flags |= LONGDBL;
			goto rflag;
#endif
		case 'h':
			flags |= SHORTINT;
			goto rflag;
		case 'l':
			flags |= LONGINT;
			goto rflag;
		case 'q':
			flags |= QUADINT;
			goto rflag;
		case 'c':
			if (flags & LONGINT)
			        goto longchar;
			else {
			        *(cp = buf) = GETARG(int);
				size = 1;
				sign = '\0';
				break;
			}
		case 'C':
longchar:		flags = (flags & ~LONGINT) | WCHAR;
			{
       				char temp[MB_CUR_MAX];
				mbstate_t ps = {0,0};

				wintval = GETARG(wint_t);
				size    = wcrtomb(temp, wintval, &ps);
				if (size == -1 || size > max) {
				        /* invalid character - write nothing? */
				        size = 0;
				        flags |= STOP;
				}
			}
		case 'D':
			flags |= LONGINT;
			/*FALLTHROUGH*/
		case 'd':
		case 'i':
#ifdef __GNUC__
			if (flags & QUADINT) {
				uqval = GETARG(quad_t);
				if ((quad_t)uqval < 0) {
					uqval = -uqval;
					sign = '-';
				}
			} else {
#endif
				ulval = SARG();
				if ((long)ulval < 0) {
					ulval = -ulval;
					sign = '-';
				}
#ifdef __GNUC__
			}
#endif
			base = 10;
			goto number;
#ifdef FLOATING_POINT
		case 'e':
		case 'E':
		case 'f':
			goto fp_begin;
		case 'g':
		case 'G':
			if (prec == 0)
				prec = 1;
fp_begin:		if (prec == -1)
				prec = DEFPREC;
			if (flags & LONGDBL)
				_double = (double)GETARG(long double);
			else
				_double = GETARG(double);
			/* do this before tricky precision changes */
			if (isinf(_double)) {
				if (_double < 0)
					sign = '-';
				cp = "Inf";
				size = 3;
				break;
			}
			if (isnan(_double)) {
				cp = "NaN";
				size = 3;
				break;
			}
			flags |= FPT;
			cp = cvt(_double, prec, flags, &softsign,
				&expt, ch, &ndig);
			if (ch == 'g' || ch == 'G') {
				if (expt <= -4 || expt > prec)
					ch = (ch == 'g') ? 'e' : 'E';
				else
					ch = 'g';
			}
			if (ch <= 'e') {	/* 'e' or 'E' fmt */
				--expt;
				expsize = exponent(expstr, expt, ch);
				size = expsize + ndig;
				if (ndig > 1 || flags & ALT)
					++size;
			} else if (ch == 'f') {		/* f fmt */
				if (expt > 0) {
					size = expt;
					if (prec || flags & ALT)
						size += prec + 1;
				} else	/* "0.X" */
					size = prec + 2;
			} else if (expt >= ndig) {	/* fixed g fmt */
				size = expt;
				if (flags & ALT)
					++size;
			} else
				size = ndig + (expt > 0 ?
					1 : 2 - expt);

			if (softsign)
				sign = '-';
			break;
#endif /* FLOATING_POINT */
		case 'n':
#ifdef __GNUC__
			if (flags & QUADINT)
				*GETARG(quad_t *) = ret;
			else
#endif
			 if (flags & LONGINT)
				*GETARG(long *) = ret;
			else if (flags & SHORTINT)
				*GETARG(short *) = ret;
			else
				*GETARG(int *) = ret;
			continue;	/* no output */
		case 'O':
			flags |= LONGINT;
			/*FALLTHROUGH*/
		case 'o':
#ifdef __GNUC__
			if (flags & QUADINT)
				uqval = GETARG(u_quad_t);
			else
#endif
				ulval = UARG();
			base = 8;
			goto nosign;
		case 'p':
			/*
			 * ``The argument shall be a pointer to void.  The
			 * value of the pointer is converted to a sequence
			 * of printable characters, in an implementation-
			 * defined manner.''
			 *	-- ANSI X3J11
			 */
			ulval = (u_long)GETARG(void *);
			base = 16;
			xdigs = "0123456789abcdef";
			flags = (flags & ~QUADINT) | HEXPREFIX;
			ch = 'x';
			goto nosign;
		case 's':
			if (flags & LONGINT)
			    goto longstr;
			else {
				if ((cp = GETARG(char *)) == NULL)
					cp = "(null)";
				if (prec >= 0) {
					/*
					* can't use strlen; can only look for the
					* NUL in the first `prec' characters, and
					* strlen() will go further.
					*/
					char *p = memchr(cp, 0, (size_t)prec);

					if (p != NULL) {
						size = p - cp;
						if (size > prec)
							size = prec;
					} else
						size = prec;
				} else
					size = strlen(cp);
			}
			sign = '\0';
			break;
longstr:	case 'S':
                        {
				/*
				* long string. We need to know the length of the /output/
				* in bytes so we can pad on the left. This means converting
				* each character here and again when we actually print it :(
				*/
				char temp[MB_CUR_MAX];
				mbstate_t ps = {0,0};
				size_t n;
				wchar_t *p;
				unsigned int limit;

				flags = (flags & ~LONGINT) | WIDESTRING;
				if ((wcharval = GETARG(wchar_t *)) == NULL)
					wcharval = L"(null)";

                                limit = ((unsigned int)prec < max) ? prec : max;
				p     = wcharval;
				size  = 0;

				while ((1)) {
					if (*p == L'\0')
						break;
					n = wcrtomb(temp, *p++, &ps);
					if (n < 0 || size + n > limit)
						break;
					size += n;
				}
				if (size + n > limit)
				        flags |= STOP; /* no more room */
				/* now size contains the number of bytes we'll output. */
                        }
                        break;
		case 'U':
			flags |= LONGINT;
			/*FALLTHROUGH*/
		case 'u':
#ifdef __GNUC__
			if (flags & QUADINT)
				uqval = GETARG(u_quad_t);
			else
#endif
				ulval = UARG();
			base = 10;
			goto nosign;
		case 'X':
			xdigs = "0123456789ABCDEF";
			goto hex;
		case 'x':
			xdigs = "0123456789abcdef";
hex:
#ifdef __GNUC__
			if (flags & QUADINT)
				uqval = GETARG(u_quad_t);
			else
#endif
				ulval = UARG();
			base = 16;
			/* leading 0x/X only if non-zero */
#ifdef __GNUC__
			if (flags & ALT &&
			    (flags & QUADINT ? uqval != 0 : ulval != 0))
				flags |= HEXPREFIX;
#else
			if (flags & ALT && ulval != 0)
				flags |= HEXPREFIX;
#endif

			/* unsigned conversions */
nosign:			sign = '\0';
			/*
			 * ``... diouXx conversions ... if a precision is
			 * specified, the 0 flag will be ignored.''
			 *	-- ANSI X3J11
			 */
number:			if ((dprec = prec) >= 0)
				flags &= ~ZEROPAD;

			/*
			 * ``The result of converting a zero value with an
			 * explicit precision of zero is no characters.''
			 *	-- ANSI X3J11
			 */
			cp = buf + BUF;
#ifdef __GNUC__
			if (flags & QUADINT) {
				if (uqval != 0 || prec != 0)
					cp = __uqtoa(uqval, cp, base,
					    flags & ALT, xdigs);
			} else {
#endif
				if (ulval != 0 || prec != 0)
					cp = __ultoa(ulval, cp, base,
					    flags & ALT, xdigs);
#ifdef __GNUC__
			}
#endif
			size = buf + BUF - cp;
			break;
		default:	/* "%?" prints ?, unless ? is NUL */
			if (ch == '\0')
				goto done;
			/* pretend it was %c with argument ch */
			cp = buf;
			*cp = ch;
			size = 1;
			sign = '\0';
			break;
		}

		/*
		 * All reasonable formats wind up here.  At this point, `cp'
		 * points to a string which (if not flags&LADJUST) should be
		 * padded out to `width' places.  If flags&ZEROPAD, it should
		 * first be prefixed by any sign or other prefix; otherwise,
		 * it should be blank padded before the prefix is emitted.
		 * After any left-hand padding and prefixing, emit zeroes
		 * required by a decimal [diouxX] precision, then print the
		 * string proper, then emit zeroes required by any leftover
		 * floating precision; finally, if LADJUST, pad with blanks.
		 *
		 * Compute actual size, so we know how much to pad.
		 * size excludes decimal prec; realsz includes it.
		 */
		realsz = dprec > size ? dprec : size;
		if (sign)
			realsz++;
		else if (flags & HEXPREFIX)
			realsz += 2;

		/* right-adjusting blank padding */
		if ((flags & (LADJUST|ZEROPAD)) == 0)
			max = pad_file (fp, ' ', width - realsz, max);

		/* prefix */
		if (sign) {
			max = out_char (sign, fp, max);
		} else if (flags & HEXPREFIX) {
			max = out_char ('0', fp, max);
			max = out_char (ch, fp, max);
		}

		/* right-adjusting zero padding */
		if ((flags & (LADJUST|ZEROPAD)) == ZEROPAD)
			max = pad_file (fp, '0', width - realsz, max);

		/* leading zeroes from decimal precision */
		max = pad_file (fp, '0', dprec - size, max);

		/* the string or number proper */
#ifdef FLOATING_POINT
		if (flags & FPT) {
		/* glue together f_p fragments */
			if (ch >= 'f') {	/* 'f' or 'g' */
				if (_double == 0) {
					/* kludge for __dtoa irregularity */
					if (expt >= ndig &&
					    (flags & ALT) == 0) {
						max = out_char ('0', fp, max);
					} else {
						max = out_char ('0', fp, max);
						max = out_char ('.', fp, max);
						max = pad_file (fp, '0', ndig - 1, max);
					}
				} else if (expt <= 0) {
					max = out_char ('0', fp, max);
					max = out_char ('.', fp, max);
					max = pad_file (fp, '0', -expt, max);
					max = out_write(cp, sizeof (char), ndig, fp, max);
				} else if (expt >= ndig) {
					max = out_write(cp, sizeof (char), ndig, fp, max);
					max = pad_file (fp, '0', expt - ndig, max);
					if (flags & ALT)
					  max = out_char ('.', fp, max);
				} else {
					max = out_write(cp, sizeof (char), expt, fp, max);
					cp += expt;
					max = out_char ('.', fp, max);
					max = out_write(cp, sizeof (char), ndig-expt, fp, max);
				}
			} else {	/* 'e' or 'E' */
				if (ndig > 1 || flags & ALT) {
					max = out_char (*cp++, fp, max);
					max = out_char ('.', fp, max);
					if (_double) {
						max = out_write(cp, sizeof (char), ndig-1, fp, max);
					} else	/* 0.[0..] */
						/* __dtoa irregularity */
						max = pad_file (fp, '0', ndig - 1, max);
				} else	/* XeYYY */
					max = out_char (*cp, fp, max);
				max = out_write(expstr, sizeof (char), expsize, fp, max);
			}
		} else {
#endif
			if (flags & WIDESTRING) {
				char temp[MB_CUR_MAX];
				mbstate_t ps = {0,0};
				size_t n;

				while (size > 0) {
					n = wcrtomb(temp, *wcharval++, &ps);
					if (n == -1)
						break;
					max = out_write(temp, sizeof (char), n, fp, max);
					size -= n;
				}
			} else if (flags & WCHAR) {
				char temp[MB_CUR_MAX];
				mbstate_t ps = {0,0};
				size_t n;

				n = wcrtomb(temp, wintval, &ps);
				if (n != -1)
					max = out_write(temp, sizeof (char), n, fp, max);
			} else
				max = out_write(cp, sizeof (char), size, fp, max);
#ifdef FLOATING_POINT
		}
#endif
		/* left-adjusting padding (always blank) */
		if (flags & LADJUST)
			max = pad_file (fp, ' ', width - realsz, max);

		/* finally, adjust ret */
		ret += width > realsz ? width : realsz;
	}
done:
	if (ferror(fp))
		ret = EOF;

	if ((argtable != NULL) && (argtable != statargtable))
		free (argtable);
	return (ret);
	/* NOTREACHED */
}

/*
 * Type ids for argument type table.
 */
#define T_UNUSED	0
#define T_SHORT		1
#define T_U_SHORT	2
#define TP_SHORT	3
#define T_INT		4
#define T_U_INT		5
#define TP_INT		6
#define T_LONG		7
#define T_U_LONG	8
#define TP_LONG		9
#define T_QUAD		10
#define T_U_QUAD	11
#define TP_QUAD		12
#define T_DOUBLE	13
#define T_LONG_DOUBLE	14
#define TP_CHAR		15
#define TP_VOID		16
#define TP_WCHAR	17
#define T_WCHAR		18

/*
 * Find all arguments when a positional parameter is encountered.  Returns a
 * table, indexed by argument number, of pointers to each arguments.  The
 * initial argument table should be an array of STATIC_ARG_TBL_SIZE entries.
 * It will be replaces with a malloc-ed on if it overflows.
 */
static void
__find_arguments (const char *fmt0, va_list ap, void ***argtable)
{
	register char *fmt;	/* format string */
	register int ch;	/* character from fmt */
	register int n, n2;	/* handy integer (short term usage) */
	register char *cp;	/* handy char pointer (short term usage) */
	register int flags;	/* flags as above */
	int width;		/* width from format (%8d), or 0 */
	unsigned char *typetable; /* table of types */
	unsigned char stattypetable [STATIC_ARG_TBL_SIZE];
	int tablesize;		/* current size of type table */
	int tablemax;		/* largest used index in table */
	int nextarg;		/* 1-based argument index */

	/*
	 * Add an argument type to the table, expanding if necessary.
	 */
#define ADDTYPE(type) \
	((nextarg >= tablesize) ? \
		__grow_type_table(nextarg, &typetable, &tablesize) : 0, \
	typetable[nextarg++] = type, \
	(nextarg > tablemax) ? tablemax = nextarg : 0)

#define	ADDSARG() \
	((flags&LONGINT) ? ADDTYPE(T_LONG) : \
		((flags&SHORTINT) ? ADDTYPE(T_SHORT) : ADDTYPE(T_INT)))

#define	ADDUARG() \
	((flags&LONGINT) ? ADDTYPE(T_U_LONG) : \
		((flags&SHORTINT) ? ADDTYPE(T_U_SHORT) : ADDTYPE(T_U_INT)))

	/*
	 * Add * arguments to the type array.
	 */
#define ADDASTER() \
	n2 = 0; \
	cp = fmt; \
	while (is_digit(*cp)) { \
		n2 = 10 * n2 + to_digit(*cp); \
		cp++; \
	} \
	if (*cp == '$') { \
		int hold = nextarg; \
		nextarg = n2; \
		ADDTYPE (T_INT); \
		nextarg = hold; \
		fmt = ++cp; \
	} else { \
		ADDTYPE (T_INT); \
	}
	fmt = (char *)fmt0;
	typetable = stattypetable;
	tablesize = STATIC_ARG_TBL_SIZE;
	tablemax = 0;
	nextarg = 1;
	memset (typetable, T_UNUSED, STATIC_ARG_TBL_SIZE);

	/*
	 * Scan the format for conversions (`%' character).
	 */
	for (;;) {
		for (cp = fmt; (ch = *fmt) != '\0' && ch != '%'; fmt++)
			/* void */;
		if (ch == '\0')
			goto done;
		fmt++;		/* skip over '%' */

		flags = 0;
		width = 0;

rflag:		ch = *fmt++;
reswitch:	switch (ch) {
		case ' ':
		case '#':
			goto rflag;
		case '*':
			ADDASTER ();
			goto rflag;
		case '-':
		case '+':
			goto rflag;
		case '.':
			if ((ch = *fmt++) == '*') {
				ADDASTER ();
				goto rflag;
			}
			while (is_digit(ch)) {
				ch = *fmt++;
			}
			goto reswitch;
		case '0':
			goto rflag;
		case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			n = 0;
			do {
				n = 10 * n + to_digit(ch);
				ch = *fmt++;
			} while (is_digit(ch));
			if (ch == '$') {
				nextarg = n;
				goto rflag;
			}
			width = n;
			goto reswitch;
#ifdef FLOATING_POINT
		case 'L':
			flags |= LONGDBL;
			goto rflag;
#endif
		case 'h':
			flags |= SHORTINT;
			goto rflag;
		case 'l':
			flags |= LONGINT;
			goto rflag;
		case 'q':
			flags |= QUADINT;
			goto rflag;
		case 'c':
			if (flags & LONGINT)
				ADDTYPE(T_WCHAR);
			else
		                ADDTYPE(T_INT);
			break;
		case 'C':
		        ADDTYPE(T_WCHAR);
		        break;
		case 'D':
			flags |= LONGINT;
			/*FALLTHROUGH*/
		case 'd':
		case 'i':
#ifdef __GNUC__
			if (flags & QUADINT) {
				ADDTYPE(T_QUAD);
			} else {
#endif
				ADDSARG();
#ifdef __GNUC__
			}
#endif
			break;
#ifdef FLOATING_POINT
		case 'e':
		case 'E':
		case 'f':
		case 'g':
		case 'G':
			if (flags & LONGDBL)
				ADDTYPE(T_LONG_DOUBLE);
			else
				ADDTYPE(T_DOUBLE);
			break;
#endif /* FLOATING_POINT */
		case 'n':
#ifdef __GNUC__
			if (flags & QUADINT)
				ADDTYPE(TP_QUAD);
			else
#endif
			 if (flags & LONGINT)
				ADDTYPE(TP_LONG);
			else if (flags & SHORTINT)
				ADDTYPE(TP_SHORT);
			else
				ADDTYPE(TP_INT);
			continue;	/* no output */
		case 'O':
			flags |= LONGINT;
			/*FALLTHROUGH*/
		case 'o':
#ifdef __GNUC__
			if (flags & QUADINT)
				ADDTYPE(T_U_QUAD);
			else
#endif
				ADDUARG();
			break;
		case 'p':
			ADDTYPE(TP_VOID);
			break;
		case 'S':
		        ADDTYPE(TP_WCHAR);
		        break;
		case 's':
                        if (flags & LONGINT)
                                ADDTYPE(TP_WCHAR);
                        else
		        	ADDTYPE(TP_CHAR);
			break;
		case 'U':
			flags |= LONGINT;
			/*FALLTHROUGH*/
		case 'u':
#ifdef __GNUC__
			if (flags & QUADINT)
				ADDTYPE(T_U_QUAD);
			else
#endif
				ADDUARG();
			break;
		case 'X':
		case 'x':
#ifdef __GNUC__
			if (flags & QUADINT)
				ADDTYPE(T_U_QUAD);
			else
#endif
				ADDUARG();
			break;
		default:	/* "%?" prints ?, unless ? is NUL */
			if (ch == '\0')
				goto done;
			break;
		}
	}
done:
	/*
	 * Build the argument table.
	 */
	if (tablemax >= STATIC_ARG_TBL_SIZE) {
		*argtable = (void **)
		    malloc (sizeof (void *) * (tablemax + 1));
	}

	(*argtable) [0] = NULL;
	for (n = 1; n <= tablemax; n++) {
		(*argtable) [n] = ap;
		switch (typetable [n]) {
		    case T_UNUSED:
			(void) va_arg (ap, int);
			break;
		    case T_SHORT:
			(void) va_arg (ap, int);
			break;
		    case T_U_SHORT:
			(void) va_arg (ap, int);
			break;
		    case TP_SHORT:
			(void) va_arg (ap, short *);
			break;
		    case T_INT:
			(void) va_arg (ap, int);
			break;
		    case T_U_INT:
			(void) va_arg (ap, unsigned int);
			break;
		    case TP_INT:
			(void) va_arg (ap, int *);
			break;
		    case T_LONG:
			(void) va_arg (ap, long);
			break;
		    case T_U_LONG:
			(void) va_arg (ap, unsigned long);
			break;
		    case TP_LONG:
			(void) va_arg (ap, long *);
			break;
#ifdef __GNUC__
		    case T_QUAD:
			(void) va_arg (ap, quad_t);
			break;
		    case T_U_QUAD:
			(void) va_arg (ap, u_quad_t);
			break;
		    case TP_QUAD:
			(void) va_arg (ap, quad_t *);
			break;
#endif
		    case T_DOUBLE:
			(void) va_arg (ap, double);
			break;
		    case T_LONG_DOUBLE:
			(void) va_arg (ap, long double);
			break;
		    case TP_CHAR:
			(void) va_arg (ap, char *);
			break;
		    case TP_VOID:
			(void) va_arg (ap, void *);
			break;
		    case TP_WCHAR:
			(void) va_arg (ap, wchar_t *);
			break;
		    case T_WCHAR:
			(void) va_arg (ap, wint_t);
			break;
		}
	}

	if ((typetable != NULL) && (typetable != stattypetable))
		free (typetable);
}

/*
 * Increase the size of the type table.
 */
static int /* void */
__grow_type_table (int nextarg, unsigned char **typetable, int *tablesize)
{
	unsigned char *oldtable = *typetable;
	int newsize = *tablesize * 2;

	if (*tablesize == STATIC_ARG_TBL_SIZE) {
		*typetable = (unsigned char *)
		    malloc (sizeof (unsigned char) * newsize);
		bcopy (oldtable, *typetable, *tablesize);
	} else {
		*typetable = (unsigned char *)
		    realloc (typetable, sizeof (unsigned char) * newsize);

	}
	memset (&typetable [*tablesize], T_UNUSED, (newsize - *tablesize));

	*tablesize = newsize;
	return 0;
}


#ifdef FLOATING_POINT

extern char *__dtoa (double, int, int, int *, int *, char **);

static char *
cvt(double value, int ndigits, int flags, char *sign, int *decpt, int ch, int *length)
{
	int mode, dsgn;
	char *digits, *bp, *rve;

	if (ch == 'f')
		mode = 3;		/* ndigits after the decimal point */
	else {
		/*
		 * To obtain ndigits after the decimal point for the 'e'
		 * and 'E' formats, round to ndigits + 1 significant
		 * figures.
		 */
		if (ch == 'e' || ch == 'E')
			ndigits++;
		mode = 2;		/* ndigits significant digits */
	}
	if (value < 0) {
		value = -value;
		*sign = '-';
	} else
		*sign = '\000';
	digits = __dtoa(value, mode, ndigits, decpt, &dsgn, &rve);
	if ((ch != 'g' && ch != 'G') || flags & ALT) {
		/* print trailing zeros */
		bp = digits + ndigits;
		if (ch == 'f') {
			if (*digits == '0' && value)
				*decpt = -ndigits + 1;
			bp += *decpt;
		}
		if (value == 0)	/* kludge for __dtoa irregularity */
			rve = bp;
		while (rve < bp)
			*rve++ = '0';
	}
	*length = rve - digits;
	return (digits);
}

static int
exponent(char *p0, int exp, int fmtch)
{
	register char *p, *t;
	char expbuf[MAXEXP];

	p = p0;
	*p++ = fmtch;
	if (exp < 0) {
		exp = -exp;
		*p++ = '-';
	}
	else
		*p++ = '+';
	t = expbuf + MAXEXP;
	if (exp > 9) {
		do {
			*--t = to_char(exp % 10);
		} while ((exp /= 10) > 9);
		*--t = to_char(exp);
		for (; t < expbuf + MAXEXP; *p++ = *t++);
	}
	else {
		*p++ = '0';
		*p++ = to_char(exp);
	}
	return (p - p0);
}
#endif /* FLOATING_POINT */


int
vfprintf(FILE *fp, const char *fmt0, va_list ap)
{
  return vfprintfsub(fp, 2147483647, fmt0, ap);
}


int
vsnprintf (char *buf, size_t limit, const char *format, va_list ap)
{
  FILE f[1];
  int ret;
  int newlimit;

  f->o_ptr = f->o_base = (unsigned char *) buf;
  /* This is one big fucking buffer.  */
  f->o_cnt = f->__bufsize = 2147483647;
  f->__mode.__read = f->__mode.__append = 0;
  f->__mode.__write = 1;
  f->__linebuf = 0;
  f->__eof = f->__error = 0;
  f->__offset = 0;
  f->fd = -1;
  f->__magic = _IOMAGIC;

  if (limit > 0)
    newlimit = limit - 1;
  else
    newlimit = 0;

  ret = vfprintfsub(f, newlimit, format, ap);
  if (limit > 0)
    putc ('\0', f);

  return ret;
}


int
vsprintf(char *buf, const char *format, va_list ap)
{
  return vsnprintf(buf, 2147483647, format, ap);
}


int
__gcc_vsprintf(char *buf, const char *format, va_list ap)
{
  return vsnprintf(buf, 2147483647, format, ap);
}


int
snprintf(char *buf, size_t limit, const char *format, ...)
{
  va_list ap;
  register int r;

  va_start (ap, format);
  r = vsnprintf (buf, limit, format, ap);
  va_end (ap);

  return (r);
}


int
sprintf (char *buf, const char *format, ...)
{
  va_list ap;
  register int r;

  va_start (ap, format);
  r = vsprintf (buf, format, ap);
  va_end (ap);

  return (r);
}

int
vprintf (const char *format, va_list ap)
{
  return vfprintf (stdout, format, ap);
}

int
fprintf (FILE * stream, const char *format,...)
{
  va_list ap;
  int r;

  va_start (ap, format);
  r = vfprintf (stream, format, ap);
  va_end (ap);

  return (r);
}

int
printf (const char *format,...)
{
  va_list ap;
  register int r;

  va_start (ap, format);
  r = vfprintf (stdout, format, ap);
  va_end (ap);

  return (r);
}

