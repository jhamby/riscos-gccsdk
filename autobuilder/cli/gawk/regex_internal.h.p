--- support/regex_internal.h.orig	2021-01-13 21:07:43.000998703 +0000
+++ support/regex_internal.h	2021-01-13 21:07:58.360695258 +0000
@@ -103,7 +103,7 @@
 # define SIZE_MAX ((size_t) -1)
 #endif
 
-#if ! defined(__DJGPP__) && (defined(GAWK) || _LIBC)
+#if ! defined(__DJGPP__) && (defined(GAWK) && !defined(__riscos__) || _LIBC)
 # define RE_ENABLE_I18N
 #endif
 
