--- src/lispbibl.d.orig	2008-06-16 14:28:19.000000000 -0700
+++ src/lispbibl.d	2009-08-27 20:45:15.000000000 -0700
@@ -96,7 +96,7 @@
    (and maybe gcc-cpp, ccpaux).
 
  this machine: WIN32 or GENERIC_UNIX */
-#if (defined(__unix) || defined(__unix__) || defined(_AIX) || defined(sinix) || defined(__MACH__) || defined(__POSIX__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__BEOS__)) && !defined(unix)
+#if (defined(__unix) || defined(__unix__) || defined(_AIX) || defined(sinix) || defined(__MACH__) || defined(__POSIX__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__BEOS__) || defined(__riscos__)) && !defined(unix)
   #define unix
 #endif
 #if (defined(_WIN32) && (defined(_MSC_VER) || defined(__MINGW32__))) || (defined(__WIN32__) && defined(__BORLANDC__))
