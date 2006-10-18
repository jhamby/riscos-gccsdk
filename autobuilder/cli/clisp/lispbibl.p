--- src/lispbibl.d.orig	2006-10-18 22:04:44.000000000 +0100
+++ src/lispbibl.d	2006-10-18 22:03:44.000000000 +0100
@@ -97,7 +97,7 @@
 */
 
 /* this machine: WIN32 or GENERIC_UNIX */
-#if (defined(__unix) || defined(__unix__) || defined(_AIX) || defined(sinix) || defined(__MACH__) || defined(__POSIX__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__BEOS__)) && !defined(unix)
+#if (defined(__unix) || defined(__unix__) || defined(_AIX) || defined(sinix) || defined(__MACH__) || defined(__POSIX__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__BEOS__) || defined(__riscos__)) && !defined(unix)
   #define unix
 #endif
 #if (defined(_WIN32) && (defined(_MSC_VER) || defined(__MINGW32__))) || (defined(__WIN32__) && defined(__BORLANDC__))
