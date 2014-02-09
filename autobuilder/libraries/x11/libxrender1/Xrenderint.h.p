--- src/Xrenderint.h.orig	2013-10-31 16:26:38.746532774 +0000
+++ src/Xrenderint.h	2013-10-31 16:27:05.565928981 +0000
@@ -115,7 +115,7 @@
 
 static inline void _XEatDataWords(Display *dpy, unsigned long n)
 {
-# ifndef LONG64
+# if !defined(LONG64) && !defined(__riscos__)
     if (n >= (ULONG_MAX >> 2))
         _XIOError(dpy);
 # endif
