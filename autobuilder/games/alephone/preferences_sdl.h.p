--- Source_Files/Misc/preferences_sdl.h.orig	2005-06-09 23:26:11.000000000 +0100
+++ Source_Files/Misc/preferences_sdl.h	2008-09-04 11:26:53.744000000 +0100
@@ -106,7 +106,7 @@
     // Hope caller's buffer is big enough.
     char* name = (char*) outName;
 
-#if defined(unix) || defined(__BEOS__) || (defined (__APPLE__) && defined (__MACH__))
+#if defined(unix) || defined(__riscos__) || defined(__BEOS__) || (defined (__APPLE__) && defined (__MACH__))
 
 	char *login = getlogin();
 	strcpy(name, login ? login : "Bob User");
