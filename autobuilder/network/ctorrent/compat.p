--- compat.c.orig	2008-01-09 20:05:08.000000000 -0800
+++ compat.c	2008-01-09 20:05:23.000000000 -0800
@@ -62,6 +62,7 @@
 
 
 #ifndef HAVE_STRNSTR
+#include <sys/types.h>
 #include <string.h>
 /* FUNCTION PROGRAMER: Siberiaic Sang */
 char *strnstr(const char *haystack, const char *needle, size_t haystacklen)
