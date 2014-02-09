--- src/Xrandrint.h.orig	2013-11-02 12:00:17.037887015 +0000
+++ src/Xrandrint.h	2013-11-02 12:01:02.641019496 +0000
@@ -48,7 +48,7 @@
 
 static inline void _XEatDataWords(Display *dpy, unsigned long n)
 {
-# ifndef LONG64
+# if !defined(LONG64) && !defined(__riscos__)
     if (n >= (ULONG_MAX >> 2))
         _XIOError(dpy);
 # endif
