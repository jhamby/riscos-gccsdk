/*
** A simple and fast generic string hasher based on Peter K. Pearson's
** article in CACM 33-6, pp. 677.
*/

#ifndef hash_header_included
#define hash_header_included

int hashstr (const char *s, int maxn, int hashs);

#endif
