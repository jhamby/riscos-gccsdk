--- src/tool/copynpaste.cpp.orig	2010-02-20 11:48:49.000000000 +0000
+++ src/tool/copynpaste.cpp	2010-02-20 11:54:10.000000000 +0000
@@ -108,7 +108,7 @@
   return ret;
 }
 
-#elif USE_X11
+#elif USE_X11 && !defined(__riscos__)
 static char* getSelection(Display *dpy, Window us, Atom selection)
 {
   int    max_events = 50;
