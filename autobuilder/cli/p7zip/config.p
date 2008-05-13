--- CPP/myWindows/config.h.orig	2008-05-12 19:17:33.000000000 -0700
+++ CPP/myWindows/config.h	2008-05-12 19:17:44.000000000 -0700
@@ -5,7 +5,7 @@
   #define FILESYSTEM_IS_CASE_SENSITIVE 1
 #endif
 
-  #if !defined(ENV_MACOSX) && !defined(ENV_BEOS)
+#if !defined(ENV_MACOSX) && !defined(ENV_BEOS) && !defined(__riscos__)
 
     /* <wchar.h> */
     #define HAVE_WCHAR_H
