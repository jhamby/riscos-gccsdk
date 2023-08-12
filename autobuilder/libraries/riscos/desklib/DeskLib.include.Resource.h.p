--- !DeskLib/include/Resource.h.orig	2023-08-11 16:34:02.265329744 -0700
+++ !DeskLib/include/Resource.h	2023-08-11 16:48:55.071870119 -0700
@@ -48,7 +48,7 @@
 #define choices_WRITE       TRUE      /* Used with Resource_ChoicesPath() */
 
 
-extern char resource_pathname[32];
+extern char resource_pathname[60];
 /*
   This string is used as a prefix for all pathnames in DeskLib that
   load resource files (eg. Template_LoadFile).  It is set to either
