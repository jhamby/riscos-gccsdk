--- rott/dosutil.c.orig	2021-03-06 09:31:14.555764120 +0000
+++ rott/dosutil.c	2021-03-06 09:31:25.955630548 +0000
@@ -197,7 +197,7 @@
 	printf ("\033[m");
 }
 
-#if !defined(__CYGWIN__) && !defined(__MINGW32__)
+#if !defined(__CYGWIN__) && !defined(__MINGW32__) && !defined(__riscos__)
 #include <execinfo.h>
 
 void print_stack (int level)
