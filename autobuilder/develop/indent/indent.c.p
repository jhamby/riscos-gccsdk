--- src/indent.c.orig	2021-01-18 14:15:47.722645009 +0000
+++ src/indent.c	2021-01-18 14:16:07.507776401 +0000
@@ -1061,8 +1061,10 @@
 #if defined (HAVE_SETLOCALE)
     setlocale(LC_ALL, "");
 #endif
+#ifndef __riscos__
     bindtextdomain(PACKAGE, LOCALEDIR);
     textdomain(PACKAGE);
+#endif
 
 #if defined (_WIN32) && !defined (__CYGWIN__)
     /* wildcard expansion of commandline arguments, see wildexp.c */
