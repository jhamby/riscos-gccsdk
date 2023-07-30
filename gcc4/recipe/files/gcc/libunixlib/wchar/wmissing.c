#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <wctype.h>

int iswupper (wint_t __wc)
{
  printf("%s: Not implemented\n", __func__);
  abort();
}

int
iswupper_l (wint_t wc, locale_t locale)
{
  printf("%s: Not implemented\n", __func__);
  abort();
}

int iswlower (wint_t __wc)
{
  printf("%s: Not implemented\n", __func__);
  abort();
}

int
iswlower_l (wint_t wc, locale_t locale)
{
  printf("%s: Not implemented\n", __func__);
  abort();
}

int wcscoll (__const wchar_t *__s1, __const wchar_t *__s2)
{
  printf("%s: Not implemented\n", __func__);
  abort();
}

int wcscoll_l (__const wchar_t *__s1, __const wchar_t *__s2,
		locale_t loc)
{
  printf("%s: Not implemented\n", __func__);
  abort();
}

int wcscasecmp (__const wchar_t *__s1, __const wchar_t *__s2)
{
  printf("%s: Not implemented\n", __func__);
  abort();
}

int wcsncasecmp (__const wchar_t *__s1, __const wchar_t *__s2,
		size_t __n)
{
  printf("%s: Not implemented\n", __func__);
  abort();
}

int wcscasecmp_l (__const wchar_t *__s1, __const wchar_t *__s2,
		 locale_t __loc)
{
  printf("%s: Not implemented\n", __func__);
  abort();
}

int wcsncasecmp_l (__const wchar_t *__s1, __const wchar_t *__s2,
		size_t __n, locale_t __loc)
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

size_t wcsxfrm_l (wchar_t *__s1, __const wchar_t *__s2,
		size_t __n, locale_t __loc)
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

int
iswblank_l (wint_t __wc, locale_t __locale)
{
  printf("%s: Not implemented\n", __func__);
  abort();
}

int iswctype (wint_t __wc, wctype_t __desc)
{
  printf("%s: Not implemented\n", __func__);
  abort();
}

int iswctype_l (wint_t __wc, wctype_t __desc, locale_t __locale)
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

wctype_t wctype_l (__const char *__property, locale_t __locale)
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
