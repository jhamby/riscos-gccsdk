--- Source_Files/Misc/preferences.cpp.orig	2009-12-07 15:30:30.000000000 -0800
+++ Source_Files/Misc/preferences.cpp	2009-12-07 15:31:02.000000000 -0800
@@ -199,7 +199,7 @@
     // Hope caller's buffer is big enough.
     char* name = (char*) outName;
 
-#if defined(unix) || defined(__BEOS__) || (defined (__APPLE__) && defined (__MACH__)) || defined(__NetBSD__) || defined(__OpenBSD__)
+#if defined(unix) || defined(__BEOS__) || (defined (__APPLE__) && defined (__MACH__)) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__riscos__)
 
 	char *login = getlogin();
 	strcpy(name, login ? login : "Bob User");
