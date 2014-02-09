--- src/eat.h.orig	2013-11-02 11:55:11.356357725 +0000
+++ src/eat.h	2013-11-02 11:55:46.907173799 +0000
@@ -31,7 +31,7 @@
 
 static inline void _XEatDataWords(Display *dpy, unsigned long n)
 {
-# ifndef LONG64
+# if !defined(LONG64) && !defined(__riscos__)
     if (n >= (ULONG_MAX >> 2))
         _XIOError(dpy);
 # endif
