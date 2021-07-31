--- src/Xinerama.c.orig	2013-11-02 12:15:10.074445136 +0000
+++ src/Xinerama.c	2013-11-02 12:15:43.450006364 +0000
@@ -41,7 +41,7 @@
 
 static inline void _XEatDataWords(Display *dpy, unsigned long n)
 {
-# ifndef LONG64
+# if !defined(LONG64) && !defined(__riscos__)
     if (n >= (ULONG_MAX >> 2))
         _XIOError(dpy);
 # endif
