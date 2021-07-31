--- lib/dpkg/progname.c.orig	2011-12-28 12:38:07.000000000 +0000
+++ lib/dpkg/progname.c	2011-12-28 12:39:12.000000000 +0000
@@ -43,7 +43,7 @@
 	progname = path_basename(name);
 }
 
-#if defined(HAVE___PROGNAME)
+#if defined(HAVE___PROGNAME) && !defined(__riscos__)
 extern const char *__progname;
 #endif
 
@@ -63,7 +63,7 @@
 	if (progname == NULL) {
 #if defined(HAVE_PROGRAM_INVOCATION_SHORT_NAME)
 		progname = program_invocation_short_name;
-#elif defined(HAVE___PROGNAME)
+#elif defined(HAVE___PROGNAME) && !defined(__riscos__)
 		progname = __progname;
 #elif defined(HAVE_GETPROGNAME)
 		progname = getprogname();
