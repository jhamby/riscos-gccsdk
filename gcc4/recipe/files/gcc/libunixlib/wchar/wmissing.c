#include <stdlib.h>
#include <wctype.h>
#include <wctype.h>
#include <stdio.h>

int iswupper (wint_t __wc)
{
  printf("%s: Not implemented\n", __func__);
  abort();
}

unsigned long int wcstoul (__const wchar_t *__restrict __nptr,
				  wchar_t **__restrict __endptr, int __base)
{
  printf("%s: Not implemented\n", __func__);
  abort();
}

int wcscoll (__const wchar_t *__s1, __const wchar_t *__s2)
{
  printf("%s: Not implemented\n", __func__);
  abort();
}

int iswlower (wint_t __wc)
{
  printf("%s: Not implemented\n", __func__);
  abort();
}

long long int wcstoll (__const wchar_t *__restrict __nptr,
			      wchar_t **__restrict __endptr, int __base)
{
  printf("%s: Not implemented\n", __func__);
  abort();
}

int swprintf (wchar_t *__restrict __s, size_t __n,
		     __const wchar_t *__restrict __format, ...)
{
  printf("%s: Not implemented\n", __func__);
  abort();
}

long double wcstold (__const wchar_t *__restrict __nptr,
			    wchar_t **__restrict __endptr)
{
  printf("%s: Not implemented\n", __func__);
  abort();
}

long int wcstol (__const wchar_t *__restrict __nptr,
			wchar_t **__restrict __endptr, int __base)
{
  printf("%s: Not implemented\n", __func__);
  abort();
}

size_t wcsxfrm (wchar_t *__restrict __s1,
		       __const wchar_t *__restrict __s2, size_t __n)
{
  printf("%s: Not implemented\n", __func__);
  abort();
}

float wcstof (__const wchar_t *__restrict __nptr,
		     wchar_t **__restrict __endptr)
{
  printf("%s: Not implemented\n", __func__);
  abort();
}

double wcstod (__const wchar_t *__restrict __nptr,
		      wchar_t **__restrict __endptr)
{
  printf("%s: Not implemented\n", __func__);
  abort();
}

int iswblank (wint_t __wc)
{
  printf("%s: Not implemented\n", __func__);
  abort();
}

int iswctype (wint_t __wc, wctype_t __desc)
{
  printf("%s: Not implemented\n", __func__);
  abort();
}

unsigned long long int wcstoull (__const wchar_t *__restrict __nptr,
					wchar_t **__restrict __endptr,
					int __base)
{
  printf("%s: Not implemented\n", __func__);
  abort();
}

size_t wcsftime(wchar_t *restrict wcs, size_t maxsize,
       const wchar_t *restrict format, const struct tm *restrict timeptr)
{
  printf("%s: Not implemented\n", __func__);
  abort();
}

wint_t putwc(wchar_t wc, FILE *stream)
{
  printf("%s: Not implemented\n", __func__);
  abort();
}

wctype_t  wctype(const char *name)
{
  printf("%s: Not implemented\n", __func__);
  abort();
}

wint_t ungetwc(wint_t wc, FILE *stream)
{
  printf("%s: Not implemented\n", __func__);
  abort();
}

wint_t getwc(FILE *stream)
{
  printf("%s: Not implemented\n", __func__);
  abort();
}
