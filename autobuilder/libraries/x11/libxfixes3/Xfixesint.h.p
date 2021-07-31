--- src/Xfixesint.h.orig	2013-11-02 12:07:27.300194235 +0000
+++ src/Xfixesint.h	2013-05-29 05:11:24.000000000 +0100
@@ -66,7 +66,7 @@
 
 static inline void _XEatDataWords(Display *dpy, unsigned long n)
 {
-# ifndef LONG64
+# if !defined(LONG64) && !defined(__riscos__)
     if (n >= (ULONG_MAX >> 2))
         _XIOError(dpy);
 # endif
